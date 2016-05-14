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

/**
 * \brief ����Session���������ӿͻ�����
 *
 */
class SessionClient : public zTCPBufferClient
{

	public:
		
		SessionClient(
				const std::string &name, 
				const std::string &ip, 
				const unsigned short port)
			: zTCPBufferClient(name, ip, port) {};

		bool connectToSessionServer();
		void run();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

};

extern SessionClient *sessionClient;

#endif

