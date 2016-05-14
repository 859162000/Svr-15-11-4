/**
 * \file
 * \version  $Id: zebraclientService.cpp  $
 * \author  
 * \date
 * \brief 
 *
 * 
 */

#include "zebraclientService.h"
#include "LoadMap.h"
#include "TimeTick.h"
#include "loginClient.h"
#include "zebraclientManager.h"



zebraclientService *zebraclientService::instance = NULL;
bool zebraclientService::loopRun()
{
	if(strcmp(ArrayTable::getInstance()->global["loop_run"].c_str() , "true") == 0)
	{
		//��ʼ������
		if (!zebraclientService::getInstance().init())
			return false;
		return true;
	}
	else
	{
		return false;
	}
}
bool zebraclientService::init()
{
	if(strcmp(ArrayTable::getInstance()->global["mapfile"].c_str() , ""))
	{
		if (!LoadMap((ArrayTable::getInstance()->global["mapfile"] + ".mps").c_str(),zebraClient::allTiles,zebraClient::sceneWH.x,zebraClient::sceneWH.y))
		{
			Zebra::logger->error("���� %s ʧ��",(ArrayTable::getInstance()->global["mapfile"] + ".mps").c_str());
			return false;
		}
	}
	else if (!LoadMap((Zebra::global["mapfile"] + ".mps").c_str(),zebraClient::allTiles,zebraClient::sceneWH.x,zebraClient::sceneWH.y))
	{
		Zebra::logger->error("���� %s ʧ��",(Zebra::global["mapfile"] + ".mps").c_str());
		return false;
	}
	// */
	Zebra::logger->info("���ص�ͼ %s �ɹ�",(Zebra::global["mapfile"] + ".mps").c_str());

	if(!ZebraClientManager::getInstance()->init())
	{
		return false;
	}


	ZebraClientTimeTick::getInstance().start();
	if (!zService::init())
		return false;
	loginClient = new LoginClient("��½������", Zebra::global["server"].c_str(), atoi(Zebra::global["port"].c_str()));
	if(!loginClient)
	{
		return false;
	}
	int first_user=atoi(Zebra::global["user"].c_str());
	int max_user=atoi(Zebra::global["count"].c_str());
	max_user+=first_user; 
	for(int i=first_user;i< max_user;i++)
	{
		char num[32];
		bzero(num,sizeof(num));
		snprintf(num ,sizeof(num), "%d",i);
		std::string temp=num;
		temp += Zebra::global["suffix"].c_str();
		if(!loginClient->loginLoginServer(temp.c_str() , Zebra::global["passwd"].c_str()))
		{
			continue;
		}
		char pstrIP[MAX_IP_LENGTH];
		WORD wdPort;
		bzero(pstrIP, sizeof(pstrIP));
		wdPort = 0;
		DWORD accid=0;
		DWORD loginTempID=0;
		DES_cblock key_des;
		loginClient->getGateInfo(pstrIP,wdPort,accid , loginTempID);
		loginClient->get_key_des((char*)key_des);
		
		/*
		zebraClient *client = new zebraClient("���Կͻ���",pstrIP , wdPort , temp);
		client->connect();
		client->init(accid , loginTempID);
		*/
		ZebraClientManager::getInstance()->addClientTask(new zebraClient("���Կͻ���",
					pstrIP , wdPort , temp) , accid , loginTempID , key_des); 
		Zebra::logger->debug("��½����:%d/%d",i-first_user+1,max_user-first_user);
	}
	return true;
}

bool zebraclientService::serviceCallback()
{
	return true;
}

void zebraclientService::final()
{
}

/*
bool zebraclientService::test()
{
	using namespace Cmd;
	tcpClient = new zebraClient("���Կͻ���", Zebra::global["server"].c_str(), atoi(Zebra::global["port"].c_str()));
	if(!tcpClient->init())
		return false;
	if (NULL == tcpClient)
		return false;

	if (!tcpClient->connect())
		return false;

	if (!tcpClient->versionCmd())
		return false;

	if (!tcpClient->loginLoginServer(Zebra::global["user"].c_str(), Zebra::global["passwd"].c_str()))
		return false;

	tcpClient->close();

	if (!tcpClient->connect(tcpClient->pstrIP, tcpClient->wdPort))
		return false;

	if (!tcpClient->versionCmd())
		return false;

	if (!tcpClient->loginGatewayServer())
		return false;

	if ("true" == Zebra::global["role"])
	{
		stDeleteSelectUserCmd dsu;
		dsu.charNo=0;
		if (!tcpClient->sendCmd(&dsu, sizeof(dsu)))
		{
			return false;
		}
		dsu.charNo=1;
		if (!tcpClient->sendCmd(&dsu, sizeof(dsu)))
		{
			return false;
		}
		char name[32];
		bzero(name ,32);
		memcpy(name, Zebra::global["user"].c_str() , 5);
		std::string role ;
		role =  name;
		role = role + "1";
		if(tcpClient->createRole(role.c_str()))
		{
			::usleep(1000 * 10);
			role =  name;
			role = role + "2";

			return tcpClient->createRole(role.c_str());
		}
		return false;
	}
	else
		return tcpClient->logonRole(0);
}

bool zebraclientService::createAcc()
{
	tcpClient = new zebraClient(Zebra::global["server"].c_str(), atoi(Zebra::global["port"].c_str()));
	if (NULL == tcpClient)
		return false;

	if (!tcpClient->connect())
		return false;

	if (!tcpClient->versionCmd())
		return false;

	if (!tcpClient->createAcc(Zebra::global["user"].c_str(), Zebra::global["passwd"].c_str()))
	{
		Zebra::logger->error("�����˺�ʧ��");
		return false;
	}

	Zebra::logger->debug("�����˺ųɹ�");
	return true;
}

bool zebraclientService::changePasswd()
{
	tcpClient = new zebraClient(Zebra::global["server"].c_str(), atoi(Zebra::global["port"].c_str()));
	if (NULL == tcpClient)
		return false;

	if (!tcpClient->connect())
		return false;

	if (!tcpClient->versionCmd())
		return false;

	if (!tcpClient->changePasswd(Zebra::global["user"].c_str(), Zebra::global["passwd"].c_str(), Zebra::global["newPasswd"].c_str()))
	{
		Zebra::logger->error("�޸�����ʧ��");
		return false;
	}

	Zebra::logger->debug("�޸�����ɹ�");
	return true;
}
*/
static struct argp_option zebraclient_options[] =
{
	{"cacc",		'c',	0,			0,	"Create account",				0},
	{"change",		'g',	0,			0,	"Change password",				0},
	{"role",		'r',	0,			0,	"Create a role",				0},
	{"newPasswd",	'n',	"passwd",	0,	"New password",					0},
	{"passwd",		'P',	"passwd",	0,	"Password",						0},
	{"user",		'u',	"name",		0,	"Account name",					0},
	{"log",			'l',	"level",	0,	"Log level",					0},
	{"server",		's',	"ip",		0,	"Login server ip address",		0},
	{"port",		'p',	"port",		0,	"Login server port number",		0},
	{"zone",		'z',	"zone",		0,	"Game zone number",				0},
	{"count",		'o',	"count",	0,	"User count",		 			0},
	{"suffix",		'f',	"suffix",	0,	"User name suffix",		 		0},
	{0,				0,		0,			0,	0,								0}
};

static error_t zebraclient_parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'c':
			{
				Zebra::global["cacc"]="true";
			}
			break;
		case 'r':
			{
				Zebra::global["role"]="true";
			}
			break;
		case 'P':
			{
				Zebra::global["passwd"]=arg;
			}
			break;
		case 'n':
			{
				Zebra::global["newPasswd"]=arg;
			}
			break;
		case 'u':
			{
				Zebra::global["user"]=arg;
			}
			break;
		case 'g':
			{
				Zebra::global["change"]="true";
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
		case 'z':
			{
				Zebra::global["zone"]=arg;
			}
			break;
		case 'o':
			{
				Zebra::global["count"]=arg;
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
static char zebraclient_doc[] = "\nzebraclient\n" "\t���Կͻ��˳���";

/**
 * \brief ����İ汾��Ϣ
 *
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
	"\nBuild version   :\t" _S(BUILD_STRING);



int main(int argc, char *argv[])
{
	Zebra::logger=new zLogger();
	Zebra::logger->addLocalFileLog("robot");
	Zebra::global["log"] = "debug";
	Zebra::global["port"]="8000";
	Zebra::global["server"]="192.168.16.220";
	Zebra::global["skilltable"] = "skilltable.xml";
	Zebra::global["zone"]="12";
	Zebra::global["user"]="test";
	Zebra::global["suffix"]="@test.com";
	Zebra::global["passwd"]="123456";
	Zebra::global["count"]="1000";
	Zebra::global["mapfile"]="qingyuancun-main";
	Zebra::global["rc5_key"]="_zhengtu_rc5_key";
	ArrayTable::getInstance()->init(0);

	//���������в���
	zArg::getArg()->add(zebraclient_options, zebraclient_parse_opt, 0, zebraclient_doc);
	zArg::getArg()->parse(argc, argv);
	//Zebra::global.dump(std::cout);
	//������־����
	Zebra::logger->setLevel(Zebra::global["log"]);

	LoginClient *loginClient = new LoginClient("��½������", Zebra::global["server"].c_str(), atoi(Zebra::global["port"].c_str()));
	
	if (loginClient->connect())
	{
		if (loginClient->versionCmd())
			Zebra::logger->error("versionCmd OK");
		else
			Zebra::logger->error("versionCmd failure");
		bool blnLogin =loginClient->loginLoginServer(Zebra::global["user"].c_str(),Zebra::global["passwd"].c_str());
		if (blnLogin)
			Zebra::logger->error("Login OK");
		else
			Zebra::logger->error("Login faile");
	}
	
	/*
	//��ʼ������
	if (!zebraclientService::getInstance().init())
		return false;
		
		while(!zebraclientService::getInstance().isTerminate())
		{			
			zThread::msleep(50);
		}
		
	*/
	/*
	if ("true" == Zebra::global["cacc"])
		zebraclientService::getInstance().createAcc();
	else if ("true" == Zebra::global["change"])
		zebraclientService::getInstance().changePasswd();
	else if ("true" == Zebra::global["role"])
		zebraclientService::getInstance().test();
	
	else 
	{
		  while((zebraclientService::getInstance().firstRun() || zebraclientService::getInstance().loopRun())) 
		//&& zebraclientService::getInstance().validate())
		{
		if(zebraclientService::getInstance().validate())
		{
		//ȷ�Ϸ����������ɹ�
		//�������ص��߳�
		while(!zebraclientService::getInstance().isTerminate())
		{
		if (!zebraclientService::getInstance().serviceCallback())
		{
		break;
		}
		}
		}
		//sleep(2);
		//���������ͷ���Ӧ����Դ
		zebraclientService::getInstance().final();
		zebraclientService::delInstance();
		sleep(3);
		}
	}
		 */
	
	//���������ͷ���Ӧ����Դ
	//zebraclientService::getInstance().final();
	//zebraclientService::delInstance();

	return EXIT_SUCCESS;
}
