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

		bool uniqueAdd(ServerTask *task);
		bool uniqueRemove(ServerTask *task);
		bool broadcast(const GameZone_t &gameZone, const void *pstrCmd, int nCmdLen);

	private:

		/**
		 * \brief ���캯��
		 *
		 */
		ServerManager() {};

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static ServerManager *instance;

		/**
		 * \brief hash����
		 *
		 */
		struct GameZone_hash
		{
			size_t operator()(const GameZone_t &gameZone) const
			{
				__gnu_cxx::hash<DWORD> H;
				return H(gameZone.id);
			}
		};
		/**
		 * \brief �����˷�������Ψһ����֤��������
		 * 
		 **/
		typedef __gnu_cxx::hash_map<const GameZone_t, ServerTask *, GameZone_hash> ServerTaskContainer;
		/**
		 * \brief ���������ĵ���������
		 *
		 */
		typedef ServerTaskContainer::iterator ServerTaskContainer_iterator;
		/**
		 * \brief �����������ĳ�������������
		 *
		 */
		typedef ServerTaskContainer::const_iterator ServerTaskContainer_const_iterator;
		/**
		 * \brief �����������ļ�ֵ������
		 *
		 */
		typedef ServerTaskContainer::value_type ServerTaskContainer_value_type;
		/**
		 * \brief �������ʵĻ������
		 *
		 */
		zMutex mlock;
		/**
		 * \brief Ψһ������ʵ��
		 *
		 */
		ServerTaskContainer taskUniqueContainer;

};

#endif

