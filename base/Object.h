#ifndef _OBJECT_H
#define _OBJECT_H

typedef unsigned long tItemThisID ;
#define INVALID_THISID 0xffffffff

// ���ߵ�����
enum enumItemType
{
	ItemType_None,

	ItemType_Resource = 16,	//16����ԭ����

	ItemType_Leechdom,	//17����ҩƷ��
	ItemType_FoodRes,	//18����ʳ��ԭ����
	ItemType_Food,		//19����ʳ����
	ItemType_Tools,		//20�����Ͷ�������
	ItemType_Arrow,		//21������Ϲ�ʹ�õļ�֧��
	ItemType_BattleHorse,	//22ս��
	ItemType_Pack,		//23���������
	ItemType_Money,		//24�����Ǯ��
	ItemType_Scroll,	//25����ת�ƾ�����
	ItemType_Move,		//26���������ƶ�������
	ItemType_LevelUp,	//27�������������Ҫ�Ĳ�����
	ItemType_CaptureWeapon,	//28����ѱ������������
	ItemType_Union,	//29�����������Ҫ�ĵ���.
	ItemType_Tonic,	//30��ʾ�Զ���ҩ�����.
	ItemType_Gift,	//31������Ʒ����Ʒ.
	ItemType_Other, 
	ItemType_MASK = 33, 	//33���������
	ItemType_Quest = 34,
	ItemType_HORSE = 35,
	ItemType_SOULSTONE = 37, //37�������ʯ��
	ItemType_Wedding = 38, //38���������
	ItemType_Change = 41,   //41 ����ϳɵ���
	ItemType_Auto = 42,   //42 �����Զ�����
	ItemType_SkillUp = 43,   //43 ��������������
	ItemType_Book = 44, //44�����鼮
	ItemType_Store = 45,   //45 ����ֿ�
	ItemType_Renew = 46,   //46 ����ϴ�����
	ItemType_Repair = 47, //47�����޸���ʯ��
	ItemType_DoubleExp = 52, //52����˫����������
	ItemType_Honor = 53, //53��������֮������
	ItemType_TONG = 54,  //������	
	ItemType_FAMILY = 55,  //������
	ItemType_Adonment = 56, //56����װ��Ʒ
	ItemType_SpecialBook = 57, //57���������鼮
	ItemType_GreatLeechdom = 58, //58�����ҩƷ
	ItemType_ClearProperty = 59, //59ϴ�����
	ItemType_UseSkill = 60, // �������������
	ItemType_Amulet = 61, // ����������
	ItemType_GreatLeechdomMp = 62,//62������Զ���������
	ItemType_KING = 65,  //������

	ItemType_ClothBody =101,		//101�����ʼ��������װ
	ItemType_FellBody =102,		    //102����Ƥ�׼�ħ�����װ
	ItemType_MetalBody =103,		//103����������׼�������װ
	ItemType_Blade =104,		    //104����������������
	ItemType_Sword =105,	        //105����������������
	ItemType_Axe =106,	           //106����������������
	ItemType_Hammer =107,	        //107����������������
	ItemType_Staff =108,		    //108��������������
	ItemType_Crossbow =109,	        //109���������������
	ItemType_Fan =110,	           //110������Ů����
	ItemType_Stick =111,	        //111�����ٻ���������
	ItemType_Shield =112,	//112���������
	ItemType_Helm =113,		//113�����ɫͷ����
	ItemType_Caestus =114,	//114�����ɫ������
	ItemType_Cuff = 115,		//115�����ɫ������
	ItemType_Shoes = 116,		//116�����ɫЬ����
	ItemType_Necklace = 117,	//117�����ɫ������
	ItemType_Fing = 118,		//118�����ɫ��ָ��
	ItemType_FashionBody = 119,		//119����ʱװ
	ItemType_Flower = 120,		//120�����ʻ�,�ɼ�����...
	ItemType_BMW = 121,		//119������
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
	DWORD dwLocation;	// ��������
	DWORD dwTableID;	// ����ID
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
	DWORD qwThisID;   //��ƷΨһid
	DWORD dwObjectID;  ////��Ʒ���id
	char strName[MAX_NAMESIZE]; //����
	
	stObjectLocation pos;	// λ��
	DWORD dwNum;	// ����
	BYTE upgrade;//��Ʒ�����ȼ�
	BYTE kind;	//��Ʒ����, 0��ͨ, 1��ɫ, 2��ɫ, 4��ʥ, 8��װ
	DWORD exp;  //���߾���
	
	WORD needlevel;				// ��Ҫ�ȼ�

	WORD maxhp;					// �������ֵ
	WORD maxmp;					// �����ֵ
	WORD maxsp;					// �������ֵ

	WORD pdamage;				// ��С������
	WORD maxpdamage;			// ��󹥻���
	WORD mdamage;				// ��С����������
	WORD maxmdamage;			// �����������

	WORD pdefence;				// ���
	WORD mdefence;				// ħ��
	BYTE damagebonus;			// �˺��ӳ� x% from ���߻�����
	BYTE damage;				// �����˺�ֵx�� from ��ʥװ����
		
	WORD akspeed;				// �����ٶ�
	WORD mvspeed;				// �ƶ��ٶ�
	WORD atrating;				// ������
	WORD akdodge;				// �����

	DWORD color;				// ��ɫ	

	WORD str;  // ����
	WORD inte;  // ����
	WORD dex;  // ����
	WORD spi;  // ����
	WORD con;  // ����
	
	WORD fivetype;  // ��������
	WORD fivepoint; // ��������
	
	WORD hpr;  // ����ֵ�ָ�
	WORD mpr;  // ����ֵ�ָ�
	WORD spr;  // ����ֵ�ָ�

	WORD holy;  //��ʥһ��	
	WORD bang;  //�ػ�
	WORD pdam;  // ������������
	WORD pdef;  // �������������
	WORD mdam;  // ����ħ��������
	WORD mdef;  // ����ħ��������
	
	WORD poisondef; //��������
	WORD lulldef; //���������
	WORD reeldef; //��ѣ������
	WORD evildef; //����ħ����
	WORD bitedef; //����������
	WORD chaosdef; //����������
	WORD colddef; //����������
	WORD petrifydef; //��ʯ������
	WORD blinddef; //��ʧ������
	WORD stabledef; //����������
	WORD slowdef; //����������
	WORD luredef; //���ջ�����

	WORD durpoint; //�ָ�װ���;öȵ���
	WORD dursecond; //�ָ�װ���;ö�ʱ�䵥λ

	struct skillbonus {
		WORD id; //���� id
		WORD point; // ���ܵ���
	} skill[10]; //���ܼӳ�

	struct skillsbonus {
		WORD id; //���� id
		WORD point; // ���ܵ���
	} skills;	//ȫϵ���ܼӳ�

	WORD poison; //�ж�����
	WORD lull; //�������
	WORD reel; //ѣ������
	WORD evil; //��ħ����
	WORD bite; //��������
	WORD chaos; //��������
	WORD cold; //��������
	WORD petrify; //ʯ������
	WORD blind; //ʧ������
	WORD stable; //��������
	WORD slow; //��������
	WORD lure; //�ջ�����
	
	struct leech
	{
		BYTE odds;    //x
		WORD effect;	//y
	};
	leech hpleech; //x%��������ֵy
	leech mpleech; //x%���շ���ֵy
	
	BYTE hptomp; //ת������ֵΪ����ֵx��
	BYTE dhpp; //�����˺�����x%	
	BYTE dmpp; //�����˺�ֵ����x%		

	BYTE incgold; //���ӽ�Ǯ����x%
	BYTE doublexp; //x%˫������		
	BYTE mf; //���ӵ�����x%
	
	BYTE bind;  //װ���Ƿ��

	union {
		BYTE _five_props[5];
		struct {
			//������װ�������
			BYTE dpdam; //�����˺�����%x
			BYTE dmdam; //�����˺�����%x
			BYTE bdam; //�����˺�x%
			BYTE rdam; //�˺�����%x
			BYTE ignoredef; //%x����Ŀ�����
		};
	};

	WORD fiveset[5]; //������װ, ��˳������

	//...
	BYTE width;  //���
	BYTE height; //�߶�
	WORD dur;    //��ǰ�;�
	WORD maxdur; //����;�
	
	DWORD socket[6]; //��
	DWORD price;     //�۸�
	DWORD cardpoint; //�㿨

	char maker[MAX_NAMESIZE]; //������

}t_Object;

#pragma pack()

namespace Object
{
	static stObjectLocation INVALID_POS;
}

#endif

