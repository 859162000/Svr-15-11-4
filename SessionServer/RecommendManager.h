/**
 * \file	RecommendManager.h
 * \version  	$Id: RecommendManager.h  $
 * \author  	
 * \date 	
 * \brief 	�Ƽ��˹���������
 *
 * 
 */
#ifndef _RECOMMEND_MANAGER_H
#define _RECOMMEND_MANAGER_H

#include <vector>
#include "zType.h"
#include "zRWLock.h"
#include "SessionCommand.h"
#include "RecordCommand.h"
#include "zSingleton.h"

class Record;
class RecordSet;
class UserSession;
class Recommend;

class RecommendSub
{
	public:
		char name[MAX_NAMESIZE]; // ���Ƽ�������
		DWORD id; // ���Ƽ����û�ID
		DWORD lastLevel; // ���һ����ȡ�����ĵȼ�
		DWORD recommendid; // �Ƽ���ID
		DWORD dwTotal; // �ۼ�����ȡ�Ľ��

		Recommend*  myRecommend; // �ҵ��Ƽ���
		void init(Record* rec);
		void writeDatabase();
		bool insertDatabase();
		bool deleteMeFromDB();
		bool fireMe();
		void pickupBounty(UserSession* pUser);
		DWORD queryBounty();
		DWORD queryTotal()
		{
			return dwTotal;
		}

		RecommendSub();
		~RecommendSub();
};

class Recommend
{
	friend class RecommendM;
	friend class RecommendSub;

	public:
		void init(Record* rec);
		void writeDatabase();
		bool insertDatabase();
		bool deleteMeFromDB();
		bool loadRecommendSubFromDB();

		bool addRecommended(DWORD dwUserID);
		void rmRecommended(DWORD dwUserID);
		void processQuery(UserSession* pUser);

		void pickupBounty(UserSession* pUser);
		DWORD queryBounty()
		{
			return this->dwBalance;
		}

		DWORD queryTotal()
		{
			return this->dwTotal;
		}
		

		Recommend();
		~Recommend();
			
	protected:
		DWORD id; // �Ƽ���ID
		char  name[MAX_NAMESIZE]; // �Ƽ�������
		DWORD dwBalance;	// �Ƽ��˿�����ȡ�Ľ���
		DWORD dwTotal;		// �ۻ���ȡ�Ľ���
		std::vector<RecommendSub*> subs; // ���Ƽ����б�
		zRWLock rwlock;
};

class RecommendM : public Singleton<RecommendM>
{
	friend class SingletonFactory<RecommendM>;
	public:
		bool init();
		static void destroyMe();
		void timer();
		bool load();

		bool processUserMessage(UserSession *pUser,const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);
		bool processSceneMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);
		void processAddRecommended(const Cmd::Record::t_chkUserExist_SessionRecord* cmd);

		Recommend* findByID(DWORD dwUserID);
		RecommendSub* findSubByID(DWORD dwUserID);

		bool addRecommend(DWORD dwUserID, Recommend* r);
		bool addRecommendSub(DWORD dwUserID, RecommendSub* rs);
		void removeRecommend(DWORD dwUserID);
		bool removeRecommendSub(DWORD dwUserID);

		void fireRecommendSub(DWORD dwUserID);

		~RecommendM();
		
		typedef std::map<DWORD, Recommend*>::value_type recommendValueType;
		typedef std::map<DWORD, Recommend*>::iterator recommendIter;
		std::map<DWORD, Recommend*> recommends;
		/// ���Ͷ���
		typedef std::map<DWORD, RecommendSub*>::value_type recommendsubIndexValueType;
				
		typedef std::map<DWORD, RecommendSub *>::iterator resubIter;
		std::map<DWORD, RecommendSub*> recommendsubIndex;
	protected:
		RecommendM();
		zRWLock rwlock;
};

#endif
