/**
 * \file
 * \version  $Id: CUnion.h  $
 * \author  
 * \date 
 * \brief �������࣬��������
 *
 */

#ifndef _CLASS_UNION_H_
#define _CLASS_UNION_H_

#include <vector>
#include "zMutex.h"
#include "zMisc.h"
#include "zType.h"
#include "zEntry.h"
#include "zEntryManager.h"
#include "Union.h"
#include "Command.h"
#include "SessionCommand.h"
#include "zUser.h"
#include "zSceneManager.h"
#include "zSingleton.h"

using namespace UnionDef;

class UserSession;
class zRWLock;
class CUnionMember;
class CUnion;

class CUnionM:public zEntryManager<zEntryID,zEntryName>,
	      public Singleton<CUnionM>
{
	friend class SingletonFactory<CUnionM>;
	private:
		/// ��Ա����
		std::map<std::string, CUnionMember*> memberIndex;

		/// ���Ͷ���
		typedef std::map<std::string, CUnionMember*>::value_type memberIndexValueType;

		/// ��д��
		zRWLock rwlock;

		CUnion *	createUnionAndAddMaster(const stUnionInfo&);
		bool 		initAddUnionMember(const DWORD &, const stUnionMemberInfo &, bool notify = false);
		bool		createUnionDBRecord(stUnionInfo &info);
		void		removeEntryByName(const char * name);
		CUnion*		createUnionByDBRecord(const stUnionInfo & info);
		inline std::map<std::string, CUnionMember *>::iterator  findMemberIndex(const char *pName);
		CUnionM();

	public:
		~CUnionM();
		template <class YourEntry>
		bool execEveryOne(execEntry<YourEntry> &exec)
		{
			rwlock.rdlock();
			bool ret=execEveryEntry<>(exec);
			rwlock.unlock();
			return ret;
		}

		template <class YourEntry>
		void removeOne_if(removeEntry_Pred<YourEntry> &pred)
		{
			rwlock.wrlock();
			removeEntry_if<>(pred);
			rwlock.unlock();
		}

		CUnion * getUnionByName( const char * pName);
		CUnion* getUnionByID(DWORD dwUnionID);

		bool init();
		static void destroyMe();

		void fireUnionMember(UserSession*, const char *);

		/**
		  * \brief ɾ������Ա
		  *
		  * ����ý�ɫ���ǰ��᳤�����߳���������ǣ���������������
		  *
		  * \param dwUserID ��ɫID
		  * \param find     �Ƿ�Ϊ����
		  *
		  * \return ����ý�ɫ�����κΰ���У��򷵻�2
		  *         ����ý�ɫ�ǰ������򷵻�0
		  *         ����ý�ɫ�ǰ��ڣ���ɾ���ɹ����򷵻�1
		  *
		  */
		int  fireUnionMember(DWORD dwUserID, bool find);

		bool addNewMemberToUnion(const DWORD dwUnionID, const stUnionMemberInfo&);
		void addNewSeptToUnion(const DWORD dwUnionID, const DWORD dwSeptID);
		
		void userOnline(UserSession *);
		void userOffline(const UserSession *);
		void createNewUnion(Cmd::Session::t_addUnion_SceneSession *data);
		bool addMemberIndex(const char *, CUnionMember *);
		bool removeMemberIndex(const char *);
		bool processUserMessage(UserSession *pUser,const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);
		void processUnionSortMessage(UserSession* pUser, const Cmd::stReqUnionSort* ptCmd);
		void processAppointCityCatcherMessage(UserSession* pUser, const Cmd::stAppointCityCatcherCmd* ptCmd);
		void processCancelCityCatcherMessage(UserSession* pUser, const Cmd::stCancelCityCatcherCmd* ptCmd);

		bool processSceneMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);
		bool processSceneUnionMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);
		
		bool processGateMessage(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen);
		void delUnion(const DWORD dwUnionID);
		void processMemberLeave(const DWORD dwUnionID, const DWORD dwCharID);
		void processMemberPower(UserSession* pUser, const Cmd::stUnionMemberPower* ptCmd);
		bool havePowerByName(const char* name, const int power);

		void sendUnionNotify(const DWORD unionID, const char* message, ...);

		void sendUnionChatMessages(const UserSession *pUser, const Cmd::stChannelChatUserCmd *, const unsigned int cmdLen);
		void sendUnionPrivateChatMessages(const UserSession *pUser, const Cmd::stChannelChatUserCmd *, const unsigned int cmdLen);
		void sendVoteListToUser(const UserSession *pUser);
		void sendListToUser(const UserSession *pUser);
		void sendDareListToUser(const UserSession* pUser);
		
		void userVote(const UserSession *pUser, const char *pName);
		void userAboutVote(const UserSession *pUser, const char *pName);
		void addVoteMemberToUnion(const UserSession *pUser, const char *pName);
		void setUnionNote(UserSession *pUser, Cmd::stNoteUnionCmd *pCmd);


		/**
		 * \brief ���ı���
		 *
		 * \param pUser ���������(Ŀǰֻ���ǰ���)
		 *
		 * \param pCmd ��������
		 *
		 * \return 
		 */
		void change_aliasname(UserSession *pUser, Cmd::stChangeUnionMemberAliasName* pCmd);
		void delAllUnion(DWORD id);


		/**
		 * \brief ʱ��ص�����
		 */
		void timer();
		char * getUnionNameByUserName(char *Name);
		DWORD getCountryIDByUserName(char *Name);
};

class CUnion:public zEntryManager<zEntryID, zEntryName>, public zEntry
{
	private:
		/// ��ἶ��
		WORD                  level;

		/// ��ᾭ��
		DWORD                 exp;

		/// ����״̬����ִ��д��Ȳ���
		bool					destroy;

		/// �Ƿ񻹴���ͶƱ�ڼ䣬1Ϊ��0Ϊ��
		BYTE					byVote;

	
       /// ��д��
		zRWLock rwlock;

	public:
		/// ���᳤
		CUnionMember          *master;
		/// ����ʱ��
		DWORD					dwCreateTime;
		/// ����
		char		note[255];

		/// �����������ID
		DWORD dwCountryID;

		/// �������
		DWORD dwMana;

		// ��ǰ�ж���
		DWORD dwActionPoint;

		// ����ʽ�������
		DWORD dwMoney;
		DWORD calltimes;

		DWORD getActionPoint();//�õ��ж���
		bool  changeActionPoint(int repute);//�ı��ж���
		
		DWORD getMoney();//�õ�����ʽ�
		bool  changeMoney(int money);//�ı����ʽ�

		void sendUnionDare(UserSession* pUser, const char* fromName, DWORD dwWarID);

		template <class YourEntry>
		bool execEveryOne(execEntry<YourEntry> &exec)
		{
			rwlock.rdlock();
			bool ret=execEveryEntry<>(exec);
			rwlock.unlock();
			return ret;
		}

		template <class YourEntry>
		void removeOne_if(removeEntry_Pred<YourEntry> &pred)
		{
			rwlock.wrlock();
			removeEntry_if<>(pred);
			rwlock.unlock();
		}

		CUnion();
		~CUnion();
		void			removeEntryByName(const char * name);
		DWORD			getID() {return id;}
		void			init(const stUnionInfo & info);
		CUnionMember *  addUnionMember(const stUnionMemberInfo&);
		CUnionMember *  addUnionMaster(const stUnionInfo& info);
		void			changeMaster(CUnionMember* pMember);
		void			sendUserUnionData(const char *pName);
		void			fireUnionMember(const char *, const char *);
		int			fireUnionMemberDirect(const DWORD dwCharID, const bool checksept=true);
		void 			fireSeptFromUnion(const DWORD dwSeptID);

		void			sendCmdToAllMember(const Cmd::stNullUserCmd * ptCmd, const unsigned int nCmdLen );
		void 			sendCmdToAllMemberScene(Cmd::Session::t_enterWar_SceneSession* ptEnterWarCmd, const unsigned int cmdLen);
		void			sendCallCmdToAllMemberScene(Cmd::Session::t_GoTo_Leader_Check_SceneSession* ptEnterWarCmd, const unsigned int cmdLen,DWORD expect);
		void			notifyMemberFire(const char *,const char *);
		void      	        notifyWarResult(const char* msg,...);
		void			writeDatabase();
		void			sendUnionMemberList(UserSession *pUser);
		void			sendUnionInfoToUser(UserSession *pUser);
		bool			deleteMeFromDB();
		void 			sendUnionNotify(const char* message, ...);

		bool			isMember(DWORD dwUserID);

		void			disbandUnion();
		void			delUnion(const DWORD dwUnionID);
		bool			loadUnionMemberFromDB();
		DWORD			size();
		bool			isVote();
		void			letVoteOver();		
		void			setNote(Cmd::stNoteUnionCmd *pCmd);
	
		void			sendUnionInfoToAll();
		void			sendUnionManaToAll();
		// �ж��ܷ��ɢ���
		bool			isDel();

		// ͳ�Ʊ����м�������
		DWORD 			septSize();

		void 			changeAllSeptRepute(int repute);
	
		// ˢ�¸ð�����м��徭����ȡ��־	
		void 			refreshSeptExp();


		// ��ð������
		DWORD			getMana();
		
		// �������г�Ա���ݵ�����	
		void 			update_all_data();

		// ����һ����Ա��ֱ���뿪
		void		fireUnionMemberLeave(DWORD dwUserID);	
};

struct CUnionMember:public zEntry
{
public:

		/// ��Ա����
		char  aliasname[MAX_NAMESIZE+1];
		/// ��ԱȨ��
		BYTE  byPower[2];

		/// ��Ա״̬
		BYTE  byStatus;

		/// ��Աְҵ
		WORD  wdOccupation;

		/// ��Ա��������
		DWORD septid;

		/// �û��ĻỰ����
		//UserSession * user;

		/// ������������ָ��
		CUnion *myUnion;

		/// ����״̬����ִ��д��Ȳ���
		bool  destroy;
		
		/// ������
		zRWLock rwlock;

		/// ���߱�־ö��
		enum {
					Offline,
					Online
		};


		void init(const stUnionMemberInfo&);
		void getMemberBaseInfo(struct Cmd::stUnionRember& info);
		void sendUserUnionPrivateData();
		void sendUnionDare(UserSession* pDareUser, const char* fromName, DWORD dwWarID);
		void sendCmdToMe(const Cmd::stNullUserCmd *pstrCmd, const unsigned int nCmdLen);
		void writeDatabase();
		void insertDatabase();
		void setUnion(CUnion * pUnion);
		void online(const unsigned int status = Cmd::UNION_MEMBER_STATUS_ONLINE);
		bool isOnline();
		void offline();
		bool havePower(const int power);
		void setPower(const int power);
		void sendMessageToMe(const char *);
		void fireMe(const bool notify = true, const bool checksept = true);
		bool deleteMeFromDB();
		void sendUserUnionData();
		void change_aliasname(const char* aliasname);
		void update_data();
			
		CUnionMember();
		~CUnionMember();
		
};

#endif

