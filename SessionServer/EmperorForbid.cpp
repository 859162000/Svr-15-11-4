#include "EmperorForbid.h"
#include "SessionServer.h"
#include "zDBConnPool.h"

const dbCol data_define[] = {
	{ "DATA",	zDBConnPool::DB_STR,	sizeof(DWORD)*10 },
	{ NULL, 0, 0}
};

EmperorForbid::EmperorForbid()
{
	loadDB();
}

EmperorForbid::~EmperorForbid()
{
	writeDB();
}

void EmperorForbid::loadDB()
{
	clear();

	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{               
		Zebra::logger->error("EmperorForbid::loadDB(): 得到数据库句柄失败");
		return;
	}

	DWORD d[10];
	DWORD ret = SessionService::dbConnPool->exeSelectLimit(handle, "`EMPERORFORBID`", data_define, NULL, NULL, 1, (BYTE *)&d[0]);
	if (0==ret)
		SessionService::dbConnPool->exeInsert(handle, "`EMPERORFORBID`", data_define, (BYTE *)&d[0]);
	else if (1==ret)
	{
		for (DWORD i=0; i<10; i++)
			if (d[i])
				list.push_back(d[i]);
	}

	SessionService::dbConnPool->putHandle(handle);
	Zebra::logger->trace("加载 %u 个皇帝禁言信息", list.size());
}

void EmperorForbid::writeDB()
{
	connHandleID handle = SessionService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{               
		Zebra::logger->error("EmperorForbid::writeDB(): 得到数据库句柄失败");
		return;
	}

	DWORD d[10];
	bzero(&d[0], sizeof(d));
	for (DWORD i=0; i<count(); i++)
		d[i] = list[i];

	SessionService::dbConnPool->exeUpdate(handle, "`EMPERORFORBID`", data_define, (BYTE *)&d[0], NULL);
	SessionService::dbConnPool->putHandle(handle);
	Zebra::logger->trace("保存 %u 个皇帝禁言信息", list.size());
}

void EmperorForbid::clear()
{
	list.clear();
}

DWORD EmperorForbid::count()
{
	return list.size();
}

bool EmperorForbid::find(DWORD id)
{
	for (DWORD i=0; i<count(); i++)
		if (list[i]==id)
			return true;
	return false;
}

bool EmperorForbid::add(DWORD id)
{
	if (count()>=10 || find(id)) return false;

	list.push_back(id);
	return true;
}

void EmperorForbid::timer()
{
	time_t timValue = time(NULL);
	struct tm tmValue;
	zRTime::getLocalTime(tmValue, timValue);

	if (tmValue.tm_hour==0 && tmValue.tm_min==0)
		clear();
}

