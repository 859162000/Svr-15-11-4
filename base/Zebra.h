#ifndef _Zebra_h_
#define _Zebra_h_

#include "zLogger.h"
#include "zVersion.h"
#include "zProperties.h"

namespace Zebra
{
	/**
	 * \brief ��Ϸʱ��
	 *
	 */
	extern volatile QWORD qwGameTime;

	/**
	 * \brief ��־ָ��
	 *
	 */
	extern zLogger *logger;

	/**
	 * \brief ��ȡȫ�ֱ���������
	 *
	 */
	extern zProperties global;
};

#endif
