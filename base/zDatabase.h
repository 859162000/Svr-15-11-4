/**
 * \file
 * \version  $Id: zDatabase.h  $
 * \author  
 * \date 
 * \brief Zebra��Ϸ�����л������ݽṹ����������
 */

#ifndef _ZDATABASE_H_
#define _ZDATABASE_H_

#include <vector>

#include "zEntry.h"
#include "zMisc.h"
#include "zMutex.h"
#include "zString.h"
#include "zNoncopyable.h"
#include "Zebra.h"
#include "Command.h"
#include "SkillBase.h"
#include "Object.h"

#pragma pack(1)
//------------------------------------
// ObjectBase
//------------------------------------
struct ObjectBase{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	
	DWORD	dwField0;		// ���
	char	strField1[64];		// ����
	DWORD	dwField2;		// �������
	DWORD	dwField3;		// ����
	DWORD	dwField4;		// ��Ҫ�Ա�
	DWORD	dwField5;		// ��Ҫ�ȼ�
	DWORD	dwField6;		// ���ߵȼ�
	char	strField7[256];		// ��ɫ��ʶ
	char	strField8[256];		// ��ɫ��ʶ
	DWORD	dwField9;		// �����Ʒ
	char	strField10[256];		// ҩƷ����
	DWORD	dwField11;		// �������ֵ
	DWORD	dwField12;		// �����ֵ
	DWORD	dwField13;		// �������ֵ
	DWORD	dwField14;		// ��С�﹥
	DWORD	dwField15;		// ����﹥
	DWORD	dwField16;		// ��Сħ��
	DWORD	dwField17;		// ���ħ��
	DWORD	dwField18;		// ���
	DWORD	dwField19;		// ħ��
	DWORD	dwField20;		// �˺��ӳ�
	char	strField21[64];		// ��
	DWORD	dwField22;		// �����ٶ�
	DWORD	dwField23;		// �ƶ��ٶ�
	DWORD	dwField24;		// ������
	DWORD	dwField25;		// �����
	DWORD	dwField26;		// ����
	DWORD	dwField27;		// �ϳɵȼ�
	DWORD	dwField28;		// ����
	char	strField29[32];		// ��Ҫ����
	char	strField30[1024];		// ��Ҫԭ��
	DWORD	dwField31;		// װ��λ��
	DWORD	dwField32;		// �;ö�
	DWORD	dwField33;		// �۸�
	DWORD	dwField34;		// ��ɫװ�����伸��
	DWORD	dwField35;		// ��ɫװ�����伸��
	DWORD	dwField36;		// ��ɫ
	DWORD	dwField37;		// ���ӿ�
	DWORD	dwField38;		// ���Ӹ�
	DWORD	dwField39;		// ����
	DWORD	dwField40;		// �ϳɵ���
	DWORD	dwField41;		// �ػ�
	DWORD	dwField42;		// ��ʥ����
	char	strField43[256];		// ��ʥ��ʶ	
};//���� ObjectBase �ɹ����� 940 ����¼

/**
 * \brief ��Ʒ������
 */
struct zObjectB:public zEntry
{
	DWORD maxnum;				// �������
	BYTE kind;					// ����
	BYTE sex;					// ��Ҫ�Ա�
	WORD level;					// ���ߵȼ�
	std::vector<DWORD> blues;	//��ɫ��ʶ
	std::vector<DWORD> golds;	//��ɫ��ʶ
	WORD needobject;			// �����Ʒ
	struct leechdom_t {
		BYTE id; //���ܱ�ʶ
		WORD effect; //Ч��
		WORD time; //ʱ��
		leechdom_t(const std::string& _id="", const std::string& _effect="", const std::string& _time="") 
			: id(atoi(_id.c_str())), effect(atoi(_effect.c_str())), time(atoi(_time.c_str()))
		{ }
	} leechdom ; 				// ҩƷ����

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
	WORD damagebonus;			// �˺��ӳ�
	
	WORD akspeed;				// �����ٶ�
	WORD mvspeed;				// �ƶ��ٶ�
	WORD atrating;				// ������
	WORD akdodge;				// �����

	DWORD color;				// ��ɫ	
	
	struct socket
	{
		WORD odds;
		BYTE min;
		BYTE max;
		socket(const std::string& odds_, const std::string& number_) : odds(atoi(odds_.c_str())), min(0), max(0)
		{
			std::string::size_type pos = 0;
			if  ( (pos = number_.find("-")) != std::string::npos ) {
				
				min = atoi(number_.substr(0, pos).c_str());
				max = atoi(number_.substr(pos+strlen("-")).c_str());
				//if (odds) Zebra::logger->debug("odds:%d\tmin:%d\tmax:%d", odds, min, max);
			}
		}
	} hole;						//��
	
	BYTE recast;				// ����

	BYTE recastlevel; 			// �ϳɵȼ�
	WORD recastcost;			// �ϳɵ���
	

	WORD make;					// ����
	struct skills 
	{
		WORD id;
		BYTE level;
		skills(const std::string& id_="0", const std::string& level_="0") : id(atoi(id_.c_str())), level(atoi(level_.c_str()))
		{ }
	};
	skills need_skill;			// ��Ҫ����
	
	struct material
	{
		WORD gold;
		struct  stuff
		{
			WORD id;
			WORD number;
			BYTE level;
			stuff(const std::string& id_, const std::string& level_, const std::string& number_) : id(atoi(id_.c_str())), number(atoi(number_.c_str())), level(atoi(level_.c_str()))
			{ }	
		};
		std::vector<stuff> stuffs;
		typedef std::vector<stuff>::iterator stuffs_iterator;
	};
	material need_material;		// ��Ҫԭ��
	
	BYTE setpos;				// װ��λ��
	WORD durability;			// �;ö�
	DWORD price;				// �۸�
	DWORD bluerating;			// ��ɫװ�����伸��
	DWORD goldrating;			// ��ɫװ�����伸��
	BYTE width;					// ���ӿ�
	BYTE height;				// ���Ӹ�
	union
	{
	DWORD cardpoint;			// ���� (�Ѿ�����)
	DWORD cointype;				// ��������
	};
	WORD bang;					//�ػ�
	DWORD holyrating;			//��ʥ����
	std::vector<DWORD> holys;     //��ʥ��ʶ
	
	void fill(ObjectBase &data)
	{
		id = data.dwField0;
		strncpy(name, data.strField1, MAX_NAMESIZE);

		maxnum = data.dwField2;	
		kind = data.dwField3;	
		sex = data.dwField4;	
		needlevel =	data.dwField5;	
		level =	data.dwField6;	

		init_identifier(blues, data.strField7);
		init_identifier(golds, data.strField8);

		needobject = data.dwField9;	
		init_leechdom(data.strField10);

		maxhp = data.dwField11;	
		maxmp = data.dwField12;
		maxsp =	data.dwField13;

		pdamage = data.dwField14;
		maxpdamage = data.dwField15;
		mdamage = data.dwField16;
		maxmdamage = data.dwField17;
		pdefence = data.dwField18;
		mdefence = data.dwField19;
		damagebonus = data.dwField20;

		init_socket(data.strField21);

		akspeed = data.dwField22;
		mvspeed = data.dwField23;
		atrating = data.dwField24;
		akdodge = data.dwField25;
		
		recast = data.dwField26;
		recastlevel = data.dwField27;

		make = data.dwField28;

		init_need_skills(data.strField29);
		init_need_material(data.strField30);
		
		setpos = data.dwField31;
		durability = data.dwField32;
		price =	data.dwField33;

		bluerating = data.dwField34;
		goldrating = data.dwField35;
		color = data.dwField36;
		width =	data.dwField37;
		height = data.dwField38;
		cardpoint = data.dwField39;
		recastcost = data.dwField40;
		bang = data.dwField41;

		holyrating = data.dwField42;
		init_identifier(holys, data.strField43);
		
	}

	zObjectB():zEntry(), hole("0", "0-0")
	{
		bzero(this,sizeof(zObjectB));
	};
	
	void init_identifier(std::vector<DWORD>& list, const std::string& info)
	{
		list.clear();
		zMisc::getAllNum(info.c_str(), list);
	}

	void init_leechdom(const std::string& info)
	{
		leechdom_t* p = Misc::Parse3<leechdom_t>()(info, ":");
		if (p) {
			leechdom = *p;
			SAFE_DELETE(p);
		}	
	}	

	void init_socket(const std::string& socket_info)
	{
		std::string::size_type pos = socket_info.find(':');
		if (pos != std::string::npos) {
			hole = socket(socket_info.substr(0, pos), socket_info.substr(pos+1));
		}
		
	}
	
	void init_need_skills(const std::string& skills_list)
	{	
		std::string::size_type pos = skills_list.find(':');
		if (pos != std::string::npos) {
			need_skill = skills(skills_list.substr(0, pos), skills_list.substr(pos+1));
		}
	}

	void init_need_material(const std::string& materials)
	{
		need_material.stuffs.clear();
		Misc::Split<Misc::Parse3> p;
		std::string::size_type pos = materials.find(':');
		if (pos != std::string::npos) {
			need_material.gold = atoi(materials.substr(0, pos).c_str());
			p(materials.substr(pos+1), need_material.stuffs, ";", "-");
		}
	}
	
};

//------------------------------------
// ColorObjectBase
//------------------------------------
struct ColorObjectBase{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	
	DWORD	dwField0;		// ���
	char	strField1[64];		// ����
	char	strField2[32];		// ���ӷ�
	char	strField3[128];		// ��ɫƷ��
	char	strField4[32];		// ����
	char	strField5[32];		// ����
	char	strField6[32];		// ����
	char	strField7[32];		// ����
	char	strField8[32];		// ����
	char	strField9[32];		// ��������
	char	strField10[32];		// ��С�﹥
	char	strField11[32];		// ����﹥
	char	strField12[32];		// ��Сħ��
	char	strField13[32];		// ���ħ��
	char	strField14[32];		// ���
	char	strField15[32];		// ħ��
	char	strField16[32];		// �������ֵ
	char	strField17[32];		// �����ֵ
	char	strField18[32];		// �������ֵ
	char	strField19[32];		// �ƶ��ٶ�
	char	strField20[32];		// ����ֵ�ָ�
	char	strField21[32];		// ����ֵ�ָ�
	char	strField22[32];		// ����ֵ�ָ�
	char	strField23[32];		// �����ٶ�
	char	strField24[32];		// ������������
	char	strField25[32];		// �������������
	char	strField26[32];		// ����ħ��������
	char	strField27[32];		// ����ħ��������
	char	strField28[32];		// ������
	char	strField29[32];		// ������
	char	strField30[32];		// ��������
	char	strField31[32];		// ���������
	char	strField32[32];		// ��ѣ������
	char	strField33[32];		// ����ħ����
	char	strField34[32];		// ����������
	char	strField35[32];		// ����������
	char	strField36[32];		// ����������
	char	strField37[32];		// ��ʯ������
	char	strField38[32];		// ��ʧ������
	char	strField39[32];		// ����������
	char	strField40[32];		// ����������
	char	strField41[32];		// ���ջ�����
	char	strField42[32];		// �ָ��;ö�
	char	strField43[32];		// �ػ�
	DWORD	dwField44;		// ��ʥװ������
	char	strField45[1024];		// ���ܼӳ�
	char	strField46[32];		// ȫϵ���ܼӳ�
};

//һ����Χֵ������
struct rangeValue
{
	WORD min;
	WORD max;
};

struct luckRangeValue
{
	WORD per;	//�Ƿ���������Եļ���
	rangeValue data;	//��������ֵ�������Χ
	WORD sleightValue;	//���������Ȳ����ļ�Ȩֵ
};

static void fillRangeValue(const char *str,rangeValue &data)
{
	std::vector<DWORD> num;
	int i =zMisc::getAllNum(str,num);
	if(i!=2)
	{
		data.min=0;
		data.max=0;
	}
	else
	{
		data.min=num[0];
		data.max=num[1];
	}
}

static void fillLuckRangeValue(char *str,luckRangeValue &data)
{
	std::vector<DWORD> num;
	int i =zMisc::getAllNum(str,num);
	if(i<3)
	{
		if(i!=1)
		{
			Zebra::logger->debug("%s,%s", __PRETTY_FUNCTION__,str);
		}
		data.per=0;
		data.data.min=0;
		data.data.max=0;
		data.sleightValue=0;
	}
	else
	{
		data.per=num[0];
		data.data.min=num[1];
		data.data.max=num[2];
		if(i==4)
			data.sleightValue=num[3];
		else
			data.sleightValue=0;
	}
}

struct skillbonus {
	WORD odds; //����
	WORD id; //���� id
	WORD level; // ���ܵȼ�
	skillbonus(std::string _odds="0", std::string _id="0", std::string _level="0") : odds(atoi(_odds.c_str())), id(atoi(_id.c_str())), level(atoi(_level.c_str()))
	{ }
}; 

template <class Base, WORD tt>
struct zColorObjectB:public zEntry
{
	//std::string prefix;			// ����
	char prefix[MAX_NAMESIZE];			// ����
	//std::string joint;			// ���ӷ�
	char joint[MAX_NAMESIZE];			// ���ӷ�
	std::vector<WORD> golds;	// ��ɫƷ��

	union {
		luckRangeValue _p1[5];
		struct {
			luckRangeValue str;			// ����
			luckRangeValue inte;		// ����
			luckRangeValue dex;			// ����
			luckRangeValue spi;			// ����
			luckRangeValue con;			// ����
		};	
	};
	luckRangeValue five;		// ��������
	
	rangeValue pdamage;			// ��С�﹥
	rangeValue maxpdamage;		// ����﹥
	rangeValue mdamage;			// ��Сħ��
	rangeValue maxmdamage;		// ���ħ��
	rangeValue pdefence;			// ���
	rangeValue mdefence;			// ħ��
	
	luckRangeValue maxhp;		// �������ֵ
	luckRangeValue maxmp;		// �����ֵ
	luckRangeValue maxsp;		// �������ֵ
	
	luckRangeValue mvspeed;		// �ƶ��ٶ�
	luckRangeValue hpr;			// ����ֵ�ָ�
	luckRangeValue mpr;			// ����ֵ�ָ�
	luckRangeValue spr;			// ����ֵ�ָ�
	luckRangeValue akspeed;		// �����ٶ�

	union {
		luckRangeValue _p2[18];
		struct {
			luckRangeValue pdam;		// ������������
			luckRangeValue pdef;		// �������������
			luckRangeValue mdam;		// ����ħ��������
			luckRangeValue mdef;		// ����ħ��������
			
			luckRangeValue poisondef;	// ��������
			luckRangeValue lulldef;		// ���������
			luckRangeValue reeldef;		// ��ѣ������
			luckRangeValue evildef;		// ����ħ����
			luckRangeValue bitedef;		// ����������
			luckRangeValue chaosdef;	// ����������
			luckRangeValue colddef;		// ����������
			luckRangeValue petrifydef;		// ��ʯ������
			luckRangeValue blinddef;		// ��ʧ������
			luckRangeValue stabledef;		// ����������
			luckRangeValue slowdef;		// ����������
			luckRangeValue luredef;		// ���ջ�����

			luckRangeValue atrating;		// ������
			luckRangeValue akdodge;		// ������

		};
	};	
	
	luckRangeValue resumedur;		// �ָ��;ö�
	luckRangeValue bang;		// �ػ�
	WORD holyrating;	//��ʥװ������

	std::vector<skillbonus> skill;	// ���ܼӳ�
	skillbonus skills;				// ȫϵ���ܼӳ�
	
	WORD type;

	public:
	void fill(Base &data)
	{
		id = data.dwField0;
		strncpy(name, data.strField1, MAX_NAMESIZE);

		//prefix =  data.strField1;
		//joint = data.strField2;
		strncpy(prefix, data.strField1, MAX_NAMESIZE);
		strncpy(joint, data.strField2, MAX_NAMESIZE);
		zMisc::getAllNum(data.strField3, golds);
		fillLuckRangeValue(data.strField4, str);
		fillLuckRangeValue(data.strField5, inte);
		fillLuckRangeValue(data.strField6, dex);
		fillLuckRangeValue(data.strField7, spi);
		fillLuckRangeValue(data.strField8, con);
		
		fillLuckRangeValue(data.strField9, five);
		
		fillRangeValue(data.strField10, pdamage);
		fillRangeValue(data.strField11, maxpdamage);
		fillRangeValue(data.strField12, mdamage);
		fillRangeValue(data.strField13, maxmdamage);
		fillRangeValue(data.strField14, pdefence);
		fillRangeValue(data.strField15, mdefence);
		
		fillLuckRangeValue(data.strField16, maxhp);
		fillLuckRangeValue(data.strField17, maxmp);
		fillLuckRangeValue(data.strField18, maxsp);
		fillLuckRangeValue(data.strField19, mvspeed);
		fillLuckRangeValue(data.strField20, hpr);
		fillLuckRangeValue(data.strField21, mpr);
		fillLuckRangeValue(data.strField22, spr);
		fillLuckRangeValue(data.strField23, akspeed);
		fillLuckRangeValue(data.strField24, pdam);
		fillLuckRangeValue(data.strField25, pdef);
		fillLuckRangeValue(data.strField26, mdam);
		fillLuckRangeValue(data.strField27, mdef);
		fillLuckRangeValue(data.strField28, atrating);
		fillLuckRangeValue(data.strField29, akdodge);

		fillLuckRangeValue(data.strField30, poisondef);
		fillLuckRangeValue(data.strField31, lulldef);
		fillLuckRangeValue(data.strField32, reeldef);
		fillLuckRangeValue(data.strField33, evildef);
		fillLuckRangeValue(data.strField34, bitedef);
		fillLuckRangeValue(data.strField35, chaosdef);
		fillLuckRangeValue(data.strField36, colddef);
		fillLuckRangeValue(data.strField37, petrifydef);
		fillLuckRangeValue(data.strField38, blinddef);
		fillLuckRangeValue(data.strField39, stabledef);
		fillLuckRangeValue(data.strField40, slowdef);
		fillLuckRangeValue(data.strField41, luredef);
		fillLuckRangeValue(data.strField42, resumedur);
		//bang = data.dwField43;
		fillLuckRangeValue(data.strField43, bang);
		holyrating = data.dwField44;
		
		init_skill(data.strField45);
		init_skills(data.strField46);

		//Zebra::logger->debug("id:%d, name:%s", id, name);
		#if 0
		//�ָ��;öȸ�ʽ��������
		{
			std::vector<DWORD> num;
			int i =zMisc::getAllNum(data.strField47,num);
			if(i!=7)
			{
				bzero(&durpoint,sizeof(durpoint));
				bzero(&dursecond,sizeof(dursecond));
			}
			else
			{
				durpoint.per=num[0];
				durpoint.data.min=num[1];
				durpoint.data.max=num[2];
				durpoint.sleightValue=num[3];
				dursecond.per=0;
				dursecond.data.min=num[4];
				dursecond.data.max=num[5];
				dursecond.sleightValue=num[6];
			}
		}
		#endif

	}
	
	zColorObjectB():zEntry()
	{
		bzero(this,sizeof(zColorObjectB));
		type=tt;
	};

	void init_skill(const std::string& info)
	{
		skill.clear();
		Misc::Split<Misc::Parse3> p;
		p(info, skill, ";", ":");
	}

	void init_skills(const std::string& info)
	{
		skillbonus* p = Misc::Parse3<skillbonus>()(info, ":");
		if (p) {
			skills = *p;
			SAFE_DELETE(p);
		}	
		else if(strcmp(info.c_str(),"0")!=0)
		{       
			Zebra::logger->debug("%s,(%d),%s", __PRETTY_FUNCTION__,id,info.c_str());
		}     
	}	
	
};

typedef ColorObjectBase GoldObjectBase;
typedef ColorObjectBase DropGoldObjectBase;
typedef ColorObjectBase BlueObjectBase;
typedef zColorObjectB<BlueObjectBase, 1> zBlueObjectB;
typedef zColorObjectB<GoldObjectBase, 2> zGoldObjectB;
typedef zColorObjectB<DropGoldObjectBase, 3> zDropGoldObjectB;

//------------------------------------
// SetObjectBase
//------------------------------------
struct SetObjectBase{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// ���
	char	strField1[32];		// ����
	char	strField2[64];		// ��װ5
	char	strField3[32];		// ��װ4
	char	strField4[32];		// ��װ3
	char	strField5[32];		// ��װ2
	char	strField6[32];		// ��װ1
	DWORD	dwField7;		// ���Ա�ʶ
};//���� SetObjectBase �ɹ����� 532 ����¼

struct zSetObjectB:public zEntry
{
	struct SET
	{
		WORD odds;
		std::vector<WORD> ids;
	};
	
	typedef std::vector<SET> SETS;
	typedef SETS::iterator iterator;
	SETS sets;
	DWORD mark;
	
	void fill(SetObjectBase& data)
	{
		id = data.dwField0;
		strncpy(name, data.strField1, MAX_NAMESIZE);
		init_set(data.strField2);
		init_set(data.strField3);
		init_set(data.strField4);
		init_set(data.strField5);
		init_set(data.strField6);
		mark = data.dwField7;
	}
	
	zSetObjectB():zEntry()
	{
		bzero(this, sizeof(zSetObjectB));
	};
	
	void init_set(const std::string& info)
	{
		sets.clear();
		std::string::size_type pos = info.find(':');
		SET set;
		if (pos != std::string::npos) {
			set.odds = atoi(info.substr(0, pos).c_str());
			zMisc::getAllNum(info.substr(pos+1).c_str(), set.ids);
		}
		sets.push_back(set);
	}
	
};

//------------------------------------
// FiveSetBase
//------------------------------------
struct FiveSetBase{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// ���Ա�ʶ
	char	strField1[16];		// �����˺�����x%
	char	strField2[16];		// �����˺�����x%
	char	strField3[16];		// �����˺�ֵx%
	char	strField4[16];		// �˺�����x%
	char	strField5[16];		// x%����Ŀ�����
};//���� FiveSetBase �ɹ����� 4 ����¼


struct zFiveSetB:public zEntry
{
	rangeValue dpdam; //�����˺�����%x
	rangeValue dmdam; //�����˺�����%x
	rangeValue bdam; //�����˺�x%
	rangeValue rdam; //�˺�����%x
	rangeValue ignoredef; //%x����Ŀ�����

	void fill(FiveSetBase& data)
	{
		id = data.dwField0;
		fillRangeValue(data.strField1, dpdam);
		fillRangeValue(data.strField2, dmdam);
		fillRangeValue(data.strField3, bdam);		
		fillRangeValue(data.strField4, rdam);		
		fillRangeValue(data.strField5, ignoredef);		
	}
	
	zFiveSetB():zEntry()
	{
		bzero(this, sizeof(zFiveSetB));
	};	
};

//------------------------------------
// HolyObjectBase
//------------------------------------
struct HolyObjectBase{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// ���
	char	strField1[64];		// ����
	char	strField2[16];		// ��ʥһ��
	char	strField3[16];		// �����˺�ֵx��
	char	strField4[16];		// ������������
	char	strField5[16];		// ����ֵ�ָ�
	char	strField6[16];		// ����ֵ�ָ�
	char	strField7[16];		// ����ֵ�ָ�
	char	strField8[16];		// �����ٶ�
	char	strField9[16];		// �ƶ��ٶ�
	char	strField10[16];		// ������
	char	strField11[16];		// ������
	char	strField12[16];		// ���ܼӳ�
	char	strField13[16];		// ȫϵ���ܼӳ�
	char	strField14[16];		// ˫������
	char	strField15[16];		// ���ӵ�����
};//���� HolyObjectBase �ɹ����� 705 ����¼

struct zHolyObjectB:public zEntry
{
	WORD	holy;				// ��ʥһ��
	luckRangeValue	damage;		// �����˺�ֵx��
	luckRangeValue	fivepoint;		// ������������

	luckRangeValue hpr;			// ����ֵ�ָ�
	luckRangeValue mpr;			// ����ֵ�ָ�
	luckRangeValue spr;			// ����ֵ�ָ�

	luckRangeValue akspeed;		// �����ٶ�
	luckRangeValue mvspeed;		// �ƶ��ٶ�
	
	luckRangeValue atrating;		// ������
	luckRangeValue akdodge;			// ������

	std::vector<skillbonus> skill;	// ���ܼӳ�
	skillbonus skills;				// ȫϵ���ܼӳ�

	luckRangeValue doublexp;		//%x˫������
	luckRangeValue mf; 			//������
	
	void fill(HolyObjectBase &data)
	{
		id = data.dwField0;
		strncpy(name, data.strField1, MAX_NAMESIZE);
		holy = atoi(data.strField2);

		fillLuckRangeValue(data.strField3, damage);		
		fillLuckRangeValue(data.strField4, fivepoint);
		fillLuckRangeValue(data.strField5, hpr);
		fillLuckRangeValue(data.strField6, mpr);
		fillLuckRangeValue(data.strField7, spr);
		fillLuckRangeValue(data.strField8, akspeed);
		fillLuckRangeValue(data.strField9, mvspeed);
		fillLuckRangeValue(data.strField10, atrating);
		fillLuckRangeValue(data.strField11, akdodge);
		
		init_skill(data.strField12);
		init_skills(data.strField13);

		fillLuckRangeValue(data.strField14, doublexp);
		fillLuckRangeValue(data.strField15, mf);

	}
	
	zHolyObjectB():zEntry()
	{
		bzero(this,sizeof(zHolyObjectB));
	};

	void init_skill(const std::string& info)
	{
		skill.clear();
		Misc::Split<Misc::Parse3> p;
		p(info, skill, ";", ":");
	}

	void init_skills(const std::string& info)
	{
		skillbonus* p = Misc::Parse3<skillbonus>()(info, ":");
		if (p) {
			skills = *p;
			SAFE_DELETE(p);
		}
	}	
};

//------------------------------------
// UpgradeObjectBase
//------------------------------------
struct UpgradeObjectBase{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// ���
	DWORD	dwField1;		// ��ƷID
	char	strField2[64];		// ����
	DWORD	dwField3;		// ����
	DWORD	dwField4;		// ����ԭ��
	DWORD	dwField5;		// ��Ҫ����
	DWORD	dwField6;		// ��Ӧ�ɹ���
	DWORD	dwField7;		// ��С�﹥����
	DWORD	dwField8;		// ����﹥����
	DWORD	dwField9;		// ��Сħ������
	DWORD	dwField10;		// ���ħ������
	DWORD	dwField11;		// �������
	DWORD	dwField12;		// ħ������
	DWORD	dwField13;		// ����ֵ����
};//���� UpgradeObjectBase �ɹ����� 6345 ����¼


struct zUpgradeObjectB:public zEntry
{
	DWORD dwObjectID;		// ��ƷID
	WORD level;		// ����
	
	WORD stuff;		// ����ԭ��
	
	WORD gold;		// ��Ҫ����
	
	WORD odds;		// ��Ӧ�ɹ���
	
	WORD pdamage;				// ��С������
	WORD maxpdamage;			// ��󹥻���
	WORD mdamage;				// ��С����������
	WORD maxmdamage;			// �����������

	WORD pdefence;				// ���
	WORD mdefence;				// ħ��
	WORD maxhp;					// �������ֵ
	
	void fill(UpgradeObjectBase  &data)
	{
		id = data.dwField0;
		dwObjectID = data.dwField1;
		strncpy(name, data.strField2, MAX_NAMESIZE);
		level = data.dwField3;
		stuff = data.dwField4;
		gold = data.dwField5;
		odds = data.dwField6;
		
		pdamage = data.dwField7;
		maxpdamage = data.dwField8;
		mdamage = data.dwField9;
		maxmdamage = data.dwField10;
		
		pdefence = data.dwField11;
		mdefence = data.dwField12;
		
		maxhp = data.dwField13;
	}

	zUpgradeObjectB():zEntry()
	{
		bzero(this,sizeof(zUpgradeObjectB));
	}
};

//------------------------------------
// NpcBase
//------------------------------------
struct NpcBase
{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// ���
	char	strField1[32];		// ����
	DWORD	dwField2;		// ����
	DWORD	dwField3;		// �ȼ�
	DWORD	dwField4;		// ����ֵ
	DWORD	dwField5;		// ����ֵ

	DWORD	dwField6;		// ��
	DWORD	dwField7;		// ��
	DWORD	dwField8;		// ����
	DWORD	dwField9;		// ����
	DWORD	dwField10;		// ����
	DWORD	dwField11;		// ����

	DWORD	dwField12;		// ��ɫ
	DWORD	dwField13;		// ai
	DWORD	dwField14;		// �ƶ����
	DWORD	dwField15;		// �������
	DWORD	dwField16;		// ��С���������
	DWORD	dwField17;		// ������������
	DWORD	dwField18;		// ��С����������
	DWORD	dwField19;		// �����������
	DWORD	dwField20;		// ��������
	DWORD	dwField21;		// ���е���
	char	strField22[1024];		// ��������
	DWORD	dwField23;		// ��С��������
	DWORD	dwField24;		// ���������
	DWORD	dwField25;		// ��С������
	DWORD	dwField26;		// ��󹥻���
	DWORD	dwField27;		// ����
	char	strField28[4096];		// Я����Ʒ
	DWORD	dwField29;		// ����֮ʯ����
	char	strField30[1024];		// ʹ�ü���
	char	strField31[1024];		// ״̬
	DWORD	dwField32;		// �����
	DWORD	dwField33;		// ������
	DWORD	dwField34;		// ͼƬ
	DWORD	dwField35;		// Ʒ��
	DWORD	dwField36;		// �������
	DWORD	dwField37;		// ֽ����ͼƬ
	char	strField38[64];		// ��Ѫ
	DWORD	dwField39;		// �����Ʊ�־
	DWORD	dwField40;		// �����Ʊ�־
};

struct CarryObject
{
	DWORD id;
	int   rate;
	int   minnum;
	int   maxnum;
	CarryObject()
	{
		id = 0;
		rate = 0;
		minnum = 0;
		maxnum = 0;
	}
};
#ifdef _POOL_ALLOC_
#include <ext/pool_allocator.h>
typedef std::vector<CarryObject, __gnu_cxx::__pool_alloc<CarryObject> > NpcLostObject;
#else
typedef std::vector<CarryObject> NpcLostObject;
#endif

struct NpcCarryObject : private zNoncopyable
{
	NpcCarryObject() {};
	bool set(const char *objects)
	{
		bool retval = true;
		//mlock.lock();
		cov.clear();
		if (strcmp(objects, "0"))
		{
			std::vector<std::string> obs;
			Zebra::stringtok(obs, objects, ";");
			for(std::vector<std::string>::const_iterator it = obs.begin(); it != obs.end(); it++)
			{
				std::vector<std::string> rt;
				Zebra::stringtok(rt, *it, ":");
				if (3 == rt.size())
				{
					CarryObject co;
					co.id = atoi(rt[0].c_str());
					co.rate = atoi(rt[1].c_str());
					std::vector<std::string> nu;
					Zebra::stringtok(nu, rt[2], "-");
					if (2 == nu.size())
					{
						co.minnum = atoi(nu[0].c_str());
						co.maxnum = atoi(nu[1].c_str());
						cov.push_back(co);
					}
					else
						retval = false;
				}
				else
					retval = false;
			}
		}
		//mlock.unlock();
		return retval;
	}

	/**
	 * \brief ��Ʒ���䴦��
	 * \param nlo npcЯ����Ʒ����
	 * \param value �����ʴ��۱�
	 * \param value1 ����������
	 * \param value2 ���ӵ���������
	 */
	void lost(NpcLostObject &nlo, int value=1, int value1=0, int value2=0, int vcharm = 0, int vlucky = 0, int player_level=0, bool double_drop=false, int level_double_drop=0)
	{
		//mlock.lock();
		if (vcharm>1000) vcharm=1000;
		if (vlucky>1000) vlucky=1000;
		for(std::vector<CarryObject>::const_iterator it = cov.begin(); it != cov.end(); it++)
		{
			//Zebra::logger->debug("%u, %u, %u, %u", (*it).id, (*it).rate, (*it).minnum, (*it).maxnum);
			switch((*it).id)
			{
				case 665:
					{
						int vrate = (int)(((*it).rate/value)*(1+value1/100.0f)*(1+value2/100.0f)*(1+vcharm/1000.0f)*(1+vlucky/1000.0f));
						if (zMisc::selectByTenTh(vrate))
						{
							nlo.push_back(*it);
						}
					}
					break;
				default:
					{
						int vrate = (int)(((*it).rate/value)*(1+value1/100.0f)*(1+vcharm/1000.0f)*(1+vlucky/1000.0f));
						if (double_drop && player_level<= level_double_drop)
						{
							if (zMisc::selectByTenTh(vrate*2))
							{
								nlo.push_back(*it);
							}
						}
						else
						{
							if (zMisc::selectByTenTh(vrate))
							{
								nlo.push_back(*it);
							}
						}
					}
					break;
			}
		}
		//mlock.unlock();
	}
	/**
	 * \brief ȫ����Ʒ���䴦��
	 * \param nlo npcЯ����Ʒ����
	 * \param value �����ʴ��۱�
	 * \param value1 ����������
	 * \param value2 ���ӵ���������
	 */
	void lostAll(NpcLostObject &nlo)
	{
		for(std::vector<CarryObject>::const_iterator it = cov.begin(); it != cov.end(); it++)
		{
			nlo.push_back(*it);
		}
	}

	/**
	 * \brief װ����Ʒȫ�����䴦��(�̹�ר��)
	 * \param nlo npcЯ����Ʒ����
	 * \param value �����ʴ��۱�
	 * \param value1 ����������
	 * \param value2 ���ӵ���������
	 */
	void lostGreen(NpcLostObject &nlo, int value=1, int value1=0, int value2=0, int vcharm = 0, int vlucky = 0);
	private:
		std::vector<CarryObject> cov;
		//zMutex mlock;
};


struct aTypeS{
	aTypeS()
	{
		byValue[0] = 0;
		byValue[1] = 0;
	}
	union {
		struct {
			BYTE byAType;
			BYTE byAction;
		};
		BYTE byValue[2];
	};
};

enum
{
	NPC_TYPE_HUMAN		= 0,	///����
	NPC_TYPE_NORMAL		= 1,	/// ��ͨ����
	NPC_TYPE_BBOSS		= 2,	/// ��Boss����
	NPC_TYPE_LBOSS		= 3,	/// СBoss����
	NPC_TYPE_BACKBONE	= 4,	/// ��Ӣ����
	NPC_TYPE_GOLD		= 5,	/// �ƽ�����
	NPC_TYPE_TRADE		= 6,	/// ��������
	NPC_TYPE_TASK		= 7,	/// ��������
	NPC_TYPE_GUARD		= 8,	/// ʿ������
	NPC_TYPE_PET		= 9,	/// ��������
	NPC_TYPE_BACKBONEBUG= 10,	/// ��������
	NPC_TYPE_SUMMONS	= 11,	/// �ٻ�����
	NPC_TYPE_TOTEM		= 12,	/// ͼ������
	NPC_TYPE_AGGRANDIZEMENT = 13,/// ǿ������
	NPC_TYPE_ABERRANCE	= 14,	/// ��������
	NPC_TYPE_STORAGE	= 15,	/// �ֿ�����
	NPC_TYPE_ROADSIGN	= 16,	/// ·������
	NPC_TYPE_TREASURE	= 17,	/// ��������
	NPC_TYPE_WILDHORSE	= 18,	/// Ұ������
	NPC_TYPE_MOBILETRADE	= 19,	/// ����С��
	NPC_TYPE_LIVENPC	= 20,	/// ����npc����ս��������ʱ��ʧ��
	NPC_TYPE_DUCKHIT	= 21,	/// ���²��ܴ��npc
	NPC_TYPE_BANNER		= 22,	/// ��������
	NPC_TYPE_TRAP		= 23,	/// ��������
	NPC_TYPE_MAILBOX	=24,	///����
	NPC_TYPE_AUCTION	=25,	///��������Ա
	NPC_TYPE_UNIONGUARD	=26,	///�������
	NPC_TYPE_SOLDIER	=27,	///ʿ����ֻ���������
	NPC_TYPE_UNIONATTACKER	=28,	///����ʿ��
	NPC_TYPE_SURFACE = 29,	/// �ر�����
	NPC_TYPE_CARTOONPET = 30,	/// ������
	NPC_TYPE_PBOSS = 31,	/// ��ɫBOSS
	NPC_TYPE_RESOURCE = 32  /// ��Դ��NPC
};

enum
{
	NPC_ATYPE_NEAR		= 1,	/// �����빥��
	NPC_ATYPE_FAR		= 2,	/// Զ���빥��
	NPC_ATYPE_MFAR		= 3,	/// ����Զ�̹���
	NPC_ATYPE_MNEAR		= 4,	/// ����������
	NPC_ATYPE_NOACTION  = 5		/// �޹�������
};

///npcʹ��һ�����ܵ�����
struct npcSkill
{
	DWORD id;///����id
	int needLevel;///����id
	int rate;///ʹ�ü���
	int coefficient;///����ϵ��

	npcSkill():id(0),needLevel(0),rate(0),coefficient(0){}
	npcSkill(const npcSkill &skill)
	{
		id = skill.id;
		needLevel = skill.needLevel;
		rate = skill.rate;
		coefficient = skill.coefficient;
	}
	npcSkill& operator = (const npcSkill &skill)
	{
		id = skill.id;
		needLevel = skill.needLevel;
		rate = skill.rate;
		coefficient = skill.coefficient;
		return *this;
	}
};

struct npcRecover
{
	DWORD start;
	BYTE type;
	DWORD num;

	npcRecover()
	{
		start = 0;
		type = 0;
		num = 0;
	}

	void parse(const char * str)
	{
		if (!str) return;

		std::vector<std::string> vec;

		vec.clear();
		Zebra::stringtok(vec, str, ":");
		if (3==vec.size())
		{
			start = atoi(vec[0].c_str());
			type = atoi(vec[1].c_str());
			num = atoi(vec[2].c_str());
		}
	}
};

/**
 * \brief Npc�����������
 *
 */
struct zNpcB : public zEntry
{
	DWORD	kind;				// ����
	DWORD	level;				// �ȼ�
	DWORD	hp;					// ����ֵ
	DWORD	exp;				// ����ֵ
	DWORD	str;				// ����
	DWORD   inte;				// ����
	DWORD   dex;				// ����
	DWORD   men;				// ����
	DWORD   con;				// ����
	DWORD   cri;				// ����
	DWORD	color;				// ��ɫ
	DWORD	ai;					// ai
	DWORD	distance;			// �ƶ����
	DWORD	adistance;			// �������
	DWORD	pdefence;			// ��С���������
	DWORD	maxpdefence;		// ������������
	DWORD	mdefence;			// ��С����������
	DWORD	maxmdefence;		// �����������
	DWORD	five;				// ��������
	DWORD   fivepoint;			// ���е���
	std::vector<aTypeS> atypelist;	// ��������
	DWORD	mdamage;			// ��С��������
	DWORD	maxmdamage;			// ���������
	DWORD	damage;				// ��С������
	DWORD	maxdamage;			// ��󹥻���
	DWORD	skill;				// ����
	//char	object[1024 + 1];	// Я����Ʒ
	NpcCarryObject nco;
	DWORD	soulrate;			// ����֮ʯ����
	char	skills[1024];		// ʹ�ü���
	char	state[1024];		// ״̬
	DWORD	dodge;				// �����
	DWORD	rating;				// ������
	DWORD	pic;				// ͼƬ
	DWORD	trait;				//Ʒ��
	DWORD	bear_type;			//�������
	DWORD	pet_pic;			//����ͼƬ
	npcRecover recover;
	DWORD	flags;			//�����Ʊ�־��Ŀǰ��һ�����ɲ��ɱ������ɱ
	DWORD	allyVisit;			//�ɱ��˹����ʵĵȼ� 0�����ɷ��� 1��1���ɷ��� 2��2���ɷ���

	std::map<int, std::vector<npcSkill> > skillMap;
	
	bool parseSkills(const char * str)
	{
		skillMap.clear();
		strncpy(skills, str, sizeof(skills));
		
		bool ret = false;
		std::vector<std::string> type_v;
		Zebra::stringtok(type_v, str, ";");
		if (type_v.size()>0)
		{
			std::vector<std::string> type_sub_v, skill_v, prop_v;
			std::vector<std::string>::iterator type_it, skill_it;

			for (type_it=type_v.begin();type_it!=type_v.end();type_it++)
			{
				type_sub_v.clear();
				Zebra::stringtok(type_sub_v, type_it->c_str(), ":");
				if (2==type_sub_v.size())
				{
					int type = atoi(type_sub_v[0].c_str());

					std::vector<npcSkill> oneTypeSkills;
					skill_v.clear();
					Zebra::stringtok(skill_v, type_sub_v[1].c_str(), ",");
					for (skill_it=skill_v.begin();skill_it!=skill_v.end();skill_it++)
					{
						prop_v.clear();
						Zebra::stringtok(prop_v, skill_it->c_str(), "-");
						if (4==prop_v.size())
						{
							npcSkill oneSkill;
							oneSkill.id = atoi(prop_v[0].c_str());
							oneSkill.needLevel = atoi(prop_v[1].c_str());
							oneSkill.rate = atoi(prop_v[2].c_str());
							oneSkill.coefficient = atoi(prop_v[3].c_str());

							oneTypeSkills.push_back(oneSkill);
						}
					}
					if (oneTypeSkills.size()>0)
					{
						skillMap[type] = oneTypeSkills;
						ret = true;
					}
				}
			}
		}
		return ret;
	}

	/**
	 * \brief �����������ȡ��һ��npc���ܵ�����
	 *
	 * \param type ��������
	 * \param skill ����ֵ��ȡ�õļ�������
	 * \return �Ƿ�ȡ�óɹ�
	 */
	bool getRandomSkillByType(int type, npcSkill &skill)
	{
		if (skillMap.find(type)==skillMap.end()) return false;
	
		skill = skillMap[type][zMisc::randBetween(0, skillMap[type].size()-1)];
		return true;
	}

	/**
	 * \brief ȡ�����п��õļ���ID
	 *
	 *
	 * \param list ����ID�б�
	 * \return bool �Ƿ��м���
	 */
	bool getAllSkills(std::vector<DWORD> & list, WORD level)
	{
		std::map<int, std::vector<npcSkill> >::iterator type_it;
		std::vector<npcSkill>::iterator skill_it;
		for (type_it=skillMap.begin();type_it!=skillMap.end();type_it++)
		{
			for (skill_it=type_it->second.begin();skill_it!=type_it->second.end();skill_it++)
				if (level>=skill_it->needLevel)
					list.push_back(skill_it->id);
		}
		return list.size()>0;
	}

	/**
	 * \brief ����һ��npc����
	 * \param type ���ܷ���
	 * \param id Ҫ���ӵļ���id
	 * \param rate ʩ�ż���
	 * \param coefficient ϵ��
	 */
	void addSkill(int type, DWORD id, int needLevel, int rate, int coefficient = 0)
	{
		npcSkill s;
		s.id = id;
		s.needLevel = needLevel;
		s.rate = rate;
		s.coefficient = coefficient;
		skillMap[type].push_back(s);
	}

	/**
	 * \brief ɾ��һ��npc����
	 *
	 *
	 * \param id Ҫɾ���ļ���id
	 * \return npcû�иü����򷵻�false
	 */
	bool delSkill(DWORD id)
	{
		std::map<int, std::vector<npcSkill> >::iterator v_it;
		for (v_it=skillMap.begin();v_it!=skillMap.end();v_it++)
		{
			std::vector<npcSkill> v = v_it->second;
			std::vector<npcSkill>::iterator s_it;
			for (s_it=v.begin();s_it!=v.end();s_it++)
			{
				if (s_it->id==id)
				{
					v.erase(s_it);
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * \brief ����npc�Ĺ�������
	 *
	 *
	 * \param data ������ַ���
	 * \param size �ַ�����С
	 */
	void setAType(const char *data , int size)
	{
		atypelist.clear(); 
		char Buf[1024];
		bzero(Buf , size + 1);
		strncpy(Buf , data , size);
		std::vector<std::string> v_fir;
		Zebra::stringtok(v_fir , Buf , ":");
		for(std::vector<std::string>::iterator iter = v_fir.begin() ; iter != v_fir.end() ; iter++)
		{
			std::vector<std::string> v_sec;
			Zebra::stringtok(v_sec , iter->c_str() , "-");

			if(v_sec.size() != 2)
			{
				return;
			}

			aTypeS aValue;
			std::vector<std::string>::iterator iter_1 = v_sec.begin();

			for(int i=0; i<2; i++)
			{
				aValue.byValue[i] = (BYTE)atoi(iter_1->c_str());
				iter_1 ++;
			}
			atypelist.push_back(aValue);
#ifdef _DEBUGLOG
			Zebra::logger->error("NPC��������: id=%u name=%s atype=%u action=%u ", id, name, aValue.byValue[0], aValue.byValue[1]);
#endif
		}
#ifdef _DEBUGLOG
			Zebra::logger->error("---------------------------------------------");
#endif
		return;
	}

	/**
	 * \brief ȡ��npc�Ĺ������ͺͶ�������
	 *
	 *
	 * \param type ��� ��������
	 * \param action
	 */
	void getATypeAndAction(BYTE &type, BYTE &action)
	{		
		int size = atypelist.size();
		if (size == 0)
		{
			type = NPC_ATYPE_NEAR;
			action = 4 ;//Cmd::AniTypeEnum::Ani_Attack;//Cmd::Ani_Attack
			return;
		}
		int num = zMisc::randBetween(0,size-1);
		type = atypelist[num].byAType;
		action = atypelist[num].byAction;
	}

	/**
	 * \brief ���ݱ���ж������������zNpcB�ṹ
	 *
	 *
	 * \param npc �ӱ��ж���������
	 */
	void fill(const NpcBase &npc)
	{
		id=					npc.dwField0;
		strncpy(name,		npc.strField1,MAX_NAMESIZE);
		kind=				npc.dwField2;
		level=				npc.dwField3;
		hp=					npc.dwField4;
#ifdef _DEBUGLOG
		Zebra::logger->error("npc����name=%s hp=%u", name, hp);
#endif
		exp=				npc.dwField5;
		str=				npc.dwField6;
		inte=				npc.dwField7;
		dex=				npc.dwField8;
		men=				npc.dwField9;
		con=				npc.dwField10;
		cri=				npc.dwField11;
		color=				npc.dwField12;
		ai=					npc.dwField13;
		distance=			(0==npc.dwField14)?640:npc.dwField14;
		adistance= 			(0==npc.dwField15)?1000:npc.dwField15;
		pdefence=			npc.dwField16;
		maxpdefence=		npc.dwField17;
		mdefence=			npc.dwField18;
		maxmdefence=		npc.dwField19;
		five=				npc.dwField20;
		fivepoint=			npc.dwField21;
		setAType(npc.strField22, 1024);
		mdamage=			npc.dwField23;
		maxmdamage=			npc.dwField24;
		damage=				npc.dwField25;
		maxdamage=			npc.dwField26;
		skill=				npc.dwField27;
		if (!nco.set(npc.strField28))
			Zebra::logger->error("Npc���Я����Ʒ��ʽ��������%u, %s, \'%s\'", id, name, npc.strField28);
		soulrate=			npc.dwField29;
#ifdef _XWL_DEBUG
		//parseSkills("1:1-0-50-1,3-0-50-1;2:280-0-30-1;3:282-0-20-1,284-0-100-1;");
		//parseSkills("1:1-12-50-1;");
		parseSkills(npc.strField30);
		//Zebra::logger->error("npcB::fill():����npc���ܳ��� npcid=%u skills=%s", id, npc.strField30);
#else
		parseSkills(npc.strField30);
		//parseSkills("1:141-20-1,281-20-1;2:284-20-1;3:1-20-1,211-20-1;");
#endif
		strncpy(state,		npc.strField31, 1024);
		dodge=				npc.dwField32;
		rating=				npc.dwField33;
		pic=				npc.dwField34;
		trait=				npc.dwField35;
		bear_type=			npc.dwField36;
		pet_pic=			npc.dwField37;
		recover.parse(npc.strField38);
		flags=				npc.dwField39;
		allyVisit=				npc.dwField40;
#ifdef _XWL_DEBUG
		//recover.parse("50:1:5");
#else
#endif
	}

	zNpcB() : zEntry()
	{
		id=					0;
		bzero(name,			sizeof(name));
		kind=				0;
		level=				0;
		hp=				0;
		exp=				0;
		str=				0;
		inte=				0;
		dex=				0;
		men=				0;
		con=				0;
		cri=				0;
		color=				0;
		ai=				0;
		distance=			0;
		adistance= 			0;
		pdefence=			0;
		maxpdefence=		0;
		mdefence=			0;
		maxmdefence=		0;
		five=				0;
		fivepoint=			0;
		atypelist.clear();
		mdamage=			0;
		maxmdamage=			0;
		damage=				0;
		maxdamage=			0;
		skill=				0;
		//bzero(object,		sizeof(object));
		soulrate=			0;
		bzero(skills,		sizeof(skills));
		bzero(state,		sizeof(state));
		dodge=				0;
		rating=				0;
		pic=				0;
		trait=				0;
		bear_type=			0;
		pet_pic=			0;
		flags=				0;
		allyVisit=			0;
	}

};

//------------------------------------
// ���ﾭ��Base
//------------------------------------
struct ExperienceBase
{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// �ȼ�
	DWORD	dwField1;		// ��Ҫ����
};//���� ���ﾭ��Base �ɹ����� 300 ����¼

struct zExperienceB : public zEntry
{
	DWORD	level;				// �ȼ�
	QWORD	nextexp;			//��Ҫ����

	void fill(const ExperienceBase &data)
	{
		id = data.dwField0;
		snprintf(name,MAX_NAMESIZE,"%u",id);
		nextexp = data.dwField1;
	}

	zExperienceB () : zEntry()
	{
		id = 0;
		nextexp = 0;
	}
};
//------------------------------------
// �������ӱ�
//------------------------------------
struct HonorBase
{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// �ȼ�
	DWORD	dwField1;		// ��Ҫ����
};//���� ���ﾭ��Base �ɹ����� 300 ����¼

struct zHonorB : public zEntry
{
	DWORD	level;				// �ȼ�
	QWORD	value;			//��Ҫ����

	void fill(const HonorBase &data)
	{
		id = data.dwField0;
		snprintf(name,MAX_NAMESIZE,"%u",id);
		value = data.dwField1;
	}

	zHonorB () : zEntry()
	{
		id = 0;
		value = 0;
	}
};

//------------------------------------
// SkillBase
//------------------------------------
/**
 * \brief ���ݼ������ͺ͵ȼ�����һ����ʱΨһ���
 *
 */
#define skill_hash(type, level) ((type - 1) * 10 + level)

struct SkillBase
{
	const DWORD getUniqueID() const
	{
		return skill_hash(dwField0 , dwField2);
	}

	DWORD	dwField0;			// ����ID
	char	strField1[32];		// ��������
	DWORD	dwField2;			// ���ܵȼ�
	DWORD	dwField3;			// ����ϵ��
	DWORD	dwField4;			// ��������
	DWORD	dwField5;			// ��Ҫ���߼��ܵ���
	DWORD	dwField6;			// ǰ�Ἴ��һ
	DWORD	dwField7;			// ǰ�Ἴ��һ�ȼ�
	DWORD	dwField8;			// ǰ�Ἴ�ܶ�
	DWORD	dwField9;			// ǰ�Ἴ�ܶ��ȼ�
	DWORD	dwField10;			// ǰ�Ἴ����
	DWORD	dwField11;			// ǰ�Ἴ�����ȼ�
	DWORD	dwField12;			// ���ʱ��
	DWORD	dwField13;			// ������ʽ
	DWORD	dwField14;			// �ܷ�����ʹ��
	DWORD	dwField15;			// ��Ҫ��Ʒ
	char	strField16[128];	// ��Ҫ����
	DWORD	dwField17;			// ��������ֵ
	DWORD	dwField18;			// ���ķ���ֵ
	DWORD	dwField19;			// ��������ֵ
	DWORD	dwField20;			// �˺��ӳ�
	char	strField21[1024];	// Ч��
	DWORD	dwField22;			// ������Ʒ����
	DWORD	dwField23;			// ��Ʒ��������
};//���� SkillBase �ɹ����� 1 ����¼

#define BENIGNED_SKILL_STATE 2
#define BAD_SKILL_STATE 4
#define NONE_SKILL_STATE 1 

struct SkillElement
{
	SkillElement()
	{
		id = 0;
		value = 0;
		percent = 0;
		time = 0;
		state = 0;
	}
	union {
		struct {
			WORD id;
			WORD percent;
			WORD value;
			WORD time;
			WORD state;
		};
		WORD element[5];
	};
	static SkillElement *create(SkillElement elem);
};
struct SkillStatus
{
	SkillStatus()
	{
		for(int i = 0 ; i < (int)(sizeof(status) / sizeof(WORD)) ; i ++)
		{
			status[i] = 0;
		}
	}
	union {
		struct {
			WORD id;//����id
			WORD target;//Ŀ��
			WORD center;//���ĵ�
			WORD range;//��Χ
			WORD mode;//����ģʽ
			WORD clear;//�ܷ����
			WORD isInjure;//�Ƿ���Ҫ�˺�����
		};
		WORD status[7];
	};
	std::vector<SkillElement> _StatusElementList;
};
struct zSkillB : public zEntry
{
	bool has_needweapon(const WORD weapontype) const
	{
		std::vector<WORD>::const_iterator iter;
		if (weaponlist.empty()) return true;
		for(iter = weaponlist.begin(); iter != weaponlist.end(); iter++)
		{
			if (*iter == weapontype) return true;
		}
		return false;
	}

	bool set_weaponlist(const char *data)
	{
		weaponlist.clear(); 
		std::vector<std::string> v_fir;
		Zebra::stringtok(v_fir , data , ":");
#ifdef _DEBUGLOG
		Zebra::logger->debug("��Ҫ����[%s]size=[%d]" , data, v_fir.size());
#endif 
		for(std::vector<std::string>::iterator iter = v_fir.begin() ; iter != v_fir.end() ; iter++)
		{
			WORD weaponkind = (WORD)atoi(iter->c_str());
			weaponlist.push_back(weaponkind);
		}
		return true;
	}

	bool set_skillState(const char *data)
	{
		skillStatus.clear(); 
		std::vector<std::string> v_fir;
		Zebra::stringtok(v_fir , data , ".");
		for(std::vector<std::string>::iterator iter = v_fir.begin() ; iter != v_fir.end() ; iter++)
		{
				//Zebra::logger->debug("%s" , iter->c_str());
			std::vector<std::string> v_sec;
			Zebra::stringtok(v_sec , iter->c_str() , ":");
			/*
			if(v_sec.size() != 2)
			{
				return false;
			}
			// */
			SkillStatus status;
			std::vector<std::string>::iterator iter_1 = v_sec.begin() ;
			std::vector<std::string> v_thi;
			Zebra::stringtok(v_thi , iter_1->c_str() , "-");
			if(v_thi.size() != 7)
			{
				//Zebra::logger->debug("����!=7");
				continue;
				//return false;
			}
			std::vector<std::string>::iterator iter_2 = v_thi.begin() ;
			for(int i = 0 ; i < 7 ; i ++)
			{
				status.status[i] = (WORD)atoi(iter_2->c_str());
				//Zebra::logger->debug("status.status[%ld]=%ld" , i , status.status[i]);
				iter_2 ++;
			}
			iter_1 ++;
			if(iter_1 == v_sec.end())
			{
				//Zebra::logger->debug("�ղ���");
				skillStatus.push_back(status);
				continue;
			}
			std::vector<std::string> v_fou;
			Zebra::stringtok(v_fou , iter_1->c_str() , ";");
			std::vector<std::string>::iterator iter_3 = v_fou.begin() ;
			for( ; iter_3 != v_fou.end() ; iter_3 ++)
			{
				std::vector<std::string> v_fiv;
				Zebra::stringtok(v_fiv , iter_3->c_str() , "-");
				if(v_fiv.size() != 5)
				{
					//Zebra::logger->debug("Ԫ�ظ�������");
					continue;
					//return false;
				}
				std::vector<std::string>::iterator iter_4 = v_fiv.begin() ;
				SkillElement element;
				for(int i = 0 ; i < 5 ; i ++)
				{
					element.element[i] = (WORD)atoi(iter_4->c_str());
					//Zebra::logger->debug("element.element[%u]=%u" , i , element.element[i]);
					iter_4 ++;
				}
				status._StatusElementList.push_back(element);
			}
			skillStatus.push_back(status);
		}
		return true;
	}
	DWORD	skillid;						//����ID
	DWORD	level;							//���ܵȼ�
	DWORD	kind;							//����ϵ��
	DWORD	subkind;						//��������
	DWORD	needpoint;						//��Ҫ���߼��ܵ���
	DWORD	preskill1;						//ǰ�Ἴ��1
	DWORD	preskilllevel1;					//ǰ�Ἴ�ܼ���1
	DWORD	preskill2;						//ǰ�Ἴ��2
	DWORD	preskilllevel2;					//ǰ�Ἴ�ܼ���2
	DWORD	preskill3;						//ǰ�Ἴ��3
	DWORD	preskilllevel3;					//ǰ�Ἴ�ܼ���3
	DWORD	dtime;							//���ʱ��
	DWORD	usetype;						//������ʽ
	DWORD	ride;							//�ɷ�����ʹ��
	DWORD	useBook;						//��Ҫ��Ʒ
	DWORD	spcost;							//��������ֵ
	DWORD	mpcost;							//���ķ���ֵ
	DWORD	hpcost;							//��������ֵ
	DWORD	damnum;							//�˺��ӳ�
	DWORD	objcost;						//������Ʒ����
	DWORD	objnum;							//������Ʒ����
	std::vector<SkillStatus> skillStatus;	//Ч��
	std::vector<WORD> weaponlist;			//�����б�
	


	void fill(const SkillBase &data)
	{
		id=skill_hash(data.dwField0 , data.dwField2);
		skillid=data.dwField0;								//����ID
		strncpy(name , data.strField1 , MAX_NAMESIZE);
		level			= data.dwField2;					//���ܵȼ�
		kind			= data.dwField3;					//����ϵ��
		subkind			= data.dwField4;					//��������
		needpoint		= data.dwField5;					//��Ҫ���߼��ܵ���
		preskill1		= data.dwField6;					//ǰ�Ἴ��1
		preskilllevel1	= data.dwField7;;					//ǰ�Ἴ�ܼ���1
		preskill2		= data.dwField8;					//ǰ�Ἴ��2
		preskilllevel2	= data.dwField9;					//ǰ�Ἴ�ܼ���2
		preskill3		= data.dwField10;					//ǰ�Ἴ��3
		preskilllevel3	= data.dwField11;					//ǰ�Ἴ�ܼ���3
		dtime			= data.dwField12;					//���ʱ��
		usetype			= data.dwField13;					//������ʽ
		ride			= data.dwField14;					//�ɷ�����ʹ��
		useBook			= data.dwField15;					//ѧϰ��Ҫ��Ʒ
		set_weaponlist(data.strField16);					//��Ҫ����
		spcost			= data.dwField17;					//��������ֵ
		mpcost			= data.dwField18;					//���ķ���ֵ
		hpcost			= data.dwField19;					//��������ֵ
		damnum			= data.dwField20;					//�˺��ӳ�
		set_skillState(data.strField21);
		objcost			= data.dwField22;					//������Ʒ����
		objnum			= data.dwField23;					//������Ʒ����

#ifdef _DEBUGLOG
		Zebra::logger->debug("skillid=%ld:", skillid);
		for(std::vector<SkillStatus>::iterator iter = skillStatus.begin() ; iter != skillStatus.end() ; iter ++)
		{
			Zebra::logger->debug("id=%u , target=%u , center=%u , range=%u , mode=%u , clear=%u:",iter->id , iter->target , iter->center , iter->range , iter->mode , iter->clear); 
			for(std::vector<SkillElement>::iterator iter_1 = iter->_StatusElementList.begin() ; iter_1 != iter->_StatusElementList.end() ; iter_1++)
			{
				Zebra::logger->debug("id=%u , percent=%u , value=%u , time=%u , state=%u",iter_1->id , iter_1->percent , iter_1->value , iter_1->time , iter_1->state);
				
			}
		}
#endif
	}


	zSkillB() : zEntry()
	{
		id = 0;
		skillid = 0;
		bzero(name , sizeof(name));				//˵��
		level			= 0;					//���ܵȼ�
		kind			= 0;					//����ϵ��
		subkind			= 0;					//��������
		needpoint		= 0;					//��Ҫ���߼��ܵ���
		preskill1		= 0;					//ǰ�Ἴ��1
		preskilllevel1	= 0;					//ǰ�Ἴ�ܼ���1
		preskill2		= 0;					//ǰ�Ἴ��2
		preskilllevel2	= 0;					//ǰ�Ἴ�ܼ���2
		preskill3		= 0;					//ǰ�Ἴ��3
		preskilllevel3	= 0;					//ǰ�Ἴ�ܼ���3
		dtime			= 0;					//���ʱ��
		usetype			= 0;					//������ʽ
		ride			= 0;					//�ɷ�����ʹ��
		useBook			= 0;					//��Ҫ��Ʒ
		spcost			= 0;					//��������ֵ
		mpcost			= 0;					//���ķ���ֵ
		hpcost			= 0;					//��������ֵ
		damnum			= 0;					//�˺��ӳ�
		objcost			= 0;					//������Ʒ����
		objnum			= 0;					//������Ʒ����
	}

};

struct LiveSkillBase{

	const DWORD getUniqueID() const
	{
		return ((0xffff & dwField11) << 16) | (0xffff & dwField0);
	}

	DWORD	dwField0;		// ����ID
	char	strField1[64];		// ��������
	DWORD	dwField2;		// ��Ҫ����
	DWORD	dwField3;		// ��ʼ����
	DWORD	dwField4;		// ��Ӧͼ��
	DWORD	dwField5;		// ���
	DWORD	dwField6;		// ������������
	DWORD	dwField7;		// �ɷ�����
	DWORD	dwField8;		// ���׼���
	DWORD	dwField9;		// ǰ�Ἴ��ID
	DWORD	dwField10;		// ����ǰ�Ἴ�ܵȼ�
	DWORD	dwField11;		// ���ܵȼ�
	char	strField12[32];		// ���ܳƺ�
	char	strField13[256];		// �����Ʒ
};

struct zLiveSkillB : public zEntry
{
	enum {
		MAX_EXP_BONUS = 30,
		MIN_POINT_BONUS = 1,
		MAX_POINT_BONUS = 3,
		WORKING_TIME = 6,
		MAX_LEVEL = 30,
	};
			
	//DWORD skill_id; //���ܱ�ʶ
	//DWORD level; //���ܵȼ�
	//WORD should be enough
	WORD skill_id; //���ܱ�ʶ
	WORD level; //���ܵȼ�
	DWORD point; //�������輼�ܵ�
	DWORD weapon_kind; //��������
	//std::string name; //��������
	std::string title; //�ƺ�
	bool orig; //��ʼ����
	bool upgrade; //�ܷ�����
	DWORD kind; //�������
	DWORD basic_skill_id; //ǰ�Ἴ��id
	DWORD basic_skill_level; //ǰ�Ἴ�ܵȼ�]
	DWORD up_skill_id; //���׼���id
	DWORD map_kind;
	
	class ITEM 
	{
	public:
		DWORD item; //�����Ʒ
		DWORD odds;	//����
		DWORD min_number; //��С����
		DWORD max_number; //�������

		ITEM( const std::string& odds_, const std::string& item_, const std::string& number_) : item(atoi(item_.c_str())), odds(atoi(odds_.c_str())), min_number(0), max_number(0)
		{
			std::string::size_type pos = 0;
			if  ( (pos = number_.find("-")) != std::string::npos ) {
				
				min_number = atoi(number_.substr(0, pos).c_str());
				max_number = atoi(number_.substr(pos+strlen("-")).c_str());
			}
			//if (item) Zebra::logger->debug("�Ͷ������Ʒ����:ID(%d), ����(%d), ����(%d-%d)", item, odds, min_number, max_number);
		}
	}; 
		
	typedef std::vector<ITEM> ITEMS;
	ITEMS items;
	
	BYTE min_point_bonus; //��С���Ӽ��ܵ�
	BYTE max_point_bonus; //������Ӽ��ܵ�
	BYTE exp_bonus; //��������
	BYTE max_level; //���ȼ�

	zLiveSkillB() : zEntry(), skill_id(0), level(0), point(0), weapon_kind(0), /*name("δ֪"),*/ title(""), orig(false), upgrade(false),
					kind(1), basic_skill_id(0), basic_skill_level(0), up_skill_id(0), map_kind(0), 
					min_point_bonus(MIN_POINT_BONUS), max_point_bonus(MAX_POINT_BONUS), exp_bonus(MAX_EXP_BONUS), 
					max_level(MAX_LEVEL)
	{

	}
		
	void fill(const LiveSkillBase& base)
	{
		skill_id = 0xffff & base.dwField0;
		//name = base.strField1;
		weapon_kind = base.dwField2;
		orig = (base.dwField3==1)?true:false;
		map_kind = base.dwField4;
		kind = base.dwField5;
		point = base.dwField6;
		upgrade = (base.dwField7==1)?true:false;
		up_skill_id = base.dwField8;
		basic_skill_id = base.dwField9;
		basic_skill_level = base.dwField10;
		level = 0xffff & base.dwField11;
		strncpy(name, base.strField1, MAX_NAMESIZE);
		title = base.strField12;
		init_items(base.strField13);
		
		id = (level << 16) | skill_id;
	}
	
	void init_items(const std::string& item_list)
	{
		items.clear();
		Misc::Split<Misc::Parse3> p;
		p(item_list, items, ";", ":");

	}

};

//------------------------------------
// SoulStoneBase
//------------------------------------
struct SoulStoneBase{
	const DWORD getUniqueID() const
	{
		return dwField2;
	}
	
	DWORD	dwField0;		// ���
	char	strField1[32];		// ����
	DWORD	dwField2;		// Ʒ��
	char	strField3[16];		// x%��������ֵy
	char	strField4[16];		// x%���շ���ֵy
	char	strField5[16];		// ת��x%����ֵΪ����ֵ����
	char	strField6[16];		// �������ӵ���x%
	char	strField7[16];		// x%˫������
	char	strField8[16];		// ���ӵ�����x%
	char	strField9[16];		// ��������
	char	strField10[16];		// ���������
	char	strField11[16];		// ��ѣ������
	char	strField12[16];		// ����ħ����
	char	strField13[16];		// ����������
	char	strField14[16];		// ����������
	char	strField15[16];		// ����������
	char	strField16[16];		// ��ʯ������
	char	strField17[16];		// ��ʧ������
	char	strField18[16];		// ����������
	char	strField19[16];		// ����������
	char	strField20[16];		// ���ջ�����
	char	strField21[16];		// �ж�����
	char	strField22[16];		// �������
	char	strField23[16];		// ѣ������
	char	strField24[16];		// ��ħ����
	char	strField25[16];		// ��������
	char	strField26[16];		// ��������
	char	strField27[16];		// ��������
	char	strField28[16];		// ʯ������
	char	strField29[16];		// ʧ������
	char	strField30[16];		// ��������
	char	strField31[16];		// ��������
	char	strField32[16];		// �ջ�����
	DWORD	dwField33;		// ����ȼ�
	char	strField34[16];		// ����
	char	strField35[16];		// ����
	char	strField36[16];		// ����
	char	strField37[16];		// ����
	char	strField38[16];		// ���� 	
};//���� SoulStoneBase �ɹ����� 40 ����¼



struct zSoulStoneB : public zEntry
{
	//DWORD id;
	//std::string name;
	
	struct Value
	{ 
		rangeValue odds; 
		rangeValue effect; 
	} hpleech, mpleech; ////x%��������ֵy, x%���շ���ֵy
	
	rangeValue hptomp; //ת������ֵΪ����ֵx��

	rangeValue incgold; //�������ӵ���x%
	rangeValue doublexp; //x%˫������		
	rangeValue mf; //���ӵ�����x%
	
	rangeValue poisondef; //��������
	rangeValue lulldef; //���������
	rangeValue reeldef; //��ѣ������
	rangeValue evildef; //����ħ����
	rangeValue bitedef; //����������
	rangeValue chaosdef; //����������
	rangeValue colddef; //����������
	rangeValue petrifydef; //��ʯ������
	rangeValue blinddef; //��ʧ������
	rangeValue stabledef; //����������
	rangeValue slowdef; //����������
	rangeValue luredef; //���ջ�����

	rangeValue poison; //�ж�����
	rangeValue lull; //�������
	rangeValue reel; //ѣ������
	rangeValue evil; //��ħ����
	rangeValue bite; //��������
	rangeValue chaos; //��������
	rangeValue cold; //��������
	rangeValue petrify; //ʯ������
	rangeValue blind; //ʧ������
	rangeValue stable; //��������
	rangeValue slow; //��������
	rangeValue lure; //�ջ�����
	
	WORD level; 	

	rangeValue str;			// ����
	rangeValue inte;		// ����
	rangeValue dex;			// ����
	rangeValue spi;			// ����
	rangeValue con;			// ����
	
	zSoulStoneB() : zEntry()
	{
		
	}
	
	void fill(const SoulStoneBase& base)
	{
		id = base.dwField2;
		strncpy(name, base.strField1, MAX_NAMESIZE);
		
		init_value(base.strField3, hpleech);
		init_value(base.strField4, mpleech);
		
		fillRangeValue(base.strField5, hptomp);
		fillRangeValue(base.strField6, incgold);
		fillRangeValue(base.strField7, doublexp);
		fillRangeValue(base.strField8, mf);

		fillRangeValue(base.strField9, poisondef);
		fillRangeValue(base.strField10, lulldef);
		fillRangeValue(base.strField11, reeldef);	
		fillRangeValue(base.strField12, evildef);
		fillRangeValue(base.strField13, bitedef);
		fillRangeValue(base.strField14, chaosdef);
		fillRangeValue(base.strField15, colddef);
		fillRangeValue(base.strField16, petrifydef);
		fillRangeValue(base.strField17, blinddef);
		fillRangeValue(base.strField18, stabledef);
		fillRangeValue(base.strField19, slowdef);
		fillRangeValue(base.strField20, luredef);
				
		fillRangeValue(base.strField21, poison);
		fillRangeValue(base.strField22, lull);
		fillRangeValue(base.strField23, reel);	
		fillRangeValue(base.strField24, evil);
		fillRangeValue(base.strField25, bite);
		fillRangeValue(base.strField26, chaos);
		fillRangeValue(base.strField27, cold);
		fillRangeValue(base.strField28, petrify);
		fillRangeValue(base.strField29, blind);
		fillRangeValue(base.strField30, stable);
		fillRangeValue(base.strField31, slow);
		fillRangeValue(base.strField32, lure);

		level = base.dwField33;

		fillRangeValue(base.strField34, str);
		fillRangeValue(base.strField35, inte);
		fillRangeValue(base.strField36, dex);
		fillRangeValue(base.strField37, spi);
		fillRangeValue(base.strField38, con);		
	}
	
	void init_value(const std::string& src, Value& value)
	{
		std::string::size_type pos = 0;
		if  ( (pos = src.find(';')) != std::string::npos ) {
			fillRangeValue(src.substr(0, pos).c_str(), value.odds);
			fillRangeValue(src.substr(pos+1).c_str(), value.effect);
		}
	}
	
};



//------------------------------------
// HairStyle
//------------------------------------
struct HairStyle{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// ���
	char	strField1[32];		// ����
	DWORD	dwField2;		// �Ա�
	DWORD	dwField3;		// ��������ͼƬ
	DWORD	dwField4;		// ֽ���޷���ͼƬ
	DWORD	dwField5;		// ����
};//���� HairStyle �ɹ����� 10 ����¼
struct zHairStyleB : public zEntry
{
	DWORD cost;
	zHairStyleB():zEntry()
	{
		cost=0;
	}
	void fill(const HairStyle& base)
	{
		id = base.dwField0;
		strncpy(name, base.strField1, MAX_NAMESIZE);
		cost=base.dwField5;
	}
};

//------------------------------------
// HairColour
//------------------------------------
struct HairColour{
	const DWORD getUniqueID() const
	{
		return dwField2 & 0x00FFFFFF;//��ɫ��
	}
	DWORD	dwField0;		// ���
	char	strField1[32];		// ����
	DWORD	dwField2;		// ��ɫ
	DWORD	dwField3;		// ����
};//���� HairColour �ɹ����� 4 ����¼
struct zHairColourB : public zEntry
{
	DWORD color;
	DWORD cost;
	zHairColourB() : zEntry()
	{
		color=0;
		cost=0;
	}
	void fill(const HairColour& base)
	{
		id = base.dwField2 & 0x00FFFFFF;//��ɫ��
		strncpy(name, base.strField1, MAX_NAMESIZE);
		color=base.dwField2;
		cost=base.dwField3;
	}
};
//------------------------------------
// HeadList
//------------------------------------
struct HeadList{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// ���
	char	strField1[16];		// ͷ����
	DWORD	dwField2;		// �Ա�
	DWORD	dwField3;		// ͷ����
	DWORD	dwField4;		// ����
};//���� HeadList �ɹ����� 10 ����¼
struct zHeadListB : public zEntry
{
	DWORD sex;
	DWORD icon;
	DWORD cost;
	zHeadListB() : zEntry()
	{
		sex=0;
		icon=0;
		cost=0;
	}
	void fill(const HeadList& base)
	{
		id = base.dwField0;
		strncpy(name, base.strField1, MAX_NAMESIZE);
		sex=base.dwField2;
		icon=base.dwField3;
		cost=base.dwField4;
	}
};


//------------------------------------
//// PetBase
////------------------------------------
struct PetBase{
	const DWORD getUniqueID() const
	{
		return dwField0;
	}
	DWORD	dwField0;		// id
	DWORD	dwField1;		// �ȼ�
	DWORD	dwField2;		// ����
	DWORD	dwField3;		// ����ֵ
	DWORD	dwField4;		// ����ֵ
	DWORD	dwField5;		// �﹥����
	DWORD	dwField6;		// �﹥����
	DWORD	dwField7;		// ħ������
	DWORD	dwField8;		// ħ������
	DWORD	dwField9;		// ���
	DWORD	dwField10;		// ħ��
	DWORD	dwField11;		// �ػ�
	DWORD	dwField12;		// ����
	DWORD	dwField13;		// ����
	DWORD	dwField14;		// ����
	DWORD	dwField15;		// ����
	DWORD	dwField16;		// ����		
};

struct zPetB : public zEntry
{
	DWORD base_id;		// id
	DWORD lv;       	// �ȼ�
	DWORD type;     	// ����
	DWORD exp;      	// ����ֵ
	DWORD hp;       	// ����ֵ
	DWORD atk;      	// �﹥����
	DWORD maxatk;   	// �﹥����
	DWORD matk;     	// ħ������
	DWORD maxmatk;  	// ħ������
	DWORD def;      	// ���
	DWORD mdef;     	// ħ��
	DWORD cri;      	// �ػ�
	DWORD str;      	// ����
	DWORD intel;    	// ����
	DWORD agi;      	// ����
	DWORD men;      	// ����
	DWORD vit;		// ����		

	zPetB() : zEntry()
	{
		base_id	= 0;	
		lv	= 0;       
		type	= 0;     
		exp	= 0;      
		hp	= 0;       
		atk	= 0;      
		maxatk	= 0;   
		matk	= 0;     
		maxmatk	= 0;  
		def	= 0;      
		mdef	= 0;     
		cri	= 0;      
		str	= 0;      
		intel	= 0;    
		agi	= 0;      
		men	= 0;      
		vit	= 0;	
	}
	void fill(PetBase &base)
	{
		base_id	= base.dwField0;	
		id	= base.dwField0;	
		char buf[32];
		sprintf(buf, "%d", base.dwField0);
		strncpy(name, buf , MAX_NAMESIZE);
		lv	= base.dwField1;	
		type	= base.dwField2;	
		exp	= base.dwField3;	
		hp	= base.dwField4;	
		atk	= base.dwField5;	
		maxatk	= base.dwField6;		
		matk	= base.dwField7;	
		maxmatk	= base.dwField8;		
		def	= base.dwField9;	
		mdef	= base.dwField10;	
		cri	= base.dwField11;	
		str	= base.dwField12;	
		intel	= base.dwField13;	
		agi	= base.dwField14;	
		men	= base.dwField15;	
		vit	= base.dwField16;			
	}
};
//------------------------------------
// CountryMaterial
//------------------------------------
struct CountryMaterial{
	const DWORD getUniqueID() const
	{
		return dwField1+dwField3;
	}
	
	DWORD	dwField0;		// ���
	DWORD	dwField1;		// ��ƷID
	DWORD	dwField2;		// ��������
	DWORD	dwField3;		// ��Ʒ���
};
struct zCountryMaterialB : public zEntry
{
	DWORD dwObjectID;
	DWORD dwMaterialKind;
	DWORD dwKind;
	
	zCountryMaterialB() : zEntry()
	{
		dwObjectID = 0;
		dwMaterialKind = 0;
		dwKind = 0;
	}
	void fill(const CountryMaterial& base)
	{
		id = base.dwField1+base.dwField3;
		
		dwObjectID = base.dwField1;
		dwMaterialKind = base.dwField2;
		dwKind = base.dwField3;
	}
};

#pragma pack()

#endif
