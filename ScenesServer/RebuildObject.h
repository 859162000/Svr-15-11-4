/**
 * \file
 * \version	$Id: RebuildObject.h  $
 * \author	
 * \date		2005-04-18
 * \brief		����Ʒ�ϳɣ����죬��������ף���Ƕϵͳ
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
 * \brief ��Ʒ�������
 *
 * ��װ�˼������õĲ���
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
 * \brief ��
 *
 * ��װ�˶������صĴ�����
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
 * \brief ����ʯ
 *
 * ��װ�˶Ի���ʯ�Ĵ�������������Ƕ���ϳɵ�
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
	 * \brief �������Լ���
	 *
	 * \param first: ��һ�����ʯ����
	 * \param second: �ڶ������ʯ����
	 * \param result: ������
	 * \param level:����ʯ�ȼ�
	 * \return ��
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
 * \brief ����
 *
 * ʵ������Ʒ��������
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
 * \brief ��Ʒ�ֽ�
 *
 * ʵ����Ʒ�ֽ⹦��
 *
 */
class Decompose
{
public:	
	/**     
	 * \brief ���캯��
	 *
	 * ��ʼ����ر���
	 *
	 * param ob : ���ֽ���Ʒ
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
//��ָ,����
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
 * \brief ����
 *
 * ʵ������Ʒ���칦��
 *
 */	
class EquipMaker
{
	public:
		EquipMaker(SceneUser* user);

		/**     
		 * \brief ��������
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
					case ItemType_ClothBody ://101�����ʼ��������װ
						additivePercent(ob->data.maxhp, bob->maxsp);
						break;
					case ItemType_FellBody :	    //102����Ƥ�׼�ħ�����װ
						additivePercent(ob->data.mdefence, bob->maxsp);	
						break;
					case ItemType_MetalBody:	//103����������׼�������װ
					case ItemType_Shield: 	//112���������
						additivePercent(ob->data.pdefence, bob->maxsp);			
						break;
					case ItemType_Blade:		    //104����������������
					case ItemType_Sword:        //105����������������
					case ItemType_Axe:	           //106����������������
					case ItemType_Hammer:	        //107����������������
					case ItemType_Crossbow:	        //109���������������
						additivePercent(ob->data.pdamage, bob->maxsp);		
						additivePercent(ob->data.maxpdamage, bob->maxsp);		
						break;
					case ItemType_Staff:		    //108��������������
					case ItemType_Stick:	        //111�����ٻ���������
					case ItemType_Fan:	           //110������Ů����
						additivePercent(ob->data.mdamage, bob->maxsp);		
						additivePercent(ob->data.maxmdamage, bob->maxsp);		
						break;
					case ItemType_Helm:		//113�����ɫͷ����
					case ItemType_Caestus:	//114�����ɫ������
					case ItemType_Cuff:		//115�����ɫ������
					case ItemType_Shoes:		//116�����ɫЬ����
						if (zMisc::randBetween(0, 1)) {
							additivePercent(ob->data.pdefence, bob->maxsp);		
						}else {
							additivePercent(ob->data.mdefence, bob->maxsp);				
						}
						break;
					case ItemType_Necklace:	//117�����ɫ������
					case ItemType_Fing:		//118�����ɫ��ָ��
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
		 * \brief ����ȡ���ֵ
		 *
		 * \param ret: ������
		 * \param lv: ����ȡֵ��Χ
		 * \return ��
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
		 * \brief ��������װ��
		 *
		 * \param bob: ��Ӧװ��������
		 * \param ob: ������Ʒ
		 * \param kind: װ������
		 * \return ��ǰ���Ƿ���true
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


				COMPUTE_R( pdamage )		// ��С�﹥
					COMPUTE_R( maxpdamage )		// ����﹥
					COMPUTE_R( mdamage )			// ��Сħ��
					COMPUTE_R( maxmdamage )		// ���ħ��
					COMPUTE_R( pdefence )			// ���
					COMPUTE_R( mdefence )			// ħ��

					if (props) {
						int index = zMisc::randBetween(0, 4);
						if (index!=5) {

							//���ڲ��������,ֱ��ȡֵ
							additivePercent(*ob->_p1[index], bob->_p1[index]);
						}else {
							fix_kind(bob, ob);
						}
					}else {
						COMPUTE_L( str )			// ����
							COMPUTE_L( inte )			// ����
							COMPUTE_L( dex )			// ����
							COMPUTE_L( spi )			// ����
							COMPUTE_L( con )				// ����
					}		

				COMPUTE_L( maxhp )		// �������ֵ
					COMPUTE_L( maxmp )		// �����ֵ
					//		COMPUTE_L( maxsp )		// �������ֵ

					COMPUTE_L( mvspeed )		// �ƶ��ٶ�
					COMPUTE_L( hpr )			// ����ֵ�ָ�
					COMPUTE_L( mpr )			// ����ֵ�ָ�
					COMPUTE_L( spr )			// ����ֵ�ָ�
					COMPUTE_L( akspeed )		// �����ٶ�

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
						//ÿ������Ҫ���
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
						//ȡ���ֵ
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
						COMPUTE_L( pdam )		// ������������
							COMPUTE_L( mdam )		// ����ħ��������
							COMPUTE_L( pdef )		// �������������
							COMPUTE_L( mdef )		// ����ħ��������
							COMPUTE_L( atrating )		// ������
							COMPUTE_L( akdodge )		// ������

							COMPUTE_L( poisondef )	// ��������
							COMPUTE_L( lulldef )		// ���������
							COMPUTE_L( reeldef )		// ��ѣ������
							COMPUTE_L( evildef )		// ����ħ����
							COMPUTE_L( bitedef )		// ����������
							COMPUTE_L( chaosdef )	// ����������
							COMPUTE_L( colddef )		// ����������
							COMPUTE_L( petrifydef )		// ��ʯ������
							COMPUTE_L( blinddef )		// ��ʧ������
							COMPUTE_L( stabledef )		// ����������
							COMPUTE_L( slowdef )		// ����������
							COMPUTE_L( luredef )		// ���ջ�����
					}

				if (!ob->data.durpoint) {
					if (additive(ob->data.durpoint, bob->resumedur, property)) {
						ob->data.durpoint -= bob->resumedur.sleightValue;
					}
					if (ob->data.durpoint) ob->data.dursecond = bob->resumedur.sleightValue;

				}

				COMPUTE_L( bang ) 			//�ػ�
					//ob->data.bang += bob->bang;
					//��ָ��������һ��
					if(ob->base->kind == ItemType_Fing || ob->base->kind == ItemType_Necklace)
					{
						BONUS_SKILL_RING
					}
				BONUS_SKILL
					BONUS_SKILLS

					if (props) 
						ob->data.kind |= 2;//��ɫװ��
					else 
						ob->data.kind |= kind;//��ɫװ��

				return true;
			}

		bool assign_holy(zObject* ob, int holy);

		bool assign_set(zObject* ob);

		/**     
		 * \brief ���Լ���
		 *
		 * \param ret: ������
		 * \param lv: ����ȡֵ��Χ
		 * \return ��
		 */	
		template <typename T>
			void additive(T& ret, const rangeValue &rv)
			{
				ret += zMisc::randBetween(rv.min,rv.max);
			}

		/**     
		 * \brief ��ʥ���Լ���
		 *
		 * \param ret: ������
		 * \param lv: ����ȡֵ��Χ
		 * \param property: ��Ʒ��ǰ������Ŀ
		 * \return ��
		 */	
		template <typename T>
			bool additive(T& ret, const luckRangeValue & lv, int& property)
			{
				int odds = lv.per;
				//		int odds = odds_of_property(lv.per, property);
				//		Zebra::logger->debug("���Բ�������%f, %f", lv.per*1.0, odds*1.0);
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
 * \brief ��Ʒ����
 *
 * ʵ�ָ�����Ʒ���칦��,�ṩһ��ͳһ���
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
	 * \brief ���캯��
	 *
	 */   	
	RebuildObject() { }
	
	/**     
	 * \brief ��������
	 *
	 */     
	~RebuildObject() { }

	
	static RebuildObject* _instance;
};

#endif
