/**
 * \file
 * \version  $Id: GYListManager.h  $
 * \author  
 * \date 
 * \brief ������Ϣ�б�
 *
 * ��½��������Ҫ�������µ��������ص���Ϣ�б����ڷ�������
 * 
 */


#ifndef _GYListManager_h_
#define _GYListManager_h_

#include <ext/hash_map>

#include "zType.h"
#include "zMisc.h"
#include "zMutex.h"
#include "Command.h"
#include "FLCommand.h"

/**
 * \brief ������Ϣ�ڵ�
 *
 */
struct GYList
{
	WORD wdServerID;			/**< ��������� */
	BYTE pstrIP[MAX_IP_LENGTH];	/**< ��������ַ */
	WORD wdPort;				/**< �������˿� */
	WORD wdNumOnline;			/**< ������������ */
	int  state;					/**< ������״̬ */
	DWORD zoneGameVersion;

	/**
	 * \brief ȱʡ���캯��
	 *
	 */
	GYList()
	{
		wdServerID = 0;
		bzero(pstrIP, sizeof(pstrIP));
		wdPort = 0;
		wdNumOnline = 0;
		state = state_none;
		zoneGameVersion = 0;
	}

	/**
	 * \brief �������캯��
	 *
	 */
	GYList(const GYList& gy)
	{
		wdServerID = gy.wdServerID;
		bcopy(gy.pstrIP, pstrIP, sizeof(pstrIP));
		wdPort = gy.wdPort;
		wdNumOnline = gy.wdNumOnline;
		state = gy.state;
		zoneGameVersion = gy.zoneGameVersion;
	}

	/**
	 * \brief ��ֵ����
	 *
	 */
	GYList & operator= (const GYList &gy)
	{
		wdServerID = gy.wdServerID;
		bcopy(gy.pstrIP, pstrIP, sizeof(pstrIP));
		wdPort = gy.wdPort;
		wdNumOnline = gy.wdNumOnline;
		state = gy.state;
		zoneGameVersion = gy.zoneGameVersion;
		return *this;
	}

};

/**
 * \brief ������Ϣ�б������
 *
 */
class GYListManager
{

	public:

		/**
		 * \brief Ĭ����������
		 *
		 */
		~GYListManager()
		{
			gyData.clear();
		}

		/**
		 * \brief �������Ψһʵ��
		 *
		 * ʵ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
		 *
		 */
		static GYListManager &getInstance()
		{
			if (NULL == instance)
				instance = new GYListManager;

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

		bool put(const GameZone_t &gameZone, const GYList &gy);
		void disableAll(const GameZone_t &gameZone);
		GYList *getAvl(const GameZone_t &gameZone);
		void full_ping_list(Cmd::stPingList* cmd, const GameZone_t& gameZone);
		bool verifyVer(const GameZone_t &gameZone, DWORD verify_client_version, BYTE &retcode);

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static GYListManager *instance;

		/**
		 * \brief Ĭ�Ϲ��캯��
		 *
		 */
		GYListManager() {};

		/**
		 * \brief hash����
		 *
		 */
		struct GameZone_hash
		{
			size_t operator()(const GameZone_t &gameZone) const
			{
				__gnu_cxx::hash<DWORD> H;
				return H(gameZone.id);
			}
		};
		/**
		 * \brief ������������
		 *
		 */
		typedef __gnu_cxx::hash_multimap<const GameZone_t, GYList, GameZone_hash> GYListContainer;
		/**
		 * \brief ������������������
		 *
		 */
		typedef GYListContainer::iterator GYListContainer_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef GYListContainer::value_type GYListContainer_value_type;
		/**
		 * \brief �洢�����б���Ϣ������
		 *
		 */
		GYListContainer gyData;
		/**
		 * \brief �������
		 *
		 */
		zMutex mlock;

};

#endif

