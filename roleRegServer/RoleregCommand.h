/**
 * \file
 * \version  $Id: RoleregCommand.h  $
 * \author  
 * \date 
 * \brief �����ɫ����ָ��
 *
 */
 
 #ifndef _RoleregCommand_h_
 #define _RoleregCommand_h_
 
 #include "zType.h"
 #include "zNullCmd.h"
 
 #pragma pack(1)
 
 namespace Cmd
 {
	namespace	RoleReg
	{
		const BYTE CMD_LOGIN = 1; 
		const BYTE CMD_ROLEREG = 2;

		//////////////////////////////////////////////////////////////
		/// ��½������ָ��
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginRoleReg : t_NullCmd   //�������������
		{
			t_LoginRoleReg()
				:t_NullCmd(CMD_LOGIN, PARA_LOGIN) { }
		};
		//////////////////////////////////////////////////////////////
		/// ��½������ָ��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// ��ɫ����Ψһ����֤���ָ��
		//////////////////////////////////////////////////////////////
		const WORD ROLEREG_STATE_TEST		= 1;	//����
		const WORD ROLEREG_STATE_WRITE		= 2;	//��д
		const WORD ROLEREG_STATE_CLEAN		= 4;	//���
		const WORD ROLEREG_STATE_HAS		= 8;	//������
		const WORD ROLEREG_STATE_OK			= 16;	//������д�ɹ�
		const BYTE PARA_CHARNAME_ROLEREG = 1;
		struct t_Charname_Rolereg : t_NullCmd
		{
			WORD wdServerID;			/**< ��������� */
			DWORD accid;				/**< �˺ű�� */
			GameZone_t gameZone;		/**< ��Ϸ����� */
			char name[MAX_NAMESIZE];	/**< ��ɫ���� */
			WORD state;					/**< �������״̬��λ��� */

			t_Charname_Rolereg()
				:t_NullCmd(CMD_ROLEREG, PARA_CHARNAME_ROLEREG) { }
		};
		//////////////////////////////////////////////////////////////
		/// ��ɫ����Ψһ����֤���ָ��
		//////////////////////////////////////////////////////////////
	};
};

#pragma pack()

#endif
