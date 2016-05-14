/**
 * \file
 * \version  $Id: SessionServer.h $
 * \author  
 * \date 
 * \brief ��Ϸȫ��Session������ 
 */

#ifndef _SessionServer_h_
#define _SessionServer_h_

#include "zSubNetService.h"
#include "Zebra.h"
#include "zMisc.h"
#include "SessionManager.h"

class zDBConnPool;
class MetaData;

/**
 * \brief ����ÿ���û��Ự��ͬһ���ҵĽ�ɫ���͵�����
 */
struct OneCountryScene: public execEntry<UserSession>
{
	DWORD country;
	unsigned int cmdLen;
	Cmd::t_NullCmd* sendCmd;

	void init(Cmd::t_NullCmd * rev, unsigned int len, DWORD countryID)
	{
		sendCmd = rev;
		cmdLen = len;

		country = countryID;
	}

	/**
	 * \brief ����ÿ���û��Ự��ͬһ���ҵĽ�ɫ���ͳ�������
	 * \param su �û��Ự
	 * \return true �ɹ� false ʧ��
	 */
	bool exec(UserSession *su)
	{
		if (country == su->country && su->scene)
		{
			if (sendCmd->para == Cmd::Session::PARA_ENTERWAR)
			{
				((Cmd::Session::t_enterWar_SceneSession*)sendCmd)->dwUserID = su->id;
			}
			
			su->scene->sendCmd(sendCmd, cmdLen);
		}
		return true;
	}
};

struct worldMsg
{
	char msg[256];
	DWORD time;
	DWORD interval;
	DWORD count;
	DWORD country;
	DWORD mapID;
	char GM[32];
	worldMsg()
	{
		bzero(msg, sizeof(msg));
		time = 0;
		interval = 0;
		count = 0;
		country = 0;
		mapID = 0;
		bzero(GM, sizeof(GM));
	}
};

/**
 * \brief Session������
 *
 * ��Ϸȫ�ֵ�Session������
 *
 */
class SessionService : public zSubNetService
{

	public:

		bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief ������������
		 *
		 */
		~SessionService()
		{
			instance = NULL;

			//�ر��̳߳�
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
		 * \brief ��ȡ���Ψһʵ��
		 *
		 * �����ʹ����Singleton����֤һ��������ֻ��һ�����ʵ��
		 *
		 * \return ���Ψһʵ��
		 */
		static SessionService &getInstance()
		{
			if (NULL == instance)
				instance = new SessionService();

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void reloadConfig();
		bool isSequeueTerminate() 
		{
			return taskPool == NULL;
		}
		
		/**
		 * \brief ָ�����ݿ����ӳ�ʵ����ָ��
		 *
		 */
		static zDBConnPool *dbConnPool;
		static MetaData* metaData;
		bool checkShutdown();
		//bool checkGumu();

		/**
		 * \brief ������ǿ��
		 *
		 */
		void checkCountry(struct tm &tmValue, bool donow = false);
		Cmd::Session::t_shutdown_SceneSession shutdown_time;
		
		static std::map<BYTE, worldMsg> wMsg;
		//static DWORD snCount;
		static std::map<DWORD,BYTE> userMap;
		DWORD countryLevel[13];
		bool uncheckCountryProcess;

		static bool reportGm(const char * fromName, const char *msg, ...);

		static DWORD emperorForbid[10];//�ʵ۽��Թ������
		DWORD loadEmperorForbid();//�����ݿ��
		void saveEmperorForbid();//д���ݿ�
		void clearEmperorForbid();//ÿ�������¼
	private:

		DWORD gumutime;
		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static SessionService *instance;

		zTCPTaskPool *taskPool;				/**< TCP���ӳص�ָ�� */

		/**
		 * \brief ���캯��
		 *
		 */
		SessionService() : zSubNetService("Session������", SESSIONSERVER)
		{
			taskPool = NULL;
			bzero(&shutdown_time,sizeof(shutdown_time));
			gumutime=0; 
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
};

#endif

