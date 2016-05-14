/**
 * \file
 * \version  $Id: roleRegServer.h  $
 * \author  
 * \date
 * \brief 角色唯一性验证服务器
 */

#ifndef _roleRegService_h_
#define _roleRegService_h_

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
#include "RoleTask.h"

/**
 *	\brief 客户端访问服务器类
 */
class roleRegService : public zNetService
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
		~roleRegService()
		{
			instance = NULL;

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
		static roleRegService &getInstance()
		{
			if (instance == NULL)
				instance = new roleRegService();
				
			return *instance;	
		}
		
		/**
		 * \brief 释放类的唯一实例
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void reloadConfig();
		
		/**
		 * \brief 指向数据库连接池的实例指针
		 */
		 static zDBConnPool *dbConnPool;
		
	private:
		
		/**
		 * \brief 类的唯一实例
		 */
		static roleRegService *instance;

		zTCPTaskPool *taskPool;				/**< TCP连接池的指针 */

		/**
		 * \brief 构造函数
		 */
		roleRegService() : zNetService("roleRegService")
		{
			taskPool = NULL;
			client_port = 0 ;
		}
		
		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
		
		unsigned short client_port ;
};

#endif
