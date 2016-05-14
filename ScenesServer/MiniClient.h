/**
 * \file
 * \version  $Id: MiniClient.h  $
 * \author  
 * \date 
 * \brief ����Ʒѷ��������ӿͻ���
 *
 */

#ifndef _MINICLIENT_H_
#define _MINICLIENT_H_

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "MiniCommand.h"

/**
 * \brief ����С��Ϸ���������ӿͻ�����
 *
 */
class MiniClient : public zTCPBufferClient
{

	public:
		
		MiniClient(
				const std::string &name, 
				const std::string &ip, 
				const unsigned short port,
				const unsigned short serverid)
			: zTCPBufferClient(name, ip, port) 
			{
				wdServerID=serverid;
			};

		bool connectToMiniServer();
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

extern MiniClient *miniClient;

#endif

