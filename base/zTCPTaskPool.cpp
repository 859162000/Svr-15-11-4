/**
 * \file
 * \version  $Id: zTCPTaskPool.cpp  $
 * \author  
 * \date 
 * \brief ʵ���̳߳��࣬���ڴ�������ӷ�����
 *
 * 
 */


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
#include <ext/pool_allocator.h>

#include "zSocket.h"
#include "zThread.h"
#include "zTCPTaskPool.h"
#include "Zebra.h"
#include "zTime.h"

int zTCPTaskPool::usleep_time=50000;										/**< ѭ���ȴ�ʱ�� */
/**
 * \brief ������������
 *
 */
typedef std::list<zTCPTask *, __gnu_cxx::__pool_alloc<zTCPTask *> > zTCPTaskContainer;

/**
 * \brief �����������������
 *
 */
typedef zTCPTaskContainer::iterator zTCPTask_IT;

#ifdef _USE_EPOLL_
typedef std::vector<struct epoll_event> epollfdContainer;
#else
typedef std::vector<struct pollfd> pollfdContainer;
#endif

class zTCPTaskQueue
{
	public:
		zTCPTaskQueue() :_size(0) {}
		virtual ~zTCPTaskQueue() {}
		inline void add(zTCPTask *task)
		{
			mlock.lock();
			_queue.push(task);
			_size++;
			mlock.unlock();
		}
		inline void check_queue()
		{
			mlock.lock();
			while(!_queue.empty())
			{
				zTCPTask *task = _queue.front();
				_queue.pop();
				_add(task);
			}
			_size = 0;
			mlock.unlock();
		}
	protected:
		virtual void _add(zTCPTask *task) = 0;
		unsigned int _size;
	private:
		zMutex mlock;
		std::queue<zTCPTask *, std::deque<zTCPTask *, __gnu_cxx::__pool_alloc<zTCPTask *> > > _queue;
};

/**
 * \brief ����TCP���ӵ���֤�������֤��ͨ������Ҫ�����������
 *
 */
class zVerifyThread : public zThread, public zTCPTaskQueue
{

	private:

		zTCPTaskPool *pool;
		zTCPTaskContainer tasks;	/**< �����б� */
		zTCPTaskContainer::size_type task_count;			/**< tasks����(��֤�̰߳�ȫ*/
#ifdef _USE_EPOLL_
		int kdpfd;
		epollfdContainer epfds;
#else
		pollfdContainer pfds;
#endif

		/**
		 * \brief ���һ����������
		 * \param task ��������
		 */
		void _add(zTCPTask *task)
		{
			Zebra::logger->trace("zVerifyThread::_add");
#ifdef _USE_EPOLL_
			task->addEpoll(kdpfd, EPOLLIN | EPOLLERR | EPOLLPRI, (void *)task);
			tasks.push_back(task);
			task_count = tasks.size();;
			if (task_count > epfds.size())
			{
				epfds.resize(task_count + 16);
			}
#else
			struct pollfd pfd;
			task->fillPollFD(pfd, POLLIN | POLLERR | POLLPRI);
			tasks.push_back(task);
			task_count = tasks.size();;
			pfds.push_back(pfd);
#endif
		}

#ifdef _USE_EPOLL_
		void remove(zTCPTask *task)
		{
			Zebra::logger->trace("zVerifyThread::remove(EPOLL)");
			task->delEpoll(kdpfd, EPOLLIN | EPOLLERR | EPOLLPRI);
			tasks.remove(task);
			task_count = tasks.size();
		}
		void remove(zTCPTask_IT &it)
		{
			(*it)->delEpoll(kdpfd, EPOLLIN | EPOLLERR | EPOLLPRI);
			tasks.erase(it);
			task_count = tasks.size();
		}
#else
		void remove(zTCPTask_IT &it, int p)
		{
			int i;
			pollfdContainer::iterator iter;
			for(iter = pfds.begin(), i = 0; iter != pfds.end(); iter++, i++)
			{
				if (i == p)
				{
					pfds.erase(iter);
					tasks.erase(it);
					task_count = tasks.size();
					break;
				}
			}
		}
#endif

	public:

		/**
		 * \brief ���캯��
		 * \param pool ���������ӳ�
		 * \param name �߳�����
		 */
		zVerifyThread(
				zTCPTaskPool *pool,
				const std::string &name = std::string("zVerifyThread"))
			: zThread(name), pool(pool)
		{
			task_count = 0;
#ifdef _USE_EPOLL_
			kdpfd = epoll_create(256);
			assert(-1 != kdpfd);
			epfds.resize(256);
#endif
		}

		/**
		 * \brief ��������
		 *
		 */
		~zVerifyThread()
		{
#ifdef _USE_EPOLL_
			TEMP_FAILURE_RETRY(::close(kdpfd));
#endif
		}

		void run();

};

/**
 * \brief �ȴ�������ָ֤���������֤
 *
 */
void zVerifyThread::run()
{
	Zebra::logger->trace("zVerifyThread::run");
#ifdef _USE_EPOLL_
	zRTime currentTime;
	zTCPTask_IT it, next;

	while(!isFinal())
	{
		currentTime.now();

		check_queue();
		if (!tasks.empty())
		{
			for(it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++)
			{
				zTCPTask *task = *it;
				if (task->checkVerifyTimeout(currentTime))
				{
					//����ָ��ʱ����֤��û��ͨ������Ҫ��������
					remove(it);
					task->resetState();
					pool->addRecycle(task);
				}
			}

			int retcode = epoll_wait(kdpfd, &epfds[0], task_count, 0);
			if (retcode > 0)
			{
				for(int i = 0; i < retcode; i++)
				{
					zTCPTask *task = (zTCPTask *)epfds[i].data.ptr;
					if (epfds[i].events & (EPOLLERR | EPOLLPRI))
					{
						//�׽ӿڳ��ִ���
						Zebra::logger->error("�׽ӿڴ���");
						remove(task);
						task->resetState();
						pool->addRecycle(task);
					}
					else if (epfds[i].events & EPOLLIN)
					{
						switch(task->verifyConn())
						{
							case 1:
								//��֤�ɹ�
								remove(task);
								//����Ψһ����֤
								if (task->uniqueAdd())
								{
									//Ψһ����֤�ɹ�����ȡ��һ��״̬
									Zebra::logger->debug("�ͻ���Ψһ����֤�ɹ�(%s:%u)",task->getIP(),task->getPort());
									task->setUnique();
									pool->addSync(task);
								}
								else
								{
									//Ψһ����֤ʧ�ܣ�������������
									Zebra::logger->debug("�ͻ���Ψһ����֤ʧ��(%s:%u)",task->getIP(),task->getPort());
									task->resetState();
									pool->addRecycle(task);
								}
								break;
							case 0:
								//��ʱ������ᴦ��
								break;
							case -1:
								//��֤ʧ�ܣ���������
								Zebra::logger->debug("�ͻ���������֤ʧ��(%s:%u)",task->getIP(),task->getPort());
								remove(task);
								task->resetState();
								pool->addRecycle(task);
								break;
						}
					}
				}
			}
		}

		zThread::msleep(50);
	}

	//�����еȴ���֤�����е����Ӽ��뵽���ն����У�������Щ����
	for(it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++)
	{
		zTCPTask *task = *it;
		remove(it);
		task->resetState();
		pool->addRecycle(task);
	}
#else
	zRTime currentTime;
	zTCPTask_IT it, next;
	pollfdContainer::size_type i;

	while(!isFinal())
	{
		currentTime.now();

		check_queue();
		if (!pfds.empty())
		{
			for(i = 0, it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++, i++)
			{
				zTCPTask *task = *it;
				if (task->checkVerifyTimeout(currentTime))
				{
					//����ָ��ʱ����֤��û��ͨ������Ҫ��������
					remove(it, i--);
					task->resetState();
					pool->addRecycle(task);
				}
			}

			for(i = 0; i < pfds.size(); i++);
			{
				pfds[i].revents = 0;
			}

			if (TEMP_FAILURE_RETRY(::poll(&pfds[0], pfds.size(), 0)) > 0)
			{
				for(i = 0, it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++, i++)
				{
					zTCPTask *task = *it;
					if (pfds[i].revents & (POLLERR | POLLPRI))
					{
						//�׽ӿڳ��ִ���
						remove(it, i--);
						task->resetState();
						pool->addRecycle(task);
					}
					else if (pfds[i].revents & POLLIN)
					{
						switch(task->verifyConn())
						{
							case 1:
								//��֤�ɹ�
								remove(it, i--);
								//����Ψһ����֤
								if (task->uniqueAdd())
								{
									//Ψһ����֤�ɹ�����ȡ��һ��״̬
									Zebra::logger->debug("�ͻ���Ψһ����֤�ɹ�");
									task->setUnique();
									pool->addSync(task);
								}
								else
								{
									//Ψһ����֤ʧ�ܣ�������������
									Zebra::logger->debug("�ͻ���Ψһ����֤ʧ��");
									task->resetState();
									pool->addRecycle(task);
								}
								break;
							case 0:
								//��ʱ������ᴦ��
								break;
							case -1:
								//��֤ʧ�ܣ���������
								remove(it, i--);
								task->resetState();
								pool->addRecycle(task);
								break;
						}
					}
				}
			}
		}

		zThread::msleep(50);
	}

	//�����еȴ���֤�����е����Ӽ��뵽���ն����У�������Щ����
	for(i = 0, it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++, i++)
	{
		zTCPTask *task = *it;
		remove(it, i--);
		task->resetState();
		pool->addRecycle(task);
	}
#endif
}

/**
 * \brief �ȴ������߳�ͬ����֤������ӣ����ʧ�ܻ��߳�ʱ������Ҫ��������
 *
 */
class zSyncThread : public zThread, public zTCPTaskQueue
{

	private:

		zTCPTaskPool *pool;
		zTCPTaskContainer tasks;	/**< �����б� */

		void _add(zTCPTask *task)
		{
			tasks.push_front(task);
		}

	public:

		/**
		 * \brief ���캯��
		 * \param pool ���������ӳ�
		 * \param name �߳�����
		 */
		zSyncThread(
				zTCPTaskPool *pool,
				const std::string &name = std::string("zSyncThread"))
			: zThread(name), pool(pool)
			{}

		/**
		 * \brief ��������
		 *
		 */
		~zSyncThread() {};

		void run();

};

/**
 * \brief �ȴ������߳�ͬ����֤�������
 *
 */
void zSyncThread::run()
{
	Zebra::logger->trace("zSyncThread::run");
	zTCPTask_IT it;

	while(!isFinal())
	{
		check_queue();

		if (!tasks.empty())
		{
			for(it = tasks.begin(); it != tasks.end();)
			{
				zTCPTask *task = *it;
				switch(task->waitSync())
				{
					case 1:
						//�ȴ������߳�ͬ����֤�ɹ�
						it = tasks.erase(it);
						if (!pool->addOkay(task))
						{
							task->resetState();
							pool->addRecycle(task);
						}
						break;
					case 0:
						it++;
						break;
					case -1:
						//�ȴ������߳�ͬ����֤ʧ�ܣ���Ҫ��������
						it = tasks.erase(it);
						task->resetState();
						pool->addRecycle(task);
						break;
				}
			}
		}

		zThread::msleep(200);
	}

	//�����еȴ�ͬ����֤�����е����Ӽ��뵽���ն����У�������Щ����
	for(it = tasks.begin(); it != tasks.end();)
	{
		zTCPTask *task = *it;
		it = tasks.erase(it);
		task->resetState();
		pool->addRecycle(task);
	}
}

/**
 * \brief TCP���ӵ��������̣߳�һ��һ���̴߳�����TCP���ӣ����������������Ч��
 *
 */
class zOkayThread : public zThread, public zTCPTaskQueue
{

	private:

		zTCPTaskPool *pool;
		zTCPTaskContainer tasks;	/**< �����б� */
		zTCPTaskContainer::size_type task_count;			/**< tasks����(��֤�̰߳�ȫ*/
#ifdef _USE_EPOLL_
		int kdpfd;
		epollfdContainer epfds;
#else
		pollfdContainer pfds;
#endif

		void _add(zTCPTask *task)
		{
#ifdef _USE_EPOLL_
			task->addEpoll(kdpfd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLPRI, (void *)task);
			tasks.push_back(task);
			task_count = tasks.size();
			if (task_count > epfds.size())
			{
				epfds.resize(task_count + 16);
			}
#else
			struct pollfd pfd;
			task->fillPollFD(pfd, POLLIN | POLLOUT | POLLERR | POLLPRI);
			tasks.push_back(task);
			task_count = tasks.size();
			pfds.push_back(pfd);
#endif
			task->ListeningRecv(false);
		}

#ifdef _USE_EPOLL_
		void remove(zTCPTask_IT &it)
		{
			(*it)->delEpoll(kdpfd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLPRI);
			tasks.erase(it);
			task_count = tasks.size();
		}
#else
		void remove(zTCPTask_IT &it, int p)
		{
			int i;
			pollfdContainer::iterator iter;
			for(iter = pfds.begin(), i = 0; iter != pfds.end(); iter++, i++)
			{
				if (i == p)
				{
					pfds.erase(iter);
					tasks.erase(it);
					task_count = tasks.size();
					break;
				}
			}
		}
#endif

	public:

		static const zTCPTaskContainer::size_type connPerThread = 512;	/**< ÿ���̴߳����������� */

		/**
		 * \brief ���캯��
		 * \param pool ���������ӳ�
		 * \param name �߳�����
		 */
		zOkayThread(
				zTCPTaskPool *pool,
				const std::string &name = std::string("zOkayThread"))
			: zThread(name), pool(pool)
		{
			task_count = 0;
#ifdef _USE_EPOLL_
			kdpfd = epoll_create(connPerThread);
			assert(-1 != kdpfd);
			epfds.resize(connPerThread);
#endif
		}

		/**
		 * \brief ��������
		 *
		 */
		~zOkayThread()
		{
#ifdef _USE_EPOLL_
			TEMP_FAILURE_RETRY(::close(kdpfd));
#endif
		}

		void run();

		/**
		 * \brief ������������ĸ���
		 * \return ����̴߳��������������
		 */
		const zTCPTaskContainer::size_type size() const
		{
			return task_count + _size;
		}

};

/**
 * \brief �������̣߳��ص��������ӵ��������ָ��
 *
 */
void zOkayThread::run()
{
	Zebra::logger->trace("zOkayThread::run");
#ifdef _USE_EPOLL_
	zRTime currentTime;
	zTCPTask_IT it, next;

	int kdpfd_r;
	epollfdContainer epfds_r;
	kdpfd_r = epoll_create(256);
	assert(-1 != kdpfd_r);
	epfds.resize(256);
	DWORD fds_count_r = 0;
	int time = pool->usleep_time;
	bool check=false;
	while(!isFinal())
	{
		currentTime.now();

		if (check) 
		{
			check_queue();
			if (!tasks.empty())
			{
				for(it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++)
				{
					zTCPTask *task = *it;

					//�������ź�ָ��
					task->checkSignal(currentTime);

					if (task->isTerminateWait())
					{
						task->Terminate();
					}
					if (task->isTerminate())
					{
						if (task->isFdsrAdd())
						{
							task->delEpoll(kdpfd_r, EPOLLIN | EPOLLERR | EPOLLPRI);
							fds_count_r --;
						}
						remove(it);
						// state_sync -> state_okay
						/*
						 * whj
						 * ������״̬���������,
						 * ����ᵼ��һ��taskͬʱ�������߳��е�Σ�����
						 */
						task->getNextState();
						pool->addRecycle(task);
					}
					else
					{
						if(!task->isFdsrAdd())
						{
							task->addEpoll(kdpfd_r, EPOLLIN | EPOLLERR | EPOLLPRI, (void *)task);
							task->fdsrAdd(); 
							fds_count_r++;
							if (fds_count_r > epfds_r.size())
							{
								epfds_r.resize(fds_count_r + 16);
							}
						}
					}
				}
			}
			check=false;
		}

		if(fds_count_r)
		{
			struct timeval _tv_1;
			struct timeval _tv_2;
			gettimeofday(&_tv_1,NULL);
			int retcode = epoll_wait(kdpfd_r, &epfds_r[0], fds_count_r, time/1000);
			if (retcode > 0)
			{
				for(int i = 0; i < retcode; i++)
				{
					zTCPTask *task = (zTCPTask *)epfds_r[i].data.ptr;
					if (epfds_r[i].events & (EPOLLERR | EPOLLPRI))
					{
						//�׽ӿڳ��ִ���
						Zebra::logger->debug("%s: �׽ӿ��쳣����", __PRETTY_FUNCTION__);
						task->Terminate(zTCPTask::terminate_active);
						check=true;
					}
					else
					{
						if (epfds_r[i].events & EPOLLIN)
						{
							//�׽ӿ�׼�����˶�ȡ����
							if (!task->ListeningRecv(true))
							{
								Zebra::logger->debug("%s: �׽ӿڶ���������", __PRETTY_FUNCTION__);
								task->Terminate(zTCPTask::terminate_active);
								check=true;
							}
						}
					}
					epfds_r[i].events=0; 
				}
			}
			gettimeofday(&_tv_2,NULL);
			int end=_tv_2.tv_sec*1000000 + _tv_2.tv_usec;
			int begin= _tv_1.tv_sec*1000000 + _tv_1.tv_usec;
			time = time - (end - begin);
		}
		else
		{
			zThread::usleep(time);
			time = 0;
		}
		if(check)
		{
			if(time <=0)
			{
				time = 0;
			}
			continue;
		}
		if (time <=0)
		{
			if (!tasks.empty())
			{
				int retcode = epoll_wait(kdpfd, &epfds[0], task_count, 0);
				if (retcode > 0)
				{
					for(int i = 0; i < retcode; i++)
					{
						zTCPTask *task = (zTCPTask *)epfds[i].data.ptr;
						if (epfds[i].events & (EPOLLERR | EPOLLPRI))
						{
							//�׽ӿڳ��ִ���
							Zebra::logger->debug("%s: �׽ӿ��쳣����", __PRETTY_FUNCTION__);
							task->Terminate(zTCPTask::terminate_active);
						}
						else
						{
							if (epfds[i].events & EPOLLIN)
							{
								//�׽ӿ�׼�����˶�ȡ����
								if (!task->ListeningRecv(true))
								{
									Zebra::logger->debug("%s: �׽ӿڶ���������", __PRETTY_FUNCTION__);
									task->Terminate(zTCPTask::terminate_active);
								}
							}
							if (epfds[i].events & EPOLLOUT)
							{
								//�׽ӿ�׼������д�����
								if (!task->ListeningSend())
								{
									Zebra::logger->debug("%s: �׽ӿ�д��������", __PRETTY_FUNCTION__);
									task->Terminate(zTCPTask::terminate_active);
								}
							}
						}
						epfds[i].events=0; 
					}
				}
				time = pool->usleep_time;
			}
			check=true;
		}

		//zThread::usleep(pool->usleep_time);
	}

	//��������������е����Ӽ��뵽���ն����У�������Щ����
	for(it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++)
	{
		zTCPTask *task = *it;
		remove(it);
		// state_sync -> state_okay
		/*
		 * whj
		 * ������״̬���������,
		 * ����ᵼ��һ��taskͬʱ�������߳��е�Σ�����
		 */
		task->getNextState();
		pool->addRecycle(task);
	}
	TEMP_FAILURE_RETRY(::close(kdpfd_r));
#else
	zRTime currentTime;
	zTCPTask_IT it, next;
	pollfdContainer::size_type i;

	int time = pool->usleep_time;
	pollfdContainer::iterator iter_r;
	pollfdContainer pfds_r;
	zTCPTaskContainer tasks_r;    
	bool check=false;
	while(!isFinal())
	{
		currentTime.now();

		check_queue();
		if(check)
		{
			if (!pfds.empty())
			{
				for(i = 0, it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++, i++)
				{
					zTCPTask *task = *it;
					//�������ź�ָ��
					task->checkSignal(currentTime);

					if (task->isTerminateWait())
					{
						task->Terminate();
					}
					if (task->isTerminate())
					{
						unsigned int j=0;
						for(iter_r = pfds_r.begin(); iter_r != pfds_r.end(); iter_r++, j++)
						{
							if (i == j)
							{
								pfds_r.erase(iter_r);
								//zTCPTaskContainer::iterator iter_del = tasks_r.find(task);
								//if(iter_del)
								//tasks_r.erase(iter_del);
								tasks_r.remove(task);
								break;
							}
						}
						remove(it, i--);
						// state_sync -> state_okay
						/*
						 * whj
						 * ������״̬���������,
						 * ����ᵼ��һ��taskͬʱ�������߳��е�Σ�����
						 */
						task->getNextState();
						pool->addRecycle(task);
					}
					else
					{
						pfds[i].revents = 0;
					}
				}
			}
			check=false;
		}

		if(!tasks_r.empty())
		{
			struct timeval _tv_1;
			struct timeval _tv_2;
			gettimeofday(&_tv_1,NULL);
			int retcode = TEMP_FAILURE_RETRY(::poll(&pfds_r[0], pfds_r.size(), time/1000));
			if (retcode > 0)
			{
				for(i = 0, it = tasks_r.begin(); it != tasks_r.end(); it++, i++)
				{
					zTCPTask *task = *it;
					if (pfds_r[i].revents & (POLLERR | POLLPRI))
					{
						//�׽ӿڳ��ִ���
						Zebra::logger->debug("%s: �׽ӿ��쳣����", __PRETTY_FUNCTION__);
						task->Terminate(zTCPTask::terminate_active);
						check=true;
					}
					else
					{
						if (pfds_r[i].revents & POLLIN)
						{
							//�׽ӿ�׼�����˶�ȡ����
							if (!task->ListeningRecv(true))
							{
								Zebra::logger->debug("%s: �׽ӿڶ���������", __PRETTY_FUNCTION__);
								task->Terminate(zTCPTask::terminate_active);
								check=true;
							}
						}
					}
					pfds_r[i].revents=0; 
				}
			}
			gettimeofday(&_tv_2,NULL);
			int end=_tv_2.tv_sec*1000000 + _tv_2.tv_usec;
			int begin= _tv_1.tv_sec*1000000 + _tv_1.tv_usec;
			time = time - (end - begin);
		}
		else
		{
			zThread::usleep(time);
			time = 0;
		}
		if(check)
		{
			if(time <=0)
			{
				time = 0;
			}
			continue;
		}
		if(time <=0/* && !tasks.empty()*/)
		{
			if (!pfds.empty())
			{
				if (TEMP_FAILURE_RETRY(::poll(&pfds[0], pfds.size(), 0)) > 0)
				{
					for(i = 0, it = tasks.begin(); it != tasks.end(); it++, i++)
					{
						zTCPTask *task = *it;
						if (pfds[i].revents & (POLLERR | POLLPRI))
						{
							//�׽ӿڳ��ִ���
							Zebra::logger->debug("%s: �׽ӿ��쳣����", __PRETTY_FUNCTION__);
							task->Terminate(zTCPTask::terminate_active);
						}
						else
						{
							if (pfds[i].revents & POLLIN)
							{
								//�׽ӿ�׼�����˶�ȡ����
								if (!task->ListeningRecv(true))
								{
									Zebra::logger->debug("%s: �׽ӿڶ���������", __PRETTY_FUNCTION__);
									task->Terminate(zTCPTask::terminate_active);
								}
							}
							if (pfds[i].revents & POLLOUT)
							{
								//�׽ӿ�׼������д�����
								if (!task->ListeningSend())
								{
									Zebra::logger->debug("%s: �׽ӿ�д��������", __PRETTY_FUNCTION__);
									task->Terminate(zTCPTask::terminate_active);
								}
							}
						}
						pfds[i].revents=0; 
					}
				}
			}
			time = pool->usleep_time;
		}
		check=true;

		//zThread::usleep(pool->usleep_time);
	}

	//��������������е����Ӽ��뵽���ն����У�������Щ����
	for(i = 0, it = tasks.begin(), next = it, next++; it != tasks.end(); it = next, next++, i++)
	{
		zTCPTask *task = *it;
		remove(it, i--);
		// state_sync -> state_okay
		/*
		 * whj
		 * ������״̬���������,
		 * ����ᵼ��һ��taskͬʱ�������߳��е�Σ�����
		 */
		task->getNextState();
		pool->addRecycle(task);
	}
#endif
}

/**
 * \brief ���ӻ����̣߳������������õ�TCP���ӣ��ͷ���Ӧ����Դ
 *
 */
class zRecycleThread : public zThread, public zTCPTaskQueue
{

	private:

		zTCPTaskPool *pool;
		zTCPTaskContainer tasks;	/**< �����б� */

		void _add(zTCPTask *task)
		{
			tasks.push_front(task);
		}

	public:

		/**
		 * \brief ���캯��
		 * \param pool ���������ӳ�
		 * \param name �߳�����
		 */
		zRecycleThread(
				zTCPTaskPool *pool,
				const std::string &name = std::string("zRecycleThread"))
			: zThread(name), pool(pool)
			{}

		/**
		 * \brief ��������
		 *
		 */
		~zRecycleThread() {};

		void run();

};

/**
 * \brief ���ӻ��մ����̣߳���ɾ���ڴ�ռ�֮ǰ��Ҫ��֤recycleConn����1
 *
 */
void zRecycleThread::run()
{
	Zebra::logger->trace("zRecycleThread::run");
	zTCPTask_IT it;

	while(!isFinal())
	{
		check_queue();

		if (!tasks.empty())
		{
			for(it = tasks.begin(); it != tasks.end();)
			{
				zTCPTask *task = *it;
				switch(task->recycleConn())
				{
					case 1:
						//���մ�����ɿ����ͷ���Ӧ����Դ
						it = tasks.erase(it);
						if (task->isUnique())
							//����Ѿ�ͨ����Ψһ����֤����ȫ��Ψһ������ɾ��
							task->uniqueRemove();
						task->getNextState();
						SAFE_DELETE(task);
						break;
					case 0:
						//���ճ�ʱ���´��ٴ���
						it++;
						break;
				}
			}
		}

		zThread::msleep(200);
	}

	//�������е�����
	for(it = tasks.begin(); it != tasks.end();)
	{
		//���մ�����ɿ����ͷ���Ӧ����Դ
		zTCPTask *task = *it;
		it = tasks.erase(it);
		if (task->isUnique())
			//����Ѿ�ͨ����Ψһ����֤����ȫ��Ψһ������ɾ��
			task->uniqueRemove();
		task->getNextState();
		SAFE_DELETE(task);
	}
}


/**
 * \brief �������ӳ��������Ӹ���
 *
 */
const int zTCPTaskPool::getSize()
{
	Zebra::logger->trace("zTCPTaskPool::getSize");
	struct MyCallback : zThreadGroup::Callback
	{
		int size;
		MyCallback() : size(0) {}
		void exec(zThread *e)
		{
			zOkayThread *pOkayThread = (zOkayThread *)e;
			size += pOkayThread->size();
		}
	};
	MyCallback mcb;
	okayThreads.execAll(mcb);
	return mcb.size;
}

/**
 * \brief ��һ��TCP������ӵ���֤�����У���Ϊ���ڶ����֤���У���Ҫ����һ�����㷨��ӵ���ͬ����֤���������
 *
 * \param task һ����������
 */
bool zTCPTaskPool::addVerify(zTCPTask *task)
{

	Zebra::logger->trace("zTCPTaskPool::addVerify");
	//��Ϊ���ڶ����֤���У���Ҫ����һ�����㷨��ӵ���ͬ����֤���������
	static unsigned int hashcode = 0;
	zVerifyThread *pVerifyThread = (zVerifyThread *)verifyThreads.getByIndex(hashcode++ % maxVerifyThreads);
	if (pVerifyThread)
	{
		// state_sync -> state_okay
		/*
		 * whj
		 * ������״̬���������,
		 * ����ᵼ��һ��taskͬʱ�������߳��е�Σ�����
		 */
		task->getNextState();
		pVerifyThread->add(task);
	}
	return true;
}

/**
 * \brief ��һ��ͨ����֤��TCP������ӵ��ȴ�ͬ����֤������
 *
 * \param task һ����������
 */
void zTCPTaskPool::addSync(zTCPTask *task)
{
	Zebra::logger->trace("zTCPTaskPool::addSync");
	// state_sync -> state_okay
	/*
	 * whj
	 * ������״̬���������,
	 * ����ᵼ��һ��taskͬʱ�������߳��е�Σ�����
	 */
	task->getNextState();
	syncThread->add(task);
}

/**
 * \brief ��һ��ͨ����֤��TCP���������
 *
 * \param task һ����������
 * \return ����Ƿ�ɹ�
 */
bool zTCPTaskPool::addOkay(zTCPTask *task)
{
	Zebra::logger->trace("zTCPTaskPool::addOkay");
	//���ȱ������е��̣߳��ҳ����еĲ������������ٵ��̣߳����ҳ�û���������߳�
	zOkayThread *min = NULL, *nostart = NULL;
	for(int i = 0; i < maxThreadCount; i++)
	{
		zOkayThread *pOkayThread = (zOkayThread *)okayThreads.getByIndex(i);
		if (pOkayThread)
		{
			if (pOkayThread->isAlive())
			{
				if (NULL == min || min->size() > pOkayThread->size())
					min = pOkayThread;
			}
			else
			{
				nostart = pOkayThread;
				break;
			}
		}
	}
	if (min && min->size() < zOkayThread::connPerThread)
	{
		// state_sync -> state_okay
		/*
		 * whj
		 * ������״̬���������,
		 * ����ᵼ��һ��taskͬʱ�������߳��е�Σ�����
		 */
		task->getNextState();
		//����߳�ͬʱ�������������û�е�������
		min->add(task);
		return true;
	}
	if (nostart)
	{
		//�̻߳�û�����У���Ҫ�����̣߳��ٰ���ӵ�����̵߳Ĵ��������
		if (nostart->start())
		{
			Zebra::logger->debug("zTCPTaskPool���������߳�");
			// state_sync -> state_okay
			/*
			 * whj
			 * ������״̬���������,
			 * ����ᵼ��һ��taskͬʱ�������߳��е�Σ�����
			 */
			task->getNextState();
			//����߳�ͬʱ�������������û�е�������
			nostart->add(task);
			return true;
		}
		else
			Zebra::logger->fatal("zTCPTaskPool���ܴ��������߳�");
	}

	Zebra::logger->fatal("zTCPTaskPoolû���ҵ����ʵ��߳�����������");
	//û���ҵ��߳�������������ӣ���Ҫ���չر�����
	return false;
}

/**
 * \brief ��һ��TCP������ӵ����մ��������
 *
 * \param task һ����������
 */
void zTCPTaskPool::addRecycle(zTCPTask *task)
{
	Zebra::logger->trace("zTCPTaskPool::addRecycle");
	recycleThread->add(task);
}


/**
 * \brief ��ʼ���̳߳أ�Ԥ�ȴ��������߳�
 *
 * \return ��ʼ���Ƿ�ɹ�
 */
bool zTCPTaskPool::init()
{
	Zebra::logger->trace("zTCPTaskPool::init");
	//������ʼ����֤�߳�
	for(int i = 0; i < maxVerifyThreads; i++)
	{
		std::ostringstream name;
		name << "zVerifyThread[" << i << "]";
		zVerifyThread *pVerifyThread = new zVerifyThread(this, name.str());
		if (NULL == pVerifyThread)
			return false;
		if (!pVerifyThread->start())
			return false;
		verifyThreads.add(pVerifyThread);
	}

	//������ʼ���ȴ�ͬ����֤�ֳ�
	syncThread = new zSyncThread(this);
	if (syncThread && !syncThread->start())
		return false;

	//������ʼ���������̳߳�
	maxThreadCount = (maxConns + zOkayThread::connPerThread - 1) / zOkayThread::connPerThread;
	Zebra::logger->debug("�߳��������������%d,ÿ�߳���������%d,�̸߳���%d",maxConns,zOkayThread::connPerThread,maxThreadCount);
	for(int i = 0; i < maxThreadCount; i++)
	{
		std::ostringstream name;
		name << "zOkayThread[" << i << "]";
		zOkayThread *pOkayThread = new zOkayThread(this, name.str());
		if (NULL == pOkayThread)
			return false;
		if (i < minThreadCount && !pOkayThread->start())
			return false;
		okayThreads.add(pOkayThread);
	}

	//������ʼ�������̳߳�
	recycleThread = new zRecycleThread(this);
	if (recycleThread && !recycleThread->start())
		return false;

	return true;
}

/**
 * \brief �ͷ��̳߳أ��ͷŸ�����Դ���ȴ������߳��˳�
 *
 */
void zTCPTaskPool::final()
{
	Zebra::logger->trace("zTCPTaskPool::final");	
	verifyThreads.joinAll();

	if (syncThread)
	{
		syncThread->final();
		syncThread->join();
		SAFE_DELETE(syncThread);
	}

	okayThreads.joinAll();

	if (recycleThread)
	{
		recycleThread->final();
		recycleThread->join();
		SAFE_DELETE(recycleThread);
	}
}

