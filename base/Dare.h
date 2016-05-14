/**
 * \file
 * \version  $Id: Dare.h 6741 2006-06-05 10:12:13Z zjw $
 * \author  �ٿ�ΰ,mark.zhong@gmail.com
 * \date 2005��03��16�� 10ʱ32��01�� CST
 * \brief ����Ŷ�ս�Ļ����ṹ
 *
 */


#ifndef _DARE_H_
#define _DARE_H_

#include "zType.h"

#pragma pack(1)

namespace DareDef
{

	const DWORD CREATE_DARE_NEED_PRICE_GOLD = 500; // ��ս���۽��
	const DWORD DARE_WINNER_GOLD = 800; // ��սʤ��һ����������
	const DWORD READYTIME  = 300; // �ȴ�Ӧս��ʱ�䣬��λ:��
	const DWORD ACTIVETIME = 3600; // ��ս����ʱ�䣬��λ:��
	const DWORD CREATE_UNION_CITY_DARE_NEED_PRICE_MONEY = 20000; //����
	const DWORD CREATE_UNION_KING_CITY_DARE_NEED_PRICE_MONEY = 50000; //�嶧
	const DWORD CREATE_UNION_NEUTRAL_CITY_DARE_NEED_PRICE_MONEY = 50000; //�嶧

	class SupportDareInterface
	{
		public:
			SupportDareInterface(){};
			virtual ~SupportDareInterface(){};


	};

	/// ״̬����
	char str_state[][20]={"DARE_READY", "DARE_READY_QUESTION", "DARE_READY_ACTIVE", "DARE_RETURN_GOLD",
		"DARE_ACTIVE", "DARE_DATAPROCESS", "DARE_READY_OVER", "DARE_WAIT_BOUNTY", "DARE_OVER"};

	char str_type[][30] = {"UNION_DARE", "SCHOOL_DARE", "SEPT_DARE", "SEPT_NPC_DARE","UNION_CITY_DARE","COUNTRY_FORMAL_DARE", "COUNTRY_FORMAL_ANTI_DARE"};

}

#pragma pack()
#endif





