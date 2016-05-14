#ifndef _BILLUSER_H_
#define _BILLUSER_H_
#include "zNoncopyable.h"
#include <iostream>
#include <ext/hash_map>

#include "Zebra.h"
#include "BillTask.h"
#include "SuperCommand.h"
#include "zTime.h"
#include "zEntry.h"
#include "UserCommand.h"
#include "BillCallback.h"

class BillUser :public zEntry 
{
	public:
		enum LoginState
		{
			WAIT_LOGIN,			/**< �ȴ�ͨ����½��֤�Ŀͻ��˵�½���ط����� */
			CONF_LOGIN,			/**< ��½���ط�������֤�Ѿ��ɹ� */
			CONF_LOGOUT,		/**< �ȴ��˳� */
			WAIT_LOGIN_TIMEOUT, /**< �ȴ���½��ʱ */
		}
		state;					/**< �Ự״̬ */ 
	private:
		static const int session_timeout_value = 10;
		//DWORD loginTempID;		    /**< ��½��ʱ��� */


		DWORD gold;		/**< ������� */
		DWORD money;		/**< �������� */
		DWORD all_in_gold;		/**< �ܳ�ֵ������� */
		DWORD all_in_money;		/**< �ܳ�ֵ�������� */
		DWORD all_out_gold;		/**< ����ȡ������� */
		DWORD all_out_money;		/**< ����ȡ�������� */
		DWORD all_tax_gold;		/**< ��˰�� */
		DWORD all_tax_money;	/**< ��˰�� */ 
		DWORD vip_time;		/**< vip����ʱ�� */
		zTime timestamp;		/**< ʱ��� */


		/// ����
		char   tid[Cmd::UserServer::SEQ_MAX_LENGTH+1];                     /// ������ˮ��

		/// ����
		char password[MAX_PASSWORD+1];
		/// �Ƿ��Ѿ���½
		bool stock_login;
		DWORD goldlistNum;	/// ���˹�Ʊ��������
		DWORD moneylistNum;	/// ���˹�Ʊ������
		char 		client_ip[MAX_IP_LENGTH];              //�ͻ�����ip
	public:
		const char *getIp();
		char account[Cmd::UserServer::ID_MAX_LENGTH+1];
		BillTask *gatewaytask;
		BillUser(DWORD acc,DWORD logintemp,const char *count,const char *ip,BillTask *gate);
		void increaseGoldListNum();
		void increaseMoneyListNum();
		void decreaseGoldListNum();
		void decreaseMoneyListNum();


		bool sendCmd(const void *pstrCmd, const int nCmdLen)
		{
			if(gatewaytask)
			{
				return gatewaytask->sendCmd(pstrCmd,nCmdLen);
			}
			return false;
		}
		bool sendCmdToMe(const void *pstrCmd, const int nCmdLen)
		{
			if(gatewaytask)
			{
				return gatewaytask->sendCmdToUser(id,pstrCmd,nCmdLen);
			}
			return false;
		}
		bool sendCmdToScene(const void *pstrCmd, const int nCmdLen)
		{
			if(gatewaytask)
			{
				return gatewaytask->sendCmdToScene(id,pstrCmd,nCmdLen);
			}
			return false;
		}

		bool restoregold();
		bool restorecard();
		static bool redeem_gold_err(const BillData* bd);
		static bool redeem_object_card_err(const BillData* bd);
		bool login(const DWORD loginTempID);
		bool logout(const DWORD loginTempID);
		bool query_point(const BillData* bd);
		bool redeem_gold(const BillData* bd);
		bool redeem_moth_card(const BillData* bd);
		bool redeem_object_card(const BillData* bd);
		bool begin_tid(const char *t);
		bool putList(DWORD num , DWORD price, unsigned char type);
		bool addGold(DWORD num,const char *disc,bool transfer=false , bool tax=false);
		bool addMoney(DWORD num,const char *disc,bool transfer=false , bool tax=false);
		bool removeGold(DWORD num,const char *disc,bool transfer=false , bool tax=false);
		bool removeMoney(DWORD num,const char *disc,bool transfer=false , bool tax=false);
		static DWORD getRealMinTime();
		static bool logger(const char *coin_type,DWORD acc,const char *act,DWORD cur,DWORD change,DWORD type,const char *action);
		void end_tid();
		DWORD loginTimeOut(zTime current);
		bool usermsgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool usermsgParseScene(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
	private:
		bool check_tid(const char *t);
		bool checkStockLogin();
};

#endif
