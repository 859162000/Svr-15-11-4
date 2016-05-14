/**
 * \file
 * \version  $Id: zRegex.cpp  $
 * \author  
 * \date 
 * \brief ������ʽ�ඨ��
 *
 */

#include "zRegex.h"
#include <iostream>

const int zRegex::REG_UNKNOW(78325);
const int zRegex::REG_FLAGS(78326);
const int zRegex::REG_COMP(78327);
const int zRegex::REG_MATCH(78328);
const int zRegex::REG_MULTILINE(REG_NEWLINE);
const int zRegex::REG_DEFAULT(0);

/**
 * \brief ���캯�� 
 */
zRegex::zRegex()
{
	compiled=false;
	matched=false;
	errcode=REG_UNKNOW;
}

/**
 * \brief �������� 
 */
zRegex::~zRegex()
{
	if(compiled)
	{
		regfree(&preg);
	}
}

/**
 * \brief ������ʽ���뺯��
 *
 * \param regex Ҫ�����������ʽ 
 * \param flags ����ѡ�Ŀǰ֧��#REG_MULTILINE,#REG_DEFAULT,����㲻֪����ʲô������Ĭ��ֵ 
 * \return �����Ƿ�ɹ� 
 */
bool zRegex::compile(const char * regex,int flags)
{
	if(compiled)
	{
		regfree(&preg);
		matched=false;
	}

	if(flags==REG_MULTILINE)
	{
		errcode=regcomp(&preg, regex, REG_EXTENDED);
	}
	else if(flags==REG_DEFAULT)
	{
		errcode=regcomp(&preg, regex, REG_EXTENDED|REG_NEWLINE);
	}
	else
		errcode=REG_FLAGS;

	compiled=(errcode==0);
	return compiled;
}

/**
 * \brief ��ʼƥ���ַ���,��ƥ��ǰ�뱣֤�Ѿ���ȷ������������ʽ#compile
 *
 * \param s Ҫƥ����ַ���
 * \return ƥ���Ƿ�ɹ� 
 */
bool zRegex::match(const char *s)
{
	if(s==NULL) return false;
	smatch=s;
	if(compiled)
	{
		errcode=regexec(&preg,s,32,rgm,0);
	}
	else
		errcode=REG_COMP;
	matched=(errcode==0);
	return matched;
}

/**
 * \brief �õ�ƥ������ַ���,�ڴ�֮ǰ�뱣֤�Ѿ���ȷ�ý���ƥ��#match
 *
 * \param s �õ����ַ���������s��
 * \param sub ���ַ�����λ�á�ע��ƥ����ַ���λ��Ϊ0���������ַ����Դ�����.���ֵΪ31
 * \return ����s 
 */
std::string &zRegex::getSub(std::string &s,int sub)
{
	if(matched)
	{
		if(sub<32 && sub >= 0 && rgm[sub].rm_so!=-1)
		{
			s=std::string(smatch,rgm[sub].rm_so,rgm[sub].rm_eo-rgm[sub].rm_so);
		}
		else
			s="";
	}
	else
		errcode=REG_MATCH;
	return s;
}

/**
 * \brief �õ�������Ϣ 
 * \return ������#compile��#matchʱ����false,�����ô˵õ�������Ϣ
 */
const std::string & zRegex::getError()
{
	if(errcode==REG_UNKNOW)
		errstr="unknow error";
	else if(errcode==REG_FLAGS)
		errstr="flags error";
	else if(errcode==REG_COMP)
		errstr="uncompiled error";
	else if(errcode==REG_MATCH)
		errstr="unmatched error";
	else
	{
		char temp[1024];
		regerror(errcode,&preg,temp,1023);
		errstr=temp;
	}
	return errstr;
}
