/**
 * \file
 * \version  $Id: zTime.h 5751 z $
 * \author  
 * \date 
 * \brief ʱ�䶨��
 *
 * 
 */

#ifndef _ZTIME_H_
#define _ZTIME_H_

#include <time.h>
#include <sys/time.h>
#include <sstream>

#include "zType.h"
#include "zMutex.h"
#include "zMisc.h"

/**
 * \brief ��ʵʱ����,��timeval�ṹ�򵥷�װ,�ṩһЩ����ʱ�亯��
 * ʱ�侫�Ⱦ�ȷ�����룬
 * ����timeval��man gettimeofday
 */
class zRTime
{

	private:

		/**
		 * \brief ��ʵʱ�任��Ϊ����
		 *
		 */
		unsigned long long _msecs;

		/**
		 * \brief �õ���ǰ��ʵʱ��
		 *
		 * \return ��ʵʱ�䣬��λ����
		 */
		unsigned long long _now()
		{
			unsigned long long retval = 0LL;
			struct timeval tv;
			gettimeofday(&tv,NULL);
			retval = tv.tv_sec;
			retval *= 1000;
			retval += tv.tv_usec / 1000;
			return retval;
		}

		/**
		 * \brief �õ���ǰ��ʵʱ���ӳٺ��ʱ��
		 * \param delay �ӳ٣�����Ϊ��������λ����
		 */
		void nowByDelay(int delay)
		{
			_msecs = _now();
			addDelay(delay);
		}

	public:

		/**
		 * \brief ���캯��
		 *
		 * \param delay ���������ʱ�����ʱ����λ����
		 */
		zRTime(const int delay = 0)
		{
			nowByDelay(delay);
		}

		/**
		 * \brief �������캯��
		 *
		 * \param rt ����������
		 */
		zRTime(const zRTime &rt)
		{
			_msecs = rt._msecs;
		}

		/**
		 * \brief ��ȡ��ǰʱ��
		 *
		 */
		void now()
		{
			_msecs = _now();
		}

		/**
		 * \brief ��������
		 *
		 * \return ����
		 */
		unsigned long sec() const
		{
			return _msecs / 1000;
		}

		/**
		 * \brief ���غ�����
		 *
		 * \return ������
		 */
		unsigned long msec() const
		{
			return _msecs % 1000;
		}

		/**
		 * \brief �����ܹ��ĺ�����
		 *
		 * \return �ܹ��ĺ�����
		 */
		unsigned long long msecs() const
		{
			return _msecs;
		}

		/**
		 * \brief �����ܹ��ĺ�����
		 *
		 * \return �ܹ��ĺ�����
		 */
		void setmsecs(unsigned long long data)
		{
			_msecs = data;
		}

		/**
		 * \brief ���ӳ�ƫ����
		 *
		 * \param delay �ӳ٣�����Ϊ��������λ����
		 */
		void addDelay(int delay)
		{
			_msecs += delay;
		}

		/**
		 * \brief ����=�������
		 *
		 * \param rt ����������
		 * \return ��������
		 */
		zRTime & operator= (const zRTime &rt)
		{
			_msecs = rt._msecs;
			return *this;
		}

		/**
		 * \brief �ع�+������
		 *
		 */
		const zRTime & operator+ (const zRTime &rt)
		{
			_msecs += rt._msecs;
			return *this;
		}

		/**
		 * \brief �ع�-������
		 *
		 */
		const zRTime & operator- (const zRTime &rt)
		{
			_msecs -= rt._msecs;
			return *this;
		}

		/**
		 * \brief �ع�>���������Ƚ�zRTime�ṹ��С
		 *
		 */
		bool operator > (const zRTime &rt) const
		{
			return _msecs > rt._msecs;
		}

		/**
		 * \brief �ع�>=���������Ƚ�zRTime�ṹ��С
		 *
		 */
		bool operator >= (const zRTime &rt) const
		{
			return _msecs >= rt._msecs;
		}

		/**
		 * \brief �ع�<���������Ƚ�zRTime�ṹ��С
		 *
		 */
		bool operator < (const zRTime &rt) const
		{
			return _msecs < rt._msecs;
		}

		/**
		 * \brief �ع�<=���������Ƚ�zRTime�ṹ��С
		 *
		 */
		bool operator <= (const zRTime &rt) const
		{
			return _msecs <= rt._msecs;
		}

		/**
		 * \brief �ع�==���������Ƚ�zRTime�ṹ�Ƿ����
		 *
		 */
		bool operator == (const zRTime &rt) const
		{
			return _msecs == rt._msecs;
		}

		/**
		 * \brief ��ʱ�����ŵ�ʱ�䣬��λ����
		 * \param rt ��ǰʱ��
		 * \return ��ʱ�����ŵ�ʱ�䣬��λ����
		 */
		unsigned long long elapse(const zRTime &rt) const
		{
			if (rt._msecs > _msecs)
				return (rt._msecs - _msecs);
			else
				return 0LL;
		}

		static std::string & getLocalTZ(std::string & s);
		static void getLocalTime(struct tm & tv1, time_t timValue)
		{
			timValue +=8*60*60;
			tv1 = *gmtime(&timValue);
		}

};

/**
 * \brief ʱ����,��struct tm�ṹ�򵥷�װ
 */

class zTime
{

	public:

		/**
		 * \brief ���캯��
		 */
		zTime()
		{
			time(&secs);
			zRTime::getLocalTime(tv, secs);
		}

		/**
		 * \brief �������캯��
		 */
		zTime(const zTime &ct)
		{
			secs = ct.secs;
			zRTime::getLocalTime(tv, secs);
		}

		/**
		 * \brief ��ȡ��ǰʱ��
		 */
		void now()
		{
			time(&secs);
			zRTime::getLocalTime(tv, secs);
		}

		/**
		 * \brief ���ش洢��ʱ��
		 * \return ʱ�䣬��
		 */
		time_t sec() const
		{
			return secs;
		}

		/**
		 * \brief ����=�������
		 * \param rt ����������
		 * \return ��������
		 */
		zTime & operator= (const zTime &rt)
		{
			secs = rt.secs;
			return *this;
		}

		/**
		 * \brief �ع�+������
		 */
		const zTime & operator+ (const zTime &rt)
		{
			secs += rt.secs;
			return *this;
		}

		/**
		 * \brief �ع�-������
		 */
		const zTime & operator- (const zTime &rt)
		{
			secs -= rt.secs;
			return *this;
		}

		/**
		 * \brief �ع�-������
		 */
		const zTime & operator-= (const time_t s)
		{
			secs -= s;
			return *this;
		}

		/**
		 * \brief �ع�>���������Ƚ�zTime�ṹ��С
		 */
		bool operator > (const zTime &rt) const
		{
			return secs > rt.secs;
		}

		/**
		 * \brief �ع�>=���������Ƚ�zTime�ṹ��С
		 */
		bool operator >= (const zTime &rt) const
		{
			return secs >= rt.secs;
		}

		/**
		 * \brief �ع�<���������Ƚ�zTime�ṹ��С
		 */
		bool operator < (const zTime &rt) const
		{
			return secs < rt.secs;
		}

		/**
		 * \brief �ع�<=���������Ƚ�zTime�ṹ��С
		 */
		bool operator <= (const zTime &rt) const
		{
			return secs <= rt.secs;
		}

		/**
		 * \brief �ع�==���������Ƚ�zTime�ṹ�Ƿ����
		 */
		bool operator == (const zTime &rt) const
		{
			return secs == rt.secs;
		}

		/**
		 * \brief ��ʱ�����ŵ�ʱ�䣬��λ��
		 * \param rt ��ǰʱ��
		 * \return ��ʱ�����ŵ�ʱ�䣬��λ��
		 */
		time_t elapse(const zTime &rt) const
		{
			if (rt.secs > secs)
				return (rt.secs - secs);
			else
				return 0;
		}

		/**
		 * \brief ��ʱ�����ŵ�ʱ�䣬��λ��
		 * \return ��ʱ�����ŵ�ʱ�䣬��λ��
		 */
		time_t elapse() const
		{
			zTime rt;
			return (rt.secs - secs);
		}

		/**
		 * \brief �õ���ǰ���ӣ���Χ0-59��
		 *
		 * \return 
		 */
		int getSec()
		{
			return tv.tm_sec;
		}
	
		/**
		 * \brief �õ���ǰ���ӣ���Χ0-59��
		 *
		 * \return 
		 */
		int getMin()
		{
			return tv.tm_min;
		}
		
		/**
		 * \brief �õ���ǰСʱ����Χ0-23��
		 *
		 * \return 
		 */
		int getHour()
		{
			return tv.tm_hour;
		}
		
		/**
		 * \brief �õ���������Χ1-31
		 *
		 * \return 
		 */
		int getMDay()
		{
			return tv.tm_mday;
		}

		/**
		 * \brief �õ���ǰ���ڼ�����Χ1-7
		 *
		 * \return 
		 */
		int getWDay()
		{
			return tv.tm_wday;
		}

		/**
		 * \brief �õ���ǰ�·ݣ���Χ1-12
		 *
		 * \return 
		 */
		int getMonth()
		{
			return tv.tm_mon+1;
		}
		
		/**
		 * \brief �õ���ǰ���
		 *
		 * \return 
		 */
		int getYear()
		{
			return tv.tm_year+1900;
		}	

	private:

		/**
		 * \brief �洢ʱ�䣬��λ��
		 */
		time_t secs;
		
		/**
		 * \brief tm�ṹ���������
		 */
		struct tm tv;


};

class Timer
{
	public:
		Timer(const float how_long , const int delay=0) : _long((int)(how_long*1000)), _timer(delay*1000)
		{

		}
		Timer(const float how_long , const zRTime cur) : _long((int)(how_long*1000)), _timer(cur)
		{
			_timer.addDelay(_long);
		}
		void next(const zRTime &cur)
		{
			_timer=cur;
			_timer.addDelay(_long);
		} 
		bool operator() (const zRTime& current)
		{
			if (_timer <= current) {
				_timer = current;
				_timer.addDelay(_long);
				return true;
			}

			return false;
		}
	private:
		int _long;
		zRTime _timer;
};

//ʱ�������������
class RandTimer
{
	public:
#define next_time(_long) (_long / 2 + zMisc::randBetween(0, _long))
		RandTimer(const float how_long , const int delay=0) : _long((int)(how_long*1000)), _timer(delay*1000)
		{

		}
		RandTimer(const float how_long , const zRTime cur) : _long((int)(how_long*1000)), _timer(cur)
		{
			_timer.addDelay(next_time(_long));
		}
		void next(const zRTime &cur)
		{
			_timer=cur;
			_timer.addDelay(next_time(_long));
		} 
		bool operator() (const zRTime& current)
		{
			if (_timer <= current) {
				_timer = current;
				_timer.addDelay(next_time(_long));
				return true;
			}

			return false;
		}
	private:
		int _long;
		zRTime _timer;
};

struct FunctionInterval
{
	struct timeval _tv_1;
	struct timeval _tv_2;
	const int _need_log;
	const char *_fun_name;
	FunctionInterval(const int interval):_need_log(interval)
	{
		_tv_1.tv_sec=-1;
		_tv_1.tv_usec=-1;
	}
	void interval(const char *func=NULL);
};
struct FunctionTime
{
	private:
	struct timeval _tv_1;
	struct timeval _tv_2;
	const int _need_log;
	const char *_fun_name;
	const char *_dis;
	const int _dis_len;
	public:
	FunctionTime(const int interval,const char *func=NULL,const char *dis=NULL,const int dis_len=16):_need_log(interval),_fun_name(func),_dis(dis),_dis_len(dis_len)
	{
		gettimeofday(&_tv_1,NULL);
	}
	~FunctionTime();
};
struct FunctionTimes
{
	struct Times
	{
		//Times();
		Times():_log_timer(600),_times(0),_total_time(0)
		{
			bzero(_dis,sizeof(_dis));
		}
		Timer _log_timer;
		char _dis[256];
		int _times;
		int _total_time;
		zMutex _mutex;
	};
	private:
	static Times _times[256]; 
	int _which;
	struct timeval _tv_1;
	struct timeval _tv_2;
	public:
	FunctionTimes(const int which , const char *dis);
	~FunctionTimes();
};
/*
struct CmdAnalysis
{
	CmdAnalysis(const char *disc,DWORD time_secs):_log_timer(time_secs)
	{
		bzero(_disc,sizeof(disc));
		strncpy(_disc,disc,sizeof(_disc)-1);
		bzero(_data,sizeof(_data));
		_switch=false;
	}
	struct
	{
		DWORD num;
		DWORD size;
	}_data[256][256] ;
	zMutex _mutex;
	Timer _log_timer;
	char _disc[256];
	bool _switch;//����
	void add(const BYTE &cmd, const BYTE &para , const DWORD &size)
	{
		if(!_switch)
		{
			return ;
		}
		_mutex.lock(); 
		_data[cmd][para].num++;
		_data[cmd][para].size +=size;
		zRTime ct;
		if(_log_timer(ct))
		{
			for(int i = 0 ; i < 256 ; i ++)
			{
				for(int j = 0 ; j < 256 ; j ++)
				{
					if(_data[i][j].num)
						Zebra::logger->debug("%s:%d,%d,%d,%d",_disc,i,j,_data[i][j].num,_data[i][j].size);
				}
			}
			bzero(_data,sizeof(_data));
		}
		_mutex.unlock(); 
	}
};
// */
/*
struct FunctionTimes
{
	private:
		int _times;
		Timer _timer;
	public:
		FunctionTimes():_times(0),_timer(60)
		{
		}
		void operator();
		
};
// */
#endif
