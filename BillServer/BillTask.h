/**
 * \file
 * \version  $Id: BillTask.h  $
 * \author  
 * \date 2
 * \brief ����Ʒ���������
 *
 */

#ifndef _BillTask_h_
#define _BillTask_h_

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <ext/hash_map>

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "BillCommand.h"
#include "MessageQueue.h"

/**
 * \brief ����Ʒ�����������
 *
 */
class BillTask : public zTCPTask , public MessageQueue
{

	public:

		/**
		 * \brief ���캯��
		 *
		 * \param pool �������ӳ�ָ��
		 * \param sock TCP/IP�׽ӿ�
		 * \param addr ��ַ
		 */
		BillTask(
				zTCPTaskPool *pool,
				const int sock,
				const struct sockaddr_in *addr = NULL) : zTCPTask(pool, sock, addr)
		{
			wdServerID = 0;
			wdServerType = UNKNOWNSERVER;
			recycle_state=0;
			veriry_ok=false; 
		}

		/**
		 * \brief ����������
		 *
		 */
		~BillTask() {};

		int verifyConn();
		int recycleConn();
		bool uniqueAdd();
		bool uniqueRemove();
		bool msgParse(const Cmd::t_NullCmd *, const unsigned int);
		bool cmdMsgParse(const Cmd::t_NullCmd *, const unsigned int);
		bool checkRecycle();

		/**
		 * \brief ��ȡ���������
		 *
		 * \return ���������
		 */
		const WORD getID() const
		{
			return wdServerID;
		}

		/**
		 * \brief ��ȡ����������
		 *
		 * \return ����������
		 */
		const WORD getType() const
		{
			return wdServerType;
		}

		bool sendCmdToUser(DWORD id, const void *pstrCmd, const unsigned int nCmdLen);
		bool sendCmdToScene(DWORD id, const void *pstrCmd, const unsigned int nCmdLen);

	private:

		/**
		 * \brief �������ʻ������
		 *
		 */
		zMutex mlock;
				
		WORD wdServerID;					/**< ��������ţ�һ����Ψһ�� */
		WORD wdServerType;					/**< ���������� */

		/**
		  * \brief ��ѯ���
		  *
		  *
		  */
		//void query_gold(const Cmd::Bill::t_Query_Gold_GateMoney* cmd);

		/**
		  * \brief �㿨�һ����
		  *
		  */
		//void change_gold(const Cmd::Bill::t_Change_Gold_GateMoney* cmd);

		/**
		  * \brief ��ҽ���
		  *
		  */
		//void trade_gold(const Cmd::Bill::t_Trade_Gold_GateMoney* cmd);

		//void trade_log(const Cmd::Bill::t_Trade_Gold_GateMoney* cmd, const char* account, double money);
		bool verifyLogin(const Cmd::Bill::t_LoginBill *ptCmd);
		int recycle_state;
		bool veriry_ok;

};

#endif

