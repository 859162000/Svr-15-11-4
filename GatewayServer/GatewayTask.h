/**
 * \file
 * \version  $Id: GatewayTask.h  $
 * \author 
 * \date 
 * \brief ����������������
 *
 */

#ifndef _GatewayTask_h_
#define _GatewayTask_h_

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "GateUserManager.h"
#include "LoginSessionManager.h"
#include "BillClient.h"
#include "zTime.h"

/**
 * \brief ��������������
 *
 */
class GatewayTask : public zTCPTask
{
	public:

		GatewayTask(zTCPTaskPool *pool, const int sock, const struct sockaddr_in *addr = NULL);
		~GatewayTask();

		int verifyConn();
		int waitSync();
		int recycleConn();
		void Terminate(const TerminateMethod method = terminate_passive);
		void addToContainer();
		void removeFromContainer();
		bool uniqueAdd();
		bool uniqueRemove();
		bool msgParse(const Cmd::t_NullCmd *ptNull, const unsigned int nCmdLen);
		bool checkTime(const zRTime &ct);

		/**
		 * \brief �õ�����ҵ��ʺ�id
		 *
		 * \return �õ���id
		 */
		DWORD getACCID() const
		{
			return accid;
		}

		/**
		 * \brief ���ø��ʺ��Ƿ���֤ͨ��
		 *
		 *
		 * \param ok �Ƿ�ͨ��
		 */
		void accountVerifyOK(const bool ok)
		{
			if (ok)
				accountVerified = ACCOUNTVERIFY_SUCCESS;
			else
				accountVerified = ACCOUNTVERIFY_FAILURE;
		}

		/**
		 * \brief �����Ƿ�δvip�û�
		 *
		 */
		void setVip(bool vip)
		{
			vip_user = vip;
		}

		/**
		 * \brief �Ƿ���vip�û�
		 *
		 */
		bool isVip()
		{
			return vip_user;
		}

	private:
		///��������
		char numPassword[MAX_NUMPASSWORD];
		/// ��������DWORD��
		DWORD numPwd;
		///vip�û�
		bool vip_user;
		///ʱ��У�Զ�ʱ��
		Timer _retset_gametime;
		///������ʱ�ȵ�(����)
		DWORD recycle_wait;
		///У��ͻ���ʱ��ļ��
		static const unsigned int sampleInterval = 20000;
		static const unsigned int sampleInterval_sec = sampleInterval/1000;
		static const unsigned int sampleInterval_error_sec = sampleInterval/1000;
		static const unsigned int sampleInterval_error_msecs = sampleInterval;

		///������Ϣת���ļ��
		static const unsigned int chatInterval = 1000;
		///�´������ʱ��
		zRTime nextChatTime;
		///�´ι��������ʱ��
		zRTime nextCountryChatTime;

		///�ͻ�����sampleIntervalʱ���ڷ��ͳ���maxSamplePPS�����ݰ����ж�Ϊʹ�������
		static const unsigned int maxSamplePPS = 145;
		///�ϴ����v_samplePackets��ʱ��
		DWORD v_lastSampleTime;
		///ͳ�����ݰ�����
		unsigned int v_samplePackets;

		///��task��ʼ����ʱ��
		zRTime initTime;
		///�ϴμ��ͻ����Ƿ��Ѿ�У����ʱ���ʱ��
		zRTime lastCheckTime;
		///�Ƿ��Ѿ�У����ʱ��
		volatile bool haveCheckTime;

		friend class GateUser;
		///ʹ�ø����ӵ����
		GateUser *pUser;

		///��Ϣ��鹤��
		//CheckerTable checker;
		/**
		 * \brief �˺ű��
		 *
		 */
		DWORD accid;

		/**     
		 ** \brief ��Ϸʱ��
		 **
		 **/
		QWORD qwGameTime;
		zRTime GameTimeSyn;
		QWORD dwTimestampServer;

		/**
		  * \brief �û��ʺ�
		  *
		  *
		  */
		char account[MAX_ACCNAMESIZE+1];

		///��¼ʱ�������ʱid
		DWORD loginTempID;
		///�Ƿ���֤�˰汾
		bool versionVerified;
		///�ʺ���֤��״̬
		enum
		{
			ACCOUNTVERIFY_NONE,
			ACCOUNTVERIFY_WAITING,
			ACCOUNTVERIFY_SUCCESS,
			ACCOUNTVERIFY_FAILURE
		}accountVerified;

		bool verifyVersion(const Cmd::stNullUserCmd *ptNullCmd);
		bool verifyACCID(const Cmd::stNullUserCmd *ptNullCmd);
		bool forwardScene(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
		bool forwardSceneBill(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
		bool forwardBill(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
		bool forwardBillScene(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
		bool forwardSession(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
		bool forwardMini(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
		bool msgParse_Select(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
		bool msgParse_Time(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
		bool checkUserCmd(const Cmd::stNullUserCmd *pCmd, const zRTime &ct);

		bool checkNewName(char *);
};

#endif

