/**
 * \file
 * \version  $Id: zHttpTask.h  $
 * \author  
 * \date 
 * \brief ����ʵ��������(lightweight)��http������
 */


#ifndef _zHttpTask_h_
#define _zHttpTask_h_

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <unistd.h>
#include <sys/timeb.h>

#include "zSocket.h"
#include "zNoncopyable.h"
#include "zTime.h"

class zHttpTaskPool;

/**
 * \brief ����������http�����࣬��װһЩ�Ͳ�ӿ�
 */
class zHttpTask : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯�������ڴ���һ������
		 * \param pool �������ӳ�ָ��
		 * \param sock �׽ӿ�
		 * \param addr ��ַ
		 */
		zHttpTask(
				zHttpTaskPool *pool,
				const int sock,
				const struct sockaddr_in *addr = NULL) : pool(pool), lifeTime()
		{
			pSocket = new zSocket(sock, addr, false);
		}

		/**
		 * \brief ������������������һ������
		 */
		virtual ~zHttpTask()
		{
			SAFE_DELETE(pSocket) ;
		}

#ifdef _USE_EPOLL_
		/**
		 * \brief ��Ӽ���¼���epoll������
		 * \param kdpfd epoll������
		 * \param events ����ӵ��¼�
		 * \param ptr �������
		 */
		void addEpoll(int kdpfd, __uint32_t events, void *ptr)
		{
			if (pSocket)
				pSocket->addEpoll(kdpfd, events, ptr);
		}
		/**
		 * \brief ��epoll��������ɾ������¼�
		 * \param kdpfd epoll������
		 * \param events ����ӵ��¼�
		 */
		void delEpoll(int kdpfd, __uint32_t events)
		{
			if (pSocket)
				pSocket->delEpoll(kdpfd, events);
		}
#else
		/**
		 * \brief ���pollfd�ṹ
		 * \param pfd �����Ľṹ
		 * \param events �ȴ����¼�����
		 */
		void fillPollFD(struct pollfd &pfd, short events)
		{
			if (pSocket)
				pSocket->fillPollFD(pfd, events);
		}
#endif

		/**
		 * \brief ����Ƿ���֤��ʱ
		 * \param ct ��ǰϵͳʱ��
		 * \param interval ��ʱʱ�䣬����
		 * \return ����Ƿ�ɹ�
		 */
		bool checkHttpTimeout(const zRTime &ct, const unsigned long long interval = 2000) const
		{
			return (lifeTime.elapse(ct) > interval);
		}

		/**
		 * \brief http������������
		 * \return �Ƿ�ɹ���1��ʾ�ɹ���0����ʾ��Ҫ�����ȴ���-1����ʾʧ��
		 */
		virtual int httpCore()
		{
			return 1;
		}

		bool sendCmd(const void *pstrCmd, int nCmdLen);

	protected:

		zSocket *pSocket;								/**< �ײ��׽ӿ� */

	private:

		zHttpTaskPool *pool;							/**< ���������ĳ� */
		zRTime lifeTime;								/**< ���Ӵ���ʱ���¼ */

};

#endif

