/**
 * \file
 * \version  $Id: zNullCmd.h  $
 * \author  
 * \date 
 * \brief ����յ�ָ��
 *
 * ����������ڲ�����ʹ�ã��Ϳͻ��˽�����ָ����Ҫ���ⶨ��
 * 
 */


#ifndef _zNullCmd_h_
#define _zNullCmd_h_

#include "zType.h"

#pragma pack(1)

namespace Cmd
{
	const BYTE CMD_NULL = 0;		/**< �յ�ָ�� */
	const BYTE PARA_NULL = 0;	/**< �յ�ָ����� */

	/**
	 * \brief �ղ���ָ������źźͶ�ʱ��ָ��
	 *
	 */
	struct t_NullCmd
	{
		BYTE cmd;					/**< ָ����� */
		BYTE para;					/**< ָ������ӱ�� */
		/**
		 * \brief ���캯��
		 *
		 */
		t_NullCmd(const BYTE cmd = CMD_NULL, const BYTE para = PARA_NULL) : cmd(cmd), para(para) {};
	};
};

#pragma pack()

#endif

