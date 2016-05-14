/**
 * \file
 * \version  $Id: ScenesServer.h  $
 * \author  
 * \date 
 * \brief zebra��Ŀ��������������Ϸ���󲿷����ݶ��ڱ�ʵ��
 */

#ifndef _ScenesServer_h_
#define _ScenesServer_h_

#include "zSubNetService.h"
#include "Zebra.h"
#include "SceneManager.h"
#include "zMisc.h"

/**
 * \brief ���峡��������
 *
 * ��������������Ϸ���󲿷����ݶ��ڱ�ʵ��<br>
 * �����ʹ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
 *
 */
class ScenesService : public zSubNetService
{

	public:
		int writeBackTimer;
		bool double_exp;
		bool double_drop;
		BYTE levelDoubleDrop;
		bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		BYTE countryPower[13];

		/**
		 * \brief ����������
		 *
		 */
		virtual ~ScenesService()
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
		static ScenesService &getInstance()
		{
			if (NULL == instance)
				instance = new ScenesService();

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
		void checkAndReloadConfig();
		bool isSequeueTerminate() 
		{
			return taskPool == NULL;
		}

		//GM_logger
		static zLogger* gm_logger;
		//��Ʒlog
		static zLogger* objlogger;
		//���_logger
		static zLogger* wg_logger;

		static Cmd::stChannelChatUserCmd * pStampData;
		static DWORD updateStampData();
	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static ScenesService *instance;
		/**
		 * \brief �������¶�ȡ���ñ�־
		 *
		 */
		static bool reload;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 *
		 */
		ScenesService() : zSubNetService("����������", SCENESSERVER)
		{
			writeBackTimer = 0;
			taskPool = NULL;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
};

#endif

