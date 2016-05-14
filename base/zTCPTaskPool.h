/**
 * \file
 * \version  $Id: zTCPTaskPool.h  $
 * \author  
 * \date 
 * \brief ��װʵ���̳߳أ����ڴ�������ӷ�����
 *
 * 
 */


#ifndef _zTCPTaskPool_h_
#define _zTCPTaskPool_h_

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <unistd.h>
#include <sys/timeb.h>

#include "zSocket.h"
#include "zThread.h"
#include "zTCPTask.h"
#include "Zebra.h"
#include "zString.h"

class zSyncThread;
class zRecycleThread;

/**
 * \brief �����̳߳��࣬��װ��һ���̴߳��������ӵ��̳߳ؿ��
 *
 */
class zTCPTaskPool : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯��
		 * \param maxConns �̳߳ز��д�����Ч���ӵ��������
		 * \param state ��ʼ����ʱ�������̳߳ص�״̬
		 */
		explicit zTCPTaskPool(const int maxConns, const int state,const int us=50000) : maxConns(maxConns), state(state)/*,usleep_time(us)// */
		{
			setUsleepTime(us);
			syncThread = NULL;
			recycleThread = NULL;
			maxThreadCount = minThreadCount;
		};

		/**
		 * \brief ��������������һ���̳߳ض���
		 *
		 */
		~zTCPTaskPool()
		{
			final();
		}

		/**
		 * \brief ��ȡ�����̳߳ص�ǰ״̬
		 *
		 * \return ���������̳߳صĵ�ǰ״̬
		 */
		const int getState() const
		{
			return state;
		}

		/**
		 * \brief ���������̳߳�״̬
		 *
		 * \param state ���õ�״̬���λ
		 */
		void setState(const int state)
		{
			this->state |= state;
		}

		/**
		 * \brief ��������̳߳�״̬
		 *
		 * \param state �����״̬���λ
		 */
		void clearState(const int state)
		{
			this->state &= ~state;
		}

		const int getSize();
		inline const int getMaxConns() const { return maxConns; }
		bool addVerify(zTCPTask *task);
		void addSync(zTCPTask *task);
		bool addOkay(zTCPTask *task);
		void addRecycle(zTCPTask *task);
		static void  setUsleepTime(int time)
		{
			usleep_time=time;
		}

		bool init();
		void final();

	private:

		const int maxConns;										/**< �̳߳ز��д������ӵ�������� */

		static const int maxVerifyThreads = 4;					/**< �����֤�߳����� */
		zThreadGroup verifyThreads;								/**< ��֤�̣߳������ж�� */

		zSyncThread *syncThread;								/**< �ȴ�ͬ���߳� */

		static const int minThreadCount = 1;					/**< �̳߳���ͬʱ�����������̵߳����ٸ��� */
		int maxThreadCount;										/**< �̳߳���ͬʱ�����������̵߳������� */
		zThreadGroup okayThreads;								/**< �������̣߳���� */

		zRecycleThread *recycleThread;							/**< ���ӻ����߳� */

		int state;												/**< ���ӳ�״̬ */
	public:
		static int usleep_time;										/**< ѭ���ȴ�ʱ�� */

};

#endif

