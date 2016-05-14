/**
 * \file
 * \version  $Id: zTCPClient.cpp  $
 * \author  
 * \date 
 * \brief ʵ����zTCPClient��TCP���ӿͻ��ˡ�
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

#include "zTCPClient.h"
#include "Zebra.h"

CmdAnalysis zTCPClient::analysis("Clientָ���ͳ��",600);
/**
 * \brief ����һ������������TCP����
 *
 *
 * \return �����Ƿ�ɹ�
 */
bool zTCPClient::connect()
{
	Zebra::logger->trace("zTCPClient::connect");
	int retcode;
	int nSocket;
	struct sockaddr_in addr;

	nSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == nSocket)
	{
		Zebra::logger->error("�����׽ӿ�ʧ��");
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
		Zebra::logger->error("������������(%s:%u) ������ʧ��", ip.c_str(), port);
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

	Zebra::logger->info("������������(%s:%u)�����ӳɹ�", ip.c_str(), port);

	return true;
}

/**
 * \brief ���׽ӿڷ���ָ��
 *
 *
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��Ĵ�С
 * \return �����Ƿ�ɹ�
 */
bool zTCPClient::sendCmd(const void *pstrCmd, const int nCmdLen)
{
	Zebra::logger->trace("zTCPClient::sendCmd");
	if (NULL == pSocket) 
		return false;
	else
	{
		/*
		Cmd::t_NullCmd *ptNullCmd = (Cmd::t_NullCmd *)pstrCmd;
		analysis.add(ptNullCmd->cmd,ptNullCmd->para,nCmdLen);
		// */
		return pSocket->sendCmd(pstrCmd, nCmdLen);
	}
}
/**
 * \brief ����zThread�еĴ��麯�������̵߳����ص����������ڴ�����յ���ָ��
 *
 */
void zTCPClient::run()
{
	Zebra::logger->trace("zTCPClient::run");
	while(!isFinal())
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen;

		nCmdLen = pSocket->recvToCmd(pstrCmd, zSocket::MAX_DATASIZE, false);
		if (nCmdLen > 0) 
		{
			Cmd::t_NullCmd *ptNullCmd = (Cmd::t_NullCmd *)pstrCmd;
			if (Cmd::CMD_NULL == ptNullCmd->cmd
					&& Cmd::PARA_NULL == ptNullCmd->para)
			{
				//Zebra::logger->debug("�ͻ����յ������ź�");
				if (!sendCmd(pstrCmd, nCmdLen))
				{
					//����ָ��ʧ�ܣ��˳�ѭ���������߳�
					break;
				}
			}
			else
				msgParse(ptNullCmd, nCmdLen);
		}
		else if (-1 == nCmdLen)
		{
			//����ָ��ʧ�ܣ��˳�ѭ���������߳�
			Zebra::logger->error("����ָ��ʧ�ܣ��ر� %s", getThreadName().c_str());
			break;
		}
	}
}

bool zTCPBufferClient::sendCmd(const void *pstrCmd, const int nCmdLen)
{
	Zebra::logger->trace("zTCPBufferClient::sendCmd");
	if (pSocket)
		return pSocket->sendCmd(pstrCmd, nCmdLen, _buffered);
	else
		return false;
}

bool zTCPBufferClient::ListeningRecv()
{
	Zebra::logger->trace("zTCPBufferClient::ListeningRecv");
	int retcode = pSocket->recvToBuf_NoPoll();
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

bool zTCPBufferClient::ListeningSend()
{	
	Zebra::logger->trace("zTCPBufferClient::ListeningSend");
	if (pSocket)
		return pSocket->sync();
	else
		return false;
}

void zTCPBufferClient::sync()
{
	Zebra::logger->trace("zTCPBufferClient::sync");
	if (pSocket)
		pSocket->force_sync();
}

void zTCPBufferClient::run()
{	
#if _USE_EPOLL_
	Zebra::logger->trace("zTCPBufferClient::run(EPOLL)");
	while(!isFinal())
        {
                unsigned char pstrCmd[zSocket::MAX_DATASIZE];
                int nCmdLen;

                nCmdLen = pSocket->recvToCmd(pstrCmd, zSocket::MAX_DATASIZE, false);
                if (nCmdLen > 0)
                {
                        Cmd::t_NullCmd *ptNullCmd = (Cmd::t_NullCmd *)pstrCmd;
                        if (Cmd::CMD_NULL == ptNullCmd->cmd
                                        && Cmd::PARA_NULL == ptNullCmd->para)
                        {
                                //Zebra::logger->debug("�ͻ����յ������ź�");
                                if (!sendCmd(pstrCmd, nCmdLen))
                                {
                                        //����ָ��ʧ�ܣ��˳�ѭ���������߳�
                                        break;
                                }
                        }
                        else
                                msgParse(ptNullCmd, nCmdLen);
                }
                else if (-1 == nCmdLen)
                {
                        //����ָ��ʧ�ܣ��˳�ѭ���������߳�
                        Zebra::logger->error("����ָ��ʧ�ܣ��ر� %s", getThreadName().c_str());
                        break;
                }
        }


#else
	Zebra::logger->trace("zTCPBufferClient::run(POLL)");
	_buffered = true;
	struct pollfd pfds;
	struct pollfd pfds_r;
	pSocket->fillPollFD(pfds, POLLIN | POLLOUT | POLLERR | POLLPRI);
	pSocket->fillPollFD(pfds_r, POLLIN | POLLERR | POLLPRI);
	int time=usleep_time;
	while(!isFinal())
	{
		struct timeval _tv_1;
		struct timeval _tv_2;
		gettimeofday(&_tv_1,NULL);
		if (TEMP_FAILURE_RETRY(::poll(&pfds_r, 1, time/1000)) > 0)
		{
			if (pfds_r.revents & (POLLERR | POLLPRI))
			{
				//�׽ӿڳ��ִ���
				Zebra::logger->fatal("%s: �׽ӿڴ���", __PRETTY_FUNCTION__);
				break;
			}
			else
			{
				if (pfds_r.revents & POLLIN)
				{
					//�׽ӿ�׼�����˶�ȡ����
					if (!ListeningRecv())
					{
						Zebra::logger->debug("%s: �׽ӿڶ���������", __PRETTY_FUNCTION__);
						break;
					}
				}
			}
		}
		gettimeofday(&_tv_2,NULL);
		int end=_tv_2.tv_sec*1000000 + _tv_2.tv_usec;
		int begin= _tv_1.tv_sec*1000000 + _tv_1.tv_usec;
		time = time - (end - begin);
		if(time <= 0)
		{
			if (TEMP_FAILURE_RETRY(::poll(&pfds, 1, 0)) > 0)
			{
				if (pfds.revents & (POLLERR | POLLPRI))
				{
					//�׽ӿڳ��ִ���
					Zebra::logger->fatal("%s: �׽ӿڴ���", __PRETTY_FUNCTION__);
					break;
				}
				else
				{
					if (pfds.revents & POLLIN)
					{
						//�׽ӿ�׼�����˶�ȡ����
						if (!ListeningRecv())
						{
							Zebra::logger->debug("%s: �׽ӿڶ���������", __PRETTY_FUNCTION__);
							break;
						}
					}
					if (pfds.revents & POLLOUT)
					{
						//�׽ӿ�׼������д�����
						if (!ListeningSend())
						{
							Zebra::logger->debug("%s: �׽ӿ�д��������", __PRETTY_FUNCTION__);
							break;
						}
					}
				}
			}
			time = usleep_time;
		}
		//zThread::usleep(usleep_time);
	}

	//��֤��������ݷ������
	sync();
	_buffered = false;
#endif
}

