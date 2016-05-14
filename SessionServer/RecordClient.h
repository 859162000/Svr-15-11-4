/**
 * \file
 * \version  $Id: RecordClient.h  $
 * \author  
 * \date
 * \brief ���嵵�����������ӿͻ���
 *
 * �����뵵����������������ȡ����
 * 
 */

#ifndef _RecordClient_h_
#define _RecordClient_h_

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "RecordCommand.h"
#include "MessageQueue.h"

class RecordClient : public zTCPClient , public MessageQueue
{

	public:

		/**
		 * \brief ���캯��
		 * ���ڵ��������Ѿ���ѹ�����ģ����ڵײ㴫���ʱ��Ͳ���Ҫѹ����
		 * \param name ����
		 * \param ip ��ַ
		 * \param port �˿�
		 */
		RecordClient(
				const std::string &name, 
				const std::string &ip, 
				const unsigned short port)
			: zTCPClient(name, ip, port, false) {};

		bool connectToRecordServer();

		void run();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool cmdMsgParse(const Cmd::t_NullCmd *, const unsigned int);

};

extern RecordClient *recordClient;

#endif

