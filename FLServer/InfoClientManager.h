/**
 * \file
 * \version  $Id: InfoClientManager.h $
 * \author  
 * \date 
 * \brief �����������Ϣ�ɼ����ӵĿͻ��˹�������
 */


#ifndef _InfoClientManager_h_
#define _InfoClientManager_h_

#include <vector>

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "InfoClient.h"
#include "zTime.h"

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
		bool broadcast(const void *pstrCmd, int nCmdLen);

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
		typedef std::vector<InfoClient *> InfoClientContainer;
		/**
		 * \brief ��������Ѿ��ɹ�����������
		 */
		InfoClientContainer allClients;
		/**
		 * \brief �������ʻ������
		 */
		zMutex mlock;

};

#endif

