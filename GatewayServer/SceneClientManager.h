/**
 * \file
 * \version  $Id: SceneClientManager.h $
 * \author  
 * \date 
 * \brief ���ص��������ݻ��巢��
 *
 * 
 */


#ifndef _SCENECLIENTMANAGER_H_
#define _SCENECLIENTMANAGER_H_

#include <map>
#include <set>

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "SceneClient.h"
#include "zTime.h"
#include "zRWLock.h"

/**
 ** \brief �����������Ϣ�ɼ����ӵĿͻ��˹�������
 **/
class SceneClientManager
{

	public:

		~SceneClientManager();

		/**
		 ** \brief ��ȡ���Ψһʵ��
		 ** \return ���Ψһʵ������
		 **/
		static SceneClientManager &getInstance()
		{
			if (NULL == instance)
				instance = new SceneClientManager();

			return *instance;
		}

		/**
		 ** \brief �������Ψһʵ��
		 **/
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		bool init();
		void timeAction(const zTime &ct);
		void add(SceneClient *sceneClient);
		void remove(SceneClient *sceneClient);
		bool broadcastOne(const void *pstrCmd, int nCmdLen);
		bool sendTo(const DWORD tempid, const void *pstrCmd, int nCmdLen);
		void setUsleepTime(int time)
		{
			sceneClientPool->setUsleepTime(time);
		}

	private:

		SceneClientManager();
		static SceneClientManager *instance;

		/**
		 ** \brief �ͻ������ӹ����
		 **/
		zTCPClientTaskPool *sceneClientPool;
		/**
		 ** \brief ���ж�����������ʱ���¼
		 **/
		zTime actionTimer;

		/**
		 ** \brief ��������Ѿ��ɹ���������������
		 **/
		typedef std::map<const DWORD, SceneClient *> SceneClient_map;
		typedef SceneClient_map::iterator iter;
		typedef SceneClient_map::const_iterator const_iter;
		typedef SceneClient_map::value_type value_type;
		/**
		 ** \brief ��������Ѿ��ɹ�����������
		 **/
		SceneClient_map allClients;


		/**
		 ** \brief �������ʶ�д��
		 **/
		zRWLock rwlock;

};

#endif

