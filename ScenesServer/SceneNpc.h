/**
 * \file
 * \version  $Id: SceneNpc.h $
 * \author  
 * \date 
 * \brief ����Npc
 *
 * 
 */

#ifndef _SceneNpc_h_
#define _SceneNpc_h_

#include "zSceneEntry.h"
#include "SceneDefine.h"
#include "zDatabaseManager.h"
#include "Command.h"
#include "zTime.h"
#include "zUniqueID.h"
//�ٻ�����
#include <set>
#include <map>
#include "SceneEntryPk.h"
#include "zAStar.h"
#include "SceneNpcAI.h"

class SceneUser;
class NpcAIController;
class SceneNpc;

//��Ҫ����AI������npc
#ifdef _POOL_ALLOC_
typedef std::set<SceneNpc *, std::less<SceneNpc *>, __gnu_cxx::__pool_alloc<SceneNpc *> > MonkeyNpcs;
#else
typedef std::set<SceneNpc *, std::less<SceneNpc *> > MonkeyNpcs;
#endif

/*
struct t_PetState
{
	DWORD id;
	Cmd::petType type;
	char name[MAX_NAMESIZE];
	DWORD hp;
	DWORD exp;
	WORD ai;
};
*/

struct t_expRec
{
	DWORD wdHP;
	zTime  attack_time;

	t_expRec()
	{
		attack_time.now();
		wdHP = 0;
	}
};

/* һЩ����npc��ID */
const unsigned int COUNTRY_MAIN_FLAG = 58001;
const unsigned int COUNTRY_SEC_FLAG  =  58002;
const unsigned int COUNTRY_MAIN_GEN = 58200;
const unsigned int COUNTRY_SEC_GEN  = 58201;
const unsigned int COUNTRY_KING_MAIN_FLAG = 58005;
const unsigned int COUNTRY_KING_SEC_FLAG = 58006;
const unsigned int COUNTRY_EMPEROR_MAIN_GEN = 58203;
const unsigned int COUNTRY_EMPEROR_SEC_GEN = 58204;

const unsigned int ALLY_GUARDNPC = 54100;//�˹��ڳ�
const unsigned int EMPEROR_HORSE_ID = 3202;//�ʵ۵���ID
const unsigned int KING_HORSE_ID = 3204;//��������ID

/**
 * \brief ����Npc
 *
 */
class SceneNpc : public SceneEntryPk, public zAStar<>, public zAStar<2>
{
	friend class NpcAIController;

	static const DWORD maxUniqueID = 100000;

	public:
	bool isMainGeneral();//�Ƿ�󽫾���

	zRTime reliveTime;//����ʱ��

	int targetDistance;//�뵱ǰĿ�����̾���
	int closeCount;//׷�������10��������̾���û�м�������ΪĿ�겻�ɵ���

	std::list<ScenePet *> semipetList;//������б�

	/**
	 * \brief ����Npc����״̬
	 *
	 */
	enum SceneNpcChase
	{
		CHASE_NONE,			/// û�и���״̬
		CHASE_ATTACK,		/// ���ٹ���״̬
		CHASE_NOATTACK		/// ��ͨ����״̬
	};

	/**
	 * \brief Npc����
	 * ��̬�Ļ��Ƕ�̬�����
	 */
	enum SceneNpcType
	{
		STATIC,				/// ��̬��
		GANG				/// ��̬��
	};

	/**
	 * \brief Npc��������
	 *
	 */
	zNpcB *npc;

	/**
	 * \brief ��ǿNpc��������
	 *
	 */
	zNpcB *anpc;

	/**
	 * \brief Npc��������
	 *
	 */
	const t_NpcDefine *define;

	/**
	 * \brief npc��ǰ����ֵ
	 *
	 */
	DWORD hp;

	///�ϴη���ʱ��hp
	DWORD lasthp;

	///��Ѫ���
	bool needRecover;
	///�´λ�Ѫ��ʱ��
	zRTime rcvTimePet;//������Ϣ
	zRTime rcvTimeUnder30;//hp30����
	zRTime rcvTimeRest;//����ս��
	//hp30���»�Ѫ
	bool recoverUnder30;
	//����ս����Ѫ
	//bool recoverLeaveBattle;

	bool checkRecoverTime(const zRTime& ct);
	void setRecoverTime(const zRTime& ct, int delay);

	virtual bool recover();
	/// npc����������ʱ��(���û���þ����䴴��ʱ��)
	DWORD dwStandTime;

	///npc����������ʱ��
	DWORD dwStandTimeCount;

	///������С������
	WORD appendMinDamage;

	///������󹥻���
	WORD appendMaxDamage;

	///�Ƿ��Ѿ�����Ŀ��
	bool lockTarget;

	///���ǵ�npc���湥��һ��ɾ��
	bool isRushNpc;

	///���������ٻ�����npc
	int summonByNpcMap(std::map<DWORD, std::pair<DWORD, DWORD> > map);

	///�Ƿ��ٻ���
	bool summoned;

	bool setCurTarget(SceneEntryPk *, bool=false);
	bool setCurTarget(DWORD, DWORD, bool=false);
	void leaveBattle();

	void setClearState();
	bool needClear();

	bool isSpecialNpc();
	bool isFunctionNpc();
	bool isTaskNpc();

	SceneNpc(Scene *scene, zNpcB *npc, const t_NpcDefine *define, const SceneNpcType type, const SceneEntryType entrytype=SceneEntry_NPC, zNpcB *anpc=NULL);
	~SceneNpc();
	bool zPosOutofRange()const;
	SceneNpcChase getChaseMode()const;
	SceneEntryPk* getChaseSceneEntry() const;
	bool chaseSceneEntry(const DWORD type, const DWORD userid);
	virtual bool isRedNamed(bool allRedMode=true);
	void unChaseUser();
	bool checkLockUserOverdue(const zRTime &ct);
	void setLockUser(const DWORD dwID);
	bool checkMoveTime(const zRTime &ct);
	void setMoveTime(const zRTime &ct);
	void setMoveTime(const zRTime &ct, const int delay);
	void delayMoveTime(const int delay);
	bool checkAttackTime(const zRTime &ct) const;
	void setAttackTime(const zRTime &ct);
	void setAttackTime(const zRTime &ct, const int delay);
	//bool checkUnchaseTime(const zRTime &ct) const;
	//void setUnchaseTime(const zRTime &ct, const int delay);
	bool canLostObject(const zRTime &ct);
	bool canRelive(const zRTime &ct);
	bool canRush();
	const SceneNpcType &getSceneNpcType() const;
	void full_t_NpcData(Cmd::t_NpcData &data);
	void full_t_MapNpcDataState(Cmd::t_MapNpcDataState &data);
	void full_t_MapNpcDataAndPosState(Cmd::t_MapNpcDataPosState &data);
	void full_t_MapNpcData(Cmd::t_MapNpcData &data);
	void full_stRTMagicPosUserCmd(Cmd::stRTMagicPosUserCmd &ret) const;
	void full_t_MapNpcDataAndPos(Cmd::t_MapNpcDataPos &data);
	virtual void sendMeToNine();
	bool isBugbear();
	bool canBeAttack();
	void death(const zRTime &ct);
	void backoff(const int direct, const int step);
	void reduceHP(SceneUser *pAtt, DWORD wdHP);
	void distributeExp();
	void distributeMoney(DWORD money);
	void clearStateToNine(WORD state);
	void setStateToNine(WORD state);
	/**
	 * \brief �ó�������
	 * \author fqnewman
	 */
	virtual void relivePet(){};

	DWORD catchme; ///�������﹥���Լ�
	int boostupPet; /// ��ǿ����
	DWORD boostupPetMDef; //��ǿ����ķ�������
	DWORD boostupSummon; ///�ٻ��޹�����ǿ
	DWORD boostupHpMaxP;  ///��������ֵ����
	DWORD dwReduceDam;  /// �ٻ����˺��ۼ�
	DWORD giddy;   ///������ʱ��ʹ�Է�ѣ�εļ���

	BYTE notifystep; //��BOSS֪ͨ����

	//*
	static void AI(const zRTime& ctv, MonkeyNpcs &affectNpc,const DWORD group , const bool every);

	/**
	 * \brief ǿ�Ƹ����û���������Ѿ��ڸ����û�����ô��45%�ļ��ʽ�Ŀ��ת����Ŀǰ���û�
	 * \param pAtt Ҫ���ٵĶ���
	 */
	bool forceChaseUser(SceneEntryPk *pAtt);
	/**
	 * \brief ���ͻ�����Ϣת�����Ự������
	 * \author fqnewman
	 */
	bool forwardSession(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
	//bool checkChaseUser(SceneUser *&sceneUser);
	bool checkChaseAttackTarget(SceneEntryPk *&entry);
	bool canChaseTarget(const SceneEntryPk *entry);
	bool canAttack(SceneEntryPk *entry);
	bool inRange(SceneEntryPk *entry);
	bool attackTarget(SceneEntryPk *entry = NULL);
	//bool chasedFindFrontUser( const int direct);
	//bool findAndChaseUser(const int radius, SceneEntryPk *&entry);
	//bool findAnyTarget(const int radius, SceneEntryPk *&entry);
	void action(const zRTime& ctv);
	virtual bool normalAction();
	bool deathAction();
	bool hideAction();
	bool moveable(const zPos &tempPos, const zPos &destPos,  int radius);
	bool move(const int direct, const int step);
	bool warp(const zPos &pos,bool ignore=false);//��ת
	void jumpTo(zPos &newPos);
	bool gotoFindPath(const zPos &srcPos, const zPos &destPos);
	bool goTo(const zPos &pos);
	bool shiftMove(const int direct);
	void set_quest_status(SceneUser* user);
	void setStandingTime(DWORD standTime);
	void refreshExpmapAttackTime(SceneUser* pAtt);

	/**
	 * \brief �ı��ɫ��hp
	 * \param hp �����HP
	 * \author fqnewman
	 */
	void changeHP(const SDWORD &hp);

	/**
	 * \brief ���ֱ���˺�
	 * \param pAtt ������
	 * \param dam �˺�ֵ
	 * \param notify ֪ͨ�˺���ʾ
	 * \author fqnewman
	 */
	SWORD directDamage(SceneEntryPk *pAtt, const SDWORD &dam, bool notify=false);

	/**
	 * \brief �ı��ɫ��sp
	 * \param sp �����SP
	 * \author fqnewman
	 */
	void changeSP(const SDWORD &sp);

	/**
	 * \brief �ı��ɫ��mp
	 * \param mp �����MP
	 * \author fqnewman
	 */
	void changeMP(const SDWORD &mp);

	/**
	 * \brief �ڱ��Լ�����֮ǰ�Ĵ���������װ����Ĵ���������Ч�����жϵ�
	 * \param pUser ������
	 * \param rev ���ι����Ĵ���ָ��
	 * \param physics �Ƿ�������
	 * \param good Ϊtrue����У�Ϊfalse��Ҫ�жϻر���
	 * \author fqnewman
	 * \return trueΪ��ι�������Ч�ģ�falseΪһ����Ч�Ĺ���
	 */
	bool preAttackMe(SceneEntryPk *pUser, const Cmd::stAttackMagicUserCmd *rev, bool physics=true, const bool good = false);

	/**
	 * \brief ��ɫ������
	 * \param pEntry ������
	 * \param rev ���ι����Ĵ�����Ϣ
	 * \param physics �Ƿ�������
	 * \author fqnewman
	 * \return trueΪ��ι�������Ч�ģ�falseΪһ����Ч�Ĺ���
	 */
	bool AttackMe(SceneEntryPk *pEntry, const Cmd::stAttackMagicUserCmd *rev, bool physics=true, SWORD rangDamageBonus=0);

	/**
	 * \brief ��ɫ������N��
	 * \param dwAttTempID �����ߵ���ʱID
	 * \param grids ���˼���
	 * \author fqnewman
	 */
	void standBack(const DWORD dwAttTempID, DWORD grids);

	/**
	 * \brief ������Ŀ�껻��dwTempID��ָ��Ľ�ɫ���
	 * \param dwTempID Ŀ���ɫ����ʱID
	 * \author fqnewman
	 */
	void changeAttackTarget(const DWORD &dwTempID);

	/**
	 * \brief �ý�ɫ����
	 * \author fqnewman
	 */
	void toDie(const DWORD &dwTempID);

	/**
	 * \brief ֪ͨ�ͻ�������ֵ�ı仯
	 * \author fqnewman
	 */
	void attackRTHpAndMp();

	/**
	 * \brief ����npcѪ��
	 * \author whj
	 */
	void showHP(SceneUser *pUser, DWORD npchp);

	/**
	 * \brief �������Ƿ񹥻�����npc
	 * \author whj
	 */
	bool isAttackMe(SceneEntryPk *);
	/**
	 * \brief �жϽ�ɫ�Ƿ�����
	 * \author fqnewman
	 * \return trueΪ����
	 */
	bool isDie();
	/**
	 * \brief ��ȡ��ɫ�ļ���
	 * \author fqnewman
	 */
	virtual DWORD getLevel() const;

	/**
	 * \brief ��Ҫ��ְҵ���ͣ���������ʹ�õļ�������
	 * \author fqnewman
	 */
	bool needType(const DWORD &needtype);

	/**
	 * \brief ��Ҫ��ְҵ���ͣ���������ʹ�õļ�������
	 * \author fqnewman
	 */
	bool addSkillToMe(zSkill *skill);

	/**
	 * \brief �Ƿ��иü�����Ҫ������
	 * \author fqnewman
	 * \return true �� false û��
	 */
	bool needWeapon(DWORD skillid);

	/**
	 * \brief �Ƿ�Pk����
	 * \other PK�����
	 * \author fqnewman
	 * \return true �� false ��
	 */
	virtual bool isPkZone(SceneEntryPk *other=NULL);

	/**
	 * \brief ������Ʒ�����ͷ���
	 * \param object ������Ʒ������
	 * \param num ������Ʒ������
	 * \author fqnewman
	 * \return true ���ĳɹ� false ʧ��
	 */
	bool reduce(const DWORD &object, const BYTE num);

	/**
	 * \brief ����������Ʒ�Ƿ��㹻
	 * \param object ������Ʒ������
	 * \param num ������Ʒ������
	 * \author fqnewman
	 * \return true �㹻 false ����
	 */
	bool checkReduce(const DWORD &object, const BYTE num);

	/**
	 * \brief ʩ�ż��������µ�����MP,HP,SP
	 * \param base ���ܻ������Զ���
	 * \author fqnewman
	 * \return true ���ĳɹ� false ʧ��
	 */
	bool doSkillCost(const zSkillB *base);

	/**
	 * \brief ���ʩ�ż��������µ�����MP,HP,SP�Ƿ��㹻
	 * \param base ���ܻ������Զ���
	 * \author fqnewman
	 * \return true ���ĳɹ� false ʧ��
	 */
	bool checkSkillCost(const zSkillB *base);

	/**
	 * \brief ��������ʩ�ųɹ����ʣ�������μ����Ƿ����ʩ��
	 * \author fqnewman
	 * \return true �ɹ� false ʧ��
	 */
	bool checkPercent();

	/**
	 * \brief �ж��Ƿ��ǵ���
	 * \author fqnewman
	 * \return true �� false ����
	 */
	//bool isEnemy(SceneUser *pUser);

	/**
	 * \brief ��������ض���ɫ���Ӷ��⾭��,����������װ���Ķ��⾭��
	 * \param wdExp ���䵽�ľ���
	 * \param pUser �������
	 * \return ���¼����ľ���ֵ
	 */
	DWORD addOtherExp(DWORD wdExp , SceneUser *pUser);

	/**
	 * \brief ��������hp
	 * \author fqnewman
	 */
	virtual void changeAndRefreshHMS(bool lock=true, bool sendData=true);

	/**
	 * \brief ���ݵȼ�������¼��㾭��
	 * \param wdExp ���䵽�ľ���ֵ
	 * \param char_level ��ɫ�ȼ�
	 * \return ���¼����ľ���ֵ
	 */
	DWORD levelExp(DWORD wdExp , DWORD char_level);

	/**
	 * \brief ���·��ͱ�NPC�ĵ�ͼ����
	 */
	void reSendMyMapData();

	//���ó��������
	virtual void setMaster(SceneEntryPk *){}

	//��ȡ���������
	virtual SceneEntryPk *getMaster() {return 0;}
	/**
	 * \brief �õ����ϲ������
	 *
	 * \return ����
	 */
	virtual SceneEntryPk *getTopMaster(){return this;}

	//�л�����
	bool changeMap(Scene *newScene,const zPos &pos);
	void setAspeedRate(float rate);
	void resetAspeedRate();

	/**
	 * \brief ֪ͨѡ���Լ����û���hp��mp�����仯
	 */
	void sendtoSelectedHpAndMp();
	void sendtoSelectedState(DWORD state , WORD value , WORD time);

	bool createRush();
	virtual bool moveToMaster();

	//AI��صķ���
	void setSpeedRate(float rate);
	float getSpeedRate();
	void resetSpeedRate();

	//const t_NpcAIDefine& getAI();
	void setAI(const t_NpcAIDefine &ad);
	//void setActRegion(zPos pos, unsigned int regionX, unsigned int regionY);
	//void setNormalAI();
	bool doNormalAI();
	bool doSayAI();
	bool doRandomChatAI();
	bool doMovetoAI();
	bool doAttackAI();
	bool doGoOnPathAI();
	bool doPatrolAI();
	bool doFleeAI();
	bool doChangeMapAI();
	bool doWarpAI();
	bool doReturnToRegionAI();
	bool doClearAI();
	bool doDropItemAI();
	//void returnToRegion();

	virtual bool isRedNamed(bool allRedMode=true) const;
	bool useSkill(SceneEntryPk * target, DWORD id);
	virtual SceneEntryPk * chooseEnemy(SceneEntryPk_vec &);
	bool debuffEnemy(SceneEntryPk *);
	bool attackEnemy(SceneEntryPk *);
	bool moveToEnemy(SceneEntryPk *);
	virtual bool runOffEnemy(SceneEntryPk_vec &);
	virtual bool isActive();
	virtual bool canMove();
	virtual bool canFight();
	//virtual bool checkMasterTarget(SceneEntryPk *&entry){entry = 0;return false;}
	virtual void petDeath(){}
	bool healSelf();
	bool buffSelf();

	bool canReach(SceneEntryPk *);
	bool isSurrounded();
	int isEnemy(SceneEntryPk *, bool notify = false, bool good = false);
	bool getEntries(int, SceneEntryPk_vec &, int);
	bool healFellow(SceneEntryPk_vec &);
	bool buffFellow(SceneEntryPk_vec &);
	bool helpFellow(SceneEntryPk_vec &);

	virtual bool randomMove();
	void randomChat(NpcChatType type);

	//�ص���
	virtual void on_reached() { }
	virtual void on_death(SceneEntryPk* att){}
	virtual void check() { }
	bool dreadProcess();
	int IsOppose(DWORD five);

	virtual void clearMaster();

	//zPos actPos;//�λ��
	//unsigned int actRegionX,actRegionY;//���Χ
	//t_NpcAIDefine oldAI;
	///npc��������ָ��
	NpcAIController * AIC;
	bool setScript(int id);
	void clearScript();
	void assaultMe(BYTE attacktype, DWORD tempid);

	BYTE getAType();

	///npcAI��־
	DWORD aif;
	///�����AIģʽ
	//WORD petAI;
	ScenePet * summonPet(DWORD, Cmd::petType, DWORD, DWORD, const char *, DWORD, zPos pos=zPos(0,0), BYTE dir=4);
	bool killOnePet(ScenePet *);
	void killAllPets();

	virtual void addPetExp(DWORD num, bool addPet, bool addCartoon){}
	
	virtual void full_PetDataStruct(Cmd::t_PetData &);
	virtual void setPetAI(Cmd::petAIMode){}
	virtual Cmd::petType getPetType();
	virtual void setPetType(Cmd::petType){}
	virtual void setAppendDamage(WORD mindamage, WORD maxdamage);
	virtual DWORD getMinMDamage();
	virtual	DWORD getMaxMDamage(); 
	virtual DWORD getMinPDamage();
	virtual	DWORD getMaxPDamage();
	virtual DWORD getMasterMana(){return 0;}
	virtual DWORD getMinMDefence();
	virtual DWORD getMaxMDefence(); 
	virtual DWORD getMinPDefence();
	virtual DWORD getMaxPDefence();
	virtual DWORD getMaxHP();
	virtual DWORD getBaseMaxHP();

	///�ι���Ŀ��
	DWORD secondTargetType;
	DWORD secondTargetID;
	bool setSecondTarget(SceneEntryPk *);
	bool setSecondTarget(DWORD, DWORD);
	bool chaseSecondTarget();
	bool chaseItsMaster();
	SceneEntryPk * getSecondTarget();
	void hideMe(int showDelay);
	void goToRandomScreen();

	/**
	 * \brief ��Ʒ����
	 *
	 */
	DWORD dwNpcLockedUser;
protected:
	/// NPC�������˵ķ�Χ
	static const int npc_search_region = 5;
	/// NPCԶ��Ŀ�����׷�ٵķ�Χ
	static const int npc_lost_target_region = 12;
	/// NPCԶ����Χ����׷�ٵľ���
	static const int npc_out_of_range_region = 20;
	/// NPC��������Ŀ��ķ�Χ
	static const int npc_chase_region = 6;
	/// ���ﱣ����������ߵķ�Χ
	static const int npc_pet_chase_region = 2;
	/// ���������˳����˾��������
	static const int npc_pet_run_region = 4;
	/// ���������˳����˾�������ת
	static const int npc_pet_warp_region = 6;
	/// Զ��npc������Χ
	static const int npc_far_attack_range = 6;
private:
	/**
	 * \brief NPC�����
	 *
	 */
	bool clearMe;

	///npc��ǰ��AI
	t_NpcAIDefine AIDefine;

	///npc������
	//SceneUser * master;

	///�ƶ��ٶȱ���
	float speedRate;
	///�����ٶȱ���
	float aspeedRate;

	///�Ƿ��Ѿ���Ϊhp<20%�������ƶ��ٶ�
	bool speedUpUnder20;

	///�Ƿ��Ѿ���Ϊhp<50%�����˹����ٶ�
	bool aspeedUpUnder50;


	bool processDeath(SceneEntryPk *pAtt);

	///�Ƿ����ں�����(ms)
	int backOffing;

	/**
	 * \brief ��һ�α�����ʱ��(������
	 *
	 */
	/// ��һ�α�������ʱ��
	zRTime first_time;

	///���붨ʱ��
	Timer _half_sec;

	///1�붨ʱ��
	Timer _one_sec;

	///3�붨ʱ��
	Timer _3_sec;

	/**
	 * \brief ���ٷ�ʽ
	 *
	 */
	///npc�ĸ��ٷ�ʽ
	SceneNpcChase chaseMode;

	//DWORD	dwNpcChasedEntryType;
	/**
	 * \brief ������Ŀ��ĵı��
	 *
	 */
	//DWORD curTarget;
	//DWORD	dwNpcChasedEntryID;

	/**
	 * \brief ��Ʒ����ʱ��
	 *
	 */
	zRTime lockedUserTime;
	/**
	 * \brief ��һ���ƶ�ʱ��
	 *
	 */
	zRTime nextMoveTime;
	/**
	 * \brief ��һ�ι���ʱ��
	 *
	 */
	zRTime nextAttackTime;

	///��������״̬��ʱ��
	zRTime showTime;

	/**
	 * \brief �Ƿ���Ե�����Ʒ
	 *
	 */
	bool lostObject;

	//�Ƿ���й��﹥�ǵ��ж�
	bool mayRush;

	/**
	 * \brief Npc����
	 * ��̬�Ļ��Ƕ�̬�����
	 */
	const SceneNpcType type;

	/**
	 * \brief ��ʱ��ŵ����Է�����
	 * ��Ҫ�ڴ�����̬Npc��ʱ����Ҫʹ��
	 */
	static DWORD serialID;
	/**
	 * \brief ��ʱ��ŵ�Ψһ������
	 * ��Ҫ�ڴ�����̬Npc��ʱ����Ҫʹ��
	 */
	static zUniqueDWORDID uniqueID;

	//typedef __gnu_cxx::hash_map<DWORD, t_expRec> NpcHpHashmap;
	typedef std::map<DWORD, t_expRec> NpcHpHashmap;
	typedef NpcHpHashmap::iterator NpcHpHashmap_iterator;
	typedef NpcHpHashmap::const_iterator NpcHpHashmap_const_iterator;
	typedef NpcHpHashmap::value_type NpcHpHashmap_pair;
	///����ֵ�б�
	///���Էֵ���npc���������б�
	NpcHpHashmap expmap;


public:
	/**
	 * \brief ���ý�ɫ�ĵ�ǰ״̬�������ݵ�ǰ״̬���ֽ�ɫ����Ч���߹ر���Ч
	 * \param state ״̬ID ����enum SceneEntryStateȡֵ
	 * \param isShow �Ƿ���ʾЧ��
	 * \author fqnewman
	 * \return trueΪ��ι�������Ч�ģ�falseΪһ����Ч�Ĺ���
	 */
	void showCurrentEffect(const WORD &state, bool isShow,bool notify=true);
	/**
	 * \brief NPCʬ���ʹ��״̬
	 * trueΪ�Ѿ���ʹ��
	 */
	bool isUse;
};

#endif

