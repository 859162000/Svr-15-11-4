/**
 * \file
 * \version  $Id: zMTCPServer.h  $
 * \author  
 * \date 
 * \brief ��װTCP�ķ���������ģ��
 *
 * 
 */

#ifndef _zMTCPServer_h_
#define _zMTCPServer_h_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <map>

#include "zNoncopyable.h"
#include "zSocket.h"
#include "zMutex.h"

/**
 * \brief zMTCPServer�࣬��װ�˷���������ģ�飬���Է���Ĵ���һ�����������󣬵ȴ��ͻ��˵�����
 * ����ͬʱ��������˿�
 */
class zMTCPServer : private zNoncopyable
{

	public:

		typedef std::map<int, unsigned short> Sock2Port;
		typedef Sock2Port::value_type Sock2Port_value_type;
		typedef Sock2Port::iterator Sock2Port_iterator;
		typedef Sock2Port::const_iterator Sock2Port_const_iterator;

		zMTCPServer(const std::string &name);
		~zMTCPServer() ;

		bool bind(const std::string &name, const unsigned short port);
		int accept(Sock2Port &res);

	private:

		static const int T_MSEC =2100;			/**< ��ѯ��ʱ������ */
		static const int MAX_WAITQUEUE = 2000;	/**< ���ȴ����� */

		std::string name;						/**< ���������� */
		Sock2Port mapper;
		zMutex mlock;
#ifdef _USE_EPOLL_
		int kdpfd;
		std::vector<struct epoll_event> epfds;
#else
		std::vector<struct pollfd> pfds;
#endif

}; 


#endif
