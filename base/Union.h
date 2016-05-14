/**
 * \file
 * \version  $Id: Union.h  $
 * \author  
 * \date 
 * \brief ������Ļ����ṹ
 *
 */


#ifndef _UNION_H_
#define _UNION_H_

#include "zType.h"

#pragma pack(1)

namespace UnionDef
{

#define WD2BA(wd,ba)       \
do                         \
{                          \
	ba[0]= (wd >>8)&0xFF;	   \
	ba[1]= wd& 0xFF;         \
}while(false)

#define BA2WD(ba,wd)			\
do												\
{													\
		wd=0;									\
		wd=ba[0];							\
		wd = wd << 8;					\
		wd|=ba[1];						\
}while(false)
    
#define SETMEMBERPOWER(ba) Cmd::set_state(ba,Cmd::GENERAL)

#define SETMASTERPOWER(ba)                \
do                                        \
{                                         \
	Cmd::set_state(ba,Cmd::CHANGE_ALIAS);   \
	Cmd::set_state(ba,Cmd::ADD_MEMBER);     \
	Cmd::set_state(ba,Cmd::FIRE_MEMBER);    \
	Cmd::set_state(ba,Cmd::GENERAL);        \
	Cmd::set_state(ba,Cmd::MASTER);        \
	Cmd::set_state(ba,Cmd::DARE);        \
	Cmd::set_state(ba,Cmd::NOTIFY);        \
}while(false)

#define SETSECONDPOWER(ba)                \
do                                        \
{                                         \
	Cmd::set_state(ba,Cmd::CHANGE_ALIAS);   \
	Cmd::set_state(ba,Cmd::ADD_MEMBER);     \
	Cmd::set_state(ba,Cmd::FIRE_MEMBER);    \
	Cmd::set_state(ba,Cmd::GENERAL);        \
	Cmd::set_state(ba,Cmd::NOTIFY); \
}while(false)

#define SETMEMBERPOWER_WD(WD) \
do                            \
{                             \
	BYTE temp[2];               \
	temp[0] = 0x00;			\
	temp[1] = 0x00;			\
	SETMEMBERPOWER(temp);       \
	BA2WD(temp,WD);             \
}while(false)


#define SETMASTERPOWER_WD(WD) \
do                            \
{                             \
	BYTE temp[2];               \
	temp[0] = 0x00;			\
	temp[1] = 0x00;			\
	SETMASTERPOWER(temp);       \
	BA2WD(temp,WD);             \
}while(false)


const DWORD CREATE_UNION_NEED_PRICE_GOLD = 50000; // ���������Ҫ��Ǯ10��
const DWORD CREATE_UNION_NEED_ITEM_ID    = 684;     // �����������Ҫ�ĵ���ID ������
const DWORD CREATE_UNION_NEED_LEVEL      = 45;      // ���������Ҫ������ȼ�
const DWORD DESTROYTIME	 		 = 30*24*60*60; //ͶƱ��Ч�� һ���µ�����
const DWORD CREATE_UNION_NEED_VOTE       =  2;
const DWORD CREATE_UNION_NEED_MAN_NUM    =30;	// ��������Ҫ����
		
#define DEFAULTMASTERALIAS "����"
#define DEFAULTMEMBERALIAS "����"
		
struct stUnionInfo	 					// ���ݿ��ȡ�ṹ���벻Ҫ�����޸ģ��޸�ע��������д���
{
	DWORD		dwUnionID;				// ����ID
	char		name[MAX_NAMESIZE+1];   		// �������
	DWORD		dwCharID;             			// �����Ľ�ɫID
	char		masterName[MAX_NAMESIZE+1];		// ����������
	BYTE		byVote;					// ǩ����־Ϊ1��ʾ����ǩ���ڼ�
	DWORD		dwCrTime;				// ��ᴴ��ʱ��
	WORD		wdLevel;     		    	       	// ��ἶ��
	QWORD		qwExp;        		         	// ��ᾭ��
	DWORD 		dwMana;					// �������
	DWORD		dwCountryID;				// ��������
	DWORD		dwActionPoint;				// ����ж���
	DWORD 		dwMoney;				// ����ʽ�
	char		note[254+1];				// ������
	DWORD		calltimes;				// ����ʹ�ô���
}__attribute__ ((packed));

struct stUnionMemberInfo                			// ���ݿ��ȡ�ṹ���벻Ҫ�����޸ģ��޸�ע��������д���
{
	DWORD	dwCharID;					// ��Ա��ɫID
	char	name[MAX_NAMESIZE+1];				// ��Ա��ɫ����		
	char    aliasname[MAX_NAMESIZE+1]; 			// ��Ա����
	WORD    wdPower;					// ��ԱȨ��
	WORD    wdOccupation;					// ��Ա��ְҵ
	DWORD   dwSeptID;					// ��Ա��������
}__attribute__ ((packed));

}

#pragma pack()
#endif





