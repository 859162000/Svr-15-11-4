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
#include <stdio.h>

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "ServerTask.h"
#include "SuperCommand.h"
#include "ServerManager.h"
#include "Zebra.h"
#include "zDBConnPool.h"
#include "SuperServer.h"
#include "zString.h"
#include "SuperServer.h"
#include "FLCommand.h"
#include "FLClient.h"
#include "FLClientManager.h"
#include "RoleregCommand.h"
#include "RoleregClient.h"
#include "RoleregClientManager.h"
#include "RoleregCache.h"
#include "InfoCommand.h"
#include "GmToolCommand.h"
#include "InfoClient.h"
#include "InfoClientManager.h"

/**
 * \brief ��֤һ�������������Ƿ�Ϸ�
 *
 * ÿһ̨����������Ϣ�����ݿ��ж��м�¼��������ݿ���û����Ӧ�ļ�¼����ô�������������������ǲ��Ϸ��ģ���Ҫ�����Ͽ�����<br>
 * ������֤��һ�����еķ�����֮������ι�ϵ
 *
 * \param wdType ����������
 * \param pstrIP ��������ַ
 * \return ��֤�Ƿ�ɹ�
 */
bool ServerTask::verify(WORD wdType, const char *pstrIP)
{
	Zebra::logger->trace("ServerTask::verify(%s:%u)",pstrIP,wdType);
	char where[64];

	this->wdServerType = wdType;
	strncpy(this->pstrIP, pstrIP, sizeof(this->pstrIP) - 1);
	Zebra::logger->debug("%s \t%u, %s", __FUNCTION__, wdType, pstrIP);

	connHandleID handle = SuperService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܴ����ݿ����ӳػ�ȡ���Ӿ��");
		return false;
	}

	bzero(where, sizeof(where));
	std::string escapeIP;
	snprintf(where, sizeof(where) - 1, "`TYPE`=%u AND `IP`='%s'", wdType, SuperService::dbConnPool->escapeString(handle,pstrIP,escapeIP).c_str());
	static const dbCol col_define[] =
	{
		{"ID",zDBConnPool::DB_WORD,sizeof(WORD)},
		{"NAME",zDBConnPool::DB_STR,sizeof(char[MAX_NAMESIZE])},
		{"PORT",zDBConnPool::DB_WORD,sizeof(WORD)},
		{"EXTIP",zDBConnPool::DB_STR,sizeof(char[MAX_IP_LENGTH])},
		{"EXTPORT",zDBConnPool::DB_WORD,sizeof(WORD)},
		{NULL, 0, 0}
	};
	struct
	{
		WORD wdServerID;
		char pstrName[MAX_NAMESIZE];
		WORD wdPort;
		char pstrExtIP[MAX_IP_LENGTH];
		WORD wdExtPort;
	} __attribute__ ((packed))
	*pData = NULL;

	unsigned int retcode = SuperService::dbConnPool->exeSelect(handle, "`SERVERLIST`", col_define, where, "`ID`",(unsigned char **)&pData);
	if (retcode == (unsigned int)-1
			|| retcode == 0
			|| NULL == pData)
	{
		Zebra::logger->error("���ݿ���û����Ӧ�ķ�������¼ %u",wdType);
		SuperService::dbConnPool->putHandle(handle);
		return false;
	}

	SuperService::dbConnPool->putHandle(handle);

	//ĳЩ���ͷ�������һ������ֻ����һ̨
	if (retcode > 1
			&& (wdType == BILLSERVER
			|| wdType == SESSIONSERVER)
	)
	{
		SAFE_DELETE_VEC(pData);
		Zebra::logger->error("�������͵ķ�����ֻ����һ̨ %u", wdType);
		return false;
	}

	//�����ݿ���ȡ���ݳɹ�����Ҫ����Щ������ȡ��һ��������
	unsigned int i;
	for(i = 0; i < retcode; i++)
	{
		if (ServerManager::getInstance().uniqueVerify(pData[i].wdServerID))
		{
			wdServerID = pData[i].wdServerID;
			strncpy(pstrName, pData[i].pstrName, sizeof(pstrName) - 1);
			wdPort = pData[i].wdPort;
			strncpy(pstrExtIP, pData[i].pstrExtIP, sizeof(pstrExtIP) - 1);
			wdExtPort = pData[i].wdExtPort;
			break;
		}
	}
	SAFE_DELETE_VEC(pData);
	if (i == retcode)
	{
		Zebra::logger->error("�������Ѿ���������ˣ�û�п��ü�¼");
		return false;
	}

	//���ط�������Ϣ��������
	using namespace Cmd::Super;
	t_Startup_Response tCmd;
	tCmd.wdServerID = wdServerID;
	tCmd.wdPort = wdPort;
	bcopy(pstrExtIP, tCmd.pstrExtIP, sizeof(pstrExtIP));
	tCmd.wdExtPort = wdExtPort;
	if (!sendCmd(&tCmd, sizeof(tCmd)))
	{
		
		Zebra::logger->error("�����������ָ��ʧ��%u(%s:%u),wdServerID,pstrExtIP,wdExprot");
		return false;
	}

	return true;
}

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
			using namespace Cmd::Super;

			t_Startup_Request *ptCmd = (t_Startup_Request *)pstrCmd;
			if (CMD_STARTUP == ptCmd->cmd
					&& PARA_STARTUP_REQUEST == ptCmd->para)
			{
				if (verify(ptCmd->wdServerType, ptCmd->pstrIP))
				{
					Zebra::logger->debug("�ͻ�������ͨ����֤(%s:%u)",ptCmd->pstrIP,ptCmd->wdServerType);
					return 1;
				}
				else
				{
					Zebra::logger->error("�ͻ���������֤ʧ��(%s:%u)",ptCmd->pstrIP,ptCmd->wdServerType);
					return -1;
				}
			}
			else
			{
				Zebra::logger->error("�ͻ�������ָ����֤ʧ��(%s:%u)",ptCmd->pstrIP,ptCmd->wdServerType);
				return -1;
			}
		}
	}
	else
		return retcode;
}

/**
 * \brief ���������֮���������ϵ
 *
 */
static __gnu_cxx::hash_map<int, std::vector<int> > serverSequence;

/**
 * \brief ��ʼ��������֮���������ϵ
	
   gcc 
 *
 */
static void initServerSequence() __attribute__ ((constructor));
void initServerSequence()
{
	printf("initServerSequence\r\n");
	serverSequence[UNKNOWNSERVER]	=	std::vector<int>();
	serverSequence[SUPERSERVER]	=	std::vector<int>();
	serverSequence[LOGINSERVER]	=	std::vector<int>();
	serverSequence[RECORDSERVER]	=	std::vector<int>();
	serverSequence[MINISERVER]	=	std::vector<int>();

	int data0[] = { RECORDSERVER };
	serverSequence[SESSIONSERVER]	=	std::vector<int>(data0, data0 + sizeof(data0) / sizeof(int));
	int data1[] = { RECORDSERVER, SESSIONSERVER , MINISERVER};
	serverSequence[SCENESSERVER]	=	std::vector<int>(data1, data1 + sizeof(data1) / sizeof(int));
	int data2[] = { RECORDSERVER, BILLSERVER, SESSIONSERVER, SCENESSERVER ,MINISERVER};
	serverSequence[GATEWAYSERVER]	=	std::vector<int>(data2, data2 + sizeof(data2) / sizeof(int));
	
}

/**
 * \brief ��֤ĳ�����͵����з������Ƿ���ȫ�������
 *
 * \param wdType ����������
 * \param sv ���������������ɹ��ķ������б�
 * \return ��֤�Ƿ�ɹ�
 */
bool ServerTask::verifyTypeOK(const WORD wdType, std::vector<ServerTask *> &sv)
{
	Zebra::logger->trace("ServerTask::verifyTypeOK(wdType=%u)",wdType);
	static const dbCol col_define[] =
	{
		{"ID",zDBConnPool::DB_WORD,sizeof(WORD)},
		{NULL, 0, 0}
	};
	char where[64];
	WORD *ID = NULL;

	if (0 == wdType)
		return true;

	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "`TYPE`=%u", wdType);

	connHandleID handle = SuperService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܴ����ݿ����ӳػ�ȡ���Ӿ��");
		return false;
	}

	unsigned int retcode = SuperService::dbConnPool->exeSelect(handle, "`SERVERLIST`", col_define, where, "`ID`",(unsigned char **)&ID);
	if (retcode == (unsigned int)-1
			|| retcode == 0
			|| NULL == ID)
	{
		Zebra::logger->error("���ݿ���û����Ӧ�ķ�������¼ %u",wdType);
		SuperService::dbConnPool->putHandle(handle);
		return false;
	}
	SuperService::dbConnPool->putHandle(handle);

	bool retval = true;
	for(unsigned int i = 0; i < retcode; i++)
	{
		//�����������Ŀ϶���OK״̬
		ServerTask * pServer = ServerManager::getInstance().getServer(ID[i]);
		if (NULL == pServer)
		{
			retval = false;
			break;
		}
		else
		{
			sv.push_back(pServer);
		}
	}
	SAFE_DELETE_VEC(ID);

	return retval;
}

/**
 * \brief ֪ͨ���������ķ�����
 * \return ֪ͨ�Ƿ�ɹ�
 */
bool ServerTask::notifyOther(WORD dstID)
{
	Zebra::logger->trace("ServerTask::notifyOther(dstID =%u)",dstID);
	using namespace Cmd::Super;

	t_Startup_ServerEntry_NotifyOther Cmd;

	bzero(&Cmd.entry, sizeof(Cmd.entry));
	Cmd.entry.wdServerID = wdServerID;
	Cmd.entry.wdServerType = wdServerType;
	strncpy(Cmd.entry.pstrName, pstrName, MAX_NAMESIZE - 1);
	strncpy(Cmd.entry.pstrIP, pstrIP, MAX_IP_LENGTH - 1);
	Cmd.entry.wdPort = wdPort;
	strncpy(Cmd.entry.pstrExtIP, pstrExtIP, MAX_IP_LENGTH - 1);
	Cmd.entry.wdExtPort = wdExtPort;
	Cmd.entry.state = state;

	for(Container::iterator it = ses.begin(); it != ses.end(); ++it)
	{
		if(dstID == it->first.wdServerID)
		{
			ServerTask * pDst = ServerManager::getInstance().getServer(dstID);
			if(pDst)
			{
				pDst->sendCmd(&Cmd, sizeof(Cmd));
			}
			break;
		}
	}

	return true;
}
/**
 * \brief ֪ͨ���������ķ�����
 * \return ֪ͨ�Ƿ�ɹ�
 */
bool ServerTask::notifyOther()
{
	Zebra::logger->trace("ServerTask::notifyOther()");
	using namespace Cmd::Super;
	t_Startup_ServerEntry_NotifyOther Cmd;
	bool retval = true;

	bzero(&Cmd.entry, sizeof(Cmd.entry));
	Cmd.entry.wdServerID = wdServerID;
	Cmd.entry.wdServerType = wdServerType;
	strncpy(Cmd.entry.pstrName, pstrName, MAX_NAMESIZE - 1);
	strncpy(Cmd.entry.pstrIP, pstrIP, MAX_IP_LENGTH - 1);
	Cmd.entry.wdPort = wdPort;
	strncpy(Cmd.entry.pstrExtIP, pstrExtIP, MAX_IP_LENGTH - 1);
	Cmd.entry.wdExtPort = wdExtPort;
	Cmd.entry.state = state;

	for(Container::iterator it = ses.begin(); it != ses.end(); ++it)
	{
		Cmd.srcID = it->first.wdServerID;
		// ֪ͨ�����ķ�����
		bool curval = ServerManager::getInstance().broadcastByID(Cmd.srcID, &Cmd, sizeof(Cmd));
		Zebra::logger->trace("ServerTask::notifyOther()srcid=%u curval = %u",Cmd.srcID,curval);
		retval &= curval;
	}
	return retval;
}

/**
 * \brief �յ�notifyOther�ظ�
 * \param wdServerID Ŀ�ķ��������
 */
void ServerTask::responseOther(const WORD wdServerID)
{
	Zebra::logger->trace("ServerTask::responseOther(%u)",wdServerID);
	for(Container::iterator it = ses.begin(); it != ses.end(); ++it)
	{
		if (it->first.wdServerID == wdServerID)
		{
			Zebra::logger->debug("�ظ��ɹ� %s, %d", it->first.pstrName, it->first.wdServerID);
			it->second = true;
		}
	}
}

/**
 * \brief ֪ͨ�������������ķ�������Ϣ�б�
 * \return ֪ͨ�Ƿ�ɹ�
 */
bool ServerTask::notifyMe()
{	
	if (hasNotifyMe) return true;

	Zebra::logger->trace("ServerTask::notifyMe(serverid=%u servertype=%u)",wdServerID,wdServerType);

	using namespace Cmd::Super;

	unsigned char pBuffer[zSocket::MAX_DATASIZE];
	t_Startup_ServerEntry_NotifyMe *ptCmd = (t_Startup_ServerEntry_NotifyMe *)pBuffer;
	constructInPlace(ptCmd);
	ptCmd->size = 0;

	//check for notify other response
	for(Container::iterator it = ses.begin(); it != ses.end(); ++it)
	{
		if (it->second)
		{
			bzero(&ptCmd->entry[ptCmd->size], sizeof(ptCmd->entry[ptCmd->size]));
			ptCmd->entry[ptCmd->size].wdServerID = it->first.wdServerID;
			ptCmd->entry[ptCmd->size].wdServerType = it->first.wdServerType;
			strncpy(ptCmd->entry[ptCmd->size].pstrName, it->first.pstrName, MAX_NAMESIZE - 1);
			strncpy(ptCmd->entry[ptCmd->size].pstrIP, it->first.pstrIP, MAX_IP_LENGTH - 1);
			ptCmd->entry[ptCmd->size].wdPort = it->first.wdPort;
			strncpy(ptCmd->entry[ptCmd->size].pstrExtIP, it->first.pstrExtIP, MAX_IP_LENGTH - 1);
			ptCmd->entry[ptCmd->size].wdExtPort = it->first.wdExtPort;
			ptCmd->entry[ptCmd->size].state = it->first.state;

			ptCmd->size++;
		}
		else
			return false;
	}

	if (sendCmd(ptCmd, sizeof(t_Startup_ServerEntry_NotifyMe) + ptCmd->size * sizeof(ServerEntry)))
		hasNotifyMe = true;

	return hasNotifyMe;
}

/**
 * \brief �����������������ϵ��Ҳ��������˳��
 * \return �Ƿ������������ķ������Ѿ��������
 */
bool ServerTask::processSequence()
{
	if (hasprocessSequence) return true;
	
	Zebra::logger->trace("ServerTask::processSequence(wdServerType=%u)",wdServerType,wdServerType);
	using namespace Cmd::Super;

	ses.clear();
	
	std::vector<int> sequence = serverSequence[wdServerType];
	for(std::vector<int>::const_iterator it = sequence.begin(); it != sequence.end(); it++)
	{		
		std::vector<ServerTask *> sv;
		if (verifyTypeOK(*it, sv))
		{
			for(std::vector<ServerTask *>::const_iterator sv_it = sv.begin(); sv_it != sv.end(); sv_it++)
			{
				ServerEntry se;
				bzero(&se, sizeof(se));
				se.wdServerID = (*sv_it)->wdServerID;
				se.wdServerType = (*sv_it)->wdServerType;
				strncpy(se.pstrName, (*sv_it)->pstrName, MAX_NAMESIZE - 1);
				strncpy(se.pstrIP, (*sv_it)->pstrIP, MAX_IP_LENGTH - 1);
				se.wdPort = (*sv_it)->wdPort;
				strncpy(se.pstrExtIP, (*sv_it)->pstrExtIP, MAX_IP_LENGTH - 1);
				se.wdExtPort = (*sv_it)->wdExtPort;
				se.state = (*sv_it)->state;

				//���������ķ����������Լ����������б���,��Ӧ����Ϣ��ɷ�
				ses.insert(Container::value_type(se, false));
			}
		}
		else
		{			
			return false;
		}
	}
	hasprocessSequence = true;
	return true;
}

/**
 * \brief �ȴ��������������
 *
 * ���������ܵ����������������ص�������Ϣ�����������ĳ�ʼ������ʼ����Ϸ����������ָ�������������<br>
 * �����������������ܵ�ȷ��������ɵ�ָ��ͻ��������������뵽��������Ķ����У������������������������������<br>
 * ʵ�����麯��<code>zTCPTask::waitSync</code>
 *
 * \return �����������Ƿ�ͨ���������ʱ��Ҫ�����ȴ�
 */
int ServerTask::waitSync()
{
	Zebra::logger->trace("ServerTask::waitSync");
	int retcode = mSocket.checkIOForRead();
	if (-1 == retcode)
	{
		Zebra::logger->error("%s", __PRETTY_FUNCTION__);
		return -1;
	}
	else if (retcode > 0)
	{
		//�׽ӿ�׼�����˽������ݣ��������ݵ����壬�����Դ���ָ��
		retcode = mSocket.recvToBuf_NoPoll();
		if (-1 == retcode)
		{
			Zebra::logger->error("%s", __PRETTY_FUNCTION__);
			return -1;
		}
	}

	unsigned char pstrCmd[zSocket::MAX_DATASIZE];
	int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
	if (nCmdLen > 0)
	{
		using namespace Cmd::Super;

		t_Startup_OK *ptCmd = (t_Startup_OK *)pstrCmd;		
		if (CMD_STARTUP == ptCmd->cmd
				&& PARA_STARTUP_OK == ptCmd->para
				&& wdServerID == ptCmd->wdServerID)
		{
			Zebra::logger->debug("�ͻ�������ͬ����֤�ɹ�(%u, %u)", ptCmd->wdServerID, wdServerID);
			return 1;
		}
		else
		{
			Zebra::logger->error("�ͻ�������ͬ����֤ʧ��(%u, %u)", ptCmd->wdServerID, wdServerID);
			return -1;
		}
	}
	
	bool blntime = checkSequenceTime();
	bool blnseq = processSequence();
	bool blnother = notifyOther();

	if (!blntime) 
	{
		Zebra::logger->debug("��鴦������˳��checkSequenceTime()=false");
		blntime = true;
	}
	if (!blnseq)
	{
		Zebra::logger->debug("��鴦������˳��processSequence()=false");
		blnseq = true;
	}
	if (!blnother)
	{
		Zebra::logger->debug("��鴦������˳��notifyOther()=false");
		blnother = true;
	}
	//���ȼ�鴦������˳��
	if (blntime && blnseq	&& blnother)
		sequenceOK = true;	
	if (sequenceOK)
	{
		notifyMe();		
	}
	//�ȴ���ʱ
	return 0;
}

/**
 * \brief ȷ��һ�����������ӵ�״̬�ǿ��Ի��յ�
 *
 * ��һ������״̬�ǿ��Ի��յ�״̬����ô��ζ��������ӵ������������ڽ��������Դ��ڴ��а�ȫ��ɾ���ˣ���<br>
 * ʵ�����麯��<code>zTCPTask::recycleConn</code>
 *
 * \return �Ƿ���Ի���
 */
int ServerTask::recycleConn()
{
	return 1;
}

/**
 * \brief ��ӵ�ȫ��������
 *
 * ʵ�����麯��<code>zTCPTask::addToContainer</code>
 *
 */
void ServerTask::addToContainer()
{
	Zebra::logger->trace("ServerTask::addToContainer");
	ServerManager::getInstance().addServer(this);
}

/**
 * \brief ��ȫ��������ɾ��
 *
 * ʵ�����麯��<code>zTCPTask::removeToContainer</code>
 *
 */
void ServerTask::removeFromContainer()
{
	Zebra::logger->trace("ServerTask::removeFromContainer");
	//��������ط������رգ�����֪ͨ���еĵ�½���������عر�
	if (GATEWAYSERVER == wdServerType)
	{
		Cmd::FL::t_GYList_FL tCmd;

		tCmd.wdServerID = wdServerID;
		bzero(tCmd.pstrIP, sizeof(tCmd.pstrIP));
		tCmd.wdPort = 0;
		tCmd.wdNumOnline = 0;
		tCmd.state = state_maintain;
		tCmd.zoneGameVersion = 0;

		FLClientManager::getInstance().broadcast(&tCmd, sizeof(tCmd));
	}

	ServerManager::getInstance().removeServer(this);
}

/**
 * \brief ��ӵ�Ψһ����֤������
 *
 * ʵ�����麯��<code>zTCPTask::uniqueAdd</code>
 *
 */
bool ServerTask::uniqueAdd()
{
	Zebra::logger->trace("ServerTask::uniqueAdd");
	return ServerManager::getInstance().uniqueAdd(this);
}

/**
 * \brief ��Ψһ����֤������ɾ��
 *
 * ʵ�����麯��<code>zTCPTask::uniqueRemove</code>
 *
 */
bool ServerTask::uniqueRemove()
{
	Zebra::logger->trace("ServerTask::uniqueRemove");
	return ServerManager::getInstance().uniqueRemove(this);
}

/**
 * \brief �������Է������������Ĺ���������ָ��
 *
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool ServerTask::msgParse_Startup(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("ServerTask::msgParse_Startup");
	using namespace Cmd::Super;

	switch(ptNullCmd->para)
	{
		case PARA_STARTUP_SERVERENTRY_NOTIFYOTHER://1,4
			{		
				// ��Ӧ�����ķ�������NOTIFYOTHER��Ӧ��Ϣ		
				t_Startup_ServerEntry_NotifyOther *ptCmd = (t_Startup_ServerEntry_NotifyOther *)ptNullCmd;	
				// 			
				ServerManager::getInstance().responseOther(ptCmd->entry.wdServerID,ptCmd->srcID);

				return true;
			}
			break;
		case PARA_STARTUP_OK:
			{
				t_Startup_OK *ptCmd= (t_Startup_OK *)ptNullCmd;
				Zebra::logger->debug("ServerTask::msgParse_Startup t_Startup_OK");
				return true;
			}
			break;
		case PARA_RESTART_SERVERENTRY_NOTIFYOTHER:
			{
				t_restart_ServerEntry_NotifyOther *notify = (t_restart_ServerEntry_NotifyOther*)ptNullCmd;
				ServerTask * pSrc = ServerManager::getInstance().getServer(notify->srcID);
				if(pSrc)
				{
					pSrc->notifyOther(notify->dstID);
					return true;
				}
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief �������ԼƷѷ�������ָ��
 *
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool ServerTask::msgParse_Bill(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Super;

	switch(ptNullCmd->para)
	{
		case PARA_BILL_IDINUSE:
			{
				t_idinuse_Bill *ptCmd = (t_idinuse_Bill *)ptNullCmd;
				Cmd::FL::t_idinuse_Session tCmd;

				tCmd.accid = ptCmd->accid;
				tCmd.loginTempID = ptCmd->loginTempID;
				tCmd.wdLoginID = ptCmd->wdLoginID;
				bcopy(ptCmd->name, tCmd.name, sizeof(tCmd.name));
				FLClientManager::getInstance().sendTo(tCmd.wdLoginID, &tCmd, sizeof(tCmd));

				return true;
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief �����������ط�������ָ��
 *
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool ServerTask::msgParse_Gateway(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("ServerTask::msgParse_Gateway(%u, %u)", ptNullCmd->cmd, ptNullCmd->para);
	using namespace Cmd::Super;

	switch(ptNullCmd->para)
	{
		case PARA_GATEWAY_GYLIST:
			{
				t_GYList_Gateway *ptCmd = (t_GYList_Gateway *)ptNullCmd;
				Cmd::FL::t_GYList_FL tCmd;

				OnlineNum = ptCmd->wdNumOnline;

				Zebra::logger->debug("GYList:serverid=%u,ip=%s,port=%u,onlines=%u, state=%u", ptCmd->wdServerID, ptCmd->pstrIP, ptCmd->wdPort, ptCmd->wdNumOnline, ptCmd->state);
				tCmd.wdServerID = ptCmd->wdServerID;
				bcopy(ptCmd->pstrIP, tCmd.pstrIP, sizeof(pstrIP));
				tCmd.wdPort = ptCmd->wdPort;
				tCmd.wdNumOnline = ptCmd->wdNumOnline;
				tCmd.state = ptCmd->state;
				tCmd.zoneGameVersion = ptCmd->zoneGameVersion;
				FLClientManager::getInstance().broadcast(&tCmd, sizeof(tCmd));

				return true;
			}
			break;
		case PARA_GATEWAY_NEWSESSION:
			{
				t_NewSession_Gateway *ptCmd = (t_NewSession_Gateway *)ptNullCmd;
				Cmd::FL::t_NewSession_Session tCmd;

				//Zebra::logger->debug("%s: %u, %u, %s, %u", __FUNCTION__, ptCmd->session.dwUserID, ptCmd->session.loginTempID, ptCmd->session.pstrIP, ptCmd->session.wdPort);
				tCmd.session = ptCmd->session;
				//bcopy(&ptCmd->session, &tCmd.session, sizeof(tCmd.session));
				FLClientManager::getInstance().sendTo(tCmd.session.wdLoginID, &tCmd, sizeof(tCmd));

				return true;
			}
			break;
		case PARA_CHARNAME_GATEWAY:
			{
				using namespace Cmd::RoleReg;
				t_Charname_Gateway *ptCmd = (t_Charname_Gateway *)ptNullCmd;
				t_Charname_Rolereg cmd;
				cmd.wdServerID = ptCmd->wdServerID;
				cmd.accid = ptCmd->accid;
				cmd.gameZone = SuperService::getInstance().getZoneID();
				bcopy(ptCmd->name, cmd.name, sizeof(cmd.name));
				cmd.state = ptCmd->state;
				Zebra::logger->debug("��ɫ����ָ�%u, %s, %u", ptCmd->accid, ptCmd->name, ptCmd->state);
				if (!RoleregClientManager::getInstance().broadcastOne(&cmd, sizeof(cmd)))
				{
					if (ptCmd->state & ROLEREG_STATE_TEST)
					{
						Zebra::logger->error("����ʧ�ܣ�����������ɫ��%u, %s", ptCmd->accid, ptCmd->name);
						t_Charname_Gateway tCmd;
						bcopy(ptCmd, &tCmd, sizeof(tCmd));
						tCmd.state |= ROLEREG_STATE_HAS;	//�����Ѿ����ڱ�־
						sendCmd(&tCmd, sizeof(tCmd));
					}
					if (ptCmd->state & (ROLEREG_STATE_WRITE | ROLEREG_STATE_CLEAN))
					{
						RoleregCache::getInstance().add(*ptCmd);
					}
				}

				return true;
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief ����GM���ߵ�ָ��
 *
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool ServerTask::msgParse_GmTool(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::GmTool;

	switch(ptNullCmd->para)
	{
		case PARA_LOG_GMTOOL:
			{
				t_Log_GmTool * rev = (t_Log_GmTool *)ptNullCmd;
				rev->zone = SuperService::getInstance().getZoneID();
				InfoClientManager::getInstance().broadcastOne(rev, nCmdLen);
				return true;
			}
			break;
		case PARA_CHAT_GMTOOL:
			{
				t_Chat_GmTool * rev = (t_Chat_GmTool *)ptNullCmd;
				rev->zone = SuperService::getInstance().getZoneID();
				strncpy(rev->server, SuperService::getInstance().getZoneName().c_str(), MAX_NAMESIZE);

				InfoClientManager::getInstance().broadcastOne(rev, nCmdLen);
				/*
				if (ret)
					Zebra::logger->debug("[GM����]ת��������Ϣ��InfoServer, %s ��(%u)", rev->userName, rev->zone.id);
				else
					Zebra::logger->debug("[GM����]ת��������Ϣ��InfoServerʧ��, %s ��(%u)", rev->userName, rev->zone.id);
					*/
				return true;
			}
			break;
		case PARA_MSG_GMTOOL:
			{
				t_Msg_GmTool * rev = (t_Msg_GmTool *)ptNullCmd;
				rev->zone = SuperService::getInstance().getZoneID();
				//strncpy(rev->server, SuperService::getInstance().getZoneName().c_str(), MAX_NAMESIZE);
				InfoClientManager::getInstance().broadcastOne(rev, sizeof(t_Msg_GmTool));
				/*
				if (ret)
					Zebra::logger->debug("[GM����]ת��GM������InfoServer, %s ��(%u)", rev->userName, rev->zone.id);
				else
					Zebra::logger->debug("[GM����]ת��GM������InfoServerʧ��, %s ��(%u)", rev->userName, rev->zone.id);
					*/
				return true;
			}
			break;
		case PARA_NEW_MSG_GMTOOL:
			{
				t_NewMsg_GmTool * rev = (t_NewMsg_GmTool *)ptNullCmd;
				rev->zone = SuperService::getInstance().getZoneID();
				InfoClientManager::getInstance().broadcastOne(rev, sizeof(t_NewMsg_GmTool));
				/*
				if (ret)
					Zebra::logger->debug("[GM����]ת��GM������InfoServer, %s ��(%u)", rev->userName, rev->zone.id);
				else
					Zebra::logger->debug("[GM����]ת��GM������InfoServerʧ��, %s ��(%u)", rev->userName, rev->zone.id);
					*/
				return true;
			}
			break;
		case PARA_PUNISH_GMTOOL:
			{
				t_Punish_GmTool * rev = (t_Punish_GmTool *)ptNullCmd;
				rev->zone = SuperService::getInstance().getZoneID();
				strncpy(rev->server, SuperService::getInstance().getZoneName().c_str(), MAX_NAMESIZE);
				InfoClientManager::getInstance().broadcastOne(rev, sizeof(t_Punish_GmTool));
				/*
				if (ret)
					Zebra::logger->debug("[GM����]ת��GM������InfoServer, %s ��(%u)", rev->userName, rev->zone.id);
				else
					Zebra::logger->debug("[GM����]ת��GM������InfoServerʧ��, %s ��(%u)", rev->userName, rev->zone.id);
					*/
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

bool ServerTask::msgParse_CountryOnline(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Super;

	switch(ptNullCmd->para)
	{
		case PARA_COUNTRYONLINE:
			{
				t_CountryOnline * rev = (t_CountryOnline *)ptNullCmd;
				unsigned char pBuffer[zSocket::MAX_DATASIZE];
				Cmd::Info::t_Country_OnlineNum *cmd = (Cmd::Info::t_Country_OnlineNum *)pBuffer;
				constructInPlace(cmd);
				cmd->rTimestamp = rev->rTimestamp;
				cmd->GameZone = SuperService::getInstance().getZoneID();
				cmd->OnlineNum = rev->OnlineNum;

				for(DWORD i = 0; i < cmd->OnlineNum; i++)
				{
					cmd->CountryOnline[i].country = rev->CountryOnline[i].country;
					cmd->CountryOnline[i].num = rev->CountryOnline[i].num;
				}
				return InfoClientManager::getInstance().sendTo(rev->infoTempID, cmd, sizeof(Cmd::Info::t_Country_OnlineNum) + cmd->OnlineNum * sizeof(Cmd::Info::t_Country_OnlineNum::Online));
			}
			break;
	}

	return false;
}

/**
 * \brief �������Ը������������ӵ�ָ��
 *
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool ServerTask::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("ServerTask::msgParse(%u,%u)",ptNullCmd->cmd, ptNullCmd->para);
	switch(ptNullCmd->cmd)
	{
		case Cmd::Super::CMD_STARTUP:
			if (msgParse_Startup(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case Cmd::Super::CMD_BILL:
			if (msgParse_Bill(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case Cmd::Super::CMD_GATEWAY:
			if (msgParse_Gateway(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case Cmd::GmTool::CMD_GMTOOL:
			if (msgParse_GmTool(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case Cmd::Super::CMD_COUNTRYONLINE:
			if (msgParse_CountryOnline(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case Cmd::Super::CMD_SESSION:
			{
				switch(ptNullCmd->para)
				{
					case Cmd::Super::PARA_SHUTDOWN:
						{
							SuperService::getInstance().Terminate();
							Zebra::logger->info("Session����ͣ��ά��");
							return true;
						}
						break;
				}
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

