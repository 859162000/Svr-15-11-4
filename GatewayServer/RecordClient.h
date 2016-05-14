/**
 * \file
 * \version  $Id: RecordClient.h  $
 * \author  
 * \date 
 * \brief �������ط��������������������ӿͻ���
 */

#ifndef _RECORDCLIENT_H_
#define _RECORDCLIENT_H_

#include "zTCPClient.h"
#include "zMutex.h"

/**
 * \brief �����뵵��������������
 *
 */
class RecordClient : public zTCPBufferClient
{

	public:
		
		/**
		 * \brief ���캯��
		 *
		 * \param name ����
		 * \param ip ��������ַ
		 * \param port �������˿�
		 */
		RecordClient(
				const std::string &name, 
				const std::string &ip, 
				const unsigned short port)
			: zTCPBufferClient(name, ip, port) {};

		bool connectToRecordServer();
		void run();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

};

extern RecordClient *recordClient;

#endif
