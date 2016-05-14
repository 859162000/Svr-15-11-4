/**
 * \file
 * \version  $Id: zebraclientService.h  $
 * \author  
 * \date 
 * \brief �����˿ͻ���������
 *
 * 
 */
#ifndef _ZEBRACLIENTSERVICE_H_
#define _ZEBRACLIENTSERVICE_H_

#include "zType.h"
#include "zebraclientService.h"
#include "zebraclient.h"
#include "loginClient.h"
#include "zSubNetService.h"
class zebraclientService : public zService
{

	public:

		~zebraclientService()
		{
		}

		static zebraclientService &getInstance()
		{
			if (NULL == instance)
				instance = new zebraclientService();

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 *
		 */
		static void delInstance()
		{
			if (instance)
			{
				delete instance;
				instance = NULL;
			}
		}

		bool init();
		bool serviceCallback();
		void final();
		
		/*
		bool test();
		
		bool createAcc();
		bool changePasswd();
		*/
		void setLoopRun()
		{
			loop = true;
		}
		bool loopRun();

	private:

		static zebraclientService *instance;

		zebraclientService() : zService("���Կͻ���")
		{
			loop = false;
		}

		bool loop;
		LoginClient *loginClient; 

};
#endif
