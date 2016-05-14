/**
 * \file
 * \version  $Id: RoleTask.cpp  $
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
#include "RoleTask.h"
#include "Zebra.h"
#include "zDBConnPool.h"
#include "roleRegServer.h"
#include "URLManager.h"
#include "RoleregCommand.h"
#include "Zebra.h"

/**
 * \brief 封装一个容器类
 */
class RoleDataContainer
{

	public:

		/**
		 * \brief 构造函数
		 */
		RoleDataContainer() {};

		/**
		 * \brief 析构函数
		 */
		~RoleDataContainer()
		{
			mlock.lock();
			barrel.clear();
			mlock.unlock();
		}

		/**
		 * \brief 添加数据到容器中
		 * \param data 待添加的容器
		 * \return 添加到容器是否成功
		 */
		bool add(const RoleData &data)
		{
			zMutex_scope_lock scope_lock(mlock);
			container_type::const_iterator it = barrel.find(data.name);
			if (it == barrel.end())
			{
				//没有找到
				std::pair<container_type::iterator, bool> p = barrel.insert(container_type::value_type(data.name, data));
				return p.second;
			}
			else
			{
				//找到存在
				return false;
			}
		}

		/**
		 * \brief 从容器中删除指定名称的内容
		 * \param name 指定删除的名称
		 */
		void remove(const char *name)
		{
			zMutex_scope_lock scope_lock(mlock);
			container_type::iterator it = barrel.find(name);
			if (it != barrel.end())
			{
				//找到存在
				barrel.erase(it);
			}
		}

	private:

		struct eqstr
		{
			bool operator()(const char* s1, const char* s2) const
			{
				return strcmp(s1, s2) == 0;
			}
		};
		/**
		 * \brief 容器类型
		 */
		typedef __gnu_cxx::hash_map<const char *, RoleData, __gnu_cxx::hash<const char *>, eqstr> container_type;
		/**
		 * \brief 容器访问互斥变量
		 */
		zMutex mlock;
		/**
		 * \brief 容器变量
		 */
		container_type barrel;

};

static RoleDataContainer SetRoleInfo; //存放角色信息的container

int RoleTask::verifyConn()
{
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
			return 0;
		else
		{
			using namespace Cmd::RoleReg;
			t_LoginRoleReg *ptCmd = (t_LoginRoleReg *)pstrCmd;
			if (CMD_LOGIN == ptCmd->cmd && PARA_LOGIN == ptCmd->para)
			{
				Zebra::logger->debug("客户端连接通过验证");
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

int RoleTask::waitSync()
{
	using namespace Cmd::RoleReg;
	t_LoginRoleReg cmd;
	Zebra::logger->debug("$$$$$$$$$");
	if (sendCmd(&cmd, sizeof(cmd)))
		return 1;
	else
		return -1;
}

/**
 * \brief 解析来自各个连接的指令
 * \param ptNullCmd 待处理的指令
 * \param nCmdLen 指令长度
 * \return 处理是否成功
 */
bool RoleTask::msgParse(const Cmd::t_NullCmd * ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::RoleReg;
	switch(ptNullCmd->cmd)
	{
		case CMD_ROLEREG:
			if (msgParse_loginServer(ptNullCmd, nCmdLen))
			{
				return true;
			}
		break;
	}
	return false;
}

/**
 * \brief 角色的创建与删除
 * \param ptNullCmd 待处理指令
 * \param nCmdLen 指令长度
 * \return 创建删除角色是否成功
 */
bool RoleTask::msgParse_loginServer(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::RoleReg;
	switch(ptNullCmd->para)
	{
		case PARA_CHARNAME_ROLEREG:
		{
			t_Charname_Rolereg *ptCmd = (t_Charname_Rolereg *)ptNullCmd;
			t_Charname_Rolereg Cmd;
			Cmd.wdServerID 	= ptCmd->wdServerID;
			Cmd.accid			= ptCmd->accid;
			Cmd.gameZone	= ptCmd->gameZone;
			strcpy(Cmd.name, ptCmd->name);
			Cmd.state = ptCmd->state;

			unsigned int hash = URLManager::hashString(ptCmd->name);
			Zebra::logger->debug("====表名=====%s", URLManager::getInstance().roleregTableName(hash).c_str());
			Zebra::logger->debug("hash=%u", hash);
			Zebra::logger->debug("ptCmd->state=%u", ptCmd->state);

			if (ptCmd->state & ROLEREG_STATE_TEST)//检测角色名
			{
				RoleData tmpinfo;
				//将数据COPY一份
				tmpinfo.accid = ptCmd->accid;
				tmpinfo.zone= ptCmd->gameZone.zone;
				tmpinfo.game=  ptCmd->gameZone.game;
				strcpy(tmpinfo.name, ptCmd->name);
				Zebra::logger->debug("accid=%u",	tmpinfo.accid);
				Zebra::logger->debug("zone=%u",	tmpinfo.zone);
				Zebra::logger->debug("game=%u",	tmpinfo.game);
				Zebra::logger->debug("name=%s",	tmpinfo.name);
				if (SetRoleInfo.add(tmpinfo))
				{
					//添加成功，表示原来没有数据
					char where[128];
					DWORD accID=0;
					static const dbCol sel_con_define[]	= {
						{"`ACCID`", zDBConnPool::DB_DWORD, sizeof(DWORD)},
						{NULL, 0, 0}
					};
					memset(where, 0, sizeof(where));
					sprintf(where, "NAME = '%s'", tmpinfo.name);
					connHandleID handle = roleRegService::dbConnPool->getHandle(&hash);
					Zebra::logger->debug("handle=%u", handle);
					if ((connHandleID)-1 != handle)
					{
						if ((unsigned int)1 == roleRegService::dbConnPool->exeSelectLimit(handle, 
									URLManager::getInstance().roleregTableName(hash).c_str(), 
									sel_con_define, where, NULL, 1, (unsigned char *)&accID))
						{
							//名字已存在数据库中，清除缓冲中的数据
							SetRoleInfo.remove(tmpinfo.name);
							Cmd.state |= ROLEREG_STATE_HAS;
							Zebra::logger->debug("名字已存在数据库中");
						}
						roleRegService::dbConnPool->putHandle(handle);
					}
					else
					{
						//获取数据库句柄出错，清除缓冲中的数据
						SetRoleInfo.remove(tmpinfo.name);
						Cmd.state |= ROLEREG_STATE_HAS;
						Zebra::logger->debug("获取数据库句柄出错");
					}
				}
				else
				{
					Cmd.state |= ROLEREG_STATE_HAS;
					Zebra::logger->debug("名字已存在缓存中");
				}
			}
			else if (ptCmd->state & ROLEREG_STATE_WRITE)//回写
			{
				RoleData Tmpinfo;
				Tmpinfo.accid = ptCmd->accid;
				Tmpinfo.zone= ptCmd->gameZone.zone;
				Tmpinfo.game=  ptCmd->gameZone.game;
				strcpy(Tmpinfo.name, ptCmd->name);
				Zebra::logger->debug("##########");
				Zebra::logger->debug("name = %s ", Tmpinfo.name);
				//数据库表的属性
				static const dbCol creat_con_define[]	= {
					{"`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE])},
					{"`GAME`", zDBConnPool::DB_WORD, sizeof(WORD)},
					{"`ZONE`", zDBConnPool::DB_WORD, sizeof(WORD)},
					{"`ACCID`", zDBConnPool::DB_DWORD, sizeof(DWORD)},
					{NULL, 0, 0}
				};
				connHandleID handle = roleRegService::dbConnPool->getHandle(&hash);
				Zebra::logger->debug("handle=%u", handle);
				if ((connHandleID)-1 != handle)
				{
					if ((unsigned int)-1 != roleRegService::dbConnPool->exeInsert(handle, 
								URLManager::getInstance().roleregTableName(hash).c_str(), 
								creat_con_define, (unsigned char *)&Tmpinfo))
					{
						Cmd.state |= ROLEREG_STATE_OK;
						Zebra::logger->debug("回写数据库成功");
					}
					else
					{
						Cmd.state &= ~ROLEREG_STATE_OK;
						Zebra::logger->debug("回写数据库失败");
					}
					roleRegService::dbConnPool->putHandle(handle);
				}
				else
				{
					Cmd.state &= ~ROLEREG_STATE_OK;
					Zebra::logger->debug("获取句柄失败");
				}
				//清除结构中该角色
				SetRoleInfo.remove(ptCmd->name);
			}
			else if (ptCmd->state & ROLEREG_STATE_CLEAN)//清除
			{
				Zebra::logger->debug("name = %s ", ptCmd->name);
				char cWhere[128];
				memset(cWhere, 0, sizeof(cWhere));
				sprintf(cWhere, "NAME = '%s'", ptCmd->name);
				connHandleID handle = roleRegService::dbConnPool->getHandle(&hash);
				Zebra::logger->debug("handle=%u", handle);
				if ((connHandleID) -1 != handle)
				{
					if ((unsigned int)-1 == roleRegService::dbConnPool->exeDelete(handle,
									URLManager::getInstance().roleregTableName(hash).c_str(), cWhere))
					{
						Cmd.state &= ~ROLEREG_STATE_OK;
						Zebra::logger->debug("删除角色%s失败", ptCmd->name);
					}
					else
					{
						Cmd.state |= ROLEREG_STATE_OK;
						Zebra::logger->debug("删除角色%s成功", ptCmd->name);
					}
					roleRegService::dbConnPool->putHandle(handle);
				}
				else
				{
					Cmd.state &= ~ROLEREG_STATE_OK;
					Zebra::logger->debug("获取句柄失败");
				}
			}

			Zebra::logger->debug("Cmd.state =%u", Cmd.state);
			return sendCmd(&Cmd, sizeof(Cmd));
		}
		break;
	}
	return false;
}


