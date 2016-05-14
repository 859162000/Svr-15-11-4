/**
 * \file
 * \version  $Id: roleRegServer.cpp  $
 * \author  
 * \date 
 * \brief ��ɫΨһ����֤������
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
			Zebra::logger->error("�������ݿ�ʧ�� %s", url.url.c_str());
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
		//�ڴ治�㣬ֱ�ӹر�����
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//�õ���һ����ȷ���ӣ���ӵ���֤������
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
static char dbaccess_doc[] = "\nroleRegServer\n" "\t���ݿ���ʷ�������";

/**
 * \brief ����İ汾��Ϣ
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING);

/**
 * \brief ��ȡ�����ļ�
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
 * \brief ���¶�ȡ�����ļ�,ΪHUP�źŵĴ�����
 */
void roleRegService::reloadConfig()
{
	roleRegConfile sc;
	sc.parse("roleRegServer");
}

/**
 * \brief ���������
 * \param argc ��������
 * \param argv �����б�
 * \return ���н��
 */
int main(int argc, char **argv)
{
	Zebra::logger=new zLogger("roleRegServer");

	//����ȱʡ����
	Zebra::global["client_port"] = "9900";
	Zebra::global["logfilename"] = "/tmp/roleregserver.log";
	Zebra::global["dbCount"] = "16";
	Zebra::global["tableCount"] = "16";

	//���������ļ�����
	roleRegConfile sc;
	if (!sc.parse("roleRegServer"))
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
	
	roleRegService::getInstance().main();
	roleRegService::delInstance();
	
	return EXIT_FAILURE;
}






