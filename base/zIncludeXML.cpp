/**
 * \file
 * \version  $Id: zIncludeXML.cpp  $
 * \author  
 * \date 2
 * \brief IncludeXMLʵ��
 */

#include "zType.h"
#include "zIncludeXML.h"
#include <string.h>

zXMLParser zIncludeXML::parser;
zXMLParser zIncludeXML::childparser;

/**
 * \brief ��չXML�ļ� 
 *
 * \param xmlFileName Ҫ��չ��xml�ļ�
 * \param xmlStr ��չ���XML����
 * \return ����xmlStr
 */
std::string &zIncludeXML::expand(const std::string &xmlFileName,std::string &xmlStr)
{
	if(parser.initFile(xmlFileName))
	{
		xmlNodePtr root=parser.getRootNode(NULL);
		if(expand(&parser,root))
			parser.dump(xmlStr);
	}
	return xmlStr;
}

/**
 * \brief ��չXML���� 
 *
 * \param xmlStr ҪXML���ݣ���չ�������Ҳ���ڴ˴�
 * \return ����xmlStr
 */
std::string &zIncludeXML::expand(std::string &xmlStr)
{
	char *xmlstr=new char[xmlStr.size()+1];
	if (xmlstr)
	{
		strncpy(xmlstr,xmlStr.c_str(),xmlStr.size());
		if(parser.initStr(xmlstr))
		{
			xmlNodePtr root=parser.getRootNode(NULL);
			if(expand(&parser,root))
				parser.dump(xmlStr);
		}
		SAFE_DELETE_VEC(xmlstr);
	}
	return xmlStr;
}

/**
 * \brief ��չ
 *
 * \param parser Ҫ��չ��XML������
 * \param parent Ҫ��չ�Ľڵ�
 * \return �ɹ�����true ,���򷵻�false
 */
bool zIncludeXML::expand(zXMLParser *parser,xmlNodePtr parent)
{
	if(parser==NULL || parent == NULL) return false;
	xmlNodePtr child=parser->getChildNode(parent,NULL);
	while(child!=NULL)
	{
		xmlNodePtr nextNode=parser->getNextNode(child,NULL);
		if(parser->getChildNodeNum(child,NULL)>0)
			expand(parser,child);
		else if(strcmp((char *)child->name,"include")==0)
		{
			char buf[1024];
			if(parser->getNodePropStr(child,"filename",buf,1024))
			{
				if(childparser.initFile(buf))
				{
					xmlNodePtr childroot=childparser.getRootNode(NULL);
					xmlAddNextSibling(child,childroot);
				}
				childparser.final();
			}
			xmlUnlinkNode(child);
			xmlFreeNode(child);
		}
		child=nextNode;
	}
	return true;
}
