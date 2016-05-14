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
#include "zConfile.h"
#include "zBase64.h"
#include "InfoTask.h"
#include "DBAccessServer.h"


zDBConnPool * DBAccessService::dbConnPool = NULL;
DBAccessService * DBAccessService::instance = NULL;
CmdThread 	* DBAccessService::pCmdThread = NULL;

//time_t CmdThread::delta = 10;
static InfoContainer pContainer;//ȫ������
/*
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
*/
bool DBAccessService::init()
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
/*
void InfoService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	DBAccessTask *tcpTask = new DBAccessTask(taskPool, sock, addr, &pContainer);
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
*/

/**
 * \brief �����в���
 */
static struct argp_option dbaccess_options[] =
{
	{"daemon",		  	'd',	0,				0,	"Run service as daemon",	0},
	{"log",				'l',	"level",		0,	"Log level",					0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",				0},
	{0,					0,		0,				0,	0,									0}
};


/**
 * \brief �����в���������
 * \param key ������д
 * \param arg ����ֵ
 * \param state ����״̬
 * \return ���ش������
 */
static error_t dbaccess_parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'd':
			{
				Zebra::global["daemon"] = "true";
			}
			break;
		case 'l':
			{
				Zebra::global["log"]=arg;
			}
			break;
		case 'f':
			{
				Zebra::global["logfilename"]=arg;
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}


/**
 * \brief ���������Ϣ
 */
static char dbaccess_doc[] = "\nDBAccessServer\n" "\t��Ϣ���з�������";

/**
 * \brief ����İ汾��Ϣ
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING);

/**
 * \brief ��ȡ�����ļ�
 */
class DBInfoConfile : public zConfile
{
	public:
		DBInfoConfile(const char *confile = "configInfo.xml") : zConfile(confile)
		{
			Zebra::logger->debug(__PRETTY_FUNCTION__);
		}
	private:
		bool parseYour(const xmlNodePtr node)
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
};


/**
 * \brief ���¶�ȡ�����ļ�,ΪHUP�źŵĴ�����
 
void InfoService::reloadConfig()
{
	InfoConfile sc;
	sc.parse("InfoServer");
}
*/
/**
 * \brief ���������
 * \param argc ��������
 * \param argv �����б�
 * \return ���н��
 */
int main(int argc, char **argv)
{
	Zebra::logger=new zLogger("DBAccessServer");

	//����ȱʡ����
	Zebra::global["bindport"] = "9903";
	Zebra::global["logfilename"] = "/tmp/dbaccessserver.log";
	Zebra::global["dbCount"] = "16";
	Zebra::global["tableCount"] = "16";

	//���������ļ�����
	InfoConfile sc;
	if (!sc.parse("DBAccessServer"))
		return EXIT_FAILURE;

	//���������в���
	zArg::getArg()->add(dbaccess_options, dbaccess_parse_opt, 0, dbaccess_doc);
	zArg::getArg()->parse(argc, argv);

	//������־����
	Zebra::logger->setLevel(Zebra::global["log"]);
	//����д������־�ļ�
	if ("" != Zebra::global["logfilename"])
		Zebra::logger->addLocalFileLog(Zebra::global["logfilename"]);

	//�Ƿ��Ժ�̨���̵ķ�ʽ����
	if ("true" == Zebra::global["daemon"]) {
		Zebra::logger->info("Program will be run as a daemon");
		Zebra::logger->removeConsoleLog();
		daemon(1, 1);
	}

	InfoService::getInstance().main();
	InfoService::delInstance();
	
	return EXIT_FAILURE;
}





