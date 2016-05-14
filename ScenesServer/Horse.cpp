/**
 * \file
 * \version  $Id: Horse.cpp  $
 * \author 
 * \date 	
 * \brief 	��ƥ���
 *
 * 
 */

#include "Horse.h"
#include "SceneUser.h"
#include "Chat.h"
#include "TimeTick.h"

/**     
 * \brief ���캯��
 *
 * ��ʼ����ر���
 *      
 * \param user: ��ƥӵ����
 */  
Horse::Horse(SceneUser& user) : _owner(user), _horse(0), _mount(false)
{
	bzero(&pkData, sizeof(pkData));
	bzero(&data, sizeof(data));
}

/**     
 * \brief ��������
 *
 */  
Horse::~Horse()
{

}

/**     
 * \brief ������ƥ������
 *
 * \param horse_id: ��ƥ����
 * \return ��
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
			Zebra::logger->debug("û���ҵ�id=%d����",horse_id);
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
 * \brief ������ƥ������
 *
 * \param horse_id: ��ƥ����
 * \return ��
 */  
bool Horse::horse(t_Object & obj)
{
	using namespace Cmd;

	if (!horse())
	{   
		Channel::sendSys(&_owner, Cmd::INFO_TYPE_FAIL, "�㻹û��������ʹ�����");
		return false;
	}

	DWORD horse_id = 0, needLevel = 0;
	switch (obj.dwObjectID)
	{
		case 880://100��
			horse_id = 3203;
			needLevel = 100;
			break;
		case 878://120��
			horse_id = 3201;
			needLevel = 120;
			break;
		case 883://130��
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
				Zebra::logger->debug("��ƥid����");
				return false;
			}
	}

	if (_owner.charbase.level<needLevel)
	{
		Channel::sendSys(&_owner, Cmd::INFO_TYPE_FAIL, "��ĵȼ�����");
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
 * \brief ȡ����ƥ������
 *
 * \return ��ƥ����
 */  
DWORD Horse::horse() const
{
	return _horse;
}

/**     
 * \brief �л��û�������״̬
 *
 * \param flag: �û�Ҫ������������
 * \param send: �Ƿ���Ҫ֪ͨ�ͻ���,����������Ҫ֪ͨ�ͻ���,Ҫ��������ʱ��ˢ����ȥ
 * \return ��
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
			Channel::sendSys(&_owner, Cmd::INFO_TYPE_FAIL, "�㲻�� 30 ������������");
			return;
		}

		putAway();//ȡ������
		_owner.setUState(Cmd::USTATE_RIDE);
		_mount = true;
		/*
		if (checkSummonTime())
		{
			putAway();//ȡ������
			_owner.setUState(Cmd::USTATE_RIDE);
			_mount = true;
			_owner.sendMeToNine();
		}
		else
		{
			Channel::sendSys(&_owner, Cmd::INFO_TYPE_FAIL, "������������Ϣ");
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
 * \brief �����û�������״̬
 *
 * \return �û��Ƿ�����
 */ 
bool Horse::mount() const
{
	return _mount;
}

/**     
 * \brief ��ȡ�û�����ƥ��Ϣ
 *
 * \param d �����Ƶ�������
 * \return �����Ķ����Ƴ���
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

	//������������
	if (data.id!=data.horseid)//npcID�͵���ID����
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
 * \brief �洢�û�����ƥ��Ϣ
 *
 * \param d: �����Ƶ�������
 * \return �浵�Ķ����Ƴ���
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
 * \brief �����´��ٻ���ƥ���ӳ�
 *
 *
 * \param delay �ӳ�ʱ��
 */
void Horse::setSummonTime(int delay)
{
	summonTime = SceneTimeTick::currentTime;
	summonTime.addDelay(delay);
}

/**
 * \brief �����´��ٻ���ƥ���ӳ�
 *
 * \return ����Ƿ�����ٻ���ƥ
 */
bool Horse::checkSummonTime()
{
	return SceneTimeTick::currentTime>=summonTime;
}

/**
 * \brief �ٻ�������������
 * \return �Ƿ�ɹ�
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
 * \brief ������ƥ
 *
 * \return �Ƿ�ɹ���û����ƥ����ʧ��
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
 * \brief ������Ƿ��ڸ���״̬
 *
 * \return �Ƿ��ڸ���
 */
bool Horse::isFollowing()
{
	return Cmd::HORSE_STATE_FOLLOW==data.state;
}

/**
 * \brief ��������Ϣ
 *
 *
 * \param d ��Ϣ�ṹ��ַ
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
 * \brief ��ͻ��˷�������
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
	Zebra::logger->debug("������ƥ��Ϣ name=%s id=%u horseid=%u state=%u time=%u", data.name, data.id, data.horseid, data.state, data.callTime);
}

/**
 * \brief �жϸ����Ƿ����ս��
 *
 * \return �Ƿ����ս��
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
