/**
 * \file
 * \version  $Id: FLServer.h  $
 * \author  
 * \date 
 * \brief zebra��Ŀ��½�������������½�������ʺš������ȹ���
 *
 */

#ifndef _FLServer_h_
#define _FLServer_h_

#include "Zebra.h"
#include "zMisc.h"
#include "zMNetService.h"
#include "zTCPTaskPool.h"
#include "zDBConnPool.h"

/**
 * \brief �����½������
 *
 * ��½���񣬸����½�������ʺš������ȹ���<br>
 * �����ʹ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
 *
 */
class FLService : public zMNetService
{

	public:

		/**
		 * \brief ����������
		 *
		 */
		~FLService()
		{
			instance = NULL;

			if (loginTaskPool)
			{
				loginTaskPool->final();
				SAFE_DELETE(loginTaskPool);
			}
			
			if (serverTaskPool)
			{
				serverTaskPool->final();
				SAFE_DELETE(serverTaskPool);
			}
			
			if (pingTaskPool)
			{
				pingTaskPool->final();
				SAFE_DELETE(pingTaskPool);
			}
		}

		/**
		 * \brief ����Ψһ����ʵ��
		 *
		 * \return Ψһ����ʵ��
		 */
		static FLService &getInstance()
		{
			if (NULL == instance)
				instance = new FLService();

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

		/**
		 * \brief ��ȡ���ӳ��е�������
		 * \return ������
		 */
		const int getPoolSize() const
		{
			return loginTaskPool->getSize();
		}

		/**
		 * \brief ��ȡ����������
		 * \return ����������
		 */
		const WORD getType() const
		{
			return LOGINSERVER;
		}

		void reloadConfig();

		bool jpeg_passport;
		
		static zDBConnPool *dbConnPool;

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static FLService *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		FLService() : zMNetService("��½������")
		{
			jpeg_passport = false;
			login_port = 0;
			inside_port = 0;
			ping_port = 0;
			loginTaskPool = NULL;
			serverTaskPool = NULL;
			pingTaskPool = NULL;
		}

		bool init();
		void newTCPTask(const int sock, const unsigned short srcPort);
		void final();

		unsigned short login_port;
		unsigned short inside_port;
		unsigned short ping_port;

		zTCPTaskPool* loginTaskPool;
		zTCPTaskPool* serverTaskPool;
		zTCPTaskPool* pingTaskPool;
};

#endif

