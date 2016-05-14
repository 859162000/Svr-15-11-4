/**
 * \file
 * \version  $Id: InfoCommand.h  $
 * \author  
 * \date 
 * \brief 定义服务器信息采集指令
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
		///登入服务器指令
		//////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginCmd : t_NullCmd
		{
			t_LoginCmd() : t_NullCmd(CMD_LOGIN, PARA_LOGIN) { }
		};
		///////////////////////////////////////
		///登入服务器指令
		///////////////////////////////////////

		//////////////////////////////////////
		///服务器信息指令
		//////////////////////////////////////
		//请求服务器信息
		const BYTE PARA_REQUEST_SERVERINFO = 1;
		struct t_Request_ServerInfo : t_NullCmd
		{
			QWORD 		rTimestamp;							//请求时间戳

			t_Request_ServerInfo() : t_NullCmd(CMD_INFO, PARA_REQUEST_SERVERINFO) { }
		};
		//服务器信息
		const BYTE PARA_SERVERINFO = 2;
		struct t_ServerInfo : t_NullCmd
		{
			QWORD 		rTimestamp;							//请求时间戳
			WORD 			ServerID;							//服务器编号
			WORD 			ServerType;							//服务器类型
			GameZone_t	GameZone;							//游戏区编号
			char 			ZoneName[MAX_NAMESIZE];			//游戏区名字
			DWORD 		ConnNum;								//连接数
			char 			xml[1];

			t_ServerInfo() : t_NullCmd(CMD_INFO, PARA_SERVERINFO)
			{
				xml[0] = '\0';
			}
		};
		//////////////////////////////////////
		///服务器信息指令
		//////////////////////////////////////
		
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////		
		
		//请求在线人数信息
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

