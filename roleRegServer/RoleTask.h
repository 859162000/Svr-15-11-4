/**
 * \file
 * \version  $Id: RoleTask.h  $
 * \author 
 * \date 
 * \brief �������ݿ���ʷ�����������
 */

#ifndef _RoleTask_h_
#define _RoleTask_h_

#include <iostream>
#include <string>
#include <vector>
#include <ext/numeric>

#include "zService.h"
#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zMisc.h"

/**
* \brief ��Ž�ɫ��Ϣ�Ľṹ��
*/
struct RoleData
{  
	char name[MAX_NAMESIZE];//��ɫ��
	unsigned short game; //��Ϸ���
	unsigned short zone; //��Ϸ�����
	unsigned int accid;	//�ʺű��
	
	RoleData(){accid=0;zone=0;game=0;memset(name, 0, sizeof(name));}
	RoleData(const RoleData &rd)
	{
		accid    = rd.accid;
		zone     = rd.zone;
		game     = rd.game;
		bcopy(rd.name, name, sizeof(name));
	}
	const RoleData & operator= (const RoleData &rd)
	{
		accid    = rd.accid;
		zone     = rd.zone;
		game     = rd.game;
		strcpy(name, rd.name);
		return *this;
	}
};


/**
 * \brief �������ݿ���ʷ�������������
 */
class RoleTask : public zTCPTask
{
	public:
	
		int verifyConn();
		int waitSync();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
	
		/**
		 * \brief ���캯��
		 * \param pool �������ӳ�
		 * \param sock TCP/IP�׽ӿ�
		 * \param addr ��ַ
		 */
		RoleTask(zTCPTaskPool *pool, const int sock, 
					const struct sockaddr_in *addr = NULL) : zTCPTask(pool, sock, addr)
		{
		}
		
		/**
		 * \brief ��������
		 */
		~RoleTask()
		{
		}
		
	private:
		
		bool msgParse_loginServer(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
};

#endif

