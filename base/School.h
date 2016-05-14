/**
 * \file
 * \version  $Id: School.h $
 * \author  
 * \date 
 * \brief ������Ļ����ṹ
 *
 */


#ifndef _SCHOOL_H_
#define _SCHOOL_H_

#include "zType.h"

#pragma pack(1)

namespace School
{

#define MAX_BULLETIN_CHAR_NUMBER 1000          // �����������������ַ���
#define MAX_PRENTICE 20							// һ�������������յ�ͽ����Ŀ
#define TEACHER_LEVEL 30						// ����ͽ����Ҫ�ȼ�
#define CREATE_SCHOOL_REQUEST_LEVEL 60			// �������ɵȼ���Ҫ 60
#define CREATE_SCHOOL_REQUEST_ITEM_ID 684      // ����������Ҫ����ĿǰΪ������
#define CREATE_SCHOOL_REQUEST_PRICE_GOLD 100000 // ����������Ҫ�Ľ�Ǯ��Ŀ
#define FIRST_LAYER_PRENTICE_REQUEST_LEVEL 50   // ���������ߵĵ�һ��ͽ����Ҫ�Ľ�ɫ����
#define SECOND_LAYER_PRENTICE_REQUEST_LEVEL 40  // ���������ߵĵڶ���ͽ����Ҫ�Ľ�ɫ����

struct stSchoolInfo
{
	DWORD dwSchoolID;
	char  name[MAX_NAMESIZE+1];
	DWORD dwMasterSerialID;
	DWORD  size;
	//char data[0];
}__attribute__ ((packed));
	
struct stSchoolMemberInfo
{
	DWORD dwSerialID;
	DWORD dwMasterID;
	DWORD dwPreSerialID;
	DWORD dwCharID;
	char  name[MAX_NAMESIZE+1];
	WORD  wdLevel;
	DWORD dwJoinTime;
	WORD  wdDegree;
	DWORD dwLastTime;
	DWORD dwSchoolID;
	BYTE  byTag;
	WORD  wdOccupation;
	DWORD master_total;
	DWORD master_balance;
	DWORD prentice_lastlevel;
	DWORD prentice_total;
}__attribute__ ((packed));


}

#pragma pack()

#endif





