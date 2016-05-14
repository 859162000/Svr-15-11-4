#ifndef _OBJECT_H
#define _OBJECT_H

typedef unsigned long tItemThisID ;
#define INVALID_THISID 0xffffffff

// 道具的类型
enum enumItemType
{
	ItemType_None,

	ItemType_Resource = 16,	//16代表原料类

	ItemType_Leechdom,	//17代表药品类
	ItemType_FoodRes,	//18代表食物原料类
	ItemType_Food,		//19代表食物类
	ItemType_Tools,		//20代表劳动工具类
	ItemType_Arrow,		//21代表配合弓使用的箭支类
	ItemType_BattleHorse,	//22战马
	ItemType_Pack,		//23代表包裹类
	ItemType_Money,		//24代表金钱类
	ItemType_Scroll,	//25代表转移卷轴类
	ItemType_Move,		//26代表特殊移动道具类
	ItemType_LevelUp,	//27代表道具升级需要的材料类
	ItemType_CaptureWeapon,	//28代表驯服宠物用武器
	ItemType_Union,	//29代表创建帮会需要的道具.
	ItemType_Tonic,	//30表示自动补药类道具.
	ItemType_Gift,	//31代表礼品类物品.
	ItemType_Other, 
	ItemType_MASK = 33, 	//33代表蒙面巾
	ItemType_Quest = 34,
	ItemType_HORSE = 35,
	ItemType_SOULSTONE = 37, //37代表魂魄石类
	ItemType_Wedding = 38, //38代表婚礼类
	ItemType_Change = 41,   //41 代表合成道具
	ItemType_Auto = 42,   //42 代表自动练功
	ItemType_SkillUp = 43,   //43 代表技能升级道具
	ItemType_Book = 44, //44代表书籍
	ItemType_Store = 45,   //45 代表仓库
	ItemType_Renew = 46,   //46 代表洗点道具
	ItemType_Repair = 47, //47代表修复宝石类
	ItemType_DoubleExp = 52, //52代表双倍经验类型
	ItemType_Honor = 53, //53代表荣誉之星类型
	ItemType_TONG = 54,  //帮主令	
	ItemType_FAMILY = 55,  //家族令
	ItemType_Adonment = 56, //56代表装饰品
	ItemType_SpecialBook = 57, //57代表特殊书籍
	ItemType_GreatLeechdom = 58, //58大计量药品
	ItemType_ClearProperty = 59, //59洗点道具
	ItemType_UseSkill = 60, // 附带技能类道具
	ItemType_Amulet = 61, // 护身符类道具
	ItemType_GreatLeechdomMp = 62,//62大计量自动补兰道具
	ItemType_KING = 65,  //国王令

	ItemType_ClothBody =101,		//101代表布质加生命类服装
	ItemType_FellBody =102,		    //102代表皮甲加魔防类服装
	ItemType_MetalBody =103,		//103代表金属铠甲加物防类服装
	ItemType_Blade =104,		    //104代表武术刀类武器
	ItemType_Sword =105,	        //105代表武术剑类武器
	ItemType_Axe =106,	           //106代表武术斧类武器
	ItemType_Hammer =107,	        //107代表武术斧类武器
	ItemType_Staff =108,		    //108代表法术杖类武器
	ItemType_Crossbow =109,	        //109代表箭术弓类武器
	ItemType_Fan =110,	           //110代表美女扇类
	ItemType_Stick =111,	        //111代表召唤棍类武器
	ItemType_Shield =112,	//112代表盾牌类
	ItemType_Helm =113,		//113代表角色头盔类
	ItemType_Caestus =114,	//114代表角色腰带类
	ItemType_Cuff = 115,		//115代表角色护腕类
	ItemType_Shoes = 116,		//116代表角色鞋子类
	ItemType_Necklace = 117,	//117代表角色项链类
	ItemType_Fing = 118,		//118代表角色戒指类
	ItemType_FashionBody = 119,		//119代表时装
	ItemType_Flower = 120,		//120代表鲜花,采集手套...
	ItemType_BMW = 121,		//119代表宝马
};

#define BOW_ARROW_ITEM_TYPE 21

namespace Object
{

enum {
	INVALID_LOC = (DWORD) -1,
	INVALID_TAB = (DWORD) -1,
	INVALID_X = (WORD) -1,
	INVALID_Y = (WORD) -1,
};

}

#pragma pack(1)

struct stObjectLocation{

private:
	DWORD dwLocation;	// 格子类型
	DWORD dwTableID;	// 包袱ID
	WORD  x;
	WORD  y;

	friend class Package;
public:
	stObjectLocation() 
		: dwLocation(Object::INVALID_LOC), dwTableID(Object::INVALID_TAB), 
		x(Object::INVALID_X), y(Object::INVALID_Y)
	{

	}

	stObjectLocation(const stObjectLocation& loc) 
		: dwLocation(loc.dwLocation), dwTableID(loc.dwTableID), x(loc.x), y(loc.y)
	{

	}

	stObjectLocation(DWORD loc, DWORD tab, WORD _x, WORD _y) 
		: dwLocation(loc), dwTableID(tab), x(_x), y(_y)
	{

	}
	
	void operator = (const stObjectLocation& loc)
	{
		dwLocation =loc.dwLocation;
		dwTableID = loc.dwTableID;
		x = loc.x;
		y = loc.y;
	}
	
	bool operator == (const stObjectLocation & st) const
	{
		return dwLocation == st.dwLocation && dwTableID == st.dwTableID && x == st.x && y == st.y;
	}

	bool operator != (const stObjectLocation & st) const
	{
		return !(*this == st);
	}

	DWORD tab() const
	{
		return dwTableID;
	}

	void tab(DWORD t) 
	{
		dwTableID = t;
	}

	DWORD loc() const
	{
		return dwLocation;
	}

	WORD xpos() const
	{
		return x;
	}

	WORD ypos() const
	{
		return y;
	}
};

struct oskill
{
	DWORD id;
	BYTE level;
	BYTE count;
	BYTE maxcount;
	BYTE other;
};

typedef struct _Object
{
	DWORD qwThisID;   //物品唯一id
	DWORD dwObjectID;  ////物品类别id
	char strName[MAX_NAMESIZE]; //名称
	
	stObjectLocation pos;	// 位置
	DWORD dwNum;	// 数量
	BYTE upgrade;//物品升级等级
	BYTE kind;	//物品类型, 0普通, 1蓝色, 2金色, 4神圣, 8套装
	DWORD exp;  //道具经验
	
	WORD needlevel;				// 需要等级

	WORD maxhp;					// 最大生命值
	WORD maxmp;					// 最大法术值
	WORD maxsp;					// 最大体力值

	WORD pdamage;				// 最小攻击力
	WORD maxpdamage;			// 最大攻击力
	WORD mdamage;				// 最小法术攻击力
	WORD maxmdamage;			// 最大法术攻击力

	WORD pdefence;				// 物防
	WORD mdefence;				// 魔防
	BYTE damagebonus;			// 伤害加成 x% from 道具基本表
	BYTE damage;				// 增加伤害值x％ from 神圣装备表
		
	WORD akspeed;				// 攻击速度
	WORD mvspeed;				// 移动速度
	WORD atrating;				// 命中率
	WORD akdodge;				// 躲避率

	DWORD color;				// 颜色	

	WORD str;  // 力量
	WORD inte;  // 智力
	WORD dex;  // 敏捷
	WORD spi;  // 精神
	WORD con;  // 体质
	
	WORD fivetype;  // 五行属性
	WORD fivepoint; // 五行属性
	
	WORD hpr;  // 生命值恢复
	WORD mpr;  // 法术值恢复
	WORD spr;  // 体力值恢复

	WORD holy;  //神圣一击	
	WORD bang;  //重击
	WORD pdam;  // 增加物理攻击力
	WORD pdef;  // 增加物理防御力
	WORD mdam;  // 增加魔法攻击力
	WORD mdef;  // 增加魔法防御力
	
	WORD poisondef; //抗毒增加
	WORD lulldef; //抗麻痹增加
	WORD reeldef; //抗眩晕增加
	WORD evildef; //抗噬魔增加
	WORD bitedef; //抗噬力增加
	WORD chaosdef; //抗混乱增加
	WORD colddef; //抗冰冻增加
	WORD petrifydef; //抗石化增加
	WORD blinddef; //抗失明增加
	WORD stabledef; //抗定身增加
	WORD slowdef; //抗减速增加
	WORD luredef; //抗诱惑增加

	WORD durpoint; //恢复装备耐久度点数
	WORD dursecond; //恢复装备耐久度时间单位

	struct skillbonus {
		WORD id; //技能 id
		WORD point; // 技能点数
	} skill[10]; //技能加成

	struct skillsbonus {
		WORD id; //技能 id
		WORD point; // 技能点数
	} skills;	//全系技能加成

	WORD poison; //中毒增加
	WORD lull; //麻痹增加
	WORD reel; //眩晕增加
	WORD evil; //噬魔增加
	WORD bite; //噬力增加
	WORD chaos; //混乱增加
	WORD cold; //冰冻增加
	WORD petrify; //石化增加
	WORD blind; //失明增加
	WORD stable; //定身增加
	WORD slow; //减速增加
	WORD lure; //诱惑增加
	
	struct leech
	{
		BYTE odds;    //x
		WORD effect;	//y
	};
	leech hpleech; //x%吸收生命值y
	leech mpleech; //x%吸收法术值y
	
	BYTE hptomp; //转换生命值为法术值x％
	BYTE dhpp; //物理伤害减少x%	
	BYTE dmpp; //法术伤害值减少x%		

	BYTE incgold; //增加金钱掉落x%
	BYTE doublexp; //x%双倍经验		
	BYTE mf; //增加掉宝率x%
	
	BYTE bind;  //装备是否绑定

	union {
		BYTE _five_props[5];
		struct {
			//五行套装相关属性
			BYTE dpdam; //物理伤害减少%x
			BYTE dmdam; //法术伤害减少%x
			BYTE bdam; //增加伤害x%
			BYTE rdam; //伤害反射%x
			BYTE ignoredef; //%x忽视目标防御
		};
	};

	WORD fiveset[5]; //五行套装, 按顺序排列

	//...
	BYTE width;  //宽度
	BYTE height; //高度
	WORD dur;    //当前耐久
	WORD maxdur; //最大耐久
	
	DWORD socket[6]; //孔
	DWORD price;     //价格
	DWORD cardpoint; //点卡

	char maker[MAX_NAMESIZE]; //打造者

}t_Object;

#pragma pack()

namespace Object
{
	static stObjectLocation INVALID_POS;
}

#endif

