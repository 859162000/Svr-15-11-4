/**
 * \file
 * \version  $Id: roleRegServer.h  $
 * \author  
 * \date
 * \brief ��ɫΨһ����֤������
 */

#ifndef _roleRegService_h_
#define _roleRegService_h_

#include <iostream>
#include <string>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zTCPTaskPool.h"
#include "zNetService.h"
#include "zMisc.h"
#include "RoleTask.h"

/**
 *	\brief �ͻ��˷��ʷ�������
 */
class roleRegService : public zNetService
{
	public:
		
		const int getPoolSize() const
		{
			return taskPool->getSize();
		}

		const int getPoolState() const
		{
			return taskPool->getState();
		}

		/**
		 * \brief ��������
		 */
		~roleRegService()
		{
			instance = NULL;

			//�ر��̳߳�
			if (taskPool)
			{
				taskPool->final();
				SAFE_DELETE(taskPool);
			}
		}
		
		/**
		 * \brief ��ȡ���Ψһʵ��
		 *	ʹ����singleton���ģʽ,��֤�˽�����ֻ��һ��ʵ��
		 * \return ���Ψһʵ��
		 */
		static roleRegService &getInstance()
		{
			if (instance == NULL)
				instance = new roleRegService();
				
			return *instance;	
		}
		
		/**
		 * \brief �ͷ����Ψһʵ��
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void reloadConfig();
		
		/**
		 * \brief ָ�����ݿ����ӳص�ʵ��ָ��
		 */
		 static zDBConnPool *dbConnPool;
		
	private:
		
		/**
		 * \brief ���Ψһʵ��
		 */
		static roleRegService *instance;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 */
		roleRegService() : zNetService("roleRegService")
		{
			taskPool = NULL;
			client_port = 0 ;
		}
		
		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
		
		unsigned short client_port ;
};

#endif
