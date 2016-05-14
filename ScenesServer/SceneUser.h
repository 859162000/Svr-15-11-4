#ifndef _SCENEUSER_H_
#define _SCENEUSER_H_

#include "zObject.h"
#include "zUser.h"
#include "zNullCmd.h"
#include "Command.h"
#include "CharBase.h"
#include "ObjectManager.h"
#include "RecordCommand.h"
#include "zTime.h"
#include "SceneTask.h"
#include "Trade.h"
#include "SkillManager.h"
#include "TeamManager.h"
#include "Quest.h"
#include "Mask.h"
#include "SceneEntryPk.h"
#include "Leechdom.h"
#include "Visitor.h"
#include "SceneNpc.h"
#include "ScenePet.h"
#include "Horse.h"
#include "SessionCommand.h"
#include "MiniUserCommand.h"


class Scene;
class GuardNpc;
class CartoonPet;
class Dice;

struct SeptGuard
{
	DWORD id; // ���ID
	DWORD money; // ��ҿ۵�Ѻ��
	bool  is_finish;

	SeptGuard()
	{
		id = 0;
		money = 0;
		is_finish = false;
	}
};

struct WarRecord
{
	DWORD type;                     // ��ս����
	DWORD relationid;               // �Է�����ϵID
	bool isAtt;                     // �Ƿ�Ϊ����
	bool isAntiAtt;			// �Ƿ���������Ŀǰ��Ҫ�ǹ�սʹ�ã�

	WarRecord()
	{
		type=0;
		relationid=0;
		isAtt = false;
		isAntiAtt = false;
	}
};

extern DWORD get_sell_price_by_dur(zObject*);

struct SceneUser:public SceneEntryPk
{
	private:
		char replyText[MAX_CHATINFO];//�Զ��ظ�

		//������ʱ�ȵ�(����)
		zRTime recycle_delay;

		static DWORD Five_Relation[];
		/*
		   zRTime tenSecondTime;
		   zRTime oneSecondTime;
		   zRTime oneMinuteTime;
		 */
		Timer _half_sec;
		Timer _one_sec;
		Timer _five_sec;
		Timer _ten_sec;
		Timer _one_min;
		Timer _five_min;
		//�浵��ʱ�����浵��϶��������ԣ����Դ��Ҵ浵Ƶ��
		RandTimer _writeback_timer;


		///5�������ÿ5��+1
		DWORD _5_sec_count;

		/// ɱ���������
		DWORD killedNpcNum;
		const zRTime loginTime;
		/// ����ʱ��ͳ��
		DWORD lastIncTime;
		/// �ϴ��ѹ�ʱ���¼
		DWORD lastChangeCountryTime;
		/// ��ʱ�رձ���
		DWORD temp_unsafety_state;
		/// �Ʋ�����״̬
		BYTE  safety;
		/// �Ʋ���������
		BYTE safety_setup;

		/// �����ȴ�ʱ�䣬������5���ӣ�ϵͳ�Զ�ʹ��ɫ���лسǸ���
		int deathWaitTime;

		//�Ƿ����ں�����(ms)
		int backOffing;

		//�û�Ȩ�޵ȼ�
		BYTE priv;

		// ��ս��¼
		std::vector<WarRecord> vWars;
		
		// ���Ͳ����������飩
	//	DWORD Give_MatarialNum;

		char wg_log[MAX_CHATINFO];
		DWORD wg_log_len;
		void getWgLog(DWORD);
	public:
		//must be deleted by Visitor
		~SceneUser();
		void destroy();

		// ����������ڵ����ID
		std::vector<SeptGuard> venterSeptGuard;
		// ����������ڵ����ID
		//std::vector<SeptGuard> vfinishSeptGuard;

	public:
		// ���Ͳ����������飩
		DWORD Give_MatarialNum;
		
		//���߿�����
		DWORD Card_num;

		Dice * miniGame;//�������С��Ϸ

		void petAutoRepair(zObject *);//�����Զ���װ��

		DWORD lastKiller;//���һ��ɱ���Լ�����ҵ���ʱID
		DWORD dropTime;//�ͻ��˾ٱ���Һ�������ߵ�ʱ��

		enum
		{
			ACCPRIV_NEWBIE_EQUIP_AT_5_15		= 0x00000001,	//5��15��ʱ��������װ��
			ACCPRIV_GOLD_EQUIP_AT_5_15			= 0x00000002,	//5��15��ʱ����ƽ�װ��
			ACCPRIV_GREEN_EQUIP_AT_5_25_50		= 0x00000004	//5��25��50��ʱ������ɫװ��
		};

		void sendGiftEquip(WORD level);//�����鿨���ƽ𿨣���ɫ����װ��
		void givePetPoint();//30,40,50,60���ͱ�������ʱ��

		DWORD processCheckTime;//�´μ����ҵ�ʱ��

		DWORD refreshPetPackSize();

		bool npcTradeGold(Cmd::stBuyObjectNpcTradeUserCmd *ptCmd, zObjectB *b, BYTE itemlevel);
		void autoReply(char *) const;

		DWORD adoptedCartoon;   //�������ĳ���ID
		std::map<DWORD, Cmd::t_CartoonData> cartoonList;
		CartoonPet * cartoon;//�ų��Ŀ�ͨ����
		std::map<DWORD, CartoonPet *> adoptList;//�����Ŀ�ͨ����

		typedef std::map<DWORD, Cmd::t_CartoonData>::iterator cartoon_it;
		typedef std::map<DWORD, CartoonPet *>::iterator adopt_it;

		//�´ο��Բ�ѯ���ݿ��ʱ�䣬������
		DWORD queryTime;

		bool isSendingMail;
		bool isGetingMailItem;

		/*      
		 ** whj 
		 ** �����˳�״̬���ٽ����κ�ָ��
		 **/
		bool unReging;
#ifdef _TEST_DATA_LOG
		//��������ͳ��
		CharTest chartest;
		void readCharTest(Cmd::Record::t_Read_CharTest_SceneRecord *rev);
		void writeCharTest(Cmd::Record::enumWriteBackTest_Type type);
#endif
		// PKģʽ
		BYTE pkMode;
		//ϵͳ����
		union{
			struct{
				BYTE savePkMode;
				BYTE setting[19];
			};
			BYTE sysSetting[20];
		};
		DWORD chatColor[8];

		static const unsigned int CALL_PET_REGION = 6;

		///��
		Horse horse;
		ScenePet * ridepet;

		//�ڹ����������������˳�,д����^_^
		DWORD deathBackToMapID;

		//�������ͼ��Sessionδ����ʱ����ʱ��¼
		char wait_gomap_name[MAX_NAMESIZE];

		void setDiplomatState(BYTE newstate);
		int isDiplomatState(); // ����0Ϊ�⽻��״̬,����1Ϊ���⽻��״̬,����2Ϊ���⽻�ٵ���Ϊ�вɼ�����,��ʱ��Ч

		void setCatcherState(BYTE newstate);
		bool isCatcherState() const; 
		// ��ս��¼����
		typedef std::vector<WarRecord>::iterator WarIter;

		void addWarRecord(DWORD type, DWORD relationid, bool isAtt=false);
		void removeWarRecord(DWORD type, DWORD relationid=0);
		
		void clearWarRecord()
		{
			vWars.clear();
		}
		
		void setAntiAttState(DWORD type, DWORD relationid);
		//   type,��ս״̬��relationid�Եз�����ϵID
		bool isWarRecord(DWORD type, DWORD relationid);


		/// �ж��Ƿ��ڲƲ�����״̬
		// ����TRUE,��ʾָ���Ĺ���,�����ڱ���״̬, ����FALSE��ʾָ���Ĺ���û�ڱ���״̬
		bool isSafety(BYTE byType);
		

		// �ж��Ƿ���ĳλ��Ҵ��ڽ�ս״̬
		bool isWar(SceneUser* entry);

		// �ж�����Լ��Ƿ���ָ�����͵Ľ�ս״̬
		bool isSpecWar(DWORD dwType);

		// �ж��Ƿ��ǹ���
		bool isAtt(DWORD dwType, DWORD relationid=0);

		// �ж��Ƿ�������
		bool isAntiAtt(DWORD dwType, DWORD relationid=0);

		// ��ǰ��ս��¼��С
		size_t warSize()
		{
			return vWars.size();
		}

		DWORD updateNotify; //hp,mp,sp ����֪ͨ
		BYTE updateCount;  //���¼���
		
		// ����״̬
		bool isQuiz;			// �Ƿ��ڴ���״̬

		// �⽻��״̬
		bool isDiplomat;

		// ��ͷ״̬
		bool isCatcher;


		// �ж��Ƿ���ĳλ��Ҵ��ڽ�ս״̬

		//�ٻ�����
		ScenePet * summonPet(DWORD id, Cmd::petType type, DWORD standTime=0, DWORD sid=0, const char * petName = "", DWORD anpcid = 0, zPos pos=zPos(0,0), BYTE dir=4);
		//�ٻ�ָ������ʱ��ĳ���
		//SceneNpc * summonPet(DWORD id, DWORD standTime, DWORD anpcid = 0);
		bool killOnePet(ScenePet *);
		void killAllPets();
		void killSummon();
		void collectPets();
		void clearGuardNpc();

		Cmd::t_PetData petData;//����ץ�ĳ�������
		DWORD savePetState(unsigned char *data);
		DWORD loadPetState(unsigned char *data, int size);
		bool saveGuard;///�Ƿ񱣴��ڳ�����ʱ����
		bool saveAdopt;///�Ƿ񱣴�
		DWORD saveTempPetState(unsigned char *data, DWORD maxSize);
		DWORD loadTempPetState(unsigned char *data);
		void saveCartoonState();

		DWORD dwChangeFaceID;				//���ݺ��NPC������ID

		enum {
			REST_RESTITUTE_SP = 3, //�Զ������ָ�
			WALK_RESTITUTE_SP = 1, //��·��������
			RUN_CONSUME_SP = 1, //�ܲ���������
		};
		WORD step_state; 

		DWORD accid;
		SceneTask *gatetask;
		CharBase charbase;
		CharState charstate;
		//const zCharacterB *charconst;
		WORD wdTire;
		WORD wdTirePer;
		bool sitdownRestitute;
		bool npcdareflag;
		DWORD npcdarePosX;
		DWORD npcdarePosY;
		DWORD npcdareCountryID;
		DWORD npcdareMapID;
		bool npcdareNotify;

		WORD  lastUseSkill; // ���һ��ʹ�õļ���
		bool  farAttack; // �������ר��

		//DWORD dwHorseID;
		DWORD dwBodyID;
		DWORD dwLeftHandID;
		DWORD dwRightHandID;
		DWORD dwBodyColorSystem;
		DWORD dwBodyColorCustom;

		char  unionName[MAX_NAMESIZE];          // �������
		char  septName[MAX_NAMESIZE];           // ��������
		char  caption[MAX_NAMESIZE];		// ���������ͷ��
		char  armyName[MAX_NAMESIZE];		// ���ھ�������


		bool  king; // �Ƿ�Ϊ����
		bool  unionMaster; // �Ƿ�Ϊ����
		bool  septMaster;  // �Ƿ�Ϊ�峤
		bool  emperor;	// �Ƿ�Ϊ�ʵ�
		BYTE  kingConsort; // 0���ǹ����ͻʵ۵���ż, 1Ϊ��������ż, 2Ϊ�ʵ۵���ż
		
		DWORD dwSeptRepute;	// ��������
		DWORD dwSeptLevel;	// ����ȼ�
		DWORD dwUnionActionPoint;	// ����ж���
		DWORD dwArmyState; // �ھ����е�ְλ


		bool answerMarry; ///ͬ����ż��������־
		DWORD friendID;   ///���ʱδ����ż��ID

		//��Ʒ
		Packages packs;

		//ҩƷ
		Leechdom leechdom;

		QuestList quest_list;
		Mask mask;

		//���
		TeamManager team;
		DWORD team_mode;//���ģʽ

		//���׵�
		TradeOrder tradeorder;
		PrivateStore privatestore;

		int messageOrder;					///�û���һ���յ���Message˳��
		zRTime lastCheckMessage;			///���һ�μ��Messageʱ��
		DWORD	lastMoveTime;
		DWORD   moveFastCount;

		//����Npc����Ϣ
		DWORD npc_dwNpcDataID;				///Npc���ݱ��
		DWORD npc_dwNpcTempID;				///Npc��ʱ���

		ScenePet* guard; //����

		struct{
			DWORD dwMapID;
			DWORD dwPosX;
			DWORD dwPosY;
		} npcHoldData;		/// ������Ƶ�NPC�������

		//��ݼ�����
		char accelData[1024];

		DWORD myOverMan;
		//�ҵ��Լ���ʦ��id,û�з���0
		DWORD getOverMan()
		{
			return myOverMan;
		}
		bool canVisitNpc(SceneNpc *pNpc);
		void visitNpc(const DWORD npcID, const DWORD npcTempID)
		{
			npc_dwNpcDataID = npcID;
			npc_dwNpcTempID = npcTempID;
		}

		bool checkVisitNpc(const DWORD npcID) const
		{
			return npcID == npc_dwNpcDataID;
		}

		void cancelVistNpc()
		{
			npc_dwNpcTempID = 0;
			npc_dwNpcDataID = 0;
		}

		SceneUser(const DWORD accid);

		void setHairType(const BYTE hairtype)
		{
			DWORD temp = hairtype;
			temp <<= 24;
			charbase.hair = (temp & HAIRTYPE_MASK) | (charbase.hair & HAIRRGB_MASK);
		}

		BYTE getHairType() const
		{
			return ((charbase.hair & HAIRTYPE_MASK) >> 24);
		}

		void setHairColor(const DWORD rgb)
		{
			charbase.hair = (rgb & HAIRRGB_MASK) | (charbase.hair & HAIRTYPE_MASK);
		}

		DWORD getHairColor() const
		{
			return (charbase.hair & HAIRRGB_MASK);
		}

		inline bool checkMessageTime(const zRTime &ct)
		{
			if (ct >= lastCheckMessage)
			{
				lastCheckMessage = ct;
				lastCheckMessage.addDelay(2000);
				return true;
			}
			else
				return false;
		}

		bool speedOutMove(DWORD time, WORD speed, int len);

		bool isNewCharBase() const
		{
			return !(CHARBASE_OK & charbase.bitmask);
		}

		bool IsPking()
		{
			return lastPkTime ? true : false;
		}
		bool getMagicType();
		int IsOppose(DWORD five);
		bool IsJoin(DWORD five);
		/*
		   bool tenSecondTimer(const zRTime &ct);
		   bool oneSecondTimer(const zRTime &ct);
		   bool oneMinuteTimer(const zRTime &ct);
		 */
		inline bool checkGoodnessTime(const zRTime &ct);
		void checkPunishTime();
		void reduceGoodness(SceneNpc *pNpc);
		void initCharBase(Scene *intoscene);

		void calReliveWeaknessProperty(bool enter);
		void enterSeptGuard();
		void finishSeptGuard();

		void setupCharBase(bool lock=true);
		bool upgrade(DWORD num = 0);
		void setStateToNine(WORD state);
		void clearStateToNine(WORD state);
		void sendGoodnessToNine();
		void sendMeToNine();
		void sendMeToNineDirect(const int direct);
		void sendNineToMe();
		void sendNineToMeDirect(const int direct);
		void sendPetDataToNine();
		void sendInitToMe();
		void addNineSeptExp(DWORD dwSeptID);
		void addNineSeptNormalExp(DWORD dwSeptID);

		void checkAutoMP();
		bool checkUserCmd(const Cmd::stNullUserCmd *, const unsigned int nCmdLen) const;
		bool checkChatCmd(const Cmd::stNullUserCmd *, const unsigned int nCmdLen) const;
		void sendCmdToMe(const void *pstrCmd, const unsigned int nCmdLen);
		void sendCmdToBill(const void *pstrCmd, const unsigned int nCmdLen);
		void sendSceneCmdToBill(const void *pstrCmd, const unsigned int nCmdLen);
		bool unreg();
		bool requestUser(Cmd::stRequestUserDataMapScreenUserCmd *rev);
		bool requestNpc(Cmd::stRequestMapNpcDataMapScreenUserCmd *rev);
		bool save(const Cmd::Record::WriteBack_Type writeback_type);
		bool attackMagic(const Cmd::stAttackMagicUserCmd *rev, const unsigned int cmdLen);
		bool reliveReady(const Cmd::stOKReliveUserCmd *rev , bool isOrigin = false);
		bool relive(const int relive_type , const int delaytime , const int data_percent=100);
		//void Death();
		void setDeathState();
		void lostObject(SceneUser *pAtt = NULL);
		bool switchPKMode(const Cmd::stPKModeUserCmd *rev);
		bool checkGoodness();		
		char *getGoodnessName();
		short getGoodnessState() const;
		bool isPkAddition();
		DWORD getPkAddition();
		bool isRedNamed(bool allRedMode=true) const;
		float getGoodnessPrice(DWORD price , bool isBuy);
		bool move(Cmd::stUserMoveMoveUserCmd *rev);
		bool backOff(const int dir, const int grids);
		bool goTo(zPos &newPos);
		void jumpTo(zPos &newPos);
		bool goToRandomRect(zPos center , WORD rectx = SCREEN_WIDTH , WORD recty = SCREEN_HEIGHT);
		bool ride(Cmd::stRideMapScreenUserCmd *rev);
		bool useObject(zObject *obj);
		bool useLeechdom(zObject *obj);
		bool useScroll(zObject *obj);
		bool useCallObj(zObject *obj);
		bool useAmulet(zObject *obj);
		bool useSkill(zObject *obj);
		bool doPropertyCmd(const Cmd::stPropertyUserCmd *rev,unsigned int cmdLen);
		bool doChatCmd(const Cmd::stChatUserCmd *rev,unsigned int cmdLen);
		bool doTradeCmd(const Cmd::stTradeUserCmd *rev,unsigned int cmdLen); 
		bool doGMCmd(char *gmcmd);
		bool doCountryCmd(const Cmd::stCountryUserCmd *rev,unsigned int cmdLen);
		bool doArmyCmd(const Cmd::stArmyUserCmd *rev, unsigned int cmdLen);
		bool doDareCmd(const Cmd::stDareUserCmd *rev,unsigned int cmdLen);
		bool doNpcDareCmd(const Cmd::stDareUserCmd *rev,unsigned int cmdLen);
		bool doUnionCmd(const Cmd::stUnionUserCmd *rev,unsigned int cmdLen);
		bool doSafetyCmd(const Cmd::stSafetyUserCmd *rev,unsigned int cmdLen);
		bool doSeptCmd(const Cmd::stSeptUserCmd *rev,unsigned int cmdLen);
		bool doSchoolCmd(const Cmd::stSchoolUserCmd *rev,unsigned int cmdLen);
		bool doRelationCmd(const Cmd::stRelationUserCmd *rev,unsigned int cmdLen);
		bool doPetCmd(const Cmd::stPetUserCmd *rev,unsigned int cmdLen);
		bool doQuizCmd(const Cmd::stQuizUserCmd *rev,unsigned int cmdLen);
		bool doMailCmd(const Cmd::stMailUserCmd *rev,unsigned int cmdLen);
		bool doAuctionCmd(const Cmd::stAuctionUserCmd *rev,unsigned int cmdLen);
		bool doStockCmd(const Cmd::stStockSceneUserCmd *ptCmd,unsigned int cmdLen);
		bool doPrisonCmd(const Cmd::stPrisonUserCmd *rev,unsigned int cmdLen);
		bool doMiniGameCmd(const Cmd::stMiniGameUserCmd *rev,unsigned int cmdLen);
		bool stockSave(DWORD mon, DWORD go);
		bool doCartoonCmd(const Cmd::stCartoonUserCmd *rev,unsigned int cmdLen);
		void full_t_UserData(Cmd::t_UserData &data);
		void full_t_MapUserData(Cmd::t_MapUserData &data);
		void full_t_MapUserDataPos(Cmd::t_MapUserDataPos &data);
		void full_t_MapUserDataState(Cmd::t_MapUserDataState &data);
		void full_t_MapUserDataPosState(Cmd::t_MapUserDataPosState &data);
		void full_t_MainUserData(Cmd::t_MainUserData &data) const;
		void sendInitHPAndMp();
		bool addSkillData(const Cmd::stAddUserSkillPropertyUserCmd *rev);
		bool removeSkill(const Cmd::stRemoveUserSkillPropertyUserCmd *rev);
		bool upgradeSkill(DWORD dwSkillID,bool needSkillPoint=true);
		bool changeMap(Scene *newscene,const zPos &pos, bool ignoreUserLevel= false);
		bool intoScene(Scene *newscene,bool needInitInfo,const zPos &initPos);
		void setDeathBackToMapID(Scene *s);
		int saveSysSetting(unsigned char *);
		int loadSysSetting(unsigned char *);
		bool LeaveScene();
		bool refreshMe();
		void sendExpToSept(const WORD &exp);
		inline void dreadProcess();
		WORD getMyMoveSpeed() const;
		bool unCombin(Cmd::stUnCombinUserCmd *rev);
		void setNpcHoldData(Cmd::Session::t_notifyNpcHoldData *rev);
		inline void checkNpcHoldDataAndPutExp();
		void addObjectToUserPacket(int objectid, SceneUser *pUser);
		void catchMyPet();
		void removeNineEntry(zPosI posi);

		void addExp(DWORD, bool=true, DWORD dwTempID=0, BYTE byType=0, bool=false);
		void addPetExp(DWORD, bool=true, bool=false);
		//void petLevelUp(ScenePet *);
		/**
		 * \brief ����ս�������ĸ��ٹ���Ŀ��
		 */
		void setPetsChaseTarget(SceneEntryPk *entry);

		/**
		 * \brief ���ݼ�֧�ı仯���㹥������
		 * \calcflag ��־�Ƿ��ں��������м��㲢֪ͨ
		 * \author fqnewman
		 * \return trueΪ��Ҫ���㣬 falseΪ����Ҫ����
		 */
		//bool recalcBySword(bool calcflag = true);

		bool isAllied(SceneUser *pUser);

		/**
		 * \brief ��������и�������ϵ���Ѻö�
		 * \author fqnewman
		 */
		inline void countFriendDegree();

		/**
		 * \brief ���ͻ�����Ϣת�����Ự������
		 * \author fqnewman
		 */
		bool forwardSession(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);

		void full_stRTMagicPosUserCmd(Cmd::stRTMagicPosUserCmd &ret) const
		{
			ret.pos.dwTempID=tempid;
			ret.pos.byDir=getDir();
			ret.pos.x=getPos().x;
			ret.pos.y=getPos().y;
			ret.byTarget=Cmd::MAPDATATYPE_USER;
		}

		/**
		 * \brief ������������Ʒ
		 *
		 * \param srcObj Ŀ����Ʒ
		 * \param needFind ��Ҫ����λ��
		 * \param from_record 
		 * \param calcflag
		 *
		 * \author fqnewman
		 * \return ʧ�ܷ���false, ���򷵻�true
		 */
		//bool packsaddObject(zObject *srcObj,bool needFind , bool from_record = false, bool calcflag = true);

		/**
		 * \brief ������Ʒid������Ʒ�ڰ����е�����
		 *
		 * \param id ��Ʒ��objectid
		 * \param num ���ٵ�����
		 * \param upgrade ����
		 *
		 * \author liqingyu 
		 *
		 * \return ʧ�ܷ���-1, ���򷵻�0
		 */
		int reduceObjectNum(DWORD id, DWORD num, BYTE upgrade = 0);

		/**
		 * \brief ������Ʒid������Ʒ�ڰ����е�����,������ڸ���Ʒ��������򴴽�һ������Ʒ
		 *
		 * \param id ��Ʒ��objectid
		 * \param num ���ӵ�����
		 * \param upgrade ����
		 * \param notify ֪ͨ
		 * \param bindit �Ƿ��
		 *
		 * \author liqingyu 
		 *
		 * \return ʧ�ܷ���-1, û����Ʒ����������0, ����������1, �ɹ�������д�������Ʒ����2
		 */
		int addObjectNum(DWORD id, DWORD num, BYTE upgrade = 0, int notify = 0, bool bindit = false);


		/**
		 * \brief ������Ʒid������Ʒ�ڰ����е�����,������ڸ���Ʒ��������򴴽�һ������Ʒ������ӿڣ�����װ��װ��
		 *
		 * \param id ��Ʒ��objectid
		 * \param num ���ӵ�����
		 * \param upgrade ����
		 * \param notify ֪ͨ
		 * \param bindit �Ƿ��
		 *
		 * \author liqingyu 
		 *
		 * \return ʧ�ܷ���-1, û����Ʒ����������0, ����������1, �ɹ�������д�������Ʒ����2
		 */
		int addGreenObjectNum(DWORD id, DWORD num, BYTE upgrade = 0, int notify = 0, bool bindit = true);


		/**
		 * \brief �Զ��ָ��û�������ԣ�����������ħ���������ȣ��ú�����UpdateSceneUserCallBack�б�����
		 * \author liqingyu
		 */
		inline void restitute();
		DWORD autoRestitute(DWORD &updated);

		/**
		 * \brief �����û����ݵ��Ự
		 * \author zjw
		 */
		void updateUserData();

		/**
		 * \brief �ı��ɫ��hp
		 *
		 * \param hp �����HP
		 *
		 * \author fqnewman
		 */
		void changeHP(const SDWORD &hp);

		/**
		 * \brief ֱ���˺�
		 *
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
		 * \brief �������hp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		DWORD getMaxHP();

		/**
		 * \brief �������hp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		DWORD getBaseMaxHP();

		/**
		 * \brief �������mp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		DWORD getMaxMP();

		/**
		 * \brief �������mp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		DWORD getBaseMaxMP();
		/**
		 * \brief ���㲢֪ͨ
		 * \author fqnewman
		 */
		void changeAndRefreshHMS(bool lock=true, bool sendData=true);
		/**
		 * \brief ��õ�ǰħ��������
		 * \author fqnewman
		 * \return ħ��������
		 */
		virtual DWORD getMaxMDamage();

		/**
		 * \brief ��õ�ǰ��������
		 * \author fqnewman
		 * \return ��������
		 */
		virtual DWORD getMaxPDamage();

		/**
		 * \brief ��õ�ǰ���������
		 * \author fqnewman
		 * \return ���������
		 */
		virtual DWORD getPDefence();

		/**
		 * \brief ��õ�ǰħ��������
		 * \author fqnewman
		 * \return ħ��������
		 */
		virtual DWORD getMDefence();

		/**
		 * \brief �ڱ��Լ�����֮ǰ�Ĵ���������װ����Ĵ���������Ч�����жϵ�
		 * \param pUser ������
		 * \param rev ���ι����Ĵ���ָ��
		 * \param physics
		 * \param good
		 * \author fqnewman
		 * \return trueΪ��ι�������Ч�ģ�falseΪһ����Ч�Ĺ���
		 */
		bool preAttackMe(SceneEntryPk *pUser, const Cmd::stAttackMagicUserCmd *rev, bool physics=true, const bool good = false);

		/**
		 * \brief ��ɫ������
		 * \param pEntry ������
		 * \param rev ���ι����Ĵ�����Ϣ
		 * \author fqnewman
		 * \return trueΪ��ι�������Ч�ģ�falseΪһ����Ч�Ĺ���
		 */
		//		virtual bool AttackMe(SceneEntryPk *pEntry, const Cmd::stAttackMagicUserCmd *rev, bool physics=true);

		/**
		 * \brief �ý�ɫ����
		 * \author fqnewman
		 */
		virtual void relive();

		/**
		 * \brief ��ɫ������N��
		 * \param dwAttTempID �����ߵ���ʱID
		 * \param grids
		 * \author fqnewman
		 */
		void standBack(const DWORD dwAttTempID, DWORD grids);

		/**
		 * \brief ��ȡ��ʱ�浵����
		 * \param revData �õ�����ʱ����
		 * \param dwSize ��ʱ���ݵĴ�С
		 */
		void setupTempArchive(const char *revData , const DWORD dwSize);

		DWORD setupBinaryArchive(const char *revData);
		bool needSaveBinayArchive(BinaryArchiveType type);
		DWORD addBinaryArchiveMember(DWORD type , char *out , DWORD maxSize);
		DWORD saveBinaryArchive(unsigned char *out , const int maxsize);
		/**
		 * \brief �ý�ɫ����
		 * \author fqnewman
		 */
		void toDie(const DWORD &dwTempID);

		/**
		 * \brief ��Ļ������ƶ�
		 * \author fqnewman
		 */
		void goToRandomScreen();

		/**
		 * \brief ֪ͨ�ͻ�������ֵ�ı仯
		 * \author fqnewman
		 */
		void attackRTHpAndMp();

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
		DWORD getLevel() const;

		/**
		 * \brief ��ȡ��ɫ����������
		 * \author fqnewman
		 * \return ��������
		 */
		DWORD getFiveType() const;

		/**
		 * \brief ��ȡ��ɫ�����е���
		 * \author fqnewman
		 * \return ���е���
		 */
		DWORD getFivePoint() const;

		/**
		 * \brief ��ȡ��ɫ�����м���
		 * \author fqnewman
		 * \return ���м���
		 */
		DWORD getFiveLevel() const;

		/**
		 * \brief ��Ҫ��ְҵ���ͣ���������ʹ�õļ�������
		 * \author fqnewman
		 */
		bool addSkillToMe(zSkill *skill);

		/**
		 * \brief ��Ҫ��ְҵ���ͣ���������ʹ�õļ�������
		 * \author fqnewman
		 */
		bool needType(const DWORD &needtype);

		/**
		 * \brief �Ƿ��иü�����Ҫ������
		 * \author fqnewman
		 * \return true �� false û��
		 */
		bool needWeapon(DWORD skillid);

		/**
		 * \brief ��ȡ��ǰ�ֳֵ���������
		 * \author fqnewman
		 * \return ��������
		 */
		BYTE getWeaponType();

		/**
		 * \brief �Ƿ�Pk����
		 * \param other �����
		 * \author fqnewman
		 * \return true �� false ��
		 */
		bool isPkZone(SceneEntryPk *other=NULL);

		/**
		 * \brief ������Ʒ�����ͷ���
		 * \param object ������Ʒ������
		 * \param num ������Ʒ������
		 * \author fqnewman
		 * \return true ���ĳɹ� false ʧ��
		 */
		bool reduce(const DWORD &object, const BYTE num);

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
		int isEnemy(SceneEntryPk *entry, bool notify = false, bool good = false);

		/**
		 * \brief ��ȡ�Լ������ˣ�һ�����NPC���ԣ�Player���������Լ�
		 * \author fqnewman
		 * \return NULL�������˵Ķ���ָ��
		 */
		SceneEntryPk *getMaster();
		/**
		 * \brief �õ����ϲ������
		 * �����Լ�
		 * \return  �Լ�
		 */
		SceneEntryPk *getTopMaster();// const;

		/**
		 * \brief ����Ƿ��ڽ�����
		 * 
		 * \return ����ڽ����ڷ���true,����false
		 */ 
		bool isForbidTalk()
		{
			zRTime ctv;
			return ctv.sec() < charbase.forbidtalk;
		}
		/** 
		 * \brief ���ý���ʱ��
		 * \param delay �ӳٵ�����
		 * \return ���Ե��ڵ�ʱ��()
		 */
		zRTime delayForbidTalk(int delay)
		{
			zRTime ctv(delay*1000);
			charbase.forbidtalk = ctv.sec();
			return ctv;
		}

		/**
		 * \brief ������ҵ�Ȩ��
		 * \param lv �ȼ�
		 */
		void setPriv(BYTE lv)
		{
			priv = lv;
		}

		/**
		 * \brief �õ���ҵ�Ȩ�޵ȼ�
		 * \return ��ҵ�Ȩ�޵ȼ�
		 */
		BYTE getPriv()
		{
			return priv;
		}

		/**
		 * \brief ���·��ͱ���ɫ�ĵ�ͼ����
		 */
		void reSendMyMapData();

		/**
		 * \brief ֪ͨѡ���Լ����û���hp��mp�����仯
		 */
		void sendtoSelectedHpAndMp();
		void sendtoSelectedPkAdditionState();
		void sendtoSelectedPkAdditionStateToUser(SceneUser *pUser);
		void sendtoSelectedReliveWeakStateToUser(SceneUser *pUser);
		void sendtoSelectedReliveWeakState();
		void sendtoSelectedTrainStateToUser(SceneUser *pUser);
		void sendtoSelectedTrainState();


		/**
		 * \brief ��ȡ��������  
		 */
		SWORD getPoisondef();

		/**
		 * \brief ��ȡ���������        
		 */
		SWORD getLulldef();

		/**
		 * \brief ��ȡ��ѣ������        
		 */
		SWORD getReeldef();

		/**
		 * \brief ��ȡ����ħ����        
		 */
		SWORD getEvildef();

		/**
		 * \brief ��ȡ����������        
		 */
		SWORD getBitedef();

		/**
		 * \brief ��ȡ����������        
		 */
		SWORD getChaosdef();

		/**
		 * \brief ��ȡ����������        
		 */
		SWORD getColddef();

		/**
		 * \brief ��ȡ��ʯ������        
		 */
		SWORD getPetrifydef();

		/**
		 * \brief ��ȡ��ʧ������        
		 */
		SWORD getBlinddef();

		/**
		 * \brief ��ȡ����������        
		 */
		SWORD getStabledef();

		/**
		 * \brief ��ȡ����������        
		 */
		SWORD getSlowdef();

		/**
		 * \brief ��ȡ���ջ�����
		 */
		SWORD getLuredef();


		/**
		 * \brief ��ȡ������  
		 */
		SWORD getPoison();

		/**
		 * \brief ��ȡ�������        
		 */
		SWORD getLull();

		/**
		 * \brief ��ȡѣ������        
		 */
		SWORD getReel();

		/**
		 * \brief ��ȡ��ħ����        
		 */
		SWORD getEvil();

		/**
		 * \brief ��ȡ��������        
		 */
		SWORD getBite();

		/**
		 * \brief ��ȡ��������        
		 */
		SWORD getChaos();

		/**
		 * \brief ��ȡ��������        
		 */
		SWORD getCold();

		/**
		 * \brief ��ȡʯ������        
		 */
		SWORD getPetrify();

		/**
		 * \brief ��ȡʧ������        
		 */
		SWORD getBlind();

		/**
		 * \brief ��ȡ��������        
		 */
		SWORD getStable();

		/**
		 * \brief ��ȡ��������        
		 */
		SWORD getSlow();

		/**
		 * \brief ��ȡ�ջ�����
		 */
		SWORD getLure();



		/**
		 * \brief ����ѡ����ҵ�״̬
		 * \param state ״̬
		 * \param value ֵ
		 * \param time ����ʱ��
		 * */
		void sendtoSelectedState(DWORD state , WORD value , WORD time);
		/**
		 * \brief ������ҵ�״̬���Լ�
		 * \param state ״̬
		 * \param value ֵ
		 * \param time ����ʱ��
		 * */
		void sendSevenStateToMe(DWORD state , WORD value , WORD time);

		/**
		 * \brief ��ȡװ���˺��ӳ�
		 * \author fqnewman
		 * \return �˺��ӳ�
		 */
		WORD getDamageBonus();

		/**
		 * \brief ����ս�������ĸ��ٹ���Ŀ��
		 * \param weaponType ��������
		 * \return true�������ͷ��ϣ�false�������Ͳ�����
		 */
		bool checkWeapon(BYTE weaponType);

		/*
		///����ս��״̬��ʱ��
		zRTime endBattleTime;
		///��ǰ�Ķ���
		SceneEntryPk * curTarget;

		void setEndBattleTime(const zRTime &, int);
		bool checkEndBattleTime(const zRTime &);
		void leaveBattle();
		 */


		// �������
		void leaveTeam();

		/**
		 * \brief �ж��Ƿ���Ի����û��ڴ�
		 * \param cur: ��ǰʱ��
		 * \return �Ƿ���Ի����ڴ�
		 * */
		bool canRecycle(const zRTime &cur)
		{
			return recycle_delay <= cur;
		}

		/**
		 * \brief ���ݴ���
		 * \param cmd ������Ϣ
		 * \param cmdLen ��Ϣ����
		 * \return true ����ɹ� false ʧ��
		 **/
		bool changeFace(const Cmd::stChangeFaceMapScreenUserCmd *cmd, const unsigned int cmdLen);

		/**
		 * \brief ����������Ӧ���ܵȼ�
		 * \param skilltype ���ܵ�����
		 * \param skillkind ���ܵ�ϵ��
		 * \return �����ļ��ܵǼ���
		 **/
		WORD skillUpLevel(WORD skilltype, WORD skillkind);

		/**
		 * \brief ����Ƿ�����
		 * \author fqnewman
		 * \return true ���� false û��
		 */
		bool checkMountHorse();
		inline void initTire();
		void  initAnswerCount();
		inline void processTire();
		void callProcessTire();
		inline void processAnswerCount();
		void sitdown();
		void standup();
		bool isSitdown();
		void getSummonAppendDamage(WORD &minDamage, WORD &maxDamage);
		void exchangeMeAndSummonPet();
		DWORD getMP();
		void clearMana();
		SceneEntryPk* getSceneEntryPk(DWORD entryType, DWORD entryID) const;
		void appendAttack(const Cmd::stAttackMagicUserCmd *rev);
		bool captureIt(SceneNpc *, BYTE);
		void leech(DWORD dwDam);
		void checkNpcDare(Cmd::Session::t_NpcDare_NotifyScene_SceneSession * rev);
		void checkNpcDareState();
		void notifySessionNpcDareResult();
		void npcDareGetGold(DWORD dwGold);
		void sendMessageToMe(const char *pattern);
		void reflectSkill(SceneEntryPk *pAtt, const Cmd::stAttackMagicUserCmd *rev);
		SWORD getWeaponPower(int powerkind);
		void relivePet();
		void sendSkill(WORD wdSkillID, BYTE level=1, DWORD target=0, BYTE attackType=Cmd::ATTACKTYPE_U2U, BYTE action=Cmd::Ani_Null);
		/**
		 * \brief ���ý�ɫ�ĵ�ǰ״̬�������ݵ�ǰ״̬���ֽ�ɫ����Ч���߹ر���Ч
		 * \param state ״̬ID ����enum SceneEntryStateȡֵ
		 * \author fqnewman
		 * \return trueΪ��ι�������Ч�ģ�falseΪһ����Ч�Ĺ���
		 */
		void showCurrentEffect(const WORD &state, bool isShow,bool notify=true);

	private:
		/**
		 * \brief ��������״̬ʱ�������˷���ϵͳ��ʾ
		 * \param entry ��������
		 * */
		void processMaskOnAttack(SceneEntryPk *entry);
		/**
		 * \brief ��������״̬ʱ������
		 * */
		void processMaskOnDefence();
		/**
		 * \brief �����˺��ۼ�ֵ
		 * 
		 * \param dwDam ������˺�ֵ
		 * \param dwDamDef ������
		 * \param physics �Ƿ�������
		 * \return
		 * */
		void processAddDam(int &dwDam, int &dwDamDef, bool physics);
		/**
		 * \brief ��������˺�
		 * \param dwDam �˺�ֵ
		 * \param dwDamDef ������
		 * \param �Ƿ�������
		 * 
		 * */
		void reduceDam(int &dwDam, int &dwDamDef, bool physics);
		/**
		 * \brief �����˺�����
		 * \param dwDam �˺�ֵ
		 * \param dwDamDef ������
		 * \param �Ƿ�������
		 * */
		void reflectDam(int &dwDamDef, int &dwDamSelf, bool physics);
		/**
		 * \brief �����������
		 * */
		bool processDeath(SceneEntryPk *pAtt);
		/**
		 * \brief hp��mpת��
		 * \param dwDamDef ������
		 * 
		 * */
		void hp2mp(int &dwDamDef);

		/**
		 * \brief ����Ԥ����ֵ
		 * 
		 * */
		void calPreValue();
		/**
		 * \brief ��������ֵ
		 * 
		 * */
		void setupCharm();
		/**
		 * \brief ���������ʱ����
		 * 
		 * */
		void saveTempArchive();
		/**
		 * \brief �洢�����ʱ�ĵ�
		 * \param type ��ʱ��������
		 * \param out �������ݿ�ʼ��λ��
		 * \param maxSize �������
		 * \return ���ӵ��ֽ�
		 * */
		DWORD addTempArchiveMember(DWORD type , char *out , DWORD maxSize);
		/**
		 * \brief ���ĳ���������Ƿ���Ҫ���浽��ʱ�ĵ�
		 * \param Ҫ�������������
		 * \return �Ƿ���Ҫ����
		 * */
		bool needSaveTempArchive(TempArchiveType type);

		zRTime& waitRecycle();
		void notifyEquipChange();

		bool do_trade_rs_cmd(const Cmd::stTradeUserCmd *rev,unsigned int cmdLen);
};


typedef Visitor<SceneUser, VisitorThread, 1> SceneUserVisitor;
extern void sendMail(char * fromName, DWORD fromID, char * toName, DWORD toID, BYTE type, DWORD money, zObject * o, char * text);

#endif
