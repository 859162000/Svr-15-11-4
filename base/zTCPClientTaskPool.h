/**
 * \file
 * \version  $Id: zTCPClientTaskPool.h  $
 * \author  
 * \date 
 * \brief ��װʵ���̳߳أ����ڴ�������ӷ�����
 *
 * 
 */


#ifndef _zTCPClientTaskPool_h_
#define _zTCPClientTaskPool_h_

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <unistd.h>
#include <sys/timeb.h>

#include "zSocket.h"
#include "zThread.h"
#include "zTCPClientTask.h"
#include "Zebra.h"
#include "zString.h"

class zCheckconnectThread;
class zCheckwaitThread;
class zTCPClientTaskThread;

/**
 * \brief �����̳߳��࣬��װ��һ���̴߳��������ӵ��̳߳ؿ��
 *
 */
class zTCPClientTaskPool : private zNoncopyable
{

	public:

		explicit zTCPClientTaskPool(const unsigned int connPerThread=512, const int us=50000) : connPerThread(connPerThread)
		{       
			usleep_time=us;
			checkwaitThread = NULL; 
		} 
		~zTCPClientTaskPool();

		bool init();
		bool put(zTCPClientTask *task);
		void timeAction(const zTime &ct);

		void addCheckwait(zTCPClientTask *task);
		bool addMain(zTCPClientTask *task);
		void setUsleepTime(int time)
		{
			usleep_time = time;
		}

	private:

		const unsigned int connPerThread;
		zTCPClientTaskThread *newThread();

		/**
		 * \brief ���Ӽ���߳�
		 *
		 */
		zCheckconnectThread *checkconnectThread;;
		/**
		 * \brief ���ӵȴ�������Ϣ���߳�
		 *
		 */
		zCheckwaitThread *checkwaitThread;;
		/**
		 * \brief ���гɹ����Ӵ�������߳�
		 *
		 */
		zThreadGroup taskThreads;

		/**
		 * \brief ������������
		 *
		 */
		typedef std::list<zTCPClientTask *, __gnu_cxx::__pool_alloc<zTCPClientTask *> > zTCPClientTaskContainer;

		/**
		 * \brief �����������������
		 *
		 */
		typedef zTCPClientTaskContainer::iterator zTCPClientTask_IT;

		zMutex mlock;					/**< ������� */
		zTCPClientTaskContainer tasks;	/**< �����б� */

	public:
		int usleep_time;                                        /**< ѭ���ȴ�ʱ�� */
};

#endif

