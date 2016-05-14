/**
 * \file
 * \version  $Id: FLClient.cpp  $
 * \author  
 * \date 
 * \brief �����½�������ͻ���
 *
 * 
 */

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "FLClient.h"
#include "SuperServer.h"
#include "FLCommand.h"
#include "SuperCommand.h"
#include "ServerManager.h"
#include "FLClientManager.h"

/**
 * \brief ��ʱ��ŷ�����
 *
 */
WORD FLClient::tempidAllocator = 0;

/**
 * \brief ���캯��
 * \param ip ��������ַ
 * \param port �������˿�
 */
FLClient::FLClient(
		const std::string &ip, 
		const unsigned short port) : zTCPClientTask(ip, port, true), tempid(++tempidAllocator), netType(NetType_near)
{
	Zebra::logger->trace("FLClient::FLClient(%s:%u)",ip.c_str(),port);
}

/**
 * \brief ��������
 *
 */
FLClient::~FLClient()
{
	Zebra::logger->trace("FLClient::~FLClient");
}

int FLClient::checkRebound()
{
	Zebra::logger->trace("FLClient::checkRebound");
	int retcode = pSocket->recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = pSocket->recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//����ֻ�Ǵӻ���ȡ���ݰ������Բ������û������ֱ�ӷ���
			return 0;
		else
		{
			using namespace Cmd::FL;

			t_LoginFL_OK *ptCmd = (t_LoginFL_OK *)pstrCmd;
			if (CMD_LOGIN == ptCmd->cmd
					&& PARA_LOGIN_OK == ptCmd->para)
			{
				Zebra::logger->debug("��½FLServer�ɹ����յ����ı�ţ�zoneid=%u(gameid=%u, zone=%u), name=%s, nettype=%u",
						ptCmd->gameZone.id,
						ptCmd->gameZone.game,
						ptCmd->gameZone.zone,
						ptCmd->name,
						ptCmd->netType);
				netType = (ptCmd->netType == 0 ? NetType_near : NetType_far);
				SuperService::getInstance().setZoneID(ptCmd->gameZone);
				SuperService::getInstance().setZoneName(ptCmd->name);
				return 1;
			}
			else
			{
				Zebra::logger->error("��½FLServerʧ��");
				return -1;
			}
		}
	}
	else
		return retcode;
}

void FLClient::addToContainer()
{
	Zebra::logger->trace("FLClient::addToContainer");
	FLClientManager::getInstance().add(this);
}

void FLClient::removeFromContainer()
{
	Zebra::logger->trace("FLClient::removeFromContainer");
	FLClientManager::getInstance().remove(this);
}

bool FLClient::connect()
{
	Zebra::logger->trace("FLClient::connect");
	if (!zTCPClientTask::connect())
		return false;

	using namespace Cmd::FL;
	t_LoginFL cmd;
	strcpy(cmd.strIP, SuperService::getInstance().getIP());
	cmd.port = SuperService::getInstance().getPort();
	return sendCmd(&cmd, sizeof(cmd));
}

bool FLClient::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("FLClient::msgParse");
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

bool FLClient::msgParse_gyList(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("FLClient::msgParse_gyList");
	using namespace Cmd::FL;

	switch(ptNullCmd->para)
	{
		case PARA_FL_RQGYLIST:
			{
				//t_RQGYList_FL *ptCmd = (t_RQGYList_FL *)ptNullCmd;
				Cmd::Super::t_RQGYList_Gateway tCmd;

				return ServerManager::getInstance().broadcastByType(GATEWAYSERVER, &tCmd, sizeof(tCmd));
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool FLClient::msgParse_session(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("FLClient::msgParse_session");
	using namespace Cmd::FL;

	switch(ptNullCmd->para)
	{
		case PARA_SESSION_NEWSESSION:
			{
				t_NewSession_Session *ptCmd = (t_NewSession_Session *)ptNullCmd;
				Cmd::Super::t_NewSession_Bill tCmd;

				//Zebra::logger->debug("%s: %u, %u, %s, %u", __FUNCTION__, ptCmd->session.accid, ptCmd->session.loginTempID, ptCmd->session.pstrIP, ptCmd->session.wdPort);
				//Zebra::logger->debug("BillService: %u %u %u %u %u %u %u %u", ptCmd->session.des_key[0], ptCmd->session.des_key[1], ptCmd->session.des_key[2], ptCmd->session.des_key[3], ptCmd->session.des_key[4], ptCmd->session.des_key[5], ptCmd->session.des_key[6], ptCmd->session.des_key[7]);
				tCmd.session = ptCmd->session;
				tCmd.session.wdLoginID = tempid;
				//bcopy(&ptCmd->session, &tCmd.session, sizeof(tCmd.session));

				return ServerManager::getInstance().broadcastByType(BILLSERVER, &tCmd, sizeof(tCmd));
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

