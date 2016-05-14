# 档案数据库升级

# CREATE TABLE `NPCDARE` (
# 		`COUNTRY` int(10) unsigned NOT NULL default '0',
# 		`MAPID` int(10) unsigned NOT NULL default '0',
# 		`NPCID` int(10) unsigned NOT NULL default '0',
# 		`HOLDSEPTID` int(10) unsigned NOT NULL default '0',
# 		`DARESEPTID` int(10) unsigned NOT NULL default '0',
# 		`GOLD` int(10) unsigned NOT NULL default '0'
# 		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 
# 
# 
# CREATE TABLE `ANSWER` (
# 		`id` bigint(20) unsigned NOT NULL auto_increment,
# 		`type` varchar(100) NOT NULL default '',
# 		`ask` blob NOT NULL,
# 		`answer_a` blob NOT NULL,
# 		`answer_b` blob NOT NULL,
# 		`answer_c` blob NOT NULL,
# 		`answer_d` blob NOT NULL,
# 		`the_answer` bigint(20) NOT NULL default '0',
# 		PRIMARY KEY  (`id`)  
# 		) TYPE=MyISAM; 
# 
# ALTER TABLE  `CHARBASE` add `PKADDITION` int(10) unsigned NOT NULL default '0';
# ALTER TABLE  `CHARBASE` add `FIVETYPE` int(10) unsigned NOT NULL default '0';
# ALTER TABLE  `CHARBASE` add `FIVELEVEL` int(10) unsigned NOT NULL default '0';
# ALTER TABLE  `CHARBASE` add  `GRACE` int(10) unsigned NOT NULL default '0';
# ALTER TABLE  `CHARBASE` add  `EXPLOIT` int(10) unsigned NOT NULL default '0';
# 
  
# ALTER TABLE  `NPCDARE` add `POSX` int(10) unsigned NOT NULL default '0';
# ALTER TABLE  `NPCDARE` add `POSY` int(10) unsigned NOT NULL default '0';
# INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','102','251','381','379','0','0','10000');
# INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','102','251','381','379','0','0','10000');
# INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','102','251','381','379','0','0','10000');
# INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','102','251','381','379','0','0','10000');

# CREATE TABLE `FORBIDTALK` (
#        	`CHARID` int(10) unsigned NOT NULL default '0',
#        	`STARTTIME` bigint(20) unsigned NOT NULL default '0',
#        	`DELAY` int(10) unsigned NOT NULL default '0',
#        	`ISVALID` int(10) unsigned NOT NULL default '0',
#        	`NOTE` varchar(255) NOT NULL default '',
#          	PRIMARY KEY  (`CHARID`)
#        	) ENGINE=MyISAM DEFAULT CHARSET=latin1;
# 

#	DROP TABLE `FORBIDTALK`;
#	CREATE TABLE `FORBIDTALK` (
#			`NAME` varchar(32) NOT NULL default '',
#			`STARTTIME` bigint(20) unsigned NOT NULL default '0',
#			`DELAY` int(10) unsigned NOT NULL default '0',
#			`ISVALID` int(10) unsigned NOT NULL default '0',
#			`NOTE` varchar(255) NOT NULL default '',
#			PRIMARY KEY  (`NAME`)
#			) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

#ALTER TABLE  `ANSWER` add `answer_e` blob NOT NULL;
#ALTER TABLE  `ANSWER` add `answer_f` blob NOT NULL;

#DROP TABLE `FORBIDTALK`;
#CREATE TABLE `FORBIDTALK` (
#		`NAME` varchar(33) NOT NULL default '',
#		`LEVEL` smallint(5) unsigned NOT NULL default '0',
#		`ACCID` int(10) unsigned NOT NULL default '0',
#		`SERVER` int(10) unsigned NOT NULL default '0',
#		`COUNTRY` int(10) unsigned NOT NULL default '0',
#		`REASON` varchar(255) NOT NULL default '',
#		`OPERATION` smallint(5) unsigned NOT NULL default '0',
#		`STARTTIME` bigint(20) unsigned NOT NULL default '0',
#		`DELAY` int(10) NOT NULL default '0',
#		`ISVALID` smallint(5) unsigned NOT NULL default '0',
#		`GM` varchar(33) NOT NULL default '0',
#		PRIMARY KEY  (`NAME`)
#		) ENGINE=MyISAM DEFAULT CHARSET=latin1;

#GM定单用的表
#CREATE TABLE `GMMESSAGE` (
#		`ID` int(10) unsigned NOT NULL auto_increment,
#		`NAME` varchar(33) NOT NULL default '',
#		`TYPE` smallint(5) unsigned NOT NULL default '0',
#		`CONTENT` varchar(255) NOT NULL default '',
#		`CONTACT` smallint(5) unsigned NOT NULL default '0',
#		`TELE` varchar(100) NOT NULL default '',
#		PRIMARY KEY  (`ID`)
#		) ENGINE=MyISAM DEFAULT CHARSET=latin1;


#INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','102','251','381','379','0','0','10000');
#INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','102','251','381','379','0','0','10000');
#INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','102','251','381','379','0','0','10000');
#INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','102','251','381','379','0','0','10000');
#INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','102','251','381','379','0','0','10000');
#INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','102','251','381','379','0','0','10000');

#ALTER TABLE  `UNION` add `COUNTRYID` int(10) unsigned  NOT NULL default '0';
#ALTER TABLE  `SEPT` add `COUNTRYID`  int(10) unsigned  NOT NULL default '0';

#银子直接显示出来,但是不在游戏中使用,只是作为参考用
#ALTER TABLE  `CHARBASE` add  `MONEY` int(10) unsigned NOT NULL default '0';

#ALTER TABLE  `CHARBASE` add  `ANSWERCOUNT` int(10) unsigned NOT NULL default '0';
#ALTER TABLE  `CHARBASE` add  `MONEY` int(10) unsigned NOT NULL default '0';

#王海军
#ALTER TABLE  `CHARBASE` add  `HONOR` int(10) unsigned NOT NULL default '0';
#ALTER TABLE  `CHARBASE` add  `MAXHONOR` int(10) unsigned NOT NULL default '0';
#ALTER TABLE  `CHARBASE` add  `GOMAPTYPE` int(10) unsigned NOT NULL default '0';
#ALTER TABLE  `CHARBASE` add  `GOLD` int(10) unsigned NOT NULL default '0';
#ALTER TABLE  `CHARBASE` add  `TICKET` int(10) unsigned NOT NULL default '0';
#ALTER TABLE  `CHARBASE` add  `CREATETIME` int(10) unsigned NOT NULL default '0';
#ALTER TABLE  `CHARBASE` add  `GOLDGIVE` int(10) unsigned NOT NULL default '0';

ALTER TABLE  	`COUNTRY`	add  	`KINGNAME` varchar(100) NOT NULL default '';
ALTER TABLE  	`COUNTRY`	add  	`KINGUNIONID` int(10) unsigned NOT NULL default '0';
ALTER TABLE 	`COUNTRY`	add 	`LASTDARETIME` int(10) unsigned NOT NULL default '0';
ALTER TABLE	`COUNTRY`	add	`NAME` varchar(33) NOT NULL default '';
ALTER TABLE 	`COUNTRY` 	add	`FORMALWIN` int(10) unsigned NOT NULL default '0';
ALTER TABLE 	`COUNTRY`	add	`FORMALFAIL` int(10) unsigned NOT NULL default '0';
ALTER TABLE 	`COUNTRY`	add 	`ANNOYWIN` int(10) unsigned NOT NULL default '0';
ALTER TABLE 	`COUNTRY`	add	`ANNOYFAIL` int(10) unsigned NOT NULL default '0';
ALTER TABLE 	`COUNTRY`	add	`DARECOUNTRYID` int(10) unsigned NOT NULL default '0';
ALTER TABLE 	`COUNTRY`	add 	`DARETIME` int(10) unsigned NOT NULL default '0';



# 邢万里添加
DROP TABLE `FORBIDTALK`;
DROP TABLE `GMMESSAGE`;
# Host: 192.168.2.33
# Database: xwl
# Table: 'MAIL'
# 
CREATE TABLE `MAIL` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`STATE` tinyint(3) unsigned NOT NULL default '0',
		`FROMNAME` varchar(32) NOT NULL default '',
		`TONAME` varchar(32) NOT NULL default '',
		`TITLE` varchar(32) NOT NULL default '',
		`TYPE` tinyint(3) unsigned NOT NULL default '0',
		`CREATETIME` int(10) unsigned NOT NULL default '0',
		`DELTIME` int(10) unsigned NOT NULL default '0',
		`ACCESSORY` tinyint(3) unsigned NOT NULL default '0',
		`ITEMGOT` tinyint(3) unsigned NOT NULL default '0',
		`TEXT` varchar(255) NOT NULL default '',
		`SENDMONEY` int(10) unsigned NOT NULL default '0',
		`RECVMONEY` int(10) unsigned NOT NULL default '0',
		`SENDGOLD` int(10) unsigned NOT NULL default '0',
		`RECVGOLD` int(10) unsigned NOT NULL default '0',
		`BIN` blob,
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('2','','宋国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('3','','魏国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('4','','齐国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('5','','燕国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('7','','唐国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('8','','汉国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('9','','吴国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('10','','赵国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('11','','周国');
INSERT INTO `COUNTRY` (ID, KINGNAME, NAME) VALUES('12','','楚国');

# Host: 192.168.2.32
# Database: zjw
# Table: 'DARERECORD'
# 
CREATE TABLE `DARERECORD` (
		`ID` int(11) NOT NULL auto_increment,
		`DARETIME` int(10) unsigned NOT NULL default '0',
		`ATTCOUNTRY` int(10) unsigned NOT NULL default '0',
		`DEFCOUNTRY` int(10) unsigned NOT NULL default '0',
		`ATTKINGNAME` varchar(33) NOT NULL default '',
		`DEFKINGNAME` varchar(33) NOT NULL default '',
		`RESULT` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 


ALTER TABLE 	`CITY`	add 	`DAREUNIONID` int(10) unsigned NOT NULL default '0';

# Host: 192.168.2.33
# Database: xwl
# Table: 'AUCTION'
# 
CREATE TABLE `AUCTION` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`OWNER` varchar(100) NOT NULL default '',
		`STATE` tinyint(3) unsigned NOT NULL default '0',
		`NAME` varchar(32) NOT NULL default '',
		`TYPE` tinyint(3) NOT NULL default '0',
		`QUALITY` tinyint(3) NOT NULL default '0',
		`NEEDLEVEL` smallint(5) NOT NULL default '0',
		`MINMONEY` int(10) unsigned NOT NULL default '0',
		`MAXMONEY` int(10) unsigned NOT NULL default '0',
		`MINGOLD` int(10) unsigned NOT NULL default '0',
		`MAXGOLD` int(10) unsigned NOT NULL default '0',
		`BIDDER` varchar(32) NOT NULL default '',
		`BIDDER2` varchar(32) NOT NULL default '',
		`STARTTIME` int(10) unsigned NOT NULL default '0',
		`ENDTIME` int(10) unsigned NOT NULL default '0',
		`ITEM` blob NOT NULL,
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

ALTER TABLE 	`AUCTION`	add 	`CHARGE` int(10) unsigned NOT NULL default '0';

#UPDATE `UNIONMEMBER` SET POWER=0;


INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('2','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('3','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('4','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('5','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('7','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('8','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('9','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('10','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('11','103','242','84','218','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','104','111','65','162','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','104','108','99','199','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','104','243','74','176','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','102','120','339','417','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','102','201','362','396','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','102','123','276','482','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','103','106','137','168','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','103','110','126','156','0','0','10000');
INSERT INTO NPCDARE (COUNTRY, MAPID, NPCID, POSX, POSY, HOLDSEPTID, DARESEPTID, GOLD) VALUES('12','103','242','84','218','0','0','10000');

ALTER TABLE  `ANSWER` add `quiz_type` int(10) unsigned NOT NULL default '0';

# xwl
ALTER TABLE  `MAIL` add `TOID` int(10) unsigned NOT NULL default '0';
ALTER TABLE  `UNION` add `MANA` int(10) unsigned NOT NULL default '0';
ALTER TABLE  `SEPT` add `REPUTE` int(10) unsigned NOT NULL default '0';
ALTER TABLE  `SEPTMEMBER` add `ALIASNAME` varchar(33) NOT NULL default '';
ALTER TABLE  `UNIONMEMBER` add `SEPTID` int(10) unsigned NOT NULL default '0';
ALTER TABLE  `SEPT` add `UNIONID` int(10) unsigned  NOT NULL default '0';
ALTER TABLE  `UNION` add `ACTIONPOINT` int(10) unsigned NOT NULL default '0';

#fqnewman
ALTER TABLE  `COUNTRY` add  `TAX` int(10) unsigned NOT NULL default '0';
ALTER TABLE  `COUNTRY` add  `GOLD` bigint(20) unsigned NOT NULL default '0';

# xwl
# Host: 192.168.2.33
# Database: xwl
# Table: 'SN'
# 
CREATE TABLE `SN` (
		`SN` varchar(16) NOT NULL default '',
		`LEVEL` smallint(5) unsigned NOT NULL default '0',
		`DAY` tinyint(3) unsigned NOT NULL default '0',
		`USER` varchar(33) NOT NULL default '',
		PRIMARY KEY  (`SN`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

ALTER TABLE  `CHARBASE` add  `MSGTIME` int(10) unsigned NOT NULL default '0';

ALTER TABLE  `COUNTRY` add `SILK` bigint(20) unsigned NOT NULL default '0';
ALTER TABLE  `COUNTRY` add `ORE` bigint(20) unsigned NOT NULL default '0';
ALTER TABLE  `COUNTRY` add `BOWLDER` bigint(20) unsigned NOT NULL default '0';
ALTER TABLE  `COUNTRY` add `WOOD` bigint(20) unsigned NOT NULL default '0';
ALTER TABLE  `COUNTRY` add `COAT` bigint(20) unsigned NOT NULL default '0';
ALTER TABLE  `COUNTRY` add `HERBAL` bigint(20) unsigned NOT NULL default '0';
ALTER TABLE  `COUNTRY` add `MATERIAL` bigint(20) unsigned NOT NULL default '0';


# Host: 192.168.2.33
# Database: xwl
# Table: 'CARTOONPET'
#
# CREATE TABLE `CARTOONPET` (
# 		`ID` int(10) unsigned NOT NULL auto_increment,
# 		`NAME` char(32) NOT NULL default '0',
# 		`NPCID` int(10) unsigned NOT NULL default '0',
# 		`MASTERID` int(10) unsigned NOT NULL default '0',
# 		`MASTERNAME` char(32) NOT NULL default '',
# 		`LEVEL` tinyint(3) unsigned NOT NULL default '0',
# 		`EXP` int(10) unsigned NOT NULL default '0',
# 		`ADDEXP` int(10) unsigned NOT NULL default '0',
# 		`STATE` tinyint(3) unsigned NOT NULL default '0',
# 		`ADOPTER` char(32) NOT NULL default '',
# 		`TIME` int(10) unsigned NOT NULL default '0',
# 		PRIMARY KEY  (`ID`)
# 		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 
 
# Host: 192.168.2.33
# Database: xwl
# Table: 'ACCPRIV'
# 
CREATE TABLE `ACCPRIV` (
		`ACCID` int(10) unsigned NOT NULL default '0',
		`PRIV` int(10) unsigned NOT NULL default '0'
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 


ALTER TABLE  `SEPT` add  `LEVEL` int(10) unsigned NOT NULL default '0';
ALTER TABLE  `CHARBASE` add  `ACCPRIV` int(10) unsigned NOT NULL default '0';

#zjw
ALTER TABLE  `COUNTRY` add  `STOCK` bigint(20) unsigned NOT NULL default '0';
# Host: 192.168.2.32
# Database: zjw
# Table: 'VOTE'
# 
CREATE TABLE `VOTE` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`COUNTRYID` int(10) unsigned NOT NULL default '0',
		`STATUS` int(10) unsigned NOT NULL default '0',
		`TYPE` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.2.32
# Database: zjw
# Table: 'VOTEDPLAYER'
# 
CREATE TABLE `VOTEDPLAYER` (
		`VOTEID` int(10) unsigned NOT NULL default '0',
		`CHARID` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`VOTEID`,`CHARID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.2.32
# Database: zjw
# Table: 'VOTEITEM'
# 
CREATE TABLE `VOTEITEM` (
		`VOTEID` int(10) unsigned NOT NULL default '0',
		`OPTIONID` int(11) NOT NULL default '0',
		`OPTIONDESC` varchar(100) NOT NULL default '',
		`BALLOT` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`VOTEID`,`OPTIONID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.2.32
# Database: zjw
# Table: 'TECH'
# 
CREATE TABLE `TECH` (
		`UID` int(10) unsigned NOT NULL auto_increment,
		`COUNTRYID` int(10) unsigned NOT NULL default '0',
		`TYPE` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`PROGRESS` int(10) unsigned NOT NULL default '0',
		`RESEARCHID` int(10) unsigned default '0',
		`RESEARCHNAME` varchar(33) default '',
		`LEVEL` int(10) unsigned default '1',
		`STATUS` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`UID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 


#xwl
alter table SEPT add SPENDGOLD int(10) unsigned not null default 0;	#家族消费金币数量

# alter table CARTOONPET add SP int(10) unsigned not null default 0;	#宠物精气
# alter table CARTOONPET add MAXSP int(10) unsigned not null default 0;	#最大精气

# Host: 192.168.2.33
# Database: xwl
# Table: 'CARTOONPET'
# 替身宠物的表
drop table CARTOONPET;
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
		PRIMARY KEY  (`CARTOONID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

#20051226
alter table `CARTOONPET` add MASTERLEVEL int(10) unsigned NOT NULL default 0;
#20060109
alter table `AUCTION` add BIDTYPE tinyint(3) unsigned NOT NULL default 0;
#20060113
alter table `CHARBASE` add PETPACK tinyint(3) unsigned NOT NULL default 0;
#20060116
alter table `MAIL` add FROMID int(10) unsigned not null default 0;
alter table `AUCTION` add OWNERID int(10) unsigned not null default 0;
alter table `AUCTION` add BIDDERID int(10) unsigned not null default 0;
alter table `AUCTION` add BIDDER2ID int(10) unsigned not null default 0;
alter table `AUCTION` add ITEMID int(10) unsigned not null default 0;

#fangq
CREATE TABLE `SORTLIST` (
  `CHARID` int(10) unsigned NOT NULL default '0',
  `LEVEL` smallint(5) unsigned NOT NULL default '0',
  `EXP` bigint(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

insert into SORTLIST(CHARID, LEVEL, EXP) select CHARID, LEVEL, EXP from CHARBASE order by level desc , exp desc limit 2000;

#zjw
alter table CITY add DAREUNIONLIST varchar(255) not null default '';	#挑战者列表

#zjw
ALTER table `UNION` add `MONEY` int(10) unsigned not null default '0';	#帮会资金


# Host: 192.168.2.32
# Database: zjw
# Table: 'CAPTAIN'
# 军队成员表
# 
CREATE TABLE `CAPTAIN` (
		`ARMYID` int(10) unsigned NOT NULL default '0',
		`CHARID` int(10) unsigned NOT NULL default '0',
		`CHARNAME` varchar(33) NOT NULL default '',
		`NPCNUM` int(10) NOT NULL default '0',
		PRIMARY KEY  (`ARMYID`,`CHARID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 


# Host: 192.168.2.32
# Database: zjw
# Table: 'ARMY'
# 国家军队表
# 
CREATE TABLE `ARMY` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`COUNTRYID` int(10) unsigned NOT NULL default '0',
		`CITYID` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`GENNAME` varchar(33) NOT NULL default '',
		`GENID` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

#whj
#ALTER TABLE  `BALBANCE` add `GOLDLIST` int(10) unsigned NOT NULL default '0';
#ALTER TABLE  `BALBANCE` add `MONEYLIST` int(10) unsigned NOT NULL default '0';

#zjw
ALTER TABLE  `SEPT` add `ISEXP` int(10) unsigned NOT NULL default '0';
ALTER TABLE  `CHARBASE` add `LEVELSEPT` int(10) unsigned NOT NULL default '0';

#zjw功勋值同比放大一百倍
UPDATE `CHARBASE` SET EXPLOIT=EXPLOIT*100

#xwl
alter table `CHARBASE` add PETPOINT int(10) unsigned not null default 0;

# Host: 192.168.2.33
# Database: xwl
# Table: 'ACT'
# 运营活动表，记录现有的活动
CREATE TABLE `ACT` (
				`ID` int(10) unsigned NOT NULL default '0',
				`NAME` varchar(32) NOT NULL default '',
				`STATE` tinyint(3) unsigned NOT NULL default '0',
				`TEXT` varchar(255) NOT NULL default ''
				) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.2.33
# Database: xwl
# Table: 'GIFT'
# 运营发放奖品的表，内容由运营来维护
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
				) ENGINE=MyISAM DEFAULT CHARSET=latin1; 
#whj
ALTER TABLE  `CHARBASE` add `CREATEIP` int(10) unsigned NOT NULL default '0';

#xwl
alter table `CHARBASE` add PUNISHTIME int(10) unsigned not null default 0;

# Host: 192.168.2.32
# Database: zjw
# Table: 'ALLY'
# 
CREATE TABLE `ALLY` (
		`COUNTRYID` int(10) unsigned NOT NULL default '0',
		`ALLYCOUNTRYID` int(10) unsigned NOT NULL default '0',
		`CREATETIME` int(10) unsigned NOT NULL default '0',
		`LASTUPTIME` int(10) unsigned NOT NULL default '0',
		`FRIENDDEGREE` int(10) unsigned NOT NULL default '0'
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

#xwl
alter table `COUNTRY` add FORBIDTALK int(10) unsigned not null default 0;
alter table `COUNTRY` add SENDPRISON int(10) unsigned not null default 0;
alter table `COUNTRY` add NOTE char(255) not null default '';

#xwl 03-22
alter table `CARTOONPET` add REPAIR tinyint(3) unsigned not null default 0;

#xwl 03-28
alter table  `COUNTRY` add GEN_EXP int(10) unsigned not null default 0;
alter table  `COUNTRY` add GEN_MAXEXP int(10) unsigned not null default 0;
alter table  `COUNTRY` add GEN_LEVEL int(10) unsigned not null default 0;

#xwl 03-30
# Host: 192.168.2.33
# Database: xwl
# Table: 'EMPERORFORBID'
# 
CREATE TABLE `EMPERORFORBID` (
		`DATA` char(40) character set latin1 collate latin1_bin NOT NULL default '0'
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 


#xwl 03-22
alter table `CHARBASE` add TRAINTIME int(10) unsigned not null default 0;

#zjw 04-05
ALTER TABLE	`COUNTRY` add `DIPLOMATNAME` varchar(33) NOT NULL default '';
ALTER TABLE	`COUNTRY` add `CATCHERNAME` varchar(33) NOT NULL default '';

#zjw 04-11
ALTER TABLE `COUNTRY` ADD `LASTDAILYMONEY` int(10) unsigned not null default 0;

#whj 04-14
ALTER TABLE	`COUNTRY` add CALLTIMES int(10) unsigned not null default 0;
ALTER TABLE	`UNION` add CALLTIMES int(10) unsigned not null default 0;
ALTER TABLE	`SEPT` add CALLTIMES int(10) unsigned not null default 0;
ALTER TABLE	`SEPT` add CALLDAYTIME int(10) unsigned not null default 0;

#zjw 04-19
ALTER TABLE	`CITY` add `CATCHERNAME` varchar(33) NOT NULL default '';
 
#whj 04-24
ALTER TABLE	`COUNTRY` add KINGTIME int(10) unsigned not null default 0;

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
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

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
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

#zjw 05-10
ALTER TABLE	`SEPT` add NORMALEXPTIME int(10) unsigned not null default 0;

#zjw 05-10
# Host: 192.168.2.32
# Database: zjw
# Table: 'RECOMMENDSUB'
# 
CREATE TABLE `RECOMMENDSUB` (
		`ID` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`LASTLEVEL` int(10) unsigned NOT NULL default '0',
		`TOTAL` int(10) unsigned NOT NULL default '0',
		`RECOMMENDID` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.2.32
# Database: zjw
# Table: 'RECOMMEND'
# 
CREATE TABLE `RECOMMEND` (
		`ID` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`TOTAL` int(10) unsigned NOT NULL default '0',
		`BALANCE` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.5.23
# Database: xwl
# Table: 'MINIGAME'
# 
#CREATE TABLE `MINIGAME` (
# 	`CHARID` int(10) unsigned NOT NULL default '0',
# 	`GAMETYPE` tinyint(10) unsigned NOT NULL default '0',
# 	`WIN` int(10) unsigned NOT NULL default '0',
# 	`LOSE` int(10) unsigned NOT NULL default '0',
# 	`DRAW` int(10) unsigned NOT NULL default '0',
# 	`SCORE` int(10) NOT NULL default '0',
# 	PRIMARY KEY  (`GAMETYPE`,`CHARID`),
# 	KEY `TOP` (`GAMETYPE`,`SCORE`),
# 	KEY `LOAD BY ID` (`CHARID`)
# 	) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.5.23
# Database: xwl
# Table: 'MINIGAME'
# 
CREATE TABLE `MINIGAME` (
		`CHARID` int(10) unsigned NOT NULL default '0',
		`NAME` char(32) NOT NULL default '',
		`COUNTRY` int(10) unsigned NOT NULL default '0',
		`FACE` int(10) unsigned NOT NULL default '0',
		`WIN` int(10) unsigned NOT NULL default '0',
		`LOSE` int(10) unsigned NOT NULL default '0',
		`DRAW` int(10) unsigned NOT NULL default '0',
		`SCORE` int(10) NOT NULL default '0',
		PRIMARY KEY  (`CHARID`),
		KEY `TOP` (`SCORE`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

#zjw 05-19
ALTER TABLE	`SCHOOLMEMBER` add MASTERBALANCE int(10) unsigned not null default 0;
ALTER TABLE	`SCHOOLMEMBER` add MASTERTOTAL bigint(20) unsigned not null default 0;
ALTER TABLE	`SCHOOLMEMBER` add PRENTICETOTAL int(10) unsigned not null default 0;
ALTER TABLE	`SCHOOLMEMBER` add PRENTICELASTLVL int(10) unsigned not null default 0;

#xwl 05-24
alter table `MINIGAME` add MONEY int(10) unsigned not null default 0;
