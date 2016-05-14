/**
 * \file
 * \version  $Id: URLManager.cpp  $
 * \author  
 * \date 
 * \brief 实现管理数据库连接url的类
 *
 * 
 */

#include <string>
#include <ext/hash_set>

#include "zMutex.h"
#include "Zebra.h"
#include "URLManager.h"

URLManager *URLManager::instance = NULL;

bool URLManager::add(const URL &url)
{
	zMutex_scope_lock scope_lock(mlock);
	//Zebra::logger->debug("%u, %s, %s", url.hashcode, url.url.c_str(), url.supportTransactions ? "yes": "no");
	std::pair<URLContainer::iterator, bool> ret = urls.insert(url);
	if (ret.second == false)
		Zebra::logger->error("(%u, %s, %s)already exist.", url.hashcode, url.url.c_str(), url.supportTransactions ? "yes": "no");
	return ret.second;
}

bool URLManager::execAll(URLCallback &callback)
{
	zMutex_scope_lock scope_lock(mlock);
	for(URLContainer::iterator it = urls.begin(); it != urls.end(); it++)
	{
		if (!callback.exec(*it))
			return false;
	}
	return true;
}

