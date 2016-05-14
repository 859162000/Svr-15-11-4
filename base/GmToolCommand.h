#ifndef _GM_TOOL_COMMAND_
#define _GM_TOOL_COMMAND_

#include "zNullCmd.h"
#pragma pack(1)

namespace Cmd
{
	namespace GmTool
	{
		const BYTE CMD_GMTOOL = 128;

		//������Ϣ,Gateway->Super->Info->GmTool
		const BYTE PARA_CHAT_GMTOOL = 1;
		struct t_Chat_GmTool : t_NullCmd
		{
			GameZone_t zone;
			char userName[MAX_NAMESIZE];
			char gmName[MAX_NAMESIZE];
			DWORD countryID;
			DWORD sceneID;
			DWORD dwType;                   /**< ��Ϣ���� */
			char content[256];    /**< �Ի����� */
			char server[MAX_NAMESIZE];
			BYTE size;                              // ��Ʒ����
			stTradeObject tobject_array[0]; // ������Ʒ�б�
			t_Chat_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_CHAT_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(content, sizeof(content));
				bzero(server, sizeof(server));
				size = 0;
			}       
		};

		//GM����,Scene->Super->Info
		const BYTE PARA_MSG_GMTOOL = 2;
		struct t_Msg_GmTool : t_NullCmd
		{
			GameZone_t zone;
			char userName[MAX_NAMESIZE];
			DWORD accid;
			char country[MAX_NAMESIZE];
			DWORD type;
			char content[512];
			WORD contact;
			char tele[101];
			int isDeal;
			DWORD startTime;
			char gmName[32];
			t_Msg_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_MSG_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(country, sizeof(country));
				bzero(content, sizeof(content));
				bzero(tele, sizeof(tele));
				isDeal = 0;
				startTime = 0;
				bzero(gmName, sizeof(gmName));
			}
		};

		//GM�����ظ�,GmTool->Info->Super->Session
		const BYTE PARA_MSG_REPLY_GMTOOL = 3;
		struct t_Msg_Reply_GmTool : t_NullCmd
		{
			GameZone_t zone;
			char userName[MAX_NAMESIZE];
			char content[32];
			char gmName[32];
			char reply[256-32];
			DWORD userID;
			t_Msg_Reply_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_MSG_REPLY_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(content, sizeof(content));
				bzero(gmName, sizeof(gmName));
				bzero(reply, sizeof(reply));
				userID = 0;
			}
		};

		//GM������GmTool->Info->Super->Scene->Super->Info
		const BYTE PARA_PUNISH_GMTOOL = 4;
		struct t_Punish_GmTool : t_NullCmd
		{
			GameZone_t zone;
			char userName[MAX_NAMESIZE];
			char country[MAX_NAMESIZE];
			char server[MAX_NAMESIZE];
			char GM[MAX_NAMESIZE];
			char reason[128];
			WORD operation;
			int delay;
			WORD level;
			DWORD accid;
			DWORD startTime;
			t_Punish_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_PUNISH_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(country, sizeof(country));
				bzero(server, sizeof(server));
				bzero(reason, sizeof(reason));
				operation = 0;
				delay = 0;
				level = 0;
				accid = 0;
				startTime = 0;
			}
		};

		//����
		const BYTE PARA_BROADCAST_GMTOOL = 5;
		struct t_Broadcast_GmTool : t_NullCmd
		{
			GameZone_t zone;
			DWORD country;
			char content[256];
			DWORD time;
			DWORD interval;
			char GM[MAX_NAMESIZE];
			BYTE id;
			DWORD mapID;
			t_Broadcast_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_BROADCAST_GMTOOL)
			{
				bzero(content, sizeof(content));
				time = 0;
				interval = 0;
				country = 0;
				bzero(GM, sizeof(GM));
				id = 0;
				mapID = 0;
			}
		};

		//��GM����,Scene->Super->Info
		const BYTE PARA_NEW_MSG_GMTOOL = 6;
		struct t_NewMsg_GmTool : t_NullCmd
		{
			GameZone_t zone;//��
			char userName[MAX_NAMESIZE];//�ٱ���
			DWORD accid;//�ٱ���accid
			char userCountry[MAX_NAMESIZE];//�ٱ��߹���
			DWORD type;//��������
			char content[512];//��Ҫ����
			BYTE contact;//��ϵ��ʽ
			char tele[101];//��ַ���绰
			char hisName[MAX_NAMESIZE];//���ٱ���
			char bugCountry[8];//bug��������
			char bugMap[16];//bug������ͼ
			char bugPos[8];//bug����λ��
			char bugTime[64];//����ʱ��
			char progName[64];//�����������
			BYTE behavior;//������Ϸ����
			DWORD userID;//�ٱ���ID
			t_NewMsg_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_NEW_MSG_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(userCountry, sizeof(userCountry));
				bzero(content, sizeof(content));
				bzero(tele, sizeof(tele));
				bzero(hisName, sizeof(hisName));
				bzero(bugCountry, sizeof(bugCountry));
				bzero(bugMap, sizeof(bugMap));
				bzero(bugPos, sizeof(bugPos));
				bzero(bugTime, sizeof(bugTime));
				bzero(progName, sizeof(progName));
				behavior = 0;
				userID = 0;
			}
		};

		//��־
		const BYTE PARA_LOG_GMTOOL = 7;
		struct t_Log_GmTool : t_NullCmd
		{
			GameZone_t zone;//��
			DWORD time;//ʱ��
			DWORD charID;
			DWORD accID;
			char GM[MAX_NAMESIZE];//GM����
			char pos[MAX_NAMESIZE];//��ͼ��
			char cmd[MAX_NAMESIZE];//ָ����
			char content[256];//����
			t_Log_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_LOG_GMTOOL)
			{
				bzero(pos, sizeof(pos));
				bzero(cmd, sizeof(cmd));
				time = 0;
				charID = 0;
				accID = 0;
				bzero(GM, sizeof(GM));
				bzero(content, sizeof(content));
			}
		};
	};
};

#pragma pack()
#endif

/*
# Host: 192.168.2.35
# Database: RecordServer
# Table: 'GMMESSAGE'
# 
CREATE TABLE `GMMESSAGE` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`ZONE` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`TYPE` smallint(5) unsigned NOT NULL default '0',
		`CONTENT` varchar(255) NOT NULL default '',
		`CONTACT` smallint(5) unsigned NOT NULL default '0',
		`TELE` varchar(100) NOT NULL default '',
		`START_TIME` varchar(100) NOT NULL default '',
		`STATE` int(11) NOT NULL default '0',
		`REPLY` varchar(100) NOT NULL default '',
		`ISDEAL` smallint(6) NOT NULL default '0',
		`ACCNAME` varchar(100) NOT NULL default '',
		`CONTRY` varchar(100) NOT NULL default '',
		`GMNAME` varchar(100) NOT NULL default '',
		`SERVER` varchar(100) NOT NULL default '',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.2.35
# Database: RecordServer
# Table: 'operator'
# 
CREATE TABLE `operator` (
		`id` int(11) NOT NULL auto_increment,
		`gmname` varchar(100) NOT NULL default '',
		`time` varchar(100) NOT NULL default '',
		`content` varchar(100) NOT NULL default '',
		`recordid` varchar(100) NOT NULL default '',
		PRIMARY KEY  (`id`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

struct stTradeObject
{
	DWORD dwObjectID;
	DWORD dwObjectTempID;
	char strName[MAX_NAMESIZE];
	BYTE upgrade;                         // �ȼ�
	BYTE kind;      //��Ʒ����, 0��ͨ, 1��ɫ, 2��ɫ, 4��ʥ, 8��װ
};

*/
