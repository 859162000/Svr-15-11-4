/**
 * \file
 * \version  $Id: InfoTask.h  $
 * \author 
 * \date 
 * \brief 定义数据库访问服务器的任务
 */

#ifndef _DBAccessTask_h_
#define _DBAccessTask_h_

#include <iostream>
#include <string>
#include <vector>
#include <ext/numeric>

#include "zService.h"
#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zMisc.h"
#include "InfoCommand.h"

class InfoContainer;
class InfoTask : public zTCPTask
{
	public:

		int verifyConn();
		int waitSync();
		void addToContainer();
		void removeFromContainer();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief 构造函数
		 * \param pool 所属连接池
		 * \param sock TCP/IP套接口
		 * \param addr 地址
		 * \param pCon INFO容器
		 */
		InfoTask(zTCPTaskPool *pool, const int sock, 
				const struct sockaddr_in *addr,
				InfoContainer * pCon) : zTCPTask(pool, sock, addr) 
		{
			pContainer = pCon;
		}

	 	/**
	 	 * \brief 析构函数
	 	 */
		~InfoTask() { }

	private:
	
		bool msgParse_loginServer(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		/**
		 * \brief 指向全局向量的指针
		 */
		InfoContainer  * pContainer;
};

class InfoContainer : private zNoncopyable
{
	public:
		
		/**
		 * \brief 构造函数
		 */
		InfoContainer() { }
		
		/**
		 * \brief 析构函数
		 */
		~InfoContainer(){ }
		
		/**
		 * \brief 从向量中添加一个元素
		 * \param iTask
		 */
		void add(InfoTask *iTask)
		{
			mlock.lock();
			vecInfo.push_back(iTask);
			mlock.unlock();
		}
		
		/**
		 * \brief 从向量中删除一个元素
		 * \param iTask
		 */
		void remove(InfoTask *iTask)
		{
			mlock.lock();
			std::vector<InfoTask *>::iterator Iter = find(vecInfo.begin(), vecInfo.end(), iTask);
			vecInfo.erase(Iter);
			mlock.unlock();
		}

		/**
		 * \brief 向所有连接的服务器广播指令
		 * \param pCmd 需要广播的指令
		 * \param nCmdLen 指令的长度
		 */
		void broadcast(Cmd::t_NullCmd *pCmd, unsigned int nCmdLen)
		{
			mlock.lock();
			for (std::vector<InfoTask *>::iterator Iter=vecInfo.begin(); Iter!=vecInfo.end(); ++Iter)
			{
				(*Iter)->sendCmd(pCmd, nCmdLen);	
			}
			mlock.unlock();
		}
		
	private:
		
		/**
		 * \brief InfoTask指针向量
		 */
		std::vector <InfoTask *>	vecInfo;
		/**
		 * \brief 容器访问互斥变量
		 */
		zMutex mlock;
};

#endif



