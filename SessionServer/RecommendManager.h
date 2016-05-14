/**
 * \file	RecommendManager.h
 * \version  	$Id: RecommendManager.h  $
 * \author  	
 * \date 	
 * \brief 	推荐人管理器定义
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
		char name[MAX_NAMESIZE]; // 被推荐人姓名
		DWORD id; // 被推荐人用户ID
		DWORD lastLevel; // 最后一次提取奖励的等级
		DWORD recommendid; // 推荐人ID
		DWORD dwTotal; // 累计已领取的金额

		Recommend*  myRecommend; // 我的推荐人
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
		DWORD id; // 推荐人ID
		char  name[MAX_NAMESIZE]; // 推荐人名字
		DWORD dwBalance;	// 推荐人可以提取的奖励
		DWORD dwTotal;		// 累积领取的奖励
		std::vector<RecommendSub*> subs; // 被推荐人列表
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
		/// 类型定义
		typedef std::map<DWORD, RecommendSub*>::value_type recommendsubIndexValueType;
				
		typedef std::map<DWORD, RecommendSub *>::iterator resubIter;
		std::map<DWORD, RecommendSub*> recommendsubIndex;
	protected:
		RecommendM();
		zRWLock rwlock;
};

#endif
