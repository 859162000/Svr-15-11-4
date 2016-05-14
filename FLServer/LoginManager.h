/**
 * \file
 * \version  $Id: LoginManager.h  $
 * \author  
 * \date 
 * \brief ��½���ӹ�������
 *
 * 
 */


#ifndef _LoginManager_h_
#define _LoginManager_h_

#include <ext/hash_map>

#include "zEntry.h"
#include "LoginTask.h"
#include "DBAccessCommand.h"

/**
 * \brief ��½���ӹ�������
 *
 * �������еĵ�½���ӵ������������������
 *
 */
class LoginManager
{

	public:

		/**
		 ** \brief ������������û���Ŀ
		 **
		 **/
		static DWORD maxGatewayUser;

		/**
		 * \brief ����ص�������
		 *
		 */
		typedef zEntryCallback<LoginTask, void> LoginTaskCallback;

		/**
		 * \brief ��������
		 *
		 */
		~LoginManager() {};

		/**
		 * \brief ��ȡ����������Ψһʵ��
		 *
		 * ����ʵ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
		 */
		static LoginManager &getInstance()
		{
			if (NULL == instance)
				instance = new LoginManager();

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

		bool add(LoginTask *task);
		void remove(LoginTask *task);
		bool broadcast(const DWORD loginTempID, const void *pstrCmd, int nCmdLen);
		void verifyReturn(const DWORD loginTempID, const BYTE retcode, const t_NewLoginSession &session);
		void loginReturn(const DWORD loginTempID, const BYTE retcode, const bool tm = true);
		void execAll(LoginTaskCallback &cb);

	private:

		/**
		 * \brief ���캯��
		 *
		 */
		LoginManager(){};

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static LoginManager *instance;
		/**
		 * \brief ������������
		 *
		 */
		typedef __gnu_cxx::hash_map<DWORD, LoginTask *> LoginTaskHashmap;
		/**
		 * \brief ������������������
		 *
		 */
		typedef LoginTaskHashmap::iterator LoginTaskHashmap_iterator;
		/**
		 * \brief ����������������������
		 *
		 */
		typedef LoginTaskHashmap::const_iterator LoginTaskHashmap_const_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef LoginTaskHashmap::value_type LoginTaskHashmap_pair;
		/**
		 * \brief �����������֤ԭ�ӷ�������
		 *
		 */
		zMutex mlock;
		/**
		 * \brief �����ӹ�����������
		 *
		 */
		LoginTaskHashmap loginTaskSet;

};

#endif

