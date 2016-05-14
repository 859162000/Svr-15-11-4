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

//技能影响的状态数据
struct SkillState
{
	SkillState();
	void init();
	union
	{
		struct 
		{
			SWORD dvalue;		//伤害值增加固定数值1
			SWORD dvaluep;		//伤害值增加x%2
			SWORD pdefence;		//物理防御数值变更 57，86
			SWORD pdefencep;	//物理防御变更百分比
			SWORD updam;		//物理攻击增加固定数值
			SWORD updamp;		//物理攻击增加百分比
			SWORD umdam;		//法术攻击增加固定数值
			SWORD umdamp;		//法术攻击增加百分比
			SWORD dpdam;		//物理攻击减少固定数值
			SWORD dpdamp;		//物理攻击减少百分比
			SWORD dmdam;		//法术攻击减少固定数值
			SWORD dmdamp;		//法术攻击减少百分比
			SWORD updef;		//物理防御增加固定数值
			SWORD updefp;		//物理防御增加百分比
			SWORD umdef;		//法术防御增加固定数值
			SWORD umdefp;		//法术防御增加百分比
			SWORD dpdef;		//物理防御减少固定数值
			SWORD dpdefp;		//物理防御减少百分比
			SWORD dmdef;		//法术防御减少固定数值
			SWORD dmdefp;		//法术防御减少百分比
			SWORD topet;		//变为随机小动物79
			SWORD appenddam;	//产生额外伤害83
			SWORD movespeed;	//移动速度变更百分比 16，56
			SWORD mgspeed;		//减少技能施放间隔17
			SWORD coldp;		//减少陷入冰冻状态几率18
			SWORD poisonp;		//减少陷入中毒状态几率19
			SWORD petrifyp;		//减少陷入石化状态几率20
			SWORD blindp;		//减少陷入失明状态几率21
			SWORD chaosp;		//减少陷入混乱状态几率22
			SWORD atrating;		//命中率增加33,64
			SWORD reduce_atrating;  // 命中率减少 ZJW加入
			SWORD hpspeedup;	//生命值恢复速度增加34
			SWORD mpspeedup;	//法术值恢复速度增加35
			SWORD spspeedup;	//体力值恢复速度增加36
			SWORD akdodge;		//闪避率上升37
			SWORD reduce_akdodge;  // 闪避率下降
			SWORD reflect;		//反弹45
			SWORD reflectp;		//反弹x%46
			SWORD reflect2;		//为反弹百分之几的敌人伤害50
			SWORD mdefence;		//法术防御变更59，             // 40
			SWORD mdefencep;	//法术防御变更百分比x%
			SWORD uattackspeed;	//攻击速度提高白分比
			SWORD dattackspeed;	//攻击速度降低白分比
			SWORD sevendownp;	//降低陷入七大状态几率82
			SWORD tsfdamp;		//伤害转移百分比
			SWORD tsfdam;		//伤害转移数值
			SWORD passdam;		//被动：额外伤害117
			SWORD maxhp;       //生命值最大值变更
			SWORD maxmp;       //法术值最大值变更
			SWORD maxsp;       //体力值最大值变更
			SWORD pupdam;		//物理攻击增加固定数值（被动）
			SWORD pumdam;		//法术攻击增加固定数值（被动）
			SWORD pupdef;		//物理防御增加固定数值（被动）
			SWORD pumdef;		//法术防御增加固定数值（被动）
			SWORD supdam;		//弓箭类增加物理攻击力固定值(长效)
			SWORD spupdam;		//弓箭类增加物理攻击力固定值(被动)
			SWORD spupdamp;		//弓箭类增加物理攻击力百分比(被动)
			SWORD rpupdam;		//棍子类增加物理攻击力固定值(被动)
			SWORD lupdam;		//长效物理攻击力增强固定值
			SWORD satrating;	//弓箭类提升命中率
			SWORD patrating;   // 增加命中率（被动）
			SWORD pattackspeed;	//增加攻击速度（被动）
			SWORD upattribute;	//属性点增加
			SWORD tuling;      //土灵增强
			SWORD kulou;		//骷髅弓手增强
			SWORD tianbing;    //天兵增强
			SWORD weaponupdamp; //提升武器物理攻击力百分比
			SWORD weaponumdamp; //提升武器法术攻击力百分比
			SWORD uppetdamage; //提升召唤兽的攻击力
			SWORD uppetdefence; //提升召唤兽的防御力
			SWORD pmaxhp;		//被动提升生命最大值
			SWORD bang;			//增加人物施放出重击的几率
			SWORD theurgy_updam; //仙术增加物理攻击力
			SWORD theurgy_updamp; //仙术增加物理攻击力百分比
			SWORD theurgy_updef; //仙术增加物理防御力
			SWORD theurgy_umdefp; //仙术增加法术防御力百分比
			SWORD theurgy_umdef; //仙术增加法术防御力
			SWORD theurgy_umdam; //仙术增加法术攻击力
			SWORD theurgy_umdamp; //仙术增加法术攻击力百分比
			SWORD theurgy_updefp;//仙术增加物理防御力百分比
			SWORD theurgy_dpdef; //仙术降低物理防御力
			SWORD theurgy_dmdef; //仙术降低物理防御力
			SWORD theurgy_dpdam;		//物理攻击减少固定数值
			SWORD theurgy_dmdam;		//法术攻击减少固定数值
			SWORD hpupbylevel; //根据人物等级提升最大血量                    //80
			SWORD reflect_ardor; // 反弹灼热状态给攻击自己的敌人
			SWORD reflect_poison; //反弹中毒状态给攻击自己的敌人
			SWORD reflect_lull;  // 反弹麻痹状态给攻击自己的敌人
			SWORD reflect_frost; // 反弹冰冻状态给攻击自己的敌人
			WORD introject_maxmdam; // 召唤合体增加魔法攻击
			WORD introject_maxpdam; // 召唤合体增加物理攻击
			WORD introject_mdam; // 召唤合体增加魔法攻击
			WORD introject_pdam; // 召唤合体增加物理攻击
			WORD introject_mdef; // 召唤合体增加魔法防御
			WORD introject_pdef; // 召唤合体增加物理防御
			WORD introject_maxhp; // 召唤合体增加生命上限
			SWORD summonrelive; // 召唤兽重生
			SWORD pdamtodef;	// 物攻转物防
			SWORD mdamtodef;	// 魔攻转魔防
			SWORD pdeftodam;	// 物防转物攻
			SWORD mdeftodam;	// 魔防转魔攻
			SWORD sept_updamp;  // 提升物理攻击百分比
			SWORD sept_umdamp;	// 提升魔法攻击百分比
			SWORD sept_updefp;	// 提升物理防御百分比
			SWORD sept_umdefp;	// 提升法术防御百分比
			SWORD sept_maxhp;	// 提升生命最大值百分比
			SWORD sept_maxmp;	// 提升法术最大值百分比
			SWORD blazeappend; // 火焰系法术伤害提升比例千分之几
			SWORD pblazeappend; // 火焰系法术伤害提升比例千分之几
			SWORD levinappend; // 雷电系法术伤害提升比例千分之几
			SWORD plevinappend; // 雷电系法术伤害提升比例千分之几(被动)
			SWORD trapappend;  // 陷阱系法术伤害提升比例千分之几
			SWORD iceappend;  // 陷阱系法术伤害提升比例千分之几
			SWORD udef;			// 防御提升固定值（物理和法术都升）
			SWORD magicattack;  // 两仪心法，攻击的时候附带法术伤害
			SWORD array_udamp; // 阵法的双攻比率提升
			SWORD array_ddefp; // 阵法的双防比率降低
			SWORD array_dmvspeed; // 阵法的移动速度降低
			SWORD upcon; //提升体质
			SWORD upint; //提升智力
			SWORD updex; //提升敏捷
			SWORD sword_udam; //提升弓箭攻击的伤害点数   // 110
			SWORD attackaddhpnum; // 攻击增加自己生命值数目
			SWORD dodge;         // 有一定几率闪避敌人攻击
			SWORD reflect_icelull; // 反弹一个麻痹状态 冰雷盾用
			SWORD pumdamp;		//被动提升法术攻击力百分比
			SWORD addmenp;   // 增加精神百分比
			SWORD ice_umdefp; //冰甲用提升魔法防御百分比
			SWORD protectdam; //保护状态之伤害
			SWORD protectUpAtt; //保护状态之攻击翻倍
			SWORD unitarybattle; //一元阵法投送小技能到队长正在攻击的敌人身上，此为投送的技能ID
			SWORD teamappend; //组队攻击加成，变量里存放的是组队队长的ID
			SWORD nsc_updamp; //逆手刺的专用技能提升状态。
			SWORD tgyt_updefp; //铁骨御体专用技能提升状态
			SWORD tgzh_updef; //铁骨之魂专用技能提升状态
			SWORD lm_updamp;   // 鲁莽提升物理攻击力百分比
			SWORD upallattrib; // 提升所有属性数值
			SWORD dnallattrib; // 所有属性降低数值
			SWORD upstr;      // 提升力量属性百分比
			SWORD dpallattrib; // 所有属性下降百分比
			SWORD upattribstr; // 提升力量属性百分比
			SWORD upattribcon; // 提升体质属性百分比
			SWORD upattribmen; // 提升精神属性百分比
			SWORD upattribdex; // 提升敏捷属性百分比
			SWORD upattribint; // 提升智力属性百分比
			SWORD relive;    // 再生
			SWORD nowrelive; // 立即复活时回复百分比
			SWORD brappenddam; //猎手印记所提供的额外伤害清
			SWORD dpintmen;   // 智力精神降低百分比
			SWORD dpstrdex;   // 力量敏捷降低百分比
			SWORD enervation; // 衰弱
			SWORD erupt;      // 破釜沉舟状态 魔物攻击翻300%持续时间完后会获得复活虚弱状态。
		};
		SWORD swdValue[151];
	};
};

//在每次换装备前调用
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
			DWORD	pdamage;					/// 物理攻击力 *
			DWORD	mdamage;					/// 法术攻击力 *
			DWORD	pdefence;					/// 物理防御力 *
			DWORD	mdefence;					/// 法术防御力 *
			DWORD	mcost;						/// 消耗法术值
			DWORD	hpcost;						/// 消耗生命值
			DWORD	spcost;						/// 消耗体力值
			DWORD	exp;						/// 经验消耗
			DWORD	dvalue;						/// 伤害值
			DWORD	dvaluep;					/// 伤害值增加百分比
			SWORD	damagebonus;				/// 技能伤害加成
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


	//善恶度检查时间间隔
	static const int goodnessPeriod = 60 * 1000;
	zRTime tGood;

	//自卫时间间隔
	static const int protectPeriod = 10 * 1000;
	zRTime tProtect;

	//最后一次检查状态的时间
	zRTime lastCheckGoodness;

	//攻击人和自卫人记录
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
	//好人杀死第一个坏人的时间
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
