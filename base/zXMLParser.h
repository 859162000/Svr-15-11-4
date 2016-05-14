/**
 * \file
 * \version  $Id: zXMLParser.h  $
 * \author  
 * \date 
 * \brief zXMLParser�������ļ�
 *
 * 
 */

#ifndef _ZXMLPARSE_H_
#define _ZXMLPARSE_H_
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>

/**
 * \brief zXMLParser����
 * 
 * ��Ҫ�ṩ�˽ڵ�����,�������Եĵõ�.
 */
class zXMLParser
{
	public:
		zXMLParser();
		~zXMLParser();

		bool initFile(const std::string &xmlFile);
		bool initFile(const char *xmlFile);
		bool initStr(const std::string &xmlStr);
		bool initStr(const char *xmlStr);
		bool init();
		void final();
		std::string & dump(std::string & s, bool format=false);
		std::string & dump(xmlNodePtr dumpNode,std::string & s,bool head=true);
		xmlNodePtr getRootNode(const char *rootName);
		xmlNodePtr getChildNode(const xmlNodePtr parent, const char *childName);
		xmlNodePtr getNextNode(const xmlNodePtr node,const char *nextName);
		unsigned int getChildNodeNum(const xmlNodePtr parent, const char *childName);

		xmlNodePtr newRootNode(const char *rootName);
		xmlNodePtr newChildNode(const xmlNodePtr parent, const char *childName, const char *content);
		bool newNodeProp(const xmlNodePtr node,const char *propName,const char *prop);

		bool getNodePropNum(const xmlNodePtr node,const char *propName,void *prop,int propSize);
		bool getNodePropStr(const xmlNodePtr node,const char *propName,void *prop,int propSize);
		bool getNodePropStr(const xmlNodePtr node,const char *propName,std::string &prop);
		bool getNodeContentNum(const xmlNodePtr node,void *content, int contentSize);
		bool getNodeContentStr(const xmlNodePtr node,void *content, int contentSize);
		bool getNodeContentStr(const xmlNodePtr node,std::string &content);
	        bool getNodeContentStr(const xmlNodePtr node,std::string &content,bool head );
	private:
		unsigned char *charConv(unsigned char *in, const char *fromEncoding, const char *toEncoding);
		xmlDocPtr doc;
};
#endif
