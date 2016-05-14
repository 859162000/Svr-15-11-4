/**
 * \file
 * \version  $Id: SceneEntryPk.h $
 * \author 
 * \date 
 * \brief PK��ر������鷽��
 */

#ifndef _ZSCENEENTRYPK_H_
#define _ZSCENEENTRYPK_H_

#include <map>
#include <set>
#include <ext/pool_allocator.h>
#include "zSceneEntry.h"
#include "Zebra.h"
#include "ScenePk.h"
#include "SkillStatusManager.h"
#include "zSceneEntryIndex.h"

class Scene;
class zSkill;
class zSkillB;
struct SceneNpc;
class ScenePet;


/**
 * \brief ��ɫPK������,�д�����
 */
struct SceneEntryPk: public zSceneEntry
{
	public:
		typedef std::set<DWORD> SelectedSet;
		typedef SelectedSet::value_type SelectedSet_value_type;
		typedef SelectedSet::iterator SelectedSet_iterator;
		///��ѡ���б�
		SelectedSet selected;
		//zMutex selected_lock;
	public:
		/**
		 * \brief ���캯������ʼ��
		 *
		 *
		 * \param type ����
		 * \param state ��ʼ״̬
		 * \return 
		 */
		SceneEntryPk(SceneEntryType type, const SceneEntryState state = SceneEntry_Normal):zSceneEntry(type,state)
		{
			attackAction = true;
			moveAction = true;
			skillAction = true;
			ignoreDam = false;
			ignorePhysicDam = false; ///����������
			ignoreMagicDam = false;  ///ħ����������
			blind  = false;
			mplock = false;
			hplock = false;
			splock = false;
			maxattack=false;
			mdeftozero=false;   /// ħ���������0
			pdeftozero=false;   /// ħ���������0
			useRes=true;		// �ɷ�ʹ����Ʒ
			isAutoRestitute=true;// �Զ��ָ�HP,MP,SP
			frenzy = false; // ��״̬
			huntermark = false; //
			isPhysics =  false; // ���������
			dread = false;// ����
			dropweapon = false; //��е
			tosh = false; //��������
			stone = false; // ʯ��
			watchTrap = false; //�۲�����
			mhpd = false;
			blazeflag = false; // ����ϵ�����ӳɱ�־
			levinflag = false; // �׵�ϵ�����ӳɱ�־
			trapflag = false; // ����ϵ�����ӳɱ�־
			iceflag = false; // ��ϵ�����ӳɱ�־
			swapdamcharid = 0; //  ����ʩ���ߵ���ʱID
			swapdamcharid2 = 0; //�ذ����˺�ת��ʩ���ߵ���ʱID
			reSendData = false;
			changeface = false; // ���������Է��ʹ���NPC
			hideme = false; //�����Լ�
			icebox = false; //�����״̬
			liquidState = false; //Һ̬�����赲��
			resist = 0;
			dmpbyhp = 0; //��ħ����
			mdmtomp = 0; //ħ���˺�ת����ħ��ֵ
			mdmtompper = 0; //ħ���˺�ת����ħ��ֵ����
			icedam	= 0; //����������յ��˺�ֵ
			unitaryBattleKeepTime = 0;

			afreetMode = false; /// ��ħɱ���⣨���˱�����
			angelMode = false;  /// ��ʹ�޵У����ڶ�ħ����������
			isPhysicBang = false;///�Ƿ񱩻�
			isMagicBang = false;///�Ƿ񱩻�
			isHPhysicBang = false;///�Ƿ񱩻�
			isHMagicBang = false;///�Ƿ񱩻�
			assault = false;
			curMagicManType =0;
			curMagicManID =0;
			lastPkTime=0;
			bombskillId = 0;
			dietodam = 0;
			attacklow = false; ///������ȡ����
			killnow=0; ///һ����ɱ�ٷֱ�
			diewithme=0; //ѪծѪ��
			switchdie=0; //ͬ����ת������״̬
			summonsplit = 0; // ��ħ������������ѱ�־
			damtohpper = 0;	/// ��ɫ���˺�ת��������ֵ�ļ���;
			damtohpvalper = 0; /// ��ɫ���˺�ת��������ֵ�İٷֱ� 
			reSendOther=false; 
		}
		virtual ~SceneEntryPk(){};
		///pk״̬
		ScenePkState pkState;
		///PK�����б仯����ֵ
		PkValue pkValue;
		///���ܸ��ӵ�״̬
		SkillState skillValue;
		///PK������Բ������ֵ
		PkPreValue pkpreValue;

		SkillStatusManager skillStatusM;	/// ����״̬������
		SkillStatusCarrier carrier;			/// ���ܲ���Ͷ����
		//std::map<BYTE,BYTE>	_effectState;	/// ����״̬��Ч������
		SceneEntryPk *attackTarget;			/// ������������״̬����Ŀ��

		bool attackAction;	/// �������
		bool moveAction;	/// �ƶ����
		bool skillAction;   /// ���ܹ������
		bool ignoreDam;		/// �����˺�
		bool ignorePhysicDam; /// ���������˺�
		bool ignoreMagicDam; /// ����ħ���˺�
		bool blind;        /// ʧ��״̬
		bool notifyHMS;		/// ֪ͨHP,MP,SP�ı�
		bool maxattack;    /// ����ȡ����
		bool mdeftozero;   /// ħ���������0
		bool pdeftozero;   /// ħ���������0
		bool useRes;		/// �ɷ�ʹ����Ʒ
		bool isAutoRestitute; // �Ƿ��Զ��ָ�
		bool frenzy;       /// ��״̬
		bool huntermark;	/// ���ֱ��
		bool isPhysics;		/// ���㹫ʽ������
		bool dread;			/// �־�
		bool dropweapon;   /// ��е
		bool tosh;        /// ��������
		bool stone;			/// ʯ��
		bool watchTrap;    /// �۲�����
		bool mhpd;  /// ħ���˺����룬�����˺��ӱ���־��
		bool blazeflag; ///����ϵ�����ӳɱ�־
		bool levinflag;  ///�׵�ϵ�����ӳɱ�־
		bool trapflag;  ///����ϵ�����ӳɱ�־
		bool iceflag;  ///��ϵ�����ӳɱ�־
		DWORD swapdamcharid; ///����ʩ���ߵ���ʱID
		DWORD swapdamcharid2; ///�ذ�ʩ���ŵ���ʱID
		bool changeface;  ///�����������Է��ʹ���NPC
		bool hideme;     /// �����Լ�
		bool icebox;	 /// �����״̬
		bool liquidState; ///Һ̬�����赲��
		BYTE resist;     ///�񵵣���һ�ε����˺��Ļ���
		BYTE dmpbyhp;  ///��ħ����
		BYTE mdmtomp;  ///�������˺�ת��ħ��ֵ�ٷֱ�
		BYTE mdmtompper; ///�������˺�ת��ħ��ֵ�ٷֱȼ���
		SDWORD icedam;  ///����������յ��˺���ֵ
		unsigned long long unitaryBattleKeepTime; //һԪ�󷨳���ʱ��
		SDWORD dietodam; ///����������ֵ
		bool attacklow; ///������ȡ����
		BYTE killnow; //һ����ɱ
		BYTE diewithme; //ѪծѪ��
		BYTE switchdie; //ͬ��ת������״̬�ļ���
		BYTE summonsplit; //��ħ������������ѱ�־


		bool reSendData;	/// ���·���NPC���ɫ���ݸ��Լ�
		bool reSendOther;	/// ���·���NPC���ɫ���ݸ�������

		bool mplock;	///��mp��������
		bool hplock;	///��hp��������
		bool splock;	///��sp��������

		bool afreetMode; /// ��ħɱ���⣨���˱�����
		bool angelMode;  /// ��ʹ�޵У����ڶ�ħ����������

		bool isPhysicBang; /// �Ƿ�������
		bool isMagicBang; /// �Ƿ�ħ������
		bool isHPhysicBang; /// �Ƿ�������
		bool isHMagicBang; /// �Ƿ�ħ������

		bool assault; ///���

		zPos keepPos;  /// ���漼�ܵĹ���Ŀ��㣬���ݸ���Ҫʹ�õĹ���
		BYTE keepDir;  /// ���漼�ܵĹ������򣬴��ݸ���Ҫʹ�õĹ���
		BYTE  lastPkTime; //���PK��־
		DWORD bombskillId; /// ����������Ͷ�ͼ���ID

		BYTE damtohpper;	/// ��ɫ���˺�ת��������ֵ�ļ���;
		BYTE damtohpvalper; /// ��ɫ���˺�ת��������ֵ�İٷֱ� 


		///���ڵĳ���ָ��
		Scene *scene;

		//����
		UserSkillM usm;

		virtual void sendtoSelectedHpAndMp() = 0;
		virtual void sendtoSelectedState(DWORD state , WORD value , WORD time) = 0;
		virtual void sendSevenStateToMe(DWORD state , WORD value , WORD time){}
		void sendCmdToSelected(const void *pstrCmd, const unsigned int nCmdLen);

		/**
		 * \brief ���ֱ���˺�
		 * \param pAtt ������
		 * \param dam �˺�
		 * \param notify �Ƿ�֪ͨ�ͻ���
		 * \author fqnewman
		 * \return �˺�ֵ
		 */
		virtual SWORD directDamage(SceneEntryPk *pAtt, const SDWORD &dam, bool notify=false);

		/**
		 * \brief �ı��ɫ��hp
		 * \param hp �����HP
		 * \author fqnewman
		 */
		virtual void changeHP(const SDWORD &hp)=0;

		/**
		 * \brief �ı��ɫ��sp
		 * \param sp �����SP
		 * \author fqnewman
		 */
		virtual void changeSP(const SDWORD &sp)=0;

		/**
		 * \brief �ı��ɫ��mp
		 * \param mp �����MP
		 * \author fqnewman
		 */
		virtual void changeMP(const SDWORD &mp)=0;

		/**
		 * \brief �������hp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		virtual DWORD getMaxHP(){return 0;}

		/**
		 * \brief �������hp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		virtual DWORD getBaseMaxHP(){return 0;}

		/**
		 * \brief �������mp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		virtual DWORD getMaxMP(){return 0;}

		/**
		 * \brief �������mp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		virtual DWORD getBaseMaxMP(){return 0;}

		/**
		 * \brief �������sp
		 * \author fqnewman
		 * \return �������ֵ
		 */
		virtual DWORD getMaxSP(){return 0;}

		/**
		 * \brief ��õ�ǰħ��������
		 * \author fqnewman
		 * \return ħ��������
		 */
		virtual DWORD getMaxMDamage(){return 0;}

		/**
		 * \brief ��õ�ǰ��������
		 * \author fqnewman
		 * \return ��������
		 */
		virtual DWORD getMaxPDamage(){return 0;}

		/**
		 * \brief ��õ�ǰ���������
		 * \author fqnewman
		 * \return ���������
		 */
		virtual DWORD getPDefence(){return 0;}

		/**
		 * \brief ��õ�ǰħ��������
		 * \author fqnewman
		 * \return ħ��������
		 */
		virtual DWORD getMDefence(){return 0;}

		/**
		 * \brief ��������hp
		 * \author fqnewman
		 */
		virtual void changeAndRefreshHMS(bool lock=true, bool sendData=true){}

		/**
		 * \brief �ڱ��Լ�����֮ǰ�Ĵ���������װ����Ĵ���������Ч�����жϵ�
		 * \param pUser ������
		 * \param rev ���ι����Ĵ���ָ��
		 * \param physics �Ƿ�������
		 * \param good Ϊtrue����У�falseҪ�жϻر���
		 * \author fqnewman
		 * \return trueΪ��ι�������Ч�ģ�falseΪһ����Ч�Ĺ���
		 */
		virtual bool preAttackMe(SceneEntryPk *pUser, const Cmd::stAttackMagicUserCmd *rev, bool physics=true, const bool good = false)=0;

		/**
		 * \brief ��ɫ������
		 * \param pUser ������
		 * \param rev ���ι����Ĵ�����Ϣ
		 * \param physics �Ƿ�������
		 * \author fqnewman
		 * \return trueΪ��ι�������Ч�ģ�falseΪһ����Ч�Ĺ���
		 */
		virtual bool AttackMe(SceneEntryPk *pUser, const Cmd::stAttackMagicUserCmd *rev, bool physics=true, SWORD rangDamageBonus=0);

		/**
		 * \brief ���ý�ɫ���ϵ���Ч״̬����Ӧ״̬�ķ��ʼ������ۼ�
		 * \param state ״̬ID ����enum SceneEntryStateȡֵ
		 * \author fqnewman
		 */
		//void setEffectStatus(const WORD &state)
		//{
			//_effectState[state]=_effectState[state]+1;
			//if (1 == _effectState[state])
			//{
				//showCurrentEffect(state, true);
			//}
		//}


		/**
		 * \brief �����ɫ���ϵ���Ч״̬����Ӧ״̬�ķ��ʼ�����������Ϊ0��ʱ�������״̬
		 * \param state ״̬ID ����enum SceneEntryStateȡֵ
		 * \author fqnewman
		 */
		//void clearEffectStatus(const WORD &state)
		//{
			//_effectState[state]=_effectState[state]-1;
			//if (0 == _effectState[state])
			//{
				//showCurrentEffect(state, false);
			//}
		//}

		/**
		 * \brief �ó�������
		 * \author fqnewman
		 */
		virtual void relivePet(){};

		/**
		 * \brief �ý�ɫ����
		 * \author fqnewman
		 */
		virtual void relive(){};

		/**
		 * \brief �õ���ҵ�Ȩ�޵ȼ�
		 * \return ��ҵ�Ȩ�޵ȼ�
		 */
		virtual BYTE getPriv(){return 0;}

		/**
		 * \brief ��ɫ������N��
		 * \author fqnewman
		 */
		virtual void standBack(const DWORD dwAttTempID, DWORD grids)=0;

		/**
		 * \brief ������Ŀ�껻��dwTempID��ָ��Ľ�ɫ���
		 * \param dwTempID Ŀ���ɫ����ʱID
		 * \author fqnewman
		 */
		virtual void changeAttackTarget(const DWORD &dwTempID){};

		/**
		 * \brief �ý�ɫ����
		 * \param dwTempID �����ߵ���ʱID
		 * \author fqnewman
		 */
		virtual void toDie(const DWORD &dwTempID)=0;

		/**
		 * \brief ����Ļ������ƶ�
		 * \author fqnewman
		 */
		virtual void goToRandomScreen(){};

		/**
		 * \brief ����Ļ�ڶ����ƶ�
		 * \author fqnewman
		 */
		virtual void jumpTo(zPos &newPos){};

		/**
		 * \brief ֪ͨ�ͻ�������ֵ�ı仯
		 * \author fqnewman
		 */
		virtual void attackRTHpAndMp(){};

		/**
		 * \brief �жϽ�ɫ�Ƿ�����
		 * \author fqnewman
		 */
		virtual bool isDie() = 0;

		/**
		 * \brief ��ȡ��ɫ�ļ���
		 * \author fqnewman
		 */
		virtual DWORD getLevel() const =0;

		/**
		 * \brief ��ȡ���е���
		 * \author whj
		 * \return ���ظ����͵����е���
		 */
		virtual DWORD getFiveType() {return 0;}

		/**
		 * \brief ��Ҫ��ְҵ���ͣ���������ʹ�õļ�������
		 * \author fqnewman
		 */
		virtual bool needType(const DWORD &needtype)=0;

		/**
		 * \brief ��Ҫ��ְҵ���ͣ���������ʹ�õļ�������
		 * \author fqnewman
		 */
		virtual bool addSkillToMe(zSkill *skill)=0;

		/**
		 * \brief �Ƿ��иü�����Ҫ������
		 * \author fqnewman
		 * \return true �� false û��
		 */
		virtual bool needWeapon(DWORD skillid)=0;

		/**
		 * \brief �Ƿ�Pk����
		 * \param other PK�����
		 * \author fqnewman
		 * \return true �� false ��
		 */
		virtual bool isPkZone(SceneEntryPk *other=NULL)=0;

		/**
		 * \brief ������Ʒ�����ͷ���
		 * \param object ������Ʒ������
		 * \param num ������Ʒ������
		 * \author fqnewman
		 * \return true ���ĳɹ� false ʧ��
		 */
		virtual bool reduce(const DWORD &object, const BYTE num)=0;

		/**
		 * \brief ����������Ʒ�Ƿ��㹻
		 * \param object ������Ʒ������
		 * \param num ������Ʒ������
		 * \author fqnewman
		 * \return true �㹻 false ����
		 */
		virtual bool checkReduce(const DWORD &object, const BYTE num)=0;
		/**
		 * \brief ��ȡװ���˺��ӳ�
		 * \author fqnewman
		 * \return �˺��ӳ�
		 */
		virtual WORD getDamageBonus() {return 0;}

		/**
		 * \brief ʩ�ż��������µ�����MP,HP,SP
		 * \param base ���ܻ������Զ���
		 * \author fqnewman
		 * \return true ���ĳɹ� false ʧ��
		 */
		virtual bool doSkillCost(const zSkillB *base) = 0;

		/**
		 * \brief ����Ƿ�����
		 * \author fqnewman
		 * \return true ���� false û��
		 */
		virtual bool checkMountHorse() {return false;}

		/**
		 * \brief ���ʩ�ż��������µ�����MP,HP,SP�Ƿ��㹻
		 * \param base ���ܻ������Զ���
		 * \author fqnewman
		 * \return true ���ĳɹ� false ʧ��
		 */
		virtual bool checkSkillCost(const zSkillB *base) = 0;

		/**
		 * \brief ��������ʩ�ųɹ����ʣ�������μ����Ƿ����ʩ��
		 * \author fqnewman
		 * \return true �ɹ� false ʧ��
		 */
		virtual bool checkPercent()=0;

		/**
		 * \brief �ж��Ƿ��ǵ���
		 * \author fqnewman
		 * \return true �� false ����
		 */
		virtual int isEnemy(SceneEntryPk *entry, bool notify = false, bool good = false)=0;

		/**
		 * \brief �ж��Ƿ�ɹ���
		 * \author fqnewman
		 * \return true �� false ����
		 */
		virtual bool isBugbear(){return true;}

		/**
		 * \brief ��ȡ�Լ������ˣ�һ�����NPC���ԣ�Player���������Լ�
		 * \author fqnewman
		 * \return NULL�������˵Ķ���ָ��
		 */
		virtual SceneEntryPk *getMaster()=0 ;

		/**
		 * \brief �õ����ϲ������
		 *
		 * \return ����
		 */
		virtual SceneEntryPk *getTopMaster() =0;

		/**
		 * \brief ������·��
		 * \param pTarget ��������
		 * \param aType ���з�ʽ
		 * \author fqnewman
		 * \return �Ƿ�˳������
		 */
		virtual bool checkMagicFlyRoute(zSceneEntry *pTarget, BYTE aType);

		/**
		 * \brief ��ɫ�������
		 * \param pAtt ������
		 * \author fqnewman
		 */
		virtual bool processDeath(SceneEntryPk *pAtt);
		virtual void leaveBattle();

		/**
		 * \brief ͨ��ɱ�����Լ��ȼ��ߵ�npc�������ƶ��
		 *
		 * \param pNpc ɱ����npc
		 */
		virtual void reduceGoodness(SceneNpc *pNpc){};

		virtual void setupCharBase(bool lock=true) {};
		/**
		 * \brief ���·��ͱ���ɫ�ĵ�ͼ����
		 */
		virtual void reSendMyMapData()=0;

		/**
		 * \brief �����߳�����
		 * \param attacktype �����ߵ�����
		 * \param tempid �����ߵ���ʱid
		 * \author fqnewman
		 */
		virtual void assaultMe(BYTE attacktype, DWORD tempid) {}

		/**
		 * \brief �ٻ�����
		 */
		//virtual ScenePet* summonPet(DWORD id, Cmd::petType type) {return NULL;}
		/**
		 * \brief �ٻ�ͼ��
		 */
		virtual ScenePet* summonPet(DWORD id, Cmd::petType type, DWORD standTime = 0, DWORD sid=0, const char * petName = "", DWORD anpcid=0, zPos pos=zPos(0,0), BYTE dir=4) = 0;

		/**
		 * \brief ����ս�������ĸ��ٹ���Ŀ��
		 */
		virtual void setPetsChaseTarget(SceneEntryPk *entry){};

		/**
		 * \brief ����ս�������ĸ��ٹ���Ŀ��
		 * \param weaponType ��������
		 * \return true�������ͷ��ϣ�false�������Ͳ�����
		 */
		virtual bool checkWeapon(BYTE weaponType){return true;}


		/**
		 * \brief ��ȡ��������	
		 */
		virtual SWORD getPoisondef(){return 0;}
		/**
		 * \brief ��ȡ���������	
		 */
		virtual SWORD getLulldef(){return 0;}
		/**
		 * \brief ��ȡ��ѣ������	
		 */
		virtual SWORD getReeldef(){return 0;}
		/**
		 * \brief ��ȡ����ħ����	
		 */
		virtual SWORD getEvildef(){return 0;}
		/**
		 * \brief ��ȡ����������	
		 */
		virtual SWORD getBitedef(){return 0;}
		/**
		 * \brief ��ȡ����������	
		 */
		virtual SWORD getChaosdef(){return 0;}
		/**
		 * \brief ��ȡ����������	
		 */
		virtual SWORD getColddef(){return 0;}
		/**
		 * \brief ��ȡ��ʯ������	
		 */
		virtual SWORD getPetrifydef(){return 0;}
		/**
		 * \brief ��ȡ��ʧ������	
		 */
		virtual SWORD getBlinddef(){return 0;}
		/**
		 * \brief ��ȡ����������	
		 */
		virtual SWORD getStabledef(){return 0;}
		/**
		 * \brief ��ȡ����������	
		 */
		virtual SWORD getSlowdef(){return 0;}
		/**
		 * \brief ��ȡ���ջ�����
		 */
		virtual SWORD getLuredef(){return 0;}


		/**
		 * \brief ��ȡ������	
		 */
		virtual SWORD getPoison(){return 0;}
		/**
		 * \brief ��ȡ�������	
		 */
		virtual SWORD getLull(){return 0;}
		/**
		 * \brief ��ȡѣ������	
		 */
		virtual SWORD getReel(){return 0;}
		/**
		 * \brief ��ȡ��ħ����	
		 */
		virtual SWORD getEvil(){return 0;}
		/**
		 * \brief ��ȡ��������	
		 */
		virtual SWORD getBite(){return 0;}
		/**
		 * \brief ��ȡ��������	
		 */
		virtual SWORD getChaos(){return 0;}
		/**
		 * \brief ��ȡ��������	
		 */
		virtual SWORD getCold(){return 0;}
		/**
		 * \brief ��ȡʯ������	
		 */
		virtual SWORD getPetrify(){return 0;}
		/**
		 * \brief ��ȡʧ������	
		 */
		virtual SWORD getBlind(){return 0;}
		/**
		 * \brief ��ȡ��������	
		 */
		virtual SWORD getStable(){return 0;}
		/**
		 * \brief ��ȡ��������	
		 */
		virtual SWORD getSlow(){return 0;}
		/**
		 * \brief ��ȡ�ջ�����
		 */
		virtual SWORD getLure(){return 0;}


		virtual void appendAttack(const Cmd::stAttackMagicUserCmd *rev) {};


		///����
		ScenePet * pet;
		///�ٻ���
		ScenePet * summon;
		///ͼ��
		std::list<ScenePet *> totems;

		/**
		 * \brief ȡ���ٻ��ĳ���
		 */
		virtual bool killOnePet(ScenePet *)=0;
		virtual void killAllPets()=0;
		virtual void killSummon() {};

		virtual void sendPetDataToNine();
		virtual void addPetExp(DWORD,bool,bool) = 0;
		void petLevelUp(ScenePet *);

		virtual bool isRedNamed(bool allRedMode=true) const = 0;
		DWORD getHp();
		DWORD getMaxHp();
		bool isFighting();
		///����ս��״̬��ʱ��
		zRTime endBattleTime;
		///ʩ����
		DWORD curMagicManID;
		///��ʩ��������
		DWORD curMagicManType;
		///����
		DWORD curTargetID;
		///��������
		DWORD curTargetType;
		virtual bool setCurTarget(SceneEntryPk *, bool=false);
		virtual bool setCurTarget(DWORD, DWORD, bool=false);
		SceneEntryPk * getCurTarget();

		///�����Ķ���
		DWORD defTargetID;
		///�����Ķ�������
		DWORD defTargetType;
		virtual bool setDefTarget(SceneEntryPk *, bool=false);
		virtual bool setDefTarget(DWORD, DWORD, bool=false);
		SceneEntryPk * getDefTarget();
		void clearDefTarget();


		void setEndBattleTime(const zRTime &, int);
		bool checkEndBattleTime(const zRTime &);

		virtual WORD skillUpLevel(WORD skilltype, WORD skillkind){return 0;}

		virtual void getSummonAppendDamage(WORD &minDamage, WORD &maxDamage){};
		virtual void exchangeMeAndSummonPet(){};
		virtual DWORD getMasterMana(){return 0;}
		virtual DWORD getMP(){return 0;}
		virtual void clearMana(){}
		virtual void leech(DWORD dwDam) {}
		virtual void sendMessageToMe(const char *pattern) {};
		virtual void setSpeedRate(float rate){};
		virtual void resetSpeedRate(){};
		/**
		 * \brief ���ý�ɫ�ĵ�ǰ״̬�������ݵ�ǰ״̬���ֽ�ɫ����Ч���߹ر���Ч
		 * \param state ״̬ID ����enum SceneEntryStateȡֵ
		 * \param isShow �Ƿ���ʾ��Ч
		 * \author fqnewman
		 */
		virtual void showCurrentEffect(const WORD &state, bool isShow,bool notify=true) = 0;
	private:
		virtual void processMaskOnAttack(SceneEntryPk *){};
		virtual void processMaskOnDefence(){};
		virtual void processAddDam(int &dwDam, int &dwDamDef, bool physics){};
		virtual void reduceDam(int &dwDam, int &dwDamDef, bool physics){};
		virtual void reflectDam(int &dwDamDef, int &dwDamSelf, bool physics){};
		virtual void hp2mp(int &dwDamDef){};
		virtual void reflectSkill(SceneEntryPk *pAtt, const Cmd::stAttackMagicUserCmd *rev){};
};


/**
 * \brief �˺�ת�ƻص�������������˺�ת�Ƶ�һ���ڵ�ĳһ�����˺�����
 * \author fqnewman
  */
struct DamagCallback: public zSceneEntryCallBack
{
	zSceneEntry *me;
	SceneEntryPk *target;
	DamagCallback(SceneEntryPk *entry)
	{
		me = entry;
		target = NULL;
	}
	bool exec(zSceneEntry *entry)
	{
		zSceneEntry::SceneEntryType myType = entry->getType();
		if (entry == me ) return false;
		if (myType == zSceneEntry::SceneEntry_NPC)
		{
			if (((SceneEntryPk *)entry)->isBugbear())
			{
				target = (SceneEntryPk *)entry;
				if (zMisc::selectByPercent(40)) return false;
			}
			else
			{
				return true;
			}
		}
		else if (myType == zSceneEntry::SceneEntry_Player)
		{
			target = (SceneEntryPk *)entry;
			if (zMisc::selectByPercent(40)) return false;
		}
		return true;
	}
	SWORD action(SDWORD dam)
	{
		if (target)
		{
			return target->directDamage((SceneEntryPk *)me, dam);
		}
		return 0;
	}
};

#ifdef _POOL_ALLOC_
typedef std::vector<SceneEntryPk *, __gnu_cxx::__pool_alloc<SceneEntryPk *> > SceneEntryPk_vec;
#else
typedef std::vector<SceneEntryPk *> SceneEntryPk_vec;
#endif

#endif
