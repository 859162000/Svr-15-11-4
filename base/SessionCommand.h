/**
 * \file
 * \version  $Id: SessionCommand.h  $
 * \author  
 * \date 
 * \brief ����Session���������ָ��
 *
 */


#ifndef _SessionCommand_h_
#define _SessionCommand_h_

#include "zNullCmd.h"
#include "Union.h"
#include "Sept.h"
#include "Object.h"
#include "Command.h"

#pragma pack(1)

namespace Cmd
{

	namespace Session
	{
		const BYTE CMD_LOGIN = 1;
		const BYTE CMD_SCENE = 2;
		const BYTE CMD_GATE = 3;
		const BYTE CMD_FORWARD = 4;
		const BYTE CMD_SESSION = 5;
		const BYTE CMD_GMTOOL = 6;
		const BYTE CMD_SCENE_SHUTDOWN = 7;
		const BYTE CMD_SCENE_TMP = 8;	// ��ʱָ��
		const BYTE CMD_SCENE_SEPT = 9;	// ����ָ��
		const BYTE CMD_SCENE_COUNTRY = 10; // ����ָ��
		const BYTE CMD_SCENE_DARE = 11; // ��սָ��
		const BYTE CMD_SCENE_UNION = 12; // ���ָ��
		const BYTE CMD_SCENE_ARMY = 13; // ����ָ��
		const BYTE CMD_SCENE_GEM = 14;  // ����ָ��
		const BYTE CMD_SCENE_RECOMMEND = 15; // �Ƽ�ϵͳָ��

		//////////////////////////////////////////////////////////////
		/// ��½Session������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginSession : t_NullCmd
		{
			WORD wdServerID;
			WORD wdServerType;
			t_LoginSession()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN) {};
		};
		//////////////////////////////////////////////////////////////
		/// ��½Session������ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// ����������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_SCENE_REGSCENE = 1;
		struct t_regScene_SceneSession : t_NullCmd
		{
			DWORD dwID;
			DWORD dwTempID;
			char byName[MAX_NAMESIZE+1];
			char fileName[MAX_NAMESIZE+1];
			DWORD dwCountryID;
			BYTE byLevel;

			t_regScene_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_REGSCENE) {};
		};

		const BYTE PARA_SCENE_UNREGSCENE = 2;
		struct t_unregScene_SceneSession : t_NullCmd
		{
			DWORD dwTempID;
			t_unregScene_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_UNREGSCENE) {};
		};

		const BYTE PARA_SCENE_REGSCENE_RET = 3;
		const BYTE REGSCENE_RET_REGOK=	0;
		const BYTE REGSCENE_RET_REGERR=	2;
		struct t_regScene_ret_SceneSession : t_NullCmd
		{
			DWORD dwTempID;
			BYTE byValue;
			t_regScene_ret_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_REGSCENE_RET) {};
		};

		const BYTE PARA_SCENE_REGUSER = 4;
		struct t_regUser_SceneSession : t_NullCmd
		{
			DWORD accid;
			DWORD dwID;
			DWORD dwTempID;
			DWORD dwMapID;
			BYTE byName[MAX_NAMESIZE+1];
			BYTE byMapName[MAX_NAMESIZE+1];
			DWORD dwGatewayServerID;
			t_regUser_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_REGUSER) {};
		};

		const BYTE PARA_SCENE_UNREGUSER = 5;
		const BYTE UNREGUSER_RET_LOGOUT=0;
		const BYTE UNREGUSER_RET_ERROR=1;
		struct t_unregUser_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwSceneTempID;
			BYTE retcode;
			t_unregUser_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_UNREGUSER) {};
		};

		const BYTE PARA_SCENE_REGUSERSUCCESS = 6;  // ע��ɹ�֪ͨ�Ự������
		struct t_regUserSuccess_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwUnionID;
			DWORD dwSchoolID;
			DWORD dwSeptID;
			DWORD dwCountryID;
			DWORD dwGrace;
			DWORD dwExploit;
			QWORD qwExp;
			t_regUserSuccess_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_REGUSERSUCCESS) {};
		};

		const BYTE PARA_SCENE_LEVELUPNOTIFY = 7;  // �������֪ͨ�Ự������
		struct t_levelupNotify_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			WORD level;
			QWORD qwExp;
			t_levelupNotify_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_LEVELUPNOTIFY) {};
		};


		const BYTE PARA_SCENE_CHANEG_SCENE = 8;  //�л�����
		struct t_changeScene_SceneSession : t_NullCmd
		{
			DWORD id;
			DWORD temp_id;
			DWORD x;
			DWORD y;
			DWORD map_id;
			BYTE map_file[MAX_NAMESIZE];
			BYTE map_name[MAX_NAMESIZE];			
			t_changeScene_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CHANEG_SCENE)
				{
					bzero(map_file, sizeof(map_file));
					bzero(map_name, sizeof(map_name));
				}
		};

		const BYTE PARA_SCENE_GM_COMMAND = 9;  //GMָ��
		const BYTE GM_COMMAND_FINDUSER = 1;  //ָ��finduser
		const BYTE GM_COMMAND_GOTOUSER = 2;  //ָ��gotouser
		const BYTE GM_COMMAND_CATCHUSER = 3;  //ָ��catchuser
		const BYTE GM_COMMAND_DONTTALK = 4;  //ָ��donttalk
		const BYTE GM_COMMAND_TALK = 5;  //ָ��talk
		const BYTE GM_COMMAND_KICK = 6;  //ָ��kick
		const BYTE GM_COMMAND_SETPRIV = 7;  //ָ��setpriv
		const BYTE GM_COMMAND_LOCKVALUE = 8;  //ָ��lockvalue
		const BYTE GM_COMMAND_LEVELUP = 9;  //ָ��levelup
		const BYTE GM_COMMAND_LOAD_GIFT = 10;  //ָ��loadgift
		const BYTE GM_COMMAND_LOAD_PROCESS = 11;  //ָ��loadprocess
		const BYTE GM_COMMAND_EMBAR = 12;  //ָ��embar
		const BYTE GM_COMMAND_NEWZONE = 13;  //����������
		const BYTE GM_COMMAND_REFRESH_GENERAL = 14;  //ˢ�´󽫾�

		const BYTE GM_COMMAND_STATE_REQ = 1;  //ָ��״̬req��ʾ����
		const BYTE GM_COMMAND_STATE_RET = 2;  //ָ��״̬ret��ʾ���ؽ��

		const BYTE GM_COMMAND_ERR_NOERR = 0;  //û�д���
		const BYTE GM_COMMAND_ERR_NOUSER = 1;  //��Ҳ�����
		const BYTE GM_COMMAND_ERR_PRIV = 2;  //Ȩ�޲���
		const BYTE GM_COMMAND_ERR_PARAM = 3;  //��������
		const BYTE GM_COMMAND_ERR_FAIL = 4;  //ָ��ִ��ʧ��
		struct t_gmCommand_SceneSession : t_NullCmd
		{
			BYTE gm_cmd;//GMָ��
			BYTE cmd_state;//ָ��ִ��״̬
			BYTE err_code;//������Ϣ
			DWORD id;//�û�ID
			BYTE src_priv;//ʹ��ָ����Ȩ��
			DWORD x;//xy�������ݸ��ӵĲ���
			DWORD y;
			BYTE src_name[MAX_NAMESIZE];
			BYTE dst_name[MAX_NAMESIZE];
			BYTE map_file[MAX_NAMESIZE];
			BYTE map_name[MAX_NAMESIZE];			
			t_gmCommand_SceneSession() 
				: t_NullCmd(CMD_SCENE, PARA_SCENE_GM_COMMAND),
			gm_cmd(0),cmd_state(1),err_code(0),id(0),src_priv(1),x(0),y(0)
			{
				src_name[0] = 0;
				dst_name[0] = 0;
				map_file[0] = 0;
				map_name[0] = 0;
			};
		};

		const BYTE PARA_SCENE_PRIVATE_CHAT = 10;  //�糡��˽��
		const BYTE PRIVATE_CHAT_ERR_NOUSER = 1;  //��Ҳ�����
		const BYTE PRIVATE_CHAT_ERR_FILTER = 2;  //�Է�û����˽��
		const BYTE PRIVATE_CHAT_ACT_INVITE = 1;  //����
		const BYTE PRIVATE_CHAT_ACT_JOIN = 2;  //����
		const BYTE PRIVATE_CHAT_ACT_LEAVE = 3;  //�뿪
		struct t_privateChat_SceneSession : t_NullCmd
		{
			BYTE act;//����
			BYTE err_code;//������Ϣ
			BYTE src_name[MAX_NAMESIZE];
			BYTE dst_name[MAX_NAMESIZE];
			DWORD cmd_size;
			BYTE chat_cmd[0];
			t_privateChat_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_PRIVATE_CHAT) {};
		};

		const BYTE PARA_SCENE_UNLOAD_SCENE = 11;  //ֹͣע��
		struct t_unloadScene_SceneSession : t_NullCmd
		{
			DWORD map_id;
			t_unloadScene_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_UNLOAD_SCENE) {};
		};

		const BYTE PARA_SCENE_SYS_SETTING = 12;  //ϵͳ����
		struct t_sysSetting_SceneSession : t_NullCmd
		{
			BYTE name[MAX_NAMESIZE];
			BYTE sysSetting[20];
			DWORD face;
			t_sysSetting_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_SYS_SETTING) {};
		};

		const BYTE PARA_SCENE_CITY_RUSH = 13;  //���﹥�ǵ�֪ͨ
		struct t_cityRush_SceneSession : t_NullCmd
		{
			char bossName[MAX_NAMESIZE];
			char rushName[MAX_NAMESIZE];
			char mapName[MAX_NAMESIZE];
			DWORD delay;
			DWORD countryID;
			t_cityRush_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CITY_RUSH) {};
		};

		const BYTE PARA_SCENE_CREATE_SCHOOL = 16;  //GMָ�������
		struct t_createSchool_SceneSession : t_NullCmd
		{
			char masterName[MAX_NAMESIZE];
			char schoolName[MAX_NAMESIZE];
			t_createSchool_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CREATE_SCHOOL) {};
		};
		
		

		const BYTE PARA_SCENE_CREATE_DARE = 17;  //������սָ��
		struct t_createDare_SceneSession : t_NullCmd
		{
			//char relation1[MAX_NAMESIZE];	// ��ս������ϵ����
			//char relation2[MAX_NAMESIZE];	// Ӧս������ϵ����
			//DWORD relationID1;		// ��ս������ϵID(��ս������һ����������)
			DWORD relationID2;		// Ӧս������ϵID

			DWORD ready_time;		// ��սǰ�ĵȴ�ʱ��
			DWORD active_time;		// ��ս����ʱ��
			BYTE type;  			//��ս����
			
			t_createDare_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CREATE_DARE) 
				{
					ready_time = 0;
					active_time = 0;
				};
		};

		const BYTE PARA_SCENE_CREATE_QUIZ = 19;
		struct t_createQuiz_SceneSession : t_NullCmd
		{
			DWORD ready_time;
			DWORD active_time;
			DWORD dwUserID;	  
			DWORD dwSubjects; // ����Ŀ��
			BYTE  type;	  // ��������
			BYTE  subject_type; // �������
			
			t_createQuiz_SceneSession() : t_NullCmd(CMD_SCENE, PARA_SCENE_CREATE_QUIZ)
			{
				ready_time = 0;
				active_time = 0;
				dwUserID = 0;
				type = 0;
				subject_type = 0;
			}
		};

		//////////////////////////////////////////////////////////////
		///  ����������֮��սָ��
		//////////////////////////////////////////////////////////////

		const BYTE PARA_SCENE_REMOVE_SCENE = 14;  //ж�ص�ͼ
		struct t_removeScene_SceneSession : t_NullCmd
		{
			DWORD map_id;
			t_removeScene_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_REMOVE_SCENE) {};
		};

		const BYTE PARA_SCENE_REQ_ADD_SCENE = 15;
		struct t_reqAddScene_SceneSession : t_NullCmd
		{
			DWORD dwServerID;
			DWORD dwCountryID;
			DWORD dwMapID;
			t_reqAddScene_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_REQ_ADD_SCENE) {};
		};

		///ֱ��ת�����û���ָ��
		const BYTE PARA_SCENE_FORWARD_USER = 18;
		struct t_forwardUser_SceneSession : t_NullCmd
		{
			DWORD id;
			DWORD tempid;
			char name[MAX_NAMESIZE];
			DWORD cmd_len;
			BYTE cmd[0];
			t_forwardUser_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_FORWARD_USER)
				{
					id = 0;
					tempid = 0;
					bzero(name, MAX_NAMESIZE);
				}
		};

		///֪ͨ��������һ�����
		const BYTE PARA_SCENE_FORBID_TALK = 20;
		struct t_forbidTalk_SceneSession : t_NullCmd
		{
			char name[MAX_NAMESIZE];
			char reason[128];
			WORD operation;
			int delay;
			char gm[MAX_NAMESIZE];
			t_forbidTalk_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_FORBID_TALK)
				{
					bzero(name, sizeof(name));
					bzero(reason, sizeof(reason));
					operation = 0;
					delay = 0;
					bzero(gm, sizeof(gm));
				}
		};

		///֪ͨ�������ʧ��
		const BYTE PARA_SCENE_GUARD_FAIL = 21;
		struct t_guardFail_SceneSession : t_NullCmd
		{
			DWORD userID;
			BYTE type;//0: ������ 1:ʱ����� 
			t_guardFail_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_GUARD_FAIL)
				{
					userID = 0;
				}
		};

		// ����֪ͨsession������ѽ��
		const BYTE PARA_SPEND_GOLD = 22;
		struct t_SpendGold_SceneSession : t_NullCmd
		{
			DWORD userID;
			DWORD gold;
			t_SpendGold_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SPEND_GOLD) {}
		};

		// ����֪ͨsession���ظ��ַ���
		const DWORD SERVICE_MAIL = 1;
		const DWORD SERVICE_AUCTION = 2;
		const DWORD SERVICE_PROCESS = 4;
		const DWORD SERVICE_HORSE = 8;
		const BYTE PARA_SET_SERVICE = 23;
		struct t_SetService_SceneSession : t_NullCmd
		{
			DWORD flag;
			t_SetService_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SET_SERVICE)
				{
					flag = 0;
				}
		};

		const BYTE PARA_SCENE_CITY_RUSH_CUST = 24;  //�Զ�����﹥�ǵ�֪ͨ
		struct t_cityRushCust_SceneSession : t_NullCmd
		{
			char text[128];
			DWORD countryID;
			t_cityRushCust_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CITY_RUSH_CUST) {};
		};

		const BYTE PARA_SCENE_BAIL_CHECK = 25;  //ȡ����ҽ�ļ��
		struct t_bailCheck_SceneSession : t_NullCmd
		{
			char name[MAX_NAMESIZE];
			DWORD money;
			t_bailCheck_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_BAIL_CHECK) {};
		};

		/*
		const BYTE PARA_SCENE_LOAD_PROCESS = 26;  //�����������ļ�
		struct t_loadProcess_SceneSession : t_NullCmd
		{
			t_loadProcess_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_LOAD_PROCESS) {};
		};
		*/

		struct giftInfo
		{
			DWORD actID;
			DWORD charID;
			char name[MAX_NAMESIZE];
			BYTE itemGot;
			char mailText[MAX_CHATINFO];
			DWORD money;
			DWORD itemID;
			BYTE itemType;
			DWORD itemNum;
			BYTE bind;
		};
		const BYTE PARA_SCENE_SEND_GIFT = 27;  //������Ʒ�ʼ���session�����������Ʒ
		struct t_sendGift_SceneSession : t_NullCmd
		{
			giftInfo info;
			t_sendGift_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_SEND_GIFT) {};
		};

		// ����ȫ��֪ͨ
		const BYTE PARA_SERVER_NOTIFY = 28;
		struct t_serverNotify_SceneSession : t_NullCmd
		{
			int infoType;
			char info[MAX_CHATINFO];

			t_serverNotify_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SERVER_NOTIFY)
				{
					infoType = Cmd::INFO_TYPE_GAME;
					bzero(info, sizeof(info));
				}
		};

		// ����һ��������GM����
		const BYTE PARA_BROADCAST_SCENE = 29;
		struct t_broadcastScene_SceneSession : t_NullCmd
		{
			int infoType;
			DWORD mapID;
			char GM[MAX_NAMESIZE];
			char info[MAX_CHATINFO];

			t_broadcastScene_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_BROADCAST_SCENE)
				{
					infoType = Cmd::INFO_TYPE_SCROLL;
					mapID = 0;
					bzero(info, sizeof(info));
					bzero(GM, sizeof(GM));
				}
		};

		const BYTE PARA_SCENE_SEND_CMD = 30;
		struct t_sendCmd_SceneSession : t_NullCmd
		{
			DWORD mapID;
			DWORD len;
			BYTE cmd[0];
			
			t_sendCmd_SceneSession() : t_NullCmd(CMD_SCENE, PARA_SCENE_SEND_CMD)
			{
				mapID = 0;
				len = 0;
			}
		};

		//////////////////////////////////////////////////////////////
		/// ����������֮����ϵ����ָ��
		//////////////////////////////////////////////////////////////
		enum{
			RELATION_TYPE_COUNTRY,
			RELATION_TYPE_SEPT,
			RELATION_TYPE_SCHOOL,
			RELATION_TYPE_UNION,
			RELATION_TYPE_NOTIFY
		};
		/// ��ɫ�ɹ�������˳�����֪ͨ�����������Ӧ������ϵID��ֵ
		const BYTE PARA_SEND_USER_RELATION_ID = 50;
		struct t_sendUserRelationID : t_NullCmd
		{
			DWORD dwUserID;			/// ��ɫID
			BYTE type;			/// ����ϵ����
			DWORD dwID;			/// ����ϵID
			char name[MAX_NAMESIZE];	/// ����ϵ����
			DWORD caption;			/// ͷ�Σ�������ڳ�MAPID,������ϵ����ʦͽ��ʱ������ֶη�ʦ����CHARID
			bool  unionmaster;		/// �Ƿ�Ϊ����
			bool  septmaster;		/// �Ƿ�Ϊ�峤
			bool  king;			/// �Ƿ�Ϊ����
			DWORD dwActionPoint;		/// �ж���
			DWORD dwRepute;			/// ��������
			DWORD dwSeptLevel;		/// ����ȼ�
			bool  emperor;			/// �Ƿ�Ϊ�ʵ�

			t_sendUserRelationID()
				: t_NullCmd(CMD_SCENE, PARA_SEND_USER_RELATION_ID) {
					dwActionPoint = 0;
					dwRepute = 0;
					king = false;
					emperor = false;
					septmaster = false;
					unionmaster = false;
					bzero(name, MAX_NAMESIZE);
				};
		};
		
		/// �û�����ʱ����������ż��ϵ
		const BYTE PARA_UPDATE_CONSORT = 51;
		struct t_updateConsort : t_NullCmd
		{
			DWORD dwConsort;
			DWORD dwUserID;
			BYTE  byKingConsort;
			t_updateConsort() : t_NullCmd(CMD_SCENE, PARA_UPDATE_CONSORT) 
			{
				byKingConsort = 0;
			};
		};
		
		/// �û�����ʱ������������Ǻ�
		const BYTE PARA_UPDATE_COUNTRY_STAR = 52;
		struct t_updateCountryStar : t_NullCmd
		{
			DWORD dwCountryStar;
			DWORD dwUserID;
			
			t_updateCountryStar() : t_NullCmd(CMD_SCENE, PARA_UPDATE_COUNTRY_STAR) {};
		};

		/// ���ӳ���
		const BYTE PARA_ADD_RELATION_ENEMY = 53;
		struct t_addRelationEnemy : t_NullCmd
		{
			DWORD dwUserID;
			char name[MAX_NAMESIZE];
			
			t_addRelationEnemy() : t_NullCmd(CMD_SCENE, PARA_ADD_RELATION_ENEMY) {};
		};

		/// ֪ͨ���߽�ɫ��ռ���NPC���
		const BYTE PARA_NOTIFY_NPC_HOLD_DATA = 54;
		struct t_notifyNpcHoldData : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwMapID;
			DWORD dwPosX;
			DWORD dwPosY;
			t_notifyNpcHoldData() : t_NullCmd(CMD_SCENE, PARA_NOTIFY_NPC_HOLD_DATA) {};
		};

		/// ֪ͨ���߽�ɫ��ռ���NPC���
		const BYTE PARA_NOTIFY_ADD_INTEGRAL = 55;
		struct t_notifyAddIntegral : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwGoldUser;
			DWORD dwNum;
			t_notifyAddIntegral() : t_NullCmd(CMD_SCENE, PARA_NOTIFY_ADD_INTEGRAL) {};
		};

		//////////////////////////////////////////////////////////////
		/// ����������֮����ϵ����ָ��
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		/// ����������֮���ָ��
		//////////////////////////////////////////////////////////////
		// ���Ӱ��
		const BYTE PARA_UNION_ADDUNION = 101;
		struct t_addUnion_SceneSession : t_NullCmd
		{
			DWORD dwItemID;             /// ������Ʒ�Ķ���id �ɹ��Ժ�ɾ��
			DWORD byRetcode;            /// ��������ʱ��0 ��ʾ����ʧ�������ظ��� 1��ʾ�ɹ�
			DWORD dwMapTempID;          /// ��ͼ��ʱ���
			UnionDef::stUnionInfo info;
			t_addUnion_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_UNION_ADDUNION) {};
		};

		// ���ӳ�Ա
		const BYTE PARA_UNION_ADDMEMBER = 102;
		struct t_addUnionMember_SceneSession : t_NullCmd
		{
			DWORD dwUnionID;
			UnionDef::stUnionMemberInfo member;
			t_addUnionMember_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_UNION_ADDMEMBER) {};
		};
		// ������Ա
		const BYTE PARA_UNION_FIREMEMBER = 103;
		struct t_fireUnionMember_SceneSession : t_NullCmd
		{
			DWORD dwCharID;
			DWORD dwMapTempID;
			t_fireUnionMember_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_UNION_FIREMEMBER) {}
		};
		//////////////////////////////////////////////////////////////
		/// ����������֮���ָ��
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		/// ����������֮����ָ��
		//////////////////////////////////////////////////////////////
		// ���Ӱ��
		const BYTE PARA_SEPT_ADDSEPT = 121;
		struct t_addSept_SceneSession : t_NullCmd
		{
			DWORD byRetcode;            /// ��������ʱ��0 ��ʾ����ʧ�������ظ��� 1��ʾ�ɹ�
			DWORD dwMapTempID;          /// ��ͼ��ʱ���
			SeptDef::stSeptInfo info;
			t_addSept_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SEPT_ADDSEPT) 
				{
					byRetcode = 0;
					bzero(&info, sizeof(info));
				};
		};

		// ���ӳ�Ա
		const BYTE PARA_SEPT_ADDMEMBER = 122;
		struct t_addSeptMember_SceneSession : t_NullCmd
		{
			DWORD dwSeptID;
			SeptDef::stSeptMemberInfo member;
			t_addSeptMember_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SEPT_ADDMEMBER) {};
		};
		// ������Ա
		const BYTE PARA_SEPT_FIREMEMBER = 123;
		struct t_fireSeptMember_SceneSession : t_NullCmd
		{
			DWORD dwCharID;
			DWORD dwMapTempID;
			t_fireSeptMember_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SEPT_FIREMEMBER) {}
		};
		
		// �������
		const BYTE PARA_SEPT_EXP_DISTRIBUTE = 124;
		struct t_distributeSeptExp_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwExp;
			t_distributeSeptExp_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SEPT_EXP_DISTRIBUTE) {}
		};

		// ����ȫ��֪ͨ
		const BYTE PARA_COUNTRY_NOTIFY = 125;
		struct t_countryNotify_SceneSession : t_NullCmd
		{
			int infoType;
			DWORD dwCountryID;
			char info[MAX_CHATINFO];

			t_countryNotify_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_COUNTRY_NOTIFY)
				{
					infoType = Cmd::INFO_TYPE_GAME;
				}
		};

		
		// ��������NPC֮��ս����
		const BYTE PARA_SEPT_NPCDARE_DARE = 127;
		struct t_NpcDare_Dare_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwCountryID;
			DWORD dwMapID;
			DWORD dwNpcID;
			t_NpcDare_Dare_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SEPT_NPCDARE_DARE) {}
		};

		// ��������NPC֮����֪ͨ
		const BYTE PARA_SEPT_NPCDARE_NOTIFYSCENE = 128;
		struct t_NpcDare_NotifyScene_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwCountryID;
			DWORD dwMapID;
			DWORD dwNpcID;
			DWORD dwPosX;
			DWORD dwPoxY;
			t_NpcDare_NotifyScene_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SEPT_NPCDARE_NOTIFYSCENE) {}
		};

		// ��������NPC֮�������
		const BYTE PARA_SEPT_NPCDARE_RESULT = 129;
		struct t_NpcDare_Result_SceneSession : t_NullCmd
		{
			DWORD dwSeptID;
			t_NpcDare_Result_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SEPT_NPCDARE_RESULT) {}
		};

		// ��������NPC֮�������
		const BYTE PARA_SEPT_NPCDARE_GETGOLD = 130;
		struct t_NpcDare_GetGold_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwGold;
			DWORD dwNpcID;
			DWORD dwMapID;
			DWORD dwCountryID;

			t_NpcDare_GetGold_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SEPT_NPCDARE_GETGOLD) {}
		};

		// ��������NPC֮���߷���
		const BYTE PARA_SEPT_NPCDARE_ITEMBACK = 131;
		struct t_NpcDare_ItemBack_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			t_NpcDare_ItemBack_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SEPT_NPCDARE_ITEMBACK) {}
		};

		// ��������NPC֮GMָ�����������ս
		const BYTE PARA_SEPT_NPCDARE_GMCMD = 132;
		struct t_NpcDare_GMCmd_SceneSession : t_NullCmd
		{
			t_NpcDare_GMCmd_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SEPT_NPCDARE_GMCMD) {}
		};

		
		// �ɳ��������Ự����ѯ��������
		const BYTE PARA_QUESTION_NPCDARE = 135;
		struct t_questionNpcDare_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwCountryID;
			DWORD dwMapID;
			DWORD dwNpcID;
			BYTE  byType;

			t_questionNpcDare_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_QUESTION_NPCDARE) {};
		};

		// �ɻỰ����������֪ͨ����˰��
		const BYTE PARA_TAX_COUNTRY = 136;
		struct t_taxCountry_SceneSession : t_NullCmd
		{
			DWORD dwCountryID;
			DWORD dwTempID;
			BYTE  byTax;

			t_taxCountry_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_TAX_COUNTRY) {};
		};

		// �ɳ������Ự��֪ͨ����˰������
		const BYTE PARA_TAXADD_COUNTRY = 137;
		struct t_taxAddCountry_SceneSession : t_NullCmd
		{
			DWORD dwCountryID;
			QWORD qwTaxMoney;
			t_taxAddCountry_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_TAXADD_COUNTRY) {};
		};
		//////////////////////////////////////////////////////////////
		/// ����������֮����ָ��
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		/// ����������֮���Ѷ�ָ��
		//////////////////////////////////////////////////////////////
		struct stCountMember
		{
			DWORD	dwUserID;
			WORD	wdDegree;
			BYTE	byType;
		};

		struct stRequestMember
		{
			char name[MAX_NAMESIZE];
		};

		enum{
			TYPE_FRIEND,	// ��������
			TYPE_CONSORT,	// ��������
			TYPE_TEACHER,	// ʦ������
			TYPE_PRENTICE,	// ͽ������
		};
		struct stRequestReturnMember
		{
			DWORD dwUserID;
			WORD  wdDegree;
			BYTE  byType; // ʹ��ö�� RELATION_TYPE
		};

		// ��Ự����������������е��Ѻö�
		const BYTE PARA_FRIENDDEGREE_COUNT = 150;
		struct t_CountFriendDegree_SceneSession : t_NullCmd
		{
			char		name[MAX_NAMESIZE];	// �������
			WORD		size;				// �Ŷ��б�Ĵ�С
			stCountMember	namelist[0];		// ʣ���Ŷ��б�
			t_CountFriendDegree_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_FRIENDDEGREE_COUNT) {}
		};

		// ��Ự���������������Ѻöȹ�ϵ�б�����ַ���������Ա����
		const BYTE PARA_FRIENDDEGREE_REQUEST = 151;
		struct t_RequestFriendDegree_SceneSession : t_NullCmd
		{
			WORD  size;				// ��������
			stRequestMember namelist[0];	// �Ŷ��б�
			t_RequestFriendDegree_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_FRIENDDEGREE_REQUEST) {}
		};


		// �Ự�������볡���������е���Ӷ�Ա�ַ��Ѻöȹ�ϵ�б�
		const BYTE PARA_FRIENDDEGREE_RETURN = 152;
		struct t_ReturnFriendDegree_SceneSession : t_NullCmd
		{
			DWORD dwID;				// �ӳ���tempID
			DWORD dwMapTempID;		// ��ͼ��tempID
			WORD  size;				// ��������
			stRequestReturnMember memberlist[0];	// �Ŷ��б�
			t_ReturnFriendDegree_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_FRIENDDEGREE_RETURN) {}
		};
		//////////////////////////////////////////////////////////////
		/// ����������֮���Ѷ�ָ��
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		/// ����������֮����ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_SCHOOL_CREATE_SUCCESS = 170;
		struct t_SchoolCreateSuccess_SceneSession : t_NullCmd
		{
			DWORD	dwID;						// ʦ���ID
			DWORD	dwSchoolID;					// ����ID
			char	schoolName[MAX_NAMESIZE];	// ��������
			t_SchoolCreateSuccess_SceneSession()
				:	t_NullCmd(CMD_SCENE, PARA_SCHOOL_CREATE_SUCCESS) {}
		};
		//////////////////////////////////////////////////////////////
		/// ����������֮����ָ��
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		///  ����������֮��ͨ����ָ�ʼ
		//////////////////////////////////////////////////////////////

		const BYTE PARA_CARTOON_CMD = 179;
		struct t_CartoonCmd : t_NullCmd
		{
			BYTE cartoonPara;
			t_CartoonCmd()
				: t_NullCmd(CMD_SCENE, PARA_CARTOON_CMD) 
				{
					cartoonPara = 0;
				};
		};

		const BYTE PARA_CARTOON_BUY = 1;
		struct t_buyCartoon_SceneSession : t_CartoonCmd
		{
			Cmd::t_CartoonData data;
			t_buyCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_BUY;

					bzero(&data, sizeof(data));
				};
		};

		const BYTE PARA_CARTOON_ADD = 2;
		struct t_addCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;//ID
			DWORD cartoonID;
			Cmd::t_CartoonData data;
			t_addCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_ADD;

					userID = 0;
					cartoonID = 0;
					bzero(&data, sizeof(Cmd::t_CartoonData));
				};
		};

		const BYTE PARA_CARTOON_GET_LIST = 3;
		struct t_getListCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;//ID
			t_getListCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_GET_LIST;

					userID = 0;
				};
		};

		enum saveType
		{
			SAVE_TYPE_DONTSAVE,
			SAVE_TYPE_PUTAWAY,
			SAVE_TYPE_LETOUT,
			SAVE_TYPE_ADOPT,
			SAVE_TYPE_RETURN,
			SAVE_TYPE_TIMEOVER,
			SAVE_TYPE_TIMETICK,
			//SAVE_TYPE_CHARGE,
			SAVE_TYPE_SYN
		};
		const BYTE PARA_CARTOON_SAVE = 4;
		struct t_saveCartoon_SceneSession : t_CartoonCmd
		{
			char userName[MAX_NAMESIZE];
			saveType type;
			DWORD cartoonID;//ID
			Cmd::t_CartoonData data;
			t_saveCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_SAVE;

					bzero(userName, MAX_NAMESIZE);
					type = SAVE_TYPE_DONTSAVE;
					cartoonID = 0;
					bzero(&data, sizeof(Cmd::t_CartoonData));
				};
		};

		const BYTE PARA_CARTOON_ADOPT = 5;
		struct t_adoptCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;//ID
			DWORD cartoonID;
			BYTE masterState;//����ʱ�������˵�״̬ 0:������ 1:����
			Cmd::t_CartoonData data;
			t_adoptCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_ADOPT;

					userID = 0;
					cartoonID = 0;
					masterState = 0;
					bzero(&data, sizeof(Cmd::t_CartoonData));
				};
		};

		const BYTE PARA_CARTOON_GET_BACK = 6;
		struct t_getBackCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;//ID
			DWORD cartoonID;
			t_getBackCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_GET_BACK;

					userID = 0;
					cartoonID = 0;
				};
		};

		const BYTE PARA_CARTOON_NOTIFY = 7;//֪ͨ����������������
		struct t_notifyCartoon_SceneSession : t_CartoonCmd
		{
			char adopter[MAX_NAMESIZE];
			BYTE state;//0:���� 1:����
			DWORD cartoonID;
			t_notifyCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_NOTIFY;

					bzero(adopter, MAX_NAMESIZE);
					state = 0;
					cartoonID = 0;
				};
		};

		const BYTE PARA_CARTOON_LOAD = 8;//����ʱ����ȡ�����б�
		struct t_loadCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;
			t_loadCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_LOAD;

					userID = 0;
				};
		};

		const BYTE PARA_CARTOON_CORRECT = 9;//�������Ϊ����������������û�г���ʱ����Ϊ�ȴ�
		struct t_correctCartoon_SceneSession : t_CartoonCmd
		{
			DWORD cartoonID;
			t_correctCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_CORRECT;

					cartoonID = 0;
				};
		};

		const BYTE PARA_CARTOON_SALE = 10;//����ʱ����ȡ�����б�
		struct t_saleCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;
			DWORD cartoonID;
			t_saleCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_SALE;

					userID = 0;
					cartoonID = 0;
				};
		};

		const BYTE PARA_CARTOON_CHARGE = 11;//��ֵ
		struct t_chargeCartoon_SceneSession : t_CartoonCmd
		{
			DWORD masterID;
			DWORD cartoonID;
			DWORD time;//����
			t_chargeCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_CHARGE;

					masterID = 0;
					cartoonID = 0;
					time = 0;
				};
		};

		const BYTE PARA_CARTOON_CHARGE_NOTIFY = 12;//֪ͨ�����߳����ֵ
		struct t_chargeNotifyCartoon_SceneSession : t_CartoonCmd
		{
			char adopter[MAX_NAMESIZE];
			DWORD cartoonID;
			DWORD time;//����
			t_chargeNotifyCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_CHARGE_NOTIFY;

					bzero(adopter, MAX_NAMESIZE);
					cartoonID = 0;
					time = 0;
				};
		};

		const BYTE PARA_CARTOON_LEVEL_NOTIFY = 13;//֪ͨ�����߳����ֵ
		struct t_levelNotifyCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;
			DWORD cartoonID;
			DWORD level;
			t_levelNotifyCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_LEVEL_NOTIFY;

					userID = 0;
					cartoonID = 0;
					level = 0;
				};
		};

		const BYTE PARA_CARTOON_DRAW = 14;//��ȡ����
		struct t_drawCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;
			DWORD cartoonID;
			DWORD num;
			t_drawCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_DRAW;

					userID = 0;
					cartoonID = 0;
					num = 0;
				};
		};

		const BYTE PARA_CARTOON_CONSIGN = 15;//ί������
		struct t_consignCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;//ί����ID
			DWORD cartoonID;//ί�еĳ���ID
			char name[MAX_NAMESIZE];//��ί�������� �������ʱ��ί���˵�����
			t_consignCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_CONSIGN;

					userID = 0;
					cartoonID = 0;
					bzero(name, sizeof(name));
				};
		};

		const BYTE PARA_CARTOON_CONSIGN_RET = 16;//ί����������
		struct t_consignRetCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;//��ί����ID
			DWORD cartoonID;
			BYTE ret;//0���ܾ� 1��ͬ�� 2���Ѿ�����5����
			t_consignRetCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_CONSIGN_RET;

					userID = 0;
					ret = 0;
					cartoonID = 0;
				};
		};

		const BYTE PARA_CARTOON_SET_REPAIR = 17;//�����Զ�����
		struct t_setRepairCartoon_SceneSession : t_CartoonCmd
		{
			DWORD userID;//����ID
			DWORD cartoonID;
			BYTE repair;//0���ر� 1����
			t_setRepairCartoon_SceneSession()
				: t_CartoonCmd()
				{
					cartoonPara = PARA_CARTOON_SET_REPAIR;

					userID = 0;
					cartoonID = 0;
					repair = 0;
				};
		};

		//////////////////////////////////////////////////////////////
		///  ����������֮��ͨ����ָ�����
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		///  ����������֮�ʼ�ָ�ʼ
		//////////////////////////////////////////////////////////////
		// �ɳ��������������ͣ�����ҷ��ʼ�

		enum mailType
		{
			MAIL_TYPE_MAIL		= 1,	//�ʼ�
			MAIL_TYPE_MSG	 	= 2,	//����
			MAIL_TYPE_RETURN	=3,	//�˻ص��ʼ�
			MAIL_TYPE_AUCTION	=4,	//�������ʼ�
			MAIL_TYPE_SYS		=5	//ϵͳ�ʼ��������˻�
		};
		enum mailState
		{
			MAIL_STATE_NEW		= 1,
			MAIL_STATE_OPENED	= 2,
			MAIL_STATE_DEL		= 3
		};

		struct mailInfo
		{
			//DWORD id;
			BYTE state;
			char fromName[MAX_NAMESIZE+1];
			char toName[MAX_NAMESIZE+1];
			char title[MAX_NAMESIZE+1];
			BYTE type;
			DWORD createTime;
			DWORD delTime;
			BYTE accessory;
			BYTE itemGot;
			char text[256];
			DWORD sendMoney;
			DWORD recvMoney;
			DWORD sendGold;
			DWORD recvGold;
			DWORD fromID;
			DWORD toID;
			DWORD itemID;
		};

		/*
		enum
		{
			CHECKSEND_RETURN_OK,
			CHECKSEND_RETURN_FAILED,
			CHECKSEND_RETURN_NOPLAYER,
			CHECKSEND_RETURN_FULL
		};
		*/
		const BYTE PARA_SCENE_CHECKSEND = 180;
		struct t_checkSend_SceneSession : t_NullCmd
		{
			mailInfo mail;
			DWORD itemID;
			//BYTE retCode;
			t_checkSend_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CHECKSEND) 
				{
					bzero(&mail, sizeof(mail));
					itemID = 0;
					//retCode = CHECKSEND_RETURN_FAILED;
				};
		};
		/*
		const BYTE PARA_SCENE_CHECKRETURN = 301;
		struct t_checkReturn_SceneSession : t_NullCmd
		{
			mailInfo mail;
			DWORD itemID;
			BYTE retCode;
			t_checkReturn_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CHECKRETURN) 
				{
					bzero(&mail, sizeof(mail));
					itemID = 0;
					retCode = CHECKSEND_RETURN_FAILED;
				};
		};
		*/

		struct SessionObject
		{
			time_t createtime;
			t_Object object;
		};
		const BYTE PARA_SCENE_SENDMAIL = 181;
		struct t_sendMail_SceneSession : t_NullCmd
		{
			mailInfo mail;
			SessionObject item;
			t_sendMail_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_SENDMAIL) 
				{
					bzero(&mail, sizeof(mail)+sizeof(SessionObject));
				};
		};

		/*
		enum
		{
			SENDMAIL_RETURN_OK,
			SENDMAIL_RETURN_FAILED,
			SENDMAIL_RETURN_NOPLAYER
		};
		const BYTE PARA_SCENE_SENDMAIL_RETURN = 303;
		struct t_sendMailReturn_SceneSession : t_NullCmd
		{
			BYTE retCode;
			t_sendMailReturn_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_SENDMAIL_RETURN) 
				{
					retCode = SENDMAIL_RETURN_FAILED;
				};
		};
		*/

		const BYTE PARA_SCENE_GET_MAIL_LIST = 182;
		struct t_getMailList_SceneSession : t_NullCmd
		{
			DWORD tempID;
			t_getMailList_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_GET_MAIL_LIST) 
				{
					tempID = 0;
				};
		};

		const BYTE PARA_SCENE_OPEN_MAIL = 183;
		struct t_openMail_SceneSession : t_NullCmd
		{
			DWORD tempID;
			DWORD mailID;
			t_openMail_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_OPEN_MAIL) 
				{
					tempID = 0;
					mailID = 0;
				};
		};

		const BYTE PARA_SCENE_GET_MAIL_ITEM = 184;
		struct t_getMailItem_SceneSession : t_NullCmd
		{
			DWORD tempID;
			DWORD mailID;
			WORD space;
			DWORD money;
			DWORD gold;
			t_getMailItem_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_GET_MAIL_ITEM) 
				{
					tempID = 0;
					mailID = 0;
					space = 0;
					money = 0;
					gold = 0;
				};
		};

		const BYTE PARA_SCENE_GET_MAIL_ITEM_RETURN = 185;
		struct t_getMailItemReturn_SceneSession : t_NullCmd
		{
			DWORD userID;
			DWORD mailID;
			DWORD sendMoney;
			DWORD recvMoney;
			DWORD sendGold;
			DWORD recvGold;
			SessionObject item;
			t_getMailItemReturn_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_GET_MAIL_ITEM_RETURN) 
				{
					userID = 0;
					mailID = 0;
					sendMoney = 0;
					recvMoney = 0;
					sendGold = 0;
					recvGold = 0;
					bzero(&item, sizeof(SessionObject));
				};
		};

		const BYTE PARA_SCENE_GET_MAIL_ITEM_CONFIRM = 186;
		struct t_getMailItemConfirm_SceneSession : t_NullCmd
		{
			DWORD userID;
			DWORD mailID;
			t_getMailItemConfirm_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_GET_MAIL_ITEM_CONFIRM) 
				{
					userID = 0;
					mailID = 0;
				};
		};

		const BYTE PARA_SCENE_DEL_MAIL = 187;
		struct t_delMail_SceneSession : t_NullCmd
		{
			DWORD tempID;
			DWORD mailID;
			t_delMail_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_DEL_MAIL) 
				{
					tempID = 0;
					mailID = 0;
				};
		};

		const BYTE PARA_SCENE_CHECK_NEW_MAIL = 188;
		struct t_checkNewMail_SceneSession : t_NullCmd
		{
			DWORD userID;
			t_checkNewMail_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CHECK_NEW_MAIL) 
				{
					userID = 0;
				};
		};

		const BYTE PARA_SCENE_TURN_BACK_MAIL = 189;
		struct t_turnBackMail_SceneSession : t_NullCmd
		{
			DWORD userID;
			DWORD mailID;
			t_turnBackMail_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_TURN_BACK_MAIL) 
				{
					userID = 0;
				};
		};
		//////////////////////////////////////////////////////////////
		///  ����������֮�ʼ�ָ�����
		//////////////////////////////////////////////////////////////

		//��session����Ĺ��﹥��
		const BYTE PARA_SCENE_CREATE_RUSH = 198;
		struct t_createRush_SceneSession : t_NullCmd
		{
			DWORD rushID;
			DWORD delay;
			DWORD countryID;
			t_createRush_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_CREATE_RUSH) 
				{
					rushID = 0;
					delay = 0;
					countryID = 0;
				};
		};

		//////////////////////////////////////////////////////////////
		///  ����������֮����ָ�ʼ
		//////////////////////////////////////////////////////////////
		enum auctionState
		{
			AUCTION_STATE_NEW	= 1,
			AUCTION_STATE_DEAL	= 2,
			AUCTION_STATE_CANCEL	= 3,
			AUCTION_STATE_TIMEOVER	= 4,
			AUCTION_STATE_DEL	= 5
		};

		struct auctionInfo
		{
			DWORD ownerID;
			char owner[MAX_NAMESIZE+1];
			BYTE state;
			DWORD charge;
			DWORD itemID;
			char itemName[MAX_NAMESIZE+1];
			BYTE type;
			BYTE quality;
			WORD needLevel;
			DWORD minMoney;
			DWORD maxMoney;
			DWORD minGold;
			DWORD maxGold;
			DWORD startTime;
			DWORD endTime;
			BYTE bidType;
		};

		const BYTE PARA_AUCTION_CMD = 199;
		struct t_AuctionCmd : t_NullCmd
		{
			BYTE auctionPara;
			t_AuctionCmd()
				: t_NullCmd(CMD_SCENE, PARA_AUCTION_CMD) 
				{
					auctionPara = 0;
				};
		};

		const BYTE PARA_AUCTION_SALE = 1;
		struct t_saleAuction_SceneSession : t_AuctionCmd
		{
			DWORD userID;//��ʱID
			auctionInfo info;
			SessionObject item;
			t_saleAuction_SceneSession()
				: t_AuctionCmd()
				{
					auctionPara = PARA_AUCTION_SALE;

					bzero(&info, sizeof(info));
					bzero(&item, sizeof(item));
				};
		};

		const BYTE PARA_AUCTION_CHECK_BID = 2;
		struct t_checkBidAuction_SceneSession : t_AuctionCmd
		{
			DWORD userID;//��ʱID
			DWORD auctionID;
			DWORD money;
			DWORD gold;
			BYTE bidType;
			t_checkBidAuction_SceneSession()
				: t_AuctionCmd()
				{
					auctionPara = PARA_AUCTION_CHECK_BID;

					userID = 0;
					auctionID = 0;
					money = 0;
					gold = 0;
					bidType = 0;
				};
		};

		const BYTE PARA_AUCTION_BID = 3;
		struct t_bidAuction_SceneSession : t_AuctionCmd
		{
			DWORD userID;//��ʱID
			DWORD auctionID;
			DWORD money;
			DWORD gold;
			t_bidAuction_SceneSession()
				: t_AuctionCmd()
				{
					auctionPara = PARA_AUCTION_BID;

					userID = 0;
					auctionID = 0;
					money = 0;
					gold = 0;
				};
		};

		const BYTE PARA_AUCTION_QUERY = 4;
		struct t_queryAuction_SceneSession : t_AuctionCmd
		{
			DWORD userID;//��ʱID
			BYTE type;
			char name[MAX_NAMESIZE];
			BYTE quality;
			WORD level;
			WORD page;
			t_queryAuction_SceneSession()
				: t_AuctionCmd()
				{
					auctionPara = PARA_AUCTION_QUERY;

					userID = 0;
					type = 0;
					bzero(name, sizeof(name));
					quality = 0;
					level = 0;
					page = 0;
				};
		};

		const BYTE PARA_AUCTION_CHECK_CANCEL = 5;
		struct t_checkCancelAuction_SceneSession : t_AuctionCmd
		{
			DWORD userID;//��ʱID
			DWORD auctionID;
			DWORD charge;
			t_checkCancelAuction_SceneSession()
				: t_AuctionCmd()
				{
					auctionPara = PARA_AUCTION_CHECK_CANCEL;

					userID = 0;
					auctionID = 0;
					charge = 0;
				};
		};

		const BYTE PARA_AUCTION_CANCEL = 6;
		struct t_cancelAuction_SceneSession : t_AuctionCmd
		{
			DWORD userID;//��ʱID
			DWORD auctionID;
			DWORD charge;
			t_cancelAuction_SceneSession()
				: t_AuctionCmd()
				{
					auctionPara = PARA_AUCTION_CANCEL;

					userID = 0;
					auctionID = 0;
					charge = 0;
				};
		};

		const BYTE PARA_AUCTION_GET_LIST = 7;
		struct t_getListAuction_SceneSession : t_AuctionCmd
		{
			DWORD userID;//��ʱID
			BYTE list;
			t_getListAuction_SceneSession()
				: t_AuctionCmd()
				{
					auctionPara = PARA_AUCTION_GET_LIST;

					userID = 0;
					list = 0;
				};
		};

		/*
		const BYTE PARA_AUCTION_BID_LIST = 7;
		struct t_bidListAuction_SceneSession : t_AuctionCmd
		{
			DWORD userID;//��ʱID
			DWORD bidList[100];
			t_bidListAuction_SceneSession()
				: t_AuctionCmd()
				{
					auctionPara = PARA_AUCTION_BID_LIST;

					bzero(&bidList[0], sizeof(bidList));
				};
		};
		*/

		//////////////////////////////////////////////////////////////
		///  ����������֮����ָ�����
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		/// ����������֮��ʱ����
		//////////////////////////////////////////////////////////////
		const BYTE PARA_USER_ARCHIVE_REQ = 200;
		struct t_ReqUser_SceneArchive : t_NullCmd
		{
			DWORD id;						/// ��ɫ
			DWORD dwMapTempID;				/// ��ͼ��ʱID
			t_ReqUser_SceneArchive()
				: t_NullCmd(CMD_SCENE, PARA_USER_ARCHIVE_REQ) 
				{
					id = 0;
					dwMapTempID = 0;
				};
		};
		const BYTE PARA_USER_ARCHIVE_READ = 201;
		struct t_ReadUser_SceneArchive : t_NullCmd
		{
			DWORD id;						/// ��ɫ
			DWORD dwMapTempID;				/// ��ͼ��ʱID
			DWORD dwSize;
			char data[0];
			t_ReadUser_SceneArchive()
				: t_NullCmd(CMD_SCENE, PARA_USER_ARCHIVE_READ) 
				{
					id = 0;
					dwMapTempID = 0;
					dwSize = 0;
				};
		};
		const BYTE PARA_USER_ARCHIVE_WRITE = 202;
		struct t_WriteUser_SceneArchive : t_NullCmd
		{
			DWORD id;						/// ��ɫ
			DWORD dwMapTempID;				/// ��ͼ��ʱID
			DWORD dwSize;
			char data[0];
			t_WriteUser_SceneArchive()
				: t_NullCmd(CMD_SCENE, PARA_USER_ARCHIVE_WRITE) 
				{
					id = 0;
					dwMapTempID = 0;
					dwSize = 0;
				};
		};
		//////////////////////////////////////////////////////////////
		/// ����������֮��ʱ����
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		/// ����������֮����ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_USER_TEAM_ADDMEMBER = 203;
		struct t_Team_AddMember : t_NullCmd
		{
			DWORD dwLeaderID;						/// ��ɫ
			DWORD dwMemberID;						/// ��ɫ
			t_Team_AddMember()
				: t_NullCmd(CMD_SCENE, PARA_USER_TEAM_ADDMEMBER) 
				{
					dwLeaderID = 0;
					dwMemberID = 0;
				};
		};
		const BYTE PARA_USER_TEAM_DELMEMBER = 204;
		struct t_Team_DelMember : t_NullCmd
		{
			DWORD dwLeaderID;						/// ��ɫ
			DWORD dwMemberID;						/// ��ɫ
			t_Team_DelMember()
				: t_NullCmd(CMD_SCENE, PARA_USER_TEAM_DELMEMBER) 
				{
					dwLeaderID = 0;
					dwMemberID = 0;
				};
		};
		const BYTE PARA_USER_TEAM_REQUEST = 205;
		struct t_Team_Request : t_NullCmd
		{
			DWORD dwUserID;						/// ��ɫ
			t_Team_Request()
				: t_NullCmd(CMD_SCENE, PARA_USER_TEAM_REQUEST) 
				{
					dwUserID = 0;
				};
		};
		const BYTE PARA_USER_TEAM_DATA = 206;
		struct t_Team_Data : t_NullCmd
		{
			DWORD dwLeaderID;						/// ��ɫ
			WORD dwSize;						/// ��������
			DWORD dwMember[0];				/// ��Ա�б�
			t_Team_Data()
				: t_NullCmd(CMD_SCENE, PARA_USER_TEAM_DATA) 
				{
					dwLeaderID = 0;
					dwSize = 0;
				};
		};
		//////////////////////////////////////////////////////////////
		/// ����������֮����ָ��
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		///  ����������֮��սָ�ʼ
		//////////////////////////////////////////////////////////////
				
		// �ɻỰ���򳡾���֪ͨ����ĳ����Ͻ��о�������
		const BYTE  PARA_QUIZ_AWARD = 214;
		struct t_quizAward_SceneSession : t_NullCmd
		{
			DWORD dwUserID;			/// ��ɫID
			DWORD dwExp;			/// ��������
			DWORD dwMoney;			/// ��������
			DWORD dwGrace;			/// ���������Ĳɵ���
			BYTE  byType;			/// ��������,Ĭ��Ϊȫ��
			
			t_quizAward_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_QUIZ_AWARD) 
				{
					byType = 0;
				};
		};

		
		// �ı����������֪ͨ�Ự
		const BYTE  PARA_CHANGE_COUNTRY = 215;
		struct t_changeCountry_SceneSession : t_NullCmd
		{
			DWORD dwUserID;			/// ��ɫID
			DWORD dwToCountryID;		/// �ı䵽�Ĺ���
			t_changeCountry_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_CHANGE_COUNTRY) {};
		};

		// �ı�������Ự֪ͨ����
		const BYTE  PARA_RETURN_CHANGE_COUNTRY = 216;
		struct t_returnChangeCountry_SceneSession : t_NullCmd
		{
			DWORD dwUserID;			/// ��ɫID

			t_returnChangeCountry_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_RETURN_CHANGE_COUNTRY) {};
		};

		// �Ự���򳡾������������ս�����ڳ���
		const BYTE PARA_TRANS_DARE_COUNTRY = 217;
		struct t_transDareCountry_SceneSession : t_NullCmd
		{
			DWORD dwUserID;	/// ��ɫID
			DWORD dwMoney;	/// ��Ҫ�۳����
			DWORD dwCountry; /// Ҫȥ�Ĺ���ID
			
			t_transDareCountry_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_TRANS_DARE_COUNTRY){};
		};

		// �ɳ��������Ự��ȷ�ϸ����ܷ�����ָ�����еİ�����
		const BYTE PARA_ENTER_UNION_MANOR = 218;
		struct t_enterUnionManor_SceneSession : t_NullCmd
		{
			DWORD dwUserID; // �����˵�ID
			DWORD dwCountryID; // ��ȥ�Ĺ���ID
			DWORD dwCityID;	// ���ڳ���ID

			t_enterUnionManor_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_ENTER_UNION_MANOR) {};
		};
		
		// �ɻỰ��������������������Լ���������
		const BYTE PARA_RETURN_ENTER_UNION_MANOR = 219;
		struct t_returnEnterUnionManor_SceneSession : t_NullCmd
		{
			DWORD dwUserID;     // �����˵�ID
			DWORD dwCountryID;  // Ҫ��ȥ�Ĺ���ID
			DWORD dwAreaID;     // ����ID:ֻ���м��������REALID����Ҫ����ͨ��COUNTRYID���м���

			t_returnEnterUnionManor_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_RETURN_ENTER_UNION_MANOR) {};
		};
		
		// �ɻỰ��������,�����������ǰ���Ա
		const BYTE PARA_CLEAR_UNION_MANOR = 220;
		struct t_clearUnionManor_SceneSession : t_NullCmd
		{
			DWORD dwUnionID;	// �ó����ϵ������ˣ�����ָ�������ˣ�����ȫrelive
			DWORD dwCountryID;  // Ҫ��ȥ�Ĺ���ID
			DWORD dwAreaID;     // ����ID:ֻ���м��������REALID����Ҫ����ͨ��COUNTRYID���м���

			t_clearUnionManor_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_CLEAR_UNION_MANOR) {};
		};
		
		// �ɳ��������Ự����ѯ��������
		const BYTE PARA_QUESTION_UNION_CITY = 221;
		struct t_questionUnionCity_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwCountryID;
			DWORD dwCityID;
			BYTE  byType;

			t_questionUnionCity_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_QUESTION_UNION_CITY) 
				{
					dwUserID = 0;
					dwCountryID = 0;
					dwCityID = 0;
					byType = 0;
				};
		};
		
				
		
		// �ɳ��������Ự��֪ͨ���û����ݱ��
		const BYTE PARA_CHANGE_USER_DATA = 224;
		struct t_changeUserData_SceneSession : t_NullCmd
		{
			WORD  wdLevel;	    // ���ͣ�0Ϊ������ս�ģ�1�ɽ�����ս��,3,���й�����ս��Ϣ
			DWORD dwExploit;    // ��ѫֵ
			DWORD dwGrace;	    // �Ĳ�ֵ
			DWORD dwUserID;	    // ���ID
			
			t_changeUserData_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_CHANGE_USER_DATA) {};
		};
		
		// ֪ͨ�������ս��ʼ�ͽ���
		const BYTE PARA_UNION_DARE_NOTIFY = 225;
		struct t_unionDareNotify_SceneSession : t_NullCmd
		{
			DWORD sceneID;
			BYTE state;//1 ��ʼ 0 ����

			t_unionDareNotify_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_UNION_DARE_NOTIFY) {};
		};
		
		// �Ự��������, ʦ����ȡ����
		const BYTE PICKUP_MASTER_SCENE_PARA = 226;
		struct t_PickupMaster_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwMoney;

			t_PickupMaster_SceneSession()
				: t_NullCmd(CMD_SCENE, PICKUP_MASTER_SCENE_PARA)
				{
					dwUserID = 0;
					dwMoney = 0;
				}
		};

		//////////////////////////////////////////////////////////////
		///  ����������֮��սָ�����
		//////////////////////////////////////////////////////////////
	
		/************************************************************
		***********����������֮������ؿ�ʼ************
		************************************************************/
		// ����      
		const BYTE QUEST_BULLETIN_USERCMD_PARA = 240;
		struct t_QuestBulletinUserCmd : public t_NullCmd
		{
			t_QuestBulletinUserCmd(): t_NullCmd(CMD_SCENE, QUEST_BULLETIN_USERCMD_PARA)
			{
			}

			BYTE kind; // 0: reserve 1: tong 2 : family
			DWORD id; //family id or tong id
			char content[MAX_CHATINFO]; //chat msg
		};

		const BYTE QUEST_CHANGE_AP = 241;
		struct t_ChangeAP : public t_NullCmd //�����ж���
		{
			t_ChangeAP(): t_NullCmd(CMD_SCENE, QUEST_CHANGE_AP)
			{
			}
			DWORD id; 
			int point; 
		};

		const BYTE QUEST_CHANGE_RP = 242; //��������
		struct t_ChangeRP : public t_NullCmd
		{
			t_ChangeRP(): t_NullCmd(CMD_SCENE, QUEST_CHANGE_RP)
			{
			}
			BYTE kind; // 1: tong 2 :fam
			int point; 
		};
		 
		// Ѱ��ʦ��,�����ȯ
		const BYTE OVERMAN_TICKET_ADD = 243; 
		struct t_OvermanTicketAdd : public t_NullCmd
		{
			t_OvermanTicketAdd(): t_NullCmd(OVERMAN_TICKET_ADD, QUEST_CHANGE_RP)
			{
			}
			DWORD id;//	ʦ��id
			DWORD ticket;//	Ӧ�õõ��ĵ�ȯ
			char name[MAX_NAMESIZE+1];//ͽ������
		};
		
		/************************************************************
		***********����������֮������ؽ���************
		************************************************************/
		

		//////////////////////////////////////////////////////////////
		///  GMά��ָ��
		//////////////////////////////////////////////////////////////
		// GMͣ��ά��ָ��
		const BYTE PARA_SHUTDOWN = 1;
		struct t_shutdown_SceneSession : t_NullCmd
		{
			time_t time;
			char info[MAX_CHATINFO];
			t_shutdown_SceneSession()
				: t_NullCmd(CMD_SCENE_SHUTDOWN, PARA_SHUTDOWN) {
					bzero(&time,sizeof(time));
					bzero(info,sizeof(info));
				};
		};
		//////////////////////////////////////////////////////////////
		///  GMά��ָ�����
		//////////////////////////////////////////////////////////////



		//////////////////////////////////////////////////////////////
		/// ����������ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// ���ط�����ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_GATE_REGUSER = 1;
		struct t_regUser_GateSession : t_NullCmd
		{
			DWORD accid;
			DWORD dwID;
			DWORD dwTempID;
			DWORD dwMapID;
			WORD wdLevel;
			WORD wdOccupation;
			WORD wdCountry;
			BYTE byCountryName[MAX_NAMESIZE+1];
			BYTE byName[MAX_NAMESIZE+1];
			BYTE byMapName[MAX_NAMESIZE+1];
			t_regUser_GateSession()
				: t_NullCmd(CMD_GATE, PARA_GATE_REGUSER) {};
		};
		const BYTE PARA_GATE_UNREGUSER = 2;
		struct t_unregUser_GateSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwSceneTempID;
			BYTE retcode;
			t_unregUser_GateSession()
				: t_NullCmd(CMD_GATE, PARA_GATE_UNREGUSER) {};
		};
		const BYTE PARA_GATE_CHANGE_SCENE_USER = 3;
		struct t_changeUser_GateSession : t_NullCmd
		{
			DWORD accid;
			DWORD dwID;
			DWORD dwTempID;
			BYTE byName[MAX_NAMESIZE+1];
			BYTE byMapFileName[MAX_NAMESIZE+1];
			t_changeUser_GateSession()
				: t_NullCmd(CMD_GATE, PARA_GATE_CHANGE_SCENE_USER) {};
		};

		/// �������������������
		const BYTE REQUEST_GATE_COUNTRY_ORDER = 5;
		struct t_request_Country_GateSession : t_NullCmd
		{
			t_request_Country_GateSession()
				: t_NullCmd(CMD_GATE, REQUEST_GATE_COUNTRY_ORDER) {};
		};
		struct CountrOrder
		{
			DWORD size;					//����
			struct {
			DWORD country;				//����	
			DWORD count;		//�Ѿ�����õĹ���id
			} order[0];
		};
		/// ����������������
		const BYTE PARA_GATE_COUNTRY_ORDER = 6;
		struct t_order_Country_GateSession : t_NullCmd
		{
			DWORD dwID;					//id
			CountrOrder order;			//����Ĺ��� 
			t_order_Country_GateSession()
				: t_NullCmd(CMD_GATE, PARA_GATE_COUNTRY_ORDER) {};
		};

		///ֱ��ת�����û���ָ��
		const BYTE PARA_GATE_FORWARD_USER = 7;
		struct t_forwardUser_GateSession : t_NullCmd
		{
			DWORD id;
			DWORD tempid;
			char name[MAX_NAMESIZE];
			DWORD cmd_len;
			BYTE cmd[0];
			t_forwardUser_GateSession()
				: t_NullCmd(CMD_GATE, PARA_GATE_FORWARD_USER)
				{
					id = 0;
					tempid = 0;
					bzero(name, MAX_NAMESIZE);
				}
		};

		const BYTE PARA_UNION_DISBAND = 100;
		struct t_disbandUnion_GateSession : t_NullCmd
		{
			DWORD dwCharID;
			DWORD dwUnionID;
			t_disbandUnion_GateSession()
				: t_NullCmd(CMD_GATE, PARA_UNION_DISBAND) {};
		};
		const BYTE PARA_SEPT_DISBAND = 101;
		struct t_disbandSept_GateSession : t_NullCmd
		{
			DWORD dwCharID;
			DWORD dwSeptID;
			t_disbandSept_GateSession()
				: t_NullCmd(CMD_GATE, PARA_SEPT_DISBAND) {};
		};
		
		const BYTE PARA_GATE_DELCHAR = 102;
		struct t_DelChar_GateSession : t_NullCmd
		{
			DWORD accid;						/// �˺�
			DWORD id;							/// ��ɫ���
			char name[MAX_NAMESIZE+1];
			DWORD status;
			t_DelChar_GateSession()
				: t_NullCmd(CMD_GATE, PARA_GATE_DELCHAR) {};
		};		

		const BYTE PARA_EXIT_QUIZ = 103;
		struct t_exitQuiz_GateSession : t_NullCmd
		{
			DWORD dwUserID;
			BYTE  type; // type=0���˳�.type=1,����

			t_exitQuiz_GateSession()
				: t_NullCmd(CMD_GATE, PARA_EXIT_QUIZ)
				{
					type = 0;
				};
		};
		
		//////////////////////////////////////////////////////////////
		/// ��ѯ��ָ��
		//////////////////////////////////////////////////////////////

		const BYTE PARA_QUESTION_OBJECT = 244;
		struct t_questionObject_SceneSession  : t_NullCmd
		{
			char from_name[MAX_NAMESIZE+1];  // ��Ʒӵ��������
			char to_name[MAX_NAMESIZE+1];    // ��Ʒ��ѯ������
			DWORD dwObjectTempID;
			
			t_questionObject_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_QUESTION_OBJECT) {};
		};		

		const BYTE PARA_RETURN_OBJECT = 245;
		struct t_returnObject_SceneSession  : t_NullCmd
		{
			char from_name[MAX_NAMESIZE+1]; // ��Ʒӵ��������
			char to_name[MAX_NAMESIZE+1];   // ��Ʒ��ѯ������

			t_Object object;
			
			t_returnObject_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_RETURN_OBJECT) {};
		};		

		const BYTE PARA_CLOSE_NPC = 246;
		struct t_CloseNpc_SceneSession  : t_NullCmd
		{
			t_CloseNpc_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_CLOSE_NPC) {};
		};

		const BYTE PARA_DEBUG_COUNTRYPOWER = 247;
		struct t_DebugCountryPower_SceneSession  : t_NullCmd
		{
			t_DebugCountryPower_SceneSession()
				: t_NullCmd(CMD_SCENE, PARA_DEBUG_COUNTRYPOWER) {};
		};

		//////////////////////////////////////////////////////////////
		/// ���ط�����ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// �Ự������ת��ָ��
		//////////////////////////////////////////////////////////////
		/// ���ص�Session
		const BYTE PARA_FORWARD_USER = 1;
		struct t_Session_ForwardUser : t_NullCmd
		{
			DWORD dwID;
			WORD  size;
			BYTE  data[0];
			t_Session_ForwardUser()
				: t_NullCmd(CMD_FORWARD, PARA_FORWARD_USER) {};
		};
		/// Session��ȫ����
		const BYTE PARA_FORWARD_WORLD = 2;
		struct t_Session_ForwardWorld : t_NullCmd
		{
			WORD  size;
			BYTE  data[0];
			t_Session_ForwardWorld()
				: t_NullCmd(CMD_FORWARD, PARA_FORWARD_WORLD) {};
		};
		/// Session������
		const BYTE PARA_FORWARD_COUNTRY = 3;
		struct t_Session_ForwardCountry : t_NullCmd
		{
			DWORD dwCountry;
			WORD  size;
			BYTE  data[0];
			t_Session_ForwardCountry()
				: t_NullCmd(CMD_FORWARD, PARA_FORWARD_COUNTRY) {};
		};
		//////////////////////////////////////////////////////////////
		/// �Ự������ת��ָ��
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		/// �Ự������ָ��
		//////////////////////////////////////////////////////////////
		// �Ự��������Gateway���ͺ�����������Ϣ��
		enum{
			BLACK_LIST_REMOVE,
			BLACK_LIST_ADD
		};
		const BYTE HANDLE_BLACK_LIST_PARA = 1;
		struct t_Session_HandleBlackList : t_NullCmd
		{
			DWORD dwID;
			char name[MAX_NAMESIZE+1];
			BYTE byOper;   // 0Ϊɾ�� 1Ϊ����
			t_Session_HandleBlackList()
				: t_NullCmd(CMD_SESSION, HANDLE_BLACK_LIST_PARA) {};
		};
		//////////////////////////////////////////////////////////////
		/// �Ự������ָ��
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		/// GM����ָ��
		//////////////////////////////////////////////////////////////

		const BYTE MSGTIME_GMTOL_PARA = 1;
		struct t_MsgTime_GmTool : t_NullCmd
		{
			DWORD id;
			DWORD time;
			t_MsgTime_GmTool()
				: t_NullCmd(CMD_GMTOOL, MSGTIME_GMTOL_PARA)
				{
					id = 0;
					time = 0;
				}
		};

		//////////////////////////////////////////////////////////////
		/// GM����ָ��
		//////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////
		///  ��ʱָ�ʼ
		//////////////////////////////////////////////////////////////
		// ��ʱָ��
		// ���������Ự��������а�ᣬ������м����Ա������Ӧ�µļ�����ϲ�ģʽ
		const BYTE CLEARRELATION_PARA = 1;
		struct t_ClearRelation_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			
			t_ClearRelation_SceneSession()
				: t_NullCmd(CMD_SCENE_TMP, CLEARRELATION_PARA)
				{
					dwUserID = 0;
				}
		};

		// �Ự������������ȡһ��������
		const BYTE GET_CREATE_UNION_ITEM_PARA = 2;
		struct t_GetCreateUnionItem_SceneSession : t_NullCmd
		{
			DWORD dwUserID; // ��ָ�������
			
			t_GetCreateUnionItem_SceneSession()
				: t_NullCmd(CMD_SCENE_TMP, GET_CREATE_UNION_ITEM_PARA)
				{
					dwUserID = 0;
				}
		};
		
		// ���������Ự������һ��������
		const BYTE RETURN_CREATE_UNION_ITEM_PARA = 3;
		struct t_ReturnCreateUnionItem_SceneSession : t_NullCmd
		{
			DWORD dwUserID; // ��ָ�������
			SessionObject item;

			t_ReturnCreateUnionItem_SceneSession()
				: t_NullCmd(CMD_SCENE_TMP, RETURN_CREATE_UNION_ITEM_PARA)
				{
				}
		};

		//////////////////////////////////////////////////////////////
		///  ��ʱָ�����
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		///  ����ָ�ʼ
		//////////////////////////////////////////////////////////////
		// ���������Ự����������
		const BYTE OP_REPUTE_PARA = 1;
		struct t_OpRepute_SceneSession : t_NullCmd
		{
			DWORD dwSeptID; // ����ID
			int dwRepute; //  ����ֵ
			
			t_OpRepute_SceneSession()
				: t_NullCmd(CMD_SCENE_SEPT, OP_REPUTE_PARA)
				{
					dwSeptID = 0;
					dwRepute = 0;
				}
		};
		
		// ���������Ự�����ļ���ȼ�
		const BYTE OP_LEVEL_PARA = 2;
		struct t_OpLevel_SceneSession : t_NullCmd
		{
			DWORD dwSeptID; // ����ID
			int   dwLevel; //  ����ֵ
			
			t_OpLevel_SceneSession()
				: t_NullCmd(CMD_SCENE_SEPT, OP_LEVEL_PARA)
				{
					dwSeptID = 0;
					dwLevel = 0;
				}
		};
		
		// �Ự������������ȡ���徭��
		const BYTE GET_SEPT_EXP_PARA = 3;
		struct t_GetSeptExp_SceneSession : t_NullCmd
		{
			DWORD dwSeptID; // ����ID
			DWORD dwUserID; // �峤ID
			
			t_GetSeptExp_SceneSession()
				: t_NullCmd(CMD_SCENE_SEPT, GET_SEPT_EXP_PARA)
				{
					dwSeptID = 0;
					dwUserID = 0;
				}
		};

		// �Ự���������������йؼ������ͨ��Ϣ�����ô浵
		const BYTE SEND_SEPT_NORMAL_PARA = 4;
		struct t_SendSeptNormal_SceneSession : t_NullCmd
		{
			DWORD dwUserID; // �峤ID
			DWORD dwRepute; // ��������
			
			t_SendSeptNormal_SceneSession()
				: t_NullCmd(CMD_SCENE_SEPT, SEND_SEPT_NORMAL_PARA)
				{
					dwUserID = 0;
					dwRepute = 0;
				}
		};

		// �Ự������������ȡ���徭��
		const BYTE GET_SEPT_NORMAL_EXP_PARA = 5;
		struct t_GetSeptNormalExp_SceneSession : t_NullCmd
		{
			DWORD dwSeptID; // ����ID
			DWORD dwUserID; // �峤ID

			t_GetSeptNormalExp_SceneSession()
				: t_NullCmd(CMD_SCENE_SEPT, GET_SEPT_NORMAL_EXP_PARA)
				{
					dwSeptID = 0;
					dwUserID = 0;
				}
		};


		//////////////////////////////////////////////////////////////
		///  ����ָ�����
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		///  ����ָ��
		//////////////////////////////////////////////////////////////
		// �ɳ��������Ự���鿴���Ҷ�ս��Ϣ
		const BYTE PARA_VIEW_COUNTRY_DARE = 1;
		struct t_viewCountryDare_SceneSession : t_NullCmd
		{
			BYTE  byType;	    // ���ͣ�0Ϊ������ս�ģ�1�ɽ�����ս��,3,���й�����ս��Ϣ
			DWORD dwUserID;	    // ��ѯ��
			
			t_viewCountryDare_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_VIEW_COUNTRY_DARE) {};
		};

		// �ɳ��������Ự�����й��Ҿ���
		const BYTE PARA_CONTRIBUTE_COUNTRY = 2;
		struct t_ContributeCountry_SceneSession : t_NullCmd
		{
			BYTE byType; // 0,��ͨ���ʣ�1,˿��,2,��ʯ,3,���,4,ľ�ģ�5,Ƥë,6,��ҩ
			DWORD dwValue;	// ���׶�
			DWORD dwCountry;

			t_ContributeCountry_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_CONTRIBUTE_COUNTRY){};
		};
		
		// ��ս���
		enum
		{
			COUNTRY_FORMAL_DARE,	// ��ʽ��ս
			COUNTRY_ANNOY_DARE,	// ɧ�Ź�ս
			EMPEROR_DARE,		// �ʳ�����ս
			COUNTRY_FORMAL_ANTI_DARE, // ��ս����
		};
		
		const BYTE PARA_COUNTRY_DARE_RESULT = 3;
		struct t_countryDareResult_SceneSession : t_NullCmd
		{
			DWORD dwAttCountryID;
			DWORD dwDefCountryID;

			char attCountryName[MAX_NAMESIZE];	// ��ս�߹�������
			char defCountryName[MAX_NAMESIZE];	// ��������������
			DWORD dwAttUserID;			// �����󽫾������ID
			
			BYTE byType;	// ��ս����
			
			t_countryDareResult_SceneSession()
				:	t_NullCmd(CMD_SCENE_COUNTRY, PARA_COUNTRY_DARE_RESULT) {}
		};

		// �ɻỰ�������������������ǳ���Ϊս��
		const BYTE PARA_SET_COUNTRY_WAR = 4;
		struct t_setCountryWar_SceneSession : t_NullCmd
		{
			DWORD dwCountryID;  // Ҫ����ս���Ĺ���ID // ���ط�
			//DWORD dwAttCountryID; // ����������ID
			DWORD dwAreaID;     // ս��
			BYTE  byStatus;	    // ���ͣ�0Ϊ�˳���1,Ϊ����
			
			t_setCountryWar_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_SET_COUNTRY_WAR) 
				{
				    this->dwCountryID = 0;
				    //this->dwAttCountryID = 0;
				    this->dwAreaID = 0;
				    this->byStatus = 0;
				};
		};
		
		// �ɻỰ�����������÷�����������ң���ת���߾�
		const BYTE PARA_SEL_TRANS_COUNTRY_WAR = 5;
		struct t_selTransCountryWar_SceneSession : t_NullCmd
		{
			DWORD dwCountryID;  // Ҫ��ת����ҹ���
			DWORD dwLevel;     // ��ҵȼ�����
			
			t_selTransCountryWar_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_SEL_TRANS_COUNTRY_WAR) {};
		};

		//��������һ�����
		const BYTE PARA_COUNTRY_PUNISH_USER = 6;
		struct t_countryPunishUser_SceneSession : t_NullCmd
		{
			char name[MAX_NAMESIZE];
			DWORD method;//������ʽ 1���� 2�ؼ���

			t_countryPunishUser_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_COUNTRY_PUNISH_USER)
				{
					bzero(name, sizeof(name));
					method = 0;
				};
		};

		struct _techItem
		{
			DWORD dwType;
			DWORD dwLevel;
		};	
		
		// ���¹��ҿƼ�������
		const BYTE PARA_UPDATE_TECH = 7;
		struct t_updateTech_SceneSession : t_NullCmd
		{
			t_updateTech_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_UPDATE_TECH)
				{
					bzero(data, sizeof(data));
					dwCountryID = 0;
				};
			
			DWORD dwCountryID;
			_techItem data[14];
		};

		// ���������Ự�����пƼ�ͶƱ�����������
		const BYTE PARA_OP_TECH_VOTE = 8;
		struct t_opTechVote_SceneSession : t_NullCmd
		{

			t_opTechVote_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_OP_TECH_VOTE)
				{
					byOpType = 0;
					dwCountryID = 0;
				};

			DWORD dwCountryID;
			BYTE byOpType; //1Ϊ������0Ϊ����
		};
		
		// ����������ᵽ����
		const BYTE PARA_UPDATE_SCENE_UNION = 9;
		struct t_updateSceneUnion_SceneSession : t_NullCmd
		{

			t_updateSceneUnion_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_UPDATE_SCENE_UNION)
				{
					dwSceneID = 0;
					dwUnionID = 0;
				};
			
			DWORD dwSceneID;
			DWORD dwUnionID;
		};

		// ����սʤ����־
		const BYTE PARA_WINNER_EXP_SCENE_COUNTRY = 10;
		struct t_updateWinnerExp_SceneSession : t_NullCmd
		{

			t_updateWinnerExp_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_WINNER_EXP_SCENE_COUNTRY)
				{
				};
			DWORD countryID;
			bool type;	//false��ʾ����,true��ʾ��ʼ
		};
		
		struct _allyItem
		{
			DWORD dwCountryID;
			DWORD dwAllyCountryID;
			DWORD dwFriendDegree;
		};	
		
		// ���¹���ͬ����Ϣ������
		const BYTE PARA_UPDATE_ALLY = 11;
		struct t_updateAlly_SceneSession : t_NullCmd
		{
			t_updateAlly_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_UPDATE_ALLY)
				{
					dwSize = 0;
				};
			
			DWORD dwSize;
			_allyItem data[0];
		};

		// �������Ự,���ӹ��������Ѻö�
		const BYTE PARA_OP_ALLY_FRIENDDEGREE = 12;
		struct t_opAllyFrienddegree_SceneSession : t_NullCmd
		{
			t_opAllyFrienddegree_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_OP_ALLY_FRIENDDEGREE)
				{
					dwCountryID = 0;
					friendDegree = 0;
				};
			
			DWORD dwCountryID;
			int friendDegree;
		};

		// �Ự������, ���������ڳ�
		const BYTE PARA_SUMMON_ALLY_NPC = 13;
		struct t_summonAllyNpc_SceneSession : t_NullCmd
		{
			t_summonAllyNpc_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_SUMMON_ALLY_NPC)
				{
					dwCountryID = 0;
				};
			
			DWORD dwCountryID;
		};

		// �������Ự,���⽻���ӵ���ʱ,���ӹ��������Ѻö�
		const BYTE PARA_ALLY_NPC_CLEAR = 14;
		struct t_allyNpcClear_SceneSession : t_NullCmd
		{
			t_allyNpcClear_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_ALLY_NPC_CLEAR)
				{
					dwCountryID = 0;
				};

			DWORD dwCountryID;
		};

		// ���ûʳ�ռ���(�Ự������)
		const BYTE PARA_SET_EMPEROR_HOLD = 15;
		struct t_setEmperorHold_SceneSession : t_NullCmd
		{
			t_setEmperorHold_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_SET_EMPEROR_HOLD)
				{
					dwCountryID = 0;
				};

			DWORD dwCountryID;
		};

		// �Ự������, ֪ͨ�󽫾��ĵȼ�
		const BYTE PARA_REFRESH_GEN = 16;
		struct t_refreshGen_SceneSession : t_NullCmd
		{
			t_refreshGen_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_REFRESH_GEN)
				{
					dwCountryID = 0;
					level = 0;
					exp = 0;
					maxExp = 0;
				};

			DWORD dwCountryID;
			DWORD level;
			DWORD exp;
			DWORD maxExp;
		};

		// �������Ự,���󽫾��Ӿ���
		const BYTE PARA_ADD_GEN_EXP = 17;
		struct t_addGenExp_SceneSession : t_NullCmd
		{
			t_addGenExp_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_ADD_GEN_EXP)
				{
					dwCountryID = 0;
					exp = 0;
				};

			DWORD dwCountryID;
			DWORD exp;
		};

		//�ʵ۴���һ�����
		const BYTE PARA_EMPEROR_PUNISH_USER = 18;
		struct t_emperorPunishUser_SceneSession : t_NullCmd
		{
			char name[MAX_NAMESIZE];
			DWORD method;//������ʽ 1���� 2�ؼ���

			t_emperorPunishUser_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_EMPEROR_PUNISH_USER)
				{
					bzero(name, sizeof(name));
					method = 0;
				};
		};

		// ���һ����ҵ��ƶ��
		const BYTE PARA_CHECK_USER = 19;
		struct t_checkUser_SceneSession : t_NullCmd
		{
			t_checkUser_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_CHECK_USER)
				{
					byState   = 0;
					dwCheckID  = 0;
					dwCheckedID = 0;
				};
			
			BYTE byState; // 0, δͨ�����, 1,ͨ�����
			DWORD dwCheckID; // ����������ID
			DWORD dwCheckedID; // �������ҵ�ID
		};

		// �����⽻��״̬
		const BYTE PARA_SET_DIPLOMAT_STATE = 20;
		struct t_setDiplomatState_SceneSession : t_NullCmd
		{
			t_setDiplomatState_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_SET_DIPLOMAT_STATE)
				{
					byState   = 0;
					dwUserID = 0;
				};
			
			BYTE byState; // 0, ȡ��״̬, 1,����״̬
			DWORD dwUserID; // �����õ����ID
		};

		// ���ò�ͷ״̬
		const BYTE PARA_SET_CATCHER_STATE = 21;
		struct t_setCatcherState_SceneSession : t_NullCmd
		{
			t_setCatcherState_SceneSession()
				: t_NullCmd(CMD_SCENE_COUNTRY, PARA_SET_CATCHER_STATE)
				{
					byState   = 0;
					dwUserID = 0;
				};
			
			BYTE byState; // 0, ȡ��״̬, 1,����״̬
			DWORD dwUserID; // �����õ����ID
		};


		const BYTE PARA_COUNTRY_POWER_SORT = 22;
		struct t_countryPowerSort_SceneSession : t_NullCmd
		{
			BYTE country[13];
			t_countryPowerSort_SceneSession() : t_NullCmd(CMD_SCENE_COUNTRY, PARA_COUNTRY_POWER_SORT)
			{
				bzero(country, sizeof(country));
			}
		};
		//////////////////////////////////////////////////////////////
		///  ����ָ�����
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		///  ��սָ�ʼ
		//////////////////////////////////////////////////////////////
		// �ɳ��������������ͣ�֪ͨ���йض�ս�Ĵ����Ƿ�ɹ������������ڼ����ս��ʱ
		enum
		{
			SCENE_ACTIVEDARE_SUCCESS,    // ����������ս���ɹ�
			SCENE_ACTIVEDARE_FAIL        // ����������ս��ʧ��
		};

		const BYTE PARA_ACTIVEDARE = 1;
		struct t_activeDare_SceneSession : t_NullCmd
		{
			DWORD dwWarID;
			DWORD dwStatus;

			t_activeDare_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_ACTIVEDARE) 
				{
					dwWarID = 0;
					dwStatus = SCENE_ACTIVEDARE_FAIL;
				};
		};

		
		// �ɻỰ�򳡾����ͣ�֪ͨ�佻ս״̬�������յ��󣬻����ɫ���Ͻ�ս��¼�����ɴ˼�����Ƿ���ʾ��ս��ʾ
		const BYTE PARA_ENTERWAR = 2;
		struct t_enterWar_SceneSession : t_NullCmd
		{
			DWORD dwWarType;        // ��ս����
			DWORD dwUserID;		//
			DWORD dwFromRelationID; // 
			DWORD dwToRelationID;   // ��֮��ս������ϵID,�ط�(�ڶ������Ͷ�սʱʹ��)
			DWORD dwSceneTempID;
			DWORD dwStatus;         // 1,Ϊ��ʼ��ս�ļ�¼��0Ϊ������ս�ļ�¼
			bool isAtt;		// false,Ϊ�ط���trueΪ�������������Ͷ�սʹ�ã�
			bool isAntiAtt;		// �Ƿ���Է���

			t_enterWar_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_ENTERWAR)
				{
					dwWarType = 0;
					dwSceneTempID = 0;
					dwUserID = 0;
					dwToRelationID = 0;
					dwFromRelationID = 0;
					dwStatus = 1;
					isAtt = false;
					isAntiAtt = false;
				};
		};

		// �ɳ������͸��Ự��֪ͨ������彻ս���
		const BYTE PARA_DARE_PK = 3;
		struct t_darePk_SceneSession : t_NullCmd
		{
			DWORD attID;    // �������û�ID
			DWORD defID;    // �������û�ID

			t_darePk_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_DARE_PK)
				{
					attID = 0;
					defID = 0;
				};
		};
			
		enum
		{
			DARE_GOLD,	// ��ս��
			RETURN_DARE_GOLD,// ������ս��
			WINNER_GOLD, // ������
			EMPEROR_GOLD, // �ʵ�ÿ�ս�����
		};
		
		// �ɻỰ���򳡾���֪ͨ����ĳ����Ͽ�Ǯ�����Ǯ
		const BYTE  PARA_DARE_GOLD = 4;
		struct t_dareGold_SceneSession : t_NullCmd
		{
			DWORD dwUserID;			/// ��ɫID
			int   dwNum;			/// ������Ľ�Ǯ��
			DWORD dwWarID;			/// ��սID
			DWORD dwType;                   /// ��Ǯ����, ����ս��0,����ս��1,��ս����2
			DWORD dwWarType;		/// ��ս����
				
			t_dareGold_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_DARE_GOLD) 
				{
					dwNum = 0;
					dwUserID = 0;
					dwWarID = 0;
				};
		};
		
		// �½������ս(���������Ự)
		const BYTE PARA_UNION_CITY_DARE = 5;
		struct t_UnionCity_Dare_SceneSession : t_NullCmd
		{
			DWORD dwCountryID;
			DWORD dwCityID;
			DWORD dwFromUserID;	// ������ս��
			DWORD dwFromUnionID;	// 
			DWORD dwToCountryID;
				
			t_UnionCity_Dare_SceneSession()
				:	t_NullCmd(CMD_SCENE_DARE, PARA_UNION_CITY_DARE) {}
		};

		// �����ս���(���������Ự)
		const BYTE PARA_UNION_CITY_DARE_RESULT = 6;
		struct t_UnionCity_DareResult_SceneSession : t_NullCmd
		{
			DWORD dwCountryID;
			DWORD dwCityID;
			DWORD dwUserID;	// Ӯ�Ұ���
			DWORD dwUnionID; // ��սӮ�Ұ��
				
			t_UnionCity_DareResult_SceneSession()
				:	t_NullCmd(CMD_SCENE_DARE, PARA_UNION_CITY_DARE_RESULT) {}
		};
		
		// GMָ��������ս(���������Ự)
		const BYTE PARA_GM_CREATE_UNION_CITY = 7;
		struct t_GMCreateUnionCity_SceneSession : t_NullCmd
		{
			DWORD dwCityID;
			DWORD dwCountryID;
			BYTE  byOpType;
				
			t_GMCreateUnionCity_SceneSession()
				:	t_NullCmd(CMD_SCENE_DARE, PARA_GM_CREATE_UNION_CITY) {}
		};
		
		// ���÷�����־(�Ự��������)
		const BYTE PARA_SET_ANTI_ATT_FLAG = 8;
		struct t_setAntiAttFlag_SceneSession : t_NullCmd
		{
			DWORD dwFromRelationID;
			DWORD dwToRelationID;
			DWORD dwType;
				
			t_setAntiAttFlag_SceneSession()
				:	t_NullCmd(CMD_SCENE_DARE, PARA_SET_ANTI_ATT_FLAG) {}
		};
		
		// ���ûʳ�����ս��ʼ��־(�Ự��������)
		const BYTE PARA_SET_EMPEROR_DARE = 9;
		struct t_setEmperorDare_SceneSession : t_NullCmd
		{
			t_setEmperorDare_SceneSession()
				:	t_NullCmd(CMD_SCENE_DARE, PARA_SET_EMPEROR_DARE) 
				{
					byState = 0;
					dwDefCountryID = 0;
				}
			
			BYTE byState; // 1,�ʳǽ�������ս,0,�ʳ��˳�����ս
			DWORD dwDefCountryID; // Ŀǰ�ʳǵ�ӵ����,Ϊ0��ʾ,û��
		};

		// �����ʳ�����ս(���������Ự)
		const BYTE PARA_BEGIN_EMPEROR_DARE = 10;
		struct t_beginEmperorDare_SceneSession : t_NullCmd
		{
			t_beginEmperorDare_SceneSession()
				:	t_NullCmd(CMD_SCENE_DARE, PARA_BEGIN_EMPEROR_DARE) 
				{
				}
		};
		//����ץ��
		const BYTE PARA_GOTO_LEADER = 11;
		struct t_GoTo_Leader_SceneSession  : t_NullCmd
		{
			t_GoTo_Leader_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_GOTO_LEADER) {};
			BYTE type;					//��������
			DWORD leaderTempID;	
			char mapName[MAX_NAMESIZE]; //��ͼ����
			WORD x;						//����x	
			WORD y;						//����y
		};
		//����ץ�˵�������֤
		const BYTE PARA_GOTO_LEADER_CHECK = 12;
		struct t_GoTo_Leader_Check_SceneSession  : t_NullCmd
		{
			t_GoTo_Leader_Check_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_GOTO_LEADER_CHECK) {};
			BYTE type;					//��������
			DWORD leaderTempID;	
			DWORD userTempID;	
			char mapName[MAX_NAMESIZE]; //��ͼ����
			WORD x;						//����x	
			WORD y;						//����y
		};

		//����ץ�˴������
		const BYTE PARA_CHECK_CALLTIMES_LEADER = 13;
		struct t_Check_CallTimes_SceneSession  : t_NullCmd
		{
			t_Check_CallTimes_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_CHECK_CALLTIMES_LEADER) {};
			BYTE type;					//��������
			DWORD leaderTempID;	
			DWORD qwThisID;				//����id
		};
		//����ʣ����ô���
		const BYTE PARA_RETURN_CALLTIMES_LEADER = 14;
		struct t_Return_CallTimes_SceneSession  : t_NullCmd
		{
			t_Return_CallTimes_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_RETURN_CALLTIMES_LEADER) {};
			BYTE type;					//��������
			DWORD leaderTempID;	
			DWORD times;
			DWORD qwThisID;				//����id
		};
		//������0GMָ��
		const BYTE PARA_RESET_CALLTIMES_LEADER = 15;
		struct t_Reset_CallTimes_SceneSession  : t_NullCmd
		{
			t_Reset_CallTimes_SceneSession()
				: t_NullCmd(CMD_SCENE_DARE, PARA_RESET_CALLTIMES_LEADER) {};
			DWORD leaderTempID;	
		};
		//////////////////////////////////////////////////////////////
		///  ��սָ�����
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		///  ���ָ�ʼ
		//////////////////////////////////////////////////////////////
		// ���������Ự���޸İ���ʽ�
		const BYTE OP_UNION_MONEY_PARA = 1;
		struct t_OpUnionMoney_SceneSession : t_NullCmd
		{
			DWORD dwUnionID; // ���ID
			int dwMoney; //  �ʽ�
			
			t_OpUnionMoney_SceneSession()
				: t_NullCmd(CMD_SCENE_UNION, OP_UNION_MONEY_PARA)
				{
					dwUnionID = 0;
					dwMoney = 0;
				}
		};

		// ���������Ự���޸İ���ж���
		const BYTE OP_UNION_ACTION_PARA = 2;
		struct t_OpUnionAction_SceneSession : t_NullCmd
		{
			DWORD dwUnionID; // ���ID
			int dwAction; //  �ʽ�
			
			t_OpUnionAction_SceneSession()
				: t_NullCmd(CMD_SCENE_UNION, OP_UNION_ACTION_PARA)
				{
					dwUnionID = 0;
					dwAction = 0;
				}
		};

		// �Ự���������������йذ�����ͨ��Ϣ�����ô浵
		const BYTE SEND_UNION_NORMAL_PARA = 3;
		struct t_SendUnionNormal_SceneSession : t_NullCmd
		{
			DWORD dwUserID; // �峤ID
			DWORD dwMana; // ��������
			
			t_SendUnionNormal_SceneSession()
				: t_NullCmd(CMD_SCENE_UNION, SEND_UNION_NORMAL_PARA)
				{
					dwUserID = 0;
					dwMana = 0;
				}
		};
		
		// ���һ����ҵ��ƶ��
		const BYTE PARA_CHECK_USER_CITY = 4;
		struct t_checkUserCity_SceneSession : t_NullCmd
		{
			t_checkUserCity_SceneSession()
				: t_NullCmd(CMD_SCENE_UNION, PARA_CHECK_USER_CITY)
				{
					byState   = 0;
					dwCheckID  = 0;
					dwCheckedID = 0;
				};
			
			BYTE byState; // 0, δͨ�����, 1,ͨ�����
			DWORD dwCheckID; // ����������ID
			DWORD dwCheckedID; // �������ҵ�ID
		};

		//////////////////////////////////////////////////////////////
		///  ���ָ�����
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		///  ����ָ�ʼ
		//////////////////////////////////////////////////////////////
		// ���������Ự��ת�������б�����
		const BYTE REQ_ARMY_LIST_SCENE_PARA = 1;
		struct t_ReqArmyList_SceneSession : t_NullCmd
		{
			BYTE byType;
			DWORD dwUserID;
			DWORD dwCityID;
			
			t_ReqArmyList_SceneSession()
				: t_NullCmd(CMD_SCENE_ARMY, REQ_ARMY_LIST_SCENE_PARA)
				{
					byType = 0;
					dwUserID = 0;
					dwCityID = 0;
				}
		};

		const BYTE SEND_USER_ARMY_INFO_PARA = 2;
		struct t_sendUserArmyInfo_SceneSession : t_NullCmd
		{
			char title[MAX_NAMESIZE]; // ������������
			BYTE byType; // 1Ϊ�ӳ���2Ϊ����
			DWORD dwUserID;
			
			t_sendUserArmyInfo_SceneSession()
				: t_NullCmd(CMD_SCENE_ARMY, SEND_USER_ARMY_INFO_PARA)
				{
					bzero(title, MAX_NAMESIZE);
					byType = 0;
					dwUserID = 0;
				}
		};
		//////////////////////////////////////////////////////////////
		///  ����ָ�����
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		///  ����ָ�ʼ
		//////////////////////////////////////////////////////////////
		// �Ự����������ˢ�����������޺�
		const BYTE SUMMON_GEMNPC_SCENE_PARA = 1;
		struct t_SummonGemNPC_SceneSession : t_NullCmd
		{
			DWORD dwMapID;
			DWORD x;
			DWORD y;
			DWORD dwBossID;

			t_SummonGemNPC_SceneSession()
				: t_NullCmd(CMD_SCENE_GEM, SUMMON_GEMNPC_SCENE_PARA)
				{
					dwMapID = 0;
					x = 0;
					y = 0;
					dwBossID = 0;
				}
		};
		
		// �Ự����������������������޺�
		const BYTE CLEAR_GEMNPC_SCENE_PARA = 2;
		struct t_ClearGemNPC_SceneSession : t_NullCmd
		{
			DWORD dwMapID;
			DWORD dwBossID;

			t_ClearGemNPC_SceneSession()
				: t_NullCmd(CMD_SCENE_GEM, CLEAR_GEMNPC_SCENE_PARA)
				{
					dwMapID = 0;
					dwBossID = 0;
				}
		};

		// �Ự�������������û���״̬���������ǣ�
		const BYTE SET_GEMSTATE_SCENE_PARA = 3;
		struct t_SetGemState_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwState; // 1Ϊ������2Ϊ����, 0Ϊ���������״̬

			t_SetGemState_SceneSession()
				: t_NullCmd(CMD_SCENE_GEM, SET_GEMSTATE_SCENE_PARA)
				{
					dwUserID = 0;
					dwState = 0;
				}
		};
		
		// �Ự����������ָ����������NPC�Ա�������Ʒ
		const BYTE BLAST_GEMNPC_SCENE_PARA = 4;
		struct t_BlastGemNPC_SceneSession : t_NullCmd
		{
			DWORD dwUserID; // ��ָ���������
			DWORD dwBossID; // Ϊָ��NPC��ID: 1002	����	1003	����	

			t_BlastGemNPC_SceneSession()
				: t_NullCmd(CMD_SCENE_GEM, BLAST_GEMNPC_SCENE_PARA)
				{
					dwUserID = 0;
					dwBossID = 0;
				}
		};
		
		// ���������Ự,���л���״̬ת��
		const BYTE CHANGE_GEMSTATE_SCENE_PARA = 5;
		struct t_ChangeGemState_SceneSession : t_NullCmd
		{
			DWORD fromUserID; // ��ĳ���
			DWORD toUserID; // ת�Ƶ�ĳ���

			DWORD dwState; // 1,������2,����

			t_ChangeGemState_SceneSession()
				: t_NullCmd(CMD_SCENE_GEM, CHANGE_GEMSTATE_SCENE_PARA)
				{
					fromUserID = 0;
					toUserID = 0;
					dwState = 0;
				}
		};

		// ���������Ự, ȡ��������ĳ����״̬
		const BYTE CANCEL_GEMSTATE_SCENE_PARA = 6;
		struct t_CancelGemState_SceneSession : t_NullCmd
		{
			DWORD dwUserID; // Ҫȡ�������
			DWORD dwState; // ��ȡ����״̬1,������2,����,0����״̬ȡ��

			t_CancelGemState_SceneSession()
				: t_NullCmd(CMD_SCENE_GEM, CANCEL_GEMSTATE_SCENE_PARA)
				{
					dwUserID = 0;
					dwState = 0;
				}
		};

		// ���������Ự, ���û�������ʼ�ͽ���
		const BYTE OP_GEMSTATE_SCENE_PARA = 7;
		struct t_OpGemState_SceneSession : t_NullCmd
		{
			DWORD dwState; // 1Ϊ��ʼ��0Ϊ����

			t_OpGemState_SceneSession()
				: t_NullCmd(CMD_SCENE_GEM, OP_GEMSTATE_SCENE_PARA)
				{
					dwState = 0;
				}
		};

		//////////////////////////////////////////////////////////////
		///  ����ָ�����
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		///  �Ƽ�ϵͳָ�ʼ
		//////////////////////////////////////////////////////////////
		// �Ự�������������н���
		const BYTE PICKUP_RECOMMEND_SCENE_PARA = 1;
		struct t_PickupRecommend_SceneSession : t_NullCmd
		{
			DWORD dwUserID;
			DWORD dwMoney;
			BYTE  byType; // 0,�Ƽ�����ȡ����, 1���Ƽ�����ȡ����

			t_PickupRecommend_SceneSession()
				: t_NullCmd(CMD_SCENE_RECOMMEND, PICKUP_RECOMMEND_SCENE_PARA)
				{
					dwUserID = 0;
					dwMoney = 0;
					byType = 0;
				}
		};
		//////////////////////////////////////////////////////////////
		///  �Ƽ�ϵͳָ�����
		//////////////////////////////////////////////////////////////
	};
};

#pragma pack()

#endif

