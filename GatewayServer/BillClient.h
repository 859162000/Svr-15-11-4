/**
 * \file
 * \version  $Id: BillClient.h  $
 * \author  
 * \date 
 * \brief ����Ʒѷ��������ӿͻ���
 *
 */

#ifndef _BillClient_h_
#define _BillClient_h_

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "BillCommand.h"

/**
 * \brief ����Ʒѷ��������ӿͻ�����
 *
 */
class BillClient : public zTCPBufferClient
{

	public:
		
		BillClient(
				const std::string &name, 
				const std::string &ip, 
				const unsigned short port,
				const unsigned short serverid)
			: zTCPBufferClient(name, ip, port) 
			{
				wdServerID=serverid;
			};

		bool connectToBillServer();
		void run();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		/**
		 * \brief ��ȡ�����������ı��
		 *
		 * \return �������������
		 */
		const WORD getServerID() const
		{
			return wdServerID;
		}
	private:
		WORD wdServerID;

};

extern BillClient *accountClient;

#endif

