/**
 * \file
 * \version  $Id: BillCallback.cpp  $
 * \author  
 * \date 
 * \brief ����Ʒѿͻ��˽ӿ�
 * <p>
 * �ṩ�Ʒѿͻ��˻ص��������Ͷ��壬<br>
 * �Լ��Ʒѿͻ���ģ���ʼ���ͻ��սӿڡ�
 * </p>
 */

#include <cstdio>
#include <cstdlib>

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "BillCache.h"
#include "BillCallback.h"
#include "BillClient.h"
#include "BillClientManager.h"

bool Bill_init(const char *confile, const char *tradelog, struct BillCallback *bc)
{
	Zebra::logger->debug(__PRETTY_FUNCTION__);
	if (NULL == confile
			|| NULL == tradelog
			|| NULL == bc)
	{
		Zebra::logger->debug("��ʼ�����׿ͻ���ʧ��");
		return false;
	}
	BillCache::newInstance();
	BillClientManager::newInstance();
	return BillClientManager::getInstance().init(confile, tradelog, *bc);
}

void Bill_final()
{
	Zebra::logger->debug(__PRETTY_FUNCTION__);
	BillClientManager::delInstance();
	BillCache::delInstance();
}

void Bill_timeAction()
{
	zTime ct;
	BillClientManager::getInstance().timeAction(ct);
	BillCache::getInstance().update(ct);
}

bool Bill_action(BillData *bd)
{
	if (NULL == bd)
		return false;
	else
		return BillClientManager::getInstance().action(bd);
}

