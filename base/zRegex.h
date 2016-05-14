/**
 * \file
 * \version  $Id: zRegex.h  $
 * \author  
 * \date 
 * \brief ������ʽ������
 */

#ifndef _ZREGEX_H_
#define _ZREGEX_H_
#include <regex.h>
#include <string>

/**
 * \brief ������ʽ�࣬��regex�����˷�װ������������ʽ��ο�man 7 regex.
 *
 * ����֧�����ַ���ƥ�䣬�����֧��31���ִ�
 *
 * ������̰߳�ȫ
 */
class zRegex
{
	private:
		/**
		 * \brief ������Ϣ��Ŵ�
		 */
		std::string errstr;
		/**
		 * \brief �������
		 */
		int errcode;
		/**
		 * \brief ������ʽ���
		 */
		regex_t preg;
		/**
		 * \brief Ҫƥ����ַ��� 
		 */
		std::string smatch;
		/**
		 * \brief ���ʽ�Ƿ��ѱ��� 
		 */
		bool compiled;
		/**
		 * \brief �Ƿ�ƥ�� 
		 */
		bool matched;
		/**
		 * \brief �Ӵ�ƥ��λ�� 
		 */
		regmatch_t rgm[32];

		/**
		 * \brief �Զ���������:��Ǵ��� 
		 */
		static const int REG_FLAGS;
		/**
		 * \brief �Զ���������:δ�������
		 */
		static const int REG_COMP;
		/**
		 * \brief �Զ���������:δ֪����
		 */
		static const int REG_UNKNOW;
		/**
		 * \brief �Զ���������:δ����ƥ����� 
		 */
		static const int REG_MATCH;
	public:
		/**
		 * \brief �Զ�����:֧�ֶ���ƥ�䣬Ĭ�ϲ�֧��
		 */
		static const int REG_MULTILINE;
		/**
		 * \brief �Զ�����:Ĭ�ϱ��
		 */
		static const int REG_DEFAULT;
		zRegex();
		~zRegex();
		bool compile(const char * regex,int flags=REG_DEFAULT);
		bool match(const char *s);
		std::string &getSub(std::string &s,int sub=0);
		const std::string & getError();
};
#endif
