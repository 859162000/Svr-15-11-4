/**
 * \file
 * \version  $Id: InfoClient.cpp  $
 * \author  
 * \date 
 * \brief �����������Ϣ�ռ��Ŀͻ�������
 */

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "InfoClient.h"
#include "FLServer.h"
#include "InfoClientManager.h"
#include "Command.h"
#include "InfoCommand.h"
#include "CollectServerInfo.h"

using namespace Cmd;
/**
 * \brief ���캯��
 * \param ip ��������ַ
 * \param port �������˿�
 */
InfoClient::InfoClient(
		const std::string &ip, 
		const unsigned short port) : zTCPClientTask(ip, port)
{
	Zebra::logger->trace("InfoClient::InfoClient");
}

/**
 * \brief ��������
 *
 */
InfoClient::~InfoClient()
{
	Zebra::logger->trace("InfoClient::~InfoClient");
}

int InfoClient::checkRebound()
{
	Zebra::logger->trace("InfoClient::checkRebound");
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
			using namespace Cmd::Info;

			t_LoginCmd *ptCmd = (t_LoginCmd *)pstrCmd;
			if (CMD_LOGIN == ptCmd->cmd
					&& PARA_LOGIN == ptCmd->para)
			{
				Zebra::logger->debug("��½�������ɹ�");
				return 1;
			}
			else
			{
				Zebra::logger->error("��½������ʧ��");
				return -1;
			}
		}
	}
	else
		return retcode;
}

void InfoClient::addToContainer()
{
	Zebra::logger->trace("InfoClient::addToContainer");
	InfoClientManager::getInstance().add(this);
}

void InfoClient::removeFromContainer()
{
	Zebra::logger->trace("InfoClient::removeFromContainer");
	InfoClientManager::getInstance().remove(this);
}

bool InfoClient::connect()
{
	Zebra::logger->trace("InfoClient::connect");
	if (!zTCPClientTask::connect())
		return false;

	using namespace Cmd::Info;
	t_LoginCmd cmd;
	return sendCmd(&cmd, sizeof(cmd));
}

bool InfoClient::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("InfoClient::msgParse");
	using namespace Cmd::Info;

	switch(ptNullCmd->cmd)
	{
		case CMD_INFO:
			{
				switch(ptNullCmd->para)
				{
					/*
					case PARA_REQUEST_SERVERINFO:
						{
							t_Request_ServerInfo *ptCmd = (t_Request_ServerInfo *)ptNullCmd;
							BYTE buff[zSocket::MAX_DATASIZE];
							t_ServerInfo *cmd = (t_ServerInfo *)buff;
							constructInPlace(cmd);
							cmd->rTimestamp = ptCmd->rTimestamp;
							cmd->ConnNum = FLService::getInstance().getPoolSize();
							cmd->ServerID = 0;
							cmd->ServerType = FLService::getInstance().getType();
							cmd->GameZone.id = 0;
							bzero(cmd->ZoneName, MAX_NAMESIZE);

							std::string xmlStr;
							ServerInfoSingleton::instance().getServerInfo(xmlStr);
							if (xmlStr.length() < 4096)
								strncpy(cmd->xml, xmlStr.c_str(), 4095);
							else
								Zebra::logger->warn("���ɷ�������Ϣxml����");

							return sendCmd(cmd, sizeof(t_ServerInfo) + strlen(cmd->xml));
						}
						break;*/
					case PARA_REQUEST_ONLINENUM:
						{
							t_Request_OnlineNum *ptCmd = (t_Request_OnlineNum *)ptNullCmd;
							t_OnlineNum cmd;
							cmd.rTimestamp = ptCmd->rTimestamp;
							cmd.OnlineNum = 0;
							cmd.ServerID = 0;
							cmd.ServerType = FLService::getInstance().getType();
							cmd.GameZone.id = 0;
							bzero(cmd.ZoneName, MAX_NAMESIZE);
							return sendCmd(&cmd, sizeof(cmd));
						}
						break;
				}
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

