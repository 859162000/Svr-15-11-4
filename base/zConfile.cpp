/**
 * \file
 * \version  $Id: zConfile.cpp  $
 * \author 
 * \date 
 * \brief �����ļ�����������,
 */

#include <string.h>

#include "zConfile.h"
#include "Zebra.h"
#include "zBase64.h"

/**
 * \brief ���캯��
 * \param confile �����ļ�����
 */
zConfile::zConfile(const char *confile)
{
	this->confile = confile;
}

/**
 * \brief ��������
 */
zConfile::~zConfile()
{
	parser.final();
}

/**
 * \brief ȫ�ֽ�������
 * \param node ȫ�����ýڵ�
 * \return �����Ƿ�ɹ�
 */
bool zConfile::globalParse(const xmlNodePtr node)
{
	xmlNodePtr child=parser.getChildNode(node,NULL);
	while(child)
	{
		if(strcasecmp((char *)child->name,"superserver")==0)
			parseSuperServer(child);
		else
			parseNormal(child);
		child=parser.getNextNode(child,NULL);
	}
	return true;
}

/**
 * \brief ��ͨ��������,ֻ�Ǽ򵥵İѲ�������global������
 * \param node Ҫ�����Ľڵ�
 * \return �����Ƿ�ɹ�
 */
bool zConfile::parseNormal(const xmlNodePtr node)
{
	char buf[128];
	if(parser.getNodeContentStr(node,buf,128))
	{
		Zebra::global[(char *)node->name]=buf;
		if (0 == strcmp((char *)node->name, "mysql")
				&& parser.getNodePropStr(node,"encode",buf,128)
				&& 0 == strcasecmp(buf, "yes"))
		{
			std::string tmpS;
			Zebra::base64_decrypt(Zebra::global[(char *)node->name], tmpS);
			Zebra::global[(char *)node->name]=tmpS;
		}
		return true;
	}
	else
		return false;
}

/**
 * \brief SuperServer��������������global�����з�����������
 *
 * server SuperServer��ַ
 *
 * port SuperServer�˿�
 * \param node SuperServer�����ڵ�
 * \return �����Ƿ�ɹ�
 */
bool zConfile::parseSuperServer(const xmlNodePtr node)
{
	char buf[64];
	if(parser.getNodeContentStr(node,buf,64))
	{
		Zebra::global["server"]=buf;
		if(parser.getNodePropStr(node,"port",buf,64))
			Zebra::global["port"]=buf;
		else
			Zebra::global["port"]="10000";
		return true;
	}
	else
		return false;
}

/**
 * \brief ��ʼ���������ļ�
 *
 * \param name ʹ�����Լ������Ķ���ڵ�����
 * \return �����Ƿ�ɹ�
 */
bool zConfile::parse(const char *name)
{
	if (parser.initFile(confile))
	{
		xmlNodePtr root=parser.getRootNode("Zebra");
		if(root)
		{
			xmlNodePtr globalNode=parser.getChildNode(root,"global");
			if(globalNode)
			{
				if(!globalParse(globalNode))
					return false;
			}
			else
				Zebra::logger->warn("��ȫ�����ö���.");
			xmlNodePtr otherNode=parser.getChildNode(root,name);
			if(otherNode)
			{
				if(!parseYour(otherNode))
					return false;
			}
			else
				Zebra::logger->warn("�� %s ���ö���.",name);
			return true;
		}
	}
	return false;
}
