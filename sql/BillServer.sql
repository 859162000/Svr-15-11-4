# Host: 192.168.2.31
# Database: zebra1
# Table: 'BALANCE'
# 
CREATE TABLE `BALANCE` (
		`ACCOUNT` varchar(48) NOT NULL default '',
		`ACCID` int(10) unsigned NOT NULL default '0',
		`PASSWORD` varchar(16) default '',
		`ALLGOLDIN` int(10) unsigned NOT NULL default '0',
		`ALLGOLDOUT` int(10) unsigned NOT NULL default '0',
		`ALLMONEYIN` int(10) unsigned NOT NULL default '0',
		`ALLMONEYOUT` int(10) unsigned NOT NULL default '0',
		`LASTTIME` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
		`GOLD` int(10) unsigned NOT NULL default '0',
		`MONEY` int(10) unsigned NOT NULL default '0',
		`GOLDTAX` int(10) unsigned NOT NULL default '0',
		`MONEYTAX` int(10) unsigned NOT NULL default '0',
		`GOLDLIST` int(10) unsigned NOT NULL default '0',
		`MONEYLIST` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ACCID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# Host: 192.168.2.31
# Database: zebra1
# Table: 'CONSIGNGOLD'
# 
CREATE TABLE `CONSIGNGOLD` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`ACCID` int(10) unsigned NOT NULL default '0',
		`NUM` int(10) unsigned NOT NULL default '0',
		`PRICE` int(10) unsigned NOT NULL default '0',
		`TIME` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`),
		KEY `ACCID` (`ACCID`),
		KEY `PRICE` (`PRICE`,`NUM`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# Host: 192.168.2.31
# Database: zebra1
# Table: 'CONSIGNGOLDHISTORY'
# 
CREATE TABLE `CONSIGNGOLDHISTORY` (
		`ID` int(10) NOT NULL default '0',
		`ACCID` int(10) unsigned NOT NULL default '0',
		`NUM` int(10) unsigned NOT NULL default '0',
		`COMMITPRICE` int(10) unsigned NOT NULL default '0',
		`PRICE` int(10) unsigned NOT NULL default '0',
		`COMMITTIME` int(10) unsigned NOT NULL default '0',
		`SYSMONEY` int(10) unsigned NOT NULL default '0',
		`OKTIME` int(10) unsigned NOT NULL default '0',
		KEY `ACCID` (`ACCID`),
		KEY `OKTIME` (`OKTIME`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# Host: 192.168.2.31
# Database: zebra1
# Table: 'CONSIGNMONEY'
# 
CREATE TABLE `CONSIGNMONEY` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`ACCID` int(10) unsigned NOT NULL default '0',
		`NUM` int(10) unsigned NOT NULL default '0',
		`PRICE` int(10) unsigned NOT NULL default '0',
		`TIME` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`ID`),
		KEY `ACCID` (`ACCID`),
		KEY `PRICE` (`PRICE`,`NUM`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

# Host: 192.168.2.31
# Database: zebra1
# Table: 'CONSIGNMONEYHISTORY'
# 
CREATE TABLE `CONSIGNMONEYHISTORY` (
		`ID` int(10) NOT NULL default '0',
		`ACCID` int(10) unsigned NOT NULL default '0',
		`NUM` int(10) unsigned NOT NULL default '0',
		`COMMITPRICE` int(10) unsigned NOT NULL default '0',
		`PRICE` int(10) unsigned NOT NULL default '0',
		`COMMITTIME` int(10) unsigned NOT NULL default '0',
		`SYSMONEY` int(10) unsigned NOT NULL default '0',
		`OKTIME` int(10) unsigned NOT NULL default '0',
		KEY `ACCID` (`ACCID`),
		KEY `OKTIME` (`OKTIME`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


