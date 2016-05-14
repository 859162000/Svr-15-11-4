/**
 * \file
 * \version  $Id: zEntry.h  $
 * \author  
 * \date 
 * \brief Entry���ඨ�壬�������ݹ���
 */

#ifndef _ZENTRY_H_
#define _ZENTRY_H_
#include "zMutex.h"
#include "zRWLock.h"
#include "zType.h"
#include "zNoncopyable.h"

/**
 * \brief Entry����
 */

#pragma pack(1)
struct zEntryC
{
	/**
	 * \brief entry������ID����ͬ���͵�Entry���ܻ��ظ�,��ʱ����ʵ�ִ�ID����entry
	 */
	DWORD id;
	/**
	 * \brief entry����ʱid,������ʵ��EntryManagerʱ����֤����Ψһ
	 */
	DWORD tempid;
	/**
	 * \brief entry�����֣���ͬ���͵�Entry���ܻ��ظ�,��ʱ����ʵ�ִ����ֲ���entry
	 */
	char name[MAX_NAMESIZE+1];
	zEntryC()
	{
		id=0;
		tempid=0;
		bzero(name,sizeof(name));
	};
};

/**
 * \brief �ص�������ģ��
 */
template <typename T, typename RTValue = bool>
struct zEntryCallback
{
	virtual RTValue exec(T *e)=0;
	virtual ~zEntryCallback(){};
};

struct zEntry:public zEntryC,private zNoncopyable
{
	virtual ~zEntry(){};
	zEntry():zEntryC()
	{
	};
};
#pragma pack()

#endif
