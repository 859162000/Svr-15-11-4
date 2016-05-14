/**
 * \file
 * \version  $Id: UserCommand.h  $
 * \author  
 * \date 2
 * \brief �û���Ϣ����������
 *
 * 
 */

#ifndef __UserCommand_H__
#define __UserCommand_H__

#include "zType.h"
#include "zNullCmd.h"

#pragma pack(1)

namespace Cmd
{
	namespace UserServer
	{
		const int SEQ_MAX_LENGTH = 32;		//�������кų���
		const int ID_MAX_LENGTH = 64;		//�ʺų���
		const int REMARK_LENGTH = 200;		//��ע����
		const int CARDID_LENGTH = 32;
		const int PCARD_NO_LENGTH = 20;		//���߿��ų���

		const BYTE CMD_USER_LOGON = 1; 		//��½����������
		const BYTE CMD_USER_CONSUME = 2; 	//��ҿ۷�����
		const BYTE CMD_USER_FILLIN = 3; 	//��ҳ�ֵ����
		const BYTE CMD_USER_QBALANCE = 4; 	//�ʺ�����ѯ
		const BYTE CMD_USER_MCARD = 5; 		//��ҳ�ֵ�¿�
		const BYTE CMD_USER_PCARD = 6; 		//��ҵ��߿�
		const BYTE CMD_USER_SCARD = 7;		//ר����
		struct t_logon : public Cmd::t_NullCmd
		{
			t_logon() 
				: t_NullCmd(CMD_USER_LOGON) { }
		};
		struct t_logon_OK : t_NullCmd//������ͬ������
		{
			t_logon_OK()
				: t_NullCmd(CMD_USER_LOGON) {};
			GameZone_t 	gameZone;			//��Ϸ��
			char 		name[MAX_NAMESIZE];	//������
			BYTE 		netType;			//��������
		};

		struct t_cmd_consume : public Cmd::t_NullCmd
		{
			t_cmd_consume() 
				: t_NullCmd(CMD_USER_CONSUME) 
			{ 
				bzero(tid, SEQ_MAX_LENGTH +1);
				bzero(remark, REMARK_LENGTH +1);
			}
			unsigned int 	uid;									//UID
			int				point;										//�۷ѵ���	
			int 			source;										//��Դ
			char			tid[SEQ_MAX_LENGTH +1];		//�������к�
			char			remark[REMARK_LENGTH +1];	//��ע
			char 			ip[MAX_IP_LENGTH +1];     //�ͻ�����ip
		};

		struct t_cmd_fillin : public Cmd::t_NullCmd
		{
			t_cmd_fillin() 
				: t_NullCmd(CMD_USER_FILLIN) 
			{ 
				bzero(account, ID_MAX_LENGTH +1);
				bzero(tid, SEQ_MAX_LENGTH +1);
				bzero(cardid, CARDID_LENGTH +1);
				bzero(remark, REMARK_LENGTH +1);
			}
			unsigned int uid;											//uid
			int				point;											//��ֵ����	
			int 			source;											//��Դ
			char			account[ID_MAX_LENGTH +1];	//����ʺ�
			char			tid[SEQ_MAX_LENGTH +1];			//�������к�
			char			cardid[CARDID_LENGTH +1];		//��ֵ����
			char			remark[REMARK_LENGTH +1];		//��ע
		};

		struct t_cmd_qbalance : public Cmd::t_NullCmd
		{
			t_cmd_qbalance() 
				: t_NullCmd(CMD_USER_QBALANCE) { }
			unsigned int	uid;						//UID
			char			account[ID_MAX_LENGTH +1];	//����ʺ�
			char			tid[SEQ_MAX_LENGTH +1];		//�������к�
		};

		struct t_cmd_mcard : public Cmd::t_NullCmd
		{
			t_cmd_mcard() 
				: t_NullCmd(CMD_USER_MCARD) 
			{ 
				bzero(account, ID_MAX_LENGTH +1);
				bzero(tid, SEQ_MAX_LENGTH +1);
				bzero(remark, REMARK_LENGTH +1);
			}
			unsigned int 	uid;										//UID
			int				point;											//�۷ѵ���	
			int 			source;											//��Դ
			char			account[ID_MAX_LENGTH +1];	//����ʺ�
			char			tid[SEQ_MAX_LENGTH +1];			//�������к�
			char			remark[REMARK_LENGTH +1];		//��ע
			char 			ip[MAX_IP_LENGTH +1];     	//�ͻ�����ip
		};

		struct t_cmd_pcard : public Cmd::t_NullCmd
		{
			t_cmd_pcard() 
				: t_NullCmd(CMD_USER_PCARD) 
			{ 
				bzero(tid, SEQ_MAX_LENGTH + 1);
				bzero(pcardid, sizeof(pcardid));
			}
			unsigned int 	uid;									//UID
			int 			source;										//��Դ
			char			tid[SEQ_MAX_LENGTH +1];		//�������к�
			char			pcardid[CARDID_LENGTH +1];//��ֵ����
			char 			ip[MAX_IP_LENGTH +1];     //�ͻ�����ip
		};

		enum	//billclient��userserverͨ�Ų����뷵��ֵ
		{
			RET_OK = 0,										//�ɹ�
			RET_FAIL = -1,								//ʧ��
			RET_ID_NOT_EXIST = -2,				//�û�������
			RET_BALANCE_NOT_ENOUGH = -3,	//����
			RET_PCARD_NOT_EXIST = -4,			//���߿�������
			RET_NOTUSE_GAMEZONE = -5,			//�����߿������ڸ���ʹ��
			RET_PCARD_NOT_REUSE = -6,			//���߿������ظ�ʹ��
			RET_SCARD_ERR = -7,						//ר�������Ŵ���
			RET_SCARD_PASSWD_ERR					//ר�����������
		};
		
		//billclient��������
		enum
		{
			AT_FILLIN = 0,			//��ֵ
			AT_CONSUME = 1,			//�۷�
			AT_SCARD = 2,				//ר����
			AT_QBALANCE = 3,		//��ѯ
			AT_MCARD = 4,				//�¿���ֵ
			AT_PCARD = 5,				//���߿�
		};

		//�ظ�billclient������
		enum
		{
			SUBAT_INVALID = -1,	//billclient����������
			SUBAT_GOLD = 4			//ר������ֵ����������//���߿��������Ͳ����ڸ�ֵ�ظ�
		};

		enum
		{
			KEEP = 0,						//����
			ESALES_FILLIN = 1,	//�����̳ǳ俨
			ESALES_CONSUME = -1	//�����̳�����
		};

		const BYTE CMD_USER_RET = 5; //�������˵ķ�������
		struct t_cmd_ret : public Cmd::t_NullCmd
		{
			t_cmd_ret() 
				: t_NullCmd(CMD_USER_RET) 
			{
				balance = 0;
				bonus = 0;
				hadfilled = 0;
				ret = RET_FAIL;
				subat = SUBAT_INVALID;
			}
			char		tid[SEQ_MAX_LENGTH + 1];	//�������к�
			int 		balance;				//���
			int 		bonus;					//����
			int 		hadfilled;			//������ֵ�ı�־,1=������ֵ,0=û��
			int 		ret;						//����ش���
			int			subat;					//������
		};
	};
};

#pragma pack()

#endif

