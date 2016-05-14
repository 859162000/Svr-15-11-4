/**
 * \file
 * \version  $Id: zCond.h  $
 * \author  
 * \date 
 * \brief ����zCond�࣬�򵥶�ϵͳ���������������з�װ
 *
 * 
 */


#ifndef _zCond_h_
#define _zCond_h_

#include <pthread.h>

#include "zNoncopyable.h"
#include "zMutex.h"

/**
 * \brief ��װ��ϵͳ����������ʹ����Ҫ�򵥣�ʡȥ���ֹ���ʼ��������ϵͳ���������Ĺ�������Щ�����������ɹ��캯���������������Զ����
 *
 */
class zCond : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯�������ڴ���һ����������
		 *
		 */
		zCond()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_cond_init(&cond, NULL);
		}

		/**
		 * \brief ������������������һ����������
		 *
		 */
		~zCond()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_cond_destroy(&cond);
		}

		/**
		 * \brief �����еȴ���������������̹߳㲥�����źţ�ʹ��Щ�߳��ܹ���������ִ��
		 *
		 */
		void broadcast()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_cond_broadcast(&cond);
		}

		/**
		 * \brief �����еȴ���������������̷߳����źţ�ʹ��Щ�߳��ܹ���������ִ��
		 *
		 */
		void signal()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_cond_signal(&cond);
		}

		/**
		 * \brief �ȴ��ض���������������
		 *
		 *
		 * \param mutex ��Ҫ�ȴ��Ļ�����
		 */
		void wait(zMutex &mutex)
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_cond_wait(&cond, &mutex.mutex);
		}

	private:

		pthread_cond_t cond;		/**< ϵͳ�������� */

};

#endif


