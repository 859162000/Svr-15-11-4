/**
 * \file
 * \version  $Id: SessionTaskManager.h  $
 * \author 
 * \date 
 * \brief ���������ӵ�����
 *
 * 
 */


#ifndef _SessionTaskManager_h_
#define _SessionTaskManager_h_

#include <iostream>
#include <list>
#include <ext/hash_map>

#include "SessionTask.h"
#include "zNoncopyable.h"
#include "zRWLock.h"

/**
 * \brief Session�����������ӹ�����
 *
 */
class SessionTaskManager : private zNoncopyable
{

	public:

		/**
		 * \brief ��������
		 *
		 */
		~SessionTaskManager() {};

		/**
		 * \brief ��ȡ�����ӹ�����Ψһʵ��
		 *
		 * \return ������Ψһʵ��
		 */
		static SessionTaskManager &getInstance()
		{
			if (NULL == instance)
				instance = new SessionTaskManager();

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void addSessionTask(SessionTask *task);
		void removeSessionTask(SessionTask *task);
		bool uniqueAdd(SessionTask *task);
		bool uniqueRemove(SessionTask *task);
		bool broadcastGateway(const void *pstrCmd, int nCmdLen);
		bool broadcastScene(const void *pstrCmd, int nCmdLen);
		bool sendCmdToWorld(const void *pstrCmd, int nCmdLen);
		bool broadcastByID(const WORD wdServerID, const void *pstrCmd, int nCmdLen);
		void execEvery();
		bool sendCmdToCountry(DWORD country,const void *pstrCmd, int nCmdLen);

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static SessionTaskManager *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		SessionTaskManager() {};

		/**
		 * \brief ������������
		 *
		 */
		typedef __gnu_cxx::hash_map<WORD, SessionTask *> SessionTaskHashmap;
		/**
		 * \brief ������������������
		 *
		 */
		typedef SessionTaskHashmap::iterator SessionTaskHashmap_iterator;
		/**
		 * \brief ����������������������
		 *
		 */
		typedef SessionTaskHashmap::const_iterator SessionTaskHashmap_const_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef SessionTaskHashmap::value_type SessionTaskHashmap_pair;
		/**
		 * \brief ������������
		 *
		 */
		typedef std::list<SessionTask *> TaskContainer;
		/**
		 * \brief ������������������
		 *
		 */
		typedef TaskContainer::iterator TaskContainer_iterator;
		/**
		 * \brief �������ʻ������
		 *
		 */
		zRWLock rwlock;
		//zMutex mlock;
		/**
		 * \brief ����һ��������������е�������
		 *
		 */
		SessionTaskHashmap sessionTaskSet;
		/**
		 * \brief �볡�����������ӵ�����
		 *
		 */
		TaskContainer sceneTaskList;
		/**
		 * \brief �����ط��������ӵ�����
		 *
		 */
		TaskContainer gatewayTaskList;

};

#endif

