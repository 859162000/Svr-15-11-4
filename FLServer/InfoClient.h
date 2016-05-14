/**
 * \file
 * \version  $Id: InfoClient.h  $
 * \author  
 * \date 
 * \brief �����������Ϣ�ռ��Ŀͻ�������
 */

#ifndef _InfoClient_h_
#define _InfoClient_h_

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"

/**
 * \brief ��Ϣ�ռ��ͻ���������
 */
class InfoClient : public zTCPClientTask
{

	public:

		InfoClient(
				const std::string &ip,
				const unsigned short port);
		~InfoClient();

		int checkRebound();
		void addToContainer();
		void removeFromContainer();
		bool connect();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

};

#endif

