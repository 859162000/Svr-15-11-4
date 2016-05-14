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

#include "Zebra.h"
#include "zTime.h"
#include "zThread.h"

class MiniTimeTick : public zThread
{

	public:

		~MiniTimeTick() {};

		/// ��ǰʱ��
		static zRTime currentTime;
		static Timer _1_min;
		static MiniTimeTick &getInstance()
		{
			if (NULL == instance)
				instance = new MiniTimeTick();

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

		static MiniTimeTick *instance;

		MiniTimeTick() : zThread("TimeTick") {};

};

#endif

