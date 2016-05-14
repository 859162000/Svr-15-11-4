/**
 * \file
 * \version  $Id: BillSessionManager.h  $
 * \author  
 * \date 
 * \brief ���������ڱ����Ҷһ�����Ϣ
 *
 * 
 */


#ifndef _MONEYSESSIONMANAGER_H_
#define _MONEYSESSIONMANAGER_H_

#include <map>

#include "zType.h"
#include "zMutex.h"
#include "zTime.h"
#include "Command.h"
#include "UserCommand.h"
#include "BillCommand.h"
#include <string>

class BillTask;

/**
 * \brief ��ɫ������ȡд��ĻỰ��¼
 *
 */
struct BillSession
{
	DWORD  accid;			/// �ʺű��
	DWORD  charid;			/// ��ɫ���
	char   tid[Cmd::UserServer::SEQ_MAX_LENGTH+1];                     /// ������ˮ��
	char   account[Cmd::UserServer::ID_MAX_LENGTH+1];     /// �ʺ�(��Ҫ���͸��Ƿѷ�����)
	char   name[MAX_NAMESIZE+1];     /// ��ɫ����
	DWORD point;                    /// �۷ѵ���


	/**
	 * \brief ȱʡ���캯��
	 *
	 */
	/*
	BillSession(const char* tid, const Cmd::Bill::t_Change_Gold_GateMoney* cmd, BillTask* task = NULL)
	{
		strncpy(this->tid, tid, Cmd::UserServer::SEQ_MAX_LENGTH);
		strncpy(this->account, cmd->account, Cmd::UserServer::ID_MAX_LENGTH);
		this->accid = cmd->accid;
		this->charid = cmd->charid;
		this->point = cmd->point;
		this->task = task;
//		strncpy(this->name, cmd->name, MAX_NAMESIZE);
	}
	// */
	
	BillSession()
	{
		accid = 0;
		charid = 0;
		tid[0] = '\0';
		account[0]  = '\0';
		name[0] = '\0';
		point = 0;
	}

};

class BillSessionManager
{

	public:

		/**
		 * \brief Ĭ����������
		 *
		 */
		~BillSessionManager()
		{
			sessionMap.clear();
		}

		/**
		 * \brief �������Ψһʵ��
		 *
		 * ʵ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
		 *
		 */
		static BillSessionManager &getInstance()
		{
			if (NULL == instance)
				instance = new BillSessionManager;

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

		bool add(BillSession &bs);
//		bool verify(const DWORD accid, const DWORD id, const WORD wdServerID);
		bool remove(const std::string& tid);
//	BillSession find(const std::string& tid);
		BillSession get(const std::string& tid);
//		void removeAllByServerID(const WORD wdServerID);

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static BillSessionManager *instance;

		/**
		 * \brief Ĭ�Ϲ��캯��
		 *
		 */
		BillSessionManager() {};

		/**
		 * \brief ������������
		 *
		 */
		typedef std::map<std::string, BillSession> BillSessionHashmap;
		/**
		 * \brief ������������������
		 *
		 */
		typedef BillSessionHashmap::iterator BillSessionHashmap_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef BillSessionHashmap::value_type BillSessionHashmap_pair;
		
		/**
		 * \brief �洢�����ʺ��б���Ϣ������
		 *
		 */
		BillSessionHashmap sessionMap;
		
		/**
		 * \brief �������
		 *
		 */
		zMutex mlock;
};

#endif

