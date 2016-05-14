/**
 * \file
 * \version  $Id: URLManager.h  $
 * \author  
 * \date 
 * \brief 定义管理数据库连接url的类
 *
 *
 */

#ifndef _URLManager_h_
#define _URLManager_h_

#include <string>
#include <ext/hash_set>

#include "zMutex.h"
#include "Zebra.h"

/**
 * \brief Mysql连接url
 *
 */
struct URL
{
	unsigned int hashcode;
	std::string url;
	bool supportTransactions;
};

/**
 * \brief Mysql连接管理容器的回调函数
 *
 */
struct URLCallback
{
	virtual ~URLCallback() {};
	virtual bool exec(const URL &url) = 0;
};

/**
 * \brief Mysql连接管理容器
 *
 */
class URLManager
{

	public:

		~URLManager()
		{
			instance = NULL;
		}

		static URLManager &getInstance()
		{
			if (NULL == instance)
				instance = new URLManager(atoi(Zebra::global["dbCount"].c_str()), atoi(Zebra::global["tableCount"].c_str()));

			return *instance;
		}

		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		static unsigned int hashString(const char* __s)
		{
			unsigned int __h = 0; 
			for ( ; *__s; ++__s)
				__h = 5*__h + *__s;
			return __h;
		}

		static unsigned int defaultHashCode(const void *anyArg)
		{
			return ((*((unsigned int *)anyArg)) % getInstance().dbCount);
		}

		bool add(const URL &url);
		bool execAll(URLCallback &callback);

		const std::string &roleregTableName(const unsigned int hashcode)
		{
			return roleregTableNames[hashcode % tableCount];	
		}

	private:

		const int dbCount;
		const int tableCount;
		std::vector<std::string> roleregTableNames;

		URLManager(
				const int dbCount,
				const int tableCount) : dbCount(dbCount), tableCount(tableCount), roleregTableNames(tableCount)
		{
			for(int i = 0; i < tableCount; i++)
			{
				char buffer[80];

				bzero(buffer, sizeof(buffer));
				snprintf(buffer, sizeof(buffer) - 1, "ROLEREG%04u", i);
				roleregTableNames[i] = buffer;
			}
		}

		static URLManager *instance;

		struct hashURL
		{
			size_t operator()(const URL &url) const
			{
				__gnu_cxx::hash<unsigned int> h;
				return h(url.hashcode);
			}
		};

		struct eqURL
		{
			bool operator()(const URL &url1, const URL &url2) const
			{
				return url1.hashcode == url2.hashcode;
			}
		};

		typedef __gnu_cxx::hash_set<URL, hashURL, eqURL> URLContainer;
		URLContainer urls;
		zMutex mlock;

};

#endif

