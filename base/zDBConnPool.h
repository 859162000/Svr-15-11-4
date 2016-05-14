#ifndef _ZDBCONNPOOL_H_
#define _ZDBCONNPOOL_H_

#include "zType.h"
#include <mysql.h>
#include <string.h>
#include <iostream>

#define MAX_HOSTSIZE	32
#define MAX_USERSIZE	32
#define MAX_DBSIZE		32

/**
 *
 * \brief ���ݿ��ֶ������ṹ���Ͷ���
 * 
 * ���ṹ������Ҫ���������ݵ����ݿ������ֶ�,���������������{NULL,0��0}��Ϊ����������ǡ�
 *
 * ע�⣺���type��DB_BIN2����DB_ZIP2����ô��С�����ǻ��������С��
 * 
 * ���ӣ�
 *
 *	dbCol mycountCol_define[]=
 *
 *	{
 *
 *		{"COUNT(*)",DB_DWORD,sizeof(DWORD)},
 *
 *		{NULL, 0, 0}
 *
 *	};
 */
typedef struct
{
	const char *name;	/**< �ֶ����� */
	int type;			/**< ZEBRA�������� */
	unsigned int size;	/**< ���ݴ�С */
} dbCol;

class Record;
class FieldSet;
class RecordSet;
/**
 * \brief ��ϣ���뺯�����Ͷ���
 * 
 * �û����Ը����Լ�����Ҫд�Լ��Ĺ�ϣ�������Ա�����Ӧ�û���������ݿ���в�����
 */
typedef unsigned int(* hashCodeFunc)(const void *data);

/**
 * \brief ���Ӿ��,�û�����ʹ��,ֻ�ܴ����ӳ��еõ�
 */
typedef unsigned int connHandleID;

/**
 * \brief �������ӳؽӿڶ���
 *
 * �����ṩ�˶����ݿ�ļ򵥵Ļ�������,����UPDATE,INSERT,SELECT,DELETE,ִ��SQL����.
 *
 *  �û�ֻ��Ҫ����Ҫ���������ݿ����ݣ����ɷ��ʡ�
 *
 * ���ѡ�ò�ͬ���ݿ�,����ʵ������ӿ�,Ŀǰ�ṩ��Mysql��ʵ��.
 */
class zDBConnPool
{
	public:
		/**
		 * \brief ���ݿ�֧�ֵ���������
		 */
		enum
		{
			DB_BYTE,		/**< BYTE���� 1�ֽڳ��� */
			DB_CHAR,		/**< CHAR���� 1�ֽڳ��� */
			DB_WORD,		/**< WORD���� 2�ֽڳ��� */
			DB_DWORD,		/**< DWORD���� 4�ֽڳ��� */
			DB_QWORD,		/**< QWORD���� 8�ֽڳ��� */
			DB_STR,			/**< �ַ������� */
			DB_BIN,			/**< �������������� */
			DB_ZIP,			/**< zipѹ���������� */
			DB_BIN2,		/**< ��չ�������������� */
			DB_ZIP2			/**< ��չzipѹ���������� */
		};

		/**
		 * \brief �½���һ�����ӳص�ʵ��
		 *
		 * ���ӿ�û��Ĭ�ϵĹ��캯����Ϊ��ʹ����������ĵײ����ݿ�Ĳ��졣�ṩ�˽ӿڡ�
		 * \param hashfunc ��ϣ����ָ�룬���ΪNULL���ӿ�ʵ����Ӧ���ṩĬ�Ϻ�����
		 * \return ��������ӿڵ�һ��ʵ�������󷵻�NULL��
		 */
		static zDBConnPool *newInstance(hashCodeFunc hashfunc);

		/**
		 * \brief ����һ�����ӳص�ʵ��
		 *
		 * ���ӿ�û��Ĭ�ϵ�����������Ϊ��ʹ����������ĵײ����ݿ�Ĳ��졣�ṩ�˽ӿڡ�
		 * \param delThisClass Ҫ���յ����ӳ�ʵ����
		 */
		static void delInstance(zDBConnPool **delThisClass);

		/**
		 * \brief ���������ֶ��������������ֶεĴ�С
		 *
		 * \param column �����ֶ�����ָ�롣
		 * \return �����ֶδ�С��
		 */
		static unsigned int getColSize(dbCol* column);

		/**
		 * \brief �õ������ֶε������ַ��� 
		 * \param type ����
		 * \return �����ַ���
		 */
		static const char *getTypeString(int type);

		/**
		 * \brief ��ӡ���������Ͷ�������  
		 * \param column ���ݶ���ָ��
		 */
		static void dumpCol(const dbCol *column);

		/**
		 * \brief �ӿ������麯��
		 */
		virtual ~zDBConnPool(){};

		/**
		 * \brief �����ӳ���������ݿ�����URL�������ô������Ƿ�֧������
		 *
		 * \param hashcode ����������Ӧ�Ĺ�ϣ���룬ʹ������Ҫָ����
		 * \param url ���ݿ����ӵ�url
		 * \param supportTransactions �������Ƿ�ֻ������
		 * \return �ɹ�����true�����򷵻�false
		 */
		virtual bool putURL(unsigned int hashcode,const char *url,bool supportTransactions) =0;

		/**
		 * \brief ����hashData�õ�����Handle
		 * 
		 * \param hashData ���ӳ��ô˲�����Ϊ����hashCodeFunc�Ĳ���������hashcode�������õ���Ӧ�����ݿ����ӡ�
		 * \return ���ݿ����Ӿ��,-1��ʾ��Ч���
		 */
		virtual connHandleID getHandle(const void *hashData=NULL) =0;

		/**
		 * \brief ���ݵ�ǰHandle�õ���һ��Handle�����������в�ͬURL��db����
		 *
		 * \param handleID ��ǰ�����Ӿ��
		 * \return ��һ�����Ӿ����-1��ʾû�в�ͬ���Ӿ����
		 */
		virtual connHandleID getNextHandle(connHandleID handleID) =0;

		/**
		 * \brief ��Handle�Ż����ӳ�
		 *
		 * �û���ʹ�������ݿ����Ӿ����Ӧ�ý���Ż����ӳأ��ѱ��´�ʹ�á�
		 * \param handleID �Ż����ӳص����Ӿ��
		 */
		virtual void putHandle(connHandleID handleID) =0;

		/**
		 * \brief ִ��Sql���,����db_real_query�ķ��ؽ��
		 *
		 * Ϊ���ṩ���������ݿ�������ṩ�˱�����
		 * \param handleID ���������Ӿ��
		 * \param sql Ҫִ�е�SQL���
		 * \param sqllen SQL���ĳ���
		 * \return ����ִ����������Ĵ��룬����������ݿ�ķ���ֵ�й�
		 */
		virtual int execSql(connHandleID handleID, const char *sql,unsigned int sqllen) =0;

		/**
		 * \brief ִ��SELECT SQL
		 *
		 * \param handleID ���������Ӿ��
		 * \param tableName Ҫ�����ı���
		 * \param column Ҫ�����������ֶ���������{NULL,0,0}Ϊ��β���
		 * \param where SQL��where���ʽ,û��ʱ��NULL
		 * \param order SQL��order���ʽ,û��ʱ��NULL
		 * \param data SELECT��Ľ�����ݴ洢��λ�ã��������ֵ����0��������Ӧ���ͷ�*data�ڴ�ռ�
		 * \return ����ֵΪ����ĸ�����������󷵻�-1
		 */
		virtual unsigned int exeSelect(connHandleID handleID, const char* tableName,
				const dbCol *column, const char *where,const char *order ,unsigned char **data) =0;
		
		/**
		 * \brief ִ��SELECT SQL
		 *
		 *  ʹ�÷����ɲμ�test/NewMySQLTest�е�ʹ�ô������� 
		 *
		 * \param handleID ���������Ӿ��
		 * \param table Ҫ�����ı�ṹ����ͨ��MetaData.getFieldsȡ��
		 * \param column Ҫ�����������ֶ���������ָ��ʱΪ���������ֶ�"*" 
		 * \param where SQL��where����,û��ʱ��NULL
		 * \param order SQL��order����,û��ʱ��NULL���ɲ���д
		 * \param limit ���ؽ����������ƣ�Ϊ0ʱ��Ϊ�����ƣ���ɲ���д
		 * \param groupby SQL�е�GROUPBY�Ӿ�������δ��ʱ���ɲ���д��Ҳ����ΪNULL
		 * \param having SQL�е�HAVING�Ӿ�������δ��ʱ���ɲ���д��Ҳ����ΪNULL
		 *
		 * \return ���ؽ����
		 *
		 * \author zjw
		 */

		virtual RecordSet* exeSelect(connHandleID handleID, FieldSet* table, Record* column,
				Record* where, Record* order = NULL, 
				unsigned int limit=0,
				Record* groupby = NULL, Record* having = NULL) = 0;
		
		/*virtual unsigned int  exeSelect(connHandleID handleID, RecordSet* result, FieldSet* table, Record* column,
				Record* where, Record* order = NULL, 
				unsigned int limit=0,
				Record* groupby = NULL, Record* having = NULL) = 0;*/


		
		/**
		 * \brief ִ��SELECT SQL,�����Ʒ��ؽ���ĸ���
		 * 
		 * \param handleID ���������Ӿ��
		 * \param tableName Ҫ�����ı���
		 * \param column Ҫ�����������ֶ���������{NULL,0,0}Ϊ��β���
		 * \param where SQL��where���ʽ,û��ʱ��NULL
		 * \param order SQL��order���ʽ,û��ʱ��NULL
		 * \param limit ���ؽ�����������
		 * \param data SELECT��Ľ�����ݴ洢��λ��,dataӦ�����㹻�Ŀռ�洢���صĽ��
		 * \return ����ֵΪ����ĸ�����������󷵻�-1
		 */
		virtual unsigned int exeSelectLimit(connHandleID handleID, const char* tableName,
				const dbCol *column, const char *where,const char *order ,unsigned int limit,unsigned char *data ,unsigned int limit_from = 0) =0;

		/**
		 * \brief ִ��SELECT SQL,�����Ʒ��ؽ���ĸ���
		 * 
		 * \param handleID ���������Ӿ��
		 * \param sql ��׼��ѯsql���
		 * \param sqlen sql����
		 * \param cloumn ��Ң���ص��нṹ
		 * \param limit ���ؽ�����������
		 * \param data SELECT��Ľ�����ݴ洢��λ��,dataӦ�����㹻�Ŀռ�洢���صĽ��
		 * \return ����ֵΪ����ĸ�����������󷵻�-1
		 */
		virtual unsigned int execSelectSql(connHandleID handleID, const char *sql,
				unsigned int sqllen ,const dbCol *column,unsigned int limit,unsigned char *data)=0;
		/**
		 * \brief ��data��ӽ����ݿ�
		 *
		 * ��������֤��ԭ�Ӳ���
		 *
		 * \param handleID ���������Ӿ��
		 * \param tableName Ҫ�����ı���
		 * \param column Ҫ�����������ֶ���������{NULL,0,0}Ϊ��β���
		 * \param data Ҫ�����������ֶ�
		 * \return �������ݵ���������ֵΪ�������ִ�к�ΪAUTO_INCREMENT column��������ID,���Ϊ-1��ʾ��������
		 * ��Ȼ������β���û��AUTO_INCREMENT column,����ֵ���ڵ���0��û�������
		 */
		virtual unsigned int exeInsert(connHandleID handleID, const char *tableName,const dbCol *column,const unsigned char *data) =0;

		/**
		 * \brief ����һ����¼
		 *
		 * ��������֤��ԭ�Ӳ���
		 *
		 * \param handleID ���������Ӿ��
		 * \param table Ҫ�����ı�ṹ��ͨ��MetaData::getFields���
		 * \param rec Ҫ�����������ֶ�
		 *
		 * \return �������ݵ���������ֵΪ�������ִ�к�ΪAUTO_INCREMENT column��������ID,���Ϊ-1��ʾ��������
		 * ��Ȼ������β���û��AUTO_INCREMENT column,����ֵ���ڵ���0��û�������
		 */
		virtual unsigned int exeInsert(connHandleID handleID, FieldSet* table, Record* rec) = 0;
		
		/**
		 * \brief ִ��ɾ������
		 *
		 * \param handleID ���������Ӿ��
		 * \param tableName Ҫ�����ı���
		 * \param where ɾ��������
		 * \return ������Ӱ��ļ�¼��,����-1��ʾ�д�����
		 */
		virtual unsigned int exeDelete(connHandleID handleID, const char *tableName, const char *where) =0;
		
		/**
		 * \brief ִ��ɾ������
		 *
		 * \param handleID ���������Ӿ��
		 * \param table Ҫ�����ı�ṹ��ͨ��MetaData::getFields���
		 * \param where ɾ��������
		 *
		 * \return ������Ӱ��ļ�¼��,����-1��ʾ�д�����
		 */
		virtual unsigned int exeDelete(connHandleID handleID, FieldSet* table, Record* where) = 0;

		/**
		 * \brief ��������
		 * \param handleID ���������Ӿ��
		 * \param tableName Ҫ�����ı���
		 * \param column Ҫ�����������ֶ���������{NULL,0,0}Ϊ��β���
		 * \param data Ҫ�����������ֶ�
		 * \param where ��������
		 * \return ������Ӱ��ļ�¼��,����-1��ʾ�д�����
		 */
		virtual unsigned int exeUpdate(connHandleID handleID, const char *tableName,const dbCol *column,const unsigned char *data, const char *where) =0;
		
		/**
		 * \brief ��������
		 * \param handleID ���������Ӿ��
		 * \param table Ҫ�����ı�ṹ��ͨ��MetaData::getFields���
		 * \param data  Ҫ���µ��ֶμ�ֵ
		 * \param where ������������
		 *
		 * \return ������Ӱ��ļ�¼��,����-1��ʾ�д�����
		 */
		virtual unsigned int exeUpdate(connHandleID handleID, FieldSet* table, Record* data, Record* where) = 0;

		/**
		 * \brief ת���ַ���Ϊ��Ч��db�ַ���
		 * \param handleID ���������Ӿ��
		 * \param src ����Դ����
		 * \param dest ת�����ַ�������ŵĿռ�,Ϊ�˳���İ�ȫ��Ӧ��Ϊdest����(size==0?strlen(src):size)*2+1�Ŀռ�
		 * \param size ���size>0,��ʾת��ָ�����ȵ��ַ��������ڶ��������ݵ�ת�������Ϊ0��ʾһ���ַ�����ת��
		 * \return ʧ�ܷ���NULL,�ɹ�����dest
		 */
		virtual char * escapeString(connHandleID handleID, const char *src,char *dest,unsigned int size) =0;

		/**
		 * \brief ת���ַ���Ϊ��Ч��db�ַ���
		 * \param handleID ���������Ӿ��
		 * \param src ����Դ����
		 * \param dest ת�����ַ�������ŵĿռ�,Ϊ�˳���İ�ȫ��Ӧ��Ϊdest����(size==0?strlen(src):size)*2+1�Ŀռ�
		 * \param size ���size>0,��ʾת��ָ�����ȵ��ַ��������ڶ��������ݵ�ת�������Ϊ0��ʾһ���ַ�����ת��
		 * \return ʧ�ܷ���NULL,�ɹ�����dest
		 */
		virtual std::string& escapeString(connHandleID handleID, const std::string &src, std::string &dest) =0;

		/**
		 * \brief ��ȡ���м�¼����
		 * \param handleID ���������Ӿ��
		 * \param tableName Ҫ�����ı���
		 * \param where ��������
		 * \return ���ؼ������
		 */
		virtual unsigned int getCount(connHandleID handleID, const char* tableName, const char *where) =0;

		/**
		 * \brief �ѱ���ĳ��ʱ���ֶθ��µ�����ʱ��
		 * \param handleID ���������Ӿ��
		 * \param tableName Ҫ�����ı���
		 * \param colName Ҫ�������ֶ���
		 */
		virtual void updateDatatimeCol(connHandleID handleID, const char* tableName, const char *colName) =0;

		/**
		 * \brief �����ύ
		 * \param handleID ���������Ӿ��
		 * \return �ɹ�����true��ʧ�ܷ���false
		 */
		virtual bool commit(connHandleID handleID) =0;


		/**
		 * \brief ����ع�
		 * \param handleID ���������Ӿ��
		 * \return �ɹ�����true��ʧ�ܷ���false
		 */
		virtual bool rollback(connHandleID handleID) =0;

		/**
		 * \brief ���ô������Ƿ�֧������
		 * \param handleID ���������Ӿ��
		 * \param supportTransactions  �Ƿ�֧������
		 * \return �ɹ�����true��ʧ�ܷ���false
		 */
		virtual bool setTransactions(connHandleID handleID, bool supportTransactions) =0;

		/**
		 * \brief ���������Ƿ�֧������
		 * \param handleID ���������Ӿ��
		 * \return ֧�ַ���true�����򷵻�false
		 */
		virtual bool supportTransactions(connHandleID handleID) =0;
};

struct UrlInfo
{
	const unsigned int hashcode;
	const std::string url;
	const bool supportTransactions;

	char host[MAX_HOSTSIZE];
	char user[MAX_USERSIZE];
	char passwd[MAX_PASSWORD];
	unsigned int port;
	char dbName[MAX_DBSIZE];

	UrlInfo()
		: hashcode(0),url(),supportTransactions(false) {};
	UrlInfo(const unsigned int hashcode,const std::string &url,const bool supportTransactions)
		: hashcode(hashcode),url(url),supportTransactions(supportTransactions)
		{
			parseMySQLURLString();
		}
	UrlInfo(const UrlInfo &ui)
		: hashcode(ui.hashcode),url(ui.url),supportTransactions(ui.supportTransactions)
		{
			parseMySQLURLString();
		}

	private:
	void parseMySQLURLString()
	{
		bzero(host,sizeof(host));
		bzero(user,sizeof(user));
		bzero(passwd,sizeof(passwd));
		port=3306;
		bzero(dbName,sizeof(dbName));

		char strPort[16] = "";
		int  j, k;
		size_t i;
		const char *connString = url.c_str();
		if (0 == strncmp(connString, "mysql://", strlen("mysql://")))
		{
			i = 0; j = 0; k = 0;
			for(i = strlen("mysql://"); i < strlen(connString) + 1; i++)
			{
				switch(j)
				{
					case 0:
						if (connString[i] == ':')
						{
							user[k] = '\0'; j++; k = 0;
						}
						else
							user[k++] = connString[i];
						break;
					case 1:
						if (connString[i] == '@')
						{
							passwd[k] = '\0'; j++; k = 0;
						}
						else
							passwd[k++] = connString[i];
						break;
					case 2:
						if (connString[i] == ':')
						{
							host[k] = '\0'; j++; k = 0;
						}
						else
							host[k++] = connString[i];
						break;
					case 3:
						if (connString[i] == '/')
						{
							strPort[k] = '\0'; j++; k = 0;
						}
						else
							strPort[k++] = connString[i];
						break;
					case 4:
						if (connString[i] == '\0')
						{
							dbName[k] = '\0'; j++; k = 0;
						}
						else
							dbName[k++] = connString[i];
						break;
					default:
						break;
				}
			}
		}
		port=atoi(strPort);
	}
};

#endif
