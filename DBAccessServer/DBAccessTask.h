/**
 * \file
 * \version  $Id: InfoTask.h  $
 * \author 
 * \date 
 * \brief �������ݿ���ʷ�����������
 */

#ifndef _DBAccessTask_h_
#define _DBAccessTask_h_

#include <iostream>
#include <string>
#include <vector>
#include <ext/numeric>

#include "zService.h"
#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zMisc.h"
#include "InfoCommand.h"

class InfoContainer;
class InfoTask : public zTCPTask
{
	public:

		int verifyConn();
		int waitSync();
		void addToContainer();
		void removeFromContainer();
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief ���캯��
		 * \param pool �������ӳ�
		 * \param sock TCP/IP�׽ӿ�
		 * \param addr ��ַ
		 * \param pCon INFO����
		 */
		InfoTask(zTCPTaskPool *pool, const int sock, 
				const struct sockaddr_in *addr,
				InfoContainer * pCon) : zTCPTask(pool, sock, addr) 
		{
			pContainer = pCon;
		}

	 	/**
	 	 * \brief ��������
	 	 */
		~InfoTask() { }

	private:
	
		bool msgParse_loginServer(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		/**
		 * \brief ָ��ȫ��������ָ��
		 */
		InfoContainer  * pContainer;
};

class InfoContainer : private zNoncopyable
{
	public:
		
		/**
		 * \brief ���캯��
		 */
		InfoContainer() { }
		
		/**
		 * \brief ��������
		 */
		~InfoContainer(){ }
		
		/**
		 * \brief �����������һ��Ԫ��
		 * \param iTask
		 */
		void add(InfoTask *iTask)
		{
			mlock.lock();
			vecInfo.push_back(iTask);
			mlock.unlock();
		}
		
		/**
		 * \brief ��������ɾ��һ��Ԫ��
		 * \param iTask
		 */
		void remove(InfoTask *iTask)
		{
			mlock.lock();
			std::vector<InfoTask *>::iterator Iter = find(vecInfo.begin(), vecInfo.end(), iTask);
			vecInfo.erase(Iter);
			mlock.unlock();
		}

		/**
		 * \brief ���������ӵķ������㲥ָ��
		 * \param pCmd ��Ҫ�㲥��ָ��
		 * \param nCmdLen ָ��ĳ���
		 */
		void broadcast(Cmd::t_NullCmd *pCmd, unsigned int nCmdLen)
		{
			mlock.lock();
			for (std::vector<InfoTask *>::iterator Iter=vecInfo.begin(); Iter!=vecInfo.end(); ++Iter)
			{
				(*Iter)->sendCmd(pCmd, nCmdLen);	
			}
			mlock.unlock();
		}
		
	private:
		
		/**
		 * \brief InfoTaskָ������
		 */
		std::vector <InfoTask *>	vecInfo;
		/**
		 * \brief �������ʻ������
		 */
		zMutex mlock;
};

#endif



