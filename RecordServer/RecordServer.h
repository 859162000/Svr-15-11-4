/**
 * \file
 * \version  $Id: RecordServer.h  $
 * \author  
 * \date 
 * \brief zebra��Ŀ���������������ڴ���������Ͷ�ȡ����
 *
 */

#ifndef _RecordServer_h_
#define _RecordServer_h_

#include "zSubNetService.h"
#include "Zebra.h"
#include "zMisc.h"
#include "zDBConnPool.h"

/**
 * \brief ���嵵��������
 *
 * ��Ŀ���������������ڴ���������Ͷ�ȡ����<br>
 * �����ʹ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
 *
 */
class RecordService : public zSubNetService
{

	public:

		bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief ����������
		 *
		 */
		~RecordService()
		{
			instance = NULL;

			//�ر��̳߳�
			if (taskPool)
			{
				taskPool->final();
				SAFE_DELETE(taskPool);
			}
		}

		const int getPoolSize() const
		{
			if(taskPool)
			{
				return taskPool->getSize();
			}
			else
			{
				return 0;
			}
		}

		/**
		 * \brief ����Ψһ����ʵ��
		 *
		 * \return Ψһ����ʵ��
		 */
		static RecordService &getInstance()
		{
			if (NULL == instance)
				instance = new RecordService();

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

		void reloadConfig();

		/**
		 * \brief ָ�����ݿ����ӳ�ʵ����ָ��
		 *
		 */
		static zDBConnPool *dbConnPool;

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static RecordService *instance;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 *
		 */
		RecordService() : zSubNetService("����������", RECORDSERVER)
		{
			taskPool = NULL;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
};

#endif

