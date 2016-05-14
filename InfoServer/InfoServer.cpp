/**
 * \file
 * \version  $Id: InfoServer.cpp $
 * \author  
 * \date 
 * \brief ��Ϣ���з�����
 */
 
#include <iostream>
#include <string>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zTCPTaskPool.h"
#include "zNetService.h"
#include "zDBConnPool.h"
#include "zMisc.h"
#include "zArg.h"
#include "zBase64.h"
#include "InfoTask.h"
#include "InfoServer.h"
#include "InfoCommand.h"

zDBConnPool * InfoService::dbConnPool = NULL;
InfoService * InfoService::instance = NULL;
CmdThread 	* InfoService::pCmdThread = NULL;

time_t CmdThread::delta = 10;
static InfoContainer pContainer;//ȫ������
void CmdThread::run()
{
	using namespace Cmd::Info;
	time_t st=tm.sec();//CmdThread�̴߳�����ʱ��
	time_t sp=tn.sec();
	while(!isFinal())
	{
		tm.now();
		tn.now();
		if ((tm.sec() - st) == delta)//10����һ�εķ�������Ϣͳ��
		{
			t_Request_ServerInfo ptCmd;
			ptCmd.rTimestamp = time(NULL);
			pContainer.broadcast(&ptCmd, sizeof(ptCmd));
			st = tm.sec();
		}
		if ((tm.sec() - sp) == delta*6)//һ����һ�ε���������ͳ��
		{
			t_Request_OnlineNum psCmd;
			psCmd.rTimestamp = time(NULL);
			pContainer.broadcast(&psCmd, sizeof(psCmd));
			sp = tn.sec();
		}
		zThread::msleep(50);
	}
}

bool InfoService::init()
{
	dbConnPool = zDBConnPool::newInstance(NULL);

	if (NULL == dbConnPool
			|| !dbConnPool->putURL(0, Zebra::global["mysql"].c_str(), false))
	{
		Zebra::logger->error("�������ݿ�ʧ�� %s", Zebra::global["mysql"].c_str());
		return false;
	}
	
	//��ʼ�������̳߳�
	int state = state_none;
	Zebra::to_lower(Zebra::global["initThreadPoolState"]);
	if ("repair" == Zebra::global["initThreadPoolState"]
			|| "maintain" == Zebra::global["initThreadPoolState"])
		state = state_maintain;
	taskPool = new zTCPTaskPool(atoi(Zebra::global["threadPoolCapacity"].c_str()), state);
	if (NULL == taskPool
			|| !taskPool->init())
		return false;

	bindport = atoi(Zebra::global["bindport"].c_str());
	if (!zNetService::init(bindport))
	{
		return false;
	}
	
	pCmdThread = new CmdThread;
	if (NULL != pCmdThread && pCmdThread->start())
	{
		return true;
	}
	return false;
}

void InfoService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	InfoTask *tcpTask = new InfoTask(taskPool, sock, addr, &pContainer);
	if (NULL == tcpTask)
		//�ڴ治�㣬ֱ�ӹر�����
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//�õ���һ����ȷ���ӣ���ӵ���֤������
		SAFE_DELETE(tcpTask);
	}
}

void InfoService::final()
{
	zNetService::final();
	zDBConnPool::delInstance(&dbConnPool);
	SAFE_DELETE(pCmdThread);
}

InfoConfile::InfoConfile(const char *confile) : zConfile(confile)
{
	Zebra::logger->debug(__PRETTY_FUNCTION__);
}
bool InfoConfile::parseYour(const xmlNodePtr node)
{
	if(node)
	{
		xmlNodePtr child=parser.getChildNode(node,NULL);
		while(child)
		{
			parseNormal(child);
			child=parser.getNextNode(child,NULL);
		}
		return true;
	}
	else
	{
		Zebra::logger->error(__PRETTY_FUNCTION__);
		return false;
	}
}

/**
 * \brief ���¶�ȡ�����ļ�,ΪHUP�źŵĴ�����
 */
void InfoService::reloadConfig()
{
	InfoConfile sc;
	sc.parse("InfoServer");
}






