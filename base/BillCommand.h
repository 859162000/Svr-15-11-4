/**
 * \file
 * \version  $Id: BillCommand.h  $
 * \author  
 * \date 
 * \brief ����Ʒѷ��������ָ��
 *
 */


#ifndef _BILLCOMMAND_H_
#define _BILLCOMMAND_H_

#include "zNullCmd.h"
#include "Command.h"
#include "UserCommand.h"
#pragma pack(1)

namespace Cmd
{

	namespace Bill
	{
		const BYTE CMD_LOGIN = 1;
		const BYTE CMD_GATE = 2;
		const BYTE CMD_FORWARD = 3;
		const BYTE CMD_REDEEM = 4;
		const BYTE CMD_STOCK = 5;


		//////////////////////////////////////////////////////////////
		/// ��½�Ʒѷ�����ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginBill : t_NullCmd
		{
			WORD wdServerID;
			WORD wdServerType;
			t_LoginBill()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN) {};
		};
		//////////////////////////////////////////////////////////////
		/// ��½�Ʒѷ�����ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// �Ʒѷ����������ط�����������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_GATE_NEWSESSION = 1;
		struct t_NewSession_Gateway : t_NullCmd
		{
			t_NewLoginSession session;

			t_NewSession_Gateway()
				: t_NullCmd(CMD_GATE, PARA_GATE_NEWSESSION) {};
		};

		const BYTE PARA_GATE_LOGINVERIFY = 2;
		struct t_LoginVerify_Gateway : t_NullCmd
		{
			DWORD accid;
			DWORD loginTempID;

			t_LoginVerify_Gateway()
				: t_NullCmd(CMD_GATE, PARA_GATE_LOGINVERIFY) {};
		};

		const BYTE PARA_GATE_LOGINVERIFY_RETURN = 3;
		struct t_LoginVerify_Gateway_Return : t_NullCmd
		{
			DWORD accid;
			DWORD loginTempID;
			BYTE retcode;

			t_LoginVerify_Gateway_Return()
				: t_NullCmd(CMD_GATE, PARA_GATE_LOGINVERIFY_RETURN) {};
		};

		const BYTE PARA_GATE_LOGOUT = 4;
		struct t_Logout_Gateway : t_NullCmd
		{
			DWORD accid;
			DWORD loginTempID;

			t_Logout_Gateway()
				: t_NullCmd(CMD_GATE, PARA_GATE_LOGOUT) {};
		};

		/// ����һ����
		const BYTE PARA_REQUEST_GATE_REDEEM_GOLD = 5;
		struct t_Request_Redeem_Gold_Gateway : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			DWORD point;				/// ��һ��ĵ���
			
			t_Request_Redeem_Gold_Gateway()
				: t_NullCmd(CMD_REDEEM, PARA_REQUEST_GATE_REDEEM_GOLD) 
				{
					bzero(account , sizeof(account));
				};
		};

		/// �һ����
		const BYTE PARA_GATE_REDEEM_GOLD = 6;
		struct t_Redeem_Gold_Gateway : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			DWORD dwGold;				/// 	��ǰӵ�н����
			DWORD dwBalance;			/// �㿨���
			BYTE byReturn;	//��������
			
			t_Redeem_Gold_Gateway()
				: t_NullCmd(CMD_REDEEM, PARA_GATE_REDEEM_GOLD) 
				{
					bzero(account , sizeof(account));
					charid=0;
					dwGold=0;
					dwBalance=0;
					byReturn=0;
				};
		};
		/// ����һ��¿�
		const BYTE PARA_REQUEST_GATE_REDEEM_MONTH_CARD = 7;
		struct t_Request_Redeem_MonthCard_Gateway : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			t_Request_Redeem_MonthCard_Gateway()
				: t_NullCmd(CMD_REDEEM, PARA_REQUEST_GATE_REDEEM_MONTH_CARD)
				{
					bzero(account , sizeof(account));
				};
		};
		/// �һ��¿�
		const BYTE PARA_GATE_REDEEM_MONTH_CARD = 8;
		struct t_Redeem_MonthCard_Gateway : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			DWORD dwNum;	//��ֵ��ĵ���ʱ��(time_t)
			DWORD dwBalance;			/// �㿨���
			BYTE byReturn;	//��������
			t_Redeem_MonthCard_Gateway()
				: t_NullCmd(CMD_REDEEM, PARA_GATE_REDEEM_MONTH_CARD)
				{
					bzero(account , sizeof(account));
				};
		};

		/// ��ѯ�¿��ͽ��
		const BYTE PARA_GATE_REQUECT_CARD_GOLD = 9;
		struct t_Request_Card_Gold_Gateway : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			t_Request_Card_Gold_Gateway()
				: t_NullCmd(CMD_REDEEM, PARA_GATE_REQUECT_CARD_GOLD)
				{
					bzero(account , sizeof(account));
				};
		};
		 
		/// ��ѯ����
		const BYTE PARA_GATE_RETURN_CARD_GOLD = 10;
		struct t_Return_Card_Gold : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			DWORD dwMonthCard;	//�¿�
			DWORD dwGold;	//���
			BYTE byReturn;	//��������
			t_Return_Card_Gold()
				: t_NullCmd(CMD_REDEEM, PARA_GATE_RETURN_CARD_GOLD)
				{
					bzero(account , sizeof(account));
				};
		};

		/// ��ѯ����
		const BYTE PARA_GATE_REQUECT_POINT = 10;
		struct t_Request_Point_Gateway : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			t_Request_Point_Gateway()
				: t_NullCmd(CMD_REDEEM, PARA_GATE_REQUECT_POINT)
				{
					bzero(account , sizeof(account));
				};
		};
		 
		/// ��ѯ����
		const BYTE PARA_GATE_RETURN_POINT = 11;
		struct t_Return_Point : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			DWORD dwPoint;	//ʣ�����
			BYTE byReturn;	//��������
			t_Return_Point()
				: t_NullCmd(CMD_REDEEM, PARA_GATE_RETURN_POINT)
				{
					bzero(account , sizeof(account));
				};
		};
		 
		/// ���ѵ��߿�
		const BYTE PARA_GATE_CONSUME_CARD = 12;
		struct stConSumeCardCard_Gateway : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			char cardid[CARDID_LENGTH_PARA];
			DWORD type;						///������
			stConSumeCardCard_Gateway()
				: t_NullCmd(CMD_REDEEM, PARA_GATE_CONSUME_CARD)
				{
					bzero(account , sizeof(account));
					bzero(cardid,sizeof(cardid));
				};
		};
		/// ���ѵ��߿����� 
		const BYTE PARA_GATE_RETURN_CARD = 13;
		struct t_Return_ObjCard : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD subatt;	//����������
			DWORD type;						///������
			DWORD balance;						///ר��������
			BYTE byReturn;	//��������
			t_Return_ObjCard()
				: t_NullCmd(CMD_REDEEM, PARA_GATE_RETURN_CARD)
				{
					bzero(account , sizeof(account));
				};
		};
		/*
		const BYTE PARA_GATE_QUERY_GOLD = 6;
		struct t_Query_Gold_GateMoney : t_NullCmd
		{
			char account[Cmd::UserServer::ID_MAX_LENGTH+1];      		/// �ʺ�     
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			DWORD dwNum;	//����ʱ��(time_t,4�ֽڽṹ)
			BYTE byReturn;	//��������

			t_Query_Gold_GateMoney()
				: t_NullCmd(CMD_GATE, PARA_GATE_QUERY_GOLD) {};
		};
	
		enum
		{
			QUERY_SUCCESS,    // ��ѯ�ɹ�
			QUERY_BUSY,       // ������æ
			QUERY_FAIL,       // ��ѯʧ��
			QUERY_EMPTY,      // ���ʺţ�û�м�¼
		};
	  	  
		const BYTE PARA_GATE_RETURN_QUERY_GOLD = 7;
		struct t_Return_Query_Gold_GateMoney : t_NullCmd
		{
			DWORD accid;			        /// �˺ű��
			DWORD charid;				/// ��ɫID
			double gold;				/// �ʻ����
			BYTE  state;                            /// ��ѯ״̬

			t_Return_Query_Gold_GateMoney()
				: t_NullCmd(CMD_GATE, PARA_GATE_CHANGE_GOLD) {
				gold = 0;
				accid = 0;
				};
		};
		
		const BYTE PARA_GATE_TRADE_GOLD = 8;
		struct t_Trade_Gold_GateMoney : t_NullCmd
		{
			DWORD accid;			      			/// �˺ű��
			DWORD charid;						/// ��ɫID
			double gold;						/// ���׵Ľ�Ϊ��ΪҪ���ٵĽ������Ϊ��ΪҪ���ӵ�
										/// �����
			DWORD object_id;					/// ������Ʒ��ID
			DWORD object_num;					/// ������Ʒ������

			t_Trade_Gold_GateMoney()
				: t_NullCmd(CMD_GATE, PARA_GATE_TRADE_GOLD) {
				accid = 0;
				gold = 0;
				object_id = 0;
				};
		};

		
		const BYTE PARA_RETURN_GATE_TRADE_GOLD = 9;
		struct t_Return_Trade_Gold_GateMoney : t_NullCmd
		{
			DWORD accid;			      			/// �˺ű��
			DWORD charid;						/// ��ɫID
			double gold;						/// ������
			DWORD object_id;					/// ������Ʒ��ID
			DWORD object_num;					/// ������Ʒ������
			BYTE  state;						/// ���׽��״̬

			t_Return_Trade_Gold_GateMoney()
				: t_NullCmd(CMD_GATE, PARA_GATE_TRADE_GOLD) {
				accid = 0;
				gold = 0;
				object_id = 0;
				};
		};
		// */

		//////////////////////////////////////////////////////////////
		/// ��Ʊָ��
		//////////////////////////////////////////////////////////////
		//CMD_BILL_GATE_SCENE
		/// ��Ʊ����
		const BYTE PARA_STOCK_FETCH = 1;
		struct t_Stock_Fetch: t_NullCmd
		{
			DWORD dwGold;					/// ���ֽ������
			DWORD dwMoney;					/// ������������
			t_Stock_Fetch()
				: t_NullCmd(CMD_STOCK, PARA_STOCK_FETCH)
				{
					dwGold=0;
					dwMoney=0;
				};
		};
		/// ��Ʊ��ֵ
		const BYTE PARA_STOCK_SAVE = 2;
		struct t_Stock_Save: t_NullCmd
		{
			DWORD dwGold;					/// ����������
			DWORD dwMoney;					/// ������������
			t_Stock_Save()
				: t_NullCmd(CMD_STOCK, PARA_STOCK_SAVE)
				{
					dwGold=0;
					dwMoney=0;
				};
		};
		/// �����½�ɹ�
		const BYTE PARA_STOCK_LOGIN = 2;
		struct t_Stock_Login: t_NullCmd
		{
			t_Stock_Login()
				: t_NullCmd(CMD_STOCK, PARA_STOCK_LOGIN)
				{
				};
		};
		/// �����½�ɹ�
		const BYTE PARA_STOCK_LOGOUT = 2;
		struct t_Stock_Logout: t_NullCmd
		{
			t_Stock_Logout()
				: t_NullCmd(CMD_STOCK, PARA_STOCK_LOGOUT)
				{
				};
		};
		//////////////////////////////////////////////////////////////
		/// ��Ʊָ�����
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		/// �Ʒѷ����������ط�����������ָ��
		//////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////
		/// �Ự������ת��ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_FORWARD_USER = 1;
		struct t_Bill_ForwardUser : t_NullCmd
		{
			DWORD dwAccid;
			WORD  size;
			BYTE  data[0];
			t_Bill_ForwardUser()
				: t_NullCmd(CMD_FORWARD, PARA_FORWARD_USER) {};
		};
		const BYTE PARA_FORWARD_BILL = 2;
		struct t_Bill_ForwardBill : t_NullCmd
		{
			DWORD dwAccid;
			WORD  size;
			BYTE  data[0];
			t_Bill_ForwardBill()
				: t_NullCmd(CMD_FORWARD, PARA_FORWARD_BILL) {};
		};
		const BYTE PARA_FORWARD_BILL_TO_SCENE = 3;
		struct t_Bill_ForwardBillToScene : t_NullCmd
		{
			DWORD id;
			WORD  size;
			BYTE  data[0];
			t_Bill_ForwardBillToScene()
				: t_NullCmd(CMD_FORWARD, PARA_FORWARD_BILL_TO_SCENE) {};
		};
		const BYTE PARA_SCENE_FORWARD_BILL = 4;
		struct t_Scene_ForwardBill : t_NullCmd
		{
			DWORD dwAccid;
			WORD  size;
			BYTE  data[0];
			t_Scene_ForwardBill()
				: t_NullCmd(CMD_FORWARD, PARA_SCENE_FORWARD_BILL) {};
		};
		// */
	};

};

#pragma pack()

#endif

