#include <stdlib.h>
#include <iostream>

#include "zMisc.h"
#include "Zebra.h"

namespace Zebra
{
	__thread unsigned int seedp = 0;

	volatile QWORD qwGameTime = 0;

	zLogger *logger = NULL;

	zProperties global;

	/**
	 * \brief ��ʼ��һЩȫ�ֱ���
	 *
	 */
	static void initGlobal() __attribute__ ((constructor));
	void initGlobal()
	{
		global["threadPoolCapacity"] = "2048";
		global["server"] = "192.168.1.162";
		global["port"] = "10000";
		global["ifname"] = "eth0";
		global["mysql"] = "mysql://Zebra:Zebra@192.168.1.162:3306/Zebra";
		global["log"] = "debug";
	}
	/**
	 * \brief �ͷ�һЩȫ�ֱ���
	 *
	 */
	static void finalGlobal() __attribute__ ((destructor));
	void finalGlobal()
	{
		SAFE_DELETE(logger);
	}
};

