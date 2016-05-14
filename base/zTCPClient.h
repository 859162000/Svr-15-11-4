/**
 * \file
 * \version  $Id: zTCPClient.h  $
 * \author  
 * \date 
 * \brief TCP�ͻ��˷�װ
 *
 * 
 */

#ifndef _zTCPClient_h_
#define _zTCPClient_h_

#include <pthread.h>
#include <string>

#include "zSocket.h"
#include "zThread.h"

/**
 * \brief TCP�ͻ���
 *
 * ��װ��һЩTCP�ͻ��˵��߼������罨�����ӵȵȣ���ʵ��Ӧ���У���Ҫ��������࣬�����ؽ���ָ��ĺ���msgParse
 *
 */
class zTCPClient : public zThread, public zProcessor
{

	public:

		/**
		 * \brief ���캯��������ʵ�����󣬳�ʼ�������Ա
		 *
		 *
		 * \param name ����
		 * \param ip ��ַ
		 * \param port �˿�
		 * \param compress �ײ����ݴ����Ƿ�֧��ѹ��
		 */
		zTCPClient(
				const std::string &name, 
				const std::string &ip = "127.0.0.1", 
				const unsigned short port = 80,
				const bool compress = false) 
			: zThread(name), ip(ip), port(port), pSocket(NULL), compress(compress) {};

		/**
		 * \brief �������������ٶ���
		 *
		 */
		~zTCPClient() 
		{
			close();
		}

		bool connect();

		/**
		 * \brief ����һ������������TCP���ӣ�ָ����������IP��ַ�Ͷ˿�
		 *
		 *
		 * \param ip ��������IP��ַ
		 * \param port �������Ķ˿�
		 * \return �����Ƿ�ɹ�
		 */
		bool connect(const char *ip, const unsigned short port)
		{
			this->ip = ip;
			this->port = port;
			return connect();
		}

		/**
		 * \brief �رտͻ�������
		 *
		 */
		virtual void close()
		{
			SAFE_DELETE(pSocket);
		}

		virtual bool sendCmd(const void *pstrCmd, const int nCmdLen);

		/**
		 * \brief ���÷�����IP��ַ
		 *
		 *
		 * \param ip ���õķ�����IP��ַ
		 */
		void setIP(const char *ip)
		{
			this->ip = ip;
		}

		/**
		 * \brief ��ȡ������IP��ַ
		 *
		 *
		 * \return ���ص�ַ
		 */
		const char *getIP() const
		{
			return ip.c_str();
		}

		/**
		 * \brief ���÷������˿�
		 *
		 *
		 * \param port ���õķ������˿�
		 */
		void setPort(const unsigned short port)
		{
			this->port = port;
		}

		/**
		 * \brief ��ȡ�������˿�
		 *
		 *
		 * \return ���ض˿�
		 */
		const unsigned short getPort() const
		{
			return port;
		}

		virtual void run();
		//ָ�����
		static CmdAnalysis analysis;

	protected:

		std::string ip;									/**< ��������ַ */
		unsigned short port;							/**< �������˿� */
		zSocket *pSocket;								/**< �ײ��׽ӿ� */

		const bool compress;							/**< �Ƿ�֧��ѹ�� */

}; 

class zTCPBufferClient : public zTCPClient
{

	public:

		zTCPBufferClient(
				const std::string &name, 
				const std::string &ip = "127.0.0.1", 
				const unsigned short port = 80,
				const bool compress = false,
				const int usleep_time = 50000) 
			: zTCPClient(name, ip, port, compress), usleep_time(usleep_time), _buffered(false) { }

		void close()
		{
			sync();
			zTCPClient::close();
		}

		void run();
		bool sendCmd(const void *pstrCmd, const int nCmdLen);
		void setUsleepTime(const int utime)
		{
			usleep_time = utime;
		}

	private :

		bool ListeningRecv();
		bool ListeningSend();
		void sync();

		int usleep_time;
		volatile bool _buffered;

};

#endif
