/**
 * \file
 * \version  $Id: SessionClient.h  $
 * \author  
 * \date 
 * \brief ����Session���������ӿͻ���
 *
 */

#ifndef _SessionClient_h_
#define _SessionClient_h_

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "SessionCommand.h"
#include "zMisc.h"
#include "zSocket.h"
#include "MessageQueue.h"

class SceneUser;

class SessionClient : public zTCPBufferClient , public MessageQueue
{

	public:
		/**
		* \brief ���캯��
		* \param  name ����
		* \param  ip   ��ַ
		* \param  port �˿�
		*/
		SessionClient(
				const std::string &name, 
				const std::string &ip, 
				const unsigned short port)
			: zTCPBufferClient(name, ip, port) {};

		bool connectToSessionServer();
		void run();

		void requestFriendDegree(SceneUser *pUser);
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool doGmCmd(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool doAuctionCmd(const Cmd::Session::t_AuctionCmd *, const DWORD);
		bool doCartoonCmd(const Cmd::Session::t_CartoonCmd *, const DWORD);
		bool cmdMsgParse(const Cmd::t_NullCmd *, const unsigned int);
		bool cmdMsgParse_Country(const Cmd::t_NullCmd*, const unsigned int);
		bool cmdMsgParse_Dare(const Cmd::t_NullCmd*, const unsigned int);
		bool cmdMsgParse_Recommend(const Cmd::t_NullCmd*, const unsigned int);
		bool cmdMsgParse_Other(const Cmd::t_NullCmd*, const unsigned int);
		bool cmdMsgParse_Temp(const Cmd::t_NullCmd*, const unsigned int);
		bool cmdMsgParse_Army(const Cmd::t_NullCmd* ptNullCmd, const unsigned int nCmdLen);
		bool cmdMsgParse_Sept(const Cmd::t_NullCmd*, const unsigned int);
		bool cmdMsgParse_Union(const Cmd::t_NullCmd*, const unsigned int);
		bool cmdMsgParse_Gem(const Cmd::t_NullCmd* ptNullCmd, const unsigned int nCmdLen);

};

/// ����
extern SessionClient *sessionClient;

#endif

