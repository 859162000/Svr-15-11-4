/**
 * \file
 * \version  $Id: TimeTick.h  $
 * \author  
 * \date 
 * \brief ʱ��ص�����
 *
 * 
 */


#ifndef _TimeTick_h_
#define _TimeTick_h_

#include <iostream>
#include <string>
#include <sys/timeb.h>

#include "Zebra.h"
#include "zThread.h"
#include "zTime.h"

class SuperTimeTick : public zThread
{

	public:

		~SuperTimeTick() {};

		static SuperTimeTick &getInstance()
		{
			if (NULL == instance)
				instance = new SuperTimeTick();

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void run();

	private:

		static zRTime currentTime;
		static SuperTimeTick *instance;

		zRTime startTime;
		QWORD qwStartGameTime;

		SuperTimeTick() : zThread("TimeTick"), startTime()
		{
			qwStartGameTime = 0;
		}

		bool readTime();
		bool saveTime();

};

#endif

