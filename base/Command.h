#ifndef _COMMAND_H
#define _COMMAND_H


#ifdef _MSC_VER
#pragma warning( push )
#pragma warning(disable : 4200)
#endif

#ifndef MAX_PASSWORD
#define MAX_PASSWORD	16
#endif

#ifndef MAX_NAMESIZE
#define MAX_NAMESIZE	32
#endif

#ifndef MAX_ACCNAMESIZE
#define MAX_ACCNAMESIZE	48
#endif

#ifndef MAX_IP_LENGTH
#define MAX_IP_LENGTH	16
#endif

#ifndef MAX_CHATINFO
#define MAX_CHATINFO	256
#endif

#ifndef MAX_NUMPASSWORD
#define MAX_NUMPASSWORD	32
#endif

#ifndef _SEX_
#define _SEX_
enum
{
	MALE	= 0,
	FEMALE	= 1
};
#endif

#include "Object.h"

#ifndef _PROFESSION_
#define _PROFESSION_
enum
{
	PROFESSION_NONE	= 0,	//��ҵ
	PROFESSION_1	= 1,	//����
	PROFESSION_2	= 2,	//��Ů
	PROFESSION_3	= 3,	//����
	PROFESSION_4	= 4,	//����
	PROFESSION_5	= 5,	//��ʦ
	PROFESSION_6	= 6,	//��Ů
	PROFESSION_7	= 7,	//��ʦ
	PROFESSION_8	= 8		//��Ů
};
#endif
#ifndef _FIVETYPE_
#define _FIVETYPE_
enum
{
	FIVE_METAL = 0,			/// ��
	FIVE_WOOD = 1,			/// ľ
	FIVE_SOIL = 2, 			/// ��
	FIVE_WATER = 3,			/// ˮ
	FIVE_FIRE = 4,			/// ��
	FIVE_NONE = 5			/// ��
};
#endif

const DWORD PUBLIC_COUNTRY = 6;
#pragma pack(1)

///�õ���һֻ��
#define otherHand(hand)	(hand)^3
///�õ���һֻ����
#define otherBangle(bangle) (bangle)^1


#ifdef _USE_CMD_NAMESPACE
#define _CMD_NAMESPACE_BEGIN namespace Cmd {
#define _CMD_NAMESPACE_END	 };
#else
#define _CMD_NAMESPACE_BEGIN 
#define _CMD_NAMESPACE_END	
#endif

_CMD_NAMESPACE_BEGIN

//BEGIN_ONE_CMD

/// ��ָ��
const BYTE NULL_USERCMD			= 0;
/// ��½ָ��
const BYTE LOGON_USERCMD		= 1;
/// ʱ��ָ��
const BYTE TIME_USERCMD			= 2;
/// ����ָ��
const BYTE DATA_USERCMD			= 3;
/// ����ָ��
const BYTE PROPERTY_USERCMD		= 4;
/// ��ͼָ��
const BYTE MAPSCREEN_USERCMD		= 5;
/// �ƶ�ָ��
const BYTE MOVE_USERCMD			= 6;
/// ����ָ��
const BYTE BUILD_USERCMD		= 8;
/// ����ָ��
const BYTE MAKEOBJECT_USERCMD		= 10;
/// ����ָ��
const BYTE RELIVE_USERCMD		= 12;
/// ����ָ��
const BYTE CHAT_USERCMD			= 14;
/// �뿪ָ��
const BYTE LEAVEONLINE_USERCMD		= 15;
/// ����ָ��
const BYTE TRADE_USERCMD		= 17;
/// ħ��ָ��
const BYTE MAGIC_USERCMD		= 18;
/// ���ָ��
const BYTE UNION_USERCMD		= 21;
/// ����ָ��
const BYTE COUNTRY_USERCMD		= 22;
/// ����ָ��
const BYTE TASK_USERCMD			= 23;
/// ѡ��ָ��
const BYTE SELECT_USERCMD		= 24;
//  ����ϵָ��
const BYTE RELATION_USERCMD    		= 25;
//  ���ɹ�ϵָ��
const BYTE SCHOOL_USERCMD		= 26;
//  �����ϵָ��
const BYTE SEPT_USERCMD			= 27;
// ս��ָ��
const BYTE DARE_USERCMD                 = 28;
// ����ָ��
const BYTE PET_USERCMD                  = 29;
// ��ȡ�������б�
const BYTE PING_USERCMD			= 30;
// ���ָ��
const BYTE GOLD_USERCMD			= 31;
// ����ָ��
const BYTE QUIZ_USERCMD			= 32;
// NPC����սָ��
const BYTE NPCDARE_USERCMD		= 33;
// ��GM���߽�����ָ��
const BYTE GMTOOL_USERCMD		= 34;
// �ʼ�ָ��
const BYTE MAIL_USERCMD			= 35;
// ����ָ��
const BYTE AUCTION_USERCMD		= 36;
// ��ͨ����ָ��
const BYTE CARTOON_USERCMD		= 37;
// ��Ʊָ��
const BYTE STOCK_SCENE_USERCMD		= 38;
const BYTE STOCK_BILL_USERCMD		= 39;
// ͶƱָ��
const BYTE VOTE_USERCMD			= 40;
// ����ָ��
const BYTE ARMY_USERCMD			= 41;
// ��������ָ��
const BYTE GEM_USERCMD			= 42;
// ����ϵͳָ��
const BYTE PRISON_USERCMD		= 43;
// ���ָ��
const BYTE GIFT_USERCMD			= 44;
// ����ͬ��ָ��
const BYTE ALLY_USERCMD			= 45;
// С��Ϸָ��
const BYTE MINIGAME_USERCMD		= 46;
// �Ƽ���ϵͳָ��
const BYTE RECOMMEND_USERCMD		= 47;
// �Ʋ�����ϵͳָ��
const BYTE SAFETY_USERCMD		= 48;



//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ��ָ��忪ʼ
//////////////////////////////////////////////////////////////
const BYTE NULL_USERCMD_PARA = 0;
struct stNullUserCmd{
	stNullUserCmd()
	{
		dwTimestamp=0;
	}
	union{
		struct {
			BYTE	byCmd;
			BYTE	byParam;
		};
		struct {
			BYTE	byCmdType;
			BYTE	byParameterType;
		};
	};
	//BYTE	byCmdSequence;
	DWORD	dwTimestamp;
};
//////////////////////////////////////////////////////////////
// ��ָ������
//////////////////////////////////////////////////////////////

enum enumMapDataType{
	MAPDATATYPE_NPC,
	MAPDATATYPE_USER,
	MAPDATATYPE_BUILDING,
	MAPDATATYPE_ITEM,
	MAPDATATYPE_PET
};

struct MapData_ItemHeader {
	WORD size;// ����
	BYTE type;// ����  enumMapDataType
};

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ��½ָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stLogonUserCmd : public stNullUserCmd
{
	stLogonUserCmd()
	{
		byCmd = LOGON_USERCMD;
	}
};

/// �ͻ�����֤�汾
const BYTE USER_VERIFY_VER_PARA = 1;
const DWORD GAME_VERSION = 1999;
struct stUserVerifyVerCmd  : public stLogonUserCmd
{
	stUserVerifyVerCmd()
	{
		byParam = USER_VERIFY_VER_PARA;
		version = GAME_VERSION;
	}

	DWORD version;
};

/// �ͻ��˵�½��½������
const BYTE USER_REQUEST_LOGIN_PARA = 2;
struct stUserRequestLoginCmd : public stLogonUserCmd
{
	stUserRequestLoginCmd()
	{
		byParam = USER_REQUEST_LOGIN_PARA;
	}
	char pstrName[MAX_ACCNAMESIZE];		/**< �ʺ� */
	char pstrPassword[MAX_PASSWORD];	/**< �û����� */
	WORD game;							/**< ��Ϸ���ͱ�ţ�Ŀǰһ����0 */
	WORD zone;							/**< ��Ϸ����� */
	char jpegPassport[7];				/**< ͼ����֤�� */
};

enum{
	LOGIN_RETURN_UNKNOWN,					/// δ֪����
	LOGIN_RETURN_VERSIONERROR,				/// �汾����
	LOGIN_RETURN_UUID,						/// UUID��½��ʽû��ʵ��
	LOGIN_RETURN_DB,						/// ���ݿ����
	LOGIN_RETURN_PASSWORDERROR,				/// �ʺ��������
	LOGIN_RETURN_CHANGEPASSWORD,			/// �޸�����ɹ�
	LOGIN_RETURN_IDINUSE,					/// ID���ڱ�ʹ����
	LOGIN_RETURN_IDINCLOSE,					/// ID����
	LOGIN_RETURN_GATEWAYNOTAVAILABLE,		/// ���ط�����δ��
	LOGIN_RETURN_USERMAX,					/// �û���
	LOGIN_RETURN_ACCOUNTEXIST,				/// �˺��Ѿ�����
	LOGON_RETURN_ACCOUNTSUCCESS,			/// ע���˺ųɹ�

	LOGIN_RETURN_CHARNAMEREPEAT,			/// ��ɫ�����ظ�
	LOGIN_RETURN_USERDATANOEXIST,			/// �û�����������
	LOGIN_RETURN_USERNAMEREPEAT,			/// �û����ظ�
	LOGIN_RETURN_TIMEOUT,					/// ���ӳ�ʱ
	LOGIN_RETURN_PAYFAILED,					/// �Ʒ�ʧ��
	LOGIN_RETURN_JPEG_PASSPORT,				/// ͼ����֤���������
	LOGIN_RETURN_LOCK,				/// �ʺű�����
	LOGIN_RETURN_WAITACTIVE,				/// �ʺŴ�����
	LOGIN_RETURN_NEWUSER_OLDZONE			///���˺Ų��������ɵ���Ϸ�� 
};
/// ��½ʧ�ܺ󷵻ص���Ϣ
const BYTE SERVER_RETURN_LOGIN_FAILED = 3;
struct stServerReturnLoginFailedCmd : public stLogonUserCmd
{
	stServerReturnLoginFailedCmd()
	{
		byParam = SERVER_RETURN_LOGIN_FAILED;
	}
	BYTE byReturnCode;			/**< ���ص��Ӳ��� */
} ;

/// ��½�ɹ����������ط�������ַ�˿��Լ���Կ����Ϣ
const BYTE SERVER_RETURN_LOGIN_OK = 4;
struct stServerReturnLoginSuccessCmd : public stLogonUserCmd 
{
	stServerReturnLoginSuccessCmd()
	{
		byParam = SERVER_RETURN_LOGIN_OK;
	}

	DWORD dwUserID;
	DWORD loginTempID;
	char pstrIP[MAX_IP_LENGTH];
	WORD wdPort;

	union{
		struct{
			BYTE randnum[58];
			BYTE keyOffset;	// �ܳ��� key �е�ƫ��
		};
		BYTE key[256];	// �����ܳף�������������������
	};

};


/// �ͻ���½���ط����������˺ź�����
const BYTE PASSWD_LOGON_USERCMD_PARA = 5;
struct stPasswdLogonUserCmd : public stLogonUserCmd
{
	stPasswdLogonUserCmd()
	{
		byParam = PASSWD_LOGON_USERCMD_PARA;
	}

	DWORD loginTempID;
	DWORD dwUserID;
	char pstrName[MAX_ACCNAMESIZE];		/**< �ʺ� */
	char pstrPassword[MAX_PASSWORD];
};

/// ���󴴽��˺�
const BYTE ACCOUNT_LOGON_USERCMD_PARA = 7;
struct stAccountLogonUserCmd : public stLogonUserCmd 
{
	stAccountLogonUserCmd()
	{
		byParam = ACCOUNT_LOGON_USERCMD_PARA;
	}

	char strName[MAX_ACCNAMESIZE];
	char strPassword[MAX_PASSWORD];
};

/// �����������
const BYTE PASSWORD_LOGON_USERCMD_PARA = 9;
	struct stPasswordLogonUserCmd : public stLogonUserCmd {
		stPasswordLogonUserCmd()
		{
			byParam = PASSWORD_LOGON_USERCMD_PARA;
		}

		char strName[MAX_ACCNAMESIZE];
		char strPassword[MAX_PASSWORD];
		char strNewPassword[MAX_PASSWORD];
	};

/// ���󷵻�ѡ���������
const BYTE BACKSELECT_USERCMD_PARA = 10;
struct stBackSelectUserCmd : public stLogonUserCmd
{
	stBackSelectUserCmd()
	{
		byParam = BACKSELECT_USERCMD_PARA;
	}
};

/// ����ͼ����֤�뵽�ͻ���
const BYTE JPEG_PASSPORT_USERCMD_PARA = 11;
struct stJpegPassportUserCmd : public stLogonUserCmd
{
	stJpegPassportUserCmd()
	{
		byParam = JPEG_PASSPORT_USERCMD_PARA;
		size = 0;
	}
	WORD size;
	BYTE data[0];
};

//���͹�����Ϣ
struct  Country_Info
{
	DWORD id;//����id
	BYTE  enableRegister; //����ע��Ϊ1 ������Ϊ0
	BYTE  enableLogin;		//�����½Ϊ1 ������Ϊ0
	char pstrName[MAX_NAMESIZE];//��������
	Country_Info()
	{
		enableRegister = 0;
		enableLogin = 0;
	}
};
const BYTE SERVER_RETURN_COUNTRY_INFO = 12;
struct stCountryInfoUserCmd : public stLogonUserCmd
{
	stCountryInfoUserCmd()
	{
		byParam = SERVER_RETURN_COUNTRY_INFO;
		size = 0;
	}
	WORD size;
	Country_Info countryinfo[0];
};

//////////////////////////////////////////////////////////////
// ��½ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ʱ��ָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stTimerUserCmd : public stNullUserCmd
{
	stTimerUserCmd()
	{
		byCmd = TIME_USERCMD;
	}
};

/// �������û�������Ϸʱ��
const BYTE GAMETIME_TIMER_USERCMD_PARA = 1;
struct stGameTimeTimerUserCmd : public stTimerUserCmd 
{
	stGameTimeTimerUserCmd()
	{
		byParam = GAMETIME_TIMER_USERCMD_PARA;
	}

	QWORD qwGameTime;			/**< ��Ϸʱ�� */
};

/// �������û�����ʱ��
const BYTE REQUESTUSERGAMETIME_TIMER_USERCMD_PARA = 2;
struct stRequestUserGameTimeTimerUserCmd : public stTimerUserCmd
{
	stRequestUserGameTimeTimerUserCmd()
	{
		byParam = REQUESTUSERGAMETIME_TIMER_USERCMD_PARA;
	}

};

/// �û������ط��͵�ǰ��Ϸʱ��
const BYTE USERGAMETIME_TIMER_USERCMD_PARA  = 3;
struct stUserGameTimeTimerUserCmd : public stTimerUserCmd
{
	stUserGameTimeTimerUserCmd()
	{
		byParam = USERGAMETIME_TIMER_USERCMD_PARA;
	}

	DWORD dwUserTempID;			/**< �û���ʱID */
	QWORD qwGameTime;			/**< �û���Ϸʱ�� */
};

/// �û�ping����(������ԭ������)
const BYTE PING_TIMER_USERCMD_PARA = 4;
struct stPingTimeTimerUserCmd : public stTimerUserCmd
{
	stPingTimeTimerUserCmd()
	{
		byParam = PING_TIMER_USERCMD_PARA;
	}

};
//////////////////////////////////////////////////////////////
/// ʱ��ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ����ָ��忪ʼ
//////////////////////////////////////////////////////////////
/// ��������״̬
enum {
	USTATE_DEATH  	= 0,//	����
	USTATE_POISON  	= 1,//	�ж�
	USTATE_FREEZE  	= 2,//	����
	USTATE_LANDIFICATION = 3,//	����
	USTATE_HIDE  	= 4,//	����
	USTATE_TRADE  	= 5,//	����
	USTATE_WAR		= 6,//	��ս״̬
	USTATE_LUCK  	= 7,//	����
	USTATE_DECLINE  = 8,//	˥��
	USTATE_CRAZY  	= 9,//	����
	USTATE_LUCKY  	= 10,//	ף��
	USTATE_FIRM  	= 11,//	���
	USTATE_ARMOUR  	= 12,//	����
	USTATE_PRAY  	= 13,//	��
	USTATE_BIBLE  	= 14,//	ʥ��֮��
	USTATE_BIBLEARMOUR  	= 15,//	ʥ��֮��
	USTATE_WEAKEN  	= 16,//	����
	USTATE_MAGICMIRROR  	= 17,//	ħ��
	USTATE_BLIND  	= 18,//	ʧ��
	USTATE_ROCK		= 19,//	ʯ��
	USTATE_CONFUSION  	= 20,//	����
	USTATE_PALSY  	= 21,//	���
	USTATE_BLED  	= 22,//	��Ѫ
	USTATE_REVERT  	= 23,//	�ظ�ͼ��
	USTATE_RECOVERY  	= 24,//	����ͼ��
	USTATE_LIFE  	= 25,//	����ͼ��
	USTATE_PROCTECTED  	= 26,//	�ӻ�ͼ��
	USTATE_ABILITY  	= 27,//	����ͼ��
	USTATE_BLOOD  	= 28,//	��Ѫͼ��
	USTATE_MAGIC  	= 29,//	����ͼ��
	USTATE_SLOW  	= 30,//	����ͼ��
	USTATE_PLANTING  	= 31,//	��ֲ
	USTATE_DIGGING   	= 32,//	�ھ�
	USTATE_PROSPECTING  = 33,//	̽��
	USTATE_CUTTING  	= 34,//	����
	USTATE_GATHERING  	= 35,//	�ɼ�
	USTATE_FISHING  	= 36,//	����
	USTATE_HUNTING  	= 37,//	����
	USTATE_GUARD 		= 38,//	����
	USTATE_MAGICSHELL	= 39,//	������
	USTATE_NOSTOPATK	= 40,//	��������
	USTATE_IRONDFE		= 41,//	��������
	USTATE_LOSTBLOOD	= 42,//	ʧѪ֮Ӱ
	USTATE_POWEROFLAND	= 43,//	���֮��
	USTATE_SPEEDNESSATK	= 44,//	���ٹ���
	USTATE_BODYWEAK		= 45,//	��������
	USTATE_ATKWEAK		= 46,//	��������
	USTATE_RELIVEWEAK	= 47,//   ��������״̬
	USTATE_BOMB			= 48,//	����ը��
	USTATE_RELIVERING	= 49,//	����⻷
	USTATE_NOTHING		= 50,//	����
	USTATE_MOREHURT		= 51,//	�˺�����
	USTATE_MIRRORHURT	= 52,//	�˺�����
	USTATE_OLD			= 53,//	˥��
	USTATE_MINGYUN		= 54,//	����
	USTATE_MINDWEAK		= 55,//	��������
	USTATE_MAGICWEAK	= 56,//	��������
	USTATE_ROSE			= 57,//	õ���
	USTATE_BLANCEHURT	= 58,//	�˺�ƽ��
	USTATE_FACEOFF		= 59,//	������
	USTATE_HURTWEAK		= 60,//	�˺�����
	USTATE_SIGNOFSNIPER	= 61,//	���ֱ��
	USTATE_GHOSTSHELL	= 62,		///	��껤��
	USTATE_ATKFAST		= 63,		///	��������
	USTATE_FASTATK		= 64,		///	���ٹ���
	USTATE_BLOODHURT	= 65,		///	��Ѫ�˺�
	USTATE_POWEROFWIND	= 66,		///	��֮��
	USTATE_POWEROFFIRE	= 67,		///	��֮��
	USTATE_THOUCHOFFLASH	= 68,	///	�׵�֮��
	USTATE_SHELLOFFLASH	= 69,		///	���绤��
	USTATE_THOUCHOFTHOUNDER	= 70,	///	����֮��
	USTATE_SPEEDOFWIND	= 71,		///	��֮�ٶ�
	USTATE_WEIHE		= 72,		///	����
	USTATE_SPDHEATH		= 73,		///	�������ٻظ�
	USTATE_DAMBOCHONG	= 74,		///	�˺�����
	USTATE_ATKMORE		= 75,		///	������ǿ
	USTATE_SHANBIZUZHOU	= 76,		///	��������
	USTATE_START_QUEST	= 77,		///	����ʼ
	USTATE_DOING_QUEST	= 78,		///	���������
	USTATE_FINISH_QUEST	= 79,		///	�������
	USTATE_TIZHIJIAQIANG	= 80,	///	���Ǽ�ǿ
	USTATE_RIDE			= 81,		///	����
	USTATE_POWERATK		= 82,		/// ����ǿ��
	USTATE_POWERDEF		= 83,		/// ����ǿ��
	USTATE_SMILE		= 84,		/// ����
	USTATE_WINHEART		= 85,		/// ʤ�ľ�
	USTATE_THREAT		= 86,		/// ����
	USTATE_ACCRTSKILL	= 87,		/// ��׼֮��
	USTATE_DEATHHURT	= 88,		/// ����һ��
	USTATE_SILENT		= 89,		/// �侲
	USTATE_HUNTERSIGN	= 90,		/// ����ӡ��
	USTATE_WONDERIDEAR	= 91,		/// ڤ��
	USTATE_PROTECTGOODS	= 92,		/// ����
	USTATE_MAGICVESSEL	= 93,		/// ��������
	USTATE_POWERBASE	= 94,		/// Ԫ����ǿ
	USTATE_BREAKBASE	= 95,		/// Ԫ�����
	USTATE_CHANGEFACE	= 96,		/// ������
	USTATE_RUSHCRAZY	= 97,		/// �����ѣ
	USTATE_DIAMONDMAGIC	= 98,		/// ��շ���
	USTATE_UPMAGIC		= 99,		/// ��������
	USTATE_GODBLESS		= 100,		/// ��ʥף��
	USTATE_REDHOT		= 101,		/// ���׺촽
	USTATE_LOSTSPRITE	= 102,		/// ��ѣ
	USTATE_BLOODATTACK	= 103,		/// Ѫצ����
	USTATE_ARROWCONTROL = 104,		/// ��������
	USTATE_SITDOWN		= 105,		/// ����
	USTATE_SUMMON_STRENGTH	= 106, //�ٻ���ǿ��
	USTATE_DEFENCE_HURT		= 107, //��������
	USTATE_HURT_ABSTRACT	= 108, //�˺�����
	USTATE_BOMB_ATTACK		= 109, //����
	USTATE_MAGIC_DEFENCE	= 110, //ħ����
	USTATE_FIRST_AID		= 111, //������
	USTATE_ANGRY			= 112, //��ŭ
	USTATE_PK               = 113, //PK׷��״̬
	USTATE_SAY_NOTHING		= 114, //��������
	USTATE_WIND_ATTACK		= 115, //����
	USTATE_FIRE_ATTACK		= 116, //����
	USTATE_PET_CARE			= 117,
	USTATE_SPECIAL_DEFENCE	= 118,
	USTATE_BREAK_DEFENCE	= 119,
	USTATE_WEIGHT_HURT		= 120,
	USTATE_RED_FIRE_JUE		= 121,
	USTATE_MOST_BEST_DEFENCE	= 122,
	USTATE_TAKE_CARE		= 123,
	USTATE_GIVEUP_MACHINE	= 124,
	USTATE_MOST_BEST_GOLD_DEFENCE	= 125,
	USTATE_FIRE_WALL_DEFENCE	= 126,
	USTATE_ICE_EQUIPMENT		= 127,
	USTATE_ICE_FIGHT		= 128,
	USTATE_ICE_DEFENCE		= 129,
	USTATE_HIDEN_SOUND		= 130,
	USTATE_GOD_GIVE_SOUND	= 131,
	USTATE_CONJIONCE_TOGETHER	= 132,
	USTATE_ROCK_STATE		= 133,
	USTATE_WIND_SKILL		= 134,
	USTATE_BAD_SAY			= 135,
	USTATE_EAT_EAGLE		= 136,
	USTATE_EAT_SOUL			= 137,
	USTATE_POISON_EQUIPMENT	= 138,
	USTATE_FIGHT_TO_DEFENCE	= 139,
	USTATE_DEFENCE_TO_FIGHT	= 140,
	USTATE_CAREFUL_LOOKAT	= 141,
	USTATE_GOLD_EQUIPMENT	= 142,
	USTATE_MAGIC_EQUIPMENT	= 143,
	USTATE_IRON_EQUIPMENT	= 144,
	USTATE_ATTACK_INCREASE	= 145,
	USTATE_DEFENCE_INCREASE	= 146,
	USTATE_FIRE_BURNING	= 147,
	USTATE_BOW_ARROW_SKILL	= 148,
	USTATE_MAGIC_SKILL	= 149,
	USTATE_SPRITE_SKILL	= 150,
	USTATE_GUNSHU_SKILL	= 151,
	USTATE_MAGIC_SKILL_MASTER	= 152,
	USTATE_DURATION	= 153,
	USTATE_COMBIN_STONE_PET	= 154,
	USTATE_COMBIN_BONE_PET	= 155,
	USTATE_COMBIN_GOD_FIGHTER	= 156,
	USTATE_RELIVE_PET		= 157,
	USTATE_GOD_BLESS		= 158,
	USTATE_DEFENCE_LOWER	= 159,
	USTATE_CUT_SOFTBONE		= 160,
	USTATE_FEEDIN_OBJ		= 161,
	USTATE_FAMILY_ADD_PATK	= 162,
	USTATE_FAMILY_ADD_MATK	= 163,
	USTATE_FAMILY_ADD_PDEF	= 164,
	USTATE_FAMILY_ADD_MDEF	= 165,
	USTATE_FAMILY_ADD_MAXHP	= 166,
	USTATE_FAMILY_ADD_MAXMP	= 167,
	USTATE_COUNTRY_WAR	= 168, // ��ս״̬(���Է����Լ����ڹ�ս״̬ʱ�����������־)
	USTATE_FIREWALL			= 169,
	USTATE_FEED_BLOOD		= 170,
	USTATE_SAY_ANGRY		= 171,
	USTATE_NEED_NOT_FACE	= 172,
	USTATE_MOVE_POS_SPEED	= 173,
	USTATE_STAND_SOLID		= 174,
	USTATE_SHAKE_ATTACK		= 175,
	USTATE_BLOOD_ANGRY		= 176,
	USTATE_KEEP_SILENCE		= 177,
	USTATE_BOTH_SKILL_IN_MIND	= 178,
	USTATE_SAY_BAD_WORDS	= 179,
	USTATE_COMBIN_WIND_PET	= 180,
	USTATE_KEEP_FAR_AWAY	= 181,
	USTATE_COLD_ICE_SKILL	= 182,
	USTATE_FAST_SHOOT		= 183,
	USTATE_EAT_GHOST		= 184,
	USTATE_NO_FOOD_EAT		= 185,
	USTATE_DEFENCE_RING		= 186,
	USTATE_COMBIN_GOD_LEADER	= 187,
	USTATE_SOFT_CLOTH		= 188,
	USTATE_GIVEUP_LIFE		= 189,
	USTATE_TWO_COLD_EFFECT	= 190,
	USTATE_DEFENCE_RING_EFFECT	= 191,
	USTATE_STEEL_MAN_SKILL	= 192,
	USTATE_NO_COUNTRY_LIMIT	= 193,
	USTATE_STRENGTH_ATTACK_BLESS	= 194,
	USTATE_TEAM_ATTACK_BIRD_FLAG	= 195,
	USTATE_TEAM_ATTACK_FLOW_CLOUD	= 196,
	USTATE_NO_MOVE			= 197,
	USTATE_MOVE_FAST		= 198,
	USTATE_GOD_BLESS_U		= 199,
	USTATE_FIRE_ROAD		= 200,
	USTATE_CRAZY_ACTION		= 201,
	USTATE_KILL_MAGIC_DEFENCE	= 202,
	USTATE_KILL_PHYSIC_DEFENCE	= 203,
	USTATE_TECH_OF_WIND		= 204,
	USTATE_UNLIMIT_MAGIC	= 205,
	USTATE_GOD_FIGHTER_EYE	= 206,
	USTATE_ROTATE_FIRE_LAMP	= 207,
	USTATE_COOL_TOUCH		= 208,
	USTATE_DANCE_SHADOW		= 209,
	USTATE_WIND_THOUNDER_DEFENCE	= 210,
	USTATE_COMMON_TEMP_USE	= 211,
	USTATE_ICE_BOX_PROTECT	= 212,
	USTATE_STATIC_LIGHT_EFFECT	= 213,
	USTATE_SOFT_ATTACK		= 214,
	USTATE_MAGIC_DEFENCE_SWORD	= 215,
	USTATE_LIQUID_MOVE		= 216,
	USTATE_MP_STRENGTH		= 217,
	USTATE_HP_STRENGTH		= 218,
	USTATE_ATTACK_BACK_HEART = 219,
	USTATE_SOFT_BOTH_HAND	= 220,
	USTATE_FIRE_HEART_SKILL	= 221,
	USTATE_CRAZY_ATTACK		= 222,
	USTATE_IGNORE_ATTACK	= 223,
	USTATE_STAND_WITH__GHOST	= 224,
	USTATE_LION_HEART		= 225,
	USTATE_WIND_LEADER		= 226,
	USTATE_COMBIN_IRON_PET	= 227,
	USTATE_COMBIN_BONE_GUARD	= 228,
	USTATE_COMBIN_XIAN_PET		= 229,
	USTATE_RESCUE_QUICKLY	= 230,
	USTATE_NO_USE_SKILL		= 231,
	USTATE_ICE_RANGE		= 232,
	USTATE_PROTECT_SKILL	= 233,
	USTATE_TEAM_ATTACK_ONE_DIM	= 234,
	USTATE_ULTRA_EQUIPMENT	= 235,
	USTATE_FIRE_EAT_ALL		= 236,
	USTATE_REJECT_LIGHT_RING	= 237,
	USTATE_SOFT_LOVE_HEART	= 238,
	USTATE_TOGETHER_WITH_DRAGON	= 239, // ��������
	USTATE_TOGETHER_WITH_TIGER	= 240, // ���Ǹ���
	USTATE_IRON_BONE_SPRITE		= 241,
	USTATE_IRON_BONE_GUARD		= 242,
	USTATE_BEFORE_DIE_STATE		= 243,
	USTATE_DO_WITHOUT_THINKING	= 244,
	USTATE_HIDE_PET_TRAINING	= 245,
	USTATE_RELIVE_FAST_STATE	= 246,
	USTATE_ATTACK_BACK_STATE	= 247,
	USTATE_FIRE_WITH_FIRE		= 248,
	USTATE_FIRE_NO_EFFECT		= 249,
	USTATE_ATTACK_BLESS		= 250,
	USTATE_GOD_LABOR_BLESS		= 251,
	USTATE_STRENGTH_DEFENCE_SKILL	= 252,
	USTATE_HIDE_NOMOVE_STATE	= 253,
	USTATE_PRIVATE_STORE		= 254, // ��̯
	USTATE_HANLIU			= 255,//����
	USTATE_XIXINGEMIA		= 256,//ϴ�ĸ���
	USTATE_QUBAOJIUYI		= 257,//ȡ����ҽ
	USTATE_BLIND_STRENGTH	= 258,
	USTATE_MAKE_FORWORD		= 259,
	USTATE_COME_ON_BABY		= 260,
	USTATE_ATTACK_STORM_SKILL	= 261,
	USTATE_PROPERTY_DROPDOWN	= 262,
	USTATE_HEART_OF_DEAMON		= 263,
	USTATE_BODY_COLD_STONE		= 264,
	USTATE_BODY_CHANGE_OLD		= 265,
	USTATE_SHAINT_SHIELD_BLESS	= 266,
	USTATE_SHAINT_LIGHT_BLESS	= 267,
	USTATE_GIVEUP_BACK_ROAD		= 268,
	USTATE_PEOPLE_CAN_DEFEAT_GOD	= 269,
	USTATE_WASH_SNOW_LIGHT_RING	= 270,
	USTATE_COMBIN_FIRE_LEADER	= 271,
	USTATE_COMBIN_SIX_GHOST		= 272,
	USTATE_COMBIN_THREE_DIM		= 273,
	USTATE_HURT_XIAN			= 274,
	USTATE_HURT_MAGIC			= 275,
	USTATE_DEATH_ATTACH			= 276,
	USTATE_POWER_SPRITE			= 277,
	USTATE_CALL_RAIN			= 278,
	USTATE_DEEP_EARTH_FIRE		= 279,
	USTATE_BLOOD_DUTY			= 280,
	USTATE_DANCE_AMONG_DURT_WATER	= 281,
	USTATE_DAOJISHI				= 282,//���շѵ�ͼ�ļ�ʱ

	MAX_STATE = 320                 /// ���״̬��
};



/// �����ƶ��
#ifndef _GOOD_NESS_
#define _GOOD_NESS_
enum
{
	GOODNESS_0		= -120 & 0x0000FFFF,			//Ӣ��
	GOODNESS_1		= -60 & 0x0000FFFF,				//��ʿ
	GOODNESS_2_1	= 0,							//��ͨ1
	GOODNESS_2_2	= 0xFF000000,					//��ͨ2
	GOODNESS_3		= 60,							//��ͽ
	GOODNESS_4		= 120,							//��ͽ
	GOODNESS_5		= 180,							//��ħ
	GOODNESS_6		= 300,							//ħͷ
	GOODNESS_7		= 10000,						//�ѹ�
	
	GOODNESS_ATT	= 0x00010000,					//�����˺�����
	GOODNESS_DEF	= 0x00010000 << 1				//������
};
#endif
///�����ƶ�Ƚ���

/// ���ĳ��״̬�Ƿ�����
inline bool isset_state(const unsigned char *state, const int teststate)
{
	return 0 != (state[teststate / 8] & (0xff & (1 << (teststate % 8))));
}

/// ����ĳ��״̬
inline void set_state(unsigned char *state, const int teststate)
{
	state[teststate / 8] |= (0xff & (1 << (teststate % 8)));
}

/// ���ĳ��״̬
inline void clear_state(unsigned char *state, const int teststate)
{
	state[teststate / 8] &= (0xff & (~(1 << (teststate % 8))));
}

struct t_MapBuildingData{
	DWORD dwMapBuildingTempID;			/**< ���������ʱID */
	DWORD dwBuildingDataID;				/**< �����������ID */
	DWORD x;
	DWORD y;
	DWORD dwContract;					/**< ������� */
	char pstrName[MAX_NAMESIZE];		/**< ����������� */
	char pstrUserName[MAX_NAMESIZE];	/**< ����������������� */
};

struct t_MapObjectData {
	DWORD dwMapObjectTempID;				/**< ��Ʒ����ʱID */
	DWORD dwObjectID;                       /**< ��Ʒ������ID */
	char pstrName[MAX_NAMESIZE];			/**<  ��Ʒ�� */
	DWORD x;
	DWORD y;
	DWORD wdNumber;                         /**< ���� */
	WORD  wdLevel;                          /**< �ȼ� */
	BYTE upgrade;
	BYTE kind;
	DWORD dwOwner;
};

//���������
enum petType
{
	PET_TYPE_NOTPET 	= 0,	//���ǳ���
	PET_TYPE_RIDE		= 1,	//����
	PET_TYPE_PET		= 2,	//����
	PET_TYPE_SUMMON		= 3,	//�ٻ���
	PET_TYPE_TOTEM		= 4,	//ͼ��
	PET_TYPE_GUARDNPC	= 5,	//Ҫ������npc
	PET_TYPE_SEMI           = 6,    //����������ͬ��ͬ����AI��ȫû��ϵ
	PET_TYPE_CARTOON        = 7     //��ͨ����
};

struct t_NpcData {
	DWORD dwMapNpcDataPosition;	/**< npc������λ�� */
	DWORD dwNpcDataID;			/**< npc������ID */
	WORD  movespeed;		/// �ƶ��ٶ�
	
	BYTE byDirect;				/**< Npc�ķ��� */
	BYTE level;///�ȼ�
};
struct t_MapNpcData :public t_NpcData
{
	BYTE byState[(MAX_STATE + 7) / 8];			/**< Npc��״̬ */
};
struct t_MapNpcDataState :public t_NpcData
{
	t_MapNpcDataState()
	{
		num=0;
	}
	BYTE num; 		///��״̬������
	WORD state[0];		///״̬�б�
};

struct t_MapPetData {
	DWORD tempID;//npc����ʱID

	char name[MAX_NAMESIZE]; //npc������
	BYTE masterType;	///���˵�����  0����� 1��npc
	DWORD masterID;	///���˵�id -1��û������
	char masterName[MAX_NAMESIZE];///���ε�����
	BYTE pet_type;//��������
};

struct t_MapUserSculpt{
	DWORD dwHorseID;					/// ���Ʊ��
	DWORD dwHairID;						/// ͷ������
	DWORD dwBodyID;						/// ������ƷID
	DWORD dwLeftHandID;					/// ������ƷID
	DWORD dwRightHandID;				/// ������ƷID
};

//�����־
enum{
	TEAD_STATE_NONE ,
	TEAD_STATE_MEMBER ,
	TEAM_STATE_LEADER,
};

/// �û���������ݣ�������״̬
struct t_UserData
{
	DWORD dwUserTempID;					//// �û���ʱID

	char  name[MAX_NAMESIZE + 1];		/// ��ɫ����
	WORD  type;							/// ְҵ����
	//WORD  sex;							/// �Ա�
	WORD  face;							/// ͷ��
	DWORD goodness;						/// �ƶ��

	t_MapUserSculpt sculpt;
	DWORD dwHairRGB;					/// ͷ����ɫ0xffrrggbb
	DWORD dwBodyColorSystem;			/// �·�ϵͳ��ɫ0xffrrggbb
	DWORD dwBodyColorCustom;			/// �·�������ɫ0xffrrggbb
	DWORD dwLeftWeaponColor;			/// ����������ɫ0xffrrggbb
	DWORD dwRightWeaponColor;			/// ����������ɫ0xffrrggbb
	WORD  attackspeed;					/// �����ٶ�
	WORD  movespeed;					/// �ƶ��ٶ�
	BYTE country;						/// ����
	DWORD dwChangeFaceID;				//���ݺ��NPC������ID
	BYTE live_skills[6]; 				//����ܵȼ�
	BYTE level;							//Level : 1(<10)   11(>=10)
	DWORD exploit;				// ��ѫֵ
	DWORD grace;				// �Ĳ�ֵ
	
	DWORD dwUnionID;			// ���ID
	DWORD dwSeptID;				// ����ID
	
	char  caption[MAX_NAMESIZE];		// ����ͷ�Σ�ĳ�ǳ�����ĳ��������
	DWORD dwTeamState;					// ����״̬
	DWORD dwArmyState;			//1Ϊ�ӳ���2Ϊ����,0Ϊδ�������
};

struct t_MapUserData : public t_UserData 
{
	BYTE state[(MAX_STATE + 7) / 8];	/// ��ɫ״̬
};
struct t_MapUserDataState : public t_UserData 
{
	t_MapUserDataState()
	{
		num=0;
	}
	BYTE num; 		///��״̬������
	WORD state[0];		///״̬�б�
};

struct t_MainUserData 
{
	DWORD dwUserTempID;				/// �û���ʱID
	WORD  level;					/// ��ɫ�ȼ�
	DWORD hp;						/// ��ǰ����ֵ
	DWORD maxhp;					/// �������ֵ
	DWORD resumehp;					/// ����ֵ�ָ�
	DWORD mp;						/// ��ǰ����ֵ
	DWORD maxmp;					/// �����ֵ
	DWORD resumemp;					/// ����ֵ�ָ�
	DWORD sp;						/// ��ǰ����ֵ
	DWORD maxsp;					/// �������ֵ
	DWORD resumesp;					/// ����ֵ�ָ�
	DWORD pdamage;					/// ��С��������
	DWORD maxpdamage;				/// �����������
	DWORD mdamage;					/// ��С����������
	DWORD maxmdamage;				/// �����������
	DWORD pdefence;					/// ���������
	DWORD mdefence;					/// ����������
	QWORD exp;						/// ��ǰ����ֵ
	QWORD nextexp;					/// ��������ֵ
	WORD  attackrating;				/// ��������
	WORD  attackdodge;				/// �������
	WORD  bang;						/// �ػ�
	WORD  lucky;					/// ����ֵ
	WORD  charm;					/// ����ֵ
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
	WORD  skillPoint;                   /// ���ܵ���
	WORD  points;                       /// ����
	DWORD country;						/// ����
	WORD  pkmode;                       /// pkģʽ


	DWORD stdpdamage;					/// ��׼��������
	DWORD stdmdamage;					/// ��׼����������
	DWORD stdpdefence;					/// ��׼���������
	DWORD stdmdefence;					/// ��׼����������
	WORD  stdbang;						/// ��׼�ػ���
	union {
		struct {
			WORD wdStdCon;	//����
			WORD wdStdStr;	//����
			WORD wdStdDex;	//����
			WORD wdStdInt;	//����
			WORD wdStdMen;	//����
		};
		WORD wdStdProperty[5];
	};
	WORD wdTire; /// ƣ��״̬ 0Ϊ�� 1Ϊƣ��
	DWORD fivetype;	///��������
	DWORD fivepoint;///���е���
	DWORD honor;///����ֵ
	DWORD maxhonor;///�������ֵ
	DWORD gold;	///�����
	DWORD ticket; //��ȯ��
	DWORD bitmask;					/// ��ɫ����
};

struct stDataUserCmd : public stNullUserCmd
{
	stDataUserCmd()
	{
		byCmd = DATA_USERCMD;
	}
};

/// ���û�����
const BYTE MAIN_USER_DATA_USERCMD_PARA = 1;
	struct stMainUserDataUserCmd : public stDataUserCmd {
		stMainUserDataUserCmd()
		{
			byParam = MAIN_USER_DATA_USERCMD_PARA;
		}

		t_MainUserData data;
	};
/*
enum {
	DECTYPE_POWER,				/// ����
	DECTYPE_STRENGTH,			/// ����
	DECTYPE_INTELLECTUALITY,	/// ����
	DECTYPE_CORPOREITY,			/// ����
	DECTYPE_DEXTERITY			/// ����
};
/// ����ʣ����������ӵ���������
const BYTE DECREMAINDER_DATA_USERCMD_PARA = 2;
	struct stDecRemainderDataUserCmd : public stDataUserCmd{
		stDecRemainderDataUserCmd()
		{
			byParam = DECREMAINDER_DATA_USERCMD_PARA;
		}

		BYTE byDecType;				**< �ӵ����� *
	};

*/

/// �����û�������ħ��
const BYTE SETHPANDMP_DATA_USERCMD_PARA = 3;
	struct stSetHPAndMPDataUserCmd : public stDataUserCmd{
		stSetHPAndMPDataUserCmd()
		{
			byParam = SETHPANDMP_DATA_USERCMD_PARA;
		}

		DWORD dwHP;					/**< HP */
		DWORD dwMP;					/**< MP */
		//DWORD dwSP;					/**< SP */
	};

enum {
	MAP_SETTING_RIDE	= 0x00000001,	/// ��½��������
	MAP_SETTING_WEATHER	= 0x00000002	/// ��½����ѩ�����仯
};
/// ��ͼ��Ϣ
const BYTE MAPSCREENSIZE_DATA_USERCMD_PARA = 28;
	struct stMapScreenSizeDataUserCmd : public stDataUserCmd{
		stMapScreenSizeDataUserCmd()
		{
			byParam = MAPSCREENSIZE_DATA_USERCMD_PARA;
		}

		DWORD width;		/**< ������ */
		DWORD height;		/**< ������ */
		char pstrMapName[MAX_NAMESIZE];	/**< ��½���� */
		char pstrFilename[MAX_NAMESIZE];	/**< �ļ����� */
		DWORD setting;				/**< ��½��־ */
		DWORD rgb;					/**< ��ɫ��־0x00rrggbb */
		char pstrGroupName[MAX_NAMESIZE];	/**< ������������ */
		char pstrCountryName[MAX_NAMESIZE];	/**< �������� */
		char pstrCityName[MAX_NAMESIZE];	/**< �������� */
		int mainRoleX;	
		int mainRoleY;
		short npc_count;
		struct {
			DWORD id, x, y;
		} npc_list[0];
	};

//�ͻ��˼��ص�ͼ���
const BYTE LOADMAPOK_DATA_USERCMD_PARA = 29;
	struct stLoadMapOKDataUserCmd : public stDataUserCmd{
		stLoadMapOKDataUserCmd()
		{
			byParam = LOADMAPOK_DATA_USERCMD_PARA;
		}
	};

/// ��ɫ�ĵȼ�����
const BYTE LEVELDEGREE_DATA_USERCMD_PARA = 30;
	struct stLevelDegreeDataUserCmd : public stDataUserCmd{
		stLevelDegreeDataUserCmd()
		{
			byParam = LEVELDEGREE_DATA_USERCMD_PARA;
		}
		WORD degree; // 1-2000��
	};

/// �����������ļ�
const BYTE STAMP_DATA_USERCMD_PARA = 51;
struct stStampDataUserCmd : public stDataUserCmd
{
	bool check;//�Ƿ������Ҽ��
	DWORD size;
	BYTE bin[0];//�������ļ�
	stStampDataUserCmd()
	{
		byParam = STAMP_DATA_USERCMD_PARA;

		check = false;
		size = 0;
	}
};

/// �ͻ��˽�����Ҽ��
const BYTE STAMP_CHECK_USERCMD_PARA = 52;
struct stStampCheckUserCmd : public stDataUserCmd
{
	DWORD flag;//0 ��; 1 ��
	stStampCheckUserCmd()
	{
		byParam = STAMP_CHECK_USERCMD_PARA;

		flag = 0;
	}
};

/// ���͵�ǰ�ϲ��汾��
const BYTE MERGE_VERSION_CHECK_USERCMD_PARA = 53;
struct stMergeVersionCheckUserCmd : public stDataUserCmd
{
	stMergeVersionCheckUserCmd()
	{
		byParam = MERGE_VERSION_CHECK_USERCMD_PARA;
		dwMergeVersion = 0;
	}

	DWORD dwMergeVersion;
};

/// ���ͳ�ʼ����Ϣ����
const BYTE ENDOFINITDATA_DATA_USERCMD_PARA = 255;
	struct stEndOfInitDataDataUserCmd : public stDataUserCmd{
		stEndOfInitDataDataUserCmd()
		{
			byParam = ENDOFINITDATA_DATA_USERCMD_PARA;
		}
	};

//////////////////////////////////////////////////////////////
/// ����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ��ͼ����ָ��忪ʼ
//////////////////////////////////////////////////////////////
	struct stMapScreenUserCmd : public stNullUserCmd{
		stMapScreenUserCmd()
		{
			byCmd = MAPSCREEN_USERCMD;
		}
	};


/// ��ͼ����������
const BYTE ADDUSER_MAPSCREEN_USERCMD_PARA = 10;
	struct stAddUserMapScreenUserCmd : public stMapScreenUserCmd {
		stAddUserMapScreenUserCmd()
		{
			byParam = ADDUSER_MAPSCREEN_USERCMD_PARA;
		}
		t_MapUserData data;
	};

/// ��ͼ����ɾ������
const BYTE REMOVEUSER_MAPSCREEN_USERCMD_PARA = 11;
	struct stRemoveUserMapScreenUserCmd : public stMapScreenUserCmd{
		stRemoveUserMapScreenUserCmd()
		{
			byParam = REMOVEUSER_MAPSCREEN_USERCMD_PARA;
		}

		DWORD dwUserTempID;			/**< �û���ʱID */
	};

/// �û����������û�����
const BYTE REQUESTUSERDATA_MAPSCREEN_USERCMD_PARA = 12;
	struct stRequestUserDataMapScreenUserCmd : public stMapScreenUserCmd{
		stRequestUserDataMapScreenUserCmd()
		{
			byParam = REQUESTUSERDATA_MAPSCREEN_USERCMD_PARA;
		}

		WORD size;
		DWORD dwUserTempID[0];	//��ʱ����б�(DWORD)
	};

/// �û�����
const BYTE RIDE_MAPSCREEN_USERCMD_PARA = 13;
struct stRideMapScreenUserCmd : public stMapScreenUserCmd
{
	stRideMapScreenUserCmd()
	{
		byParam = RIDE_MAPSCREEN_USERCMD_PARA;
	}

	DWORD dwUserTempID;			/**< �û���ʱID */
	BYTE bySwitch;				/**< �����أ����������������� */
};

/// ˢ�½�ɫ״̬
const BYTE REFRESHSTATE_MAPSCREEN_USERCMD_PARA = 14;
struct stRefreshStateMapScreenUserCmd : public stMapScreenUserCmd{
	stRefreshStateMapScreenUserCmd()
	{
		byParam = REFRESHSTATE_MAPSCREEN_USERCMD_PARA;
		bzero(state , sizeof(state));
	}
	DWORD dwUserTempID;			/**< �û���ʱID */
	BYTE state[(MAX_STATE + 7) / 8];	/// ��ɫ״̬
	
};

/// ������Ʒ��������
enum{
		OBJECTACTION_DROP,			/// ������Ʒ
			OBJECTACTION_UPDATE,		/// ������Ʒ����
};
/// �ڵ�ͼ��������Ʒ
const BYTE ADDMAPOBJECT_MAPSCREEN_USERCMD_PARA = 20;
	struct stAddMapObjectMapScreenUserCmd  : public stMapScreenUserCmd{
		stAddMapObjectMapScreenUserCmd()
		{
			byParam = ADDMAPOBJECT_MAPSCREEN_USERCMD_PARA;
			bzero(&data,sizeof(data));
		}
		BYTE	action;
		t_MapObjectData data;
	};

/// �ڵ�ͼ��ɾ����Ʒ
const BYTE REMOVEMAPOBJECT_MAPSCREEN_USERCMD_PARA = 21;
	struct stRemoveMapObjectMapScreenUserCmd  : public stMapScreenUserCmd{
		stRemoveMapObjectMapScreenUserCmd()
		{
			byParam = REMOVEMAPOBJECT_MAPSCREEN_USERCMD_PARA;
		}
		DWORD dwMapObjectTempID;	/**< ��Ʒ����ʱID */
	};

/// �ڵ�ͼ�����ӽ�����
const BYTE ADDMAPBUILDING_MAPSCREEN_USERCMD_PARA = 30;
	struct stAddMapBuildingMapScreenUserCmd : public stMapScreenUserCmd{
		stAddMapBuildingMapScreenUserCmd()
		{
			byParam = ADDMAPBUILDING_MAPSCREEN_USERCMD_PARA;
		}

		t_MapBuildingData data;
	};


/// �ڵ�ͼ��ɾ��������
const BYTE REMOVEMAPBUILDING_MAPSCREEN_USERCMD_PARA = 31;
	struct stRemoveMapBuildingMapScreenUserCmd : public stMapScreenUserCmd{
		stRemoveMapBuildingMapScreenUserCmd()
		{
			byParam = REMOVEMAPBUILDING_MAPSCREEN_USERCMD_PARA;
		}

		DWORD dwMapBuildingTempID;	/**< ���������ʱID */
	};

///����ʹ��������
const BYTE CHANGEFACE_MAPSCREEN_USERCMD_PARA = 32;
	struct stChangeFaceMapScreenUserCmd : public stMapScreenUserCmd{
		stChangeFaceMapScreenUserCmd()
		{
			byParam = CHANGEFACE_MAPSCREEN_USERCMD_PARA;
		}

		DWORD	dwUserTempID;		//�û���ʱID
        DWORD	dwChangeFaceID;		//���ݺ��NPC������ID
	};

/// �ڵ�ͼ������NPC
const BYTE ADDMAPNPC_MAPSCREEN_USERCMD_PARA = 50;
	struct stAddMapNpcMapScreenUserCmd : public stMapScreenUserCmd{
		stAddMapNpcMapScreenUserCmd()
		{
			byParam = ADDMAPNPC_MAPSCREEN_USERCMD_PARA;
		}

		t_MapNpcData data;
	};

/// �ڵ�ͼ��ɾ��NPC
const BYTE REMOVEMAPNPC_MAPSCREEN_USERCMD_PARA = 51;
	struct stRemoveMapNpcMapScreenUserCmd : public stMapScreenUserCmd{
		stRemoveMapNpcMapScreenUserCmd()
		{
			byParam = REMOVEMAPNPC_MAPSCREEN_USERCMD_PARA;
		}
		DWORD dwMapNpcDataPosition;	/**< npc������λ�� */
	};

/// ����Npc����
const BYTE REQUESTMAPNPCDATA_MAPSCREEN_USERCMD_PARA = 52;
	struct stRequestMapNpcDataMapScreenUserCmd : public stMapScreenUserCmd{
		stRequestMapNpcDataMapScreenUserCmd()
		{
			byParam = REQUESTMAPNPCDATA_MAPSCREEN_USERCMD_PARA;
		}

		WORD size;
		DWORD dwNpcTempID[0];	//��ʱ����б�(DWORD)
	};

/// NpcѪ��
const BYTE NPCHP_MAPSCREEN_USERCMD_PARA = 53;
	struct stNPCHPMapScreenUserCmd : public stMapScreenUserCmd{
		stNPCHPMapScreenUserCmd()
		{
			byParam = NPCHP_MAPSCREEN_USERCMD_PARA;
		}
//BEGIN_MUTABLE_VARS
		DWORD dwMapNpcDataPosition;	/**< npc������λ�� */
		DWORD dwHP;					/**< HP */
		DWORD dwMaxHP;				/**< MaxHP */
//END_MUTABLE_VARS
	};

// NPC ���� λ����Ϣ
struct stNpcPosition{
	DWORD dwTempID;				//Ŀ����
	WORD x;					/**< ����*/
	WORD y;				
	BYTE byDir;					// �������Ϊ -1 ��ȷ������
};

// NPC ���� TEMPID��Ϣ
struct stNpcTempID{
	DWORD dwTempID;				//Ŀ����
};

/// ��9���û���NPC
const BYTE ALL_MAPSCREEN_USERCMD_PARA = 54;
	struct stAllMapScreenUserCmd : public stMapScreenUserCmd{
		stAllMapScreenUserCmd()
		{
			byParam = ALL_MAPSCREEN_USERCMD_PARA;
		}

		MapData_ItemHeader mdih;
		stNpcTempID psi[0];
	};

struct t_MapUserDataPos :public t_MapUserData
{
	WORD x;					/**< ����*/
	WORD y;				
	BYTE byDir;					// �������Ϊ -1 ��ȷ������
};
struct t_MapUserDataPosState :public t_UserData
{
/*
	t_MapUserDataPosState()
	{
		num=0;
	} 
// */
	WORD x;					/**< ����*/
	WORD y;				
	BYTE byDir;					// �������Ϊ -1 ��ȷ������
	BYTE num; 		///��״̬������
	WORD state[0];		///״̬�б�
};
struct t_MapNpcDataPos :public t_MapNpcData
{
	WORD x;					/**< ����*/
	WORD y;				
	BYTE byDir;					// �������Ϊ -1 ��ȷ������
};
struct t_MapNpcDataPosState :public t_NpcData
{
/*
	t_MapNpcDataPosState()
	{
		num=0;
	} 
// */
	WORD x;					/**< ����*/
	WORD y;				
	BYTE byDir;					// �������Ϊ -1 ��ȷ������
	BYTE num; 		///��״̬������
	WORD state[0];		///״̬�б�
};
struct t_MapBuildingDataPos :public t_MapBuildingData
{
	WORD x;					/**< ����*/
	WORD y;				
	BYTE byDir;					// �������Ϊ -1 ��ȷ������
};

// ��ͼ���ݴ������
const BYTE MAPDATA_MAPSCREEN_USERCMD_PARA = 55;
	struct stMapDataMapScreenUserCmd : public stMapScreenUserCmd{
		stMapDataMapScreenUserCmd()
		{
			byParam = MAPDATA_MAPSCREEN_USERCMD_PARA;
		}

		MapData_ItemHeader mdih;
		union
		{
			t_MapUserDataPos mud[0];
			t_MapNpcDataPos mnd[0];
			t_MapBuildingDataPos mbd[0];
			t_MapPetData mpd[0];
		};
	};

#define SELECTED_HPMP_PROPERTY_USERCMD_PARA 56//֪ͨѡ���Լ������hp��mp�ı仯
struct stRTSelectedHpMpPropertyUserCmd : public stMapScreenUserCmd
{
	stRTSelectedHpMpPropertyUserCmd()
	{
		byParam = SELECTED_HPMP_PROPERTY_USERCMD_PARA;
	}
	BYTE byType;//enumMapDataType
//BEGIN_MUTABLE_VARS
	DWORD dwTempID;//��ʱ���
	DWORD dwHP;//��ǰѪ
	DWORD dwMaxHp;//���hp
	DWORD dwMP;//��ǰmp
	DWORD dwMaxMp;//���mp
//END_MUTABLE_VARS	
};
/// �����ͼ����Ʒ����
const BYTE CLEAR_OBJECTOWNER_MAPSCREEN_USERCMD_PARA = 58;
	struct stClearObjectOwnerMapScreenUserCmd  : public stMapScreenUserCmd{
		stClearObjectOwnerMapScreenUserCmd()
		{
			byParam = CLEAR_OBJECTOWNER_MAPSCREEN_USERCMD_PARA;
		}
		DWORD dwMapObjectTempID;	/**< ��Ʒ����ʱID */
	};
/// �ڵ�ͼ������NPC������
const BYTE ADDMAPNPC_AND_POS_MAPSCREEN_USERCMD_PARA = 59;
	struct stAddMapNpcAndPosMapScreenUserCmd : public stMapScreenUserCmd{
		stAddMapNpcAndPosMapScreenUserCmd()
		{
			byParam = ADDMAPNPC_AND_POS_MAPSCREEN_USERCMD_PARA;
		}

		t_MapNpcDataPos data;
	};

/// ��ͼ���������������
const BYTE ADDUSER_AND_POS_MAPSCREEN_USERCMD_PARA = 60;
	struct stAddUserAndPosMapScreenUserCmd : public stMapScreenUserCmd {
		stAddUserAndPosMapScreenUserCmd()
		{
			byParam = ADDUSER_AND_POS_MAPSCREEN_USERCMD_PARA;
		}
		t_MapUserDataPos data;
	};
/// ����ɾ��NPCָ��
const BYTE BATCHREMOVENPC_MAPSCREEN_USERCMD_PARA = 61;
	struct stBatchRemoveNpcMapScreenUserCmd : public stMapScreenUserCmd {
		stBatchRemoveNpcMapScreenUserCmd()
		{       
			byParam = BATCHREMOVENPC_MAPSCREEN_USERCMD_PARA;
		}
		WORD	num;
		DWORD   id[0];
	};

/// ����ɾ��USERָ��
const BYTE BATCHREMOVEUSER_MAPSCREEN_USERCMD_PARA = 62;
	struct stBatchRemoveUserMapScreenUserCmd : public stMapScreenUserCmd {
		stBatchRemoveUserMapScreenUserCmd()
		{       
			byParam = BATCHREMOVEUSER_MAPSCREEN_USERCMD_PARA;
		}
		WORD	num;
		DWORD   id[0];
	};
/// ���ý�ɫ״̬
const BYTE SETSTATE_MAPSCREEN_USERCMD_PARA = 63;
struct stSetStateMapScreenUserCmd : public stMapScreenUserCmd{
	stSetStateMapScreenUserCmd()
	{
		byParam = SETSTATE_MAPSCREEN_USERCMD_PARA;
	}
	BYTE	type;		/**<���� enumMapDataType*/
	DWORD	dwTempID;			/**< �û���ʱID */
	WORD	wdState;	/// ��Ҫ���õ�״̬
};
/// ȡ����ɫ״̬
const BYTE CLEARSTATE_MAPSCREEN_USERCMD_PARA = 64;
struct stClearStateMapScreenUserCmd : public stMapScreenUserCmd{
	stClearStateMapScreenUserCmd()
	{
		byParam = CLEARSTATE_MAPSCREEN_USERCMD_PARA;
	}
	BYTE	type;		/**<���� enumMapDataType*/
	DWORD	dwTempID;			/**< �û���ʱID */
	WORD	wdState;	/// ��Ҫȡ����״̬
};
/// �ڵ�ͼ������Npc�ͳ�������
const BYTE ADDMAPNPCPET_MAPSCREEN_USERCMD_PARA = 65;
	struct stAddMapNpcPetMapScreenUserCmd : public stMapScreenUserCmd{
		stAddMapNpcPetMapScreenUserCmd()
		{
			byParam = ADDMAPNPCPET_MAPSCREEN_USERCMD_PARA;
		}

		t_MapNpcDataPos data;
		t_MapPetData pet; 
	};
/// �ڵ�ͼ�����ӳ�������
const BYTE ADDMAPPET_MAPSCREEN_USERCMD_PARA = 66;
	struct stAddMapPetMapScreenUserCmd : public stMapScreenUserCmd{
		stAddMapPetMapScreenUserCmd()
		{
			byParam = ADDMAPPET_MAPSCREEN_USERCMD_PARA;
		}

		t_MapPetData pet; 
	};
/// ���ý�ɫ�ƶ�ֵ
const BYTE GOODNESS_MAPSCREEN_USERCMD_PARA = 67;
struct stGoodnessStateMapScreenUserCmd : public stMapScreenUserCmd{
	stGoodnessStateMapScreenUserCmd()
	{
		byParam = GOODNESS_MAPSCREEN_USERCMD_PARA;
	}
	DWORD	dwTempID;			/**< �û���ʱID */
	DWORD	dwGoodness;	/// �ƶ�ֵ
};
/// ��ͼ����������
const BYTE ADDUSER_MAPSCREEN_STATE_USERCMD_PARA = 68;
	struct stAddUserMapScreenStateUserCmd : public stMapScreenUserCmd {
		stAddUserMapScreenStateUserCmd()
		{
			byParam = ADDUSER_MAPSCREEN_STATE_USERCMD_PARA;
		}
		t_MapUserDataState data;
		int size()
		{
			return sizeof(*this) + data.num * sizeof(data.state[0]);
		}
	};
/// �ڵ�ͼ������NPC
const BYTE ADDMAPNPC_MAPSCREEN_STATE_USERCMD_PARA = 69;
	struct stAddMapNpcMapScreenStateUserCmd : public stMapScreenUserCmd{
		stAddMapNpcMapScreenStateUserCmd()
		{
			byParam = ADDMAPNPC_MAPSCREEN_STATE_USERCMD_PARA;
		}
		t_MapNpcDataState data;
		int size()
		{
			return sizeof(*this) + data.num * sizeof(data.state[0]);
		}
	};
/// ��ͼ���������������
const BYTE ADDUSER_AND_POS_MAPSCREEN_STATE_USERCMD_PARA = 70;
	struct stAddUserAndPosMapScreenStateUserCmd : public stMapScreenUserCmd {
		stAddUserAndPosMapScreenStateUserCmd()
		{
			byParam = ADDUSER_AND_POS_MAPSCREEN_STATE_USERCMD_PARA;
		}
		t_MapUserDataPosState data;
		int size()
		{
			return sizeof(*this) + data.num * sizeof(data.state[0]);
		}
	};
/// �ڵ�ͼ������NPC������
const BYTE ADDMAPNPC_AND_POS_MAPSCREEN_STATE_USERCMD_PARA = 71;
	struct stAddMapNpcAndPosMapScreenStateUserCmd : public stMapScreenUserCmd{
		stAddMapNpcAndPosMapScreenStateUserCmd()
		{
			byParam = ADDMAPNPC_AND_POS_MAPSCREEN_STATE_USERCMD_PARA;
		}
		t_MapNpcDataPosState data;
		int size()
		{
			return sizeof(*this) + data.num * sizeof(data.state[0]);
		}
	};
/// �ڵ�ͼ������Npc�ͳ�������
const BYTE ADDMAPNPCPET_MAPSCREEN_STATE_USERCMD_PARA = 72;
	struct stAddMapNpcPetMapScreenStateUserCmd : public stMapScreenUserCmd{
		stAddMapNpcPetMapScreenStateUserCmd()
		{
			byParam = ADDMAPNPCPET_MAPSCREEN_STATE_USERCMD_PARA;
		}

		t_MapPetData pet; 
		t_MapNpcDataPosState data;
		int size()
		{
			return sizeof(*this) + data.num * sizeof(data.state[0]);
		}
	};
// ��ͼ���ݴ������
const BYTE MAPDATA_MAPSCREEN_STATE_USERCMD_PARA = 73;
	struct stMapDataMapScreenStateUserCmd : public stMapScreenUserCmd{
		stMapDataMapScreenStateUserCmd()
		{
			byParam = MAPDATA_MAPSCREEN_STATE_USERCMD_PARA;
		}

		MapData_ItemHeader mdih;
		union
		{
			t_MapUserDataPosState mud[0];
			t_MapNpcDataPosState mnd[0];
			t_MapBuildingDataPos mbd[0];
			t_MapPetData mpd[0];
		};
	};
/*
/// ˢ�³�����Ϣ
const BYTE ADD_PET_MAPSCREEN_USERCMD_PARA = 61;
	struct stAddPetMapScreenUserCmd : public stMapScreenUserCmd {
		stAddPetMapScreenUserCmd()
		{
			byParam = ADD_PET_MAPSCREEN_USERCMD_PARA;
		}
		BYTE size;
		t_MapPetData data[0];
	};
	*/
//////////////////////////////////////////////////////////////
/// ��ͼ����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ħ��ָ��忪ʼ
//////////////////////////////////////////////////////////////
	struct stMagicUserCmd : public stNullUserCmd{
		stMagicUserCmd()
		{
			byCmd = MAGIC_USERCMD;
		}
	};

/// ���幥������
enum ATTACKTYPE{
	ATTACKTYPE_U2U,	/// �û������û�
	ATTACKTYPE_U2N,	/// �û�����Npc
	ATTACKTYPE_N2U,	/// Npc�����û�
	ATTACKTYPE_U2B,	/// �û���������
	ATTACKTYPE_U2P,	/// �û�������
	ATTACKTYPE_N2N	/// Npc����Npc
};


#ifndef _ANI_TYPE_ENUM
#define _ANI_TYPE_ENUM
enum AniTypeEnum
{
Ani_Null = 0,

	Ani_Wait,Ani_Stand = Ani_Wait,	// վ��
	Ani_Walk,						// ��
	Ani_Run,						// ��

	Ani_Attack,						// ����
	Ani_Attack2,					// ����2
	Ani_Attack3,					// ����3

	Ani_Magic,						// ħ������
	Ani_Magic2,						// ħ������2
	Ani_Magic3,						// ħ������3

	Ani_Bow,						// ���
	Ani_Bow2,						// Ŭ����

	Ani_Hurt,						// ����(����)
	Ani_Die,						// ����

	Ani_Sit,						// ����

	Ani_Ride_Wait,					// ����վ��
	Ani_Ride_Walk,					// ������
	Ani_Ride_Run,					// ������

	Ani_Ride_Attack,				// ������
	Ani_Ride_Magic,					// ����ʩ��

	Ani_Ride_Hurt,					// ����(����)
	Ani_Ride_Die,					// ��������

	Ani_Appear,						// ����(����)

	Ani_Attack_Stand,				// ����վ��
	Ani_Attack2_Stand,				// ����2վ��
	Ani_Attack3_Stand,				// ����2վ��

	Ani_Magic_Stand,				// ħ������վ��
	Ani_Magic2_Stand,				// ħ��2����վ��
	Ani_Magic3_Stand,				// ħ��3����վ��

	Ani_Bow_Stand,					// ��������վ��
	Ani_Bow2_Stand,					// Ŭ����վ��

	Ani_Ride_Attack_Stand,			// ������վ��
	Ani_Ride_Magic_Stand,			// ����ʩ��վ��

	Ani_Back,						// ����
	Ani_Ride_Back,					// �������

	Ani_Ride_Bow,					// �������
	Ani_Ride_Bow_Stand,				// �������վ��

	Ani_Fly_Walk,					// ��������
	Ani_Fly_Run,					// ���п���

	Ani_Fly_Hurt,					// ���б���
	Ani_Fly_Die,					// ��������

	Ani_Gather,						// ��ҩ

	Ani_FirstRun,					// ��һ����

	Ani_Idle0,
	Ani_Idle1,
	Ani_Idle2,
	Ani_Idle3,

	Ani_Gather_Stand,
	Ani_OpenBox_Stand,
	Ani_Num
};
#endif

/// ����ָ��
const BYTE MAGIC_USERCMD_PARA = 5;
struct stAttackMagicUserCmd : public stMagicUserCmd{

	stAttackMagicUserCmd()
	{
		byParam = MAGIC_USERCMD_PARA;
	}

//BEGIN_MUTABLE_VARS
	DWORD dwUserTempID;			/**< ������ʱ��� */
	DWORD dwDefenceTempID;		/**< ������ʱ��� */

	WORD wdMagicType;			/**< ħ������ */

	WORD xDes;					/**< Ŀ��� */
	WORD yDes;					/**< Ŀ��� */
//END_MUTABLE_VARS	
	
	BYTE byDirect;				/**< �������� */
	BYTE byAttackType;			/**< �������ͣ�ATTACKTYPE_U2U,ATTACKTYPE_U2N,ATTACKTYPE_N2U */
	BYTE byAction;				// ��������  AniTypeEnum ( Ani_Null Ϊ��ȷ���ģ��ɿͻ���ȷ�� )
	DWORD dwTempIDList[0];		// ����Ŀ���б�
};

enum {
	RTMAGIC_FAILURE,		/// ����ʧ��
	RTMAGIC_DUCK,			/// ��������
	RTMAGIC_FREEZE,			/// ����
	RTMAGIC_SPEEDUP,		/// ����
	RTMAGIC_POISON,			/// �ж�
	RTMAGIC_PLAGUE,			/// ����
	RTMAGIC_LANDIFICATION,	/// ʯ��
	RTMAGIC_VAMPIRE,		/// ��Ѫ
	RTMAGIC_VAMPIREMAGIC,	/// ��ħ
	RTMAGIC_HIDE,			/// ����
	RTMAGIC_BLANKSCREEN,	/// ����
	RTMAGIC_SUCCESS,		/// �����ɹ�
	RTMAGIC_ATTACKED,		/// ����
	RTMAGIC_DEFENCE			/// ��
};

/// �������ظ��Լ�
const BYTE RTMAGIC_USERCMD_PARA = 6;
struct stRTMagicUserCmd : public stMagicUserCmd{

	stRTMagicUserCmd()
	{
		byParam = RTMAGIC_USERCMD_PARA;
	}

	DWORD dwUserTempID;			/**< Ŀ����ʱ��� */
	BYTE byTarget;				/**< Ŀ�����ͣ�enumMapDataType */
	BYTE byRetcode;				/**< ���ش��룺RTMAGIC_SUCCESS,RTMAGIC_FAILURE... */
	BYTE byDirect;				/**< ���� */
	BYTE bySrc;					/**< ���������ͣ�enumMapDataType */

	DWORD dwSrcTempID;			/**< ��������ʱ��� */

	DWORD dwHP;					// ��ǰѪ
	SDWORD sdwHP;					/**< ��Ѫ */
	BYTE byLuck;				//���ˣ�Ϊ1��ʾ�������˹�����Ϊ0��ʾ��ͨ�������
};

enum enumPopEffect
{
	POP_HP,
	POP_MP,
	POP_NUM,
};

/// ����Ŀ��������Ѫ֪ͨ
const BYTE OBJECT_HPMP_POP_PARA = 11;
struct stObjectHpMpPopUserCmd : public stMagicUserCmd{

	stObjectHpMpPopUserCmd()
	{
		byParam = OBJECT_HPMP_POP_PARA;
	}

	DWORD dwUserTempID;			/**< Ŀ����ʱ��� */
	BYTE byTarget;				/**< Ŀ�����ͣ�enumMapDataType */

	int vChange;				/**< >0 :��		<0 :��*/
	enumPopEffect	type;
};

/// ����Ŀ������
const BYTE RTMAGIC_POS_USERCMD_PARA	= 12;
struct stRTMagicPosUserCmd : public stMagicUserCmd{

	stRTMagicPosUserCmd()
	{
		byParam = RTMAGIC_POS_USERCMD_PARA;
	}
	BYTE byTarget;				/**< Ŀ�����ͣ�enumMapDataType */
	stNpcPosition pos;
};

/// ����������
const BYTE BACKOFF_USERCMD_PARA	= 7;
	struct stBackOffMagicUserCmd : public stMagicUserCmd {
		stBackOffMagicUserCmd()
		{
			byParam = BACKOFF_USERCMD_PARA;
		}

		DWORD dwTempID;				/**< ��������ʱ��� */
		BYTE byType;				/**< Ŀ������ enumMapDataType */
		BYTE byDirect;				/**< ���˵ķ��� */
		DWORD x;
		DWORD y;
	};

/// ����Npc�õ�����
const BYTE OBTAINEXP_USERCMD_PARA = 8;
	struct stObtainExpUserCmd : public stMagicUserCmd{
		stObtainExpUserCmd()
		{
			byParam = OBTAINEXP_USERCMD_PARA;
		}

		DWORD dwTempID;				/**< ����ֵ��Դ��ʱ��� */
		BYTE byType;				/**< ����ֵ��Դ enumMapDataType */
		DWORD dwExp;				/**< �õ��ľ��� */
		QWORD dwUserExp;			/**< ���ﵱǰ����ֵ�ľ��� */
	};

/// Npc����
const BYTE NPCDEATH_USERCMD_PARA = 9;
	struct stNpcDeathUserCmd : public stMagicUserCmd{
		stNpcDeathUserCmd()
		{
			byParam = NPCDEATH_USERCMD_PARA;
		}

		DWORD dwNpcTempID;			/**< Npc��ʱ��� */
	};

/// ����ȼ�����
const BYTE LEVELUP_USERCMD_PARA	= 10;
	struct stLevelUpUserCmd : public stMagicUserCmd{
		stLevelUpUserCmd()
		{
			byParam = LEVELUP_USERCMD_PARA;
		}

		DWORD dwUserTempID;			/**< �û���ʱ��� */
	};

//PKģʽ�л�
enum {
	PKMODE_NORMAL,		//��ƽģʽ
	PKMODE_ENTIRE,		//ȫ��ģʽ
	PKMODE_TEAM,		//���ģʽ
	PKMODE_TONG,		//���ģʽ
	PKMODE_SEPT,		//����ģʽ
	//	PKMODE_SCHOOL,		//ʦ��ģʽ
	PKMODE_COUNTRY,		//����ģʽ
	//	PKMODE_CHALLENGE,	//��սģʽ
	PKMODE_GOODNESS,	//�ƶ�ģʽ
	PKMODE_ALLY,		//��������
	PKMODE_MAX,			//���ֵ
};

const BYTE PKMODE_USERCMD_PARA = 20;
	struct stPKModeUserCmd : public stMagicUserCmd{
		stPKModeUserCmd()
		{
			byParam = PKMODE_USERCMD_PARA;
		}
		BYTE byPKMode;
	};
const BYTE OBJECT_EXP_USERCMD_PARA = 22;
	struct stObjectExpUserCmd : public stMagicUserCmd{
		stObjectExpUserCmd()
		{
			byParam = OBJECT_EXP_USERCMD_PARA;
		}
		DWORD id;	
		DWORD exp;	
	};

enum enmUnStateType{
	UN_STATE_COMBIN,
	UN_STATE_CHANGE_FACE,
	UN_STATE_TEAM_ATTACK_BIRD = USTATE_TEAM_ATTACK_BIRD_FLAG,
	UN_STATE_TEAM_ATTACK_FLOW = USTATE_TEAM_ATTACK_FLOW_CLOUD,
	UN_STATE_TEAM_ATTACK_ONE_DIM = USTATE_TEAM_ATTACK_ONE_DIM,
	UN_STATE_NUM
};
//�������״̬
const BYTE UNCOMBIN_USERCMD_PARA = 23;
	struct stUnCombinUserCmd : public stMagicUserCmd{
		stUnCombinUserCmd()
		{
			byParam = UNCOMBIN_USERCMD_PARA;
		}
		enum enmUnStateType type;
	};

//ȼ�����
const BYTE FIREWORK_USERCMD_PARA = 24;
struct stFireWorkUserCmd : public stMagicUserCmd{
	stFireWorkUserCmd()
	{
		byParam = FIREWORK_USERCMD_PARA;
	}

	QWORD qwObjectTempID;	//������ID
	BYTE byType;		//
	DWORD dwUserID;		//���ȼ����ID
	DWORD dwFireID;		//���ID
};

//ʱ��ͬ����Ϣ
const BYTE TIMESYNC_USERCMD_PARA = 25;
struct stTimeSyncUserCmd : public stMagicUserCmd{
	stTimeSyncUserCmd()
	{
		byParam = TIMESYNC_USERCMD_PARA;
	}

	time_t serverTime;	//������ʱ��
};

//GM command: Switch character obstacle state
const BYTE SWITCH_CHARACTER_OBSTACLE_USERCMD_PARA      = 26;
struct stSwitchCharacterObstacleUserCmd : public stMagicUserCmd{
       stSwitchCharacterObstacleUserCmd()
       {
               byParam = SWITCH_CHARACTER_OBSTACLE_USERCMD_PARA;
       }

       bool bOn;               //true: Use obstacle
};
/// �������ظ�������
const BYTE RT_OTHER_MAGIC_USERCMD_PARA = 27;
struct stRTOtherMagicUserCmd : public stMagicUserCmd{

	stRTOtherMagicUserCmd()
	{
		byParam = RT_OTHER_MAGIC_USERCMD_PARA;
	}

	BYTE byTarget;				/**< Ŀ�����ͣ�enumMapDataType */
	DWORD dwUserTempID;			/**< Ŀ����ʱ��� */
	BYTE bySrc;					/**< ���������ͣ�enumMapDataType */
	DWORD dwSrcTempID;			/**< ��������ʱ��� */
};

//////////////////////////////////////////////////////////////
/// ħ��ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ����ָ��忪ʼ
//////////////////////////////////////////////////////////////
	struct stReliveUserCmd : public stNullUserCmd{
		stReliveUserCmd()
		{
			byCmd = RELIVE_USERCMD;
		}
	};

/// ���û�����ָ��
const BYTE MAINUSERDEATH_RELIVE_USERCMD_PARA = 1;
	struct stMainUserDeathReliveUserCmd : public stReliveUserCmd{
		stMainUserDeathReliveUserCmd()
		{
			byParam = MAINUSERDEATH_RELIVE_USERCMD_PARA;

			dwUserTempID = 0;
			deathType = 0;
		}

		DWORD dwUserTempID;			/**< �û���ʱID */
		DWORD deathType;
	};

/// ���û�����ָ��
const BYTE MAINUSERRELIVE_RELIVE_USERCMD_PARA = 2;
	struct stMainUserReliveReliveUserCmd : public stReliveUserCmd{
		stMainUserReliveReliveUserCmd()
		{
			byParam = MAINUSERRELIVE_RELIVE_USERCMD_PARA;
		}

		DWORD dwUserTempID;			/**< �û���ʱID */

		DWORD x,y;					// ����
	};

//���ʽ
enum
{
	ReliveHome,//�سǸ���
	ReliveMoney,//��Ǯ����
	ReliveSkill,//���ܸ���
	Relive_1_min,//׷���׷�1����
	Relive_5_min,//׷���׷�5����
	Relive_10_min//׷���׷�10����
};
/// ���������������
const BYTE OK_RELIVE_USERCMD_PARA = 3;
	struct stOKReliveUserCmd : public stReliveUserCmd{
		stOKReliveUserCmd()
		{
			byParam = OK_RELIVE_USERCMD_PARA;
		}

		DWORD dwUserTempID;			/**< �û���ʱID */
		BYTE  byType;				/**< ���ʽ */
	};
//////////////////////////////////////////////////////////////
/// ����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// �û��ƶ�ָ��忪ʼ
//////////////////////////////////////////////////////////////
	struct stMoveUserCmd : public stNullUserCmd{
		stMoveUserCmd()
		{
			byCmd = MOVE_USERCMD;
		}
	};

/// �ƶ�����Ķ���
enum {
	_DIR_UP			= 0,	/// ����
	_DIR_UPRIGHT	= 1,	/// ����
	_DIR_RIGHTUP	= 1,	/// ����
	_DIR_RIGHT		= 2,	/// ����
	_DIR_RIGHTDOWN	= 3,	/// ����
	_DIR_DOWNRIGHT	= 3,	/// ����
	_DIR_DOWN		= 4,	/// ����
	_DIR_DOWNLEFT	= 5,	/// ����
	_DIR_LEFTDOWN	= 5,	/// ����
	_DIR_LEFT		= 6,	/// ����
	_DIR_LEFTUP		= 7,	/// ����
	_DIR_UPLEFT		= 7,	/// ����
	_DIR_WRONG		= 8		/// ������
};
/// �û��ƶ�
const BYTE USERMOVE_MOVE_USERCMD_PARA = 2;
	struct stUserMoveMoveUserCmd : public stMoveUserCmd {
		stUserMoveMoveUserCmd()
		{
			byParam = USERMOVE_MOVE_USERCMD_PARA;
		}

		DWORD dwUserTempID;			/**< �û���ʱ��� */

		BYTE byDirect;				/**< �ƶ����� */
		BYTE bySpeed;				/**< �ƶ��ٶ� */
		WORD x;					/**< Ŀ������ */
		WORD y;
	};

/// ���������û����͵�˲��ָ��
const BYTE USERINSTANTJUMP_MOVE_USERCMD_PARA = 10;
	struct stUserInstantJumpMoveUserCmd : public stMoveUserCmd{
		stUserInstantJumpMoveUserCmd()
		{
			byParam = USERINSTANTJUMP_MOVE_USERCMD_PARA;
		}

		DWORD dwUserTempID;			/**< �û���ʱID */

		DWORD dwNextScreenPosition;	/**< �����������λ�� */
		WORD wdNextOffsetX;			/**< �������������ƫ��X */
		WORD wdNextOffsetY;			/**< �������������ƫ��Y */
	};

/// �û������ͼָ������
/// ĿǰֻӦ���ڽ�����̳
const BYTE USERGOTOMAP_MOVE_USERCMD_PARA = 12;
struct stUserGotoMapMoveUserCmd : public stMoveUserCmd
{
	stUserGotoMapMoveUserCmd()
	{
		byParam = USERGOTOMAP_MOVE_USERCMD_PARA;
	}
	WORD IsBackToCity;
	DWORD dwMapType;
	char mapName[MAX_NAMESIZE];
};

/// Npc�ƶ�ָ��
const BYTE NPCMOVE_MOVE_USERCMD_PARA = 20;
	struct stNpcMoveMoveUserCmd : public stMoveUserCmd{
		stNpcMoveMoveUserCmd()
		{
			byParam = NPCMOVE_MOVE_USERCMD_PARA;
		}

		DWORD dwNpcTempID;			/**< Npc��ʱ��� */

		BYTE byDirect;				/**< �ƶ����� */
		BYTE bySpeed;				/**< �ƶ����ٶ� */
		WORD x;					/**< Ŀ������ */
		WORD y;
	};

/// �����ȡ��½�б�
const BYTE REQUESTLANDMASS_MOVE_USERCMD_PARA = 30;
	struct stRequestLandMassMoveUserCmd : public stMoveUserCmd{
		stRequestLandMassMoveUserCmd()
		{
			byParam = REQUESTLANDMASS_MOVE_USERCMD_PARA;
		}
	};

struct t_LandMass {
	BYTE groupName[MAX_NAMESIZE];   /**< ��½���� */
	BYTE countryName[MAX_NAMESIZE]; /**< �������� */
	BYTE cityName[MAX_NAMESIZE];    /**< �������� */
	WORD wdOnlineUser;          /**< ��½�������� */
	BYTE byAvailable;           /**< ��½�Ƿ���� */
	DWORD price;				// ��ת����
};

/// ���������ش�½�б�
const BYTE RESPONSELANDMASS_MOVE_USERCMD_PARA = 31;
	struct stResponseLandMassMoveUserCmd : public stMoveUserCmd{
		stResponseLandMassMoveUserCmd()
		{
			byParam = RESPONSELANDMASS_MOVE_USERCMD_PARA;
		}

		int size;
		//�б�
	};

/// ��ת��½
const BYTE LANDMASSJUMP_MOVE_USERCMD_PARA = 32;
	struct stLandMassJumpMoveUserCmd : public stMoveUserCmd{
		stLandMassJumpMoveUserCmd()
		{
			byParam = LANDMASSJUMP_MOVE_USERCMD_PARA;
		}
	};

/// ��������ָ����������»�����������ǰ״̬�й�
const BYTE SITDOWN_MOVE_USERCMD_PARA = 33;
	struct stSitDownMoveUserCmd : public stMoveUserCmd{
		stSitDownMoveUserCmd()
		{
			byParam = SITDOWN_MOVE_USERCMD_PARA;
		}
	};
//////////////////////////////////////////////////////////////
/// �û��ƶ�ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ѡ��ָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stSelectUserCmd : public stNullUserCmd
{
	stSelectUserCmd()
	{
		byCmd = SELECT_USERCMD;
	}
};

/**
 * \brief ��ɫ״̬��־λ
 *
 */
#define CHARBASE_OK 1		///�Ѿ���½����Ϸ
#define CHARBASE_FORBID 2	///��ɫ����
#define CHARBASE_VIP 4		///�շ��û�(���ֵ)
/// ����ɫ��Ϣ����
const WORD MAX_CHARINFO = 2;
/// ��ɫ��Ϣ
struct SelectUserInfo
{
	DWORD id;						/// ��ɫ���
	char  name[MAX_NAMESIZE+1];		/// ��ɫ����
	WORD type;						/// ��ɫ����
	WORD level;						/// ��ɫ�ȼ�
	DWORD mapid;					/// ��ɫ���ڵ�ͼ���
	char  mapName[MAX_NAMESIZE+1];	/// ��ɫ���ڵ�ͼ����
	WORD country;					///	����ID
	WORD face;
	char  countryName[MAX_NAMESIZE+1];	/// ��������
	DWORD bitmask;					/// ��ɫ����
//	WORD five;						///	����������
//	DWORD unionid;        /// ���ID
};

const BYTE USERINFO_SELECT_USERCMD_PARA = 1;
struct stUserInfoUserCmd : public stSelectUserCmd
{
	stUserInfoUserCmd()
	{
		byParam = USERINFO_SELECT_USERCMD_PARA;
		bzero(charInfo, sizeof(charInfo));
		size = 0;
	}
	SelectUserInfo charInfo[MAX_CHARINFO];
	WORD size;
	BYTE data[0];
};

// �ж�ͷ����Ա�
inline bool IsMaleFace(int face)
{
	return (face & 0x1) == 1;
}

inline bool IsFemaleFace(int face)
{
	return !IsMaleFace(face);
}

inline int getCharTypeByFace(int face)
{
	if(IsMaleFace(face)) return PROFESSION_1;
	return PROFESSION_2;
}

/// ���󴴽��û�����
const BYTE CREATE_SELECT_USERCMD_PARA = 2;
struct stCreateSelectUserCmd : public stSelectUserCmd
{
	stCreateSelectUserCmd()
	{
		byParam = CREATE_SELECT_USERCMD_PARA;
	}

	char strUserName[MAX_NAMESIZE];	/**< �û�����  */
	WORD charType;
	BYTE byHairType;			/**< ͷ������ */
	DWORD byRGB;				/**< ��ɫRGB */
	WORD country;				/**< ����ID */
	WORD five;					/**< ���������� */
};

/// �����½
const BYTE LOGIN_SELECT_USERCMD_PARA = 3;
struct stLoginSelectUserCmd : public stSelectUserCmd
{
	stLoginSelectUserCmd()
	{
		byParam = LOGIN_SELECT_USERCMD_PARA;
	}

	DWORD charNo;
	char jpegPassport[7];				/**< ͼ����֤�� */
};

/// ����ɾ���ʺ�
const BYTE DELETE_SELECT_USERCMD_PARA = 4;
struct stDeleteSelectUserCmd : public stSelectUserCmd
{
	stDeleteSelectUserCmd()
	{
		byParam = DELETE_SELECT_USERCMD_PARA;
		bzero(numPassword,sizeof(numPassword));
	}

	DWORD	charNo;
	char	numPassword[MAX_NUMPASSWORD];
};

/// �������ɫ��
const BYTE CHECKNAME_SELECT_USERCMD_PARA = 5;
struct stCheckNameSelectUserCmd : public stSelectUserCmd
{
	stCheckNameSelectUserCmd()
	{
		byParam = CHECKNAME_SELECT_USERCMD_PARA;
		name[0] = 0;
		err_code = 0;
	}

	char name[MAX_NAMESIZE];	/**< �û�����  */
	BYTE err_code; //����ʱ����Ϣ 0 û�д��� 1 �����ظ� 2 ���ְ������Ϸ�������
};

/// ����ɾ����ɫʧ��
const BYTE RETURN_DELETE_SELECT_USERCMD_PARA = 6;
struct stReturnDeleteSelectUserCmd : public stSelectUserCmd
{
	stReturnDeleteSelectUserCmd()
	{
		byParam = RETURN_DELETE_SELECT_USERCMD_PARA;
	}

	char name[MAX_NAMESIZE];	/**< ��ɫ����  */
	BYTE err_code; //����ʱ����Ϣ 1: ���ǰ��������ɢ������ɾ����ɫ
	               //             2: ����ʦ�����ɢʦ�ź���ɾ����ɫ
	               //             3: �����峤�����ɢ�������ɾ����ɫ
				   //             4: �����쳣��������
};

/// ��������б�
const BYTE REQUEST_COUNTRY_SELECT_USERCMD_PARA = 7;
struct stRequestSelectUserCmd : public stSelectUserCmd
{
	stRequestSelectUserCmd()
	{
		byParam = REQUEST_COUNTRY_SELECT_USERCMD_PARA;
	}

};
/// ����ɾ���ʺ�ʱ�����������
const BYTE DELETE_ERROR_SELECT_USERCMD_PARA = 8;
struct stDeleteErrorSelectUserCmd : public stSelectUserCmd
{
	stDeleteErrorSelectUserCmd()
	{
		byParam = DELETE_ERROR_SELECT_USERCMD_PARA;
	}

};
//////////////////////////////////////////////////////////////
// ѡ��ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// �û�����ָ��忪ʼ
//////////////////////////////////////////////////////////////
#ifndef _PROPERTY_USERCMD_DEFINITION_
#define _PROPERTY_USERCMD_DEFINITION_
	struct stPropertyUserCmd : public stNullUserCmd{
		stPropertyUserCmd()
		{
			byCmd = PROPERTY_USERCMD;
		}
	};

/// ����װ����������
enum {
	EQUIPCELLTYPE_NONE=0,       /// ����װ��
	EQUIPCELLTYPE_HELM=1,       /// ͷ��
	EQUIPCELLTYPE_BODY=2,       /// ��װ
	EQUIPCELLTYPE_HANDR=3,		/// ����
	EQUIPCELLTYPE_HANDL=4,		/// ����
	EQUIPCELLTYPE_NECKLACE=5,	/// ����
	EQUIPCELLTYPE_GLOVES=6,		/// ����,����
	EQUIPCELLTYPE_RING=7,       /// ��ָ
	EQUIPCELLTYPE_BELT=8,       /// ����
	EQUIPCELLTYPE_SHOES=9,		/// Ь��
	EQUIPCELLTYPE_OTHER=10,		/// ������Ʒ,��������
	EQUIPCELLTYPE_PACKAGE=11,		/// װ���İ���
	EQUIPCELLTYPE_MAKE=12,		 /// �ϳɡ���������Ƕ�İ���
	EQUIPCELLTYPE_ADORN=13,       /// װ��Ʒ

};

enum{
	EQUIPCELLTYPE_LEFT,			///���
	EQUIPCELLTYPE_RIGHT,		///�ұ�
};

/// ����װ����������
enum{
	EQUIPACTION_INIT,		/// ��ʼװ��
	EQUIPACTION_MOVE,		/// �ƶ�װ��
	EQUIPACTION_SHATTER,	/// װ����
	EQUIPACTION_OBTAIN,		/// װ�����
	EQUIPACTION_DROP,		/// װ������
	EQUIPACTION_REFRESH,	/// ˢ��װ��
};

/// ������Ʒ��������
enum{
	OBJECTCELLTYPE_NONE,		/// ���Ǹ��ӣ����ڶ��������Ʒ
	OBJECTCELLTYPE_COMMON,		/// ��ͨ��Ʒ����
	OBJECTCELLTYPE_EQUIP,		/// װ��
	OBJECTCELLTYPE_MOUSE,		/// ���
	OBJECTCELLTYPE_TRADE,		/// �Լ��Ľ��׸���
	OBJECTCELLTYPE_OTHERTRADE,	/// �Է��Ľ��׸���
	OBJECTCELLTYPE_BANK,		/// ����
	OBJECTCELLTYPE_SELL,		/// ��
	OBJECTCELLTYPE_STORE,		/// �ֿ�
	OBJECTCELLTYPE_EQUIPSHOW,	/// ���Լ����ŵ�װ��
	OBJECTCELLTYPE_PACKAGE,    /// �����ĸ���
	OBJECTCELLTYPE_MAKE,       /// �ϳɡ���������Ƕ�ĸ���
	OBJECTCELLTYPE_MYSHOP,		/// �Լ�̯λ�ĸ���
	OBJECTCELLTYPE_OTHERSSHOP,	/// ������̯λ�ĸ���
	OBJECTCELLTYPE_MAIL,		/// �ʼ�ϵͳ�ĸ���
	OBJECTCELLTYPE_COUNTRY_SAVEBOX, /// ���҂}��
	OBJECTCELLTYPE_PET,       /// �������
};

/// ����������������
enum {
	MAKECELLTYPE_EQUIP=0,         /// ���ϳɡ���������Ƕ����Ʒ
	MAKECELLTYPE_MATERIAL1=1,       /// ����1
	MAKECELLTYPE_MATERIAL2=2,       /// ����2
	MAKECELLTYPE_MATERIAL3=3,       /// ����3
	MAKECELLTYPE_MATERIAL4=4,       /// ����4
};

/// ����������������
enum {
	UPGRADECELLTYPE_UPER=0,       /// ����������Ʒ
	UPGRADECELLTYPE_JEWEL1=1,       /// ������ʯ����1
	UPGRADECELLTYPE_JEWEL2=2,       /// ������ʯ����2
};

/// �û���������
#define ADDUSEROBJECT_PROPERTY_USERCMD_PARAMETER 1
	struct stAddObjectPropertyUserCmd : public stPropertyUserCmd{
		stAddObjectPropertyUserCmd()
		{
			byParam = ADDUSEROBJECT_PROPERTY_USERCMD_PARAMETER;
		}
		BYTE byActionType;			/**< ��Ʒ�������� */
		t_Object object;			/**< ��Ʒ���� */
	};

/// ɾ����������
#define REMOVEUSEROBJECT_PROPERTY_USERCMD_PARAMETER 2
	struct stRemoveObjectPropertyUserCmd : public stPropertyUserCmd {
		stRemoveObjectPropertyUserCmd()
		{
			byParam = REMOVEUSEROBJECT_PROPERTY_USERCMD_PARAMETER;
		}
		DWORD qwThisID;				/**< ��ƷΨһID */
	};

/// �������ƶ����û���Ʒ
#define SWAPUSEROBJECT_PROPERTY_USERCMD_PARAMETER 3
	struct stSwapObjectPropertyUserCmd : public stPropertyUserCmd{
		stSwapObjectPropertyUserCmd()
		{
			byParam = SWAPUSEROBJECT_PROPERTY_USERCMD_PARAMETER;
		}
		DWORD qwThisID;				/**< ��ƷΨһID */
		stObjectLocation	dst;	/**< ��ƷĿ��λ�� ���Ŀ��λ��������Ʒ���򽻻�*/
	};

/// ������Ʒָ��ͻ����Զ�����Դ��Ʒ��������
#define SPLITUSEROBJECT_PROPERTY_USERCMD_PARAMETER 4
	struct stSplitObjectPropertyUserCmd : public stPropertyUserCmd{
		stSplitObjectPropertyUserCmd()
		{
			byParam = SPLITUSEROBJECT_PROPERTY_USERCMD_PARAMETER;
		}
		DWORD qwThisID;				/**< ��ƷΨһID */
		DWORD qwNewThisID;			/**< ����Ʒ��ΨһID */
		DWORD dwNum;				/**< ���ѵ����� */
		stObjectLocation	dst;	/**< ��ƷĿ��λ��*/
	};

/// �ϲ���Ʒ
#define UNIONUSEROBJECT_PROPERTY_USERCMD_PARAMETER 5
	struct stUnionObjectPropertyUserCmd : public stPropertyUserCmd{
		stUnionObjectPropertyUserCmd()
		{
			byParam = UNIONUSEROBJECT_PROPERTY_USERCMD_PARAMETER;
		}
		DWORD qwSrcThisID;			/**< Դ��ƷΨһID */
		DWORD qwDstThisID;			/**< Ŀ����ƷΨһID */
	};

/// ���ø����е���Ʒ���������ںϲ���Ʒ��
#define REFCOUNTOBJECT_PROPERTY_USERCMD_PARAMETER 6
	struct stRefCountObjectPropertyUserCmd : public stPropertyUserCmd{
		stRefCountObjectPropertyUserCmd()
		{
			byParam = REFCOUNTOBJECT_PROPERTY_USERCMD_PARAMETER;
		}
		DWORD qwThisID;				/**< ��ƷΨһID */
		DWORD dwNum;				/**< ���� */
	};

/// �û�ʹ����Ʒ
#define USEUSEROBJECT_PROPERTY_USERCMD_PARAMETER 7
	struct stUseObjectPropertyUserCmd : public  stPropertyUserCmd{
		stUseObjectPropertyUserCmd()
		{
			byParam = USEUSEROBJECT_PROPERTY_USERCMD_PARAMETER;
		}
		DWORD qwThisID;				/**< ��ƷΨһID */
		DWORD dwNumber;				/**< ʹ�õ����� */
	};

/// �û����ͼ�ϵ���Ʒ
#define PICKUPITEM_PROPERTY_USERCMD_PARA 8
	struct stPickUpItemPropertyUserCmd : public stPropertyUserCmd {
		stPickUpItemPropertyUserCmd()
		{
			byParam = PICKUPITEM_PROPERTY_USERCMD_PARA;
		}

		DWORD x;
		DWORD y;
	};

#define FOUNDITEM_PROPERTY_USERCMD_PARA  9
	struct stFoundItemPropertyUserCmd : public stPropertyUserCmd {
		stFoundItemPropertyUserCmd()
		{
			byParam = FOUNDITEM_PROPERTY_USERCMD_PARA;
		}

		DWORD up_id;	//��������ƷID, ��ƷΨһid
		DWORD count; //���ϸ���
		struct material{
			DWORD gem_id; //����id, ���id
			BYTE gem_level; //���ϵȼ�
			BYTE gem_num; //��������
		} list[0];
	};

#define UPGRADEITEM_PROPERTY_USERCMD_PARA 10
	struct stUpgradeItemPropertyUserCmd : public stPropertyUserCmd {
		stUpgradeItemPropertyUserCmd() 
		{
			byParam = UPGRADEITEM_PROPERTY_USERCMD_PARA;
		}
		BYTE location; //λ�ã�0 ���� 1 ����
	};

#define COMPOSEITEM_PROPERTY_USERCMD_PARA 11
	struct stComposeItemPropertyUserCmd : public stPropertyUserCmd {
		stComposeItemPropertyUserCmd()
		{
			byParam = COMPOSEITEM_PROPERTY_USERCMD_PARA;
		}
		BYTE per_num; //���κϳ�����
		BYTE location; //λ�ã�0 ���� 1 ����
	};

enum enumGameAccelKeyType{
	GameAccelKeyType_None = 0,
	GameAccelKeyType_Item,	// ʹ����Ʒ
	GameAccelKeyType_Skill,	// ����
	GameAccelKeyType_Face,	// ����
	GameAccelKeyType_Num,
};

#define MAIN_ACCESS_KEY_NUM		12
#define MAIN_ACCESS_KEY_GROUP	4
struct stGameAccelKey {
	BYTE szAccelKey[4];
	enumGameAccelKeyType type;
	DWORD dwID;
	BYTE mGroup;
	stGameAccelKey()
	{
		mGroup = MAIN_ACCESS_KEY_GROUP;
	}
};
// �����û���ݼ�
const BYTE ACCELKEY_PROPERTY_USERCMD_PARA = 12;
	struct stAccekKeyPropertyUserCmd : public stPropertyUserCmd{
		stAccekKeyPropertyUserCmd()
		{
			byParam = ACCELKEY_PROPERTY_USERCMD_PARA;
		}
		WORD	accelNum;
		BYTE	activeGroup;
		stGameAccelKey accelKeys[0];
	};
/// ����û���������
#define ADDUSERSKILL_PROPERTY_USERCMD_PARA 13
struct stAddUserSkillPropertyUserCmd : public stPropertyUserCmd
{
	stAddUserSkillPropertyUserCmd()
	{
		byParam = ADDUSERSKILL_PROPERTY_USERCMD_PARA;
	}

	DWORD dwSkillID;			/**< ����ID */
	WORD wdLevel;				/**< ���ܵȼ� */
	WORD wdUpNum;				/**< ���ܵȼ��������� */
	DWORD dwExperience;			/**< ��ǰ���� */
	DWORD dwMaxExperience;		/**< �������� */
};

/// ��������ָ��
#define UPGRADEUSERSKILL_PROPERTY_USERCMD_PARA 14
struct stUpgradeUserSkillPropertyUserCmd : public stPropertyUserCmd
{
	stUpgradeUserSkillPropertyUserCmd()
	{
		byParam = UPGRADEUSERSKILL_PROPERTY_USERCMD_PARA;
	}

	DWORD dwSkillID;			/**< ����ID */
	WORD wdLevel;				/**< ���ܵȼ� */
};

/// ɾ���û�����
#define REMOVEUSERSKILL_PROPERTY_USERCMD_PARAMETER 15
struct stRemoveUserSkillPropertyUserCmd : public stPropertyUserCmd
{
	stRemoveUserSkillPropertyUserCmd()
	{
		byParam = REMOVEUSERSKILL_PROPERTY_USERCMD_PARAMETER;
	}

	DWORD dwSkillID;				/**< �û����ܱ�� */
};

/// ϵͳ����ָ�� 
enum  enumUserSetting          
{       
	USER_SETTING_TEAM = 8,          // ������ӱ�־
	USER_SETTING_TRADE,             // �����ױ�־
	USER_SETTING_AUTOFINDPATH,      // �Զ�Ѱ·
	USER_SETTING_FAMILY,            // �����������־
	USER_SETTING_UNION,             // ����������־

	USER_SETTING_SHOW_HEADPORTRAIT,	//��ʾСͷ��
	USER_SETTING_SHOW_MIMIMAP,		//��ʾС��ͼ
	USER_SETTING_AUTO_KILL_SUMMON,	//�Զ����
	USER_SETTING_SHOW_PLAYERNAME,   //��ʾ�������
	USER_SETTING_SHOW_HELPTIP,      //��ʾ�����̳�
	USER_SETTING_SHOW_ALLMINIMAP,	//С��ͼȫ��ʾ
	USER_SETTING_SHOW_ITEM,			//������ʾ��Ʒ

	USER_SETTING_SHOW_COUNTRYNAME,  //��ʾ��������
	USER_SETTING_SHOW_UNIONNAME,    //��ʾ�������
	USER_SETTING_SHOW_SEPTNAME,     //��ʾ��������
	USER_SETTING_SHOW_OFFICENAME,   //��ʾ��ְ
	USER_SETTING_SHOW_GRACENAME,    //��ʾ�Ĳ�

	USER_SETTING_CHAT_NINE,			//����
	USER_SETTING_CHAT_COUNTRY,		//����
	USER_SETTING_CHAT_AREA,			//����
	USER_SETTING_CHAT_WHISPER,		//����
	USER_SETTING_CHAT_UNION,		//���
	USER_SETTING_CHAT_TEAM,			//���
	USER_SETTING_CHAT_FAMILY,		//����
	USER_SETTING_CHAT_PRIVATE,		//˽��
	USER_SETTING_CHAT_FRIEND,		//����
	USER_SETTING_CHAT_SCHOOL,		//ʦ��
	USER_SETTING_CHAT_WORLD,		//����
	//USER_SETTING_CHAT_ALL,			//ȫ��

	USER_SETTING_HELP_MOVE,                     //�ƶ�
	USER_SETTING_HELP_NPC,                      //NPC
	USER_SETTING_HELP_NPCDIALOG,                //NPC�Ի�
	USER_SETTING_HELP_ATTACK,                   //����
	USER_SETTING_HELP_PK,                       //PK
	USER_SETTING_HELP_CLEARREDNAME,             //��������
	USER_SETTING_HELP_TASK,                     //����
	USER_SETTING_HELP_RIGHTBTN,                 //�Ҽ�ѡ��
	USER_SETTING_HELP_TRADE,                    //����
	USER_SETTING_HELP_TEAM,                     //���
	USER_SETTING_HELP_MEMBERHEAD,               //��Աͷ��
	USER_SETTING_HELP_ACCELKEY,                 //���ʹ����Ʒ
	USER_SETTING_HELP_PACK,                     //����
	USER_SETTING_HELP_EQUIP,                    //���װ����Ʒ
	USER_SETTING_HELP_EXPACK,                   //�������
	USER_SETTING_HELP_SKILL,                    //����
	USER_SETTING_HELP_ADDPROPPOINT,             //�ӵ�����
	USER_SETTING_HELP_STUDY,                    //ѧϰ
	USER_SETTING_HELP_AUTO_FIND_PATH,			//�Զ�Ѱ·
	USER_SETTING_HELP_ITEM_LINK,				//��Ʒ����
	USER_SETTING_HELP_PRIVATE_CHAT,				//���Ļ�
	USER_SETTING_HELP_MATERIAL_COMPOSE,			//ԭ�Ϻϳ�
	USER_SETTING_HELP_SOULSTONEL_COMPOSE,		//���Ǻϳ�
	USER_SETTING_HELP_SOULSTONE_MAKE,			//������Ƕ
	USER_SETTING_HELP_ITEM_UPGRID,				//��������
	USER_SETTING_HELP_ATTACK_MODE_CHANGE,		//����ģʽ�л�
	USER_SETTING_HELP_CHAT,						//����
	USER_SETTING_HELP_SELECT,					//ѡ��Ŀ��
	USER_SETTING_HELP_ALT_SELECT_NPC,			//altѡ��npc

	USER_SETTING_FRIEND,             // �����Ϊ����
	USER_SETTING_SCHOOL,             // �������ʦ�ű�־

	USER_SETTING_MINIGAME,			// ����С��Ϸ
	USER_SETTING_EFFECT			// ������Ч
};

struct stSystemSettings
{
	BYTE bySettings[20];	// ʹ�� set_state ���ñ�־
	DWORD dwChatColor[8];	//��ɫ����
};

#define SYSTEMSETTINGS_PROPERTY_USERCMD_PARA 16
struct stSystemSettingsUserCmd : public stPropertyUserCmd
{
	stSystemSettingsUserCmd()
	{
		byParam = SYSTEMSETTINGS_PROPERTY_USERCMD_PARA;
	}
	stSystemSettings data;
};

enum{
	PROPERTY_CON = 0,	//����
	PROPERTY_STR = 1,	//����
	PROPERTY_DEX = 2,	//����
	PROPERTY_INT = 3,	//����
	PROPERTY_MEN = 4	//����
};

/// �����û����Ե���
#define DISTRIBUTEUPOINT_PROPERTY_USERCMD_PARA 17
struct stDistributePointPropertyUserCmd : public stPropertyUserCmd
{
	stDistributePointPropertyUserCmd()
	{
		byParam = DISTRIBUTEUPOINT_PROPERTY_USERCMD_PARA;
	}
	WORD type;

};

// ���Ӽ��ܵ���
#define ADDSKILLPOINT_PROPERTY_USERCMD 18
struct stAddSkillPointPropertyUserCmd : public stPropertyUserCmd
{
	stAddSkillPointPropertyUserCmd()
	{
		byParam = ADDSKILLPOINT_PROPERTY_USERCMD;
	}
	DWORD dwSkillID;
};

/// ����û����Ե���
#define CLEARPOINT_PROPERTY_USERCMD_PARA 19
struct stClearPointPropertyUserCmd : public stPropertyUserCmd
{
	stClearPointPropertyUserCmd()
	{
		byParam = CLEARPOINT_PROPERTY_USERCMD_PARA;
	}
	DWORD dwItemID;
};

/// ����û����ܵ���
#define CLEARUSERSKILLPOINT_PROPERTY_USERCMD_PARAMETER 20
struct stClearUserSkillPointPropertyUserCmd : public stPropertyUserCmd
{
	stClearUserSkillPointPropertyUserCmd()
	{
		byParam = CLEARUSERSKILLPOINT_PROPERTY_USERCMD_PARAMETER;
	}
	DWORD dwItemID;
};

/*
#define MAKE_ODDS_PROPERTY_USERCMD_PARA 19
struct stMakeOddsPropertyUserCmd : public stPropertyUserCmd
{
	stMakeOddsPropertyUserCmd()
	{
		byParam = MAKE_ODDS_PROPERTY_USERCMD_PARA;
	}
	
	DWORD odds;
};

#define MAKE_SELECT_PROPERTY_USERCMD_PARA 20
struct stMakeSelectPropertyUserCmd : public stPropertyUserCmd
{
	stMakeSelectPropertyUserCmd()
	{
		byParam = MAKE_SELECT_PROPERTY_USERCMD_PARA;
	}
	
	DWORD id; //��Ʒid
};
*/
#define COMPOSE_SOUL_STONE_PROPERTY_USERCMD_PARA 21
struct stComposeSoulStonePropertyUserCmd : public stPropertyUserCmd {
	stComposeSoulStonePropertyUserCmd()
	{
		byParam = COMPOSE_SOUL_STONE_PROPERTY_USERCMD_PARA;
	}
	BYTE location; //λ�ã�0 ���� 1 ����

};

#define ENCHASEL_PROPERTY_USERCMD_PARA 22
struct stEnchasePropertyUserCmd : public stPropertyUserCmd {
	stEnchasePropertyUserCmd()
	{
		byParam = ENCHASEL_PROPERTY_USERCMD_PARA;
	}
	BYTE location; //λ�ã�0 ���� 1 ����
};

#define HOLE_PROPERTY_USERCMD_PARA 23
struct stHolePropertyUserCmd : public stPropertyUserCmd {
	stHolePropertyUserCmd()
	{
		byParam = HOLE_PROPERTY_USERCMD_PARA;
	}
	DWORD up_id; //�����װ��id, ��ƷΨһid
	DWORD gem_id; //��ħʯid����Ʒ���id
	BYTE gem_level; //��ħʯ�ȼ�
	BYTE gem_num; //��ħʯ����
};

enum enumHairStyle
{
	HairStype_1,//�ֿ���
	HairStype_2,//������
	HairStype_3,//������
	HairStype_4,//������
	HairStype_5,//������
	HairStype_6//�߹���
};
enum enumHairColor
{
	HairColor_black = 0x00001900,//��ɫ
	HairColor_drink = 0x00C9004C,//�ƺ�
	HairColor_purple = 0x009300DD//����
};

#define DECOMPOSE_PROPERTY_USERCMD_PARA 24
struct stDecomposePropertyUserCmd : public stPropertyUserCmd {
	stDecomposePropertyUserCmd()
	{
		byParam = DECOMPOSE_PROPERTY_USERCMD_PARA;
	}
	DWORD up_id; //���ֽ�װ��id, ��ƷΨһid
};

#define BODYCOLOR_PROPERTY_USERCMD_PARA 25//�·���ɫ
struct stBodyColorPropertyUserCmd : public stPropertyUserCmd
{
	stBodyColorPropertyUserCmd()
	{
		byParam = BODYCOLOR_PROPERTY_USERCMD_PARA;
	}
	DWORD dwBodyColorCustom;//��ɫ����ɫ
};

#define HAIR_PROPERTY_USERCMD_PARA 26//������
struct stHairPropertyUserCmd : public stPropertyUserCmd
{
	stHairPropertyUserCmd()
	{
		byParam = HAIR_PROPERTY_USERCMD_PARA;
	}
	BYTE byHairType;//ͷ������
	DWORD dwHairColor;//ͷ����ɫֵ
};

#define SELECT_PROPERTY_USERCMD_PARA 27//ѡ��ĳ������
struct stSelectPropertyUserCmd : public stPropertyUserCmd
{
	stSelectPropertyUserCmd()
	{
		byParam = SELECT_PROPERTY_USERCMD_PARA;
	}
	BYTE byType;//enumMapDataType
	DWORD dwTempID;//��ʱ���
	BYTE byOldType;//enumMapDataType
	DWORD dwOldTempID;//��ʱ���
};

#define SELECT_MAINUSER_PROPERTY_USERCMD_PARA 29//����۲�ѡ���˵�����
struct stSelectMainUserPropertyUserCmd : public stPropertyUserCmd
{
	stSelectMainUserPropertyUserCmd()
	{
		byParam = SELECT_MAINUSER_PROPERTY_USERCMD_PARA;
	}
	BYTE byType;//enumMapDataType
	DWORD dwTempID;//��ʱ���
};

enum enumEquipType{
	Equip_head,
	Equip_necklace,
	Equip_body,
	Equip_handL,
	Equip_handR,
	Equip_belt,
	Equip_ringL,
	Equip_ringR,
	Equip_bangleL,
	Equip_bangleR,
	Equip_boots,
	Equip_others1,
	Equip_others2,
	Equip_others3,
	Equip_others4,
	Equip_others5
};
struct EquipedObject
{
	BYTE byWhere;//λ��enumEquipType
	t_Object object;//��Ʒ

};
#define SELECT_RETURN_MAINUSER_PROPERTY_USERCMD_PARA 30//�ش�۲�ѡ���˵�����
struct stSelectReturnMainUserPropertyUserCmd : public stPropertyUserCmd
{
	stSelectReturnMainUserPropertyUserCmd()
	{
		byParam = SELECT_RETURN_MAINUSER_PROPERTY_USERCMD_PARA;
	}
	DWORD dwTempID;//��ʱ���
	t_MainUserData mainuser_data;
	t_MapUserData mapuser_data;
	DWORD dwSize; //��װ����Ʒ����
	EquipedObject object_data[0];
};

//�;öȱ仯
const BYTE DURABIILTY_USERCMD_PARA = 31;
	struct stDurabilityUserCmd : public stPropertyUserCmd{
		stDurabilityUserCmd()
		{
			byParam = DURABIILTY_USERCMD_PARA;
		}
		DWORD dwThisID;	/*װ��ID */
		DWORD dwDur;	/*��ǰ�;ö�*/
		DWORD dwMaxDur;	/*����;ö�*/
	};

#define SELECT_RETURN_STATES_PROPERTY_USERCMD_PARA 32//��ѡ���˵�״̬
struct stSelectReturnStatesPropertyUserCmd : public stPropertyUserCmd
{
	stSelectReturnStatesPropertyUserCmd()
	{
		byParam = SELECT_RETURN_STATES_PROPERTY_USERCMD_PARA;
	}
	BYTE byType;//enumMapDataType
	DWORD dwTempID;//��ʱ���
	DWORD size;//״̬����
	struct
	{
		DWORD state;
		union{
			struct{
				WORD result;//��ֵ
				WORD time;//ʱ��
			};
			DWORD value;
		};
	}states[0];
};

//�������
const BYTE CLEARSKILL_USERCMD_PARA = 33;
	struct stClearSkillUserCmd : public stPropertyUserCmd{
		stClearSkillUserCmd()
		{
			byParam = CLEARSKILL_USERCMD_PARA;
		}
	};

#define FACE_PROPERTY_USERCMD_PARA 34//����ͷ��
struct stFacePropertyUserCmd : public stPropertyUserCmd
{
	stFacePropertyUserCmd()
	{
		byParam = FACE_PROPERTY_USERCMD_PARA;
	}
	DWORD dwFace;//Ŀ��ͷ������
};
#define HONOR_TO_PKVALUE_PROPERTY_USERCMD_PARA 35//������ϴpkֵ
struct stHonorToPkValuePropertyUserCmd : public stPropertyUserCmd
{
	stHonorToPkValuePropertyUserCmd()
	{
		byParam = HONOR_TO_PKVALUE_PROPERTY_USERCMD_PARA;
	}
};
// ʹ��Ѫ�����������ʱ��Ҫ���͵�ͼ��Ϣ��ȥ
const BYTE SCROLL_MAP_PROPERTY_USERCMD_PARA = 36;
	struct stScrollMapPropertyUserCmd : public stPropertyUserCmd{
		stScrollMapPropertyUserCmd()
		{
			byParam = SCROLL_MAP_PROPERTY_USERCMD_PARA;
		}
		DWORD qwThisID;				/**< ��ƷΨһID */
		DWORD	size;
		struct
		{
			char strMapName[MAX_NAMESIZE];
		}mapname[0];
		
	};

/// ����û����Ե���ֻ��5��
#define CLEARPOINT_LIMIT_PROPERTY_USERCMD_PARA 37
struct stClearPointLimitPropertyUserCmd : public stPropertyUserCmd
{
	stClearPointLimitPropertyUserCmd()
	{
		byParam = CLEARPOINT_LIMIT_PROPERTY_USERCMD_PARA;
	}
	QWORD qwThisID;
	BYTE  byProperty;
};

// ������ֵ��ͼ�б�
const BYTE REQUEST_INC_MAP_PROPERTY_USERCMD_PARA = 38;
	struct stRequestIncMapPropertyUserCmd : public stPropertyUserCmd{
		stRequestIncMapPropertyUserCmd()
		{
			byParam = REQUEST_INC_MAP_PROPERTY_USERCMD_PARA;
		}
	};

enum horseState
{
	HORSE_STATE_PUTUP,	//����
	HORSE_STATE_FOLLOW,	//����
	HORSE_STATE_RIDE	//���
};

enum horseSkill
{
	HORSE_SKILL_DEF_UP = 1,	//������ħ��
	HORSE_SKILL_PATK_UP,	//����﹥
	HORSE_SKILL_MATK_UP,	//���ħ��
	HORSE_SKILL_HP_UP,	//�����������
	HORSE_SKILL_MP_UP,	//���ħ������
	HORSE_SKILL_DMG_UP,	//�����˺�
	HORSE_SKILL_DMG_DOWN,	//���������˺�
};

//�������
struct t_HorseData
{
	DWORD id;		//npcid
	char name[MAX_NAMESIZE];//����
	DWORD lv;		//�ȼ�
	WORD str;		//����
	WORD intel;		//����
	WORD dex;		//����
	WORD men;		//����
	WORD con;		//����
	WORD speed;		//�ٶ�
	BYTE pdam;		//������������%
	BYTE pdef;		//�������������%
	BYTE mdam;		//����ħ��������%
	BYTE mdef;		//����ħ��������%
	WORD maxhp;		//���HP
	WORD maxmp;		//���MP
	DWORD callTime;		//�ٻ����
	horseState state;	//���״̬
	WORD poisonRes;		//����
	WORD lullRes;		//�����
	WORD faintRes;		//����ѣ
	WORD chaosRes;		//������
	WORD freezeRes;		//������
	WORD petrifyRes;	//��ʯ��
	WORD blindRes;		//��ʧ��
	WORD slowRes;		//������
	DWORD horseid;		//���id������id��
};

#define SELECT_RETURN_HORSE_PROPERTY_USERCMD_PARA 39//�ش�۲�ѡ���˵�����
struct stSelectReturnHorsePropertyUserCmd : public stPropertyUserCmd
{
	stSelectReturnHorsePropertyUserCmd()
	{
		byParam = SELECT_RETURN_HORSE_PROPERTY_USERCMD_PARA;
	}
	t_HorseData data;
};

#define GO_TRAIN_PROPERTY_USERCMD_PARA 40//�������������ͼ
struct stGoTrainPropertyUserCmd : public stPropertyUserCmd
{
	stGoTrainPropertyUserCmd()
	{
		byParam = GO_TRAIN_PROPERTY_USERCMD_PARA;
	}
	DWORD level;//ȥ�ڼ���
};

/// ��������û���������
#define ADDUSEROBJECT_LIST_PROPERTY_USERCMD_PARAMETER 41
	struct stAddObjectListPropertyUserCmd : public stPropertyUserCmd{
		stAddObjectListPropertyUserCmd()
		{
			byParam = ADDUSEROBJECT_LIST_PROPERTY_USERCMD_PARAMETER;
			num=0;
		}
		WORD num;
		struct
		{
			BYTE byActionType;			/**< ��Ʒ�������� */
			t_Object object;			/**< ��Ʒ���� */
		}list[0];
	};
/*can not be processed by make cmd */#define RESPONSE_PROPERTY_USERCMD_PARA 0x81
struct stResponsePropertyUserCmd : public stPropertyUserCmd
{
	stResponsePropertyUserCmd()
	{
		byParam = RESPONSE_PROPERTY_USERCMD_PARA;
	}
	
	BYTE kind; //���1:�ϳ� 2:���� 3:����
	BYTE status; //��� 0 �ɹ� ��0 ʧ��
};

#endif

//////////////////////////////////////////////////////////////
// �û�����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// �û�����ָ��忪ʼ
//////////////////////////////////////////////////////////////
#ifndef _CHAT_USERCMD_DEFINITION_
#define _CHAT_USERCMD_DEFINITION_

struct stChatUserCmd : public stNullUserCmd
{
	stChatUserCmd()
	{
		byCmd = CHAT_USERCMD;
	}
};

enum enumChatType{
	CHAT_TYPE_PRIVATE  = 1,		/// ˽��Ƶ��
	CHAT_TYPE_NINE,			/// ����Ƶ��
	CHAT_TYPE_TEAM,                 /// ����Ƶ��
	CHAT_TYPE_FRIEND,               /// ����Ƶ��
	CHAT_TYPE_GM,                   /// GM��Ƶ��
	CHAT_TYPE_SYSTEM,               /// ϵͳƵ��
	CHAT_TYPE_UNION,                /// ���Ƶ��
	CHAT_TYPE_POP,                  /// ����ʽϵͳ��ʾ
	CHAT_TYPE_PERSON,               /// ����Ƶ��
	CHAT_TYPE_WHISPER,              ///���Ļ�
	CHAT_TYPE_WHISPERTOME,		///���Ļ�
	CHAT_TYPE_COUNTRY,              /// ����Ƶ��
	CHAT_TYPE_AREA,			///����Ƶ��
	CHAT_TYPE_FAMILY,               /// ����Ƶ��

	CHAT_TYPE_FRIEND_AFFICHE,       /// ���ѹ���
	CHAT_TYPE_UNION_AFFICHE,        /// ��ṫ��
	CHAT_TYPE_OVERMAN_AFFICHE,      /// ʦ�Ź���
	CHAT_TYPE_FAMILY_AFFICHE,       /// ���幫��

	CHAT_TYPE_FRIEND_PRIVATE,       /// ����˽��
	CHAT_TYPE_UNION_PRIVATE,        /// ���˽��
	CHAT_TYPE_OVERMAN_PRIVATE,      /// ʦ��˽��
	CHAT_TYPE_FAMILY_PRIVATE,	/// ����˽��

	CHAT_TYPE_NPC,			///npc˵��

	CHAT_TYPE_EMOTION,		///����
	CHAT_TYPE_SHOPADV,		///��̯���
	CHAT_TYPE_WORLD,		///����Ƶ��
	CHAT_TYPE_OVERMAN,		/// ʦ��Ƶ��
	CHAT_TYPE_AUTO,			/// �Զ��ظ�
	CHAT_TYPE_COUNTRY_PK,	/// ���������PK��Ϣ
	CHAT_TYPE_BLESS_MSG,	/// ����ף����Ϣ
	CHAT_TYPE_COUNTRY_MARRY, /// �����Ϣ�㲥
	CHAT_TYPE_ERROR_GM,	///���͵�GM���ߵľ�����Ϣ
	CHAT_TYPE_MINIGAME	/// ��С��Ϸ����
};


enum enumSysInfoType{
	INFO_TYPE_SYS   =       1,      /// ϵͳ��Ϣ��GM��Ϣ�������촰��
	INFO_TYPE_GAME,         /// ��Ϸ��Ϣ����Ļ����
	INFO_TYPE_STATE,                /// ״̬ת������Ļ����
	INFO_TYPE_FAIL,                 /// ʧ����Ϣ����Ļ����
	INFO_TYPE_EXP,          /// ������Ϣ,��þ��顢��Ʒ��������ͷ��
	INFO_TYPE_MSG,          /// �����û�ȷ�Ͽ��ϵͳ��Ϣ
	INFO_TYPE_KING,         /// ����������������Ϣ
	INFO_TYPE_CASTELLAN,    /// ����������������Ϣ
	INFO_TYPE_EMPEROR,	/// �ʵ۷�����������Ϣ
	INFO_TYPE_SCROLL,       /// ��Ļ�Ϸ�������ϵͳ��Ϣ
	INFO_TYPE_ADDFRIEND,    /// �Ӻ���������ʾ�Ͷ�������
	INFO_TYPE_BREAKFRIEND,  /// ���۶���������ʾ�Ͷ�������
	INFO_TYPE_SKYROCKET1,	/// �������1
	INFO_TYPE_SKYROCKET2,	/// �������2
	INFO_TYPE_ATT_FLAG,	/// ���ӱ�����
};

#define ALL_CHAT_USERCMD_PARAMETER 1
struct stTradeObject
{
	DWORD dwObjectID;
	DWORD dwObjectTempID;
	char strName[MAX_NAMESIZE];
	BYTE upgrade;                         // �ȼ�
	BYTE kind;      //��Ʒ����, 0��ͨ, 1��ɫ, 2��ɫ, 4��ʥ, 8��װ
};

///�ͻ��˷��͵�����������Ϣ
struct  stChannelChatUserCmd: public stChatUserCmd
{
	DWORD dwType;			/**< ��Ϣ���� */
	DWORD dwSysInfoType;		/**< ϵͳ��Ϣ������ */ //��������
	DWORD dwCharType;		/**< ��ɫ���� */
	DWORD dwChannelID;
	DWORD dwFromID;			///�����ߵ�ID,����ʱid
	DWORD dwChatTime;			// ̸��ʱ��
	BYTE size;				// ��Ʒ����
	char pstrName[MAX_NAMESIZE];	/**< ���� */ 
	char pstrChat[MAX_CHATINFO];	/**< �Ի� */
	stTradeObject tobject_array[0]; // ������Ʒ�б�
	stChannelChatUserCmd()
	{
		bzero(pstrChat,sizeof(pstrChat));
		bzero(pstrName,sizeof(pstrName));
		byParam = ALL_CHAT_USERCMD_PARAMETER;
		dwChannelID=0;
		dwSysInfoType =1;
		dwCharType = 1;
		size = 0;
	}
};
struct  stServerChannelChatUserCmd: public stChatUserCmd
{
	DWORD dwType;			/**< ��Ϣ���� */
	DWORD dwSysInfoType;		/**< ϵͳ��Ϣ������ */ //��������
	DWORD dwCharType;		/**< ��ɫ���� */
	DWORD dwChannelID;
	DWORD dwFromID;			///�����ߵ�ID,����ʱid
	DWORD dwChatTime;			// ̸��ʱ��
	BYTE size;				// ��Ʒ����
	char info[0];
	stServerChannelChatUserCmd()
	{
		byParam = ALL_CHAT_USERCMD_PARAMETER;
		dwChannelID=0;
		dwSysInfoType =1;
		dwCharType = 1;
		size = 0;
	}
};

// ��ɢ����
#define REMOVE_TEAM_USERCMD_PARA 2
struct stRemoveTeamUserCmd : public stChatUserCmd
{
	stRemoveTeamUserCmd()
	{
		byParam = REMOVE_TEAM_USERCMD_PARA;
	}
};

// ��Ӷ�Ա��ˢ�¶�Ա���� ָ��
#define ADD_TEAMMEMBER_USERCMD_PARA 3
struct stTeamMemberData
{
	char pstrName[MAX_NAMESIZE];	// ��Ա����
	BYTE byHead;					// �Ƿ��Ƕӳ�
	DWORD dwTempID;					//��Աtempid
	DWORD dwMaxHealth;				// HP
	DWORD dwHealth;
	DWORD dwMaxMp;					//MP
	DWORD dwMp;
	WORD wdFace;					//Ф��
};
struct stAddTeamMemberUserCmd: public stChatUserCmd
{
	stAddTeamMemberUserCmd()
	{
		byParam = ADD_TEAMMEMBER_USERCMD_PARA;
	}
	DWORD dwTeamID;					// ����ID
	stTeamMemberData data;
};

// ɾ����Աָ��
#define REMOVE_TEAMMEMBER_USERCMD_PARA 4
struct stRemoveTeamMemberUserCmd: public stChatUserCmd
{
	stRemoveTeamMemberUserCmd()
	{
		byParam = REMOVE_TEAMMEMBER_USERCMD_PARA;
	}
	DWORD dwTeamID;					// ����ID
	//char pstrName[MAX_NAMESIZE];	// ��Ա����
	DWORD dwTempID;					//��Աtempid
};

/**
 * ����ģʽ
 */
enum
{
	TEAM_NORMAL,//��ͨģʽ
	TEAM_HONOR,//����ģʽ
	TEAM_MAX,//��Чģʽ
};
// �������ָ��, ��ָ����Ҫ������������
 #define REQUEST_TEAM_USERCMD_PARA 5
 struct stRequestTeamUserCmd: public stChatUserCmd
{
	stRequestTeamUserCmd()
	{
		byParam = REQUEST_TEAM_USERCMD_PARA;
	}
	DWORD dwAnswerUserID;		// �ش���ID,���ظ��ͻ�����������ID
	DWORD dwTeamMode;			// ����ģʽ
};

// �ش����ָ��
#define ANSWER_TEAM_USERCMD_PARA 6
struct stAnswerTeamUserCmd: public stChatUserCmd
{
	stAnswerTeamUserCmd()
	{
		byParam = ANSWER_TEAM_USERCMD_PARA;
	}
	DWORD dwRequestUserID;		// ������ID
	DWORD dwAnswerUserID;		// �ش���ID
	BYTE  byAgree;				// �Ƿ�ͬ��
	DWORD dwTeamMode;			// ����ģʽ
};

#define CREATE_CHANNEL_USERCMD_PARAMETER 7
struct  stCreateChannelUserCmd: public stChatUserCmd
{
	DWORD dwChannelID;
	DWORD dwClientID;
	char name[MAX_NAMESIZE];
	char name2[MAX_NAMESIZE];

	stCreateChannelUserCmd()
	{
		byParam = CREATE_CHANNEL_USERCMD_PARAMETER;
	}
};

#define INVITE_CHANNEL_USERCMD_PARAMETER 8
struct  stInvite_ChannelUserCmd: public stChatUserCmd
{
	DWORD dwChannelID;
	char name[MAX_NAMESIZE];
	DWORD   dwCharType;

	stInvite_ChannelUserCmd()
	{
		byParam = INVITE_CHANNEL_USERCMD_PARAMETER;
	}
};

#define JOIN_CHANNEL_USERCMD_PARAMETER 9
struct  stJoin_ChannelUserCmd: public stChatUserCmd
{
	DWORD dwChannelID;
	char name[MAX_NAMESIZE];

	stJoin_ChannelUserCmd()
	{
		byParam = JOIN_CHANNEL_USERCMD_PARAMETER;
	}
};

#define LEAVE_CHANNEL_USERCMD_PARAMETER 10
struct  stLeave_ChannelUserCmd: public stChatUserCmd
{
	DWORD dwChannelID;
	char name[MAX_NAMESIZE];

	stLeave_ChannelUserCmd()
	{
		byParam = LEAVE_CHANNEL_USERCMD_PARAMETER;
	}
};

#define REQUEST_TEAMMEMBER_POSITION_USERCMD_PARA 11

// �����Աλ��
struct stRequestTeamMemberPosition : public stChatUserCmd
{
	stRequestTeamMemberPosition()
	{
		byParam = REQUEST_TEAMMEMBER_POSITION_USERCMD_PARA;
	}
};
// ���������ض�Աλ��
#define RETURN_TEAMMEMBER_POSITION_USERCMD_PARA 11
struct stTeamMemberPosition
{
	char szName[MAX_NAMESIZE];
	char szMapName[MAX_NAMESIZE];
	int x,y;
};
struct stReturnTeamMemberPosition : public stChatUserCmd
{
	stReturnTeamMemberPosition()
	{
		byParam = RETURN_TEAMMEMBER_POSITION_USERCMD_PARA;
	}
	DWORD size;
	stTeamMemberPosition memberPos[0];
};

#define QUESTION_OBJECT_USERCMD_PARA 12
struct stQuestionObject : public stChatUserCmd
{
	stQuestionObject()
	{
		byParam = QUESTION_OBJECT_USERCMD_PARA;
	}
	
	char name[MAX_NAMESIZE];	/**< ��Ʒӵ���ߵ����� */
	DWORD dwObjectTempID;          /// ��Ʒ��ʱID
};

#define RETURN_QUESTION_OBJECT_USERCMD_PARA 13
struct stReturnQuestionObject : public stChatUserCmd
{
	stReturnQuestionObject()
	{
		byParam = RETURN_QUESTION_OBJECT_USERCMD_PARA; 
	}
	
	char name[MAX_NAMESIZE];	/**< ��Ʒӵ���ߵ����� */
	t_Object object;               /// ��Ʒ����
};

// ��Ա������Ϣ                
#define TEAMMEMBER_OFFLINE_USERCMD_PARA 14
struct stTeamMemberOfflineUserCmd : public stChatUserCmd
{                                       
        stTeamMemberOfflineUserCmd()
        {               
                byParam = TEAMMEMBER_OFFLINE_USERCMD_PARA; 
        }       
        DWORD dwTempID;                                 //��Աtempid
};

// �����Զ���Ϣ                
#define AUTO_REPLY_USERCMD_PARA 15
struct stAutoReplyUserCmd : public stChatUserCmd
{                                       
        stAutoReplyUserCmd()
        {               
                byParam = AUTO_REPLY_USERCMD_PARA; 
        }
	char text[MAX_CHATINFO];
};

enum
{
	TEAM_EXP_MODE_NORMAL,//��ͨ���侭��ģʽ
	TEAM_EXP_MODE_SPECIAL,//������侭��ģʽ
};
enum
{
	TEAM_OBJ_MODE_NORMAL,//��ͨ��Ʒ����ģʽ
	TEAM_OBJ_MODE_SPECIAL,//������Ʒ����ģʽ
};

// ����(����)���龭�����ģʽ
#define EXPMODE_TEAMMEMBER_USERCMD_PARA 16
struct stExpModeTeamMemberUserCmd: public stChatUserCmd
{
	stExpModeTeamMemberUserCmd()
	{
		byParam = EXPMODE_TEAMMEMBER_USERCMD_PARA;
	}
	BYTE byType;
};
// ����(����)������Ʒ����ģʽ
#define OBJMODE_TEAMMEMBER_USERCMD_PARA 17
struct stObjModeTeamMemberUserCmd: public stChatUserCmd
{
	stObjModeTeamMemberUserCmd()
	{
		byParam = OBJMODE_TEAMMEMBER_USERCMD_PARA;
	}
	BYTE byType;
};
// �������ģʽ
#define TEAM_MODE_USERCMD_PARA 18
struct stTeamModeUserCmd: public stChatUserCmd
{
	stTeamModeUserCmd()
	{
		byParam = TEAM_MODE_USERCMD_PARA;
	}
	BYTE byType;
};
// ˢ�¶�ԱHP,MP ָ��
#define FRESH_TEAMMEMBER_USERCMD_PARA 19
struct stFreshTeamMemberUserCmd: public stChatUserCmd
{
	stFreshTeamMemberUserCmd()
	{
		byParam = FRESH_TEAMMEMBER_USERCMD_PARA;
	}
	DWORD dwTempID;					//��Աtempid
	DWORD dwMaxHealth;				// HP
	DWORD dwHealth;
	/*
	 * ����Ϊ�˷�ֹ8�Լ�����,��DWORD�ĳ�WORD,�������,���Խ����ݸĳ�3�ֽڵ�Ҳ����,������������
	 */
	WORD dwMaxMp;					//MP
	WORD dwMp;
};
#endif

// ��⵽�Ƿ��û�
#define DIRTY_USER_USERCMD_PARA 20
enum DIRTYUSER_TYPE{
		DIRTYUSER_DLL_CALL = 1,	// �ⲿDLL���ñ����̺�����һ������û�
		DIRTYUSER_DEBUGGER = 2,	// ���õ��������Ա����̣����������������
};
struct stDirtyUserUserCmd : public stChatUserCmd
{
		stDirtyUserUserCmd()
		{
				byParam = DIRTY_USER_USERCMD_PARA;
		}
		BYTE type;
};
// �ش����ָ��
#define ANSWER_NAME_TEAM_USERCMD_PARA 21
struct stAnswerNameTeamUserCmd: public stChatUserCmd
{
	stAnswerNameTeamUserCmd()
	{
		byParam = ANSWER_NAME_TEAM_USERCMD_PARA;
	}
	char byRequestUserName[MAX_NAMESIZE];		// ����������
	char byAnswerUserName[MAX_NAMESIZE];		// �ش�������
	BYTE  byAgree;				// �Ƿ�ͬ��
	DWORD dwTeamMode;			// ����ģʽ
};
// �������ָ��, ��ָ����Ҫ������������
 #define REQUEST_NAME_TEAM_USERCMD_PARA 22
 struct stRequestNameTeamUserCmd: public stChatUserCmd
{
	stRequestNameTeamUserCmd()
	{
		byParam = REQUEST_NAME_TEAM_USERCMD_PARA;
	}
	char byAnswerUserName[MAX_NAMESIZE];		// �ش�������,���ظ��ͻ���������������
	DWORD dwTeamMode;			// ����ģʽ
};
 #define REQUEST_COUNTRY_HELP_USERCMD_PARA 23
 struct stRequestCountryHelpUserCmd: public stChatUserCmd
{
	stRequestCountryHelpUserCmd()
	{
		byParam = REQUEST_COUNTRY_HELP_USERCMD_PARA;
	}
	char mapName[MAX_NAMESIZE];		//����֧Ԯ�ĵ�ͼ����
	WORD x;
	WORD y;
};
 #define KILL_FOREIGNER_USERCMD_PARA 24
 struct stKillForeignerUserCmd: public stChatUserCmd
{
	stKillForeignerUserCmd()
	{
		byParam = KILL_FOREIGNER_USERCMD_PARA;
	}
	char heroName[MAX_NAMESIZE];	//Ӣ������
	char mapName[MAX_NAMESIZE];		//������ͼ
};

 // BOSSˢ����ʾ��Ϣ
#define REFRESH_BOSS_USERCMD_PARA 25
 struct stRefreshBossUserCmd: public stChatUserCmd
{
	stRefreshBossUserCmd()
	{
		byParam = REFRESH_BOSS_USERCMD_PARA;
	}
	DWORD npcid;
	BYTE country; //����
	char mapName[MAX_NAMESIZE];		//BOSSˢ�µĵ�ͼ����
	BYTE time;     // ʣ���ˢ��ʱ�䣬���Ϊ0��ʾˢ��
	//WORD x;
	//WORD y;
};

 // BOSS��ɱ��ʾ��Ϣ
#define KILL_BOSS_USERCMD_PARA 26
 struct stKillBossUserCmd: public stChatUserCmd
{
	stKillBossUserCmd()
	{
		byParam = KILL_BOSS_USERCMD_PARA;
	}
	DWORD npcid;
	char name[MAX_NAMESIZE];		//�������
	char mapName[MAX_NAMESIZE];		//BOSSˢ�µĵ�ͼ����
	WORD x;
	WORD y;
};


//////////////////////////////////////////////////////////////
// �û�����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ����ָ��忪ʼ
//////////////////////////////////////////////////////////////
#ifndef _TRADE_USERCMD_DEFINITION_
#define _TRADE_USERCMD_DEFINITION_
struct stTradeUserCmd : public stNullUserCmd
{
	stTradeUserCmd()
	{
		byCmd = TRADE_USERCMD;
	}
};

#define REQUEST_TRADE_USERCMD_PARAMETER 0
struct stRequestTradeUserCmd :public stTradeUserCmd
{
	stRequestTradeUserCmd()
	{
		byParam = REQUEST_TRADE_USERCMD_PARAMETER;
	}
	DWORD dwAskerTempID;	/**< ������ */
	DWORD dwAnswerTempID;	/**< �ش��� */
};

/// ��Ӧ��������
#define ANSWER_TRADE_USERCMD_PARAMETER 1
struct stAnswerTradeUserCmd:public stTradeUserCmd
{
	stAnswerTradeUserCmd()
	{
		byParam = ANSWER_TRADE_USERCMD_PARAMETER;
	}

	BYTE byAgree;				/**< �Ƿ�ͬ�⽻�� */
	DWORD dwAskerTempID;	/**< ������ */
	DWORD dwAnswerTempID;	/**< �ش��� */
};

/// ��ʼ����
#define BEGIN_TRADE_USERCMD_PARAMETER 2
struct stBeginTradeUserCmd:public stTradeUserCmd
{
	stBeginTradeUserCmd()
	{
		byParam = BEGIN_TRADE_USERCMD_PARAMETER;
	}
	DWORD dwAskerTempID;	/**< ������ */
	DWORD dwAnswerTempID;	/**< �ش��� */
};

/// ȷ������
#define COMMIT_TRADE_USERCMD_PARAMETER 5
struct stCommitTradeUserCmd:public stTradeUserCmd
{
	stCommitTradeUserCmd()
	{
		byParam = COMMIT_TRADE_USERCMD_PARAMETER;
	}
	DWORD dwUserTempID;
};

/// ȡ������
#define CANCEL_TRADE_USERCMD_PARAMETER 6
struct stCancelTradeUserCmd:public stTradeUserCmd
{
	stCancelTradeUserCmd()
	{
		byParam = CANCEL_TRADE_USERCMD_PARAMETER;
	}
	DWORD dwUserTempID;
};

/// �������
#define FINISH_TRADE_USERCMD_PARAMETER 7
struct stFinishTradeUserCmd:public stTradeUserCmd
{
	stFinishTradeUserCmd()
	{
		byParam = FINISH_TRADE_USERCMD_PARAMETER;
	}
};

/// ��ӽ�����Ʒ
#define ADD_OBJECT_TRADE_USERCMD_PARAMETER 8
struct stAddObjectTradeUserCmd:public stTradeUserCmd
{
	stAddObjectTradeUserCmd()
	{
		byParam = ADD_OBJECT_TRADE_USERCMD_PARAMETER;
	}
	DWORD user_id; //�û���ʱid
//	DWORD object_id; //��ƷΨһid
	t_Object object; //��Ʒ����
	BYTE x;  //��Ʒ����λ��x����
	BYTE y; //��Ʒ����λ��y����
};

/// ɾ��������Ʒ
#define REMOVE_OBJECT_TRADE_USERCMD_PARAMETER 9
struct stRemoveObjectTradeUserCmd:public stTradeUserCmd
{
	stRemoveObjectTradeUserCmd()
	{
		byParam = REMOVE_OBJECT_TRADE_USERCMD_PARAMETER;
	}
	DWORD user_id; //�û���ʱid
	DWORD object_id; //��ƷΨһid
};

/// ����Npc
#define VISITNPC_TRADE_USERCMD_PARAMETER 10
struct stVisitNpcTradeUserCmd:public stTradeUserCmd
{
	stVisitNpcTradeUserCmd()
	{
		byParam = VISITNPC_TRADE_USERCMD_PARAMETER;
		menuTxt[0] = '\0';
		byReturn = 0;
	}
	DWORD dwNpcTempID;
	BYTE byReturn;
	char menuTxt[1];
};

/// ����Ʒ
#define BUYOBJECT_NPCTRADE_USERCMD_PARAMETER 11
struct stBuyObjectNpcTradeUserCmd:public stTradeUserCmd
{
	stBuyObjectNpcTradeUserCmd()
	{
		byParam = BUYOBJECT_NPCTRADE_USERCMD_PARAMETER;

		dwObjectID = 0;
		dwNum = 1;
		itemLevel = 0;
	}
	DWORD dwObjectID;
	DWORD dwNum;
	BYTE itemLevel;
};

/// ����Ʒ
#define SELLOBJECT_NPCTRADE_USERCMD_PARAMETER 12
struct stSellObjectNpcTradeUserCmd:public stTradeUserCmd
{
	stSellObjectNpcTradeUserCmd()
	{
		byParam = SELLOBJECT_NPCTRADE_USERCMD_PARAMETER;
	}
	DWORD qwThisID;				/**< ��ƷΨһID */
};

/// ������Ʒ
#define REPAIROBJECT_NPCTRADE_USERCMD_PARAMETER 13
struct stRepairObjectNpcTradeUserCmd:public stTradeUserCmd
{
	stRepairObjectNpcTradeUserCmd()
	{
		byParam = REPAIROBJECT_NPCTRADE_USERCMD_PARAMETER;
	}
	DWORD id;  //������Ʒid��ȫ������ʱΪ0
	DWORD gem_id;  //�޸���ʯ��Ψһid����ͨ������0
};

/// ����
#define SELLHORSE_NPCTRADE_USERCMD_PARAMETER 14
struct stSellHorseNpcTradeUserCmd:public stTradeUserCmd
{
	stSellHorseNpcTradeUserCmd()
	{
		byParam = SELLHORSE_NPCTRADE_USERCMD_PARAMETER;
	}

	BYTE action; //action 0:��, action 1: ����
};

/// �ֿ���Ϣ
#define STORE_INFO_NPCTRADE_USERCMD_PARAMETER 15
struct stStoreInfoNpcTradeUserCmd:public stTradeUserCmd
{
	stStoreInfoNpcTradeUserCmd()
	{
		byParam = STORE_INFO_NPCTRADE_USERCMD_PARAMETER;
	}
	
	BYTE page; //ҳ��
	BYTE day[0];	//����
};

/// ��ʼ��̯
#define START_SELL_USERCMD_PARAMETER 16
struct stSellTradeUserCmd:public stTradeUserCmd
{
	stSellTradeUserCmd()
	{
		byParam = START_SELL_USERCMD_PARAMETER;
	}
};

/// ��ɰ�̯
#define FINISH_SELL_USERCMD_PARAMETER 17
struct stFinishSellUserCmd:public stTradeUserCmd
{
	stFinishSellUserCmd()
	{
		byParam = FINISH_SELL_USERCMD_PARAMETER;
	}
};

/// �����Ʒ
#define ADD_OBJECT_SELL_USERCMD_PARAMETER 18
struct stAddObjectSellUserCmd:public stTradeUserCmd
{
	stAddObjectSellUserCmd()
	{
		byParam = ADD_OBJECT_SELL_USERCMD_PARAMETER;
	}
	t_Object object; //��Ʒ����
	DWORD price; //��Ʒ��Ǯ
	BYTE x;  //��Ʒ����λ��x����
	BYTE y; //��Ʒ����λ��y����
};

/// ɾ����Ʒ
#define REMOVE_OBJECT_SELL_USERCMD_PARAMETER 19
struct stRemoveObjectSellUserCmd:public stTradeUserCmd
{
	stRemoveObjectSellUserCmd()
	{
		byParam = REMOVE_OBJECT_SELL_USERCMD_PARAMETER;
	}
	DWORD object_id; //��ƷΨһid
};

/// ����ĳ�˵İ�̯��Ϣ
#define REQUEST_SELL_INFO_USERCMD_PARAMETER 20
struct stRequestSellInfoUserCmd:public stTradeUserCmd
{
	stRequestSellInfoUserCmd()
	{
		byParam = REQUEST_SELL_INFO_USERCMD_PARAMETER;
	}
	DWORD temp_id; //�û���ʱID
};

/// ��������Ʒ
#define REQUEST_SELL_BUY_USERCMD_PARAMETER 21
struct stRequestSellBuyUserCmd:public stTradeUserCmd
{
	stRequestSellBuyUserCmd()
	{
		byParam = REQUEST_SELL_BUY_USERCMD_PARAMETER;
	}
	DWORD temp_id; //�û���ʱID
	DWORD object_id; //��ƷΨһID
};

/// �޸Ĳֿ�����
#define UPDATE_STORE_PASS_USERCMD_PARAMETER 22
struct stUpdateStorePassUserCmd:public stTradeUserCmd
{
	stUpdateStorePassUserCmd()
	{
		byParam = UPDATE_STORE_PASS_USERCMD_PARAMETER;
	}
	char oldpass[8]; //������
	char newpass[8]; //������
};

/// �ܷ��̯
#define CAN_SELL_USERCMD_PARAMETER 23
struct stCanSellTradeUserCmd:public stTradeUserCmd
{
	stCanSellTradeUserCmd()
	{
		byParam = CAN_SELL_USERCMD_PARAMETER;
	}
	BYTE status; // 0 success, 1 fail
};

enum
{
	STORN = 1,//������ʯ
	SIVER =2,//��
	SILK  =3,//˿��
	CRYSTAL =4,//ˮ��
	EBONY  =5,//��ľ
	YINGPI =6,//ӲƤ
};
/// ����ƷƷ����������ʯ
#define GOLD_GIVE_USERCMD_PARAMETER 24
struct stGoldGiveTradeUserCmd:public stTradeUserCmd
{
	stGoldGiveTradeUserCmd()
	{
		byParam = GOLD_GIVE_USERCMD_PARAMETER;
	}
	BYTE type;
};
/// ���������Ʒ
#define REPAIROBJECT_GOLD_NPCTRADE_USERCMD_PARAMETER 25
struct stRepairObjectGoldNpcTradeUserCmd:public stTradeUserCmd
{
	stRepairObjectGoldNpcTradeUserCmd()
	{
		byParam = REPAIROBJECT_GOLD_NPCTRADE_USERCMD_PARAMETER;
	}
	DWORD id;  //������Ʒid��ȫ������ʱΪ0
};
/// ������������ʯ����
#define REQUEST_GOLD_GIVE_USERCMD_PARAMETER 26
struct stRequestGoldGiveTradeUserCmd:public stTradeUserCmd
{
	stRequestGoldGiveTradeUserCmd()
	{
		byParam = REQUEST_GOLD_GIVE_USERCMD_PARAMETER;
	}
};
/// ���ؾ���������ʯ����
#define RETURN_GOLD_GIVE_USERCMD_PARAMETER 27
struct stReturnGoldGiveTradeUserCmd:public stTradeUserCmd
{
	stReturnGoldGiveTradeUserCmd()
	{
		byParam = RETURN_GOLD_GIVE_USERCMD_PARAMETER;
	}
	DWORD Storn_num;//��ʯ����
	DWORD Matarial_num; //��������
	DWORD Card_num;    //���߿�����
};
/// ���������Ʒ
#define ADD_OBJECT_SELL_LIST_USERCMD_PARAMETER 28
struct stAddObjectSellListUserCmd:public stTradeUserCmd
{
	stAddObjectSellListUserCmd()
	{
		byParam = ADD_OBJECT_SELL_LIST_USERCMD_PARAMETER;
		num=0;
	}
	WORD num;
	struct
	{
		t_Object object; //��Ʒ����
		DWORD price; //��Ʒ��Ǯ
		BYTE x;  //��Ʒ����λ��x����
		BYTE y; //��Ʒ����λ��y����
	}list[0];
};
/// �û����������Ʒid�б�
#define REQUEST_ADD_OBJECT_SELL_USERCMD_PARAMETER 29
struct stRequestAddObjectSellUserCmd:public stTradeUserCmd
{
	stRequestAddObjectSellUserCmd()
	{
		byParam = REQUEST_ADD_OBJECT_SELL_USERCMD_PARAMETER;
		num=0;
	}
	WORD num;
	struct
	{
		DWORD qwThisID; //��Ʒid
		DWORD price; //��Ʒ��Ǯ
		BYTE x;  //��Ʒ����λ��x����
		BYTE y; //��Ʒ����λ��y����
	}list[0];
};

/*
/// ����Ʒ����
#define MATARIAL_GIVE_USERCMD_PARAMETER 30
struct stMatarialgiveTradeUserCmd:public stTradeUserCmd
{
        stMatarialgiveTradeUserCmd()
        {
                byParam = MATARIAL_GIVE_USERCMD_PARAMETER;
        }
};

#define REQUEST_MATARIAL_GIVE_USERCMD_PARAMETER 31
struct stRequestMatarialGiveTradeUserCmd:public stTradeUserCmd
{
        stRequestGoldGiveTradeUserCmd()
        {
                byParam = REQUEST_GOLD_GIVE_USERCMD_PARAMETER;
        }
};

#define RETURN_MATARIAl_GIVEL_USERCMD_PARAMETER 32
struct stReturnMatarialGiveTradeUserCmd:public stTradeUserCmd
{
        stReturnGoldGiveTradeUserCmd()
        {
                byParam = RETURN_GOLD_GIVE_USERCMD_PARAMETER;
        }
        DWORD num;//����
};
*/
#endif

//////////////////////////////////////////////////////////////
// ����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ���ָ��忪ʼ
//////////////////////////////////////////////////////////////

struct stUnionUserCmd : public stNullUserCmd
{
	stUnionUserCmd()
	{
		byCmd = UNION_USERCMD;
	}
};

/// ��ᴴ��ǰ���û�״̬�����Ϣ
const BYTE UNION_STATUS_CHECK_PARA = 1;
struct stUnionStatusCheckCmd  : public stUnionUserCmd
{
	stUnionStatusCheckCmd()
	{
		byParam = UNION_STATUS_CHECK_PARA;
	}
};

// ��ᴴ������
const BYTE CREATE_UNION_PARA = 2;
struct stCreateUnionCmd: public stUnionUserCmd
{
	stCreateUnionCmd()
	{
		byParam = CREATE_UNION_PARA;
	}
	char UnionName[MAX_NAMESIZE];  // �������
};

// ��ᴴ���ɹ�ָ֪ͨ��
const BYTE SERVER_RETURN_UNION_CREATE_FAILED = 3;
struct stServerReturnUnionCheckCmd : public stUnionUserCmd
{
	stServerReturnUnionCheckCmd()
	{
		byParam = SERVER_RETURN_UNION_CREATE_FAILED;
	}
};

struct stUnionRember{
	char  memberName[MAX_NAMESIZE];			// ����Ա����
	char  aliasname[MAX_NAMESIZE];			// ��Ա����[ְ������]
	WORD  level;					// ��Ա�ȼ�
	BYTE  byOnline;                      		// ��Ա���߱�־ 0 Ϊ������ ��0Ϊ����
	WORD  occupation;				// ְҵ����
	DWORD exploit;					// ��ѫֵ
	char  septName[MAX_NAMESIZE];			// ��������
	BYTE  bySeptMaster;				// 0�����峤��1���峤
};

// ���������ذ���Ա�б��������ߵĸ��˰����Ϣ
const BYTE ALL_UNION_MEMBER_PARA = 5;
struct stAllUnionMemberCmd : public stUnionUserCmd
{
	stAllUnionMemberCmd()
	{
		byParam = ALL_UNION_MEMBER_PARA;
	}
	WORD size;                     // ����
	stUnionRember memberList[0];   // ��Ա�б�
};


// ����Ȩ�޶���
enum UNIONPOWER {
	CHANGE_ALIAS,              // �ı�ƺ�
	ADD_MEMBER,                // ���ճ�Ա
	FIRE_MEMBER,               // ������Ա
	GENERAL,                   // ��ͨ��Ա
	MASTER,			   // ����Ȩ��
	SECOND,			   // ������Ȩ��
        DARE,			   // �������ܰ��ս
	NOTIFY,			   // ������
};


// ���ذ���Ա�ĸ�����Ϣ
const BYTE RETURN_UNION_MEMBER_INFO_PARA = 6;
struct stReturnUnionMemberInfoCmd : public stUnionUserCmd
{
	stReturnUnionMemberInfoCmd()
	{
		byParam = RETURN_UNION_MEMBER_INFO_PARA;
	}

	char alias[MAX_NAMESIZE];      // ��Ϣ�����ߵı���
	BYTE byPower[2];                    // ��Ϣ�����ߵ�Ȩ����Ϣ
};

enum UNION_ADD_MEMBER{
			QUESTION,          // ��������ĳ�˵�ѯ��
			ANSWER_YES,        // ȷ������
			ANSWER_NO          // ������
};

// �������˼�����
const BYTE ADD_MEMBER_TO_UNION_PARA = 7;  // �����Ϣ�ɿͻ��˷��������������ĳ�ˣ��ɷ���˷����ͻ�����ѯ��ĳ���Ƿ��������
struct stAddMemberToUnionCmd : public stUnionUserCmd
{
	stAddMemberToUnionCmd()
	{
		byParam = ADD_MEMBER_TO_UNION_PARA;
	}
	char 	memberName[MAX_NAMESIZE];   // ���������
	DWORD memberID;                   // ��������ID
	char 	unionName[MAX_NAMESIZE];    // ��������
	BYTE 	byState;                    // �μ� enum UNION_ADD_MEMBER
};

// ��������Ա
const BYTE FIRE_MEMBER_FROM_UNION_PARA = 8;
struct stFireMemberFromUnionCmd : public stUnionUserCmd
{
	stFireMemberFromUnionCmd()
	{
		byParam = FIRE_MEMBER_FROM_UNION_PARA ;
	}
	char memberName[MAX_NAMESIZE];
};

enum{
	UNION_MEMBER_STATUS_ONLINE,			// ����Ա����
	UNION_MEMBER_STATUS_OFFLINE,			// ����Ա����
	UNION_MEMBER_STATUS_FIRE,				// ����Ա������
	UNION_MEMBER_STATUS_NEWMEMBER,			// �����Ӱ���Ա
	UNION_MEMBER_STATUS_ALIASCHANGE		// ��Ա�����ı�
};

// ����˸��°���Ա״̬����Ϣ֪ͨ��Ϣ ״̬���ϱ�
const BYTE BROADCAST_MEMBER_INFO_PARA = 10;
struct stBroadcastMemberInfo : public stUnionUserCmd
{
	stBroadcastMemberInfo()
	{
		byParam = BROADCAST_MEMBER_INFO_PARA;
		level = 0;
		bzero(name, MAX_NAMESIZE);
		bzero(septName, MAX_NAMESIZE);
		bySeptMaster = 0;
	}
	BYTE byStatus;					// ��Ա״̬
	char name[MAX_NAMESIZE];			// ��Ա����
	char aliasname[MAX_NAMESIZE];			// ��Ա����
	WORD wdOccupation;				// ��Աְҵ
	WORD level;					// ��Ա�ȼ�
	DWORD exploit;					// ��ѫֵ
	char  septName[MAX_NAMESIZE];			// ��������
	BYTE bySeptMaster;				// 0,�����峤��1Ϊ�峤
};

// �������˷��Ͱ����Ϣ
const BYTE UNION_BASE_INFO_PARA = 11;
struct stUnionBaseInfoCmd : public stUnionUserCmd
{
	stUnionBaseInfoCmd()
	{
		byParam = UNION_BASE_INFO_PARA;
	}
	char  unionName[MAX_NAMESIZE];         // �������
	char  master[MAX_NAMESIZE];				// ���᳤
	WORD  wdLevel;                       // ��ἶ��
	QWORD qwExp;                         // ��ᾭ��
	char  note[255];						// ������
	DWORD dwMana;				// �������
	DWORD dwAction;				// �ж���
};

// ����Ա�����˻�
const BYTE UNIONMEMBER_LEAVE_UNION_PARA = 12;
struct stUnionMemberLeaveUnionCmd : public stUnionUserCmd
{
	stUnionMemberLeaveUnionCmd()
	{
		byParam = UNIONMEMBER_LEAVE_UNION_PARA;
	}
};

struct stUnionVoteRecord{
	char unionName [MAX_NAMESIZE];
	char master[MAX_NAMESIZE];
	WORD wdVoteNumber;
	char note[255];
};

// ���ͶƱ�б�
const BYTE VOTELIST_UNION_PARA = 13;
struct stVoteListUnionCmd : public stUnionUserCmd
{
	stVoteListUnionCmd()
	{
		byParam = VOTELIST_UNION_PARA;
	}
	char unionName[MAX_NAMESIZE];
	DWORD dwSize;
	BYTE flag; // 0Ϊ��Ϣͷ1Ϊ������Ϣ
	stUnionVoteRecord data[0];
};

// ���ͶƱ��Ϣ
const BYTE VOTE_UNION_PARA = 14;
struct stVoteUnionCmd : public stUnionUserCmd
{
	stVoteUnionCmd()
	{
		byParam = VOTE_UNION_PARA;
	}
	char unionName[MAX_NAMESIZE];
	BYTE bySuccess;   // Ϊ1Ϊ�ɹ� Ϊ0Ϊʧ��
};

// ������ð��˵��
const BYTE NOTE_UNION_PARA = 15;
struct stNoteUnionCmd : public stUnionUserCmd
{
	stNoteUnionCmd()
	{
		byParam = NOTE_UNION_PARA;
	}
	char noteBuf[255];
};

// ����Ա��������
const BYTE CHANGE_UNION_MEMBER_ALIASNAME_PARA = 16;
struct stChangeUnionMemberAliasName : public stUnionUserCmd
{
	stChangeUnionMemberAliasName()
	{
		byParam = CHANGE_UNION_MEMBER_ALIASNAME_PARA;
	}

	char name[MAX_NAMESIZE];             // ��������
	char aliasname[MAX_NAMESIZE];        // ���ڱ���
};

// ��������İ�����
const BYTE ENTER_UNION_CITY_AREA_PARA = 17;
struct stEnterUnionCityArea : public stUnionUserCmd
{
	stEnterUnionCityArea()
	{
		byParam = ENTER_UNION_CITY_AREA_PARA;
	}
};

enum
{
	QUESTION_UNION_MEMBER_POWER, // ������Ȩ��
	RESPOND_UNION_MEMBER_POWER, //  ����ĳ����Ȩ��
	SET_UNION_MEMBER_POWER,	    //  ����ĳ����Ȩ��
};

// ����(����)����Ȩ��
const BYTE UNION_MEMBER_POWER_PARA = 18;
struct stUnionMemberPower : public stUnionUserCmd
{
	stUnionMemberPower()
	{
		byParam = UNION_MEMBER_POWER_PARA;
	}
	
	char name[MAX_NAMESIZE]; // ��������
	BYTE byPower[2]; // Ȩ��
	BYTE byType; // ��Ϣ����
};

enum
{
	QUESTION_CITY_HOLD, // ��ѯ��������
	QUESTION_CITY_DARE, // �����������ս
};

// ��ѯ�ʳ��а����Ϣ
const BYTE QUESTION_UNION_CITY_INFO_PARA = 19;
struct stQuestionUnionCityInfo : public stUnionUserCmd
{
	stQuestionUnionCityInfo()
	{
		byParam = QUESTION_UNION_CITY_INFO_PARA;
	}
	
	BYTE byType;	
};

enum
{
	COUNTRY_UNION_SORT, 	// ����������а�
	WORLD_UNION_SORT,	// ���������а�
};

// ��ѯ�������а񣨸���������
const BYTE REQ_UNION_SORT_PARA = 20;
struct stReqUnionSort : public stUnionUserCmd
{
	stReqUnionSort()
	{
		byParam = REQ_UNION_SORT_PARA;
		byType = COUNTRY_UNION_SORT;
	}
	
	BYTE byType;
};

struct stUnionSortInfo
{
	char  unionName[MAX_NAMESIZE];         // �������
	DWORD dwMana;				// �������
	DWORD dwOrder;				// �������
	DWORD dwCountryID;			// ����ID
};

// ���ذ�����а񣨸���������
const BYTE RTN_UNION_SORT_PARA = 21;
struct stRtnUnionSort : public stUnionUserCmd
{
	stRtnUnionSort()
	{
		byParam = RTN_UNION_SORT_PARA;
		dwSize = 0;
	}
	
	BYTE byType;
	DWORD dwSize;
	stUnionSortInfo data[0];
};

// ����ʽ����
const BYTE CONTRIBUTE_UNION_PARA = 22;
struct stContributeUnion : public stUnionUserCmd
{
	stContributeUnion()
	{
		byParam = CONTRIBUTE_UNION_PARA;
	}
	
	DWORD dwMoney;
};

// ����ʽ��ѯ
const BYTE REQ_UNION_MONEY_PARA = 23;
struct stReqUnionMoney : public stUnionUserCmd
{
	stReqUnionMoney()
	{
		byParam = REQ_UNION_MONEY_PARA;
	}
};

struct stUnionRecord{
	char unionName [MAX_NAMESIZE];
	char master[MAX_NAMESIZE];
	char note[255];
};

// ���ͶƱ�б�
const BYTE LIST_UNION_PARA = 24;
struct stListUnionCmd : public stUnionUserCmd
{
	stListUnionCmd()
	{
		byParam = LIST_UNION_PARA;
	}
	DWORD dwSize;
	BYTE flag; // 0Ϊ��Ϣͷ1Ϊ������Ϣ
	stUnionRecord data[0];
};

// �����������Ϣ
const BYTE REQUEST_JOIN_UNION_PARA = 25;
struct stRequestJoinUnionCmd : public stUnionUserCmd
{
	stRequestJoinUnionCmd()
	{
		byParam = REQUEST_JOIN_UNION_PARA;
	}
	char name[MAX_NAMESIZE]; //������������ʱ����Է��᳤�����֣���������������ʱ������������˵�����
	BYTE status;
};

// ����ռ�����
const BYTE REQUEST_ABJURATION_CITY_PARA = 26;
struct stRequestAbjurationCitynCmd : public stUnionUserCmd
{
	stRequestAbjurationCitynCmd()
	{
		byParam = REQUEST_ABJURATION_CITY_PARA;
	}
};

// ����������
const BYTE REQ_UNION_NAME_PARA = 27;
struct stReqUnionNameCmd : public stUnionUserCmd
{
	stReqUnionNameCmd()
	{
		byParam = REQ_UNION_NAME_PARA;
		dwUnionID = 0;
	}

	DWORD dwUnionID;
};

// ���ذ������
const BYTE RTN_UNION_NAME_PARA = 28;
struct stRtnUnionNameCmd : public stUnionUserCmd
{
	stRtnUnionNameCmd()
	{
		byParam = RTN_UNION_NAME_PARA;
		bzero(name, MAX_NAMESIZE);
		dwUnionID = 0;
	}

	DWORD dwUnionID;
	char name[MAX_NAMESIZE];
};

// �����������б�
const BYTE REQ_UNION_NAME_LIST_PARA = 29;
struct stReqUnionNameLlistCmd : public stUnionUserCmd
{
	stReqUnionNameLlistCmd()
	{
		byParam = REQ_UNION_NAME_LIST_PARA;
		num=0;
	}
	BYTE num;
	DWORD dwUnionID[0];
};

// ���ذ�������б�
const BYTE RTN_UNION_NAME_LIST_PARA = 30;
struct stRtnUnionNameListCmd : public stUnionUserCmd
{
	stRtnUnionNameListCmd()
	{
		byParam = RTN_UNION_NAME_LIST_PARA;
		num=0;
	}
	BYTE num;
	struct
	{
		DWORD dwUnionID;
		char name[MAX_NAMESIZE];
	} list[0];
};

// ����������ͷ
#define APPOINT_CITY_CATCHER_PARA  31
struct stAppointCityCatcherCmd : public stUnionUserCmd
{
	stAppointCityCatcherCmd()
	{
		byParam = APPOINT_CITY_CATCHER_PARA;
		bzero(name, sizeof(name));
	}

	char name[MAX_NAMESIZE];
};

// ������ͷ
#define CANCEL_CITY_CATCHER_PARA  32
struct stCancelCityCatcherCmd : public stUnionUserCmd 
{
	stCancelCityCatcherCmd()
	{
		byParam = CANCEL_CITY_CATCHER_PARA;
	}
};

//////////////////////////////////////////////////////////////
// ���ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ����ָ��忪ʼ
//////////////////////////////////////////////////////////////

struct stCountryUserCmd : public stNullUserCmd
{
	stCountryUserCmd()
	{
		byCmd = COUNTRY_USERCMD;
	}
};

// �������
const BYTE CHANGE_COUNTRY_PARA = 1;
struct stChangeCountryCmd : public stCountryUserCmd
{
    stChangeCountryCmd()
    {
	    byParam = CHANGE_COUNTRY_PARA;
    }

    DWORD dwToCountryID; // ��������ҵ�ID
};

enum 
{
	REQUEST_BATTLEFIELD_COUNTRY_LIST, // ����ս���б�
	REQUEST_DARE_COUNTRY_LIST,	// ����ս���б�
};

// ����ս���б�
const BYTE REQUEST_DARE_COUNTRY_PARA =2;
struct stRequestDareCountryCmd : public stCountryUserCmd
{
	stRequestDareCountryCmd()
	{
		byParam = REQUEST_DARE_COUNTRY_PARA;
	}

	BYTE byType; // 0,����ս���б�1��������ս�б�
};

struct stCountryInfo
{
	DWORD dwID; 	// ����ID
	BYTE  byType;	// 0,��ս����1����������
};

// ����ս���б�
const BYTE RETURN_DARE_COUNTRY_PARA = 3;
struct stReturnDareCountryCmd : public stCountryUserCmd
{
	stReturnDareCountryCmd()
	{
		byParam = RETURN_DARE_COUNTRY_PARA;
		dwSize = 0;
	}
	
	DWORD dwSize; // ս������
	BYTE  byType; // 0,ս���б���ս�б�
	
	stCountryInfo country_list[0]; //ս������ID����
};

// �����͵�ָ��ս��
const BYTE TRANS_DARE_COUNTRY_PARA = 4;
struct stTransDareCountryCmd : public stCountryUserCmd
{
	stTransDareCountryCmd()
	{
		byParam = TRANS_DARE_COUNTRY_PARA;
		dwCountryID = 0;
	}

	DWORD dwCountryID;
};

// �����ս��ս
const BYTE DARE_COUNTRY_FORMAL_PARA = 5;
struct stDareCountryFormalCmd : public stCountryUserCmd
{
	stDareCountryFormalCmd()
	{
		byParam = DARE_COUNTRY_FORMAL_PARA;
	}

	DWORD dwCountryID; // Ҫ��ս�Ĺ���
};

enum
{
	DARE_RECORD_RESULT,	// ��ս�����¼
	DARE_RECORD_STAT,	// ��ս���ͳ��
	DARE_RECORD_PLAN,	// �����ս����
};

// �����¼
const BYTE REQUEST_DARE_RECORD_PARA = 6;
struct stRequestDareRecordCmd : public stCountryUserCmd
{
	stRequestDareRecordCmd()
	{
		byParam = REQUEST_DARE_RECORD_PARA;
	}

	BYTE byType;	// ��ѯ����
};

struct stDareResult
{
	DWORD  dareTime;      // ��սʱ��
	DWORD  attCountry;    // ��ս��ID
	DWORD  defCountry;    // ������ID
	BYTE   byResult;      // 0,��ս��ʤ(���ݶԷ��ʳ�)��1,��ս����(ʱ�䵽�ˣ�ȴδ����)��2,�����������ɹ�(������ս���ʳ�)
	char   attKingName[MAX_NAMESIZE];
	char   defKingName[MAX_NAMESIZE];
};

// ��ս���
const BYTE RETURN_DARE_RECORD_RESULT_PARA = 7;
struct stReturnDareRecordResultCmd : public stCountryUserCmd
{
	stReturnDareRecordResultCmd()
	{
		byParam = RETURN_DARE_RECORD_RESULT_PARA;
		dwSize = 0;
	}
	
	DWORD dwSize;
	stDareResult dare_result[0];
};

struct stDareStat
{
	DWORD dwCountry;	// ����ID
	DWORD dwFormalWin;	// ��ʽ��սʤ������
	DWORD dwFormalFail;	// ��ʽ��սʧ�ܴ���
	DWORD dwAnnoyWin;	// ɧ�Ź�սʤ������
	DWORD dwAnnoyFail;	// ɧ�Ź�սʧ�ܴ���
};
// ��սͳ��
const BYTE RETURN_DARE_RECORD_STAT_PARA =8;
struct stReturnDareRecordStatCmd : public stCountryUserCmd
{
	stReturnDareRecordStatCmd()
	{
		byParam = RETURN_DARE_RECORD_STAT_PARA;
		dwSize = 0;
	}
	
	DWORD dwSize;
	stDareStat dare_stat[0];
};

struct stDarePlan
{
	DWORD planTime;
	DWORD attCountry;	// ��ս��
	DWORD defCountry;	// ������
};

// ��ս�ƻ�
const BYTE RETURN_DARE_RECORD_PLAN_PARA =9;
struct stReturnDareRecordPlanCmd : public stCountryUserCmd
{
	stReturnDareRecordPlanCmd()
	{
		byParam = RETURN_DARE_RECORD_PLAN_PARA;
		dwSize = 0;
	}
	
	DWORD dwSize;
	stDarePlan dare_plan[0];
};

// �ѹ�����Ϊ�޹�����
const BYTE CANCEL_COUNTRY_PARA = 10;
struct stCancelCountryCmd : public stCountryUserCmd
{
    stCancelCountryCmd()
    {
	    byParam = CANCEL_COUNTRY_PARA;
    }
};

// ����������
const BYTE APPLY_COUNTRY_PARA = 11;
struct stApplyCountryCmd : public stCountryUserCmd
{
    stApplyCountryCmd()
    {
	    byParam = APPLY_COUNTRY_PARA;
    }

    DWORD dwToCountryID; // �������Ĺ���
};

/// ˰�ʲ�ѯ
const BYTE TAX_COUNTRY_PARA = 12;
struct stTaxCountryUserCmd:public stCountryUserCmd
{
	stTaxCountryUserCmd()
	{
		byParam = TAX_COUNTRY_PARA;
	}
	DWORD dwCountry;
	BYTE byTax; 
};

/// �����ѯ
const BYTE FISK_COUNTRY_PARA = 13;
struct stFiskCountryUserCmd:public stCountryUserCmd
{
	stFiskCountryUserCmd()
	{
		byParam = FISK_COUNTRY_PARA;
		qwGold = 0;
		qwMaterial = 0;	
		qwStock = 0;
	}
	
	DWORD dwCountry;
	QWORD qwGold; 
	QWORD qwMaterial; // ����
	QWORD qwStock;	  // ԭ��
};

/// ˰������
const BYTE SETTAX_COUNTRY_PARA = 14;
struct stSetTaxCountryUserCmd:public stCountryUserCmd
{
	stSetTaxCountryUserCmd()
	{
		byParam = SETTAX_COUNTRY_PARA;
	}
	DWORD dwCountry;
	BYTE byTax; 
};

struct stCountryStar
{
	DWORD dwCountry;
	DWORD dwStar;
};

/// ���ù����Ǻ�
const BYTE UPDATE_COUNTRY_STAR = 15;
struct stUpdateCountryStarCmd : public stCountryUserCmd
{
	stUpdateCountryStarCmd()
	{
		byParam = UPDATE_COUNTRY_STAR;
		dwSize = 0;
	}
	DWORD dwSize;
	stCountryStar data[0];
};


/// ���Ҿ���
const BYTE CONTRIBUTE_COUNTRY_MATERIAL = 16;
struct stContributeCountryMaterialCmd : public stCountryUserCmd
{
	stContributeCountryMaterialCmd()
	{
		byParam = CONTRIBUTE_COUNTRY_MATERIAL;
		itemID = 0;
	}
	DWORD itemID;
};

/// ��ս״̬����
const BYTE SETCOUNTRY_DARE_PARA = 17;
struct stSetCountryDareUserCmd:public stCountryUserCmd
{       
	stSetCountryDareUserCmd()
	{
		byParam = SETCOUNTRY_DARE_PARA;
		dwCountry = 0;
		byStatus = 0;
		byResult = 0;
	}

	
	DWORD dwCountry; // �жԹ���ID
	BYTE  byStatus; // 1Ϊ�����ս��0Ϊ�˳���ս
	BYTE  byResult; // ��ս�����1Ϊʤ����0Ϊʧ�ܷ�,��byStatusΪ0ʱ�����ֶ�������
};      

/// ���¹�����Ϣ
const BYTE UPDATE_COUNTRY_KING = 18;
struct stUpdateCountryKingUserCmd : public stCountryUserCmd
{
	stUpdateCountryKingUserCmd()
	{
		byParam = UPDATE_COUNTRY_KING;
		bzero(kingName, sizeof(kingName));
		isEmperor = 0;
	}
	char   kingName[MAX_NAMESIZE];
	BYTE   isEmperor; // 0���ǻʵ����ڹ�,1�ǻʵ����ڹ�
};

enum
{
	ANSWER_COUNTRY_DARE_QUESTION,
	ANSWER_COUNTRY_DARE_NO,
	ANSWER_COUNTRY_DARE_YES
};
/// ��սӦ��
const BYTE ANSWER_COUNTRY_DARE_PARA= 19;
struct stAnswerCountryDareUserCmd : public stCountryUserCmd
{
	stAnswerCountryDareUserCmd()
	{
		byParam = ANSWER_COUNTRY_DARE_PARA;
		byStatus = ANSWER_COUNTRY_DARE_QUESTION;
	}
	
	BYTE byStatus;
};

/// ѡ��Ҫ�ڹ�սʱ���͵��߾�����ҵȼ�
const BYTE SELECT_TRANS_LEVEL= 20;
struct stSelectTransLevelUserCmd : public stCountryUserCmd
{
	stSelectTransLevelUserCmd()
	{
		byParam = SELECT_TRANS_LEVEL;
		dwLevel = 0;
	}

	DWORD dwLevel;//���ڵ��ڸõȼ�
};

//��������һ�����
#define FORBID_TALK_COUNTRY_PARA 21
struct stForbidTalkCountryUserCmd : public stCountryUserCmd
{
	stForbidTalkCountryUserCmd()
	{
		byParam = FORBID_TALK_COUNTRY_PARA;
		bzero(name, MAX_NAMESIZE);
	}
	char name[MAX_NAMESIZE];
};

enum
{
	WAIT_TECH_TYPE = 1,      // ͶƱѡ���ĿƼ����ȴ�����ѡ���Ա
	ACTIVE_TECH_TYPE = 2,    // ���ڽ�������
	FINISH_TECH = 3,    // �Ѿ�������������Դ�����Ӧװ��(�����ȼ�����0������״̬�ĿƼ�)
};

// ��ȡָ�����ͿƼ��б�
#define REQ_TECH_PARA 22
struct stReqTechUserCmd : public stCountryUserCmd
{
	stReqTechUserCmd()
	{
		byParam = REQ_TECH_PARA;
		dwType = 0;
	}
	
	DWORD dwType; // 1Ϊ���о��ĿƼ���2�����������ĿƼ�,3������ɵĿƼ�
};

struct stTechItem
{
	DWORD dwOption;
	char  szOptionDesc[MAX_NAMESIZE];
	char  szResearchName[MAX_NAMESIZE];
	DWORD dwLevel;
	DWORD dwProgress;
};

// ����ָ�����ͿƼ��б�
#define RTN_TECH_PARA 23
struct stRtnTechUserCmd : public stCountryUserCmd
{
	stRtnTechUserCmd()
	{
		byParam = RTN_TECH_PARA;
		dwSize = 0;
		dwType = 0;
	}
	
	DWORD dwType;
	DWORD dwSize;
	stTechItem data[0];
};

// ��ȡ��ѡ��Ա�б�
#define REQ_WAIT_OFFICIAL_PARA 24
struct stReqWaitOfficialUserCmd : public stCountryUserCmd
{
	stReqWaitOfficialUserCmd()
	{
		byParam = REQ_WAIT_OFFICIAL_PARA;
	}
};

struct stWaitOfficialItem
{
	DWORD dwCharID;
	char  szName[MAX_NAMESIZE];
};

// ���ش�ѡ��Ա�б�
#define RTN_WAIT_OFFICIAL_PARA 25
struct stRtnWaitOfficialUserCmd : public stCountryUserCmd
{
	stRtnWaitOfficialUserCmd()
	{
		byParam = RTN_WAIT_OFFICIAL_PARA;
		dwSize = 0;
	}
	
	DWORD dwSize;
	stWaitOfficialItem data[0];
};

// ���ÿƼ��о�Ա
#define SET_TECH_SEARCH_PARA 26
struct stSetTechSearchUserCmd : public stCountryUserCmd
{
	stSetTechSearchUserCmd()
	{
		byParam = SET_TECH_SEARCH_PARA;
		dwOption = 0;
		dwCharID = 0;
	}

	DWORD dwOption;
	DWORD dwCharID;
};

// �����Ƽ���ɶ�
#define UP_TECH_DEGREE_PARA 27
struct stUpTechDegreeUserCmd : public stCountryUserCmd
{
	stUpTechDegreeUserCmd()
	{
		byParam = UP_TECH_DEGREE_PARA;
		dwOption = 0;
	}

	DWORD dwOption;
};

enum
{
	QUESTION_CONFIRM_SEARCHER, // ѯ���Ƿ�ͬ���Ϊ�о�Ա
	YES_CONFIRM_SEARCHER,  // ͬ���Ϊ�о�Ա
	NO_CONFIRM_SEARCHER // ��ͬ���Ϊ�о�Ա
};

// ѯ���Ƿ�ͬ���Ϊ�о�Ա
#define CONFIRM_SEARCHER_PARA 28
struct stConfirmSearcherUserCmd : public stCountryUserCmd
{
	stConfirmSearcherUserCmd()
	{
		byParam = CONFIRM_SEARCHER_PARA;
		byStatus = QUESTION_CONFIRM_SEARCHER;
		dwOption = 0;
	}
			
	DWORD dwOption;	// �Ƽ�ID	
	BYTE byStatus;
};

// �ù��˲��ܵ�½
#define  DISENABLEL_OGIN_COUNTRY_PARA  29 
enum  FALSE_COUNTRY_STATUS
{
	FALSE_REGISTER_COUNTRY,  //return when register fail 
	FALSE_LOGIN_COUNTRY     // return when login fail 
};
struct stDisableLoginCountryCmd : public stCountryUserCmd
{
	stDisableLoginCountryCmd()
	{
		byParam = DISENABLEL_OGIN_COUNTRY_PARA;
	}
	FALSE_COUNTRY_STATUS status;
};

// ȡ���Ƽ��о�Ա
#define CANCEL_TECH_SEARCH_PARA 30
struct stCancelTechSearchUserCmd : public stCountryUserCmd
{
	stCancelTechSearchUserCmd()
	{
		byParam = CANCEL_TECH_SEARCH_PARA;
		dwOption = 0;
	}

	DWORD dwOption;
};

// �����ս����
#define ANTI_DARE_COUNTRY_FORMAL_PARA  31
struct stAntiDareCountryFormalCmd : public stCountryUserCmd
{
	stAntiDareCountryFormalCmd()
	{
		byParam = ANTI_DARE_COUNTRY_FORMAL_PARA;
	}

	DWORD dwCountryID; // Ҫ��ս�Ĺ���
};

// �������ҹ���
#define COUNTRY_NOTE_PARA  32
struct stCountryNoteCmd : public stCountryUserCmd
{
	stCountryNoteCmd()
	{
		byParam = COUNTRY_NOTE_PARA;
	}

	char note[255];
};

// ������ҹ���
#define REQ_COUNTRY_NOTE_PARA  33
struct stReqCountryNoteCmd : public stCountryUserCmd
{
	stReqCountryNoteCmd()
	{
		byParam = REQ_COUNTRY_NOTE_PARA;
	}
};

// �����������
#define KING_PUNISH_COUNTRY_PARA  34
struct stKingPunishCountryCmd : public stCountryUserCmd
{
	stKingPunishCountryCmd()
	{
		byParam = KING_PUNISH_COUNTRY_PARA;
	}
	char name[MAX_NAMESIZE];//ָ���������
	DWORD method;//������ʽ 1:���� 2:�ؼ���
};

// �ʵ۴������
#define EMPEROR_PUNISH_COUNTRY_PARA  35
struct stEmperorPunishCountryCmd : public stCountryUserCmd
{
	stEmperorPunishCountryCmd()
	{
		byParam = EMPEROR_PUNISH_COUNTRY_PARA;
	}
	char name[MAX_NAMESIZE];//ָ���������
	DWORD method;//������ʽ 1:���� 2:�ؼ���
};

// ����󽫾�����Ϣ
#define REQ_GEN_COUNTRY_PARA  36
struct stReqGenCountryCmd : public stCountryUserCmd
{
	stReqGenCountryCmd()
	{
		byParam = REQ_GEN_COUNTRY_PARA;
	}
};

// ���ش󽫾�����Ϣ
#define RET_GEN_COUNTRY_PARA  37
struct stRetGenCountryCmd : public stCountryUserCmd
{
	stRetGenCountryCmd()
	{
		byParam = RET_GEN_COUNTRY_PARA;
	}
	DWORD level;
	DWORD exp;
	DWORD maxExp;
};

// ������ͷ
#define APPOINT_CATCHER_PARA  38
struct stAppointCatcherCmd : public stCountryUserCmd
{
	stAppointCatcherCmd()
	{
		byParam = APPOINT_CATCHER_PARA;
		bzero(name, sizeof(name));
	}

	char name[MAX_NAMESIZE];
};

// ������ͷ
#define CANCEL_CATCHER_PARA  39
struct stCancelCatcherCmd : public stCountryUserCmd
{
	stCancelCatcherCmd()
	{
		byParam = CANCEL_CATCHER_PARA;
	}
};

// �����⽻��
#define APPOINT_DIPLOMAT_PARA  40
struct stAppointDiplomatCmd : public stCountryUserCmd
{
	stAppointDiplomatCmd()
	{
		byParam = APPOINT_DIPLOMAT_PARA;
		bzero(name, sizeof(name));
	}

	char name[MAX_NAMESIZE];
};

// �����⽻��
#define CANCEL_DIPLOMAT_PARA  41
struct stCancelDiplomatCmd : public stCountryUserCmd
{
	stCancelDiplomatCmd()
	{
		byParam = CANCEL_DIPLOMAT_PARA;
	}
};

// ��ѯ����ǿ��
#define REQUEST_COUNTRY_POWER_PARA  42
struct stRequestCountryPowerCmd : public stCountryUserCmd
{
	stRequestCountryPowerCmd()
	{
		byParam = REQUEST_COUNTRY_POWER_PARA;
	}
};

// ���ع���ǿ��
#define RETURN_COUNTRY_POWER_PARA  43
struct stReturnCountryPowerCmd : public stCountryUserCmd
{
	stReturnCountryPowerCmd()
	{
		byParam = RETURN_COUNTRY_POWER_PARA;
	}
	BYTE country[2];
};

// ��ȡÿ�ջʵ۵Ľ���
#define REQ_DAILY_EMPEROR_MONEY 44
struct stReqDailyEmperorMoneyCmd : public stCountryUserCmd
{
	stReqDailyEmperorMoneyCmd()
	{
		byParam = REQ_DAILY_EMPEROR_MONEY;
	}
};

// ����������а�
#define REQ_KING_LIST_PARA  45
struct stReqKingListCmd : public stCountryUserCmd
{
	stReqKingListCmd()
	{
		byParam = REQ_KING_LIST_PARA;
	}
};

struct _KingListItem
{
	char  king_name[MAX_NAMESIZE]; // ��������
	WORD  online_time; //����ʱ�� ��λΪСʱ
	DWORD country_id; // ����ID
};

// ���ع������а�
#define RTN_KING_LIST_PARA  46
struct stRtnKingListCmd : public stCountryUserCmd
{
	stRtnKingListCmd()
	{
		byParam = RTN_KING_LIST_PARA;
		size = 0;
	}
	BYTE size;
	_KingListItem data[0];
};
 
// �������ռ����
#define REQ_CITY_OWNER_LIST_PARA  47
struct stReqCityOwnerListCmd : public stCountryUserCmd
{
	stReqCityOwnerListCmd()
	{
		byParam = REQ_CITY_OWNER_LIST_PARA;
	}
};

// ���س���ռ�����б�
#define RTN_CITY_OWNER_LIST_PARA  48
struct stRtnCityOwnerListCmd : public stCountryUserCmd
{
	stRtnCityOwnerListCmd()
	{
		byParam = RTN_CITY_OWNER_LIST_PARA;
		size = 0;
	}
	WORD size;
	struct
	{
		char cityName[MAX_NAMESIZE];
		char unionName[MAX_NAMESIZE];
	}list[0];
};
 
// �������ռ����
#define REQ_NPC_OWNER_LIST_PARA  49
struct stReqNpcOwnerListCmd : public stCountryUserCmd
{
	stReqNpcOwnerListCmd()
	{
		byParam = REQ_NPC_OWNER_LIST_PARA;
	}
};
// ���س���ռ����
#define RTN_NPC_OWNER_LIST_PARA  50
struct stRtnNpcOwnerListCmd : public stCountryUserCmd
{
	stRtnNpcOwnerListCmd()
	{
		byParam = RTN_NPC_OWNER_LIST_PARA;
		size=0;
	}
	WORD size;
	struct
	{
		DWORD npcID;//npc������id
		char mapName[MAX_NAMESIZE];//��ͼ����
		WORD x;		//����λ��
		WORD y;		//����λ��
		char septName[MAX_NAMESIZE];//ռ��ļ�������
	}list[0];
};
//////////////////////////////////////////////////////////////
// ����ָ������
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// ͶƱָ��忪ʼ
//////////////////////////////////////////////////////////////
enum
{
	TECH_VOTE // �Ƽ�ͶƱ
};

struct stVoteUserCmd : public stNullUserCmd
{
    stVoteUserCmd()
    {
        byCmd = VOTE_USERCMD;
    }
};

// ����ͶƱ�б�
#define REQUEST_VOTE_LIST_PARA 1
struct stRequestVoteListUserCmd : public stVoteUserCmd
{
	stRequestVoteListUserCmd()
	{
		byParam = REQUEST_VOTE_LIST_PARA;
		byType = TECH_VOTE;
	}

	BYTE byType; // ͶƱ����
};

struct stVoteItem
{
	DWORD dwOption; // ѡ����
	DWORD dwBallot; // ѡ�ǰͶƱ��
	char  szOptionDesc[MAX_NAMESIZE];
};

// ����ͶƱ�б�
#define RETURN_VOTE_LIST_PARA 2
struct stReturnVoteListUserCmd : public stVoteUserCmd
{
	stReturnVoteListUserCmd()
	{
		byParam = RETURN_VOTE_LIST_PARA;
		dwSize = 0;
		byType = TECH_VOTE;
		dwVoteID = 0;
	}

	DWORD dwVoteID;
	BYTE byType;
	DWORD dwSize;
	stVoteItem data[0];
};

// �ύͶƱ
#define COMMIT_VOTE_PARA 3
struct stCommitVoteUserCmd : public stVoteUserCmd
{
	stCommitVoteUserCmd()
	{
		byParam = COMMIT_VOTE_PARA;
		dwVoteID = 0;
		dwOption = 0;
	}

	DWORD dwVoteID; // �ô�ͶƱ��ID
	DWORD dwOption; // �ô��ύ��ѡ��
};
//////////////////////////////////////////////////////////////
// ͶƱָ������
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// ����ָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stArmyUserCmd : public stNullUserCmd
{
    stArmyUserCmd()
    {
        byCmd = ARMY_USERCMD;
    }
};

// ��������
#define CREATE_ARMY_PARA 1
struct stCreateArmyUserCmd : public stArmyUserCmd
{
	stCreateArmyUserCmd()
	{
		byParam = CREATE_ARMY_PARA;
		bzero(armyName, sizeof(armyName));
		bzero(genName, sizeof(genName));
	}

	char armyName[8+1]; // ���������޶�Ϊ4������
	char genName[MAX_NAMESIZE];
};

// �����ѡ�����б�
#define REQ_WAIT_GEN_PARA 2
struct stReqWaitGenUserCmd : public stArmyUserCmd
{
	stReqWaitGenUserCmd()
	{
		byParam = REQ_WAIT_GEN_PARA;
	}
};

struct stWaitGenItem
{
	DWORD dwCharID;
	char  szName[MAX_NAMESIZE];
};

// ���غ�ѡ�����б�
#define RTN_WAIT_GEN_PARA 3
struct stRtnWaitGenUserCmd : public stArmyUserCmd
{
	stRtnWaitGenUserCmd()
	{
		byParam = REQ_WAIT_GEN_PARA;
		dwSize = 0;
	}

	DWORD dwSize;
	stWaitGenItem data[0];
};

enum
{
	QUESTION_ARMY_GEN, // ѯ���Ƿ�Ը���Ϊ����
	YES_ARMY_GEN, // ͬ���Ϊ����
	NO_ARMY_GEN, // �ܾ���Ϊ����
};

// ѯ���Ƿ�ͬ���Ϊ����
#define REQ_ARMY_GEN_PARA 4
struct stReqArmyGenUserCmd : public stArmyUserCmd
{
	stReqArmyGenUserCmd()
	{
		byParam = REQ_ARMY_GEN_PARA;
		byStatus = QUESTION_ARMY_GEN;
	}

	BYTE byStatus;
	
};

enum
{
	COUNTRY_ARMY_LIST, // ���ڹ��ҶԻ�����ʾȫ�������б�
	CITY_ARMY_LIST,  // ����NPC�Ի�����ʾ���Ǿ����б�
};
// ��������б�
#define REQ_ARMY_LIST_PARA 5
struct stReqArmyListUserCmd : public stArmyUserCmd
{
	stReqArmyListUserCmd()
	{
		byParam = REQ_ARMY_LIST_PARA;
		byType = COUNTRY_ARMY_LIST;
	}

	BYTE byType;
};

struct stArmyBaseInfo
{
	DWORD dwArmyID;
	char  name[MAX_NAMESIZE]; // ��������
	char  cityname[MAX_NAMESIZE]; // ��������
	char  genname[MAX_NAMESIZE]; // ��������
	DWORD dwCapNum; // �ӳ�����
};

// ���ؾ����б�
#define RTN_ARMY_LIST_PARA 6
struct stRtnArmyListUserCmd : public stArmyUserCmd
{
	stRtnArmyListUserCmd()
	{
		byParam = RTN_ARMY_LIST_PARA;
		dwSize = 0;
	}

	DWORD dwSize;
	BYTE  byType;
	stArmyBaseInfo data[0];
};

// ���������ϸ��Ϣ
#define REQ_ARMY_SPEC_PARA 7
struct stReqArmySpecUserCmd : public stArmyUserCmd
{
	stReqArmySpecUserCmd()
	{
		byParam = REQ_ARMY_SPEC_PARA;
		dwArmyID = 0;
	}
	
	DWORD dwArmyID;
};

struct stCaptainBase
{
	char name[MAX_NAMESIZE];
	DWORD dwCharID;
};
// ���ؾ�����ϸ��Ϣ
#define RTN_ARMY_SPEC_PARA 8
struct stRtnArmySpecUserCmd : public stArmyUserCmd
{
	stRtnArmySpecUserCmd()
	{
		byParam = RTN_ARMY_SPEC_PARA;
		dwSize = 0;
		bzero(name, MAX_NAMESIZE);
		bzero(cityname, MAX_NAMESIZE);
		bzero(genname, MAX_NAMESIZE);
	}

	char name[MAX_NAMESIZE]; // ������
	char cityname[MAX_NAMESIZE]; // ������
	char genname[MAX_NAMESIZE]; // ������
	DWORD dwSize;
	stCaptainBase data[0];
};

// �뿪����
#define EXIT_ARMY_PARA 9
struct stExitArmyUserCmd : public stArmyUserCmd
{
	stExitArmyUserCmd()
	{
		byParam = EXIT_ARMY_PARA;
	}
};	

// ��ɢ����
#define REMOVE_ARMY_PARA 10
struct stRemoveArmyUserCmd : public stArmyUserCmd
{
	stRemoveArmyUserCmd()
	{
		byParam = REMOVE_ARMY_PARA;
		dwArmyID = 0;
	}

	DWORD dwArmyID;
};

// ���ľ�������
#define CHANGE_ARMY_NAME_PARA 11
struct stChangeArmyNameUserCmd : public stArmyUserCmd
{
	stChangeArmyNameUserCmd()
	{
		byParam = CHANGE_ARMY_NAME_PARA;
		dwArmyID = 0;
		bzero(newArmyName, MAX_NAMESIZE);
	}

	DWORD dwArmyID;
	char  newArmyName[MAX_NAMESIZE];
};

enum ADD_ARMY_CAPTAIN{
	QUESTION_CAPTAIN, // ����ѯ��
	ANSWER_CAPTAIN_YES, // ȷ������
	ANSWER_CAPTAIN_NO, // �ܾ�����
};
// ���նӳ�
#define ADD_ARMY_CAPTAIN_PARA 12
struct stAddArmyCaptainUserCmd : public stArmyUserCmd
{
	stAddArmyCaptainUserCmd()
	{
		byParam = ADD_ARMY_CAPTAIN_PARA;
		bzero(capName, MAX_NAMESIZE);
		bzero(armyName, MAX_NAMESIZE);
		capID = 0;
		armyID = 0;
		byState = QUESTION_CAPTAIN;
	}

	char capName[MAX_NAMESIZE]; // ���������
	char armyName[MAX_NAMESIZE]; // ����ľ�������
	DWORD armyID; // ����ID
	DWORD capID; // ��������ID
	BYTE byState; // �μ� enum ADD_ARMY_CAPTAIN
};

// �����ӳ�
#define FIRE_ARMY_CAPTAIN_PARA 13
struct stFireArmyCaptainUserCmd : public stArmyUserCmd
{
	stFireArmyCaptainUserCmd()
	{
		byParam = FIRE_ARMY_CAPTAIN_PARA;
		dwUserID = 0;
	}

	DWORD dwUserID; // �������Ķӳ�ID,ͨ��������ϸ��Ϣ���
};

/*// ���ľ��ӽ���
#define CHANGE_ARMY_GEN_PARA 12
struct stChangeArmyGenUserCmd : public stArmyUserCmd
{
	stChangeArmyGenUserCmd()
	{
		byParam = CHANGE_ARMY_GEN_PARA;
		dwArmyID = 0;
		bzero(newArmyGen, MAX_NAMESIZE);
	}

	DWORD dwArmyID;
	char  newArmyGen[MAX_NAMESIZE];
};
*/
//////////////////////////////////////////////////////////////
// ����ָ������
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// ����ָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stGemUserCmd : public stNullUserCmd
{
    stGemUserCmd()
    {
        byCmd = GEM_USERCMD;
    }
};

// ��ȡ����
#define REQUEST_DRAGON_PARA 1
struct stReqDragonUserCmd : public stGemUserCmd
{
	stReqDragonUserCmd()
	{
		byParam = REQUEST_DRAGON_PARA;
	}
};

// ��ȡ����
#define REQUEST_TIGER_PARA 2
struct stReqTigerUserCmd : public stGemUserCmd
{
	stReqTigerUserCmd()
	{
		byParam = REQUEST_TIGER_PARA;
	}
};

//////////////////////////////////////////////////////////////
// ����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ս��ָ��忪ʼ
//////////////////////////////////////////////////////////////

// ��ս���Ͷ���
enum
{
	UNION_DARE	= 0,    // ����ս 
	SCHOOL_DARE	= 1,    // ʦ�Ŷ�ս
	SEPT_DARE	= 2,    // �����ս
	SEPT_NPC_DARE   = 3,    // ����NPC����ս
	UNION_CITY_DARE	= 4,	// �����ս
	COUNTRY_FORMAL_DARE = 5, // ��ʽ��ս
	COUNTRY_FORMAL_ANTI_DARE = 6, // ��ʽ��ս����
	EMPEROR_DARE	= 7,	// �ʳ�����ս
};

struct stDareUserCmd : public stNullUserCmd
{
    stDareUserCmd()
    {
        byCmd = DARE_USERCMD;
    }
};

enum 
{
	DARE_QUESTION,  // �����ѯ����ս
	DARE_YES,       // ������ս
	DARE_NO         // �ܾ���ս
};

enum
{
	DARE_REPUTE_ONE = 1,
	DARE_REPUTE_TWO = 2,
	DARE_REPUTE_THREE = 3,
	DARE_REPUTE_FOUR = 4,
};

// ������ս
const BYTE ACTIVE_DARE_PARA = 1;
struct stActiveDareCmd : public stDareUserCmd
{
    stActiveDareCmd()
    {
	    byParam = ACTIVE_DARE_PARA;
	    dwWarID = 0;
	    dwMsgType = DARE_QUESTION;
	    byDareRepute = 0;
    }
    
    char name[MAX_NAMESIZE]; // ��ս������
    char fromRelationName[MAX_NAMESIZE]; // ��ս�߰���ʦ�Ż��������
    char toRelationName[MAX_NAMESIZE];  //����ս�߰���ʦ�Ż��������
    DWORD dwWarID;    // ��սID
    DWORD dwDareType; // ��ս����: ��ᡢʦ�š�����
    DWORD dwMsgType;  // ��ս�߷�����ս�������塣�������յ����������ΪDARE_QUESTION��ת������Ȩ�޵���Ӧ��ҡ�
                      // �ͻ��˶�������ʺ󣬷���DARE_YES��DARE_NO.�ṹ������Ϣ������
    BYTE byDareRepute; // ��������սʱ���¶���������Ϊ�������ͣ�����������:1,2,3,4 �ֱ��Ӧ1��5��10��20����
};


enum
{
    NOTIFY_NO_EXIST, // ����ʦ�Ż������������
};

// ֪ͨ��Ϣ
const BYTE NOTIFY_DARE_PARA = 2;
struct stNotifyDare : public stDareUserCmd
{
    stNotifyDare()
    {
	    byParam = NOTIFY_DARE_PARA;
    }

    DWORD dwType;       // ս������
    DWORD dwNotify;     // ֪ͨ����
};

// ֪ͨ��սʤ�������ĸı�
const BYTE CHANGE_COUNTRY_STAR_PARA = 3;
struct stChangeCountryStar : public stDareUserCmd
{
	stChangeCountryStar()
	{
		byParam = CHANGE_COUNTRY_STAR_PARA;
	}
	
	DWORD dwStar; // ��ǰʤ������
};

// ������ս�б�
const BYTE QUERY_DARE_LIST_PARA = 4;
struct stQueryDareList : public stDareUserCmd
{
	stQueryDareList()
	{
		byParam = QUERY_DARE_LIST_PARA;
	}
	
	BYTE byType; // UNION_DARE,SEPT_DARE
};

struct stDareList
{
	char name[MAX_NAMESIZE];
};

// ֪ͨ������ս������ϵ�б�
const BYTE SEND_DARE_LIST_PARA = 5;
struct stSendDareList : public stDareUserCmd
{
	stSendDareList()
	{
		byParam = SEND_DARE_LIST_PARA;
	}

	DWORD dwSize;
	BYTE  byType;
	struct stDareList dare_list[0];
};

// ����������ս
const BYTE ACTIVE_UNION_CITY_DARE_PARA = 6;
struct stActiveUnionCityDare : public stDareUserCmd
{
	stActiveUnionCityDare()
	{
		byParam = ACTIVE_UNION_CITY_DARE_PARA;
		toCountryID = 0;
	}

	DWORD toCountryID;
};

// ��ȡ����˰��
const BYTE GET_UNION_CITY_TAX_PARA = 7;
struct stGetUnionCityTax : public stDareUserCmd
{
	stGetUnionCityTax()
	{
		byParam = GET_UNION_CITY_TAX_PARA;
	}
};

// ֪ͨ�ͻ��˽���ʳ�����ս����
const BYTE ENTER_EMPEROR_DARE_ZONE = 8;
struct stEnterEmperorDareZone : public stDareUserCmd
{
	stEnterEmperorDareZone()
	{
		byParam = ENTER_EMPEROR_DARE_ZONE;
		dwDefCountryID = 0;
		state = 0;
	}

	DWORD dwDefCountryID; // �ط�����ID
	BYTE state; // 0,Ϊ�˳�,1,Ϊ����
};

// �û�����,֪ͨ�ͻ���,���ڽ��лʳ�����ս
const BYTE ENTER_EMPEROR_DARE_ACTIVE = 9;
struct stEnterEmperorDareActive : public stDareUserCmd
{
	stEnterEmperorDareActive()
	{
		byParam = ENTER_EMPEROR_DARE_ACTIVE;
	}
};

enum enmCallDutyType
{
	CALL_DUTY_KING,		//����
	CALL_DUTY_UNION,	//����
	CALL_DUTY_SEPT,		//�峤
	CALL_DUTY_NUM
};
enum enmCallDutyPipeline
{
	PIPELINE_CALL = 0,	  //�ٻ�
	PIPELINE_REFUSE,	  //�ܾ�
	PIPELINE_AGREE,		  //ͬ��
	PIPELINE_NUM
};

//�������������峤����
const BYTE DARE_CALL_DUTY = 10;
struct stDareCallDuty : public stDareUserCmd
{
	stDareCallDuty()
	{
		leaderTempID=0;
		byParam = DARE_CALL_DUTY;
		byCallerType = CALL_DUTY_NUM;
		byPipeline = PIPELINE_NUM;
	}
	DWORD	leaderTempID;			//�쵼tempid
	BYTE	byCallerType;			//enmCallDutyType
	BYTE	byPipeline;				//enmCallDutyPipeline
	char	mapName[MAX_NAMESIZE];	//Ŀ���ͼ����
	WORD	x;						//����x
	WORD	y;						//����y
};

///////////////////////////////////////////////////////////////
// ս��ָ������
///////////////////////////////////////////////////////////////


//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ����ϵָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stRelationUserCmd : public stNullUserCmd
{
	stRelationUserCmd()
	{
		byCmd = RELATION_USERCMD;
	}
};

enum {
	RELATION_TYPE_LOVE,			// ���޹�ϵ
	RELATION_TYPE_TEACHER,		// ʦͽ��ϵ
	RELATION_TYPE_FRIEND,		// ���ѹ�ϵ
	RELATION_TYPE_BAD,			// ������
	RELATION_TYPE_OVER,			// ��ϵ�Ѿ��Ͼ�����Ҫ���������ݿ��д�������֪ͨ�����������ߺ�����״̬��ͻ��˷���Ϣ��
	RELATION_TYPE_ENEMY			// ���˹�ϵ
};
struct stRelation
{
	char  name[MAX_NAMESIZE];	//	��ϵ������
	BYTE  type;					//	��ϵ���� 
	WORD  level;				//  ��ϵ����
	WORD  user_level;              // ��ϵ�˵ȼ�
	BYTE  online;             //  ����״̬ 0Ϊ�����ߣ�1Ϊ����
	WORD  occupation;			// ְҵ����
	DWORD exploit;		// ��ѫֵ
	DWORD country;		// ����
	char  unionName[MAX_NAMESIZE];	// �������
};

// ��������ϵ�б�
const BYTE RELATION_SEND_RELATIONLIST_PARA = 1;
struct stSendRelationListCmd : public stRelationUserCmd
{
	WORD size;
	stRelation member[0];
	stSendRelationListCmd()
	{
		byParam = RELATION_SEND_RELATIONLIST_PARA;
	}
};

enum RELATION{
	RELATION_ADD,				// ���״̬
	RELATION_QUESTION,         // ��������ĳ�˵�ѯ��
	RELATION_ANSWER_YES,       // ȷ������
	RELATION_ANSWER_NO,        // ������
	RELATION_ONLINE,			// ����״̬
	RELATION_OFFLINE,			// ����״̬
	RELATION_TYPECHANGE,		// ���͸ı�
	RELATION_REMOVE				// ɾ��״̬
};

// ��ϵ״̬������Լ����뽻��
const BYTE RELATION_STATUS_PARA =3;
struct stRelationStatusCmd : public stRelationUserCmd
{
	char	name[MAX_NAMESIZE];
	BYTE	type;
	WORD	occupation;
	WORD    user_level;

	union {
		WORD level;
		DWORD userid;
	};

	BYTE	byState;			//	�μ� enum RELATION
	DWORD   exploit;			// ��ѫֵ
	DWORD   country;
		
	char  unionName[MAX_NAMESIZE];	// �������
	
	stRelationStatusCmd()
	{
		byParam = RELATION_STATUS_PARA;
		user_level = 0;
		exploit = 0;
		country = 0;
		bzero(unionName, MAX_NAMESIZE);
	}
};


enum{
	MARRY_AHEAD_CHECK,
	MARRY_AHEAD_CHECK_FAIL,
	MARRY_ITEM_CHECK,
	MARRY_NO_ITEM,
	MARRY_ANSWER,
	MARRY_REFUSE,
	MARRY_AGREE,
	MARRY_PAY_MONEY,
	MARRY_PAY_MONEY1,
	MARRY_PAY_MONEY2,
	MARRY_NO_MONEY
};

/// ���ǰ���û�״̬�����Ϣ
const BYTE MARRY_STATUS_CHECK_PARA = 4;
struct stMarryStatusCheckCmd  : public stRelationUserCmd
{
	stMarryStatusCheckCmd()
	{
		byParam = MARRY_STATUS_CHECK_PARA;

	}
	BYTE byStep;
};

/// �����Ϣ
#define UNMARRY_PARA 5
struct stUnmarryCmd  : public stRelationUserCmd
{
	stUnmarryCmd()
	{
		byParam = UNMARRY_PARA;

	}
};

//////////////////////////////////////////////////////////////
// ����ϵָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ʦͽ���ɹ�ϵָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stSchoolUserCmd : public stNullUserCmd
{
	stSchoolUserCmd()
	{
		byCmd = SCHOOL_USERCMD;
	}
};

enum SCHOOL_ADD_MEMBER{
			TEACHER_QUESTION,         // ��������ĳ�˼���ʦ�ŵ�ѯ��
			TEACHER_ANSWER_YES,       // ȷ���������ʦ��
			TEACHER_ANSWER_NO         // ���������ʦ��
};

// �������˼������ɣ��������˼���֪ͨ��Ϣ
const BYTE ADD_MEMBER_TO_SCHOOL_PARA = 1;  // �����Ϣ�ɿͻ��˷��������������ĳ�ˣ��ɷ���˷����ͻ�����ѯ��ĳ���Ƿ��������
struct stAddMemberToSchoolCmd : public stSchoolUserCmd
{
	stAddMemberToSchoolCmd()
	{
		byParam = ADD_MEMBER_TO_SCHOOL_PARA;
		level = 0;
	}
	char 	memberName[MAX_NAMESIZE];			// ���������
	union {
			DWORD	memberID;					// ��������ID
			DWORD	degree;						// ��ͻ��˷��͵�ʱ����߿ͻ����³�Ա������һ���㼶֮��
	};
	union {
			char 	schoolName[MAX_NAMESIZE];	// �������ƣ����û������Ϊ��
			char    prename[MAX_NAMESIZE];		// ����ʦ��
	};
	BYTE 	byState;							// �μ� enum SCHOOL_ADD_MEMBER
	WORD	wdOccupation;						// �³�Աְҵ
	WORD    level;					// �����Ա�ĵȼ�
};

// ��ָ�����˿�����ʦ�Ż�������
const BYTE FIRE_MEMBER_FROM_SCHOOL_PARA = 2;
struct stFireMemberFromSchoolCmd : public stSchoolUserCmd
{
	stFireMemberFromSchoolCmd()
	{
		byParam = FIRE_MEMBER_FROM_SCHOOL_PARA ;
	}
	char memberName[MAX_NAMESIZE];
};

/// ���ɴ���ǰ���û�״̬�����Ϣ
const BYTE SCHOOL_STATUS_CHECK_PARA = 3;
struct stSchoolStatusCheckCmd  : public stSchoolUserCmd
{
	stSchoolStatusCheckCmd()
	{
		byParam = SCHOOL_STATUS_CHECK_PARA;
	}
};

// ���ɴ�������
const BYTE CREATE_SCHOOL_PARA = 4;
struct stCreateSchoolCmd: public stSchoolUserCmd
{
	stCreateSchoolCmd()
	{
		byParam = CREATE_SCHOOL_PARA;
	}
	char SchoolName[MAX_NAMESIZE];
};

// ���ɻ�ʦ�ų�Ա���������ϵ
const BYTE SCHOOLMEMBER_LEAVE_SCHOOL_PARA = 5;
struct stSchoolMemberLeaveSchoolCmd : public stSchoolUserCmd
{
	stSchoolMemberLeaveSchoolCmd()
	{
		byParam = SCHOOLMEMBER_LEAVE_SCHOOL_PARA;
	}
};

enum SCHOOL_PACKAGE_TAG{
	SCHOOL_PACKAGE_BODY=0,	// ��Ϣ����
	SCHOOL_PACKAGE_HEAD=1,	// ��Ϣ��ͷ
	SCHOOL_PACKAGE_TAIL=2	// ��Ϣ��β
};

enum SCHOOL_LAYER {
	SCHOOL_NEWLAYER,		// ��һ�²�
	SCHOOL_NEWLINE,        // ��һ����
	SCHOOL_NONE				// �޶���
};

struct stSchoolMember{
	char name[MAX_NAMESIZE];
	BYTE tag;		// enum SCHOOL_LAYER
	WORD level;             // ��Ա�ȼ�
	BYTE online;	// 0 Ϊ�ٲ�����
	WORD occupation;// ְҵ����
};

struct stTeacherMember{
	stTeacherMember()
	{
		level = 0;
	}
	char name[MAX_NAMESIZE];
	WORD degree;   // �Ѻö�
	BYTE tag;	// enum SCHOOL_MEMBER_TAG
	WORD level;     // ��Ա�ȼ�
	BYTE online;	// 0 Ϊ�ٲ�����
	WORD occupation;// ְҵ����
	DWORD country;          // ����
	char  unionName[MAX_NAMESIZE];  // �������
	char  septName[MAX_NAMESIZE];  // ��������
};

// ������Ϣ�б�
const BYTE SENDLIST_SCHOOL_PARA = 6;
struct stSendListSchoolCmd : public stSchoolUserCmd
{
	stSendListSchoolCmd()
	{
		byParam = SENDLIST_SCHOOL_PARA;
	}
	DWORD dwMasterTempID;
	BYTE byPackageTag; //enum SCHOOL_PACKAGE_TAG
	WORD size;
	stSchoolMember list[0];
};

enum SCHOOL_MEMBER_TAG{
	TEACHER,				// ʦ��
	BIGBROTHER,				// ʦ��
	LITTLEBROTHER,			// ʦ��
	PRENTICE				// ͽ��
	
};

// ʦ����Ϣ�б�
const BYTE SENDMEMBERINFO_SCHOOL_PARA = 7;
struct stSendMemberInfoCmd : public stSchoolUserCmd
{
	stSendMemberInfoCmd()
	{
		byParam = SENDMEMBERINFO_SCHOOL_PARA;
	}
	WORD size; //��С
	stTeacherMember list[0]; // list���Ա��tagʹ�� enum SCHOOL_MEMBER_TAG
};

enum {
	SCHOOL_STATUS_NONE,
	SCHOOL_STATUS_TEACHER,
	SCHOOL_STATUS_SCHOOL
};

// ��ʼ��֪ͨ
const BYTE MEMBER_STATUS_SCHOOL_PARA = 8;
struct stMemberStatusSchool : public stSchoolUserCmd
{
	stMemberStatusSchool()
	{
		byParam = MEMBER_STATUS_SCHOOL_PARA;
	}
	char schoolName[MAX_NAMESIZE]; // ��������
	BYTE byStatus; //	SCHOOL_STATUS_NONE,	SCHOOL_STATUS_TEACHER,	SCHOOL_STATUS_SCHOOL
};

enum {
	SCHOOL_MEMBER_OFFLINE=0,		// ����״̬
	SCHOOL_MEMBER_ONLINE=1			// ����״̬
};

const BYTE SCHOOL_MEMBER_STATUS_PARA =9;
struct stSchoolMemberStatusCmd : public stSchoolUserCmd
{
	char	name[MAX_NAMESIZE];
	BYTE	byState;						//	�μ� SCHOOL_MEMBER_ONLINE SCHOOL_MEMBER_OFFLINE
	WORD    level;							//  ��Ա�ȼ�
	DWORD	country;						//  ����
	char	unionName[MAX_NAMESIZE];		//  �������
	char	septName[MAX_NAMESIZE];			//  ��������
	stSchoolMemberStatusCmd()
	{
		byParam = SCHOOL_MEMBER_STATUS_PARA;
	}
};

#define BULLETIN_MAX_SIZE 1000

enum {
	SCHOOL_BULLETIN_SET,		// ���ù���
	SCHOOL_BULLETIN_GET,		// ���󹫸�����
	SCHOOL_BULLETIN_EDIT		// �༭����
};

const BYTE SCHOOL_BULLETIN__PARA =10;
struct stSchoolBulletinCmd : public stSchoolUserCmd
{
	BYTE byState; // SCHOOL_BULLETIN_SET SCHOOL_BULLETIN_GET
	WORD wdSize;  // ��������0
	char data[0];
	stSchoolBulletinCmd()
	{
		byParam = SCHOOL_BULLETIN__PARA;
	}
};

// ���ɽ�ɢָ��
const BYTE DESTROY_SCHOOL_PARA = 11;
struct stDestroySchoolCmd: public stSchoolUserCmd
{
	stDestroySchoolCmd()
	{
		byParam = DESTROY_SCHOOL_PARA;
	}
};

const BYTE SCHOOL_BULLETIN_NOTIFY_PARA = 12;
struct stSchoolBulletinNotifyCmd : public stSchoolUserCmd
{
	stSchoolBulletinNotifyCmd()
	{
		byParam = SCHOOL_BULLETIN_NOTIFY_PARA;
	}
};

enum
{
	QUERY_SCHOOL_BOUNTY, // ��ѯ
	GET_SCHOOL_BOUNTY,   // ��ȡ
	PUT_SCHOOL_BOUNTY,	// ����
};

// ��Ϊʦ����ѯ����ȡ����
#define REQ_MASTER_BOUNTY_PARA 13
struct stReqMasterBountyUserCmd : public stSchoolUserCmd
{
	stReqMasterBountyUserCmd()
	{
		byParam = REQ_MASTER_BOUNTY_PARA;
	}

	BYTE byState;
};

// ��Ϊͽ�ܲ�ѯ�ͽ��н���
#define REQ_PRENTICE_BOUNTY_PARA 14
struct stReqPrenticeBountyUserCmd : public stSchoolUserCmd
{
	stReqPrenticeBountyUserCmd()
	{
		byParam = REQ_PRENTICE_BOUNTY_PARA;
	}

	BYTE byState;
};

// ����ʦ����ѯ
#define RTN_MASTER_BOUNTY_PARA 15
struct stRtnMasterBountyUserCmd : public stSchoolUserCmd
{
	stRtnMasterBountyUserCmd()
	{
		byParam = RTN_MASTER_BOUNTY_PARA;
		dwBalance = 0;
		dwTotal = 0;
	}

	DWORD dwBalance; // ����ȡ�Ľ��
	DWORD dwTotal;	// �ۼ�����ȡ�Ľ��
};

// ����ͽ�ܲ�ѯ
#define RTN_PRENTICE_BOUNTY_PARA 16
struct stRtnPrenticeBountyUserCmd : public stSchoolUserCmd
{
	stRtnPrenticeBountyUserCmd()
	{
		byParam = RTN_PRENTICE_BOUNTY_PARA;
		dwBalance = 0;
		dwTotal = 0;
		dwLastLevel = 0;
		bzero(name, sizeof(name));
	}
	
	DWORD dwBalance; // �ɽ����Ľ��
	DWORD dwTotal;   // �ۼ�����ȡ�Ľ��
	DWORD dwLastLevel; // �ϴ���ȡʱ�ĵȼ�
	char name[MAX_NAMESIZE]; // ʦ��������
};

//////////////////////////////////////////////////////////////
// ʦͽ���ɹ�ϵָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
/************************************************************
************************����ָ��忪ʼ*********************
*************************************************************/
struct stQuestUserCmd : public stNullUserCmd
{
	stQuestUserCmd()
	{
		byCmd = TASK_USERCMD;	
	}
	DWORD id; //����id
};

static const BYTE QUEST_INFO_PARA = 1;
struct stQuestInfoUserCmd : public stQuestUserCmd
{
	stQuestInfoUserCmd()
	{
		byParam = QUEST_INFO_PARA;
	}

	BYTE name[64];
	DWORD start;
	BYTE info[0];
};

static const BYTE QUEST_VARS_PARA = 2;
struct stQuestVarsUserCmd : public stQuestUserCmd
{
	enum {
		MAX_NSIZE = 32,
		MAX_VSIZE = 128,
	};

	stQuestVarsUserCmd()
	{
		byParam = QUEST_VARS_PARA;	
	}
	BYTE count; //��������
	struct Var {
		BYTE name[MAX_NSIZE];
		BYTE value[MAX_VSIZE];	
	} vars_list[0]; //�����б�
};

static const BYTE REQUEST_QUEST_PARA = 3;
struct stRequestQuestUserCmd : public stQuestUserCmd
{
	stRequestQuestUserCmd()
	{
		byParam = REQUEST_QUEST_PARA;
	}
	char target[16];
	BYTE offset; //�����֧
};

static const BYTE ABANDON_QUEST_PARA = 4;
struct stAbandonQuestUserCmd : public stQuestUserCmd
{
	stAbandonQuestUserCmd()
	{
		byParam = ABANDON_QUEST_PARA;
	}
};

static const BYTE CART_CONTROL_QUEST_PARA = 5;//�����ڳ��ߺ�ͣ
struct stCartControlQuestUserCmd : public stQuestUserCmd
{
	stCartControlQuestUserCmd()
	{
		byParam = CART_CONTROL_QUEST_PARA;
	}
};

static const BYTE CHECK_VALID_QUEST_PARA = 6;//�����Խӵ������б�
struct stCheckValidQuestUserCmd : public stQuestUserCmd
{
	stCheckValidQuestUserCmd()
	{
		byParam = CHECK_VALID_QUEST_PARA;
	}
};

static const BYTE REQ_VALID_QUEST_PARA = 7;//�����Խӵ������б�
struct stReqValidQuestUserCmd : public stQuestUserCmd
{
	stReqValidQuestUserCmd()
	{
		byParam = REQ_VALID_QUEST_PARA;
	}
};

static const BYTE RET_VALID_QUEST_PARA = 8;//���ؿ��Խӵ������б�
struct stRetValidQuestUserCmd : public stQuestUserCmd
{
	stRetValidQuestUserCmd()
	{
		byParam = RET_VALID_QUEST_PARA;
		size = 0;
	}
	DWORD size;
	char content[0];
};

/************************************************************
************************����ָ������*********************
*************************************************************/

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ����ָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stSeptUserCmd : public stNullUserCmd
{
	stSeptUserCmd()
	{
		byCmd = SEPT_USERCMD;
	}
};

/// ���崴��ǰ���û�״̬�����Ϣ
const BYTE SEPT_STATUS_CHECK_PARA = 1;
struct stSeptStatusCheckCmd  : public stSeptUserCmd
{
	stSeptStatusCheckCmd()
	{
		byParam = SEPT_STATUS_CHECK_PARA;

	}
};

// ���崴������
const BYTE CREATE_SEPT_PARA = 2;
struct stCreateSeptCmd: public stSeptUserCmd
{
	stCreateSeptCmd()
	{
		byParam = CREATE_SEPT_PARA;
	}
	char SeptName[MAX_NAMESIZE];  // ��������
};

// ���崴���ɹ�ָ֪ͨ��
const BYTE SERVER_RETURN_SEPT_CREATE_FAILED = 3;
struct stServerReturnSeptCheckCmd : public stSeptUserCmd
{
	stServerReturnSeptCheckCmd()
	{
		byParam = SERVER_RETURN_SEPT_CREATE_FAILED;
	}
};

struct stSeptRember{
	char  memberName[MAX_NAMESIZE];				// �����Ա����
	WORD  level;						// ��Ա�ȼ�
	BYTE  byOnline;						// ��Ա���߱�־ 0 Ϊ������ ��0Ϊ����
	WORD  occupation;					// ְҵ����
	DWORD exploit;						// ��ѫֵ
	DWORD grace;						// �Ĳ�ֵ
	char memberAliasName[MAX_NAMESIZE];			// ��Ա����
};

// ���������ؼ����Ա�б��������ߵĸ��˼�����Ϣ
const BYTE ALL_SEPT_MEMBER_PARA = 5;
struct stAllSeptMemberCmd : public stSeptUserCmd
{
	stAllSeptMemberCmd()
	{
		byParam = ALL_SEPT_MEMBER_PARA;
	}
	WORD size;                     // ����
	stSeptRember memberList[0];   // ��Ա�б�
};


enum SEPT_ADD_MEMBER{
			SEPT_QUESTION,          // ��������ĳ�˵�ѯ��
			SEPT_ANSWER_YES,        // ȷ������
			SEPT_ANSWER_NO          // ������
};

// �������˼������
const BYTE ADD_MEMBER_TO_SEPT_PARA = 7;  // �����Ϣ�ɿͻ��˷��������������ĳ�ˣ��ɷ���˷����ͻ�����ѯ��ĳ���Ƿ��������
struct stAddMemberToSeptCmd : public stSeptUserCmd
{
	stAddMemberToSeptCmd()
	{
		byParam = ADD_MEMBER_TO_SEPT_PARA;
	}
	char 	memberName[MAX_NAMESIZE];   // ���������
	DWORD memberID;                   // ��������ID
	char 	septName[MAX_NAMESIZE];    // ��������
	BYTE 	byState;                    // �μ� enum SEPT_ADD_MEMBER
};

// ���������Ա
const BYTE FIRE_MEMBER_FROM_SEPT_PARA = 8;
struct stFireMemberFromSeptCmd : public stSeptUserCmd
{
	stFireMemberFromSeptCmd()
	{
		byParam = FIRE_MEMBER_FROM_SEPT_PARA ;
	}
	char memberName[MAX_NAMESIZE];
};

enum{
		 SEPT_MEMBER_STATUS_ONLINE,          // �����Ա����
		 SEPT_MEMBER_STATUS_OFFLINE,         // �����Ա����
		 SEPT_MEMBER_STATUS_FIRE,            // �����Ա������
		 SEPT_MEMBER_STATUS_NEWMEMBER,       // �����Ӽ����Ա
		 SEPT_MEMBER_STATUS_ALIASCHANGE,     // ���ı���
};

// ����˸��¼����Ա״̬����Ϣ֪ͨ��Ϣ ״̬���ϱ�
const BYTE BROADCAST_SEPT_MEMBER_INFO_PARA = 10;
struct stBroadcastSeptMemberInfo : public stSeptUserCmd
{
	stBroadcastSeptMemberInfo()
	{
		byParam = BROADCAST_SEPT_MEMBER_INFO_PARA;
		level = 0;
		bzero(name, sizeof(name));
		bzero(aliasName, sizeof(name));
		exploit = 0;
		grace = 0;
	}
	BYTE byStatus;                       // ��Ա״̬
	WORD wdOccupation;	 	     // ��Աְҵ
	WORD level;			     // ��Ա�ȼ�
	char name[MAX_NAMESIZE];             // ��Ա����
	char aliasName[MAX_NAMESIZE];	     // ��Ա����
	DWORD exploit;			     // ��ѫֵ
	DWORD grace;			     // �Ĳ�ֵ
};

// �������˷��ͼ�����Ϣ
const BYTE SEPT_BASE_INFO_PARA = 11;
struct stSeptBaseInfoCmd : public stSeptUserCmd
{
	stSeptBaseInfoCmd()
	{
		byParam = SEPT_BASE_INFO_PARA;
		dwLevel = 0;
	}
	DWORD dwMasterTempID;
	char  septName[MAX_NAMESIZE];         // ��������
	char  master[MAX_NAMESIZE];            // �����峤
	char  note[255];
	DWORD dwRepute;			// ��������
	DWORD dwLevel;			// ����ȼ�
};

// �����Ա��������
const BYTE SEPTMEMBER_LEAVE_SEPT_PARA = 12;
struct stSeptMemberLeaveSeptCmd : public stSeptUserCmd
{
	stSeptMemberLeaveSeptCmd()
	{
		byParam = SEPTMEMBER_LEAVE_SEPT_PARA;
	}
};

struct stSeptVoteRecord{
	char septName [MAX_NAMESIZE];
	char master[MAX_NAMESIZE];
	WORD wdVoteNumber;
	char note[255];
};

// ����ͶƱ�б�
const BYTE VOTELIST_SEPT_PARA = 13;
struct stVoteListSeptCmd : public stSeptUserCmd
{
	stVoteListSeptCmd()
	{
		byParam = VOTELIST_SEPT_PARA;
	}
	char septName[MAX_NAMESIZE];
	DWORD dwSize;
	BYTE flag; // 0Ϊ��Ϣͷ 1Ϊ������Ϣ
	stSeptVoteRecord data[0];
};

// ����ͶƱ��Ϣ
const BYTE VOTE_SEPT_PARA = 14;
struct stVoteSeptCmd : public stSeptUserCmd
{
	stVoteSeptCmd()
	{
		byParam = VOTE_SEPT_PARA;
	}
	char septName[MAX_NAMESIZE];
	BYTE bySuccess;   // Ϊ1Ϊ�ɹ� Ϊ0Ϊʧ��
};

// �������ð��˵��
const BYTE NOTE_SEPT_PARA = 15;
struct stNoteSeptCmd : public stSeptUserCmd
{
	stNoteSeptCmd()
	{
		byParam = NOTE_SEPT_PARA;
	}
	char noteBuf[255];
};

// �����Ա��������
const BYTE CHANGE_SEPT_MEMBER_ALIASNAME_PARA = 16;
struct stChangeSeptMemberAliasName : public stSeptUserCmd
{
	stChangeSeptMemberAliasName()
	{
		byParam = CHANGE_SEPT_MEMBER_ALIASNAME_PARA;
	}

	char name[MAX_NAMESIZE];             // ��Ա����
	char aliasname[MAX_NAMESIZE];        // ��Ա����
};

// �����Ա����������
const BYTE DONATE_HONOR_PARA = 17;
struct stDonateHonor : public stSeptUserCmd
{
	stDonateHonor()
	{
		byParam = DONATE_HONOR_PARA;
		dwHonor = 0;
	}
	
	DWORD dwHonor;
};

enum
{
	COUNTRY_SEPT_SORT, 	// �����������а�
	WORLD_SEPT_SORT,	// ����������а�
};

// ��ѯ�������а񣨸���������
const BYTE REQ_SEPT_SORT_PARA = 18;
struct stReqSeptSort : public stSeptUserCmd
{
	stReqSeptSort()
	{
		byParam = REQ_SEPT_SORT_PARA;
		byType = COUNTRY_SEPT_SORT;
	}
	
	BYTE byType;
};

struct stSeptSortInfo
{
	char  septName[MAX_NAMESIZE];         	// ��������
	DWORD dwRepute;				// ��������
	DWORD dwOrder;				// ��������
	DWORD dwCountryID;			// ����ID
};

// ���ؼ������а񣨸���������
const BYTE RTN_SEPT_SORT_PARA = 19;
struct stRtnSeptSort : public stSeptUserCmd
{
	stRtnSeptSort()
	{
		byParam = RTN_SEPT_SORT_PARA;
		dwSize = 0;
	}
	
	BYTE byType;
	DWORD dwSize;
	stSeptSortInfo data[0];
};

struct stSeptRecord{
	char septName [MAX_NAMESIZE];
	char master[MAX_NAMESIZE];
	char note[255];
};

// ����ͶƱ�б�
const BYTE LIST_SEPT_PARA = 20;
struct stListSeptCmd : public stSeptUserCmd
{
	stListSeptCmd()
	{
		byParam = LIST_SEPT_PARA;
	}
	DWORD dwSize;
	BYTE flag; // 0Ϊ��Ϣͷ 1Ϊ������Ϣ
	stSeptRecord data[0];
};

enum {
	REQUEST_JOIN_OK,
	REQUEST_JOIN_CANCEL,
	REQUEST_JOIN_TIMEOUT
};

// ������������Ϣ
const BYTE REQUEST_JOIN_SEPT_PARA = 21;
struct stRequestJoinSeptCmd : public stSeptUserCmd
{
	stRequestJoinSeptCmd()
	{
		byParam = REQUEST_JOIN_SEPT_PARA;
	}
	char name[MAX_NAMESIZE]; //������������ʱ����Է��峤�����֣���������������ʱ������������˵�����
	BYTE status;
};

// �������ռ��NPC��Ϣ
const BYTE REQUEST_NPCINFO_SEPT_PARA = 22;
struct stRequestNpcInfoSeptCmd : public stSeptUserCmd
{
	stRequestNpcInfoSeptCmd()
	{
		byParam = REQUEST_NPCINFO_SEPT_PARA;
	}
};

// ������ȡ���徭��
const BYTE REQUEST_SEPT_EXP_PARA = 23;
struct stRequestSeptExpCmd : public stSeptUserCmd
{
	stRequestSeptExpCmd()
	{
		byParam = REQUEST_SEPT_EXP_PARA;
	}
};

// �������ռ���NPC
const BYTE REQUEST_ABJURATION_SEPTNPC_PARA = 24;
struct stRequestAbjurationSeptNpcSeptCmd : public stSeptUserCmd
{
	stRequestAbjurationSeptNpcSeptCmd()
	{
		byParam = REQUEST_ABJURATION_SEPTNPC_PARA;
	}
};

// �����������
const BYTE REQ_SEPT_NAME_PARA = 25;
struct stReqSeptNameCmd : public stSeptUserCmd
{
	stReqSeptNameCmd()
	{
		byParam = REQ_SEPT_NAME_PARA;
		dwSeptID = 0;
	}

	DWORD dwSeptID;
};

// �����������
const BYTE RTN_SEPT_NAME_PARA = 26;
struct stRtnSeptNameCmd : public stSeptUserCmd
{
	stRtnSeptNameCmd()
	{
		byParam = RTN_SEPT_NAME_PARA;
		dwSeptID = 0;
		bzero(name, MAX_NAMESIZE);
	}

	DWORD dwSeptID;
	char name[MAX_NAMESIZE];
};
// ������������б�
const BYTE REQ_SEPT_NAME_LIST_PARA = 27;
struct stReqSeptNameListCmd : public stSeptUserCmd
{
	stReqSeptNameListCmd()
	{
		byParam = REQ_SEPT_NAME_LIST_PARA;
		num=0;
	}
	BYTE num;
	DWORD dwSeptID[0];
};

// ������������б�
const BYTE RTN_SEPT_NAME_LIST_PARA = 28;
struct stRtnSeptNameListCmd : public stSeptUserCmd
{
	stRtnSeptNameListCmd()
	{
		byParam = RTN_SEPT_NAME_LIST_PARA;
		num=0;
	}
	BYTE num;
	struct
	{
		DWORD dwSeptID;
		char name[MAX_NAMESIZE];
	} list[0];
};

// ������ȡ���徭��
const BYTE REQUEST_SEPT_NORMAL_EXP_PARA = 29;
struct stRequestSeptNormalExpCmd : public stSeptUserCmd
{
	stRequestSeptNormalExpCmd()
	{
		byParam = REQUEST_SEPT_NORMAL_EXP_PARA;
	}
};

//////////////////////////////////////////////////////////////
// ����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ����ָ��忪ʼ
//////////////////////////////////////////////////////////////
struct stPetUserCmd : public stNullUserCmd
{
	stPetUserCmd()
	{
		byCmd = PET_USERCMD;
	}
};

//����AIģʽ
enum petAIMode
{
	PETAI_MOVE_FOLLOW	= 0x0001,	//����
	PETAI_MOVE_STAND	= 0x0002,	//վ������
	PETAI_ATK_ACTIVE	= 0x0100,	//�������е���
	PETAI_ATK_PASSIVE	= 0x0200,	//�������ڹ������˵ĵ���
	PETAI_ATK_NOATK		= 0x0400	//��ս��
};
// ���ó����AIģʽ
const BYTE SETAI_PET_PARA = 1;
struct stSetAIPetCmd : public stPetUserCmd
{
	stSetAIPetCmd()
	{
		byParam = SETAI_PET_PARA;
	}
	petType type;
	petAIMode mode;
};

//����״̬
enum petState
{
	PET_STATE_NOPET		= 0,	//û�г���
	PET_STATE_NORMAL	= 1,	//��ͨ
	PET_STATE_PUTUP		= 2,	//����
	PET_STATE_DEAD		= 3,	//����
	PET_STATE_CONSIGN	= 4		//�Ĵ�
};

//���������
struct t_PetData
{
	DWORD id;		//npcid
	char name[MAX_NAMESIZE];//����
	BYTE lv;		//�ȼ�
	DWORD hp;		//����
	DWORD maxhp;		//�������
	WORD str;		//����
	WORD intel;		//����
	WORD agi;		//����
	WORD men;		//����
	WORD vit;		//����
	DWORD exp;		//����
	DWORD maxexp;		//�����
	DWORD atk;		//������
	DWORD maxatk;		//��󹥻���
	DWORD matk;		//ħ��
	DWORD maxmatk;		//���ħ��
	DWORD def;		//���
	DWORD mdef;		//ħ��
	WORD cri;		//����
	DWORD skills[4];	//����
	WORD ai;		//AI
	DWORD maxhp_plus;	//�����������
	WORD atk_plus;		//������������
	WORD maxatk_plus;	//���������������
	WORD matk_plus;		//����ħ��������
	WORD maxmatk_plus;	//�������ħ��������
	WORD pdef_plus;		//�������
	WORD mdef_plus;		//����ħ��
	petType type;		//����
	petState state;		//״̬
};

// ������������
const BYTE REQUESTDATA_PET_PARA = 2;
struct stRequestDataPetCmd : public stPetUserCmd
{
	stRequestDataPetCmd()
	{
		byParam = REQUESTDATA_PET_PARA;
	}
	petType type;
};

//���س��������
const BYTE REFRESHDATA_PET_PARA = 3;
struct stRefreshDataPetCmd : public stPetUserCmd
{
	stRefreshDataPetCmd()
	{
		byParam = REFRESHDATA_PET_PARA;
		bzero(&data, sizeof(data));
	}
	petType type;
	DWORD id;

	t_PetData data;
};

//�ĳ�������
const BYTE CHANGENAME_PET_PARA = 4;
struct stChangeNamePetCmd : public stPetUserCmd
{
	stChangeNamePetCmd()
	{
		byParam = CHANGENAME_PET_PARA;
	}
	petType type;
	char name[MAX_NAMESIZE];
};

//���ӳ���
const BYTE ADDPET_PET_PARA = 5;
struct stAddPetPetCmd : public stPetUserCmd
{
	stAddPetPetCmd()
	{
		byParam = ADDPET_PET_PARA;
	}
	petType type;
	DWORD id;
};

//ɾ������
const BYTE DELPET_PET_PARA = 6;
struct stDelPetPetCmd : public stPetUserCmd
{
	stDelPetPetCmd()
	{
		byParam = DELPET_PET_PARA;
	}
	petType type;
	DWORD id;
};

//ɾ������
const BYTE DELSKILL_PET_PARA = 7;
struct stDelSkillPetCmd : public stPetUserCmd
{
	stDelSkillPetCmd()
	{
		byParam = DELSKILL_PET_PARA;
	}
	petType type;///��������
	DWORD id;///����id
};

//�ٻ����������
const BYTE CALLHORSE_PET_PARA = 8;
struct stCallHorsePetCmd : public stPetUserCmd
{
	stCallHorsePetCmd()
	{
		byParam = CALLHORSE_PET_PARA;
	}
};

//������ƥ
const BYTE PUTHORSE_PET_PARA = 9;
struct stPutHorsePetCmd : public stPetUserCmd
{
	stPutHorsePetCmd()
	{
		byParam = PUTHORSE_PET_PARA;
	}
};

//ˢ���������
const BYTE HORSEDATA_PET_PARA = 10;
struct stHorseDataPetCmd : public stPetUserCmd
{
	stHorseDataPetCmd()
	{
		byParam = HORSEDATA_PET_PARA;
		type = PET_TYPE_RIDE;
	}
	petType type;
	DWORD id;
	t_HorseData data;
};

//ǿ�Ƴ��﹥��Ŀ��
const BYTE SETTARGET_PET_PARA = 11;
struct stSetTargetPetCmd : public stPetUserCmd
{
	stSetTargetPetCmd()
	{
		byParam = SETTARGET_PET_PARA;
	}
	petType type;///��������
	DWORD targetType;///Ŀ������ 0:��� 1:NPC
	DWORD id;///Ŀ�����ʱID
};

//ͬ�������Ѫ�;���
const BYTE HP_EXP_PET_PARA = 12;
struct stHpExpPetCmd : public stPetUserCmd
{
	stHpExpPetCmd()
	{
		byParam = HP_EXP_PET_PARA;
	}
	petType type;
	DWORD id;//tempid
	DWORD hp;
	DWORD exp;
};

/*
//����
const BYTE CHANGEHORSE_PET_PARA = 11;
struct stChangeHorsePetCmd : public stPetUserCmd
{
	stChangeHorsePetCmd()
	{
		byParam = CHANGEHORSE_PET_PARA;
	}
	DWORD id;
};
*/

//////////////////////////////////////////////////////////////
// ����ָ������
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
/// ������ͻ���PING���ߵ�����
//////////////////////////////////////////////////////////////
struct stPingUserCmd : public stNullUserCmd
{
	stPingUserCmd()
	{
		byCmd = PING_USERCMD;
	}
};

struct ping_element
{
	BYTE gateway_ip[15+1];
	/**
	  * EMPTY = 0,	    ����0-399,
	  * VERYIDLE = 1,   ����400-799,
	  * IDLE = 2,       ����800-1199,
	  * BUSY = 3,       ����1200-1599
	  * VERYBUSY= 4     ����1600-1999
	  */
	BYTE state;
	ping_element()
	{
		bzero(gateway_ip, sizeof(gateway_ip));
	}
};

const BYTE PING_LIST_PARA = 1;
struct stPingList : public stPingUserCmd
{
	DWORD zone_id;
	struct ping_element ping_list;

	stPingList()
	{
		byParam = PING_LIST_PARA;
	};
};

const BYTE REQUEST_PING_LIST_PARA = 2;
struct stRequestPingList : public stPingUserCmd
{
	DWORD id;  //��Ϸ����

	stRequestPingList() 
	{
		byParam = REQUEST_PING_LIST_PARA;
	}
};

const BYTE LOGIN_PING_PARA = 3;
struct stLoginPing : public stPingUserCmd
{
	stLoginPing() 
	{
		byParam = LOGIN_PING_PARA;
	}
};


//////////////////////////////////////////////////////////////
/// ������ͻ���PING���ߵ�����
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ���������ʼ
//////////////////////////////////////////////////////////////
#ifndef _BUGTYPE_
#define _BUGTYPE_
enum enumBuyType{
	eBuySilver = 1,
	eBuyGold = 2,
	eBuyTicket = 4,
};
#endif
struct stGoldUserCmd : public stNullUserCmd
{
	stGoldUserCmd()
	{
		byCmd = GOLD_USERCMD;
	}
};

const BYTE QUERY_GOLD_PARA = 1;
struct stQueryGold : public stGoldUserCmd
{
	stQueryGold() 
	{
		byParam = QUERY_GOLD_PARA;
	}
};

enum
{
	REDEEM_SUCCESS,    // �һ��ɹ� 
	REDEEM_BUSY,       // ������æ
	REDEEM_STOP,      // �һ�ϵͳֹͣ����
	REDEEM_FAIL,       // ʧ��
};
#define REDEEM_RATE_GOLD 1 //�һ���ұ���1:1
#define	REDEEM_RATE_MONTH_CARD 18//�һ��¿�����1:18
//����һ����
const BYTE REQUEST_REDEEM_GOLD_PARA = 2;
struct stRequestRedeemGold : public stGoldUserCmd
{
	stRequestRedeemGold()
	{
		byParam = REQUEST_REDEEM_GOLD_PARA;
	}
	DWORD dwNum;	//��Ҫ�һ��ĵ���
};
//�һ���ҷ���
const BYTE REDEEM_GOLD_PARA = 3;
struct stRedeemGold : public stGoldUserCmd
{
	stRedeemGold()
	{
		byParam = REDEEM_GOLD_PARA;
	}
	DWORD dwNum;	//���ص�ǰ�����
	DWORD dwBalance;			/// �㿨���
	BYTE byReturn;	//��������
};
 
//����һ��¿�
const BYTE REQUEST_REDEEM_MONTH_CARD_PARA = 4;
struct stRequestRedeemMonthCard : public stGoldUserCmd
{
	stRequestRedeemMonthCard()
	{
		byParam = REQUEST_REDEEM_MONTH_CARD_PARA;
	}
};

//�һ��¿�����
const BYTE REDEEM_MONTH_CARD_PARA = 5;
struct stRedeemMonthCard : public stGoldUserCmd
{
	stRedeemMonthCard()
	{
		byParam = REDEEM_MONTH_CARD_PARA;
	}
	DWORD dwNum;	//����ʱ��(time_t,4�ֽڽṹ)
	DWORD dwBalance;			/// �㿨���
	BYTE byReturn;	//��������
};

//��ѯ�¿��ͽ��
const BYTE REQUEST_CARD_AND_GOLD_PARA = 6;
struct stRequestCardAndGold :  public stGoldUserCmd
{
	stRequestCardAndGold()
	{
		byParam = REQUEST_CARD_AND_GOLD_PARA;
	}
};

//��ѯ����
const BYTE RETURN_CARD_AND_GOLD_PARA = 7;
struct stReturnCardAndGold :  public stGoldUserCmd
{
	stReturnCardAndGold()
	{
		byParam = RETURN_CARD_AND_GOLD_PARA;
	}
	DWORD dwMonthCard;//�¿�
	DWORD dwGold;//���
	BYTE byReturn;//��������
};
//��ѯ�㿨
const BYTE REQUEST_POINT_PARA = 8;
struct stRequestPoint :  public stGoldUserCmd
{
	stRequestPoint()
	{
		byParam = REQUEST_POINT_PARA;
	}
};
//��ѯ�㿨����
const BYTE RETURN_REQUEST_POINT_PARA = 9;
struct stReturnRequestPoint :  public stGoldUserCmd
{
	stReturnRequestPoint()
	{
		byParam = RETURN_REQUEST_POINT_PARA;
	}
	DWORD dwPoint;//ʣ�����
	BYTE byReturn;//��ѯ��������
};
//���ϵֹͣ����
const BYTE STOP_SERVICE_GOLD_PARA = 10;
struct stStopServiceGold :  public stGoldUserCmd
{
	stStopServiceGold()
	{
		byParam = STOP_SERVICE_GOLD_PARA;
	}
};
#define CARDID_LENGTH_PARA 32
///���ѿ���������
enum
{
	ZONE_CARD_OBJECT=1, ///���߿� 
	ZONE_CARD_PROFRESSION=2,///ר����
};
//���ѿ�����
const BYTE CONSUME_CARD_PARA = 11;
struct stConSumeCardCard : public stGoldUserCmd
{
	stConSumeCardCard()
	{
		byParam = CONSUME_CARD_PARA;
		bzero(cardid,sizeof(cardid));
	}
	DWORD type;
	char cardid[CARDID_LENGTH_PARA];
};
/// ����������
enum
{
	OBJ_GOLD_STONE = 1,//�;���������ʯ
	OBJ_GOLD_OBJECT = 2,//�ͽ�ɫװ��
	OBJ_GREEN_OBJECT = 3,//����ʥװ��
	SUBAT_GOLD = 4,	//ר������ֵ����������//���߿��������Ͳ����ڸ�ֵ�ظ�
};
//���ѿ����ѷ���
const BYTE RETURN_CONSUME_CARD_PARA = 12;
struct stReturnConSumeCardCard : public stGoldUserCmd
{
	stReturnConSumeCardCard()
	{
		byParam = RETURN_CONSUME_CARD_PARA;
	}
	BYTE byType;	//��������
	BYTE byReturn;	//��������
	DWORD balance;	//ר������������
};
/*
enum
{
	QUERY_SUCCESS,    // ��ѯ�ɹ�
	QUERY_BUSY,       // ������æ
	QUERY_FAIL,       // ��ѯʧ��
	QUERY_EMPTY,      // ���ʺţ�û�м�¼
};

enum
{
	TYPE_QUERY,     // ��ѯ�ķ���
	TYPE_CHANGE    // ��������ҵķ���
};

const BYTE RETURN_QUERY_GOLD_PARA = 2;
struct stReturnQueryGold : public stGoldUserCmd
{
	double gold;				/// �ʻ����
	BYTE   state;                           /// ��ѯ״̬
	BYTE   type;                            /// ��������

	stReturnQueryGold()
	{
		gold = 0;
		type = TYPE_QUERY;
		byParam = RETURN_QUERY_GOLD_PARA;
	};
};

const BYTE CHANGE_POINTTOGOLD_PARA = 3;
struct stChangePointGold : public stGoldUserCmd
{
	DWORD point;				/// Ҫ�һ��ĵ���

	stChangePointGold()
	{
		point = 0;
		byParam = CHANGE_POINTTOGOLD_PARA;
	};
};
// */

//////////////////////////////////////////////////////////////
/// �������������
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
/// �����Ʊ���ʼ
//////////////////////////////////////////////////////////////
//STOCK_USERCMD
struct stStockSceneUserCmd : public stNullUserCmd
{
	stStockSceneUserCmd()
	{
		byCmd = STOCK_SCENE_USERCMD;
	}
};
struct stStockBillUserCmd : public stNullUserCmd
{
	stStockBillUserCmd()
	{
		byCmd = STOCK_BILL_USERCMD;
	}
};
/// ��Ʊ����
#define PASSWORD_STOCKPARA 1
struct stPassowrdStockUserCmd : public stStockBillUserCmd
{
	stPassowrdStockUserCmd()
	{
		byParam = PASSWORD_STOCKPARA;
		bzero(byPawword,sizeof(byPawword));
		bzero(byNew1,sizeof(byNew1));
		bzero(byNew2,sizeof(byNew2));
	}
	char byPawword[MAX_PASSWORD];	/// ��ǰ����
	char byNew1[MAX_PASSWORD];		/// ������
	char byNew2[MAX_PASSWORD];		/// ȷ������
};

/// ���ù�Ʊ���뷵��
enum
{
	STOCK_LOGIN_OK,	/// ��½ok
	STOCK_LOGIN_NOTLOGIN,	/// δ��½(��ʾ���ȵ�½)
	STOCK_OPEN_OK,	/// �����ʺ�ok
	STOCK_CHANGE_OK,	/// �޸�����
	STOCK_ERROR,/// ���벻��
	STOCK_DIFF, /// �����������벻һ��
	STOCK_NONE, /// ���벻��Ϊ��
	STOCK_SHORT,/// ����̫��
	STOCK_EXIST,/// �ʺ��Ѿ�����
	STOCK_SERVER_WRONG,/// ����������
	STOCK_GOLDLIST_MAX,/// ��Ʊ������
	STOCK_MONEYLIST_MAX,/// ��Ʊ������
};
#define RETUEN_PASSWORD_STOCKPARA 2
struct stReturnPasswordStockIserCmd : public stStockBillUserCmd
{
	stReturnPasswordStockIserCmd()
	{
		byParam = RETUEN_PASSWORD_STOCKPARA;
	}
	BYTE byReturn;	///�������÷�������
};
/// �����Ʊ�ʺ����
#define QUERY_FUND_STOCKPARA 3
struct stQueryFundStockUserCmd : public stStockBillUserCmd
{
	stQueryFundStockUserCmd() 
	{
		byParam = QUERY_FUND_STOCKPARA;
	}
	
};

/// ���ص�ǰ��Ʊ�ʺ����
#define RETURN_FUND_STOCKPARA 4 
struct stReturnFundStockUserCmd : public stStockBillUserCmd
{
	stReturnFundStockUserCmd() 
	{
		byParam = RETURN_FUND_STOCKPARA;
	}
	DWORD dwGold;/// ��ǰ�ʺŽ��
	DWORD dwMoney;/// ��ǰ�ʺ�����
};
 
/// ���Ʊ�ʺ�ע���ʽ�
#define TRANSFER_FUND_SAVE_STOCKPARA 5
struct stTransferFundStockSaveUserCmd : public stStockSceneUserCmd
{
	stTransferFundStockSaveUserCmd() 
	{
		byParam = TRANSFER_FUND_SAVE_STOCKPARA;
	}
	DWORD dwGold;/// �����������
	DWORD dwMoney;/// ������������
};

/// �ӹ�Ʊ�ʺ���ȡ�ʽ�
#define TRANSFER_FUND_FETCH_STOCKPARA 6 
struct stTransferFundStockFetchUserCmd : public stStockBillUserCmd
{
	stTransferFundStockFetchUserCmd() 
	{
		byParam = TRANSFER_FUND_FETCH_STOCKPARA;
	}
	DWORD dwGold;/// �����������
	DWORD dwMoney;/// ������������
};
 
/// ί������
#define CONSIGN_GOLD_STOCKPARA 7 
struct stConsignGoldStockUserCmd : public stStockBillUserCmd
{
	stConsignGoldStockUserCmd() 
	{
		byParam = CONSIGN_GOLD_STOCKPARA;
	}
	DWORD dwNum;/// �������
};
 
/// ί����
#define CONSIGN_MONEY_STOCKPARA 8
struct stConsignMoneyStockUserCmd : public stStockBillUserCmd
{
	stConsignMoneyStockUserCmd() 
	{
		byParam = CONSIGN_MONEY_STOCKPARA;
	}
	DWORD dwNum;/// ��������
};
/// ��������
#define CONSIGN_CANCEL_GOLD_STOCKPARA 9
struct stConsignCancelGoldStockUserCmd : public stStockBillUserCmd
{
	stConsignCancelGoldStockUserCmd() 
	{
		byParam = CONSIGN_CANCEL_GOLD_STOCKPARA;
	}
	DWORD dwNum;/// ������ˮ��
};

/// ������
#define CONSIGN_CANCEL_MONEY_STOCKPARA 10
struct stConsignCancelMoneyStockUserCmd : public stStockBillUserCmd
{
	stConsignCancelMoneyStockUserCmd() 
	{
		byParam = CONSIGN_CANCEL_MONEY_STOCKPARA;
	}
	DWORD dwNum;/// ������ˮ��
};
enum StockType
{
	STOCK_GOLD,
	STOCK_MONEY,
};
struct StockList
{
	DWORD id;			/// ��ˮ��
	DWORD dwNum;		/// ����
	DWORD dwPrice;	/// ί�м۸�
	DWORD dwTime;/// ί��ʱ��
};
/// ����δ�ɽ�ί�е�
#define REQUEST_CONSIGN_LIST_STOCKPARA 11
struct stRequestConsignListStockUserCmd : public stStockBillUserCmd
{
	stRequestConsignListStockUserCmd() 
	{
		byParam = REQUEST_CONSIGN_LIST_STOCKPARA;
	}
};

/// δ�ɽ��Ľ��ί�е�
#define CONSIGN_GOLD_LIST_STOCKPARA 12
struct stConsignGoldListStockUserCmd : public stStockBillUserCmd
{
	stConsignGoldListStockUserCmd() 
	{
		byParam = CONSIGN_GOLD_LIST_STOCKPARA;
	}
	DWORD size;
	StockList list[0];
};
/// δ�ɽ�������ί�е�
#define CONSIGN_MONEY_LIST_STOCKPARA 13
struct stConsignMoneyListStockUserCmd : public stStockBillUserCmd
{
	stConsignMoneyListStockUserCmd() 
	{
		byParam = CONSIGN_MONEY_LIST_STOCKPARA;
	}
	DWORD size;
	StockList list[0];
};
/// ���󳷵�
#define REQUEST_CANCEL_LIST_STOCKPARA 14
struct stRequestCancelListStockUserCmd : public stStockBillUserCmd
{
	stRequestCancelListStockUserCmd() 
	{
		byParam = REQUEST_CANCEL_LIST_STOCKPARA;
	}
	DWORD id;
	BYTE byType;
};
enum
{
	STOCK_CANCEL_ERROR,
	STOCK_CANCEL_OK,
};
/// Ӧ�𳷵�
#define RETURN_CANCEL_LIST_STOCKPARA 15
struct stReturnCancelListStockUserCmd : public stStockBillUserCmd
{
	stReturnCancelListStockUserCmd() 
	{
		byParam = RETURN_CANCEL_LIST_STOCKPARA;
	}
	DWORD id;
	BYTE byType;
	BYTE byReturn;/*0��ʾʧ��,1��ʾ�ɹ�*/
};
/// ��������ǰ10λ�ĵ���
#define REQUEST_FIRSTTEN_LIST_STOCKPARA 16
struct stRequestFirstTenListStockUserCmd : public stStockBillUserCmd
{
	stRequestFirstTenListStockUserCmd() 
	{
		byParam = REQUEST_FIRSTTEN_LIST_STOCKPARA;
	}
};

struct FirstTen
{
	DWORD dwNum;
	DWORD dwPrice;
};
/// ����ǰ5λ�ļ۸������
#define FIRSTFIVE_LIST_STOCKPARA 17
struct stFirstFiveListStockUserCmd : public stStockBillUserCmd
{
	stFirstFiveListStockUserCmd() 
	{
		byParam = FIRSTFIVE_LIST_STOCKPARA;
	}
	BYTE byType;//�����
	DWORD size;
	FirstTen list[0];
};

/// �ύί�е�
#define PUT_LIST_STOCKPARA 18
struct stPutListStockUserCmd : public stStockBillUserCmd
{
	stPutListStockUserCmd() 
	{
		byParam = PUT_LIST_STOCKPARA;
	}
	DWORD dwNum;	//��һ�����������
	DWORD dwPrice;  //����(����/���)
	BYTE  byType;	//��������,��һ�����
};
/// �����ύ��ί�е�
#define RETURN_PUT_LIST_STOCKPARA 19
struct stReturnPutListStockUserCmd : public stStockBillUserCmd
{
	stReturnPutListStockUserCmd() 
	{
		byParam = RETURN_PUT_LIST_STOCKPARA;
	}
	StockList list;
};
//��Ʊ��ʷ���ݽṹ
struct ConsignHistoryType
{
	ConsignHistoryType()
	{
		dwTime=0;
		dwPrice=0;
		dwTotal=0;
	}
	DWORD dwTime;
	WORD dwPrice;
	WORD dwTotal;
};
/// ������������ʷ��������
#define REQUEST_HISTORY_STOCKPARA 20
struct stRequstHistoryStockUserCmd : public stStockBillUserCmd
{
	stRequstHistoryStockUserCmd() 
	{
		byParam = REQUEST_HISTORY_STOCKPARA;
		begintime=0;
		num=0;
	}
	DWORD begintime;
	DWORD num;
};
/// �������´�����ʷ����(��ǰһ���ӵ�ͳ������)
#define NEW_HISTORY_STOCKPARA 21
struct stNewHistoryStockUserCmd : public stStockBillUserCmd
{
	stNewHistoryStockUserCmd() 
	{
		byParam = NEW_HISTORY_STOCKPARA;
	}
	ConsignHistoryType gold;
	ConsignHistoryType money;
};
/// ���ش�����ʷ�������(������)
#define HISTORY_GOLD_STOCKPARA 22
struct stHistoryGoldStockUserCmd : public stStockBillUserCmd
{
	stHistoryGoldStockUserCmd() 
	{
		byParam = HISTORY_GOLD_STOCKPARA;
	}
	DWORD size;
	ConsignHistoryType list[0];
};
/// ���ش�����ʷ��������(������)
#define HISTORY_MONEY_STOCKPARA 23
struct stHistoryMoneyStockUserCmd : public stStockBillUserCmd
{
	stHistoryMoneyStockUserCmd() 
	{
		byParam = HISTORY_MONEY_STOCKPARA;
	}
	DWORD size;
	ConsignHistoryType list[0];
};
/// ��������Լ���ʷ��������
#define REQUEST_SELF_HISTORY_STOCKPARA 24
struct stRequstSelfHistoryStockUserCmd : public stStockBillUserCmd
{
	stRequstSelfHistoryStockUserCmd() 
	{
		byParam = REQUEST_SELF_HISTORY_STOCKPARA;
		begintime=0;
		num=0;
	}
	DWORD begintime;
	DWORD num;
};
//����ί�е���ʷ����
struct ConsignSelfHistoryType
{
	ConsignSelfHistoryType()
	{
	}
	DWORD dwID;
	DWORD wdNum;
	DWORD wdCommitPrice;
	DWORD wdPrice;
	DWORD dwCommitTime;
	DWORD dwOkTime;
};
/// �����Լ���ʷ�������(������)
#define SELF_HISTORY_GOLD_STOCKPARA 25
struct stSelfHistoryGoldStockUserCmd : public stStockBillUserCmd
{
	stSelfHistoryGoldStockUserCmd() 
	{
		byParam = SELF_HISTORY_GOLD_STOCKPARA;
	}
	DWORD size;
	ConsignSelfHistoryType list[0];
};
/// �����Լ���ʷ��������(������)
#define SELF_HISTORY_MONEY_STOCKPARA 26
struct stSelfHistoryMoneyStockUserCmd : public stStockBillUserCmd
{
	stSelfHistoryMoneyStockUserCmd() 
	{
		byParam = SELF_HISTORY_MONEY_STOCKPARA;
	}
	DWORD size;
	ConsignSelfHistoryType list[0];
};
/// ��Ʊϵͳֹͣ����
#define STOP_SERVICE_STOCKPARA 27
struct stStopServiceStockUserCmd : public stStockBillUserCmd
{
	stStopServiceStockUserCmd() 
	{
		byParam = STOP_SERVICE_STOCKPARA;
	}
};
 
/// ���δ�ɽ�ί�е�
#define CONSIGN_CLEAN_LIST_STOCKPARA 28
struct stConsignCleanListStockUserCmd : public stStockBillUserCmd
{
	stConsignCleanListStockUserCmd() 
	{
		byParam = CONSIGN_CLEAN_LIST_STOCKPARA;
	}
};

/// ����õ���ǰ�û��ļ���͵�½״̬
#define REQUEST_STATE_STOCKPARA 29
struct stRequestStateStockUserCmd : public stStockBillUserCmd
{
	stRequestStateStockUserCmd() 
	{
		byParam = REQUEST_STATE_STOCKPARA;
	}
	bool  active;
	bool  login;
};
//��Ʊϵͳ����ָ��
#define PARA_CANCELLISTALL_STOCKPARA  30
struct t_CancelListAllStock_GateScene : public stStockBillUserCmd
{
	t_CancelListAllStock_GateScene()
		{
			byParam = PARA_CANCELLISTALL_STOCKPARA;
		}
};
//////////////////////////////////////////////////////////////
/// �����Ʊ�������
//////////////////////////////////////////////////////////////


//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ����������ʼ
//////////////////////////////////////////////////////////////
struct stQuizUserCmd : public stNullUserCmd
{
	stQuizUserCmd()
	{
		byCmd = QUIZ_USERCMD;
	}
};

enum
{
	QUIZ_WORLD,
	QUIZ_PERSONAL
};

enum 
{
	QUIZ_QUESTION,  // ѯ���Ƿ�����ʴ���
	QUIZ_YES,       // ���뾺��
	QUIZ_NO         // �ܾ�����
};

const BYTE QUERY_QUIZ_PARA = 1;
struct stQueryQuiz : public stQuizUserCmd
{
	stQueryQuiz() 
	{
		byParam = QUERY_QUIZ_PARA;
		byMsgType = QUIZ_QUESTION;
		byQuizType = 0;
	}
	
	BYTE byMsgType;
	BYTE byQuizType; // 1����ͨȫ����2�ǻ
};

enum
{
	CREATE_QUESTION, // ������������ʴ�
	CREATE_YES,	 // ����ɹ�
	CREATE_FAIL	 // ��Ǯ���㣬ʧ��
};

const BYTE CREATE_QUIZ_PARA = 2;
struct stCreateQuiz : public stQuizUserCmd
{
	stCreateQuiz() 
	{
		byParam = CREATE_QUIZ_PARA;
		byType	= CREATE_QUESTION;
		dwMoney = 100;
	}

	DWORD dwMoney;	// �����ʴ�ʱ����������
	BYTE byType;
};

const BYTE QUESTION_QUIZ_PARA = 3;
struct stQuestionQuiz : public stQuizUserCmd
{
	stQuestionQuiz()
	{
		byParam = QUESTION_QUIZ_PARA;
		dwSize = 7;
		dwTotal = 0;
	}

	DWORD dwID;  // ��Ŀ���
	BYTE  bySpareTime; // ʣ��ʱ��
	BYTE  byNextStartTime;
	BYTE  dwSize; // ��Ŀ����
	DWORD dwTotal;	// ������
	
	char subject[0]; // ��Ŀ ��һ �𰸶� ����....���м���0��Ϊ�ָ�
};

const BYTE ANSWER_QUIZ_PARA = 4;
struct stAnswerQuiz : public stQuizUserCmd
{
	stAnswerQuiz()
	{
		byParam = ANSWER_QUIZ_PARA;
		byLuck = 0;
	}

	DWORD dwID;	 // ��Ŀ���
	BYTE byType;	 // ��������
	BYTE  byLuck;	// �Ƿ�ʹ��������,1ʹ�ã�����ֵ�ǲ�ʹ��
	DWORD dwScore;	// �÷�

	DWORD answerID; // ��
};

const BYTE ANSWER_RETURN_QUIZ_PARA = 5;
struct stAnswerReturnQuiz : public stQuizUserCmd
{
	stAnswerReturnQuiz()
	{
		byParam = ANSWER_RETURN_QUIZ_PARA;
		byAnswerStatus = -1;
	}

	int byAnswerStatus; // -1:������0:�ش���ȷ, 1:�ش����
};

struct rank_elem
{
	char name[MAX_NAMESIZE];   // �������
	char countryName[MAX_NAMESIZE]; // ��������
	int  score;	// �÷�
	int  grace;
	int  rank; // �������
};

const BYTE QUIZ_POS_LIST_PARA = 6;
struct stQuizPosList : public stQuizUserCmd
{
	stQuizPosList()
	{
		byParam = QUIZ_POS_LIST_PARA;
		bzero(rank_list, sizeof(rank_list));
		dwSize = 1;
	}
	
	DWORD dwSize;
	rank_elem rank_list[21];
};

const BYTE QUIZ_CUR_SCORE_PARA = 7;
struct stQuizCurScore : public stQuizUserCmd
{
	stQuizCurScore()
	{
		byParam = QUIZ_CUR_SCORE_PARA;
		dwScore = 0;
		dwGrace = 0;
	}

	DWORD dwScore; // ���δ����ۻ�����
	DWORD dwGrace; // ���δ����ۻ��Ĳ�
};

const BYTE QUIZ_EXIT_PARA = 8;
struct stQuizExit : public stQuizUserCmd
{
	stQuizExit()
	{
		byParam = QUIZ_EXIT_PARA;
	}
};

const BYTE QUIZ_PARAM_PARA = 9;
struct stQuizParam : public stQuizUserCmd
{
	stQuizParam() 
	{
		byParam = QUIZ_PARAM_PARA;
		byReadQuestionTime = 10;
		byAnswerQuestionTime = 10;
		byCalcTime = 5;
		bySubjectNumber = 90;
	}

	BYTE byReadQuestionTime;	// ����ʱ��
	BYTE byAnswerQuestionTime;	// ����ʱ��
	BYTE byCalcTime;		// �����ʱ��
	BYTE bySubjectNumber;		// ������
	BYTE byType;			// ��������
	BYTE byStartTime;		// ������ʼʣ��ʱ��
};

//////////////////////////////////////////////////////////////
/// ��������������
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/// ����NPC�������ʼ
//////////////////////////////////////////////////////////////
struct stNpcDareUserCmd : public stNullUserCmd
{
	stNpcDareUserCmd()
	{
		byCmd = NPCDARE_USERCMD;
	}
};

const BYTE NPCDARE_DARE_PARA = 1;
struct stDareNpcDare : public stNpcDareUserCmd
{
	stDareNpcDare() 
	{
		byParam = NPCDARE_DARE_PARA;
	}
	DWORD dwNpcID;
};

const BYTE NPCDARE_GETGOLD_PARA = 2;
struct stDareNpcGetGold : public stNpcDareUserCmd
{
	stDareNpcGetGold() 
	{
		byParam = NPCDARE_GETGOLD_PARA;
	}
	DWORD dwNpcID;
};

enum
{
	QUESTION_NPCDARE_HOLD, // ��ѯ��������
	QUESTION_NPCDARE_DARE, // �����������ս
};

// ��ѯ��NPC����ս��Ϣ
const BYTE QUESTION_NPCDARE_INFO_PARA = 3;
struct stQuestionNpcDareInfo : public stNpcDareUserCmd
{
	stQuestionNpcDareInfo()
	{
		byParam = QUESTION_NPCDARE_INFO_PARA;
	}
	BYTE byType;	
	DWORD dwNpcID;
};
//////////////////////////////////////////////////////////////
/// ����NPC�����������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ������GM���߽��������ʼ
//////////////////////////////////////////////////////////////
struct stGmToolUserCmd : public stNullUserCmd
{
	stGmToolUserCmd()
	{
		byCmd = GMTOOL_USERCMD;
	}
};

const BYTE CHAT_GMTOOL_PARA = 1;
struct stChatGmTool : public stGmToolUserCmd
{
	char userName[MAX_NAMESIZE];
	DWORD countryID;
	DWORD sceneID;
	stChannelChatUserCmd chat_cmd;
	stChatGmTool() 
	{
		byParam = CHAT_GMTOOL_PARA;
		countryID = 0;
		sceneID = 0;
	}
};

const BYTE MSG_GMTOOL_PARA = 2;
struct stMsgGmTool : public stGmToolUserCmd
{
	BYTE type;//��������
	char content[512];//��Ҫ����
	BYTE contact;//��ϵ��ʽ
	char tele[64];//��ַ���绰
	char hisName[MAX_NAMESIZE];//���ٱ���
	char bugCountry[8];//bug��������
	char bugMap[16];//bug������ͼ
	char bugPos[8];//bug����λ��
	char bugTime[64];//����ʱ��
	char progName[64];//�����������
	BYTE behavior;//������Ϸ����
	stMsgGmTool()
	{

		type = 0;
		bzero(content, sizeof(content));
		contact = 0;
		bzero(tele, sizeof(tele));
		bzero(hisName, sizeof(hisName));
		bzero(bugCountry, sizeof(bugCountry));
		bzero(bugMap, sizeof(bugMap));
		bzero(bugPos, sizeof(bugPos));
		bzero(bugTime, sizeof(bugTime));
		bzero(progName, sizeof(progName));
		behavior = 0;
	}
};

//////////////////////////////////////////////////////////////
/// ������GM���߽������������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
// ��ͨ����ָ��忪ʼ
//////////////////////////////////////////////////////////////

//������״̬
enum cartoonState
{
	CARTOON_STATE_PUTAWAY,	//����
	CARTOON_STATE_FOLLOW,	//��������
	CARTOON_STATE_WAITING,	//�ȴ�����
	CARTOON_STATE_ADOPTED	//������
};

//����������
struct t_CartoonData
{
	//DWORD cartoonID;		//����id
	char name[MAX_NAMESIZE];	//����
	DWORD npcID;			//npcid
	DWORD masterID;			//����id
	char masterName[MAX_NAMESIZE];	//��������
	BYTE lv;			//�ȼ�
	DWORD exp;			//����
	DWORD maxExp;			//��������
	DWORD addExp;			//���������ӵľ���
	BYTE state;		//״̬
	char adopter[MAX_NAMESIZE];	//������
	DWORD time;			//�һ�ʣ��ʱ��(��)
	DWORD sp;			//����ֵ
	DWORD maxSp;			//�����ֵ
	DWORD masterLevel;		//���˵ȼ�
	BYTE repair;			//�Զ���������װ��

	t_CartoonData & operator = (const t_CartoonData& reg)
	{
		bcopy(&reg, this, sizeof(t_CartoonData));

		return *this;
	}

	t_CartoonData & operator << (const t_CartoonData& reg)
	{
		addExp += reg.addExp;
		state = reg.state;
		bcopy(reg.adopter, adopter, MAX_NAMESIZE);
		time = reg.time;
		/*
		DWORD e = addExp;
		bcopy(&reg, this, sizeof(t_CartoonData));
		addExp += e;
		*/

		return *this;
	}
};

struct stCartoonUserCmd : public stNullUserCmd
{
	stCartoonUserCmd()
	{
		byCmd = CARTOON_USERCMD;
	}
};

#define BUY_CARTOON_PARA 1	//����
struct stBuyCartoonCmd : public stCartoonUserCmd
{
	stBuyCartoonCmd()
	{
		byParam = BUY_CARTOON_PARA;
	}
	DWORD npcID;
};

#define ADD_CARTOON_PARA 2	//����б�
struct stAddCartoonCmd : public stCartoonUserCmd
{
	stAddCartoonCmd()
	{
		byParam = ADD_CARTOON_PARA;
	}
	bool isMine;
	DWORD cartoonID;
	t_CartoonData data;
};

#define REMOVE_CARTOON_PARA 3	//�Ƴ��б�
struct stRemoveCartoonCmd : public stCartoonUserCmd
{
	stRemoveCartoonCmd()
	{
		byParam = REMOVE_CARTOON_PARA;
	}
	DWORD cartoonID;
};

#define FOLLOW_CARTOON_PARA 4	//����/ȡ������
struct stFollowCartoonCmd : public stCartoonUserCmd
{
	stFollowCartoonCmd()
	{
		byParam = FOLLOW_CARTOON_PARA;
	}
	DWORD cartoonID;//ȡ��������0
};

#define LETOUT_CARTOON_PARA 5	//�ų���ʹ���˿�������
struct stLetOutCartoonCmd : public stCartoonUserCmd
{
	stLetOutCartoonCmd()
	{
		byParam = LETOUT_CARTOON_PARA;
	}
	DWORD cartoonID;
};

#define ADOPT_CARTOON_PARA 6	//��������
struct stAdoptCartoonCmd : public stCartoonUserCmd
{
	stAdoptCartoonCmd()
	{
		byParam = ADOPT_CARTOON_PARA;
	}
	DWORD cartoonID;
};

#define RETURN_CARTOON_PARA 7	//�黹����
struct stReturnCartoonCmd : public stCartoonUserCmd
{
	stReturnCartoonCmd()
	{
		byParam = RETURN_CARTOON_PARA;
	}
	DWORD cartoonID;
};

#define GETBACK_CARTOON_PARA 8	//�ջس���
struct stGetBackCartoonCmd : public stCartoonUserCmd
{
	stGetBackCartoonCmd()
	{
		byParam = GETBACK_CARTOON_PARA;
	}
};

#define DRAWEXP_CARTOON_PARA 9	//��ȡ����
struct stDrawExpCartoonCmd : public stCartoonUserCmd
{
	stDrawExpCartoonCmd()
	{
		byParam = DRAWEXP_CARTOON_PARA;
	}
	DWORD cartoonID;
};

#define CHANGENAME_CARTOON_PARA 10	//������
struct stChangeNameCartoonCmd : public stCartoonUserCmd
{
	stChangeNameCartoonCmd()
	{
		byParam = CHANGENAME_CARTOON_PARA;
	}
	DWORD cartoonID;
	char name[MAX_NAMESIZE];
};

#define ADD_WAITING_CARTOON_PARA 11	//��ӵȴ��б�
struct stAddWaitingCartoonCmd : public stCartoonUserCmd
{
	stAddWaitingCartoonCmd()
	{
		byParam = ADD_WAITING_CARTOON_PARA;
	}
	DWORD cartoonID;		//����id
	DWORD npcID;		//NPC ID
	char name[MAX_NAMESIZE];	//����
	char masterName[MAX_NAMESIZE];	//��������
	BYTE state;		//״̬
	DWORD time;			//�һ�ʣ��ʱ��(��)

	void operator = (const t_CartoonData& reg)
	{
		npcID = reg.npcID;
		bcopy(reg.name, name, MAX_NAMESIZE);
		bcopy(reg.masterName, masterName, MAX_NAMESIZE);
		state = reg.state;
		time = reg.time;
	}
};

#define REMOVE_WAITING_CARTOON_PARA 12	//�Ƴ��ȴ��б�
struct stRemoveWaitingCartoonCmd : public stCartoonUserCmd
{
	stRemoveWaitingCartoonCmd()
	{
		byParam = REMOVE_WAITING_CARTOON_PARA;
	}
	DWORD cartoonID;
};

#define REQUEST_LIST_CARTOON_PARA 13	//����ȴ��б�
struct stRequestListCartoonCmd : public stCartoonUserCmd
{
	stRequestListCartoonCmd()
	{
		byParam = REQUEST_LIST_CARTOON_PARA;
	}
};

#define SALE_CARTOON_PARA 14	//��������
struct stSaleCartoonCmd : public stCartoonUserCmd
{
	DWORD cartoonID;
	stSaleCartoonCmd()
	{
		byParam = SALE_CARTOON_PARA;

		cartoonID = 0;
	}
};

#define CHARGE_CARTOON_PARA 15	//��ֵ
struct stChargeCartoonCmd : public stCartoonUserCmd
{
	stChargeCartoonCmd()
	{
		byParam = CHARGE_CARTOON_PARA;
	}
	DWORD cartoonID;
	DWORD time;//��೤ʱ��(��)
};

#define HP_EXP_CARTOON_PARA 16	//ͬ��sp�;���
struct stHpExpCartoonCmd : public stCartoonUserCmd
{
	stHpExpCartoonCmd()
	{
		byParam = HP_EXP_CARTOON_PARA;
	}
	DWORD cartoonID;
	DWORD sp;
	DWORD exp;
};

#define SELL_ALL_CARTOON_PARA 17	//��������������Ʒ
struct stSellAllCartoonCmd : public stCartoonUserCmd
{
	stSellAllCartoonCmd()
	{
		byParam = SELL_ALL_CARTOON_PARA;
	}
};

#define CONSIGN_CARTOON_PARA 18	//ί������
struct stConsignCartoonCmd : public stCartoonUserCmd
{
	stConsignCartoonCmd()
	{
		byParam = CONSIGN_CARTOON_PARA;
	}
	DWORD cartoonID;
	char name[MAX_NAMESIZE];//����ʱ�Ǳ�ί���ˣ�������ʱ��ί����
};

#define CONSIGN_RET_CARTOON_PARA 19	//ȷ��ί������
struct stConsignRetCartoonCmd : public stCartoonUserCmd
{
	stConsignRetCartoonCmd()
	{
		byParam = CONSIGN_RET_CARTOON_PARA;
	}
	DWORD cartoonID;
	BYTE ret;//0���ܾ� 1��ͬ��
};

#define REPAIR_CARTOON_PARA 20	//�����Զ�����
struct stRepairCartoonCmd : public stCartoonUserCmd
{
	stRepairCartoonCmd()
	{
		byParam = REPAIR_CARTOON_PARA;
	}
	DWORD cartoonID;
	BYTE repair;//0���ر� 1����
};

//////////////////////////////////////////////////////////////
// ��ͨ����ָ������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// �����ʼ����ʼ
//////////////////////////////////////////////////////////////
const unsigned int mail_postage = 50;	///����50��

struct stMailUserCmd : public stNullUserCmd
{
	stMailUserCmd()
	{
		byCmd = MAIL_USERCMD;
	}
};

/*
const BYTE CHECK_ITEM_MAIL_PARA = 1;
struct stCheckItemMail : public stMailUserCmd
{
	DWORD itemID;
	stCheckItemMail()
	{
		byParam = CHECK_ITEM_MAIL_PARA;
	}
};

enum CheckReturn
{
	CHECK_RETURN_SUCCESS = 1,
	CHECK_RETURN_FAIL = 2
};
const BYTE CHECK_RETURN_MAIL_PARA = 2;
struct stCheckReturnMail : public stMailUserCmd
{
	CheckReturn retCode;
	stCheckReturnMail()
	{
		byParam = CHECK_RETURN_MAIL_PARA;
	}
};
*/

//�����ʼ�
const BYTE SEND_MAIL_PARA = 3;
struct stSendMail : public stMailUserCmd
{
	char toName[MAX_NAMESIZE];
	char title[MAX_NAMESIZE];
	char text[256];
	DWORD sendMoney;
	DWORD recvMoney;
	DWORD sendGold;
	DWORD recvGold;
	DWORD itemID;
	stSendMail() 
	{
		byParam = SEND_MAIL_PARA;
		sendMoney = 0;
		recvMoney = 0;
		sendGold = 0;
		recvGold = 0;
		itemID = 0;
	}
};

/*
enum sendReturn
{
	SEND_RETURN_SUCCESS = 1,
	SEND_RETURN_FAIL = 2
};
const BYTE SEND_RETURN_MAIL_PARA = 4;
struct stSendReturnMail : public stMailUserCmd
{
	sendReturn retCode;
	stSendReturnMail()
	{
		byParam = SEND_RETURN_MAIL_PARA;
	}
};
*/

//�����ʼ��б�
const BYTE GET_LIST_MAIL_PARA = 5;
struct stGetListMail : public stMailUserCmd
{
	stGetListMail()
	{
		byParam = GET_LIST_MAIL_PARA;
	}
};

//����ʼ��б�
const BYTE ADD_LIST_MAIL_PARA = 6;
struct stAddListMail : public stMailUserCmd
{
	DWORD id;
	BYTE state;
	char fromName[MAX_NAMESIZE];
	bool accessory;
	DWORD endTime;
	BYTE type;	//0:��ͨ 1��ϵͳ
	stAddListMail()
	{
		byParam = ADD_LIST_MAIL_PARA;
		id = 0;
		state = 0;
		accessory = 0;
		endTime = 0;
		bzero(fromName, sizeof(fromName));
		type = 0;
	}
};

//�����ʼ�����
const BYTE OPEN_MAIL_PARA = 7;
struct stOpenMail : public stMailUserCmd
{
	DWORD mailID;
	stOpenMail()
	{
		byParam = OPEN_MAIL_PARA;
	}
};

//�ʼ�����
const BYTE CONTENT_MAIL_PARA = 8;
struct stContentMail : public stMailUserCmd
{
	DWORD mailID;
	char title[MAX_NAMESIZE];
	bool accessory;
	char text[256];
	DWORD sendMoney;
	DWORD recvMoney;
	DWORD sendGold;
	DWORD recvGold;
	t_Object item;
	stContentMail()
	{
		byParam = CONTENT_MAIL_PARA;
		mailID = 0;
		bzero(title, sizeof(title));
		accessory = false;
		bzero(text, sizeof(text));
		sendMoney = recvMoney = 0;
		sendGold = 0;
		recvGold = 0;
		bzero(&item, sizeof(item));
	}
};

//ȡ���ʼ�����
const BYTE GET_ITEM_MAIL_PARA = 9;
struct stGetItemMail : public stMailUserCmd
{
	DWORD mailID;
	stGetItemMail()
	{
		byParam = GET_ITEM_MAIL_PARA;
	}
};

//ɾ���ʼ�����
const BYTE REMOVE_ITEM_MAIL_PARA = 10;
struct stRemoveItemMail : public stMailUserCmd
{
	DWORD mailID;
	stRemoveItemMail()
	{
		byParam = REMOVE_ITEM_MAIL_PARA;
	}
};

//ɾ���ʼ�
const BYTE DEL_MAIL_PARA = 11;
struct stDelMail : public stMailUserCmd
{
	DWORD mailID;
	stDelMail()
	{
		byParam = DEL_MAIL_PARA;
	}
};

//���ʼ���ʾ
const BYTE NOTIFY_NEW_MAIL_PARA = 12;
struct stNotifyNewMail : public stMailUserCmd
{
	stNotifyNewMail()
	{
		byParam = NOTIFY_NEW_MAIL_PARA;
	}
};

//�˻��ʼ�
const BYTE TURN_BACK_MAIL_PARA = 13;
struct stTurnBackMail : public stMailUserCmd
{
	DWORD mailID;
	stTurnBackMail()
	{
		byParam = TURN_BACK_MAIL_PARA;
	}
};

/*
*/
//////////////////////////////////////////////////////////////
/// �����ʼ��������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// �����������ʼ
//////////////////////////////////////////////////////////////
//const unsigned int mail_postage = 50;	///����50��

struct stAuctionUserCmd : public stNullUserCmd
{
	stAuctionUserCmd()
	{
		byCmd = AUCTION_USERCMD;
	}
};

enum
{
	AUCTION_WEAPOM_SWORD = 1,
	AUCTION_WEAPOM_AXE = 2,
	AUCTION_WEAPOM_BOW = 3,
	AUCTION_WEAPOM_STICK = 4,
	AUCTION_WEAPOM_WAND = 5,
	AUCTION_WEAPOM_FAN = 6,
	AUCTION_EQUIP_HEAD = 11,
	AUCTION_EQUIP_BODY = 12,
	AUCTION_EQUIP_WRIST = 13,
	AUCTION_EQUIP_SHIELD = 14,
	AUCTION_EQUIP_WAIST = 15,
	AUCTION_EQUIP_FOOT = 16,
	AUCTION_ACCESSORY_NECKLACE = 21,
	AUCTION_ACCESSORY_RING = 22,
	AUCTION_ACCESSORY_ADORNMENT = 23,
	AUCTION_BOOK_FIGHTER = 31,
	AUCTION_BOOK_ARCHER = 32,
	AUCTION_BOOK_WIZARD = 33,
	AUCTION_BOOK_SUMMONER = 34,
	AUCTION_BOOK_PRIEST = 35,
	AUCTION_BOOK_SPECIAL = 36,
	AUCTION_OTHER_GEM = 41,
	AUCTION_OTHER_ITEM = 42,
	AUCTION_OTHER_MATERIAL = 43,
	AUCTION_OTHER_LEECHDOM = 44
};

const BYTE AUCTION_BID_TYPE_MONEY = 0;
const BYTE AUCTION_BID_TYPE_GOLD = 1;

//����һ����Ʒ
const BYTE SALE_AUCTION_PARA = 1;
struct stSaleAuction : public stAuctionUserCmd
{
	DWORD itemID;
	DWORD minMoney;
	DWORD maxMoney;
	DWORD minGold;
	DWORD maxGold;
	BYTE bidType;
	stSaleAuction()
	{
		byParam = SALE_AUCTION_PARA;
		itemID = 0xffffffff;
		minMoney = 0;
		maxMoney = 0;
		minGold = 0;
		maxGold = 0;
		bidType = 0;
	}
};

//����
const BYTE BID_AUCTION_PARA = 2;
struct stBidAuction : public stAuctionUserCmd
{
	DWORD auctionID;
	DWORD money;
	DWORD gold;
	stBidAuction()
	{
		byParam = BID_AUCTION_PARA;
		auctionID = 0;
		money = 0;
		gold = 0;
	}
};

//����б�
const BYTE ADD_LIST_AUCTION_PARA = 3;
struct stAddListAuction : public stAuctionUserCmd
{
	BYTE list;//1:�鿴�б� 2:�����б� 3:�����б� 4:�����б�
	DWORD auctionID;
	DWORD minMoney;
	DWORD maxMoney;
	DWORD minGold;
	DWORD maxGold;
	DWORD endTime;
	t_Object item;
	bool mine;//�Ƿ��ҵļ۸����
	char owner[MAX_NAMESIZE];//������
	DWORD max;//�ܹ����ٸ�
	BYTE bidType;
	stAddListAuction()
	{
		byParam = ADD_LIST_AUCTION_PARA;

		list = 0;
		auctionID = 0;
		minMoney = 0;
		maxMoney = 0;
		minGold = 0;
		maxGold = 0;
		bzero(&item, sizeof(item));
		mine = false;
		bzero(owner, sizeof(owner));
		max = 0;
		bidType = 0;
	}
};

//�Ƴ��б�
const BYTE REMOVE_LIST_AUCTION_PARA = 4;
struct stRemoveListAuction : public stAuctionUserCmd
{
	BYTE list;//1:�鿴�б� 2:�����б� 3:�����б� 4:�����б�
	DWORD auctionID;
	stRemoveListAuction()
	{
		byParam = REMOVE_LIST_AUCTION_PARA;

		list = 0;
		auctionID = 0;
	}
};

//����
const BYTE QUERY_AUCTION_PARA = 5;
struct stQueryAuction : public stAuctionUserCmd
{
	BYTE type;
	char name[MAX_NAMESIZE];
	BYTE quality;//1��ͨ, 2��ɫ, 3��ɫ, 5��ʥ
	WORD level;
	WORD page;
	stQueryAuction()
	{
		byParam = QUERY_AUCTION_PARA;

		type = 0;
		bzero(name, sizeof(name));
		quality = 0;
		level = (DWORD)-1;
		page = 0;
	}
};

//ȡ������
const BYTE CANCEL_SALE_AUCTION_PARA = 6;
struct stCancelSaleAuction : public stAuctionUserCmd
{
	DWORD auctionID;
	stCancelSaleAuction()
	{
		byParam = CANCEL_SALE_AUCTION_PARA;

		auctionID = 0;
	}
};

//�����б�
const BYTE GET_LIST_AUCTION_PARA = 7;
struct stGetListAuction : public stAuctionUserCmd
{
	BYTE list;//2:�����б� 3:�����б� 
	stGetListAuction()
	{
		byParam = GET_LIST_AUCTION_PARA;

		list = 0;
	}
};

//////////////////////////////////////////////////////////////
/// ���������������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ����������ʼ
//////////////////////////////////////////////////////////////

struct stPrisonUserCmd : public stNullUserCmd
{
	stPrisonUserCmd()
	{
		byCmd = PRISON_USERCMD;
	}
};

//�������
const BYTE OUT_PRISON_PARA = 1;
struct stOutPrison : public stPrisonUserCmd
{
	stOutPrison()
	{
		byParam = OUT_PRISON_PARA;
	}
};

//��¸
const BYTE BRIBE_PRISON_PARA = 2;
struct stBribePrison : public stPrisonUserCmd
{
	DWORD money;
	stBribePrison()
	{
		byParam = BRIBE_PRISON_PARA;

		money = 0;
	}
};

//ȡ����ҽ(�����ϴPKֵ)
const BYTE BAIL_PRISON_PARA = 3;
struct stBailPrison : public stPrisonUserCmd
{
	char name[MAX_NAMESIZE];
	DWORD money;
	stBailPrison()
	{
		byParam = BAIL_PRISON_PARA;

		bzero(name, sizeof(name));
		money = 0;
	}
};

//���������
const BYTE LEAVE_PRISON_PARA = 4;
struct stLeavePrison : public stPrisonUserCmd
{
	stLeavePrison()
	{
		byParam = LEAVE_PRISON_PARA;
	}
};

//////////////////////////////////////////////////////////////
/// ��������������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ����������ʼ
//////////////////////////////////////////////////////////////

struct stGiftUserCmd : public stNullUserCmd
{
	stGiftUserCmd()
	{
		byCmd = GIFT_USERCMD;
	}
};

//�����б�
const BYTE GET_LIST_GIFT_PARA = 1;
struct stGetListGift : public stGiftUserCmd
{
	stGetListGift()
	{
		byParam = GET_LIST_GIFT_PARA;
	}
};

struct actTitle
{
	DWORD id;
	char name[MAX_NAMESIZE];
	BYTE state;
};

//���ͻ�б�
const BYTE LIST_GIFT_PARA = 2;
struct stListGift : public stGiftUserCmd
{
	DWORD num;
	actTitle titles[0];
	stListGift()
	{
		byParam = LIST_GIFT_PARA;
		num = 0;
	}
};

//��������
const BYTE GET_DETAIL_GIFT_PARA = 3;
struct stGetDetailGift : public stGiftUserCmd
{
	DWORD id;
	stGetDetailGift()
	{
		byParam = GET_DETAIL_GIFT_PARA;
		id = 0;
	}
};

//���ͻ����
const BYTE DETAIL_GIFT_PARA = 4;
struct stDetailGift : public stGiftUserCmd
{
	DWORD id;//�ID
	char text[MAX_CHATINFO];
	stDetailGift()
	{
		byParam = DETAIL_GIFT_PARA;
		id = 0;
		bzero(text, sizeof(text));
	}
};

//��ȡ��Ʒ
const BYTE GET_ITEM_GIFT_PARA = 5;
struct stGetItemGift : public stGiftUserCmd
{
	DWORD id;
	stGetItemGift()
	{
		byParam = GET_ITEM_GIFT_PARA;
		id = 0;
	}
};

//���������
const BYTE GET_WINNER_GIFT_PARA = 6;
struct stGetWinnerGift : public stGiftUserCmd
{
	DWORD id;//�ID
	stGetWinnerGift()
	{
		byParam = GET_WINNER_GIFT_PARA;
		id = 0;
	}
};

//���ͻ�����
const BYTE WINNER_GIFT_PARA = 7;
struct stWinnerGift : public stGiftUserCmd
{
	DWORD num;
	char winners[0];
	stWinnerGift()
	{
		byParam = WINNER_GIFT_PARA;
		num = 0;
	}
};

//////////////////////////////////////////////////////////////
/// ��������������
//////////////////////////////////////////////////////////////

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ��������������ʼ
//////////////////////////////////////////////////////////////

struct stAllyUserCmd : public stNullUserCmd
{
	stAllyUserCmd()
	{
		byCmd = ALLY_USERCMD;
	}
};

enum
{
	QUESTION_COUNTRY_ALLY, // ��������
	YES_COUNTRY_ALLY, // ͬ������
	NO_COUNTRY_ALLY, // �ܾ�����
};

//�������
#define REQ_COUNTRY_ALLY_PARA  1
struct stReqCountryAlly : public stAllyUserCmd
{
	stReqCountryAlly()
	{
		byParam = REQ_COUNTRY_ALLY_PARA;
		dwCountryID = 0;
		dwAllyCountryID = 0;
		byStatus = QUESTION_COUNTRY_ALLY;
	}
	
	DWORD dwCountryID; // ��������Ĺ���,����ʱ������д
	DWORD dwAllyCountryID; // ������Ĺ���
	BYTE  byStatus;
};

// �����˹���Ϣ
#define UPDATE_COUNTRY_ALLY_PARA 2
struct stUpdateCountryAlly : public stAllyUserCmd
{
	stUpdateCountryAlly()
	{
		byParam = UPDATE_COUNTRY_ALLY_PARA;
		dwAllyCountryID = 0;
	}

	DWORD dwAllyCountryID;
};

// ȡ����Լ
#define CANCEL_COUNTRY_ALLY_PARA 3
struct stCancelCountryAlly : public stAllyUserCmd
{
	stCancelCountryAlly()
	{
		byParam = CANCEL_COUNTRY_ALLY_PARA;
		dwAllyCountryID = 0;
	}

	DWORD dwAllyCountryID;
};

// �������������Ϣ
#define REQ_COUNTRY_ALLY_INFO_PARA 4
struct stReqCountryAllyInfo : public stAllyUserCmd
{
	stReqCountryAllyInfo()
	{
		byParam = REQ_COUNTRY_ALLY_INFO_PARA;
	}
};

// ���ع���������Ϣ
#define RTN_COUNTRY_ALLY_INFO_PARA 5
struct stRtnCountryAllyInfo : public stAllyUserCmd
{
	stRtnCountryAllyInfo()
	{
		byParam = RTN_COUNTRY_ALLY_INFO_PARA;
		dwAllyCountryID = 0;
		dwFriendDegree = 0;
	}
	
	DWORD dwAllyCountryID;
	DWORD dwFriendDegree;
};

//////////////////////////////////////////////////////////////
/// ������������������
//////////////////////////////////////////////////////////////

/*
//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ����С��Ϸ���ʼ
//////////////////////////////////////////////////////////////

struct stMiniGameUserCmd : public stNullUserCmd
{
	stMiniGameUserCmd()
	{
		byCmd = MINIGAME_USERCMD;
	}
};

// ����
#define INVITE_MINI_PARA 1
struct stInviteMiniGame : public stMiniGameUserCmd
{
	stInviteMiniGame()
	{
		byParam = INVITE_MINI_PARA;
	}
	DWORD tempid;//������/�������ߵ���ʱID
	DWORD money;//��ע
};

// ����ظ�
#define INVITE_RET_MINI_PARA 2
struct stInviteRetMiniGame : public stMiniGameUserCmd
{
	stInviteRetMiniGame()
	{
		byParam = INVITE_RET_MINI_PARA;
	}
	DWORD tempid;//�����ߵ���ʱID
	DWORD money;//��ע
	DWORD ret;//1:ͬ�� 0:��ͬ��
};

// ��ʼ��Ϸ
#define START_MINI_PARA 3
struct stStartMiniGame : public stMiniGameUserCmd
{
	stStartMiniGame()
	{
		byParam = START_MINI_PARA;
	}
	DWORD tempid;//�Է�����ʱID
	DWORD money;//��ע
};

// ֹͣɫ��
#define STOP_DICE_MINI_PARA 4
struct stStopDiceMiniGame : public stMiniGameUserCmd
{
	stStopDiceMiniGame()
	{
		byParam = STOP_DICE_MINI_PARA;
	}
};

// ɫ������
#define DICE_NUM_MINI_PARA 5
struct stDiceNumMiniGame : public stMiniGameUserCmd
{
	stDiceNumMiniGame()
	{
		byParam = DICE_NUM_MINI_PARA;
	}
	DWORD tempid;//��ɫ���˵���ʱID
	DWORD num;
};

// ���
#define RESULT_MINI_PARA 6
struct stResultMiniGame : public stMiniGameUserCmd
{
	stResultMiniGame()
	{
		byParam = RESULT_MINI_PARA;
	}
	DWORD res;//0:�� 1:Ӯ 2:ƽ
};

// ��Ϸ���� //�����뿪��Ϸ
#define END_MINI_PARA 7
struct stEndMiniGame : public stMiniGameUserCmd
{
	stEndMiniGame()
	{
		byParam = END_MINI_PARA;

		reason = 0;
	}
	DWORD reason;//����ԭ�� 0:�������� 1:�Է��뿪
};

// ������Ϸ
#define CONTINUE_MINI_PARA 8
struct stContinueMiniGame : public stMiniGameUserCmd
{
	stContinueMiniGame()
	{
		byParam = CONTINUE_MINI_PARA;
	}
	DWORD ret;//0:������ 1:����
};

// ֪ͨ�ͻ��˽�����Ϸ
#define ENTER_MINI_PARA 9
struct stEnterMiniGame : public stMiniGameUserCmd
{
	stEnterMiniGame()
	{
		byParam = ENTER_MINI_PARA;
	}
};

//////////////////////////////////////////////////////////////
/// ����С��Ϸ�������
//////////////////////////////////////////////////////////////
*/

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// �����Ƽ���ϵͳ���ʼ
//////////////////////////////////////////////////////////////

struct stRecommendUserCmd : public stNullUserCmd
{
	stRecommendUserCmd()
	{
		byCmd = RECOMMEND_USERCMD;
	}
};

// ��д�Լ����Ƽ���,  <=15������Ҳ�����д
#define SET_RECOMMEND_PARA 1
struct stSetRecommendUserCmd : public stRecommendUserCmd
{
	stSetRecommendUserCmd()
	{
		byParam = SET_RECOMMEND_PARA;
		bzero(name, MAX_NAMESIZE);
	}

	char name[MAX_NAMESIZE];
};

enum
{
	QUERY_BOUNTY, // ��ѯ��ǰ������
	GET_BOUNTY,   // ��ȡ��ǰ������
};

// �Ƽ��˲�ѯ����ȡ����
#define REQ_RECOMMEND_BOUNTY_PARA 2
struct stReqRecommendBountyUserCmd : public stRecommendUserCmd
{
	stReqRecommendBountyUserCmd()
	{
		byParam = REQ_RECOMMEND_BOUNTY_PARA;
	}

	BYTE byState;
};

// ���Ƽ��˲�ѯ����ȡ����
#define REQ_RECOMMENDED_BOUNTY_PARA 3
struct stReqRecommendedBountyUserCmd : public stRecommendUserCmd
{
	stReqRecommendedBountyUserCmd()
	{
		byParam = REQ_RECOMMENDED_BOUNTY_PARA;
	}

	BYTE byState;
};

struct stRecommendItem
{
	char name[MAX_NAMESIZE]; // ���Ƽ�������
	DWORD dwLevel;	// ���һ����ȡʱ�ĵȼ�
	DWORD dwTotal;  // �ۼƹ��׶��ٽ��
};

// �����Ƽ��˽���
#define RTN_RECOMMEND_BOUNTY_PARA 4
struct stRtnRecommendBountyUserCmd : public stRecommendUserCmd
{
	stRtnRecommendBountyUserCmd()
	{
		byParam = RTN_RECOMMEND_BOUNTY_PARA;
		dwBalance = 0;
		dwTotal = 0;
		dwSize = 0;
	}

	DWORD dwBalance; // ����ȡ�Ľ��
	DWORD dwTotal;	// �ۼ�����ȡ�Ľ��
	DWORD dwSize;	// ���Ƽ�����Ŀ, �п����кܶ�,����������ڴ�С,�ܲ������ɿ�������
	stRecommendItem data[0];
};

// ���ر��Ƽ��˽���
#define RTN_RECOMMENDED_BOUNTY_PARA 5
struct stRtnRecommendedBountyUserCmd : public stRecommendUserCmd
{
	stRtnRecommendedBountyUserCmd()
	{
		byParam = RTN_RECOMMENDED_BOUNTY_PARA;
		dwBalance = 0;
		dwTotal = 0;
		dwLastLevel = 0;
		bzero(name, sizeof(name));
	}
	
	DWORD dwBalance; // ����ȡ�����
	DWORD dwTotal;   // �ۼ�����ȡ�Ľ��
	DWORD dwLastLevel; // �ϴ���ȡʱ�ĵȼ�
	char name[MAX_NAMESIZE]; // �Ƽ�������
};

//////////////////////////////////////////////////////////////
/// �����Ƽ���ϵͳ�������
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
/// ����Ʋ�����ϵͳָ�ʼ
//////////////////////////////////////////////////////////////
struct stSafetyUserCmd : public stNullUserCmd
{
	stSafetyUserCmd()
	{
		byCmd = SAFETY_USERCMD;
	}
};

// ������ñ���״̬
#define SET_SAFETY_PARA 1
struct stSetSafetyUserCmd : public stSafetyUserCmd
{
	stSetSafetyUserCmd()
	{
		byParam = SET_SAFETY_PARA;
		pwd = 0;
		state = 0;
	}

	DWORD pwd; // ��������
	BYTE  state; // ����״̬,0Ϊ�ر�,1Ϊ����,������������Ҫ������������
};

enum
{
	SAFETY_OPEN, // ����״̬,��״̬��,���ܽ��в�װ������
	SAFETY_TEMP_CLOSE, // ���ε�¼,��ʱ�ر�״̬,���Խ����κβ���
};

// �������ʱ,������֪ͨ�ͻ���,����Ƿ��ڱ���״̬
#define NOTIFY_SAFETY_PARA 2
struct stNotifySafetyUserCmd : public stSafetyUserCmd
{
	stNotifySafetyUserCmd()
	{
		byParam = NOTIFY_SAFETY_PARA;
		safe_setup = 0;
		byState = 0;
	}

	BYTE byState;
	BYTE safe_setup; 
};

// ��ҽ�����ʱδ����״̬,ֻ��һ�ε�¼����Ч,С�˺͹ؿͻ��˺�,��Ч
#define SET_TEMP_UNSAFETY_PARA 3
struct stSetTempUnSafetyUserCmd : public stSafetyUserCmd
{
	stSetTempUnSafetyUserCmd()
	{
		byParam = SET_TEMP_UNSAFETY_PARA;
		pwd = 0;
	}
	
	DWORD pwd;
};

enum
{
	SAFE_SPLIT_OBJECT, // ��װ��ʱ,��Ҫ��֤
	SAFE_THIRD_PACK,   // �ӵڶ�,�����������ó���Ʒ,��Ҫ��֤
	SAFE_GOLD_STOCK,   // ��ҹ����Ʊ
};
// ������Ҫ�����Ĺ���
// ��,set_state,clear_state,������
#define SET_SAFETY_DETAIL_PARA 4
struct stSetSafetyDetailUserCmd : public stSafetyUserCmd
{
	stSetSafetyDetailUserCmd()
	{
		byParam = SET_SAFETY_DETAIL_PARA;
		safe_setup=0;
		pwd = 0;
	}
	
	DWORD pwd;
	BYTE safe_setup;
};

//////////////////////////////////////////////////////////////
/// ����Ʋ�����ϵͳָ�����
//////////////////////////////////////////////////////////////

_CMD_NAMESPACE_END

#pragma pack()

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
