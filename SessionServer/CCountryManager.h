/**
 * \file
 * \version  $Id: CCountryManager.h  $
 * \author  
 * \date 
 * \brief 国家管理器
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
	SILK_TECH=1,	// '布料类服装'
	SKIN_TECH=2,	// '皮甲类服装'
	LORICAE_TECH=3, 	// '铠甲类服装'
	SWORD_TECH=4, 	// '刀盾类'
	BOW_TECH=5, 	// '弓类'
	WAND_TECH=6, 	// '棍类'
	STAFF_TECH=7, 	// '杖类'
	FAN_TECH=8, 	// '扇类'
	NECKLACE_TECH=9, 	// '项链类'
	FINGER_TECH=10, 	// '戒指类'
	CUFF_TECH=11, 	// '护腕类'
	GIRDLE_TECH=12, 	// '腰带类'
	SHOES_TECH=13, 	// '鞋子类'
	ARMET_TECH=14, 	// '头盔类'
};

const int TECH_MAX_NUM = 15; // 科技的最大数目
const int COUNTRY_MATERIAL = 0; // 国家仓库中的物资
const int COUNTRY_STOCK = 1;	// 国家仓库中的原料 
const int COUNTRY_MONEY = 2; // 国家仓库中的银两
const int COUNTRY_ALLY_NPC_HORTATION_MONEY = 40 * 10000; // 40 锭
const int COUNTRY_ALLY_NPC_HORTATION_MATERIAL = 1000; // 1000个单位的物资
const DWORD NEUTRAL_COUNTRY_ID = 6; // 中立国ID

class CTech
{
	public:
		enum 
		{
			INIT_TECH,	// 初始状态
			WAIT_TECH,	// 投票选出的科技，等待国王选择官员
			ACTIVE_TECH,    // 正在进行升级
			FINISH_TECH,	// 已经完成升级，可以打造相应装备(包括等级大于0的所有状态的科技)
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
			dwTax = 0;	///税率
			qwGold = 0;	///国库
			isBeging = false;
			qwSilk	=	0;	// 丝线
			qwOre	=	0;	// 矿石
			qwBowlder	=	0;	// 玉石
			qwWood	=	0;	// 木材
			qwCoat	=	0;	// 皮毛
			qwHerbal	=	0;	// 草药
			qwMaterial	=	0; // 物资
			qwStock		=	0; // 原料
			forbidTalk	=	0; //禁言
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
		bool changeDiplomat(UserSession* pUser);// 改变外交官
		bool cancelDiplomat(); // 罢免外交官
		
		bool changeCatcher(UserSession* pUser);// 改变捕头
		bool cancelCatcher(); // 罢免外交官

		void beginDare();
		void beginAntiDare(DWORD dwAttCountry);
		void endDare();


		void addTaxMoney(QWORD qwTaxMoney);
		
		/// 开始进行科技投票
		void beginTechVote();

		DWORD dwID;
		DWORD dwKingUnionID;
		DWORD dwDareTime;	// 上次发起挑战的时间
		DWORD dwLastDareTime;   // 上次接受挑战的时间
		DWORD dwLastDailyMoney; // 皇帝上次领取5D奖励的时间
		DWORD dwDareCountryID;
		bool  isBeging;

		char  name[MAX_NAMESIZE+1]; // 国家名称
		char  kingName[MAX_NAMESIZE+1]; // 国王名称
		char  diplomatName[MAX_NAMESIZE+1]; // 外交官名称
		char  catcherName[MAX_NAMESIZE+1]; // 捕头名称
		
		DWORD dwFormalWin;
		DWORD dwFormalFail;
		DWORD dwAnnoyWin;
		DWORD dwAnnoyFail;
		DWORD dwStar;
		DWORD dwTax;	///税率
		QWORD qwGold;	///国库
		QWORD qwSilk;	// 丝线
		QWORD qwOre;	// 矿石
		QWORD qwBowlder;	// 玉石
		QWORD qwWood;	// 木材
		QWORD qwCoat;	// 皮毛
		QWORD qwHerbal;	// 草药
		QWORD qwMaterial; // 物资
		QWORD qwStock;	// 原料
		DWORD forbidTalk; //国王禁言
		DWORD sendPrison; //国王关监狱
		DWORD gen_level; //大将军的等级
		DWORD gen_exp; //大将军的经验
		DWORD gen_maxexp; //大将军的最大经验
		DWORD gen_refreshTime;//大将军复活的时间
		DWORD calltimes;	//令牌使用次数
		DWORD kingtime;		//国王在位时间(小时)

		char note[256];

		void  changeStar(int star); // 改变量
		DWORD getStar();

		/// 改变指定类型物资的百分比，0为普通物资，1原料,2为银子
		/// 返回值为，具体的改变值
		int changeMaterialByPer(int type, float per);
		// 改变指定类型物资的数值
		void  changeMaterial(int type, int value);
		// 得到指定类型的物资
		QWORD getMaterial(int type);
		void swapMaterialByPer(CCountry* pToCountry, float per);

		/// 科技索引
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
		void execEveryCountry(countryCallback &);//不包括无国籍国家
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

