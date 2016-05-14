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
#include "zThread.h"
#include "zTime.h"

/**
 * \brief ʱ��ص�����
 */
class SceneTimeTick : public zThread
{

	public:

		/// ��ǰʱ��
		static zRTime currentTime;

		/**
		 * \brief ��������
		 */
		~SceneTimeTick() {};

		/**
		 * \brief ��ȡΨһʵ��
		 */
		static SceneTimeTick &getInstance()
		{
			if (NULL == instance)
				instance = new SceneTimeTick();

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void run();

	private:
		/// �����Ӽ�����
		Timer _five_sec;

		// һ���Ӽ�����
		Timer _one_min;

		// �������б�־
		bool quiz;

		/// Ψһʵ��
		static SceneTimeTick *instance;

		/**
		 * \brief ���캯��
		 */
		SceneTimeTick() : zThread("TimeTick"), _five_sec(5),_one_min(60),quiz(false) {};

};

#if 0
class My_FunctionTime
{
	private:
		struct My_Times
		{
			My_Times():_times(0),_total_time(0),_all_times(0),_all_total_time(0) {}
			unsigned long _times;			//���ô���
			unsigned long _total_time;		//����ʱ��
			unsigned long _all_times;		//�ܵ��ô���
			unsigned long _all_total_time;	//�ܵ���ʱ��
		};
		std::map<std::string, My_Times> _times; 
		Timer _log_timer;
	public:
		My_FunctionTime(const int how_long) : _log_timer(how_long) {}
		~My_FunctionTime() {}
		void inc(const std::string &func, const unsigned long total)
		{
			My_Times &mt = _times[func];
			if (mt._times)
			{
				//if (mt._total_time && mt._total_time * 10 <= total * mt._times)
				//	Zebra::logger->debug("[��ʱͳ��]���쳣��%s, %luus, %luus, %lu��", func.c_str(), mt._total_time, total * mt._times, mt._times);
				mt._times++;
				mt._total_time += total;
			}
			else
			{
				mt._times = 1;
				mt._total_time = total;
			}
			mt._all_times++;
			mt._all_total_time += total;
		}
		void reset(const zRTime &ct, const bool force_print)
		{
			if (force_print || _log_timer(ct))
			{
				Zebra::logger->debug("[��ʱͳ��]��%s, %u", force_print ? "force" : "timer", _times.size());
				for(std::map<std::string, My_Times>::iterator it = _times.begin(); it != _times.end(); ++it)
				{
					if (it->second._times)
					{
						Zebra::logger->debug("[��ʱͳ��]��%s, %luus, %lu��, %luus/��, %luus, %lu��, %luus/��",
								it->first.c_str(),
								it->second._total_time, it->second._times, it->second._total_time / it->second._times,
								it->second._all_total_time, it->second._all_times, it->second._all_times ? it->second._all_total_time / it->second._all_times : 0);
						it->second._times = 0;
					}
				}
			}
			else
			{
				for(std::map<std::string, My_Times>::iterator it = _times.begin(); it != _times.end(); ++it)
				{
					it->second._times = 0;
				}
			}
		}
};

class My_FunctionTime_wrapper
{
	private:
		const std::string _func;
		struct timeval _tv_1;
	public:
		static My_FunctionTime my_func;
		My_FunctionTime_wrapper(const std::string &func);
		~My_FunctionTime_wrapper();
};
#define __MY_FUNCTIONTIME_WRAPPER__
#else
#define My_FunctionTime_wrapper(func)
#endif

#endif

