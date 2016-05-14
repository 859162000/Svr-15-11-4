/**
 * \file CQuiz.h
 * \version  $Id: CQuiz.h  $
 * \author 
 * \date 
 * \brief ������������
 *
 */


#ifndef _CLASS_CQUIZ_H_
#define _CLASS_CQUIZ_H_

#include <vector>
#include <set>
#include "zMutex.h"
#include "zMisc.h"
#include "zType.h"
#include "zEntry.h"
#include "zEntryManager.h"
#include "Command.h"
#include "SessionCommand.h"
#include "zUser.h"
#include "zUniqueID.h"
#include "zSingleton.h"

class UserSession;
class zRWLock;
class CQuiz;
extern unsigned int quiz_active_time;
extern unsigned int quiz_ready_time;
typedef zUniqueID<DWORD> zUniqueDWORDID;

/**
 * \brief �����¼������
 *
 * ��������¼������,ʹ��Singletonģʽ
 */
class CQuizM : public zEntryManager<zEntryTempID>, 
	       public Singleton<CQuizM>
{
	friend class SingletonFactory<CQuizM>;

	private:
		zUniqueDWORDID *channelUniqeID;
		zRWLock rwlock;
		CQuizM();

	public:
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

		~CQuizM();
		bool init();
		static void destroyMe();

		/**
		 * \brief ��ս�������Ķ�ʱ���ص�����
		 *
		 * ��ʱ�������ж�ս��¼��ɾ����Ч��¼�������ﵽʱ��Ķ�ս����ش���
		 */
		void timer();
		void printSize();


					


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
		 * \brief �����µľ�����¼
		 *
		 * \param pCmd ����ָ��
		 * \return û���ظ���¼��������ɹ�������true,���򷵻�false
		 *
		 */
		bool addNewQuiz_sceneSession(Cmd::Session::t_createQuiz_SceneSession* pCmd);



		/**
		 * \brief ����ȫ��������¼
		 *
		 * \return ����ҵ������ض�Ӧָ�룬���û�У�����NULL
		 */
		CQuiz* findWorldQuiz();

		/**
		 * \brief ���Ҹ����ʴ��¼
		 *
		 * \return ����ҵ������ض�Ӧָ�룬���û�У�����NULL
		 */
		CQuiz* findPersonalQuiz(DWORD dwUserID);
		
		/**
		  * \brief �û����ߴ���
		  *
		  * �û�����ʱ��ͬʱ�жϸ��û��ľ���״̬��������Ӧ����
		  *
		  * \param pUser �����û�
		  *
		  */
		void userOnline(UserSession* pUser);
		
		/**
		  * \brief �û����ߴ���
		  *
		  * �û�����ʱ��ͬʱ�жϸ��û��ľ���״̬��������Ӧ����
		  *
		  * \param pUser �����û�
		  *
		  */
		void userOffline(UserSession* pUser);
};


/**
 * \brief ������
 *
 */
class CPothunter
{
	public:
		DWORD dwUserID;   // ������ID
		int dwScore;    // �÷�
		DWORD dwGrace;    // �����Ĳɵ÷�
		int dwLuck;	// ������ʹ�ô���

		int dwAnswerStatus; // ���δ���״̬
		
		CPothunter()
		{
			dwUserID = 0;
			dwScore = 0;
			dwGrace = 0;
			dwLuck = 0;

			dwAnswerStatus = -1;
		}
		/*bool operator ==(CPothunter& ref)
		{
			return ref.dwUserID==this->dwUserID;
		}*/
		
		~CPothunter(){}
};



class CSubject
{
	public:
		/*std::string title;
		std::string answer_a;
		std::string answer_b;
		std::string answer_c;
		std::string answer_d;
		*/
		char title[256];
		char answer_a[56];
		char answer_b[56];
		char answer_c[56];
		char answer_d[56];
		char answer_e[56];
		char answer_f[56];

		DWORD answer;
		int  quiz_type; // 0Ϊȫ��,1Ϊ����,2Ϊ������

		CSubject(const CSubject& ref)
		{
			strcpy(this->title, ref.title);
			strcpy(this->answer_a, ref.answer_a);
			strcpy(this->answer_b, ref.answer_b);
			strcpy(this->answer_c, ref.answer_c);
			strcpy(this->answer_d, ref.answer_d);
			strcpy(this->answer_e, ref.answer_e);
			strcpy(this->answer_f, ref.answer_f);

			this->answer = ref.answer;
			this->quiz_type = ref.quiz_type;
		}

		CSubject()
		{
			answer=0;
			bzero(title, sizeof(title));
			bzero(answer_a, sizeof(answer_a));
			bzero(answer_b, sizeof(answer_b));
			bzero(answer_c, sizeof(answer_c));
			bzero(answer_d, sizeof(answer_d));
			bzero(answer_e, sizeof(answer_e));
			bzero(answer_f, sizeof(answer_f));
			quiz_type = 0;
		}

		const CSubject & operator= (const CSubject &ref)
		{
			strcpy(this->title, ref.title);
			strcpy(this->answer_a, ref.answer_a);
			strcpy(this->answer_b, ref.answer_b);
			strcpy(this->answer_c, ref.answer_c);
			strcpy(this->answer_d, ref.answer_d);
			strcpy(this->answer_e, ref.answer_e);
			strcpy(this->answer_f, ref.answer_f);
			
			this->answer = ref.answer;
			this->quiz_type = ref.quiz_type;
			return *this;
		}

		~CSubject()
		{
#ifdef _ZJW_DEBUG			
			Zebra::logger->debug("~CSubject:%s", this->title);
#endif				
		}
		
};

class CSubjectM : public Singleton<CSubjectM>
{
	public:
		~CSubjectM();
		bool init();
		static void destroyMe();
		
		std::vector<CSubject> gsubjects;
		std::vector<CSubject> levels;
		std::vector<CSubject> personals;
		CSubjectM();
};

class CQuiz:public zEntry
{
	protected:
		zRWLock rwlock;                             // ��д��

		/// ����ǰ�ĵȴ�ʱ��
		DWORD ready_time;

		/// ready_question����ʱ��,Ĭ��10��
		DWORD ready_question_time;
		

		/// ������ǰ״̬
		BYTE state;

		/// ��������
		BYTE type;
		
		virtual void sendExitToAll(){};
		std::set<int> answered;


	public:
		/// ������ʱ,��ready_question����ʱ����������ٽ��м�ʱ, Ĭ�Ͻ���30����
		DWORD count;

		/// �������
		BYTE subject_type;

		DWORD active_time;

		/// ��ǰ�Ѵ���Ŀ��
		DWORD cur_subject;

		/// ���δ����������
		DWORD total_subject;

		/// �����ʱ
		DWORD question_count;

		enum{
			QUIZ_READY,		// ����ȴ�״̬
			QUIZ_READY_QUESTION,    // �ȴ�����Ӧ�߻�Ӧ״̬,����10��
			QUIZ_SEND_QUESTION,	// ����״̬
			QUIZ_ACTIVE_QUESTION,	// ����״̬
			QUIZ_END_QUESTION,      // ���������ʴ�,������Ӧ����(ȫ������ʱ����������������������÷֣�
						// �����ʴ�����е÷ֺ��Ĳɵķ���)
			QUIZ_READY_OVER,        // ����������֪ͨ����ȡ�������˵Ķ�ս״̬����������Ӯ,���ò鿴����ʱ��
			QUIZ_READ_SORT,		// ��״ֻ̬����ȫ��,�ȴ�����ʱ����ڴ�״̬�ٷ���һ�ν���
			QUIZ_RETURN_GOLD,	// �����ʴ����������δ���н���
			QUIZ_OVER		// �������״̬
		};

		CQuiz();
		CQuiz(DWORD active_time, DWORD ready_time); 
				
		virtual ~CQuiz();

		/**
		 * \brief ʱ���¼�����ص���������TimeTick.cpp�к����ص�
		 *
		 *
		 */
		void timer();	//ʱ���¼�����ص�


		// ����Ϊ���ö�ս״̬�Ĵ���������ս״̬����鿴��սϵͳ״̬ת��ͼ
		// ״̬ת������Ĭ��������Բ���Ҫ���ء�ֻ������ָ���ļ���������
		// �������ʵ��һ����Ĭ����Ϊ��ͬ��״̬ת�����̡�������Ҫ��������״̬ת������
		
		virtual void setReadyState();
		virtual void setReadyQuestionState();
		virtual void setSendQuestionState();
		virtual void setActiveQuestionState();
		virtual void setEndQuestionState();
		virtual void setReadyOverState();
		virtual void setReturnGoldState();
		virtual void setReadSortState();
		virtual void setOverState();

		virtual bool addPothunters(UserSession* pUser) = 0;

		/**
		 * \brief ���������ǰ״̬
		 *
		 */
		void printState();

		/**
		  * \brief �û����ߴ���
		  *
		  * �û�����ʱ��ͬʱ�жϸ��û��ľ���״̬��������Ӧ����,�����ʴ����߲��账��
		  * ���羺��ʱ����Ҫ�ж����Ƿ��Ѿ��μ��˾���,���û�вμӣ��򷢳�ѯ�ʣ�����Ѿ��μӣ����ٴ���
		  *
		  * \param pUser �����û�
		  *
		  */
		virtual void userOnline(UserSession* pUser) = 0;
		
		/**
		  * \brief �û����ߴ���
		  *
		  * �û�����ʱ��ͬʱ�жϸ��û��ľ���״̬��������Ӧ����, �����ʴ�����ʱ������setReadyOver״̬��ȫ������ʱ��������
		  *
		  * \param pUser �����û�
		  *
		  */
		virtual void userOffline(UserSession* pUser) = 0;

		/**
		  * \brief �ж��Ƿ����ھ����ڼ�
		  *
		  * \return ������ڶ�սʱ�ڣ�����TRUE,���򷵻�FALSE
		  */
		virtual bool isActivePeriod()=0;

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
		  * \brief �ж��Ƿ�����׼����
		  *
		  * \return �������׼����ʱ�ڣ�����TRUE,���򷵻�FALSE
		  */
		virtual bool isReadyQuestionPeriod()
		{
			return this->count<ready_question_time ? true : false;
		}


		/**
		 * \brief  �ж��Ƿ����ڲ鿴������
		 *
		 */
		virtual bool isReadSortPeriod()
		{
			return this->count<5?true:false;
		}
		
		virtual bool isActiveQuestionPeriod()
		{
			return this->question_count<=20 ? true : false;
		}

		virtual bool isEndQuestionPeriod()
		{
			return this->question_count<=25 ? true : false;
		}
		
		/**
		  * \brief �ж��Ƿ�����׼����
		  *
		  * \return �������׼����ʱ�ڣ�����TRUE,���򷵻�FALSE
		  */
		virtual void updateTime()
		{
			rwlock.wrlock();
			this->count += 1;
			this->question_count += 1;
			rwlock.unlock();
		}

		virtual BYTE getType()
		{
			return this->type;
		}

		virtual BYTE getState()
		{
			return this->state;
		}

		virtual int answer(Cmd::stAnswerQuiz* pCmd, DWORD dwUserID) = 0;
		virtual void exitQuiz(DWORD dwUserID) = 0;

		/// ���ξ�������Ŀ
		std::vector<CSubject>   subjects;
//		CSubject subjects[1];
//		int subjects[100];
};


class CQuizPersonal : public CQuiz
{
	public:
		CQuizPersonal();
		CQuizPersonal(DWORD active_time, DWORD ready_time);
		virtual ~CQuizPersonal();
		
		bool isActivePeriod()
		{
			return this->cur_subject<=total_subject;
		}
		
		void setSendQuestionState();
		void setEndQuestionState();
		void setReadyOverState();
		void setReturnGoldState();

		int answer(Cmd::stAnswerQuiz* pCmd, DWORD dwUserID);
		bool addPothunters(UserSession* pUser);
		void exitQuiz(DWORD dwUserID);
		
		/**
		  * \brief �û����ߴ���
		  *
		  * �û�����ʱ��ͬʱ�жϸ��û��ľ���״̬��������Ӧ����
		  *
		  * \param pUser �����û�
		  *
		  */
		void userOnline(UserSession* pUser);
		void userOffline(UserSession* pUser);
		CPothunter pothunter;
	protected:
		bool award();
		UserSession* user;
};

class CQuizWorld : public CQuiz
{
	public:
		CQuizWorld();
		CQuizWorld(DWORD active_time, DWORD ready_time, BYTE subject_type=0);
		virtual ~CQuizWorld();

		void setSendQuestionState();
		void setEndQuestionState();
		void setReadyOverState();
		
		bool addPothunters(UserSession* pUser);
		int answer(Cmd::stAnswerQuiz* pCmd, DWORD dwUserID);
		
		void exitQuiz(DWORD dwUserID);

		bool isActivePeriod()
		{
			return this->cur_subject<=total_subject;
		}

		/**
		  * \brief �û����ߴ���
		  *
		  * �û�����ʱ��ͬʱ�жϸ��û��ľ���״̬��������Ӧ����
		  *
		  * \param pUser �����û�
		  *
		  */
		void userOnline(UserSession* pUser);
		void userOffline(UserSession* pUser);
	public:		
		/// ������Ա�б�
		std::vector<CPothunter> pothunters;
//		int valid_pothunter; // ������Ч����
	private:
		virtual void sendExitToAll();

};

#endif

