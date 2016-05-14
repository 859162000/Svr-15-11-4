# Host: 192.168.2.33
# Database: xwl
# Table: 'CHARBASE'
# 
CREATE TABLE `CHARBASE` (
		`CHARID` int(10) unsigned NOT NULL auto_increment, # ���ID
		`ACCID` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '', # ��ɫ����
		`TYPE` smallint(5) unsigned NOT NULL default '0',
		`LEVEL` smallint(5) unsigned NOT NULL default '1',
		`FACE` smallint(5) unsigned NOT NULL default '0',
		`HAIR` int(10) unsigned NOT NULL default '0',
		`BODYCOLOR` int(10) unsigned NOT NULL default '0',
		`GOODNESS` int(10) unsigned NOT NULL default '0', # �ƶ��
		`PKADDITION` int(10) unsigned NOT NULL default '0',
		`MAPID` int(10) unsigned NOT NULL default '0', # ������ڵ�ͼ���,������ڵط���MAPNAMEΪ׼,��ֵδ��
		`MAPNAME` varchar(33) default '', # ������ڵ�ͼ����
		`X` int(10) unsigned NOT NULL default '0',
		`Y` int(10) unsigned NOT NULL default '0',
		`HP` int(10) unsigned NOT NULL default '0',
		`MP` int(10) unsigned NOT NULL default '0',
		`SP` int(10) unsigned NOT NULL default '0',
		`CREATEIP` int(10) unsigned NOT NULL default '0',
		`EXP` bigint(20) unsigned NOT NULL default '0',
		`LUCKY` smallint(5) unsigned NOT NULL default '0',
		`SKILLPOINTS` smallint(5) unsigned NOT NULL default '0',
		`POINTS` smallint(5) unsigned NOT NULL default '0',
		`COUNTRY` int(10) unsigned NOT NULL default '0', # ��ҹ���
		`UNIONID` int(10) unsigned NOT NULL default '0', # ������ڰ��ID,��UNION��Ϊ׼
		`CONSORT` int(10) unsigned NOT NULL default '0',
		`SEPTID` int(10) unsigned NOT NULL default '0', # ������ڼ���ID,��SEPT��Ϊ׼
		`SCHOOLID` int(10) unsigned NOT NULL default '0',
		`SYSSET` int(10) unsigned zerofill NOT NULL default '0000000000',
		`FORBIDTALK` bigint(20) unsigned NOT NULL default '0',
		`BITMASK` int(10) unsigned NOT NULL default '0',
		`ONLINETIME` int(10) unsigned NOT NULL default '0',
		`AVAILABLE` tinyint(3) unsigned NOT NULL default '1',
		`LASTACTIVEDATE` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
		`MEN` smallint(5) unsigned NOT NULL default '0',
		`INT` smallint(5) unsigned NOT NULL default '0',
		`DEX` smallint(5) unsigned NOT NULL default '0',
		`STR` smallint(5) unsigned NOT NULL default '0',
		`CON` smallint(5) unsigned NOT NULL default '0',
		`RELIVEWEAKTIME` smallint(5) unsigned NOT NULL default '0', # ��������ʣ��ʱ��
		`GRACE` int(10) unsigned NOT NULL default '0',	# �Ĳ�ֵ
		`EXPLOIT` int(10) unsigned NOT NULL default '0', # ��ѫֵ
		`TIRETIME` tinyblob NOT NULL,
		`OFFLINETIME` int(10) unsigned NOT NULL default '0',
		`FIVETYPE` int(10) unsigned NOT NULL default '0',
		`FIVELEVEL` int(10) unsigned NOT NULL default '0',
		`ALLBINARY` blob,
		`ANSWERCOUNT` int(10) unsigned NOT NULL default '0',
		`MONEY` int(10) unsigned NOT NULL default '0',
		`HONOR` int(10) unsigned NOT NULL default '0',
		`GOMAPTYPE` int(10) unsigned NOT NULL default '0',
		`MAXHONOR` int(10) unsigned NOT NULL default '0',
		`MSGTIME` int(10) unsigned NOT NULL default '0',
		`ACCPRIV` int(10) unsigned NOT NULL default '0',
		`GOLD` int(10) unsigned NOT NULL default '0',
		`TICKET` int(10) unsigned NOT NULL default '0',
		`CREATETIME` int(10) unsigned NOT NULL default '0',
		`GOLDGIVE` int(10) unsigned NOT NULL default '0',
		`PETPACK` int(3) unsigned NOT NULL default '0',
		`PETPOINT` int(10) unsigned NOT NULL default '0', # ������õĳ�������ʱ�佱��
		`LEVELSEPT` int(3) unsigned NOT NULL default '0', # �뿪�����ʱ��time_t
		`PUNISHTIME` int(10) unsigned NOT NULL default '0', # �������׼�����ʱ��
		`TRAINTIME` int(10) unsigned NOT NULL default '0', # ��������ͼͣ����ʱ��
		PRIMARY KEY  (`CHARID`),
		KEY `ACCID` (`ACCID`),
		KEY `NAME` (`NAME`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


# Host: 192.168.2.12
# Database: zebra1
# Table: 'CHARTEST'
# 
	CREATE TABLE `CHARTEST` (
			`NAME` varchar(33) NOT NULL default '',
			`LEVEL` int(10) unsigned NOT NULL default '1',
			`UPDATETIME` int(10) unsigned NOT NULL default '0',
			`UPDATEUSETIME` int(10) unsigned NOT NULL default '0',
			`DEATHTIMES` int(10) NOT NULL default '0',
			`HPLEECHDOM` int(10) unsigned NOT NULL default '0',
			`MPLEECHDOM` int(10) unsigned NOT NULL default '0',
			`SPLEECHDOM` int(10) unsigned NOT NULL default '0',
			`GETMONEY` int(10) unsigned NOT NULL default '0',
			`GETHEIGH` int(10) unsigned NOT NULL default '0',
			`GETSOCKET` int(10) unsigned NOT NULL default '0',
			`GETMATERIAL` int(10) unsigned NOT NULL default '0',
			`GETSTONE` int(10) unsigned NOT NULL default '0',
			`GETSCROLL` int(10) unsigned NOT NULL default '0',
			`MONEY` int(10) unsigned NOT NULL default '0',
			PRIMARY KEY  (`NAME`,`LEVEL`)
			) TYPE=MyISAM; 


# Host: 192.168.2.33
# Database: xwl
# Table: 'ACCPRIV'
# �ʺ�����Ȩ�ޱ�Ŀǰֻ��һ������Ȩ�޾���5��15��ʱ������װ��
# 
CREATE TABLE `ACCPRIV` (
		`ACCID` int(10) unsigned NOT NULL default '0',	#ACCID
		`PRIV` int(10) unsigned NOT NULL default '0'	#Ȩ�ޱ�־
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# Host: 192.168.2.33
# Database: xwl
# Table: 'ACT'
# ��Ӫ�����¼���еĻ
CREATE TABLE `ACT` (
				`ID` int(10) unsigned NOT NULL default '0',
				`NAME` varchar(32) NOT NULL default '',
				`STATE` tinyint(3) unsigned NOT NULL default '0',
				`TEXT` varchar(255) NOT NULL default ''
				) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# Host: 192.168.2.33
# Database: xwl
# Table: 'GIFT'
# ��Ӫ���Ž�Ʒ�ı���������Ӫ��ά��
CREATE TABLE `GIFT` (
				`ACTID` int(10) unsigned NOT NULL default '0',
				`NAME` varchar(32) NOT NULL default '',
				`CHARID` int(10) unsigned NOT NULL default '0',
				`ITEMGOT` tinyint(3) unsigned NOT NULL default '0',
				`ITEMID` int(10) unsigned NOT NULL default '0',
				`ITEMTYPE` tinyint(3) unsigned NOT NULL default '0',
				`ITEMNUM` int(10) unsigned NOT NULL default '0',
				`BIND` tinyint(3) unsigned NOT NULL default '0',
				`MONEY` int(10) unsigned NOT NULL default '0',
				`MAILTEXT` varchar(255) NOT NULL default ''
				) ENGINE=MyISAM DEFAULT CHARSET=utf8; 



#whj 04-26
# Host: 192.168.2.31
# Database: zebra1
# Table: 'RESTORECARD'
# 
CREATE TABLE `RESTORECARD` (
		`ACCID` int(10) unsigned NOT NULL default '0',
		`SUBAT` int(10) unsigned NOT NULL default '0',
		`BALANCE` int(10) unsigned NOT NULL default '0',
		`TID` char(100) NOT NULL default '',
		PRIMARY KEY  (`ACCID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# Host: 192.168.2.31
# Database: zebra1
# Table: 'RESTOREGOLD'
# 
CREATE TABLE `RESTOREGOLD` (
		`ACCID` int(10) unsigned NOT NULL default '0',
		`RESTOREGOLD` int(10) unsigned NOT NULL default '0',
		`BALANCE` int(10) unsigned NOT NULL default '0',
		`TID` char(100) NOT NULL default '',
		PRIMARY KEY  (`ACCID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 
