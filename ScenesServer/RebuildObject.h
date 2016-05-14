/**
 * \file
 * \version	$Id: RebuildObject.h  $
 * \author	
 * \date		2005-04-18
 * \brief		新物品合成，打造，升级，打孔，镶嵌系统
 * 
 */
 
#ifndef __REBUILD_OBJECT__
#define __REBUILD_OBJECT__


#include <map>

#include "zType.h"
#include "zMisc.h"
#include "zDatabase.h"
#include "zObject.h"
#include "SceneUser.h"

namespace Cmd{
struct stPropertyUserCmd;
struct stFoundItemPropertyUserCmd;
}

class zObjectB;

/**
 * \brief 物品改造基类
 *
 * 封装了几个常用的操作
 *
 */	
class Base
{
public:	
	enum ReuildType{
		COMPOSE = 1,
		MAKE = 2,
		UPGRADE = 3,
		HOLE = 4,
		ENCHANCE = 5,
		DECOMPOSE = 6,
	};
	
	static bool check_npc(SceneUser& user, zObjectB* base, int action);
	
	static bool check_space(SceneUser& user, DWORD w, DWORD h);

	static bool remove_object(SceneUser& user, zObject* ob);
	
	static bool add_object(SceneUser& user, zObject* ob, bool add = true);

	static bool response(SceneUser& user, int status, ReuildType type);
		
	static void refresh_pack(SceneUser& user, ObjectPack* pack);
};

/**
 * \brief 孔
 *
 * 封装了对与孔相关的处理函数
 *
 */	
class Hole
{
public:
	enum {
		INVALID_INDEX = 6,
		INVALID_NUM = 7,
	};
	
	
	static bool can_hole(zObject * ob);
	
	static int get_empty_hole(zObject* ob);
	
	static int get_hole_num(zObject* ob);

	static int add_hole_num(zObject* ob, int num);
	
	static bool add_hole(zObject* ob, int index);

	static bool put_hole(zObject* ob, int index, int id);	

};

/**
 * \brief 魂魄石
 *
 * 封装了对魂魄石的处理函数，包括镶嵌及合成等
 *
 */	
class SoulStone
{
public:
	static zObject* compose(SceneUser& user, zObject* first, zObject* second, int odds);

	static bool enchance(SceneUser& user, zObject* dest, zObject* src);

	static int id(DWORD trait);
	
	static bool assign(zObject* ob, int monster);
	
private:
	static bool do_enchance(zObject* dest, zObject* src);
	
	static bool do_compose(zObject* first, zObject* second, zObject* dest);
	
	/**     
	 * \brief 魂魄属性计算
	 *
	 * \param first: 第一块魂魄石属性
	 * \param second: 第二块魂魄石属性
	 * \param result: 计算结果
	 * \param level:魂魄石等级
	 * \return 无
	 */	
	template<typename T>
	static void additive(T first, T second, T& result, int level)
	{
		if (first && second) {
			//result = std::max(first, second) + 0.1*std::min(first, second)	
			T max = first, min = second, grade = level & 0xff;
			if (max < min) {
				max = second;
				min = first;
				grade = level >> 8;
			}
			
			result =  max + static_cast<T>(min*0.1*grade);	
		}else {
			result = first + second;
		}
		
	}

	static const int _ids[];
};

/**
 * \brief 升级
 *
 * 实现了物品升级功能
 *
 */	
class Upgrade
{
public:
	static bool upgrade(SceneUser& user, zObject* ob, int extra_odds);

private:	
	static bool do_upgrade(zObject* ob, zUpgradeObjectB* base);		
	static bool do_downgrade(zObject* ob, zUpgradeObjectB* base);		

};

/**
 * \brief 物品分解
 *
 * 实现物品分解功能
 *
 */
class Decompose
{
public:	
	/**     
	 * \brief 构造函数
	 *
	 * 初始化相关变量
	 *
	 * param ob : 待分解物品
	 *
	 */   
	Decompose(zObject* ob) : _ob(ob)
	{ }
	
	bool bonus_items(SceneUser& user);
	bool bonus_exp(SceneUser& user);
	bool remove_from(SceneUser& user);	
	int gold() const;
		
private:
	
	int chance() const;
	int index() const;
	
	zObject* _ob;
	
	const static int _odds[];
	const static int _items[];
};

#define COMPUTE_R(x) additive(ob->data.x, bob->x);
#define COMPUTE_L(x) additive(ob->data.x, bob->x, property);

#define BONUS_SKILL std::vector<skillbonus>::iterator it = bob->skill.begin();  \
		bool must = false; \
		for ( ; it!=bob->skill.end(); ++it) { \
			int odds = odds_of_property(it->odds, property); \
			if (zMisc::selectByTenTh(odds) || must) { \
				int i = 0; \
				while ( i <3) {\
					if (ob->data.skill[i].id == 0 || ob->data.skill[i].id==it->id) {\
						ob->data.skill[i].id = it->id; \
						ob->data.skill[i].point += it->level; \
						break;\
					}else {\
						++i;\
					}\
					if (i>2 &&must) break;\
				}\
			} \
		}
//戒指,项链
#define BONUS_SKILL_RING {\
	if(!bob->skill.empty())\
	{\
		int index = zMisc::randBetween(0, bob->skill.size()-1 );\
				int i = 0; \
				while ( i <3) {\
					if (ob->data.skill[i].id == 0 || ob->data.skill[i].id==bob->skill[index].id) {\
						ob->data.skill[i].id = bob->skill[index].id; \
						ob->data.skill[i].point += bob->skill[index].level; \
						break;\
					}else {\
						++i;\
					}\
				}\
		}\
	}

#define BONUS_SKILLS int odds = odds_of_property(bob->skills.odds, property); \
		if (zMisc::selectByTenTh(odds)) { \
			ob->data.skills.id = bob->skills.id; \
			ob->data.skills.point = bob->skills.level; \
		}

/**
 * \brief 打造
 *
 * 实现了物品打造功能
 *
 */	
class EquipMaker
{
	public:
		EquipMaker(SceneUser* user);

		/**     
		 * \brief 析构函数
		 *
		 */     
		~EquipMaker() { }

		bool check_skill(SceneUser& user, zObjectB* ob);

		bool add_skill(SceneUser& user, zObjectB* ob);
		bool add_exp(SceneUser& user, DWORD exp);

		bool check_material(zObjectB* ob, const std::map<DWORD, DWORD>& list, bool is_resource = false);

		//	void pre_level_of_material(zObjectB* base);
		void pre_level_of_material(int id, int level);

		bool level_of_material(DWORD id, DWORD num, DWORD level, zObjectB* base);

		bool is_odds_gem(DWORD kind, DWORD id);

		zObject* make_material(zObjectB* base);

		zObject* make(SceneUser* user, zObjectB* base, int flag = 0);

		void assign(SceneUser* user, zObject* ob, zObjectB* base, bool drop =false , int flag = 0);

		void fix(zObject* ob);

		void bonus_hole(zObject* ob);

	private:
		struct Odds
		{
			int per;
			int luck;
			double material_level;
			int skill_level;
			int odds;		
			int sleight;
			int odds_gem;
			Odds() : per(0), luck(0), material_level(0), skill_level(0), odds(0), sleight(0), odds_gem(0)
			{ }
		};

		Odds _odds;
		double _current;
		double _base;

		bool _make;

		int _need;
		int _1_id;
		int _1_level;
		int _2_id;
		int _2_level;

		int odds_of_white(const zObjectB* ob);
		int odds_of_blue(const zObjectB* ob);
		int odds_of_gold(const zObjectB* ob);
		int odds_of_holy(int object);
		int odds_of_property(int object, int property);

		template <typename T>
			void fix_kind(T* bob, zObject* ob)
			{
				switch (ob->base->kind)
				{
					case ItemType_ClothBody ://101代表布质加生命类服装
						additivePercent(ob->data.maxhp, bob->maxsp);
						break;
					case ItemType_FellBody :	    //102代表皮甲加魔防类服装
						additivePercent(ob->data.mdefence, bob->maxsp);	
						break;
					case ItemType_MetalBody:	//103代表金属铠甲加物防类服装
					case ItemType_Shield: 	//112代表盾牌类
						additivePercent(ob->data.pdefence, bob->maxsp);			
						break;
					case ItemType_Blade:		    //104代表武术刀类武器
					case ItemType_Sword:        //105代表武术剑类武器
					case ItemType_Axe:	           //106代表武术斧类武器
					case ItemType_Hammer:	        //107代表武术斧类武器
					case ItemType_Crossbow:	        //109代表箭术弓类武器
						additivePercent(ob->data.pdamage, bob->maxsp);		
						additivePercent(ob->data.maxpdamage, bob->maxsp);		
						break;
					case ItemType_Staff:		    //108代表法术杖类武器
					case ItemType_Stick:	        //111代表召唤棍类武器
					case ItemType_Fan:	           //110代表美女扇类
						additivePercent(ob->data.mdamage, bob->maxsp);		
						additivePercent(ob->data.maxmdamage, bob->maxsp);		
						break;
					case ItemType_Helm:		//113代表角色头盔类
					case ItemType_Caestus:	//114代表角色腰带类
					case ItemType_Cuff:		//115代表角色护腕类
					case ItemType_Shoes:		//116代表角色鞋子类
						if (zMisc::randBetween(0, 1)) {
							additivePercent(ob->data.pdefence, bob->maxsp);		
						}else {
							additivePercent(ob->data.mdefence, bob->maxsp);				
						}
						break;
					case ItemType_Necklace:	//117代表角色项链类
					case ItemType_Fing:		//118代表角色戒指类
						if (ob->data.pdamage || ob->data.maxpdamage) {
							additivePercent(ob->data.pdamage, bob->maxsp);		
							additivePercent(ob->data.maxpdamage, bob->maxsp);		
						}
						if (ob->data.mdamage || ob->data.maxmdamage) {
							additivePercent(ob->data.mdamage, bob->maxsp);		
							additivePercent(ob->data.maxmdamage, bob->maxsp);		
						}
						break;
				}

			}


		/**     
		 * \brief 属性取最大值
		 *
		 * \param ret: 计算结果
		 * \param lv: 属性取值范围
		 * \return 无
		 */	
		template <typename T>
			bool max(T& ret, const luckRangeValue &rv)
			{
				if( zMisc::selectByTenTh(rv.per) )	{
					ret += rv.data.max;

				}
				/*
				if ( zMisc::selectByPercent(_odds.sleight) ) {
					ret += rv.sleightValue;
					return true;
				}
				// */
				return false;
			}
		/**     
		 * \brief 生成特殊装备
		 *
		 * \param bob: 对应装备基本表
		 * \param ob: 打造物品
		 * \param kind: 装备类型
		 * \return 当前总是返回true
		 */	
		template <typename T>
			bool assign_color(T* bob, zObject* ob, int kind, int props = 0,zObjectB *base=NULL ,bool drop=false)
			{
				char tmp[MAX_NAMESIZE];
				snprintf(tmp, MAX_NAMESIZE, "%s%s%s", bob->prefix, bob->joint, ob->data.strName);
				strncpy(ob->data.strName, tmp, MAX_NAMESIZE);

				int property = 1;

				if(props >= 17)
				{
					++property;
					//ob->data.fivepoint += zMisc::randBetween(bob->five.data.min, bob->five.data.max);
					if (/*ob->data.fivepoint &&// */ ob->data.fivetype == FIVE_NONE) 
					{
						ob->data.fivetype = zMisc::randBetween(0, 4);
					}
				}
				else //if (additive(ob->data.fivepoint, bob->five, property)) 
				{
					//ob->data.fivepoint -= bob->five.sleightValue;
					if (/*ob->data.fivepoint && // */ob->data.fivetype == FIVE_NONE && zMisc::selectByTenTh(bob->five.per)) 
					{
						ob->data.fivetype = zMisc::randBetween(0, 4);
					}
				}


				COMPUTE_R( pdamage )		// 最小物攻
					COMPUTE_R( maxpdamage )		// 最大物攻
					COMPUTE_R( mdamage )			// 最小魔攻
					COMPUTE_R( maxmdamage )		// 最大魔攻
					COMPUTE_R( pdefence )			// 物防
					COMPUTE_R( mdefence )			// 魔防

					if (props) {
						int index = zMisc::randBetween(0, 4);
						if (index!=5) {

							//现在不用随机了,直接取值
							additivePercent(*ob->_p1[index], bob->_p1[index]);
						}else {
							fix_kind(bob, ob);
						}
					}else {
						COMPUTE_L( str )			// 力量
							COMPUTE_L( inte )			// 智力
							COMPUTE_L( dex )			// 敏捷
							COMPUTE_L( spi )			// 精神
							COMPUTE_L( con )				// 体质
					}		

				COMPUTE_L( maxhp )		// 最大生命值
					COMPUTE_L( maxmp )		// 最大法术值
					//		COMPUTE_L( maxsp )		// 最大体力值

					COMPUTE_L( mvspeed )		// 移动速度
					COMPUTE_L( hpr )			// 生命值恢复
					COMPUTE_L( mpr )			// 法术值恢复
					COMPUTE_L( spr )			// 体力值恢复
					COMPUTE_L( akspeed )		// 攻击速度

					if (props) {
						for (int i=0; i<=17; ++i){
							if(props == 17){
								if( i == 0 || i == 2)
								{
									if(bob->_p2[i].per)
										*ob->_p2[i] += bob->_p2[i].data.max;
								}
								else if (i == 1 || i == 3)
								{
									if(bob->_p2[i].per)
									{
										int temp = zMisc::randBetween(((bob->_p2[i].data.max - bob->_p2[i].data.min)/2 + bob->_p2[i].data.min + 1),bob->_p2[i].data.max);
										*ob->_p2[i] += temp;
									}
								}
								else
								{
										max(*ob->_p2[i], bob->_p2[i]);
								}
							}else if(props == 18){
								if( i == 0 || i == 2)
								{
									if(bob->_p2[i].per)
										*ob->_p2[i] += 10;
								}
								else if (i == 1 || i == 3)
								{
									if(bob->_p2[i].per)
										*ob->_p2[i] += bob->_p2[i].data.max;
								}
								else
								{
										max(*ob->_p2[i], bob->_p2[i]);
								}
							}else{
								additive(*ob->_p2[i], bob->_p2[i]);
							}
						}
						/*
						   std::vector<int> list;
						   for (int i=0; i<=17; ++i) list.push_back(i);
						//每个都需要随机
						//int geted = props;
						// */
						/*
						   if(props == 5){
						   if(list.size() >=4){
						   additive(*ob->_p2[list[3]], bob->_p2[list[3]]);
						   }
						   if(list.size() >=8){
						   additive(*ob->_p2[list[7]], bob->_p2[list[7]]);
						   }
						   }else{
						// */
						//while (/*geted -- > 0 && */list.size() > 0) {
						/*
						   int index = zMisc::randBetween(0, list.size()-1 );
						   int p = list[index];
						//取最大值
						if(props == 17){
						max(*ob->_p2[p], bob->_p2[p]);
						}else{
						additive(*ob->_p2[p], bob->_p2[p]);
						}
						std::vector<int>::iterator it = list.begin();
						list.erase(it+index);
						// */
						//}
						//}

					}else {
						COMPUTE_L( pdam )		// 增加物理攻击力
							COMPUTE_L( mdam )		// 增加魔法攻击力
							COMPUTE_L( pdef )		// 增加物理防御力
							COMPUTE_L( mdef )		// 增加魔法防御力
							COMPUTE_L( atrating )		// 命中率
							COMPUTE_L( akdodge )		// 闪避率

							COMPUTE_L( poisondef )	// 抗毒增加
							COMPUTE_L( lulldef )		// 抗麻痹增加
							COMPUTE_L( reeldef )		// 抗眩晕增加
							COMPUTE_L( evildef )		// 抗噬魔增加
							COMPUTE_L( bitedef )		// 抗噬力增加
							COMPUTE_L( chaosdef )	// 抗混乱增加
							COMPUTE_L( colddef )		// 抗冰冻增加
							COMPUTE_L( petrifydef )		// 抗石化增加
							COMPUTE_L( blinddef )		// 抗失明增加
							COMPUTE_L( stabledef )		// 抗定身增加
							COMPUTE_L( slowdef )		// 抗减速增加
							COMPUTE_L( luredef )		// 抗诱惑增加
					}

				if (!ob->data.durpoint) {
					if (additive(ob->data.durpoint, bob->resumedur, property)) {
						ob->data.durpoint -= bob->resumedur.sleightValue;
					}
					if (ob->data.durpoint) ob->data.dursecond = bob->resumedur.sleightValue;

				}

				COMPUTE_L( bang ) 			//重击
					//ob->data.bang += bob->bang;
					//戒指项链至少一个
					if(ob->base->kind == ItemType_Fing || ob->base->kind == ItemType_Necklace)
					{
						BONUS_SKILL_RING
					}
				BONUS_SKILL
					BONUS_SKILLS

					if (props) 
						ob->data.kind |= 2;//有色装备
					else 
						ob->data.kind |= kind;//有色装备

				return true;
			}

		bool assign_holy(zObject* ob, int holy);

		bool assign_set(zObject* ob);

		/**     
		 * \brief 属性计算
		 *
		 * \param ret: 计算结果
		 * \param lv: 属性取值范围
		 * \return 无
		 */	
		template <typename T>
			void additive(T& ret, const rangeValue &rv)
			{
				ret += zMisc::randBetween(rv.min,rv.max);
			}

		/**     
		 * \brief 神圣属性计算
		 *
		 * \param ret: 计算结果
		 * \param lv: 属性取值范围
		 * \param property: 物品当前属性数目
		 * \return 无
		 */	
		template <typename T>
			bool additive(T& ret, const luckRangeValue & lv, int& property)
			{
				int odds = lv.per;
				//		int odds = odds_of_property(lv.per, property);
				//		Zebra::logger->debug("属性产生概率%f, %f", lv.per*1.0, odds*1.0);
				if( zMisc::selectByTenTh(odds) )	{
					++property;

					ret += zMisc::randBetween(lv.data.min, lv.data.max);

					/*
					if ( zMisc::selectByPercent(_odds.sleight) ) {
						ret += lv.sleightValue;
						return true;
					}
					// */
				}

				return false;
			}	

		template <typename T>
			bool additive(T& ret, const luckRangeValue & lv)
			{
				if( zMisc::selectByTenTh(lv.per) )	{
					ret += zMisc::randBetween(lv.data.min, lv.data.max);

				}
				/*
				if ( zMisc::selectByPercent(_odds.sleight) ) {
					ret += lv.sleightValue;
					return true;
				}
				// */

				return false;
			}	
		template <typename T>
			bool additivePercent(T& ret, const luckRangeValue & lv)
			{
				ret += zMisc::randBetween(lv.data.min, lv.data.max);
				/*
				if ( zMisc::selectByPercent(_odds.sleight) ) {
					ret += lv.sleightValue;
					return true;
				}
				// */

				return false;
			}	

};

#include "zDatabaseManager.h"

/**
 * \brief 物品改造
 *
 * 实现各种物品改造功能,提供一个统一入口
 *
 */		
class RebuildObject : public Base
{
public:		
	enum {
		MAX_NUMBER = 50,
		
		HOLE_MONEY = 1000,
		ENCHANCE_MONEY = 500,
		
		COMP_SOUL_STONE_ID = 677,
		ENCHANCE_SONTE_ID = 678,
		HOLE_SONTE_ID = 679,
		LEVELUP_STONE_ID = 681,
	};
	
	static RebuildObject& instance();
			
	bool compose(SceneUser& user, const Cmd::stPropertyUserCmd* cmd);
	
	bool compose_soul_stone(SceneUser& user, const Cmd::stPropertyUserCmd* cmd);
	
	bool upgrade(SceneUser& user, const Cmd::stPropertyUserCmd* cmd);
	
	bool make(SceneUser& user, const Cmd::stPropertyUserCmd* cmd);
	
	bool hole(SceneUser& user, const Cmd::stPropertyUserCmd* cmd);
	
	bool enchance(SceneUser& user, const Cmd::stPropertyUserCmd* cmd);

	bool decompose(SceneUser& user, const Cmd::stPropertyUserCmd* cmd);
private:	
	/**     
	 * \brief 构造函数
	 *
	 */   	
	RebuildObject() { }
	
	/**     
	 * \brief 析构函数
	 *
	 */     
	~RebuildObject() { }

	
	static RebuildObject* _instance;
};

#endif
