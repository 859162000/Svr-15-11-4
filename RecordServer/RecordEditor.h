#ifndef _USE_CMD_NAMESPACE
#define _USE_CMD_NAMESPACE
#endif

#include<mysql.h>
//#include<iostream>
#include<string.h>
#include"zType.h"
#include"Command.h"
#include"zNullCmd.h"
#include"Zebra.h"
#include"RecordCommand.h"
#include"Object.h"
#include"zObject.h"
#include "LiveSkill.h"
#include "Skill.h"
//#include"RecordCommand.h" 

#define  MAX_UZLIB_CHAR   (200 * 1024) 
#define MAX_NAMESIZE 32
#define MAX_SIZE 300
const unsigned int PACKET_MASK           =   0x0000ffff;                 
const unsigned int MAX_DATABUFFERSIZE    =   (PACKET_MASK + 1);          
const unsigned int MAX_DATASIZE          =   (MAX_DATABUFFERSIZE - 4);

#pragma pack()
struct Data{
    DWORD size;
    char data[0];
};


class RecordEditor{
    public:
	RecordEditor(){}
	RecordEditor(char * querystring ){                       
	    strcpy(QueryString,querystring);
	}
	int MysqlInit();
	int Query();                              //��ALLBINARY��������ݽ��в���
        void Index();                             //��������ALLBINARY
        //void Modify(){}
	~RecordEditor(){
	    mysql_close(&Mysql);
	}
    
    private:
	MYSQL  Mysql;
	char  QueryString[MAX_SIZE];                                    //Query()�����е�SQL���
        unsigned char uzBuf[MAX_UZLIB_CHAR];                            //��Ž�ѹ��ALLBINARY
	ZlibObject *zo;                            
	SaveObject *object;                                             //ָ���һ��SaveObject
	ZlibSkill *zs;
	SaveSkill * skill;                                              //ָ���һ��SaveSkill
        ZlibSkill *zls;
	LiveSkill* live_skill;                                          //ָ���һ��LiveSkill
        int Uncompress(const unsigned char * data ,const DWORD dataSize);
        void Modify();                                                  //��ALLBINARY�д�ŵ����ݽ����޸�
        void AddObject(SaveObject * Obj);                               //���һ��SaveObject(��Modify()��ʹ��)
        void DelObject(SaveObject * Obj);                               //ɾ��һ��SaveObject(��Modify()��ʹ��)
        void AddSkill(SaveSkill * Skl);                                 //���һ��SaveSkill(��Modify()��ʹ��)
        void DelSkill(SaveSkill * Skill);                               //ɾ��һ��SaveSkill(��Modify()��ʹ��)
        void AddLiveSkill(LiveSkill * Live_Skill);                      //���һ��LiveSkill(��Modify()��ʹ��)
        void DelLiveSkill(LiveSkill * Live_Skill);                      //ɾ��һ��LiveSkill(��Modify()��ʹ��)
};
