/**
 * \file
 * \version  $Id: InfoCommand.h  $
 * \author  
 * \date 
 * \brief �����������Ϣ�ɼ�ָ��
 */

#ifndef _InfoCommand_h
#define _InfoCommand_h

#include "zType.h"
#include "zNullCmd.h"

#pragma pack(1)
namespace Cmd
{
	namespace Info
	{
		const BYTE CMD_LOGIN = 1;
		const BYTE CMD_INFO = 2;

		///////////////////////////////////////
		///���������ָ��
		//////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginCmd : t_NullCmd
		{
			t_LoginCmd() : t_NullCmd(CMD_LOGIN, PARA_LOGIN) { }
		};
		///////////////////////////////////////
		///���������ָ��
		///////////////////////////////////////

		//////////////////////////////////////
		///��������Ϣָ��
		//////////////////////////////////////
		//�����������Ϣ
		const BYTE PARA_REQUEST_SERVERINFO = 1;
		struct t_Request_ServerInfo : t_NullCmd
		{
			QWORD 		rTimestamp;							//����ʱ���

			t_Request_ServerInfo() : t_NullCmd(CMD_INFO, PARA_REQUEST_SERVERINFO) { }
		};
		//��������Ϣ
		const BYTE PARA_SERVERINFO = 2;
		struct t_ServerInfo : t_NullCmd
		{
			QWORD 		rTimestamp;							//����ʱ���
			WORD 			ServerID;							//���������
			WORD 			ServerType;							//����������
			GameZone_t	GameZone;							//��Ϸ�����
			char 			ZoneName[MAX_NAMESIZE];			//��Ϸ������
			DWORD 		ConnNum;								//������
			char 			xml[1];

			t_ServerInfo() : t_NullCmd(CMD_INFO, PARA_SERVERINFO)
			{
				xml[0] = '\0';
			}
		};
		//////////////////////////////////////
		///��������Ϣָ��
		//////////////////////////////////////
		
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////		
		
		//��������������Ϣ
		const BYTE PARA_REQUEST_ONLINENUM = 3;
		struct t_Request_OnlineNum : t_NullCmd
		{
			QWORD			rTimestamp;
			
			t_Request_OnlineNum() : t_NullCmd(CMD_INFO, PARA_REQUEST_ONLINENUM) { }
		};
		const BYTE PARA_ONLINENUM =4;
		struct t_OnlineNum : t_NullCmd
		{
			QWORD			rTimestamp;
			WORD			ServerID;
			WORD			ServerType;
			GameZone_t	GameZone;
			char			ZoneName[MAX_NAMESIZE];
			DWORD			OnlineNum;
			
			t_OnlineNum() : t_NullCmd(CMD_INFO, PARA_ONLINENUM) { }	
		};
	};
};
#pragma pack()

#endif

