/**
 * \file
 * \version  $Id: zSubNetService.h  $
 * \author  
 * \date 
 * \brief ʵ������������Ŀ�ܴ���
 *
 * �����Ҫ��ʹ������Ҫ���ӷ����������������������
 * 
 */

#ifndef _zSubNetService_h_
#define _zSubNetService_h_

#include <iostream>
#include <string>
#include <deque>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zNetService.h"
#include "zTCPClient.h"
#include "zMisc.h"
#include "Zebra.h"
#include "SuperCommand.h"
#include "Zebra.h"

class SuperClient;

/**
 * \brief �����������ܴ���
 *
 * ����Ҫ��������������������ӵ������������ʹ��
 *
 */
class zSubNetService : public zNetService
{

	public:

		virtual ~zSubNetService();

		/**
		 * \brief ��ȡ���Ψһʵ��
		 *
		 * �����ʵ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
		 *
		 */
		static zSubNetService *subNetServiceInstance()
		{
			return subNetServiceInst;
		}

		/**
		 * \brief �������Է�������������ָ��
		 *
		 * ��Щָ���������ķ������йصģ���Ϊͨ�õ�ָ��Ѿ�������
		 *
		 * \param ptNullCmd �������ָ��
		 * \param nCmdLen ָ���
		 * \return �����Ƿ�ɹ�
		 */
		virtual bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen) = 0;

		bool sendCmdToSuperServer(const void *pstrCmd, const int nCmdLen);
		void setServerInfo(const Cmd::Super::t_Startup_Response *ptCmd);
		void addServerEntry(const Cmd::Super::ServerEntry &entry);
		const Cmd::Super::ServerEntry *getServerEntry(const WORD wdServerID);
		const Cmd::Super::ServerEntry *getServerEntryByType(const WORD wdServerType);
		const Cmd::Super::ServerEntry *getNextServerEntryByType(const WORD wdServerType, const Cmd::Super::ServerEntry **prev);

		/**
		 * \brief ���ط��������
		 *
		 * \return ���������
		 */
		const WORD getServerID() const
		{
			return wdServerID;
		}

		/**
		 * \brief ���ط���������
		 *
		 * \return ����������
		 */
		const WORD getServerType() const
		{
			return wdServerType;
		}

	protected:

		zSubNetService(const std::string &name, const WORD wdType);

		bool init();
		bool validate();
		void final();

		WORD wdServerID;					/**< ��������ţ�һ����Ψһ�� */
		WORD wdServerType;					/**< ���������ͣ�������ʵ����ʱ���Ѿ�ȷ�� */
		char pstrName[MAX_NAMESIZE];		/**< ���������� */
		char pstrIP[MAX_IP_LENGTH];			/**< ������������ַ */
		WORD wdPort;						/**< �����������˿ڣ�Ҳ���ǰ�˿� */
		char pstrExtIP[MAX_IP_LENGTH];		/**< ������������ַ��Ҳ���Ƿ���ǽ��ַ */
		WORD wdExtPort;						/**< �����������˿ڣ�Ҳ����ӳ�䵽����ǽ�Ķ˿� */

	private:

		unsigned short superPort;		/**< �������������Ķ˿� */
		char superIP[MAX_IP_LENGTH];	/**< �������������ĵ�ַ */

		SuperClient *superClient;		/**< �������������Ŀͻ���ʵ�� */

		static zSubNetService *subNetServiceInst;			/**< ���Ψһʵ��ָ�룬���������࣬��ʼ��Ϊ��ָ�� */
		zMutex mlock;										/**< ������������Ϣ�б���ʻ����� */
		std::deque<Cmd::Super::ServerEntry> serverList;		/**< ������������Ϣ�б���֤������֮�����֤��ϵ */

};

#endif

