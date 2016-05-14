/**
 * \file
 * \version  $Id: zTCPClientTask.cpp $
 * \author  
 * \date 
 * \brief ʵ����zTCPClientTask��TCP���ӿͻ��ˡ�
 *
 * 
 */


#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

#include "zTCPClientTask.h"
#include "Zebra.h"

/**
 * \brief ����һ������������TCP����
 *
 *
 * \return �����Ƿ�ɹ�
 */
bool zTCPClientTask::connect()
{
	//Zebra::logger->trace("zTCPClientTask::connect");
	int retcode;
	int nSocket;
	struct sockaddr_in addr;

	nSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == nSocket)
	{
		Zebra::logger->error("�����׽ӿ�ʧ��: %s", strerror(errno));
		return false;
	}

	//�����׽ӿڷ��ͽ��ջ��壬���ҿͻ��˵ı�����connect֮ǰ����
	socklen_t window_size = 128 * 1024;
	retcode = ::setsockopt(nSocket, SOL_SOCKET, SO_RCVBUF, &window_size, sizeof(window_size));
	if (-1 == retcode)
	{
		TEMP_FAILURE_RETRY(::close(nSocket));
		return false;
	}
	retcode = ::setsockopt(nSocket, SOL_SOCKET, SO_SNDBUF, &window_size, sizeof(window_size));
	if (-1 == retcode)
	{
		TEMP_FAILURE_RETRY(::close(nSocket));
		return false;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);

	retcode = TEMP_FAILURE_RETRY(::connect(nSocket, (struct sockaddr *) &addr, sizeof(addr)));
	if (-1 == retcode)
	{
		Zebra::logger->error("������������ %s(%u) ������ʧ��", ip.c_str(), port);
		TEMP_FAILURE_RETRY(::close(nSocket));
		return false;
	}

	pSocket = new zSocket(nSocket, &addr, compress);
	if (NULL == pSocket)
	{
		Zebra::logger->fatal("û���㹻���ڴ棬���ܴ���zSocketʵ��");
		TEMP_FAILURE_RETRY(::close(nSocket));
		return false;
	}

	Zebra::logger->info("������������ %s:%u �����ӳɹ�", ip.c_str(), port);

	return true;
}

void zTCPClientTask::checkConn()
{
	//Zebra::logger->trace("zTCPClientTask::checkConn");
	zRTime currentTime;
	if (_ten_min(currentTime))
	{
		Cmd::t_NullCmd tNullCmd;		
		sendCmd(&tNullCmd, sizeof(tNullCmd));
	}
}

/**
 * \brief ���׽ӿڷ���ָ��
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��Ĵ�С
 * \return �����Ƿ�ɹ�
 */
bool zTCPClientTask::sendCmd(const void *pstrCmd, const int nCmdLen)
{
	//Zebra::logger->trace("zTCPClientTask::sendCmd");
	switch(state)
	{
		case close:
		case sync:
			if (NULL == pSocket) 
				return false;
			else
				return pSocket->sendCmd(pstrCmd, nCmdLen);
			break;
		case okay:
		case recycle:
			if (NULL == pSocket)
				return false;
			else
				return pSocket->sendCmd(pstrCmd, nCmdLen, true);
			break;
	}

	return false;
}

/**
 * \brief ���׽ӿ��н������ݣ����Ҳ�����д����ڵ����������֮ǰ��֤�Ѿ����׽ӿڽ�������ѯ
 *
 * \param needRecv �Ƿ���Ҫ�������׽ӿڽ������ݣ�false����Ҫ���գ�ֻ�Ǵ�������ʣ���ָ�true��Ҫʵ�ʽ������ݣ�Ȼ��Ŵ���
 * \return �����Ƿ�ɹ���true��ʾ���ճɹ���false��ʾ����ʧ�ܣ�������Ҫ�Ͽ����� 
 */
bool zTCPClientTask::ListeningRecv(bool needRecv)
{
	//Zebra::logger->trace("zTCPClientTask::ListeningRecv");
	int retcode = 0;
	if (needRecv) {
		retcode = pSocket->recvToBuf_NoPoll();
	}
	if (-1 == retcode)
	{
		Zebra::logger->error("%s", __PRETTY_FUNCTION__);
		return false;
	}
	else
	{
		do
		{
			unsigned char pstrCmd[zSocket::MAX_DATASIZE];
			int nCmdLen = pSocket->recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
			if (nCmdLen <= 0)
				//����ֻ�Ǵӻ���ȡ���ݰ������Բ������û������ֱ�ӷ���
				break;
			else
			{
				Cmd::t_NullCmd *ptNullCmd = (Cmd::t_NullCmd *)pstrCmd;
				if (Cmd::CMD_NULL == ptNullCmd->cmd
						&& Cmd::PARA_NULL == ptNullCmd->para)
				{
					//Zebra::logger->debug("�ͻ����յ������ź�");
					if (!sendCmd(pstrCmd, nCmdLen))
					{
						//����ָ��ʧ�ܣ��˳�ѭ���������߳�
						return false;
					}
				}
				else
					msgParse(ptNullCmd, nCmdLen);
			}
		}
		while(true);
	}
	return true;
}

/**
 * \brief ���ͻ����е����ݵ��׽ӿڣ��ٵ������֮ǰ��֤�Ѿ����׽ӿڽ�������ѯ
 *
 * \return �����Ƿ�ɹ���true��ʾ���ͳɹ���false��ʾ����ʧ�ܣ�������Ҫ�Ͽ�����
 */
bool zTCPClientTask::ListeningSend()
{
	//Zebra::logger->trace("zTCPClientTask::ListeningSend");
	if (pSocket)
		return pSocket->sync();
	else
		return false;
}

/**
 * \brief ��TCP�������񽻸���һ��������У��л�״̬
 *
 */
void zTCPClientTask::getNextState()
{
	//Zebra::logger->trace("zTCPClientTask::getNextState");
	ConnState old_state = getState();

	lifeTime.now();
	switch(old_state)
	{
		case close:
			setState(sync);
			break;
		case sync:
			addToContainer();
			setState(okay);
			break;
		case okay:
			removeFromContainer();
			setState(recycle);
			break;
		case recycle:
			if (terminate == TM_service_close)
				recycleConn();
			setState(close);
			final();
			break;
	}

	Zebra::logger->debug("%s(%s, %u, %s -> %s)", __FUNCTION__,  ip.c_str(), port, getStateString(old_state), getStateString(getState()));
}

/**
 * \brief ��ֵ��������״̬����������
 *
 */
void zTCPClientTask::resetState()
{
	//Zebra::logger->trace("zTCPClientTask::resetState");
	ConnState old_state = getState();

	lifeTime.now();
	setState(close);
	final();

	Zebra::logger->debug("%s(%s, %u, %s -> %s)", __FUNCTION__,  ip.c_str(), port, getStateString(old_state), getStateString(getState()));
}

