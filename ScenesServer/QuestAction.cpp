/**
 * \file
 * \version	$Id: QuestAction.cpp $
 * \author	
 * \date	
 * \brief	任务系统
 * 
 */

#include "QuestAction.h"

#include "SceneUser.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Chat.h"
#include "GuardNpc.h"
#include "SessionClient.h"

/**     
 * \brief  执行脚本定义的动作
 *
 * template method模式,进行参数检查，并执行脚本定义的动作
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */     
int Action::do_it (SceneUser* user, Vars* vars)
{
	if (!check_args(user, vars)) return Action::FAILED;	
	
	return done(user, vars);
}

/**     
 * \brief  参数检查
 *
 * 提供了一个默认的参数需求检测,继承类需要根据自己的要求重载此函数
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return true表示参数合法,false表示参数非法
 */     
bool Action::check_args(SceneUser* user, Vars* vars) const
{
	if (user && vars) return true;
	
	return false;
}

/**     
 * \brief  执行脚本定义的动作
 *
 * template method模式,进行参数检查，并对于队伍中的每一个成员执行脚本定义的动作
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */   
int TeamAction::do_it(SceneUser* user, Vars* vars)
{
	if (!check_args(user, vars) ) return false;
	
	if (_team) {
		DWORD leader_id = user->team.getLeader();
		//SceneUser* leader = SceneUserManager::getMe().getUserByTempID(leader_id);
		SceneUser* leader = SceneUserManager::getMe().getUserByTempID(leader_id);
		int result = 0;
		if (leader) {
			Team& team = const_cast<Team&>(leader->team.getTeam());

			team.rwlock.rdlock();
			std::vector<TeamMember>::iterator it = team.member.begin();;
			for(; it!=team.member.end(); ++it) {
				SceneUser* member = SceneUserManager::getMe().getUserByTempID(it->tempid);
				if (member) {
					Vars* v = member->quest_list.vars(vars->quest_id());
/*					
					bool new_vars = false;
					if (!v) {
						new_vars = true;
						v = new Vars(vars->quest_id());	
					}
*/
					if (v) result |= done(member, v);
/*
					if (new_vars) {
						member->quest_list.add_quest(vars->quest_id(), *v, *member);
						SAFE_DELETE(v);
					}
*/
				}
			}
			team.rwlock.unlock();
		}else {
			result = done(user, vars);		
		}
		
		return result;	
	}	

	return done(user, vars);
}

/**     
 * \brief  系统聊天
 *
 * 重载了done函数,发送系统聊天信息给特定用户
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */     	
int NotifyAction::done (SceneUser* user, Vars* vars)
{
	Channel::sendSys(user, Cmd::INFO_TYPE_GAME, _info.c_str());
	return Action::SUCCESS;
}

/**     
 * \brief  任务日志
 *
 * 重载了done函数,输出任务日志到Log中。
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */     	
int LogAction::done (SceneUser* user, Vars* vars)
{
	Zebra::logger->trace("任务日志: 角色%s, %s", user->name, _info.c_str());
	return Action::SUCCESS;
}

int BulletinAction::done (SceneUser* user, Vars* vars)
{
	Cmd::Session::t_QuestBulletinUserCmd cmd;
	cmd.kind = _kind;
	strncpy(cmd.content, _info.c_str(), _info.length());
	if (_kind == 1) {
		cmd.id = user->charbase.unionid;
	}
	if (_kind == 2) {
		cmd.id = user->charbase.septid;
	}
	
	sessionClient->sendCmd(&cmd, sizeof(cmd));
	return Action::SUCCESS;
}

/**     
 * \brief  系统聊天
 *
 * 重载了done函数,发送系统聊天信息给特定用户
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */     	
int Notify1Action::done (SceneUser* user, Vars* vars)
{
	Channel::sendSys(user, Cmd::INFO_TYPE_EXP, _info.c_str());
	return Action::SUCCESS;
}

/**     
 * \brief  菜单
 *
 * 重载了done函数,设置用户访问的npc的任务菜单
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */  
   	
int MenuAction::done (SceneUser* user, Vars* vars)
{
	user->quest_list.set_menu(_menu);
	return Action::SUCCESS;
}

/**     
 * \brief  菜单
 *
 * 重载了done函数,设置用户访问的npc的任务菜单
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */  
   	
int SubMenuAction::done (SceneUser* user, Vars* vars)
{
	user->quest_list.add_menu(_menu);
	return Action::SUCCESS;
}

/**     
 * \brief  菜单
 *
 * 重载了done函数,设置用户访问的npc的任务菜单
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */     	
int MainMenuAction::done (SceneUser* user, Vars* vars)
{
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::stVisitNpcTradeUserCmd *cmd=(Cmd::stVisitNpcTradeUserCmd *)buf;
	bzero(buf, sizeof(buf));
	constructInPlace(cmd);

	strcpy(cmd->menuTxt, _menu.c_str());	
	cmd->byReturn = 1;
	user->sendCmdToMe(cmd, sizeof(Cmd::stVisitNpcTradeUserCmd) + strlen(cmd->menuTxt));

	return Action::SUCCESS;
}


/**     
 * \brief  刷新变量
 *
 * 重载了done函数,刷新任务变量值给该用户
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */    
int RefreshAction::done (SceneUser* user, Vars* vars)
{
	if (_id) {
		Vars* v = user->quest_list.vars(_id);
		if (v) {
			v->notify(*user, _name);
		}
	}else {
		vars->notify(*user, _name);
	}

	return Action::SUCCESS;
}

/**     
 * \brief  增加经验
 *
 * 重载了done函数,增加用户的经验
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */    
int ExpAction::done (SceneUser* user, Vars* vars)
{
	Vars* vs = vars;
	if (_id) vs = user->quest_list.vars(_id);

	if (!vs) {
		user->addExp(_exp);
		return Action::SUCCESS;
	}

	int ratio;
	if (!vs->get_value(_name, ratio)) {
		user->addExp(_exp);
		return Action::SUCCESS;
	}
	
	//add exp bonus	
	user->addExp(int(_exp*(ratio/100.0)));	
	return Action::SUCCESS;
}

int EnterSeptGuardAction::done(SceneUser* user, Vars* vars)
{
	user->enterSeptGuard();
	return Action::SUCCESS;
}

int FinishSeptGuardAction::done(SceneUser* user, Vars* vars)
{
	user->finishSeptGuard();
	return Action::SUCCESS;
}

int Exp1Action::done (SceneUser* user, Vars* vars)
{
	Vars* vs = vars;
	if (_id) vs = user->quest_list.vars(_id);

	if (!vs) {
		return Action::SUCCESS;
	}

	int exp;
	if (!vs->get_value(_name, exp)) {
		return Action::SUCCESS;
	}
	
	//add exp bonus	
	user->addExp(exp);	
	return Action::SUCCESS;
}

/**     
 * \brief  增加技能等级
 *
 * 重载了done函数,增加用户某一技能的等级
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int SkillAction::done (SceneUser* user, Vars* vars)
{
	user->upgradeSkill(_id, false);
	return Action::SUCCESS;
}

/**     
 * \brief  禁止动作
 *
 * 重载了done函数,禁止用户做特定的动作
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int DisableAction::done (SceneUser* user, Vars* vars)
{
	return Action::DISABLE;
}

/**     
 * \brief  跳转
 *
 * 重载了done函数,使用户跳转到特定地图位置
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int GotoAction::done (SceneUser* user, Vars* vars)
{
	
	if (_name.length()/*strcmp(user->scene->name, _name.c_str())*/)  {
		
		//Zebra::logger->debug("换地图：%s", _name.c_str());
		std::string tmpStr = "name=" + _name + " pos=" +  _pos;
		bool bret=false;
		switch(zMisc::randBetween(1,4))
		{
			case 1:
				if (_pos1.length() >3) {tmpStr = "name=" + _name + " pos=" +  _pos1;bret=true;}
				break;
			case 2:
				if (_pos2.length() >3) {tmpStr = "name=" + _name + " pos=" +  _pos2;bret=true;}
				break;
			case 3:
				if (_pos3.length() >3) {tmpStr = "name=" + _name + " pos=" +  _pos3;bret=true;}
				break;
			case 4:
				if (_pos4.length() >3) {tmpStr = "name=" + _name + " pos=" +  _pos4;bret=true;}
				break;
		}
		if(!bret)
		{
			if(_cpos.length() >4 )
			{
				if(_rlen.length( ) > 4)
					tmpStr = "name=" + _name + " cpos=" + _cpos + " rlen=" + _rlen;
				else
					tmpStr = "name=" + _name + " cpos=" + _cpos;
			}
		}
		// */
		Gm::gomap(user, tmpStr.c_str());
	}
	//Zebra::logger->debug("换坐标：%s", _pos.c_str());
	else
	{
		std::string tmpStr = _pos;

		switch(zMisc::randBetween(1,4))
		{
			case 1:
				if (_pos1.length() >3) tmpStr = _pos1;
				break;
			case 2:
				if (_pos2.length() >3) tmpStr = _pos2;
				break;
			case 3:
				if (_pos3.length() >3) tmpStr = _pos3;
				break;
			case 4:
				if (_pos4.length() >3) tmpStr = _pos4;
				break;
		}
		Gm::goTo(user, tmpStr.c_str());	
	}
	
	return Action::SUCCESS;
}

/**     
 * \brief  增加物品
 *
 * 重载了done函数,在用户包裹中增加特定物品
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int AddItemAction::done (SceneUser* user, Vars* vars)
{
	if (!_odds || zMisc::selectByTenTh(_odds)) {
		user->addObjectNum(_id, _value);
	}
	return Action::SUCCESS;
}

/**     
 * \brief  增加绑定物品
 *
 * 重载了done函数,在用户包裹中增加特定绑定物品
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int AddBindItemAction::done (SceneUser* user, Vars* vars)
{
	if (!_odds || zMisc::selectByTenTh(_odds)) {
		user->addObjectNum(_id, _value, 0, 0, true);
	}
	return Action::SUCCESS;
}

/**     
 * \brief  增加绿色绑定物品
 *
 * 重载了done函数,在用户包裹中增加特定绿色绑定物品
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int AddGreenBindItemAction::done (SceneUser* user, Vars* vars)
{
	if (!_odds || zMisc::selectByTenTh(_odds)) {
		user->addGreenObjectNum(_id, _value, 0, 0, true);
	}
	return Action::SUCCESS;
}

/**     
 * \brief  删除物品
 *
 * 重载了done函数,在用户包裹中删除特定物品
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int RemoveItemAction::done (SceneUser* user, Vars* vars)
{
	user->reduceObjectNum(_id, _value);
	return Action::SUCCESS;
}

/**     
 * \brief  丢弃物品
 *
 * 重载了done函数,从用户包裹中丢弃特定物品
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int DropItemAction::done (SceneUser* user, Vars* vars)
{
	zObject* ob = user->packs.uom.getObjectByID(_id, _level, true);
	if (ob) user->packs.moveObjectToScene(ob, user->getPos());
	
	return Action::SUCCESS;
}

/**     
 * \brief 未实现
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int DropAction::done (SceneUser* user, Vars* vars)
{
/*
	zObject* ob = user->packs.main.getObjectByID(_id, _level, true);
	if (ob) user->packs.moveObjectToScene(ob, user->getPos());
*/	
	return Action::SUCCESS;
}

/**     
 * \brief  下马
 *
 * 重载了done函数,如果用户在马上则使用户下马
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int RideDownAction::done (SceneUser* user, Vars* vars)
{
//	Cmd::stRideMapScreenUserCmd ret;
//	ret.dwUserTempID = user->tempid;
//	ret.bySwitch = 0;
//	clear_state(user->getByState(), Cmd::USTATE_RIDE);
//	user->dwHorseID = 0;
//	user->sendMeToNine();
	user->horse.mount(false);
	
//	user->scene->sendCmdToNine(user->getPosI(), &ret, sizeof(ret));
	
	return Action::SUCCESS;
}

/**     
 * \brief  定时器
 *
 * 重载了done函数,增加用户完成该任务的时间限制
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int TimeoutsAction::done (SceneUser* user, Vars* vars)
{

	if (_id) {
		Vars* v = user->quest_list.vars(_id);
		if (v) {
			v->set_timer();
		}
	}else {
		vars->set_timer();	
	}

	return Action::SUCCESS;
}

/**     
 * \brief  设定状态
 *
 * 重载了done函数,设定用户的状态
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int SetStateAction::done (SceneUser* user, Vars* vars)
{
	user->setUState(_state);
	return Action::SUCCESS;
}

/**     
 * \brief  清除状态
 *
 * 重载了done函数,清除用户的状态
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int ClearStateAction::done (SceneUser* user, Vars* vars)
{
	user->clearUState(_state);
	return Action::SUCCESS;
}

/**     
 * \brief  马匹
 *
 * 重载了done函数,给予用户马匹
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int HorseAction::done (SceneUser* user, Vars* vars)
{
	user->horse.horse(_id);
	return Action::SUCCESS;
}


/**     
 * \brief  添加NPC
 *
 * 重载了done函数,在地图上增加一个NPC
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int AddNpcAction::done (SceneUser* user, Vars* vars)
{
	zNpcB *base = npcbm.get(_id);
	if (!base) return  Action::FAILED;

	t_NpcDefine define;
	define.id = base->id;
	strcpy(define.name, base->name);
	define.pos = _ltpos;
	define.width = _rbpos.x - _ltpos.x;
	define.height = _rbpos.y - _ltpos.y;
	define.num = _num;
	define.interval = 30;
	define.initstate = zSceneEntry::SceneEntry_Normal;

	if (!_s) {
		return  Action::FAILED;
	}

	_s->zPosRevaluate(define.pos);
	_s->initRegion(define.region, define.pos, define.width, define.height);

	_s->summonNpc(define, define.pos, base);

	//Zebra::logger->debug("增加npc成功");	
	
	return Action::SUCCESS;
}

/**     
 * \brief  删除NPC
 *
 * 重载了done函数,在地图上指定范围删除一个NPC
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int RemoveNpcAction::done (SceneUser* user, Vars* vars)
{	
	SceneNpcManager::getMe().removeNpc_if(_remove);
	
	return Action::SUCCESS;
}

/**     
 * \brief  添加镖车
 *
 * 重载了done函数,在地图上添加一辆镖车
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int AddGuardAction::done(SceneUser* user, Vars* vars)
{
	DWORD i=_id;

	DWORD o = zMisc::randBetween(1, 100);
	if (_id3 && (o>(_odds1+_odds2)*100/(_odds1+_odds2+_odds3)))
		i=_id3;
	else if (_id2 && (o>_odds1*100/(_odds1+_odds2+_odds3)))
	{
		i=_id2;
		Channel::sendSys(user, Cmd::INFO_TYPE_GAME, "恭喜！你接到了一辆无敌镖车！");
	}

	zNpcB *base = npcbm.get(i);
	if (!base) return  Action::FAILED;

	if (user->guard) {
		Channel::sendSys(user, Cmd::INFO_TYPE_FAIL, "你还在护镖中!");
		return  Action::FAILED;
	}

	t_NpcDefine define;
	define.id = base->id;
	//snprintf(define.name, MAX_NAMESIZE, "%s的%s", user->name, base->name );
	strcpy(define.name, base->name);
	define.pos = _ltpos;
	define.width = _rbpos.x - _ltpos.x;
	define.height = _rbpos.y - _ltpos.y;
	define.num = _num;
	define.interval = 30;
	define.initstate = zSceneEntry::SceneEntry_Normal;
	//define.scriptID = _script;
	//define.setPath(_path.c_str());
	user->scene->initRegion(define.region, define.pos, define.width, define.height);

	//if (!_s) {
	//	return  Action::FAILED;
	//}

	user->scene->zPosRevaluate(define.pos);

	GuardNpc* npc = user->scene->summonOneNpc<GuardNpc>(define, define.pos, base);
	if (!npc)
	{
		Zebra::logger->error("召唤 %s 失败", define.name);
		return Action::FAILED;
	}
	npc->setMaster(user);
	npc->setPetType(Cmd::PET_TYPE_GUARDNPC);
	user->guard = npc;
	//t_NpcAIDefine	ai;
	//ai.type = NPC_AI_PATROL;
	//npc->setAI(ai);
	//npc->sendData();
	/*
	GuardNpc* npc = user->scene->summonPet(user, Cmd::PET_TYPE_GUARDNPC, define, define.pos, base);
	if (!npc) {
		Zebra::logger->error("召唤 %s 失败", define.name);
		return Action::FAILED;
	}
	npc->setMaster(user);
	user->pets.push_back(npc);

	//set owner
	*/
	npc->gold(_gold);
	if (user->venterSeptGuard.size())//家族运镖
	{
		DWORD m=0;
		for (unsigned int i=0; i<user->venterSeptGuard.size(); i++)
			m += user->venterSeptGuard[i].money;
		npc->gold(m);
		npc->isSeptGuard = true;
		Zebra::logger->debug("%s(%u) 增加家族镖车 gold=%u", user->name, user->id, m);
	}
	npc->owner(user);
	npc->dest(_dest);
	npc->map(_map);
	npc->exp(_exp);
	SceneNpcManager::getMe().addSpecialNpc(npc);

	return Action::SUCCESS;
}

/**     
 * \brief  设置五行类型
 *
 * 重载了done函数,设置用户五行类型
 *      
 * \param user: 触发动作的用户
 * \param vars: 用户所带的该任务相关变量
 * \return SUCCESS表示成功，FAILED表示失败，DISABLE表示禁用某项功能
 */
int FiveTypeAction::done (SceneUser* user, Vars* vars)
{
	user->charbase.fivetype = _type;
	Cmd::stMainUserDataUserCmd  userinfo;
	user->full_t_MainUserData(userinfo.data);
	user->sendCmdToMe(&userinfo,sizeof(userinfo));
	
	return Action::SUCCESS;
}

int UseSkillAction::done (SceneUser* user, Vars* vars)
{
	user->sendSkill(_id, _level);
	
	return Action::SUCCESS;
}

