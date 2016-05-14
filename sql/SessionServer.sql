# host: 192.168.2.15
# Database: zebra2
# Table: 'SAMPLERELATION'
# 社会关系表包括黑名单和好友等一对一的社会关系

DROP TABLE IF EXISTS `SAMPLERELATION`;
CREATE TABLE `SAMPLERELATION` (
		`CHARID` int(10) unsigned NOT NULL default '0',          #角色的ID
		`RELATIONID` int(10) unsigned NOT NULL default '0',      #其关系人的ID
		`RELATIONNAME` varchar(33) NOT NULL default '',          #其关系人的名字
		`TYPE` tinyint(3) unsigned NOT NULL default '0',         #关系类型
		`LASTTIME` int(10) unsigned NOT NULL default '0',        #最后组队时间
		`OCCUPATION` smallint(5) unsigned NOT NULL default '0',  #头像定义
		`DEGREE` smallint(5) unsigned NOT NULL default '0',      #好友度
		PRIMARY KEY  (`CHARID`,`RELATIONID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'SCHOOL'

# 门派定义

DROP TABLE IF EXISTS `SCHOOL`;
CREATE TABLE `SCHOOL` (
		`SCHOOLID` int(10) unsigned NOT NULL auto_increment,    #门派ID为0表示其为师徒关系(0这条记录不存档在内存中创建
		`NAME` varchar(33) NOT NULL default '',                 #门派名称
		`MASTERSERIAL` int(10) unsigned NOT NULL default '0',   #师尊的 CHARID
		`BULLETIN` blob NOT NULL,                               #公告
		PRIMARY KEY  (`SCHOOLID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'SCHOOLMEMBER'
# 
# 门派成员和师徒成员表
#
DROP TABLE IF EXISTS `SCHOOLMEMBER`;
CREATE TABLE `SCHOOLMEMBER` (
		`SERIALID` int(10) unsigned NOT NULL auto_increment,           #本节点的ID
		`MASTERID` int(10) unsigned NOT NULL default '0',              #师傅的ID
		`PRESERIALID` int(10) unsigned NOT NULL default '0',           #前一节点的ID 在门派中如果师傅被开那么MASTERID将会为0但是前一节点并不为0
		`CHARID` int(10) unsigned NOT NULL default '0',                #角色ID
		`NAME` varchar(33) NOT NULL default '',                        #角色名称
		`LEVEL` smallint(5) unsigned NOT NULL default '0',             #角色等级
		`JOINTIME` int(10) unsigned NOT NULL default '0',              #师徒关系建立时间
		`DEGREE` smallint(5) unsigned NOT NULL default '0',            #友好度
		`LASTTIME` int(10) unsigned NOT NULL default '0',              #最后组队时间
		`SCHOOLID` int(10) unsigned NOT NULL default '0',              #门派ID 为0表示是师徒关系
		`OCCUPATION` smallint(5) unsigned NOT NULL default '0',        #头像定义
		`TAG` tinyint(3) unsigned NOT NULL default '0',                #节点有效标志
		`MASTERBALANCE` int(10) unsigned NOT NULL default '0',		   #做为师傅,当前可以领取的进贡金额
		`PRENTICETOTAL` int(10) unsigned NOT NULL default '0',		   #做为徒弟,累计的进贡金额
		`PRENTICELASTLVL` int(10) unsigned NOT NULL default '0',	   #徒弟上次进贡时的等级
		`MASTERTOTAL` bigint(20) unsigned NOT NULL default '0',        #做为师傅,累计领取的进贡金额
		PRIMARY KEY  (`SERIALID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'SEPT'
# 
# 家族定义表
#
DROP TABLE IF EXISTS `SEPT`;
CREATE TABLE `SEPT` (
		`SEPTID` int(10) unsigned NOT NULL auto_increment, # 家族ID
		`NAME` varchar(33) NOT NULL default '', # 家族名称
		`CHARID` int(10) unsigned NOT NULL default '0', # 族长ID
		`MASTER` varchar(33) NOT NULL default '',	# 族长姓名
		`VOTE` tinyint(3) unsigned NOT NULL default '0', # 投票状态
		`CREATETIME` int(10) unsigned NOT NULL default '0',# 创建时间
		`COUNTRYID` int(10) unsigned NOT NULL default '0',# 所属国家
		`REPUTE` int(10) unsigned NOT NULL default '0', # 声望
		`UNIONID` int(10) unsigned NOT NULL default '0', # 所属帮会
		`LEVEL` int(10) unsigned NOT NULL default '0', # 家族等级
		`NOTE` varchar(255) NOT NULL default '',	# 说明
		`SPENDGOLD` int(10) unsigned NOT NULL default '0', # 家族消费金币数
		`ISEXP` int(10) unsigned NOT NULL default '0', # 是否领取当日占领家族经验
		`NORMALEXPTIME` int(10) unsigned NOT NULL default '0', # 最后一次领取当日普通家族经验的时
		`CALLTIMES` int(10) unsigned NOT NULL default '0',
		`CALLDAYTIME` timestamp NOT NULL default CURRENT_TIMESTAMP,
		PRIMARY KEY  (`CHARID`, `SEPTID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'SEPTMEMBER'
# 
# 家族成员表
#
DROP TABLE IF EXISTS `SEPTMEMBER`;
CREATE TABLE `SEPTMEMBER` (
		`SEPTID` int(10) unsigned NOT NULL default '0',        #家族ID
		`CHARID` int(10) unsigned NOT NULL default '0',        #角色ID
		`ALIASNAME` varchar(33) NOT NULL default '',           # 族别名
		`NAME` varchar(33) NOT NULL default '',                #角色名称
		`OCCUPATION` smallint(5) unsigned NOT NULL default '0',#头像定义
		PRIMARY KEY  (`CHARID`,`SEPTID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'UNION'
# 
# 帮会定义表
#
DROP TABLE IF EXISTS `UNION`;
CREATE TABLE `UNION` (
		`UNIONID` int(10) unsigned NOT NULL auto_increment, # 帮会ID
		`NAME` varchar(33) NOT NULL default '',             # 帮会名称
		`CHARID` int(10) unsigned NOT NULL default '0',     # 帮主ID
		`LEVEL` smallint(5) NOT NULL default '0',           # 帮会级别
		`MASTER` varchar(33) NOT NULL default '',           # 帮主
		`EXP` bigint(20) NOT NULL default '0',              # 帮会经验值
		`VOTE` tinyint(3) unsigned NOT NULL default '0',    # 是否在投票期
		`CREATETIME` int(10) unsigned NOT NULL default '0', # 帮会建立时间
		`COUNTRYID` int(10) unsigned NOT NULL default '0',  # 国家ID
		`MANA` int(10) unsigned NOT NULL default '0',       # 帮会威望
		`ACTIONPOINT` int(10) unsigned NOT NULL default '0',# 帮会行动力
		`MONEY` int(10) unsigned NOT NULL default '0',	    # 帮会资金
		`NOTE` varchar(255) NOT NULL default '',            # 帮会描述
		`CALLTIMES` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`UNIONID`)
		) TYPE=MyISAM; 

# Host: 192.168.2.15
# Database: zebra2
# Table: 'UNIONMEMBER'
# 
# 帮会成员表
#
DROP TABLE IF EXISTS `UNIONMEMBER`;
CREATE TABLE `UNIONMEMBER` (
		`UNIONID` int(10) unsigned NOT NULL default '0', # 帮会ID
		`CHARID` int(10) unsigned NOT NULL default '0', # 帮众ID
		`ALIASNAME` varchar(33) NOT NULL default '', # 帮众别名
		`NAME` varchar(33) NOT NULL default '', # 帮众姓名
		`OCCUPATION` smallint(5) unsigned NOT NULL default '0', # 帮众成员职业
		`POWER` int(10) unsigned NOT NULL default '0',   # 帮众成员权限
		`SEPTID` int(10) unsigned NOT NULL default '0', # 会员所属家族ID
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
# NPC争夺战定义用表，数据记录里包含可争夺NPC的 国家ID, 地图ID, NPC ID, 坐标， 控制家族的ID, 挑战家族的ID, 当前可领金币
#

DROP TABLE IF EXISTS `NPCDARE`;
CREATE TABLE `NPCDARE` (
  `COUNTRY` int(10) unsigned NOT NULL default '0',     #NPC所属国家ID
  `MAPID` int(10) unsigned NOT NULL default '0',       #NPC所属地图ID
  `NPCID` int(10) unsigned NOT NULL default '0',       #NPC  ID
  `POSX` int(10) unsigned NOT NULL default '0',        #NPC在地图中的X坐标
  `POSY` int(10) unsigned NOT NULL default '0',        #NPC在地图中的Y坐标
  `HOLDSEPTID` int(10) unsigned NOT NULL default '0',  #控制该NPC的家族ID
  `DARESEPTID` int(10) unsigned NOT NULL default '0',  #争夺该NPC的家族ID
  `GOLD` int(10) unsigned NOT NULL default '0'         #当前玩家可领取的银子
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

# 主机: 192.168.2.32
# 数据库: zebra1
# 表: 'CITY'

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

# 主机: 192.168.2.32
# 数据库: zebra1
# 表: 'FORBIDTALK'
# 通过GM工具处罚玩家用的表
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
# 玩家向GM提交定单用的表
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
# 拍卖表
# 

DROP TABLE IF EXISTS `AUCTION`;
CREATE TABLE `AUCTION` (
		`ID` int(10) unsigned NOT NULL auto_increment,		#拍卖记录ID
		`OWNER` varchar(100) NOT NULL default '',		#拍卖者
		`STATE` tinyint(3) unsigned NOT NULL default '0',	#拍卖状态
		`CHARGE` int(10) unsigned NOT NULL default '0',		#取消拍卖时，需要的手续费
		`NAME` varchar(32) NOT NULL default '',			#物品名字
		`TYPE` tinyint(3) NOT NULL default '0',			#物品类型
		`QUALITY` tinyint(3) NOT NULL default '0',		#品质
		`NEEDLEVEL` smallint(5) NOT NULL default '0',		#等级
		`MINMONEY` int(10) unsigned NOT NULL default '0',	#起价
		`MAXMONEY` int(10) unsigned NOT NULL default '0',	#一口价
		`MINGOLD` int(10) unsigned NOT NULL default '0',	#起价（金币）
		`MAXGOLD` int(10) unsigned NOT NULL default '0',	#一口价（金币）
		`BIDDER` varchar(32) NOT NULL default '',		#最高出价人
		`BIDDER2` varchar(32) NOT NULL default '',		#次高出价人
		`STARTTIME` int(10) unsigned NOT NULL default '0',	#开始时间
		`ENDTIME` int(10) unsigned NOT NULL default '0',	#结束时间
		`BIDTYPE` tinyint(3) unsigned NOT NULL default '0',	#竟价方式，金币还是银币
		`ITEM` blob NOT NULL,					#物品，二进制
		`OWNERID` int(10) unsigned NOT NULL default '0',	#拍卖者的ID
		`BIDDERID` int(10) unsigned NOT NULL default '0',	#最高出价人ID
		`BIDDER2ID` int(10) unsigned NOT NULL default '0',	#次高出价人ID
		`ITEMID` int(10) unsigned NOT NULL default '0',		#物品的唯一ID
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Host: 192.168.2.33
# Database: xwl
# Table: 'MAIL'
# 邮件
# 

DROP TABLE IF EXISTS `MAIL`;
CREATE TABLE `MAIL` (
		`ID` int(10) unsigned NOT NULL auto_increment,		#邮件ID
		`STATE` tinyint(3) unsigned NOT NULL default '0',	#邮件状态
		`FROMNAME` varchar(32) NOT NULL default '',		#发送者
		`TONAME` varchar(32) NOT NULL default '',		#接收者
		`TITLE` varchar(32) NOT NULL default '',		#标题
		`TYPE` tinyint(3) unsigned NOT NULL default '0',	#邮件类型
		`CREATETIME` int(10) unsigned NOT NULL default '0',	#发送时间
		`DELTIME` int(10) unsigned NOT NULL default '0',	#过期时间
		`ACCESSORY` tinyint(3) unsigned NOT NULL default '0',	#是否有附件
		`ITEMGOT` tinyint(3) unsigned NOT NULL default '0',	#附件是否已经收取
		`TEXT` varchar(255) NOT NULL default '',		#内容文本
		`SENDMONEY` int(10) unsigned NOT NULL default '0',	#发送金钱
		`RECVMONEY` int(10) unsigned NOT NULL default '0',	#收取金钱
		`SENDGOLD` int(10) unsigned NOT NULL default '0',	#发送金币数量
		`RECVGOLD` int(10) unsigned NOT NULL default '0',	#收取金币数量
		`BIN` blob,						#物品，二进制
		`TOID` int(10) unsigned NOT NULL default '0',		#收件人ID
		`FROMID` int(10) unsigned NOT NULL default '0',		#寄件人ID
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


# Host: 192.168.2.33
# Database: xwl
# Table: 'SN'
# 序列号，运行活动发放序列号用
# 

DROP TABLE IF EXISTS `SN`;
	CREATE TABLE `SN` (
			`SN` varchar(16) NOT NULL default '',		#序列号
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


# 主机: 192.168.2.32
# 数据库: zebra3
# 表: 'SORTLIST'
# 
# 角色等级排名表，此表存储前2000名角色的id,级别，经验等数据
#

DROP TABLE IF EXISTS `SORTLIST`;
CREATE TABLE `SORTLIST` (
  `CHARID` int(10) unsigned NOT NULL default '0',      #角色ID
  `LEVEL` smallint(5) unsigned NOT NULL default '0',   #角色等级
  `EXP` bigint(20) unsigned NOT NULL default '0',      #角色经验
  PRIMARY KEY  (`CHARID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'CAPTAIN'
# 军队成员表
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
# 国家军队表
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
# 运营活动表，记录现有的活动

DROP TABLE IF EXISTS `ACT`;
CREATE TABLE `ACT` (
				`ID` int(10) unsigned NOT NULL default '0',			#活动ID
				`NAME` varchar(32) NOT NULL default '',				#活动名字
				`STATE` tinyint(3) unsigned NOT NULL default '0',	#状态
				`TEXT` varchar(255) NOT NULL default ''				#描述
				) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.33
# Database: xwl
# Table: 'GIFT'
# 运营发放奖品的表，内容由运营来维护

DROP TABLE IF EXISTS `GIFT`;
CREATE TABLE `GIFT` (
				`ACTID` int(10) unsigned NOT NULL default '0',		#活动ID
				`NAME` varchar(32) NOT NULL default '',				#中奖人名字
				`CHARID` int(10) unsigned NOT NULL default '0',		#中奖人CHARID
				`ITEMGOT` tinyint(3) unsigned NOT NULL default '0',	#是否已经领取
				`ITEMID` int(10) unsigned NOT NULL default '0',		#奖励物品ID
				`ITEMTYPE` tinyint(3) unsigned NOT NULL default '0',#奖励物品类型
				`ITEMNUM` int(10) unsigned NOT NULL default '0',	#奖励物品数量
				`BIND` tinyint(3) unsigned NOT NULL default '0',	#是否绑定
				`MONEY` int(10) unsigned NOT NULL default '0',		#奖励金钱
				`MAILTEXT` varchar(255) NOT NULL default ''			#邮件的内容
				) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'ALLY'
# 

DROP TABLE IF EXISTS `ALLY`;
CREATE TABLE `ALLY` (
		`COUNTRYID` int(10) unsigned NOT NULL default '0',	# 联盟国ID1
		`ALLYCOUNTRYID` int(10) unsigned NOT NULL default '0',  # 联盟国ID2
		`CREATETIME` int(10) unsigned NOT NULL default '0',     # 联盟建立时间
		`LASTUPTIME` int(10) unsigned NOT NULL default '0',     # 联盟数据最后更新时间
		`FRIENDDEGREE` int(10) unsigned NOT NULL default '0'    # 联盟友好度
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Host: 192.168.2.33
# Database: xwl
# Table: 'EMPERORFORBID'
# 皇帝禁言玩家的表

DROP TABLE IF EXISTS `EMPERORFORBID`;
CREATE TABLE `EMPERORFORBID` (
		`DATA` char(40)  NOT NULL default '0'
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


# Host: 192.168.2.32
# Database: zjw
# Table: 'RECOMMENDSUB'
# 被推荐人表

DROP TABLE IF EXISTS `RECOMMENDSUB`;
CREATE TABLE `RECOMMENDSUB` (
		`ID` int(10) unsigned NOT NULL default '0', # 被推荐人CHARID
		`NAME` varchar(33) NOT NULL default '', # 被推荐人姓名
		`LASTLEVEL` int(10) unsigned NOT NULL default '0', # 被推荐人上次领取奖励的等级
		`TOTAL` int(10) unsigned NOT NULL default '0', # 累计领取奖励值
		`RECOMMENDID` int(10) unsigned NOT NULL default '0', # 推荐人ID
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


# Host: 192.168.2.32
# Database: zjw
# Table: 'RECOMMEND'
# 推荐人表

DROP TABLE IF EXISTS `RECOMMEND`;
CREATE TABLE `RECOMMEND` (
		`ID` int(10) unsigned NOT NULL default '0', # 推荐人ID
		`NAME` varchar(33) NOT NULL default '', # 推荐人姓名
		`TOTAL` int(10) unsigned NOT NULL default '0', # 累计领取奖励值
		`BALANCE` int(10) unsigned NOT NULL default '0', # 当前可领取的奖金额
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8; 



