/**
 * \file
 * \version  $Id: SuperServer.cpp  $
 * \author  
 * \date 
 * \brief ʵ�ַ�����������
 *
 * ��һ�����е����з��������й���
 * 
 */

#include <iostream>
#include <string>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zNetService.h"
#include "ServerTask.h"
#include "zDBConnPool.h"
#include "SuperServer.h"
#include "Zebra.h"
#include "zMisc.h"
#include "zArg.h"
#include "zConfile.h"
#include "TimeTick.h"
#include "ServerManager.h"
#include "FLClient.h"
#include "FLClientManager.h"
#include "RoleregClient.h"
#include "RoleregClientManager.h"
#include "RoleregCache.h"
#include "InfoClient.h"
#include "InfoClientManager.h"

zDBConnPool *SuperService::dbConnPool = NULL;

SuperService *SuperService::instance = NULL;

/**
 * \brief �����ݿ��л�ȡ��������Ϣ
 *
 * ������ݿ���û�з���������������Ϣ����Ҫ��ʼ��һ����¼
 *
 */
bool SuperService::getServerInfo()
{
	static const dbCol col_define[] =
	{
		{"ID",zDBConnPool::DB_WORD,sizeof(WORD)},
		{"TYPE",zDBConnPool::DB_WORD,sizeof(WORD)},
		{"NAME",zDBConnPool::DB_STR,sizeof(char[MAX_NAMESIZE])},
		{"IP",zDBConnPool::DB_STR,sizeof(char[MAX_IP_LENGTH])},
		{"PORT",zDBConnPool::DB_WORD,sizeof(WORD)},
		{"EXTIP",zDBConnPool::DB_STR,sizeof(char[MAX_IP_LENGTH])},
		{"EXTPORT",zDBConnPool::DB_WORD,sizeof(WORD)},
		{NULL, 0, 0}
	};
	struct
	{
		WORD wdServerID;
		WORD wdServerType;
		char pstrName[MAX_NAMESIZE];
		char pstrIP[MAX_IP_LENGTH];
		WORD wdPort;
		char pstrExtIP[MAX_IP_LENGTH];
		WORD wdExtPort;
	} __attribute__ ((packed))
	data, *pData = NULL;
	char where[32];

	connHandleID handle = dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܴ����ݿ����ӳػ�ȡ���Ӿ��");
		return false;
	}
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "`TYPE`=%u", SUPERSERVER);
	unsigned int retcode = dbConnPool->exeSelect(handle, "`SERVERLIST`", col_define, where, NULL,(unsigned char **)&pData);
	if ((unsigned int)1 == retcode && pData)
	{
		//ֻ��һ�����������ļ�¼
		if (strcmp(pstrIP, pData->pstrIP) == 0)
		{
			wdServerID = pData->wdServerID;
			strncpy(pstrName, pData->pstrName, sizeof(pstrName) - 1);
			wdPort = pData->wdPort;
			wdExtPort = pData->wdExtPort;
			SAFE_DELETE_VEC(pData);

			Zebra::logger->debug("ServerID=%u IP=%s Port=%u",wdServerID, pstrIP, wdPort);
		}
		else
		{
			Zebra::logger->error("���ݿ��еļ�¼�����ϣ�%s, %s", pstrIP, pData->pstrIP);
			SAFE_DELETE_VEC(pData);
			dbConnPool->putHandle(handle);
			return false;
		}
	}
	else if(0 == retcode)
	{
		//���ݿ���û�м�¼������һЩȱʡ��Ϣ������һ����¼
		strncpy(pstrName, "������������", sizeof(pstrName) - 1);
		wdExtPort = wdPort = 10000;

		bzero(&data, sizeof(data));
		data.wdServerID = wdServerID;
		data.wdServerType = wdServerType;
		strncpy(data.pstrName, pstrName, sizeof(data.pstrName) - 1);
		strncpy(data.pstrIP, pstrIP, sizeof(data.pstrIP) - 1);
		data.wdPort = wdPort;
		strncpy(data.pstrExtIP, pstrExtIP, sizeof(data.pstrExtIP) - 1);
		data.wdExtPort = wdExtPort;
		if ((unsigned int)-1 == dbConnPool->exeInsert(handle, "`SERVERLIST`", col_define, (const unsigned char *)&data))
		{
			Zebra::logger->error("�����ݿ��в����������Ϣ��¼ʧ��");
			dbConnPool->putHandle(handle);
			return false;
		}
	}
	else
	{
		//��ѯ�������߼�¼̫��
		Zebra::logger->error("��ѯ���ݳ����������ݿ��з������������ļ�¼̫�࣬��Ҫ����");
		SAFE_DELETE_VEC(pData);
		dbConnPool->putHandle(handle);
		return false;
	}
	dbConnPool->putHandle(handle);

	return true;
}

/**
 * \brief ��ʼ���������������
 *
 * ʵ�ִ��麯��<code>zService::init</code>
 *
 * \return �Ƿ�ɹ�
 */
bool SuperService::init()
{
	Zebra::logger->trace("SuperService::init");

	dbConnPool = zDBConnPool::newInstance(NULL);
	if (NULL == dbConnPool
			|| !dbConnPool->putURL(0, Zebra::global["mysql"].c_str(), false))
	{
		Zebra::logger->error("�������ݿ�ʧ��");
		return false;
	}

	strncpy(pstrIP, zSocket::getIPByIfName(Zebra::global["ifname"].c_str()), MAX_IP_LENGTH - 1);
	//Zebra::logger->debug("%s", pstrIP);

	if (!getServerInfo())
		return false;

	if (!FLClientManager::getInstance().init())
		return false;

	if (!RoleregClientManager::getInstance().init())
		return false;

	if (!InfoClientManager::getInstance().init())
		return false;

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

	if (!zNetService::init(wdPort))
	{
		return false;
	}

	SuperTimeTick::getInstance().start();

	return true;
}

/**
 * \brief �½���һ����������
 *
 * ʵ�ִ��麯��<code>zNetService::newTCPTask</code>
 *
 * \param sock TCP/IP����
 * \param addr ��ַ
 */
void SuperService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	ServerTask *tcpTask = new ServerTask(taskPool, sock, addr);
	if (NULL == tcpTask)
		//�ڴ治�㣬ֱ�ӹر�����
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//�õ���һ����ȷ���ӣ���ӵ���֤������
		SAFE_DELETE(tcpTask);
	}
}

/**
 * \brief �������������
 *
 * ʵ�ִ��麯��<code>zService::final</code>
 *
 */
void SuperService::final()
{
	SuperTimeTick::getInstance().final();
	SuperTimeTick::getInstance().join();
	SuperTimeTick::delInstance();

	if(taskPool)
	{
		taskPool->final();
		SAFE_DELETE(taskPool);
	}
	InfoClientManager::delInstance();

	zNetService::final();

	ServerManager::delInstance();

	FLClientManager::delInstance();

	RoleregClientManager::delInstance();

	RoleregCache::delInstance();

	zDBConnPool::delInstance(&dbConnPool);

	Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
}

/**
 * \brief �����в���
 *
 */
static struct argp_option super_options[] =
{
	{"daemon",		'd',	0,			0,	"Run service as daemon",						0},
	{"log",			'l',	"level",	0,	"Log level",									0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",								0},
	{"mysql",		'y',	"mysql",	0,	"MySQL[mysql://user:passwd@host:port/dbName]",	0},
	{"ifname",		'i',	"ifname",	0,	"Local network device",							0},
	{0,				0,		0,			0,	0,												0}
};

/**
 * \brief �����в���������
 *
 * \param key ������д
 * \param arg ����ֵ
 * \param state ����״̬
 * \return ���ش������
 */
static error_t super_parse_opt(int key, char *arg, struct argp_state *state)
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
		case 'y':
			{
				Zebra::global["mysql"]=arg;
			}
			break;
		case 'i':
			{
				Zebra::global["ifname"]=arg;
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/**
 * \brief ���������Ϣ
 *
 */
static char super_doc[] = "\nSuperServer\n" "\t��������������";

/**
 * \brief ����İ汾��Ϣ
 *
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING)\
									"\nBuild time      :\t" __DATE__ ", " __TIME__;

/**
 * \brief ��ȡ�����ļ�
 *
 */
class SuperConfile:public zConfile
{
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
			return false;
	}
};

/**
 * \brief ���¶�ȡ�����ļ���ΪHUP�źŵĴ�����
 *
 */
void SuperService::reloadConfig()
{
	Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
	SuperConfile sc;
	sc.parse("SuperServer");
	//ָ���⿪��
	if(Zebra::global["cmdswitch"] == "true")
	{
		zTCPTask::analysis._switch = true;
	}
	else
	{
		zTCPTask::analysis._switch = false;
	}
}

/**
 * \brief ���������
 *
 * \param argc ��������
 * \param argv �����б�
 * \return ���н��
 */
int main(int argc, char **argv)
{
	Zebra::logger=new zLogger("SuperServer");

	//����ȱʡ����
	Zebra::global["loginServerListFile"] = "SuperServer/loginServerList.xml";
	Zebra::global["mysql"] = "mysql://Zebra:Zebra@192.168.1.162:3306/SuperServer";
	Zebra::global["logfilename"] = "/tmp/superserver.log";

	//���������ļ�����
	SuperConfile sc;
	if (!sc.parse("SuperServer"))
		return EXIT_FAILURE;

	//ָ���⿪��
	if(Zebra::global["cmdswitch"] == "true")
	{
		zTCPTask::analysis._switch = true;
	}
	else
	{
		zTCPTask::analysis._switch = false;
	}
	//���������в���
	zArg::getArg()->add(super_options, super_parse_opt, 0, super_doc);
	zArg::getArg()->parse(argc, argv);
	//Zebra::global.dump(std::cout);

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

	SuperService::getInstance().main();
	SuperService::delInstance();

	return EXIT_SUCCESS;
}

