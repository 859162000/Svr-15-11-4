/**
 * \file
 * \version  $Id: LoginTask.cpp  $
 * \author  
 * \date 
 * \brief �����½��������
 *
 */

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <ext/hash_map>

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "LoginTask.h"
#include "LoginManager.h"
#include "Command.h"
#include "zDBConnPool.h"
#include "FLServer.h"
#include "GYListManager.h"
#include "zMisc.h"
#include "FLCommand.h"
#include "ServerManager.h"
#include "DBAccessCommand.h"
#include "DBAccessClient.h"
#include "DBAccessClientManager.h"
#include "zJpeg.h"

DWORD LoginTask::uniqueID = 0;

/**
 * \brief ���캯��
 * \param pool ���������ӳ�
 * \param sock TCP/IP�׽ӿ�
 */
LoginTask::LoginTask( zTCPTaskPool *pool, const int sock) : zTCPTask(pool, sock, NULL, true, false), lifeTime()
{
	Zebra::logger->trace("LoginTask::LoginTask");
	bzero(jpegPassport, sizeof(jpegPassport));
#ifdef _ENCDEC_MSG
	Zebra::logger->debug("LoginTask::LoginTask Have ENCDEC_MSG");
	mSocket.setEncMethod(CEncrypt::ENCDEC_RC5);
	//pSocket.enc.set_key_rc5((const unsigned char *)Zebra::global["rc5_key"].c_str(), 16, 12);
	unsigned char key[16] = {28, 196, 25, 36, 193, 125, 86, 197, 35, 92, 194, 41, 31, 240, 37, 223};
	mSocket.set_key_rc5((const unsigned char *)key, 16, 12);
#else
	Zebra::logger->debug("LoginTask::LoginTask Have NO ENCDEC_MSG");
#endif
}

int LoginTask::verifyConn()
{
	Zebra::logger->trace("LoginTask::verifyConn()");
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//����ֻ�Ǵӻ���ȡ���ݰ������Բ������û������ֱ�ӷ���
			return 0;
		else
		{
			using namespace Cmd;
			Zebra::logger->debug(__PRETTY_FUNCTION__);

			stUserVerifyVerCmd *ptCmd = (stUserVerifyVerCmd *)pstrCmd;
			Zebra::logger->debug("�ͻ���version(%u)",ptCmd->version);
			if (LOGON_USERCMD == ptCmd->byCmd
					&& USER_VERIFY_VER_PARA == ptCmd->byParam)
			{				
				verify_client_version = ptCmd->version;
				Zebra::logger->debug("�ͻ�������ָ����֤ͨ��(%s:%u)",mSocket.getIP(),mSocket.getPort());
				return 1;
			}
			else
			{
				Zebra::logger->error("�ͻ�������ָ����֤ʧ��(%s:%u)",mSocket.getIP(),mSocket.getPort());
				return -1;
			}			
		}
	}
	else
		return retcode;
}

int LoginTask::recycleConn()
{
	Zebra::logger->trace("LoginTask::recycleConn()");
	return 1;
/*
	mutex.lock();
	while(!cmd_queue.empty())
	{
		zSocket::t_BufferCmd *ptCmd = cmd_queue.front();
		int retcode = mSocket.sendRawData_NoPoll(&ptCmd->pstrCmd[ptCmd->offset], ptCmd->nCmdLen - ptCmd->offset);
		if (retcode > 0)
		{
			ptCmd->offset += retcode;
			if (ptCmd->offset < ptCmd->nCmdLen)
				//�������û�з�����ɲ��ܷ�����һ������break
				break;
			else if (ptCmd->offset == ptCmd->nCmdLen)
			{
				//������巢�������continue
				cmd_queue.pop();
				SAFE_DELETE(ptCmd);
			}
#if 0
			else if (ptCmd->offset > ptCmd->nCmdLen)
				//���ش��󣬲����ܳ����������
				assert(0);
#endif
		}
		else if (0 == retcode)
			// should retry
			break;
		else if (-1 == retcode)
		{
			Zebra::logger->error("%s", __PRETTY_FUNCTION__);
			break;
		}
	}
	mutex.unlock();
	return 1;
*/
}

void LoginTask::addToContainer()
{
	Zebra::logger->trace("LoginTask::addToContainer()");
	using namespace Cmd;
	BYTE buf[zSocket::MAX_DATASIZE];
	stJpegPassportUserCmd *cmd = (stJpegPassportUserCmd *)buf;
	constructInPlace(cmd);

	int size = 0;
	void *ret = Zebra::jpegPassport(jpegPassport, sizeof(jpegPassport), &size);
	if (ret)
	{
		if (size >  0 && size <= (int)(zSocket::MAX_DATASIZE - sizeof(stJpegPassportUserCmd) - 100))
		{
			Zebra::logger->debug("����ͼ����֤�룺%s", jpegPassport);
			cmd->size = size;
			bcopy(ret, cmd->data, size);
		}
		free(ret);
	}
	sendCmd(cmd, sizeof(stJpegPassportUserCmd) + cmd->size);
}

bool LoginTask::uniqueAdd()
{
	Zebra::logger->trace("LoginTask::uniqueAdd()");
	return LoginManager::getInstance().add(this);
}

bool LoginTask::uniqueRemove()
{
	Zebra::logger->trace("LoginTask::uniqueRemove");
	LoginManager::getInstance().remove(this);
	return true;
}

bool LoginTask::requestLogin(const Cmd::stUserRequestLoginCmd *ptCmd)
{
	Zebra::logger->trace("LoginTask::requestLogin");
#if 0
	using namespace Cmd;
	using namespace Cmd::DBAccess;

	//������Ψһ���
	GameZone_t gameZone;
	gameZone.game = ptCmd->game;
	gameZone.zone = ptCmd->zone;
	Zebra::logger->debug("�����½��Ϸ����%u(%u, %u), %s", gameZone.id, gameZone.game, gameZone.zone, ptCmd->jpegPassport);

	//��֤�ͻ��˰汾��
	BYTE retcode = LOGIN_RETURN_VERSIONERROR;
	if (GYListManager::getInstance().verifyVer(gameZone, verify_client_version, retcode))
	{
		Zebra::logger->debug("�ͻ�������ͨ���汾����֤");
	}
	else
	{
		Zebra::logger->error("�ͻ�������û��ͨ���汾����֤ , �ͻ��˰汾��:%d",verify_client_version);
		LoginReturn(retcode);
		return false;
	}

	//��֤jpegͼ����֤��
	if (FLService::getInstance().jpeg_passport
			&& strncmp(jpegPassport, ptCmd->jpegPassport, sizeof(jpegPassport)))
	{
		Zebra::logger->error("ͼ����֤�����%s, %s", jpegPassport, ptCmd->jpegPassport);
		LoginReturn(LOGIN_RETURN_JPEG_PASSPORT);
		return false;
	}

	t_LoginServer_SessionCheck tCmd;
	bzero(&tCmd.session, sizeof(tCmd.session));
	tCmd.session.gameZone = gameZone;
	tCmd.session.loginTempID = tempid;
	strncpy(tCmd.session.client_ip, getIP(), MAX_IP_LENGTH);
		
	strncpy(tCmd.session.name, ptCmd->pstrName, sizeof(tCmd.session.name));
	strncpy(tCmd.session.passwd, ptCmd->pstrPassword, sizeof(tCmd.session.passwd));
	if (!DBAccessClientManager::getInstance().broadcast(&tCmd, sizeof(tCmd)))
	{
		LoginReturn(LOGIN_RETURN_DB);
		return false;
	}

	return true;
#endif

#if 1
	using namespace Cmd;
	using namespace Cmd::DBAccess;
	
	//������Ψһ���
	GameZone_t gameZone;
	gameZone.game = ptCmd->game;
	gameZone.zone = ptCmd->zone;
	Zebra::logger->debug("�����½��Ϸ����gameid=%u(game=%u, zone=%u), jpegPassport=%s", gameZone.id, gameZone.game, gameZone.zone, ptCmd->jpegPassport);

	t_LoginServer_SessionCheck tCmd;
	bzero(&tCmd.session, sizeof(tCmd.session));
	tCmd.session.gameZone = gameZone;
	tCmd.session.loginTempID = tempid;
	strncpy(tCmd.session.client_ip, getIP(), MAX_IP_LENGTH);		
	strncpy(tCmd.session.name, ptCmd->pstrName, sizeof(tCmd.session.name));
	strncpy(tCmd.session.passwd, ptCmd->pstrPassword, sizeof(tCmd.session.passwd));
	

	//��֤�ͻ��˰汾��
	/*
	BYTE retcode = LOGIN_RETURN_VERSIONERROR;
	if (GYListManager::getInstance().verifyVer(gameZone, verify_client_version, retcode))
	{
		Zebra::logger->debug("�ͻ�������ͨ���汾����֤");
	}
	else
	{
		Zebra::logger->error("�ͻ�������û��ͨ���汾����֤ , �ͻ��˰汾��:%d",verify_client_version);
		LoginReturn(retcode);
		return false;
	}*/

	

	//��֤jpegͼ����֤��
	if (FLService::getInstance().jpeg_passport
			&& strncmp(jpegPassport, ptCmd->jpegPassport, sizeof(jpegPassport)))
	{
		Zebra::logger->error("ͼ����֤�����%s, %s", jpegPassport, ptCmd->jpegPassport);
		LoginReturn(LOGIN_RETURN_JPEG_PASSPORT);
		return false;
	}

	

	//��֤�û����ƺ�����Ϸ���
	if (strlen(ptCmd->pstrName) == 0
			|| strlen(ptCmd->pstrName) >= MAX_NAMESIZE
			|| strlen(ptCmd->pstrPassword) == 0
			|| strlen(ptCmd->pstrPassword) >= MAX_PASSWORD)
	{
		LoginReturn(LOGIN_RETURN_PASSWORDERROR);
		return false;
	}

	static const dbCol verifylogin_define[] = {
		{ "ISUSED", zDBConnPool::DB_BYTE, sizeof(BYTE) }, 
		{ "ISFORBID", zDBConnPool::DB_BYTE, sizeof(BYTE) }, 
		{ "USERID", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
		{ "PASSWORD", zDBConnPool::DB_STR, sizeof(char[MAX_PASSWORD]) }, 
		{ "LOGINID", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE]) },
		{ NULL, 0, 0}
	};
	struct {
		BYTE isUsed, isForbid;
		DWORD accid;
		char pstrPassword[MAX_PASSWORD];
		char pstrName[MAX_NAMESIZE];
	} __attribute__ ((packed))
	data;
	char where[128];

	//��֤�û��˺ź�����
	bzero(&data, sizeof(data));
	connHandleID handle = FLService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		LoginReturn(LOGIN_RETURN_DB);
		return false;
	}
	Zebra::logger->debug("�û� %s �ַ�ID��½", ptCmd->pstrName);
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "LOGINID = '%s'", ptCmd->pstrName);
	if (FLService::dbConnPool->exeSelectLimit(handle, "`LOGIN`", verifylogin_define, where, NULL, 1, (BYTE *)(&data)) != 1)
	{
		FLService::dbConnPool->putHandle(handle);
		LoginReturn(LOGIN_RETURN_PASSWORDERROR);
		Zebra::logger->error("û���ҵ���¼");
		tCmd.session.state = 4;
		//return false;
	}
	if (strcmp(data.pstrPassword, ptCmd->pstrPassword) //�ȶ�����
			|| strcmp(data.pstrName, ptCmd->pstrName))
	{
		FLService::dbConnPool->putHandle(handle);
		LoginReturn(LOGIN_RETURN_PASSWORDERROR);
		Zebra::logger->error("������󣬲��ܵ�½");		
		return false;
	}
	FLService::dbConnPool->updateDatatimeCol(handle, "`LOGIN`", "`LASTACTIVEDATE`");
	FLService::dbConnPool->putHandle(handle);

	

	//�˺��Ѿ���ʹ����
	if (data.isUsed)
	{
		LoginReturn(LOGIN_RETURN_IDINUSE);
		Zebra::logger->error("�˺�����ʹ����");		
		return false;
	}
	// �˺��Ѿ�����ֹ
	if (data.isForbid)
	{
		LoginReturn(LOGIN_RETURN_IDINCLOSE);
		Zebra::logger->error("�˺��Ѿ�����");
		tCmd.session.state = 1;
		//return false;
	}

	tCmd.retcode = SESSIONCHECK_SUCCESS;
	tCmd.session.state = 0;
	tCmd.session.accid = data.accid;
	LoginManager::getInstance().verifyReturn(tCmd.session.loginTempID, tCmd.retcode, tCmd.session);
	
	return true;	
	
#endif
}

bool LoginTask::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("LoginTask::msgParse");
	using namespace Cmd;
	stLogonUserCmd *logonCmd = (stLogonUserCmd *)ptNullCmd;

	if (LOGON_USERCMD == logonCmd->byCmd)
	{
		switch(logonCmd->byParam)
		{
			case USER_REQUEST_LOGIN_PARA:
				{
					stUserRequestLoginCmd *ptCmd = (stUserRequestLoginCmd *)logonCmd;
					if (requestLogin(ptCmd))
					{
						return true;
					}
				}
				break;
		}
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

