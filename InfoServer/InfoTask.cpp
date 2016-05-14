/**
 * \file
 * \version  $Id: InfoTask.cpp  $
 * \author  
 * \date 
 * \brief 定义数据库访问服务器的任务
 */

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <ext/hash_map>

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "Zebra.h"
#include "zMisc.h"
#include "zDBConnPool.h"
#include "InfoCommand.h"
#include "InfoTask.h"
#include "InfoServer.h"

int InfoTask::verifyConn()
{
	int retcode = mSocket.recvToBuf_NoPoll();
	if ( retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdlen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdlen <= 0)
		{
			return 0;
		}
		else
		{
			using namespace Cmd::Info;
			t_LoginCmd *ptCmd = (t_LoginCmd *)pstrCmd;
			if (CMD_LOGIN == ptCmd->cmd && PARA_LOGIN == ptCmd->para)
			{
				Zebra::logger->debug("superServer连接通过验证");
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		return retcode;
	}
}


int InfoTask::waitSync()
{
	using namespace Cmd::Info;
	t_LoginCmd cmd;
	if (sendCmd(&cmd, sizeof(cmd)))
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

void InfoTask::addToContainer()
{
	pContainer->add(this);
}

void InfoTask::removeFromContainer()
{
	pContainer->remove(this);
}

bool InfoTask::msgParse(const Cmd::t_NullCmd * ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Info;
	switch(ptNullCmd->cmd)
	{
		case CMD_INFO:
			if (msgParse_loginServer(ptNullCmd, nCmdLen))
			{
				return true;
			}
		break;
	}
	return false;
}

bool InfoTask::msgParse_loginServer(const Cmd::t_NullCmd * ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Info;
	switch(ptNullCmd->para)
	{
		case PARA_SERVERINFO:
		{
			t_ServerInfo *pCmd = (t_ServerInfo *)ptNullCmd;

			//表列属性
			static const dbCol info_col_define[] = {
				{"`rTimestamp`",	zDBConnPool::DB_QWORD,	sizeof(QWORD)},
				{"`ServerID`",		zDBConnPool::DB_WORD,	sizeof(WORD)},
				{"`ServerType`",	zDBConnPool::DB_WORD, 	sizeof(WORD)},
				{"`GameZone`",		zDBConnPool::DB_DWORD,	sizeof(DWORD)},
				{"`ZoneName`",		zDBConnPool::DB_STR,		sizeof(char[MAX_NAMESIZE])},
				{"`OnlineNum`",	zDBConnPool::DB_DWORD,	sizeof(DWORD)},
				{"`Info`",			zDBConnPool::DB_STR,		5120},
				{NULL,				0,								0}
			};
			//表列属性对应的数据结构
			struct {
				QWORD 	rTimestamp;						//请求时间戳
				WORD 		ServerID;						//服务器编号
				WORD 		ServerType;                //服务器类型
				DWORD		ID;								//游戏区编号
				char 		ZoneName[MAX_NAMESIZE];    //区名字
				DWORD		OnlineNum;                 //在线人数
				char		info[5120];						//服务器时实信息
			} __attribute__ ((packed))
			serverinfo_data;

			memset(&serverinfo_data, 0, sizeof(serverinfo_data));
			serverinfo_data.rTimestamp		= 	pCmd->rTimestamp;
			serverinfo_data.ServerID 		= 	pCmd->ServerID;
			serverinfo_data.ServerType		=	pCmd->ServerType;
			serverinfo_data.ID   			=  pCmd->GameZone.id;
			strncpy(serverinfo_data.ZoneName, pCmd->ZoneName, MAX_NAMESIZE);
			serverinfo_data.OnlineNum     =   pCmd->ConnNum;
			strncpy(serverinfo_data.info, pCmd->xml, 5120 - 1);
			
			//Zebra::logger->debug("rTimestamp = %ld", serverinfo_data.rTimestamp);
			//Zebra::logger->debug("ServerID = %d", serverinfo_data.ServerID);
			//Zebra::logger->debug("ServerType = %d", serverinfo_data.ServerType);
			//Zebra::logger->debug("ID = %d", serverinfo_data.ID);
			//Zebra::logger->debug("ZoneName = %s", serverinfo_data.ZoneName);
			//Zebra::logger->debug("OnlineNum = %ld", serverinfo_data.OnlineNum);
			//Zebra::logger->debug("%s", serverinfo_data.info);
			
			connHandleID handle = InfoService::dbConnPool->getHandle();//用默认hashcode
			if ((connHandleID)-1 != handle)
			{
				const char *sql_template = "create table if not exists `%s` ( `NO` int(10) unsigned NOT NULL auto_increment,`rTimestamp` bigint(20) unsigned default '0',  `ServerType` int(10) unsigned default '0',  `ServerID` int(10) unsigned default '0',`GameZone` int(10) unsigned default '0', `ZoneName` varchar(100) default NULL,`OnlineNum` int(10) unsigned default '0', `Info` blob, PRIMARY KEY  (`NO`), KEY `Index_2` (`ServerID`,`GameZone`,`rTimestamp`), KEY `Index_3` (`GameZone`,`rTimestamp`))";
				char name[32];  //表名
				char sqlBuf[512]; //存放SQL语句缓冲区
				char timeBuffer[64];//存放时间戳缓冲
				bzero(name, sizeof(name));
				bzero(sqlBuf, sizeof(sqlBuf));
				bzero(timeBuffer, sizeof(timeBuffer));
				zMisc::getTimeString((time_t)serverinfo_data.rTimestamp,timeBuffer, sizeof(timeBuffer), "%Y%m%d");
				sprintf(name, "SERVERINFO%s", timeBuffer);

				sprintf(sqlBuf, sql_template, "SERVERREALTIME");
				if ((int)-1 == InfoService::dbConnPool->execSql(handle, sqlBuf, strlen(sqlBuf)))
				{
					Zebra::logger->debug("执行SQL语句出错");
					InfoService::dbConnPool->putHandle(handle);
					return false;
				}

				sprintf(sqlBuf, sql_template, name);
				if ((int)-1 == InfoService::dbConnPool->execSql(handle, sqlBuf, strlen(sqlBuf)))
				{
					Zebra::logger->debug("执行SQL语句出错");
					InfoService::dbConnPool->putHandle(handle);
					return false;
				}

				if ((unsigned int) -1 != InfoService::dbConnPool->exeInsert(handle, name, info_col_define, (unsigned char *)&serverinfo_data))
				{
					char buffer[strlen(serverinfo_data.info)+256];
					memset(buffer, 0, sizeof(buffer));
					sprintf(buffer, "replace into SERVERREALTIME ( rTimestamp, ServerType, ServerID, GameZone, ZoneName, OnlineNum, 	Info) VALUES (%lld, %d, %d, %d, '%s',%d, '%s')", serverinfo_data.rTimestamp, serverinfo_data.ServerType,serverinfo_data.ServerID,serverinfo_data.ID,serverinfo_data.ZoneName,serverinfo_data.OnlineNum,serverinfo_data.info);
					int sqllen = strlen(buffer);
					if ((int)-1 == InfoService::dbConnPool->execSql(handle, buffer, sqllen))
					{
						Zebra::logger->debug("执行SQL语句出错");
						InfoService::dbConnPool->putHandle(handle);
						return false;
					}
					InfoService::dbConnPool->putHandle(handle);
					return true;
				}
				InfoService::dbConnPool->putHandle(handle);
			}
			return false;
		}
		break;
		case PARA_ONLINENUM:
		{
			t_OnlineNum *ptCmd = (t_OnlineNum *)ptNullCmd;
			//表列属性
			static const dbCol num_col_define[] = {
				{"`rTimestamp`",	zDBConnPool::DB_QWORD,	sizeof(QWORD)},
				{"`ServerID`",		zDBConnPool::DB_WORD,	sizeof(WORD)},
				{"`ServerType`",	zDBConnPool::DB_WORD, 	sizeof(WORD)},
				{"`GameZone`",		zDBConnPool::DB_DWORD,	sizeof(DWORD)},
				{"`ZoneName`",		zDBConnPool::DB_STR,		sizeof(char[MAX_NAMESIZE])},
				{"`OnlineNum`",	zDBConnPool::DB_DWORD,	sizeof(DWORD)},
				{NULL,				0,								0}
			};
			//表列属性对应的数据结构
			struct {
				QWORD 	rTimestamp;						//请求时间戳
				WORD 		ServerID;						//服务器编号
				WORD 		ServerType;                //服务器类型
				DWORD		ID;								//游戏区编号
				char 		ZoneName[MAX_NAMESIZE];    //区名字
				DWORD		OnlineNum;                 //在线人数
			} __attribute__ ((packed))
			onlinenum_data;
			
			memset(&onlinenum_data, 0, sizeof(onlinenum_data));
			onlinenum_data.rTimestamp		= 	ptCmd->rTimestamp;
			onlinenum_data.ServerID 		= 	ptCmd->ServerID;
			onlinenum_data.ServerType		=	ptCmd->ServerType;
			onlinenum_data.ID   				=  ptCmd->GameZone.id;
			strncpy(onlinenum_data.ZoneName, ptCmd->ZoneName, MAX_NAMESIZE);
			onlinenum_data.OnlineNum     	=  ptCmd->OnlineNum;
			
			//Zebra::logger->debug("rTimestamp = %ld", onlinenum_data.rTimestamp);
			//Zebra::logger->debug("ServerID = %d", onlinenum_data.ServerID);
			//Zebra::logger->debug("ServerType = %d", onlinenum_data.ServerType);
			//Zebra::logger->debug("ID = %d", onlinenum_data.ID);
			//Zebra::logger->debug("ZoneName = %s", onlinenum_data.ZoneName);
			//Zebra::logger->debug("OnlineNum = %ld", onlinenum_data.OnlineNum);
			
			connHandleID handle = InfoService::dbConnPool->getHandle();//用默认hashcode
			if ((connHandleID)-1 != handle)
			{
				const char *sql_template = "create table if not exists `%s` ( `NO` int(10) unsigned NOT NULL auto_increment,`rTimestamp` bigint(20) unsigned default '0',  `ServerType` int(10) unsigned default '0',  `ServerID` int(10) unsigned default '0',`GameZone` int(10) unsigned default '0', `ZoneName` varchar(100) default NULL,`OnlineNum` int(10) unsigned default '0', PRIMARY KEY  (`NO`), KEY `Index_2` (`ServerID`,`GameZone`,`rTimestamp`), KEY `Index_3` (`GameZone`,`rTimestamp`))";
				char name[32];  //表名
				char sqlBuf[512]; //存放SQL语句缓冲区
				char timeBuffer[64];//存放时间戳缓冲
				bzero(name, sizeof(name));
				bzero(sqlBuf, sizeof(sqlBuf));
				bzero(timeBuffer, sizeof(timeBuffer));
				zMisc::getTimeString((time_t)onlinenum_data.rTimestamp,timeBuffer, sizeof(timeBuffer), "%Y%m%d");
				sprintf(name, "ONLINENUM%s", timeBuffer);

				sprintf(sqlBuf, sql_template, "ONLINENUMREALTIME");
				if ((int)-1 == InfoService::dbConnPool->execSql(handle, sqlBuf, strlen(sqlBuf)))
				{
					Zebra::logger->debug("执行SQL语句出错");
					InfoService::dbConnPool->putHandle(handle);
					return false;
				}

				sprintf(sqlBuf, sql_template, name);
				if ((int)-1 == InfoService::dbConnPool->execSql(handle, sqlBuf, strlen(sqlBuf)))
				{
					Zebra::logger->debug("执行SQL语句出错");
					InfoService::dbConnPool->putHandle(handle);
					return false;
				}

				if ((unsigned int) -1 != InfoService::dbConnPool->exeInsert(handle, name, num_col_define, (unsigned char *)&onlinenum_data))
				{
					char buffer[1024];
					memset(buffer, 0, sizeof(buffer));
					sprintf(buffer, "replace into ONLINENUMREALTIME ( rTimestamp, ServerType, ServerID, GameZone, ZoneName, OnlineNum) VALUES (%lld, %d, %d, %d, '%s',%d)", onlinenum_data.rTimestamp, onlinenum_data.ServerType,onlinenum_data.ServerID,onlinenum_data.ID,onlinenum_data.ZoneName,onlinenum_data.OnlineNum);
					int sqllen = strlen(buffer);
					if ((int)-1 == InfoService::dbConnPool->execSql(handle, buffer, sqllen))
					{
						Zebra::logger->debug("执行SQL语句出错");
						InfoService::dbConnPool->putHandle(handle);
						return false;
					}
					InfoService::dbConnPool->putHandle(handle);
					return true;
				}
				InfoService::dbConnPool->putHandle(handle);
			}
			return false;
		}
		break;
	}
	return false;
}
