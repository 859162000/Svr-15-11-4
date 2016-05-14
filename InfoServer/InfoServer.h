/**
 * \file
 * \version  $Id: InfoServer.h  $
 * \author  
 * \date 
 * \brief ��Ϣ���з�����
 */
 
#ifndef _InfoServer_h_
#define _InfoServer_h_

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

/**
 * \brief ����һ���㲥��Ϣ���߳�
 */
class CmdThread : public zThread
{
	public:
	
		void run();

		/**
		 * \brief ���캯��
		 */
		CmdThread() : zThread(),tm(),tn() {}
		
		/**
		 * \brief ���캯��
		 */
		~CmdThread() {}
		
	private:

		zTime tm, tn;
		static time_t delta;//�㲥��Ϣ��ʱ����(��λ:��)
		
};

/**
 * \brief ����һ����Ϣ���з�������
 */
class InfoService : public zNetService
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
		~InfoService()
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
		static InfoService &getInstance()
		{
			if (NULL == instance)
			{
				instance = new InfoService();
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
		static InfoService *instance;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 */
		InfoService() : zNetService("InfoService")
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
#include "zConfile.h"
class InfoConfile : public zConfile
{
	public:
		InfoConfile(const char *confile = "configInfo.xml");

	private:
		bool parseYour(const xmlNodePtr node);
};


#endif

 
