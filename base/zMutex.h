/**
 * \file
 * \version  $Id: zMutex.h  $
 * \author  
 * \date 
 * \brief ������ķ�װ����Ҫ��Ϊ��ʹ�÷���
 *
 * 
 */

#ifndef _zMutex_h_
#define _zMutex_h_

#include <pthread.h>
#include <iostream>

#include "zNoncopyable.h"

/**
 * \brief �����壬��װ��ϵͳ�����壬������ʹ��ϵͳ������ʱ����Ҫ�ֹ���ʼ�������ٻ��������Ĳ���
 *
 */
class zMutex : private zNoncopyable
{

	friend class zCond;

	public:

		/**
		 * \brief ���캯��������һ�����������
		 *
		 */
		zMutex(int kind = PTHREAD_MUTEX_FAST_NP) 
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			pthread_mutexattr_t attr;
			::pthread_mutexattr_init(&attr);
			::pthread_mutexattr_settype(&attr, kind);
			::pthread_mutex_init(&mutex, &attr);
		}

		/**
		 * \brief ��������������һ�����������
		 *
		 */
		~zMutex()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_mutex_destroy(&mutex);
		}

		/**
		 * \brief ����һ��������
		 *
		 */
		inline void lock()
		{
			::pthread_mutex_lock(&mutex);
		}

		/**
		 * \brief ����һ��������
		 *
		 */
		inline void unlock()
		{
			::pthread_mutex_unlock(&mutex);
		}

	private:

		pthread_mutex_t mutex;		/**< ϵͳ������ */

};

/**
 * \brief Wrapper
 * �����ڸ��Ӻ���������ʹ��
 */
class zMutex_scope_lock : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯��
		 * ��������lock����
		 * \param m ��������
		 */
		zMutex_scope_lock(zMutex &m) : mlock(m)
		{
			mlock.lock();
		}

		/**
		 * \brief ��������
		 * ��������unlock����
		 */
		~zMutex_scope_lock()
		{
			mlock.unlock();
		}

	private:

		/**
		 * \brief ��������
		 */
		zMutex &mlock;

};

#endif

