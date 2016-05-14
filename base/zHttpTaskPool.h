/**
 * \file
 * \version  $Id: zHttpTaskPool.h  $
 * \author  
 * \date 
 * \brief ����ʵ��������(lightweight)��http������
 */


#ifndef _zHttpTaskPool_h_
#define _zHttpTaskPool_h_

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <unistd.h>
#include <sys/timeb.h>

#include "zSocket.h"
#include "zThread.h"
#include "zHttpTask.h"
#include "Zebra.h"
#include "zString.h"

/**
 * \brief ����ʵ��������(lightweight)��http��������
 */
class zHttpTaskPool : private zNoncopyable
{

	public:

		/**
		 * \brief ���캯��
		 */
		zHttpTaskPool()
		{
		};

		/**
		 * \brief ��������������һ���̳߳ض���
		 *
		 */
		~zHttpTaskPool()
		{
			final();
		}

		bool addHttp(zHttpTask *task);
		bool init();
		void final();

	private:

		static const int maxHttpThreads = 8;					/**< �����֤�߳����� */
		zThreadGroup httpThreads;								/**< http�������߳��� */

};

#endif
