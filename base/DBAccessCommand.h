/**
 * \file
 * \version  $Id: DBAccessCommand.h 856 2005-04-04 13:55:19Z song $
 * \author  Songsiliang,songsiliang@netease.com
 * \date 2004��12��11�� 14ʱ36��43�� CST
 * \brief �������ݿ�����м����ָ��
 *
 */


#ifndef _DBAccessCommand_h_
#define _DBAccessCommand_h_

#include "zType.h"
#include "zNullCmd.h"

#pragma pack(1)

namespace Cmd
{

	namespace DBAccess
	{
		const BYTE CMD_LOGON = 1;
		const BYTE CMD_LOGINSERVER = 2;


		//////////////////////////////////////////////////////////////
		// �����½���ݿ�����м��ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGON = 1;
		struct t_LogonDBAccess : t_NullCmd
		{
			t_LogonDBAccess()
				: t_NullCmd(CMD_LOGON, PARA_LOGON) {};
		};
		//////////////////////////////////////////////////////////////
		// �����½���ݿ�����м��ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		// �������ݿ�����м�����½������֮���ָ��
		//////////////////////////////////////////////////////////////
		enum
		{
			SESSIONCHECK_SUCCESS = 0,
			SESSIONCHECK_DB_FAILURE = 1,
			SESSIONCHECK_PWD_FAILURE = 2
		};
		const BYTE PARA_LOGINSERVER_SESSIONCHECK = 1;
		struct t_LoginServer_SessionCheck : t_NullCmd
		{
			t_LoginServer_SessionCheck()
				: t_NullCmd(CMD_LOGINSERVER, PARA_LOGINSERVER_SESSIONCHECK)
				{
					retcode = SESSIONCHECK_DB_FAILURE;
				}

			BYTE retcode;
			t_NewLoginSession session;
		};
		//////////////////////////////////////////////////////////////
		// �������ݿ�����м�����½������֮���ָ��
		//////////////////////////////////////////////////////////////
	};

};

#pragma pack()

#endif

