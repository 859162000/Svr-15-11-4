/**
 * \file
 * \version  $Id: FLClientManager.cpp  $
 * \author  
 * \date 
 * \brief ʵ��ͳһ�û�ƽ̨�ͻ������ӵĹ�������
 */


#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "FLClient.h"
#include "FLClientManager.h"
#include "zXMLParser.h"

/**
 * \brief ���Ψһʵ��ָ��
 */
FLClientManager *FLClientManager::instance = NULL;

/**
 * \brief ���캯��
 */
FLClientManager::FLClientManager()
{
	flClientPool = NULL;
}

/**
 * \brief ��������
 */
FLClientManager::~FLClientManager()
{
	SAFE_DELETE(flClientPool);
}

/**
 * \brief ��ʼ��������
 * \return ��ʼ���Ƿ�ɹ�
 */
bool FLClientManager::init()
{
	Zebra::logger->trace("FLClientManager::init");
	flClientPool = new zTCPClientTaskPool();
	if (NULL == flClientPool
			|| !flClientPool->init())
		return false;

	zXMLParser xml;
	if (!xml.initFile(Zebra::global["loginServerListFile"]))
	{
		Zebra::logger->error("����ͳһ�û�ƽ̨��½�������б��ļ� %s ʧ��", Zebra::global["loginServerListFile"].c_str());
		return false;
	}
	xmlNodePtr root = xml.getRootNode("Zebra");
	if (root)
	{
		xmlNodePtr zebra_node = xml.getChildNode(root, "LoginServerList");
		while(zebra_node)
		{
			if (strcmp((char *)zebra_node->name, "LoginServerList") == 0)
			{
				xmlNodePtr node = xml.getChildNode(zebra_node, "server");
				while(node)
				{
					if (strcmp((char *)node->name, "server") == 0)
					{
						Zebra::global["FLServer"] = "";
						Zebra::global["FLPort"] = "";
						if (xml.getNodePropStr(node, "ip", Zebra::global["FLServer"])
								&& xml.getNodePropStr(node, "port", Zebra::global["FLPort"]))
						{
							Zebra::logger->debug("LoginServer: %s, %s", Zebra::global["FLServer"].c_str(), Zebra::global["FLPort"].c_str());
							flClientPool->put(new FLClient(Zebra::global["FLServer"], atoi(Zebra::global["FLPort"].c_str())));
						}
					}

					node = xml.getNextNode(node, NULL);
				}
			}

			zebra_node = xml.getNextNode(zebra_node, NULL);
		}
	}

	Zebra::logger->info("����ͳһ�û�ƽ̨��½�������б��ļ��ɹ�");
	return true;
}

/**
 * \brief ���ڼ���������ӵĶ�����������
 * \param ct ��ǰʱ��
 */
void FLClientManager::timeAction(const zTime &ct)
{
	Zebra::logger->trace("FLClientManager::timeAction");
	if (actionTimer.elapse(ct) > 4)
	{
		if (flClientPool)
			flClientPool->timeAction(ct);
		actionTimer = ct;
	}
}

/**
 * \brief ������������Ѿ��ɹ�������
 * \param flClient ����ӵ�����
 */
void FLClientManager::add(FLClient *flClient)
{
	Zebra::logger->trace("FLClientManager::add");
	if (flClient)
	{
		zRWLock_scope_wrlock scope_wrlock(rwlock);
		const_iter it = allClients.find(flClient->getTempID());
		if (it == allClients.end())
		{
			allClients.insert(value_type(flClient->getTempID(), flClient));
		}
	}
}

/**
 * \brief ���������Ƴ��Ͽ�������
 * \param flClient ���Ƴ�������
 */
void FLClientManager::remove(FLClient *flClient)
{
	Zebra::logger->trace("FLClientManager::remove");
	if (flClient)
	{
		zRWLock_scope_wrlock scope_wrlock(rwlock);
		iter it = allClients.find(flClient->getTempID());
		if (it != allClients.end())
		{
			allClients.erase(it);
		}
	}
}

/**
 * \brief ��ɹ����������ӹ㲥ָ��
 * \param pstrCmd ���㲥��ָ��
 * \param nCmdLen ���㲥ָ��ĳ���
 */
void FLClientManager::broadcast(const void *pstrCmd, int nCmdLen)
{
	Zebra::logger->trace("FLClientManager::broadcast");
	zRWLock_scope_rdlock scope_rdlock(rwlock);
	for(iter it = allClients.begin(); it != allClients.end(); ++it)
	{
		it->second->sendCmd(pstrCmd, nCmdLen);
	}
}

/**
 * \brief ��ָ���ĳɹ����ӹ㲥ָ��
 * \param tempid ���㲥ָ���������ʱ���
 * \param pstrCmd ���㲥��ָ��
 * \param nCmdLen ���㲥ָ��ĳ���
 */
void FLClientManager::sendTo(const WORD tempid, const void *pstrCmd, int nCmdLen)
{
	Zebra::logger->trace("FLClientManager::sendTo");
	zRWLock_scope_rdlock scope_rdlock(rwlock);
	iter it = allClients.find(tempid);
	if (it != allClients.end())
	{
		it->second->sendCmd(pstrCmd, nCmdLen);
	}
}

