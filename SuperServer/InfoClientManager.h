/**
 * \file
 * \version  $Id: InfoClientManager.h  $
 * \author  
 * \date 
 * \brief �����������Ϣ�ɼ����ӵĿͻ��˹�������
 */


#ifndef _InfoClientManager_h_
#define _InfoClientManager_h_

#include <map>
#include <set>

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "InfoClient.h"
#include "zTime.h"
#include "zRWLock.h"

/**
 * \brief �����������Ϣ�ɼ����ӵĿͻ��˹�������
 */
class InfoClientManager
{

	public:

		~InfoClientManager();

		/**
		 * \brief ��ȡ���Ψһʵ��
		 * \return ���Ψһʵ������
		 */
		static InfoClientManager &getInstance()
		{
			if (NULL == instance)
				instance = new InfoClientManager();

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
		void add(InfoClient *infoClient);
		void remove(InfoClient *infoClient);
		bool broadcastOne(const void *pstrCmd, int nCmdLen);
		bool sendTo(const DWORD tempid, const void *pstrCmd, int nCmdLen);

	private:

		InfoClientManager();
		static InfoClientManager *instance;

		/**
		 * \brief �ͻ������ӹ����
		 */
		zTCPClientTaskPool *infoClientPool;
		/**
		 * \brief ���ж�����������ʱ���¼
		 */
		zTime actionTimer;

		/**
		 * \brief ��������Ѿ��ɹ���������������
		 */
		typedef std::map<const DWORD, InfoClient *> InfoClient_map;
		typedef InfoClient_map::iterator iter;
		typedef InfoClient_map::const_iterator const_iter;
		typedef InfoClient_map::value_type value_type;
		/**
		 * \brief ��������Ѿ��ɹ�����������
		 */
		InfoClient_map allClients;

		struct lt_client
		{
			bool operator()(InfoClient *s1, InfoClient *s2) const
			{
				return s1->getNetType() < s2->getNetType();
			}
		};
		typedef std::multiset<InfoClient *, lt_client> InfoClient_set;
		InfoClient_set setter;

		/**
		 * \brief �������ʶ�д��
		 */
		zRWLock rwlock;

};

#endif

