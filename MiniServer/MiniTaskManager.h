/**
 * \file
 * \version  $Id: MiniTaskManager.h $
 * \author  
 * \date 
 * \brief ���������ӵ�����
 *
 * 
 */


#ifndef _MINITASKMANAGER_H_
#define _MINITASKMANAGER_H_

#include <iostream>
#include <ext/hash_map>

#include "MiniTask.h"
#include "zRWLock.h"

/**
 * \brief �Ʒѷ����������ӹ�����
 *
 */
class MiniTaskManager
{

	public:

		/**
		 * \brief ��������
		 *
		 */
		~MiniTaskManager() {};

		/**
		 * \brief ��ȡ�����ӹ�����Ψһʵ��
		 *
		 * \return ������Ψһʵ��
		 */
		static MiniTaskManager &getInstance()
		{
			if (NULL == instance)
				instance = new MiniTaskManager();

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

		bool uniqueAdd(MiniTask *task);
		bool uniqueRemove(MiniTask *task);
		bool broadcastByID(const WORD wdServerID, const void *pstrCmd, int nCmdLen);
		MiniTask *getTaskByID(const WORD wdServerID);
		void execEvery();

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static MiniTaskManager *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		MiniTaskManager() {};

		/**
		 * \brief ������������
		 *
		 */
		typedef __gnu_cxx::hash_map<WORD, MiniTask *> MiniTaskHashmap;
		/**
		 * \brief ������������������
		 *
		 */
		typedef MiniTaskHashmap::iterator MiniTaskHashmap_iterator;
		/**
		 * \brief ����������������������
		 *
		 */
		typedef MiniTaskHashmap::const_iterator MiniTaskHashmap_const_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef MiniTaskHashmap::value_type MiniTaskHashmap_pair;
		/**
		 * \brief �������ʻ������
		 *
		 */
		zRWLock rwlock;
		/**
		 * \brief ����һ��������������е�������
		 *
		 */
		MiniTaskHashmap sessionTaskSet;

};

#endif

