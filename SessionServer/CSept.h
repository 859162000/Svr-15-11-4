/**
 * \file
 * \version  $Id: CSept.h  $
 * \author  
 * \date
 * \brief ��������࣬���������
 *
 */


#ifndef _CLASS_SEPT_H_
#define _CLASS_SEPT_H_

#include <vector>
#include "zMutex.h"
#include "zMisc.h"
#include "zType.h"
#include "zEntry.h"
#include "zEntryManager.h"
#include "Sept.h"
#include "Command.h"
#include "SessionCommand.h"
#include "zUser.h"
#include "zSceneManager.h"

using namespace SeptDef;

class UserSession;
class zRWLock;
class CSeptMember;
class CSept;

class CSeptM:public zEntryManager<zEntryID,zEntryName>
{
	private:
		/// ��Ա����
		std::map<std::string, CSeptMember*> memberIndex;

		/// ���Ͷ���
		typedef std::map<std::string, CSeptMember*>::value_type memberIndexValueType;

		/// Ψһʵ��ָ��
		static CSeptM * um;

		/// ��д��
		zRWLock rwlock;

		CSept *	createSeptAndAddMaster(const stSeptInfo&);
		bool 		initAddSeptMember(const DWORD &, const stSeptMemberInfo &, bool notify = false);
		bool		createSeptDBRecord(stSeptInfo &info);
		void		removeEntryByName(const char * name);
		CSept*		createSeptByDBRecord(const stSeptInfo & info);
		inline std::map<std::string, CSeptMember *>::iterator  findMemberIndex(const char *pName);
		CSeptM();
		~CSeptM();

	public:
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

		CSept * getSeptByName( const char * name);
		
		CSept* getSeptByID(DWORD dwSeptID);
		CSeptMember * getMemberByName(const char * name);

		bool init();
		static CSeptM &getMe();
		static void destroyMe();

		void fireSeptMember(UserSession*, const char *);
		void notifyNpcHoldData(DWORD septid);

		/**
		  * \brief ɾ�������Ա
		  *
		  * ����ý�ɫ���Ǽ����峤�����߳���������ǣ���������������
		  *
		  * \param dwUserID ��ɫID
		  * \param find     ΪTRUE��ʾ���ң�ΪFALSEΪɾ��
		  *
		  * \return ����ý�ɫ�����κμ����У��򷵻�2
		  *         ����ý�ɫ���峤���򷵻�0
		  *         ����ý�ɫ����Ա����ɾ���ɹ����򷵻�1
		  *
		  */
		int  fireSeptMember(DWORD dwUserID, bool find);

		bool addNewMemberToSept(const DWORD dwSeptID, const stSeptMemberInfo&);
		void userOnline(UserSession *);
		void userOffline(const UserSession *);
		void createNewSept(Cmd::Session::t_addSept_SceneSession *data);
		bool addMemberIndex(const char *, CSeptMember *);
		bool removeMemberIndex(const char *);
		bool processUserMessage(UserSession *pUser,const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);
		
		void processSeptSortMessage(UserSession* pUser, 
				const Cmd::stReqSeptSort* ptCmd);
		
		void processRequestSeptExpMessage(UserSession* pUser, const Cmd::stRequestSeptExpCmd* ptCmd);
		void processRequestSeptNormalExpMessage(UserSession* pUser, const Cmd::stRequestSeptNormalExpCmd* ptCmd);
		
		bool processSceneMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);
		bool processSceneSeptMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);
		bool processGateMessage(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen);
		void delSept(const DWORD dwSeptID);
		void processMemberLeave(const DWORD dwSeptID, const DWORD dwCharID);
		void sendSeptNotify(const DWORD septID, const char* message, ...);
		void sendNpcDareCmdToScene(const DWORD septID, Cmd::Session::t_NpcDare_NotifyScene_SceneSession* ptCmd, unsigned int nCmdLen);
		void sendSeptChatMessages(const UserSession *pUser, const Cmd::stChannelChatUserCmd *rev, const unsigned int cmdLen);
		void sendSeptPrivateChatMessages(const UserSession *pUser, const Cmd::stChannelChatUserCmd *rev, const unsigned int cmdLen);
		void sendVoteListToUser(const UserSession *pUser);
		void sendListToUser(const UserSession *pUser);
		void sendDareListToUser(const UserSession *pUser);
		void userVote(const UserSession *pUser, const char *pName);
		void userAboutVote(const UserSession *pUser, const char *pName);
		void addVoteMemberToSept(const UserSession *pUser, const char *pName);
		void disributeExp(Cmd::Session::t_distributeSeptExp_SceneSession *cmd);
		void setSeptNote(UserSession *pUser, Cmd::stNoteSeptCmd *pCmd);
		DWORD findUserSept(DWORD dwUserID);
		void delSeptAllMember();

		/**
		 * \brief ���ı���
		 *
		 * \param pUser ���������(Ŀǰֻ���ǰ���)
		 *
		 * \param pCmd ��������
		 *
		 * \return 
		 */
		void change_aliasname(UserSession *pUser, Cmd::stChangeSeptMemberAliasName* pCmd);
		void changeAllRepute(DWORD countryid, int repute);
		DWORD getRepute(DWORD dwSeptID);//�õ�����ֵ
		void changeRepute(DWORD dwSeptID, int repute);//�ı�����ֵ
		void changeLevel(DWORD dwSeptID, int level);
		char * getSeptNameByUserName(char *Name);
		DWORD getSeptIDByUserName(char *Name);		
};

class CSept:public zEntryManager<zEntryID, zEntryName>, public zEntry
{
	private:
		/// ����״̬����ִ��д��Ȳ���
		bool					destroy;
		
		/// ��д��
		zRWLock rwlock;

	public:
		/// �Ƿ񻹴���ͶƱ�ڼ䣬1Ϊ��0Ϊ��
		BYTE					byVote;

		/// ����᳤
		CSeptMember          *master;

		/// ����ʱ��
		DWORD					dwCreateTime;

		/// ����
		char					note[255];

		/// ������������
		DWORD dwCountryID;

		/// �����������
		DWORD dwUnionID;

		/// 
		void clearUnion()
		{
			this->rwlock.wrlock();
			this->dwUnionID = 0;
			this->rwlock.unlock();
		}
		

		/// ����ȼ�
		DWORD dwLevel;

		/// ��������
		DWORD dwRepute;

		/// �������ѽ��
		DWORD dwSpendGold;
		
		/// �����Ƿ���ȡ����
		DWORD dwIsExp;

		/// ����ʹ�ô���
		DWORD calltimes;
		/// ����ʹ��ʱ��
		DWORD calldaytime;
		/// �ϴ���ȡ��ͨ���徭���ʱ��
		DWORD normal_exp_time;

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

		CSept();
		~CSept();
		void			removeEntryByName(const char * name);
		DWORD			getID() {return id;}
		void			init(const stSeptInfo & info);
		CSeptMember *  		addSeptMember(const stSeptMemberInfo&);
		CSeptMember *  		addSeptMaster(const stSeptInfo& info);
		CSeptMember *		getMemberByName(const char* pName);
		void			sendUserSeptData(const char *pName);
		void			fireSeptMember(const char *, const char *);
		int			fireSeptMemberDirect(const DWORD dwCharID, const bool checkunion=true);

		void			sendCmdToAllMember(const Cmd::stNullUserCmd * ptCmd, const unsigned int nCmdLen );
		void			sendCmdToAllMemberExcept(const Cmd::stNullUserCmd * ptCmd, const unsigned int nCmdLen , const char * except);
		bool			isMember(DWORD dwUserID);
		void                    sendSeptNotify(const char* message, ...);

		void			sendCmdToAllMemberScene(Cmd::Session::t_enterWar_SceneSession* ptEnterWarCmd, const unsigned int cmdLen);
		void			sendCallCmdToAllMemberScene(Cmd::Session::t_GoTo_Leader_Check_SceneSession* ptEnterWarCmd, const unsigned int cmdLen,DWORD expect);
		void			sendNpcDareCmdToScene(Cmd::Session::t_NpcDare_NotifyScene_SceneSession* pCmd, unsigned int cmdLen);

		void			sendDistributeSeptExpToScene(const DWORD dwUserID, const Cmd::Session::t_distributeSeptExp_SceneSession * ptCmd);
		void			notifyMemberFire(const char *);
		void			writeDatabase();
		void			sendSeptMemberList(UserSession *pUser);
		void			full_SeptMember_Data(Cmd::stUnionRember*& tempPoint, unsigned int& count);
		void			sendSeptInfoToUser(UserSession *pUser);
		bool			deleteMeFromDB();
		void			disbandSept();
		void			delSept(const DWORD dwSeptID);
		bool			loadSeptMemberFromDB();
		DWORD			size();
		bool			isVote();
		void			letVoteOver();
		
		void	setExp()
		{
			rwlock.wrlock();
			this->dwIsExp = 1;
			rwlock.unlock();
			this->writeDatabase();
		}
		
		void	clearExp()
		{
			rwlock.wrlock();
			this->dwIsExp = 0;
			rwlock.unlock();
			this->writeDatabase();
		}

		DWORD getExp()
		{
			return this->dwIsExp;
		}

		void			setNote(Cmd::stNoteSeptCmd *pCmd);
		void                    notifyWarResult(const char* msg, ...);
		void 			delSeptAllMember();
		DWORD 			getRepute();//�õ�����ֵ
		void                    changeRepute(int repute);//�ı�����ֵ
		void			changeLevel(int level);
		void			sendSeptInfoToAll(); // ����֪ͨ������Ϣ
		void			sendSeptReputeToAll(); // ֪ͨ���м����Ա�����ĸı�
		void			notifyNpcHoldData();
		void			sendGoldToMember(DWORD userID, DWORD num);
};

struct CSeptMember:public zEntry
{
public:
		/// ��Ա״̬
		BYTE  byStatus;
		
		/// ��Ա����
		char  aliasname[MAX_NAMESIZE+1];

		/// ��Աְҵ
		WORD  wdOccupation;

		/// �û��ĻỰ����
		//UserSession * user;

		/// �������������ָ��
		CSept *mySept;

		/// ����״̬����ִ��д��Ȳ���
		bool  destroy;
		
		/// ������
		zRWLock rwlock;

		/// ����״̬ö��
		enum {
					Offline,
					Online
		};


		void init(const stSeptMemberInfo&);
		void getMemberBaseInfo(struct Cmd::stSeptRember& info);
		void sendCmdToMe(const Cmd::stNullUserCmd *pstrCmd, const unsigned int nCmdLen);
		void writeDatabase();
		void insertDatabase();
		void setSept(CSept * pSept);
		void online(const unsigned int status = Cmd::SEPT_MEMBER_STATUS_ONLINE);
		void offline();
		bool isOnline();
		void sendMessageToMe(const char *);
		void sendMessageToMe(int type, const char *);
		void fireMe(const bool notify = true, const bool checkunion=true);
		bool deleteMeFromDB();
		void sendUserSeptData();
		void change_aliasname(const char* aliasname);
		void update_data();
		void update_normal_data();
		void notifyNpcHoldData();
		void sendGoldToMember(DWORD userID, DWORD num);
		CSeptMember();
		~CSeptMember();
		
};

#endif

