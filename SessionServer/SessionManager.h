/**
 * \file
 * \version  $Id: SessionManager.h  $
 * \author 
 * \date 
 * \brief 会话管理器的类
 *
 * 
 */

#ifndef _SESSIONMANAGER_H_
#define _SESSIONMANAGER_H_
#include "Session.h"
#include "zUserManager.h"
#include "zUniqueID.h"
#include "SessionTask.h"

/**
 * \brief 用户会话管理器
 *
 */
class UserSessionManager:public zUserManager
{
	private:
		///管理器实例
		static UserSessionManager *sm;

		///是否初始化成功
		bool inited;

		UserSessionManager();
		~UserSessionManager();
		inline bool getUniqeID(DWORD &tempid);
		inline void putUniqeID(const DWORD &tempid);

	public:
		static UserSessionManager *getInstance();
		static void delInstance();
		struct Compare
		{
			virtual bool operator ()(UserSession*) = 0;
			virtual ~Compare(){}
		};

		bool init();
		void final();
		UserSession *getUserSessionByName( const char * name);
		UserSession *getUserByTempID(DWORD tempid);
		UserSession *getUserByID(DWORD id);
		void sendGraceSort(UserSession* pUser);
		void sendExploitSort(UserSession* pUser);
		void sendCmdByCondition(const Cmd::stNullUserCmd *pstrCmd, const unsigned int nCmdLen, Compare* compare);
		void removeAllUserByTask(SessionTask *task);
};


/**
 * \brief 场景会话管理器
 *
 */
class SceneSessionManager:public zSceneManager
{
	private:
		///管理器实例
		static SceneSessionManager *sm;

		///是否初始化成功
		bool inited;

		SceneSessionManager();
		~SceneSessionManager();
		bool getUniqeID(DWORD &tempid);
		void putUniqeID(const DWORD &tempid);

	public:
		static SceneSessionManager *getInstance();
		static void delInstance();

		bool init();
		void final();

		bool addScene(SceneSession *scene);
		SceneSession * getSceneByName(const char *name);
		SceneSession * getSceneByFile(const char *file);
		SceneSession * getSceneByID(DWORD id);
		SceneSession * getSceneByTempID(DWORD tempid);
		void removeAllSceneByTask(SessionTask *task);
		void removeScene(SceneSession *scene);
		void notifyCountryTax(DWORD dwCountry,  BYTE byTax);

};
#endif
