/**
 * \file
 * \version  $Id: CArmy.h$
 * \author  
 * \date 
 * \brief ���Ҿ���
 *
 * 
 */

#ifndef _CARMY_MANAGER_H
#define _CARMY_MANAGER_H
#include <vector>
#include "zType.h"
#include "zRWLock.h"
#include "SessionCommand.h"
#include "zSingleton.h"
#include "zEntry.h"
#include "zEntryManager.h"

class Record;
class RecordSet;
class UserSession;
class CArmy;

class CCaptain
{//�ӳ�
	public:
		DWORD dwArmyID; // ����ID
		DWORD dwCharID; // �ӳ�ID
		char  szCapName[MAX_NAMESIZE]; // �ӳ�����
		DWORD dwNpcNum; // �ӳ������NPC������ʱδ��
		CArmy* myArmy;
		
		void init(Record* rec);
		void writeDatabase();
		bool insertDatabase();
		bool deleteMeFromDB();
		void update_scene();
		void fireMe();

		CCaptain();
		~CCaptain();
};

class CArmy
{
	friend class CArmyM;
	friend class CCaptain;
	public:
		void init(Record* rec);
		void writeDatabase();
		bool insertDatabase();
		bool deleteMeFromDB();
		bool loadCaptainFromDB();

		bool fireCaptain(DWORD dwUserID);
		bool hireCaptain(DWORD dwUserID);
		void changeName(const char* newname);
		void update_all_captain();
		bool canAddCaptain();
		
		CArmy();
		~CArmy();
		enum
		{
			WAIT_CREATE = 1,
			FINISH_CREATE = 2,
		};

		void status(BYTE value)
		{
			rwlock.wrlock();
			byStatus = value;
			rwlock.unlock();
		}

	protected:
		DWORD dwID; // ����ID
		DWORD dwCountryID; // ��������ID
		DWORD dwCityID;	// ��������ID
		
		char  name[MAX_NAMESIZE]; // ��������
		DWORD  dwGenID; // ���ӽ�����ɫID
		char  genName[MAX_NAMESIZE]; // ���ӽ�������

		BYTE  byStatus;
		DWORD dwCreateTime;

		zRWLock rwlock;
		std::vector<CCaptain*> captains;
};

class CArmyM : public Singleton<CArmyM>
{
	friend class SingletonFactory<CArmyM>;
	public:
		bool init();
		static void destroyMe();
		void timer();
		bool load();
		
		bool processUserMessage(UserSession *pUser,const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);
		bool processSceneMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);

		void processReqWaitGen(UserSession* pUser, Cmd::stReqWaitGenUserCmd* rev);
		void processCreateArmy(UserSession* pUser, Cmd::stCreateArmyUserCmd* rev);
		void processChangeArmyName(UserSession* pUser, Cmd::stChangeArmyNameUserCmd* rev);
		
		void processReqArmyList(Cmd::Session::t_ReqArmyList_SceneSession* rev);
		void processReqArmySpec(UserSession* pUser, Cmd::stReqArmySpecUserCmd* rev);
		void processExitArmy(UserSession* pUser, Cmd::stExitArmyUserCmd* rev);
		void processRemoveArmy(UserSession* pUser, Cmd::stRemoveArmyUserCmd* rev);
		void processAddCaptain(UserSession* pUser, Cmd::stAddArmyCaptainUserCmd* rev);
		void processFireCaptain(UserSession* pUser, Cmd::stFireArmyCaptainUserCmd* rev);
		
		void userOnline(UserSession *pUser);
		
		//CArmy* find(DWORD dwCountryID, DWORD dwCityID);
		int    countByCity(DWORD dwCountryID, DWORD dwCityID);
		CArmy* findByID(DWORD dwArmyID);
		CArmy* findByGenID(DWORD dwGenID);
		CArmy* findByName(const char* value);

		bool	addCaptain(DWORD dwUserID, CCaptain* pCaptain);
		bool	removeCaptain(DWORD dwUserID);
		bool    isCaptain(DWORD dwUserID);

		~CArmyM();
		void removeArmyByID(DWORD dwArmyID);
		void removeArmyByGenID(DWORD dwGenID);

		std::vector<CArmy*> armys;
		/// ���Ͷ���
		typedef std::map<DWORD, CCaptain*>::value_type captainIndexValueType;
				
		typedef std::map<DWORD, CCaptain *>::iterator capIter;
		std::map<DWORD, CCaptain*> captainIndex;
		

	protected:
		CArmyM();
		zRWLock rwlock;
};

#endif

