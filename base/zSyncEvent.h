/**
 * \file
 * \version  $Id: zSyncEvent.h  $
 * \author  
 * \date 
 * \brief ʵ��һ��ͬ���¼�ģ��
 *
 * 
 */

#ifndef _zSyncEvent_h_
#define _zSyncEvent_h_

#include "zNoncopyable.h"
#include "zMutex.h"
#include "zCond.h"

/**
 * \brief ʵ����ͬ���¼���
 *
 * 	һ���߳�ͨ��signal()�������źţ�����һ���߳�ͨ��wait()�ȴ�����źŴ���
 *
 */
class zSyncEvent : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯��
		 *
		 * ���ڴ���һ������ʵ��
		 *
		 * \param initstate ��ʼ״̬
		 */
		zSyncEvent(const bool initstate = false) : state(initstate) {};

		/**
		 * \brief ��������
		 *
		 * ����һ������ʵ��
		 *
		 */
		~zSyncEvent() {};

		/**
		 * \brief �����ź�
		 *
		 * ����һ���¼��źţ����ѵȴ�����źŵ��߳�
		 *
		 */
		void signal()
		{
			mutex.lock();
			while(state)
				cond2.wait(mutex);
			state = true;
			cond1.signal();
			mutex.unlock();
		}

		/**
		 * \brief �ȴ�һ���¼��źŵĵ���
		 *
		 * �ȴ�һ���źţ�ֱ����һ���̵߳���signal��������߳�
		 *
		 */
		void wait()
		{
			mutex.lock();
			while(!state)
				cond1.wait(mutex);
			state = false;
			cond2.signal();
			mutex.unlock();
		}

	private:

		volatile bool state;		/**< �¼���ǰ״̬ */
		zMutex mutex;				/**< ������ */
		zCond cond1, cond2;			/**< �������� */

};

#endif

