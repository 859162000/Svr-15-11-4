/**
 * \file
 * \version  $Id: zService.cpp  $
 * \author  
 * \date 
 * \brief ʵ�ַ����������
 *
 * 
 */


#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
#include <signal.h>
#include <unistd.h>

#include "zSocket.h"
#include "zTCPServer.h"
#include "zService.h"
#include "zProperties.h"
#include "zMisc.h"
#include "Zebra.h"

/**
 * \brief CTRL + C���źŵĴ���������������
 *
 * \param signum �źű��
 */
static void ctrlcHandler(int signum)
{
	Zebra::logger->trace("ctrlcHandler");
	//���û�г�ʼ��zServiceʵ������ʾ����
	zService *instance = zService::serviceInstance();
	instance->Terminate();
}

/**
 * \brief HUP�źŴ�����
 *
 * \param signum �źű��
 */
static void hupHandler(int signum)
{
	Zebra::logger->trace("hupHandler");
	//���û�г�ʼ��zServiceʵ������ʾ����
	zService *instance = zService::serviceInstance();
	instance->reloadConfig();
}

zService *zService::serviceInst = NULL;

/**
 * \brief ��ʼ������������������Ҫʵ���������
 *
 * \return �Ƿ�ɹ�
 */
bool zService::init()
{
	Zebra::logger->trace("zService::init");
	//�洢��������
	int i = 0;
	while(environ[i])
	{
		std::string s(environ[i++]);
		std::vector<std::string> v;
		Zebra::stringtok(v, s, "=", 1);
		if (!v.empty() && v.size() == 2)
			env[v[0]] = v[1];
	}
	//env.dump(std::cout);

	//�����źŴ���
	struct sigaction sig;

	sig.sa_handler = ctrlcHandler;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);
	sigaction(SIGABRT, &sig, NULL);
	sigaction(SIGTERM, &sig, NULL);
	sig.sa_handler = hupHandler;
	sigaction(SIGHUP, &sig, NULL);
	sig.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sig, NULL);

	//��ʼ�������
	srand(time(NULL));
	Zebra::seedp = time(NULL);
	
	return true;
}

/**
 * \brief ��������ܵ�������
 */
void zService::main()
{
	Zebra::logger->trace("zService::main");
	//��ʼ�����򣬲�ȷ�Ϸ����������ɹ�
	if (init()
	&& validate())
	{
		//�������ص��߳�
		while(!isTerminate())
		{
			if (!serviceCallback())
			{
				break;
			}
		}
	}

	//���������ͷ���Ӧ����Դ
	final();
}

