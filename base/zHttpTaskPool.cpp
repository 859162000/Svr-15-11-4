/**
 * \file
 * \version  $Id: zHttpTaskPool.cpp  $
 * \author  
 * \date 
 * \brief ����ʵ��������(lightweight)��http������
 */


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>

#include "zSocket.h"
#include "zThread.h"
#include "zHttpTaskPool.h"
#include "Zebra.h"
#include "zTime.h"

/**
 * \brief ������http������������߳�
 */
class zHttpThread : public zThread
{

	private:

		/**
		 * \brief http����������������
		 */
		typedef std::list<zHttpTask *, __gnu_cxx::__pool_alloc<zHttpTask *> > zHttpTaskContainer;

#ifdef _USE_EPOLL_
		/**
		 * \brief epoll�¼��ṹ��������
		 */
		typedef std::vector<struct epoll_event> epollfdContainer;
#else
		/**
		 * \brief poll�¼��ṹ��������
		 */
		typedef std::vector<struct pollfd> pollfdContainer;
#endif

		zHttpTaskPool *pool;		/**< �����ĳ� */
		zRTime currentTime;			/**< ��ǰʱ�� */
		zMutex mutex;				/**< ������� */
		zHttpTaskContainer tasks;	/**< �����б� */

#ifdef _USE_EPOLL_
		int kdpfd;
		epollfdContainer epfds;
		epollfdContainer::size_type fds_count;
#else
		pollfdContainer pfds;
#endif

	public:

		/**
		 * \brief ���캯��
		 * \param pool ���������ӳ�
		 * \param name �߳�����
		 */
		zHttpThread(
				zHttpTaskPool *pool,
				const std::string &name = std::string("zHttpThread"))
			: zThread(name), pool(pool), currentTime()
			{
#ifdef _USE_EPOLL_
				kdpfd = epoll_create(256);
				assert(-1 != kdpfd);
				epfds.resize(256);
				fds_count = 0;
#endif
			}

		/**
		 * \brief ��������
		 */
		~zHttpThread()
		{
#ifdef _USE_EPOLL_
			TEMP_FAILURE_RETRY(::close(kdpfd));
#endif
		}

		void run();

		/**
		 * \brief ���һ����������
		 * \param task ��������
		 */
		void add(zHttpTask *task)
		{
			mutex.lock();
#ifdef _USE_EPOLL_
			task->addEpoll(kdpfd, EPOLLIN | EPOLLERR | EPOLLPRI, (void *)task);
			tasks.push_back(task);
			fds_count++;
			if (fds_count > epfds.size())
			{
				epfds.resize(fds_count + 16);
			}
#else
			struct pollfd pfd;
			task->fillPollFD(pfd, POLLIN | POLLERR | POLLPRI);
			tasks.push_back(task);
			pfds.push_back(pfd);
#endif
			mutex.unlock();
		}

#ifdef _USE_EPOLL_
		typedef zHttpTask* zHttpTaskP;
		void remove(zHttpTaskP &task)
		{
			task->delEpoll(kdpfd, EPOLLIN | EPOLLERR | EPOLLPRI);
			tasks.remove(task);
			SAFE_DELETE(task);
			fds_count--;
		}
		void remove(zHttpTaskContainer::iterator &it)
		{
			zHttpTask *task = *it;
			task->delEpoll(kdpfd, EPOLLIN | EPOLLERR | EPOLLPRI);
			tasks.erase(it);
			SAFE_DELETE(task);
			fds_count--;
		}
#else
		void remove(zHttpTaskContainer::iterator &it, int p)
		{
			int i;
			pollfdContainer::iterator iter;
			for(iter = pfds.begin(), i = 0; iter != pfds.end(); iter++, i++)
			{
				if (i == p)
				{
					zHttpTask *task = *it;
					pfds.erase(iter);
					tasks.erase(it);
					SAFE_DELETE(task);
					break;
				}
			}
		}
#endif

};

/**
 * \brief �߳����ص�����
 */
void zHttpThread::run()
{
#ifdef _USE_EPOLL_
	zHttpTaskContainer::iterator it, next;

	while(!isFinal())
	{
		mutex.lock();
		if (!tasks.empty())
		{
			int retcode = epoll_wait(kdpfd, &epfds[0], fds_count, 0);
			if (retcode > 0)
			{
				for(int i = 0; i < retcode; i++)
				{
					zHttpTask *task = (zHttpTask *)epfds[i].data.ptr;
					if (epfds[i].events & (EPOLLERR | EPOLLPRI))
					{
						//�׽ӿڳ��ִ���
						remove(task);
					}
					else if (epfds[i].events & EPOLLIN)
					{
						switch(task->httpCore())
						{
							case 1:		//���ճɹ�
							case -1:	//����ʧ��
								remove(task);
								break;
							case 0:		//���ճ�ʱ��
								break;
						}
					}
				}
			}

			currentTime.now();
			for(it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++)
			{
				zHttpTask *task = *it;
				if (task->checkHttpTimeout(currentTime))
				{
					//����ָ��ʱ����֤��û��ͨ������Ҫ��������
					remove(it);
				}
			}
		}
		mutex.unlock();

		zThread::msleep(50);
	}

	//�����еȴ���֤�����е����Ӽ��뵽���ն����У�������Щ����
	for(it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++)
	{
		remove(it);
	}
#else
	zHttpTaskContainer::iterator it, next;
	pollfdContainer::size_type i;

	while(!isFinal())
	{
		mutex.lock();
		if (!pfds.empty())
		{
			for(i = 0; i < pfds.size(); i++);
			{
				pfds[i].revents = 0;
			}

			if (TEMP_FAILURE_RETRY(::poll(&pfds[0], pfds.size(), 0)) > 0)
			{
				for(i = 0, it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++, i++)
				{
					zHttpTask *task = *it;
					if (pfds[i].revents & (POLLERR | POLLPRI))
					{
						//�׽ӿڳ��ִ���
						remove(it, i--);
					}
					else if (pfds[i].revents & POLLIN)
					{
						switch(task->httpCore())
						{
							case 1:		//���ճɹ�
							case -1:	//����ʧ��
								remove(it, i--);
								break;
							case 0:		//���ճ�ʱ��
								break;
						}
					}
				}
			}

			currentTime.now();
			for(i = 0, it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++, i++)
			{
				zHttpTask *task = *it;
				if (task->checkHttpTimeout(currentTime))
				{
					//����ָ��ʱ����֤��û��ͨ������Ҫ��������
					remove(it, i--);
				}
			}
		}
		mutex.unlock();

		zThread::msleep(50);
	}

	//�����еȴ���֤�����е����Ӽ��뵽���ն����У�������Щ����
	for(i = 0, it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++, i++)
	{
		remove(it, i--);
	}
#endif
}

/**
 * \brief ��һ��TCP������ӵ���֤�����У���Ϊ���ڶ����֤���У���Ҫ����һ�����㷨��ӵ���ͬ����֤���������
 * \param task һ����������
 */
bool zHttpTaskPool::addHttp(zHttpTask *task)
{
	//��Ϊ���ڶ����֤���У���Ҫ����һ�����㷨��ӵ���ͬ����֤���������
	static unsigned int hashcode = 0;
	zHttpThread *pHttpThread = (zHttpThread *)httpThreads.getByIndex(hashcode++ % maxHttpThreads);
	if (pHttpThread)
		pHttpThread->add(task);
	return true;
}

/**
 * \brief ��ʼ���̳߳أ�Ԥ�ȴ��������߳�
 * \return ��ʼ���Ƿ�ɹ�
 */
bool zHttpTaskPool::init()
{
	//������ʼ����֤�߳�
	for(int i = 0; i < maxHttpThreads; i++)
	{
		std::ostringstream name;
		name << "zHttpThread[" << i << "]";
		zHttpThread *pHttpThread = new zHttpThread(this, name.str());
		if (NULL == pHttpThread)
			return false;
		if (!pHttpThread->start())
			return false;
		httpThreads.add(pHttpThread);
	}

	return true;
}

/**
 * \brief �ͷ��̳߳أ��ͷŸ�����Դ���ȴ������߳��˳�
 */
void zHttpTaskPool::final()
{
	httpThreads.joinAll();
}

