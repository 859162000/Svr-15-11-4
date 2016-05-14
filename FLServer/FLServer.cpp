/**
 * \file
 * \version  $Id: FLServer.cpp  $
 * \author  
 * \date 
 * \brief zebra��Ŀ��½�������������½�������ʺš������ȹ���
 *
 */

#include "Zebra.h"
#include "zMisc.h"
#include "FLServer.h"
#include "zMNetService.h"
#include "LoginTask.h"
#include "LoginManager.h"
#include "zArg.h"
#include "zConfile.h"
#include "GYListManager.h"
#include "Command.h"
#include "ServerTask.h"
#include "ServerManager.h"
#include "PingTask.h"
#include "ServerACL.h"
#include "DBAccessClient.h"
#include "DBAccessClientManager.h"
#include "TimeTick.h"
#include "InfoClient.h"
#include "InfoClientManager.h"


zDBConnPool * FLService::dbConnPool = NULL;
FLService *FLService::instance = NULL;

/**
 * \brief ��ʼ���������������
 *
 * ʵ�����麯��<code>zService::init</code>
 *
 * \return �Ƿ�ɹ�
 */
bool FLService::init()
{
	Zebra::logger->trace("FLService::init");	
#if 1
	dbConnPool = zDBConnPool::newInstance(NULL);

	if (NULL == dbConnPool
			|| !dbConnPool->putURL(0, Zebra::global["mysql"].c_str(), false))
	{
		Zebra::logger->error("�������ݿ�ʧ�� %s", Zebra::global["mysql"].c_str());
		return false;
	}
#endif

	/*
	if (!DBAccessClientManager::getInstance().init())
		return false;
	*/
	if (!zMNetService::init())
	{
		return false;
	}

	
	if (!ServerACLSingleton::instance().init())
		return false;

	if (!InfoClientManager::getInstance().init())
		return false;
	

	//��ʼ�������̳߳�
	int state = state_none;
	Zebra::to_lower(Zebra::global["initThreadPoolState"]);
	if ("repair" == Zebra::global["initThreadPoolState"]
			|| "maintain" == Zebra::global["initThreadPoolState"])
		state = state_maintain;
	
	loginTaskPool = new zTCPTaskPool(atoi(Zebra::global["threadPoolCapacity"].c_str()), state);
	if (NULL == loginTaskPool
			|| !loginTaskPool->init())
		return false;
	
	serverTaskPool = new zTCPTaskPool(atoi(Zebra::global["threadPoolCapacity"].c_str()), state);
	if (NULL == serverTaskPool
			|| !serverTaskPool->init())
		return false;
	
	pingTaskPool = new zTCPTaskPool(atoi(Zebra::global["threadPoolCapacity"].c_str()), state);
	if (NULL == pingTaskPool
			|| !pingTaskPool->init())
		return false;

	login_port = atoi(Zebra::global["login_port"].c_str());
	inside_port = atoi(Zebra::global["inside_port"].c_str());
	ping_port  = atoi(Zebra::global["ping_port"].c_str());
	
	if (!zMNetService::bind("��½�˿�", login_port)
			|| !zMNetService::bind("�ڲ�����˿�", inside_port)
			|| !zMNetService::bind("PING�˿�", ping_port))
	{
		return false;
	}

	FLTimeTick::getInstance().start();

#if 0
	GameZone_t zone;
	Zebra::logger->debug("sizeof(GameZone_t) = %u", sizeof(GameZone_t));
	zone.id = 0x0000ffff;
	Zebra::logger->debug("id = 0x%08x, game = 0x%04x, zone = 0x%04x", zone.id, zone.game, zone.zone);
#endif

	return true;
}

/**
 * \brief �½���һ����������
 * ʵ�ִ��麯��<code>zMNetService::newTCPTask</code>
 * \param sock TCP/IP����
 * \param srcPort ������Դ�˿�
 * \return �µ���������
 */
void FLService::newTCPTask(const int sock, const unsigned short srcPort)
{	
	Zebra::logger->trace("FLService::newTCPTask");
	
	if (srcPort == login_port)
	{
		//�ͻ��˵�½��֤����
		zTCPTask *tcpTask = new LoginTask(loginTaskPool, sock);
		if (NULL == tcpTask)
			TEMP_FAILURE_RETRY(::close(sock));
		else if(!loginTaskPool->addVerify(tcpTask))
		{
			SAFE_DELETE(tcpTask);
		}
	}
	else if (srcPort == inside_port)
	{
		//ÿ�����ķ���������������
		zTCPTask *tcpTask = new ServerTask(serverTaskPool, sock);
		if (NULL == tcpTask)
			TEMP_FAILURE_RETRY(::close(sock));
		else if(!serverTaskPool->addVerify(tcpTask))
		{
			SAFE_DELETE(tcpTask);
		}
	}
	else if(srcPort == ping_port)
	{
		// ��ȡPING�������б�
		zTCPTask *tcpTask = new PingTask(serverTaskPool, sock);
		if (NULL == tcpTask)
			TEMP_FAILURE_RETRY(::close(sock));
		else if(!pingTaskPool->addVerify(tcpTask))
		{
			SAFE_DELETE(tcpTask);
		}
	}
	else
		TEMP_FAILURE_RETRY(::close(sock));
}

/**
 * \brief �������������
 *
 * ʵ���˴��麯��<code>zService::final</code>
 *
 */
void FLService::final()
{
	Zebra::logger->trace("FLService::final");
	zMNetService::final();

	ServerManager::delInstance();
	LoginManager::delInstance();
	GYListManager::delInstance();

	FLTimeTick::getInstance().final();
	FLTimeTick::getInstance().join();
	FLTimeTick::delInstance();

	InfoClientManager::delInstance();

	//DBAccessClientManager::delInstance();

}

/**
 * \brief �����в���
 *
 */
static struct argp_option login_options[] =
{
	{"daemon",		'd',	0,			0,	"Run service as daemon",						0},
	{"log",			'l',	"level",	0,	"Log level",									0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",								0},
	{"ifname",		'i',	"ifname",	0,	"Local network device",							0},
	{"server",		's',	"ip",		0,	"Super server ip address",						0},
	{"port",		'p',	"port",		0,	"Super server port number",						0},
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
static error_t login_parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'd':
			{
				Zebra::global["daemon"] = "true";
			}
			break;
		case 'p':
			{
				Zebra::global["port"]=arg;
			}
			break;
		case 's':
			{
				Zebra::global["server"]=arg;
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
static char login_doc[] = "\nFLServer\n" "\t��½��������";

/**
 * \brief ����İ汾��Ϣ
 *
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING);

/**
 * \brief ��ȡ�����ļ�
 *
 */
class LoginConfile:public zConfile
{
	bool parseYour(const xmlNodePtr node)
	{
		if (node)
		{
			xmlNodePtr child=parser.getChildNode(node,NULL);
			while(child)
			{
				if(strcasecmp((char *)child->name,"InfoServer")==0)
				{
					char buf[64];
					if(parser.getNodeContentStr(child,buf,64))
					{
						Zebra::global["InfoServer"]=buf;
						if(parser.getNodePropStr(child,"port",buf,64))
							Zebra::global["InfoPort"]=buf;
					}
				}
				else
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
void FLService::reloadConfig()
{
	Zebra::logger->trace("FLService::reloadConfig");
	LoginConfile sc;
	sc.parse("FLServer");
	if ("true" == Zebra::global["jpeg_passport"])
		FLService::getInstance().jpeg_passport = true;
	else
		FLService::getInstance().jpeg_passport = false;
	ServerACLSingleton::instance().init();
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
	Zebra::logger=new zLogger("FLServer");

	//����ȱʡ����
	Zebra::global["login_port"] = "7000";
	Zebra::global["inside_port"] = "7001";
	Zebra::global["ping_port"] = "7002";
	Zebra::global["dbAccessServerListFile"] = "FLServer/dbAccessServerList.xml";
	Zebra::global["log"] = "debug";
	Zebra::global["logfilename"] = "/tmp/flserver.log";
	Zebra::global["zoneInfoFile"] = "FLServer/zoneInfo.xml";
	Zebra::global["InfoServer"]="192.168.2.11";
	Zebra::global["InfoPort"]="9903";
	Zebra::global["jpeg_passport"]="true";

	//���������ļ�����
	LoginConfile sc;
	if (!sc.parse("FLServer"))
		return EXIT_FAILURE;
	if ("true" == Zebra::global["jpeg_passport"])
		FLService::getInstance().jpeg_passport = true;
	else
		FLService::getInstance().jpeg_passport = false;
	if(atoi(Zebra::global["maxGatewayUser"].c_str()))
	{
		LoginManager::maxGatewayUser = atoi(Zebra::global["maxGatewayUser"].c_str());
	}

	//���������в���
	zArg::getArg()->add(login_options, login_parse_opt, 0, login_doc);
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

	FLService::getInstance().main();
	FLService::delInstance();

	return EXIT_SUCCESS;
}

