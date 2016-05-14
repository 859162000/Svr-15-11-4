/**
 * \file
 * \version  $Id: TimeTick.cpp $
 * \author  
 * \date 
 * \brief ʱ��ص�����
 *
 * 
 */


#include <iostream>
#include <string>

#include "Zebra.h"
#include "zThread.h"
#include "zTime.h"
#include "TimeTick.h"
#include "LoginSessionManager.h"
#include "GatewayServer.h"
#include "GatewayTaskManager.h"

zRTime GatewayTimeTick::currentTime;
GatewayTimeTick *GatewayTimeTick::instance = NULL;

struct GatewayTaskCheckTime : public GatewayTaskManager::GatewayTaskCallback
{
	bool exec(GatewayTask *gt)
	{
		return gt->checkTime(GatewayTimeTick::currentTime);
	}
};

/**
 * \brief �߳�������
 *
 */
void GatewayTimeTick::run()
{
	while(!isFinal())
	{
		zThread::sleep(1);

		//��ȡ��ǰʱ��
		currentTime.now();

		if (one_second(currentTime) ) {
			LoginSessionManager::getInstance().update(currentTime);

			GatewayTaskCheckTime gtct;
			GatewayTaskManager::getInstance().execAll(gtct);
		}
	}
}

