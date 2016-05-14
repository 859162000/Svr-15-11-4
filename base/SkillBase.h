#ifndef _SKILLBASE_H_
#define _SKILLBASE_H_

#include "zType.h"
#include "zDatabase.h"
#pragma pack(1)

typedef struct _Skill
{
	DWORD	skillid;					//����ID
	DWORD	level;						//���ܵȼ�
//	DWORD	maxlevel;					//���ܵȼ�
//	DWORD	nextlevel;					//��Ҫ��ɫ�ȼ�
//	DWORD	needtype;					// ��Ҫְҵ
//	DWORD	five;						// ��������
//	DWORD	firstfive;					// ������е���
	//DWORD	nextfive;					// ���е���
}t_Skill;


#pragma pack()
namespace SkillDef
{
	enum ATTACK_TYPE
	{
		AttackNear = 0,
		AttackFly = 1,
		AttackDirect = 2
	};

	enum
	{
		AttackNine = 1,
		AttackEight = 2,
		AttackThree = 3,
		AttackFive_1 = 4,
		AttackFive_2 = 5,
		AttackFive_3 = 6,
		AttackCross = 7

	};
	enum 	SKILL_STATUS
	{
		SKILL_default, //0Ĭ��Ϊ��������
		SKILL_dvalue, //�˺�ֵ
		SKILL_reflect, //����
		SKILL_sdam, //���ܹ�����
		SKILL_mdam,//����������
		SKILL_maxmp,//����ֵ����
		SKILL_mpspeed,//����ֵ�ظ��ٶ�
		SKILL_mppersist,//����ֵ����
		SKILL_mp,				//����ֵ
		SKILL_mpdef,		//��������
		SKILL_sp,				//����ֵ
		SKILL_maxsp,//����ֵ����
		SKILL_sppersist,//����ֵ����
		SKILL_spspeed,	//����ֵ�ظ��ٶ�
		SKILL_hp,				//����ֵ
		SKILL_maxhp,		//����ֵ����
		SKILL_hppersist,//����ֵ����
		SKILL_hpspeed,	//����ֵ�ظ��ٶ�
		SKILL_mvspeed,	//�ƶ��ٶ�
		SKILL_pdam,			//��������
		SKILL_atrating,	//������
		SKILL_skillrating,//����������
		SKILL_akdodge, //������
		SKILL_cancel, //״̬����
		SKILL_aspeed,	//�����ٶ�
		SKILL_relive,//����
		SKILL_hitback,//����
		SKILL_change,//����Ŀ���Ϊʩ����
		SKILL_topet,//��Ϊ���С����
		SKILL_dam2ddef,//����ת��
		SKILL_poison, //�ж�״̬
		SKILL_petrify, //ʯ��״̬
		SKILL_blind, //ʧ��״̬
		SKILL_chaos, //����״̬
		SKILL_cold, //����״̬
		SKILL_palsy,//���״̬
		SKILL_bleeding,//��Ѫ״̬
		SKILL_sevenp,//���������ߴ�״̬����
		SKILL_coldp, //�����������״̬����
		SKILL_poisonp, //���������ж�״̬����
		SKILL_petrifyp, //��������ʯ��״̬����
		SKILL_blindp, //��������ʧ��״̬����
		SKILL_chaosp, //�����������״̬����
		SKILL_mgspeed, //���ټ���ʩ�ż��
		SKILL_cancelatt //���ܹ���ʩ����
	};
#define TARGET_SELF 1
#define TARGET_FRIEND 2
#define TARGET_ENEMY 4
#define TARGET_NPC 8
#define TARGET_TERRASURFACE 16
#define TARGET_PET 32
#define TARGET_SUMMON 64

#define SKILL_CENTER_TYPE_SELF  2
#define SKILL_CENTER_TYPE_MOUSE 1

#define SKILLINVALID 0 
#define SKILLNORMAL 377
#define NORMALBOW 378
#define MAX_SKILLLEVEL 7

};

#endif
