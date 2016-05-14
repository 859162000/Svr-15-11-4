/**
 * \file
 * \version  $Id: zConfile.h  $
 * \author  
 * \date 
 * \brief �����ļ�����������
 */

#ifndef _ZCONFILE_H_
#define _ZCONFILE_H_
#include <string>
#include "zXMLParser.h"
/**
 * \brief �����ļ�������
 *
 * �������̳�ʹ�á�����ʵ����ȫ�ֲ����Ľ������Ϊ\<global\>\</global\>
 * ���ѽ����Ĳ���������һ��ȫ�ֵĲ�������global�С�
 *
 * ����û����Լ�������,�û�Ӧ��ʵ���Լ��Ĳ���������
 *
 */
class zConfile
{
	protected:
		/**
		 * \brief xml������
		 */
		zXMLParser parser;
		/**
		 * \brief �����ļ�����
		 *
		 */
		std::string confile;

		bool globalParse(const xmlNodePtr node);
		bool parseNormal(const xmlNodePtr node);
		bool parseSuperServer(const xmlNodePtr node);
		virtual bool parseYour(const xmlNodePtr node)=0;

	public:
		zConfile(const char *confile="config.xml");
		virtual ~zConfile();
		bool parse(const char *name);
};
#endif
