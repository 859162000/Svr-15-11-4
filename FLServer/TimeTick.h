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

class FLTimeTick : public zThread
{

	public:

		~FLTimeTick() {};

		static FLTimeTick &getInstance()
		{
			if (NULL == instance)
				instance = new FLTimeTick();

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

		static FLTimeTick *instance;

		FLTimeTick() : zThread("TimeTick")
		{
		}

};

#endif

