/**
 * \file
 * \version  $Id: RecordClient.cpp  $
 * \author  
 * \date 
 * \brief ���嵵�����������ӿͻ���
 *
 * �����뵵����������������ȡ����
 * 
 */

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "RecordCommand.h"
#include "RecordClient.h"
#include "SessionServer.h"
#include "Zebra.h"
//#include <zlib.h>
#include "RecommendManager.h"


RecordClient *recordClient = NULL;

bool RecordClient::connectToRecordServer()
{
	if (!connect())
	{
		Zebra::logger->error("���ӵ���������ʧ��");
		return false;
	}

	using namespace Cmd::Record;
	t_LoginRecord tCmd;
	tCmd.wdServerID = SessionService::getInstance().getServerID();
	tCmd.wdServerType = SessionService::getInstance().getServerType();

	return sendCmd(&tCmd, sizeof(tCmd));
}

void RecordClient::run()
{
	zTCPClient::run();

	//�뵵�������������ӶϿ����رշ�����
	SessionService::getInstance().Terminate();
	while(!SessionService::getInstance().isSequeueTerminate())
	{
		zThread::msleep(10);
	}
}

bool RecordClient::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}

bool RecordClient::cmdMsgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Record;

	if (ptNullCmd->cmd==CMD_SESSION)
	{
		switch (ptNullCmd->para)
		{
			case PARA_CHK_USER_EXIST:
				{
					Cmd::Record::t_chkUserExist_SessionRecord* rev = 
						(Cmd::Record::t_chkUserExist_SessionRecord*)ptNullCmd;
					
					RecommendM::getMe().processAddRecommended(rev);
					return true;
				}
				break;
			default:
				break;
		}
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

