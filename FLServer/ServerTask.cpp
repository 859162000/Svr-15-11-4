/**
 * \file
 * \version  $Id: ServerTask.cpp  $
 * \author  
 * \date 
 * \brief ʵ�ַ�����������
 *
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
#include "ServerTask.h"
#include "ServerManager.h"
#include "Zebra.h"
#include "zDBConnPool.h"
#include "zString.h"
#include "FLServer.h"
#include "FLCommand.h"
#include "ServerACL.h"
#include "GYListManager.h"
#include "Command.h"
#include "LoginManager.h"

/**
 * \brief �ȴ�������ָ֤�������֤
 *
 * ʵ���麯��<code>zTCPTask::verifyConn</code>
 *
 * \return ��֤�Ƿ�ɹ������߳�ʱ
 */
int ServerTask::verifyConn()
{
	Zebra::logger->trace("ServerTask::verifyConn");
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
			using namespace Cmd::FL;

			t_LoginFL *ptCmd = (t_LoginFL *)pstrCmd;
			if (CMD_LOGIN == ptCmd->cmd
					&& PARA_LOGIN == ptCmd->para
					)
			{
				bool mcheck = ServerACLSingleton::instance().check(getIP(), ptCmd->port, gameZone, name);
				if (mcheck)
				{
					Zebra::logger->debug("�ͻ�������ͨ����֤(%s:%u)",getIP(),ptCmd->port);
					return 1;
				}else{
					Zebra::logger->error("�ͻ�������ָ����֤ʧ��(%s:%u)ChcekFailer",getIP(),ptCmd->port);
                                	return -1;
				}
			}
			else
			{
				Zebra::logger->error("�ͻ�������ָ����֤ʧ��(%s:%u)",getIP(),ptCmd->port);
				return -1;
			}
		}
	}
	else
		return retcode;
}

int ServerTask::waitSync()
{
	Zebra::logger->trace("ServerTask::waitSync");
	using namespace Cmd::FL;
	t_LoginFL_OK cmd;
	cmd.gameZone = gameZone;
	bzero(cmd.name, sizeof(cmd.name));
	strncpy(cmd.name, name.c_str(), sizeof(cmd.name) - 1);
	t_RQGYList_FL tRQ;
	if (sendCmd(&cmd, sizeof(cmd))
			&& sendCmd(&tRQ, sizeof(tRQ)))
		return 1;
	else
		return -1;
}

/**
 * \brief ��ӵ�Ψһ����֤������
 * ʵ�����麯��<code>zTCPTask::uniqueAdd</code>
 */
bool ServerTask::uniqueAdd()
{
	Zebra::logger->trace("ServerTask::uniqueAdd");
	return ServerManager::getInstance().uniqueAdd(this);
}

/**
 * \brief ��Ψһ����֤������ɾ��
 * ʵ�����麯��<code>zTCPTask::uniqueRemove</code>
 */
bool ServerTask::uniqueRemove()
{
	Zebra::logger->trace("ServerTask::uniqueRemove");
	GYListManager::getInstance().disableAll(gameZone);
	return ServerManager::getInstance().uniqueRemove(this);
}

/**
 * \brief �������Ը������������ӵ�ָ��
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool ServerTask::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("ServerTask::msgParse");
	using namespace Cmd::FL;

	switch(ptNullCmd->cmd)
	{
		case CMD_GYLIST:
			if (msgParse_gyList(ptNullCmd, nCmdLen))
				return true;
			break;
		case CMD_SESSION:
			if (msgParse_session(ptNullCmd, nCmdLen))
				return true;
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool ServerTask::msgParse_gyList(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("ServerTask::msgParse_gyList");
	using namespace Cmd::FL;

	switch(ptNullCmd->para)
	{
		case PARA_FL_GYLIST:
			{
				t_GYList_FL *ptCmd = (t_GYList_FL *)ptNullCmd;
				GYList gy;

				Zebra::logger->debug("GYList:zoneid=%u(gameid=%u, zone=%u), serverid=%u, ip=%s, port=%u, onlines=%u, state=%u,version=%u", gameZone.id, gameZone.game, gameZone.zone, ptCmd->wdServerID, ptCmd->pstrIP, ptCmd->wdPort, ptCmd->wdNumOnline, ptCmd->state, ptCmd->zoneGameVersion);
				gy.wdServerID = ptCmd->wdServerID;
				bcopy(ptCmd->pstrIP, gy.pstrIP, sizeof(gy.pstrIP));
				gy.wdPort = ptCmd->wdPort;
				gy.wdNumOnline = ptCmd->wdNumOnline;
				gy.state = ptCmd->state;
				gy.zoneGameVersion = ptCmd->zoneGameVersion;

				return GYListManager::getInstance().put(gameZone, gy);
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool ServerTask::msgParse_session(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("ServerTask::msgParse_session");
	using namespace Cmd::FL;

	switch(ptNullCmd->para)
	{
		case PARA_SESSION_NEWSESSION:
			{
				t_NewSession_Session *ptCmd = (t_NewSession_Session *)ptNullCmd;
				using namespace Cmd;
				stServerReturnLoginSuccessCmd tCmd;

				Zebra::logger->debug("��½�ɹ�:%u, %u, %s, %u", ptCmd->session.accid, ptCmd->session.loginTempID, ptCmd->session.pstrIP, ptCmd->session.wdPort);
				tCmd.dwUserID = ptCmd->session.accid;
				tCmd.loginTempID = ptCmd->session.loginTempID;
				bcopy(ptCmd->session.pstrIP, tCmd.pstrIP, sizeof(tCmd.pstrIP));
				tCmd.wdPort = ptCmd->session.wdPort;
#ifdef _ENCDEC_MSG
				bzero(tCmd.key, sizeof(tCmd.key));
				//��Կ������һ��������
				for (int i=0; i<256; i++)
					tCmd.key[i] = zMisc::randBetween(0,255);
				
				do
					tCmd.key[58] = zMisc::randBetween(0,248);
				while((tCmd.key[58]>49)&&((tCmd.key[58]<59)));

				bcopy(ptCmd->session.des_key, &tCmd.key[tCmd.key[58]], sizeof(DES_cblock));
#endif

				return LoginManager::getInstance().broadcast(ptCmd->session.loginTempID, &tCmd, sizeof(tCmd));
			}
			break;
		case PARA_SESSION_IDINUSE:
			{
				t_idinuse_Session *ptCmd = (t_idinuse_Session *)ptNullCmd;
				using namespace Cmd;

				Zebra::logger->debug("�˺�����ʹ���� accid = %u", ptCmd->accid);
				LoginManager::getInstance().loginReturn(ptCmd->loginTempID, LOGIN_RETURN_IDINUSE);

				return true;
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

