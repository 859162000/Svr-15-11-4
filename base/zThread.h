/**
 * \file
 * \version  $Id: zThread.h  $
 * \author  
 * \date 
 * \brief ������zThread
 *
 * 
 */

#ifndef _zThread_h_
#define _zThread_h_

#include <pthread.h>
#include <unistd.h>

#include "zNoncopyable.h"
#include "Zebra.h"
#include "zMutex.h"
#include "zCond.h"
#include "zRWLock.h"

/**
 * \brief ��װ���̲߳���������ʹ���̵߳Ļ���
 *
 */
class zThread : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯��������һ������
		 *
		 * \param name �߳�����
		 * \param joinable ��������߳��˳���ʱ���Ƿ񱣴�״̬�����Ϊtrue��ʾ�߳��˳�����״̬�����򽫲������˳�״̬
		 */
		zThread(const std::string &name = std::string("zThread"), const bool joinable = true) 
			: threadName(name), alive(false), complete(false), thread(0), joinable(joinable) {};

		/**
		 * \brief ������������������һ�����󣬻��ն���ռ�
		 *
		 */
		virtual ~zThread() {};

		/**
		 * \brief ��ȡ��ǰ�̱߳��
		 *
		 *
		 * \return �̱߳��
		 */
		static pthread_t getCurrentThreadId()
		{
			return ::pthread_self();
		}

		/**
		 * \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬��
		 *
		 *
		 * \param sec ָ����ʱ�䣬��
		 */
		static void sleep(const long sec)
		{
			::sleep(sec);
		}

		/**
		 * \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬����
		 *
		 *
		 * \param msec ָ����ʱ�䣬����
		 */
		static void msleep(const long msec)
		{
			::usleep(1000 * msec);
		}

		/**
		 * \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬΢��
		 *
		 *
		 * \param usec ָ����ʱ�䣬΢��
		 */
		static void usleep(const long usec)
		{
			::usleep(usec);
		}

		/**
		 * \brief �߳��Ƿ���joinable��
		 *
		 *
		 * \return joinable
		 */
		const bool isJoinable() const
		{
			return joinable;
		}

		/**
		 * \brief ����߳��Ƿ�������״̬
		 *
		 * \return �߳��Ƿ�������״̬
		 */
		const bool isAlive() const
		{
			return alive;
		}

		static void *threadFunc(void *arg);
		bool start();
		void join();

		/**
		 * \brief ���������߳�
		 *
		 * ��ʵֻ�����ñ�ǣ���ô�̵߳�run���ص�ѭ���ؼ�������ǣ�����������Ѿ����ã����˳�ѭ��
		 *
		 */
		void final()
		{
			//Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
			complete = true;
		}

		/**
		 * \brief �ж��߳��Ƿ����������ȥ
		 *
		 * ��Ҫ����run()����ѭ���У��ж�ѭ���Ƿ����ִ����ȥ
		 *
		 * \return �߳����ص��Ƿ����ִ��
		 */
		const bool isFinal() const 
		{
			//if (complete)
			//	Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
			return complete;
		}

		/**
		 * \brief ���鹹�������߳����ص�������ÿ����Ҫʵ��������������Ҫ�����������
		 *
		 * ���������ѭ����Ҫ��ÿ��ѭ������߳��˳����isFinal()�������ܹ���֤�̰߳�ȫ�˳�
		 * <pre>
		 * 	while(!isFinal())
		 * 	{
		 * 		...
		 * 	}
		 * 	</pre>
		 *
		 */
		virtual void run() = 0;

		/**
		 * \brief �ж������߳��Ƿ���ͬһ���߳�
		 * \param other ���Ƚϵ��߳�
		 * \return �Ƿ���ͬһ���߳�
		 */
		bool operator==(const zThread& other) const
		{
			return pthread_equal(thread, other.thread) != 0;
		}

		/**
		 * \brief �ж������߳��Ƿ���ͬһ���߳�
		 * \param other ���Ƚϵ��߳�
		 * \return �Ƿ���ͬһ���߳�
		 */
		bool operator!=(const zThread& other) const
		{
			return !operator==(other);
		}

		/**
		 * \brief �����߳�����
		 *
		 * \return �߳�����
		 */
		const std::string &getThreadName() const
		{
			return threadName;
		}

	private:

		std::string threadName;			/**< �߳����� */
		zMutex mlock;					/**< ������ */
		zCond cond;						/**< �������� */
		volatile bool alive;			/**< �߳��Ƿ������� */
		volatile bool complete;			/**< �߳��Ƿ񽫽��� */
		pthread_t thread;				/**< �̱߳�� */
		bool joinable;					/**< �߳����ԣ��Ƿ�����joinable��� */

}; 

/**
 * \brief ���߳̽��з���������
 *
 */
class zThreadGroup : private zNoncopyable
{

	public:

		struct Callback
		{
			virtual void exec(zThread *e)=0;
			virtual ~Callback(){};
		};

		typedef std::vector<zThread *> Container;	/**< �������� */

		zThreadGroup();
		~zThreadGroup();
		void add(zThread *thread);
		zThread *getByIndex(const Container::size_type index);
		zThread *operator[] (const Container::size_type index);
		void joinAll();
		void execAll(Callback &cb);
		
		const Container::size_type size()
		{
			zRWLock_scope_rdlock scope_rdlock(rwlock);
			return vts.size();
		}

	private:

		Container vts;								/**< �߳����� */
		zRWLock rwlock;								/**< ��д�� */

};

#endif

