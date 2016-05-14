/**
 * \file
 * \version  $Id: SuperCommand.h  $
 * \author  
 * \date 
 * \brief �����������������ָ��
 *
 */


#ifndef _SuperCommand_h_
#define _SuperCommand_h_

#include "zType.h"
#include "zNullCmd.h"

#pragma pack(1)

namespace Cmd
{

	namespace Super
	{
		const BYTE CMD_STARTUP	= 1;
		const BYTE CMD_BILL		= 3;
		const BYTE CMD_GATEWAY	= 4;
		//const BYTE CMD_GMTOOL	= 5;
		const BYTE CMD_SESSION	= 5;
		const BYTE CMD_COUNTRYONLINE	= 166;


		//////////////////////////////////////////////////////////////
		// �����������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_STARTUP_REQUEST = 1;
		struct t_Startup_Request : t_NullCmd
		{
			WORD wdServerType;
			char pstrIP[MAX_IP_LENGTH];
			t_Startup_Request()
				: t_NullCmd(CMD_STARTUP, PARA_STARTUP_REQUEST) {};
		};

		const BYTE PARA_STARTUP_RESPONSE = 2;
		struct t_Startup_Response : t_NullCmd
		{
			WORD wdServerID;
			WORD wdPort;
			char pstrExtIP[MAX_IP_LENGTH];
			WORD wdExtPort;
			t_Startup_Response()
				: t_NullCmd(CMD_STARTUP, PARA_STARTUP_RESPONSE) {};
		};

		struct ServerEntry
		{
			WORD wdServerID;
			WORD wdServerType;
			char pstrName[MAX_NAMESIZE];
			char pstrIP[MAX_IP_LENGTH];
			WORD wdPort;
			char pstrExtIP[MAX_IP_LENGTH];
			WORD wdExtPort;
			WORD state;
			ServerEntry()
			{
				wdServerID = 0;
				wdServerType = 0;
				bzero(pstrName, sizeof(pstrName));
				bzero(pstrIP, sizeof(pstrIP));
				wdPort = 0;
				bzero(pstrExtIP, sizeof(pstrExtIP));
				wdExtPort = 0;
				state = 0;
			}
			ServerEntry(const ServerEntry& se)
			{
				wdServerID = se.wdServerID;
				wdServerType = se.wdServerType;
				bcopy(se.pstrName, pstrName, sizeof(pstrName));
				bcopy(se.pstrIP, pstrIP, sizeof(pstrIP));
				wdPort = se.wdPort;
				bcopy(se.pstrExtIP, pstrExtIP, sizeof(pstrExtIP));
				wdExtPort = se.wdExtPort;
				state = se.state;
			}
			ServerEntry & operator= (const ServerEntry &se)
			{
				wdServerID = se.wdServerID;
				wdServerType = se.wdServerType;
				bcopy(se.pstrName, pstrName, sizeof(pstrName));
				bcopy(se.pstrIP, pstrIP, sizeof(pstrIP));
				wdPort = se.wdPort;
				bcopy(se.pstrExtIP, pstrExtIP, sizeof(pstrExtIP));
				wdExtPort = se.wdExtPort;
				state = se.state;
				return *this;
			}
		};
		const BYTE PARA_STARTUP_SERVERENTRY_NOTIFYME = 3;
		struct t_Startup_ServerEntry_NotifyMe : t_NullCmd
		{
			WORD size;
			ServerEntry entry[0];
			t_Startup_ServerEntry_NotifyMe()
				: t_NullCmd(CMD_STARTUP, PARA_STARTUP_SERVERENTRY_NOTIFYME), size(0) {};
		};
		const BYTE PARA_STARTUP_SERVERENTRY_NOTIFYOTHER = 4;
		struct t_Startup_ServerEntry_NotifyOther : t_NullCmd
		{
			WORD srcID;
			ServerEntry entry;
			t_Startup_ServerEntry_NotifyOther()
				: t_NullCmd(CMD_STARTUP, PARA_STARTUP_SERVERENTRY_NOTIFYOTHER) {};
		};

		const BYTE PARA_STARTUP_OK = 5;
		struct t_Startup_OK : t_NullCmd
		{
			WORD wdServerID;
			t_Startup_OK()
				: t_NullCmd(CMD_STARTUP, PARA_STARTUP_OK) {};
		};

		const BYTE PARA_GAMETIME = 6;
		struct t_GameTime : t_NullCmd
		{
			QWORD qwGameTime;
			t_GameTime()
				: t_NullCmd(CMD_STARTUP, PARA_GAMETIME) {};
		};

		const BYTE PARA_RESTART_SERVERENTRY_NOTIFYOTHER = 9;
		struct t_restart_ServerEntry_NotifyOther : t_NullCmd
		{
			WORD srcID;
			WORD dstID;
			t_restart_ServerEntry_NotifyOther()
				: t_NullCmd(CMD_STARTUP, PARA_RESTART_SERVERENTRY_NOTIFYOTHER) {};
		};
		//////////////////////////////////////////////////////////////
		// �����������ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		// �����������������Ʒѷ�����������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_BILL_NEWSESSION = 1;
		struct t_NewSession_Bill : t_NullCmd
		{
			t_NewLoginSession session;

			t_NewSession_Bill()
				: t_NullCmd(CMD_BILL, PARA_BILL_NEWSESSION) {};
		};

		const BYTE PARA_BILL_IDINUSE = 2;
		struct t_idinuse_Bill : t_NullCmd
		{
			DWORD accid;
			DWORD loginTempID;
			WORD wdLoginID;
			char name[48];

			t_idinuse_Bill()
				: t_NullCmd(CMD_BILL, PARA_BILL_IDINUSE) { bzero(name, sizeof(name)); };
		};
		//////////////////////////////////////////////////////////////
		// �����������������Ʒѷ�����������ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		// ��������������������ط�����������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_GATEWAY_GYLIST = 1;
		struct t_GYList_Gateway : t_NullCmd
		{
			WORD wdServerID;			/**< ��������� */
			BYTE pstrIP[MAX_IP_LENGTH];	/**< ��������ַ */
			WORD wdPort;				/**< �������˿� */
			WORD wdNumOnline;			/**< ������������ */
			int  state;					/**< ������״̬ */
			DWORD zoneGameVersion;
			t_GYList_Gateway()
				: t_NullCmd(CMD_GATEWAY, PARA_GATEWAY_GYLIST) {};
		};

		const BYTE PARA_GATEWAY_RQGYLIST = 2;
		struct t_RQGYList_Gateway : t_NullCmd
		{
			t_RQGYList_Gateway()
				: t_NullCmd(CMD_GATEWAY, PARA_GATEWAY_RQGYLIST) {};
		};

		const BYTE PARA_GATEWAY_NEWSESSION = 3;
		struct t_NewSession_Gateway : t_NullCmd
		{
			t_NewLoginSession session;

			t_NewSession_Gateway()
				: t_NullCmd(CMD_GATEWAY, PARA_GATEWAY_NEWSESSION) {};
		};

		const BYTE PARA_CHARNAME_GATEWAY = 4;
		struct t_Charname_Gateway : t_NullCmd
		{
			WORD wdServerID;			/**< ��������� */
			DWORD accid;				/**< �˺ű�� */
			char name[MAX_NAMESIZE];	/**< ��ɫ���� */
			WORD state;					/**< �������״̬��λ��� */

			t_Charname_Gateway()
				:t_NullCmd(CMD_GATEWAY, PARA_CHARNAME_GATEWAY) { }
		};
		//////////////////////////////////////////////////////////////
		// ��������������������ط�����������ָ��
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////
		///���������������ָ��
		//////////////////////////////////////
		//�����������������Ϣ
		const BYTE PARA_REQUEST_COUNTRYONLINE = 0;
		struct t_Request_CountryOnline : t_NullCmd
		{
			QWORD 		rTimestamp;							//����ʱ���
			DWORD		infoTempID;
			t_Request_CountryOnline()
				: t_NullCmd(CMD_COUNTRYONLINE, PARA_REQUEST_COUNTRYONLINE) {};
		};
		//��������������Ϣ
		const BYTE PARA_COUNTRYONLINE = 1;
		struct t_CountryOnline : t_NullCmd
		{
			QWORD 		rTimestamp;							//����ʱ���
			DWORD		infoTempID;
			DWORD		OnlineNum;
			struct Online
			{
				DWORD country;
				DWORD num;
			}
			CountryOnline[0];

			t_CountryOnline() : t_NullCmd(CMD_COUNTRYONLINE, PARA_COUNTRYONLINE)
			{
				OnlineNum = 0;
			}
		};
		//////////////////////////////////////
		///���������������ָ��
		//////////////////////////////////////
		//�����������������Ϣ
		
		const BYTE PARA_SHUTDOWN =1;
		struct t_shutdown_Super : t_NullCmd
		{
			t_shutdown_Super(): t_NullCmd(CMD_SESSION, PARA_SHUTDOWN)
			{
			}
		};
	};

};

#pragma pack()

#endif

