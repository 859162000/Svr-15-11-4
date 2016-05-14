#include "zTimer.h"
#include <algorithm>

/**
 * \brief ��ʱ�����캯��
 *
 * \param name ��ʱ�����֣��������Ĭ��Ϊ"Timer"
 */
zTimer::zTimer(const std::string &name): taskqueue(),thread(name,&taskqueue)
{
	taskqueue.canAddTask=true;
	thread.start();
}

/**
 * \brief ��ʱ����������
 *
 */
zTimer::~zTimer()
{
	cancel();
}

int zTimer::id(0);

/**
 * \brief �õ�һ������ID ,�����Զ������ʱ������ 
 *
 * \return ID 
 */
int zTimer::getID()
{
	return (id++);
}

/**
 * \brief ͣ�������ʱ�����е�����
 *
 */
void zTimer::cancel()
{
	taskqueue.qmutex.lock();
	taskqueue.canAddTask=false;
	for(int i=0;i<taskqueue.size();i++)
	{
		taskqueue.queue[i]->cancel();
	}
	taskqueue.qmutex.unlock();
}

/**
 * \brief ���һ��һ��������
 * \param task Ҫִ�е����� 
 * \param tv �����ִ��ʱ�� 
 * \return ����Ƿ�ɹ�
 */
bool zTimer::schedule(zTimerTask *task, zRTime &tv)
{
	return addTask(task,tv,0);
}

/**
 * \brief ���һ��һ��������
 * \param task Ҫִ�е����� 
 * \param delay delay�����ִ������ 
 * \return ����Ƿ�ɹ�
 */
bool zTimer::schedule(zTimerTask *task, int delay)
{
	zRTime tv(delay);
	return addTask(task,tv,0);
}

/**
 * \brief ���һ����������,�ӳ�ģʽ
 *
 * �����ӳ�ģʽ��
 *
 * �ӳ�ģʽָ��һ��ִ������ʼ���ڶ���ִ������ʼʱ�ļ���ǹ̶��ģ�Ϊperiod
 * \param task Ҫִ�е����� 
 * \param delay delay�����ʼִ������ 
 * \param period ��������,��λ����
 * \return ����Ƿ�ɹ�
 */
bool zTimer::scheduleAtDelay(zTimerTask *task, int delay ,int period)
{
	if(delay < 0 || period < 0) return false;
	zRTime tv(delay);
	return addTask(task,tv,-period);
}

/**
 * \brief ���һ����������,�ӳ�ģʽ
 *
 * �����ӳ�ģʽ��
 *
 * �ӳ�ģʽָ��һ��ִ������ʼ���ڶ���ִ������ʼʱ�ļ���ǹ̶��ģ�Ϊperiod
 * \param task Ҫִ�е����� 
 * \param tv ����ʼִ�е�ʱ�� 
 * \param period ��������,��λ����
 * \return ����Ƿ�ɹ�
 */
bool zTimer::scheduleAtDelay(zTimerTask *task, zRTime &tv,int period)
{
	if(period<0) return false;
	return addTask(task,tv,-period);
}

/**
 * \brief ���һ����������,Ƶ��ģʽ
 *
 * ����Ƶ��ģʽ��
 *
 * Ƶ��ģʽָÿ��ִ�������ʱ��Ƶ���ǹ̶��ģ�Ϊperiod
 * \param task Ҫִ�е����� 
 * \param delay delay�����ʼִ������ 
 * \param period ��������,��λ����
 * \return ����Ƿ�ɹ�
 */
bool zTimer::scheduleAtRate(zTimerTask *task, int delay ,int period)
{
	if(delay < 0 || period < 0) return false;
	zRTime ctv(delay);
	return addTask(task,ctv,period);
}

/**
 * \brief ���һ����������,Ƶ��ģʽ
 *
 * ����Ƶ��ģʽ��
 *
 * Ƶ��ģʽָÿ��ִ�������ʱ��Ƶ���ǹ̶��ģ�Ϊperiod
 * \param task Ҫִ�е����� 
 * \param tv ����ʼִ�е�ʱ�� 
 * \param period ��������,��λ����
 * \return ����Ƿ�ɹ�
 */
bool zTimer::scheduleAtRate(zTimerTask *task, zRTime &tv,int period)
{
	if(period<0) return false;
	return addTask(task,tv,period);
}

/**
 * \brief �������
 *
 * \param task ����ָ�� 
 * \param tv �Ӻ�ʱ��ʼִ������ 
 * \param period ִ�����ڣ����Ϊ0��ʾһ��������
 * \return  �Ƿ���ӳɹ�
 */
bool zTimer::addTask(zTimerTask *task, zRTime &tv,int period)
{
	taskqueue.qmutex.lock();
	if(!taskqueue.canAddTask)
	{
		taskqueue.qmutex.unlock();
		return false;
	}

	if(task->state!=zTimerTask::VIRGIN)
	{
		taskqueue.qmutex.unlock();
		return false;
	}

	//��ʼ��timerTask
	task->nextExecTime=tv;
	task->period=period;
	task->state=zTimerTask::SCHEDULED;

	//���
	taskqueue.push(task);
	taskqueue.qmutex.unlock();
	return true;
}


/**
 * \brief �����߳���������
 *
 */
zTimer::zTimerThread::~zTimerThread()
{
}

/**
 * \brief �����߳���������
 *
 * \param name �߳����� 
 * \param queue ������� 
 */
zTimer::zTimerThread::zTimerThread(const std::string & name,zTimerQueue *queue)
:zThread(name,false)
{
	taskQueue=queue;
}

/**
 * \brief �μ� <code>zThread::run</code>
 *
 * ��ʱ��ִ�к������������ִ����ϣ�������Ӷ�����ɾ��
 */
void zTimer::zTimerThread::run()
{
	while(!isFinal())
	{

		taskQueue->qmutex.lock();
		if(taskQueue->empty() && !taskQueue->canAddTask)
		{
			taskQueue->qmutex.unlock();
			break;
		}

		if(taskQueue->empty())
		{
			taskQueue->qmutex.unlock();
			usleep(1000);
		}
		else
		{
			// �õ�����ִ�е�task
			zTimerTask *task=taskQueue->top();
			if(task==NULL)
			{
				taskQueue->pop();
				taskQueue->qmutex.unlock();
				continue;
			}

			task->taskMutex.lock();
			if(task->state==zTimerTask::CANCELLED)
			{
				taskQueue->pop();
				task->taskMutex.unlock();
				taskQueue->qmutex.unlock();
				continue;
			}
			zRTime ctv;
			zRTime etv(task->nextExecTime);
			bool taskFired=(ctv>etv);
			if(taskFired)//����������ִ����
			{
				if(task->period==0)
				{
					task->state=zTimerTask::EXECUTED;
					taskQueue->pop();
				}
				else
				{
					//�������д�����
					taskQueue->pop();
					//Zebra::logger->debug("%lu", task->period);
					task->schedExecTime(task->period>0?ctv:etv);
					taskQueue->push(task);
				}
			}

			task->taskMutex.unlock();
			taskQueue->qmutex.unlock();

			if(taskFired)
			{
				task->run();
			}
			else if(etv > ctv)
			{
				usleep(1000);
			}
		}
	}
}

/**
 * \brief ��ʱ�������캯��
 *
 */
zTimer::zTimerTask::zTimerTask():nextExecTime()
{
	period=0;
	state=VIRGIN;
}

/**
 * \brief ��ʱ��������������
 *
 */
zTimer::zTimerTask::~zTimerTask()
{
}

/**
 * \brief ���ز����� > ���Ƚ�����#zTimerTask��С
 *
 */
bool zTimer::zTimerTask::operator > (const zTimerTask &right) const
{
	return this->nextExecTime > right.nextExecTime;
}

/**
 * \brief ȡ���������
 * \return �ƻ����񱻳ɹ�ȡ��
 */
bool zTimer::zTimerTask::cancel()
{
	taskMutex.lock();
	bool result = (state == SCHEDULED);
	state = CANCELLED;
	taskMutex.unlock();
	return result;
}

/**
 * \brief �����趨����ִ��ʱ��
 *
 * \param tv ����ִ�е�ʱ��= tv + period 
 */
void zTimer::zTimerTask::schedExecTime(const zRTime &tv)
{
	nextExecTime = tv;
	nextExecTime.addDelay(abs(period));
}
