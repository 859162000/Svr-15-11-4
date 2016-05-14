/**
 * \file
 * \version  $Id: zRWLock.h  $
 * \author  
 * \date 
 * \brief ����zRWLock�࣬�򵥶�ϵͳ��д���������з�װ
 *
 * 
 */


#ifndef _zRWLock_h_
#define _zRWLock_h_

#include <pthread.h>

#include "zNoncopyable.h"

/**
 * \brief ��װ��ϵͳ��д����ʹ����Ҫ�򵥣�ʡȥ���ֹ���ʼ��������ϵͳ��д���Ĺ�������Щ�����������ɹ��캯���������������Զ����
 *
 */
class zRWLock : private zNoncopyable
{

	public:
		//��д���������
		//unsigned int rd_count;
		//unsigned int wr_count;

		/**
		 * \brief ���캯�������ڴ���һ����д��
		 *
		 */
		zRWLock()//:rd_count(0),wr_count(0)
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_rwlock_init(&rwlock, NULL);
		}

		/**
		 * \brief ������������������һ����д��
		 *
		 */
		~zRWLock()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_rwlock_destroy(&rwlock);
		}

		/**
		 * \brief �Զ�д�����ж���������
		 *
		 */
		inline void rdlock()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_rwlock_rdlock(&rwlock);
			//++rd_count;
		};

		/**
		 * \brief �Զ�д������д��������
		 *
		 */
		inline void wrlock()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_rwlock_wrlock(&rwlock);
			//++wr_count;
			//++rd_count;
		}

		/**
		 * \brief �Զ�д�����н�������
		 *
		 */
		inline void unlock()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_rwlock_unlock(&rwlock);
			//--rd_count;
		}

	private:

		pthread_rwlock_t rwlock;		/**< ϵͳ��д�� */

};

/**
 * \brief rdlock Wrapper
 * �����ڸ��Ӻ����ж�д����ʹ��
 */
class zRWLock_scope_rdlock : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯��
		 * ��������rdlock����
		 * \param m ��������
		 */
		zRWLock_scope_rdlock(zRWLock &m) : rwlock(m)
		{
			rwlock.rdlock();
		}

		/**
		 * \brief ��������
		 * ��������unlock����
		 */
		~zRWLock_scope_rdlock()
		{
			rwlock.unlock();
		}

	private:

		/**
		 * \brief ��������
		 */
		zRWLock &rwlock;

};

/**
 * \brief wrlock Wrapper
 * �����ڸ��Ӻ����ж�д����ʹ��
 */
class zRWLock_scope_wrlock : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯��
		 * ��������wrlock����
		 * \param m ��������
		 */
		zRWLock_scope_wrlock(zRWLock &m) : rwlock(m)
		{
			rwlock.wrlock();
		}

		/**
		 * \brief ��������
		 * ��������unlock����
		 */
		~zRWLock_scope_wrlock()
		{
			rwlock.unlock();
		}

	private:

		/**
		 * \brief ��������
		 */
		zRWLock &rwlock;

};

#endif

