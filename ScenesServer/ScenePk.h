#ifndef _SCENEPK_H_
#define _SCENEPK_H_

#include "zTime.h"
#include "SkillBase.h"
#include "SceneSkill.h"
#include "SkillManager.h"
#include <vector>
#include <map>
#include <ext/hash_map>
struct SceneUser;
struct SceneNpc;
struct SceneEntryPk;

//����Ӱ���״̬����
struct SkillState
{
	SkillState();
	void init();
	union
	{
		struct 
		{
			SWORD dvalue;		//�˺�ֵ���ӹ̶���ֵ1
			SWORD dvaluep;		//�˺�ֵ����x%2
			SWORD pdefence;		//���������ֵ��� 57��86
			SWORD pdefencep;	//�����������ٷֱ�
			SWORD updam;		//���������ӹ̶���ֵ
			SWORD updamp;		//���������Ӱٷֱ�
			SWORD umdam;		//�����������ӹ̶���ֵ
			SWORD umdamp;		//�����������Ӱٷֱ�
			SWORD dpdam;		//���������ٹ̶���ֵ
			SWORD dpdamp;		//���������ٰٷֱ�
			SWORD dmdam;		//�����������ٹ̶���ֵ
			SWORD dmdamp;		//�����������ٰٷֱ�
			SWORD updef;		//����������ӹ̶���ֵ
			SWORD updefp;		//����������Ӱٷֱ�
			SWORD umdef;		//�����������ӹ̶���ֵ
			SWORD umdefp;		//�����������Ӱٷֱ�
			SWORD dpdef;		//����������ٹ̶���ֵ
			SWORD dpdefp;		//����������ٰٷֱ�
			SWORD dmdef;		//�����������ٹ̶���ֵ
			SWORD dmdefp;		//�����������ٰٷֱ�
			SWORD topet;		//��Ϊ���С����79
			SWORD appenddam;	//���������˺�83
			SWORD movespeed;	//�ƶ��ٶȱ���ٷֱ� 16��56
			SWORD mgspeed;		//���ټ���ʩ�ż��17
			SWORD coldp;		//�����������״̬����18
			SWORD poisonp;		//���������ж�״̬����19
			SWORD petrifyp;		//��������ʯ��״̬����20
			SWORD blindp;		//��������ʧ��״̬����21
			SWORD chaosp;		//�����������״̬����22
			SWORD atrating;		//����������33,64
			SWORD reduce_atrating;  // �����ʼ��� ZJW����
			SWORD hpspeedup;	//����ֵ�ָ��ٶ�����34
			SWORD mpspeedup;	//����ֵ�ָ��ٶ�����35
			SWORD spspeedup;	//����ֵ�ָ��ٶ�����36
			SWORD akdodge;		//����������37
			SWORD reduce_akdodge;  // �������½�
			SWORD reflect;		//����45
			SWORD reflectp;		//����x%46
			SWORD reflect2;		//Ϊ�����ٷ�֮���ĵ����˺�50
			SWORD mdefence;		//�����������59��             // 40
			SWORD mdefencep;	//������������ٷֱ�x%
			SWORD uattackspeed;	//�����ٶ���߰׷ֱ�
			SWORD dattackspeed;	//�����ٶȽ��Ͱ׷ֱ�
			SWORD sevendownp;	//���������ߴ�״̬����82
			SWORD tsfdamp;		//�˺�ת�ưٷֱ�
			SWORD tsfdam;		//�˺�ת����ֵ
			SWORD passdam;		//�����������˺�117
			SWORD maxhp;       //����ֵ���ֵ���
			SWORD maxmp;       //����ֵ���ֵ���
			SWORD maxsp;       //����ֵ���ֵ���
			SWORD pupdam;		//���������ӹ̶���ֵ��������
			SWORD pumdam;		//�����������ӹ̶���ֵ��������
			SWORD pupdef;		//����������ӹ̶���ֵ��������
			SWORD pumdef;		//�����������ӹ̶���ֵ��������
			SWORD supdam;		//�������������������̶�ֵ(��Ч)
			SWORD spupdam;		//�������������������̶�ֵ(����)
			SWORD spupdamp;		//�������������������ٷֱ�(����)
			SWORD rpupdam;		//�������������������̶�ֵ(����)
			SWORD lupdam;		//��Ч����������ǿ�̶�ֵ
			SWORD satrating;	//����������������
			SWORD patrating;   // ���������ʣ�������
			SWORD pattackspeed;	//���ӹ����ٶȣ�������
			SWORD upattribute;	//���Ե�����
			SWORD tuling;      //������ǿ
			SWORD kulou;		//���ù�����ǿ
			SWORD tianbing;    //�����ǿ
			SWORD weaponupdamp; //�����������������ٷֱ�
			SWORD weaponumdamp; //�������������������ٷֱ�
			SWORD uppetdamage; //�����ٻ��޵Ĺ�����
			SWORD uppetdefence; //�����ٻ��޵ķ�����
			SWORD pmaxhp;		//���������������ֵ
			SWORD bang;			//��������ʩ�ų��ػ��ļ���
			SWORD theurgy_updam; //����������������
			SWORD theurgy_updamp; //�����������������ٷֱ�
			SWORD theurgy_updef; //�����������������
			SWORD theurgy_umdefp; //�������ӷ����������ٷֱ�
			SWORD theurgy_umdef; //�������ӷ���������
			SWORD theurgy_umdam; //�������ӷ���������
			SWORD theurgy_umdamp; //�������ӷ����������ٷֱ�
			SWORD theurgy_updefp;//������������������ٷֱ�
			SWORD theurgy_dpdef; //�����������������
			SWORD theurgy_dmdef; //�����������������
			SWORD theurgy_dpdam;		//���������ٹ̶���ֵ
			SWORD theurgy_dmdam;		//�����������ٹ̶���ֵ
			SWORD hpupbylevel; //��������ȼ��������Ѫ��                    //80
			SWORD reflect_ardor; // ��������״̬�������Լ��ĵ���
			SWORD reflect_poison; //�����ж�״̬�������Լ��ĵ���
			SWORD reflect_lull;  // �������״̬�������Լ��ĵ���
			SWORD reflect_frost; // ��������״̬�������Լ��ĵ���
			WORD introject_maxmdam; // �ٻ���������ħ������
			WORD introject_maxpdam; // �ٻ���������������
			WORD introject_mdam; // �ٻ���������ħ������
			WORD introject_pdam; // �ٻ���������������
			WORD introject_mdef; // �ٻ���������ħ������
			WORD introject_pdef; // �ٻ����������������
			WORD introject_maxhp; // �ٻ�����������������
			SWORD summonrelive; // �ٻ�������
			SWORD pdamtodef;	// �﹥ת���
			SWORD mdamtodef;	// ħ��תħ��
			SWORD pdeftodam;	// ���ת�﹥
			SWORD mdeftodam;	// ħ��תħ��
			SWORD sept_updamp;  // �����������ٷֱ�
			SWORD sept_umdamp;	// ����ħ�������ٷֱ�
			SWORD sept_updefp;	// ������������ٷֱ�
			SWORD sept_umdefp;	// �������������ٷֱ�
			SWORD sept_maxhp;	// �����������ֵ�ٷֱ�
			SWORD sept_maxmp;	// �����������ֵ�ٷֱ�
			SWORD blazeappend; // ����ϵ�����˺���������ǧ��֮��
			SWORD pblazeappend; // ����ϵ�����˺���������ǧ��֮��
			SWORD levinappend; // �׵�ϵ�����˺���������ǧ��֮��
			SWORD plevinappend; // �׵�ϵ�����˺���������ǧ��֮��(����)
			SWORD trapappend;  // ����ϵ�����˺���������ǧ��֮��
			SWORD iceappend;  // ����ϵ�����˺���������ǧ��֮��
			SWORD udef;			// ���������̶�ֵ������ͷ���������
			SWORD magicattack;  // �����ķ���������ʱ�򸽴������˺�
			SWORD array_udamp; // �󷨵�˫����������
			SWORD array_ddefp; // �󷨵�˫�����ʽ���
			SWORD array_dmvspeed; // �󷨵��ƶ��ٶȽ���
			SWORD upcon; //��������
			SWORD upint; //��������
			SWORD updex; //��������
			SWORD sword_udam; //���������������˺�����   // 110
			SWORD attackaddhpnum; // ���������Լ�����ֵ��Ŀ
			SWORD dodge;         // ��һ���������ܵ��˹���
			SWORD reflect_icelull; // ����һ�����״̬ ���׶���
			SWORD pumdamp;		//�������������������ٷֱ�
			SWORD addmenp;   // ���Ӿ���ٷֱ�
			SWORD ice_umdefp; //����������ħ�������ٷֱ�
			SWORD protectdam; //����״̬֮�˺�
			SWORD protectUpAtt; //����״̬֮��������
			SWORD unitarybattle; //һԪ��Ͷ��С���ܵ��ӳ����ڹ����ĵ������ϣ���ΪͶ�͵ļ���ID
			SWORD teamappend; //��ӹ����ӳɣ��������ŵ�����Ӷӳ���ID
			SWORD nsc_updamp; //���̵ִ�ר�ü�������״̬��
			SWORD tgyt_updefp; //��������ר�ü�������״̬
			SWORD tgzh_updef; //����֮��ר�ü�������״̬
			SWORD lm_updamp;   // ³ç�������������ٷֱ�
			SWORD upallattrib; // ��������������ֵ
			SWORD dnallattrib; // �������Խ�����ֵ
			SWORD upstr;      // �����������԰ٷֱ�
			SWORD dpallattrib; // ���������½��ٷֱ�
			SWORD upattribstr; // �����������԰ٷֱ�
			SWORD upattribcon; // �����������԰ٷֱ�
			SWORD upattribmen; // �����������԰ٷֱ�
			SWORD upattribdex; // �����������԰ٷֱ�
			SWORD upattribint; // �����������԰ٷֱ�
			SWORD relive;    // ����
			SWORD nowrelive; // ��������ʱ�ظ��ٷֱ�
			SWORD brappenddam; //����ӡ�����ṩ�Ķ����˺���
			SWORD dpintmen;   // �������񽵵Ͱٷֱ�
			SWORD dpstrdex;   // �������ݽ��Ͱٷֱ�
			SWORD enervation; // ˥��
			SWORD erupt;      // �Ƹ�����״̬ ħ�﹥����300%����ʱ�������ø�������״̬��
		};
		SWORD swdValue[151];
	};
};

//��ÿ�λ�װ��ǰ����
struct PkPreValue
{
	PkPreValue()
	{
		fiveexpress = 0;
		bzero(wdValue , sizeof(wdValue));
	}
	void init()
	{
		fiveexpress = 0;
		bzero(wdValue , sizeof(wdValue));
	}
	float fiveexpress;
	union
	{
		struct 
		{
			WORD fivedam;
			WORD fivemaxdam;
			WORD fivedef;
			WORD nofivedam;
			WORD nofivemaxdam;
			WORD nofivedef;
			WORD fivemdam;
			WORD fivemaxmdam;
			WORD fivemdef;
			WORD nofivemdam;
			WORD nofivemaxmdam;
			WORD nofivemdef;
		};
		WORD wdValue[12];
	};
};

struct PkValue
{
	PkValue()
	{
		bzero(dwValue , sizeof(dwValue));
	}
	void init()
	{
		bzero(dwValue , sizeof(dwValue));
	}
	union
	{
		struct 
		{
			DWORD	pdamage;					/// �������� *
			DWORD	mdamage;					/// ���������� *
			DWORD	pdefence;					/// ��������� *
			DWORD	mdefence;					/// ���������� *
			DWORD	mcost;						/// ���ķ���ֵ
			DWORD	hpcost;						/// ��������ֵ
			DWORD	spcost;						/// ��������ֵ
			DWORD	exp;						/// ��������
			DWORD	dvalue;						/// �˺�ֵ
			DWORD	dvaluep;					/// �˺�ֵ���Ӱٷֱ�
			SWORD	damagebonus;				/// �����˺��ӳ�
		};
		DWORD dwValue[11];
	};
};

struct ScenePkState
{
	ScenePkState()
	{
		zRTime ctv;
		lastPTime = 0;
		lastMTime = 0;
		tGood = ctv;
		tGood.addDelay(ScenePkState::goodnessPeriod);
		tProtect = ctv;
		lastCheckGoodness = ctv;
		lastCheckGoodness.addDelay(ScenePkState::protectPeriod);
		protect_time=0;
	}


	//�ƶ�ȼ��ʱ����
	static const int goodnessPeriod = 60 * 1000;
	zRTime tGood;

	//����ʱ����
	static const int protectPeriod = 10 * 1000;
	zRTime tProtect;

	//���һ�μ��״̬��ʱ��
	zRTime lastCheckGoodness;

	//�����˺������˼�¼
	struct attProtect
	{
		DWORD	id;
		BYTE	time;
		attProtect()
		{
			id = 0;
			time = 0;
		}
	};
	bool speedOutP(WORD speed, DWORD dwTime);
	bool speedOutM(WORD speed, DWORD dwTime);
	private:
	DWORD lastPTime;
	DWORD lastMTime;
	public:
	//����ɱ����һ�����˵�ʱ��
	zRTime tGoodNormal;
	void clearProtect()
	{
		protect_time=0; 
	}
	bool deathUserProtect(SceneUser *pThis , DWORD defid);
	bool cancelProtect(SceneUser *pThis,DWORD time=0);
	bool addProtect(SceneUser * pThis,DWORD time=0);
	bool hasProtected();
	DWORD leftProtectTime()
	{
		return protect_time;
	}
	private:
	DWORD protect_time;
};

struct zSkill;
using namespace SkillDef;
class ScenePk  
{
	public:
		static bool physicalMagicU2U(const Cmd::stAttackMagicUserCmd *rev , SceneUser *pAtt , SceneUser *pDef);
		static bool attackDeathUser(SceneUser *pAtt , SceneUser *pDef);
		static bool sendChangedUserData(SceneUser *pUser);
		static bool attackDeathNpc(SceneEntryPk *pAtt , SceneNpc *pDef);
		static bool attackFailToMe(const Cmd::stAttackMagicUserCmd *rev , SceneEntryPk * pAtt, bool failed=true, bool me=false);
		static void	checkProtect(SceneEntryPk *pAtt , SceneEntryPk *pDef);
		static bool attackUserCmdToNine(const Cmd::stAttackMagicUserCmd *rev , SceneEntryPk *pAtt);
		static void attackRTHpAndMp(SceneUser *pUser);
		static void attackRTExp(SceneUser *pUser , DWORD exp, DWORD dwTempID=0, BYTE byType=0);
		static bool attackRTCmdToNine(const Cmd::stAttackMagicUserCmd *rev , SceneEntryPk *pAtt , SceneEntryPk *pDef , const SDWORD sdwHP , BYTE byLuck);
		static bool checkAttackSpeed(SceneUser *pAtt , const Cmd::stAttackMagicUserCmd *rev);
		static void	calpdamU2U(const Cmd::stAttackMagicUserCmd *rev , SceneUser *pAtt , SceneUser *pDef);
		static void	calpdamU2N(const Cmd::stAttackMagicUserCmd *rev , SceneUser *pAtt , SceneNpc *pDef);
		static void	calmdamU2U(const Cmd::stAttackMagicUserCmd *rev , SceneUser *pAtt , SceneUser *pDef);
		static void calmdamU2N(const Cmd::stAttackMagicUserCmd *rev , SceneUser *pAtt , SceneNpc *pDef);
		static void calpdamN2U(const Cmd::stAttackMagicUserCmd *rev , SceneNpc *pAtt , SceneUser *pDef);
		static void calmdamN2U(const Cmd::stAttackMagicUserCmd *rev , SceneNpc *pAtt , SceneUser *pDef);
		static void calpdamN2N(const Cmd::stAttackMagicUserCmd *rev , SceneNpc *pAtt , SceneNpc *pDef);
		static void calmdamN2N(const Cmd::stAttackMagicUserCmd *rev , SceneNpc *pAtt , SceneNpc *pDef);
};

// */
#endif
