/**
 * \file
 * \version  $Id: zMisc.h  $
 * \author  
 * \date 
 * \brief ��װһЩ���ú���
 *
 * 
 */

#ifndef _ZMISC_H_
#define _ZMISC_H_

#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <ctime>
#include <vector>
#include <queue>
#include <dirent.h>
#include <unistd.h>
#include "zType.h"
#include "zMutex.h"

struct odds_t
{
	unsigned int upNum;
	unsigned int downNum;
};

enum ServerType {
	UNKNOWNSERVER	=	0,		/**< δ֪���������� */
	SUPERSERVER	=	1,		/**< ������������ */
	LOGINSERVER	=	10,		/**< ��½������ */
	RECORDSERVER	=	11,		/**< ���������� */
	BILLSERVER	=	12,		/**< �Ʒѷ����� */
	SESSIONSERVER	=	20,		/**< Session������ */
	SCENESSERVER	=	21,		/**< ���������� */
	GATEWAYSERVER	=	22,		/**< ���ط����� */
	MINISERVER	=	23,		/**< С��Ϸ������ */
	MAX_SERVERTYPE				/**< ���������͵������ */
};

namespace Misc
{
	template <typename V>
	class Parse
	{
	public:
		V* operator () (const std::string& down, const std::string& separator_down)
		{
			std::string::size_type pos = 0;
			if  ( (pos = down.find(separator_down)) != std::string::npos ) {
				
				std::string first_element = down.substr(0, pos);
				std::string second_element = down.substr(pos+separator_down.length());
				return new V(first_element, second_element);
			}
			
			return NULL;
		}
	};
	
	template <typename V>
	class Parse3
	{
	public:
		V* operator () (const std::string& down, const std::string& separator_down)
		{
			std::string::size_type pos = 0;
			if  ( (pos = down.find(separator_down)) != std::string::npos ) {
				
				std::string first_element = down.substr(0, pos);
				std::string::size_type npos = 0;
				if ( (npos = down.find(separator_down, pos+separator_down.length())) != std::string::npos) {
					std::string second_element = down.substr(pos+separator_down.length(), npos-pos);
					std::string third_element = down.substr(npos+separator_down.length());
					return new V(first_element, second_element, third_element);
				}
			}
			
			return NULL;
		}
	};

	/**
	 * \brief  �ָ��ɶ����ָ����ָ����ַ���
	 * \param list ���ָ����ַ���
	 * \param dest �洢�ָ���������������ض�������Ҫ��
	 * \param separator_up һ���ָ���
	 * \param separator_down �����ָ���		 
	 * \author liqingyu 
	 */
	template <template <typename> class P = Parse>
	class Split
	{
	public:

		template <typename T>
		void operator() (const std::string& list, T& dest, const std::string& separator_up = ";", const std::string& separator_down = ",")
		{	
			typedef typename T::value_type value_type;
			typedef typename T::pointer pointer;

			std::string::size_type lpos = 0;
			std::string::size_type pos = 0;
			P<value_type> p;
			
			
			while ( ( lpos = list.find(separator_up, pos)) != std::string::npos) {
				/*
				std::string down = list.substr(pos, lpos - pos);
				std::string::size_type dpos = 0;
				if  ( (dpos = down.find(separator_down)) != std::string::npos ) {
					
					std::string first_element = down.substr(0, dpos);
					std::string second_element = down.substr(dpos+separator_down.length());
					dest.push_back(typename T::value_type(first_element, second_element));
				}
				pos = lpos+1;
				*/
				std::string down = list.substr(pos, lpos - pos);
				pointer v = p(down, separator_down);
				if (v) {
					dest.push_back(*v);
					SAFE_DELETE(v);
				}
				pos = lpos+1;
			}

			std::string down = list.substr(pos, lpos - pos);
			pointer v = p(down, separator_down);
			if (v) {
				dest.push_back(*v);
				SAFE_DELETE(v);
			}
		}
	};
}

namespace Zebra
{
	// rand number generator's seed
	extern __thread unsigned int seedp;
};

class zMisc
{
	public:	
		//���ַ����в��ҵ�pos(���㿪ʼ)�����֣����δ�ҵ�����defValue
		template <typename T>
		static WORD getAllNum(const char *s,std::vector<T> & data)
		{
			size_t i;
			int count = 0;
			if (s == NULL) return count;
			bool preIsD = false;
			for (i = 0; i < strlen(s); i++)
			{
				if (isdigit(*(s + i)))
				{
					if (!preIsD)
					{
						count++;
						data.push_back(atoi(s+i));
					}
					preIsD = true;
				}
				else
					preIsD = false;
			}
			return count;
		}

		//�������min~max֮������֣�����min��max
		static int randBetween(int min,int max)
		{
			if(min==max)
				return min;
			else if (min > max)
				return max + (int) (((double) min - (double)max + 1.0) * rand_r(&Zebra::seedp) / (RAND_MAX + 1.0));
			else
				return min + (int) (((double) max - (double)min + 1.0) * rand_r(&Zebra::seedp) / (RAND_MAX + 1.0));
		}

		//��ȡ����֮�ļ���
		static bool selectByOdds(const unsigned int upNum, const unsigned int downNum)
		{
			unsigned int m_rand;
			if (downNum < 1) return false;
			if (upNum < 1) return false;
			if (upNum > downNum - 1) return true;
			m_rand = 1 + (unsigned int) ((double)downNum * rand_r(&Zebra::seedp) / (RAND_MAX + 1.0));
			return (m_rand <= upNum);
		}

		//��ȡ����֮���ļ���
		static bool selectByt_Odds(const odds_t &odds)
		{
			return selectByOdds(odds.upNum, odds.downNum);
		}

		//��ȡ�ٷ�֮�ļ���
		static bool selectByPercent(const unsigned int percent)
		{
			return selectByOdds(percent, 100);
		}

		//��ȡ���֮�ļ���
		static bool selectByTenTh(const unsigned int tenth)
		{
			return selectByOdds(tenth, 10000);
		}

		//��ȡʮ���֮�ļ���
		static bool selectByLakh(const unsigned int lakh)
		{
			return selectByOdds(lakh, 100000);
		}

		//��ȡ�ڷ�֮֮�ļ���
		static bool selectByOneHM(const unsigned int lakh)
		{
			return selectByOdds(lakh, 100000000);
		}

		//��ȡ��ǰʱ���ַ�������Ҫ������ʽ
		static void getCurrentTimeString(char *buffer, const int bufferlen, const char *format)
		{
			time_t now;
			struct tm tmnow;
			time(&now);
			localtime_r(&now, &tmnow);
			strftime(buffer, bufferlen, format, &tmnow);
		}

		static char *getTimeString(time_t t,char *buffer, const int bufferlen)
		{
			return getTimeString(t,buffer,bufferlen,"%C/%m/%d %T");
		}

		static char *getTimeString(time_t t,char *buffer, const int bufferlen, const char *format)
		{
			struct tm tmnow;
			localtime_r(&t, &tmnow);
			strftime(buffer, bufferlen, format, &tmnow);
			return buffer;
		}
		
		/**
		 * \brief ɾ��Ŀ¼��
		 * \author fqnewman
		 */
		static bool rmDirTree(std::string path)
		{
			struct dirent *record;
			DIR* tDir = opendir(path.c_str());
			if (tDir == NULL) return false;

			while(NULL != (record = readdir(tDir)))
			{
				if (record->d_type == DT_DIR)
				{
					if (strcmp(record->d_name,".") == 0 ||	strcmp(record->d_name,"..") == 0) continue;

					if (!rmDirTree((path+"/"+record->d_name).c_str())) return false;
				}
				else
				{
					if (unlink((path+"/"+record->d_name).c_str()) != 0) return false;
				}
		}
		closedir(tDir);
		if (rmdir(path.c_str())!=0)return false;
		return true;
	}
};

template <typename T>
struct singleton_default
{
	private:
		singleton_default();

	public:
		typedef T object_type;

		static object_type & instance()
		{
			return obj;
		}

		static object_type obj;
};
template <typename T>
typename singleton_default<T>::object_type singleton_default<T>::obj;

//�ֶ����ù��캯�����������ڴ�
	template<class _T1> 
inline	void constructInPlace(_T1  *_Ptr)
{
	new (static_cast<void*>(_Ptr)) _T1();
}
/// �����䳤ָ��
#define BUFFER_CMD(cmd,name,len) char buffer##name[len];\
				cmd *name=(cmd *)buffer##name;constructInPlace(name);

#include <ext/pool_allocator.h>
#include <ext/mt_allocator.h>
typedef std::pair<unsigned int , unsigned char *> CmdPair;
template <int QueueSize=102400>
class MsgQueue
{
	public:
		MsgQueue()
		{
			queueRead=0;
			queueWrite=0;
		}
		~MsgQueue()
		{
			clear();
		}
		typedef std::pair<volatile bool , CmdPair > CmdQueue;
		CmdPair *get()
		{
			CmdPair *ret=NULL;
			if(cmdQueue[queueRead].first)
			{
				ret=&cmdQueue[queueRead].second;
			}
			return ret;
		}
		void erase()
		{
				//SAFE_DELETE_VEC(cmdQueue[queueRead].second.second);
				__mt_alloc.deallocate(cmdQueue[queueRead].second.second, cmdQueue[queueRead].second.first);
				cmdQueue[queueRead].first=false;
				queueRead = (++queueRead)%QueueSize;
		}
		bool put(const void *ptNullCmd, const unsigned int cmdLen)
		{
			//unsigned char *buf = new unsigned char[cmdLen];
			unsigned char *buf = __mt_alloc.allocate(cmdLen);
			if(buf)
			{
				bcopy(ptNullCmd , buf , cmdLen);
				if(!putQueueToArray() && !cmdQueue[queueWrite].first)
				{
					cmdQueue[queueWrite].second.first = cmdLen;
					cmdQueue[queueWrite].second.second = buf;
					cmdQueue[queueWrite].first=true;
					queueWrite = (++queueWrite)%QueueSize;
					return true;
				}
				else
				{
					queueCmd.push(std::make_pair(cmdLen , buf));
				}
				return true;
			}
			return false;

		}
	private:
		void clear()
		{
			while(putQueueToArray())
			{
				while(get())
				{
					erase();
				}
			}
			while(get())
			{
				erase();
			}
		}
		bool putQueueToArray()
		{
			bool isLeft=false;
			while(!queueCmd.empty())
			{
				if(!cmdQueue[queueWrite].first)
				{
					cmdQueue[queueWrite].second = queueCmd.front();;
					cmdQueue[queueWrite].first=true;
					queueWrite = (++queueWrite)%QueueSize;
					queueCmd.pop();
				}
				else
				{
					isLeft = true; 
					break;
				}
			}
			return isLeft;
		}
		__gnu_cxx::__mt_alloc<unsigned char> __mt_alloc;
		CmdQueue cmdQueue[QueueSize];
#ifdef _POOL_ALLOC_		
		std::queue<CmdPair, std::deque<CmdPair, __gnu_cxx::__pool_alloc<CmdPair> > > queueCmd;
#else		
		std::queue<CmdPair, std::deque<CmdPair> > queueCmd;
#endif		
		unsigned int queueWrite;
		unsigned int queueRead;
};
template < typename T>
T **make_2d_array(const int x,const int y , T *value=NULL,const int width=0)
{
	T **t;
	t = new T*[x];
	if(value)
	{
		for(int i=0 ; i < x ; i++)
		{
			t[i] = new T[y];
			for(int j=0 ; j < y ; j++)
			{
				t[i][j]=value[j*width + i];
			}
		}
	}
	else
	{
		for(int i=0 ; i < x ; i++)
		{
			*t = new T[y];
		}
	}
	return t;
}
template < typename T>
void free_2d_array(T **&t ,const int x,const int y)
{
	for(int i=0 ; i < x ; i++)
	{
			SAFE_DELETE_VEC(t[i]);
	}
	SAFE_DELETE_VEC(t);
}


#endif

