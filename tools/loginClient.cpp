/**
 * \file
 * \version  $Id: loginClient.cpp $
 * \author  
 * \date 
 * \brief 
 *
 * 
 */


#include "loginClient.h"

bool LoginClient::connect()
{
	if (!zTCPClient::connect())
		return false;
	pSocket->setEncMethod(CEncrypt::ENCDEC_RC5);
	unsigned char key[16] = {28, 196, 25, 36, 193, 125, 86, 197, 35, 92, 194, 41, 31, 240, 37, 223};
	pSocket->set_key_rc5((const unsigned char *)key, 16, 12);
	return true;
}

bool LoginClient::versionCmd()
{
	using namespace Cmd;
	stUserVerifyVerCmd tCmd;
	//tCmd.version = 1999;
	return sendCmd(&tCmd, sizeof(tCmd));
}

bool LoginClient::loginLoginServer(const char *name, const char *passwd)
{
	if(!connect())
		return false;
	if (!versionCmd())
		return false;
	using namespace Cmd;
	stUserRequestLoginCmd tCmd;
	tCmd.game = 0;
	tCmd.zone = atoi(Zebra::global["zone"].c_str());
	strncpy(tCmd.pstrName, name, sizeof(tCmd.pstrName));
	strncpy(tCmd.pstrPassword, passwd, sizeof(tCmd.pstrPassword));
	if (!sendCmd(&tCmd, sizeof(tCmd)))
		return false;


	unsigned char pstrCmd[zSocket::MAX_DATASIZE];
	stServerReturnLoginSuccessCmd *ptCmd = (stServerReturnLoginSuccessCmd *)pstrCmd;
	int nCmdLen = pSocket->recvToCmd(pstrCmd, zSocket::MAX_DATASIZE, true);
	stServerReturnLoginFailedCmd *fail = (stServerReturnLoginFailedCmd*)pstrCmd;
	if (LOGON_USERCMD == fail->byCmd
			&& SERVER_RETURN_LOGIN_FAILED == fail->byParam)
	{
		Zebra::logger->debug("��½������Ϣ(%u, %u), ��ַ:%s, �˿�:%u , �������:%u", ptCmd->dwUserID, ptCmd->loginTempID, pstrIP, wdPort ,fail->byReturnCode);
		msgParse_logon(ptCmd,nCmdLen);
		return false;
	}
	else if (LOGON_USERCMD == ptCmd->byCmd
			&& SERVER_RETURN_LOGIN_OK == ptCmd->byParam) 
	{
		accid = ptCmd->dwUserID;
		loginTempID = ptCmd->loginTempID;
		bcopy(ptCmd->pstrIP, pstrIP, MAX_IP_LENGTH);
		wdPort = ptCmd->wdPort;

		//�õ�des��Կ
		bcopy(&(ptCmd->key[ptCmd->key[58]]), &key_des, sizeof(key_des));
		//Zebra::logger->debug("��½�ɹ�������Ϣ��%u, %u, %s, %u", ptCmd->dwUserID, ptCmd->loginTempID, pstrIP, wdPort);
		//Zebra::logger->debug("�����Կ��%u, %u, %u, %u, %u, %u, %u, %u λ�ã�%u", key_des[0], key_des[1], key_des[2], key_des[3], key_des[4], key_des[5], key_des[6], key_des[7], ptCmd->key[58]);
		return true;
	}
	return false;
}
bool LoginClient::msgParse_logon(const Cmd::stNullUserCmd *ptNull, const unsigned int nCmdLen)
{
	using namespace Cmd;
	switch(ptNull->byParam)
	{
		case SERVER_RETURN_LOGIN_FAILED:
			{
				stServerReturnLoginFailedCmd *ptCmd = (stServerReturnLoginFailedCmd *)ptNull;
				bool retval = false;
				switch(ptCmd->byReturnCode)
				{
					case LOGIN_RETURN_UNKNOWN:
						Zebra::logger->error("%s:δ֪����", __FUNCTION__);
						break;
					case LOGIN_RETURN_VERSIONERROR:
						Zebra::logger->error("%s:�汾����", __FUNCTION__);
						break;
					case LOGIN_RETURN_UUID:
						Zebra::logger->error("%s:UUID��½��ʽû��ʵ��", __FUNCTION__);
						break;
					case LOGIN_RETURN_DB:
						Zebra::logger->error("%s:���ݿ����", __FUNCTION__);
						break;
					case LOGIN_RETURN_PASSWORDERROR:
						Zebra::logger->error("%s:�ʺ��������", __FUNCTION__);
						break;
					case LOGIN_RETURN_CHANGEPASSWORD:
						Zebra::logger->error("%s:�޸�����ɹ�", __FUNCTION__);
						retval = true;
						break;
					case LOGIN_RETURN_IDINUSE:
						Zebra::logger->error("%s:ID���ڱ�ʹ����", __FUNCTION__);
						break;
					case LOGIN_RETURN_IDINCLOSE:
						Zebra::logger->error("%s:ID����", __FUNCTION__);
						break;
					case LOGIN_RETURN_GATEWAYNOTAVAILABLE:
						Zebra::logger->error("%s:���ط�����δ��", __FUNCTION__);
						break;
					case LOGIN_RETURN_USERMAX:
						Zebra::logger->error("%s:�û���", __FUNCTION__);
						break;
					case LOGIN_RETURN_ACCOUNTEXIST:
						Zebra::logger->error("%s:�˺��Ѿ�����", __FUNCTION__);
						break;
					case LOGON_RETURN_ACCOUNTSUCCESS:
						Zebra::logger->error("%s:ע���˺ųɹ�", __FUNCTION__);
						retval = true;
						break;
					case LOGIN_RETURN_USERNAMEREPEAT:
						Zebra::logger->error("%s:�û����ظ�", __FUNCTION__);
						break;
					case LOGIN_RETURN_TIMEOUT:
						Zebra::logger->error("%s:���ӳ�ʱ", __FUNCTION__);
						break;
					case LOGIN_RETURN_PAYFAILED:
						Zebra::logger->error("%s:�Ʒ�ʧ��", __FUNCTION__);
						break;
				}
				return retval;
			}
			break;
	}
	return false;
}
bool LoginClient::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd;
	const stNullUserCmd *ptNull = (const stNullUserCmd *)ptNullCmd;
	switch(ptNull->byCmd)
	{
		case LOGON_USERCMD:
			{
			if (msgParse_logon(ptNull, nCmdLen))
				return true;
			}
			break;
	}
	return false;
}
