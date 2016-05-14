/**
 * \file
 * \version  $Id: SceneDefine.h  $
 * \author  
 * \date
 * \brief ����һЩ���ݽṹ
 *
 * 
 */

#ifndef _SceneDefine_h_
#define _SceneDefine_h_

#include <vector>
#include <list>
#include <map>

#include "zSceneEntry.h"
#include "Zebra.h"

typedef std::vector<zPos> zPosIndex;

/**
 * \brief ����Ķ���
 * ������ͷ�����ġ������������������ڷ��赲������
 *
 */
struct zRegion
{
	zPos s;				/// ���ϵ�
	zPos c;				/// ���ĵ�
	zPos e;				/// ���µ�
	zPosIndex index;	/// ���赲������
	/**
	 * \brief ���캯��
	 *
	 */
	zRegion()
	{
	}
	/**
	 * \brief �������캯��
	 *
	 */
	zRegion(const zRegion &reg)
	{
		s = reg.s;
		c = reg.c;
		e = reg.e;
		index = reg.index;
	}
	/**
	 * \brief ��ֵ��������
	 *
	 */
	zRegion & operator= (const zRegion &reg)
	{
		s = reg.s;
		c = reg.c;
		e = reg.e;
		index = reg.index;
		return *this;
	}

	/**
	 * \brief �ж������Ƿ�������Χ֮��
	 *
	 */
	const bool isIn(const zPos &pos) const
	{
		return pos >= s && pos <= e;
	}

};

//�����ļ��й����������Ͷ���
/**
 * \brief �������͵Ķ���
 * �������͡���С��
 *
 */
struct ZoneTypeDef
{
	enum
	{
		ZONE_NONE				=	0,	//һ����
		ZONE_PK_SAFE			=	1,		// pk��ȫ��
		ZONE_ABSOLUTE_SAFE		=	2,		// ���԰�ȫ��
		ZONE_RELIVE				=	4,		// ����������
		ZONE_NEWBIE				=	8,		// ���ֳ�����
		ZONE_SPORTS				=	16,		// ������
		ZONE_FOREIGN_RELIVE		=	32,		// ��������������
		ZONE_PRIVATE_STORE 		= 	128,       // ��̯��
		ZONE_PRIVATE_DARE 		= 	256,       // ��ս��ת��
		ZONE_PRIVATE_RELIVE		=	512,	   // ��ս������
		ZONE_PRIVATE_UNION		= 	1024,      // �����������ת��
		ZONE_COUNTRY_WAR		= 	2048,	   // �߾��Ĺ�ս��ת��
		ZONE_PRIVATE_DARE_UNION		=	4096,	   // ���ս��ս����ת��
		ZONE_DARE_SAFE			= 	8192,	   // ��սʱΪ���԰�ȫ����ƽʱΪ�ǰ�ȫ��
		ZONE_PRIVATE_THIRD_UNION        =       16384,     // ��������ص�������ת��
		ZONE_EMPEROR_DEF_RELIVE		= 	32768,	   // �ʳ�ս�ط�������
		ZONE_EMPEROR_ATT_RELIVE		=	65536,	   // �ʳ�ս����������	
	};

	zPos  pos;								//����
	WORD  width;							//���ο�
	WORD  height;							//���γ�
	DWORD type;								//�������������
	zSceneEntry::SceneEntryState initstate;	//��ʼ״̬,���ػ��߷�����
	zSceneEntry::SceneEntryState state;		//��ǰ״̬,���ػ��߷�����
	zRegion region;							//��Χ���ݣ�������Χ�ڵĿ����ߵ���������

	/**
	 * \brief ���캯��
	 *
	 */
	ZoneTypeDef() : pos(), region()
	{
		width = 0;
		height = 0;
		type = 0;
		initstate = zSceneEntry::SceneEntry_Normal;
		state = zSceneEntry::SceneEntry_Normal;
	}
	/**
	 * \brief �������캯��
	 *
	 */
	ZoneTypeDef(const ZoneTypeDef &zone)
	{
		pos = zone.pos;
		width = zone.width;
		height = zone.height;
		type = zone.type;
		initstate = zone.initstate;
		state = zone.state;
		region = zone.region;
	}
	/**
	 * \brief ��ֵ��������
	 *
	 */
	ZoneTypeDef & operator= (const ZoneTypeDef &zone)
	{
		pos = zone.pos;
		width = zone.width;
		height = zone.height;
		type = zone.type;
		initstate = zone.initstate;
		state = zone.state;
		region = zone.region;
		return *this;
	}
};

typedef std::vector<ZoneTypeDef> ZoneTypeDefVector;

//�����ļ��й���NPC�Ķ���
/**
 * \brief npc����ṹ
 * ������ţ�����λ�á���Χ���������Ϣ 
 *
 */
struct t_NpcDefine
{
	DWORD id;								//���
	char  name[MAX_NAMESIZE+1];				//����
	zPos  pos;								//����
	WORD  width;							//���ο�
	WORD  height;							//���γ�
	DWORD num;								//����
	DWORD interval;							//ˢ��ʱ��
	zSceneEntry::SceneEntryState initstate;	//��ʼ״̬,���ػ��߷�����
	zRegion region;							//��Χ���ݣ�������Χ�ڵĿ����ߵ���������
	//std::vector<zPos> path;//�̶��ƶ���·��
	
	DWORD rushID;//��������ID
	BYTE rushRate;//�������ǵļ���
	DWORD rushDelay;//�������ǵ���ʱ

	int scriptID;//���ܽű���id
	/*
	DWORD summonID;///�ٻ���npc��ID
	DWORD summonNum;///�ٻ�npc������
	DWORD petList[10];
	DWORD deathSummonList[10][2];
	*/
	std::map<DWORD, std::pair<DWORD, DWORD> > petList;
	std::map<DWORD, std::pair<DWORD, DWORD> > summonList;
	std::map<DWORD, std::pair<DWORD, DWORD> > deathSummonList;
	std::list< std::pair<DWORD,zPos> > dieList;
	/**
	 * \brief ���캯��
	 *
	 */
	t_NpcDefine() : pos(), region()
	{
		id = 0;
		bzero(name, sizeof(name));
		width = 0;
		height = 0;
		num = 0;
		interval = 0;
		rushID = 0;
		rushRate = 0;
		rushDelay = 0;
		initstate = zSceneEntry::SceneEntry_Normal;
		scriptID = 0;
		//summonID = 0;
		//summonNum = 0;
		//bzero(petList, sizeof(petList));
		//bzero(deathSummonList, sizeof(deathSummonList));
	}
	/**
	 * \brief �������캯��
	 *
	 */
	t_NpcDefine(const t_NpcDefine &reg)
	{
		id = reg.id;
		bcopy(reg.name, name, sizeof(name));
		pos = reg.pos;
		width = reg.width;
		height = reg.height;
		num = reg.num;
		interval = reg.interval;
		initstate = reg.initstate;
		region = reg.region;
		//path = reg.path;
		rushID = reg.rushID;
		rushRate = reg.rushRate;
		rushDelay = reg.rushDelay;
		scriptID = reg.scriptID;
		petList = reg.petList;
		summonList = reg.summonList;
		deathSummonList = reg.deathSummonList;
		dieList = reg.dieList;
		//summonID = reg.summonID;
		//summonNum = reg.summonNum;

		//bcopy(reg.petList, petList, sizeof(petList));
		//bcopy(reg.deathSummonList, deathSummonList, sizeof(deathSummonList));
	}
	/**
	 * \brief ��ֵ��������
	 *
	 */
	t_NpcDefine & operator= (const t_NpcDefine &reg)
	{
		id = reg.id;
		bcopy(reg.name, name, sizeof(name));
		pos = reg.pos;
		width = reg.width;
		height = reg.height;
		num = reg.num;
		interval = reg.interval;
		initstate = reg.initstate;
		region = reg.region;
		//path = reg.path;
		rushID = reg.rushID;
		rushRate = reg.rushRate;
		rushDelay = reg.rushDelay;
		scriptID = reg.scriptID;
		petList = reg.petList;
		summonList = reg.summonList;
		deathSummonList = reg.deathSummonList;
		dieList = reg.dieList;
		//summonID = reg.summonID;
		//summonNum = reg.summonNum;
		//bcopy(reg.petList, petList, sizeof(petList));
		//bcopy(reg.deathSummonList, deathSummonList, sizeof(deathSummonList));
		return *this;
	}

	void fillNpcMap(const char * tempChar, std::map<DWORD, std::pair<DWORD,DWORD> > &map)
	{
		std::vector<std::string> vs;
		std::vector<std::string> sub_vs;
		vs.clear();
		Zebra::stringtok(vs, tempChar, ";");
		for (DWORD i=0; i<vs.size(); i++)
		{
			sub_vs.clear();
			Zebra::stringtok(sub_vs, vs[i].c_str(), "-");
			if (sub_vs.size()==2)
				map[atoi(sub_vs[0].c_str())] = std::make_pair(atoi(sub_vs[1].c_str()), 100);
			else if (sub_vs.size()==3) //npcID,num,rate
				map[atoi(sub_vs[0].c_str())] = std::make_pair(atoi(sub_vs[1].c_str()), atoi(sub_vs[2].c_str()));
		}
	}
	/*
	void setPath(const char * str)
	{
		path.clear();
		std::vector<std::string> vs,vc;
		std::vector<std::string>::iterator it;
		Zebra::stringtok(vs, str, ";");
		if (!vs.empty())
		{
			for (it=vs.begin(); it!=vs.end(); it++)
			{
				zPos pos;
				vc.clear();
				Zebra::stringtok(vc, (*it).c_str(), ",");
				pos.x = atoi(vc.begin()->c_str());
				pos.y = atoi(vc.rbegin()->c_str());

				path.push_back(pos);
			}
		}
	}
	*/
};

typedef std::vector<t_NpcDefine> NpcDefineVector;

#endif

