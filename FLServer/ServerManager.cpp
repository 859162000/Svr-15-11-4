/**
 * \file
 * \version  $Id: ServerManager.cpp  $
 * \author  
 * \date 
 * \brief ʵ�ַ�������������
 *
 * �����������ȫ��������Ψһ����֤����
 * 
 */


#include <iostream>
#include <list>

#include "ServerTask.h"
#include "zMutex.h"
#include "zNoncopyable.h"
#include "ServerManager.h"
#include "Zebra.h"

ServerManager *ServerManager::instance = NULL;

/**
 * \brief ��һ������������������ӵ�Ψһ��������
 *
 * \param task ��������������
 * \return ����Ƿ�ɹ���Ҳ����Ψһ����֤�Ƿ�ɹ�
 */
bool ServerManager::uniqueAdd(ServerTask *task)
{
	Zebra::logger->trace("ServerManager::uniqueAdd");
	zMutex_scope_lock scope_lock(mlock);
	ServerTaskContainer_const_iterator it;
	it = taskUniqueContainer.find(task->getZoneID());
	if (it != taskUniqueContainer.end())
	{
		Zebra::logger->error("%s", __PRETTY_FUNCTION__);
		return false;
	}
	taskUniqueContainer.insert(ServerTaskContainer_value_type(task->getZoneID(), task));
	return true;
}

/**
 * \brief ��Ψһ��������ɾ��һ����������
 *
 * \param task ��������������
 * \return ɾ���Ƿ�ɹ�
 */
bool ServerManager::uniqueRemove(ServerTask *task)
{
	Zebra::logger->trace("ServerManager::uniqueRemove");
	zMutex_scope_lock scope_lock(mlock);
	ServerTaskContainer_iterator it;
	it = taskUniqueContainer.find(task->getZoneID());
	if (it != taskUniqueContainer.end())
	{
		taskUniqueContainer.erase(it);
	}
	else
		Zebra::logger->warn("%s", __PRETTY_FUNCTION__);
	return true;
}

bool ServerManager::broadcast(const GameZone_t &gameZone, const void *pstrCmd, int nCmdLen)
{
	Zebra::logger->trace("ServerManager::broadcast");
	zMutex_scope_lock scope_lock(mlock);
	ServerTaskContainer_iterator it;
	it = taskUniqueContainer.find(gameZone);
	if (it != taskUniqueContainer.end())
	{
		return it->second->sendCmd(pstrCmd, nCmdLen);
	}
	else
		return false;
}

