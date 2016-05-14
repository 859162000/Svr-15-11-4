/**
 * \file CDare.h
 * \version  $Id: CDare.h  $
 * \author  
 * \date 
 * \brief ��������࣬���������
 *
 */


#ifndef _CLASS_CDARE_H_
#define _CLASS_CDARE_H_
#include <vector>
#include "zMutex.h"
#include "zMisc.h"
#include "zType.h"
#include "zEntry.h"
#include "zEntryManager.h"
#include "Command.h"
#include "SessionCommand.h"
#include "zUser.h"
#include "zUniqueID.h"
#include "zTime.h"
#include "zSingleton.h"

class UserSession;
class zRWLock;
class CDare;
class CSchool;
class CSept;
class CUnion;

extern unsigned int dare_active_time;
extern unsigned int dare_ready_time;
extern int dare_need_gold;
extern int dare_winner_gold;

extern int dare_need_gold_sept;
extern int dare_winner_gold_sept;

typedef zUniqueID<DWORD> zUniqueDWORDID;

/**
 * \brief ��ս��¼������
 *
 * �����ս��¼������,ʹ��Singletonģʽ
 */
class CDareM:public zEntryManager<zEntryTempID>, 
	     public Singleton<CDareM>
{
	friend class SingletonFactory<CDareM>;

	private:
		zUniqueDWORDID *channelUniqeID;
		zRWLock rwlock;

		CDareM();

	public:
		~CDareM();

		bool getUniqeID(DWORD &tempid);
		void putUniqeID(const DWORD &tempid);

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

		bool init();
		static void destroyMe();

		/**
		 * \brief ��ս�������Ķ�ʱ���ص�����
		 *
		 * ��ʱ�������ж�ս��¼��ɾ����Ч��¼�������ﵽʱ��Ķ�ս����ش���
		 */
		void timer();


		/**
		 * \brief �����û��Ķ�ս����
		 *
		 * \param pUser ���͸������������Ӧ��UserSession����
		 * \param ptNullCmd �յ�������
		 * \param cmdLen �����
		 * \return �Ƕ�ս������õ���Ӧ��������Ϊtrue, ����Ϊfalse
		 *
		 */
		bool processUserMessage(UserSession *pUser, const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);

		
		/**
		 * \brief �����û��ļ�������ս��ս����
		 *
		 * \param pUser ���͸������������Ӧ��UserSession����
		 * \param ptNullCmd �յ�������
		 * \param cmdLen �����
		 * \return �Ƕ�ս������õ���Ӧ��������Ϊtrue, ����Ϊfalse
		 *
		 */
		bool processSeptDare(UserSession *pUser, Cmd::stActiveDareCmd *pCmd, const unsigned int cmdLen);

		/**
		 * \brief �����û��İ������ս��ս����
		 *
		 * \param pUser ���͸������������Ӧ��UserSession����
		 * \param ptNullCmd �յ�������
		 * \param cmdLen �����
		 * \return �Ƕ�ս������õ���Ӧ��������Ϊtrue, ����Ϊfalse
		 *
		 */
		bool processUnionDare(UserSession *pUser, Cmd::stActiveDareCmd *pCmd, const unsigned int cmdLen);


		/**
		 * \brief ���������͹����ķ���������Ϣ
		 *
		 * \param cmd �������������͹���������ָ��
		 * \param cmdLen �����
		 *
		 * \return ���Ѷ���ķ�������������õ���Ӧ��������true, ����Ϊfalse.
		 *
		 */
		bool processSceneMessage(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen);

		/**
		 * \brief ���ҷ��������Ķ�ս��¼
		 *
		 *  ��ID2Ϊ0ʱ�������Ƿ���ID1��ս�ļ�¼��dwType�����������롣
		 *
		 * \param dwType ��ս����:Cmd:UNION_DARE, Cmd:SCHOOL_DARE, Cmd:SEPT_DARE
		 * \param dwID1  ����ID
		 * \param dwID2  �ط�ID
		 *
		 * \return �ҵ��򷵻ض�Ӧ��ս��¼��ָ��
		 */
		CDare * findDareRecord(DWORD dwType, DWORD dwID1, DWORD dwID2);
		CDare*  findDareRecordByFull(DWORD dwType, DWORD dwDefID, std::vector<DWORD>& vDareList);

		/**
		 * \brief ���Ҹ�����ϵָ���ĵĶ�ս��¼,���ܸ�����ϵ���ڹ��������ط�
		 *
		 * \param dwType ��ս����:Cmd:UNION_DARE, Cmd:SCHOOL_DARE, Cmd:SEPT_DARE
		 * \param dwID  ����ϵID
		 *
		 * \return �ҵ��򷵻ض�Ӧ��ս��¼��ָ��
		 */
		CDare*  findDareRecordByID(DWORD dwType, DWORD dwID);
		
		/**
		 * \brief ���ҷ��������Ķ�ս��¼
		 *
		 *  r1,r2û��˳�����ƣ���������һ��r����ս��������
		 *  ��r2ΪNULLʱ�������Ƿ���r1��ս�ļ�¼��dwType�����������롣
		 *
		 * \param dwType ��ս����:Cmd:UNION_DARE, Cmd:SCHOOL_DARE, Cmd:SEPT_DARE
		 * \param r1  ����ϵ����1
		 * \param r2  ����ϵ����2
		 *
		 * \return �ҵ��򷵻ض�Ӧ��ս��¼��ָ��
		 */

		CDare * findDareRecordByRelationName(DWORD dwType, const char* r1, const char* r2);

		/**
		 * \brief ���ҷ��������Ķ�ս��¼
		 *
		 *  ID1,ID2û��˳�����ƣ���������һ��ID����ս����ID��
		 *  ��ID2Ϊ0ʱ�������Ƿ���ID1��ս�ļ�¼��dwType�����������롣
		 *
		 * \param dwType ��ս����:Cmd:UNION_DARE, Cmd:SCHOOL_DARE, Cmd:SEPT_DARE
		 * \param dwUserID1  ���ID1
		 * \param dwUserID2  ���ID2
		 *
		 * \return �ҵ��򷵻ض�Ӧ��ս��¼��ָ��
		 */
		CDare*  findDareRecordByUser(DWORD dwType, DWORD dwUserID1, DWORD dwUserID2);


		/**
		 * \brief �����µĶ�ս��¼
		 *
		 * \param pCmd ��ս��������
		 * \param dwType ��ս����
		 * \param dwID1 ��ս������ϵID
		 * \param dwID2 Ӧս������ϵID
		 * \param dwUserID ��ս��ID
		 *
		 * \return û���ظ���¼��������ɹ�������true,���򷵻�false
		 *
		 */
		bool addNewDareRecord(Cmd::stActiveDareCmd *pCmd, DWORD dwType, DWORD dwID1, DWORD dwID2, DWORD dwUserID);

		/**
		  * \brief �û����ߴ���
		  *
		  * �û�����ʱ��ͬʱ�жϸ��û��Ķ�ս״̬��������Ӧ����
		  *
		  * \param pUser �����û�
		  *
		  */
		void userOnline(UserSession* pUser);

		void userOnlineCountry(UserSession* pUser);

		/**
		 * \brief �������ظ���t_activeDare����
		 *
		 * \param cmd ��ս��¼
		 */
		void activeDare_sceneSession(Cmd::Session::t_activeDare_SceneSession* cmd);

		/**
		 * \brief ��������������ս��ʱ��PKս������
		 *
		 * \param cmd ����ָ��
		 */
		void darePk_sceneSession(Cmd::Session::t_darePk_SceneSession* cmd);
		
		/**
		 * \brief ���������������½���ս������
		 *
		 * \param pCmd ����ָ��
		 */

		void createDare_sceneSession(Cmd::Session::t_createDare_SceneSession* pCmd, std::vector<DWORD>& dare_list);
		

		void createUnionCityDare_sceneSession(Cmd::Session::t_UnionCity_Dare_SceneSession* pCmd);
};

class CDare:public zEntry
{
	protected:
		zRWLock rwlock;                             // ��д��

		/**
		 * \brief ���㲢���ͽ���
		 *
		 *  �ú���Ŀǰֻ��setWaitBountyState��������
		 *
		 * \return ���ͽ���ɹ�������TRUE, ���򷵻�ʧ�ܡ�
		 */
		virtual bool computeResult();

		virtual char* getFirstName(){return NULL;}    	// ��ս������ϵ������
		virtual DWORD getSecondUserID(){return 0;}  	// Ӧս����ҵ�ID

		/// ��ս����ʱ��
		DWORD active_time;

		/// ��սǰ�ĵȴ�ʱ��
		DWORD ready_time;

		// ��ս��ʼʱ��
		time_t	start_time;

	public:
		enum{
			DARE_READY,		// ����ȴ�״̬
			DARE_READY_QUESTION,    // �ȴ�����ս�߻�Ӧ״̬      
			DARE_READY_ACTIVE,      // �ȴ��۳�Ӧս�߽�ң����ʧ�ܣ������DARE_RETURN_GOLD
			DARE_RETURN_GOLD,       // �����Ҳ��������Ԥ�ȿ۳�����ս���Ľ�Ǯ����������������DARE_OVER״̬��
			DARE_ACTIVE,		// ��ս״̬
			DARE_DATAPROCESS,	// ���ݴ���״̬,��ʱδ��
			DARE_READY_OVER,        // ������ս��֪ͨ����ȡ�������˵Ķ�ս״̬����������Ӯ,
			DARE_WAIT_BOUNTY,       // ���Ӯ�����������ߣ��������״̬����������ߺ󣬰ѽ������Ÿ���
			DARE_OVER		// �������״̬
		};

		CDare(DWORD active_time, DWORD ready_time); 
				
		virtual ~CDare();

		/**
		 * \brief ʱ���¼�����ص���������TimeTick.cpp�к����ص�
		 *
		 *  ����ս����DARE_READY״̬����ӣ���ȡ����ս��������DARE_RETURN_GOLD״̬
		 *  ����ս����DARE_READY_QUESTION  ����ս����DARE_READY
		 *
		 *
		 */
		virtual void timer();	//ʱ���¼�����ص�


		// ����Ϊ���ö�ս״̬�Ĵ���������ս״̬����鿴��սϵͳ״̬ת��ͼ
		// ״̬ת������Ĭ��������Բ���Ҫ���ء�ֻ������ָ���ļ���������
		// �������ʵ��һ����Ĭ����Ϊ��ͬ��״̬ת�����̡�������Ҫ��������״̬ת������
		
		virtual void setReadyState();
		virtual void setReadyQuestionState();
		virtual void setReadyActiveState(UserSession* pUser);
		virtual void setReturnGoldState();
		virtual void setActiveState();
		virtual void setReadyOverState();
		virtual void setWaitBountyState();
		virtual void setOverState();
		virtual void sendAntiAtt(){};

		virtual void setSecondID(DWORD dwID)=0;
		virtual void addFirstID(DWORD dwID)=0;
		virtual bool isInvalid() {return false; }
		
		// ������ϵ�Ƿ����ڸö�ս��¼�Ĺ���
		virtual bool isAtt(DWORD dwID);
		
		virtual void notifyWarResult(int winner_type) = 0;

		/**
		 * \brief ���Ͷ�ս����״̬������
		 *
		 *  ��������Ӧ�����Ӷ�ս��¼����sendNineToMeʱ�����ж�ս״̬�ļ���,��
		 *  ����ֻ���û����ߴ���ʱ�����á�
		 *
		 */
		virtual void sendActiveStateToScene(UserSession* pUser);


		/**
		  * \brief ������������в���ö�ս�����
		  *
		  * ���Ͷ�ս�������������setActiveState��setReadyOverState��������
		  *
		  */
		virtual void 
		sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, 
				unsigned int cmdLen, unsigned int relationID) = 0;

		/**
		  * \brief �ж��Ƿ����ڶ�սʱ��
		  *
		  * \return ������ڶ�սʱ�ڣ�����TRUE,���򷵻�FALSE
		  */
		virtual bool isActivePeriod()
		{
			time_t cur_time = time(NULL);
			return (cur_time-start_time)<this->active_time ? true : false;
		}

		/**
		  * \brief �ж��Ƿ�����׼����
		  *
		  * \return �������׼����ʱ�ڣ�����TRUE,���򷵻�FALSE
		  */
		virtual bool isReadyPeriod()
		{
			return this->count<this->ready_time ? true : false;
		}

		/**
		  * \brief ���¼�����
		  *
		  */
		virtual void updateTime()
		{
			rwlock.wrlock();
			this->count += 5;
			rwlock.unlock();
		}

		/**
		 * \brief ս���ӷ�
		 *
		 *  ����һ�Σ�����Ӧ������ŶӼ���PK����һ�ˡ��÷ֹ�ʽ���£�
		 *  grade = grade + user.level/10;
		 *
		 * \param pAtt ������
		 * \param pDef ���ط�
		 */
		virtual void addGrade(UserSession* pAtt, UserSession* pDef) = 0;

		void	printState();


		DWORD type;		// ս������:Cmd::UNION_DARE, Cmd::SCHOOL_DARE, Cmd::SEPT_DARE
		
		//DWORD firstID;		// ��ս������ϵID1
		std::vector<DWORD> attList;	// ��ս���б�
		DWORD secondID;		// ���ط�����ϵID2

		DWORD userid1;     	// ��ս��ID
		DWORD userid2;     	// Ӧս��ID
		DWORD grade1;           // ��ս����1�����÷���
		DWORD grade2;		// ��ս����2�����÷���
		DWORD pk1;              // ����ϵ1PK����
		DWORD pk2;              // ����ϵ2PK����
		DWORD count;		// ������λΪ��
		BYTE  state;       	// ����״̬
		DWORD dwWinnerID;	// ���յ�Ӯ��ID�����ڶ������ʱʹ��
		bool  isAntiAtt;	// ���÷�����־
};


class CDareUnion : public CDare
{
	public:
		CDareUnion();
		CDareUnion(DWORD active_time, DWORD ready_time);
		virtual ~CDareUnion();

	public:		
		// ����Ϊ���ö�ս״̬�Ĵ���������ս״̬����鿴��սϵͳ״̬ת��ͼ
		virtual void setSecondID(DWORD dwID);
		virtual void addFirstID(DWORD dwID);

		virtual void sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, unsigned int cmdLen, unsigned int relationID);
		virtual void sendActiveStateToScene(UserSession* pUser);
		virtual void notifyWarResult(int winner_type);

		/**
		 * \brief ս���ӷ�
		 *
		 *  ����һ�Σ�����Ӧ������ŶӼ���PK����һ�ˡ��÷ֹ�ʽ���£�
		 *  grade = grade + user.level/10;
		 *
		 * \param pAtt ������
		 * \param pDef ���ط�
		 */
		virtual void addGrade(UserSession* pAtt, UserSession* pDef);

		virtual void timer();
		virtual bool isInvalid();

	protected:		
		virtual char* getFirstName();
		virtual DWORD getSecondUserID();
};

class CDareSchool : public CDare
{
	public:
		CDareSchool();
		virtual ~CDareSchool();

	public:		
		// ����Ϊ���ö�ս״̬�Ĵ���������ս״̬����鿴��սϵͳ״̬ת��ͼ
		virtual void setSecondID(DWORD dwID);
		virtual void addFirstID(DWORD dwID);
		
		virtual void sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, unsigned int cmdLen, unsigned int relationID);
		virtual void sendActiveStateToScene(UserSession* pUser);
		virtual void notifyWarResult(int winner_type);

		/**
		 * \brief ս���ӷ�
		 *
		 *  ����һ�Σ�����Ӧ������ŶӼ���PK����һ�ˡ��÷ֹ�ʽ���£�
		 *  grade = grade + user.level/10;
		 *
		 * \param pAtt ������
		 * \param pDef ���ط�
		 */
		virtual void addGrade(UserSession* pAtt, UserSession* pDef);

	protected:		
		virtual char* getFirstName();
		virtual DWORD getSecondUserID();

//		CSchool* pFirst;
//		CSchool* pSecond;
};

class CDareUnionCity : public CDareUnion
{
	public:
		CDareUnionCity(DWORD active_time, DWORD ready_time);
		virtual ~CDareUnionCity();

	public:		
		virtual void setSecondID(DWORD dwID){CDareUnion::setSecondID(dwID);}
		virtual void addFirstID(DWORD dwID){CDareUnion::addFirstID(dwID);}
		
		virtual void sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, 
				unsigned int cmdLen, unsigned int relationID)
		{
			CDareUnion::sendCmdToAllDarePlayer(cmd, cmdLen, relationID);
		}
		
		virtual void sendActiveStateToScene(UserSession* pUser)
		{
			CDareUnion::sendActiveStateToScene(pUser);
		}
		
		virtual void setReadyState();
		virtual void setReadyOverState();

		/**
  		  * \brief ��ս���֪ͨ
		  *
		  * \param winner_type �������:0 ��ս��ʤ�� 1 Ӧս��ʤ�� 2 սƽ
		  *		  
		  */
 		virtual void notifyWarResult(int winner_type){};

		virtual void timer();	//ʱ���¼�����ص�

		/**
		 * \brief ս���ӷ�
		 *
		 *  ����һ�Σ�����Ӧ������ŶӼ���PK����һ�ˡ��÷ֹ�ʽ���£�
		 *  grade = grade + user.level/10;
		 *
		 * \param pAtt ������
		 * \param pDef ���ط�
		 */
		virtual void addGrade(UserSession* pAtt, UserSession* pDef){};
		int last_fulltime;
};

class CDareCountry : public CDare
{
	public:
		CDareCountry();
		CDareCountry(DWORD active_time, DWORD ready_time);
		virtual ~CDareCountry();

	public:		
		// ����Ϊ���ö�ս״̬�Ĵ���������ս״̬����鿴��սϵͳ״̬ת��ͼ
		virtual void setSecondID(DWORD dwID);
		virtual void addFirstID(DWORD dwID);
		virtual void sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, unsigned int cmdLen, unsigned int relationID);

		virtual bool isInvalid();

		virtual void setActiveState();
		virtual void setReadyOverState();
		virtual void notifyWarResult(int winner_type) {};
		virtual void addGrade(UserSession* pAtt, UserSession* pDef){};

		virtual void sendAntiAtt();
		virtual void sendActiveStateToScene(UserSession* pUser);
		
		virtual void timer();
};

class CDareAntiCountry : public CDareCountry
{
	public:
		CDareAntiCountry();
		CDareAntiCountry(DWORD active_time, DWORD ready_time);
		virtual ~CDareAntiCountry();

	public:		
		// ֻ���ؽ�������.
		virtual void setReadyOverState();
};

#endif

