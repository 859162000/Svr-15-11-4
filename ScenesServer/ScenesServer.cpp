/**
 * \file
 * \version  $Id: ScenesServer.cpp  $
 * \author  
 * \date 
 * \brief zebra��Ŀ��������������Ϸ���󲿷����ݶ��ڱ�ʵ��
 */

#include "zSubNetService.h"
#include "Zebra.h"
#include "SceneManager.h"
#include "SceneTaskManager.h"
#include "zMisc.h"
#include "ScenesServer.h"
#include "SceneTask.h"
#include "RecordClient.h"
#include "SessionClient.h"
#include "zConfile.h"
#include "TimeTick.h"
#include "zDatabaseManager.h"
#include "SceneNpcManager.h"
#include "NpcTrade.h"
//#include "MessageSystem.h"
#include "QuestTable.h"
#include "MagicRange.h"
#include "CountryDare.h"
#include "CountryTech.h"
#include "CountryAlly.h"
#include "GmToolCommand.h"
#include "QuestEvent.h"
//#include "QuestTable.h"
#include "MiniClient.h"

ScenesService *ScenesService::instance = NULL;
bool ScenesService::reload=false;
zLogger * ScenesService::gm_logger = NULL;
zLogger * ScenesService::objlogger = NULL;
zLogger * ScenesService::wg_logger = NULL;

Cmd::stChannelChatUserCmd * ScenesService::pStampData = 0;

/// �й����辭��
unsigned int cancel_country_need_money = 50000; //Ĭ���嶧
unsigned int is_cancel_country = 0; // �Ƿ������ѹ�

//std::string COfflineSkillStatus::rootpath = "";

/**
 * \brief ��ʼ���������������
 *
 * ʵ�����麯��<code>zService::init</code>
 *
 * \return �Ƿ�ɹ�
 */
bool ScenesService::init()
{
	//Zebra::logger->debug(__PRETTY_FUNCTION__);

	for(int i=0; i<13; i++) countryPower[i]=1;

	//��ʼ�������̳߳�
	int state = state_none;
	Zebra::to_lower(Zebra::global["initThreadPoolState"]);
	if ("repair" == Zebra::global["initThreadPoolState"]
			|| "maintain" == Zebra::global["initThreadPoolState"])
		state = state_maintain;
	taskPool = new zTCPTaskPool(atoi(Zebra::global["threadPoolCapacity"].c_str()), state,5000);
	if (NULL == taskPool
			|| !taskPool->init())
		return false;

	strncpy(pstrIP, zSocket::getIPByIfName(Zebra::global["ifname"].c_str()), MAX_IP_LENGTH - 1);
	//Zebra::logger->debug("%s", pstrIP);

	if (!zSubNetService::init())
	{
		return false;
	}

	const Cmd::Super::ServerEntry *serverEntry = NULL;
	//Zebra::logger->debug(__PRETTY_FUNCTION__);

	//���ӵ���������
	serverEntry = getServerEntryByType(RECORDSERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("�����ҵ����������������Ϣ���������ӵ���������");
		return false;
	}
	recordClient = new RecordClient("����������", serverEntry->pstrExtIP, serverEntry->wdExtPort);
	if (NULL == recordClient)
	{
		Zebra::logger->error("û���㹻�ڴ棬���ܽ��������������ͻ���ʵ��");
		return false;
	}
	if (!recordClient->connectToRecordServer())
	{
		Zebra::logger->error("���ӵ���������ʧ�� %s", __PRETTY_FUNCTION__);
		return false;
	}
	if(recordClient->start())
		Zebra::logger->info("��ʼ��Record������ģ��(%s:%ld)...�ɹ�",serverEntry->pstrExtIP,serverEntry->wdExtPort);

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
		return false;
	}
	if(sessionClient->start())
		Zebra::logger->info("��ʼ��Session������ģ��(%s:%ld)...�ɹ�",serverEntry->pstrExtIP,serverEntry->wdExtPort);

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
	if (miniClient->start())
		Zebra::logger->info("��ʼ��Mini������ģ��(%s:%ld)...�ɹ�",serverEntry->pstrExtIP,serverEntry->wdExtPort);

	if (SceneNpcManager::getMe().init())
	{
		Zebra::logger->info("��ʼ��NPC������...�ɹ�");
	}

	if(SceneTimeTick::getInstance().start())
		Zebra::logger->info("��ʼ��TimeTickģ��...�ɹ�");

	//���ػ�������
	if(!loadAllBM())
	{
		Zebra::logger->error("��ʼ����������ģ��...ʧ��");
		return false;
	}
	char srv[256];
	bzero(srv, sizeof(srv));
	sprintf(srv , "WS[%d]",getServerID());
	objlogger = new zLogger(srv);
	objlogger->setLevel(Zebra::global["log"]);
	//����д������־�ļ�
	if ("" != Zebra::global["objlogfilename"])
	{
		bzero(srv, sizeof(srv));
		char sub[256];
		bzero(sub, sizeof(sub));
		snprintf(srv , sizeof(srv),"%s",Zebra::global["objlogfilename"].c_str());
		char *tok = strstr(srv , ".");
		if( tok != NULL)
		{
			strncpy(sub,tok,sizeof(sub));
			bzero(tok , strlen(tok));
			sprintf(srv + strlen(srv) , "%d",getServerID());
			strncat(srv,sub,sizeof(srv) - strlen(srv));
		}
		else
		{
			//strncat(srv,itoa(getServerID()),sizeof(srv) - strlen(srv));
			snprintf(srv + strlen(srv) , sizeof(srv) - strlen(srv) , "%d",getServerID());
		}
		objlogger->addLocalFileLog(srv);
		objlogger->removeConsoleLog();
		//objlogger->debug("asdfsdfasdfasdf");
	}

	gm_logger = new zLogger("gm_log");
	gm_logger->setLevel(Zebra::global["log"]);
	if ("" != Zebra::global["gm_logfile"])
		gm_logger->addLocalFileLog(Zebra::global["gm_logfile"]);
	gm_logger->removeConsoleLog();

	wg_logger = new zLogger("wg_log");
	wg_logger->setLevel(Zebra::global["log"]);
	if ("" != Zebra::global["wg_logfile"])
		wg_logger->addLocalFileLog(Zebra::global["wg_logfile"]);
	wg_logger->removeConsoleLog();

	Zebra::logger->info("�����������ļ�...����С %u", updateStampData());
	/*
	//�Ƿ��Ժ�̨���̵ķ�ʽ����
	if ("true" == Zebra::global["daemon"]) {
		objlogger->removeConsoleLog();
		daemon(1, 1);
	}
	else
		Zebra::logger->info("��ʼ����������ģ��...�ɹ�");
	// */

	if (!SceneManager::getInstance().init())
	{
		Zebra::logger->error("��ʼ������������...ʧ��");
		return false;
	}
	else
		Zebra::logger->info("��ʼ������������...�ɹ�");

	if(!NpcTrade::getInstance().init())
	{
		Zebra::logger->error("��ʼ��NPC��������ģ��...ʧ��");
		return false;
	}

/*	if(!MessageSystem::getInstance().init())
	{
		Zebra::logger->error("��ʼ���Զ���Ϣģ��...ʧ��");
		return false;
	}
	*/

	ALLVARS1(server_id, getServerID());
	ALLVARS(load);

	if(!QuestTable::instance().init())
	{
		Zebra::logger->error("��ʼ������ģ��...ʧ��");
		return false;
	}

//	LuaVM* vm = ScriptingSystemLua::instance().createVM();
	//sppeed up
//	ScriptQuest::get_instance().sort();
	
	if(!MagicRangeInit::getInstance().init())
	{
		Zebra::logger->error("��ʼ��������Χ����ģ��...ʧ��");
		return false;
	}

	CountryDareM::getMe().init();
	CountryTechM::getMe().init();
	CountryAllyM::getMe().init();

/*
	if(!COfflineSkillStatus::init())
	{
		Zebra::logger->error("��ʼ����������״̬��¼ģ��...ʧ��");
		return false;
	}
*/
//	SceneUserVisitor::Thread::instance().start();
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
void ScenesService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	//Zebra::logger->debug(__PRETTY_FUNCTION__);
	SceneTask *tcpTask = new SceneTask(taskPool, sock, addr);
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
 * \brief �������Է�������������ָ��
 *
 * ��Щָ�������غͷ�����������������ָ��<br>
 * ʵ�����麯��<code>zSubNetService::msgParse_SuperService</code>
 *
 * \param ptNullCmd ��������ָ��
 * \param nCmdLen ��������ָ���
 * \return �����Ƿ�ɹ�
 */
bool ScenesService::msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	switch(ptNullCmd->cmd)
	{
		case Cmd::GmTool::CMD_GMTOOL:
			{
				using namespace Cmd::GmTool;

				switch(ptNullCmd->para)
				{
					case PARA_PUNISH_GMTOOL:
						{
							t_Punish_GmTool * rev = (t_Punish_GmTool *)ptNullCmd;
							SceneUser *pUser = SceneUserManager::getMe().getUserByName(rev->userName);
							if (!pUser) break;
							switch (rev->operation)
							{
							case 1://����
								{
									pUser->delayForbidTalk(rev->delay);
									if (rev->delay>0)
									{
										Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�㱻GM���� %d ��", rev->delay);                                               
										ScenesService::gm_logger->trace("��� %s ������ %d ��", pUser->name,rev->delay);
									}
									else
									{
										Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�㱻GM������ԣ����ڿ���˵����");
										ScenesService::gm_logger->trace("��� %s ���������", pUser->name);
									}
								}
								break;
							case 2://�ؽ���
								break;
							case 3://������
								{
									OnQuit event(1);
									EventTable::instance().execute(*pUser, event);

									pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
									//pUser->killAllPets();
									Cmd::Session::t_unregUser_SceneSession ret;
									ret.dwUserID=pUser->id;
									ret.dwSceneTempID=pUser->scene->tempid;
									ret.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
									sessionClient->sendCmd(&ret,sizeof(ret));
									Cmd::Scene::t_Unreg_LoginScene retgate;
									retgate.dwUserID = pUser->id;
									retgate.dwSceneTempID = pUser->scene->tempid;
									retgate.retcode = Cmd::Scene::UNREGUSER_RET_ERROR;
									pUser->gatetask->sendCmd(&retgate,sizeof(retgate));

									pUser->unreg();
								}
								break;
							case 4://����
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, rev->reason);
								}
								break;
							default:
								return true;
							}

							rev->level = pUser->charbase.level;
							rev->accid = pUser->charbase.accid;
							zRTime ct;
							rev->startTime = ct.sec();
							strncpy(rev->country, SceneManager::getInstance().getCountryNameByCountryID(pUser->charbase.country), MAX_NAMESIZE);
							ScenesService::getInstance().sendCmdToSuperServer(rev, sizeof(t_Punish_GmTool));
						}
						break;
					default:
						break;
				}
			}
			break;
		default:
			break;
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
void ScenesService::final()
{

	SceneTimeTick::getInstance().final();
	SceneTimeTick::getInstance().join();
	SceneTimeTick::delInstance();
	SceneUserManager::getMe().removeAllUser();

	if(taskPool)
	{
		taskPool->final();
		SAFE_DELETE(taskPool);
	}
	if (sessionClient)
	{
		sessionClient->final();
		sessionClient->join();
		SAFE_DELETE(sessionClient);
	}

	if (recordClient)
	{
		recordClient->final();
		recordClient->join();
		SAFE_DELETE(recordClient);
	}

	SceneTaskManager::delInstance();

	SceneManager::delInstance();
	GlobalObjectIndex::delInstance();
	//SceneNpcManager::delInstance();
	NpcTrade::delInstance();
	//MessageSystem::delInstance();

	unloadAllBM();
	zSubNetService::final();

	Zebra::logger->debug(__PRETTY_FUNCTION__);
	SAFE_DELETE(gm_logger);
}

/**
 * \brief �����в���
 *
 */
static struct argp_option scenes_options[] =
{
	{"daemon",'d',0,0,"Run service as daemon",0},
	{"log",'l',"level",0,"Log level",0},
	{"logfilename",'f',"filename",0,"Log file name",0},
	{"mysql",'y',"mysql",0,"MySQL[mysql://user:passwd@host:port/dbName]",0},
	{"ifname",'i',"ifname",0,"Local network device",0},
	{"server",'s',"ip",0,"Super server ip address",0},
	{"port",'p',"port",0,"Super server port number",0},
	{"info",'n',0,0,"show info",0},
	{0,0,0,0,0,0}
};

/**
 * \brief �����в���������
 *
 * \param key ������д
 * \param arg ����ֵ
 * \param state ����״̬
 * \return ���ش������
 */
static error_t scenes_parse_opt(int key, char *arg, struct argp_state *state)
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
		case 'n':
			{
				std::cout<<"����ѡ��:\t"<<DEBUG_STRING<<'\n';
				std::cout<<"GMģʽ����:\t"<<Zebra::global["gm_mode"]<<'\n';
				if (!strcmp("_ALL_SUPER_GM", DEBUG_STRING) && Zebra::global["gm_mode"]=="true")
					std::cout<<"���������˶���\tdebug_mode\n";
				else
					std::cout<<"������\t\t����ģʽ\n";
				exit(0);
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
static char scenes_doc[] = "\nScenesServer\n" "\t������������";

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
class SceneConfile:public zConfile
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
void ScenesService::reloadConfig()
{
	reload=true;
	Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
}

void ScenesService::checkAndReloadConfig()
{
	if(reload)
	{
		reload=false;
		Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
		SceneConfile sc;
		sc.parse("ScenesServer");
		loadAllBM();
		NpcTrade::getInstance().init();
		//MessageSystem::getInstance().init();
		//��ʱ�浵����
		if(atoi(Zebra::global["writebacktimer"].c_str()))
		{
			ScenesService::getInstance().writeBackTimer = atoi(Zebra::global["writebacktimer"].c_str());
		}
		else
		{
			ScenesService::getInstance().writeBackTimer = 600;
		}

		if(Zebra::global["double_exp"] == "true")
		{
			ScenesService::getInstance().double_exp = true;
		}
		else
		{
			ScenesService::getInstance().double_exp = false;
		}

		if(Zebra::global["double_drop"] == "true")
		{
			ScenesService::getInstance().double_drop = true;
		}
		else
		{
			ScenesService::getInstance().double_drop = false;
		}

		if(atoi(Zebra::global["level_double_drop"].c_str()))
		{
			ScenesService::getInstance().levelDoubleDrop = atoi(Zebra::global["level_double_drop"].c_str());
		}
		else
		{
			ScenesService::getInstance().levelDoubleDrop = 0;
		}

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
	}
}

/**
 * \brief ���¶�ȡ�������ļ�
 *
 */
DWORD ScenesService::updateStampData()
{
	if (pStampData)
	{
		free(pStampData);
		pStampData = 0;
	}

	int f=0;
	if (Zebra::global["process_file"]!="")
		f = open(Zebra::global["process_file"].c_str(), O_RDONLY);
	else
		f = open("process.dat", O_RDONLY);
	if (f != -1)
	{
		pStampData = (Cmd::stChannelChatUserCmd *)malloc(zSocket::MAX_DATASIZE);
		bzero(pStampData, zSocket::MAX_DATASIZE);
		constructInPlace(pStampData);

		pStampData->dwType = Cmd::CHAT_TYPE_SYSTEM;
		pStampData->dwSysInfoType = Cmd::INFO_TYPE_GAME;
		strncpy(pStampData->pstrChat, "��ӭ��������", MAX_CHATINFO-1);
		pStampData->dwFromID = read(f, (void *)(pStampData->tobject_array), zSocket::MAX_DATASIZE-sizeof(Cmd::stChannelChatUserCmd));
		close(f);

		pStampData->dwChannelID = atoi(Zebra::global["service_flag"].c_str()) & Cmd::Session::SERVICE_PROCESS;
		return pStampData->dwFromID;
	}
	return 0;
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
	Zebra::logger=new zLogger("ScenesServer");
	
	//����ȱʡ����
	Zebra::global["logfilename"] = "/tmp/scenesserver.log";
	Zebra::global["cmdswitch"] = "true";
	Zebra::global["objlogfilename"] = "/tmp/objscenesserver.log";
	Zebra::global["sceneinfofile"] = "ScenesServer/scenesinfo.xml";
	Zebra::global["npctradefile"] = "ScenesServer/npctrade.xml";
	Zebra::global["questdir"] = "quest/";
//	Zebra::global["messageSystemFile"] = "ScenesServer/messageSystem.xml";
	Zebra::global["charinitinfoFile"] = "ScenesServer/initcharinfo.xml";
	Zebra::global["magicrangefile"] = "ScenesServer/magicrangefile.xml";
	Zebra::global["writebacktimer"] = "600";
	Zebra::global["mapdir"] = "map/";
	Zebra::global["tbldir"] = "map/";
	Zebra::global["double_exp"] = "false";
	Zebra::global["mail_service"] = "on";
	Zebra::global["auction_service"] = "on";


	//���������ļ�����
	SceneConfile sc;
	if (!sc.parse("ScenesServer"))
		return EXIT_FAILURE;

	//���������в���
	zArg::getArg()->add(scenes_options, scenes_parse_opt, 0, scenes_doc);
	zArg::getArg()->parse(argc, argv);
	//Zebra::global.dump(std::cout);

	//������־����
	Zebra::logger->setLevel(Zebra::global["log"]);
	//����д������־�ļ�
	if ("" != Zebra::global["logfilename"])
		Zebra::logger->addLocalFileLog(Zebra::global["logfilename"]);

	if(atoi(Zebra::global["writebacktimer"].c_str()))
	{
		ScenesService::getInstance().writeBackTimer = atoi(Zebra::global["writebacktimer"].c_str());
	}
	else
	{
		ScenesService::getInstance().writeBackTimer = 600;
	}

	if(Zebra::global["double_exp"] == "true")
	{
		ScenesService::getInstance().double_exp = true;
	}
	else
	{
		ScenesService::getInstance().double_exp = false;
	}

	if(Zebra::global["double_drop"] == "true")
	{
		ScenesService::getInstance().double_drop = true;
	}
	else
	{
		ScenesService::getInstance().double_drop = false;
	}

	if(atoi(Zebra::global["level_double_drop"].c_str()))
	{
		ScenesService::getInstance().levelDoubleDrop = atoi(Zebra::global["level_double_drop"].c_str());
	}
	else
	{
		ScenesService::getInstance().levelDoubleDrop = 0;
	}

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

	//�Ƿ��Ժ�̨���̵ķ�ʽ����
	if ("true" == Zebra::global["daemon"]) {
		Zebra::logger->info("Program will be run as a daemon");
//		Zebra::logger->removeConsoleLog();
		daemon(1, 1);
	}


	ScenesService::getInstance().main();
	ScenesService::delInstance();
	SceneUserManager::destroyMe();
	return EXIT_SUCCESS;
}
