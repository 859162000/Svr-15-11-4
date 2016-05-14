/**
 * \file
 * \version  $Id: zHttpTask.cpp  $
 * \author  
 * \date 
 * \brief ����ʵ��������(lightweight)��http������
 */


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>

#include "zSocket.h"
#include "zThread.h"
#include "zHttpTask.h"
#include "zHttpTaskPool.h"
#include "Zebra.h"

/**
 * \brief ���׽ӿڷ���ָ��
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��Ĵ�С
 * \return �����Ƿ�ɹ�
 */
bool zHttpTask::sendCmd(const void *pstrCmd, int nCmdLen)
{
	if (pSocket) 
		return pSocket->sendCmdNoPack(pstrCmd, nCmdLen);
	else
		return false;
}

