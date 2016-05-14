/**
 * \file
 * \version  $Id: SceneSkill.cpp $
 * \author  
 * \date 
 * \brief 实现技能命令的处理
 *
 */
#include "SceneUser.h"
#include "Scene.h"
#include "SceneSkill.h"
#include "ScenePk.h"
#include "zObject.h"
#include "Chat.h"

using namespace Cmd;


struct TeamSkillExec : public TeamMemExec
{
	SceneUser *leader;
	zSkill *skill;
	TeamSkillExec(SceneUser *user , zSkill *s)
	{
		leader = user;
		skill = s;
	}
	bool exec(TeamMember &member)
	{
		SceneUser *u = leader->scene->getUserByTempID(member.tempid);
		if(u)
		{
			/*
			std::vector<DWORD>::const_iterator iter = skill->base->benignID.gid.begin();
			std::vector<zSkillB::Odds>::const_iterator iter_1 = skill->base->benignrating.gr.begin();
			for(;iter != skill->base->benignID.gid.end(),iter_1 != skill->base->benignrating.gr.end();iter++ , iter_1++)
			{
				if(zMisc::selectByPercent(iter_1->percent))
				{
					u->skillState.plus.setCharState(u , *iter , false);
					u->skillState.plus.good[*iter].value = iter_1->value;
					u->skillState.plus.good[*iter].leavetime = skill->base->ptime + 1;
					u->skillState.plus.setCharState(u , *iter , true);
				}
			}
			Cmd::stRefreshStateMapScreenUserCmd ret;
			ret.dwUserTempID = u->tempid;
			bcopy(u->getByState(),ret.state,sizeof(ret.state));
			u->scene->sendCmdToNine(u->getPosI(),&ret,sizeof(ret) , false);
			// */
			return true;
		}
		return false;
	}
};
/**
  * \brief 添加玩家技能
  *
  * \param rev 添加技能命令
  *
  * \return 添加成功返回TRUE,否则返回FALSE
  *
  */
bool SceneUser::addSkillData(const Cmd::stAddUserSkillPropertyUserCmd *rev)
{
	zSkill::create(this , rev->dwSkillID , rev->wdLevel) ;
	charbase.skillpoint --;

	//刷新用户数据
	Cmd::stMainUserDataUserCmd ret;
	full_t_MainUserData(ret.data);
	sendCmdToMe(&ret , sizeof(ret));
	return true;
}
/**
  * \brief 删除玩家技能
  *
  * \param rev 删除技能命令
  *
  * \return 删除成功返回TRUE,否则返回FALSE
  *
  */
bool SceneUser::removeSkill(const Cmd::stRemoveUserSkillPropertyUserCmd *rev)
{
	zSkill *skill = usm.findSkill(rev->dwSkillID);
	if(!skill)
	{
		return false;
	}
	usm.removeSkill(skill);

	//刷新用户数据
	Cmd::stMainUserDataUserCmd  userinfo;
	full_t_MainUserData(userinfo.data);
	sendCmdToMe(&userinfo,sizeof(userinfo));
	return true;
}
/**
  * \brief 更新玩家技能
  *
  * \param dwSkillID 技能ID
  * \param needSkillPoint 所需技能点
  *
  * \return 更新成功返回TRUE,否则返回FALSE
  *
  */
bool SceneUser::upgradeSkill(DWORD dwSkillID ,bool needSkillPoint)
{
// 他们说阵法书又要技能点了，不是我不明白是策划变太快
//	switch(dwSkillID)
//	{
//		case 554:
//		case 568:
//			{
//				needSkillPoint = false;
//			}
//			break;
//		default:
//			break;
//	}
	if(charbase.skillpoint < 1 && needSkillPoint)
	{
		return false;
	}

	zSkill *skill = usm.findSkill(dwSkillID);
	if(!skill)
	{
		skill = zSkill::create(this , dwSkillID , 1);
		if(!skill)
		{
			return false;
		}
		if(needSkillPoint)
			charbase.skillpoint --;

		Cmd::stAddUserSkillPropertyUserCmd ret;
		ret.dwSkillID = skill->data.skillid;
		ret.wdLevel = skill->data.level;
		ret.wdUpNum = this->skillUpLevel(skill->base->skillid, skill->base->kind);
		if (ret.wdUpNum+ret.wdLevel>10) ret.wdUpNum = 10 - ret.wdLevel;
		ret.dwExperience = 0;
		ret.dwMaxExperience = 0;
		sendCmdToMe(&ret , sizeof(ret));

		this->setupCharBase();
		//刷新用户数据
		Cmd::stMainUserDataUserCmd ret_1;
		full_t_MainUserData(ret_1.data);
		sendCmdToMe(&ret_1 , sizeof(ret_1));
		return true;
	}
/*
	if(!skill->canUpgrade(this))
	{
		return false;
	}
*/

	if(skill->data.level >= MAX_SKILLLEVEL)
	{
		return false;
	}

	if (!skill->checkSkillStudy(true))
	{
		return false;
	}

	if (!skill->checkSkillBook(true))
	{
		return false;
	}

	zSkillB *tempbm = skillbm.get(skill_hash(skill->base->skillid , skill->base->level+1));
	if (tempbm)
	{
		if (this->usm.getPointInTree(skill->base->kind, skill->base->subkind) < tempbm->needpoint)
		{
			return false;
		}
	}
	else
	{
		return false;
	}
#ifdef _DEBUGLOG
	Zebra::logger->error("技能升级检查前提技能点数 当前点[%u] 需要点[%u] 当前BASE技能等级[%u] dwSkillID=[%u]", this->usm.getPointInTree(skill->base->kind, skill->base->subkind), skill->base->needpoint, skill->base->level, dwSkillID);
#endif

	if ((int)charbase.skillpoint - 1 >=0)
	{
		charbase.skillpoint -=1;
	}
	else
	{
		return false;
	}

	skill->data.level ++;
	skill->setupSkillBase(this);
	zRTime ctv;
	skill->lastUseTime = ctv.msecs() - skill->base->dtime;
	this->setupCharBase();

	Cmd::stAddUserSkillPropertyUserCmd ret;
	ret.dwSkillID = skill->data.skillid;
	ret.wdLevel = skill->data.level;
	ret.wdUpNum = this->skillUpLevel(skill->base->skillid, skill->base->kind);
	if (ret.wdUpNum+ret.wdLevel>10) ret.wdUpNum = 10 - ret.wdLevel;
	ret.dwExperience = 0;
	ret.dwMaxExperience = 0;
	sendCmdToMe(&ret , sizeof(ret));
	
	//刷新用户数据
	Cmd::stMainUserDataUserCmd ret_1;
	full_t_MainUserData(ret_1.data);
	sendCmdToMe(&ret_1 , sizeof(ret_1));
	return true;
}

int getCharType(DWORD type)
{
	int ret = 0;
	if(type != 0)
	{
		for( ; type > 0; type = type >> 1 ,ret ++);
		ret --;
	}
	else
	{
		return -1;
	}
	return ret;
}


