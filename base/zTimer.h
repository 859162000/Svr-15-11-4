/**
 * \file
 * \version  $Id: zTimer.h  $
 * \author  
 * \date 
 * \brief ��ʱ�������ļ�
 *
 */

#ifndef _ZTIMER_H_
#define _ZTIMER_H_

#include <queue>
#include <sstream>

#include "zMutex.h"
#include "zThread.h"
#include "zTime.h"
#include "zNoncopyable.h"
/**
 * \brief ��ʱ��,��ʱ����Ϊ����
 *
 * ���ڲ���һ�������Ĵ����̡߳�������Ӷ���������ڶ��������һ���̣߳���ִ�е�ʱ��������Ӱ�����������ִ�С�
 */
class zTimer
{
	private:
		class zTimerThread;
		class zTimerQueue;
	public:
		/**
		 * \brief ��ʱ���������,�û�����̳�ʵ��#run���� 
		 *
		 */
		class zTimerTask : private zNoncopyable
		{
			friend class zTimer;
			friend class zTimerThread;
			friend class zTimerQueue;

			private:
				void schedExecTime(const zRTime &tv);
			protected:

			zMutex taskMutex;				/**< ���񻥳��� */	
			int period;						/**< ������,���Ϊ0��ʾһ�������� */
			zRTime nextExecTime;			/**< ��һ��ִ�������ʱ�� */
			enum
			{
				VIRGIN,						/**< ������ */
				SCHEDULED,					/**< �Ѿ���ӵ���ʱ���е����� */
				EXECUTED,					/**< �Ѿ�ִ�й������� */
				CANCELLED					/**< ��ȡ�������� */
			}
			state;							/**< ����״̬ */

			zTimerTask();

			public:
			bool cancel();
			bool operator > (const zTimerTask & right)const;

			virtual ~zTimerTask();
			virtual void run() =0;
		};

		zTimer(const std::string &name="Timer-" + ((std::ostringstream &)(std::ostringstream()<<getID())).str() );
		~zTimer();
		bool schedule(zTimerTask *task, zRTime &tv);
		bool schedule(zTimerTask *task, int delay);
		bool scheduleAtDelay(zTimerTask *task, int delay ,int period);
		bool scheduleAtDelay(zTimerTask *task, zRTime &tv,int period);
		bool scheduleAtRate(zTimerTask *task, int delay ,int period);
		bool scheduleAtRate(zTimerTask *task, zRTime &tv,int period);
		void cancel();
		
	private:
		/**
		 * \brief ��ʱ�����������,�������� 
		 *
		 */
		class zTimerQueue
		{
			friend class zTimer;
			friend class zTimerThread;
			private:
				std::vector <zTimerTask *> queue;
				zMutex qmutex;
				bool canAddTask;

			public:

				void push(zTimerTask * p)
				{
					std::vector <zTimerTask *> ::iterator it=queue.end();
					for(it = queue.begin();it!=queue.end();it++)
					{
						if(**it > *p)
						{
							break;
						}
					}
					queue.insert(it,p);
				}

				zTimerTask * top()
				{
					if(queue.empty())
						return  (zTimerTask *)0;
					else
						return queue[0];
				}

				void pop()
				{
					if(!queue.empty())
						queue.erase(queue.begin());
				}

				int size()
				{
					return queue.size();
				}

				bool empty()
				{
					return queue.empty();
				}
		};


		/**
		 * \brief ��ʱ��������ִ���̣߳����������У������������ִ��������ִ�� 
		 *
		 */
		class zTimerThread:public zThread
		{
			private:
				zTimerQueue *taskQueue;
			public:
				zTimerThread(const std::string & name,zTimerQueue *queue);
				~zTimerThread();
				virtual void run();
		};


		zTimerQueue taskqueue;	/**< ִ���������,��#threadǰ��ʼ�� */
		zTimerThread thread;	/**< ִ�������߳�,��#taskqueue���ʼ�� */ 

		static int id;	/**< һ�������������ṩĬ�ϼ�ʱ������ʱ������ */ 
		static int getID();
		bool addTask(zTimerTask *task, zRTime &tv,int period);
};

#endif
