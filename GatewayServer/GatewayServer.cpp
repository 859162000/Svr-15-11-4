/**
 * \file
 * \version  $Id: GatewayServer.cpp $
 * \author  
 * \date
 * \brief zebra��ĿGateway������,�����û�ָ����ת�������ܽ��ܵ�
 */

#include "zSubNetService.h"
#include "Zebra.h"
#include "zMisc.h"
#include "GatewayServer.h"
#include "GatewayTask.h"
#include "BillClient.h"
#include "SceneClient.h"
#include "RecordClient.h"
#include "GateUserManager.h"
#include "zConfile.h"
#include "LoginSessionManager.h"
#include "GatewayTaskManager.h"
#include "TimeTick.h"
#include "RoleregCommand.h"
#include "RecordClient.h"
#include "MiniClient.h"
#include "zXMLParser.h"
#include "SceneClientManager.h"

GatewayService *GatewayService::instance = NULL;
zTCPTaskPool * GatewayService::taskPool = NULL;
bool GatewayService::service_gold=true;
bool GatewayService::service_stock=true;
unsigned int merge_version = 0;

/**
 * \brief ��ʼ���������������
 *
 * ʵ�����麯��<code>zService::init</code>
 *
 * \return �Ƿ�ɹ�
 */
bool GatewayService::init()
{
	Zebra::logger->trace("GatewayService::init");
	verify_client_version = atoi(VERSION_STRING);
	Zebra::logger->info("�������汾��:%d",verify_client_version);
	//�����������Ҫ��У�԰汾�Ͳ��԰汾
	/*
	if(verify_client_version==0)
		verify_client_version=Cmd::GAME_VERSION;
	// */	
	//Zebra::logger->debug(__PRETTY_FUNCTION__);
	
	//���ع�������(��ͼ)��Ϣ
	if(!country_info.init())
	{
		Zebra::logger->error("���ص�ͼ����ʧ��!");
	}

	//��ʼ�������̳߳�
	int state = state_none;
	Zebra::to_lower(Zebra::global["initThreadPoolState"]);
	if ("repair" == Zebra::global["initThreadPoolState"]
			|| "maintain" == Zebra::global["initThreadPoolState"])
		state = state_maintain;

	taskPool = new zTCPTaskPool(4096/*atoi(Zebra::global["threadPoolCapacity"].c_str())*/, state, 65000);
	if (NULL == taskPool
			|| !taskPool->init())
		return false;

	strncpy(pstrIP, zSocket::getIPByIfName(Zebra::global["ifname"].c_str()), MAX_IP_LENGTH - 1);
	Zebra::logger->debug("GatewayService::init() %s", pstrIP);

	if (!zSubNetService::init())
	{
		return false;
	}

	const Cmd::Super::ServerEntry *serverEntry = NULL;
	
	//����Session������
	serverEntry = getServerEntryByType(SESSIONSERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("�����ҵ�Session�����������Ϣ����������Session������");
		return false;
	}
	sessionClient = new SessionClient("Session������", serverEntry->pstrExtIP, serverEntry->wdExtPort);
	if (NULL == sessionClient)
	{
		Zebra::logger->error("û���㹻�ڴ棬���ܽ���Session�������ͻ���ʵ��");
		return false;
	}
	if (!sessionClient->connectToSessionServer())
	{
		Zebra::logger->error("����Session������ʧ�� %s", __PRETTY_FUNCTION__);
		//return false;
	}
	sessionClient->start();

	//���ӼƷѷ�����
	serverEntry = getServerEntryByType(BILLSERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("�����ҵ��Ʒѷ����������Ϣ���������ӼƷѷ�����");
		return false;
	}
	accountClient = new BillClient("�Ʒѷ�����", serverEntry->pstrExtIP, serverEntry->wdExtPort,serverEntry->wdServerID);
	if (NULL == accountClient)
	{
		Zebra::logger->error("û���㹻�ڴ棬���ܽ����Ʒѷ������ͻ���ʵ��");
		return false;
	}
	if (!accountClient->connectToBillServer())
	{
		Zebra::logger->error("���ӼƷѷ�����ʧ�� %s", __PRETTY_FUNCTION__);
		return false;
	}
	accountClient->start();

	//�������еĳ���������
	serverEntry = getServerEntryByType(SCENESSERVER);
	if(serverEntry)
	{
		if (!SceneClientManager::getInstance().init())
			return false;
	}	

	//�������еĵ���������
	serverEntry = getServerEntryByType(RECORDSERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("�����ҵ�Record�����������Ϣ����������Record������");
		return false;
	}
	recordClient = new RecordClient("Record������", serverEntry->pstrExtIP, serverEntry->wdExtPort);
	if (NULL == recordClient)
	{
		Zebra::logger->error("û���㹻�ڴ棬���ܽ���Record�������ͻ���ʵ��");
		return false;
	}
	if (!recordClient->connectToRecordServer())
	{
		Zebra::logger->error("����Record������ʧ�� %s", __PRETTY_FUNCTION__);
		return false;
	}
	recordClient->start();

	//����С��Ϸ������
	serverEntry = getServerEntryByType(MINISERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("�����ҵ�С��Ϸ�����������Ϣ����������С��Ϸ������");
		return false;
	}
	miniClient = new MiniClient("С��Ϸ������", serverEntry->pstrExtIP, serverEntry->wdExtPort,serverEntry->wdServerID);
	if (NULL == miniClient)
	{
		Zebra::logger->error("û���㹻�ڴ棬���ܽ���С��Ϸ�������ͻ���ʵ��");
		return false;
	}
	if (!miniClient->connectToMiniServer())
	{
		Zebra::logger->error("����С��Ϸ������ʧ�� %s", __PRETTY_FUNCTION__);
		return false;
	}
	miniClient->start();

	if(!GateUserManager::getInstance()->init())
		return false;

	GatewayTimeTick::getInstance().start();

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
void GatewayService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	Zebra::logger->trace("GatewayService::newTCPTask");
	GatewayTask *tcpTask = new GatewayTask(taskPool, sock, addr);
	if (NULL == tcpTask)
		//�ڴ治�㣬ֱ�ӹر�����
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//�õ���һ����ȷ���ӣ���ӵ���֤������
		SAFE_DELETE(tcpTask);
	}
}

bool GatewayService::notifyLoginServer()
{
	Zebra::logger->trace("GatewayService::notifyLoginServer");
	using namespace Cmd::Super;
	t_GYList_Gateway tCmd;

	tCmd.wdServerID = wdServerID;
	bcopy(pstrExtIP, tCmd.pstrIP, sizeof(tCmd.pstrIP));
	tCmd.wdPort = wdExtPort;
	if(!GatewayService::getInstance().isTerminate())
	{
		tCmd.wdNumOnline = getPoolSize();
		Zebra::logger->debug("����Ŀǰ��������:%d",tCmd.wdNumOnline);
	}
	else
	{
		tCmd.wdNumOnline = 0;
	}
	tCmd.state = getPoolState();
	tCmd.zoneGameVersion = verify_client_version;

	return sendCmdToSuperServer(&tCmd, sizeof(tCmd));
}

/**
 * \brief �������Է�������������ָ��
 *
 * ��Щָ�������غͷ�����������������ָ��<br>
 * ʵ�����麯��<code>zSubNetService::msgParse_SuperService</code>
 *
 * \param ptNullCmd ��������ָ��
 * \param nCmdLen ��������ָ���
 * \return �����Ƿ�ɹ�
 */
bool GatewayService::msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("GatewayService::msgParse_SuperService");
	using namespace Cmd::Super;

	if (CMD_GATEWAY == ptNullCmd->cmd)
	{
		switch(ptNullCmd->para)
		{
			case PARA_GATEWAY_RQGYLIST:
				{
					//t_GYList_Gateway *ptCmd = (t_GYList_Gateway *)ptNullCmd;

					return notifyLoginServer();
				}
				break;
			case PARA_CHARNAME_GATEWAY:
				{
					using namespace Cmd::RoleReg;
					t_Charname_Gateway *rev = (t_Charname_Gateway *)ptNullCmd;
					GateUser *pUser=GateUserManager::getInstance()->getUserByAccID(rev->accid);
					if(pUser
							&& pUser->isCreateState()
							&& rev->state & ROLEREG_STATE_TEST)
					{
						if (rev->state & ROLEREG_STATE_HAS)
						{
							//������ɫʧ�ܣ���ɫ�����ظ�
							pUser->nameRepeat();
							Zebra::logger->trace("��ɫ���ظ� GatewayService::msgParse_SuperService");
						}
						else
						{
							if (!recordClient->sendCmd(&pUser->createCharCmd,sizeof(pUser->createCharCmd)))
								return false;
						}
					}

					return true;
				}
				break;
		}
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief �������������
 *
 * ʵ���˴��麯��<code>zService::final</code>
 *
 */
void GatewayService::final()
{
	Zebra::logger->trace("GatewayService::final");
	GatewayTimeTick::getInstance().final();
	GatewayTimeTick::getInstance().join();
	GatewayTimeTick::delInstance();
	GateUserManager::getInstance()->removeAllUser(); 

	if(taskPool)
	{
		taskPool->final();
		SAFE_DELETE(taskPool);
	}
	//�������taskPool֮����,�����down��
	//SceneClientManager::getInstance().final();
	SceneClientManager::delInstance();
	// */
	if(sessionClient)
	{
		sessionClient->final();
		sessionClient->join();
		SAFE_DELETE(sessionClient);
	}
	if(recordClient)
	{
		recordClient->final();
		recordClient->join();
		SAFE_DELETE(recordClient);
	}
	zSubNetService::final();

	GatewayTaskManager::delInstance();

	LoginSessionManager::delInstance();

	GateUserManager::delInstance();

}

/**
 * \brief �����в���
 *
 */
static struct argp_option gateway_options[] =
{
	{"daemon",		'd',	0,			0,	"Run service as daemon",						0},
	{"log",			'l',	"level",	0,	"Log level",									0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",								0},
	{"mysql",		'y',	"mysql",	0,	"MySQL[mysql://user:passwd@host:port/dbName]",	0},
	{"ifname",		'i',	"ifname",	0,	"Local network device",							0},
	{"server",		's',	"ip",		0,	"Super server ip address",						0},
	{"port",		'p',	"port",		0,	"Super server port number",						0},
	{"maintain",	'm',	0,			0,	"Run service as maintain mode",					0},
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
static error_t gateway_parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'd':
			{
				Zebra::global["daemon"] = "true";
			}
			break;
		case 'm':
			{
				Zebra::global["initThreadPoolState"]="maintain";
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
static char gateway_doc[] = "\nGatewayServer\n" "\t���ط�������";

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
class GatewayConfile:public zConfile
{
	bool parseYour(const xmlNodePtr node)
	{
		if (node)
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
void GatewayService::reloadConfig()
{
	Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
	GatewayConfile gc;
	gc.parse("GatewayServer");
	if ("1" == Zebra::global["rolereg_verify"])
		GatewayService::getInstance().rolereg_verify = true;
	else
		GatewayService::getInstance().rolereg_verify = false;
	
	//ָ���⿪��
	if(Zebra::global["cmdswitch"] == "true")
	{
		zTCPTask::analysis._switch = true;
		zTCPClient::analysis._switch=true;
	}
	else
	{
		zTCPTask::analysis._switch = false;
		zTCPClient::analysis._switch=false;
	}
	
	if(!country_info.reload())
	{
		Zebra::logger->error("���¼��ع�������!");
	}

	merge_version = atoi(Zebra::global["merge_version"].c_str());
	#ifdef _ZJW_DEBUG
	Zebra::logger->debug("[����]: ���¼��غ����汾��", merge_version);
	#endif	
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
	Zebra::logger=new zLogger("GatewayServer");

	//����ȱʡ����
	Zebra::global["logfilename"] = "/tmp/gatewayserver.log";
	Zebra::global["sceneinfofile"] = "ScenesServer/scenesinfo.xml";
	Zebra::global["countryorder"] = "0";

	//���������ļ�����
	GatewayConfile gc;
	if (!gc.parse("GatewayServer"))
		return EXIT_FAILURE;

	//Zebra::logger->debug("%s", Zebra::global["rolereg_verify"].c_str());
	if ("1" == Zebra::global["rolereg_verify"])
		GatewayService::getInstance().rolereg_verify = true;
	else
		GatewayService::getInstance().rolereg_verify = false;
	//ָ���⿪��
	if(Zebra::global["cmdswitch"] == "true")
	{
		zTCPTask::analysis._switch = true;
		zTCPClient::analysis._switch=true;
	}
	else
	{
		zTCPTask::analysis._switch = false;
		zTCPClient::analysis._switch=false;
	}

	merge_version = atoi(Zebra::global["merge_version"].c_str());

	//���������в���
	zArg::getArg()->add(gateway_options, gateway_parse_opt, 0, gateway_doc);
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

	GatewayService::getInstance().main();
	//GatewayService::delInstance();

	return EXIT_SUCCESS;
}

