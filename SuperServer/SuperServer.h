/**
 * \file
 * \version  $Id: SuperServer.h  $
 * \author  
 * \date 
 * \brief ʵ�ַ�����������
 *
 * ��һ�����е����з��������й���
 * 
 */

#ifndef _SuperServer_h_
#define _SuperServer_h_

#include <iostream>
#include <string>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zNetService.h"
#include "ServerTask.h"
#include "zDBConnPool.h"
#include "zMisc.h"

/**
 * \brief ��������������
 *
 * �����˻���<code>zNetService</code>
 *
 */
class SuperService : public zNetService
{

	public:

		/**
		 * \brief ��������
		 *
		 * �麯��
		 *
		 */
		~SuperService()
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
			if (taskPool)
				return taskPool->getSize();
			else
				return 0;
		}

		/**
		 * \brief ��ȡ���Ψһʵ��
		 *
		 * ʹ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
		 *
		 * \return ���Ψһʵ��
		 */
		static SuperService &getInstance()
		{
			if (NULL == instance)
				instance = new SuperService();

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
		 * \brief ��ȡ��Ϸ�����
		 * \return ������Ϸ�����
		 */
		const GameZone_t &getZoneID() const
		{
			return gameZone;
		}

		/**
		 * \brief ������Ϸ�����
		 * \param gameZone ��Ϸ�����
		 */
		void setZoneID(const GameZone_t &gameZone)
		{
			this->gameZone = gameZone;
		}

		/**
		 * \brief ��ȡ��Ϸ������
		 * \return ������Ϸ������
		 */
		const std::string &getZoneName() const
		{
			return zoneName;
		}

		/**
		 * \brief ������Ϸ������
		 * \param zoneName �����õ�����
		 */
		void setZoneName(const char *zoneName)
		{
			this->zoneName = zoneName;
		}

		/**
		 * \brief ��ȡ���������
		 * \return ���������
		 */
		const WORD getID() const
		{
			return wdServerID;
		}

		/**
		 * \brief ��ȡ����������
		 * \return ����������
		 */
		const WORD getType() const
		{
			return wdServerType;
		}

		/**
		 * \brief ��ȡip��ַ
		 * \return ����ip��ַ
		 */
		const char *getIP() const
		{
			return pstrIP;
		}

		/**
		 * \brief ��ȡ�˿�
		 * \return ���ض˿�
		 */
		const WORD getPort() const
		{
			return wdPort;
		}

		/**
		 * \brief ָ�����ݿ����ӳ�ʵ����ָ��
		 *
		 */
		static zDBConnPool *dbConnPool;

	private:

		/**
		 * \brief ��Ϸ�����
		 */
		GameZone_t gameZone;
		/**
		 * \brief ��Ϸ������
		 */
		std::string zoneName;

		WORD wdServerID;					/**< ��������ţ�һ����Ψһ�� */
		WORD wdServerType;					/**< ���������ͣ�������ʵ����ʱ���Ѿ�ȷ�� */
		char pstrName[MAX_NAMESIZE];		/**< ���������� */
		char pstrIP[MAX_IP_LENGTH];			/**< ������������ַ */
		WORD wdPort;						/**< �����������˿ڣ�Ҳ���ǰ�˿� */
		char pstrExtIP[MAX_IP_LENGTH];		/**< ������������ַ��Ҳ���Ƿ���ǽ��ַ */
		WORD wdExtPort;						/**< �����������˿ڣ�Ҳ����ӳ�䵽����ǽ�Ķ˿� */

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static SuperService *instance;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 *
		 */
		SuperService() : zNetService("������������")
		{
			wdServerID = 1;
			wdServerType = SUPERSERVER;
			bzero(pstrName, sizeof(pstrName));
			bzero(pstrIP, sizeof(pstrIP));
			wdPort = 0;
			bzero(pstrExtIP, sizeof(pstrExtIP));
			wdExtPort = 0;
			taskPool = NULL;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();

		bool getServerInfo();

};


#endif

