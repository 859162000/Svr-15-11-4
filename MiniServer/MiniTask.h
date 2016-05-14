/**
 * \file
 * \version  $Id: MiniTask.h  $
 * \author  
 * \date 
 * \brief ����Ʒ���������
 *
 */

#ifndef _MINITASK_H_
#define _MINITASK_H_

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <ext/hash_map>

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "MiniCommand.h"
#include "MessageQueue.h"

/**
 * \brief ����Ʒ�����������
 *
 */
class MiniTask : public zTCPTask , public MessageQueue
{

	public:

		/**
		 * \brief ���캯��
		 *
		 * \param pool �������ӳ�ָ��
		 * \param sock TCP/IP�׽ӿ�
		 * \param addr ��ַ
		 */
		MiniTask(
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
		~MiniTask() {};

		int verifyConn();
		int recycleConn();
		bool uniqueAdd();
		bool uniqueRemove();
		bool msgParse(const Cmd::t_NullCmd *, const unsigned int);
		bool cmdMsgParse(const Cmd::t_NullCmd *, const unsigned int);
		bool parseGateMsg(const Cmd::t_NullCmd *, const unsigned int);
		bool parseForwardMsg(const Cmd::t_NullCmd *, const unsigned int);
		bool parseSceneMsg(const Cmd::t_NullCmd *, const unsigned int);
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

		bool verifyLogin(const Cmd::Mini::t_LoginMini *ptCmd);
		int recycle_state;
		bool veriry_ok;

		bool addDBMoney(DWORD userID, DWORD num);
};

#endif

