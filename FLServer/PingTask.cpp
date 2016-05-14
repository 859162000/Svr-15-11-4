/**
 * \file
 * \version  $Id: PingTask.cpp  $
 * \author  
 * \date 
 * \brief ����PING��������
 *
 */

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <ext/hash_map>

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "PingTask.h"
#include "LoginManager.h"
#include "Command.h"
#include "zDBConnPool.h"
#include "FLServer.h"
#include "GYListManager.h"
#include "zMisc.h"
#include "FLCommand.h"
#include "ServerManager.h"
#include "DBAccessCommand.h"
#include "DBAccessClient.h"
#include "DBAccessClientManager.h"
#include "zJpeg.h"

int PingTask::verifyConn()
{
	Zebra::logger->trace("PingTask::verifyConn");
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//����ֻ�Ǵӻ���ȡ���ݰ������Բ������û������ֱ�ӷ���
			return 0;
		else
		{
			using namespace Cmd;

			stLoginPing *ptCmd = (stLoginPing *)pstrCmd;
			
			Zebra::logger->debug("PingTask::verifyConn(%u, %u)", ptCmd->byCmd, ptCmd->byParam);
			if (PING_USERCMD == ptCmd->byCmd
					&& LOGIN_PING_PARA == ptCmd->byParam)	
			{
				Zebra::logger->debug("�ͻ�������ͨ����֤");
				return 1;
			}
			else
			{
				Zebra::logger->error("�ͻ�������ָ����֤ʧ��");
				return -1;
			}
		}	
	}
	else
		return retcode;
}

int PingTask::recycleConn()
{
	Zebra::logger->trace("PingTask::recycleConn");
	return 1;
	/*
	mutex.lock();
	while(!cmd_queue.empty())
	{
		zSocket::t_BufferCmd *ptCmd = cmd_queue.front();
		int retcode = pSocket->sendRawData_NoPoll(&ptCmd->pstrCmd[ptCmd->offset], ptCmd->nCmdLen - ptCmd->offset);
		if (retcode > 0)
		{
			ptCmd->offset += retcode;
			if (ptCmd->offset < ptCmd->nCmdLen)
				//�������û�з�����ɲ��ܷ�����һ������break
				break;
			else if (ptCmd->offset == ptCmd->nCmdLen)
			{
				//������巢�������continue
				cmd_queue.pop();
				SAFE_DELETE(ptCmd);
			}
#if 0
			else if (ptCmd->offset > ptCmd->nCmdLen)
				//���ش��󣬲����ܳ����������
				assert(0);
#endif
		}
		else if (0 == retcode)
			// should retry
			break;
		else if (-1 == retcode)
		{
			Zebra::logger->error("%s", __PRETTY_FUNCTION__);
			break;
		}
	}
	mutex.unlock();
	return 1;
	*/
}

bool PingTask::msgParse(const Cmd::t_NullCmd *ptNull, const unsigned int nCmdLen)
{
	Zebra::logger->trace("PingTask::msgParse");
	using namespace Cmd;
	const stNullUserCmd *ptNullCmd = (const stNullUserCmd *)ptNull;
	
	if (PING_USERCMD == ptNullCmd->byCmd)
	{
		switch(ptNullCmd->byParam)
		{
			case REQUEST_PING_LIST_PARA:
				{

					stRequestPingList* pingCmd = (stRequestPingList*)ptNullCmd;
					GameZone_t gameZone;
					gameZone.id = pingCmd->id;
					//BYTE buf[zSocket::MAX_DATASIZE];
					
					//stPingList *retCmd=(stPingList *)buf;
					//constructInPlace(retCmd);
					
					stPingList retCmd;
					
					GYListManager::getInstance().full_ping_list(&retCmd, gameZone);
//					sendCmd(retCmd, (retCmd->size*sizeof(Cmd::ping_element)+sizeof(Cmd::stRequestPingList)));
					Zebra::logger->debug("zone = %u, ip = %s", retCmd.zone_id, retCmd.ping_list.gateway_ip);
					sendCmd(&retCmd, sizeof(retCmd));
					
					return true;
				}
				break;
		}
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNull->cmd, ptNull->para, nCmdLen);
	return false;
}

