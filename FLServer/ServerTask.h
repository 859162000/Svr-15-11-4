/**
 * \file
 * \version  $Id: ServerTask.h  $
 * \author  
 * \date 
 * \brief �����������������
 */

#ifndef _ServerTask_h_
#define _ServerTask_h_

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zMisc.h"

/**
 * \brief ��������������
 */
class ServerTask : public zTCPTask
{

	public:

		/**
		 * \brief ���캯��
		 * ���ڴ���һ����������������
		 * \param pool ���������ӳ�
		 * \param sock TCP/IP�׽ӿ�
		 */
		ServerTask(
				zTCPTaskPool *pool,
				const int sock) : zTCPTask(pool, sock, NULL)
		{
		}

		/**
		 * \brief ����������
		 */
		~ServerTask() {};

		int verifyConn();
		int waitSync();
		bool uniqueAdd();
		bool uniqueRemove();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/*void setZoneID(const GameZone_t &gameZone)
		{
			this->gameZone = gameZone;
		}*/

		const GameZone_t &getZoneID() const
		{
			return gameZone;
		}

	private:

		GameZone_t gameZone;
		std::string name;

		bool msgParse_gyList(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool msgParse_session(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

};

#endif

