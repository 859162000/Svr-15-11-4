/**
 * \file
 * \version  $Id: Sept.h  $
 * \author  
 * \date 
 * \brief �������Ļ����ṹ
 *
 */


#ifndef _SEPT_H_
#define _SEPT_H_

#include "zType.h"

#pragma pack(1)

namespace SeptDef
{

//#define DWORD CREATE_SEPT_NEED_ITEM_ID    = 0	// ������������Ҫ�ĵ���ID Ϊ0��ʾ����Ҫ
const DWORD CREATE_SEPT_NEED_PRICE_GOLD = 2000;	// ����������Ҫ��Ǯ20��
const DWORD CREATE_SEPT_NEED_LEVEL     = 35;	// ����������Ҫ������ȼ�
const DWORD JOIN_SEPT_NEED_LEVEL		= 20;	// ���������Ҫ������ȼ�
const DWORD PER_LEVEL_MAN_NUM		= 1;		// ÿ������������
const DWORD DESTROYTIME					= 30*24*60*60; //ͶƱ��Ч�� һ���µ�����
const DWORD CREATE_SEPT_NEED_MAN_NUM = 15;		// ��������
		
struct stSeptInfo											// ���ݿ��ȡ�ṹ���벻Ҫ�����޸ģ��޸�ע��������д���
{
	DWORD		dwSeptID;									  // �����ID
	char		name[MAX_NAMESIZE+1];   		// ��������
	DWORD		dwCharID;             			// �᳤�Ľ�ɫID
	char		masterName[MAX_NAMESIZE+1];		// �᳤������
	BYTE		byVote;					// ͶƱ��־Ϊ1��ʾ����ͶƱ�ڼ�
	DWORD		dwCrTime;				// ���崴��ʱ��
	DWORD 		dwRepute;				// ����
	DWORD 		dwCountryID;				// �����������ң����峤����Ϊ���ݣ�
	DWORD 		dwUnionID;				// �����������
	DWORD 		dwLevel;				// ����ȼ�
	DWORD 		dwSpendGold;				// �������ѽ��
	DWORD 		dwIsExp;				// �Ƿ���ȡ���徭��
	char		note[254+1];				// �������
	DWORD		calltimes;				// ����ʹ�ô���
	DWORD		calldaytime;				// ����ʹ������
	DWORD		normalexptime;				// ����ʹ������
}__attribute__ ((packed));


struct stSeptMemberInfo                			// ���ݿ��ȡ�ṹ���벻Ҫ�����޸ģ��޸�ע��������д���
{
	DWORD	dwCharID;				// ��Ա��ɫID
	char	name[MAX_NAMESIZE+1];			// ��Ա��ɫ����		
	char    aliasname[MAX_NAMESIZE+1];		// ����
	WORD    wdOccupation;				// ��Ա��ְҵ
}__attribute__ ((packed));

}

#pragma pack()
#endif






