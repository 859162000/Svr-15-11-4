CREATE TABLE `CITY`(
`COUNTRY` int(10) unsigned NOT NULL default '0',`CITYID` int(10) unsigned NOT NULL default '0',
`UNIONID` int(10) unsigned NOT NULL default '0',
`LASTAWARDTIME` timestamp NOT NULL default CURRENT_TIMESTAMP,
`ISAWARD` int(11) NOT NULL default '0',
`DAREUNIONID` int(10) unsigned NOT NULL default '0',
`DAREUNIONLIST` varchar(255) not null default '',	#挑战者列表
`GOLD` int(10) unsigned NOT NULL default '0'
) TYPE=MyISAM ;
