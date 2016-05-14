/**
 * \file
 * \version  $Id: CUnion.cpp  $
 * \author  
 * \date 
 * \brief ʵ�ְ�������
 *
 */

#include <stdarg.h>
#include "zRWLock.h"
#include "Union.h"
#include "CUnion.h"
#include "Command.h"
#include "zDBConnPool.h"
#include "SessionServer.h"
#include "Session.h"
#include "OfflineMessage.h"
#include "SessionCommand.h"
#include "zMetaData.h"
#include "CCityManager.h"
#include "CCountryManager.h"
#include "CSept.h"
#include "CDare.h"
#include "SessionChat.h"

using namespace UnionDef;

class CUnionSort 
{               
	public: 
		char  unionName[MAX_NAMESIZE];           // �������
		DWORD dwMana;                         // �������
		DWORD dwOrder;                          // �������
		DWORD dwCountryID;                      // ��������

		CUnionSort()
		{               
			dwMana = 0;
			dwOrder  = 0;
			dwCountryID = 0;
			bzero(unionName, MAX_NAMESIZE);
		}

		CUnionSort(const CUnionSort& ref)
		{
			dwMana = ref.dwMana;
			dwOrder = ref.dwOrder;
			dwCountryID = ref.dwCountryID;      
			strncpy(unionName, ref.unionName, MAX_NAMESIZE);
		}

		~CUnionSort()
		{
		}

		CUnionSort& operator =(const CUnionSort& ref)
		{       
			this->dwMana = ref.dwMana;
			this->dwOrder = ref.dwOrder;
			this->dwCountryID = ref.dwCountryID;
			strncpy(this->unionName, ref.unionName, MAX_NAMESIZE);
			return *this;
		}

		friend bool operator<(const CUnionSort& lhs, const CUnionSort& rhs)
		{       
			return lhs.dwMana > rhs.dwMana;
		}
};


//==[CUnionMember]==================================================

/**
* \brief ����Ա���캯������ʼ����������
* \author fqnewman
*/
CUnionMember::CUnionMember()
{
	destroy  = false;
	//user = NULL;
}

/**
* \brief ����Ա��ʼ��
* \param info ��Ա��Ϣ�ṹ
* \author fqnewman
*/
void CUnionMember::init(const stUnionMemberInfo& info)
{
	rwlock.wrlock();
	byPower[0] = 0x00;
	byPower[1] = 0x00;
	id = info.dwCharID;
	strncpy(name,info.name,MAX_NAMESIZE);
	strncpy(aliasname,info.aliasname,MAX_NAMESIZE);
	WD2BA(info.wdPower, byPower);
	wdOccupation = info.wdOccupation;
	septid = info.dwSeptID;
	byStatus = CUnionMember::Offline;                  // ��Ա״̬
	rwlock.unlock();
}

/**
* \brief ��ȡ��Ա�Ļ�����Ϣ
* \param info ���ص���Ϣ�ṹ
* \author fqnewman,zjw
*/
void CUnionMember::getMemberBaseInfo(struct Cmd::stUnionRember& info)
{
	rwlock.rdlock();
	strncpy(info.memberName,name,MAX_NAMESIZE);
	strncpy(info.aliasname,aliasname,MAX_NAMESIZE);
	info.byOnline = byStatus;
	
	UserSession *pUser = UserSessionManager::getInstance()->getUserByID(id);
	if (pUser)
	{
		if (pUser->occupation != this->wdOccupation)
		{
			this->wdOccupation = pUser->occupation;
			this->writeDatabase();
		}
		
		info.level = pUser->level;
		info.exploit = pUser->dwExploit;
	}
	else
	{
		info.level = 0;
		info.exploit = 0;
	}
	
	info.occupation = this->wdOccupation;
	rwlock.unlock();
}

/**
* \brief ���ͳ�Ա�İ�����ݸ��ͻ���
* \author fqnewman
*/
void CUnionMember::sendUserUnionPrivateData()
{
	Cmd::stReturnUnionMemberInfoCmd ret;

	rwlock.rdlock();
	strncpy(ret.alias,aliasname,MAX_NAMESIZE);     // ����
	ret.byPower[0]=byPower[0];
	ret.byPower[1]=byPower[1];
	rwlock.unlock();

	UserSession *pUser = UserSessionManager::getInstance()->getUserByID(id);
	if (pUser) pUser->sendCmdToMe(&ret,sizeof(ret));
}

/**
* \brief ������Ϣ����Ա��Ӧ�Ŀͻ���
* \param  pstrCmd ��Ϣ��
* \param  nCmdLen ��Ϣ����
* \author fqnewman
*/
void CUnionMember::sendCmdToMe(const Cmd::stNullUserCmd *pstrCmd, const unsigned int nCmdLen)
{
	UserSession *pUser = UserSessionManager::getInstance()->getUserByID(id);
	if (pUser)
	{
		if ((Cmd::CHAT_USERCMD == pstrCmd->byCmd) && 
				(ALL_CHAT_USERCMD_PARAMETER == pstrCmd->byParam))
		{

#ifdef _ZJW_DEBJG
			Zebra::logger->debug("���Ͱ�ṫ���:%s", pUser->name);
#endif	
		}	
		pUser->sendCmdToMe(pstrCmd,nCmdLen);
	}
	else
	{
		if ((Cmd::CHAT_USERCMD == pstrCmd->byCmd) && 
				(ALL_CHAT_USERCMD_PARAMETER == pstrCmd->byParam))
		{
			Cmd::stChannelChatUserCmd *pCmd = (Cmd::stChannelChatUserCmd *)pstrCmd;
			if (Cmd::CHAT_TYPE_UNION_AFFICHE == pCmd->dwType)
			{
				COfflineMessage::writeOfflineMessage(pCmd->dwType, this->id, pstrCmd, nCmdLen);
			}
		}
	}
}

/**
* \brief ���³�Ա�����ݿ�浵
* \author fqnewman
*/
void CUnionMember::writeDatabase()
{
	static const dbCol unionmember_define[] = {
		{ "`ALIASNAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "`POWER`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
		{ "`OCCUPATION`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
		{ NULL, 0, 0}
	};

	struct {
		char  aliasname[MAX_NAMESIZE+1]; 		// ��Ա����
		WORD  wdPower;                			// ��ԱȨ��
		WORD  wdOccupation;						// ��Ա��ְҵ
	} __attribute__ ((packed))
	updateunionmember_data;
	char where[128];

	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܻ�ȡ���ݿ���");
		return;
	}

	rwlock.rdlock();

	WORD wdValue;
	BA2WD(byPower, wdValue);
	updateunionmember_data.wdPower = wdValue;
	strncpy(updateunionmember_data.aliasname, aliasname, MAX_NAMESIZE);
	updateunionmember_data.wdOccupation = wdOccupation;

	rwlock.unlock();

	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "UNIONID = %u AND CHARID = %u", myUnion->getID(), id);
	unsigned int retcode = SessionService::dbConnPool->exeUpdate(handle, "`UNIONMEMBER`", unionmember_define, (BYTE *)(&updateunionmember_data), where);
	SessionService::dbConnPool->putHandle(handle);

	if ((DWORD)-1 == retcode)
	{
		Zebra::logger->error("CUnionMember �޸Ļ�Ա����ʧ�ܣ�UNIONID=%u CHARID=%u retcode=%u", myUnion->getID(), id, retcode);
	}
}

/**
* \brief ����Ա��¼�������ݿ�
* \author fqnewman
*/
void CUnionMember::insertDatabase()
{
	static const dbCol unionmember_define[] = {
		{ "`UNIONID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
		{ "`CHARID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
		{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "`ALIASNAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "`POWER`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
		{ "`OCCUPATION`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
		{ "`SEPTID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
		{ NULL, 0, 0}
	};
	struct {
		DWORD dwUnionID;						// �����
		DWORD	dwCharID;						// ��Ա��ɫID
		char  name[MAX_NAMESIZE+1];           // ��Ա����
		char  aliasname[MAX_NAMESIZE+1]; 		// ��Ա����
		WORD  wdPower;                		// ��ԱȨ��
		WORD  wdOccupation;						// ��Ա��ɫ
		DWORD dwSeptID;
	} __attribute__ ((packed))
	createunionmember_data;

	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܻ�ȡ���ݿ���");
		return;
	}

	//�������ݿ��ɫ��Ϣ
	bzero(&createunionmember_data, sizeof(createunionmember_data));

	rwlock.rdlock();

	createunionmember_data.dwUnionID = myUnion->getID();
	createunionmember_data.dwCharID = id;
	strncpy(createunionmember_data.name, name, MAX_NAMESIZE);
	strncpy(createunionmember_data.aliasname, aliasname, MAX_NAMESIZE);
	WORD wdValue;
	BA2WD(byPower, wdValue);
	createunionmember_data.wdPower = wdValue;
	createunionmember_data.wdOccupation = wdOccupation;
	createunionmember_data.dwSeptID = this->septid;
	
	rwlock.unlock();

	unsigned int retcode = SessionService::dbConnPool->exeInsert(handle, "`UNIONMEMBER`", unionmember_define, (const unsigned char *)(&createunionmember_data));
	SessionService::dbConnPool->putHandle(handle);
	if ((unsigned int)-1 == retcode)
	{
		Zebra::logger->error("�������Ա���ݿ���� %u %u, %s", myUnion->getID(), id, aliasname);
	}
	return;
}

/**
 * \brief �������ݵ�����
 *
 *
 * \param 
 * \return 
 */
void CUnionMember::update_data()
{
	Cmd::Session::t_sendUserRelationID send;
	send.dwUserID = this->id;
	send.dwID = this->myUnion->id;
	send.type = Cmd::Session::RELATION_TYPE_UNION;

	bzero(send.name, sizeof(send.name));
	send.caption = 0;
	send.unionmaster = false;
	send.septmaster = false;
	send.king = false;
	send.emperor = false;

	UserSession* pUser  = UserSessionManager::getInstance()->getUserByID(id);
	if (pUser)
	{
		// TODO:�ж��Ƿ��ǳ��������
		if (this->myUnion->master && this->myUnion->master->id == pUser->id)  
		{//�ǰ���
			send.unionmaster = true;
			send.dwActionPoint = this->myUnion->getActionPoint();

			if (CCityM::getMe().find(pUser->country, KING_CITY_ID, this->myUnion->id) !=NULL)
			{//�ǹ���
				CCountry* pCountry = CCountryM::getMe().find(pUser->country);
				if (pCountry) pCountry->updateKing(pUser);

				CCountry* pEmperorCountry = CCountryM::getMe().find(NEUTRAL_COUNTRY_ID);
				if (pEmperorCountry && pEmperorCountry->dwKingUnionID == pUser->unionid)
				{
					pEmperorCountry->updateKing(pUser);
					send.emperor = true;
				}
				send.king = true;
			}
		}
		
		if (CCityM::getMe().findByUnionID(this->myUnion->id) != NULL)
		{
			send.caption = CCityM::getMe().findByUnionID(this->myUnion->id)->dwCityID;
		}
		
		strncpy(send.name, this->myUnion->name, sizeof(send.name));

		pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_sendUserRelationID));
	}
}

/**
* \brief ����������������ʱ��ˢ�³�Ա�����ݿ�浵
* \author fqnewman
*/
CUnionMember::~CUnionMember()
{
//	if (!destroy) writeDatabase();
}

void CUnionMember::sendUnionDare(UserSession* pDareUser, const char* fromName, DWORD dwWarID)
{
	UserSession* pUser = UserSessionManager::getInstance()->getUserByID(this->id);

	if (pUser && pDareUser)
	{//�ҵ���ЧӦս��,���䷢����սѯ������
		Cmd::stActiveDareCmd send;

		send.dwMsgType = Cmd::DARE_QUESTION;
		send.dwWarID   = dwWarID;
		send.dwDareType = Cmd::UNION_DARE;

		strncpy(send.name, pDareUser->name, MAX_NAMESIZE);
		strncpy(send.fromRelationName, fromName, MAX_NAMESIZE);

		pUser->sendCmdToMe(&send, sizeof(send));
#ifdef _ZJW_DEBUG                   
		Zebra::logger->debug("��Ӧս�߷���ѯ�ʡ�");
#endif
	}
}

/**
* \brief ���ó�Ա������������
* \param pUnion ������ָ��
* \author fqnewman
*/
void CUnionMember::setUnion(CUnion * pUnion)
{
	rwlock.wrlock();
	myUnion = pUnion;
	rwlock.unlock();
}

/**
* \brief ����Ա�Ƿ����ĳ��Ȩ��
* \param power ������Ȩ����
* \author fqnewman
* \return true��Ȩ�� false��Ȩ
*/
bool CUnionMember::havePower(const int power)
{
	return Cmd::isset_state(byPower,power);
}

void  CUnionMember::setPower(const int power)
{
	Cmd::set_state(byPower, power);
}

/**
* \brief ����������Ϣ����Ա�Ŀͻ���
* \param message ��Ϣ��
* \author fqnewman
*/
void CUnionMember::sendMessageToMe(const char *message)
{
	UserSession *pUser = UserSessionManager::getInstance()->getUserByID(id);
	if (NULL !=pUser) pUser->sendSysChat(Cmd::INFO_TYPE_SYS, message);
}

/**
* \brief ������Ա�����ݿ��¼�ӿ���ɾ��
* \author fqnewman
* \return true �ɹ��� false ʧ��
*/
bool CUnionMember::deleteMeFromDB()
{
	char where[128];

	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܻ�ȡ���ݿ���");
		return false;
	}

	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "UNIONID = %u AND CHARID = %u", myUnion->getID(), id);
	unsigned int retcode = SessionService::dbConnPool->exeDelete(handle, "`UNIONMEMBER`", where);
	SessionService::dbConnPool->putHandle(handle);
	if ((unsigned int)-1 == retcode)
	{
		Zebra::logger->debug("ɾ������Աʧ�� %u", id);
		return false;
	}
	else
	{
		destroy = true;
	}
	return true;
}

/**
* \brief �����Ա����Ĺ�ϵ
* \param notify ֪ͨ��־��Ϊtrue��ʾҪ֪ͨ���е����߳�Ա��Ϊflase��ʾ��֪ͨ
* \author fqnewman
*/
void CUnionMember::fireMe(const bool notify, const bool checksept)
{
	if (deleteMeFromDB())
	{
		if (!CUnionM::getMe().removeMemberIndex(name))
		{
			Zebra::logger->error("[���]:%s ִ��removeMemberIndexʧ��", name);
		}
		
		if (notify) 
		{
			myUnion->notifyMemberFire(name,aliasname);
			myUnion->sendUnionNotify("%s(%s) �뿪�˰��", name, aliasname);	
		}

		CSept* pSept = CSeptM::getMe().getSeptByID(this->septid);
		if (pSept && checksept)
		{
			pSept->fireSeptMemberDirect(this->id, false);
		}
		
		UserSession *pUser = UserSessionManager::getInstance()->getUserByID(id);
		if (pUser)
		{
			Cmd::Session::t_fireUnionMember_SceneSession send;
			send.dwCharID = id;

			send.dwMapTempID = pUser->scene->tempid;
			pUser->scene->sendCmd(&send,sizeof(Cmd::Session::t_fireUnionMember_SceneSession)); /// ֪ͨ����������
			pUser->unionid = 0;
		}
	}
}

/**
* \brief �����û��İ���Ա����
* \author fqnewman
*/
void CUnionMember::sendUserUnionData()
{
	UserSession *pUser = UserSessionManager::getInstance()->getUserByID(id);
	if (myUnion && pUser)
	{
		myUnion->sendUnionInfoToUser(pUser); // ���Ͱ�����Ϣ����ǰ��Ա
		sendUserUnionPrivateData();         // ���͵�ǰ��Ա�ĸ�����Ϣ����ǰ��Ա
		myUnion->sendUnionMemberList(pUser); // ���Ͱ��ĳ�Ա�б����ǰ��Ա
	}
}

void CUnionMember::change_aliasname(const char* newaliasname)
{
	rwlock.wrlock();
	strncpy(this->aliasname, newaliasname, MAX_NAMESIZE);
	rwlock.unlock();
	this->writeDatabase();
	this->online(Cmd::UNION_MEMBER_STATUS_ALIASCHANGE);
}

/**
* \brief �жϳ�Ա�Ƿ�����
* \author fqnewman
* \return true����  false ������
*/
bool CUnionMember::isOnline()
{
    return (byStatus==CUnionMember::Online)?true:false;
}

/** 
* \brief ��Ա���ߴ���
* \param status ��Ա������״̬
* \author fqnewman
*/
void CUnionMember::online(const unsigned int status)
{
	Cmd::stBroadcastMemberInfo send;

	rwlock.wrlock();
	UserSession *pUser = UserSessionManager::getInstance()->getUserByID(id);
	if (pUser)
	{
		send.exploit = pUser->dwExploit;
		byStatus = CUnionMember::Online;
		wdOccupation = pUser->occupation;
		send.level = pUser->level;
	}
	
	send.byStatus = status;
	send.wdOccupation = wdOccupation;
	strncpy(send.name,name,MAX_NAMESIZE);
	strncpy(send.aliasname, aliasname, MAX_NAMESIZE);
	if (pUser) wdOccupation = pUser->occupation;
	rwlock.unlock();
	
	CSept* pSept = CSeptM::getMe().getSeptByID(this->septid);
	if (pSept)
	{
		if (pSept->tempid == this->id)
		{
			send.bySeptMaster = 1;
		}
		else
		{
			send.bySeptMaster = 0;
		}
		strncpy(send.septName, pSept->name, MAX_NAMESIZE);
	}
	else
	{
		bzero(send.septName, MAX_NAMESIZE);
	}
	
		
			
	if (myUnion && myUnion->master) 
	{
		if (myUnion->master->id == this->id && pUser && myUnion->dwCountryID<1)
		{
			myUnion->dwCountryID = pUser->country;
			myUnion->writeDatabase();
		}
		
		myUnion->sendCmdToAllMember(&send,sizeof(send));
	}
	else
	{
		Zebra::logger->error("�ڰ���Ա%s�Ҳ����Լ��İ�����",send.name);
	}
}

/**
* \brief ��Ա���ߴ���
* \author fqnewman
*/
void CUnionMember::offline()
{
	Cmd::stBroadcastMemberInfo send;

	rwlock.wrlock();
	byStatus = CUnionMember::Offline; 
	//user = NULL;
	send.byStatus = Cmd::UNION_MEMBER_STATUS_OFFLINE;
	strncpy(send.name,name,MAX_NAMESIZE);
	strncpy(send.aliasname, aliasname, MAX_NAMESIZE);
	rwlock.unlock();
	if (myUnion) myUnion->sendCmdToAllMember(&send,sizeof(send));
	else
	{
		Zebra::logger->error("�ڰ���Ա%s�Ҳ����Լ��İ�����",send.name);
	}
}

//---------------------------------------new---------------------------------------------

/**
* \brief ɾ��ʵ���������
* \param name ʵ������
* \author fqnewman
*/
void CUnion::removeEntryByName(const char * name)
{
	zEntry *rm=getEntryByName(name);
	removeEntry(rm);
}

/**
* \brief ����ʼ�������ݴ������ݳ�ʼ�����Ļ�����Ϣ
* \param info ������ݽṹ
* \author fqnewman
*/
void CUnion::init(const stUnionInfo & info)
{
	rwlock.wrlock();		
	id = info.dwUnionID;
	strncpy(name,info.name,MAX_NAMESIZE);
	byVote = info.byVote;
	level = info.wdLevel;
	exp   = info.qwExp;
	dwMana = info.dwMana;
	dwCountryID = info.dwCountryID;
	tempid = info.dwCharID;
	dwCreateTime = info.dwCrTime;
	dwActionPoint = info.dwActionPoint;
	calltimes = info.calltimes;
	dwMoney	= info.dwMoney;
	strncpy(note, info.note, sizeof(note));
	master = NULL;
	rwlock.unlock();
}

/**
* \brief ����һ�����İ�����Ա������Ȩ��
* \param info ����Ա���ݽṹ
* \author fqnewman
*/
CUnionMember * CUnion::addUnionMaster(const stUnionInfo& info)
{
	stUnionMemberInfo masterInfo;

	masterInfo.dwCharID = info.dwCharID;
	UserSession *pUser = UserSessionManager::getInstance()->getUserByID(masterInfo.dwCharID);
	if (pUser) 
	{
		masterInfo.wdOccupation = pUser->occupation;
		masterInfo.dwSeptID = pUser->septid;
	}

	strncpy(masterInfo.name,info.masterName,MAX_NAMESIZE);
	strncpy(masterInfo.aliasname,DEFAULTMASTERALIAS,MAX_NAMESIZE);
	SETMASTERPOWER_WD(masterInfo.wdPower);
	
	return addUnionMember(masterInfo);	
}

/**
* \brief ��������ʼ������Ա
* \param info ����Ա���ݽṹ
* \author fqnewman
* \return �ɹ������´����İ���Ա����ʧ�ܷ���NULL
*/
CUnionMember * CUnion::addUnionMember(const stUnionMemberInfo& info)
{
	CUnionMember *pMember = NULL;

	pMember = new CUnionMember();
	pMember->init(info);
	
	if (pMember)
	{
		pMember->setUnion(this);
		
		if (info.wdPower == 0)
		{//���³�ʼ������ԱȨ��
			pMember->byPower[0] = 0x00;
			pMember->byPower[1] = 0x00;

			if (info.dwCharID == this->tempid)
			{//���ð���Ȩ��
				SETMASTERPOWER(pMember->byPower);
			}
			else
			{//����Ϊ����Ȩ��
				SETMEMBERPOWER(pMember->byPower);
			}
			pMember->writeDatabase();
		}
		
		rwlock.wrlock();
		if (!addEntry(pMember))
		{
			Zebra::logger->debug("[���]: %s �����ӳ�Ա %s ����������ʧ��", this->name, info.name);
		}
		rwlock.unlock();
		CUnionM::getMe().addMemberIndex(info.name, pMember);  // ��CUnionM�м���������������ʹ����ķ�����ͨ��CUnionM�ҵ��Լ���
	}
	return pMember;
}

/**
* \brief ��������������������г�Ա��ս��״̬��֪ͨ�ͻ���
* \param ptEnterWarCmd ����ս��״̬֪ͨ��Ϣ
* \param cmdLen ��Ϣ����
*/
void CUnion::sendCmdToAllMemberScene(Cmd::Session::t_enterWar_SceneSession* ptEnterWarCmd, const unsigned int cmdLen)
{
	struct findall : public execEntry<CUnionMember> 
	{
		Cmd::Session::t_enterWar_SceneSession* cmd;
		unsigned int cmdLen;

		findall(Cmd::Session::t_enterWar_SceneSession* ptCmd, const unsigned int nCmdLen)
		{
			cmd = ptCmd;
			cmdLen = nCmdLen;
		}

		~findall(){}
		bool exec(CUnionMember* pMember)
		{
			UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pMember->id);
			if (pUser && pUser->scene)
			{
				cmd->dwUserID = pUser->id;
				cmd->dwSceneTempID = pUser->scene->tempid;
				if (cmd->dwStatus)
				{
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���ѽ������ս״̬��");
				}
				else
				{
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�����˳�����ս״̬��");
				}
				pUser->scene->sendCmd(cmd, cmdLen);
			}
			return true;
		}
	};

	findall myList(ptEnterWarCmd, cmdLen);
	execEveryOne(myList);
}
void CUnion::sendCallCmdToAllMemberScene(Cmd::Session::t_GoTo_Leader_Check_SceneSession* ptCmd, const unsigned int cmdLen,DWORD expect)
{
	struct findall : public execEntry<CUnionMember> 
	{
		Cmd::Session::t_GoTo_Leader_Check_SceneSession* cmd;
		unsigned int cmdLen;
		DWORD expect;

		findall(Cmd::Session::t_GoTo_Leader_Check_SceneSession* ptCmd, const unsigned int nCmdLen,DWORD exp)
		{
			cmd = ptCmd;
			cmdLen = nCmdLen;
			expect=exp;
		}

		~findall(){}
		bool exec(CUnionMember* pMember)
		{
			UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pMember->id);
			if (pUser && pUser->scene)
			{
				if(pUser->tempid != expect)
				{
					cmd->userTempID = pUser->tempid;
					pUser->scene->sendCmd(cmd, cmdLen);
				}
			}
			return true;
		}
	};

	findall myList(ptCmd, cmdLen,expect);
	execEveryOne(myList);
}

void CUnion::fireUnionMemberLeave(DWORD dwUserID)
{
	CUnionMember * pMember = NULL;
	pMember = (CUnionMember *)getEntryByID(dwUserID);

	if (pMember)
	{
		CSept* pSept = CSeptM::getMe().getSeptByID(pMember->septid);

		if (pSept && pSept->master && pSept->master->id == dwUserID)
		{
			this->fireSeptFromUnion(pMember->septid);
		}       
		else
		{
			this->fireUnionMemberDirect(dwUserID, false);
		}
	}
}

void CUnion::sendUnionDare(UserSession* pUser, const char* fromName, DWORD dwWarID)
{
	struct findall : public execEntry<CUnionMember>
	{
		UserSession* _pUser;
		char _fromName[MAX_NAMESIZE];
		DWORD _dwWarID;
		findall(UserSession* pUser, const char* fromName, DWORD dwWarID)
		{
			_pUser = pUser;
			strncpy(_fromName, fromName, MAX_NAMESIZE);
			_dwWarID = dwWarID;
		}
		~findall(){}

		bool exec(CUnionMember *pMember)
		{
			if (pMember->havePower(Cmd::DARE))
			{
				pMember->sendUnionDare(_pUser, _fromName, _dwWarID);
			}
			return true;
		}
	};

	if (1==byVote) return;
	findall myList(pUser, fromName, dwWarID);
	execEveryOne(myList);
}

/**
* \brief ������������еĳ�Ա���ͻ��ˣ�
* \param ptCmd ��Ϣ��
* \param nCmdLen ��Ϣ����
* \author fqnewman
*/
void CUnion::sendCmdToAllMember(const Cmd::stNullUserCmd * ptCmd, const unsigned int nCmdLen )
{
	struct findall : public execEntry<CUnionMember>
	{
		const Cmd::stNullUserCmd * cmd;
		unsigned int cmdLen;
		findall(const Cmd::stNullUserCmd * ptCmd, const unsigned int nCmdLen)
		{
			cmd = ptCmd;
			cmdLen = nCmdLen;
		}
		~findall(){}

		bool exec(CUnionMember *pMember)
		{
			pMember->sendCmdToMe(cmd,cmdLen);
			return true;
		}
	};

	if (1==byVote) return;
	findall myList(ptCmd,nCmdLen);
	execEveryOne(myList);
}

/**
* \brief ֪ͨ������г�Ա������
* \param pName �������ߵ�����
* \param pAliasname �������ߵı���
* \author fqnewman
*/
void CUnion::notifyMemberFire(const char * pName, const char *pAliasname)
{
	Cmd::stBroadcastMemberInfo send;

	send.byStatus = Cmd::UNION_MEMBER_STATUS_FIRE;
	strncpy(send.name,pName,MAX_NAMESIZE);
	strncpy(send.aliasname, pAliasname, MAX_NAMESIZE);
	sendCmdToAllMember(&send,sizeof(send));
}

/**
* \brief �����ʽ�����ĺ�
*/

#define getMessage(msg,msglen,pat)      \
do      \
{       \
	va_list ap;     \
		bzero(msg, msglen);     \
		va_start(ap, pat);              \
		vsnprintf(msg, msglen - 1, pat, ap);    \
		va_end(ap);     \
}while(false)

/**
* \brief ֪ͨ���ս���
* \param  msg ... ֪ͨ����
*/
void CUnion::notifyWarResult(const char* msg, ...)
{
	char buf[1024+1];
	bzero(buf, sizeof(buf));

	struct findall : public execEntry<CUnionMember> 
	{
		const char* msg;

		findall(const char* pMsg)
		{
			msg = pMsg;
		}

		~findall(){}

		bool exec(CUnionMember* pMember)
		{
			UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pMember->id);
			if (pUser)
			{
				pUser->sendSysChat(Cmd::INFO_TYPE_SYS, msg);
			}

			return true;
		}
	};

	getMessage(buf, 1024, msg);

	findall myList(buf);
	execEveryOne(myList);



}

/**
* \brief �����û��İ�����ݵ��ͻ���
* \param pName ������Ϣ���û�
* \author fqnewman
*/
void CUnion::sendUserUnionData(const char *pName)
{
	CUnionMember *pMember;

	pMember = (CUnionMember *)getEntryByName(pName);
	if (pMember)
	{
		pMember->sendUserUnionData();
	}
}

/**
* \brief ���Ͱ����Ϣ��ָ���û��Ŀͻ��ˣ������û����߳�ʼ����ʱ��
* \param pUser �����û�
* \author fqnewman
*/
void CUnion::sendUnionInfoToUser(UserSession *pUser)
{
	Cmd::stUnionBaseInfoCmd retUnion;

	rwlock.rdlock();
	strncpy(retUnion.unionName,name,MAX_NAMESIZE);		// �������
	if (master) strncpy(retUnion.master,master->name,MAX_NAMESIZE);	// ���᳤
	else  strncpy(retUnion.master,"��¼���������֤",MAX_NAMESIZE);	// ���᳤
	retUnion.wdLevel 	= level;						// ��ἶ��
	retUnion.qwExp 		= exp;							// ��ᾭ��
	retUnion.dwMana		= this->getMana();
	retUnion.dwAction	= this->dwActionPoint;
	strncpy(retUnion.note, this->note, sizeof(retUnion.note));
	rwlock.unlock();
	pUser->sendCmdToMe(&retUnion,sizeof(retUnion));
}

DWORD CUnion::getMana()
{
	struct findList : public execEntry<CUnionMember>
	{
		DWORD _dwMana;
		
		findList()
		{
			_dwMana = 0;
		}
		~findList(){}
		bool exec(CUnionMember *pMember)
		{
			if (pMember)
			{
				CSept* pSept = CSeptM::getMe().getSeptByID(pMember->septid);
				if (pSept && pSept->master && pSept->master->id==pMember->id)
				{
					_dwMana = _dwMana + pSept->getRepute();
				}
			}
			
			return true;
		}
	};

	findList myList;
	execEveryOne(myList);
	return (myList._dwMana+this->dwMana);

}

/**
* \brief ���Ͱ���Ա�б�������������ƣ����ֻ��100������Ա��¼��
* \param pUser ���ݽ����߶���
* \author fqnewman
*/
void CUnion::sendUnionMemberList(UserSession *pUser)
{
	BYTE buf[zSocket::MAX_DATASIZE];
	unsigned int count;
	Cmd::stUnionRember *tempPoint;

	std::list<CUnionMember *> _deleteList;
        std::list<CUnionMember *>::iterator vIterator;
	
	rwlock.rdlock();
	Cmd::stAllUnionMemberCmd *retCmd=(Cmd::stAllUnionMemberCmd *)buf;
	constructInPlace(retCmd);
	tempPoint = (Cmd::stUnionRember *)retCmd->memberList;
	unsigned int len = sizeof(Cmd::stUnionRember);
	count = 0;
	
	for (zEntryName::hashmap::iterator it=zEntryName::ets.begin();it!=zEntryName::ets.end();it++)
	{
		struct Cmd::stUnionRember mInfo;
		CUnionMember *temp=(CUnionMember *)it->second;
		CSept* pSept = CSeptM::getMe().getSeptByID(temp->septid);

		if (pSept == NULL || pSept->master == NULL)
		{
			_deleteList.push_back(temp);
			Zebra::logger->error("[���]: %s ��������(%d)�Ѳ�����", temp->name, temp->id);
			continue;
		}

#ifdef _ZJW_DEBUG			
		Zebra::logger->debug("����:%s", temp->name);
#endif			
		temp->getMemberBaseInfo(mInfo);
		bcopy(&mInfo,tempPoint,len);
		strncpy(tempPoint->septName, pSept->name, MAX_NAMESIZE);
		if (pSept->master && pSept->master->id!=temp->id &&  pSept->getMemberByName(temp->name) != NULL)
		{
			strncpy(tempPoint->aliasname, pSept->getMemberByName(temp->name)->aliasname, MAX_NAMESIZE);
		}

		if (pSept->tempid == temp->id)
		{
			tempPoint->bySeptMaster = 1;
		}
		else
		{
			tempPoint->bySeptMaster = 0;
		}

		tempPoint++;
		count++;

		//pSept->full_SeptMember_Data(tempPoint, count);

		if (1000==count)
		{
			goto breakfor; // ����¼����100��ʱ��ᳬ������͵��������
		}
	}
breakfor:
	rwlock.unlock();
	
	retCmd->size = count;
	pUser->sendCmdToMe(retCmd,(count*sizeof(Cmd::stUnionRember)+sizeof(Cmd::stAllUnionMemberCmd)));
}

/**
* \brief ��������Ա
* \param master ��ʹȨ���ߵ����� 
* \param member �������ߵ�����
* \author fqnewman
* \return 
*/
void  CUnion::fireUnionMember(const char * master, const char *member)
{
	CUnionMember *pMaster = (CUnionMember *)getEntryByName(master);
	CUnionMember *pMember = (CUnionMember *)getEntryByName(member);
	
	if (pMaster)
	{
		if (pMember)
		{
			if (pMaster->id == pMember->id)
			{
				pMaster->sendMessageToMe("���ܿ����Լ�");
				return;
			}

			if (pMember->id == pMaster->myUnion->master->id)
			{
				pMaster->sendMessageToMe("���ܿ�������");
				return;
			}
			
			CUnion* pUnion = pMaster->myUnion;

			if (pUnion && (pUnion->master==pMaster || pMaster->havePower(Cmd::FIRE_MEMBER)) )
			{
				CSept* pSept = CSeptM::getMe().getSeptByID(pMember->septid);

				if (pSept && pSept->master && pSept->master->id == pMember->id)
				{
					this->fireSeptFromUnion(pMember->septid);
				}
				else
				{
					pMaster->sendMessageToMe("ֻ�ܿ����峤");
				}
			}
			else
			{
				pMaster->sendMessageToMe("��û�п�����Ա��Ȩ��������㷢�ִ����뱨���GM");
			}
		}
		else
		{
			pMaster->sendMessageToMe("�����ڵİ����û�д���,��ȷ�������Ƿ���ȷ");
		}
	}
	else
	{
		Zebra::logger->debug("��������Ա�����г��ִ����״̬");
	}
}

/**
* \brief ֱ�ӿ�������Ա
* \param dwCharID �������İ���ԱID
* \author fqnewman
* \return ɾ���ɹ�����1 ɾ��ʧ�ܷ���2
*/
int CUnion::fireUnionMemberDirect(const DWORD dwCharID, const bool checksept)
{
	int ret = 0;

	rwlock.wrlock();
	CUnionMember * pMember = NULL;
	pMember = (CUnionMember *)getEntryByID(dwCharID);
	removeEntry(pMember);
	rwlock.unlock();

	if (pMember)
	{
		pMember->fireMe(true, checksept);
		SAFE_DELETE(pMember);
		ret = 1;
	}
	else
	{
		ret = 2;
	}

	return ret;
}

bool  CUnion::isMember(DWORD dwUserID)
{
	CUnionMember * pMember = NULL;
	pMember = (CUnionMember *)getEntryByID(dwUserID);

	if (pMember)
	{
		return true;
	}

	return false;
}

/**
* \brief �������ݿ��¼
* \author fqnewman
*/
void CUnion::writeDatabase()
{
	const dbCol union_update_define[] = {
		{ "`CHARID`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`VOTE`",				zDBConnPool::DB_BYTE,	sizeof(BYTE) },
		{ "`LEVEL`",				zDBConnPool::DB_WORD,		sizeof(WORD) }, 
		{ "`EXP`",				zDBConnPool::DB_QWORD,	sizeof(QWORD) },
		{ "`MANA`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`COUNTRYID`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`ACTIONPOINT`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`MONEY`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`MASTER`",     	                zDBConnPool::DB_STR,    sizeof(char[MAX_NAMESIZE+1]) },
		{ "`NOTE`",				zDBConnPool::DB_STR,	sizeof(char[254+1])},
		{ "`CALLTIMES`",		zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ NULL, 0, 0}
	};
	struct {
		DWORD dwCharid;
		BYTE  byVote;
		WORD 	wdLevel;
		QWORD qwExp;
		DWORD dwMana;
		DWORD dwCountryID;
		DWORD dwActionPoint;
		DWORD dwMoney;
		char  master[MAX_NAMESIZE+1];
		char  note[254+1];
		DWORD calltimes;
	} __attribute__ ((packed))
	updateunion_data;
	char where[128];

	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܻ�ȡ���ݿ���");
		return;
	}

	bzero(&updateunion_data,sizeof(updateunion_data));
	rwlock.rdlock();
	
	if (master) 
	{
		updateunion_data.dwCharid = master->id;
	}
	
	updateunion_data.dwCountryID = this->dwCountryID;
	updateunion_data.dwMana = this->dwMana;	
	updateunion_data.dwActionPoint = this->dwActionPoint;
	updateunion_data.dwMoney = this->dwMoney;
	updateunion_data.byVote = byVote;
	updateunion_data.wdLevel  = level;
	updateunion_data.qwExp 		= exp;
	updateunion_data.calltimes	= this->calltimes;
#ifdef _ZJW_DEBUG	
	if (updateunion_data.dwActionPoint>1000)
	{
		Zebra::logger->trace("[���]:����,�ж�������:%d", updateunion_data.dwActionPoint);
	}
#endif	

	if (master)
	{
		strncpy(updateunion_data.master, this->master->name, MAX_NAMESIZE);
	}

	strncpy(updateunion_data.note,note, sizeof(updateunion_data.note));
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "`UNIONID` = %u", id);
	rwlock.unlock();
	unsigned int retcode = SessionService::dbConnPool->exeUpdate(handle, "`UNION`", union_update_define, (BYTE *)(&updateunion_data), where);
	SessionService::dbConnPool->putHandle(handle);

	if ((DWORD)-1 == retcode)
	{
		Zebra::logger->error("CUnion �޸Ļ�Ա����ʧ�ܣ�UNIONID=%u retcode=%u", id, retcode);
	}
}

/**
* \brief �����ᣬ��ʼ������
* \author fqnewman
*/
CUnion::CUnion()
{
	destroy = false;
	master = NULL;
	byVote  = 1;
	calltimes = 0;
}

/**
* \brief ������ἰ�����г�Ա
* \author fqnewman
*/
CUnion::~CUnion()
{
	if (!destroy)
	{
		writeDatabase();
		rwlock.wrlock();
		for(zEntryName::hashmap::iterator it=zEntryName::ets.begin();it!=zEntryName::ets.end();it++)
		{
			CUnionMember *temp = (CUnionMember *)it->second;
			SAFE_DELETE(temp);
		}
		clear();
		rwlock.unlock();
	}
}

/**
* \brief ��ɢ��ᣬ�����г�Ա�İ���ϵ������������Լ�
* \author fqnewman
*/
void CUnion::disbandUnion()
{
	rwlock.wrlock();
	Zebra::logger->trace("[���]����ɢ%s", this->name);
	destroy = true;
	for(zEntryName::hashmap::iterator it=zEntryName::ets.begin();it!=zEntryName::ets.end();it++)
	{
		CUnionMember *temp=(CUnionMember *)it->second;
		if (temp)
		{
			CSept* pSept = CSeptM::getMe().getSeptByID(temp->septid);

			if (pSept && pSept->master && pSept->master->id == temp->id)
			{
				pSept->dwUnionID = 0;
				pSept->writeDatabase();
			}

			temp->fireMe(false, false);
			SAFE_DELETE(temp);
		}
	}

	//TODO:ͬʱ���øð�ӵ�еĳ��г�����Ϣ
	CCity* pCity =  CCityM::getMe().findByUnionID(this->id);	
	if (pCity)
	{
		if (pCity->dwCityID == KING_CITY_ID)
		{
			CCountry* pCountry = CCountryM::getMe().find(pCity->dwCountry, this->id);
			if (pCountry)
			{
				pCountry->changeKing(NULL);
			}
		}
		
		pCity->dwUnionID = 0;
		pCity->writeDatabase();
	}
	
	clear();
	deleteMeFromDB();
	rwlock.unlock();
}

bool CUnion::isDel()
{
	DWORD septs = this->septSize();
	return septs>1?false:true;
}

DWORD CUnion::septSize()
{
	DWORD ret=0;
	
	for(zEntryName::hashmap::iterator it=zEntryName::ets.begin();it!=zEntryName::ets.end();it++)
	{
		CUnionMember *temp=(CUnionMember *)it->second;
		if (temp)
		{
			CSept* pSept = CSeptM::getMe().getSeptByID(temp->septid);
			if (pSept && pSept->master && pSept->master->id == temp->id)
			{//ͳ���峤
				ret++;
			}
		}
	}
	
	return ret;
}

void CUnion::refreshSeptExp()
{
	if (CCityM::getMe().findByUnionID(this->id) != NULL)
	{
		for(zEntryName::hashmap::iterator it=zEntryName::ets.begin();it!=zEntryName::ets.end();it++)
		{
			CUnionMember *temp=(CUnionMember *)it->second;
			if (temp)
			{
				CSept* pSept = CSeptM::getMe().getSeptByID(temp->septid);
				if (pSept && pSept->master && pSept->master->id == temp->id)
				{//���ü��徭����ȡ��־
					Zebra::logger->trace("[����]: %s ˢ�¼��徭��", 
							pSept->name);
					pSept->setExp();
				}
			}
		}
	}
}

void CUnion::changeAllSeptRepute(int repute)
{
	for(zEntryName::hashmap::iterator it=zEntryName::ets.begin();it!=zEntryName::ets.end();it++)
	{
		CUnionMember *temp=(CUnionMember *)it->second;
		if (temp)
		{
			CSept* pSept = CSeptM::getMe().getSeptByID(temp->septid);
			if (pSept && pSept->master && pSept->master->id == temp->id)
			{//ͳ���峤
				pSept->changeRepute(repute);
			}
		}
	}
}
/**
* \brief ɾ�����������ݿ��¼
* \author fqnewman
* \return true �ɹ� false ʧ��
*/
bool CUnion::deleteMeFromDB()
{
	char where[128];

	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܻ�ȡ���ݿ���");
		return false;
	}

	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "UNIONID = %u ", id);
	unsigned int retcode = SessionService::dbConnPool->exeDelete(handle, "`UNION`", where);
	SessionService::dbConnPool->putHandle(handle);
	if ((unsigned int)-1 == retcode)
	{
		Zebra::logger->error("ɾ�����ʧ�� %u", id);
		return false;
	}
	else
	{
		destroy = true;
	}
	return true;
}

/**
* \brief �����ݿ��м��ذ���Ա
* \author fqnewman
* \return true ���سɹ��� false ����ʧ��
*/
bool CUnion::loadUnionMemberFromDB()
{
	static const dbCol unionmember_define[] = {
		{ "CHARID",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "NAME",			zDBConnPool::DB_STR,	sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "ALIASNAME",		zDBConnPool::DB_STR,	sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "POWER",			zDBConnPool::DB_WORD,	sizeof(WORD) },
		{ "`OCCUPATION`",	zDBConnPool::DB_WORD,	sizeof(WORD) }, 
		{ "SEPTID",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ NULL, 0, 0}
	};

	stUnionMemberInfo *memberList, *tempPoint;
	stUnionMemberInfo info;
	char where[128];

	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܻ�ȡ���ݿ���");
		return false;
	}
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "UNIONID = %u", id);
	unsigned int retcode = SessionService::dbConnPool->exeSelect(handle, "`UNIONMEMBER`", unionmember_define, where, NULL, (unsigned char **)&memberList);
	SessionService::dbConnPool->putHandle(handle);
	if ((unsigned int)-1 == retcode
			|| 0 == retcode)
	{
		Zebra::logger->debug("û���ҵ�����Ա��¼");
		return true;
	}

	if (memberList)
	{
		tempPoint = &memberList[0];
		for (unsigned int i=0; i< retcode; i++)
		{
			bcopy(tempPoint, &info, sizeof(stUnionMemberInfo));
			CUnionMember *member = addUnionMember(info);
			if (info.dwCharID == tempid) 
			{
				//Zebra::logger->trace("[������]:%s(%d) �������سɹ�", member->name, this->id);
				master = member; // masterid��������tempid�У������ʼ��master����
			}
			
			if (member->septid>0)
			{
				CSept* pSept = (CSept*)(CSeptM::getMe().getSeptByID(member->septid));
				if (pSept==NULL && this->tempid!=member->id)
				{
					this->fireUnionMemberLeave(member->id);
					Zebra::logger->trace("[���]: %s ���������Ѿ�������,�Ӱ���Ա����ɾ��", member->name);
				}
				else
				{
					if (pSept && !pSept->isMember(member->id) && this->tempid!=member->id)
					{
						this->fireUnionMemberLeave(member->id);
						Zebra::logger->trace("[���]: %s �Ѳ��ڶ�Ӧ������,�Ӱ���Ա����ɾ��", member->name);
					}
					else if (pSept && pSept->dwUnionID!=this->id && this->tempid!=member->id)
					{
						this->fireUnionMemberLeave(member->id);
						Zebra::logger->trace("[���]: %s ���������ͬһ���,�Ӱ���Ա����ɾ��", 
								member->name);
					}
				}
			}
			// */

			tempPoint++;
		}
		SAFE_DELETE_VEC(memberList);
	}
	else
	{
		Zebra::logger->error("������ݳ�ʼ��ʧ�ܣ�exeSelect ������Чbufָ��");
	}

	if (master == NULL)
	{
		Zebra::logger->trace("[������]:%d ��������ʧ��", this->id);
	}
	
	return true;
}

/**
* \brief ���Ͱ��֪ͨ
*
* \param message ��Ϣ
*
* \author zjw
*/
void CUnion::sendUnionNotify(const char* message, ...)
{
	if (message == NULL)
	{
		return;
	}

	char buf[1024+1];
	bzero(buf, sizeof(buf));
	getMessage(buf, 1024, message);
	
	Cmd::stChannelChatUserCmd send;
//	send.dwType=Cmd::CHAT_TYPE_UNION;
	send.dwType = Cmd::CHAT_TYPE_SYSTEM;
	send.dwSysInfoType = Cmd::INFO_TYPE_EXP;
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));
	
	sprintf((char*)send.pstrName, "���֪ͨ");
	sprintf((char*)send.pstrChat, "%s", buf);

	this->sendCmdToAllMember(&send, sizeof(send));
}

/**
* \brief ��ȡ����Ա��Ŀ
* \author fqnewman
* \return ����Ա����
*/
DWORD CUnion::size()
{
	return zEntryName::size();
}

/**
* \brief �ж��Ƿ���ͶƱ״̬
* \author fqnewman
* \return true ����ͶƱ�� false ͶƱ�ѽ���
*/
bool CUnion::isVote()
{
	return 1==byVote;
}

void CUnion::update_all_data()
{
	struct findList : public execEntry<CUnionMember>
	{
		findList()
		{
		}
		~findList(){}
		bool exec(CUnionMember *pMember)
		{
			pMember->update_data();
			return true;
		}
	};

	findList myList;
	execEveryOne(myList);
}

/**
* \brief ͶƱ��������
* \author fqnewman
*/
void CUnion::letVoteOver()
{
	struct findList : public execEntry<CUnionMember>
	{
		findList()
		{
		}
		~findList(){}
		bool exec(CUnionMember *pMember)
		{
			UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pMember->id);
			if (pUser)
			{
				//pMember->online();
				pMember->sendUserUnionData();
				pMember->update_data();
				pUser->unionid = pMember->myUnion->id;
			}
			return true;
		}
	};

	byVote = 0;
	findList myList;
	execEveryOne(myList);
	writeDatabase();
}

/**
* \brief ���ð��Ľ��ܣ�����⣩
* \param pCmd ���������Ϣ
* \author fqnewman
*/
void CUnion::setNote(Cmd::stNoteUnionCmd *pCmd)
{
	rwlock.wrlock();
	strncpy(note,pCmd->noteBuf, sizeof(note));
	rwlock.unlock();
	writeDatabase();
	sendCmdToAllMember(pCmd,sizeof(Cmd::stNoteUnionCmd));
}

void  CUnion::changeMaster(CUnionMember* pMember)
{
	rwlock.wrlock();

	//UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pMember->id);
	
	// �ݴ��������
	CUnionMember* pSrcMaster = this->master;
	
	// ���µİ�������
	this->master = pMember;
	this->tempid = pMember->id;

	// �޸����ߵ�Ȩ�޼��ƺ�
	strncpy(this->master->aliasname, DEFAULTMASTERALIAS, MAX_NAMESIZE);
	this->master->byPower[0] = 0x00;
	this->master->byPower[1] = 0x00;

	SETMASTERPOWER(this->master->byPower);
	
	pSrcMaster->byPower[0] = 0x00;
	pSrcMaster->byPower[1] = 0x00;	
	strncpy(pSrcMaster->aliasname, "�峤", MAX_NAMESIZE);
	SETMEMBERPOWER(pSrcMaster->byPower);

	pSrcMaster->writeDatabase();
	this->master->writeDatabase();	
	rwlock.unlock();
	
	this->writeDatabase();
	
	struct findall : public execEntry<CUnionMember>
	{
		findall()
		{
		}
		~findall(){}

		bool exec(CUnionMember *pMember)
		{
			if (pMember) pMember->sendUserUnionData();
			return true;
		}
	};

	findall myList;
	execEveryOne(myList);
	this->update_all_data();
}

void CUnion::fireSeptFromUnion(const DWORD dwSeptID)
{
	struct findList : public execEntry<CUnionMember>
	{
		DWORD _dwSeptID;
		std::vector<DWORD> _removeList;
		
		findList(DWORD dwSeptID)
		{
			_dwSeptID = dwSeptID;
		}
		~findList(){}
		
		void remove(CUnion* pUnion)
		{
			for (std::vector<DWORD>::iterator vIter=_removeList.begin(); 
				vIter!=_removeList.end();vIter++)
			{
				 if (pUnion) pUnion->fireUnionMemberDirect(*vIter, false);
			}
		}
		
		bool exec(CUnionMember *pMember)
		{
			if (pMember->septid == _dwSeptID)
			{
				_removeList.push_back(pMember->id);
			}
			
			return true;
		}
	};

	findList myList(dwSeptID);
	execEveryOne(myList);
	myList.remove(this);
	writeDatabase();
	
	CSept* pSept = CSeptM::getMe().getSeptByID(dwSeptID);
	if (pSept)
	{
		pSept->clearUnion();
		pSept->writeDatabase();
	}

	if (this->isDel())
	{
		CUnionM::getMe().delUnion(this->id);
	}
}

DWORD CUnion::getActionPoint()
{
	return this->dwActionPoint;
}

bool CUnion::changeActionPoint(int repute)
{
	bool ret = false;
	
	if (repute<0)
	{
		if ((int)this->dwActionPoint>=abs(repute))
		{
			this->dwActionPoint = (int)this->dwActionPoint - abs(repute);
			ret = true;
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		this->dwActionPoint += repute;
		ret = true;
	}

	if (this->master) this->master->update_data();
	this->writeDatabase();
	this->sendUnionInfoToAll();

	return ret;
}

DWORD CUnion::getMoney()
{
	return this->dwMoney;
}

bool CUnion::changeMoney(int money)
{
	bool ret = false;
	
	if (money<0)
	{
		if ((int)this->dwMoney>=abs(money))
		{
			this->dwMoney = (int)this->dwMoney - abs(money);
			ret = true;
		}
		else
		{
			ret = false;
		}
		if (this->master) this->master->update_data();
		this->writeDatabase();
		this->sendUnionInfoToAll();
	}
	else
	{
		this->dwMoney += money;
		this->writeDatabase();
		this->sendUnionInfoToAll();
		return true;
	}

	return ret;
}

void CUnion::sendUnionManaToAll()
{
	FunctionTime func_time(0,__PRETTY_FUNCTION__,"�ı�һ�������������ʱ��" , 32);
	
	struct findList : public execEntry<CUnionMember>
	{
		findList()
		{
		}
		~findList(){}
		bool exec(CUnionMember *pMember)
		{
			if (pMember)
			{
				UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pMember->id);
				if (pMember->myUnion && pUser)
				{
					pMember->myUnion->sendUnionInfoToUser(pUser); // ���Ͱ�����Ϣ����ǰ��Ա
				}
			}
			return true;
		}
	};

	findList myList;
	execEveryOne(myList);
}

void CUnion::sendUnionInfoToAll()
{
	struct findList : public execEntry<CUnionMember>
	{
		findList()
		{
		}
		~findList(){}
		bool exec(CUnionMember *pMember)
		{
			if (pMember)
			{
				pMember->sendUserUnionData();
				pMember->update_data();
			}
			return true;
		}
	};

	findList myList;
	execEveryOne(myList);
}

//-------------------------------------------------------------------------------------
/**
* \brief ���������캯��
* \author fqnewman
*/
CUnionM::CUnionM()
{
}

/**
* \brief ��������������
* \author fqnewman
*/
CUnionM::~CUnionM()
{
	rwlock.wrlock();
	for(zEntryName::hashmap::iterator it=zEntryName::ets.begin();it!=zEntryName::ets.end();it++)
	{
		CUnion *temp =(CUnion *)it->second;
		SAFE_DELETE(temp);
	}
	clear();
	rwlock.unlock();
}

/**
* \brief �����ͷŹ�����
* \author fqnewman
*/
void CUnionM::destroyMe()
{
	delMe();
}

/**
* \brief �ڳ�Ա����������ҳ�Ա����
* return ����ָ��
*/
std::map<std::string, CUnionMember *>::iterator  CUnionM::findMemberIndex(const char *pName)
{
        char temp_name[MAX_NAMESIZE];
        bzero(temp_name, MAX_NAMESIZE);
        strncpy(temp_name, pName, MAX_NAMESIZE);
        return memberIndex.find(temp_name);
}

/**
* \brief �������ƻ�ȡ������
* \param pName ��������
* \author fqnewman
* \return �ɹ����ذ����� ʧ�ܷ���NULL
*/
CUnion* CUnionM::getUnionByName(const char* pName)
{
	CUnion *ret = NULL;
	rwlock.rdlock();
	ret =(CUnion *)getEntryByName(pName);
	rwlock.unlock();
	return ret;
}

/**
* \brief ����ID��ȡ������
* \param dwUnionID ����ID
* \author fqnewman
* \return �ɹ����ذ�����ʧ�ܷ��� NULL
*/
CUnion* CUnionM::getUnionByID(DWORD dwUnionID)
{
	CUnion* ret = NULL;
	rwlock.rdlock();
	ret = (CUnion*)getEntryByID(dwUnionID);
	rwlock.unlock();
	return ret;
}

/**
* \brief ����ʵ������ɾ������ʵ��
* \param name ʵ�������
* \author fqnewman
*/
void CUnionM::removeEntryByName(const char * name)
{
	zEntry *rm=getEntryByName(name);
	removeEntry(rm);
}

/**
* \brief ��ʼ�����������������ݿ��м������еİ������
* \author fqnewman
* \return true ���سɹ�  false ����ʧ��
*/
bool CUnionM::init()
{
	FunctionTime func_time(0,__PRETTY_FUNCTION__,"�������а������ʱ��" , 32);
	
	/*
	   Cmd::Record::t_GetUnion_UnionRecord tCmd;
	   return RecordClientManager::getInstance().get().sendCmd(&tCmd, sizeof(tCmd));
	 */
	const dbCol union_read_define[] = {
		{ "UNIONID",		zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "NAME",			zDBConnPool::DB_STR,	sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "CHARID",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "MASTER",			zDBConnPool::DB_STR,	sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "`VOTE`",			zDBConnPool::DB_BYTE,	sizeof(BYTE) }, 
		{ "`CREATETIME`",		zDBConnPool::DB_DWORD,	sizeof(DWORD) },
		{ "LEVEL",			zDBConnPool::DB_WORD,	sizeof(WORD) }, 
		{ "EXP",			zDBConnPool::DB_QWORD,	sizeof(QWORD) },
		{ "`MANA`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`COUNTRYID`",		zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`ACTIONPOINT`",		zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`MONEY`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`NOTE`",			zDBConnPool::DB_STR,	sizeof(char[254+1]) },
		{ "`CALLTIMES`",	zDBConnPool::DB_DWORD,  sizeof(DWORD) },
		{ NULL, 0, 0}
	};

	stUnionInfo *unionList,*tempPoint;
	stUnionInfo info;

	unionList = NULL;
	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܻ�ȡ���ݿ���");
		return false;
	}

	unsigned int retcode = SessionService::dbConnPool->exeSelect(handle, "`UNION`", union_read_define, NULL, NULL, (BYTE **)&unionList);
	SessionService::dbConnPool->putHandle(handle);
	if ((unsigned int)-1 == retcode
			|| 0 == retcode)
	{
		return true;
	}

	if (unionList)
	{
		tempPoint = &unionList[0];
		for (unsigned int i=0; i< retcode; i++)
		{
			bcopy(tempPoint, &info, sizeof(stUnionInfo));
			CUnion *pUnion = createUnionByDBRecord(info);
			pUnion->loadUnionMemberFromDB();
			//if ((CREATE_UNION_NEED_MAN_NUM <= pUnion->size()) && (pUnion->isVote())) 
			//{
				//pUnion->letVoteOver();
			//}
			tempPoint++;
		}
		SAFE_DELETE_VEC(unionList);
		return true;
	}
	else
	{
		Zebra::logger->error("������ݳ�ʼ��ʧ�ܣ�exeSelect ������Чbufָ��");
	}
	return false;
}

/**
* \brief ��ʼ����������Ӱ�����Ա ��createNewUnion()���ô˷�����
* \param info �������ݽṹ
* \author fqnewman
* \return ��������ʼ���õİ�����
*/
CUnion* CUnionM::createUnionAndAddMaster(const stUnionInfo & info)
{
	CUnion *pUnion = NULL;

	pUnion = new CUnion();
	pUnion->init(info);
	pUnion->master = pUnion->addUnionMaster(info);
	pUnion->master->insertDatabase();

	rwlock.wrlock();
	addEntry(pUnion);
	rwlock.unlock();

	if (pUnion->master->septid>0)
	{
		this->addNewSeptToUnion(pUnion->id, pUnion->master->septid);
	}

	pUnion->master->online(Cmd::UNION_MEMBER_STATUS_NEWMEMBER);

	if (pUnion->master)
	{   
		pUnion->master->sendUserUnionData();
		UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pUnion->master->id);

		if (pUser)
		{
			pUser->unionid = pUnion->id;
		}

		pUnion->master->update_data();
	}

	Zebra::logger->trace("[���]:%s ��Ὠ���ɹ�", pUnion->name);
	SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, pUnion->dwCountryID, 
			"��ϲ %s ������", pUnion->name);
	return pUnion;
}

/**
* \brief �������ݿ��¼����������ϵͳ���ص�ʱ��ʹ��
* \param info �������ݽṹ
* \author fqnewman
* \return �����ɹ��İ�����ָ��
*/
CUnion* CUnionM::createUnionByDBRecord(const stUnionInfo & info)
{
	CUnion *pUnion = NULL;
	bool ret = false;

	pUnion = new CUnion();
	pUnion->init(info);
	rwlock.wrlock();
	ret = addEntry(pUnion);
	rwlock.unlock();
	
	if (!ret)
	{
		Zebra::logger->trace("[���]: %s(%u) �����ӽ�CUnionM������ʧ��", pUnion->name, pUnion->id);
	}

	return pUnion;
}

/**
* \brief ����ָ���İ���Ա
* \param master ��ʹ����Ȩ�İ���Ա��һ���ǻ᳤
* \param member �������ĳ�Ա
* \author fqnewman
* \return 
*/
void CUnionM::fireUnionMember(UserSession * master, const char * member)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember=NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(master->name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	
	if (pUnionMember) 
	{
		CUnion *pUnion = pUnionMember->myUnion;
		
		if (pUnion)
		{
			pUnion->fireUnionMember(master->name, member);
		}
	}
	else
	{
		master->sendSysChat(Cmd::INFO_TYPE_FAIL, "��û�м�����");
	}
}

/**
  * \brief ɾ������Ա
  *
  * ����ý�ɫ���ǰ��᳤�����߳���������ǣ���������������
  *
  * \param dwUserID ��ɫID
  *
  * \return ����ý�ɫ�����κΰ���У��򷵻�2
  *         ����ý�ɫ�ǰ������򷵻�0
  *         ����ý�ɫ�ǰ��ڣ���ɾ���ɹ����򷵻�1
  *         ����ǰ���������ɾ��������3
  *         ����ý�ɫ�ǰ��ڣ���ɾ��ʧ�ܣ��򷵻�-1
  *
  */
int  CUnionM::fireUnionMember(DWORD dwUserID, bool find)
{
    struct findList : public execEntry<CUnion>
	{
		DWORD _dwUserID;
		int _status;
		CUnion* _pUnion;

		findList(DWORD dwUserID)
		{
			_dwUserID = dwUserID;
			_status = 2;
			_pUnion = NULL;
		}
		~findList(){}

		void removeUnionMember()
		{
			if (_status == 1)
			{
#ifdef _ZJW_DEBUG
				Zebra::logger->debug("%d �ǰ��ڣ����������ϵ", _dwUserID);
#endif
				if (_pUnion)
				{
					_pUnion->fireUnionMemberLeave(_dwUserID);
				}
				else
				{
					_status = -1;
				}
			}

			if (_status == 3)
			{
				
#ifdef _ZJW_DEBUG
						Zebra::logger->debug("%d �ǰ�����������", _dwUserID);
#endif
						if (_pUnion)
						{
							CUnionM::getMe().delUnion(_pUnion->id);
						}
						
				//_status = -1;
			}
		}

		bool exec(CUnion *pUnion)
		{
			if (pUnion)
			{
				if (pUnion->master)
				{
					if (pUnion->master->id == _dwUserID)
					{
#ifdef _ZJW_DEBUG
						Zebra::logger->debug("%d�ǰ�������ɢ���", _dwUserID);
#endif
						_pUnion = pUnion;
						_status = 3;
						return false;
					}
					else
					{
							if (pUnion->isMember(_dwUserID))
							{
#ifdef _ZJW_DEBUG
								Zebra::logger->debug("%d �� %s ���ڣ��ܽ������ϵ", _dwUserID, pUnion->name);
#endif
								_pUnion = pUnion;
								_status = 1;
								return false;
							}
							else
							{
#ifdef _ZJW_DEBUG
								Zebra::logger->debug("%d ���� %s �İ���", _dwUserID, pUnion->name);
#endif
								_status = 2;
							}
					}
				}
				else
				{
					Zebra::logger->error("%s û�а�����Ϣ����������Ϣ�������ԡ�", pUnion->name);
				}
			}

			return true;
		}
	};

	findList myList(dwUserID);
	execEveryOne(myList);

	if (!find)
	{
		myList.removeUnionMember();
	}

	return myList._status;
}

/**
* \brief �����³�Ա�����
* \param dwUnionID ���ID
* \param info �³�Ա����Ϣ�ṹ
* \author fqnewman
* \return true �ɹ� false ʧ��
*/
bool CUnionM::addNewMemberToUnion(const DWORD dwUnionID, const stUnionMemberInfo& info)
{
	CUnion * pUnion = NULL;

	rwlock.rdlock();
	pUnion = (CUnion *)getEntryByID(dwUnionID);
	rwlock.unlock();
	if (pUnion && pUnion->tempid != info.dwCharID)
	{
		CUnionMember *pMember = pUnion->addUnionMember(info);
		if (pMember)
		{
			pMember->insertDatabase();
			pMember->online(Cmd::UNION_MEMBER_STATUS_NEWMEMBER);
			UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pMember->id);
			if (pUser)
			{
				pUser->unionid = pMember->myUnion->id;
			}

			pMember->sendUserUnionData(); 
			pMember->update_data();

			if (pUnion) pUnion->sendUnionNotify("��ӭ %s ������", pMember->name);

			return true;
		}
		else
		{
			Zebra::logger->error("�޷�����Ա%u������%u��",info.dwCharID, dwUnionID);
		}
	}
	else 
	{
		Zebra::logger->error("���ݲ���������������û��%u��ᣬ����Ա%u�������ð��, �������Ѿ��ǰ���,�����ټ���",dwUnionID,info.dwCharID);
	}
	return false;
}

/**
* \brief �û����ߴ�����������û���ĳ�����ĳ�Ա������Ӧ�����߳�ʼ��
* \param pUser �����û�
* \author fqnewman
*/
void CUnionM::userOnline(UserSession * pUser)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;
	
	CUnion* pUnion = CUnionM::getMe().getUnionByID(pUser->unionid);
	if (pUnion && pUnion->dwCountryID>1 && (pUnion->dwCountryID != pUser->country))
	{
		CUnionM::getMe().fireUnionMember(pUser->id, false);
		return;
	}

	rwlock.rdlock();
	sIterator = findMemberIndex(pUser->name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();

	if (pUnionMember)
	{
		//CDareM::getMe().userOnline(pUser);

		if (!pUnionMember->isOnline())
		{
			pUnionMember->online();
			if(pUnionMember->myUnion->isVote()) return;

			pUnionMember->sendUserUnionData();
		}

		pUnionMember->update_data();
		//if ((0 == pUser->unionid) && pUnionMember->user)
		if (pUser)
		{
			pUser->unionid = pUnionMember->myUnion->id;
		}
	}
	else
	{
		if (pUser->unionid!=0)
		{
			Cmd::Session::t_fireUnionMember_SceneSession send;
			send.dwCharID = pUser->id;
			send.dwMapTempID = pUser->scene->tempid;
			pUser->scene->sendCmd(&send,sizeof(Cmd::Session::t_fireUnionMember_SceneSession)); /// ֪ͨ����������
			pUser->unionid = 0;
		}
	}
}

/**
* \brief ��Ա���ߴ���,�����ж�ָ�����û��Ƿ��ǰ���Ա�����������ߴ���
* \param pUser �����û�
* \author fqnewman
*/
void CUnionM::userOffline(const UserSession * pUser)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(pUser->name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)		pUnionMember->offline();
}

/**
* \brief �����µİ�����
* \param data ��ᴴ����Ϣ
* \author fqnewman
*/
void CUnionM::createNewUnion(Cmd::Session::t_addUnion_SceneSession *data)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(data->info.masterName);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{
		UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pUnionMember->id);
		if (pUser) pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�㲻���ٴ�������ˣ�");
		return;
	}

	zRTime ctv;
	data->info.dwCrTime = ctv.sec();
	if (createUnionDBRecord(data->info))  /// ������ݿ��¼��������
	{
		/*CUnion *pUnion = */createUnionAndAddMaster(data->info);  /// ��ʼ�����������еİ�����

		data->byRetcode =1;            /// ��������ʱ��0 ��ʾ����ʧ�������ظ��� 1��ʾ�ɹ�
		SceneSession * pScene = SceneSessionManager::getInstance()->getSceneByTempID(data->dwMapTempID);
		if (pScene)
		{
			pScene->sendCmd(data,sizeof(Cmd::Session::t_addUnion_SceneSession)); /// ֪ͨ����������
		}
		//because vote
		//if (pUnion) pUnion->sendUserUnionData(data->info.masterName);
	}
	else
	{
		data->byRetcode =0;            /// ��������ʱ��0 ��ʾ����ʧ�������ظ��� 1��ʾ�ɹ�
		SceneSession * pScene = SceneSessionManager::getInstance()->getSceneByTempID(data->dwMapTempID);
		if (pScene)
		{
			pScene->sendCmd(data,sizeof(Cmd::Session::t_addUnion_SceneSession)); /// ֪ͨ����������
		}
	}
}

/**
* \brief �����µİ�����ݿ��¼
* \param info �µİ��ṹ��Ϣ
* \author fqnewman
* \return true �ɹ�  false ʧ��
*/
bool CUnionM::createUnionDBRecord(stUnionInfo& info)
{
	static const dbCol createunion_define[] = {
		{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "`CHARID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
		{ "`MASTER`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "`VOTE`",	zDBConnPool::DB_BYTE,	sizeof(BYTE) }, 
		{ "`CREATETIME`",zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`LEVEL`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
		{ "`EXP`", zDBConnPool::DB_QWORD, sizeof(QWORD) },
		{ "`MANA`",	zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`COUNTRYID`",zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`ACTIONPOINT`",zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`MONEY`",zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
		{ "`NOTE`",	zDBConnPool::DB_STR,	sizeof(char[254+1]) },
		{ NULL, 0, 0}
	};
	struct {
		char name[MAX_NAMESIZE+1];
		DWORD charid;
		char master[MAX_NAMESIZE+1];
		BYTE vote;
		DWORD dwCrTime;
		WORD level;
		QWORD EXP;
		DWORD dwMana;
		DWORD dwCountryID;
		DWORD dwActionPoint;
		DWORD dwMoney;
		char note[254+1];
	} __attribute__ ((packed))
	createunion_data;
	static const dbCol verifyname_define[] = {
		{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
		{ NULL, 0, 0}
	};
	static const dbCol verifymasterid_define[] = {
		{ "`CHARID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
		{ NULL, 0, 0}
	};

	char strName[MAX_NAMESIZE+1];
	DWORD dwMasterID;
	char where[128];
	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("���ܻ�ȡ���ݿ���");
		return false;
	}
	//������֤�����Ƿ��ظ�
	std::string escapeName;
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "NAME = '%s'", SessionService::dbConnPool->escapeString(handle,info.name,escapeName).c_str());
	unsigned int retcode = SessionService::dbConnPool->exeSelectLimit(handle, "`UNION`", verifyname_define, where, NULL, 1, (BYTE *)(strName));
	if (retcode == 1)
	{
		SessionService::dbConnPool->putHandle(handle);
		return false;
	}

	//��֤�Ƿ����а�����¼,��ֹ��¼�ظ�
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "CHARID = %d", info.dwCharID);
	retcode = SessionService::dbConnPool->exeSelectLimit(handle, "`UNION`", verifymasterid_define, where, NULL, 1, (BYTE *)&dwMasterID);

	if (retcode == 1)
	{
		SessionService::dbConnPool->putHandle(handle);
		return false;
	}

	UserSession *pUser = NULL;
	pUser = UserSessionManager::getInstance()->getUserByID(info.dwCharID);

	//�������ݿ��ɫ��Ϣ
	bzero(&createunion_data, sizeof(createunion_data));
	if (pUser) 
	{
		createunion_data.dwCountryID = pUser->country;
	}
	else
	{
		createunion_data.dwCountryID = 0;
	}
	
	createunion_data.dwActionPoint = info.dwActionPoint;
	createunion_data.dwMoney = info.dwMoney;
	createunion_data.dwMana = info.dwMana;
	strncpy(createunion_data.name, info.name, MAX_NAMESIZE);
	strncpy(createunion_data.master, info.masterName, MAX_NAMESIZE);
	createunion_data.vote = info.byVote;
	createunion_data.dwCrTime = info.dwCrTime;
	createunion_data.charid = info.dwCharID;
	createunion_data.level = info.wdLevel;
	createunion_data.EXP = info.qwExp;
	strncpy(createunion_data.note, info.note, sizeof(createunion_data.note));
	retcode = SessionService::dbConnPool->exeInsert(handle, "`UNION`", createunion_define, (const unsigned char *)(&createunion_data));
	SessionService::dbConnPool->putHandle(handle);
	if ((unsigned int)-1 == retcode)
	{
		Zebra::logger->error("�������������ݿ���� %u, %s", info.dwCharID, info.name);
		return false;
	}

	info.dwUnionID = retcode;
	return true;
}

/**
* \brief ��ӳ�Ա����
*
* \param pName ���ӵĳ�Ա������
* \param pUnionMember ��Ա����
*
* \author fqnewman
* \return true ��ӳɹ� false ���ʧ��
*/
bool CUnionM::addMemberIndex(const char *pName, CUnionMember *pUnionMember)
{
	std::pair<std::map<std::string, CUnionMember*>::iterator, bool> retval;
	char temp_name[MAX_NAMESIZE];
	bzero(temp_name, MAX_NAMESIZE);
	strncpy(temp_name, pName, MAX_NAMESIZE);
	
	rwlock.wrlock();
	retval = memberIndex.insert(memberIndexValueType(temp_name, pUnionMember));
	rwlock.unlock();
	return retval.second;
}

/**
* \brief ɾ����Ա�������еļ�¼
* \param pName ��Ա����
* \author fqnewman
* \return true ɾ���ɹ� false ɾ��ʧ��
*/
bool CUnionM::removeMemberIndex(const char *pName)
{
	bool ret;
	std::map <std::string, CUnionMember*>::iterator sIterator;
	rwlock.wrlock();
	ret = true;
	sIterator = findMemberIndex(pName);
	if (sIterator != memberIndex.end()) memberIndex.erase(sIterator);
	else ret = false;
	rwlock.unlock();
	return ret;
}

/**
* \brief ��ɢ���
* \param dwUnionID ����ɢ�İ���ID
* \author fqnewman
*/
void CUnionM::delUnion(const DWORD dwUnionID)
{
	CUnion *pUnion = NULL;
	pUnion = (CUnion *)getEntryByID(dwUnionID);
	if (pUnion)
	{
		//CDare* pDare = CDareM::getMe().findDareRecordByRelation(Cmd::UNION_DARE, dwUnionID, 0);
		//CDare* pUnionCityDare = CDareM::getMe().findDareRecordByRelation(Cmd::UNION_CITY_DARE, dwUnionID, 0);
		
		//if (pDare == NULL && pUnionCityDare == NULL)
		//{
			pUnion->sendUnionNotify("%s ���ɢ", pUnion->name);
			Zebra::logger->trace("[���]:%s ���ɢ", pUnion->name);

			//SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, pUnion->dwCountryID, 
			//		"���ź� %s ����ɢ", pUnion->name);

			rwlock.wrlock();
			removeEntry(pUnion);
			rwlock.unlock();
			pUnion->disbandUnion();
			SAFE_DELETE(pUnion);
		//}
		//else
		//{
		//	UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pUnion->master->id);
		//	if (pUnion->master && pUser)
		//	{
		//		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "����ս״̬���������ɢ��ᣡ");
		//	}
			
		//}
	}
}

/**
* \brief �����Ա���
* \param dwUnionID ���ID
* \param dwCharID �뿪�ߵ�ID
* \author fqnewman,zjw
*/
void CUnionM::processMemberLeave(const DWORD dwUnionID, const DWORD dwCharID)
{
	CUnion *pUnion = NULL;
	pUnion = (CUnion *)getEntryByID(dwUnionID);
	if (pUnion)
	{
		CUnionMember *pMember = NULL;
		pMember = pUnion->master;
		if (pMember)
		{
			if (pMember->id != dwCharID)
			{
				UserSession *pUser = UserSessionManager::getInstance()->getUserByID(dwCharID);

				if (pUser)
				{
					CSept* pSept = CSeptM::getMe().getSeptByID(pUser->septid);

					if (pSept && pSept->master && pSept->master->id == dwCharID)
					{       
						pUnion->fireSeptFromUnion(pUser->septid);
					}
					/*else
					{
						pUnion->fireUnionMemberDirect(dwCharID, false);
					}*/
				}
			}
			else
			{
				if (pUnion->isDel())	
				{
					delUnion(dwUnionID);
				}
				else
				{
					UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pMember->id);
					if (pUser)
					{
						pUser->sendSysChat(Cmd::INFO_TYPE_MSG, 
								"������Ѱ���֮λת�ø������峤�������뿪");
					}
				}
			}
		}
		else
		{
			Zebra::logger->error("���%sû����ȷ�Ļ᳤�����������ݵ�������", pUnion->name);
		}
	}
}

void CUnionM::processCancelCityCatcherMessage(UserSession* pUser, const Cmd::stCancelCityCatcherCmd* ptCmd)
{
	CUnion* pUnion = this->getUnionByID(pUser->unionid);
	
	if (pUnion && pUnion->tempid == pUser->id)
	{
		CCity* pCity = CCityM::getMe().findByUnionID(pUser->unionid);
		if (pCity)
		{
			pCity->cancelCatcher();
			pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "ȡ����ͷ�ɹ�");
		}
	}
}

void CUnionM::processAppointCityCatcherMessage(UserSession* pUser, const Cmd::stAppointCityCatcherCmd* ptCmd)
{
	CUnion* pUnion = this->getUnionByID(pUser->unionid);
	
	if (pUnion && pUnion->tempid == pUser->id)
	{
		CCity* pCity = CCityM::getMe().findByUnionID(pUser->unionid);
		if (pCity)
		{
			UserSession *pCatcher = UserSessionManager::getInstance()->getUserSessionByName(ptCmd->name);
			if (pCatcher && pCatcher->scene)
			{
				if (pCatcher->id == pUser->id)
				{
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���������Լ�Ϊ��ͷ");
					return;
				}
				
				if (pCatcher->country != pUser->country)
				{
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�������������Ϊ��ͷ");
					return;
				}

				if (CCityM::getMe().isCastellan(pCatcher) || CCountryM::getMe().isOfficial(pCatcher))
				{
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���������������о�ԱΪ��ͷ");
					return;
				}
				
				// �ж��Ƿ��ѱ�����Ϊ���Ҳ�ͷ���⽻��
				CCountry* pCountry = CCountryM::getMe().find(pUser->country);
				if (pCountry && (strncmp(pCountry->diplomatName, pCatcher->name, MAX_NAMESIZE) == 0 ||
							strncmp(pCountry->catcherName, pCatcher->name, MAX_NAMESIZE) == 0))
				{
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�Է����ǲ�ͷ���⽻�ٲ�������");
					return;
				}

				if (CCityM::getMe().isCatcher(pUser))
				{
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�Է����ǲ�ͷ��������");
					return;
				}

				Cmd::Session::t_checkUserCity_SceneSession send;
				send.dwCheckID = pUser->id;
				send.dwCheckedID = pCatcher->id;
				pCatcher->scene->sendCmd(&send, sizeof(send));
			}
			else
			{
				pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�Է�������,��������");
			}
		}
		else
		{
			pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�����ǳ���,����ʹ�øù���");
		}
	}
	else
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�����ǳ���,����ʹ�øù���");
	}
}

void CUnionM::processUnionSortMessage(UserSession* pUser, const Cmd::stReqUnionSort* ptCmd)
{
	struct findList : public execEntry<CUnion>
	{
		DWORD _dwCountry;
		DWORD _byType;
		std::vector<CUnionSort> _vUnion;
		
		findList(DWORD dwCountry, BYTE byType)
		{       
			_dwCountry = dwCountry;
			_byType = byType;
		}
		~findList(){}

		void sendUnionSort(UserSession* pUser)
		{
			BYTE buf[zSocket::MAX_DATASIZE];
			Cmd::stUnionSortInfo* tempPoint;
			
			Cmd::stRtnUnionSort* retCmd = (Cmd::stRtnUnionSort*)buf;
			constructInPlace(retCmd);
			
			std::stable_sort(_vUnion.begin(), _vUnion.end());
			
			tempPoint = (Cmd::stUnionSortInfo *)retCmd->data;
			retCmd->byType = _byType;

			for (DWORD i=0; i<_vUnion.size(); i++)
			{
				if (i<10)
				{
					tempPoint->dwMana = _vUnion[i].dwMana;
					strncpy(tempPoint->unionName, _vUnion[i].unionName, MAX_NAMESIZE);
					tempPoint->dwCountryID = _vUnion[i].dwCountryID;
					tempPoint++;
					retCmd->dwSize++;	
				}
				else{
					break;
				}
			}
			
			pUser->sendCmdToMe(retCmd, (retCmd->dwSize*sizeof(Cmd::stUnionSortInfo)+
						sizeof(Cmd::stRtnUnionSort)));
		}
			
		
		bool exec(CUnion *pUnion)
		{      
			if (pUnion && (_byType != Cmd::COUNTRY_UNION_SORT || pUnion->dwCountryID==_dwCountry))
			{
				CUnionSort temp;
				strncpy(temp.unionName, pUnion->name, MAX_NAMESIZE);

				temp.dwMana = pUnion->getMana();
				temp.dwOrder  = 0;
				temp.dwCountryID = pUnion->dwCountryID;
				_vUnion.push_back(temp);
			}
			
			return true;
		}
	};
	
	findList myList(pUser->country, ptCmd->byType);
	execEveryOne(myList);
	myList.sendUnionSort(pUser);
}

/**
* \brief ����Ȩ��
* \param pUser ���
* \param ptNullCmd Ȩ������
* \author zjw
*/
void CUnionM::processMemberPower(UserSession* pUser, const Cmd::stUnionMemberPower* ptCmd)
{
	Cmd::stUnionMemberPower send;

	CUnion* pUnion = this->getUnionByID(pUser->unionid);
	
	if (pUnion)
	{
		if (pUnion->dwCountryID>1 && pUser->country!=pUnion->dwCountryID)
		{
			pUnion->fireUnionMemberDirect(pUser->id, false);
			return;
		}
			
		if (pUnion->master->id != pUser->id)
		{
			pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "Ȩ�޲����ǰ�����ר��Ŷ��");
			return;
		}
	}
	else
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�ϴ�������ᶼ��û���أ�");
		return;
	}

	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;
	rwlock.rdlock();
	sIterator = findMemberIndex(ptCmd->name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	strncpy(send.name, ptCmd->name, sizeof(send.name));
			
	switch (ptCmd->byType)
	{
		case Cmd::QUESTION_UNION_MEMBER_POWER:
			{
				if (pUnionMember)
				{
					 send.byPower[0]=pUnionMember->byPower[0];
					 send.byPower[1]=pUnionMember->byPower[1];
					 send.byType = Cmd::RESPOND_UNION_MEMBER_POWER;

					 if (pUser) pUser->sendCmdToMe(&send, sizeof(send));
				}
			}
			break;
		case Cmd::SET_UNION_MEMBER_POWER:
			{
				if (pUnionMember)
				{
					//pUnionMember->byPower[0] = 0x00;
					//pUnionMember->byPower[1] = 0x00;
					if (pUnionMember->id == pUnionMember->myUnion->master->id)
					{//�԰����Լ��������Ȩ�޸�ֵ,���Ҫת�ð�������ĳһ������Ϊ����Ȩ�޼ȿ�
						return;
					}

					CSept* pSept = CSeptM::getMe().getSeptByID(pUnionMember->septid);
					if (pSept==NULL || (pSept && pSept->master && pSept->master->id != pUnionMember->id))
					{
						pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "��ֻ�ܶ��峤���в���");
					}

										
					if (isset_state(ptCmd->byPower, Cmd::MASTER) 
						&& pUnionMember->id != pUnionMember->myUnion->id)
					{//�ı����
						pUnion->changeMaster(pUnionMember);
					}
					else
					{
						pUnionMember->byPower[0] = 0x00;
						pUnionMember->byPower[1] = 0x00;

						// Ĭ�ϣ���Ҫ����GENERALȨ��
						set_state(pUnionMember->byPower, Cmd::GENERAL);

						/*if (isset_state(ptCmd->byPower, Cmd::SECOND))
						{//���ø�����Ȩ�޶�Ӧ��Ȩ��
							SETSECONDPOWER(pUnionMember->byPower);
						}*/

						/*if (isset_state(ptCmd->byPower, Cmd::ADD_MEMBER))
						{
							pUnionMember->sendMessageToMe("�������������˵�Ȩ��");
							set_state(pUnionMember->byPower, Cmd::ADD_MEMBER);
						}*/

						if (isset_state(ptCmd->byPower, Cmd::CHANGE_ALIAS))
						{
							pUnionMember->sendMessageToMe("�����������������˳ƺŵ�Ȩ��");
							set_state(pUnionMember->byPower, Cmd::CHANGE_ALIAS);
						}

						/*if (isset_state(ptCmd->byPower, Cmd::FIRE_MEMBER))
						{
							pUnionMember->sendMessageToMe("�������������˵�Ȩ��");
							set_state(pUnionMember->byPower, Cmd::FIRE_MEMBER);
						}*/

						if (isset_state(ptCmd->byPower, Cmd::NOTIFY))
						{
							pUnionMember->sendMessageToMe("�������������Ĺ����Ȩ��");
							set_state(pUnionMember->byPower, Cmd::NOTIFY);
						}
						
						if (isset_state(ptCmd->byPower, Cmd::DARE))
						{
							pUnionMember->sendMessageToMe("������������ս�ͽ�����ս��Ȩ��");
							set_state(pUnionMember->byPower, Cmd::DARE);
						}


						pUnionMember->writeDatabase();
						pUnionMember->sendUserUnionData();
					}
				}
			}
			break;
		default:
			{
				return;
			}
			break;
	}

	return;
}

/**
* \brief �ж�Ȩ��
* \param name ���
*

* \author zjw
*/
bool CUnionM::havePowerByName(const char* name, const int power)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{
		return pUnionMember->havePower(power);
	}

	return false;
}



/**
* \brief ���Ͱ��������Ϣ
* \param pUser ��Ϣ������
* \param pCmd ������Ϣ��
* \param cmdLen ��Ϣ����
* \author fqnewman
*/
void CUnionM::sendUnionChatMessages(const UserSession *pUser, const Cmd::stChannelChatUserCmd *pCmd, const unsigned int cmdLen)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(pCmd->pstrName);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{
		if(pUnionMember->myUnion)
		{
			pUnionMember->myUnion->sendCmdToAllMember(pCmd,cmdLen);
		}
	}
	else
	{
		if (pUser) pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�����ȼ��������ʹ�ð������");
	}
}

/**
* \brief ���Ͱ��֪ͨ
*
* \param unionID ���ID
* \param message ��Ϣ����
*
* \author zjw
*/
void CUnionM::sendUnionNotify(const DWORD unionID, const char* message, ...)
{
	CUnion *pUnion = NULL;
	char buf[1024+1];
	bzero(buf, sizeof(buf));
	
	if (message==NULL)
	{
		return;
	}
	
	pUnion = this->getUnionByID(unionID);
	getMessage(buf, 1024, message);
	
	Cmd::stChannelChatUserCmd send;
	send.dwType=Cmd::CHAT_TYPE_SYSTEM;
	send.dwSysInfoType = Cmd::INFO_TYPE_EXP;
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));

	sprintf((char*)send.pstrName, "���֪ͨ");
	sprintf((char*)send.pstrChat, "%s", buf);

	if (pUnion)
	{
		pUnion->sendCmdToAllMember(&send, sizeof(send));
	}
	else
	{
		Zebra::logger->error("���Ͱ��֪ͨʱ��δ�ҵ����:%d", unionID);
	}
}
/**
* \brief ���Ͱ��˽����Ϣ
* \param pUser ��Ϣ������
* \param pCmd ������Ϣ��
* \param cmdLen ��Ϣ����
* \author fqnewman
*/
void CUnionM::sendUnionPrivateChatMessages(const UserSession *pUser, const Cmd::stChannelChatUserCmd *pCmd, const unsigned int cmdLen)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(pCmd->pstrName);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::stChannelChatUserCmd *chatCmd;

		chatCmd = (Cmd::stChannelChatUserCmd *)buf;
		memcpy(chatCmd,pCmd,cmdLen);
		strncpy(chatCmd->pstrName, pUser->name,MAX_NAMESIZE);

		UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pUnionMember->id);
		if (pUser)
		{
			if (!pUnionMember->myUnion->isVote()) pUser->sendCmdToMe(chatCmd, cmdLen);
		}
		else
		{
			COfflineMessage::writeOfflineMessage(chatCmd->dwType, pUnionMember->id, chatCmd,cmdLen);
		}
	}
}

/**
* \brief ���� Gateway ת�������Ŀͻ�����Ϣ
* \param pUser ��Ϣ������
* \param ptNullCmd ��Ϣ����
* \param cmdLen ��Ϣ����
* \author fqnewman
* \return true ����ɹ� false ��Ϣ���ڴ���Χ֮��
*/
bool CUnionM::processUserMessage(UserSession *pUser,const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen)
{
	switch(ptNullCmd->byCmd)
	{
		case Cmd::UNION_USERCMD:
			{
				switch(ptNullCmd->byParam)
				{
					case CANCEL_CITY_CATCHER_PARA:
						{
							Cmd::stCancelCityCatcherCmd* rev = (Cmd::stCancelCityCatcherCmd*)ptNullCmd;
							this->processCancelCityCatcherMessage(pUser, rev);

							return true;
						}
						break;
					case APPOINT_CITY_CATCHER_PARA:
						{
							Cmd::stAppointCityCatcherCmd* rev = (Cmd::stAppointCityCatcherCmd*)ptNullCmd;
							this->processAppointCityCatcherMessage(pUser, rev);
							return true;
						}
						break;
					case Cmd::REQ_UNION_NAME_LIST_PARA:
						{
							//FunctionTimes times(108, __FUNCTION__);
							Cmd::stReqUnionNameLlistCmd* rev = (Cmd::stReqUnionNameLlistCmd*)ptNullCmd;
							char buffer[zSocket::MAX_DATASIZE];
							Cmd::stRtnUnionNameListCmd *send = (Cmd::stRtnUnionNameListCmd*)buffer;
							constructInPlace(send);
							int i=0;
							while(i < rev->num && i <= 200)
							{
								CUnion* pUnion = this->getUnionByID(rev->dwUnionID[i]);

								if (pUnion)
								{
#ifdef _ZJW_DEBUG
									Zebra::logger->debug("[�Ż�]: %s ���� %s �������", pUser->name, pUnion->name);
#endif								
									strncpy(send->list[send->num].name, pUnion->name, MAX_NAMESIZE);
									send->list[send->num].dwUnionID = pUnion->id;
									send->num ++;
								}
								i ++; 
							}
							if(send->num)
							{
								pUser->sendCmdToMe(send, sizeof(Cmd::stRtnUnionNameListCmd) + (send->num * sizeof(send->list[0])));
							}
							return true;
						}
						break;
					case Cmd::REQ_UNION_NAME_PARA:
						{
							Cmd::stReqUnionNameCmd* rev = (Cmd::stReqUnionNameCmd*)ptNullCmd;
							
							Cmd::stRtnUnionNameCmd send;
							CUnion* pUnion = this->getUnionByID(rev->dwUnionID);
							//FunctionTimes times(107, __FUNCTION__);
							
							if (pUnion)
							{
#ifdef _ZJW_DEBUG
								Zebra::logger->debug("[�Ż�]: %s ���� %s �������", pUser->name, pUnion->name);
#endif								
								strncpy(send.name, pUnion->name, MAX_NAMESIZE);
								send.dwUnionID = rev->dwUnionID;
								pUser->sendCmdToMe(&send, sizeof(send));
							}
							
							return true;
						}
						break;
					case Cmd::REQUEST_ABJURATION_CITY_PARA:
						{
							CCity* pCity = CCityM::getMe().findByUnionID(pUser->unionid);
							CUnion* pUnion = this->getUnionByID(pUser->unionid);
							
							if (pUnion && pUnion->tempid == pUser->id)
							{
								if (pCity)
								{
									pCity->abandonCity();
								}
							}
							else
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, 
										"�����ǰ���������ʹ�øù���");
							}

							return true;
						}
						break;
					case Cmd::REQ_UNION_MONEY_PARA:
						{
							CUnion* pUnion = this->getUnionByID(pUser->unionid);
							
							if (pUnion)
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_MSG, 
										"%s ��ᵱǰ�ʽ�Ϊ:%d ��", pUnion->name,
										pUnion->dwMoney);
							}

							return true;
						}
						break;
					case Cmd::REQ_UNION_SORT_PARA:
						{
							Cmd::stReqUnionSort* ptCmd = (Cmd::stReqUnionSort*)ptNullCmd;
							processUnionSortMessage(pUser, ptCmd);
							return true;
						}
						break;
					case Cmd::UNION_MEMBER_POWER_PARA:
						{
							Cmd::stUnionMemberPower* ptCmd = (Cmd::stUnionMemberPower*)ptNullCmd;
							processMemberPower(pUser, ptCmd);

							return true;
						}
						break;
					case Cmd::FIRE_MEMBER_FROM_UNION_PARA:
						{
							Cmd::stFireMemberFromUnionCmd *ptCmd=(Cmd::stFireMemberFromUnionCmd *)ptNullCmd;
							fireUnionMember(pUser,ptCmd->memberName);
							return true;
						}
						break;
					case Cmd::UNIONMEMBER_LEAVE_UNION_PARA:
						{
							processMemberLeave(pUser->unionid, pUser->id);
							return true;
						}
						break;
					case Cmd::LIST_UNION_PARA:
						{
							sendListToUser(pUser);
							return true;
						}
						break;
					case Cmd::REQUEST_JOIN_UNION_PARA:
						{
							Cmd::stRequestJoinUnionCmd *ptCmd=(Cmd::stRequestJoinUnionCmd *)ptNullCmd;
							switch(ptCmd->status)
							{
								case Cmd::REQUEST_JOIN_OK:
									{
										UserSession *master = UserSessionManager::getInstance()->getUserSessionByName(ptCmd->name);
										if (master)
										{
											strncpy(ptCmd->name, pUser->name, MAX_NAMESIZE);
											master->sendCmdToMe(ptCmd, sizeof(Cmd::stRequestJoinUnionCmd));
										}
										else
										{
											pUser->sendSysChat(Cmd::INFO_TYPE_MSG, "�᳤%sĿǰ������", ptCmd->name);
										}
									}
									break;
								case Cmd::REQUEST_JOIN_CANCEL:
								case Cmd::REQUEST_JOIN_TIMEOUT:
								default:
									{
										UserSession *tUser = UserSessionManager::getInstance()->getUserSessionByName(ptCmd->name);
										if (tUser)
										{
											strncpy(ptCmd->name, pUser->name, MAX_NAMESIZE);
											tUser->sendCmdToMe(ptCmd, sizeof(Cmd::stRequestJoinUnionCmd));
										}
									}
									break;
							}

							return true;
						}
						break;
					case Cmd::VOTELIST_UNION_PARA:
						{
							/*if (pUser->level<10)
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, 
								"��ȼ�����10�����ܲ���ͶƱ");
								return true;
							}*/

							sendVoteListToUser(pUser);
							return true;
						}
						break;
					case Cmd::VOTE_UNION_PARA:
						{
							if (pUser->level<10)
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "��ȼ�����10�����ܲ���ǩ��");
								return true;
							}

							Cmd::stVoteUnionCmd *ptCmd=(Cmd::stVoteUnionCmd *)ptNullCmd;
							if (ptCmd->bySuccess)
							{
								userVote(pUser, ptCmd->unionName);
							}
							else
							{
								userAboutVote(pUser, ptCmd->unionName);
							}
							return true;
						}
						break;
					case Cmd::NOTE_UNION_PARA:
						{
							Cmd::stNoteUnionCmd *ptCmd=(Cmd::stNoteUnionCmd *)ptNullCmd;
							setUnionNote(pUser, ptCmd);
							return true;
						}
						break;
					case Cmd::CHANGE_UNION_MEMBER_ALIASNAME_PARA:
						{
							Cmd::stChangeUnionMemberAliasName* ptCmd=
								(Cmd::stChangeUnionMemberAliasName*)ptNullCmd;

							change_aliasname(pUser, ptCmd);
							return true;
						}
						break;
					default:
						break;
				}
			}
			break;
		default:
			break;
	}
	return false;
}

/**
* \brief ����ӳ�����������Ϣ
* \param cmd ��Ϣ��
* \param cmdLen ��Ϣ����
* \author fqnewman
* \return true ����ɹ� false ��Ϣ���ڴ���Χ֮��
*/
bool CUnionM::processSceneUnionMessage(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen)
{
	switch(cmd->para)
	{
		case Cmd::Session::PARA_CHECK_USER_CITY:
			{
				Cmd::Session::t_checkUserCity_SceneSession* rev = 
					(Cmd::Session::t_checkUserCity_SceneSession*)cmd;
				UserSession* pCatcher  = UserSessionManager::getInstance()->getUserByID(rev->dwCheckedID);
				UserSession* u  = UserSessionManager::getInstance()->getUserByID(rev->dwCheckID);

				if (pCatcher && u) 
				{       
					CCity* pCity = CCityM::getMe().findByUnionID(u->unionid);

					if (pCity)
					{       
						pCity->changeCatcher(pCatcher);

						if (u)  
						{       
							u->sendSysChat(Cmd::INFO_TYPE_MSG, "�ɹ����� %s Ϊ��ͷ", pCatcher->name);
						}       
					}      
				} 
				return true;
			}
			break;
		case Cmd::Session::OP_UNION_ACTION_PARA:
			{
				Cmd::Session::t_OpUnionAction_SceneSession* ptCmd = 
					(Cmd::Session::t_OpUnionAction_SceneSession*)cmd;

				CUnion* pUnion = this->getUnionByID(ptCmd->dwUnionID);
				if (pUnion)
				{
					pUnion->changeActionPoint(ptCmd->dwAction);
				}
				return true;
			}
			break;
		case Cmd::Session::OP_UNION_MONEY_PARA:
			{
				Cmd::Session::t_OpUnionMoney_SceneSession* ptCmd = 
					(Cmd::Session::t_OpUnionMoney_SceneSession*)cmd;

				CUnion* pUnion = this->getUnionByID(ptCmd->dwUnionID);
				if (pUnion)
				{
					pUnion->changeMoney(ptCmd->dwMoney);
				}
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

/**
* \brief ����ӳ�����������Ϣ
* \param cmd ��Ϣ��
* \param cmdLen ��Ϣ����
* \author fqnewman
* \return true ����ɹ� false ��Ϣ���ڴ���Χ֮��
*/
bool CUnionM::processSceneMessage(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen)
{
	switch(cmd->para)
	{
		case Cmd::Session::PARA_QUESTION_UNION_CITY:
			{
				Cmd::Session::t_questionUnionCity_SceneSession* ptCmd = 
					(Cmd::Session::t_questionUnionCity_SceneSession*)cmd;

				UserSession* pUser = UserSessionManager::getInstance()->getUserByID(ptCmd->dwUserID);
				
				if (pUser)
				{
					CCity* pCity = CCityM::getMe().find(ptCmd->dwCountryID, ptCmd->dwCityID);

					if (pCity == NULL)
					{
						pUser->sendSysChat(Cmd::INFO_TYPE_MSG, "�ó���δ���κΰ��ռ��");
						return true;
					}

					CUnion* pUnion = this->getUnionByID(pCity->dwUnionID);

					if (ptCmd->byType == Cmd::QUESTION_CITY_HOLD)
					{
						if (pUser) 
						{
							if (pUnion)
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_MSG, "�������� %s ���", 
										pUnion->name);
							}
							else
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_MSG, "Ŀǰ���ǻ�δ���κΰ��ռ��");
							}
						}
					}
					else if (ptCmd->byType == Cmd::QUESTION_CITY_DARE)
					{
						if (pUser) 
						{
							if (pCity->dareSize()>0)
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_MSG, "������ս��ս�����Ϊ: %d ����",
										pCity->dareSize());
							}
							else
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_MSG, "������δ���κΰ��ǰ����ս");
							}
						}
					}
				}
				
				return true;
			}
			break;
		case Cmd::Session::PARA_ENTER_UNION_MANOR:
			{
				Cmd::Session::t_enterUnionManor_SceneSession* ptCmd = 
					(Cmd::Session::t_enterUnionManor_SceneSession*)cmd;
				
				Cmd::Session::t_returnEnterUnionManor_SceneSession send;
				UserSession* pUser = UserSessionManager::getInstance()->getUserByID(ptCmd->dwUserID);
				
				if (pUser)
				{
					CCity* pOwnCity = CCityM::getMe().findByUnionID(pUser->unionid);
					CCity* pCurCity = CCityM::getMe().find(ptCmd->dwCountryID, ptCmd->dwCityID);

					if (pOwnCity)
					{
						if (ptCmd->dwCountryID == pOwnCity->dwCountry
						&& ptCmd->dwCityID == pOwnCity->dwCityID)
						{// �ڰ����ռ����,�������Լ�������.
							send.dwUserID = pUser->id;
							send.dwCountryID = pOwnCity->dwCountry;
							send.dwAreaID = pOwnCity->dwCityID*10+1;

							if (pUser->scene) pUser->scene->sendCmd(&send, sizeof(send));
						}
						else
						{// ���ڰ����ռ����, ���ж��Ƿ�������ռ����ID��ͬ.
						 // ����������ڹ�ռ�˹������
							if (ptCmd->dwCityID == pOwnCity->dwCityID)
							{
								send.dwUserID = pUser->id;
								send.dwCountryID = pOwnCity->dwCountry;
								send.dwAreaID = pOwnCity->dwCityID*10+1;

								if (pUser->scene) pUser->scene->sendCmd(&send, sizeof(send));
							}
							else
							{//�Ȳ����Լ���ռ����,Ҳ�����Լ���ռ���������Ӧ�ĳ���
							// ���������������ռ���е�����,�жϸó����Ƿ��ڴ���ս
							// ֻ׼��ս�����İ���Ա����,��ʱδ�ж�
								if (pCurCity==NULL)
								{
									return true;
								}

								CDare* pDare = CDareM::getMe().
							findDareRecordByID(Cmd::UNION_CITY_DARE, pCurCity->dwUnionID);

								if (pDare
										&& pDare->secondID == pCurCity->dwUnionID
										&& pDare->state == CDare::DARE_ACTIVE)
								{//��ս�������
									send.dwUserID = pUser->id;
									send.dwCountryID = pCurCity->dwCountry;
									send.dwAreaID = pCurCity->dwCityID*10+1;

									if (pUser->scene) pUser->scene->sendCmd(&send, sizeof(send));
								}
								else
								{
									pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, 
									"�óǲ����ڹ���δ�����ս��");
								}
							}
						}
					}
					else
					{// �Լ�û�г�,Ҫ���뵱ǰ�ǵ�����.���жϵ�ǰ�ó��Ƿ��ڶ��ս�ڼ�
						// �Լ�û��,��Ѱ����û���Լ����μӵĶ��ս,�����,��ȡ���ط�,���ڳ���
						// Ȼ��,�ж��ط����ڳ���,�Ƿ�Ϊ��ǰ�Լ����ڳ���ID���,���,������

						CDare* pDare = CDareM::getMe().
							findDareRecordByID(Cmd::UNION_CITY_DARE, pUser->unionid);

						if (pDare)
						{
							CCity* pDefCity = CCityM::getMe().findByUnionID(pDare->secondID);

							if (pDefCity && pDare->isAtt(pUser->unionid) 
									&& pDefCity->dwCityID == ptCmd->dwCityID
									&& pDare->state == CDare::DARE_ACTIVE)
							{
								send.dwUserID = pUser->id;
								send.dwCountryID = pDefCity->dwCountry;
								send.dwAreaID = pDefCity->dwCityID*10+1;

								if (pUser->scene) pUser->scene->sendCmd(&send, sizeof(send));
							}
							else
							{
								pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, 
										"�óǲ����ڹ���δ�����ս��");
							}
						}
					}
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_UNION_ADDMEMBER:
			{
				Cmd::Session::t_addUnionMember_SceneSession *ptCmd=(Cmd::Session::t_addUnionMember_SceneSession *)cmd;
				std::map<std::string, CUnionMember *>::iterator sIterator;
				CUnionMember *pUnionMember = NULL;

				rwlock.rdlock();        
				sIterator = findMemberIndex(ptCmd->member.name);
				if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
				rwlock.unlock();
				
				CUnion * pUnion = NULL;

				rwlock.rdlock();
				pUnion = (CUnion *)getEntryByID(ptCmd->dwUnionID);
				rwlock.unlock();
				if (pUnion == NULL)
				{
					return true;
				}

				if (pUnionMember != NULL && pUnionMember->myUnion && pUnionMember->myUnion->isVote())
				{
					UserSession *pUser = UserSessionManager::getInstance()->getUserByID(pUnion->master->id);
					if (pUnion && pUnion->master && pUser)
					{

						pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�Է���ǩ������������");
					}

					pUser = UserSessionManager::getInstance()->getUserByID(pUnionMember->id);
					if (pUnionMember && pUser)
					{
						pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "����ǩ���������������ܼ�������İ��");
					}
				}
				else
				{
					if (!pUnionMember)
					{
						UserSession *pUser = UserSessionManager::getInstance()->
							getUserByID(pUnion->master->id);

						UserSession* pAddUser = UserSessionManager::getInstance()->
							getUserByID(ptCmd->member.dwCharID);

						if (pUnion && pUnion->master && pUser)	
						{
							pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "%s ������������������˰��", ptCmd->member.name);
						}

						if (pAddUser)
						{
							addNewSeptToUnion(ptCmd->dwUnionID, pAddUser->septid);
						}
					}
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_UNION_ADDUNION:
			{
				Cmd::Session::t_addUnion_SceneSession *ptCmd=(Cmd::Session::t_addUnion_SceneSession *)cmd;
				createNewUnion(ptCmd);
				return true;
			}
		default:
			break;
	}

	return false;
}

/**
* \brief �����Gateway��������Ϣ
* \param cmd ��Ϣ��
* \param cmdLen ��Ϣ����
* \author fqnewman
* \return true ����ɹ� false ��Ϣ���ڴ���Χ֮��
*/
bool CUnionM::processGateMessage(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen)
{
	switch(cmd->para)
	{
		case Cmd::Session::PARA_UNION_DISBAND:
			{
				Cmd::Session::t_disbandUnion_GateSession *ptCmd=(Cmd::Session::t_disbandUnion_GateSession *)cmd;
				processMemberLeave(ptCmd->dwUnionID, ptCmd->dwCharID);
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

/**
* \brief ���͵�ǰϵͳ���ڵȴ�ͶƱ�İ���б��ָ�����û�
* \param pUser ����ͶƱ�б���
* \author fqnewman
*/
void CUnionM::sendVoteListToUser(const UserSession *pUser)
{
	/*
	struct findList : public execEntry<CUnion>
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::stVoteListUnionCmd *retCmd;
		Cmd::stUnionVoteRecord *tempPoint;
		std::vector<DWORD> removed;
		UserSession *user;
		findList(char *pName,UserSession *pUser)
		{
			user = pUser;
			retCmd=(Cmd::stVoteListUnionCmd *)buf;
			constructInPlace(retCmd);
			tempPoint = (Cmd::stUnionVoteRecord *)retCmd->data;
			retCmd->dwSize = 0;
			retCmd->flag = 0;

			strncpy(retCmd->unionName,pName, MAX_NAMESIZE);
		}
		~findList(){}

		void sendList(const UserSession *pUser)
		{
			pUser->sendCmdToMe(retCmd,(retCmd->dwSize*sizeof(Cmd::stUnionVoteRecord)+sizeof(Cmd::stVoteListUnionCmd)));
		}

		void removeInvalidUnion()
		{
			std::vector<DWORD>::iterator tIterator;

			for(tIterator = removed.begin(); tIterator != removed.end(); tIterator++)
			{
				CUnionM::getMe().delUnion(*tIterator);
			}
		}

		bool exec(CUnion *punion)
		{
			if (punion && punion->isVote())
			{
				zRTime ctv;
				if (ctv.sec() - punion->dwCreateTime > DESTROYTIME)
				{
					removed.push_back(punion->id);
					return true;
				}
				

				if (retCmd->dwSize>100)
				{
					this->sendList(user);
					char tName[MAX_NAMESIZE];
					strncpy(tName, retCmd->unionName, MAX_NAMESIZE);
					tempPoint = (Cmd::stUnionVoteRecord *)retCmd->data;
					retCmd->dwSize = 0;
					retCmd->flag=1;
				}

				if (user && punion->dwCountryID == user->country)
				{// ֻ���ͱ�������ͶƱ�б�
					strncpy(tempPoint->unionName,punion->name,MAX_NAMESIZE);
					if (punion->master)
						strncpy(tempPoint->master, punion->master->name,MAX_NAMESIZE);
					else
						strncpy(tempPoint->master, "δ֪",MAX_NAMESIZE);
					tempPoint->wdVoteNumber = punion->size();
					strncpy(tempPoint->note, punion->note, 255));
					tempPoint++;
					retCmd->dwSize++;
				}
			}
			return true;
		}
	};
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;
	char tempName[MAX_NAMESIZE];

	rwlock.rdlock();
	sIterator = findMemberIndex(pUser->name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();

	bzero(tempName, MAX_NAMESIZE);
	if (pUnionMember)
	{
		if (pUnionMember->myUnion->isVote())
		{
			strncpy(tempName, pUnionMember->myUnion->name, MAX_NAMESIZE);
		}
	}

	findList myList(tempName, (UserSession *)pUser);
	execEveryOne(myList);
	myList.sendList(pUser);
	myList.removeInvalidUnion();
	*/
}

/**
* \brief ���͵�ǰϵͳ���ڵȴ�ͶƱ�İ���б��ָ�����û�
* \param pUser ����ͶƱ�б���
* \author fqnewman
*/
void CUnionM::sendListToUser(const UserSession *pUser)
{
	struct findList : public execEntry<CUnion>
	{
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::stListUnionCmd *retCmd;
		Cmd::stUnionRecord *tempPoint;
		UserSession *user;
		findList(UserSession *pUser)
		{
			user = pUser;
			retCmd=(Cmd::stListUnionCmd *)buf;
			constructInPlace(retCmd);
			tempPoint = (Cmd::stUnionRecord *)retCmd->data;
			retCmd->dwSize = 0;
			retCmd->flag = 0;
		}
		~findList(){}

		void sendList(const UserSession *pUser)
		{
			pUser->sendCmdToMe(retCmd,(retCmd->dwSize*sizeof(Cmd::stUnionRecord)+sizeof(Cmd::stListUnionCmd)));
		}

		bool exec(CUnion *punion)
		{
			if (punion)
			{
				if (retCmd->dwSize>100)
				{
					this->sendList(user);
					tempPoint = (Cmd::stUnionRecord *)retCmd->data;
					retCmd->dwSize = 0;
					retCmd->flag=1;
				}

				if (user && punion->dwCountryID == user->country)
				{// ֻ���ͱ�������ͶƱ�б�
					strncpy(tempPoint->unionName,punion->name,MAX_NAMESIZE);
					if (punion->master)
						strncpy(tempPoint->master, punion->master->name,MAX_NAMESIZE);
					else
						strncpy(tempPoint->master, "δ֪",MAX_NAMESIZE);
					//tempPoint->wdVoteNumber = punion->size();
					strncpy(tempPoint->note, punion->note, 255);
					tempPoint++;
					retCmd->dwSize++;
				}
			}
			return true;
		}
	};

	findList myList((UserSession *)pUser);
	execEveryOne(myList);
	myList.sendList(pUser);
}

/**
* \brief ���͵�ǰϵͳ�п��Ա���ս�İ���б�
* \param pUser ����ͶƱ��
* \author zjw
*/
void CUnionM::sendDareListToUser(const UserSession *pUser)
{
	struct findList : public execEntry<CUnion>
	{
		BYTE buf[zSocket::MAX_DATASIZE];
  	        Cmd::stSendDareList *send;
		Cmd::stDareList* tempPoint;
		const UserSession* _pUser;
		
		findList(const UserSession* pUser)
		{
			send = (Cmd::stSendDareList *)buf;
			constructInPlace(send);
			tempPoint = (Cmd::stDareList *)send->dare_list;
			send->dwSize = 0;
			send->byType = Cmd::UNION_DARE;
			_pUser = pUser;
			
		}
		~findList(){}

		void sendList()
		{
			if (_pUser) _pUser->sendCmdToMe(send,(send->dwSize*sizeof(Cmd::stDareList)+sizeof(Cmd::stSendDareList)));
		}

		bool exec(CUnion *punion)
		{
			if (_pUser)
			{
				if (punion && punion->master)
				{
					UserSession *pUser = UserSessionManager::getInstance()->getUserByID(punion->master->id);
					if (pUser && pUser->country==_pUser->country 
							&& punion->id!=_pUser->unionid)
					{

						strncpy(tempPoint->name, punion->name, MAX_NAMESIZE);
						tempPoint++;
						send->dwSize++;
					}
				}
			}

			return true;
		}
	};


	findList myList(pUser);
	execEveryOne(myList);
	myList.sendList();
}

/**
* \brief �û�ͶƱ��ָ���İ��
* \param pUser ͶƱ��
* \param pName �������
* \author fqnewman
*/
void CUnionM::userVote(const UserSession *pUser, const char *pName)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(pUser->name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{

		if (pUser)
		{
			if (pUnionMember->myUnion->isVote())
			{
				if (pUnionMember == pUnionMember->myUnion->master)
				{
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "����%s���Ԥ���᳤������ǩ��", pUnionMember->myUnion->name);
				}
				else
				{
					if (strncmp(pName,pUnionMember->myUnion->name,MAX_NAMESIZE)!=0)
					{
						pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���Ѷ� %s ��ǩ������֮ǰ��%s���ǩ����������.",pName, pUnionMember->myUnion->name);
						processMemberLeave(pUnionMember->myUnion->id, pUser->id);
						addVoteMemberToUnion(pUser,pName);
						return;
					}
				}
			}
			else
			{
				pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���Ѿ�������������ˣ�û��ǩ�����ʸ�");
			}
		}
		else
		{
			Zebra::logger->error("CUnionM::userVote():һ�����ߵİ���Աû����Ч���û�ָ��");
		}
	}
	else
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���%s�������ǩ��", pName);
		addVoteMemberToUnion(pUser,pName);
		return;
	}

	// ֪ͨͶƱʧ��
	Cmd::stVoteUnionCmd send;
	strncpy(send.unionName,pName,MAX_NAMESIZE);
	send.bySuccess = 0;
	pUser->sendCmdToMe(&send, sizeof(send));
}

/**
* \brief �û�����ͶƱ
* \param pUser ����ͶƱ�ĳ�Ա
* \param pName �������������
* \author fqnewman
* \return 
*/
void CUnionM::userAboutVote(const UserSession *pUser, const char *pName)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(pUser->name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{

		if (pUser)
		{
			if (pUnionMember->myUnion->isVote())
			{
				if (pUnionMember == pUnionMember->myUnion->master)
				{
					
					pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "����%s���Ԥ������, ����ǩ��������ɢ", 
							pUnionMember->myUnion->name);
					
					this->processMemberLeave(pUnionMember->myUnion->id, pUser->id);
					sendVoteListToUser(pUser);
					return;
				}
				else
				{
					if (strncmp(pName,pUnionMember->myUnion->name,MAX_NAMESIZE)==0)
					{
						pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�������%s���ǩ��.",pName);
						processMemberLeave(pUnionMember->myUnion->id, pUser->id);

						Cmd::stVoteUnionCmd send;
						strncpy(send.unionName,pName,MAX_NAMESIZE);
						send.bySuccess = 2;
						pUser->sendCmdToMe(&send, sizeof(send));
						return;
					}
				}
			}
			else
			{
				Zebra::logger->error("[%s]����Ѿ�����������ǩ����ûȡ��",pName);
			}
		}
		else
		{
			Zebra::logger->error("CUnionM::userVote():һ�����ߵİ���Աû����Ч���û�ָ��");
		}
	}

	// ֪ͨͶƱʧ��
	Cmd::stVoteUnionCmd send;
	strncpy(send.unionName,pName,MAX_NAMESIZE);
	send.bySuccess = 0;
	pUser->sendCmdToMe(&send, sizeof(send));
}

/**
* \brief ͶƱ��ָ���İ��
* \param pUser ͶƱ��
* \param pName �������
* \author fqnewman
*/
void CUnionM::addVoteMemberToUnion(const UserSession *pUser, const char *pName)
{
	rwlock.rdlock();
	CUnion *myUnion = (CUnion *)getEntryByName(pName);
	rwlock.unlock();

	if (myUnion)
	{
		if (myUnion->isVote())
		{
			stUnionMemberInfo member;
			member.dwCharID = pUser->id;
			strncpy(member.name, pUser->name, MAX_NAMESIZE);
			member.wdOccupation = pUser->occupation;
			addNewMemberToUnion(myUnion->id, member);

			Cmd::stVoteUnionCmd send;
			strncpy(send.unionName,pName,MAX_NAMESIZE);
			send.bySuccess = 1;
			pUser->sendCmdToMe(&send, sizeof(send));

			//if (myUnion->size()>=CREATE_UNION_NEED_MAN_NUM) // 10
			//{
			//	myUnion->letVoteOver();
			//}
		}
		else
		{
			pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "%s���ǩ�����Ѿ���������ֱ��ȥ�һ᳤Э���й��������.", pName);
			Cmd::stVoteUnionCmd send;
			strncpy(send.unionName,pName,MAX_NAMESIZE);
			send.bySuccess = 0;
			pUser->sendCmdToMe(&send, sizeof(send));
		}
	}
}

/**
* \brief ���ð�����
* \param pUser �����ߵ� UserSession����
* \param pCmd ����������Ϣ
* \author fqnewman
*/
void CUnionM::setUnionNote(UserSession *pUser, Cmd::stNoteUnionCmd *pCmd)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(pUser->name);
	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{
		CUnion *myUnion = pUnionMember->myUnion;
		if (myUnion && ((myUnion->master == pUnionMember) || pUnionMember->havePower(Cmd::NOTIFY)))
		{
			myUnion->setNote(pCmd);
		}
		else
		{
			pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���޷����ð�����");
		}
	}
}

/**
 * \brief ���ı���
 *
 *
 * \param pUser ���������(Ŀǰֻ���ǰ���)
 * \param pName ���������ߵ�����(�����ǰ���Ա)
 * \param pAliasName ����
 * \return 
 */
void CUnionM::change_aliasname(UserSession *pUser, Cmd::stChangeUnionMemberAliasName* pCmd)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(pUser->name);

	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	
	if (pUnionMember)
	{
		CUnion *myUnion = pUnionMember->myUnion;
		if (myUnion && ((myUnion->master == pUnionMember) || (pUnionMember->havePower(Cmd::CHANGE_ALIAS))))
		{
			pUnionMember = NULL;
			sIterator = findMemberIndex(pCmd->name);
			if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
			
			if (pUnionMember && pUnionMember->myUnion == myUnion)
			{
				pUnionMember->change_aliasname(pCmd->aliasname);		
			}
		}
		else
		{
			pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "����Ȩ���ĳƺ�");
		}
	}
	else
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���������κΰ��");
	}
}

// ������а��
void CUnionM::delAllUnion(DWORD id)
{
	struct findList : public execEntry<CUnion>
	{
		std::vector<DWORD> vUnionID;
		DWORD dwUserID;
		
		findList(DWORD id)
		{
			dwUserID = id;
		}
		~findList(){}

		void removeUnion()
		{
			for (std::vector<DWORD>::iterator vIter = vUnionID.begin(); vIter!=vUnionID.end(); vIter++)
			{
				CUnionM::getMe().delUnion(*vIter);
			}
		}

		bool exec(CUnion *pUnion)
		{
			if (pUnion)
			{
				Cmd::Session::t_GetCreateUnionItem_SceneSession send;
				UserSession *pUser = UserSessionManager::getInstance()->getUserByID(dwUserID);
				
				if (pUser && pUser->scene)
				{
					send.dwUserID = pUnion->tempid;
					pUser->scene->sendCmd(&send, sizeof(send));
				}
				
				vUnionID.push_back(pUnion->id);
			}

			return true;
		}
	};

	findList myList(id);
	execEveryOne(myList);

	myList.removeUnion();
}

void CUnionM::addNewSeptToUnion(const DWORD dwUnionID, const DWORD dwSeptID)
{
	CUnion * pUnion;

	rwlock.rdlock();
	pUnion = (CUnion *)getEntryByID(dwUnionID);
	rwlock.unlock();
	
	struct findall : public execEntry<CSeptMember> 
	{                    
		DWORD _dwUnionID;

		findall(DWORD dwUnionID)
		{           
			_dwUnionID = dwUnionID;
		}

		~findall(){}

		bool exec(CSeptMember* pMember)
		{
			if (pMember)
			{
				UnionDef::stUnionMemberInfo info;

				info.dwCharID = pMember->id;
				strncpy(info.name, pMember->name, MAX_NAMESIZE);

				if (pMember->mySept && pMember->mySept->master 
						&& pMember->mySept->master->id == pMember->id)
				{
					strncpy(info.aliasname, "�峤", MAX_NAMESIZE);
				}
				else
				{
					strncpy(info.aliasname, DEFAULTMEMBERALIAS, MAX_NAMESIZE);
				}

				SETMEMBERPOWER_WD(info.wdPower);

				info.wdOccupation = 0;
				info.dwSeptID = pMember->mySept->id;
				CUnionM::getMe().addNewMemberToUnion(_dwUnionID, info);
			}
			
			return true;
		}
	};

	
	DWORD max_septsize = 30;
	DWORD cur_mana = pUnion->getMana();
	
	if (cur_mana>=10000 && cur_mana<30000)
	{
		max_septsize = 40;
	}
	else if (cur_mana>=30000 && cur_mana<80000)
	{
		max_septsize = 50;
	}
	else if (cur_mana>=80000)
	{
		max_septsize = 60;
	}

	if (pUnion)
	{
		if (pUnion->septSize()<max_septsize)
		{
			findall myList(dwUnionID);
			CSept* pSept = CSeptM::getMe().getSeptByID(dwSeptID);
			if (!pSept)
				return;

			if (!pSept->dwUnionID)
			{
				pSept->dwUnionID= pUnion->id;

				pSept->execEveryOne(myList);	
				pSept->writeDatabase();
				pUnion->sendUnionNotify("��ӭ %s ������뱾��", pSept->name);
				Zebra::logger->trace("[���]: %s ������� %s ���ɹ�", pSept->name, pUnion->name);
			}
			else
			{
				Zebra::logger->trace("[���]: ������ʧ�� %s �����Ѽ����������.", 
						pSept->name);
			}
		}
	}
}

/**
 * \brief ʱ��ص�����
 */
void CUnionM::timer()
{
	time_t timValue = time(NULL);
	struct tm tmValue;
	zRTime::getLocalTime(tmValue, timValue);
	struct findList : public execEntry<CUnion>
	{
		findList()
		{
		}
		~findList(){}

		bool exec(CUnion *pUnion)
		{
			if (pUnion)
			{
				pUnion->dwActionPoint = pUnion->getMana()/100;
				Zebra::logger->trace("[���]: %s ˢ���ж���:(%d,%d)", pUnion->name, pUnion->getMana(), 
						pUnion->dwActionPoint);
				pUnion->sendUnionInfoToAll();
				pUnion->calltimes=0;
				pUnion->writeDatabase();
				pUnion->refreshSeptExp();
				
			}

			return true;
		}
	};

	if (tmValue.tm_hour==0 && tmValue.tm_min==0 && tmValue.tm_sec==0)
	{
		findList myList;
		execEveryOne(myList);
	}
}

/**
 * \brief ���ݽ�ɫ���ֻ�ȡ��������������
 *
 *
 * \param Name ��ɫ����
 * \return ���ذ�����ƻ���NULL
 */
char * CUnionM::getUnionNameByUserName(char *Name)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(Name);

	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{
		if (pUnionMember->myUnion)
			return pUnionMember->myUnion->name;
	}
	return NULL;
}

/**
 * \brief ���ݽ�ɫ���ֻ�ȡ��������������
 *
 *
 * \param Name ��ɫ����
 * \return ��ɫ�����Ĺ��ұ��
 */
DWORD CUnionM::getCountryIDByUserName(char *Name)
{
	std::map<std::string, CUnionMember *>::iterator sIterator;
	CUnionMember *pUnionMember = NULL;

	rwlock.rdlock();
	sIterator = findMemberIndex(Name);

	if (sIterator != memberIndex.end()) pUnionMember = (*sIterator).second;
	rwlock.unlock();
	if (pUnionMember)
	{
		if (pUnionMember->myUnion)
			return pUnionMember->myUnion->dwCountryID;
	}
	return 0;
}
