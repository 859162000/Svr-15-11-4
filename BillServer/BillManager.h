/**
 * \file
 * \version  $Id: BillManager.h  $
 * \author  
 * \date 
 * \brief �˺Ź�����¼һ�����������Ѿ���½���˺�
 *
 * 
 */

#ifndef _BillManager_h_
#define _BillManager_h_

#include <iostream>
#include <ext/hash_map>

#include "Zebra.h"
#include "SuperCommand.h"
#include "zTime.h"
#include "UserCommand.h"

#pragma pack(1)
/**
 * \brief �˺���Ϣ
 *
 */
struct BillInfo
{
	DWORD accid;			    /**< �˺ű�� */
	DWORD loginTempID;		    /**< ��½��ʱ��� */

	WORD wdGatewayID;		/**< ��½�����ر�� */

	double gold;		/**< ������� */
	DWORD vip_time;		/**< vip����ʱ�� */

	enum
	{
		WAIT_LOGIN,			/**< �ȴ�ͨ����½��֤�Ŀͻ��˵�½���ط����� */
		CONF_LOGIN			/**< ��½���ط�������֤�Ѿ��ɹ� */
	}
	state;					/**< �Ự״̬ */
	zTime timestamp;		/**< ʱ��� */
	char client_ip[MAX_IP_LENGTH];

	/**
	 * \brief ȱʡ���캯��
	 *
	 */
	BillInfo() : timestamp()
	{
		accid = 0;
		loginTempID = 0;
		wdGatewayID = 0;
		state = WAIT_LOGIN;
		bzero(client_ip,sizeof(client_ip));
	}
	
	/**
	 * \brief �������캯��
	 *
	 */
	BillInfo(const BillInfo &ai)
	{
		accid = ai.accid;
		loginTempID = ai.loginTempID;
		wdGatewayID = ai.wdGatewayID;
		state = ai.state;
		gold = 0;
		vip_time = 0;
		timestamp = ai.timestamp;
		strncpy(client_ip,ai.client_ip,sizeof(client_ip));
	}

	/**
	 * \brief ��ֵ��������
	 *
	 */
	BillInfo & operator= (const BillInfo &ai)
	{
		accid = ai.accid;
		loginTempID = ai.loginTempID;
		wdGatewayID = ai.wdGatewayID;
		state = ai.state;
		timestamp = ai.timestamp;
		strncpy(client_ip,ai.client_ip,sizeof(client_ip));

		return *this;
	}

};
#pragma pack()

/**
 * \brief �˺���Ϣ��������
 *
 * ������һ���������������˺��б�
 *
 */
class BillManager
{

	public:

		/**
		 * \brief �Ự��ʱʱ��
		 * ��λ����
		 */
		static const int session_timeout_value = 10;

		/**
		 * \brief ȱʡ��������
		 *
		 */
		~BillManager() {};

		/**
		 * \brief �������Ψһʵ��
		 *
		 * \return ���Ψһʵ��
		 */
		static BillManager &getInstance()
		{
			if (NULL == instance)
				instance = new BillManager();

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

		bool verify(const t_NewLoginSession &session);
		void update();
		void updateByGatewayID(const WORD wdGatewayID);
		bool login(const DWORD accid, const DWORD loginTempID);
		bool logout(const DWORD accid, const DWORD loginTempID);
		bool updateGold(DWORD acc , double gold);
		bool updateVipTime(DWORD acc , DWORD vip);
		DWORD getVipTime(DWORD acc);
		double getGold(DWORD acc);
		BillInfo get(const DWORD accid);

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static BillManager *instance;

		/**
		 * \brief ȱʡ���캯��
		 *
		 */
		BillManager() {};

		/**
		 * \brief �˺Ź�����������
		 *
		 */
		typedef __gnu_cxx::hash_map<DWORD, BillInfo> BillInfoMap;
		/**
		 * \brief ���������ĵ���������
		 *
		 */
		typedef BillInfoMap::iterator BillInfoMap_iterator;
		/**
		 * \brief ���������ļ�ֵ������
		 *
		 */
		typedef BillInfoMap::value_type BillInfoMap_pair;

		/**
		 * \brief �������ʻ������
		 *
		 */
		zMutex mlock;
		/**
		 * \brief �˺Ź�������
		 *
		 */
		BillInfoMap infoMap;

};

#endif

