/**
 * \file
 * \version  $Id: RoleTask.h  $
 * \author 
 * \date 
 * \brief 定义数据库访问服务器的任务
 */

#ifndef _RoleTask_h_
#define _RoleTask_h_

#include <iostream>
#include <string>
#include <vector>
#include <ext/numeric>

#include "zService.h"
#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zMisc.h"

/**
* \brief 存放角色信息的结构体
*/
struct RoleData
{  
	char name[MAX_NAMESIZE];//角色名
	unsigned short game; //游戏编号
	unsigned short zone; //游戏区编号
	unsigned int accid;	//帐号编号
	
	RoleData(){accid=0;zone=0;game=0;memset(name, 0, sizeof(name));}
	RoleData(const RoleData &rd)
	{
		accid    = rd.accid;
		zone     = rd.zone;
		game     = rd.game;
		bcopy(rd.name, name, sizeof(name));
	}
	const RoleData & operator= (const RoleData &rd)
	{
		accid    = rd.accid;
		zone     = rd.zone;
		game     = rd.game;
		strcpy(name, rd.name);
		return *this;
	}
};


/**
 * \brief 定义数据库访问服务器的任务类
 */
class RoleTask : public zTCPTask
{
	public:
	
		int verifyConn();
		int waitSync();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
	
		/**
		 * \brief 构造函数
		 * \param pool 所属连接池
		 * \param sock TCP/IP套接口
		 * \param addr 地址
		 */
		RoleTask(zTCPTaskPool *pool, const int sock, 
					const struct sockaddr_in *addr = NULL) : zTCPTask(pool, sock, addr)
		{
		}
		
		/**
		 * \brief 析构函数
		 */
		~RoleTask()
		{
		}
		
	private:
		
		bool msgParse_loginServer(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
};

#endif

