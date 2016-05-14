/**
 * \file
 * \version  $Id: zSubNetService.cpp  $
 * \author  
 * \date 
 * \brief ʵ������������Ŀ�ܴ���
 *
 * 
 */

#include <iostream>
#include <string>
#include <deque>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zNetService.h"
#include "zSubNetService.h"
#include "Zebra.h"
#include "SuperCommand.h"

zSubNetService *zSubNetService::subNetServiceInst = NULL;

/**
 * \brief �����������������ӿͻ�����
 *
 */
class SuperClient : public zTCPBufferClient
{

	public:

		friend class zSubNetService;

		/**
		 * \brief ���캯��
		 *
		 */
		SuperClient() : zTCPBufferClient("�������������ͻ���"), verified(false)
		{
			Zebra::logger->trace("SuperClient::SuperClient");
		}

		/**
		 * \brief ��������
		 *
		 */
		~SuperClient() {};

		void run();
		bool msgParse_Startup(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

	private:

		bool verified;			/**< �Ƿ��Ѿ�ͨ���˷���������������֤ */

};

/**
 * \brief ����zThread�еĴ��麯�������̵߳����ص����������ڴ�����յ���ָ��
 *
 */
void SuperClient::run()
{
	Zebra::logger->trace("SuperClient::run");
	zTCPBufferClient::run();

	//�������������֮������ӶϿ�����Ҫ�رշ�����
	zSubNetService::subNetServiceInstance()->Terminate();
}

/**
 * \brief �������Է������������Ĺ���������ָ��
 *
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool SuperClient::msgParse_Startup(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("SuperClient::msgParse_Startup(%u,%u)",ptNullCmd->cmd, ptNullCmd->para);
	using namespace Cmd::Super;

	switch(ptNullCmd->para)
	{
		case PARA_GAMETIME:
			{
				t_GameTime *ptCmd = (t_GameTime *)ptNullCmd;

				Zebra::logger->debug("ˢ����Ϸʱ�䣺%lu", ptCmd->qwGameTime);
				Zebra::qwGameTime = ptCmd->qwGameTime;

				return true;
			}
			break;
		case PARA_STARTUP_RESPONSE:
			{
				t_Startup_Response *ptCmd = (t_Startup_Response *)ptNullCmd;

				Zebra::logger->debug("��������������Ӧ��Ϣ %u %u %u", ptCmd->wdServerID, ptCmd->wdPort, ptCmd->wdExtPort);

				zSubNetService::subNetServiceInstance()->setServerInfo(ptCmd);
			

				return true;
			}

			break;
		case PARA_STARTUP_SERVERENTRY_NOTIFYME:
			{
				t_Startup_ServerEntry_NotifyMe *ptCmd = (t_Startup_ServerEntry_NotifyMe *)ptNullCmd;

				Zebra::logger->debug("�յ�����NotifyMe����,�Լ��������ķ������б�");
				for(WORD i = 0; i < ptCmd->size; i++)
				{
					Zebra::logger->debug("serverid=%u, servertype=%u, servername=%s,serverip=%s, port=%u, extip=%s, extport%u, state=%u",
							ptCmd->entry[i].wdServerID,
							ptCmd->entry[i].wdServerType,
							ptCmd->entry[i].pstrName,
							ptCmd->entry[i].pstrIP,
							ptCmd->entry[i].wdPort,
							ptCmd->entry[i].pstrExtIP,
							ptCmd->entry[i].wdExtPort,
							ptCmd->entry[i].state);
					//��Ҫһ��������������Щ�������б�
					zSubNetService::subNetServiceInstance()->addServerEntry(ptCmd->entry[i]);
				}

				if (!verified)
					verified = true;

				return true;
			}

			break;
		case PARA_STARTUP_SERVERENTRY_NOTIFYOTHER://1,4
			{
				t_Startup_ServerEntry_NotifyOther *ptCmd = (t_Startup_ServerEntry_NotifyOther *)ptNullCmd;

				Zebra::logger->debug("�����������Լ��ķ�����");
				Zebra::logger->debug("serverid=%u, servertype=%u, servername=%s,serverip=%s, port=%u, extip=%s, extport%u, state=%u",
						ptCmd->entry.wdServerID,
						ptCmd->entry.wdServerType,
						ptCmd->entry.pstrName,
						ptCmd->entry.pstrIP,
						ptCmd->entry.wdPort,
						ptCmd->entry.pstrExtIP,
						ptCmd->entry.wdExtPort,
						ptCmd->entry.state);
				//��Ҫһ��������������Щ�������б�
				zSubNetService::subNetServiceInstance()->addServerEntry(ptCmd->entry);
				// �������SuperServer
				return sendCmd(ptCmd, nCmdLen);
			}

			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief �������Է�������������ָ��
 *
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool SuperClient::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("SuperClient::msgParse(%u,%u)",ptNullCmd->cmd, ptNullCmd->para);
	using namespace Cmd::Super;

	switch(ptNullCmd->cmd)
	{
		case CMD_STARTUP:
			if (msgParse_Startup(ptNullCmd, nCmdLen))
			{
				return true;
			}

			break;
		default:
			if (zSubNetService::subNetServiceInstance()->msgParse_SuperService(ptNullCmd, nCmdLen))
			{
				return true;
			}

			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief ���캯��
 * 
 * \param name ����
 * \param wdType ����������
 */
zSubNetService::zSubNetService(const std::string &name, const WORD wdType) : zNetService(name), superClient(NULL)
{
	Zebra::logger->trace("zSubNetService::zSubNetService");

	subNetServiceInst = this;

	superPort = atoi(Zebra::global["port"].c_str());
	bzero(superIP, sizeof(superIP));
	strncpy(superIP, Zebra::global["server"].c_str(), sizeof(superIP) - 1);

	superClient = new SuperClient();

	wdServerID = 0;
	wdServerType = wdType;
	bzero(pstrName, sizeof(pstrName));
	bzero(pstrIP, sizeof(pstrIP));
	wdPort = 0;
	bzero(pstrExtIP, sizeof(pstrExtIP));
	wdExtPort = 0;
}

/**
 * \brief ����������
 *
 */
zSubNetService::~zSubNetService()
{
	Zebra::logger->trace("zSubNetService::~zSubNetService");
	serverList.clear();

	SAFE_DELETE(superClient);

	subNetServiceInst = NULL;
}

/**
 * \brief ��ʼ���������������
 *
 * ʵ�ִ��麯��<code>zService::init</code>
 * �����������������������ӣ����õ���������Ϣ
 *
 * \return �Ƿ�ɹ�
 */
bool zSubNetService::init()
{
	Zebra::logger->trace("zSubNetService::init");

	//������������������������
	if (!superClient->connect(superIP, superPort))
	{
		Zebra::logger->error("���ӷ�����������ʧ��(%s:%u)",superIP,superPort);
		return false;
	}

	//���͵�½��������������ָ��
	using namespace Cmd::Super;
	t_Startup_Request tCmd;
	tCmd.wdServerType = wdServerType;
	strcpy(tCmd.pstrIP, pstrIP);
	if (!superClient->sendCmd(&tCmd, sizeof(tCmd)))
	{
		Zebra::logger->error("(zSubNetService::init)����������������͵�½ָ��ʧ��");
		return false;
	}

	
	//�ȴ�������������������Ϣ
	while(!superClient->verified)
	{
		
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = superClient->pSocket->recvToCmd(pstrCmd, sizeof(pstrCmd), true);		
		if (-1 == nCmdLen)
		{
			Zebra::logger->error("(zSubNetService::init)�ȴ�������������������Ϣʧ��");
			return false;
		}
		else if (nCmdLen > 0)
		{
			Cmd::t_NullCmd *cmd = 0;
			cmd = (Cmd::t_NullCmd *)pstrCmd ;			
			if (!superClient->msgParse((Cmd::t_NullCmd *)pstrCmd, nCmdLen))
				return false;
		}
	}
		
	Zebra::logger->trace("zSubNetService::init wdServerID=%u",wdServerID);

	//zThread::sleep(1);

	//�����߳������������������
	superClient->start();

	//������ʵ�ĳ�ʼ������
	if (!zNetService::init(wdPort))
		return false;

	return true;
}

/**
 * \brief ȷ�Ϸ�������ʼ���ɹ��������������ص�����
 *
 * �����������t_Startup_OKָ����ȷ�Ϸ����������ɹ�
 *
 * \return ȷ���Ƿ�ɹ�
 */
bool zSubNetService::validate()
{
	Zebra::logger->trace("zSubNetService::validate");
	Cmd::Super::t_Startup_OK tCmd;
	tCmd.wdServerID = wdServerID;
	return superClient->sendCmd(&tCmd, sizeof(tCmd));
}

/**
 * \brief �������������
 *
 * ʵ�ִ��麯��<code>zService::final</code>
 *
 */
void zSubNetService::final()
{
	Zebra::logger->trace("zSubNetService::final");
	zNetService::final();
	
	//�رյ�������������������
	superClient->final();
	superClient->join();
	superClient->close();
}

/**
 * \brief �����������������ָ��
 *
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��Ĵ�С
 * \return �����Ƿ�ɹ�
 */
bool zSubNetService::sendCmdToSuperServer(const void *pstrCmd, const int nCmdLen)
{
	Zebra::logger->trace("zSubNetService::sendCmdToSuperServer");
	return superClient->sendCmd(pstrCmd, nCmdLen);
}

/**
 * \brief ���ݷ�����������������Ϣ�����÷���������Ϣ
 *
 * \param ptCmd ������������������Ϣ
 */
void zSubNetService::setServerInfo(const Cmd::Super::t_Startup_Response *ptCmd)
{	
	wdServerID = ptCmd->wdServerID;
	wdPort = ptCmd->wdPort;
	bcopy(ptCmd->pstrExtIP,pstrExtIP,MAX_IP_LENGTH);
	wdExtPort = ptCmd->wdExtPort;
	Zebra::logger->debug("zSubNetService::setServerInfo IP=%s Port=%u ServerID=%u",ptCmd->pstrExtIP,wdExtPort,wdServerID);
}

/**
 * \brief ��ӹ�����������Ϣ��һ��������
 *
 */
void zSubNetService::addServerEntry(const Cmd::Super::ServerEntry &entry)
{
	Zebra::logger->trace("zSubNetService::addServerEntry(wdserverID=%u)",entry.wdServerID);
	mlock.lock();
	//���Ȳ�����û���ظ���
	std::deque<Cmd::Super::ServerEntry>::iterator it;
	bool found = false;
	for(it = serverList.begin(); it != serverList.end(); it++)
	{
		if (entry.wdServerID == it->wdServerID)
		{
			found = true;
			break;
		}
	}

	if (found)
	{
		//�Ѿ�����ֻ�Ǹ���
		(*it) = entry;
	}
	else
	{
		//�������ڣ���Ҫ�½���һ���ڵ�
		serverList.push_back(entry);
	}
	mlock.unlock();
}

/**
 * \brief ������ط�������Ϣ
 *
 * \param wdServerID ���������
 * \return ��������Ϣ
 */
const Cmd::Super::ServerEntry *zSubNetService::getServerEntry(const WORD wdServerID)
{
	Zebra::logger->trace("zSubNetService::getServerEntry(%u)",wdServerID);
	Cmd::Super::ServerEntry *ret = NULL;
	std::deque<Cmd::Super::ServerEntry>::iterator it;
	mlock.lock();
	for(it = serverList.begin(); it != serverList.end(); it++)
	{
		if (wdServerID == it->wdServerID)
		{
			ret = &(*it);
			break;
		}
	}
	mlock.unlock();
	return ret;
}

/**
 * \brief ������ط�������Ϣ
 *
 * \param wdServerType ����������
 * \return ��������Ϣ
 */
const Cmd::Super::ServerEntry *zSubNetService::getServerEntryByType(const WORD wdServerType)
{
	Zebra::logger->trace("zSubNetService::getServerEntryByType(type=%u)",wdServerType);
	Cmd::Super::ServerEntry *ret = NULL;
	std::deque<Cmd::Super::ServerEntry>::iterator it;
	mlock.lock();
	for(it = serverList.begin(); it != serverList.end(); it++)
	{
		Zebra::logger->debug("��������Ϣ��%u, %u", wdServerType, it->wdServerType);
		if (wdServerType == it->wdServerType)
		{
			ret = &(*it);
			break;
		}
	}
	mlock.unlock();
	return ret;
}

/**
 * \brief ������ط�������Ϣ
 *
 * \param wdServerType ����������
 * \param prev ��һ����������Ϣ
 * \return ��������Ϣ
 */
const Cmd::Super::ServerEntry *zSubNetService::getNextServerEntryByType(const WORD wdServerType, const Cmd::Super::ServerEntry **prev)
{
	Zebra::logger->trace("zSubNetService::getNextServerEntryByType");
	Cmd::Super::ServerEntry *ret = NULL;
	bool found = false;
	std::deque<Cmd::Super::ServerEntry>::iterator it;
	mlock.lock();
	for(it = serverList.begin(); it != serverList.end(); it++)
	{
		Zebra::logger->debug("��������Ϣ��%u, %u", wdServerType, it->wdServerType);
		if (wdServerType == it->wdServerType)
		{
			if (NULL == prev
					|| found)
			{
				ret = &(*it);
				break;
			}
			else if (found == false
					&& (*prev)->wdServerID == it->wdServerID)
			{
				found = true;
			}
		}
	}
	mlock.unlock();
	return ret;
}

