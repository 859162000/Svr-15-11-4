/**
 * \file
 * \version  $Id: roleRegServer.cpp  $
 * \author  
 * \date 
 * \brief 角色唯一性验证服务器
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
#include "URLManager.h"
#include "zMisc.h"
#include "zArg.h"
#include "zConfile.h"
#include "zBase64.h"
#include "RoleTask.h"
#include "roleRegServer.h"

zDBConnPool *roleRegService::dbConnPool = NULL;
roleRegService *roleRegService::instance = NULL;

struct MURLCallback : public URLCallback
{
	bool exec(const URL &url)
	{
		if (!roleRegService::dbConnPool->putURL(url.hashcode, url.url.c_str(), url.supportTransactions))
		{
			Zebra::logger->error("连接数据库失败 %s", url.url.c_str());
			return false;
		}
		return true;
	}
};

bool roleRegService::init()
{
	MURLCallback callback;
	
	dbConnPool = zDBConnPool::newInstance(URLManager::defaultHashCode);
	if (NULL == dbConnPool
			|| !URLManager::getInstance().execAll(callback))
	{
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

	client_port = atoi(Zebra::global["client_port"].c_str());
	if (!zNetService::init(client_port))
	{
		return false;
	}
	return true;
}

void roleRegService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	RoleTask *tcpTask = new RoleTask(taskPool, sock, addr);
	if (NULL == tcpTask)
		//内存不足，直接关闭连接
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//得到了一个正确连接，添加到验证队列中
		SAFE_DELETE(tcpTask);
	}
}

void roleRegService::final()
{
	zNetService::final();
	zDBConnPool::delInstance(&dbConnPool);
	URLManager::delInstance();
}
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
static char dbaccess_doc[] = "\nroleRegServer\n" "\t数据库访问服务器。";

/**
 * \brief 程序的版本信息
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING);

/**
 * \brief 读取配置文件
 */
class roleRegConfile : public zConfile
{
	public:
		roleRegConfile(const char *confile = "configrole.xml") : zConfile(confile)
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
					char buf[128];
					if(parser.getNodeContentStr(child,buf,128))
					{
						if (0 == strcmp((char *)child->name, "mysql"))
						{
							char my_buf[128];
							std::string tmpS;
							URL url;
							if (parser.getNodePropStr(child,"encode",my_buf,128)
									&& 0 == strcasecmp(my_buf, "yes"))
								Zebra::base64_decrypt(buf, tmpS);
							else
								tmpS = buf;

							if (parser.getNodePropStr(child,"hashcode",my_buf,128))
								url.hashcode = atoi(my_buf);
							else
								url.hashcode = 0;
							url.url = tmpS;
							if (parser.getNodePropStr(child,"transaction",my_buf,128)
									&& 0 == strcasecmp(my_buf, "yes"))
								url.supportTransactions = true;
							else
								url.supportTransactions = false;
							URLManager::getInstance().add(url);
						}
						else
							Zebra::global[(char *)child->name]=buf;
					}

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
 */
void roleRegService::reloadConfig()
{
	roleRegConfile sc;
	sc.parse("roleRegServer");
}

/**
 * \brief 主程序入口
 * \param argc 参数个数
 * \param argv 参数列表
 * \return 运行结果
 */
int main(int argc, char **argv)
{
	Zebra::logger=new zLogger("roleRegServer");

	//设置缺省参数
	Zebra::global["client_port"] = "9900";
	Zebra::global["logfilename"] = "/tmp/roleregserver.log";
	Zebra::global["dbCount"] = "16";
	Zebra::global["tableCount"] = "16";

	//解析配置文件参数
	roleRegConfile sc;
	if (!sc.parse("roleRegServer"))
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
	
	roleRegService::getInstance().main();
	roleRegService::delInstance();
	
	return EXIT_FAILURE;
}






