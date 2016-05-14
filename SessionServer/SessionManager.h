/**
 * \file
 * \version  $Id: SessionManager.h  $
 * \author 
 * \date 
 * \brief �Ự����������
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
 * \brief �û��Ự������
 *
 */
class UserSessionManager:public zUserManager
{
	private:
		///������ʵ��
		static UserSessionManager *sm;

		///�Ƿ��ʼ���ɹ�
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
 * \brief �����Ự������
 *
 */
class SceneSessionManager:public zSceneManager
{
	private:
		///������ʵ��
		static SceneSessionManager *sm;

		///�Ƿ��ʼ���ɹ�
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
