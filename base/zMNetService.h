/**
 * \file
 * \version  $Id: zMNetService.h  $
 * \author  
 * \date 
 * \brief ʵ������������Ŀ�ܴ���
 *
 * �����Ƚ�ͨ��һ�㣬�ٴ����Ƚ�һ�����������������ʱ���ǱȽ�����
 * 
 */

#ifndef _zMNetService_h_
#define _zMNetService_h_

#include <iostream>
#include <string>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zMTCPServer.h"

/**
 * \brief �����������
 * ʵ���������������ܴ��룬�����Ƚ�ͨ��һ��
 */
class zMNetService : public zService
{

	public:

		/**
		 * \brief ����������
		 */
		virtual ~zMNetService() { instance = NULL; };

		/**
		 * \brief ���ݵõ���TCP/IP���ӻ�ȡһ����������
		 * \param sock TCP/IP�׽ӿ�
		 * \param srcPort ���ڷ��������˶���˿ڣ��������ָ�����������Ǹ��󶨶˿�
		 * \return �½�������������
		 */
		virtual void newTCPTask(const int sock, const unsigned short srcPort) = 0;

		/**
		 * \brief �󶨷���ĳ���˿�
		 * \param name ���󶨶˿�����
		 * \param port ���󶨵Ķ˿�
		 * \return ���Ƿ�ɹ�
		 */
		bool bind(const std::string &name, const unsigned short port)
		{
			if (tcpServer)
				return tcpServer->bind(name, port);
			else
				return false;
		}

	protected:

		/**
		 * \brief ���캯��
		 * �ܱ����Ĺ��캯����ʵ����Singleton���ģʽ����֤��һ��������ֻ��һ����ʵ��
		 * \param name ����
		 */
		zMNetService(const std::string &name) : zService(name)
		{
			instance = this;

			serviceName = name;
			tcpServer = NULL;
		}

		bool init();
		bool serviceCallback();
		void final();

	private:

		static zMNetService *instance;		/**< ���Ψһʵ��ָ�룬���������࣬��ʼ��Ϊ��ָ�� */
		std::string serviceName;			/**< ������������� */
		zMTCPServer *tcpServer;				/**< TCP������ʵ��ָ�� */

};

#endif

