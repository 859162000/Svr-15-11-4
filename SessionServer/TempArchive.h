/**
 * \file
 * \version  $Id: TempArchive.h  $
 * \author  
 * \date 
 * \brief 临时档案存储
 *
 * 
 */

#ifndef _TEMPARCHIVE_H_
#define _TEMPARCHIVE_H_
#include "zDatabaseManager.h"
#include "zType.h"
#include "Command.h"
#include "SessionCommand.h"
#include "zSceneEntry.h"
#include "zEntryManager.h"
#include "zTime.h"
#include <map>

struct zMutex;
class zRTime;
/**
 * \brief 临时档案数据结构
 *
 */
struct TempArchive 
{
	TempArchive() 
	{
		id = 0;
		createtime = 0;
		dwSize = 0;
	}
	DWORD id;
	zRTime createtime;
	DWORD dwSize;
	char data[0];
};

/**
 * \brief 临时档案管理器
 *
 */
class GlobalTempArchiveIndex//:private zEntryManager< zEntryID >
{
	private:
		static GlobalTempArchiveIndex *_instance;
		typedef std::map<DWORD, TempArchive *> TempArchive_map;
		typedef TempArchive_map::iterator TempArchive_iterator;
		typedef TempArchive_map::value_type TempArchive_value_type;
		TempArchive_map tempArchive;
		zMutex mlock;

		GlobalTempArchiveIndex();
		~GlobalTempArchiveIndex();
		void remove(TempArchive_iterator del_iter);
	public:
		static GlobalTempArchiveIndex *getInstance();
		static void delInstance();
		bool readTempArchive(DWORD id , char *out , DWORD &outSize);
		void checkOverdue();
		bool writeTempArchive(DWORD id , char *data , DWORD  dwSize);
};


#endif
