/**
 * \file
 * \version  $Id: SchoolManager.h  $
 * \author  
 * \date 
 * \brief ����ʦ�����ɹ�����
 *
 */

#ifndef __CLASS_SCHOOLMANAGER__
#define __CLASS_SCHOOLMANAGER__

#include <vector>
#include <list>
#include "zRWLock.h"
#include "zMisc.h"
#include "zType.h"
#include "zEntry.h"
#include "zEntryManager.h"
#include "zSocket.h"
#include "Command.h"
#include "SessionCommand.h"
#include "zUser.h"
#include "School.h"

using namespace School;

class UserSession;
class CSchool;
class CSchoolMember;

/**
 * \brief ������Ա�б����
 *
 */
class CSchoolMemberListCallback
{
private:
	/**
	 * \brief ��Ӱ��ĳ�Ա�б�
	 */
	std::list<CSchoolMember * > memberList;
public:

	/**
	 * \brief �����Ч�ڵ��ϵ
	 * \author fqnewman
	 */
	void clearInValidNodeRelation();

	/**
	 * \brief ����
	 * \param member ��ǰ�ڵ�
	 * \author fqnewman
	 */
	void exec(CSchoolMember *member);

	/**
	 * \brief ����֪ͨ����Ӱ��ĳ�Ա
	 * \author fqnewman
	 */
	void sendNotifyToMember();
};

/**
 * \brief ���Ա����������Ա��Ϣ�Ļص�
 *
 */
class CSendSchoolCallback
{
private:
	///��Ա�б�
	std::list<struct Cmd::stSchoolMember> memberList;
public:
	void exec(CSchoolMember *member, const BYTE tag);
	void sendListToMember(CSchoolMember *member);
};

/**
 * \brief ���ɹ�����
 *
 */
class CSchoolM : public zEntryManager<zEntryID,zEntryName>
{
private:
		/// ��Ա������
		std::map<std::string, CSchoolMember*> memberIndex;

		/// ���Ͷ���
		typedef std::map<std::string, CSchoolMember*>::value_type memberIndexValueType;

		/// Ψһʵ��
		static CSchoolM * sm;
		///��д��
		zRWLock rwlock;
		inline std::map<std::string, CSchoolMember *>::iterator  findMemberIndex(const char *pName);

public:
		
		/**
		 * \brief ��������
		 * \author fqnewman 
		 */
		~CSchoolM();
		
		/**
		 * \brief ��ʼ��ʦ�����ɹ���������
		 * \author fqnewman 
		 * \return true Ϊ�ɹ���falseΪʧ��
		 */
		bool init();

		template <class YourEntry>
		bool execEveryOne(execEntry<YourEntry> &exec)
		{
			rwlock.rdlock();
			bool ret=execEveryEntry<>(exec);
			rwlock.unlock();
			return ret;
		}

		/**
		 * \brief ��ȡΨһ����ʵ��
		 * \author fqnewman
		 * \return Ψһ����ʵ��
		 */
		static CSchoolM &getMe();

		/**
		 * \brief �ݻ�ʦ�����ɹ��������������ƺ���
		 * \author fqnewman
		 */
		static void destroyMe();

		/**
		 * \brief ��ɫ����֪ͨ���������ж�Ӧ�Ľڵ�������ߴ������UserSession���Ҷ������������֪ͨ
		 * \param pUser ���߽�ɫ�ĻỰ����
		 * \author fqnewman
		 */
		void userOnline(UserSession *pUser);

		/**
		 * \brief ��ɫ����֪ͨ���������ж�Ӧ�Ľڵ�������ߴ������Ҷ������������֪ͨ
		 * \param pUser ���ߵĽ�ɫ
		 * \author fqnewman
		 */
		void userOffline(UserSession * pUser);

		/**
		 * \brief �������������ָ����ɫ������
		 * \param name ָ����ɫ������
		 * \param member ��ɫ�Ľڵ����
		 * \author fqnewman
		 * \return ʧ�ܷ���false,�ɹ�����true
		 */
		bool addMemberIndex(const char *name, CSchoolMember *member);

		/**
		 * \brief ����������ɾ��ָ���Ľ�ɫ������
		 * \param name ָ����ɫ������
		 * \author fqnewman
		 * \return ʧ�ܷ���false,�ɹ�����true
		 */
		bool removeMemberIndex(const char *name);

		/**
		 * \brief ����ͻ����͹�������Ϣ��Ҳ�п���������������ת�������Ŀͻ�����Ϣ��
		 * \param pUser ������Ϣ�Ľ�ɫ
		 * \param ptNullCmd ��Ϣ�ṹ
		 * \param cmdLen ��Ϣ����
		 * \author fqnewman
		 * \return ʧ�ܷ���-1, û����Ʒ����������0, ����������1, �ɹ�������д�������Ʒ����2
		 */
		bool processUserMessage(UserSession *pUser,const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);

		/**
		 * \brief �������������͹�������Ϣ
		 * \param ptNullCmd ��Ϣ�ṹ
		 * \param cmdLen ��Ϣ����
		 * \author fqnewman
		 * \return ʧ�ܷ���-1, û����Ʒ����������0, ����������1, �ɹ�������д�������Ʒ����2
		 */
		bool processSceneMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);

		/**
		 * \brief �����������������Ϣת��
		 * \param pUser ������Ϣ������
		 * \param rev ��Ϣ�ṹ��
		 * \param cmdLen ��Ϣ����
		 * \author fqnewman
		 */
		void sendSchoolChatMessages(const UserSession *pUser, const Cmd::stChannelChatUserCmd *rev, const unsigned int cmdLen);

		/**
		 * \brief �����������������Ϣת��
		 * \param pUser ������Ϣ������
		 * \param rev ��Ϣ�ṹ��
		 * \param cmdLen ��Ϣ����
		 * \author fqnewman
		 */
		void sendSchoolPrivateChatMessages(const UserSession *pUser, const Cmd::stChannelChatUserCmd *rev, const unsigned int cmdLen);

		/**
		 * \brief �ж��û���ͽ���Ƿ�����
		 * \param master �����Ľ�ɫ����
		 * \param schoolName �������ƣ�������˵�ͽ��δ���Ҵ����������ɵ���ô��������ͨ�����ﷵ��
		 * \author fqnewman
		 * \return ����true��ʾmaster��ͽ��û������ ����false��ʾmaster��ͽ���Ѿ���������
		 */
		bool getUserPrenticeInfo(const char *master, char *schoolName);

		/**
		 * \brief �ж�ĳ�����Ƿ�Ϊ�ɾ��ģ���˼����û��ʦ����ͽ�ܵĹ�ϵ���������ڴ˹�������
		 * \param name �����Ľ�ɫ������
		 * \author fqnewman
		 * \return trueΪ�ɾ��ģ�false Ϊ�Ѿ���ʦ��ͽ�ܹ�ϵ
		 */
		bool isClean(const char *name);

		/**
		 * \brief Ϊ master ����һ��ͽ�� prentice��
		 *
		 * �ں����л�������˵�������о���Ĳ�����������ɹ������Ӧ��ɫ������Ϣ
		 *
		 * \param master ʦ��
		 * \param prentice ͽ��
		 * \author fqnewman
		 */
		void addMember(UserSession *master, UserSession *prentice);

		/**
		 * \brief master ����һ��ͽ�� prentice��
		 *
		 * �ں����л�������˵�������о���Ĳ�����������ɹ������Ӧ��ɫ������Ϣ
		 *
		 * \param master ʦ��
		 * \param prentice ͽ��
		 * \author fqnewman
		 */
		void frieMember(UserSession *master, const char *prentice);

		/**
		 * \brief ��ʦ�Ŵӿ���һ����ɫ
		 *
		 * \param roleName ��ɫ����
		 * \param find �Ƿ�ֻ�ǽ����ж�
		 *
		 * \return ����ý�ɫ�����κ�ʦ���У��򷵻�2
		 *         ����ý�ɫ���峤���򷵻�0
		 *         ����ý�ɫ����Ա����ɾ���ɹ����򷵻�1
		 *
		 * \author zhongjunwei
		 */
		int fireSchoolMember(const char* roleName, bool find);

		/**
		 * \brief ��ʼ����ʱ��������ݿ���load�ļ�¼����һ�����ɹ�����
		 * \param info ���ɵ���Ϣ�ṹ
		 * \author fqnewman
		 * \return �ɹ�����true,ʧ�ܷ���false
		 */
		bool createSchoolFromDB(const stSchoolInfo &info);

		/**
		 * \brief �����ݿ��м������ɵ���Ϣ�������ɹ�����
		 * \author fqnewman
		 * \return trueΪ�ɹ��� falseΪʧ��
		 */
		bool loadSchoolFromDB();

		/**
		 * \brief �����ݿ��м����������ɵĳ�Ա
		 * \author fqnewman
		 * \return trueΪ�ɹ���falseΪʧ��
		 */
		bool loadSchoolMemberFromDB();

		/**
		 * \brief ���ָ���Ľ�ɫ�Ƿ�߱��������ɵ�����
		 * \param pUser �����Ľ�ɫ
		 * \author fqnewman
		 * \return trueΪ�߱���falseΪ���߱�
		 */
		bool checkSchoolCreateCondition(const UserSession *pUser);

		/**
		 * \brief ����һ���µ�����
		 * \param userName ʦ�������
		 * \param schoolName ���ɵ�����
		 * \author fqnewman
		 * \return true Ϊ�����ɹ��� false Ϊ����ʧ��
		 */
		bool createNewSchool(const char *userName, const char *schoolName);

		/**
		 * \brief ��Ӧ���湦�����ù�����߶�ȡ����
		 * \param pUser ������
		 * \param rev ������Ϣ
		 * \author fqnewman
		 */
		void processBulletin(const UserSession *pUser, const Cmd::stSchoolBulletinCmd *rev);

		/**
		 * \brief ��ȡ������ָ���Ľڵ����
		 * \param pName ����
		 * \author fqnewman
		 * \return ��ȡָ���Ľڵ������������ڸ����ƵĶ����򷵻�NULL
		 */
		CSchoolMember *getMember(const char *pName);

		/**
		 * \brief �����Ա�˳����ɻ���ʦ��
		 * \param pUser �˳��ߵ�UserSession����
		 * \author fqnewman
		 */
		void processLeaveGroup(UserSession *pUser);

		/**
		 * \brief �����Ա�˳����ɻ���ʦ��
		 * \param roleName ��ɫ����
		 * \author zhongjunwei
		 */
		void processLeaveGroupDirect(const char* roleName);

		/**
		 * \brief ��ɢ���ɣ������������pName���쵼�����ɽ�����ɢ�����е���Ա��ϵ�ظ���ԭʼ��ʦ�Ź�ϵ
		 * \param pName ��ɢ�����ߵ�����
		 * \author fqnewman
		 * \return true ��ɢ�ɹ� false ��ɢʧ��
		 */
		bool destroySchool(const char *pName);

		/**
		 * \brief ����ID��ö�Ӧ�Ĺ�����
		 * \param id ������ID
		 * \author fqnewman
		 * \return ����ɹ����ع���������ʧ�ܷ���NULL
		 */
		CSchool *getSchool(DWORD id);

		/**
		 * \brief ����ָ���û��ļ���
		 * \param pName �û�����
		 * \param level �¼���
		 * \author fqnewman
		 */
		void setUserLevel(const char *pName, const WORD &level);

		CSchool * getSchoolByName( const char * name);
		
};

class CSchool : public zEntryManager<zEntryID, zEntryName>, public zEntry
{
private:
		/**
		 * \ʦ���ID
		 * \author fqnewman
		 */
		DWORD dwMasterSerialID;

		/**
		 * \����
		 * \author fqnewman
		 */
		std::string bulletin;

		/**
		 * \brief ������Ч��־��һ���ж��������Ƿ񻹴������ݿ���
		 * \author fqnewman
		 */
		bool destroy;

		/**
		 * \brief ��������д��
		 * \author fqnewman
		 */
		zRWLock rwlock;

public:
/*
		bool getUniqeID(DWORD &tempid){return true;};
		void putUniqeID(const DWORD &tempid){};
*/

		/**
		 * \brief ����ģ�嶨��
		 * \author fqnewman
		 */
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


		/**
		 * \brief ���캯��
		 * \author fqnewman
		 */
		CSchool();

		/**
		 * \brief ��������
		 * \author fqnewman
		 */
		~CSchool();



		/**
		 * \brief ��ȡ��������
		 * \author fqnewman
		 * \return ��������
		 */
		char *getSchoolName();

		/**
		 * \brief ������������δ����
		 * \param pName ��������
		 * \author fqnewman
		 */
		void setSchoolName(const char *pName);

		/**
		 * \brief ����ʦ��Ľڵ�ID
		 * \param id ʦ��Ľڵ�ID
		 * \author fqnewman
		 */
		void setMasterSerialID(const DWORD &id);

		/**
		 * \brief ���һ��ʦ�ŵĸ��ڵ�
		 * \param master ��ɫ����
		 * \author fqnewman
		 * \return �¼ӵĽڵ����ʧ�ܷ���NULL
		 */
		CSchoolMember *  addTeacher(UserSession *master);

		/**
		 * \brief ��ӹ�����������ָ���Ľڵ㣬���������������
		 * \param member ָ���Ľڵ�
		 * \author fqnewman
		 */
		bool addMember(CSchoolMember *member);

		/**
		 * \brief �ӹ�������ɾ��ָ���Ľڵ㣬���������������ȥ��
		 * \param member ָ���Ľڵ�
		 * \author fqnewman
		 */
		void removeMember(CSchoolMember *member);
	
		/**
		 * \brief ����������ʼ����ʦ�Ź�ϵ������
		 * \author fqnewman
		 * \return ����NULL��ʾ���ͽ��ʧ�ܣ����򷵻�ͽ�ܵĽڵ����
		 */
		void initToNoneSchool();

		/**
		 * \brief ��ʼ����������������Ϣ�ṹ�������ݿ��ʼ����ʱ����
		 * \param info ��������Ϣ�ṹ
		 * \author fqnewman
		 */
		void initSchool(const stSchoolInfo &info);

		/**
		 * \brief �����ݿ���ر��������ڵ����нڵ���Ϣ
		 * \author fqnewman
		 * \return ����true ��ʾ���سɹ�������false��ʾ����ʧ��
		 */
		bool loadSchoolMemberFromDB();

		/**
		 * \brief �������ɵ���Ϣ�����ݿ�
		 * \author fqnewman
		 * \return ����true ��ʾ���سɹ�������false��ʾ����ʧ��
		 */
		bool updateSchoolInDB();

		/**
		 * \brief ����һ���ڵ㣬���ݿ��ʼ����ʱ����ñ�������ͨ�����ñ�������ʱ��ڵ����ݶ�������ģ��ڵ����к���С����
		 * \param info �ڵ���Ϣ
		 * \author fqnewman
		 */
		bool addNode(const stSchoolMemberInfo &info);

		/**
		 * \brief �����������School��Ľ�ɫ
		 * \param ptNullCmd ��Ϣ�ṹ
		 * \param cmdLen ��Ϣ����
		 * \author fqnewman
		 */
		void sendCmdToSchool(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);

		/**
		 * \brief �������ɼ�¼�����ݿ���
		 * \author fqnewman
		 * \return true ����ɹ� false ����ʧ��
		 */
		bool insertSchoolToDB();

		/**
		 * \brief �����ڵ�����������ݿ���ɾ��
		 * \author fqnewman
		 * \return true ɾ���ɹ� false ɾ��ʧ��
		 */
		bool deleteSchoolFromDB();

		/**
		 * \brief userName������һ���µİ��pSchool�������е�����userName��ͽ��ͽ��鵽��School������pSchool�У��������ֻ���������ɹ�����
		 * \param userName �µ�ʦ�������
		 * \param pSchool �µĹ���������ָ��
		 * \author fqnewman
		 * \return true �ɹ� false ʧ��
		 */
		bool moveMemberToNewSchool(const char *userName, CSchool *pSchool);


		/**
		 * \brief �ж�һ����ɫ�ǲ��Ǹ�ʦ�ų�Ա
		 * 
		 * \param memberName �������
		 * \author zhongjunwei
		 *
		 * \return true ��ʦ�ų�Ա��FALSE����ʦ�ų�Ա
		 */
		bool	isMember(const char* memberName);
		
		/**
		 * \brief ������ʦ�ų�Ա����ת��������������
		 *
		 * \param ptEnterWarCmd ��ת��������
		 * \param cmdLen �����
		 * \author zhongjunwei
		 *
		 */
		void sendCmdToAllMemberScene(Cmd::Session::t_enterWar_SceneSession* ptEnterWarCmd, const unsigned int cmdLen);

		/**
		 * \brief ֱ�����һ���ڵ㣬�ýڵ��Ѿ�������������֮����
		 * \param member ����ӵĽڵ�
		 * \author fqnewman
		 */
		void directAdddMember(CSchoolMember *member);

		/**
		 * \brief ֱ��ɾ��һ���ڵ㣬��������������ɾ���ýڵ�
		 * \param member ����ӵĽڵ�
		 * \author fqnewman
		 */
		void directRemoveMember(CSchoolMember *member);

		/**
		 * \brief ֱ��ɾ��һ���ڵ㣬��������������ɾ���ýڵ�
		 * \author fqnewman
		 * \return �����������ɵĽڵ�����쳣����NULL
		 */
		CSchoolMember * getMasterNode();

		/**
		 * \brief ���ù���
		 * \param buf ��������
		 * \author fqnewman
		 */
		void setBulletin(const char *buf);

		/**
		 * \brief ��ù���
		 * \author fqnewman
		 * \return �������ݻ���NULL
		 */
		const char * getBulletin();

		/**
		 * \brief ���ָ���ĳ�Ա
		 * \param pName ָ��������
		 * \author fqnewman
		 * \return �ҵ��ĳ�Ա�������NULL
		 */
		CSchoolMember *getMember(const char *pName);

		/**
		 * \brief ����ָ����Ա�Ĺ�ϵ�Ͼ���������������ȫ�˳�����ʦ�����ǶϾ���ʦ���Ĺ�ϵ
		 * \param member Ҫ�˳���Ա�Ľڵ����
		 * \param deleteTeacher�Ƿ�ɾ��ʦ���ڵ㣬�˱�־���ڷ�ֹ�ں�����ɾ�����ö��󣬴˱�־�������в������ã���˼�ǲ��������ɳ�Ա�����е��ô˺���
		 * \author fqnewman
		 * \return true Ϊ�ɹ� falseΪʧ��
		 */
		bool processLeaveSchool(CSchoolMember * member, bool deleteTeacher = true);

		/**
		 * \brief �������ɽ�ɢ���г�Ա��ϵת�Ƶ�ʦ�Ź���������
		 * \author fqnewman
		 * \return true Ϊ�ɹ� falseΪʧ��
		 */
		bool moveMemberToTeacherGroup();

		void notifyWarResult(const char* msg,...);

		DWORD getID(){return id;}
};

class CSchoolMember : public zEntry
{
private:

		/**
		 * \brief ������Ч��־��һ���ж��������Ƿ񻹴������ݿ���
		 * \author fqnewman
		 */
		bool destroy;

		/**
		 * \brief ��ǰ��ɫ��UserSession����
		 * \author fqnewman
		 */
		DWORD dwCharID;

		/**
		 * \brief ��ǰ��ɫ��ְҵ
		 * \author fqnewman
		 */
		DWORD wdOccupation;

		/**
		 * \brief ��ǰ��ɫ��UserSession����
		 * \author fqnewman
		 */
		UserSession		*user;

		/**
		 * \brief ʦ���ڵ�
		 * \author fqnewman
		 */
		CSchoolMember	*preLevelNode;

		/**
		 * \brief ����
		 * \author fqnewman
		 */
		struct ltword
		{
			bool operator()(const DWORD s1, const DWORD s2) const
			{
				return s1<s2;
			}
		};
		/**
		 * \brief ͽ���б�
		 * \author fqnewman
		 */
		std::map<DWORD, CSchoolMember*,ltword> prenticeList;

		/// ���Ͷ���
		typedef std::map<DWORD, CSchoolMember*,ltword>::value_type prenticeListValueType;

		/**
		 * \brief ���ڵ����ɹ������������ɵĹ��Ϊһ�����ɣ�
		 * \author fqnewman
		 */
		CSchool			*school;

		/**
		 * \brief ʦ���ڵ��ID
		 * \author fqnewman
		 */
		DWORD dwMasterID;

		/**
		 * \brief ǰһ���ڵ�����к�
		 * \author fqnewman
		 */
		DWORD dwPreSerialID;

		/**
		 * \brief ��ɫ��ǰ����
		 * \author fqnewman
		 */
		DWORD wdLevel;

		/**
		 * \brief ��ʦʱ��
		 * \author fqnewman
		 */
		DWORD dwJoinTime;

		/**
		 * \brief ��ʦ������Ѻö�
		 * \author fqnewman
		 */
		WORD  wdDegree;

		/**
		 * \brief ������ʱ��
		 * \author fqnewman
		 */
		DWORD dwLastTime;

		/**
		 * \brief ��������ID���������Ϊ0
		 * \author fqnewman
		 */
		DWORD dwSchoolID;

		/**
		 * \brief �ڵ���Ч��ǣ�1Ϊ��Ч��0Ϊ��Ч����Ч�Ľڵ�ֻ����������֮������������֮��
		 * \author fqnewman
		 */
		BYTE  byTag;
		
	
		/**
		 * \brief ���ڵ�Ķ�д��
		 * \author fqnewman
		 */
		zRWLock rwlock;

public:
		/**
		 * \brief �����ڵ����Ϣ�������ݿ⣬�ڵ������ʱ����
		 * \author fqnewman
		 * \return trueΪ�ɹ���false Ϊʧ��
		 */
		bool insertRecord(); // ���Լ��������ݿ�

		/**
		 * \brief �������ݿ��еĽڵ���Ϣ
		 * \param locked �Ƿ��Ѿ����� Ĭ��Ϊ��
		 * \author fqnewman
		 * \return trueΪ�ɹ���false Ϊʧ��
		 */
		bool updateRecord(bool locked = false); // ���Լ��������ݿ�

		/**
		 * \brief ���Լ������ݿ��¼ɾ��
		 * \author fqnewman
		 * \return trueΪ�ɹ���false Ϊʧ��
		 */
		bool deleteRecord();

		/**
		 * \brief ���캯�����Խڵ���в��ֳ�ʼ��
		 * \param pSchool ���ڵ����ڵĹ���������
		 * \param pUser �ڵ��ӵ����
		 * \author fqnewman
		 */
		CSchoolMember(CSchool *pSchool, UserSession *pUser);

		/**
		 * \brief ��������
		 * \author fqnewman
		 */
		~CSchoolMember();

		/**
		 * \brief ��ȡ�ڵ��Ӧ�Ľ�ɫID
		 * \author fqnewman
		 * \return ��ɫID
		 */
		DWORD getCharID();

		/**
		 * \brief ��ȡ�ڵ����к�
		 * \author fqnewman
		 * \return �ڵ����к�
		 */
		DWORD getSerialID();

		/**
		 * \brief ��ȡ�ڵ��������ɵ�ID
		 * \author fqnewman
		 * \return ����ID
		 */
		DWORD getSchoolID();

		/**
		 * \brief ��ȡ�Ѻö�
		 * \author fqnewman
		 * \return �Ѻö�
		 */
		WORD getDegree();

		/**
		 * \brief �����Ѻö�
		 * \param degree �Ѻö�
		 * \author fqnewman
		 */
		void setDegree(const WORD &degree);

		/**
		 * \brief ����������ʱ��
		 * \param lasttime ������ʱ��
		 * \author fqnewman
		 */
		void setLastTime(const DWORD &lasttime);

		/**
		 * \brief ��ȡ���ڵ����������
		 * \author fqnewman
		 * \return ��������
		 */
		char *getSchoolName();

		/**
		 * \brief ��õ�ǰ�ڵ��ͽ�ܼ�������Ч�ڵ�����
		 * \author fqnewman
		 * \return ��ǰ�ڵ��ͽ�ܼ���
		 */
		BYTE getPrenticeCount();

		/**
		 * \brief ��ð�ʦʱ��
		 * \author fqnewman
		 * \return ��ʦʱ��
		 */
		DWORD getJoinTime();

		/**
		 * \brief ���������ʱ��
		 * \author fqnewman
		 * \return ������ʱ��
		 */
		DWORD getLastTime();

		/**
		 * \brief ��ý�ɫ����
		 * \author fqnewman
		 * \return ��ɫ����
		 */
		WORD getLevel();
		
		/**
		 * \brief Ϊ���ڵ����һ��ͽ��
		 * \param pUser ͽ�ܵ�UserSession����
		 * \author fqnewman
		 * \return ����NULL��ʾ���ͽ��ʧ�ܣ����򷵻�ͽ�ܵĽڵ����
		 */
		CSchoolMember *addPrentice(UserSession *pUser);

		/**
		 * \brief ���һ�������ʦ���ڵ㣬ֻ��������Щ��δ���չ�ͽ�ܵ��˵�һ������ͽ��ʱ�ڹ������г�ʼ���Լ�
		 * \author fqnewman
		 */
		void initRootMember();

		/**
		 * \brief ��ͽ�ܽڵ���г�ʼ��
		 * \param master ʦ���ڵ�Ľڵ����
		 * \author fqnewman
		 */
		void initGeneralMember(CSchoolMember * master);

		/**
		 * \brief ����Ч�ڵ��ʼ������Ч�ڵ�
		 * \param master ʦ���ڵ�Ľڵ����
		 * \author fqnewman
		 */
		void initInValidNode(CSchoolMember * master);
		/**
		 * \brief �жϽڵ��Ƿ񻹴��ڹ������ϼ��ڵ�����¼��ڵ�
		 * \author fqnewman
		 * \return trueΪ���ڵ��Ѿ��������ڵ�Ͼ�һ�й�ϵ��false Ϊ���ڵ��������ڵ㻹���ڹ�ϵ
		 */
		bool isClean();

		/**
		 * \brief ��ñ��ڵ����ڵĹ���������
		 * \author fqnewman
		 * \return �ɹ����ع���������ʧ�ܷ���NULL
		 */
		CSchool * getSchool();

		/**
		 * \brief ���Լ���������ɾ��������ɾ����Ӧ�����ݿ��¼
		 * \author fqnewman
		 */
		void deleteMe();

		/**
		 * \brief ���ߴ���
		 * \author fqnewman
		 */
		void online(UserSession *pUser);

		/**
		 * \brief ���ߴ���
		 * \author fqnewman
		 */
		void offline();


		/**
		 * \brief �ҵ���λ��member֮ǰ��ʦ��
		 * \param member �����Ľڵ�
		 * \param name ʦ�ֵ�����
		 * \author fqnewman
		 */
		void getMyBigBrother(CSchoolMember *member, char *name);

		/**
		 * \brief ֪ͨ�ܱ��Ա�ҵļ��벢���ͳ�ʼ����Ϣ���Լ�
		 * \author fqnewman
		 */
		void notifyNewMemberAdd();

		/**
		 * \brief ���͸��˵�ʦͽ������Ϣ���ͻ��˽��г�ʼ��
		 * \param callback ������Ϣ�Ļص����󣬿������������Ȼ��������Ϣ�ʺ϶����ɶ����˹㲥������ϢĬ��Ϊ��
		 * \author fqnewman
		 */
		void sendInfomationToMe(CSendSchoolCallback *callback = NULL);

		/**
		 * \brief ͽ��������ʦ�����ֵܵ���Ϣ
		 * \param count ���������ۼƵ�ǰ�Ѿ��ж�����Ϣ������buf
		 * \param point ��Ϣ���buf��ָ��
		 * \param me ������Ϣ�Ľ�ɫ�Ľڵ����
		 * \author fqnewman
		 * \return trueΪ�ɾ��ģ�false Ϊ�Ѿ���ʦ��ͽ�ܹ�ϵ
		 */
		void prenticeRequestMemberInfo(unsigned int &count, Cmd::stTeacherMember *point, CSchoolMember *me);

		/**
		 * \brief �������ݿ��¼��ʼ���ڵ�
		 * \param info ���ݿ��¼��Ϣ�ṹ
		 * \author fqnewman
		 */
		void initByDBRecord(const stSchoolMemberInfo &info);

		/**
		 * \brief �жϽڵ��Ƿ����ʦ��,����info���������ж�
		 * \author fqnewman
		 * \return trueΪ��ʦ����false Ϊû��ʦ��
		 */
		bool haveTeacher();

		/**
		 * \brief �жϽڵ��Ƿ����ǰһ�ڵ�,��������һ���˿���û��ʦ�����ǻ���ǰһ�ڵ�
		 * \author fqnewman
		 * \return trueΪ�нڵ㣬false ��ǰ�ڵ�Ϊ���ڵ㣨ʦ��
		 */
		bool havePreNode();

		/**
		 * \brief ���һ����һ���ڵ㣬������֪ͨ�������������ݿ��ʼ����ʱ��
		 * \param member ��һ���ڵ�
		 * \author fqnewman
		 */
		bool addNextLevelNode(CSchoolMember *member);

		/**
		 * \brief ���ñ��ڵ���ϲ�ڵ�
		 * \param pPreNode �ϲ�ڵ�
		 * \author fqnewman
		 */
		void setPreLevelNode(CSchoolMember *pPreNode);

		/**
		 * \brief ����ָ�����ֵ�ͽ��
		 * \param PrenticeName �������˵�����
		 * \author fqnewman
		 * \return true Ϊ�����ɹ��� falseΪû�д���
		 */
		bool firePrentice(const char *PrenticeName);

		/**
		  * \brief ��������ͽ��
		  *
		  * \author zhongjunwei
		  */
		bool fireAllPrentice();

		/**
		 * \brief �˶Ա��ڵ��ǲ������ƶ�Ӧ�Ľڵ�
		 * \param pName ָ��������
		 * \author fqnewman
		 * \return true ����ڵ����Ҫ�ҵĽڵ㣬 false �ڵ㲻���������
		 */
		bool isMe(const char *pName);

		/**
		 * \brief �ڵ��Ӧ��ɫ�Ƿ�����
		 * \author fqnewman
		 * \return true ��ɫ���ߣ� false ��ɫ������
		 */
		bool isOnline();

		/**
		 * \brief �жϽڵ��Ƿ�����Ч�ģ��ж�����Ϊtag�Ƿ�Ϊ1
		 * \author fqnewman
		 * \return true ��Ч�ģ� false ��Ч��
		 */
		bool isValid();

		/**
		 * \brief ��ýڵ������еĶ�
		 * \param layer ��
		 * \author fqnewman
		 */
		void getMyLayer(DWORD &layer);

		/**
		 * \brief ֪ͨ�ýڵ㼰������ͽ��,�����Լ���ʦ���б�
		 * \author fqnewman
		 */
		void notifyTeacherGroup();

		/**
		 * \brief ��������ڵ��Ӧ�Ľ�ɫ
		 * \param ptNullCmd ��Ϣ�ṹ
		 * \param cmdLen ��Ϣ����
		 * \author fqnewman
		 */
       void sendCmdToMe(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);

		/**
		 * \brief ����������Ϣ���ڵ��Ӧ�Ķ���
		 * \param type ϵͳ��Ϣ������ 
		 * \param pattern ��Ϣ
		 * \param ... ����
		 * \author fqnewman
		 */
		void sendSysChat(int type, const char *pattern, ...);

		/**
		 * \brief ���ʦͽ��ϵ��preLevelNodeָ���ÿգ�ʦ����Ϣ��0
		 * \author fqnewman
		 */
		void clearTeacherRelation();

		/**
		 * \brief �����˵�ͽ��ͽ�����Ŀ�ͼ����Ƿ�ﵽ�������ɵ�����
		 * \author fqnewman
		 * \return true Ϊ�������� false Ϊ����������
		 */
		bool checkSchoolCreateCondition();

		/**
		 * \brief ���ڵ��Լ���ͽ�ܵ������Ƿ�ﵽʦ���������ɵ�Ҫ��
		 * \author fqnewman
		 * \return true Ϊ�������� false Ϊ����������
		 */
		bool checkMeAndPrenticeNumberAndLevel();

		/**
		 * \brief ���ڵ��Լ��������е�ͽ�ܶ�Ų��������pSchool֮��
		 * \param pSchool �ڵ��ƶ���Ŀ�������
		 * \param memberSet ��Ա����ص������������Ҫ�Ա��ƶ���Ա���д����������ΪNULL��Ĭ��ΪNULL
		 * \author fqnewman
		 */
		void moveAllToSchool(CSchool *pSchool, CSchoolMemberListCallback *memberSet = NULL);

		/**
		 * \brief ֪ͨ����ϵ���еĽڵ����·��ͳ�ʼ����Ϣ���ͻ���
		 * \param callback ������Ϣ�ص����������������ɵ���֯�ṹ��ʼ����Ϣ���ͻ���
		 * \author fqnewman
		 */
		void notifyAllReSendInitData(CSendSchoolCallback *callback);

		/**
		 * \brief ������ɵ����г�Ա�б�����ģ�
		 * \param callback �ص���
		 * \author fqnewman
		 */
		void getSchoolTree(CSendSchoolCallback &callback);

		/**
		 * \brief ������ɵ����г�Ա�б�����ģ�
		 * \param level ��ǰ��
		 * \param tgLevel �˴ε��õ�Ŀ���
		 * \param condition ���ױ�־������Ѿ��������ĵײ���ô�ͷ��� false
		 * \param callback �ص�����
		 * \param tag ���л����־ �μ�enum SCHOOL_LAYER
		 * \author fqnewman
		 */
		void schoolTreeCallback(DWORD level, DWORD tgLevel, bool &condition, CSendSchoolCallback &callback, BYTE &tag);

		/**
		 * \brief ���������ݷ��͸��ڵ��Ӧ�Ľ�ɫ
		 * \author fqnewman
		 */
		void sendBulletinToMe();

		/**
		 * \brief ���ñ����ɵĹ������ݣ���Ȼ�ڵ������ʦ�����
		 * \param buf ��������
		 * \author fqnewman
		 */
		void setBulletin(const char *buf);

		/**
		 * \brief ��ڵ㼰��ͽ�ܷ�����Ϣ
		 * \param ptNullCmd ��Ϣ�ṹ
		 * \param cmdLen ��Ϣ����
		 * \param exceptMe �Ƿ�������ڵ㣬Ĭ��Ϊ����
		 * \author fqnewman
		 */
		void sendCmdToTeacherGroup(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen, bool exceptMe = false);

		/**
		 * \brief ������֪ͨ�ҵ�����״̬
		 * \param onlineStatus �ڵ��Ӧ��ɫ������״̬
		 * \author fqnewman
		 */
		void sendOnlineStatusMessage(BYTE onlineStatus);

		/**
		 * \brief ����ϼ��ڵ����ָ��
		 * \author fqnewman
		 * \return �ϼ��ڵ�Ķ����п���ΪNULL
		 */
		CSchoolMember * getPreLevelNode();

		/**
		 * \brief ���ʦ���ڵ㣬�ϼ��ڵ㲻һ����ʦ���ڵ㣬����֮����ܲ������ڹ�ϵ���Ի��ʦ���ڵ㲻��ʹ��getPreLevelNode
		 * \author fqnewman
		 * \return �����ֱ��ʦ������ʦ���ڵ�Ķ��󣬷���ΪNULL
		 */
		CSchoolMember * getTeacher();

		/**
		 * \brief ���ڵ����ó���Ч��.
		 * \author fqnewman
		 */
		void setInValid();

		/**
		 * \brief �Ͼ���ʦ��֮��Ĺ�ϵ
		 * \author fqnewman
		 */
		void clearMaster();

		/**
		 * \brief �����˳����ɵ���Ϣ���ͻ���
		 * \author fqnewman
		 */
		void sendDestroyNotifyToMe();

		/**
		 * \brief ֱ���ڱ��ڵ���²�ڵ��б���ɾ��ָ���Ľڵ��ϵ
		 * \param member ��ϵ��Ҫɾ���Ľڵ����
		 * \author fqnewman
		 */
		void directRemovePrentice(const CSchoolMember *member);

		/**
		 * \brief ֪ͨ�������ɵĿͻ���ɾ�����ڵ��Ӧ�Ľ�ɫ
		 * \author fqnewman
		 */
		void notifySchoolMemberRemove();

		/**
		 * \brief ��õ�һ����Ч���¼��ڵ����
		 * \author fqnewman
		 * \return �¼���Ч�ڵ�������û�к��ʵĶ�����ܻ᷵��NULL
		 */
		CSchoolMember* getFirstInValideNode();

		/**
		 * \brief ������Ч�ڵ������UserSession����
		 * \param pUser ��ɫ��Ӧ��UserSession����
		 * \author fqnewman
		 */
		void setUser(UserSession *pUser);

		/**
		 * \brief ��ñ��ڵ��ֱ���¼��ڵ�������������Ч��Ч
		 * \author fqnewman
		 * \return ֱ���¼��ڵ���
		 */
		BYTE getNextLevelNodeCount();

		/**
		 * \brief ������ڵ���ǰһ�ڵ�Ĺ�ϵ���ԣ�����ص�ָ��������ÿջ���0
		 * \author fqnewman
		 */
		void clearPreLevelNode();

		/**
		 * \brief ������ڵ����һ���ڵ�Ĺ�ϵ���ԣ�����һ���ڵ���ص�ָ��������ÿջ���0�����󼶽ڵ��б����
		 * \author fqnewman
		 */
		void clearAllNextLevelRelation();

		/**
		 * \brief �����϶��ѺöȽ��б���
		 * \param rev �ѺöȽ�����Ϣ��
		 * \author fqnewman
		 */
		void setFriendDegree(const Cmd::Session::t_CountFriendDegree_SceneSession *rev);

		/**
		 * \brief ת��������Ϣ
		 * \param rev ������Ϣ�ṹ��
		 * \param cmdLen ��Ϣ����
		 * \author fqnewman
		 */
		void sendChatMessages(const Cmd::stChannelChatUserCmd *rev, const unsigned int cmdLen);

		/**
		 * \brief ת��������Ϣ
		 * \author fqnewman
		 * \return ��ȡ��ɫ��Ӧ��UserSession ���������ɫ�����߻᷵��NULL
		 */
		UserSession *getUser();

		/**
		 * \brief ���ü���
		 * \param level �µĽ�ɫ�ȼ�
		 * \author fqnewman
		 */
		void setLevel(const WORD level);

		/**
		 * \brief ��ȡ��ɫ��ְҵ
		 * \author fqnewman
		 * \param ְҵ
		 */
		WORD getOccupation();

		/**
		 * \brief ��ѯ��ǰ���Խ����Ľ��
		 * \author zjw
		 * \param ���
		 */
		DWORD queryBounty();

		/**
		 * \brief ����
		 * \author zjw
		 */
		bool putBounty();

		/**
		 * \brief ��������ϵ����֪ͨ������
		 * \author fqnewman
		 */
		void sendNotifyToScene();

		/**
		 * \brief ��Ϊʦ���ۼ��յ��Ľ���,��λ:��
		 * \author zjw
		 */
		DWORD  master_total;
		
		/**
		 * \brief ��Ϊʦ����ǰ������ȡ�Ľ������,��λ:��
		 * \author zjw
		 */
		DWORD  master_balance;
		
		/**
		 * \brief ��Ϊͽ���ۼƽ����Ľ��,��λ:��
		 * \author zjw
		 */
		DWORD  prentice_total;

		/**
		 * \brief ��Ϊͽ���ϴν���ʱ�ĵȼ�
		 * \author zjw
		 */
		DWORD  prentice_lastlevel;

};

#endif
