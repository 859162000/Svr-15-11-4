/**
 * \file
 * \version  $Id: Horse.cpp  $
 * \author 
 * \date 	
 * \brief 	马匹相关
 *
 * 
 */

#include "Horse.h"
#include "SceneUser.h"
#include "Chat.h"
#include "TimeTick.h"

/**     
 * \brief 构造函数
 *
 * 初始化相关变量
 *      
 * \param user: 马匹拥有者
 */  
Horse::Horse(SceneUser& user) : _owner(user), _horse(0), _mount(false)
{
	bzero(&pkData, sizeof(pkData));
	bzero(&data, sizeof(data));
}

/**     
 * \brief 析构函数
 *
 */  
Horse::~Horse()
{

}

/**     
 * \brief 设置马匹的种类
 *
 * \param horse_id: 马匹种类
 * \return 无
 */  
void Horse::horse(DWORD horse_id)
{
	using namespace Cmd;

	if (horseType(horse_id)==HORSE_TYPE_NOTHORSE)
		horse_id = 0;

	_horse = horse_id;

	if (_horse)
	{
		data.id = horse_id;
		zNpcB *b = npcbm.get(data.id);
		if (!b)
		{
			_horse = 0;
			data.id = 0;
			return;
		}

		if (_owner.ridepet)
			_owner.killOnePet(_owner.ridepet);

		data.horseid = horse_id;
		if(b)
		{
			data.lv = b->level;
			interval = b->adistance;
			data.state = Cmd::HORSE_STATE_PUTUP;
			data.callTime = 0;
			summonTime = SceneTimeTick::currentTime;
			bcopy(b->name, data.name, MAX_NAMESIZE-1);

			if (HORSE_TYPE_SUPER!=horseType(horse_id))
			{
				bzero(&data.str, sizeof(DWORD)*5);
				bzero(&data.poisonRes, sizeof(DWORD)*4);
			}
			if (_owner.emperor && HORSE_TYPE_SUPER==horseType(horse_id))
				data.speed += 50;

			sendData();
		}
		else
		{
			Zebra::logger->debug("没有找到id=%d的马",horse_id);
		}
	}
	else
	{
		bzero(&data, sizeof(data));
		bzero(&pkData, sizeof(pkData));

		Cmd::stDelPetPetCmd del;                             
		del.type = Cmd::PET_TYPE_RIDE;                  
		_owner.sendCmdToMe(&del, sizeof(del));
		_owner.clearUState(Cmd::USTATE_RIDE);
	}

	if (_owner.scene)
	{
		//FunctionTimes times(2,"__FUNCTION__(id)");
		_owner.sendMeToNine();
	}
}

/**     
 * \brief 设置马匹的种类
 *
 * \param horse_id: 马匹种类
 * \return 无
 */  
bool Horse::horse(t_Object & obj)
{
	using namespace Cmd;

	if (!horse())
	{   
		Channel::sendSys(&_owner, Cmd::INFO_TYPE_FAIL, "你还没有马，不能使用马甲");
		return false;
	}

	DWORD horse_id = 0, needLevel = 0;
	switch (obj.dwObjectID)
	{
		case 880://100级
			horse_id = 3203;
			needLevel = 100;
			break;
		case 878://120级
			horse_id = 3201;
			needLevel = 120;
			break;
		case 883://130级
			horse_id = 3205;
			needLevel = 130;
			break;
		case 884:
			horse_id = 3206;
			needLevel = 130;
			break;
		case 885:
			horse_id = 3207;
			needLevel = 130;
			break;
		case 886:
			horse_id = 3208;
			needLevel = 130;
			break;
		case 887:
			horse_id = 3209;
			needLevel = 130;
			break;
		default:
			{
				Zebra::logger->debug("马匹id不对");
				return false;
			}
	}

	if (_owner.charbase.level<needLevel)
	{
		Channel::sendSys(&_owner, Cmd::INFO_TYPE_FAIL, "你的等级不够");
		return false;
	}

	_horse = horse_id;

	if (_horse)
	{
		data.id = horse_id;
		zNpcB *b = npcbm.get(data.id);
		if (!b)
		{
			_horse = 0;
			data.id = 0;
			return false;
		}

		if (_owner.ridepet)
			_owner.killOnePet(_owner.ridepet);

		//data.id = horse_id;
		data.horseid = horse_id;

		data.lv = b->level;
		//data.hp = data.maxhp = b->hp;
		data.speed = obj.mvspeed;
		if (_owner.emperor)
			data.speed += 50;
		//data.hitDown = b->str;
		interval = b->adistance;
		data.state = Cmd::HORSE_STATE_PUTUP;
		data.callTime = 0;
		summonTime = SceneTimeTick::currentTime;
		bcopy(b->name, data.name, MAX_NAMESIZE-1);

		data.str = obj.str;
		data.intel = obj.inte;
		data.dex = obj.dex;
		data.men = obj.spi;
		data.con = obj.con;

		data.pdam = obj.pdam;
		data.pdef = obj.pdef;
		data.mdam = obj.mdam;
		data.mdef = obj.mdef;

		data.maxhp = obj.maxhp;
		data.maxmp = obj.maxmp;

		data.poisonRes = obj.poisondef;
		data.lullRes = obj.lulldef;
		data.faintRes = obj.reeldef;
		data.chaosRes = obj.chaosdef;
		data.freezeRes = obj.colddef;
		data.petrifyRes = obj.petrifydef;
		data.blindRes = obj.blinddef;
		data.slowRes = obj.slowdef;

		sendData();
	}
	else
	{
		bzero(&pkData, sizeof(pkData));
		bzero(&data, sizeof(data));

		Cmd::stDelPetPetCmd del;                             
		//del.id= kill->tempid;                           
		del.type = Cmd::PET_TYPE_RIDE;                  
		_owner.sendCmdToMe(&del, sizeof(del));
		_owner.clearUState(Cmd::USTATE_RIDE);

		return false;
	}

	if (_owner.scene)
	{
		//FunctionTimes times(3,"__FUNCTION__(obj)");
		_owner.sendMeToNine();
	}

	return true;
}

/**     
 * \brief 取得马匹的种类
 *
 * \return 马匹种类
 */  
DWORD Horse::horse() const
{
	return _horse;
}

/**     
 * \brief 切换用户的骑马状态
 *
 * \param flag: 用户要求上马还是下马
 * \param send: 是否需要通知客户端,死亡下马不需要通知客户端,要在重生的时候刷新下去
 * \return 无
 */ 
void Horse::mount(bool flag , bool send)
{
	//ride down
	if (!flag && _mount) {
		_owner.clearUState(Cmd::USTATE_RIDE);
		_mount = false;
		data.state = Cmd::HORSE_STATE_PUTUP;
		/*
		setSummonTime(interval);
		if(send)
		{
			_owner.sendMeToNine();
		}
		*/
	}

	//ride up
	if (flag && !_mount) {
		if (_owner.charbase.level < 30 ) {
			Channel::sendSys(&_owner, Cmd::INFO_TYPE_FAIL, "你不满 30 级，不能骑马。");
			return;
		}

		putAway();//取消跟随
		_owner.setUState(Cmd::USTATE_RIDE);
		_mount = true;
		/*
		if (checkSummonTime())
		{
			putAway();//取消跟随
			_owner.setUState(Cmd::USTATE_RIDE);
			_mount = true;
			_owner.sendMeToNine();
		}
		else
		{
			Channel::sendSys(&_owner, Cmd::INFO_TYPE_FAIL, "您的马正在休息");
			return;
		}
		*/
	}

	getPkData();
	_owner.setupCharBase();
	if(send)
	{
		//FunctionTimes times(4,__FUNCTION__);
		_owner.sendMeToNine();
	}

	Cmd::stMainUserDataUserCmd  userinfo;
	_owner.full_t_MainUserData(userinfo.data);
	_owner.sendCmdToMe(&userinfo,sizeof(userinfo));
}

/**     
 * \brief 返回用户的骑马状态
 *
 * \return 用户是否骑马
 */ 
bool Horse::mount() const
{
	return _mount;
}

/**     
 * \brief 读取用户的马匹信息
 *
 * \param d 二进制档案内容
 * \return 读档的二进制长度
 */ 
int Horse::load(unsigned char* d)
{
	if (0==d) return 0;

	Cmd::t_HorseData * p = (Cmd::t_HorseData *)d;

	bcopy(p, &data, sizeof(data));

	if (0!=data.id)
	{
		_horse = data.id;
		horse(_horse);
		/*
		if (Cmd::HORSE_STATE_RIDE==data.state)
			mount(true);
		else
		*/
			data.state = Cmd::HORSE_STATE_PUTUP;
	}

	//修正错误数据
	if (data.id!=data.horseid)//npcID和道具ID不符
		data.horseid = data.id;
	/*
	   _horse = *data & 0xffff;
	   _mount = (*data >> 16) & 0xffff;

	   if (_horse)
	   horse(_horse);
	   if (_mount)
	   mount(_mount);
	   */

	return sizeof(Cmd::t_HorseData);
}

/**     
 * \brief 存储用户的马匹信息
 *
 * \param d: 二进制档案内容
 * \return 存档的二进制长度
 */ 
int Horse::save(unsigned char *d)
{
	if (0==d) return 0;
	Cmd::t_HorseData * p = (Cmd::t_HorseData *)d;

	//full_HorseDataStruct(p);
	bcopy(&data, p, sizeof(data));
	if (_owner.emperor && HORSE_TYPE_SUPER==horseType())
		p->speed -= 50;

	return sizeof(Cmd::t_HorseData);
}

/**
 * \brief 设置下次召唤马匹的延迟
 *
 *
 * \param delay 延迟时间
 */
void Horse::setSummonTime(int delay)
{
	summonTime = SceneTimeTick::currentTime;
	summonTime.addDelay(delay);
}

/**
 * \brief 设置下次召唤马匹的延迟
 *
 * \return 检查是否可以召唤马匹
 */
bool Horse::checkSummonTime()
{
	return SceneTimeTick::currentTime>=summonTime;
}

/**
 * \brief 召唤出来跟随主人
 * \return 是否成功
 */
bool Horse::comeOut()
{
	if (!(Cmd::HORSE_STATE_FOLLOW==data.state))
	{
		if ((_mount||Cmd::HORSE_STATE_RIDE==data.state))
			mount(false);

		if (_owner.summonPet(data.id, Cmd::PET_TYPE_RIDE))
		{
			data.state = Cmd::HORSE_STATE_FOLLOW;
			return true;
		}
	}
	return false;
}

/**
 * \brief 收起马匹
 *
 * \return 是否成功，没有马匹返回失败
 */
bool Horse::putAway()
{
	if (Cmd::HORSE_STATE_FOLLOW==data.state)
	{
		if (_owner.ridepet)
		{
			_owner.killOnePet(_owner.ridepet);
			//data.state = Cmd::HORSE_STATE_PUTUP;
			//setSummonTime(_owner.ridepet->npc->adistance);
		}
	}
	return false;
}

/**
 * \brief 检查马是否在跟随状态
 *
 * \return 是否在跟随
 */
bool Horse::isFollowing()
{
	return Cmd::HORSE_STATE_FOLLOW==data.state;
}

/**
 * \brief 填充马的信息
 *
 *
 * \param d 信息结构地址
 */
bool Horse::full_HorseDataStruct(Cmd::t_HorseData *d)
{
	if (!_horse) return false;

	/*
	if (summonTime>SceneTimeTick::currentTime)
	{
		data.callTime = summonTime.sec()-SceneTimeTick::currentTime.sec();
	}
	else
	*/
		data.callTime = 0;

	bcopy(&data, d, sizeof(data));

	if (_owner.king)
	{
			d->id = KING_HORSE_ID;
			d->horseid = KING_HORSE_ID;
	}

	if (_owner.emperor)
	{
			d->id = EMPEROR_HORSE_ID;
			d->horseid = EMPEROR_HORSE_ID;
	}

	return true;
}

/**
 * \brief 向客户端发送数据
 *
 */
void Horse::sendData()
{
	if (!_horse) return;
	Cmd::stHorseDataPetCmd ret;
	ret.type = Cmd::PET_TYPE_RIDE;
	full_HorseDataStruct(&ret.data);
	ret.id = ret.data.id;
	_owner.sendCmdToMe(&ret, sizeof(ret));
	Zebra::logger->debug("发送马匹信息 name=%s id=%u horseid=%u state=%u time=%u", data.name, data.id, data.horseid, data.state, data.callTime);
}

/**
 * \brief 判断该马是否可以战斗
 *
 * \return 是否可以战斗
 */
bool Horse::canFight()
{
	if (!_horse) return false;

	if (horseType()==HORSE_TYPE_BATTLE ||
					horseType()==HORSE_TYPE_SUPER) return true;

	//zNpcB *b = npcbm.get(_horse);
	if (!npcbm.get(_horse)) return false;
	zObjectB *base = objectbm.get(_horse);
	if (!base) return false;

	if (base->kind == ItemType_BattleHorse && data.lv>=10)
		//if (3200 == _horse && data.lv>=10)
		return true;
	else
		return false;
}

void Horse::getPkData()
{
		if (_horse && HORSE_TYPE_SUPER==horseType() && mount())
				bcopy(&data, &pkData, sizeof(pkData));
		else
				bzero(&pkData, sizeof(pkData));
}

DWORD Horse::horseType()
{
		if (3000==_horse) return HORSE_TYPE_NORMAL;
		if (3200==_horse) return HORSE_TYPE_BATTLE;
		if (_horse>=3201 && _horse<=3209) return HORSE_TYPE_SUPER;
		return HORSE_TYPE_NOTHORSE;
}

DWORD Horse::horseType(DWORD type)
{
		if (3000==type) return HORSE_TYPE_NORMAL;
		if (3200==type) return HORSE_TYPE_BATTLE;
		if (type>=3201 && type<=3203) return HORSE_TYPE_SUPER;
		if (type>=3205 && type<=3209) return HORSE_TYPE_SUPER;
		return HORSE_TYPE_NOTHORSE;
}
