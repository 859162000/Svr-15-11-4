/**
 * \file
 * \version  $Id: NpcDare.h $
 * \author  
 * \date 
 * \brief ����NPC����Ĺ�����Ϣ
 *
 */


#ifndef _NPCDARE_H_
#define _NPCDARE_H_

#include "zType.h"

#pragma pack(1)

namespace NpcDareDef
{

	const DWORD CREATE_NPCDARE_NEED_ITEM = 738; // �����ս��Ҫ�ĵ��� ������

	struct NpcDareRecord {
		DWORD dwCountry;			/// ����
		DWORD dwMapID;				/// ��ͼID
		DWORD dwNpcID;				/// NPC id
		DWORD dwPosX;				/// npc�� x ����
		DWORD dwPosY;				/// npc�� y ����
		DWORD dwHoldSeptID;			/// Ŀǰ��npc�����м���
		DWORD dwDareSeptID;			/// Ŀǰ��npc����ս����
		DWORD dwGold;				/// ����˰��
	} __attribute__ ((packed));

}

#pragma pack()
#endif





