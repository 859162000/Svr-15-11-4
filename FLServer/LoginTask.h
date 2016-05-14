/**
 * \file
 * \version  $Id: LoginTask.h  $
 * \author  
 * \date 
 * \brief �����½��������
 *
 */

#ifndef _LoginTask_h_
#define _LoginTask_h_

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "zDBConnPool.h"
#include "Command.h"
#include "zTCPTask.h"
#include "zTime.h"

using namespace Cmd;//add by Victor

/**
 * \brief ��������������
 *
 */
class LoginTask : public zTCPTask
{

	public:

		LoginTask( zTCPTaskPool *pool, const int sock);
		/**
		 * \brief ����������
		 *
		 */
		~LoginTask() {};

		int verifyConn();
		int recycleConn();
		void addToContainer();
		bool uniqueAdd();
		bool uniqueRemove();
		bool msgParse(const Cmd::t_NullCmd *, const unsigned int);

		void genTempID()
		{
			tempid = ++uniqueID;
		}

		const DWORD getTempID() const
		{
			return tempid;
		}

		/**
		 * \brief ��½���󣬷��ش�����뵽�ͻ�����ʾ
		 *
		 * \param retcode �������
		 * \param tm �Ƿ�Ͽ�����
		 */
		void LoginReturn(const BYTE retcode, const bool tm = true)
		{
			using namespace Cmd;
			stServerReturnLoginFailedCmd tCmd;
			
			tCmd.byReturnCode = retcode;
			sendCmd(&tCmd, sizeof(tCmd));

			//���ڵ�½������Ҫ�Ͽ�����
			//whj ���ܵ���coredown,���β���
			if (tm) Terminate();
		}

		/**
		 * \brief �жϵ�½�����Ƿ����
		 * �����½����̫������½������Ӧ�������Ͽ�����
		 * \param ct ��ǰʱ��
		 * \return ��½ʱ���Ƿ����
		 */
		bool timeout(const zTime &ct)
		{
			if (lifeTime.elapse(ct) >= 600)
				return true;
			else
				return false;
		}

	private:

		/**
		 * \brief У��ͻ��˰汾��
		 */
		DWORD verify_client_version;
		
		/**
		 * \brief ������ʱ��
		 */
		zTime lifeTime;
		/**
		 * \brief ��ʱΨһ���
		 *
		 */
		DWORD tempid;
		/**
		 * \brief ��ʱΨһ��ŷ�����
		 *
		 */
		static DWORD uniqueID;
		/**
		 * \brief ��֤��
		 *
		 */
		char jpegPassport[5];

		bool requestLogin(const Cmd::stUserRequestLoginCmd *ptCmd);

};

#endif

