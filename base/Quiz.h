/**
 * \file
 * \version  $Id: Quiz.h  $
 * \author  
 * \date 
 * \brief ����Ŷ�ս�Ļ����ṹ
 *
 */


#ifndef _DARE_H_
#define _DARE_H_

#include "zType.h"

#pragma pack(1)

namespace QuizDef
{
	const DWORD READYTIME  = 300; // �ȴ�Ӧս��ʱ�䣬��λ:��
	const DWORD ACTIVETIME = 3600; // ��ս����ʱ�䣬��λ:��


	const DWORD PERSONAL_QUIZ_NEED_GOLD = 100; // �����ʴ���������
	enum
	{
		WORLD_QUIZ = 0,  // ȫ������
		PERSONAL_QUIZ = 1 // �����ʴ�
	};

	/// ״̬����
	char str_state[][30]={"QUIZ_READY", "QUIZ_READY_QUESTION", "QUIZ_SEND_QUESTION", "QUIZ_ACTIVE_QUESTION", 
		"QUIZ_END_QUESTION", "QUIZ_READY_OVER", "QUIZ_READ_SORT", "QUIZ_RETURN_GOLD", "QUIZ_OVER"};
	char str_type[][20] = {"WORLD_QUIZ", "PERSONAL_QUIZ"};

}

#pragma pack()
#endif





