/**
 * \file
 * \version  $Id: CRelationManager.h  $
 * \author  
 * \date 
 * \brief ������ѹ�ϵ������
 *
 */


#ifndef _CRELATION_MANAGER_H
#define _CRELATION_MANAGER_H

//#include "zRWLock.h"
#include "zMisc.h"
#include "zType.h"
#include "zEntry.h"
#include "zEntryManager.h"
#include "Command.h"
#include "SessionCommand.h"


class UserSession;

/// �����ʮСʱ����Ϊ��λ������
#define MAX_GROUP_TIME_GAP 60*60*60

/// �Ѻöȿ۳�������λΪ����
#define DEDUCT_POINT 5*60

struct CRelation : public zEntry
{
public:
	///	��ϵ���� 
	BYTE  type;

	///  ��ϵ����
	WORD  level;

	///	�û�Session����
//	UserSession *user;
	bool online;

	/// �û���ɫID
	DWORD charid;

	///  ������ʱ��
	DWORD lasttime;

	///  ְҵ
	WORD  occupation;

	CRelation();
	void sendNotifyToScene();
	bool isOnline();
private:
	/// ��д��
	//zRWLock rwlock;
};

class CRelationManager : public zEntryManager<zEntryID, zEntryName>
{
private:
	/// ��д��
	//zRWLock rwlock;

	/// �û�����
	UserSession *user;

public:
	~CRelationManager();
	template <class YourEntry>
	bool execEveryOne(execEntry<YourEntry> &exec)
	{
		//rwlock.rdlock();
		bool ret=execEveryEntry<>(exec);
		//rwlock.unlock();
		return ret;
	}


	void init();
	void loadFromDatabase();
	void deleteDBRecord(const DWORD dwID);
	bool insertDBRecord(const CRelation *);
	void updateDBRecord(const CRelation *relation);
	void updateOtherOfflineUserDBRecord(const CRelation *relation);
	void writeOfflineNotify(const CRelation *relation);
//	void writeDatabase();
	bool processUserMessage(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);
	void removeRelation(const char *);
	void addBadRelation(const char *name);
	void addEnemyRelation(const char *name);
	inline void sendStatusChange(const CRelation *, const BYTE byState);
	void addRelation(const DWORD dwID, const BYTE type);
	void changeRelationType(const char * name, const BYTE type);
	void sendRelationList();
	void online(const DWORD dwID);
	void offline(const DWORD dwID, const char* name);
	void setUser(UserSession *);
	void sendChatToMyFriend(const Cmd::stChannelChatUserCmd *, const unsigned int cmdLen);
	void sendCmdToMyFriend(const void *, const DWORD, bool sendMe=true);
	void sendCmdToMyFriendExcept(const void *, const DWORD, bool sendMe=true, const char * = "");
	void sendPrivateChatToFriend(const Cmd::stChannelChatUserCmd *, const unsigned int cmdLen);
	void sendBlackListToGateway(const char *, const BYTE oper);
	void sendAllBlackListToGateway();
	CRelation * getRelationByName(const char *);
	CRelation*  getRelationByID(DWORD dwRelationID);
	CRelation*  getMarryRelation();
	void setFriendDegree(Cmd::Session::t_CountFriendDegree_SceneSession *);
	void setFriendDegreeByOtherUser(const DWORD dwUserID, const WORD wdDegree, const DWORD currTime);
};

#endif

