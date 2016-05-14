/**
 * \file
 * \version  $Id: MiniServer.h  $
 * \author  
 * \date 
 * \brief zebra��Ŀ�Ʒѷ�����
 *
 */

#ifndef _MINISERVER_H_
#define _MINISERVER_H_

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
class MiniService : public zSubNetService
{

	public:

		bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief ����������
		 *
		 */
		~MiniService()
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
		static MiniService &getInstance()
		{
			if (NULL == instance)
				instance = new MiniService();

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
				static zLogger* miniLogger;		
		 */



	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static MiniService *instance;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 *
		 */
		MiniService() : zSubNetService("С��Ϸ������", MINISERVER)
		{
			taskPool = NULL;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
};

#endif

