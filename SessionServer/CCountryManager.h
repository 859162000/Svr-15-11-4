/**
 * \file
 * \version  $Id: CCountryManager.h  $
 * \author  
 * \date 
 * \brief ���ҹ�����
 *
 * 
 */

#ifndef _CCOUNTRY_MANAGER_H
#define _CCOUNTRY_MANAGER_H
#include <vector>
#include <map>
#include "zType.h"
#include "zRWLock.h"
#include "SessionCommand.h"
#include "zSingleton.h"
#include "Ally.h"

class Record;
class UserSession;
class SessionTask;
enum
{
	SILK_TECH=1,	// '�������װ'
	SKIN_TECH=2,	// 'Ƥ�����װ'
	LORICAE_TECH=3, 	// '�������װ'
	SWORD_TECH=4, 	// '������'
	BOW_TECH=5, 	// '����'
	WAND_TECH=6, 	// '����'
	STAFF_TECH=7, 	// '����'
	FAN_TECH=8, 	// '����'
	NECKLACE_TECH=9, 	// '������'
	FINGER_TECH=10, 	// '��ָ��'
	CUFF_TECH=11, 	// '������'
	GIRDLE_TECH=12, 	// '������'
	SHOES_TECH=13, 	// 'Ь����'
	ARMET_TECH=14, 	// 'ͷ����'
};

const int TECH_MAX_NUM = 15; // �Ƽ��������Ŀ
const int COUNTRY_MATERIAL = 0; // ���Ҳֿ��е�����
const int COUNTRY_STOCK = 1;	// ���Ҳֿ��е�ԭ�� 
const int COUNTRY_MONEY = 2; // ���Ҳֿ��е�����
const int COUNTRY_ALLY_NPC_HORTATION_MONEY = 40 * 10000; // 40 ��
const int COUNTRY_ALLY_NPC_HORTATION_MATERIAL = 1000; // 1000����λ������
const DWORD NEUTRAL_COUNTRY_ID = 6; // ������ID

class CTech
{
	public:
		enum 
		{
			INIT_TECH,	// ��ʼ״̬
			WAIT_TECH,	// ͶƱѡ���ĿƼ����ȴ�����ѡ���Ա
			ACTIVE_TECH,    // ���ڽ�������
			FINISH_TECH,	// �Ѿ�������������Դ�����Ӧװ��(�����ȼ�����0������״̬�ĿƼ�)
		};
		
		CTech();
		~CTech();

		void init(Record* rec);
		void writeDatabase();
		void upLevel(UserSession* pUser);
		void setSearcher(UserSession* pUser);
		void clearSearcher(UserSession* pUser);
			
		DWORD state()
		{
			return this->dwStatus;
		}

		void state(DWORD newState)
		{
			rwlock.wrlock();
			this->dwStatus = newState;
			rwlock.unlock();
			this->writeDatabase();
		}

		DWORD type()
		{
			return this->dwType;
		}


		DWORD level()
		{
			return this->dwLevel;
		}

		DWORD dwUID;
		DWORD dwCountryID;
		DWORD dwType;
		char  szName[MAX_NAMESIZE];
		DWORD dwProgress;
		DWORD dwResearchID;
		char  szResearchName[MAX_NAMESIZE];
		DWORD dwStatus;
		DWORD dwLevel;
		DWORD dwLastUpTime;

		zRWLock rwlock;
};

class CCountry
{
	public:
		CCountry()
		{
			dwID = 0;
			dwKingUnionID = 0;
			dwLastDareTime = 0;
			dwLastDailyMoney = 0;
			dwDareTime = 0;
			dwDareCountryID = 0;
			bzero(name, sizeof(name));
			bzero(kingName, sizeof(kingName));
			dwFormalWin = 0;
			dwFormalFail  = 0;
			dwAnnoyWin = 0;
			dwAnnoyFail = 0;
			dwStar	 = 0;
			dwTax = 0;	///˰��
			qwGold = 0;	///����
			isBeging = false;
			qwSilk	=	0;	// ˿��
			qwOre	=	0;	// ��ʯ
			qwBowlder	=	0;	// ��ʯ
			qwWood	=	0;	// ľ��
			qwCoat	=	0;	// Ƥë
			qwHerbal	=	0;	// ��ҩ
			qwMaterial	=	0; // ����
			qwStock		=	0; // ԭ��
			forbidTalk	=	0; //����
			winner_exp = 0;
			winner_time = 0;
			bzero(note, sizeof(note));
			sendPrison = 0;
			gen_level = 0;
			gen_exp = 0;
			gen_maxexp = 0;
			gen_refreshTime = 0;
			calltimes = 0;
			kingtime = 0;
		}
		
		~CCountry()
		{
		}
		
		void init(Record* rec);
		void writeDatabase();
		void loadTechFromDB();

		bool insertDatabase();
		bool isMe(DWORD country);
		bool isKing(UserSession* pUser);
		bool isOfficial(UserSession* pUser);

		bool changeKing(UserSession* pUser);
		bool changeEmperor(DWORD dwCountryID);
		void updateKing(UserSession* pUser);
		bool changeDiplomat(UserSession* pUser);// �ı��⽻��
		bool cancelDiplomat(); // �����⽻��
		
		bool changeCatcher(UserSession* pUser);// �ı䲶ͷ
		bool cancelCatcher(); // �����⽻��

		void beginDare();
		void beginAntiDare(DWORD dwAttCountry);
		void endDare();


		void addTaxMoney(QWORD qwTaxMoney);
		
		/// ��ʼ���пƼ�ͶƱ
		void beginTechVote();

		DWORD dwID;
		DWORD dwKingUnionID;
		DWORD dwDareTime;	// �ϴη�����ս��ʱ��
		DWORD dwLastDareTime;   // �ϴν�����ս��ʱ��
		DWORD dwLastDailyMoney; // �ʵ��ϴ���ȡ5D������ʱ��
		DWORD dwDareCountryID;
		bool  isBeging;

		char  name[MAX_NAMESIZE+1]; // ��������
		char  kingName[MAX_NAMESIZE+1]; // ��������
		char  diplomatName[MAX_NAMESIZE+1]; // �⽻������
		char  catcherName[MAX_NAMESIZE+1]; // ��ͷ����
		
		DWORD dwFormalWin;
		DWORD dwFormalFail;
		DWORD dwAnnoyWin;
		DWORD dwAnnoyFail;
		DWORD dwStar;
		DWORD dwTax;	///˰��
		QWORD qwGold;	///����
		QWORD qwSilk;	// ˿��
		QWORD qwOre;	// ��ʯ
		QWORD qwBowlder;	// ��ʯ
		QWORD qwWood;	// ľ��
		QWORD qwCoat;	// Ƥë
		QWORD qwHerbal;	// ��ҩ
		QWORD qwMaterial; // ����
		QWORD qwStock;	// ԭ��
		DWORD forbidTalk; //��������
		DWORD sendPrison; //�����ؼ���
		DWORD gen_level; //�󽫾��ĵȼ�
		DWORD gen_exp; //�󽫾��ľ���
		DWORD gen_maxexp; //�󽫾��������
		DWORD gen_refreshTime;//�󽫾������ʱ��
		DWORD calltimes;	//����ʹ�ô���
		DWORD kingtime;		//������λʱ��(Сʱ)

		char note[256];

		void  changeStar(int star); // �ı���
		DWORD getStar();

		/// �ı�ָ���������ʵİٷֱȣ�0Ϊ��ͨ���ʣ�1ԭ��,2Ϊ����
		/// ����ֵΪ������ĸı�ֵ
		int changeMaterialByPer(int type, float per);
		// �ı�ָ���������ʵ���ֵ
		void  changeMaterial(int type, int value);
		// �õ�ָ�����͵�����
		QWORD getMaterial(int type);
		void swapMaterialByPer(CCountry* pToCountry, float per);

		/// �Ƽ�����
		typedef std::map<DWORD, CTech*> CTechMap;
		CTechMap techIndex;
		CTech* getTech(DWORD dwType);
		void   addTech(DWORD dwType, CTech* pTech);
	
		zRWLock rwlock;
		void setWinnerExp();
		void checkWinnerExp();

		void addGeneralExp(DWORD num);
		void generalLevelDown();
		void refreshGeneral();
	private:
		bool winner_exp;
		DWORD winner_time;
};

class CCountryM : public Singleton<CCountryM>
{
	friend class SingletonFactory<CCountryM>;
	public:
		bool init();
		static void destroyMe();

		void timer();
		void timerPerHour();
		void save();
		void beginDare();
		void endDare();

		void resetCallTimes();
		void beginGem();
		bool load();
		bool refreshDB();
		bool processUserMessage(UserSession *pUser,const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);
		bool processSceneMessage(const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);

		void processRequestDare(UserSession* pUser, Cmd::stRequestDareCountryCmd* rev);
		void processDareCountry(UserSession* pUser, Cmd::stDareCountryFormalCmd* rev);
		void processAntiDareCountry(UserSession* pUser, Cmd::stAntiDareCountryFormalCmd* rev);
		void processRequestTech(UserSession* pUser, Cmd::stReqTechUserCmd* rev);
		void processUpTech(UserSession* pUser, Cmd::stUpTechDegreeUserCmd* rev);
		void processSetTechSearch(UserSession* pUser, Cmd::stSetTechSearchUserCmd* rev);
		void processConfirmSearcher(UserSession* pUser, Cmd::stConfirmSearcherUserCmd* rev);
		void processReqWaitOfficial(UserSession* pUser, Cmd::stReqWaitOfficialUserCmd* rev);
		void processCancelTechSearch(UserSession* pUser, Cmd::stCancelTechSearchUserCmd* rev);
		void processReqDailyEmperorMoney(UserSession* pUser, Cmd::stReqDailyEmperorMoneyCmd* rev);
		void processSetDiplomat(UserSession* pUser, Cmd::stAppointDiplomatCmd* rev);
		void processSetCatcher(UserSession* pUser, Cmd::stAppointCatcherCmd* rev);
		void processCancelDiplomat(UserSession* pUser, Cmd::stCancelDiplomatCmd* rev);
		void processCancelCatcher(UserSession* pUser, Cmd::stCancelCatcherCmd* rev);
		
		CCountry* addNewCountry(DWORD country);

		CCountry* find(DWORD country, DWORD unionid);
		CCountry* find(DWORD country);
		CCountry* findByDare(DWORD country, bool findDare = true);

		struct countryCallback
		{
			virtual void exec(CCountry *)=0;
			virtual ~countryCallback(){};
		};
		void execEveryCountry(countryCallback &);//�������޹�������
		void userOnline(UserSession * pUser);
		void refreshTax();
		void refreshTech(SessionTask* scene, DWORD dwCounryID);

		void broadcastTech(DWORD dwCountryID);

		bool isKing(UserSession* pUser);
		bool isEmperor(UserSession* pUser);
		bool isOfficial(UserSession* pUser);

		void refreshGeneral(DWORD country);
	private:
		void clearForbid();
		void clearDiplomat();
		void clearCatcher();
		DWORD clearForbidTime;
		CCountryM();
		std::vector<CCountry*> countries;
		bool isBeging;
			
		zRWLock rwlock;
};

#endif

