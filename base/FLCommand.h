/**
 * \file
 * \version  $Id: FLCommand.h  $
 * \author  
 * \date 
 * \brief ����ͳһ�û�ƽ̨��½������ָ��
 *
 * 
 */

#ifndef _FLCommand_h_
#define _FLCommand_h_

#include "zType.h"
#include "zNullCmd.h"

#pragma pack(1)

namespace Cmd
{
	namespace FL
	{
		const BYTE CMD_LOGIN = 1;
		const BYTE CMD_GYLIST = 2;
		const BYTE CMD_SESSION = 3;


		//////////////////////////////////////////////////////////////
		/// ��½FL������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginFL : t_NullCmd
		{
			char strIP[MAX_IP_LENGTH];
			unsigned short port;

			t_LoginFL()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN) {};
		};

		const BYTE PARA_LOGIN_OK = 2;
		struct t_LoginFL_OK : t_NullCmd
		{
			GameZone_t gameZone;
			char name[MAX_NAMESIZE];
			BYTE netType;

			t_LoginFL_OK()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN_OK) {};
		};
		//////////////////////////////////////////////////////////////
		/// ��½FL������ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// ����������Ϣ���ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_FL_GYLIST = 1;
		struct t_GYList_FL : t_NullCmd
		{
			WORD wdServerID;			/**< ��������� */
			BYTE pstrIP[MAX_IP_LENGTH];	/**< ��������ַ */
			WORD wdPort;				/**< �������˿� */
			WORD wdNumOnline;			/**< ������������ */
			int  state;					/**< ������״̬ */
			DWORD zoneGameVersion;
			t_GYList_FL()
				: t_NullCmd(CMD_GYLIST, PARA_FL_GYLIST) {};
		};

		const BYTE PARA_FL_RQGYLIST = 2;
		struct t_RQGYList_FL : t_NullCmd
		{
			t_RQGYList_FL()
				: t_NullCmd(CMD_GYLIST, PARA_FL_RQGYLIST) {};
		};
		//////////////////////////////////////////////////////////////
		/// ����������Ϣ���ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// �������½session���ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_SESSION_NEWSESSION = 1;
		struct t_NewSession_Session : t_NullCmd
		{
			t_NewLoginSession session;

			t_NewSession_Session()
				: t_NullCmd(CMD_SESSION, PARA_SESSION_NEWSESSION) {};
		};

		const BYTE PARA_SESSION_IDINUSE = 2;
		struct t_idinuse_Session : t_NullCmd
		{
			DWORD accid;
			DWORD loginTempID;
			WORD wdLoginID;
			char name[48];

			t_idinuse_Session()
				: t_NullCmd(CMD_SESSION, PARA_SESSION_IDINUSE) { bzero(name, sizeof(name)); };
		};
		//////////////////////////////////////////////////////////////
		/// �������½session���ָ��
		//////////////////////////////////////////////////////////////
	};
};

#pragma pack()

#endif

