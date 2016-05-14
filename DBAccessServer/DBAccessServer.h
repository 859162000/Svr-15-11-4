/**
 * \file
 * \version  $Id: InfoServer.h  $
 * \author  
 * \date 
 * \brief 信息集中服务器
 */
 
#ifndef _DBAccessServer_h_
#define _DBAccessServer_h_

#include <iostream>
#include <string>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zTCPTaskPool.h"
#include "zNetService.h"
#include "zMisc.h"
#include "InfoCommand.h"
#include "InfoTask.h"
#include "InfoServer.h"
/**
 * \brief 定义一个广播消息的线程
 */
// class CmdThread : public zThread
// {
// 	public:
// 	
// 		void run();
// 
// 		/**
// 		 * \brief 构造函数
// 		 */
// 		CmdThread() : zThread(),tm(),tn() {}
// 		
// 		/**
// 		 * \brief 析造函数
// 		 */
// 		~CmdThread() {}
// 		
// 	private:
// 
// 		zTime tm, tn;
// 		static time_t delta;//广播消息的时间间隔(单位:秒)
// 		
// };

/**
 * \brief 定义一个信息集中服务器类
 */
class DBAccessService : public zNetService
{

	public:

		const int getPoolSize() const
		{
			return taskPool->getSize();
		}

		const int getPoolState() const
		{
			return taskPool->getState();
		}

		/**
		 * \brief 析构函数
		 */
		~DBAccessService()
		{
			//关闭线程池
			if (taskPool)
			{
				taskPool->final();
				SAFE_DELETE(taskPool);
			}
		}

		/**
		 * \brief 获取类的唯一实例
		 *	使用了singleton设计模式,保证了进程中只有一个实例
		 * \return 类的唯一实例
		 */
		static DBAccessService &getInstance()
		{
			if (NULL == instance)
			{
				instance = new DBAccessService();
			}

			return *instance;
		}

		/**
		 * \brief 释放类的唯一实例
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		/**
		 * \brief 重读配置文件函数
		 */
		void reloadConfig();

		/**
		 * \brief 指向数据库连接池实例指针
		 */
		static zDBConnPool *dbConnPool;

		/**
		 * \brief 指向消息广播线程的指针
		 */
		static CmdThread *pCmdThread;
		
	private:

		/**
		 * \brief 类的唯一实例
		 */
		static DBAccessService *instance;

		zTCPTaskPool *taskPool;				/**< TCP连接池的指针 */

		/**
		 * \brief 构造函数
		 */
		DBAccessService() : zNetService("DBAccessService")
		{
			taskPool = NULL;
			bindport = 0;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();

		/**
		 * \brief 区服务器端口
		 */
		unsigned short bindport;
};
#endif

 
