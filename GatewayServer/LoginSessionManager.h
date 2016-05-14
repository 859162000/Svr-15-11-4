/**
 * \file
 * \version  $Id: LoginSessionManager.h  $
 * \author  
 * \date 
 * \brief �����½�Ự����֮ǰ����Ϣ
 *
 * 
 */


#ifndef _LoginSessionManager_h_
#define _LoginSessionManager_h_

#include <ext/hash_map>

#include "zMutex.h"
#include "SuperCommand.h"
#include "Zebra.h"
#include "zTime.h"

#include "EncDec/EncDec.h"

/**
 * \brief ��½�Ự������
 *
 */
class LoginSessionManager
{

	public:

		/**
		 * \brief ��������
		 *
		 */
		~LoginSessionManager() {};

		/**
		 * \brief ��ȡ��½�Ự��������Ψһʵ��
		 *
		 * \return �Ự������Ψһʵ��
		 */
		static LoginSessionManager &getInstance()
		{
			if (NULL == instance)
				instance = new LoginSessionManager();

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

		void put(const t_NewLoginSession &session);
		bool verify(const DWORD loginTempID, const DWORD accid,char *numPassword, DES_cblock *key=0);
		void update(const zRTime &ct);

	private:

		/**
		 * \brief �Ự��������Ψһʵ��ָ��
		 *
		 */
		static LoginSessionManager *instance;

		/**
		 * \brief ���캯��
		 *
		 */
		LoginSessionManager() : lastUpdateTime() {};

		/**
		 * \brief ���һ�θ���ʱ��
		 *
		 */
		zRTime lastUpdateTime;

		/**
		 * \brief �����¼��
		 *
		 */
		bool checkUpdateTime(const zRTime &ct)
		{
			bool retval = false;
			if (ct >= lastUpdateTime)
			{
				lastUpdateTime = ct;
				lastUpdateTime.addDelay(1000);
				retval = true;
			}
			return retval;
		}

		struct LoginSession
		{
			t_NewLoginSession session;
			zTime timestamp;

			LoginSession(const t_NewLoginSession &session) : session(session), timestamp()
			{
			}
			LoginSession(const LoginSession& ls)
			{
				session = ls.session;
				timestamp = ls.timestamp;
			}
			LoginSession & operator= (const LoginSession &ls)
			{
				session = ls.session;
				timestamp = ls.timestamp;
				return *this;
			}
		};
		/**
		 * \brief ������������
		 *
		 */
		typedef __gnu_cxx::hash_map<DWORD, LoginSession> LoginSessionHashmap;
		/**
		 * \brief �������������
		 *
		 */
		typedef LoginSessionHashmap::iterator LoginSessionHashmap_iterator;
		/**
		 * \brief �����ֵ������
		 *
		 */
		typedef LoginSessionHashmap::value_type LoginSessionHashmap_pair;
		/**
		 * \brief �Ự����
		 *
		 */
		LoginSessionHashmap sessionData;
		/**
		 * \brief �������ʻ������
		 *
		 */
		zMutex mlock;

};

#endif

