/**
 * \file
 * \version  $Id: zNetService.cpp  $
 * \author  
 * \date 
 * \brief ʵ�����������
 *
 * 
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
#include "Zebra.h"

zNetService *zNetService::instance = NULL;

/**
 * \brief ��ʼ������������
 *
 * ʵ��<code>zService::init</code>���麯��
 *
 * \param port �˿�
 * \return �Ƿ�ɹ�
 */
bool zNetService::init(unsigned short port)
{
	Zebra::logger->trace("zNetService::init");
	if (!zService::init())
		return false;
	
	//��ʼ��������
	tcpServer = new zTCPServer(serviceName);
	if (NULL == tcpServer)
		return false;
	if (!tcpServer->bind(serviceName, port))
		return false;

	Zebra::logger->debug("zNetService::init bind(%s:%u)",serviceName.c_str(),port);
	return true;
}

/**
 * \brief ��������������ص�����
 *
 * ʵ���麯��<code>zService::serviceCallback</code>����Ҫ���ڼ�������˿ڣ��������false���������򣬷���true����ִ�з���
 *
 * \return �ص��Ƿ�ɹ�
 */
bool zNetService::serviceCallback()
{
	Zebra::logger->trace("zNetService::serviceCallback");
	struct sockaddr_in addr;
	int retcode = tcpServer->accept(&addr);
	if (retcode >= 0) 
	{
		//�������ӳɹ�����������
		newTCPTask(retcode, &addr);
	}

	return true;
}

/**
 * \brief �����������������
 *
 * ʵ�ִ��麯��<code>zService::final</code>��������Դ
 *
 */
void zNetService::final()
{
	Zebra::logger->trace("zNetService::final");
	SAFE_DELETE(tcpServer);
}

