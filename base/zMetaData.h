/**
 * \file zMetaData.h
 * \version  $Id: zMetaData.h  $
 * \author  
 * \date 
 * \brief ���ݿ��ṹ�������������Ķ���
 *
 * 
 */

#ifndef _METADATA_H_
#define _METADATA_H_
#include <mysql.h>
#include <pthread.h>
#include <unistd.h>
#include <zlib.h>
#include <iostream>
#include <ext/hash_map>
#include <sstream>
#include <string>
#include <map>
#include "zDBConnPool.h"


/**
  * \brief �ֶ���Ϣ��
  *
  * ά���ֶ����Ƽ����ͣ�Ŀǰϵͳֻ�ܴ���MYSQL�����б�׼���ͣ�
  *
  * ����֧��Zebra�ж����ZIP,ZIP2��������
  * ����֧�ָ�����ֶ����ͣ����޸�zMysqlDBConnPool::exeSelect, exeUpdate,exeInsert,exeDelete�ĸ�����
  *
  */
  
class Field
{
	public:
		/**
		  * \brief ���췽��
		  *
		  *
		  */
		Field(int fieldType, const std::string& fieldName) 
			: type(fieldType), name(fieldName)
		{
		}
		
		/**
		  * \ brief ��������
		  *
		  */
		~Field(){}

		/// �ֶ�����
		int type;

		/// �ֶ�����
		std::string name;
};


/**
  * \brief �ֶ�����
  *
  * ά��һ����������ֶ�
  *
  */
class FieldSet
{
	public:
		/**
		  * \brief Ĭ�Ϲ��췽��
		  *
		  */
	       	FieldSet(){}

		
		/**
		  * \brief ��ʼ�������Ĺ��췽��
		  *
		  * ��֧����ʽ����ת��
		  */
		explicit FieldSet(const std::string& tbn) : tableName(tbn)
		{
		}
		

		/**
		  * \brief ��������
		  *
		  */
		virtual ~FieldSet();

		/**
	  	  * \brief ȡ�ֶθ���
		  *
		  * \return �����ֶθ���
	          */	  
		unsigned int size();
		

		/**
		  * \brief ����operator[]�����
		  *
		  * \param pos�� ָ���������ĳ���ֶε�λ�� 
		  *
		  * \return ����ҵ����ֶ��򷵻ظ��ֶε�ָ�룬���û�ҵ����򷵻�NULL
		  */
		Field* operator[](unsigned int pos);


		/**
		  * \brief ����operator[]�����
		  *
		  * \param name�� ָ���������ĳ���ֶε�����
		  *
		  * \return ����ҵ����ֶ��򷵻ظ��ֶε�ָ�룬���û�ҵ����򷵻�NULL
		  */
		Field* operator[](const std::string& name);


	
		/**
		  * \brief ȡָ��λ�õ��ֶεķ���
		  *
		  * \param pos: ָ���������ĳ���ֶε�λ��
		  *
		  * \return ����ҵ����ֶ��򷵻ظ��ֶε�ָ�룬���û�ҵ����򷵻�NULL
		  */
		Field* getField(unsigned int pos)
		{
			return fields[pos];
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
		bool addField(int fieldType, const std::string& fieldName);

	        /**
		  * \brief �ṩ��һ������ֶεķ���
		  *
		  *  ����addField
		  */
		bool addField(Field* field);

		/**
		  * \brief ���ø��ֶμ���֮�����ı���
		  *
		  * ��ͨ��Ĭ�Ϲ��캯�����ɵĶ����������ʽ���øú�����������
		  *
		  * \param name������
		  */
		void setTableName(const std::string& name);
		
		/**
		  * \brief �õ�����
		  *
		  * \return ���ر���
		  */
		const char* getTableName() const
		{
			return tableName.c_str();	
		}
	 	
	protected:
		/// �ֶ�����
		std::vector<Field*> fields;

		/// ����
		std::string tableName;	
};


/**
  * \brief ��ṹ������
  *
  * �������Builderģʽ����֧�ֲ�ͬ���ݿ�ı�ṹ����
  *
  */
class MetaData
{
	public:
		/**
		  * \brief builder������ͨ��������������������ɶ�Ӧ��ʵ��
		  *
		  * \param type ���ݿ����ͣ�Ŀǰֻ֧��MYSQL������ʱ����Ϊ�ա�Ҳ������"MYSQL"
		  * 
		  * \return  ���ػ���ָ��
		  */
		static MetaData* newInstance(const char* type);
		
		/**
		  * \brief Ĭ�Ϲ��캯��
		  *
		  */
		MetaData()
		{
		}

		/**
		  * \brief ��������
		  *
		  */
		virtual ~MetaData();
		
		/**
		  * \brief ��ʼ����ṹ
		  *
		  * �������ݿ����ӣ���ȡ�ø����ݿ������б�ı�ṹ
		  *  
		  * \param url:  ���ݿ����Ӵ�
		  */
		virtual bool init(const std::string& url) = 0;
		
		/**
		  * \brief ͨ��ָ����������ȡ�ñ�ı�ṹ
		  *
		  * \param tableName: ����
		  *
		  * \return ����ҵ��ñ����ر�ṹָ��,���򣬷���Ϊ��
		  */
		FieldSet* getFields(const std::string& tableName);

	protected:
		
		typedef std::map<std::string, FieldSet*> TableManager;
		typedef TableManager::iterator TableMember;
		typedef TableManager::value_type valueType;

		/// �ڲ���ṹ������
		TableManager tables;
};


class VarType
{
	public:
		VarType()
		{
			val_us = 0;
			val_ul = 0;
			val_short = 0;
			val_int = 0;
			val_dword = 0;
			val_qword = 0;
			val_sqword = 0;
			val_long = 0;
			
			val_float = 0.0;
			val_double = 0.0;
			val_byte = 0;
		
		        val_pstr = NULL;
			valid = false;
		}
		
		operator unsigned short() const
		{
			return val_us;
		}

		operator short() const
		{
			return val_short;
		}
		
		operator int() const
		{
			return val_int;
		}

		operator unsigned int() const
		{
			return val_dword;
		}

		operator unsigned long() const
		{
			return val_ul;
		}

		operator unsigned long long() const
		{
			return val_qword;
		}

		operator long long() const
		{
			return val_sqword;
		}

		operator long() const
		{
			return val_long;
		}

		operator float() const
		{
			return val_float;
		}

		operator double() const
		{
			return val_double;
		}


		operator const char*() const
		{
			return val_pstr;
		}
		
		operator unsigned char() const
		{
			return val_byte;
		}

		void setValid(bool value)
		{
			valid = value;
		}

		bool isValid()
		{
			return valid;
		}
	
		unsigned short val_us;
		short val_short;
		int val_int;
		unsigned int val_dword;
		unsigned long val_ul;
		unsigned long long val_qword;
		long long val_sqword;
		long val_long;
		float val_float;
		double val_double;
		unsigned char val_byte;
		
	        const char* val_pstr;
		//const unsigned char* val_ustr;

		bool valid;
};
/**
  * \brief ��¼��
  *
  * ά��һ�����ݿ��¼
  */
class Record
{
	public:
		/**
		  * \brief Ĭ�Ϲ��췽��
		  */
		Record()
		{
//			std::cout << "Record" << std::endl;
			fields = NULL;
		}

		/**
		  * \brief ��������
		  */
		virtual ~Record()
		{
//			std::cout << "~Record" << std::endl;
			field.clear();
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
		VarType operator[](const std::string& name);

		

		/**
		  * \brief ����operator[]�����
		  *
		  * ͨ��ָ���е�λ�û�ȡ��ֵ�����Ƽ��ڶ�λ���������Ĵ�����ʹ�ã���Ϊ�е�λ�ò�һ���ǹ̶��ġ�
		  * 
		  * \param idx: ָ����λ��
		  *
		  * \return ���ָ��������ֵ���򷵻���ֵ�����򣬷���ΪNULL
		  */
		const char* operator[](unsigned int idx);
	  
		/**
		  * \brief �����
		  *
		  * ע�⣺�ڶ�����������������ΪNULLֵ������ᵼ�³������
		  *
		  * \param fieldName: �ֶ�����
		  * \param value: �ֶ�ֵ
		  * 
		  */
		template <typename X>
		void put(const char* fieldName, const X& value, bool binary = false)
		{
			if (fieldName == NULL)
			{
				return;
			}

			std::ostringstream oss;
			std::string tempname = fieldName;
			
			std::transform(tempname.begin(), tempname.end(), 
				tempname.begin(),
				::toupper);

			if (binary)
			{
				
			}
			else
			{
				oss << value;
			}

			field_it member = field.find(tempname);

			if (member==field.end())
			{
				field.insert(valType(tempname, oss.str()));
			}
			else
			{
				field.erase(member);
				field.insert(valType(tempname, oss.str()));
			}

		}
		
		/**
		  * \brief �����
		  *
		  * ע�⣺�ڶ�����������������ΪNULLֵ������ᵼ�³������
		  *
		  * \param fieldName: �ֶ�����
		  * \param value: �ֶ�ֵ
		  * 
		  */
		/*template <> void put<const char*>(const char* fieldName, const char* value)
		{
			if (fieldName == NULL)
			{
				return;
			}

			std::ostringstream oss;
			std::string tempname = fieldName;
			
			std::transform(tempname.begin(), tempname.end(), 
				tempname.begin(),
				::toupper);

			if (value)
			{
				oss << value;
			}

			field_it member = field.find(tempname);

			if (member==field.end())
			{
				field.insert(valType(tempname, oss.str()));
			}
			else
			{
				field.erase(member);
				field.insert(valType(tempname, oss.str()));
			}

		}*/

		/**
		  * \brief �����
		  *
		  *  ��Ҫ���ڵ����Record����,SELECTʱ��column,groupby�Ӿ䡣
		  *  ���һ���У�������е�ֵΪ��
		  *
		  * \param fieldName: �ֶ�����
		  * 
		  */
		void put(const char* fieldName);

		/**
		  * \brief ���������
		  *
		  */
		void clear()
		{
			field.clear();
		}

		/**
		  * \brief ��ȡָ���ֶε�ֵ��ͨ�÷���
		  * 
		  * �ɻ�������ֶ����͵�ֵ�������ַ�������ʽ������ֵ��
		  * ���谴�ֶ����ͻ����ֵ���������Ӧ��get����
		  */
		VarType get(const std::string& fieldName);
		

		//bool getBool(const std::string& fieldName);
		//double getDouble(const std::string& fieldName);
		//int    getInt(const std::string& fieldName);

		/**
		  * \brief �ж�ĳ���ֶ��Ƿ���Ч 
		  *
		  * \param fieldName: �ֶ�����

		  * \return ����ü�¼�������ֶΣ�����TRUE,����ΪFALSE
		  */
		bool find(const std::string& fieldName);
		
		/**
		  * \brief ��ȡ�ü�¼������
		  *
		  * \return ���ظü�¼��������Ϊ0��ʾû���С�
		  */
		unsigned int size()
		{
			return field.size();
		}


		FieldSet* fields;

	private:
		typedef std::map<std::string, std::string> FIELD;
		typedef FIELD::value_type valType;
		typedef FIELD::iterator field_it;

		/// �ֶ�-ֵ��
		FIELD field;
};


class RecordSet
{
	public:
		/**
		  * \brief Ĭ�Ϲ��캯��
		  *
		  */
		RecordSet()
		{
//			std::cout << "RecordSet" << std::endl;
		}

		/**
		  * \brief ��������
		  *
		  */
		virtual ~RecordSet();
		
		/**
		  * \brief ����operator[]�����
		  *
		  * ͨ��ָ������������ȡ��Ӧ�ļ�¼
		  *
		  * \param idx:ָ��������
		  *
		  * \return ���ָ����������Ч���򷵻���Ӧ�ļ�¼ָ�룬�����Ч���򷵻�NULL
		  */
		Record* operator[](unsigned int idx);
		
		/**
		  * \brief ��ȡ��¼��
		  *
		  * \return ���ؼ�¼�������û�м�¼������Ϊ0
		  */
		unsigned int size();

		/**
		  * \brief ��ȡ��¼��
		  *
		  * \return ���ؼ�¼�������û�м�¼������Ϊ0
		  */
		bool empty(){return recordSet.empty();}

		/**
		  * \brief ��Ӽ�¼
		  *
		  */
		void put(Record* rec);


		/**
		  * \brief ������м�¼
		  *
		  */
		void clear()
		{
			recordSet.clear();
		}
		
		/**
		  * \brief ��ȡָ������
		  *
		  * ���������ص�operator[]�������ͬ��
		  */
		Record* get(unsigned int idx);

	private:
		/// ��¼��
		std::vector<Record*>    recordSet;
};

#endif

