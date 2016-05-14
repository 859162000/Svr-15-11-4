# host: 192.168.2.15
# Database: zebra2
# Table: 'SAMPLERELATION'
# ����ϵ������������ͺ��ѵ�һ��һ������ϵ

DROP TABLE IF EXISTS `SAMPLERELATION`;
CREATE TABLE `SAMPLERELATION` (
		`CHARID` int(10) unsigned NOT NULL default '0',          #��ɫ��ID
		`RELATIONID` int(10) unsigned NOT NULL default '0',      #���ϵ�˵�ID
		`RELATIONNAME` varchar(33) NOT NULL default '',          #���ϵ�˵�����
		`TYPE` tinyint(3) unsigned NOT NULL default '0',         #��ϵ����
		`LASTTIME` int(10) unsigned NOT NULL default '0',        #������ʱ��
		`OCCUPATION` smallint(5) unsigned NOT NULL default '0',  #ͷ����
		`DEGREE` smallint(5) unsigned NOT NULL default '0',      #���Ѷ�
		PRIMARY KEY  (`CHARID`,`RELATIONID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'SCHOOL'

# ���ɶ���

DROP TABLE IF EXISTS `SCHOOL`;
CREATE TABLE `SCHOOL` (
		`SCHOOLID` int(10) unsigned NOT NULL auto_increment,    #����IDΪ0��ʾ��Ϊʦͽ��ϵ(0������¼���浵���ڴ��д���
		`NAME` varchar(33) NOT NULL default '',                 #��������
		`MASTERSERIAL` int(10) unsigned NOT NULL default '0',   #ʦ��� CHARID
		`BULLETIN` blob NOT NULL,                               #����
		PRIMARY KEY  (`SCHOOLID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'SCHOOLMEMBER'
# 
# ���ɳ�Ա��ʦͽ��Ա��
#
DROP TABLE IF EXISTS `SCHOOLMEMBER`;
CREATE TABLE `SCHOOLMEMBER` (
		`SERIALID` int(10) unsigned NOT NULL auto_increment,           #���ڵ��ID
		`MASTERID` int(10) unsigned NOT NULL default '0',              #ʦ����ID
		`PRESERIALID` int(10) unsigned NOT NULL default '0',           #ǰһ�ڵ��ID �����������ʦ��������ôMASTERID����Ϊ0����ǰһ�ڵ㲢��Ϊ0
		`CHARID` int(10) unsigned NOT NULL default '0',                #��ɫID
		`NAME` varchar(33) NOT NULL default '',                        #��ɫ����
		`LEVEL` smallint(5) unsigned NOT NULL default '0',             #��ɫ�ȼ�
		`JOINTIME` int(10) unsigned NOT NULL default '0',              #ʦͽ��ϵ����ʱ��
		`DEGREE` smallint(5) unsigned NOT NULL default '0',            #�Ѻö�
		`LASTTIME` int(10) unsigned NOT NULL default '0',              #������ʱ��
		`SCHOOLID` int(10) unsigned NOT NULL default '0',              #����ID Ϊ0��ʾ��ʦͽ��ϵ
		`OCCUPATION` smallint(5) unsigned NOT NULL default '0',        #ͷ����
		`TAG` tinyint(3) unsigned NOT NULL default '0',                #�ڵ���Ч��־
		`MASTERBALANCE` int(10) unsigned NOT NULL default '0',		   #��Ϊʦ��,��ǰ������ȡ�Ľ������
		`PRENTICETOTAL` int(10) unsigned NOT NULL default '0',		   #��Ϊͽ��,�ۼƵĽ������
		`PRENTICELASTLVL` int(10) unsigned NOT NULL default '0',	   #ͽ���ϴν���ʱ�ĵȼ�
		`MASTERTOTAL` bigint(20) unsigned NOT NULL default '0',        #��Ϊʦ��,�ۼ���ȡ�Ľ������
		PRIMARY KEY  (`SERIALID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'SEPT'
# 
# ���嶨���
#
DROP TABLE IF EXISTS `SEPT`;
CREATE TABLE `SEPT` (
		`SEPTID` int(10) unsigned NOT NULL auto_increment, # ����ID
		`NAME` varchar(33) NOT NULL default '', # ��������
		`CHARID` int(10) unsigned NOT NULL default '0', # �峤ID
		`MASTER` varchar(33) NOT NULL default '',	# �峤����
		`VOTE` tinyint(3) unsigned NOT NULL default '0', # ͶƱ״̬
		`CREATETIME` int(10) unsigned NOT NULL default '0',# ����ʱ��
		`COUNTRYID` int(10) unsigned NOT NULL default '0',# ��������
		`REPUTE` int(10) unsigned NOT NULL default '0', # ����
		`UNIONID` int(10) unsigned NOT NULL default '0', # �������
		`LEVEL` int(10) unsigned NOT NULL default '0', # ����ȼ�
		`NOTE` varchar(255) NOT NULL default '',	# ˵��
		`SPENDGOLD` int(10) unsigned NOT NULL default '0', # �������ѽ����
		`ISEXP` int(10) unsigned NOT NULL default '0', # �Ƿ���ȡ����ռ����徭��
		`NORMALEXPTIME` int(10) unsigned NOT NULL default '0', # ���һ����ȡ������ͨ���徭���ʱ
		`CALLTIMES` int(10) unsigned NOT NULL default '0',
		`CALLDAYTIME` timestamp NOT NULL default CURRENT_TIMESTAMP,
		PRIMARY KEY  (`CHARID`, `SEPTID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'SEPTMEMBER'
# 
# �����Ա��
#
DROP TABLE IF EXISTS `SEPTMEMBER`;
CREATE TABLE `SEPTMEMBER` (
		`SEPTID` int(10) unsigned NOT NULL default '0',        #����ID
		`CHARID` int(10) unsigned NOT NULL default '0',        #��ɫID
		`ALIASNAME` varchar(33) NOT NULL default '',           # �����
		`NAME` varchar(33) NOT NULL default '',                #��ɫ����
		`OCCUPATION` smallint(5) unsigned NOT NULL default '0',#ͷ����
		PRIMARY KEY  (`CHARID`,`SEPTID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'UNION'
# 
# ��ᶨ���
#
DROP TABLE IF EXISTS `UNION`;
CREATE TABLE `UNION` (
		`UNIONID` int(10) unsigned NOT NULL auto_increment, # ���ID
		`NAME` varchar(33) NOT NULL default '',             # �������
		`CHARID` int(10) unsigned NOT NULL default '0',     # ����ID
		`LEVEL` smallint(5) NOT NULL default '0',           # ��ἶ��
		`MASTER` varchar(33) NOT NULL default '',           # ����
		`EXP` bigint(20) NOT NULL default '0',              # ��ᾭ��ֵ
		`VOTE` tinyint(3) unsigned NOT NULL default '0',    # �Ƿ���ͶƱ��
		`CREATETIME` int(10) unsigned NOT NULL default '0', # ��Ὠ��ʱ��
		`COUNTRYID` int(10) unsigned NOT NULL default '0',  # ����ID
		`MANA` int(10) unsigned NOT NULL default '0',       # �������
		`ACTIONPOINT` int(10) unsigned NOT NULL default '0',# ����ж���
		`MONEY` int(10) unsigned NOT NULL default '0',	    # ����ʽ�
		`NOTE` varchar(255) NOT NULL default '',            # �������
		`CALLTIMES` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`UNIONID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'UNIONMEMBER'
# 
# ����Ա��
#
DROP TABLE IF EXISTS `UNIONMEMBER`;
CREATE TABLE `UNIONMEMBER` (
		`UNIONID` int(10) unsigned NOT NULL default '0', # ���ID
		`CHARID` int(10) unsigned NOT NULL default '0', # ����ID
		`ALIASNAME` varchar(33) NOT NULL default '', # ���ڱ���
		`NAME` varchar(33) NOT NULL default '', # ��������
		`OCCUPATION` smallint(5) unsigned NOT NULL default '0', # ���ڳ�Աְҵ
		`POWER` int(10) unsigned NOT NULL default '0',   # ���ڳ�ԱȨ��
		`SEPTID` int(10) unsigned NOT NULL default '0', # ��Ա��������ID
		PRIMARY KEY  (`CHARID`,`UNIONID`)
		) TYPE=MyISAM; 

DROP TABLE IF EXISTS `ANSWER`;
CREATE TABLE `ANSWER` (
		`id` bigint(20) unsigned NOT NULL auto_increment,
		`type` varchar(100) NOT NULL default '',
		`ask` blob NOT NULL,
		`answer_a` blob NOT NULL,
		`answer_b` blob NOT NULL,
		`answer_c` blob NOT NULL,
		`answer_d` blob NOT NULL,
		`answer_e` blob NOT NULL,
		`answer_f` blob NOT NULL,
		`the_answer` bigint(20) NOT NULL default '0',
		`quiz_type` int(11) NOT NULL default '0',
		PRIMARY KEY  (`id`)
		) TYPE=MyISAM; 

# Host: 192.168.2.32
# Database: zebra3
# Table: 'NPCDARE'
# 
# NPC����ս�����ñ����ݼ�¼�����������NPC�� ����ID, ��ͼID, NPC ID, ���꣬ ���Ƽ����ID, ��ս�����ID, ��ǰ������
#

DROP TABLE IF EXISTS `NPCDARE`;
CREATE TABLE `NPCDARE` (
  `COUNTRY` int(10) unsigned NOT NULL default '0',     #NPC��������ID
  `MAPID` int(10) unsigned NOT NULL default '0',       #NPC������ͼID
  `NPCID` int(10) unsigned NOT NULL default '0',       #NPC  ID
  `POSX` int(10) unsigned NOT NULL default '0',        #NPC�ڵ�ͼ�е�X����
  `POSY` int(10) unsigned NOT NULL default '0',        #NPC�ڵ�ͼ�е�Y����
  `HOLDSEPTID` int(10) unsigned NOT NULL default '0',  #���Ƹ�NPC�ļ���ID
  `DARESEPTID` int(10) unsigned NOT NULL default '0',  #�����NPC�ļ���ID
  `GOLD` int(10) unsigned NOT NULL default '0'         #��ǰ��ҿ���ȡ������
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


# Host: 192.168.2.32
# Database: zjw
# Table: 'COUNTRY'
# 

DROP TABLE IF EXISTS `COUNTRY`;
CREATE TABLE `COUNTRY` (
		`ID` int(10) unsigned NOT NULL default '0',
		`KINGNAME` varchar(100) NOT NULL default '',
		`KINGUNIONID` int(10) unsigned NOT NULL default '0',
		`DARETIME` int(10) unsigned NOT NULL default '0',
		`LASTDARETIME` int(10) unsigned NOT NULL default '0',
		`LASTDAILYMONEY` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`DIPLOMATNAME` varchar(33) NOT NULL default '',
		`CATCHERNAME` varchar(33) NOT NULL default '',
		`FORMALWIN` int(10) unsigned NOT NULL default '0',
		`FORMALFAIL` int(10) unsigned NOT NULL default '0',
		`ANNOYWIN` int(10) unsigned NOT NULL default '0',
		`ANNOYFAIL` int(10) unsigned NOT NULL default '0',
		`DARECOUNTRYID` int(10) unsigned NOT NULL default '0',
		`STAR` int(10) unsigned NOT NULL default '0',
		`TAX` int(10) unsigned NOT NULL default '0',
		`GOLD` bigint(20) unsigned NOT NULL default '0',
		`SILK` bigint(20) unsigned NOT NULL default '0',
		`ORE` bigint(20) unsigned NOT NULL default '0',
		`BOWLDER` bigint(20) unsigned NOT NULL default '0',
		`WOOD` bigint(20) unsigned NOT NULL default '0',
		`COAT` bigint(20) unsigned NOT NULL default '0',
		`HERBAL` bigint(20) unsigned NOT NULL default '0',
		`MATERIAL` bigint(20) unsigned NOT NULL default '0',
		`NOTE` varchar(255) NOT NULL default '',
		`STOCK` bigint(20) unsigned NOT NULL default '0',
		`FORBIDTALK` int(10) NOT NULL default '0',
		`SENDPRISON` int(10) NOT NULL default '0',
		`GEN_EXP` int(10) unsigned NOT NULL default '0',
		`GEN_MAXEXP` int(10) unsigned NOT NULL default '0',
		`GEN_LEVEL` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# ����: 192.168.2.32
# ���ݿ�: zebra1
# ��: 'CITY'

#DROP TABLE IF EXISTS `CITY`;
#CREATE TABLE `CITY` (
#		`COUNTRY` int(10) unsigned NOT NULL default '0',
#		`CITYID` int(10) unsigned NOT NULL default '0',
#		`UNIONID` int(10) unsigned NOT NULL default '0',
#		`LASTAWARDTIME` int(10) unsigned  NOT NULL default '0',
#		`ISAWARD` int(10) unsigned NOT NULL default '0',
#		`DAREUNIONID` int(10) unsigned NOT NULL default '0',
#		`GOLD` int(10) unsigned NOT NULL default '0',
#		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# ����: 192.168.2.32
# ���ݿ�: zebra1
# ��: 'FORBIDTALK'
# ͨ��GM���ߴ�������õı�
#

DROP TABLE IF EXISTS `FORBIDTALK`;
CREATE TABLE `FORBIDTALK` (
		`NAME` varchar(33) NOT NULL default '',
		`LEVEL` smallint(5) unsigned NOT NULL default '0',
		`ACCID` int(10) unsigned NOT NULL default '0',
		`SERVER` int(10) unsigned NOT NULL default '0',
		`COUNTRY` int(10) unsigned NOT NULL default '0',
		`REASON` varchar(255) NOT NULL default '',
		`OPERATION` smallint(5) unsigned NOT NULL default '0',
		`STARTTIME` bigint(20) unsigned NOT NULL default '0',
		`DELAY` int(10) NOT NULL default '0',
		`ISVALID` smallint(5) unsigned NOT NULL default '0',
		`GM` varchar(33) NOT NULL default '0',
		PRIMARY KEY  (`NAME`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# Host: 192.168.2.33
# Database: xwl
# Table: 'GMMESSAGE'
# �����GM�ύ�����õı�
# 

DROP TABLE IF EXISTS `GMMESSAGE`;
# CREATE TABLE `GMMESSAGE` (
#        	`ID` int(10) unsigned NOT NULL auto_increment,
#        	`NAME` varchar(33) NOT NULL default '',
#        	`TYPE` smallint(5) unsigned NOT NULL default '0',
#        	`CONTENT` varchar(255) NOT NULL default '',
# 	`CONTACT` smallint(5) unsigned NOT NULL default '0',
# 	`TELE` varchar(100) NOT NULL default '',
## 	PRIMARY KEY  (`ID`)
# 	) ENGINE=MyISAM DEFAULT CHARSET=utf8; 
# 
# Host: 192.168.2.32
# Database: zjw
# Table: 'DARERECORD'
# 

DROP TABLE IF EXISTS `DARERECORD`;
CREATE TABLE `DARERECORD` (
		`ID` int(11) NOT NULL auto_increment,
		`DARETIME` int(10) unsigned NOT NULL default '0',
		`ATTCOUNTRY` int(10) unsigned NOT NULL default '0',
		`DEFCOUNTRY` int(10) unsigned NOT NULL default '0',
		`ATTKINGNAME` varchar(33) NOT NULL default '',
		`DEFKINGNAME` varchar(33) NOT NULL default '',
		`RESULT` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


# Host: 192.168.2.33
# Database: xwl
# Table: 'AUCTION'
# ������
# 

DROP TABLE IF EXISTS `AUCTION`;
CREATE TABLE `AUCTION` (
		`ID` int(10) unsigned NOT NULL auto_increment,		#������¼ID
		`OWNER` varchar(100) NOT NULL default '',		#������
		`STATE` tinyint(3) unsigned NOT NULL default '0',	#����״̬
		`CHARGE` int(10) unsigned NOT NULL default '0',		#ȡ������ʱ����Ҫ��������
		`NAME` varchar(32) NOT NULL default '',			#��Ʒ����
		`TYPE` tinyint(3) NOT NULL default '0',			#��Ʒ����
		`QUALITY` tinyint(3) NOT NULL default '0',		#Ʒ��
		`NEEDLEVEL` smallint(5) NOT NULL default '0',		#�ȼ�
		`MINMONEY` int(10) unsigned NOT NULL default '0',	#���
		`MAXMONEY` int(10) unsigned NOT NULL default '0',	#һ�ڼ�
		`MINGOLD` int(10) unsigned NOT NULL default '0',	#��ۣ���ң�
		`MAXGOLD` int(10) unsigned NOT NULL default '0',	#һ�ڼۣ���ң�
		`BIDDER` varchar(32) NOT NULL default '',		#��߳�����
		`BIDDER2` varchar(32) NOT NULL default '',		#�θ߳�����
		`STARTTIME` int(10) unsigned NOT NULL default '0',	#��ʼʱ��
		`ENDTIME` int(10) unsigned NOT NULL default '0',	#����ʱ��
		`BIDTYPE` tinyint(3) unsigned NOT NULL default '0',	#���۷�ʽ����һ�������
		`ITEM` blob NOT NULL,					#��Ʒ��������
		`OWNERID` int(10) unsigned NOT NULL default '0',	#�����ߵ�ID
		`BIDDERID` int(10) unsigned NOT NULL default '0',	#��߳�����ID
		`BIDDER2ID` int(10) unsigned NOT NULL default '0',	#�θ߳�����ID
		`ITEMID` int(10) unsigned NOT NULL default '0',		#��Ʒ��ΨһID
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Host: 192.168.2.33
# Database: xwl
# Table: 'MAIL'
# �ʼ�
# 

DROP TABLE IF EXISTS `MAIL`;
CREATE TABLE `MAIL` (
		`ID` int(10) unsigned NOT NULL auto_increment,		#�ʼ�ID
		`STATE` tinyint(3) unsigned NOT NULL default '0',	#�ʼ�״̬
		`FROMNAME` varchar(32) NOT NULL default '',		#������
		`TONAME` varchar(32) NOT NULL default '',		#������
		`TITLE` varchar(32) NOT NULL default '',		#����
		`TYPE` tinyint(3) unsigned NOT NULL default '0',	#�ʼ�����
		`CREATETIME` int(10) unsigned NOT NULL default '0',	#����ʱ��
		`DELTIME` int(10) unsigned NOT NULL default '0',	#����ʱ��
		`ACCESSORY` tinyint(3) unsigned NOT NULL default '0',	#�Ƿ��и���
		`ITEMGOT` tinyint(3) unsigned NOT NULL default '0',	#�����Ƿ��Ѿ���ȡ
		`TEXT` varchar(255) NOT NULL default '',		#�����ı�
		`SENDMONEY` int(10) unsigned NOT NULL default '0',	#���ͽ�Ǯ
		`RECVMONEY` int(10) unsigned NOT NULL default '0',	#��ȡ��Ǯ
		`SENDGOLD` int(10) unsigned NOT NULL default '0',	#���ͽ������
		`RECVGOLD` int(10) unsigned NOT NULL default '0',	#��ȡ�������
		`BIN` blob,						#��Ʒ��������
		`TOID` int(10) unsigned NOT NULL default '0',		#�ռ���ID
		`FROMID` int(10) unsigned NOT NULL default '0',		#�ļ���ID
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


# Host: 192.168.2.33
# Database: xwl
# Table: 'SN'
# ���кţ����л�������к���
# 

DROP TABLE IF EXISTS `SN`;
	CREATE TABLE `SN` (
			`SN` varchar(16) NOT NULL default '',		#���к�
			PRIMARY KEY  (`SN`)
			) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.33
# Database: xwl
# Table: 'CARTOONPET'
# 

DROP TABLE IF EXISTS `CARTOONPET`;
	CREATE TABLE `CARTOONPET` (
					`CARTOONID` int(10) unsigned NOT NULL auto_increment,
					`NAME` char(32) NOT NULL default '0',
					`NPCID` int(10) unsigned NOT NULL default '0',
					`MASTERID` int(10) unsigned NOT NULL default '0',
					`MASTERNAME` char(32) NOT NULL default '',
					`LEVEL` tinyint(3) unsigned NOT NULL default '0',
					`EXP` int(10) unsigned NOT NULL default '0',
					`MAXEXP` int(10) unsigned NOT NULL default '0',
					`ADDEXP` int(10) unsigned NOT NULL default '0',
					`STATE` tinyint(3) unsigned NOT NULL default '0',
					`ADOPTER` char(32) NOT NULL default '',
					`TIME` int(10) unsigned NOT NULL default '0',
					`SP` int(10) unsigned NOT NULL default '0',
					`MAXSP` int(10) unsigned NOT NULL default '0',
					`MASTERLEVEL` int(10) unsigned NOT NULL default '0',
					`REPAIR` tinyint(3) unsigned NOT NULL default '0',
					PRIMARY KEY  (`CARTOONID`)
					) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'VOTE'
# 

DROP TABLE IF EXISTS `VOTE`;
CREATE TABLE `VOTE` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`COUNTRYID` int(10) unsigned NOT NULL default '0',
		`STATUS` int(10) unsigned NOT NULL default '0',
		`TYPE` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'VOTEDPLAYER'
# 

DROP TABLE IF EXISTS `VOTEDPLAYER`;
CREATE TABLE `VOTEDPLAYER` (
		`VOTEID` int(10) unsigned NOT NULL default '0',
		`CHARID` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`VOTEID`,`CHARID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'VOTEITEM'
# 

DROP TABLE IF EXISTS `VOTEITEM`;
CREATE TABLE `VOTEITEM` (
		`VOTEID` int(10) unsigned NOT NULL default '0',
		`OPTIONID` int(11) NOT NULL default '0',
		`OPTIONDESC` varchar(100) NOT NULL default '',
		`BALLOT` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`VOTEID`,`OPTIONID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'TECH'
# 

DROP TABLE IF EXISTS `TECH`;
CREATE TABLE `TECH` (
		`UID` int(10) unsigned NOT NULL auto_increment,
		`COUNTRYID` int(10) unsigned NOT NULL default '0',
		`TYPE` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`PROGRESS` int(10) unsigned NOT NULL default '0',
		`RESEARCHID` int(10) unsigned default '0',
		`RESEARCHNAME` varchar(33) default '',
		`LEVEL` int(10) unsigned NOT NULL default '1',
		`STATUS` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`UID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# ����: 192.168.2.32
# ���ݿ�: zebra3
# ��: 'SORTLIST'
# 
# ��ɫ�ȼ��������˱�洢ǰ2000����ɫ��id,���𣬾��������
#

DROP TABLE IF EXISTS `SORTLIST`;
CREATE TABLE `SORTLIST` (
  `CHARID` int(10) unsigned NOT NULL default '0',      #��ɫID
  `LEVEL` smallint(5) unsigned NOT NULL default '0',   #��ɫ�ȼ�
  `EXP` bigint(20) unsigned NOT NULL default '0',      #��ɫ����
  PRIMARY KEY  (`CHARID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'CAPTAIN'
# ���ӳ�Ա��
# 

DROP TABLE IF EXISTS `CAPTAIN`;
CREATE TABLE `CAPTAIN` (
		`ARMYID` int(10) unsigned NOT NULL default '0',
		`CHARID` int(10) unsigned NOT NULL default '0',
		`CHARNAME` varchar(33) NOT NULL default '',
		`NPCNUM` int(10) NOT NULL default '0',
		PRIMARY KEY  (`ARMYID`,`CHARID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'ARMY'
# ���Ҿ��ӱ�
# 

DROP TABLE IF EXISTS `ARMY`;
CREATE TABLE `ARMY` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`COUNTRYID` int(10) unsigned NOT NULL default '0',
		`CITYID` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`GENNAME` varchar(33) NOT NULL default '',
		`GENID` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.33
# Database: xwl
# Table: 'ACT'
# ��Ӫ�����¼���еĻ

DROP TABLE IF EXISTS `ACT`;
CREATE TABLE `ACT` (
				`ID` int(10) unsigned NOT NULL default '0',			#�ID
				`NAME` varchar(32) NOT NULL default '',				#�����
				`STATE` tinyint(3) unsigned NOT NULL default '0',	#״̬
				`TEXT` varchar(255) NOT NULL default ''				#����
				) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.33
# Database: xwl
# Table: 'GIFT'
# ��Ӫ���Ž�Ʒ�ı���������Ӫ��ά��

DROP TABLE IF EXISTS `GIFT`;
CREATE TABLE `GIFT` (
				`ACTID` int(10) unsigned NOT NULL default '0',		#�ID
				`NAME` varchar(32) NOT NULL default '',				#�н�������
				`CHARID` int(10) unsigned NOT NULL default '0',		#�н���CHARID
				`ITEMGOT` tinyint(3) unsigned NOT NULL default '0',	#�Ƿ��Ѿ���ȡ
				`ITEMID` int(10) unsigned NOT NULL default '0',		#������ƷID
				`ITEMTYPE` tinyint(3) unsigned NOT NULL default '0',#������Ʒ����
				`ITEMNUM` int(10) unsigned NOT NULL default '0',	#������Ʒ����
				`BIND` tinyint(3) unsigned NOT NULL default '0',	#�Ƿ��
				`MONEY` int(10) unsigned NOT NULL default '0',		#������Ǯ
				`MAILTEXT` varchar(255) NOT NULL default ''			#�ʼ�������
				) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'ALLY'
# 

DROP TABLE IF EXISTS `ALLY`;
CREATE TABLE `ALLY` (
		`COUNTRYID` int(10) unsigned NOT NULL default '0',	# ���˹�ID1
		`ALLYCOUNTRYID` int(10) unsigned NOT NULL default '0',  # ���˹�ID2
		`CREATETIME` int(10) unsigned NOT NULL default '0',     # ���˽���ʱ��
		`LASTUPTIME` int(10) unsigned NOT NULL default '0',     # ��������������ʱ��
		`FRIENDDEGREE` int(10) unsigned NOT NULL default '0'    # �����Ѻö�
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Host: 192.168.2.33
# Database: xwl
# Table: 'EMPERORFORBID'
# �ʵ۽�����ҵı�

DROP TABLE IF EXISTS `EMPERORFORBID`;
CREATE TABLE `EMPERORFORBID` (
		`DATA` char(40)  NOT NULL default '0'
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'RECOMMENDSUB'
# ���Ƽ��˱�

DROP TABLE IF EXISTS `RECOMMENDSUB`;
CREATE TABLE `RECOMMENDSUB` (
		`ID` int(10) unsigned NOT NULL default '0', # ���Ƽ���CHARID
		`NAME` varchar(33) NOT NULL default '', # ���Ƽ�������
		`LASTLEVEL` int(10) unsigned NOT NULL default '0', # ���Ƽ����ϴ���ȡ�����ĵȼ�
		`TOTAL` int(10) unsigned NOT NULL default '0', # �ۼ���ȡ����ֵ
		`RECOMMENDID` int(10) unsigned NOT NULL default '0', # �Ƽ���ID
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


# Host: 192.168.2.32
# Database: zjw
# Table: 'RECOMMEND'
# �Ƽ��˱�

DROP TABLE IF EXISTS `RECOMMEND`;
CREATE TABLE `RECOMMEND` (
		`ID` int(10) unsigned NOT NULL default '0', # �Ƽ���ID
		`NAME` varchar(33) NOT NULL default '', # �Ƽ�������
		`TOTAL` int(10) unsigned NOT NULL default '0', # �ۼ���ȡ����ֵ
		`BALANCE` int(10) unsigned NOT NULL default '0', # ��ǰ����ȡ�Ľ����
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 



