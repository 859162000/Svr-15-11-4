/**
 * \file
 * \version  $Id: InfoClient.h  $
 * \author  
 * \date 
 * \brief �����������Ϣ�ռ��Ŀͻ�������
 */

#ifndef _InfoClient_h_
#define _InfoClient_h_

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "NetType.h"

/**
 * \brief ��Ϣ�ռ��ͻ���������
 */
class InfoClient : public zTCPClientTask
{

	public:

		InfoClient(
				const std::string &ip,
				const unsigned short port);
		~InfoClient();

		int checkRebound();
		void addToContainer();
		void removeFromContainer();
		bool connect();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		const DWORD getTempID() const
		{
			return tempid;
		}

		const NetType getNetType() const
		{
			return netType;
		}

	private:
		const DWORD tempid;
		static DWORD tempidAllocator;
		NetType netType;

};

#endif

