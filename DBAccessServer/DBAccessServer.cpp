/**
 * \file
 * \version  $Id: InfoServer.cpp $
 * \author  
 * \date 
 * \brief 信息集中服务器
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
static InfoContainer pContainer;//全局容器
/*
void CmdThread::run()
{
	using namespace Cmd::Info;
	time_t st=tm.sec();//CmdThread线程创建的时间
	time_t sp=tn.sec();
	while(!isFinal())
	{
		tm.now();
		tn.now();
		if ((tm.sec() - st) == delta)//10秒钟一次的服务器信息统计
		{
			t_Request_ServerInfo ptCmd;
			ptCmd.rTimestamp = time(NULL);
			pContainer.broadcast(&ptCmd, sizeof(ptCmd));
			st = tm.sec();
		}
		if ((tm.sec() - sp) == delta*6)//一分钟一次的在线人数统计
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
		Zebra::logger->error("连接数据库失败 %s", Zebra::global["mysql"].c_str());
		return false;
	}
	
	//初始化连接线程池
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
		//内存不足，直接关闭连接
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//得到了一个正确连接，添加到验证队列中
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
 * \brief 命令行参数
 */
static struct argp_option dbaccess_options[] =
{
	{"daemon",		  	'd',	0,				0,	"Run service as daemon",	0},
	{"log",				'l',	"level",		0,	"Log level",					0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",				0},
	{0,					0,		0,				0,	0,									0}
};


/**
 * \brief 命令行参数解析器
 * \param key 参数缩写
 * \param arg 参数值
 * \param state 参数状态
 * \return 返回错误代码
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
 * \brief 简短描述信息
 */
static char dbaccess_doc[] = "\nDBAccessServer\n" "\t信息集中服务器。";

/**
 * \brief 程序的版本信息
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING);

/**
 * \brief 读取配置文件
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
 * \brief 重新读取配置文件,为HUP信号的处理函数
 
void InfoService::reloadConfig()
{
	InfoConfile sc;
	sc.parse("InfoServer");
}
*/
/**
 * \brief 主程序入口
 * \param argc 参数个数
 * \param argv 参数列表
 * \return 运行结果
 */
int main(int argc, char **argv)
{
	Zebra::logger=new zLogger("DBAccessServer");

	//设置缺省参数
	Zebra::global["bindport"] = "9903";
	Zebra::global["logfilename"] = "/tmp/dbaccessserver.log";
	Zebra::global["dbCount"] = "16";
	Zebra::global["tableCount"] = "16";

	//解析配置文件参数
	InfoConfile sc;
	if (!sc.parse("DBAccessServer"))
		return EXIT_FAILURE;

	//解析命令行参数
	zArg::getArg()->add(dbaccess_options, dbaccess_parse_opt, 0, dbaccess_doc);
	zArg::getArg()->parse(argc, argv);

	//设置日志级别
	Zebra::logger->setLevel(Zebra::global["log"]);
	//设置写本地日志文件
	if ("" != Zebra::global["logfilename"])
		Zebra::logger->addLocalFileLog(Zebra::global["logfilename"]);

	//是否以后台进程的方式运行
	if ("true" == Zebra::global["daemon"]) {
		Zebra::logger->info("Program will be run as a daemon");
		Zebra::logger->removeConsoleLog();
		daemon(1, 1);
	}

	InfoService::getInstance().main();
	InfoService::delInstance();
	
	return EXIT_FAILURE;
}





