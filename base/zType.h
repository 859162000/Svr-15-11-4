/**
 * \file
 * \version  $Id: zType.h  $
 * \author  
 * \date 
 * \brief �����������
 *
 * 
 */

#ifndef _zType_h_
#define _zType_h_

#include <time.h>
#include <strings.h>
#include "EncDec/EncDec.h"

#define SAFE_DELETE(x) { if (x) { delete (x); (x) = NULL; } }
#define SAFE_DELETE_VEC(x) { if (x) { delete [] (x); (x) = NULL; } }

/**
 * \brief ���ֽ��޷�������
 *
 */
typedef unsigned char BYTE;

/**
 * \brief ˫�ֽ��޷�������
 *
 */
typedef unsigned short WORD;

/**
 * \brief ˫�ֽڷ�������
 *
 */
typedef signed short SWORD;

/**
 * \brief ���ֽ��޷�������
 *
 */
typedef unsigned int DWORD;

/**
 * \brief ���ֽڷ�������
 *
 */
typedef signed int SDWORD;

/**
 * \brief ���ֽ��޷�������
 *
 */
typedef unsigned long long QWORD;

/**
 * \brief ���ֽڷ�������
 *
 */
typedef signed long long SQWORD;

/**
 * \brief ���ֵ���󳤶�
 */
#define MAX_NAMESIZE 32

/**
 * \brief �˺���󳤶�
 */
#define MAX_ACCNAMESIZE	48

/**
 * \brief IP��ַ��󳤶�
 *
 */
#define MAX_IP_LENGTH	16

/**
 * \brief ������������û���Ŀ
 *
 */
#define MAX_GATEWAYUSER 4000	

/**
 * \brief ������󳤶�
 *
 */
#define MAX_PASSWORD  16

/**
 * \brief ����
 */
#define SCREEN_WIDTH 13

/**
 * \brief ����
 */
#define SCREEN_HEIGHT 19

/**
 * \brief �����̳߳ص�״̬���λ
 *
 */
enum {
	state_none		=	0,							/**< �յ�״̬ */
	state_maintain	=	1 << 0,						/**< ά���У���ʱ���������µ����� */
};

/**
 * \brief ��������
 */
#ifndef MAX_NUMPASSWORD
#define MAX_NUMPASSWORD	32
#endif

#pragma pack(1)
/**
 * \brief ������Ϸ��
 * ����Ϸ���з��࣬Ȼ����ͬ����Ϸ���ٷ���
 */
struct GameZone_t
{
	union
	{
		/**
		 * \brief Ψһ���
		 */
		DWORD id;
		struct
		{
			/**
			 * \brief ��Ϸ�������
			 */
			WORD zone;
			/**
			 * \brief ��Ϸ������
			 */
			WORD game;
		};
	};

	GameZone_t()
	{
		this->game = 0;
		this->zone = 0;
	}
	GameZone_t(const GameZone_t &gameZone)
	{
		this->id = gameZone.id;
	}
	GameZone_t & operator= (const GameZone_t &gameZone)
	{
		this->id = gameZone.id;
		return *this;
	}
	bool operator== (const GameZone_t &gameZone) const
	{
		return this->id == gameZone.id;
	}
};

struct t_NewLoginSession
{
	WORD wdLoginID;
	WORD wdGatewayID;
	DWORD loginTempID;
	char pstrIP[MAX_IP_LENGTH];
	WORD wdPort;
	GameZone_t gameZone;
	union{
		DWORD accid;
		DWORD uid;
	};
	BYTE state;
	//BYTE type;
	unsigned int type;
	union{
		char name[48];
		char account[48];
	};
	char passwd[MAX_PASSWORD];
	DES_cblock des_key;//des��Կ
	char client_ip[MAX_IP_LENGTH];
	char numpasswd[MAX_NUMPASSWORD];

	t_NewLoginSession()
	{

		//wdLoginID = 0;
		//wdGatewayID = 0;
		//loginTempID = 0;
		//bzero(pstrIP, sizeof(pstrIP));
		//wdPort = 0;
		//accid = 0;
		//state = 0;
		//type = 0;
		//bzero(name, sizeof(name));
		//bzero(passwd, sizeof(passwd));
		bzero(numpasswd, sizeof(numpasswd));

	}
	t_NewLoginSession(const t_NewLoginSession& session)
	{
		wdLoginID = session.wdLoginID;
		wdGatewayID = session.wdGatewayID;
		loginTempID = session.loginTempID;
		bcopy(session.pstrIP, pstrIP, sizeof(pstrIP));
		wdPort = session.wdPort;
		gameZone = session.gameZone;
		accid = session.accid;
		state = session.state;
		type = session.type;
		bcopy(session.name, name, sizeof(name));
		bcopy(session.passwd, passwd, sizeof(passwd));
		bcopy(session.des_key, des_key, sizeof(des_key));
		bcopy(session.client_ip, client_ip, sizeof(client_ip));
		bcopy(session.numpasswd, numpasswd, sizeof(numpasswd));
	}
	t_NewLoginSession & operator= (const t_NewLoginSession &session)
	{
		wdLoginID = session.wdLoginID;
		wdGatewayID = session.wdGatewayID;
		loginTempID = session.loginTempID;
		bcopy(session.pstrIP, pstrIP, sizeof(pstrIP));
		wdPort = session.wdPort;
		gameZone = session.gameZone;
		accid = session.accid;
		state = session.state;
		type = session.type;
		bcopy(session.name, name, sizeof(name));
		bcopy(session.passwd, passwd, sizeof(passwd));
		bcopy(session.des_key, des_key, sizeof(des_key));
		bcopy(session.client_ip, client_ip, sizeof(client_ip));
		bcopy(session.numpasswd, numpasswd, sizeof(numpasswd));
		return *this;
	}
};

#pragma pack()
#endif

