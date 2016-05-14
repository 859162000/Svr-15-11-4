-- MySQL dump 9.11
--
-- Host: localhost    Database: LoginServer
-- ------------------------------------------------------
-- Server version	4.0.21yes

DROP TABLE IF EXISTS `LOGIN`;
CREATE TABLE `LOGIN` (
		`USERID` int(10) unsigned NOT NULL auto_increment,
		`LOGINID` varchar(32) default NULL,
		`PASSWORD` varchar(16) NOT NULL default '',
		`ISUSED` tinyint(3) unsigned NOT NULL default '0',
		`ISFORBID` tinyint(3) unsigned NOT NULL default '0',
		`NAME` varchar(32) default NULL,
		`BIRTH` varchar(16) default NULL,
		`PHONE` varchar(32) default NULL,
		`QUESTION1` varchar(64) default NULL,
		`ANSWER1` varchar(64) default NULL,
		`QUESTION2` varchar(64) default NULL,
		`ANSWER2` varchar(64) default NULL,
		`EMAIL` varchar(64) default NULL,
		`IDCARD` varchar(32) default NULL,
		`MOBILE` varchar(32) default NULL,
		`COMMENDNAME` varchar(32) default NULL,
		`CREATEDATE` datetime default NULL,
		`LASTACTIVEDATE` datetime default NULL,
		PRIMARY KEY  (`USERID`),
		KEY `unique` (`LOGINID`)

		) TYPE=MyISAM; 

DROP TABLE IF EXISTS `LOGINSTAT`;
CREATE TABLE `LOGINSTAT` (
		`STATDAY` varchar(64) NOT NULL default '',
		`LOGINCOUNT` int(10) unsigned NOT NULL default '0',
		`LOGINSUCCESSCOUNT` int(10) unsigned NOT NULL default '0',
		`CREATEACCOUNT` int(10) unsigned NOT NULL default '0',
		`CREATESUCCESSACCOUNT` int(10) unsigned NOT NULL default '0',
		`CREATERECORD` int(10) unsigned NOT NULL default '0',
		`CREATESUCCESSRECORD` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`STATDAY`)
		) TYPE=MyISAM; 

