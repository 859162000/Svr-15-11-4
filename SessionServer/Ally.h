/**
 * \file	Ally.h
 * \version  	$Id: Ally.h  $
 * \author  	
 * \date 	
 * \brief 	ͬ�˹�����������
 *
 * 
 */
#ifndef _ALLY_H
#define _ALLY_H

#include <vector>
#include <map>
#include "zType.h"
#include "zRWLock.h"
#include "SessionCommand.h"
#include "zSingleton.h"

class Record;
class SessionTask;
const int CREATE_ALLY_NEED_MONEY = 200000; // 20��

class CAlly
{
	public:
		CAlly();
		~CAlly();

		void init(Record* rec);
		void writeDatabase();
		bool insertDatabase();
		bool deleteMeFromDB();

		bool changeFriendDegree(int degree);
		void refreshAlly(bool isFire = false);
		void refreshAllyToAllUser(bool isFire = false);

		DWORD friendDegree()
		{
			return this->dwFriendDegree;
		}

		DWORD dwCountryID; // ID1
		DWORD dwAllyCountryID; // ID2
		DWORD dwFriendDegree;
		DWORD dwCreateTime;
		DWORD dwLastUpTime; // ÿ���Զ��ۼ��Ѻöȵ����һ�θ���ʱ��

		BYTE  byStatus; // 2�ѽ���,1�ȴ�����

		zRWLock rwlock;
};

class CAllyM : public Singleton<CAllyM>
{
	friend class SingletonFactory<CAllyM>;
	public:
		bool init();
		static void destroyMe();

		void timer();
		void save();

		bool loadAllyFromDB();
		bool processUserMessage(UserSession* pUser, const Cmd::stNullUserCmd* ptNullCmd, const unsigned int cmdLen);
                bool processSceneMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);

		void processReqCountryAlly(UserSession* pUser, Cmd::stReqCountryAlly* rev);
		void processCancelCountryAlly(UserSession* pUser, Cmd::stCancelCountryAlly* rev);
		void processReqCountryAllyInfo(UserSession* pUser, Cmd::stReqCountryAllyInfo* rev);

		void refreshAlly(SessionTask* scene);
		void userOnline(UserSession* pUser);

		CAlly*  findAlly(DWORD dwCountryID1, DWORD dwCountryID2);
		CAlly*  findAlly(DWORD dwCountryID1);
		bool 	addNewAlly(DWORD dwCountryID1, DWORD dwCountryID2);
		void 	addReadyAlly(DWORD dwCountryID1, DWORD dwCountryID2);
		bool    fireAlly(DWORD dwCountryID1, DWORD dwCountryID2);
		void    removeAlly(DWORD dwCountryID1, DWORD dwCountryID2);
	private:
		std::vector<CAlly*> allies;
		zRWLock rwlock;
};

#endif

