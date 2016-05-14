/**
 * \file
 * \version  $Id: InfoClientManager.cpp  $
 * \author  
 * \date 
 * \brief �����������Ϣ�ɼ����ӵĿͻ��˹�������
 */


#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "InfoClient.h"
#include "InfoClientManager.h"
#include "zXMLParser.h"

/**
 * \brief ���Ψһʵ��ָ��
 */
InfoClientManager *InfoClientManager::instance = NULL;

/**
 * \brief ���캯��
 */
InfoClientManager::InfoClientManager()
{
	infoClientPool = NULL;
}

/**
 * \brief ��������
 */
InfoClientManager::~InfoClientManager()
{
	SAFE_DELETE(infoClientPool);
}

/**
 * \brief ��ʼ��������
 * \return ��ʼ���Ƿ�ɹ�
 */
bool InfoClientManager::init()
{
	Zebra::logger->trace("InfoClientManager::init");
	infoClientPool = new zTCPClientTaskPool();
	if (NULL == infoClientPool
			|| !infoClientPool->init())
		return false;

	Zebra::logger->debug("InfoServer: %s, %s", Zebra::global["InfoServer"].c_str(), Zebra::global["InfoPort"].c_str());
	infoClientPool->put(new InfoClient(Zebra::global["InfoServer"], atoi(Zebra::global["InfoPort"].c_str())));

	return true;
}

/**
 * \brief ���ڼ���������ӵĶ�����������
 * \param ct ��ǰʱ��
 */
void InfoClientManager::timeAction(const zTime &ct)
{
	Zebra::logger->trace("InfoClientManager::timeAction");
	if (actionTimer.elapse(ct) > 4)
	{
		if (infoClientPool)
			infoClientPool->timeAction(ct);
		actionTimer = ct;
	}
}

/**
 * \brief ������������Ѿ��ɹ�������
 * \param infoClient ����ӵ�����
 */
void InfoClientManager::add(InfoClient *infoClient)
{
	Zebra::logger->trace("InfoClientManager::add");
	if (infoClient)
	{
		mlock.lock();
		allClients.push_back(infoClient);
		mlock.unlock();
	}
}

/**
 * \brief ���������Ƴ��Ͽ�������
 * \param infoClient ���Ƴ�������
 */
void InfoClientManager::remove(InfoClient *infoClient)
{
	Zebra::logger->trace("InfoClientManager::remove");
	if (infoClient)
	{
		mlock.lock();
		InfoClientContainer::iterator it = find(allClients.begin(), allClients.end(), infoClient);
		if (it != allClients.end())
		{
			allClients.erase(it);
		}
		mlock.unlock();
	}
}

/**
 * \brief ��ɹ����������ӹ㲥ָ��
 * \param pstrCmd ���㲥��ָ��
 * \param nCmdLen ���㲥ָ��ĳ���
 */
bool InfoClientManager::broadcast(const void *pstrCmd, int nCmdLen)
{
	Zebra::logger->trace("InfoClientManager::broadcast");
	bool retval = false;
	mlock.lock();
	for(InfoClientContainer::iterator it = allClients.begin(); it != allClients.end(); ++it)
	{
		retval = retval | (*it)->sendCmd(pstrCmd, nCmdLen);
	}
	mlock.unlock();
	return retval;
}

