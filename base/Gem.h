/**
 * \file
 * \version  $Id: Gem.h 5600 2006-02-28 12:39:55Z zjw $
 * \author  �ٿ�ΰ,mark.zhong@gmail.com
 * \date 2005��03��16�� 10ʱ32��01�� CST
 * \brief ���廤������Ļ����ṹ
 *
 */


#ifndef _GEM_H_
#define _GEM_H_

#include "zType.h"

#pragma pack(1)

namespace GemDef
{

	const DWORD GEM_ACTIVE_TIME = 4*60*60; // �����������ʱ��


	/// ״̬����
	char str_gem_state[][20]={"GEM_READY", "GEM_ACTIVE", "GEM_READY_OVER", "GEM_OVER"};

}

#pragma pack()
#endif





