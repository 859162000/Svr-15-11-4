/**
 * \file
 * \version  $Id: TimeTick.h  $
 * \author  
 * \date 2
 * \brief ʱ��ص�����
 *
 * 
 */


#ifndef _TimeTick_h_
#define _TimeTick_h_

#include <iostream>
#include <string>

#include "Zebra.h"
#include "zTime.h"
#include "zThread.h"

class BillTimeTick : public zThread
{

	public:

		~BillTimeTick() {};

		/// ��ǰʱ��
		static zRTime currentTime;
		static Timer _one_min;
		static Timer _one_sec;
		static BillTimeTick &getInstance()
		{
			if (NULL == instance)
				instance = new BillTimeTick();

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

		static BillTimeTick *instance;

		BillTimeTick() : zThread("TimeTick") {};

};

#endif

