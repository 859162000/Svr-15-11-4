/**
 * \file
 * \version  $Id: SceneTask.h  $
 * \author  
 * \date 
 * \brief �����½��������
 *
 */

#ifndef _SceneTask_h_
#define _SceneTask_h_

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "zEntry.h"
#include "SceneCommand.h"
#include "zSocket.h"
#include "MessageQueue.h"
#include <list>

class SceneUser;
/**
 * \brief ��������������
 *
 */
class SceneTask : public zEntry, public zTCPTask , public MessageQueue
{

	public:

		/**
		 * \brief ���캯��
		 *
		 * \param pool �������ӳ�ָ��
		 * \param sock TCP/IP�׽ӿ�
		 * \param addr ��ַ
		 */
		SceneTask(
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
		virtual ~SceneTask();

		int verifyConn();
		int waitSync();
		int recycleConn();
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

		bool usermsgParse(SceneUser *pUser,const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);
		bool usermsgParseBill(SceneUser *pUser,const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);
		bool loginmsgParse(const Cmd::t_NullCmd *ptNullCmd, unsigned int cmdLen);
		WORD wdServerID;
		WORD wdServerType;

		bool verifyLogin(const Cmd::Scene::t_LoginScene *ptCmd);
		int recycle_state;
		bool veriry_ok;


};

#endif

