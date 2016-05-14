/**
 * \file
 * \version  $Id: CharBase.h  $
 * \author  
 * \date 
 * \brief ���������ɫ�����Ϣ
 *
 * 
 */


#ifndef _CharBase_h_
#define _CharBase_h_

#include "zType.h"
#include "Object.h"
#pragma pack(1)

#define HAIRTYPE_MASK	0xff000000		/// ͷ������
#define HAIRRGB_MASK	0x00ffffff		/// ͷ����ɫ
const int exploit_arg = 100;			// ��ѫֵͬ�ȷŴ�ϵ��

//---------------------------------
#define LEVELUP_HP_N			15
#define LEVELUP_MP_N			1
#define LEVELUP_SP_N			5

#define LEVELUP_RESUMEHP_N		0
#define	LEVELUP_RESUMEMP_N		0
#define LEVELUP_RESUMESP_N		0

#define LEVELUP_ATTACKRATING_N	1
#define LEVELUP_ATTACKDODGE_N	1

#define LEVELUP_MOVESPEED_N		0
#define LEVELUP_ATTACKSPEED_N	0

#define LEVELUP_PDAMAGE_N		0
#define LEVELUP_MDAMAGE_N		0
#define LEVELUP_PDEFENCE_N		0
#define LEVELUP_MDEFENCE_N		0

#define LEVELUP_BANG_N		0
//----------------------------------
#define BASEDATA_M_HP			500
#define BASEDATA_M_MP			60
#define BASEDATA_M_SP			100

#define BASEDATA_M_RESUMEHP		1
#define	BASEDATA_M_RESUMEMP		1
#define BASEDATA_M_RESUMESP		1

#define BASEDATA_M_ATTACKRATING	1
#define BASEDATA_M_ATTACKDODGE	1

#define BASEDATA_M_MOVESPEED	0
#define BASEDATA_M_ATTACKSPEED	0

#define BASEDATA_M_PDAMAGE		11
#define BASEDATA_M_MDAMAGE		11
#define BASEDATA_M_PDEFENCE		1
#define BASEDATA_M_MDEFENCE		1

#define BASEDATA_M_BANG		1
//----------------------------------
#define BASEDATA_F_HP			500
#define BASEDATA_F_MP			60
#define BASEDATA_F_SP			90

#define BASEDATA_F_RESUMEHP		1
#define	BASEDATA_F_RESUMEMP		1
#define BASEDATA_F_RESUMESP		1

#define BASEDATA_F_ATTACKRATING	1
#define BASEDATA_F_ATTACKDODGE	1

#define BASEDATA_F_MOVESPEED	0
#define BASEDATA_F_ATTACKSPEED	0

#define BASEDATA_F_PDAMAGE		11
#define BASEDATA_F_MDAMAGE		11
#define BASEDATA_F_PDEFENCE		1
#define BASEDATA_F_MDEFENCE		1

#define BASEDATA_F_BANG		1
//----------------------------------

struct CharBase
{
	DWORD accid;						/// �˺�
	DWORD id;							/// ��ɫ���
	char  name[MAX_NAMESIZE + 1];		/// ��ɫ����
	WORD  type;							/// ��ɫ����
	WORD  level;						/// ��ɫ�ȼ�
	WORD  face;							/// �Ա�
	DWORD hair;							/// ͷ�������ͺ���ɫ
	DWORD bodyColor;							/// ������ɫ
	DWORD goodness;						/// �ƶ��
	DWORD mapid;						/// ��ɫ���ڵ�ͼ���
	char  mapName[MAX_NAMESIZE + 1];	/// ��ɫ���ڵ�ͼ����
	DWORD x;							/// ��ɫ��������x
	DWORD y;							/// ��ɫ��������y
	DWORD unionid;						/// ���ID
	DWORD schoolid;						/// ����ID
	DWORD septid;						/// ����ID
	DWORD hp;							/// ��ǰ����ֵ
//	DWORD maxhp;						/// �������ֵ *
//	DWORD resumehp;						/// ����ֵ�ָ� *
	DWORD mp;							/// ��ǰ����ֵ
//	DWORD maxmp;						/// �����ֵ *
//	DWORD resumemp;						/// ����ֵ�ָ� *
	DWORD sp;							/// ��ǰ����ֵ
//	DWORD maxsp;						/// �������ֵ *
//	DWORD resumesp;						/// ����ֵ�ָ� *
//	DWORD pdamage;						/// ��С�������� *
//	DWORD mdamage;						/// ��С���������� *
//	DWORD pdefence;						/// ��С��������� *
//	DWORD mdefence;						/// ��С���������� *
	QWORD exp;							/// ��ǰ����ֵ
//	WORD  attackspeed;					/// �����ٶ� *
//	WORD  movespeed;					/// �ƶ��ٶ� *
//	WORD  attackrating;					/// �������� *
//	WORD  attackdodge;					/// ������� *
//	WORD  bang;							/// �ػ� *
	WORD  lucky;						/// ����ֵ *
	WORD  skillpoint;					/// ���ܵ���
	WORD  points;						/// δ�����������Ե���
	DWORD	country;						/// ����
	DWORD	consort;							/// ��ż
	/*
	//ϵͳ����
	union{
		struct{
			union{
				struct{
					BYTE pkMode;
					BYTE setting[19];
				};
				BYTE sysSetting[20];
			};
		};
		DWORD dwSeting;
	};
	*/
	QWORD forbidtalk;					///����
	DWORD bitmask;						/// ��־����
	DWORD onlinetime;					/// ����ʱ��ͳ��
	union {
		struct {
			WORD wdCon;	//����
			WORD wdStr;	//����
			WORD wdDex;	//����
			WORD wdInt;	//����
			WORD wdMen;	//����
		};
		WORD wdProperty[5];
	};
	WORD reliveWeakTime;			/// ��������ʣ��ʱ��
	DWORD grace;				// �Ĳ�ֵ
	DWORD exploit;				/// ��ѫֵ
	char tiretime[36+1];			/// ƣ��ʱ����
	DWORD offlinetime;				/// �ϴ�����ʱ��
	DWORD fivetype;					/// ��������
	DWORD fivelevel;				/// ���е���
	DWORD pkaddition;				/// �ƶ�׷��ֵ
	DWORD money;				/// ��ǰ����,ֻ�洢ά��,����ʹ��
	DWORD answerCount;			/// ����������
	DWORD honor;				//����ֵ
	DWORD maxhonor;				//�������ֵ
	DWORD gomaptype;			//����ͼ����
	DWORD msgTime;				//�´η���GM���Ե�ʱ��
	DWORD accPriv;			//�ʺ�Ȩ��
	DWORD gold;					/// ���
	DWORD ticket; 				//��ȯ��
	DWORD createtime;			/// ��ɫ����ʱ��
	DWORD goldgive;				/// ��ҳ�ֵ��Ʒ����
	BYTE petPack;				/// ���������С
	DWORD petPoint;				/// �����ĳ�������ʱ��
	DWORD levelsept;			/// �뿪�����ʱ��
	DWORD punishTime;			/// ɱ�˱�ץ��ʱ�䣬����Ϊ��λ
	DWORD trainTime;			/// ��������ͼ������ʱ��
//	char pass[8]; 					//�ֿ�����
};

struct CharState
{
	DWORD maxhp;						/// �������ֵ *
	DWORD resumehp;						/// ����ֵ�ָ� *
	DWORD maxmp;						/// �����ֵ *
	DWORD resumemp;						/// ����ֵ�ָ� *
	DWORD maxsp;                     /// �������ֵ *
	DWORD resumesp;                  /// �����ָ�ֵ *

	DWORD pdamage;                   /// ��С�������� *
	DWORD maxpdamage;					/// ����������� *
	DWORD mdamage;						/// ��С���������� *
	DWORD maxmdamage;					/// ����������� *
	DWORD pdefence;						/// ��������� *
	DWORD mdefence;						/// ���������� *
	QWORD nextexp;						/// ��������ֵ *
	WORD  attackspeed;					/// �����ٶ� *
	WORD  movespeed;					/// �ƶ��ٶ� *
	SWORD  attackrating;				/// �������� *
	SWORD  attackdodge;					/// ������� *
	WORD  bang;							/// �ػ� *
	WORD  lucky;						/// ����ֵ *
	WORD  charm;                     /// ����ֵ *
	BYTE  attackfive;					/// �������� *
	BYTE  defencefive;					/// �������� *
	union {
		struct {
			WORD wdCon;	//����
			WORD wdStr;	//����
			WORD wdDex;	//����
			WORD wdInt;	//����
			WORD wdMen;	//����
		};
		WORD wdProperty[5];
	};

	DWORD stdpdamage;					/// ��׼��������
	DWORD stdmdamage;					/// ��׼����������
	DWORD stdpdefence;					/// ��׼���������
	DWORD stdmdefence;					/// ��׼����������
	WORD  stdbang;						/// ��׼�ػ���
};
#ifdef _TEST_DATA_LOG
struct CharTest
{
	DWORD upgrade_time;//����ʱ��
	DWORD upgrade_usetime;//����ʹ��ʱ��
	DWORD death_times;//��������
	DWORD hp_leechdom;//ʹ������ֵҩƷ����
	DWORD mp_leechdom;//ʹ�÷���ֵҩƷ����
	DWORD sp_leechdom;//ʹ������ֵҩƷ����
	DWORD get_money;//�õ�������
	DWORD get_heigh;//�õ��߼�װ������
	DWORD get_socket;//�õ�����װ������
	DWORD get_material;//�õ�ԭ�ϸ���
	DWORD get_stone;//�õ���ʯ����
	DWORD get_scroll;//�õ��������
	DWORD money;//��ǰ������
};
#endif


struct CharSave
{
	CharBase 	charbase;
	DWORD		dataSize;
	char	 	data[0];
};

#define MAX_TEMPARCHIVE_SIZE 0x10000
struct TempArchiveMember
{
	DWORD type;
	DWORD size;
	char data[0];
};
enum TempArchiveType
{
	TEAM,	//����
	ENTRY_STATE,	//����
	PET,	//����
	SAFETY_STATE,	// ��ʱ�ر����뱣��
};
struct BinaryArchiveMember
{
	DWORD type;
	DWORD size;
	char data[0];
};
enum BinaryArchiveType
{
	BINARY_DOUBLE_EXP_OBJ,	///˫���������
	BINARY_TONG_OBJ,		///������������
	BINARY_KING_OBJ,		///�������������
	BINARY_FAMILY_OBJ,		///�������������
	BINARY_CHANGE_COUNTRY_TIME, /// ���һ���ѹ���ʱ��
	BINARY_SAFETY,			/// �Ƿ���вƲ�����
	BINARY_GIVE_MATARIAL_NUM,   //���߿��һ����ϵ�����
	BINARY_CARD_NUM,            //���߿�����
	BINARY_SAFETY_SETUP,	/// ��������
	BINARY_MAX,				///�����ֵ(ռλ��)
};

#ifndef _SEX_
#define _SEX_
enum
{
	MALE	= 0,
	FEMALE	= 1
};
#endif

#ifndef _PROFESSION_
#define _PROFESSION_
enum
{
	PROFESSION_NONE	= 0,	//��ҵ
	PROFESSION_1	= 1,	//����
	PROFESSION_2	= 2,	//��Ů
	PROFESSION_3	= 4,	//����
	PROFESSION_4	= 8,	//����
	PROFESSION_5	= 16,	//��ʦ
	PROFESSION_6	= 32,	//��Ů
	PROFESSION_7	= 64,	//��ʦ
	PROFESSION_8	= 128	//��Ů
};
#endif


const WORD  MAX_LEVEL			= 150;					/// ����ɫ�ȼ�
const DWORD MAX_GOODNESS		= 9999;					/// ����ƶ��
const DWORD MAX_HP			= 999999;				/// �������ֵ
const DWORD MAX_RESUMEHP		= 1000;					/// ����ֵ�ָ�
const DWORD MAX_MP			= 999999;				/// �����ֵ
const DWORD MAX_RESUMEMP		= 1000;					/// ����ֵ�ָ�
const DWORD MAX_SP			= 999999;				/// �������ֵ
const DWORD MAX_RESUMESP		= 1000;					/// ����ֵ�ָ�
const DWORD MAX_PDAMAGE			= 999999;				/// �����������
const DWORD MAX_RDAMAGE			= 999999;				/// ���Զ�̹�����
const DWORD MAX_TDAMAGE 		= 999999;				/// �������������
const DWORD MAX_MDAMAGE			= 999999;				/// �����������
const DWORD MAX_PDEFENCE		= 999999;				/// ������������
const DWORD MAX_MDEFENCE		= 999999;				/// �����������
const QWORD MAX_EXP			= 9999999999LL;			/// �����ֵ
const WORD  MAX_ATTACKSPEED		= 100;					/// �����ٶ�
const WORD  MAX_MAGICSPEED		= 100;					/// ʩ���ٶ�
const WORD  MAX_MOVESPEED		= 100;					/// �ƶ��ٶ�
const WORD  MAX_ATTACKRATING		= 999;					/// ��������
const WORD  MAX_ATTACKDODGE		= 999;					/// �������
const WORD  MAX_MAGICRATING		= 999;					/// ��������
const WORD  MAX_MAGICDODGE		= 999;					/// �������
const WORD  MAX_LUCKY			= 999;					/// ����ֵ
const WORD  MAX_METAL			= 999;					/// ��
const WORD  MAX_WOOD			= 999;					/// ľ
const WORD  MAX_WATER			= 999;					/// ˮ
const WORD  MAX_FIRE			= 999;					/// ��
const WORD  MAX_SOIL			= 999;					/// ��
const WORD  MAX_METALRESISTANCE		= 10000;				/// ��ϵ�ֿ�
const WORD  MAX_WOODRESISTANCE		= 10000;				/// ľϵ�ֿ�
const WORD  MAX_WATERRESISTANCE		= 10000;				/// ˮϵ�ֿ�
const WORD  MAX_FIRERESISTANCE		= 10000;				/// ��ϵ�ֿ�
const WORD  MAX_SOILRESISTANCE		= 10000;				/// ��ϵ�ֿ�
const WORD  MAX_POINTS			= 1500;					/// ����

#pragma pack()

#endif

