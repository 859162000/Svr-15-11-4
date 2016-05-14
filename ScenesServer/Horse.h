/**
 * \file
 * \version  $Id: Horse.h $
 * \author  
 * \date 	
 * \brief 	��ƥ���
 *
 * 
 */
#ifndef __HORSE_H__
#define __HORSE_H__

class SceneUser;

#include "zType.h"
#include "zTime.h"
#include "Command.h"
#include "zObject.h"

//�������
const DWORD HORSE_TYPE_NOTHORSE = 0;//û��
const DWORD HORSE_TYPE_NORMAL = 1;//��ͨ
const DWORD HORSE_TYPE_BATTLE = 2;//ս��
const DWORD HORSE_TYPE_SUPER = 3;//�߼�ս��

//���PK����.���ʱ�ӵ��������ϵ�
struct t_HorsePkData
{
		DWORD id;       //npcid
		char name[MAX_NAMESIZE];//����
		DWORD lv;       //�ȼ�
		WORD str;       //����
		WORD intel;     //����
		WORD dex;       //����
		WORD men;       //����
		WORD con;       //����
		WORD speed;     //�ٶ�
		BYTE pdam;      //������������%
		BYTE pdef;      //�������������%
		BYTE mdam;      //����ħ��������%
		BYTE mdef;      //����ħ��������%
		WORD maxhp;     //���HP
		WORD maxmp;     //���MP
		DWORD callTime;     //�ٻ����
		Cmd::horseState state;   //���״̬
		WORD poisonRes;     //����
		WORD lullRes;       //�����
		WORD faintRes;      //����ѣ
		WORD chaosRes;      //������
		WORD freezeRes;     //������
		WORD petrifyRes;    //��ʯ��
		WORD blindRes;      //��ʧ��
		WORD slowRes;       //������
		DWORD horseid;      //���id������id��
};

/**
 * \brief ��ƥ��
 *
 * ��װ���й���ƥ�Ĵ�����
 */
class Horse
{
public:
	Horse(SceneUser& user);	
	
	~Horse();

	void horse(DWORD horse_id);
	bool horse(t_Object &);
	
	DWORD horse() const;

	void mount(bool flag , bool send=true);
	
	bool mount() const;
	
	int load(unsigned char* );
	
	int save(unsigned char *);

	void setSummonTime(int delay);
	bool checkSummonTime();
	bool isFollowing();
	bool comeOut();
	bool putAway();

	bool full_HorseDataStruct(Cmd::t_HorseData *);

	void sendData();

	bool canFight();

	Cmd::t_HorseData data;
	t_HorsePkData pkData;
	void getPkData();

	DWORD horseType();
	DWORD horseType(DWORD);
private:
	DWORD interval;
	zRTime summonTime;
	SceneUser& _owner;
	//Cmd::horseState _state;
	DWORD _horse;	
	bool _mount;
	//bool _follow;
};

#endif
