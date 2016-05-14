/**
 * \file
 * \version  $Id: LoginManager.cpp  $
 * \author  
 * \date 
 * \brief ��½���ӹ�������
 *
 * 
 */

#include "zMisc.h"
#include "LoginTask.h"
#include "LoginManager.h"
#include "FLCommand.h"
#include "DBAccessCommand.h"
#include "GYListManager.h"
#include "ServerManager.h"

LoginManager *LoginManager::instance = NULL;
DWORD LoginManager::maxGatewayUser=MAX_GATEWAYUSER;

/**
 * \brief �����������һ������
 *
 * \param task һ����������
 * \return ����Ƿ�ɹ�
 */
bool LoginManager::add(LoginTask *task)
{
	Zebra::logger->trace("LoginManager::add");
	if (task)
	{
		task->genTempID();
		mlock.lock();
		LoginTaskHashmap_const_iterator it = loginTaskSet.find(task->getTempID());
		if (it != loginTaskSet.end())
		{
			mlock.unlock();
			return false;
		}
		loginTaskSet.insert(LoginTaskHashmap_pair(task->getTempID(), task));
		mlock.unlock();
		return true;
	}
	else
		return false;
}

/**
 * \brief ��һ���������Ƴ�һ������
 *
 * \param task һ����������
 */
void LoginManager::remove(LoginTask *task)
{
	Zebra::logger->trace("LoginManager::remove");
	if (task)
	{
		mlock.lock();
		loginTaskSet.erase(task->getTempID());
		mlock.unlock();
	}
}

/**
 * \brief �㲥ָ�ָ���ĵ�½����
 *
 * \param loginTempID ��½���ӵ�Ψһ���
 * \param pstrCmd ��ת����ָ��
 * \param nCmdLen ��ת����ָ���
 * \return ת���Ƿ�ɹ�
 */
bool LoginManager::broadcast(const DWORD loginTempID, const void *pstrCmd, int nCmdLen)
{
	Zebra::logger->trace("LoginManager::broadcast");
	zMutex_scope_lock scope_lock(mlock);
	LoginTaskHashmap_iterator it = loginTaskSet.find(loginTempID);
	if (it != loginTaskSet.end())
		return it->second->sendCmd(pstrCmd, nCmdLen);
	else
		return false;
}

/**
 * \brief ��½��֤����
 * \param loginTempID ָ���Ŀͻ���������ʱ���
 * \param retcode �����صĴ���
 * \param session ��½��֤���صĻỰ��Ϣ
 */
void LoginManager::verifyReturn(const DWORD loginTempID, const BYTE retcode, const t_NewLoginSession &session)
{
	Zebra::logger->trace("LoginManager::verifyReturn");
	using namespace Cmd::DBAccess;
	zMutex_scope_lock scope_lock(mlock);

	LoginTaskHashmap_iterator it = loginTaskSet.find(loginTempID);
	if (it != loginTaskSet.end())
	{
		LoginTask *task = it->second;
		switch(retcode)
		{
			case SESSIONCHECK_SUCCESS:
				{
					if (session.state == 0)
					{
						Zebra::logger->debug("��½�ɹ�ֱ�ӷ�������");
						//��½�ɹ�ֱ�ӷ�������
						GYList *gy = GYListManager::getInstance().getAvl(session.gameZone);
						if (NULL == gy)
						{
							task->LoginReturn(Cmd::LOGIN_RETURN_GATEWAYNOTAVAILABLE);
							Zebra::logger->error("����û�п�");
						}
						else if (gy->wdNumOnline >= (maxGatewayUser - 10))
						{
							task->LoginReturn(Cmd::LOGIN_RETURN_USERMAX);
							Zebra::logger->error("�û�����,��ǰ����%d" , gy->wdNumOnline);
						}
						else
						{
							Cmd::FL::t_NewSession_Session tCmd;
							tCmd.session = session;
							tCmd.session.wdGatewayID = gy->wdServerID;

#ifdef _ENCDEC_MSG
							//����des������Կ
							CEncrypt e;
							e.random_key_des(&tCmd.session.des_key);
							Zebra::logger->debug("������Կ:%u %u %u %u %u %u %u %u", tCmd.session.des_key[0], tCmd.session.des_key[1], tCmd.session.des_key[2], tCmd.session.des_key[3], tCmd.session.des_key[4], tCmd.session.des_key[5], tCmd.session.des_key[6], tCmd.session.des_key[7]);
							/*
							   for (int i=0; i<sizeof(tCmd.session.des_key); i++)
							   tCmd.session.des_key[i] = (unsigned int)zMisc::randBetween(0,255);
							 */
#endif

							bcopy(gy->pstrIP, tCmd.session.pstrIP, sizeof(tCmd.session.pstrIP));
							tCmd.session.wdPort = gy->wdPort;
							ServerManager::getInstance().broadcast(session.gameZone, &tCmd, sizeof(tCmd));
						}
					}
					else if (session.state == 1)
					{//�ʺŴ�������״̬
						task->LoginReturn(Cmd::LOGIN_RETURN_LOCK);
						Zebra::logger->error("�ʺ�������");
					}
					else if (session.state == 4)
					{//�ʺŴ��ڴ�����״̬
						task->LoginReturn(Cmd::LOGIN_RETURN_WAITACTIVE);
						Zebra::logger->error("�ʺŴ�����");
					}
				}
				break;
			case SESSIONCHECK_DB_FAILURE:
				{
					Zebra::logger->debug("���ݿ������½ʧ��");
					task->LoginReturn(Cmd::LOGIN_RETURN_DB);
				}
				break;
			case SESSIONCHECK_PWD_FAILURE:
				{
					Zebra::logger->debug("�˺�������󣬵�½ʧ��");
					task->LoginReturn(Cmd::LOGIN_RETURN_PASSWORDERROR);
				}
				break;
		}
	}
}

/**
 * \brief ���ش�����뵽ָ���Ŀͻ���
 * \param loginTempID ָ���Ŀͻ���������ʱ���
 * \param retcode �����صĴ���
 * \param tm ������Ϣ�Ժ��Ƿ�Ͽ����ӣ�ȱʡ�ǶϿ�����
 */
void LoginManager::loginReturn(const DWORD loginTempID, const BYTE retcode, const bool tm)
{
	Zebra::logger->trace("LoginManager::loginReturn");
	mlock.lock();
	LoginTaskHashmap_iterator it = loginTaskSet.find(loginTempID);
	if (it != loginTaskSet.end())
		it->second->LoginReturn(retcode, tm);
	mlock.unlock();
}

/**
 * \brief �������е�����Ԫ�ص��ûص�����
 * \param cb �ص�����ʵ��
 */
void LoginManager::execAll(LoginTaskCallback &cb)
{
	Zebra::logger->trace("LoginManager::execAll");
	zMutex_scope_lock scope_lock(mlock);
	for(LoginTaskHashmap_iterator it = loginTaskSet.begin(); it != loginTaskSet.end(); ++it)
	{
		cb.exec(it->second);
	}
}

