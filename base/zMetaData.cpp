/**
 * \file zMetaData.cpp
 * \version  $Id: zMetaData.cpp  $
 * \author  
 * \date 
 * \brief ��ṹ��������������ʵ��
 *
 * 
 */


#include <mysql.h>
#include <pthread.h>
#include <unistd.h>
#include <zlib.h>
#include <iostream>
#include <ext/hash_map>
#include <sstream>

#include "zDBConnPool.h"
#include "zType.h"
#include "Zebra.h"
#include "zMutex.h"
#include "zNoncopyable.h"

#include "zMetaData.h"
//MetaData* MetaData::instance = NULL;
using namespace Zebra;


/**
  * \brief ��������
  *
  */
MetaData:: ~MetaData()
{
	TableMember it;
	for (it=tables.begin(); it!=tables.end(); it++)
	{
		FieldSet* temp = it->second;
		SAFE_DELETE(temp);
	}	
}

class MySQLMetaData : public MetaData
{
	/**
	  * \brief ��ʼ����ṹ
	  *
	  * �������ݿ����ӣ���ȡ�ø����ݿ������б�ı�ṹ
	  *  
	  * \param url:  ���ݿ����Ӵ�
	  */
	bool init(const std::string& url)
	{
		UrlInfo urlInfo(0, url, false);	
		if (!this->loadMetaDataFromDB(urlInfo))
		{
			return false;
		}

		// TODO:����һЩ��Ҫ��ʼ���Ĵ���д������
		return true;
	}	

	/**
	  * \brief ͨ��ָ�������ӣ��������ݱ�ṹ
	  *
	  */
	bool loadMetaDataFromDB(const UrlInfo& url)
	{
		MYSQL* mysql_conn = NULL;
		MYSQL_RES* table_res = NULL;

		mysql_conn=mysql_init(NULL);

		if(mysql_conn==NULL)
		{
			logger->error("Initiate mysql error...");
			return false;
		}

		if(mysql_real_connect(mysql_conn,url.host,url.user,url.passwd,url.dbName,url.port,NULL,CLIENT_COMPRESS|CLIENT_INTERACTIVE)==NULL)
		{
			logger->error("loadMetaDataFromDB():connect mysql://%s:%u/%s failed...",url.host,url.port,url.dbName);
			logger->error("loadMetaDataFromDB():reason: %s",mysql_error(mysql_conn));
			return false;
		}

		logger->info("loadMetaDataFromDB():connect mysql://%s:%u/%s successful...",url.host,url.port,url.dbName);

		if (mysql_conn)
		{
			if ((table_res = mysql_list_tables(mysql_conn, NULL)) == NULL)
			{
				logger->error("loadMetaDataFromDB():mysql_list_tables fail.");
				logger->error("loadMetaDataFromDB():reason: %s",mysql_error(mysql_conn));
				if (table_res) mysql_free_result(table_res);
				if (mysql_conn) mysql_close(mysql_conn);
				return false;
			}
			MYSQL_ROW row;

			while ((row=mysql_fetch_row(table_res)))
			{
				this->addNewTable(mysql_conn, row[0]);
			}

			mysql_free_result(table_res);
		}

		if (mysql_conn) mysql_close(mysql_conn);

		return true;
	}
	
	/**
	  * \brief ����һ���±�
	  */
	bool addNewTable(MYSQL* mysql_conn, const char* tableName)
	{
		MYSQL_RES* field_res = NULL;

		/*char query_string[20+strlen(tableName)+1];
		bzero(query_string, sizeof(query_string));

		snprintf(query_string, sizeof(query_string), "SELECT * FROM  `%s`", tableName);

		//mysql_real_escape_string();
		if (mysql_real_query(mysql_conn, query_string, strlen(query_string)) != 0)
		{
			logger->error("��ѯ%sʧ��", tableName);
			return false;
		}

		field_res = mysql_store_result(mysql_conn);
		*/
		field_res = mysql_list_fields(mysql_conn, tableName, NULL);

		if (field_res)
		{
			unsigned int num_fields = mysql_num_fields(field_res);
			MYSQL_FIELD* mysql_fields = NULL;
			mysql_fields = mysql_fetch_fields(field_res);

			FieldSet *fields = new FieldSet(tableName);

			if (!fields)
			{
				mysql_free_result(field_res);
				return false;
			}

			for (unsigned int i=0; i<num_fields; i++)
			{
				if (!fields->addField(mysql_fields[i].type, mysql_fields[i].name))
				{
					mysql_free_result(field_res);
					return false;
				}
			}

			tables.insert(valueType(tableName, fields));
			mysql_free_result(field_res);
		}
		else
		{
			return false;
		}

		return true;	
	}

};

/**
  * \brief ȡ�ֶθ���
  *
  * \return �����ֶθ���
  */	  
unsigned int FieldSet::size()
{
	return fields.size();
}

/**
  * \brief ����operator[]�����
  *
  * \param pos�� ָ���������ĳ���ֶε�λ�� 
  *
  * \return ����ҵ����ֶ��򷵻ظ��ֶε�ָ�룬���û�ҵ����򷵻�NULL
  */
Field* FieldSet::operator[] (unsigned int pos)
{
	if (pos<0 || pos>=fields.size())
	{
		return NULL;
	}

	return fields[pos];
}

/**
  * \brief ����operator[]�����
  *
  * \param pos�� ָ���������ĳ���ֶε�����
  *
  * \return ����ҵ����ֶ��򷵻ظ��ֶε�ָ�룬���û�ҵ����򷵻�NULL
  */
Field* FieldSet::operator[](const std::string& name)
{
	for (unsigned int i=0; i<fields.size(); i++)
	{
		Field* ret = fields.at(i);
		
		if (ret)
		{
			if (ret->name == name)
			{
				return ret;
			}
		}
	}
	
	return NULL;
}


/**
  * \brief �����µ��ֶ�
  *
  *  �ֶ�����Ŀǰ֧����������:
  *
  *  FIELD_TYPE_TINY TINYINT field
  *  FIELD_TYPE_SHORT SMALLINT field
  *  FIELD_TYPE_LONG INTEGER field
  *  FIELD_TYPE_INT24 MEDIUMINT field
  *  FIELD_TYPE_LONGLONG BIGINT field
  *  FIELD_TYPE_DECIMAL DECIMAL or NUMERIC field
  *  FIELD_TYPE_FLOAT FLOAT field
  *  FIELD_TYPE_DOUBLE DOUBLE or REAL field
  *  FIELD_TYPE_TIMESTAMP TIMESTAMP field
  *  FIELD_TYPE_DATE DATE field
  *  FIELD_TYPE_TIME TIME field
  *  FIELD_TYPE_DATETIME DATETIME field
  *  FIELD_TYPE_YEAR YEAR field
  *  FIELD_TYPE_STRING CHAR field
  *  FIELD_TYPE_VAR_STRING VARCHAR field
  *  FIELD_TYPE_BLOB BLOB or TEXT field 
  *  FIELD_TYPE_SET SET field
  *  FIELD_TYPE_ENUM ENUM field
  *  FIELD_TYPE_NULL NULL-type field
  *  FIELD_TYPE_CHAR Deprecated; use FIELD_TYPE_TINY instead
  *
  * \param fieldType: �ֶ�����
  * \param fieldName: �ֶ�����
  *
  *
  */
bool FieldSet::addField(int fieldType, const std::string& fieldName)
{
	std::string tempname = fieldName;

	transform(tempname.begin(), tempname.end(),
			tempname.begin(),
			toupper);

	Field* field = new Field(fieldType, tempname);

	if (field)
	{
		fields.push_back(field);
		return true;
	}

	return false;
}

/**
  * \brief �ṩ��һ������ֶεķ���
  *
  *  ����addField
  */
bool FieldSet::addField(Field* field)
{
	if (!field)
	{
		fields.push_back(field);
		return true;
	}

	return false;
}

/**
  * \brief ��������
  *
  * �ͷſռ�
  */
FieldSet::~FieldSet()
{
 	unsigned int num_field = fields.size(); 

	for (unsigned int i=0; i<num_field; i++)
	{
		SAFE_DELETE(fields[i]);
	}
   	
}
/**
  * \brief ͨ��ָ����������ȡ�ñ�ı�ṹ
  *
  * \param tableName: ����
  *
  * \return ����ҵ��ñ����ر�ṹָ��,���򣬷���Ϊ��
  */
FieldSet* MetaData::getFields(const std::string& tableName)
{
	std::string tempname = tableName;

	//transform(tempname.begin(), tempname.end(),
	//		tempname.begin(),
	//		toupper);

	TableMember tm = tables.find(tempname);	
	if (tm!=tables.end())
	{
		return (FieldSet*)(tm->second);
	}
	
	return NULL;
}

/**
  * \brief ����operator[]�����
  *
  * ��ͨ��ָ���ֶ�������ȡ����ֶε�ֵ��
  * ������ֶ�����Ϊ��ֵ�ͣ�ͨ���ú���Ҳ�ɷ�����ֵ��Ӧ�ó���Ա��Ҫ�Լ�������Ӧ��������ת��
  * ������ʽ�����������ƥ���get����
  *
  * \param name: �ֶ����������ִ�Сд
  * 
  * \return ������ֶδ��ڣ��򷵻���ֵ����������ڣ��򷵻�ΪNULL
  */
VarType Record::operator[](const std::string& name)
{
/*	std::string tempname = name;

	transform(tempname.begin(), tempname.end(),
			tempname.begin(),
			toupper);

	field_it it = field.find(tempname);
	
	
	if (it != field.end())
	{
		return it->second.c_str();
	}
	
	return NULL;*/
	return this->get(name);
}

/**
  * \brief ����operator[]�����
  *
  * ͨ��ָ���е�λ�û�ȡ��ֵ�����Ƽ��ڶ�λ���������Ĵ�����ʹ�ã���Ϊ�е�λ�ò�һ���ǹ̶��ġ�
  * 
  * \param idx: ָ����λ��
  *
  * \return ���ָ��������ֵ���򷵻���ֵ�����򣬷���ΪNULL
  */
const char* Record::operator[](unsigned int idx)
{
	field_it it;
	unsigned int i=0;

	for (it = field.begin(); it!=field.end(); i++, it++)
	{
		if (idx == i)
		{
			return it->second.c_str();	
		}
	}

	return NULL;
}

/**
  * \brief �����
  *
  * \param fieldName: �ֶ�����
  * \param value: �ֶ�ֵ
  * 
  */
void Record::put(const char* fieldName)
{
	if (fieldName == NULL)
	{
		return;
	}
	   
	std::string tempname = fieldName;

	transform(tempname.begin(), tempname.end(),
			tempname.begin(),
			toupper);
	
	
	field.insert(valType(tempname, ""));
	
}

/**
  * \brief ��ȡָ���ֶε�ֵ��ͨ�÷���
  * 
  * �ɻ�������ֶ����͵�ֵ�������ַ�������ʽ������ֵ��
  * ���谴�ֶ����ͻ����ֵ���������Ӧ��get����
  */
VarType Record::get(const std::string& fieldName)
{
	std::string tempname = fieldName;
	VarType ret;

	transform(tempname.begin(), tempname.end(),
			tempname.begin(),
			toupper);

	field_it it = field.find(tempname);

	if (it == field.end())
	{
		return ret;
	}
	
	if (fields)
	{
		Field* fl = (*fields)[tempname];

		if (fl)
		{
			ret.setValid(true);

			switch (fl->type)
			{
				case FIELD_TYPE_TINY:
				case FIELD_TYPE_SHORT:
				case FIELD_TYPE_LONG:
				case FIELD_TYPE_INT24:
				case FIELD_TYPE_LONGLONG:
				case FIELD_TYPE_DECIMAL:
					{// �������������ﴦ��
						ret.val_us = atoi(it->second.c_str());
						ret.val_short = atoi(it->second.c_str());
						ret.val_int = atoi(it->second.c_str());
						ret.val_dword = atoi(it->second.c_str());
						ret.val_ul = strtoul(it->second.c_str(), NULL, 10);
						ret.val_qword = strtoull(it->second.c_str(), NULL, 10);
						ret.val_sqword = strtoll(it->second.c_str(), NULL, 10);
						ret.val_long = atol(it->second.c_str());
						ret.val_byte = atoi(it->second.c_str());
						break;
					}
				case FIELD_TYPE_FLOAT:
				case FIELD_TYPE_DOUBLE:
					{//���и����������ﴦ��`
						ret.val_float = atof(it->second.c_str());
						ret.val_double = atof(it->second.c_str());
						break;
					}
				default:
					{// �����������Ͱ��ַ�������
						ret.val_pstr = it->second.c_str();
					}
			}
		}
	}
	else
	{
		ret.setValid(true);
		ret.val_pstr = it->second.c_str();
	}

	return ret;
}

/**
 * \brief �ж�ĳ���ֶ��Ƿ���Ч 
 *
 * \param fieldName: �ֶ�����

 * \return ����ü�¼�������ֶΣ�����TRUE,����ΪFALSE
 */
bool Record::find(const std::string& fieldName)
{
	std::string tempname = fieldName;

	transform(tempname.begin(), tempname.end(),
			tempname.begin(),
			toupper);

	if (field.find(tempname) == field.end())
	{
		return false;
	}

	return true;

}

/**
 * \brief ��������
 */
RecordSet::~RecordSet()
{
//	unsigned int num_record = recordSet.size();
//	std::cout << "~RecordSet" << std::endl;
	for (std::vector<Record*>::iterator pos = recordSet.begin(); pos!=recordSet.end(); pos++)
	{
		SAFE_DELETE(*pos);
	}
}

/**
 * \brief ����operator[]�����
 *
 * ͨ��ָ������������ȡ��Ӧ�ļ�¼
 *
 * \param idx:ָ��������
 *
 * \return ���ָ����������Ч���򷵻���Ӧ�ļ�¼ָ�룬�����Ч���򷵻�NULL
 */
Record* RecordSet::operator[](unsigned int idx)
{
	return this->get(idx);
}

/**
 * \brief ��ȡ��¼��
 *
 * \return ���ؼ�¼�������û�м�¼������Ϊ0
 */
unsigned int RecordSet::size()
{
	return recordSet.size();
}

/**
 * \brief ��Ӽ�¼
 *
 */
void RecordSet::put(Record* rec)
{
	recordSet.push_back(rec);
}

/**
 * \brief ��ȡָ������
 *
 * ���������ص�operator[]�������ͬ��
 */
Record* RecordSet::get(unsigned int idx)
{
	return recordSet[idx];
}

/**
 * \brief builder������ͨ��������������������ɶ�Ӧ��ʵ��
 *
 * \param ���ݿ����ͣ�Ŀǰֻ֧��MYSQL������ʱ����Ϊ�ա�Ҳ������"MYSQL"
 * 
 * \return  ���ػ���ָ��
 */
MetaData* MetaData::newInstance(const char* type)
{
	if (type == NULL || !strcmp(type, "MYSQL"))
		return (new MySQLMetaData());
	else 
		return (new MySQLMetaData());
}

