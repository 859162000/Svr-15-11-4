/**
 * \file
 * \version  $Id: RecordCommand.h  $
 * \author  
 * \date 
 * \brief ���嵵�����������ָ��
 *
 */


#ifndef _RecordCommand_h_
#define _RecordCommand_h_

#include "zNullCmd.h"
#include "Command.h"
#include "CharBase.h"

#pragma pack(1)

namespace Cmd
{

	namespace Record
	{
		const BYTE CMD_LOGIN = 1;
		const BYTE CMD_GATE = 2;
		const BYTE CMD_SCENE = 3;
		const BYTE CMD_SESSION = 4;

		//////////////////////////////////////////////////////////////
		/// ��½����������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginRecord : t_NullCmd
		{
			WORD wdServerID;
			WORD wdServerType;
			t_LoginRecord()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN) {};
		};
		//////////////////////////////////////////////////////////////
		/// ��½����������ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// ���������������ؽ�����ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_GATE_GET_SELECTINFO = 1;
		struct t_Get_SelectInfo_GateRecord : t_NullCmd
		{
			DWORD accid;					/// �˺�
			t_Get_SelectInfo_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_GET_SELECTINFO) {};
		};

		const BYTE PARA_GATE_RET_SELECTINFO = 2;
		struct t_Ret_SelectInfo_GateRecord : t_NullCmd
		{
			DWORD accid;						/// �˺�
			WORD num;							/// ��Ч��ɫ��Ϣ����
			SelectUserInfo info[MAX_CHARINFO];	/// ��ɫ��Ϣ�б�
			t_Ret_SelectInfo_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_RET_SELECTINFO)
				{
					num = 0;
					bzero(info, sizeof(info));
				}
		};

		const BYTE PARA_GATE_CREATECHAR = 3;
		struct t_CreateChar_GateRecord : t_NullCmd
		{
			DWORD accid;						/// �˺�
			char  name[MAX_NAMESIZE+1];		/// ��ɫ����
			WORD  type;						/// ��ɫ����
			DWORD hair;						/// ��ɫ�����Լ���ɫ
			WORD country;				/// ����ID
			WORD five;					/// ����������
			char mapName[MAX_NAMESIZE];	/**< �û���ͼ����  */
			DWORD createip;				/// ������ɫʱ��ip
			t_CreateChar_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_CREATECHAR) {};
		};

		const BYTE PARA_GATE_CREATECHAR_RETURN = 4;
		struct t_CreateChar_Return_GateRecord : t_NullCmd
		{
			DWORD accid;						/// �˺�
			BYTE retcode;						/// ���ش��룬0��ʾʧ�ܣ���ɫ�����ظ���1��ʾ������ɫ�ɹ�
			SelectUserInfo charinfo;			/// ��ɫ��Ϣ
			DWORD dwFace;					// ��ɫ����
			t_CreateChar_Return_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_CREATECHAR_RETURN) {};
		};

		const BYTE PARA_GATE_DELCHAR = 5;
		struct t_DelChar_GateRecord : t_NullCmd
		{
			DWORD accid;						/// �˺�
			DWORD id;							/// ��ɫ���
			char name[MAX_NAMESIZE+1];
			t_DelChar_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_DELCHAR) {};
		};

		const BYTE PARA_GATE_DELCHAR_RETURN = 6;
		struct t_DelChar_Return_GateRecord : t_NullCmd
		{
			DWORD accid;						/// �˺�
			DWORD id;							/// ��ɫ���
			BYTE retcode;						/// ���ش��룬0��ʾɾ����ɫʧ�ܣ�1��ʾɾ����ɫ�ɹ�
			char name[MAX_NAMESIZE+1];
			t_DelChar_Return_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_DELCHAR_RETURN) {};
		};

		const BYTE PARA_GATE_CHECKNAME = 7;
		struct t_CheckName_GateRecord : t_NullCmd
		{
			DWORD accid;						/// �˺�
			char name[MAX_NAMESIZE+1];
			t_CheckName_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_CHECKNAME) {};
		};

		const BYTE PARA_GATE_CHECKNAME_RETURN = 8;
		struct t_CheckName_Return_GateRecord : t_NullCmd
		{
			DWORD accid;						/// �˺�
			BYTE err_code;						/// ���ش��� 0: ����ʹ�� 1:��ɫ���ظ�
			char name[MAX_NAMESIZE+1];
			t_CheckName_Return_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_CHECKNAME_RETURN) {};
		};
		/// ������ҵ�����������
		const BYTE REQUEST_GATE_COUNTRY_ORDER = 9;
		struct t_request_Country_GateRecord : t_NullCmd
		{
			t_request_Country_GateRecord()
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
		/// ���ҵ�����������
		const BYTE PARA_GATE_COUNTRY_ORDER = 10;
		struct t_order_Country_GateRecord : t_NullCmd
		{
			DWORD dwID;					//�û�id
			CountrOrder order;			//����Ĺ��� 
			t_order_Country_GateRecord()
				: t_NullCmd(CMD_GATE, PARA_GATE_COUNTRY_ORDER) {};
		};
		//////////////////////////////////////////////////////////////
		/// ���������������ؽ�����ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// �����������ͳ���������ָ��
		//////////////////////////////////////////////////////////////
#define PARA_SCENE_USER_READ_ERROR -1
		const BYTE PARA_SCENE_USER_READ = 1;
		struct t_ReadUser_SceneRecord : t_NullCmd
		{
			DWORD accid;
			DWORD id;						/// ��ɫ
			DWORD dwMapTempID;				/// ��ͼ��ʱID
			t_ReadUser_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_USER_READ) {};
		};

		enum WriteBack_Type
		{
			TIMETICK_WRITEBACK,		/// ��ʱ��д����
			LOGOUT_WRITEBACK,		/// �˳���д����
			CHANGE_SCENE_WRITEBACK, //�л�����ʱ��д����
			OPERATION_WRITEBACK,		/// ������д����
		};
		const BYTE PARA_SCENE_USER_WRITE = 2;
		struct t_WriteUser_SceneRecord : t_NullCmd
		{
			DWORD accid;
			DWORD id;							/// ��ɫ
			DWORD dwMapTempID;					/// ��ͼ��ʱID
			DWORD writeback_type;				/// ��д��������
			CharBase    charbase;				///�浵�Ļ�����Ϣ
			DWORD      dataSize;				///�浵�����ƵĴ�С
			char        data[0];				///�浵�Ķ���������
			t_WriteUser_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_USER_WRITE)
				{
					bzero(&charbase , sizeof(charbase));
					dataSize = 0;
				}
		};

		const BYTE PARA_SCENE_USERINFO = 3;
		struct t_UserInfo_SceneRecord : t_NullCmd
		{
			DWORD id;							/// ��ɫ
			DWORD dwMapTempID;					/// ��ͼ��ʱID
			CharBase    charbase;				///�浵�Ļ�����Ϣ
			DWORD      dataSize;				///�浵�����ƵĴ�С
			char        data[0];				///�浵�Ķ���������
			t_UserInfo_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_USERINFO)
				{
					bzero(&charbase , sizeof(charbase));
					dataSize = 0;
				}
		};

		//ɾ�������������û�����,������û�ע��󵵰�������δ���ض��յ��˳�ָ��ʱ��Ҫʹ��
		const BYTE PARA_SCENE_USER_REMOVE = 4;
		struct t_RemoveUser_SceneRecord : t_NullCmd
		{
			DWORD accid;
			DWORD id;
			t_RemoveUser_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_USER_REMOVE)
				{
				}
		};

		//����û��Ƿ����
		const BYTE PARA_SCENE_USER_EXIST = 5;
		struct t_userExist_SceneRecord : t_NullCmd
		{
			DWORD fromID;//��ʱID
			DWORD toID;
			Cmd::stSendMail sm;
			t_userExist_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_USER_EXIST)
				{
					fromID = 0;
					toID = 0;
					bzero(&sm, sizeof(sm));
				}
		};
		const BYTE PARA_SCENE_USER_WRITE_OK = 6;
		struct t_WriteUser_SceneRecord_Ok : t_NullCmd
		{
			DWORD type;						/// ж������
			DWORD accid;
			DWORD id;						/// ��ɫ
			t_WriteUser_SceneRecord_Ok()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_USER_WRITE_OK) {};
		};

#ifdef _TEST_DATA_LOG
		enum enumWriteBackTest_Type
		{
			NEWCHAR_WRITEBACK,//�½�ɫ����
			LEVELUP_WRITEBACK,//����
			DEATH_WRITEBACK,//����
			HP_WRITEBACK,//HP
			MP_WRITEBACK,//MP
			SP_WRITEBACK,//SP
		};
		const BYTE PARA_SCENE_INSERT_CHARTEST = 4;
		struct t_Insert_CharTest_SceneRecord : t_NullCmd
		{
			char name[MAX_NAMESIZE];				/// ��ɫ����
			DWORD level;							/// ��ɫ�ȼ�
			CharTest    chartest;				///�浵����
			t_Insert_CharTest_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_INSERT_CHARTEST)
				{
					bzero(&chartest , sizeof(chartest));
				}
		};
		const BYTE PARA_SCENE_UPDATE_CHARTEST = 5;
		struct t_Update_CharTest_SceneRecord : t_NullCmd
		{
			char name[MAX_NAMESIZE];				/// ��ɫ����
			DWORD level;							/// ��ɫ�ȼ�
			CharTest    chartest;				///�浵����
			t_Update_CharTest_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_UPDATE_CHARTEST)
				{
					bzero(&chartest , sizeof(chartest));
				}
		};
		const BYTE PARA_SCENE_DELETE_CHARTEST = 6;
		struct t_Delete_CharTest_SceneRecord : t_NullCmd
		{
			char name[MAX_NAMESIZE];				/// ��ɫ����
			DWORD level;							/// ��ɫ�ȼ�
			t_Delete_CharTest_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_DELETE_CHARTEST)
				{
				}
		};
		const BYTE PARA_SCENE_READ_CHARTEST = 7;
		struct t_Read_CharTest_SceneRecord : t_NullCmd
		{
			char name[MAX_NAMESIZE];				/// ��ɫ����
			DWORD level;							/// ��ɫ�ȼ�
			CharTest    chartest;				///�浵����
			t_Read_CharTest_SceneRecord()
				: t_NullCmd(CMD_SCENE, PARA_SCENE_READ_CHARTEST)
				{
					bzero(&chartest , sizeof(chartest));
				}
		};
#endif

		//////////////////////////////////////////////////////////////
		/// �����������ͳ���������ָ��
		//////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////
		/// �����������ͻỰ������ָ�ʼ
		//////////////////////////////////////////////////////////////
		//����û��Ƿ����
		const BYTE PARA_CHK_USER_EXIST = 1;
		struct t_chkUserExist_SessionRecord : t_NullCmd
		{
			char  name[MAX_NAMESIZE+1];		/// ��ɫ����
			DWORD user_id;					/// �Ƽ���ID
			DWORD  from_id;					/// ���Ƽ���ID
			DWORD user_level;				/// �Ƽ��˵ȼ�

			t_chkUserExist_SessionRecord()
				: t_NullCmd(CMD_SESSION, PARA_CHK_USER_EXIST)
				{
					bzero(name, sizeof(name));
					user_id = 0;
					user_level = 0;
					from_id = 0;
				}
		};

		//////////////////////////////////////////////////////////////
		/// �����������ͻỰ������ָ�����
		//////////////////////////////////////////////////////////////

	};

};

#pragma pack()

#endif

