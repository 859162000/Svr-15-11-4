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
		`MONEY` int(10) unsigned NOT NULL default '0',
		`SCORE` int(10) NOT NULL default '0',
		PRIMARY KEY  (`CHARID`),
		KEY `TOP` (`SCORE`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

