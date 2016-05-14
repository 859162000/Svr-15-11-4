/**
 * \file
 * \version  $Id: ScenesServer.h  $
 * \author  
 * \date 
 * \brief zebra项目场景服务器，游戏绝大部分内容都在本实现
 */

#ifndef _ScenesServer_h_
#define _ScenesServer_h_

#include "zSubNetService.h"
#include "Zebra.h"
#include "SceneManager.h"
#include "zMisc.h"

/**
 * \brief 定义场景服务类
 *
 * 场景服务器，游戏绝大部分内容都在本实现<br>
 * 这个类使用了Singleton设计模式，保证了一个进程中只有一个类的实例
 *
 */
class ScenesService : public zSubNetService
{

	public:
		int writeBackTimer;
		bool double_exp;
		bool double_drop;
		BYTE levelDoubleDrop;
		bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		BYTE countryPower[13];

		/**
		 * \brief 虚析构函数
		 *
		 */
		virtual ~ScenesService()
		{
			instance = NULL;

			//关闭线程池
			if (taskPool)
			{
				taskPool->final();
				SAFE_DELETE(taskPool);
			}
		}

		const int getPoolSize() const
		{
			if(taskPool)
			{
				return taskPool->getSize();
			}
			else
			{
				return 0;
			}
		}

		/**
		 * \brief 返回唯一的类实例
		 *
		 * \return 唯一的类实例
		 */
		static ScenesService &getInstance()
		{
			if (NULL == instance)
				instance = new ScenesService();

			return *instance;
		}

		/**
		 * \brief 释放类的唯一实例
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void reloadConfig();
		void checkAndReloadConfig();
		bool isSequeueTerminate() 
		{
			return taskPool == NULL;
		}

		//GM_logger
		static zLogger* gm_logger;
		//物品log
		static zLogger* objlogger;
		//外挂_logger
		static zLogger* wg_logger;

		static Cmd::stChannelChatUserCmd * pStampData;
		static DWORD updateStampData();
	private:

		/**
		 * \brief 类的唯一实例指针
		 *
		 */
		static ScenesService *instance;
		/**
		 * \brief 设置重新读取配置标志
		 *
		 */
		static bool reload;

		zTCPTaskPool *taskPool;				/**< TCP连接池的指针 */

		/**
		 * \brief 构造函数
		 *
		 */
		ScenesService() : zSubNetService("场景服务器", SCENESSERVER)
		{
			writeBackTimer = 0;
			taskPool = NULL;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
};

#endif

