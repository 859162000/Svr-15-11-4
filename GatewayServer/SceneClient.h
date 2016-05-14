/**
 * \file
 * \version  $Id: SceneClient.h  $
 * \author 
 * \date 
 * \brief ���峡�����������ӿͻ���
 * 
 */

#ifndef _SceneClient_h_
#define _SceneClient_h_

#include <unistd.h>
#include <iostream>
#include <vector>

#include "zTCPClient.h"
#include "SceneCommand.h"
#include "zMutex.h"
#include "SuperCommand.h"
#include "ScreenIndex.h"
#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "SceneClientManager.h"

/**
 * \brief ���峡�����������ӿͻ�����
 **/
class SceneClient : public zTCPClientTask
{

	public:

		SceneClient(
				const std::string &ip,
				const unsigned short port);
		SceneClient( const std::string &name,const Cmd::Super::ServerEntry *serverEntry)
			: zTCPClientTask(serverEntry->pstrExtIP, serverEntry->wdExtPort)
			{
				wdServerID=serverEntry->wdServerID;
			};
		~SceneClient()
		{
			Zebra::logger->debug("SceneClient����");
		}

		int checkRebound();
		void addToContainer();
		void removeFromContainer();
		bool connectToSceneServer();
		bool connect();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		void freshIndex(GateUser *pUser , const DWORD map , const DWORD screen)
		{
			MapIndexIter iter = mapIndex.find(map); 
			if(iter != mapIndex.end())
			{
				iter->second->refresh(pUser , screen);
			}
		}
		void removeIndex(GateUser *pUser , const DWORD map)
		{
			MapIndexIter iter = mapIndex.find(map); 
			if(iter != mapIndex.end())
			{
				iter->second->removeGateUser(pUser);
			}
		}
		const WORD getServerID() const
		{
			return wdServerID;
		}

	private:

		/**
		 * \brief ����������
		 *
		 *
		 * \param 
		 * \return 
		 */
		MapIndex mapIndex;
		/**
		 * \brief ���������
		 *
		 */
		WORD wdServerID;



};
#if 0
/**
 * \brief ���峡�����������ӿͻ�����
 *
 */
class SceneClient : public zTCPBufferClient
{

	public:
		
		/**
		 * \brief ���캯��
		 *
		 * \param name ����
		 * \param serverEntry ������������Ϣ
		 */
		SceneClient( const std::string &name,const Cmd::Super::ServerEntry *serverEntry)
			: zTCPBufferClient(name, serverEntry->pstrExtIP, serverEntry->wdExtPort, false, 8000)
			{
				wdServerID=serverEntry->wdServerID;
			};

		~SceneClient()
		{
			Zebra::logger->debug("SceneClient����");
		}
		bool connectToSceneServer();
		void run();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief ��ȡ�����������ı��
		 *
		 * \return �������������
		 */
		const WORD getServerID() const
		{
			return wdServerID;
		}
		void freshIndex(GateUser *pUser , const DWORD map , const DWORD screen)
		{
			MapIndexIter iter = mapIndex.find(map); 
			if(iter != mapIndex.end())
			{
				iter->second->refresh(pUser , screen);
			}
		}
		void removeIndex(GateUser *pUser , const DWORD map)
		{
			MapIndexIter iter = mapIndex.find(map); 
			if(iter != mapIndex.end())
			{
				iter->second->removeGateUser(pUser);
			}
		}

	private:

		/**
		 * \brief ����������
		 *
		 *
		 * \param 
		 * \return 
		 */
		MapIndex mapIndex;
		/**
		 * \brief ���������
		 *
		 */
		WORD wdServerID;

};

/**
 * \brief �������������ӹ�����
 *
 */
class SceneClientManager
{

	public:

		/**
		 * \brief ��������
		 *
		 */
		~SceneClientManager() {};

		/**
		 * \brief ��ȡ��������Ψһʵ��
		 *
		 * \return ������Ψһʵ��
		 */
		static SceneClientManager &getInstance()
		{
			if (NULL == instance)
				instance = new SceneClientManager();

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

		void add(SceneClient *client);
		void remove(SceneClient *client);
		SceneClient *getByServerID(WORD serverid);
		void final();

	private:

		/**
		 * \brief �������������ӹ�����Ψһʵ��ָ��
		 *
		 */
		static SceneClientManager *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		SceneClientManager() {};

		/**
		 * \brief �������ʻ������
		 *
		 */
		zMutex mlock;
		/**
		 * \brief ������������������
		 *
		 */
		std::vector<SceneClient *> sceneClients;

};

#endif
#endif

