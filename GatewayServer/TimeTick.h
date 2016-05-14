/**
 * \file
 * \version  $Id: TimeTick.h $
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
#include "zThread.h"
#include "zTime.h"

/**
 * \brief ���ض�ʱ���߳�
 *
 */
class GatewayTimeTick : public zThread
{

	public:

		static zRTime currentTime;

		~GatewayTimeTick() {};

		/**
		 * \brief �õ�Ψһʵ��
		 *
		 *
		 * \return Ψһʵ��
		 */
		static GatewayTimeTick &getInstance()
		{
			if (NULL == instance)
				instance = new GatewayTimeTick();

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

		static GatewayTimeTick *instance;

		GatewayTimeTick() : zThread("TimeTick"), one_second(1) {};

		Timer one_second;

};

#endif

