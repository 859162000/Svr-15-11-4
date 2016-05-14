/**
 * \file
 * \version  $Id: BillTaskManager.h  $
 * \author  
 * \date 
 * \brief ���������ӵ�����
 *
 * 
 */


#ifndef _BillTaskManager_h_
#define _BillTaskManager_h_

#include <iostream>
#include <ext/hash_map>

#include "BillTask.h"
#include "zRWLock.h"

/**
 * \brief �Ʒѷ����������ӹ�����
 *
 */
class BillTaskManager
{

	public:

		/**
		 * \brief ��������
		 *
		 */
		~BillTaskManager() {};

		/**
		 * \brief ��ȡ�����ӹ�����Ψһʵ��
		 *
		 * \return ������Ψһʵ��
		 */
		static BillTaskManager &getInstance()
		{
			if (NULL == instance)
				instance = new BillTaskManager();

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

		bool uniqueAdd(BillTask *task);
		bool uniqueRemove(BillTask *task);
		bool broadcastByID(const WORD wdServerID, const void *pstrCmd, int nCmdLen);
		BillTask *getTaskByID(const WORD wdServerID);
		void execEvery();

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static BillTaskManager *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		BillTaskManager() {};

		/**
		 * \brief ������������
		 *
		 */
		typedef __gnu_cxx::hash_map<WORD, BillTask *> BillTaskHashmap;
		/**
		 * \brief ������������������
		 *
		 */
		typedef BillTaskHashmap::iterator BillTaskHashmap_iterator;
		/**
		 * \brief ����������������������
		 *
		 */
		typedef BillTaskHashmap::const_iterator BillTaskHashmap_const_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef BillTaskHashmap::value_type BillTaskHashmap_pair;
		/**
		 * \brief �������ʻ������
		 *
		 */
		zRWLock rwlock;
		/**
		 * \brief ����һ��������������е�������
		 *
		 */
		BillTaskHashmap sessionTaskSet;

};

#endif

