/**
 * \file
 * \version  $Id: SceneTaskManager.h  $
 * \author  
 * \date 
 * \brief ���������ӵ�����
 *
 * 
 */


#ifndef _SceneTaskManager_h_
#define _SceneTaskManager_h_

#include <iostream>
#include <ext/hash_map>

#include "SceneTask.h"
#include "zNoncopyable.h"
#include "zEntryManager.h"
#include "zRWLock.h"

/**
 * \brief ���������������ӹ�����
 *
 */
class SceneTaskManager : private zNoncopyable
{

	public:

		/**
		 * \brief ��������
		 *
		 */
		~SceneTaskManager() {};

		/**
		 * \brief ��ȡ���������ӹ�����Ψһʵ��
		 *
		 * \return ���������ӹ�����Ψһʵ��
		 */
		static SceneTaskManager &getInstance()
		{
			if (NULL == instance)
				instance = new SceneTaskManager();

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

		bool uniqueAdd(SceneTask *task);
		bool uniqueRemove(SceneTask *task);
		SceneTask *uniqueGet(WORD wdServerID);
		void execEvery();
		bool broadcastCmd(const void *pstrCmd, const int nCmdLen);
		template <class YourEntry>
		void execEverySceneTask(execEntry<YourEntry> &exec)
		{
			rwlock.rdlock();
			SceneTaskHashmap_iterator it;
			it = sceneTaskSet.begin();
			for (; it != sceneTaskSet.end() ; it ++)
			{
				exec.exec(it->second);
			}
			rwlock.unlock();
		}

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static SceneTaskManager *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		SceneTaskManager() {};

		/**
		 * \brief �����ӹ�����������
		 *
		 */
		typedef __gnu_cxx::hash_map<WORD, SceneTask *> SceneTaskHashmap;
		/**
		 * \brief ������������������
		 *
		 */
		typedef SceneTaskHashmap::iterator SceneTaskHashmap_iterator;
		/**
		 * \brief ����������������������
		 *
		 */
		typedef SceneTaskHashmap::const_iterator SceneTaskHashmap_const_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef SceneTaskHashmap::value_type SceneTaskHashmap_pair;
		/**
		 * \brief �������ʻ������
		 *
		 */
		zRWLock rwlock;
		/**
		 * \brief �����ӹ�����������
		 *
		 */
		SceneTaskHashmap sceneTaskSet;

};

#endif

