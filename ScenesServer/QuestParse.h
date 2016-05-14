/**
 * \file
 * \version	$Id: QuestParse.h  $
 * \author	
 * \date		2005-04-07
 * \brief		�ű��ؼ����б�
 * 
 */
 
#ifndef __QUEST_PARSE_H__
#define __QUEST_PARSE_H__

#include "zXMLParser.h"

#include <string>
#include <sstream>


/**
 * \brief �ؼ����б���
 *
 * �洢�˹ؼ��ʵ��б�,���ṩ�˶�Ӧ�Ĳ���.
 *
 */
class Parse
{
public:

	/**     
	 * \brief ȡ��xml�ڵ�����
	 *
	 * �����ؼ����б�,ȡ�����Ƕ�Ӧ��ֵ
	 *      
	 * \param xml: Ŀ��xml�ļ�
	 * \param node: Ŀ��ڵ�
	 * \return ��ǰ���Ƿ���true
	 */     	
	bool parse (zXMLParser& xml, xmlNodePtr& node)
	{		
		for(iterator it=_kvs.begin(); it!=_kvs.end(); ++it) {
			if (!xml.getNodePropStr(node, it->first.c_str(), it->second)) {	
				_kvs[it->first] = "0";
				//return false;
			}
		}

		return true;
	}
	

	/**     
	 * \brief ����һ���ؼ���
	 *
	 * �ڹؼ����б�������һ���ؼ���,��Ӧ��ֵΪ"0"
	 *      
	 * \param key: �ؼ�������
	 * \return ��
	 */     	
	void key(const std::string& key) 
	{
		_kvs[key] = "0";
	}

	/**     
	 * \brief ȡֵ
	 *
	 * �ڹؼ����б�����Ѱ��Ӧ�Ĺؼ���,�����ض�Ӧ��ֵ,û�ҵ�������ֵ
	 *      
	 * \param key: �ؼ�������
	 * \param value: ȡ�õ�ֵ
	 * \return ��
	 */  
	template <typename T>
	void value(const std::string& key, T& value)
	{
		std::stringstream os(_kvs[key]);	
		os >> value;
	}
	
	
private:
	typedef std::map<std::string, std::string> KV;
	typedef KV::iterator iterator;
	KV _kvs;	
};

#endif
