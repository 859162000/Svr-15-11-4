/**
 * \file
 * \version  $Id: zIncludeXML.h  $
 * \author  
 * \date 
 * \brief includeģʽXML��չ���� 
 */

#ifndef _ZINCLUDEXML_H_
#define _ZINCLUDEXML_H_

#include <string>
#include "zXMLParser.h"
#include "zNoncopyable.h"

/**
 * \brief zIncludeXML��������չ�Զ���XML�Ĺ���
 *
 * include�ڵ��ʽΪ<include filename="xxx.xml" /> xxx.xml����Ҫ������XML�ļ�·��.
 *
 * include�ڵ㲻�ܰ����ӽڵ㣬��Ϊ����չ��ɾ���˽ڵ�.
 *
 * ������̰߳�ȫ
 */
class zIncludeXML:private zNoncopyable
{
	private:
		/**
		 * \brief ���캯�� 
		 */
		zIncludeXML(){};
		/**
		 * \brief �������� 
		 */
		~zIncludeXML(){};
		/**
		 * \brief Ҫ��չXML�Ľ����� 
		 */
		static zXMLParser parser; 
		/**
		 * \brief Ҫ����XML�ļ��Ľ����� 
		 */
		static zXMLParser childparser; 
		static bool expand(zXMLParser *parser,xmlNodePtr parent);
	public:
		static std::string &expand(const std::string &xmlFileName,std::string &xmlStr);
		static std::string &expand(std::string &xmlStr);
};
#endif
