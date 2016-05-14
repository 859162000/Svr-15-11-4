/**
 * \file
 * \version  $Id: GatewayTask.cpp $
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
#include "GatewayTask.h"
#include "GatewayServer.h"
#include "Command.h"
#include "GmToolCommand.h"
#include "SessionClient.h"
#include "LoginSessionManager.h"
#include "RecordClient.h"
#include "SceneCommand.h"
#include "MiniCommand.h"
#include "MiniUserCommand.h"
#include "SceneClient.h"
#include "MiniClient.h"
#include "GatewayTaskManager.h"
#include "BillCommand.h"
#include "BillClient.h"
#include "TimeTick.h"
#include "RoleregCommand.h"
#include "wordFilter.h"
#include "CountryInfo.h"

#include "EncDec/EncDec.h"
extern unsigned int merge_version;

#define checkTimeInterval 30000
/**
 * \brief ���캯��
 *
 * \param pool �������ӳ�ָ��
 * \param sock TCP/IP�׽ӿ�
 * \param addr ��ַ
 */
GatewayTask::GatewayTask(
		zTCPTaskPool *pool,
		const int sock,
		const struct sockaddr_in *addr) :
	zTCPTask(pool, sock, addr, true, false),
	//v_lastSampleTime(GatewayTimeTick::currentTime),
	_retset_gametime(3600), 
	initTime(),
	lastCheckTime(GatewayTimeTick::currentTime)
{
	
	Zebra::logger->trace("GatewayTask::GatewayTask");
	recycle_wait = 5000;
	//v_lastSampleTime.addDelay(sampleInterval);
	v_lastSampleTime=0;
	lastCheckTime.addDelay(checkTimeInterval);
	haveCheckTime = true;
	v_samplePackets = 0;
	accid = 0;
	loginTempID = 0;
	versionVerified = false;
	dwTimestampServer=0;
	qwGameTime=0;
	accountVerified = ACCOUNTVERIFY_NONE;
	pUser=NULL;
	vip_user=true;
	nextChatTime.now();
	nextCountryChatTime.now();
	bzero(numPassword,sizeof(numPassword));
	numPwd = 0;

	mSocket.setEncMethod(CEncrypt::ENCDEC_RC5);
	//mSocket.enc.set_key_rc5((const unsigned char *)Zebra::global["rc5_key"].c_str(), 16, 12);
	unsigned char key[16] = {28, 196, 25, 36, 193, 125, 86, 197, 35, 92, 194, 41, 31, 240, 37, 223};
	mSocket.set_key_rc5((const unsigned char *)key, 16, 12);
}

/**
 * \brief ��������
 *
 */
GatewayTask::~GatewayTask()
{
	if(pUser)
	{
		//pUser->lock();
		//pUser->gatewaytask=NULL;
		//if(pUser->isWaitPlayState() || pUser->isPlayState())
		//{
		//	pUser->unreg();
			//pUser->unlock();
		//}
		//else
		//{
			//pUser->unlock();
		//	pUser->final();
		//}
		// */
		SAFE_DELETE(pUser);
	}
}

/**
 * \brief ��֤�汾��
 * \param ptNullCmd ����֤��ָ��
 * \return ��֤�汾�����Ƿ�ɹ�
 */
bool GatewayTask::verifyVersion(const Cmd::stNullUserCmd *ptNullCmd)
{
	Zebra::logger->trace("GatewayTask::verifyVersion");
	using namespace Cmd;
	if (LOGON_USERCMD == ptNullCmd->byCmd
			&& USER_VERIFY_VER_PARA == ptNullCmd->byParam)
	{
		stUserVerifyVerCmd *ptCmd = (stUserVerifyVerCmd *)ptNullCmd;
		if (GatewayService::getInstance().verify_client_version == 0 || ptCmd->version >= GatewayService::getInstance().verify_client_version)
		{
			Zebra::logger->debug("�ͻ�������ͨ���汾����֤[%u]",ptCmd->version);
			return true;
		}
		else
		{
			Zebra::logger->debug("�ͻ�������û��ͨ���汾����֤[%u][%u]",ptCmd->version,GatewayService::getInstance().verify_client_version);
			return false;
		}
		/*
		*/
	}

	Zebra::logger->error("�ͻ�������û��ͨ���汾����֤");
	return false;
}

/**
 * \brief ��֤��½����ָ��
 * ��֤��½���ص�ָ��
 * \param ptNullCmd ����֤�ĵ�½ָ��
 * \return ��֤��½ָ���Ƿ�ɹ�
 */
bool GatewayTask::verifyACCID(const Cmd::stNullUserCmd *ptNullCmd)
{
	Zebra::logger->trace("GatewayTask::verifyACCID");
	using namespace Cmd;
	if (LOGON_USERCMD == ptNullCmd->byCmd
			&& PASSWD_LOGON_USERCMD_PARA == ptNullCmd->byParam)
	{
		stPasswdLogonUserCmd *ptCmd = (stPasswdLogonUserCmd *)ptNullCmd;
		Zebra::logger->trace("�û���¼��%u %u", ptCmd->loginTempID, ptCmd->dwUserID);
		DES_cblock des_key;
		if (LoginSessionManager::getInstance().verify(ptCmd->loginTempID, ptCmd->dwUserID,numPassword, &des_key))
		{
			accid = ptCmd->dwUserID;
			loginTempID = ptCmd->loginTempID;
			strncpy(account, ptCmd->pstrName, MAX_ACCNAMESIZE);
			if (numPassword[0])
			{
				numPwd = atoi(numPassword);
				Zebra::logger->debug("[�Ʊ�]: %s ��������Ϊ: %u", account, numPwd);
			}

			//����zSocket�ļ�����Կ
			mSocket.setEncMethod(CEncrypt::ENCDEC_DES);
			mSocket.set_key_des(&des_key);

			Cmd::stMergeVersionCheckUserCmd send;
			send.dwMergeVersion = merge_version;
			sendCmd(&send, sizeof(send));

			//Zebra::logger->debug("����des��Կ��%u %u %u %u %u %u %u %u", des_key[0], des_key[1], des_key[2], des_key[3], des_key[4], des_key[5], des_key[6], des_key[7]);

			//Zebra::logger->debug("�ͻ�������ͬ����֤�ɹ����ȴ��Ʒѷ���������");
			return true;
		}
	}

	Zebra::logger->error("�ͻ�������ͬ����֤ʧ��");
	return false;
}

/**
 * \brief ����ȷ��
 *
 * \return ���û�����ݷ���0,��֤�ɹ�����1,��֤ʧ�ܷ���-1
 */
int GatewayTask::verifyConn()
{
	Zebra::logger->trace("GatewayTask::verifyConn");
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		while(true)
		{
			unsigned char pstrCmd[zSocket::MAX_DATASIZE];
			int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
			if (nCmdLen <= 0)
				//����ֻ�Ǵӻ���ȡ���ݰ������Բ������û������ֱ�ӷ���
				return 0;
			else
			{
				using namespace Cmd;
				//Zebra::logger->debug(__PRETTY_FUNCTION__);

				stNullUserCmd *ptNullCmd = (stNullUserCmd *)pstrCmd;
				if (versionVerified)
				{
					if (verifyACCID(ptNullCmd))
						return 1;
					else
						return -1;
				}
				else if (verifyVersion(ptNullCmd))
				{
					versionVerified = true;
				}
				else
				{
					Zebra::logger->error("�ͻ���������֤ʧ��");
					return -1;
				}
			}
		}
	}
	else
		return retcode;
}

/**
 * \brief ����ͬ���ȴ�
 *
 *
 * \return ���󷵻�-1,�ɹ�����1,�����ȴ�����0
 */
int GatewayTask::waitSync()
{
	switch(accountVerified)
	{
		case ACCOUNTVERIFY_NONE:
			{
				using namespace Cmd::Bill;
				t_LoginVerify_Gateway tCmd;
				tCmd.accid = accid;
				tCmd.loginTempID = loginTempID;
				if (accountClient->sendCmd(&tCmd, sizeof(tCmd)))
					accountVerified = ACCOUNTVERIFY_WAITING;
				else
					return -1;
			}
			break;
		case ACCOUNTVERIFY_WAITING:
			{
				//�����ڵȴ��Ʒ���֤ͨ��
			}
			break;
		case ACCOUNTVERIFY_SUCCESS:
			{
				return 1;
			}
			break;
		case ACCOUNTVERIFY_FAILURE:
			{
				return -1;
			}
			break;
	}

	return 0;
}

/**
 * \brief �û�����
 *
 * \return 1:���Ի���,2������ʱ
 */
int GatewayTask::recycleConn()
{
	/*
	if(recycle_wait == 5000)
	{
		//֪ͨ�Ʒѷ������û�����
		using namespace Cmd::Bill;
		t_Logout_Gateway tCmd;

		tCmd.accid = accid;
		tCmd.loginTempID = loginTempID;
		accountClient->sendCmd(&tCmd, sizeof(tCmd));
	}
	// */
	if((int)recycle_wait > 0)
	{
		recycle_wait -= 200;
		return 0;
	}
	else
	{
		//֪ͨ�Ʒѷ������û�����
		using namespace Cmd::Bill;
		t_Logout_Gateway tCmd;

		tCmd.accid = accid;
		tCmd.loginTempID = loginTempID;
		accountClient->sendCmd(&tCmd, sizeof(tCmd));
		// */
		return 1;
	}
}

/**
 * \brief ��Ҫ�����Ͽ��ͻ��˵�����
 *
 * \param method ���ӶϿ���ʽ
 */
void GatewayTask::Terminate(const TerminateMethod method)
{
	if(pUser)
	{
		Zebra::logger->trace("�û�Terminate(%ld,%ld,%s)ע��,ip=%s",pUser->accid,pUser->id,pUser->name,getIP());

		Cmd::Mini::t_UserLogout_Gateway s;
		s.userID = pUser->id;
		miniClient->sendCmd(&s, sizeof(s));

		if(!isTerminate())
		{
			pUser->unreg(true);
		}
		pUser->final();
	}
	else
	{
		Zebra::logger->trace("ע�����û��Ѿ�������");
	}
	// */
	zTCPTask::Terminate(method);
}
/**
 * \brief ���û���½
 *
 */
void GatewayTask::addToContainer()
{
	GateUser *pUser=new GateUser(this->accid,this);
	if(pUser)
	{
		//������Ϸʱ�䣬��ʼ��ʱ
		using namespace Cmd;
		stGameTimeTimerUserCmd cmd;
		cmd.qwGameTime = Zebra::qwGameTime;
		if(!qwGameTime)
		{
			qwGameTime = Zebra::qwGameTime; 
			GameTimeSyn.now();
		}
		sendCmd(&cmd, sizeof(cmd));
		initTime = GatewayTimeTick::currentTime;

#define country_info GatewayService::getInstance().country_info
		char Buf[country_info.getCountrySize()*sizeof(Cmd::Country_Info) + sizeof(Cmd::stCountryInfoUserCmd)];
		bzero(Buf ,sizeof(Buf));
		Cmd::stCountryInfoUserCmd *ciu = (Cmd::stCountryInfoUserCmd*)Buf;
		constructInPlace(ciu);
		ciu->size = country_info.getAll((char *)ciu->countryinfo);
		pUser->sendCmd(ciu , sizeof(Cmd::stCountryInfoUserCmd) + ciu->size *sizeof(Cmd::Country_Info));
		if(pUser->beginSelect())
		{
			//֪ͨ��½������������������
			GatewayService::getInstance().notifyLoginServer();
		}
		else {
			/*
			   SAFE_DELETE(pUser);
			   pUser = NULL;
			// */
		}
	}
}

/**
 * \brief ��������ɾ��
 *
 */
void GatewayTask::removeFromContainer()
{
	//֪ͨ��½������������������
	GatewayService::getInstance().notifyLoginServer();
}

/**
 * \brief ���һ�������߳�
 *
 *
 * \return �ɹ�����ture,���򷵻�false
 */
bool GatewayTask::uniqueAdd()
{
	return GatewayTaskManager::getInstance().uniqueAdd(this);
}

/**
 * \brief ɾ��һ�������߳�
 *
 *
 * \return �ɹ�����ture,���򷵻�false
 */
bool GatewayTask::uniqueRemove()
{
	return GatewayTaskManager::getInstance().uniqueRemove(this);
}

/**
 * \brief �����û�����Ϣת����Bill
 *
 *
 * \param ptNullCmd: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return ת���Ƿ�ɹ�
 */
bool GatewayTask::forwardBillScene(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(pUser)
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::Bill::t_Scene_ForwardBill *sendCmd=(Cmd::Bill::t_Scene_ForwardBill *)buf;
		constructInPlace(sendCmd);
		sendCmd->dwAccid=pUser->accid;
		sendCmd->size=nCmdLen;
		bcopy(ptNullCmd,sendCmd->data,nCmdLen);
		accountClient->sendCmd(buf,sizeof(Cmd::Bill::t_Scene_ForwardBill)+nCmdLen);
		//Zebra::logger->debug("ת��%ld����Ϣ��%ld����",pUser->id,pUser->sceneTempID);
		return true;
	}
	return false;
}
/**
 * \brief ����Ϣת����Bill
 *
 *
 * \param ptNullCmd: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return ת���Ƿ�ɹ�
 */
bool GatewayTask::forwardBill(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(pUser)
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::Bill::t_Bill_ForwardBill *sendCmd=(Cmd::Bill::t_Bill_ForwardBill *)buf;
		constructInPlace(sendCmd);
		sendCmd->dwAccid=pUser->accid;
		sendCmd->size=nCmdLen;
		bcopy(ptNullCmd,sendCmd->data,nCmdLen);
		accountClient->sendCmd(buf,sizeof(Cmd::Bill::t_Bill_ForwardBill)+nCmdLen);
		//Zebra::logger->debug("ת��%ld����Ϣ��%ld����",pUser->id,pUser->sceneTempID);
		return true;
	}
	return false;
}
/**
 * \brief ��Bill����Ϣת��������
 *
 *
 * \param ptNullCmd: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return ת���Ƿ�ɹ�
 */
bool GatewayTask::forwardSceneBill(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(pUser && pUser->scene)
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::Scene::t_Bill_ForwardScene *sendCmd=(Cmd::Scene::t_Bill_ForwardScene *)buf;
		constructInPlace(sendCmd);
		sendCmd->dwUserID=pUser->id;
		sendCmd->dwSceneTempID=pUser->sceneTempID;
		sendCmd->size=nCmdLen;
		bcopy(ptNullCmd,sendCmd->data,nCmdLen);
		pUser->scene->sendCmd(buf,sizeof(Cmd::Scene::t_Bill_ForwardScene)+nCmdLen);
		//Zebra::logger->debug("ת��%ld����Ϣ��%ld����",pUser->id,pUser->sceneTempID);
		return true;
	}
	return false;
}

/**
 * \brief ����Ϣת����С��Ϸ������
 *
 *
 * \param ptNullCmd: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return ת���Ƿ�ɹ�
 */
bool GatewayTask::forwardMini(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(pUser)
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::Mini::t_Mini_UserForwardMini *sendCmd=(Cmd::Mini::t_Mini_UserForwardMini *)buf;
		constructInPlace(sendCmd);
		sendCmd->id=pUser->id;
		sendCmd->size=nCmdLen;
		bcopy(ptNullCmd, sendCmd->data, nCmdLen);
		miniClient->sendCmd(buf, sizeof(Cmd::Mini::t_Mini_UserForwardMini)+nCmdLen);
		Zebra::logger->debug("ת��%ld����Ϣ��Mini������", pUser->id);
		return true;
	}   
	return false;
}

/**
 * \brief ����Ϣת��������
 *
 *
 * \param ptNullCmd: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return ת���Ƿ�ɹ�
 */
bool GatewayTask::forwardScene(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(pUser && pUser->scene)
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::Scene::t_Scene_ForwardScene *sendCmd=(Cmd::Scene::t_Scene_ForwardScene *)buf;
		constructInPlace(sendCmd);
		sendCmd->dwUserID=pUser->id;
		sendCmd->dwSceneTempID=pUser->sceneTempID;
		sendCmd->size=nCmdLen;
		bcopy(ptNullCmd,sendCmd->data,nCmdLen);
		pUser->scene->sendCmd(buf,sizeof(Cmd::Scene::t_Scene_ForwardScene)+nCmdLen);
		//Zebra::logger->debug("ת��%ld����Ϣ��%ld����",pUser->id,pUser->sceneTempID);
		return true;
	}
	return false;
}

/**
 * \brief ����Ϣת����Session
 *
 *
 * \param ptNullCmd: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return ת���Ƿ�ɹ�
 */
bool GatewayTask::forwardSession(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(pUser)
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::Session::t_Session_ForwardUser *sendCmd=(Cmd::Session::t_Session_ForwardUser *)buf;
		constructInPlace(sendCmd);
		sendCmd->dwID=pUser->id;
		sendCmd->size=nCmdLen;
		bcopy(ptNullCmd,sendCmd->data,nCmdLen);
		sessionClient->sendCmd(buf,sizeof(Cmd::Session::t_Session_ForwardUser)+nCmdLen);
		//Zebra::logger->debug("ת��%ld����Ϣ��Session������",pUser->id,pUser->sceneTempID);
		return true;
	}
	return false;
}

/**
 * \brief ����ѡ����Ϣ
 *
 *
 * \param ptNullCmd: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return �����Ƿ�ɹ�
 */
bool GatewayTask::msgParse_Select(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd;

	switch(ptNullCmd->byParam)
	{
		case CHECKNAME_SELECT_USERCMD_PARA:
			{
				stCheckNameSelectUserCmd *rev=(stCheckNameSelectUserCmd *)ptNullCmd;

				//�ʻ����
				if (! (wordFilter::getInstance().doFilter(rev->name, 16) && checkNewName(rev->name)))
				{
					Cmd::stCheckNameSelectUserCmd ret;
					ret.err_code = 2;//��ʾ�����а�����ֹ�Ĵʻ�
					strncpy(ret.name, rev->name, MAX_NAMESIZE);
					sendCmd(&ret, sizeof(ret));
					return true;
				}

				/*
				//TODO:Ψһ����֤����������ʱû�У�������
				if (GatewayService::getInstance().rolereg_verify)
				{
					using namespace Cmd::RoleReg;
					using namespace Cmd::Super;
					t_Charname_Gateway cmd;
					cmd.wdServerID = GatewayService::getInstance().getServerID();
					cmd.accid = pUser->accid;
					strcpy(cmd.name, rev->name);
					GatewayService::getInstance().sendCmdToSuperServer(&cmd, sizeof(cmd));
				}
				*/
				Cmd::Record::t_CheckName_GateRecord send;
				strncpy(send.name, rev->name, MAX_NAMESIZE-1);
				send.accid = pUser->accid;
				//���͵�RecordServer��֤
				return recordClient->sendCmd(&send, sizeof(send));
			}
			break;
		case CREATE_SELECT_USERCMD_PARA:
			{
				stCreateSelectUserCmd *rev=(stCreateSelectUserCmd *)ptNullCmd;
				
					Zebra::logger->trace("������ɫ��%u, %s, %u", pUser->accid, rev->strUserName, rev->charType);
					if (!pUser->charInfoFull())
					{
						if(strlen(rev->strUserName) > 16)
						{
							Zebra::logger->trace("���ֳ���̫��(%d)" , strlen(rev->strUserName));
							return true;
						}
						//��ɫ����û�������������ٽ�����ɫ
						pUser->createCharCmd.accid = pUser->accid;
						bzero(pUser->createCharCmd.name, sizeof(pUser->createCharCmd.name));
						strncpy(pUser->createCharCmd.name, rev->strUserName, MAX_NAMESIZE - 1);
						pUser->createCharCmd.type = rev->charType;
						//ͷ�������Լ���ɫ
						pUser->createCharCmd.hair = rev->byHairType;
						pUser->createCharCmd.hair = pUser->createCharCmd.hair << 24;
						pUser->createCharCmd.hair = (pUser->createCharCmd.hair & HAIRTYPE_MASK) | (rev->byRGB & HAIRRGB_MASK);
						//if(rev->country >11)
#define country_info GatewayService::getInstance().country_info
						//Zebra::logger->debug("������Ϣ����%u" , country_info.getCountrySize());
						if(rev->country && rev->country !=country_info.getCountryID(rev->country))
						{
							Zebra::logger->trace("�������ݲ��Ϸ�");
							return true;
						}
						if (!country_info.isEnableRegister(rev->country))
						{
							if (pUser)
							{
								Cmd::stDisableLoginCountryCmd send;
								send.status = Cmd::FALSE_REGISTER_COUNTRY;
								pUser->sendCmd(&send, sizeof(send));
							}
							return true;
						}
						strncpy(pUser->createCharCmd.mapName,country_info.getMapName(rev->country).c_str(),MAX_NAMESIZE);
						pUser->createCharCmd.country =rev->country;
						pUser->createCharCmd.createip = this->getAddr();
						if(rev->five >=FIVE_NONE)
						{
							Zebra::logger->trace("���������Բ��Ϸ�");
							return true;
						}
						pUser->createCharCmd.five = rev->five;
						if (GatewayService::getInstance().rolereg_verify)
						{
							using namespace Cmd::RoleReg;
							using namespace Cmd::Super;
							t_Charname_Gateway cmd;
							cmd.wdServerID = GatewayService::getInstance().getServerID();
							cmd.accid = pUser->accid;
							strncpy(cmd.name, pUser->createCharCmd.name, MAX_NAMESIZE);
							cmd.state = ROLEREG_STATE_TEST;
							GatewayService::getInstance().sendCmdToSuperServer(&cmd, sizeof(cmd));
						}
						else
						{
							if (!recordClient->sendCmd(&pUser->createCharCmd,sizeof(pUser->createCharCmd)))
								return true;
						}
						pUser->createState();
					}
					else
						Zebra::logger->trace("��ɫ��Ϣ���������ٴ�����ɫ��");

				return true;
			}
			break;
		case LOGIN_SELECT_USERCMD_PARA: 
			{
				if(!pUser->isWaitUnregState())
				{
					stLoginSelectUserCmd *rev=(stLoginSelectUserCmd *)ptNullCmd;
					SelectUserInfo *userinfo=pUser->getSelectUserInfo(rev->charNo);
					Zebra::logger->trace("�յ�%ld�ĵ�%d����¼�û�ָ��",pUser->accid,rev->charNo);
					if(!userinfo)
					{
						Zebra::logger->debug("�յ������ڵĽ�ɫ,�����Ƕ��⹥��!");
						return true;
					}
					if(userinfo->bitmask & CHARBASE_FORBID)
					{
						Zebra::logger->debug("�յ��Ѿ�����ŵĽ�ɫ,�����Ƕ��⹥��!");
						return true;
					}

					if (!GatewayService::getInstance().country_info.isEnableLogin(userinfo->country))
					{
						Zebra::logger->debug("���Ҳ������½ %u, %u", pUser->accid, userinfo->country);
						Cmd::stDisableLoginCountryCmd send;
						send.status = Cmd::FALSE_LOGIN_COUNTRY;
						pUser->sendCmd(&send, sizeof(send));
						return true;
					}
					if (pUser->checkPassport(rev->jpegPassport))
					{
						if(pUser->tempid == 0 && userinfo && userinfo->id!=0)
						{
							//Zebra::logger->debug("%u, %u, %u", pUser->systemstate, pUser->tempid, userinfo->id);
							pUser->id=userinfo->id;
							pUser->face=userinfo->face;
							strncpy(pUser->name,userinfo->name,MAX_NAMESIZE);
							//�����ɫ��������ʱID
							if(GateUserManager::getInstance()->addUser(pUser) && GateUserManager::getInstance()->addCountryUser(pUser))
							{
								Zebra::logger->trace("ע�� %u, %s", pUser->accid,userinfo->name);
								//ע���ɫ
								pUser->reg(rev->charNo);
								return true;
							}
							else
							{
								Zebra::logger->debug("ѡ��%sʱ����û�ʧ��",userinfo->name);
							}
						}
						else
							Zebra::logger->debug("�û�%s(%ld)�ظ�ѡ���ɫ",userinfo->name,userinfo->id);
					}
					else
						Zebra::logger->debug("��֤ͼ����֤��ʧ�� %u",pUser->accid);
				}
				else
				{
					Zebra::logger->debug("�û�%s(%ld)�����˳�״̬",pUser->name,pUser->id);
				}
				return true;
			}
			break;
		case DELETE_SELECT_USERCMD_PARA:
			{
				stDeleteSelectUserCmd *rev=(stDeleteSelectUserCmd *)ptNullCmd;
				SelectUserInfo *userinfo=pUser->getSelectUserInfo(rev->charNo);
				if(!userinfo)
				{
					Zebra::logger->debug("�յ������ڵĽ�ɫ,�����Ƕ��⹥��!");
					return true;
				}
				//debug�汾ȥ���������빦��
#ifndef _ALL_SUPER_GM 
				//���������������
				if(numPassword[0] && strncmp(numPassword,rev->numPassword,sizeof(numPassword)) != 0)
				{
					stDeleteErrorSelectUserCmd ret;
					sendCmd(&ret, sizeof(ret));
					return true;
				}
#endif
				if(pUser->tempid == 0 && userinfo && userinfo->id!=0)
				{
					Zebra::logger->info("�û�(%s,%d)ɾ����ɫ",userinfo->name , userinfo->id);
					//Cmd::Record::t_DelChar_GateRecord send;
					Cmd::Session::t_DelChar_GateSession send;
					send.accid=pUser->accid;
					send.id=userinfo->id;
					strncpy(send.name, userinfo->name, MAX_NAMESIZE);
					sessionClient->sendCmd(&send, sizeof(send));

					Cmd::Mini::t_UserDelete_Gateway d;
					d.userID = userinfo->id;
					miniClient->sendCmd(&d, sizeof(d));

					pUser->delSelectUserInfo(rev->charNo);
					//recordClient->sendCmd(&send,sizeof(send));
				}
				else
					Zebra::logger->debug("�û�%s(%ld)ɾ����ɫʧ��",userinfo->name , userinfo->id);
				return true;
			}
			break;
		case REQUEST_COUNTRY_SELECT_USERCMD_PARA:
			{
				if(Zebra::global["countryorder"] == "2")
				{
					Cmd::Record::t_request_Country_GateRecord reqRecord; 
					recordClient->sendCmd(&reqRecord, sizeof(reqRecord));
				}
				else  if(Zebra::global["countryorder"] == "1")
				{
					Cmd::Session::t_request_Country_GateSession reqSession;
					sessionClient->sendCmd(&reqSession,sizeof(reqSession));
				}
/*
#define country_info GatewayService::getInstance().country_info
				char Buf[country_info.getCountrySize()*sizeof(Cmd::Country_Info) + sizeof(Cmd::stCountryInfoUserCmd)];
				bzero(Buf ,sizeof(Buf));
				Cmd::stCountryInfoUserCmd *ciu = (Cmd::stCountryInfoUserCmd*)Buf;
				constructInPlace(ciu);
				ciu->size = country_info.getAll((char *)ciu->countryinfo);
				pUser->sendCmd(ciu , sizeof(Cmd::stCountryInfoUserCmd) + ciu->size *sizeof(Cmd::Country_Info));
// */
				return true;
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->byCmd, ptNullCmd->byParam, nCmdLen);
	return false;
}

/**
 * \brief ����ʱ����Ϣ
 *
 *
 * \param ptNullCmd: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return �����Ƿ�ɹ�
 */
bool GatewayTask::msgParse_Time(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd;

	switch(ptNullCmd->byParam)
	{
		case USERGAMETIME_TIMER_USERCMD_PARA:
			{
				if(!dwTimestampServer)
				{
					dwTimestampServer=GatewayTimeTick::currentTime.msecs() - ptNullCmd->dwTimestamp;
				}
				stUserGameTimeTimerUserCmd *rev=(stUserGameTimeTimerUserCmd *)ptNullCmd;
				if(qwGameTime && (SQWORD)(rev->qwGameTime - (qwGameTime + 
								(GatewayTimeTick::currentTime.sec() - GameTimeSyn.sec()))) >= sampleInterval_error_sec)
				{   
					if(this->pUser)
					{
						Zebra::logger->trace("�ͻ�����Ϸʱ��̫�죬ʹ���˼���������Ҫ�Ͽ����ӣ�accid = %u, %u, %llu��", this->pUser->accid, ptNullCmd->dwTimestamp, rev->qwGameTime - (qwGameTime + (GatewayTimeTick::currentTime.sec() - GameTimeSyn.sec())));
					}
					Zebra::logger->debug("��Ϸʱ��:(accid=%u,%llu,%llu)", accid,rev->qwGameTime, qwGameTime + (GatewayTimeTick::currentTime.sec() - GameTimeSyn.sec()));
					Terminate();
				}
				if(!haveCheckTime)
					haveCheckTime = true;
				else
					Zebra::logger->warn("У��ʱ���Ǵ���(accid=%u)", accid);
				return true;
			}
			break;
		case PING_TIMER_USERCMD_PARA:
			{       
				stPingTimeTimerUserCmd cmd;
				sendCmd(&cmd, sizeof(cmd));
				return true;
			}       
			break;  
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->byCmd, ptNullCmd->byParam, nCmdLen);
	return false;
}

/**
 * \brief ���ʱ��
 *
 * \param ct: ��ǰʱ��
 * \return �Ƿ�ͨ�����
 */
bool GatewayTask::checkTime(const zRTime &ct)
{
	if (accountVerified == ACCOUNTVERIFY_SUCCESS)
	{
		if (ct >= lastCheckTime)
		{
			if (haveCheckTime)
			{
				//У��ʱ��
				if(_retset_gametime(ct))
				{
					Cmd::stGameTimeTimerUserCmd cmd;
					if(qwGameTime)
					{
						cmd.qwGameTime = qwGameTime + (GatewayTimeTick::currentTime.sec() - GameTimeSyn.sec());
						sendCmd(&cmd, sizeof(cmd));
					}
					//׼����������ͬ��ʱ��
					dwTimestampServer=0;
				}
				haveCheckTime = false;
				Cmd::stRequestUserGameTimeTimerUserCmd cmd;
				sendCmd(&cmd, sizeof(cmd));
			}
			else
			{
				Zebra::logger->trace("�ͻ���ָ��ʱ����û�з���У��ʱ��ָ�accid = %u, snd_queue_size = %u��,ע��", accid, mSocket.snd_queue_size());
				TerminateWait();
			}

			lastCheckTime = ct;
			lastCheckTime.addDelay(checkTimeInterval);
		}
	}

	return true;
}

/**
 * \brief �Կͻ��˷��͹�����ָ����м��
 * ��Ҫ���ʱ����ȣ��Լ������Ƚ��з�ֹ
 * \param pCmd ������ָ��
 * \param ct ��ǰʱ��
 * \return ����Ƿ�ɹ�
 */
bool GatewayTask::checkUserCmd(const Cmd::stNullUserCmd *pCmd, const zRTime &ct)
{
	if(dwTimestampServer)
	{
		if(abs((dwTimestampServer + pCmd->dwTimestamp) - ct.msecs()) > sampleInterval_error_msecs)
		{
			Zebra::logger->trace("�ͻ���ָ��ʱ��̫�죬ʹ���˼���������Ҫ�Ͽ����ӣ�accid = %u, %d, %llu��", accid, (dwTimestampServer + pCmd->dwTimestamp) - ct.msecs(), initTime.elapse(ct));
			v_lastSampleTime = pCmd->dwTimestamp + sampleInterval;
			dwTimestampServer = ct.msecs() - pCmd->dwTimestamp;
			return false;
		}
		if (pCmd->dwTimestamp >= v_lastSampleTime)
		{
			v_lastSampleTime = pCmd->dwTimestamp + sampleInterval;
			/*
			if(v_samplePackets > 30)
			{
				Zebra::logger->debug("%d�����յ�ָ�����%d",sampleInterval , v_samplePackets);
			}
			// */
			v_samplePackets = 0;
		}
		if (pCmd->byCmd != Cmd::MAPSCREEN_USERCMD)
			v_samplePackets++;
		if (v_samplePackets > maxSamplePPS)
		{
			Zebra::logger->trace("�ͻ���ָ��͹��죬��Ҫ�Ͽ����ӣ�accid = %u, %u/%u���룩", accid, v_samplePackets, sampleInterval);
			return false;
		}
		/*
		int interval = pCmd->dwTimestamp - initTime.elapse(ct);
		if (interval > 1200)
		{
			Zebra::logger->trace("�ͻ���ʱ��̫�죬����ʹ���˼��������Ͽ����ӣ�accid = %u, %u, %llu��", accid, pCmd->dwTimestamp, initTime.elapse(ct));
			return false;
		}
		// */
	}
	return true;

}

/**
 * \brief ������Ϣ
 *
 * \param ptNull: ��Ҫת����ָ��
 * \param nCmdLen: ָ���
 * \return �����Ƿ�ɹ�
 */
bool GatewayTask::msgParse(const Cmd::t_NullCmd *ptNull, const unsigned int nCmdLen)
{
	using namespace Cmd;
	const stNullUserCmd *ptNullCmd = (const stNullUserCmd *)ptNull;
	//Zebra::logger->debug("�յ�ָ��:%d,%d",ptNullCmd->byCmd , ptNullCmd->byParam);

/*
	static DWORD dwTimestamp;
	Zebra::logger->debug("cmd:%d\tlast:%d\tcurrent:%d\tinterval:%d", ptNullCmd->byCmd, dwTimestamp, ptNullCmd->dwTimestamp, ptNullCmd->dwTimestamp - dwTimestamp);
	dwTimestamp = ptNullCmd->dwTimestamp;
	
	static DWORD p_cur;;
	Zebra::logger->debug("p_cur:%ld\t\tcurrent:%ld\tinterval:%d", p_cur, GatewayTimeTick::currentTime.msecs(), GatewayTimeTick::currentTime.msecs() -p_cur);
	p_cur = GatewayTimeTick::currentTime.msecs();
*/

	if(isTerminate())
	{
		return false;
	}
	if (!checkUserCmd(ptNullCmd, GatewayTimeTick::currentTime))
	{
		Terminate();
		return false;
	}
	// */

	if (pUser)
	{
		switch(ptNullCmd->byCmd)
		{
			case LOGON_USERCMD:
				{
					if(ptNullCmd->byParam==BACKSELECT_USERCMD_PARA)
					{
						if(pUser && pUser->isPlayState())
						{
							Zebra::logger->trace("%s(%ld)С��",pUser->name,pUser->id);
							pUser->backSelect=true;
							pUser->unreg();
						}
						return true;
					}
					return true;
				}
				break;
			case SELECT_USERCMD:
				{
					if (pUser->isSelectState())
					{
						if (msgParse_Select(ptNullCmd,nCmdLen))
						{
							return true;
						}
					}
					else
						Zebra::logger->trace("%s(%ld)����ѡ���û�״̬,����ѡ���û�ϵ��ָ��",pUser->name,pUser->id);
					return true;
				}
				break;
			case TIME_USERCMD:
				{
					if (msgParse_Time(ptNullCmd, nCmdLen))
					{
						return true;
					}
					return false;
				}
				break;
			case CHAT_USERCMD:
				{
					if(pUser && pUser->isPlayState())
					{
						//�������ͨ������Ϣ����дʻ����
						if (ALL_CHAT_USERCMD_PARAMETER == ptNullCmd->byParam)
						{
							Cmd::stChannelChatUserCmd *rev = (Cmd::stChannelChatUserCmd *)ptNullCmd;
							if (rev->dwType==CHAT_TYPE_COUNTRY||rev->dwType==CHAT_TYPE_WORLD)
							{
#ifndef _ALL_SUPER_GM
								if (GatewayTimeTick::currentTime<nextCountryChatTime)
								{
									zRTime ctv;
									Cmd::stChannelChatUserCmd send;
									send.dwType=Cmd::CHAT_TYPE_SYSTEM;
									send.dwSysInfoType = Cmd::INFO_TYPE_FAIL;
									send.dwChatTime = ctv.sec();
									bzero(send.pstrName, sizeof(send.pstrName));
									bzero(send.pstrChat, sizeof(send.pstrChat));
									strncpy((char *)send.pstrChat, "�Բ��𣬸�Ƶ��3���ӿ��Է���һ��",MAX_CHATINFO-1);

									pUser->sendCmd(&send, sizeof(send));
									return true;
								}
#endif								
							}
							else
							{
#ifndef _ALL_SUPER_GM
								if (GatewayTimeTick::currentTime<nextChatTime)
								{
									zRTime ctv;
									Cmd::stChannelChatUserCmd send;
									send.dwType=Cmd::CHAT_TYPE_SYSTEM;
									send.dwSysInfoType = Cmd::INFO_TYPE_FAIL;
									send.dwChatTime = ctv.sec();
									bzero(send.pstrName, sizeof(send.pstrName));
									bzero(send.pstrChat, sizeof(send.pstrChat));
									strncpy((char *)send.pstrChat, "�Բ�����˵��̫���ˣ��벻Ҫˢ��",MAX_CHATINFO-1);

									pUser->sendCmd(&send, sizeof(send));
									return true;
								}
#endif								
								if (rev->dwType == CHAT_TYPE_BLESS_MSG)
								{
									forwardSession(ptNullCmd , nCmdLen);
									return true;
								}
							}
							//wordFilter::getInstance().doFilter(rev->pstrChat , strlen(rev->pstrChat));

							nextChatTime.now();
							nextChatTime.addDelay(chatInterval);
							nextCountryChatTime.now();
							nextCountryChatTime.addDelay(3000);

							/*
							   if (rev->dwType==CHAT_TYPE_OVERMAN)
							   {
							   BYTE buf[zSocket::MAX_DATASIZE];
							   Cmd::GmTool::t_Chat_GmTool *cmd=(Cmd::GmTool::t_Chat_GmTool *)buf;
							   bzero(buf, sizeof(buf));
							   constructInPlace(cmd);

							   strncpy(cmd->userName, pUser->name, MAX_NAMESIZE);
							   cmd->countryID = pUser->countryID;
							   cmd->sceneID = pUser->sceneID;
							   cmd->dwType = rev->dwType;
							   strncpy(cmd->content, rev->pstrChat, 255);
							   cmd->size = rev->size;
							   if (cmd->size)
							   bcopy(rev->tobject_array, cmd->tobject_array, cmd->size*sizeof(Cmd::stTradeObject));
							   GatewayService::getInstance().sendCmdToSuperServer(cmd, sizeof(Cmd::GmTool::t_Chat_GmTool)+cmd->size*sizeof(Cmd::stTradeObject));
							   }
							   */
						}

						switch (ptNullCmd->byParam)
						{
							case CREATE_CHANNEL_USERCMD_PARAMETER:
							case INVITE_CHANNEL_USERCMD_PARAMETER:
							case JOIN_CHANNEL_USERCMD_PARAMETER:
							case LEAVE_CHANNEL_USERCMD_PARAMETER:
								if(forwardSession(ptNullCmd , nCmdLen))
									return true;
								break;
							case QUESTION_OBJECT_USERCMD_PARA:
								if(forwardSession(ptNullCmd , nCmdLen))
								{
#ifdef _ZJW_DEBUG
									Zebra::logger->debug("�յ�������Ʒ��ѯ����,ת�����Ự");
#endif							
									return true;
								}
								break;
							default:
								if(forwardScene(ptNullCmd , nCmdLen))
								{
									return true;
								}
						}
					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case UNION_USERCMD: // ����Ϣת�����Ự�����������ﴦ��������
				{
					if(pUser && pUser->isPlayState())
					{
						switch(ptNullCmd->byParam)
						{
							case CREATE_UNION_PARA:			// �������
								{
									Cmd::stCreateUnionCmd * cmd = (stCreateUnionCmd *)ptNullCmd;
									if (!checkNewName(cmd->UnionName))
									{
										zRTime ctv;
										Cmd::stChannelChatUserCmd send;
										send.dwType=Cmd::CHAT_TYPE_SYSTEM;
										send.dwSysInfoType = Cmd::INFO_TYPE_FAIL;
										send.dwChatTime = ctv.sec();
										bzero(send.pstrName, sizeof(send.pstrName));
										bzero(send.pstrChat, sizeof(send.pstrChat));
										strncpy((char *)send.pstrChat, "�Բ��𣬰�����ֲ��Ϸ�����ʹ����ĸ�����ֺͺ���",MAX_CHATINFO-1);

										sendCmd(&send, sizeof(send));
										return true;
									}
								}
							case UNION_STATUS_CHECK_PARA:	// ���״̬����жϴ����Ƿ���Խ����°��
							case ADD_MEMBER_TO_UNION_PARA:		// ������Ҽ�����
							case ENTER_UNION_CITY_AREA_PARA: // ���������ó��а�������
							case QUESTION_UNION_CITY_INFO_PARA:
							case CONTRIBUTE_UNION_PARA: // ������
								{// ������Ϣת������������������
									if(forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;
							default:
								{
									if(forwardSession(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;

						}
					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case SAFETY_USERCMD:
				{
					if (pUser==NULL || !pUser->isPlayState())
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",
								ptNullCmd->byCmd,ptNullCmd->byParam);
						return true;
					}

					switch (ptNullCmd->byParam)
					{
						case SET_SAFETY_PARA:
							{
								Cmd::stSetSafetyUserCmd* cmd = (Cmd::stSetSafetyUserCmd*)ptNullCmd;
								if (cmd->pwd == this->numPwd)
								{
									if (forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								else
								{
									zRTime ctv;
									Cmd::stChannelChatUserCmd send;
									send.dwType=Cmd::CHAT_TYPE_SYSTEM;
									send.dwSysInfoType = Cmd::INFO_TYPE_FAIL;
									send.dwChatTime = ctv.sec();
									bzero(send.pstrName, sizeof(send.pstrName));
									bzero(send.pstrChat, sizeof(send.pstrChat));
									strncpy((char *)send.pstrChat, "�Բ����������벻��ȷ",MAX_CHATINFO-1);

									pUser->sendCmd(&send, sizeof(send));
								}
							}
							break;
						case SET_TEMP_UNSAFETY_PARA:
							{
								Cmd::stSetTempUnSafetyUserCmd* cmd = (Cmd::stSetTempUnSafetyUserCmd*)ptNullCmd;
								if (cmd->pwd == this->numPwd)
								{
									if (forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								else
								{
									zRTime ctv;
									Cmd::stChannelChatUserCmd send;
									send.dwType=Cmd::CHAT_TYPE_SYSTEM;
									send.dwSysInfoType = Cmd::INFO_TYPE_FAIL;
									send.dwChatTime = ctv.sec();
									bzero(send.pstrName, sizeof(send.pstrName));
									bzero(send.pstrChat, sizeof(send.pstrChat));
									strncpy((char *)send.pstrChat, "�Բ����������벻��ȷ",MAX_CHATINFO-1);

									pUser->sendCmd(&send, sizeof(send));
								}
							}
							break;
						case SET_SAFETY_DETAIL_PARA:
							{
								Cmd::stSetSafetyDetailUserCmd* cmd = (Cmd::stSetSafetyDetailUserCmd*)ptNullCmd;
								if (cmd->pwd == this->numPwd)
								{
									if (forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								else
								{
									zRTime ctv;
									Cmd::stChannelChatUserCmd send;
									send.dwType=Cmd::CHAT_TYPE_SYSTEM;
									send.dwSysInfoType = Cmd::INFO_TYPE_FAIL;
									send.dwChatTime = ctv.sec();
									bzero(send.pstrName, sizeof(send.pstrName));
									bzero(send.pstrChat, sizeof(send.pstrChat));
									strncpy((char *)send.pstrChat, "�Բ����������벻��ȷ",MAX_CHATINFO-1);

									pUser->sendCmd(&send, sizeof(send));
								}
							}
							break;
						default:
							{
								// ������Ϣת������������������
								if(forwardScene(ptNullCmd , nCmdLen))
								{
									return true;
								}
							}
							break;
					}

					return true;
				}
				break;
			case COUNTRY_USERCMD:
				{
					if (pUser && pUser->isPlayState())
					{
						switch(ptNullCmd->byParam)
						{
							case ANSWER_COUNTRY_DARE_PARA:
							case CONTRIBUTE_COUNTRY_MATERIAL:
							case CANCEL_COUNTRY_PARA:
							case APPLY_COUNTRY_PARA:
							case CHANGE_COUNTRY_PARA:// �������
							case REQUEST_COUNTRY_POWER_PARA:
								{
									// ������Ϣת������������������
									if(forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;
							case CONFIRM_SEARCHER_PARA:
							case UP_TECH_DEGREE_PARA:
							case SET_TECH_SEARCH_PARA:
							case REQ_WAIT_OFFICIAL_PARA:
							case SELECT_TRANS_LEVEL:
							case REQ_TECH_PARA:	
							case DARE_COUNTRY_FORMAL_PARA:
							case REQUEST_DARE_COUNTRY_PARA:
							case REQUEST_DARE_RECORD_PARA:
							case TRANS_DARE_COUNTRY_PARA:
							case FORBID_TALK_COUNTRY_PARA:
							case CANCEL_TECH_SEARCH_PARA:
							case ANTI_DARE_COUNTRY_FORMAL_PARA:
							case COUNTRY_NOTE_PARA:
							case CANCEL_DIPLOMAT_PARA:
							case CANCEL_CATCHER_PARA:
							case APPOINT_CATCHER_PARA:
							case APPOINT_DIPLOMAT_PARA:
							case REQ_DAILY_EMPEROR_MONEY:
							case REQ_KING_LIST_PARA:
								{
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}

								}
								break;
							default:
								{
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}
								}
								break;
						}

					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case VOTE_USERCMD:
				{
					if (pUser && pUser->isPlayState())
					{
						switch(ptNullCmd->byParam)
						{
							case REQUEST_VOTE_LIST_PARA:
							case COMMIT_VOTE_PARA:
							case REQ_ARMY_GEN_PARA:
								{
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}

								}
								break;
							default:
								{
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}
								}
								break;
						}

					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case ARMY_USERCMD:
				if (pUser && pUser->isPlayState())
				{
					switch (ptNullCmd->byParam)
					{
						case CREATE_ARMY_PARA:
						case REQ_WAIT_GEN_PARA:
						case REQ_ARMY_SPEC_PARA:
						case REMOVE_ARMY_PARA:
						case CHANGE_ARMY_NAME_PARA:
						case EXIT_ARMY_PARA:
						case ADD_ARMY_CAPTAIN_PARA:
						case FIRE_ARMY_CAPTAIN_PARA:
							{
								if (forwardSession(ptNullCmd, nCmdLen))
								{
									return true;
								}

							}
							break;
						case REQ_ARMY_LIST_PARA:
							{
								// ������Ϣת������������������
								if(forwardScene(ptNullCmd , nCmdLen))
								{
									return true;
								}
							}
							break;
						default:
							{
								if (forwardSession(ptNullCmd, nCmdLen))
								{
									return true;
								}
							}
							break;
					}

				}
				else
				{
					Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
				}
				break;
			case GEM_USERCMD:
				{
					if (pUser && pUser->isPlayState())
					{
						switch (ptNullCmd->byParam)
						{
							case REQUEST_DRAGON_PARA:
							case REQUEST_TIGER_PARA:
								{
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}
								}
								break;
							default:
								{
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}
								}
								break;
						}
					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
				}
				break;
			case DARE_USERCMD: // ս����Ϣת�����Ự������
				{
					if (pUser && pUser->isPlayState())
					{
						switch (ptNullCmd->byParam)
						{
							case ACTIVE_DARE_PARA: // ������ս
							case QUERY_DARE_LIST_PARA:
							case NOTIFY_DARE_PARA: // ս�����֪ͨ
							case GET_UNION_CITY_TAX_PARA:
								{//������Ϣת�����Ự����������
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}
								}
								break;
							case ACTIVE_UNION_CITY_DARE_PARA:
								{
									if(forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;
							default:
								{
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}
								}
								break;
						}
					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case NPCDARE_USERCMD: // ս����Ϣת�����Ự������
				{
					if (pUser && pUser->isPlayState())
					{
						switch (ptNullCmd->byParam)
						{
							case QUESTION_NPCDARE_INFO_PARA:
							case NPCDARE_DARE_PARA: // ������ս
							case NPCDARE_GETGOLD_PARA:
								{//������Ϣת������������������
									if (forwardScene(ptNullCmd, nCmdLen))
									{
										return true;
									}
								}
								break;
							default:
								{
									if (forwardSession(ptNullCmd, nCmdLen))
									{
										return true;
									}
								}
								break;
						}
					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case SEPT_USERCMD:
				{
					if(pUser && pUser->isPlayState())
					{
						switch(ptNullCmd->byParam)
						{
							case CREATE_SEPT_PARA:			// ��������
								{
									Cmd::stCreateSeptCmd * cmd = (stCreateSeptCmd *)ptNullCmd;
									if (!checkNewName(cmd->SeptName))
									{
										zRTime ctv;
										Cmd::stChannelChatUserCmd send;
										send.dwType=Cmd::CHAT_TYPE_SYSTEM;
										send.dwSysInfoType = Cmd::INFO_TYPE_FAIL;
										send.dwChatTime = ctv.sec();
										bzero(send.pstrName, sizeof(send.pstrName));
										bzero(send.pstrChat, sizeof(send.pstrChat));
										strncpy((char *)send.pstrChat, "�Բ��𣬼������ֲ��Ϸ�����ʹ����ĸ�����ֺͺ���",MAX_CHATINFO-1);

										sendCmd(&send, sizeof(send));
										return true;
									}
								}
							case SEPT_STATUS_CHECK_PARA:	// ����״̬����жϴ����Ƿ���Խ����¼���
							case ADD_MEMBER_TO_SEPT_PARA:	// ������Ҽ������
								{							// ������Ϣת������������������
									if(forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;
							default:
								{
									if(forwardSession(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;

						}
					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case RELATION_USERCMD:
				{
					if(pUser && pUser->isPlayState())
					{
						switch(ptNullCmd->byParam)
						{
							case MARRY_STATUS_CHECK_PARA:
								{		// ������Ϣת������������������
									if(forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;
							default:
								{
									if(forwardSession(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;

						}
					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case SCHOOL_USERCMD:
				{
					if(pUser && pUser->isPlayState())
					{
						switch(ptNullCmd->byParam)
						{
//						case Cmd::SCHOOL_BULLETIN__PARA:
//						pUser->sendCmd(ptNullCmd, nCmdLen);
//						return true;
							case Cmd::SCHOOL_STATUS_CHECK_PARA:
							case Cmd::CREATE_SCHOOL_PARA:
							case Cmd::ADD_MEMBER_TO_SCHOOL_PARA:
								{		// ������Ϣת������������������
									if(forwardScene(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;
							default:
								{
									if(forwardSession(ptNullCmd , nCmdLen))
									{
										return true;
									}
								}
								break;
						}
					}
					else
					{
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case MOVE_USERCMD:
				{
					zRTime ctv;
					if (ptNullCmd->byParam == Cmd::USERMOVE_MOVE_USERCMD_PARA)
					{
						Cmd::stUserMoveMoveUserCmd send = *(Cmd::stUserMoveMoveUserCmd *)ptNullCmd;
						send.dwTimestamp = ctv.msecs();
						if(pUser && pUser->isPlayState())
						{
							if(forwardScene(&send , sizeof(send)))
							{
								return true;
							}
						}
						else
						{
							Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
						}
					}
					else
					{
						if(pUser && pUser->isPlayState())
						{
							if(forwardScene(ptNullCmd , nCmdLen))
							{
								return true;
							}
						}
						else
						{
							Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
						}
					}
					return true;
				}
				break;
			case MAGIC_USERCMD:
				{
#ifdef _DEBUGLOG
					Zebra::logger->debug("����ָ��ʱ���:%ld" , ptNullCmd->dwTimestamp);
#endif
					zRTime ctv;
					if (ptNullCmd->byParam == Cmd::MAGIC_USERCMD_PARA)
					{
						Cmd::stAttackMagicUserCmd *send = (Cmd::stAttackMagicUserCmd *)ptNullCmd;
						send->dwTimestamp = ctv.msecs();
						if(pUser && pUser->isPlayState())
						{
							if(forwardScene(send , nCmdLen))
							{
								return true;
							}
						}
						else
						{
							Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
						}
					}
					else
					{
						if(pUser && pUser->isPlayState())
						{
							if(forwardScene(ptNullCmd , nCmdLen))
							{
								return true;
							}
						}
						else
						{
							Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
						}
					}
					return true;
				}
				break;
			case DATA_USERCMD:
				{
					switch(ptNullCmd->byParam)
					{
						case LEVELDEGREE_DATA_USERCMD_PARA:
							{
								if(forwardSession(ptNullCmd , nCmdLen))
								{
									return true;
								}
							}
							break;
						case LOADMAPOK_DATA_USERCMD_PARA:
							{
								if(forwardScene(ptNullCmd , nCmdLen))
								{
									return true;
								}
							}
							break;
						default:
							{
								Zebra::logger->error("��Ч��DATA_USERCMD�ӿͻ��˷��͹���byParam=%u", ptNullCmd->byParam);
							}
							break;
					}
					return true;
				}
				break;
			case PET_USERCMD: 
				{
					if (ptNullCmd->byParam==Cmd::CHANGENAME_PET_PARA)
					{
						Cmd::stChangeNamePetCmd * cmd = (stChangeNamePetCmd *)ptNullCmd;
						if (!checkNewName(cmd->name))
						{
							zRTime ctv;
							Cmd::stChannelChatUserCmd send;
							send.dwType=Cmd::CHAT_TYPE_SYSTEM;
							send.dwSysInfoType = Cmd::INFO_TYPE_FAIL;
							send.dwChatTime = ctv.sec();
							bzero(send.pstrName, sizeof(send.pstrName));
							bzero(send.pstrChat, sizeof(send.pstrChat));
							strncpy((char *)send.pstrChat, "�������ֲ��Ϸ�����ʹ����ĸ�����ֺͺ���",MAX_CHATINFO-1);

							sendCmd(&send, sizeof(send));
							return true;
						}
					}
				}
				//ע������û��break
			case PROPERTY_USERCMD:
#ifdef _DEBUGLOG
				if (ptNullCmd->byCmd == PROPERTY_USERCMD) Zebra::logger->error("�յ��������Բ���ָ�");
#endif
				//ע������û��break
			case RELIVE_USERCMD:
				//ע������û��break
			case TRADE_USERCMD:
				//ע������û��break
			case MAPSCREEN_USERCMD:
				//ע������û��break
			case TASK_USERCMD: 
				{
					if(pUser && pUser->isPlayState())
					{
						if(forwardScene(ptNullCmd , nCmdLen))
						{
							return true;
						}
					}
					else
					{
							Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					}
					return true;
				}
				break;
			case GOLD_USERCMD:
				{
					if(GatewayService::service_gold)
					{
						if(pUser && pUser->isPlayState())
						{
							switch(ptNullCmd->byParam)
							{
								using namespace Bill;
								case QUERY_GOLD_PARA:
								{
								}
								break;
								//����һ����
								case REQUEST_REDEEM_GOLD_PARA:
								{
									using namespace Bill;
									stRequestRedeemGold* cmd = (stRequestRedeemGold*)ptNullCmd;
									if((int)cmd->dwNum < 0)
									{
										Zebra::logger->error("����һ����(accid=%d),����%d,�Ǹ���,�������",this->accid,cmd->dwNum);
										return true;
									}

									t_Request_Redeem_Gold_Gateway send;
									strncpy(send.account, this->account, MAX_ACCNAMESIZE);
									send.accid = this->accid;

									if (this->pUser)
									{
										send.charid = this->pUser->id;
									}

									send.point = cmd->dwNum;
									accountClient->sendCmd(&send, sizeof(send));	
									//Zebra::logger->debug("����һ����(accid=%d),����%d",this->accid,cmd->dwNum);
								}
								break;
								//����һ��¿�
								case REQUEST_REDEEM_MONTH_CARD_PARA:
								{
									using namespace Bill;

									t_Request_Redeem_MonthCard_Gateway send;
									strncpy(send.account, this->account, MAX_ACCNAMESIZE);
									send.accid = this->accid;

									if (this->pUser)
									{
										send.charid = this->pUser->id;
									}
									accountClient->sendCmd(&send, sizeof(send));	
									//Zebra::logger->debug("����һ��¿�(accid=%d)",this->accid);
								}
								break;
								//�����ѯ�㿨
								case REQUEST_POINT_PARA:
								{
									t_Request_Point_Gateway send;
									strncpy(send.account, this->account, MAX_ACCNAMESIZE);
									send.accid = this->accid;
									if (this->pUser)
									{
										send.charid = this->pUser->id;
									}
									accountClient->sendCmd(&send, sizeof(send));	
									//Zebra::logger->debug("�����ѯʣ�����(accid=%d)",this->accid);
								}
								break;
								case CONSUME_CARD_PARA:
								{
									Cmd::stConSumeCardCard *rev = (Cmd::stConSumeCardCard *)ptNullCmd; 
									stConSumeCardCard_Gateway send;
									send.type=rev->type;
									strncpy(send.account, this->account, MAX_ACCNAMESIZE);
									strncpy(send.cardid,rev->cardid,sizeof(send.cardid));
									send.accid = this->accid;
									accountClient->sendCmd(&send, sizeof(send));	
									Zebra::logger->debug("%s(%u)�������ѵ��߿�:%s,����:%d",pUser->name , pUser->id,rev->cardid,rev->type);
									return true;
								}
								break;
								default:
								break;

							}
						}
					}
					else
					{
						stStopServiceGold ret;
						this->sendCmd(&ret,sizeof(ret));
						Zebra::logger->debug("���ϵͳֹͣά��");
					}
					
					return true;
				}
				break;
			case QUIZ_USERCMD:
				{//��������
					switch(ptNullCmd->byParam)
					{
						case ANSWER_QUIZ_PARA:
							{
								if (forwardSession(ptNullCmd , nCmdLen))
								{
									return true;
								}
							}
							break;
						case QUERY_QUIZ_PARA:
							{
								if (forwardSession(ptNullCmd , nCmdLen) 
									&& forwardScene(ptNullCmd, nCmdLen))
								{
									return true;
								}
							}
							break;
						case CREATE_QUIZ_PARA:
							{
								if (forwardScene(ptNullCmd , nCmdLen))
								{
									return true;
								}	
							}
							break;
						case  QUIZ_EXIT_PARA:
							{
								if (forwardSession(ptNullCmd , nCmdLen))
								{
									return true;
								}
							}
							break;
						default:
							break;
					}

					return true;
				}
				break;
			case GMTOOL_USERCMD:
			case MAIL_USERCMD:
			case AUCTION_USERCMD:
			case CARTOON_USERCMD:
			case PRISON_USERCMD:
				{
					if (forwardScene(ptNullCmd , nCmdLen))
					{
						return true;
					}
					/*
					   switch(ptNullCmd->byParam)
					   {
					   case SEND_MAIL_PARA:
					   if (forwardSession(ptNullCmd , nCmdLen))
					   {
					   return true;
					   }
					   break;
					   default:
					   if (forwardScene(ptNullCmd , nCmdLen))
					   {
					   return true;
					   }
					   break;
					   }
					   */
					return true;
				}
				break;
			case STOCK_SCENE_USERCMD:
				{
					if(GatewayService::service_stock)
					{
						if(pUser && pUser->isPlayState())
						{
							if(forwardScene(ptNullCmd , nCmdLen))
							{
								return true;
							}
						}
						else
						{
							Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
						}
					}
					else
					{
						stStopServiceStockUserCmd ret;
						this->sendCmd(&ret,sizeof(ret));
						Zebra::logger->debug("��Ʊϵͳֹͣά��");
					}
					return true;
				}
				break;
			case STOCK_BILL_USERCMD:
				{
					if(GatewayService::service_stock)
					{
						if(pUser && pUser->isPlayState())
						{
							switch(ptNullCmd->byParam)
							{
								case PARA_CANCELLISTALL_STOCKPARA:
									{
										return true;
									}
									break;
								default:
									if(pUser->forwardBill(ptNullCmd,nCmdLen))
									{
										return true;
									}
							}
						}
						else
						{
							Zebra::logger->error("����״̬����ȷ,��������ָ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
						}
					}
					else
					{
						stStopServiceStockUserCmd ret;
						this->sendCmd(&ret,sizeof(ret));
						Zebra::logger->debug("��Ʊϵͳֹͣά��");
					}
					return true;
				}
				break;
			case GIFT_USERCMD:
				{
					if(pUser && pUser->isPlayState())
						if(forwardSession(ptNullCmd, nCmdLen))
							return true;
					else
						Zebra::logger->error("����״̬����ȷ,��������ָ��(%d, %d)",ptNullCmd->byCmd,ptNullCmd->byParam);
					return true;
				}
				break;
			case ALLY_USERCMD:
				{
					switch (ptNullCmd->byParam)
					{
						case REQ_COUNTRY_ALLY_INFO_PARA:
						case REQ_COUNTRY_ALLY_PARA:
						case CANCEL_COUNTRY_ALLY_PARA:
							{
								if (forwardSession(ptNullCmd , nCmdLen))
								{
									return true;
								}
							}
							break;
						/*case CREATE_QUIZ_PARA:
							{
								if (forwardScene(ptNullCmd , nCmdLen))
								{
									return true;
								}	
							}
							break;*/
						default:
							break;
					}

					return true;
				}
				break;
			case MINIGAME_USERCMD:
				{
					if(pUser && pUser->isPlayState())
					{
						Cmd::stMiniGameUserCmd * rev = (Cmd::stMiniGameUserCmd *)ptNullCmd;
						if (COMMON_MINI_PARA==rev->byParam && LOGIN_COMMON_MINI_PARA==rev->subParam)
						{
							Cmd::Mini::t_UserLogin_Gateway send;
							send.userID = pUser->id;
							send.countryID = pUser->countryID;
							send.face = pUser->face;
							send.gateServerID = GatewayService::getInstance().getServerID();
							send.sceneServerID = pUser->scene->getServerID();
							strncpy(send.name, pUser->name, MAX_NAMESIZE-1);
							return miniClient->sendCmd(&send, sizeof(send));
						}
						if (rev->byParam>10)
							return forwardScene(ptNullCmd , nCmdLen);
						else
							return forwardMini(ptNullCmd , nCmdLen);
					}
				}
				break;
			case RECOMMEND_USERCMD:
				{
					if(pUser && pUser->isPlayState())
					{
						if (forwardSession(ptNullCmd , nCmdLen))
						{
							return true;
						}
					}
				}
				break;
			default:
				break;
		}
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->byCmd, ptNullCmd->byParam, nCmdLen);
	return false;
}

/**
 * \brief ����´��������������Ƿ�Ϸ�
 *
 *
 * \param newName ������
 * \return �Ƿ�Ϸ�
 */
bool GatewayTask::checkNewName(char * newName)
{
	/*

		*/
	DWORD i = 0;
	while (newName[i]!=0 && i<strlen(newName) && i<16)
	{
		BYTE a = newName[i];
		if (a<0x80)//asc��
		{
			if (!((a>='0' && a<='9') || (a>='A' && a<='Z') || (a>='a' && a<='z')))
				return false;
		}
		else//����
		{
			if (!(i<strlen(newName)-1 && i<15)) return false;

			BYTE b = newName[i+1];
			if (b==0x7F || b<0x40 || b>0xFE)//β�ֽ��� 40-FE ֮�䣬�޳� xx7F һ����
				return false;

			WORD val = (a<<8)|b;
			if (!((val>=0xB0A1 && val<=0xF7FE)))//GB 2312 ���� 6763 ��
						//||(val>=0xB140 && val<=0xA0FE)//CJK���� 6080 ��
						//||(val>=0xAA40 && val<=0xFEA0)))//CJK ���ֺ������ĺ��� 8160 ��
				return false;
			i++;
		}
		i++;
	}

	if ((!wordFilter::getInstance().doFilter(newName, 16))
			||strstr(newName, "������")
			||strstr(newName, "GM")
			||strstr(newName, "GameMaster"))
		return false;

	return true;
}

