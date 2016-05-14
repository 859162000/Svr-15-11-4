/**
 * \file
 * \version  $Id: RoleregClientManager.h  $
 * \author  
 * \date 
 * \brief �����ɫ����Ψһ����֤�������ӵĿͻ��˹�������
 */


#ifndef _RoleregClientManager_h_
#define _RoleregClientManager_h_

#include <set>

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "RoleregClient.h"
#include "zTime.h"
#include "zRWLock.h"

/**
 * \brief �����ɫ����Ψһ����֤�������ӵĿͻ��˹�������
 */
class RoleregClientManager
{

	public:

		~RoleregClientManager();

		/**
		 * \brief ��ȡ���Ψһʵ��
		 * \return ���Ψһʵ������
		 */
		static RoleregClientManager &getInstance()
		{
			if (NULL == instance)
				instance = new RoleregClientManager();

			return *instance;
		}

		/**
		 * \brief �������Ψһʵ��
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		bool init();
		void timeAction(const zTime &ct);
		void add(RoleregClient *roleregClient);
		void remove(RoleregClient *roleregClient);
		bool broadcastOne(const void *pstrCmd, int nCmdLen);

	private:

		RoleregClientManager();
		static RoleregClientManager *instance;

		/**
		 * \brief �ͻ������ӹ����
		 */
		zTCPClientTaskPool *roleregClientPool;
		/**
		 * \brief ���ж�����������ʱ���¼
		 */
		zTime actionTimer;

		struct lt_client
		{
			bool operator()(RoleregClient *s1, RoleregClient *s2) const
			{
				return s1->getNetType() < s2->getNetType();
			}
		};
		/**
		 * \brief ��������Ѿ��ɹ���������������
		 */
		typedef std::multiset<RoleregClient *, lt_client> RoleregClient_set;
		typedef RoleregClient_set::iterator iter;
		typedef RoleregClient_set::const_iterator const_iter;
		/**
		 * \brief ��������Ѿ��ɹ�����������
		 */
		RoleregClient_set allClients;
		/**
		 * \brief �������ʶ�д��
		 */
		zRWLock rwlock;

};

#endif

