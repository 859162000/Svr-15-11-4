/**
 * \file
 * \version  $Id: BillServer.h  $
 * \author  
 * \date 
 * \brief zebra��Ŀ�Ʒѷ�����
 *
 */

#ifndef _BillServer_h_
#define _BillServer_h_

#include "zSubNetService.h"
#include "Zebra.h"
#include "zMisc.h"

class zDBConnPool;
class MetaData;

/**
 * \brief ����Ʒѷ�����
 *
 * �����ʹ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
 *
 */
class BillService : public zSubNetService
{

	public:

		bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief ����������
		 *
		 */
		~BillService()
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
		static BillService &getInstance()
		{
			if (NULL == instance)
				instance = new BillService();

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

		/**
		 * \brief ָ�����ݿ���������ָ��
		 *
		 */
                static MetaData* metaData;		

		/**
		 * \brief ָ������־��ָ��
		 *
		 */
                static zLogger* tradelog;		



	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static BillService *instance;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 *
		 */
		BillService() : zSubNetService("�Ʒѷ�����", BILLSERVER)
		{
			taskPool = NULL;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
};

#endif

