/**
 * \file
 * \version  $Id: InfoServer.h  $
 * \author  
 * \date 
 * \brief ��Ϣ���з�����
 */
 
#ifndef _DBAccessServer_h_
#define _DBAccessServer_h_

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
#include "InfoCommand.h"
#include "InfoTask.h"
#include "InfoServer.h"
/**
 * \brief ����һ���㲥��Ϣ���߳�
 */
// class CmdThread : public zThread
// {
// 	public:
// 	
// 		void run();
// 
// 		/**
// 		 * \brief ���캯��
// 		 */
// 		CmdThread() : zThread(),tm(),tn() {}
// 		
// 		/**
// 		 * \brief ���캯��
// 		 */
// 		~CmdThread() {}
// 		
// 	private:
// 
// 		zTime tm, tn;
// 		static time_t delta;//�㲥��Ϣ��ʱ����(��λ:��)
// 		
// };

/**
 * \brief ����һ����Ϣ���з�������
 */
class DBAccessService : public zNetService
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
		~DBAccessService()
		{
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
		static DBAccessService &getInstance()
		{
			if (NULL == instance)
			{
				instance = new DBAccessService();
			}

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		/**
		 * \brief �ض������ļ�����
		 */
		void reloadConfig();

		/**
		 * \brief ָ�����ݿ����ӳ�ʵ��ָ��
		 */
		static zDBConnPool *dbConnPool;

		/**
		 * \brief ָ����Ϣ�㲥�̵߳�ָ��
		 */
		static CmdThread *pCmdThread;
		
	private:

		/**
		 * \brief ���Ψһʵ��
		 */
		static DBAccessService *instance;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 */
		DBAccessService() : zNetService("DBAccessService")
		{
			taskPool = NULL;
			bindport = 0;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();

		/**
		 * \brief ���������˿�
		 */
		unsigned short bindport;
};
#endif

 
