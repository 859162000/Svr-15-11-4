/**
 * \file
 * \version  $Id: GatewayTaskManager.h  $
 * \author  
 * \date 
 * \brief ���������ӵ�����
 *
 * 
 */


#ifndef _GatewayTaskManager_h_
#define _GatewayTaskManager_h_

#include <iostream>
#include <ext/hash_map>

#include "zEntry.h"
#include "GatewayTask.h"
#include "zRWLock.h"

/**
 * \brief �����������ӹ�����
 *
 */
class GatewayTaskManager
{

	public:

		/**
		 * \brief �ص�����
		 *
		 */
		typedef zEntryCallback<GatewayTask> GatewayTaskCallback;

		/**
		 * \brief ��������
		 *
		 */
		~GatewayTaskManager();

		/**
		 * \brief ��ȡ�����ӹ�����Ψһʵ��
		 *
		 * \return �����ӹ�����Ψһʵ��
		 */
		static GatewayTaskManager &getInstance()
		{
			if (NULL == instance)
				instance = new GatewayTaskManager();

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

		bool uniqueAdd(GatewayTask *task);
		bool uniqueRemove(GatewayTask *task);
		void accountVerifyOK(const DWORD accid, const bool ok);
		void execAll(GatewayTaskCallback &callback);

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static GatewayTaskManager *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		GatewayTaskManager();

		/**
		 * \brief ������������
		 *
		 */
		typedef __gnu_cxx::hash_map<DWORD, GatewayTask *> GatewayTaskHashmap;
		/**
		 * \brief ������������������
		 *
		 */
		typedef GatewayTaskHashmap::iterator GatewayTaskHashmap_iterator;
		/**
		 * \brief ����������������������
		 *
		 */
		typedef GatewayTaskHashmap::const_iterator GatewayTaskHashmap_const_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef GatewayTaskHashmap::value_type GatewayTaskHashmap_pair;
		/**
		 * \brief �������ʻ������
		 *
		 */
		zRWLock rwlock;
		/**
		 * \brief �����ӹ�����������
		 *
		 */
		GatewayTaskHashmap gatewayTaskSet;

};

#endif

