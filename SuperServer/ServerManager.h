/**
 * \file
 * \version  $Id: ServerManager.h  $
 * \author  
 * \date 
 * \brief �����������������
 *
 * �����������ȫ��������Ψһ����֤����
 * 
 */


#ifndef _ServerManager_h_
#define _ServerManager_h_

#include <iostream>
#include <list>
#include <ext/hash_map>

#include "ServerTask.h"
#include "zMutex.h"
#include "zNoncopyable.h"

/**
 * \brief ����������������
 *
 * �����������ȫ��������Ψһ����֤����
 *
 */
class ServerManager : zNoncopyable
{

	public:

		/**
		 * \brief ȱʡ��������
		 *
		 */
		~ServerManager() {};

		/**
		 * \brief ��ȡ���Ψһʵ��
		 *
		 * �����ʹ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
		 *
		 */
		static ServerManager &getInstance()
		{
			if (NULL == instance)
				instance = new ServerManager();

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

		void addServer(ServerTask *task);
		void removeServer(ServerTask *task);
		ServerTask *getServer(WORD wdServerID);
		bool uniqueAdd(ServerTask *task);
		bool uniqueVerify(const WORD wdServerID);
		bool uniqueRemove(ServerTask *task);
		bool broadcast(const void *pstrCmd, int nCmdLen);
		bool broadcastByID(const WORD wdServerID, const void *pstrCmd, int nCmdLen);
		bool broadcastByType(const WORD wdType, const void *pstrCmd, int nCmdLen);
		const DWORD caculateOnlineNum();
		void responseOther(const WORD srcID, const WORD wdServerID);

	private:

		/**
		 * \brief �����������������
		 *
		 */
		typedef std::list<ServerTask *> Container;
		/**
		 * \brief ����������������͵ĵ�����
		 *
		 */
		typedef Container::iterator Container_iterator;
		/**
		 * \brief ����������������͵ĳ���������
		 *
		 */
		typedef Container::const_iterator Containter_const_iterator;
		/**
		 * \brief �����˷�������Ψһ����֤��������
		 * 
		 **/
		typedef __gnu_cxx::hash_map<WORD, ServerTask *> ServerTaskHashmap;
		/**
		 * \brief ���������ĵ���������
		 *
		 */
		typedef ServerTaskHashmap::iterator ServerTaskHashmap_iterator;
		/**
		 * \brief �����������ĳ�������������
		 *
		 */
		typedef ServerTaskHashmap::const_iterator ServerTaskHashmap_const_iterator;
		/**
		 * \brief �����������ļ�ֵ������
		 *
		 */
		typedef ServerTaskHashmap::value_type ServerTaskHashmap_pair;
		/**
		 * \brief �������ʵĻ������
		 *
		 */
		zMutex mutex;
		/**
		 * \brief ������ȫ��������ʵ��
		 *
		 */
		Container container;
		/**
		 * \brief Ψһ������ʵ��
		 *
		 */
		ServerTaskHashmap taskUniqueContainer;

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static ServerManager *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		ServerManager() {};

};

#endif

