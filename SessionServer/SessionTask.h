/**
 * \file
 * \version  $Id: SessionTask.h $
 * \author  
 * \date 
 * \brief �����½��������
 *
 */

#ifndef _SessionTask_h_
#define _SessionTask_h_

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "zEntry.h"
#include "MessageQueue.h"
#include "SessionCommand.h"

class UserSession;

/**
 * \brief ��������������
 *
 */
class SessionTask : public zEntry, public zTCPTask , public MessageQueue
{

	public:

		/**
		 * \brief ���캯��
		 *
		 * \param pool �������ӳ�ָ��
		 * \param sock TCP/IP�׽ӿ�
		 * \param addr ��ַ
		 */
		SessionTask(
				zTCPTaskPool *pool,
				const int sock,
				const struct sockaddr_in *addr = NULL) : zTCPTask(pool, sock, addr)
		{
			wdServerID = 0;
			wdServerType = UNKNOWNSERVER;
			recycle_state = 0;
		}

		/**
		 * \brief ����������
		 *
		 */
		virtual ~SessionTask();

		int verifyConn();
		int recycleConn();
		void addToContainer();
		void removeFromContainer();
		bool uniqueAdd();
		bool uniqueRemove();
		bool msgParse(const Cmd::t_NullCmd *, const unsigned int);
		bool cmdMsgParse(const Cmd::t_NullCmd *, const unsigned int);

		/**
		 * \brief ���ط��������
		 *
		 * �����һ��������Ψһ�ģ������ڷ�������������
		 *
		 * \return ���������
		 */
		const WORD getID() const
		{
			return wdServerID;
		}

		/**
		 * \brief ���ط���������
		 *
		 * \return ����������
		 */
		const WORD getType() const
		{
			return wdServerType;
		}
		bool checkRecycle();

	private:

		/**
		 * \brief ���������
		 *
		 */
		WORD wdServerID;

		/**
		 * \brief ����������
		 *
		 */
		WORD wdServerType;
		///���û��ձ�־
		int recycle_state;

		bool verifyLogin(const Cmd::Session::t_LoginSession *ptCmd);

		/**
		 * \brief ɾ����ɫ�Ĵ���
		 *
		 * �ӽ�ɫ��Ӧ������ϵ�а���ɾ���������ɫ��ĳһ����ϵ�Ľ�����������ɾ���ý�ɫ
		 * �������ɾ������֪ͨGATEWAY����ӵ�����ɾ�������������ɾ��������һ��֪ͨ������ͻ��ˡ�
		 *
		 *
		 */
		bool del_role(const Cmd::t_NullCmd* cmd, const unsigned int cmdLen);
		/**
		 * \brief ��������
		 *
		 * \param dwUserID : �����������û�ID
		 *
		 */
		bool change_country(const Cmd::Session::t_changeCountry_SceneSession* cmd);
		bool msgParse_Scene(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen);
		bool msgParse_Gate(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen);
		bool msgParse_Forward(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);

};

#endif

