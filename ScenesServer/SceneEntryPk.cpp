/**
 * \file
 * \version  $Id: SceneEntryPk.cpp  $
 * \author  
 * \date 
 * \brief PK相关变量和虚方法
 */

#include "zSceneEntry.h"
#include "SceneNpc.h"
#include "Zebra.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TeamManager.h"
#include "TimeTick.h"
#include "SessionClient.h"

#ifdef _DEBUGLOG
#include "Chat.h"
#endif


/**
 * \brief 角色PK定义类,有待扩充
 */
bool SceneEntryPk::AttackMe(SceneEntryPk *pAtt, const Cmd::stAttackMagicUserCmd *rev, bool physics, SWORD rangDamageBonus)
{
	//设置下次自动恢复hp的所需要的时间
	pAtt->lastPkTime = 10;

	if (rev->wdMagicType == SKILLNORMAL) this->skillValue.brappenddam = 0; //非弓箭攻击猎手印记所提供的而外伤害清0

	if (pAtt->getType() == zSceneEntry::SceneEntry_Player &&
		this->getType() == zSceneEntry::SceneEntry_Player)
	{
		pAtt->processMaskOnAttack(this);
		this->processMaskOnDefence();
	}


	if (angelMode)	return true;
	if (pAtt->afreetMode)
	{
		toDie(pAtt->tempid);
		return true;
	}

	//计算技能伤害
	//完全挡住攻击
	if(this->ignoreDam)
	{
		return true;
	}

	if(this->ignorePhysicDam&&physics)
	{
		return true;
	}
	else if (this->ignoreMagicDam&&(!physics))
	{
		return true;
	}

	int dwDam;

	if (physics)
	{
		dwDam = (int)pAtt->pkValue.pdamage	- (this->pdeftozero?0:this->pkValue.pdefence);
#ifdef _DEBUGLOG
		Zebra::logger->debug("攻击者[%s]---防御者[%s]----------------------------------------------------------", pAtt->name, this->name);
		Zebra::logger->debug("物理攻击数值  =%d", (int)pAtt->pkValue.pdamage);
		Zebra::logger->debug("物理防御数值  =%d", (this->pdeftozero?0:this->pkValue.pdefence));
		Zebra::logger->debug("物理伤害值为dwDam=%d", dwDam);
#endif
	}
	else
	{
		dwDam = (int)pAtt->pkValue.mdamage	- (this->mdeftozero?0:this->pkValue.mdefence);
#ifdef _DEBUGLOG
		Zebra::logger->debug("攻击者[%s]---防御者[%s]----------------------------------------------------------", pAtt->name, this->name);
		Zebra::logger->debug("法术攻击数值  =%d", (int)pAtt->pkValue.mdamage);
		Zebra::logger->debug("法术防御数值  =%d", (this->mdeftozero?0:this->pkValue.mdefence));
		Zebra::logger->debug("法术伤害值为%d", dwDam);
#endif
	}
	// dwDamSelf 表示反弹的伤害  dwDamDef 表示
	int dwDamSelf = 0 , dwDamDef = 0, dwReduce = dwDam>=0?0:dwDam;
	if (dwDam<0) dwDam = 0;

		// 技能增加伤害值
	// 对防御者的伤害 = 伤害值
	dwDamDef += this->skillValue.dvalue;
#ifdef _DEBUGLOG
	Zebra::logger->debug("根据技能增加伤害值计算出来的结果累加值dwDamDef:%ld", dwDamDef);
#endif
	dwDamDef += (int)(dwDam * ((this->skillValue.dvaluep>100?this->skillValue.dvaluep-100:0)/100.0f));
#ifdef _DEBUGLOG
	Zebra::logger->debug("根据技能增加伤害率计算出来的结果累加值dwDamDef:%ld", dwDamDef);
#endif

	pAtt->processAddDam(dwDam,dwDamDef,physics);
	this->reduceDam(dwDam, dwDamDef,physics);

	dwDamDef +=dwDam+dwReduce; // 
#ifdef _DEBUGLOG
	Zebra::logger->debug("中间值dwDamDef:%d", dwDamDef);
#endif

	if(dwDamDef <0)
	{
		dwDamDef = 0;
	}

	//最后处理技能的直接伤害
	WORD temp = pAtt->getDamageBonus();
	if (temp>0)
	{
		dwDamDef = (int)(dwDamDef*((100-temp)/100.0f));
	}

	if (pAtt->pkValue.damagebonus>0)
	{
		dwDamDef = (int)(dwDamDef*((100-pAtt->pkValue.damagebonus)/100.0f));
	}

	if (rangDamageBonus>0)
	{
		dwDamDef = (int)(dwDamDef*((100-rangDamageBonus)/100.0f));
	}

	//加上攻击者的被动伤害
	dwDamDef +=pAtt->skillValue.passdam;

	/*
	//战马增加伤害
	if (pAtt->getType()==zSceneEntry::SceneEntry_Player && ((SceneUser *)pAtt)->horse.mount() && ((SceneUser *)pAtt)->horse.data.skills[0])
	{
		for (int i=0; i<4; i++)
			if (((SceneUser *)pAtt)->horse.data.skills[i]>>16==Cmd::HORSE_SKILL_DMG_UP)
			{
				dwDamDef += ((SceneUser *)pAtt)->horse.data.skills[i]&0x0000ffff;
				break;
			}
	}

	//战马减少伤害
	if (getType()==zSceneEntry::SceneEntry_Player && ((SceneUser *)this)->horse.mount() && ((SceneUser *)this)->horse.data.skills[0])
	{
		for (int i=0; i<4; i++)
			if (((SceneUser *)this)->horse.data.skills[i]>>16==Cmd::HORSE_SKILL_DMG_DOWN)
			{
				if (dwDamDef>(int)((SceneUser *)this)->horse.data.skills[i]&0x0000ffff)
					dwDamDef -= ((SceneUser *)this)->horse.data.skills[i]&0x0000ffff;
				else
					dwDamDef = 0;
				break;
			}
	}
	*/

	if (dwDamDef >0)
	{
		//计算伤害随机转移----------------------------
		if (skillValue.tsfdamp != 0)
		{
			DamagCallback callback(this);
			scene->execAllOfScreen(posi,callback);
			SWORD temp=0;
			if ((temp=callback.action((SDWORD)(this->skillValue.tsfdam + dwDamDef*(this->skillValue.tsfdamp/100.0f))))!=0)
			{
				dwDamDef -= temp;
			}
#ifdef _DEBUGLOG
			Zebra::logger->debug("根据随机转移伤害率扣除后计算结果值dwDamDef:%d", dwDamDef);
#endif
		}
		//--------------------------------------------
		this->reflectDam(dwDamDef, dwDamSelf,physics);
		this->hp2mp(dwDamDef);
		if (this->dmpbyhp >0)
		{
			if (this->getType()==zSceneEntry::SceneEntry_Player)
			{
				SDWORD dmp= (SDWORD)(dwDamDef*(this->dmpbyhp/100.0f));
				this->changeMP(0-dmp);
				pAtt->changeMP(dmp);
			}
		}
		if (!physics&&(this->mdmtomp >0)&&(zMisc::selectByPercent(this->mdmtompper)))
		{
			if (this->getType()==zSceneEntry::SceneEntry_Player)
			{
				SDWORD dmp= (SDWORD)(dwDamDef*(this->mdmtomp/100.0f));
				this->changeMP(dmp);
			}
		}
	}

//	if ((getType() == zSceneEntry::SceneEntry_Player) && (pAtt->getType() == zSceneEntry::SceneEntry_Player))
//	{
//		dwDam=dwDam/2; // 伟大的策划要求的,人打人的时候伤害减半
//	}

	//抓马的固定伤害
	if ((zSceneEntry::SceneEntry_NPC==getType()) && (NPC_TYPE_WILDHORSE==((SceneNpc *)this)->npc->kind))
	{
		if (zSceneEntry::SceneEntry_Player==pAtt->getType())
		{
			zObject *o;
			SceneUser * u = (SceneUser *)pAtt;
			if(u->packs.equip.getObjectByZone(&o , 0 , Cmd::EQUIPCELLTYPE_HANDR))
			{
				if (ItemType_CaptureWeapon==o->base->kind)
					dwDamSelf = o->data.pdamage;
				else
					dwDamSelf = 0;
			}
		}
	}

	if (zSceneEntry::SceneEntry_NPC == this->getTopMaster()->getType())
	{
		if (zSceneEntry::SceneEntry_Player == pAtt->getTopMaster()->getType())
		{
			int attLevel = (int)pAtt->getTopMaster()->getLevel();
			int defLevel = (int)this->getLevel();
			int num = defLevel - attLevel -15;
			if (num >5) num=5;
			if (num >0)
			{
				dwDamDef = (DWORD)(dwDamDef*(1-num/5.0f));
			}
		}
	}

	if (zSceneEntry::SceneEntry_NPC == this->getType())
	{
		dwDamDef -=((SceneNpc *)this)->dwReduceDam;
		if (dwDamDef<0) dwDamDef=0;
	}

	if (this->skillValue.teamappend)
	{
		if (pAtt->getType() == zSceneEntry::SceneEntry_Player)
		{
			SceneUser *pUser = (SceneUser *)pAtt;
			if (pUser->team.getLeader() == (DWORD)this->skillValue.teamappend)
			{
				dwDamDef+=30;
			}
		}
	}

	if (dwDamDef > 0)
	{
		pAtt->leech(dwDamDef);
	}
#ifdef _DEBUGLOG
	Zebra::logger->debug("最终值dwDamDef:%ld", dwDamDef);
#endif	

	if (pAtt->getType() == zSceneEntry::SceneEntry_Player && this->getType() == zSceneEntry::SceneEntry_Player)
	{
		dwDamDef += zMisc::randBetween(0,pAtt->getLevel()/2);
	}

	if (pAtt->blazeflag&&pAtt->skillValue.blazeappend>0) // 提高火焰系法术伤害
	{
		dwDamDef = (int)(dwDamDef*(1+pAtt->skillValue.blazeappend/1000.0f));
	}
	else if (pAtt->blazeflag&&pAtt->skillValue.pblazeappend>0) // 提高火焰系法术伤害
	{
		dwDamDef = (int)(dwDamDef*(1+pAtt->skillValue.pblazeappend/1000.0f));
	}
	else if (pAtt->levinflag&&pAtt->skillValue.levinappend>0) // 提高雷电系法术伤害
	{
		dwDamDef = (int)(dwDamDef*(1+pAtt->skillValue.levinappend/1000.0f));
	}
	else if (pAtt->levinflag&&pAtt->skillValue.plevinappend>0) // 提高雷电系法术伤害
	{
		dwDamDef = (int)(dwDamDef*(1+pAtt->skillValue.plevinappend/1000.0f));
	}
	else if (pAtt->trapflag&&pAtt->skillValue.trapappend>0) // 提高陷阱系法术伤害
	{
		dwDamDef = (int)(dwDamDef*(1+pAtt->skillValue.trapappend/1000.0f));
	}
	else if (pAtt->iceflag&&pAtt->skillValue.iceappend>0) // 提高陷阱系法术伤害
	{
		dwDamDef = (int)(dwDamDef*(1+pAtt->skillValue.iceappend/1000.0f));
	}

	if (this->mhpd)
	{
		if (physics)
		{
			dwDamDef = dwDamDef*2;
		}
		else
		{
			dwDamDef = dwDamDef/2;
		}
	}
	if (dwDamDef ==0)
	{
		dwDamDef = zMisc::randBetween(0,5); 
	}

	if (stone&&(dwDamDef>1)) dwDamDef=1;
	this->reflectSkill(pAtt, rev);


	dwDamDef += this->skillValue.appenddam + this->skillValue.brappenddam;

#ifdef _DEBUGLOG
		Zebra::logger->debug("技能加额外伤害appenddam:%d 猎手印记额外伤害%d", this->skillValue.appenddam, this->skillValue.brappenddam);
#endif

	SWORD wdHP=0;
	SceneUser *pUser= NULL;
	if (this->swapdamcharid>0 || this->swapdamcharid2>0)
	{
		pUser = SceneUserManager::getMe().getUserByTempID(this->swapdamcharid);
		if (pUser&&(pUser->scene==this->scene)&&(pUser->getState()!=zSceneEntry::SceneEntry_Death))
		{
			wdHP = pUser->directDamage(pAtt,(int)(dwDamDef*1.2f));
			ScenePk::attackRTCmdToNine(rev , pAtt , pUser , wdHP , 0);
			pUser->attackRTHpAndMp();
		}
		else
		{
			if (this->swapdamcharid>0) this->skillStatusM.clearRecoveryElement(222);
			pUser = SceneUserManager::getMe().getUserByTempID(this->swapdamcharid2);
			if (pUser&&(pUser->scene==this->scene)&&(pUser->getState()!=zSceneEntry::SceneEntry_Death))
			{
				wdHP = pUser->directDamage(pAtt,(int)(dwDamDef));
				ScenePk::attackRTCmdToNine(rev , pAtt , pUser , wdHP , 0);
				pUser->attackRTHpAndMp();
			}
			else
			{
				if (this->swapdamcharid2>0) this->skillStatusM.clearRecoveryElement(270);
			}
		}
	}

	if (!pUser)
	{
		if (this->liquidState)
		{
			this->skillStatusM.clearRecoveryElement(248);  ///清除液态状态。
		}
		else if (this->resist==0 || !zMisc::selectByPercent(this->resist))
		{
			if (this->icedam >0)
			{
				this->icedam-=(SDWORD)dwDamDef;
				if (this->icedam<=0)
				{
					if (this->icedam<0)
					{
						wdHP = this->directDamage(pAtt,0-this->icedam);
						ScenePk::attackRTCmdToNine(rev , pAtt , this , wdHP , 0);
						this->attackRTHpAndMp();
					}
					this->skillStatusM.clearSkill(182);
				}
			}
			else
			{
				wdHP = this->directDamage(pAtt,dwDamDef);
				ScenePk::attackRTCmdToNine(rev , pAtt , this , wdHP , 0);
				this->attackRTHpAndMp();
			}
		}
	}

	if(dwDamSelf > 0)
	{
		SceneEntryPk * aMaster = pAtt->getTopMaster();
		if (aMaster->getType()==zSceneEntry::SceneEntry_Player)
		{
			wdHP = pAtt->directDamage(pAtt, dwDamSelf);
			ScenePk::attackRTCmdToNine(rev , this , pAtt , wdHP , 0);
			pAtt->attackRTHpAndMp();
			pAtt->processDeath(pAtt);
		}
		else
		{
			wdHP = pAtt->directDamage(this, dwDamSelf);
			ScenePk::attackRTCmdToNine(rev , this , pAtt , wdHP , 0);
			pAtt->attackRTHpAndMp();
			pAtt->processDeath(this);
		}
	}
	 
	SceneEntryPk * aMaster = pAtt->getTopMaster();
	SceneEntryPk * pMaster = getTopMaster();
	SDWORD pkadditionbackup = 0;
	if (aMaster->getType()==zSceneEntry::SceneEntry_Player)
	{
		SceneUser * a = (SceneUser *)aMaster;
		if (pMaster->getType()==zSceneEntry::SceneEntry_Player)
		{
			SceneUser * p = (SceneUser *)pMaster;
			pkadditionbackup = a->charbase.goodness&0X0000FFFF;
			ScenePk::checkProtect(a, p);
		}
		/*
		else if (pMaster->getType()==zSceneEntry::SceneEntry_NPC)//攻击卫兵
		{
			SceneNpc * p = (SceneNpc *)pMaster;
			if ((p->aif&AIF_ATK_REDNAME)||(p->npc->kind==NPC_TYPE_GUARD))
			{
				a->reSendMyMapData();
				Channel::sendSys(a, Cmd::INFO_TYPE_GAME, "你攻击了 %s ,两分钟内所有玩家可以对你正当攻击", p->name);
			}
		}
		*/
	}

	/*
	//骑马击落
	if (getType()==zSceneEntry::SceneEntry_Player)
	{
		if (((SceneUser *)this)->horse.mount())
			if ((DWORD)zMisc::randBetween(1, 100)<((SceneUser *)this)->horse.data.hitDown)
				((SceneUser *)this)->horse.mount(false);
	}
	*/

	/*
	if ((pAtt->getMaster() && (pAtt->getMaster()->getType() == zSceneEntry::SceneEntry_Player)) && 
		(this->getMaster() && (this->getMaster()->getType() == zSceneEntry::SceneEntry_Player)))
	{
		if (pAtt->getMaster()!=this->getMaster()) ScenePk::checkProtect(pAtt->getMaster() , this->getMaster());
	}
	if (pAtt->getMaster() && (pAtt->getMaster()->getType() == zSceneEntry::SceneEntry_Player))
	{
		SceneUser * a = (SceneUser *)pAtt->getMaster();
		if (getType()==zSceneEntry::SceneEntry_NPC)
		{
			SceneNpc * s = (SceneNpc *)this;
			if ((s->aif&AIF_ATK_REDNAME)||(s->npc->kind==NPC_TYPE_GUARD))
			{
				if (a->pkState.addProtect(a))
				{
					a->reSendMyMapData();
					//mark
					//Cmd::stAddUserMapScreenUserCmd send;
					//a->full_t_MapUserData(send.data);
					//a->scene->sendCmdToNine(a->getPosI(),&send,sizeof(send) , false);
					
					Channel::sendSys(a, Cmd::INFO_TYPE_GAME, "你攻击了 %s ,两分钟内所有玩家可以对你正当攻击", name);
				}
			}
		}
	}
	*/

	if (this->hideme)
	{
		this->skillStatusM.clearRecoveryElement(241);  ///清除隐身状态。
	}
	if (this->icebox)
	{
		this->skillStatusM.clearRecoveryElement(245);  ///冰灵柜状态。
	}

	if ((pAtt->getState() != zSceneEntry::SceneEntry_Death)&&(pAtt->dietodam >0))
	{
		wdHP = this->directDamage(pAtt,pAtt->dietodam);
		ScenePk::attackRTCmdToNine(rev , pAtt , this , wdHP , 0);
		this->attackRTHpAndMp();
		pAtt->toDie(0);
	}

	if ((this->getState() != zSceneEntry::SceneEntry_Death)&&(pAtt->killnow >0))
	{
		if (zMisc::selectByPercent(pAtt->killnow))
		{
			this->toDie(pAtt->tempid);
			pAtt->skillStatusM.clearSkill(234);
		}
	}

	if (pAtt->damtohpper >0)
	{
		if (zMisc::selectByPercent(pAtt->damtohpper))
		{
			DWORD value = (DWORD)(dwDamDef *(pAtt->damtohpvalper/100.0f));
			pAtt->changeHP(value);
		}
	}

	SceneEntryPk *myMaster =getMaster();

	if (!this->processDeath(pAtt))
	{
/*		pAtt->attackTarget = this;
		///还活着，设置对手
		*
		if (0==curTargetID)
		{
			//Zebra::logger->debug("%s 被 %s 攻击，进入战斗", name, pAtt->name);
			curTargetID = pAtt->tempid;
			curTargetType = pAtt->getType();
		}
		*
		pAtt->setCurTarget(this);
		SceneEntryPk * dt = getDefTarget();
		if (!dt || dt->getState()!=zSceneEntry::SceneEntry_Normal
				|| scene->getDistance(getPos(), pAtt->getPos())<scene->getDistance(getPos(), dt->getPos()))
		{
			if (!(pAtt->getType()==zSceneEntry::SceneEntry_NPC &&(((SceneNpc *)pAtt)->npc->kind == NPC_TYPE_SURFACE)))
			{
				setDefTarget(pAtt);
				if (getType()==zSceneEntry::SceneEntry_NPC)
					setCurTarget(pAtt);
			}
		}
		setEndBattleTime(SceneTimeTick::currentTime, 10*1000);
		*/
	}
	else
	{
		SceneEntryPk *attMaster = pAtt->getMaster();
		if (attMaster&&myMaster&&(attMaster->getType()==zSceneEntry::SceneEntry_Player)&&(myMaster->getType()==zSceneEntry::SceneEntry_Player))
		{
			SceneUser *pAttUser = (SceneUser *)attMaster;
			SceneUser *pDefUser = (SceneUser *)myMaster;

			if ((pkadditionbackup < (SDWORD)(pAttUser->charbase.goodness&0X0000FFFF)) ||
			   ((pAttUser->charbase.country != pDefUser->charbase.country)&&(!pAttUser->isWar(pDefUser))))
			{
				Cmd::Session::t_addRelationEnemy send;
				send.dwUserID = pDefUser->id;
				strncpy(send.name, pAttUser->name, MAX_NAMESIZE);
				sessionClient->sendCmd(&send, sizeof(send));
			}

			DWORD sceneCountry = attMaster->scene->getCountryID();
			if (pAttUser->charbase.country != pDefUser->charbase.country)
			{
				//char buf[MAX_CHATINFO];

				if (pAttUser->charbase.country == sceneCountry)
				{
					if (!pAttUser->isWarRecord(Cmd::COUNTRY_FORMAL_DARE, pDefUser->charbase.country) && 
						!pAttUser->isSpecWar(Cmd::COUNTRY_FORMAL_DARE))
					{
						if (this->getType() == zSceneEntry::SceneEntry_Player)
						{
							Cmd::stKillForeignerUserCmd send;
							strncpy(send.heroName,pAttUser->name,MAX_NAMESIZE);
							strncpy(send.mapName,pAttUser->scene->getName(),MAX_NAMESIZE);
							pAttUser->forwardSession(&send, sizeof(send));
						}
					}
				}
				if (pDefUser->charbase.country == sceneCountry)
				{
					if (!pAttUser->isWarRecord(Cmd::COUNTRY_FORMAL_DARE, pDefUser->charbase.country) &&
						!pDefUser->isSpecWar(Cmd::COUNTRY_FORMAL_DARE))
					{
						if (this->getType() == zSceneEntry::SceneEntry_Player)
						{
							Cmd::stRequestCountryHelpUserCmd send;
							strncpy(send.mapName,pAttUser->scene->getName(),sizeof(send.mapName));
							send.x = pAttUser->getPos().x;
							send.y = pAttUser->getPos().y;
							pDefUser->forwardSession(&send, sizeof(send));
						}
					}
				}
			}
		}
		else if (attMaster&&(attMaster->getType()==zSceneEntry::SceneEntry_Player)&&(this->getType()==zSceneEntry::SceneEntry_NPC))
		{
			SceneNpc *pNpc = (SceneNpc *)this;
			if (pNpc->npc->kind == NPC_TYPE_BBOSS)
			{
				SceneUser *pAttUser = (SceneUser *)attMaster;
				if (pNpc->dwNpcLockedUser == pAttUser->id)
				{
					Cmd::stKillBossUserCmd send;
					strncpy(send.mapName,this->scene->getName(),sizeof(send.mapName));
					send.x = this->getPos().x;
					send.y = this->getPos().y;
					send.npcid = pNpc->npc->id;
					strncpy(send.name,pAttUser->name,sizeof(send.name));
					pAttUser->forwardSession(&send, sizeof(send));
				}
			}
		}

		if (this->getType() == zSceneEntry::SceneEntry_NPC)
		{
			SceneNpc *pNpc = (SceneNpc *)this;
			if (pNpc->getPetType() == Cmd::PET_TYPE_SUMMON)
			{
				if (myMaster&&zMisc::selectByPercent(this->skillValue.summonrelive)) myMaster->summonPet(this->id, Cmd::PET_TYPE_SUMMON);
			}
		}
	}

	return true;
}

/**
 * \brief 检查魔法飞行路线是否有阻挡
 *
 *
 * \param pTarget 攻击目标
 * \param 攻击类型
 * \return 是否可顺利攻击
 */
bool SceneEntryPk::checkMagicFlyRoute(zSceneEntry *pTarget, BYTE aType)
{
	switch(aType)
	{
		case 0: // 近身
			{
				if(abs(this->pos.x - pTarget->getPos().x) > 1 || abs(this->pos.y - pTarget->getPos().y) > 1)
				{
#ifdef	_DEBUGLOG 
					Zebra::logger->debug("超出攻击范围(%s(%ld) x间距=%u y间距=%u)" , this->name , this->id ,abs(this->pos.x - pTarget->getPos().x),abs(this->pos.y - pTarget->getPos().y));
					Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"穿越方式：近身");
#endif
					if(abs(this->pos.x - pTarget->getOldPos1().x) > 1 || abs(this->pos.y - pTarget->getOldPos1().y) > 1)
					{
#ifdef	_DEBUGLOG 
					Zebra::logger->debug("超出攻击范围(%s(%ld) oldx间距=%u oldy间距=%u)" , this->name , this->id ,abs(this->pos.x - pTarget->getOldPos1().x),abs(this->pos.y - pTarget->getOldPos1().y));
					Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"穿越方式：近身");
#endif
						if(abs(this->pos.x - pTarget->getOldPos2().x) > 1 || abs(this->pos.y - pTarget->getOldPos2().y) > 1)
						{
#ifdef	_DEBUGLOG 
					Zebra::logger->debug("超出攻击范围(%s(%ld) oldx间距=%u oldy间距=%u)" , this->name , this->id ,abs(this->pos.x - pTarget->getOldPos2().x),abs(this->pos.y - pTarget->getOldPos2().y));
					Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"穿越方式：近身");
#endif
							return false;
						}
					}
				}
			}
			break;
		case 1: // 飞行
			{
#ifdef	_DEBUGLOG 
					Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"穿越方式：飞行");
#endif

					zPos pos1 = this->pos;
					zPos pos2 = pTarget->getPos();

					if (pos1 == pos2) return true;

					float ftA=0.0f;
					float ftB=0.0f;
					if (pos1.x == pos2.x)
					{
						// TILE_MAGIC_BLOCK 阻挡点类型
						// 
						if (pos1.y> pos2.y)
						{
#ifdef _DEBUGLOG
							Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"方式1：（x=%u,y=%u)（x=%u,y=%u)",pos1.x,pos1.y,pos2.x,pos2.y);
#endif
							zPos pos;
							pos.x = pos1.x;
							for(DWORD i = pos2.y+1; i<pos1.y;i++)
							{
								pos.y = i;
#ifdef _DEBUGLOG
								Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"检查的坐标点（x=%u,y=%u) %s",pos.x,pos.y,this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)?"有阻挡":"无阻挡");
#endif
								if (this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)) return false;
							}
						}
						else
						{
#ifdef _DEBUGLOG
							Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"方式2：（x=%u,y=%u)（x=%u,y=%u)",pos1.x,pos1.y,pos2.x,pos2.y);
#endif
							zPos pos;
							pos.x = pos1.x;
							for(DWORD i = pos1.y+1; i<pos2.y;i++)
							{
								pos.y = i;
#ifdef _DEBUGLOG
								Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"检查的坐标点（x=%u,y=%u) %s",pos.x,pos.y,this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)?"有阻挡":"无阻挡");
#endif
								if (this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)) return false;
							}
						}
						
					}
					else if (pos1.y == pos2.y)
					{
						if (pos1.x> pos2.x)
						{
#ifdef _DEBUGLOG
							Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"方式3：（x=%u,y=%u)（x=%u,y=%u)",pos1.x,pos1.y,pos2.x,pos2.y);
#endif
							zPos pos;
							pos.y = pos1.y;
							for(DWORD i = pos2.x+1; i<pos1.x;i++)
							{
								pos.x = i;
#ifdef _DEBUGLOG
								Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"检查的坐标点（x=%u,y=%u) %s",pos.x,pos.y,this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)?"有阻挡":"无阻挡");
#endif
								if (this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)) return false;
							}
						}
						else
						{
#ifdef _DEBUGLOG
							Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"方式4：（x=%u,y=%u)（x=%u,y=%u)",pos1.x,pos1.y,pos2.x,pos2.y);
#endif
							zPos pos;
							pos.y = pos1.y;
							for(DWORD i = pos1.x+1; i<pos2.x;i++)
							{
								pos.x = i;
#ifdef _DEBUGLOG
								Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"检查的坐标点（x=%u,y=%u) %s",pos.x,pos.y,this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)?"有阻挡":"无阻挡");
#endif
								if (this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)) return false;
							}
						}
					}
					else
					{
						std::set<zPos> posSet;
						typedef std::set<zPos>::value_type ValueType;
						std::set<zPos>::iterator vIterator;
						if (pos1.x> pos2.x)
						{
#ifdef _DEBUGLOG
							Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"方式5：（x=%u,y=%u)（x=%u,y=%u)",pos1.x,pos1.y,pos2.x,pos2.y);
#endif 
							ftA =(float)((float)pos1.y-(float)pos2.y)/(float)((float)pos1.x-(float)pos2.x);
							ftB = (float)pos1.y - (float)ftA*(float)pos1.x;

							zPos pos;
							for(DWORD i = pos2.x+1; i<pos1.x;i++)
							{
								pos.x = i;
								pos.y = (DWORD)(ftA*pos.x +ftB);
								posSet.insert(ValueType(pos));
								//if (this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)) return false;
							}
						}
						else
						{
#ifdef _DEBUGLOG
							Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"方式6：（x=%u,y=%u)（x=%u,y=%u)",pos1.x,pos1.y,pos2.x,pos2.y);
#endif 
							ftA =(float)((float)pos1.y-(float)pos2.y)/(float)((float)pos1.x-(float)pos2.x);
							ftB = (float)pos1.y - (float)(ftA*(float)pos1.x);

							zPos pos;
							for(DWORD i = pos1.x+1; i<pos2.x;i++)
							{
								pos.x = i;
								pos.y = (DWORD)(ftA*pos.x +ftB);
								posSet.insert(ValueType(pos));
								//if (this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)) return false;
							}
						}
						if (pos1.y> pos2.y)
						{
#ifdef _DEBUGLOG
							Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"方式7：（x=%u,y=%u)（x=%u,y=%u)",pos1.x,pos1.y,pos2.x,pos2.y);
#endif 
							ftA =(float)((float)pos1.y-(float)pos2.y)/(float)((float)pos1.x-(float)pos2.x);
							ftB = (float)pos1.y - (float)ftA*(float)pos1.x;

							zPos pos;
							for(DWORD i = pos2.y+1; i<pos1.y;i++)
							{
								pos.y = i;
								pos.x = (DWORD)(((float)pos.y - (float)ftB)/(float)ftA);
								posSet.insert(ValueType(pos));
								//if (this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)) return false;
							}
						}
						else
						{
#ifdef _DEBUGLOG
							Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"方式8：（x=%u,y=%u)（x=%u,y=%u)",pos1.x,pos1.y,pos2.x,pos2.y);
#endif 
							ftA =(float)((float)pos1.y-(float)pos2.y)/(float)((float)pos1.x-(float)pos2.x);
							ftB = (float)pos1.y - (float)(ftA*(float)pos1.x);

							zPos pos;
							for(DWORD i = pos1.y+1; i<pos2.y;i++)
							{
								pos.y = i;
								pos.x = (DWORD)(((float)pos.y - (float)ftB)/(float)ftA);
								posSet.insert(ValueType(pos));
								//if (this->scene->checkBlock(pos, TILE_MAGIC_BLOCK)) return false;
							}
						}

						for(vIterator = posSet.begin(); vIterator != posSet.end(); vIterator++)
						{
#ifdef _DEBUGLOG
								Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"检查的坐标点（x=%u,y=%u) %s",vIterator->x,vIterator->y,this->scene->checkBlock(*vIterator, TILE_MAGIC_BLOCK)?"有阻挡":"无阻挡");
#endif
							if (this->scene->checkBlock(*vIterator, TILE_MAGIC_BLOCK)) return false;
						}
					}
			}
			break;
		case 2: // 直达
			{
#ifdef	_DEBUGLOG 
					Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"穿越方式：直达");
#endif
			}
			break;
		case 3: //无条件直达
			{
				return true;
			}
		default:
			{
				Zebra::logger->debug("(%s , %ld)缺少攻击类型" , this->name , this->tempid);
#ifdef	_DEBUGLOG 
					Channel::sendSys(tempid,Cmd::INFO_TYPE_GAME,"穿越方式：无效类型");
#endif
				return false;
			}
			break;
	}
	//超出一屏
	if(!this->scene->zPosShortRange(this->getPos() , pTarget->getPos() , 11, 11))//SCREEN_WIDTH , SCREEN_HEIGHT))
	{
		return false;
	}
	return true;
}

/**
 * \brief 检查npc或玩家是否红名
 *
 * \return 是否红名
bool SceneEntryPk::isRedNamed()
{
	switch (getType())
	{
		case zSceneEntry::SceneEntry_Player:
			{
				return ((SceneUser *)this)->isRedNamed();
			}
		case zSceneEntry::SceneEntry_NPC:
			{
				SceneEntryPk * m = ((SceneNpc *)this)->getMaster();
				if (m)
					return m->isRedNamed();
				else
					return false;
			}
		default:
			return false;
	}
	return false;
}
 */

/**
 * \brief 得到当前的hp
 *
 * \return 当前的hp
 */
DWORD SceneEntryPk::getHp()
{
	switch (getType())
	{
		case zSceneEntry::SceneEntry_Player:
			return ((SceneUser *)this)->charbase.hp;
			break;          
		case zSceneEntry::SceneEntry_NPC:
			return ((SceneNpc *)this)->hp;
			break;
		default:
			return 0;
	}               
}

/**
 * \brief 得到最大hp
 *
 * \return 最大的hp
 */
DWORD SceneEntryPk::getMaxHp()
{
	switch (getType())
	{
		case zSceneEntry::SceneEntry_Player:
			return ((SceneUser *)this)->charstate.maxhp;
			break;          
		case zSceneEntry::SceneEntry_NPC:
			return ((SceneNpc *)this)->getMaxHP();
			break;
		default:
			return 0;
	}               
}

/**
 * \brief 检查是否在战斗状态
 *
 * \return 是否在战斗状态
 */
bool SceneEntryPk::isFighting()
{
	return curTargetID!=0;
	return !checkEndBattleTime(SceneTimeTick::currentTime);
	switch (getType())
	{
		case zSceneEntry::SceneEntry_Player:
			return !((SceneUser *)this)->checkEndBattleTime(SceneTimeTick::currentTime);
			break;          
		case zSceneEntry::SceneEntry_NPC:
			return getCurTarget()!=0;
			break;
		default:
			return 0;
	}      
}

/**
 * \brief 设置脱离战斗的时间
 *
 * \param ct 开始计时的时间
 * \param delay 结束的时间延迟
 * \return 
 */
void SceneEntryPk::setEndBattleTime(const zRTime &ct, int delay)
{
	endBattleTime = ct;
	endBattleTime.addDelay(delay);
}

/**
 * \brief 检查是否已到脱离战斗的时间
 *
 *
 * \param ct 用于比较的时间
 * \return 是否到时间
 */
bool SceneEntryPk::checkEndBattleTime(const zRTime &ct)
{
	return ct>=endBattleTime;
}

/**
 * \brief 脱离战斗
 *
 */
void SceneEntryPk::leaveBattle()
{
	for (std::list<ScenePet*>::iterator it=totems.begin(); it!=totems.end(); it++)
	{
		if (getCurTarget()==(*it)->getCurTarget())
			(*it)->leaveBattle();
	}

	if (pet && pet->getCurTarget()==getCurTarget())
		pet->leaveBattle();

	if (summon && summon->getCurTarget()==getCurTarget())
		summon->leaveBattle();

	//if (getCurTarget()
	//		&& getCurTarget()->getDefTarget()==this)
	//	getCurTarget()->clearDefTarget();

	curTargetID = 0;
	curTargetType = 0;
	clearDefTarget();
	setEndBattleTime(SceneTimeTick::currentTime, 0);
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("%s 脱离战斗", name);
#endif
}

/**
 * \brief 处理死亡
 *
 */
bool SceneEntryPk::processDeath(SceneEntryPk *pAtt)
{
	leaveBattle();
	return true;
}

/**
 * \brief 查找正在战斗的对手
 *
 * \return 对手指针
 */
SceneEntryPk * SceneEntryPk::getCurTarget()
{       
	switch (curTargetType)
	{
		case zSceneEntry::SceneEntry_Player:
			return scene->getUserByTempID(curTargetID);
		case zSceneEntry::SceneEntry_NPC:
			return SceneNpcManager::getMe().getNpcByTempID(curTargetID);
		default:
			return 0;
	}      
}

/**
 * \brief 查找正在攻击自己的对手
 *
 * \return 对手指针
 */
SceneEntryPk * SceneEntryPk::getDefTarget()
{       
	switch (defTargetType)
	{
		case zSceneEntry::SceneEntry_Player:
			return scene->getUserByTempID(defTargetID);
		case zSceneEntry::SceneEntry_NPC:
			return SceneNpcManager::getMe().getNpcByTempID(defTargetID);
		default:
			return 0;
	}      
}

/**
 * \brief 设置当前的目标
 *
 *
 * \param tempid 对手的tempid
 * \param type 对手的类型
 * \param force 强制设置目标
 * \return 
 */
bool SceneEntryPk::setCurTarget(DWORD tempid, DWORD type, bool force)
{
	if (tempid==this->tempid) return false;//不设置自己

	SceneEntryPk * def = NULL;
	if (zSceneEntry::SceneEntry_Player==type)
		def = scene->getUserByTempID(tempid);
	else if (zSceneEntry::SceneEntry_NPC==type)
		def = SceneNpcManager::getMe().getNpcByTempID(tempid);
	if (def)
		if (!isEnemy(def)) return false;

	curTargetID = tempid;
	curTargetType = type;
	setEndBattleTime(SceneTimeTick::currentTime, 10*1000);
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("%s 设置攻击对象 %u", name, tempid);
#endif
	return true;
}


/**
 * \brief 设置当前的目标
 *
 *
 * \param target 对手的指针
 * \param force 强制设置目标
 * \return 
 */
bool SceneEntryPk::setCurTarget(SceneEntryPk * target, bool force)
{
	if (target==this) return false;//不设置自己
	if (!isEnemy(target)) return false;//不设置朋友

	curTargetID = target->tempid;
	curTargetType = target->getType();
	setEndBattleTime(SceneTimeTick::currentTime, 10*1000);
	//Zebra::logger->debug("%s 设置攻击对象 %s", name, target->name);
	return true;
}

/**
 * \brief 设置当前的防御目标
 *
 *
 * \param tempid 对手的tempid
 * \param type 对手的类型
 * \param force 强制设置目标
 * \return 
 */
bool SceneEntryPk::setDefTarget(DWORD tempid, DWORD type, bool force)
{
	if (tempid==this->tempid) return false;//不设置自己

	SceneEntryPk * def = NULL;
	if (zSceneEntry::SceneEntry_Player==type)
		def = scene->getUserByTempID(tempid);
	else if (zSceneEntry::SceneEntry_NPC==type)
		def = SceneNpcManager::getMe().getNpcByTempID(tempid);
	if (def)
		if (!isEnemy(def)) return false;

	defTargetID = tempid;
	defTargetType = type;
	setEndBattleTime(SceneTimeTick::currentTime, 10*1000);
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("%s 设置防御对象 %u", name, tempid);
#endif
	return true;
}

/**
 * \brief 设置当前的防御目标
 *
 *
 * \param target 对手的指针
 * \param force 强制设置目标
 * \return 
 */
bool SceneEntryPk::setDefTarget(SceneEntryPk * target, bool force)
{
	if (target==this) return false;//不设置自己
	if (!isEnemy(target)) return false;//不设置朋友

	defTargetID = target->tempid;
	defTargetType = target->getType();
	setEndBattleTime(SceneTimeTick::currentTime, 10*1000);
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("%s 设置防御对象 %s", name, target->name);
#endif
	return true;
}

/**
 * \brief 取消当前的防御目标
 */
void SceneEntryPk::clearDefTarget()
{
	defTargetID = 0;
	defTargetType = 0;
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("%s 清除防御目标", name);
#endif
}

/**
 * \brief 给宠物升级
 * 换掉原来的宠物
 *
 * \param up 要升级的宠物
 */
void SceneEntryPk::petLevelUp(ScenePet * up)
{
	//FunctionTimes times(29,__FUNCTION__);
	if (Cmd::PET_TYPE_GUARDNPC==up->getPetType()
			|| Cmd::PET_TYPE_RIDE==up->getPetType()) return;

	//宠物最高等级=主人技能等级+2
	int maxlv = 2;
	zSkill *s = usm.findSkill(up->npc->soulrate);
	if (s) maxlv = s->actionbase->level+2;

	while ((up->petData.exp>=up->petData.maxexp)&&(up->petData.lv<maxlv))
	{
		zNpcB *base = npcbm.get(up->id+1);
		if (!base)
		{
			return;
		}

		up->petData.exp -= up->petData.maxexp;
		up->petData.maxexp = base->exp;
		up->petData.lv++;
		up->npc = base;
		up->id++;
		up->petData.id++;
		up->hp = up->npc->hp;
	}

	up->sendMeToNine();
}

/**
 * \brief 向9屏发送宠物信息
 *
 */
void SceneEntryPk::sendPetDataToNine()
{
	for (std::list<ScenePet *>::iterator it=totems.begin(); it!=totems.end(); it++)
		(*it)->sendPetDataToNine();
	if (pet) pet->sendPetDataToNine();
	if (summon) summon->sendPetDataToNine();
}

/**
 * \brief 造成直接伤害
 * \param pAtt 攻击者
 * \param dam 伤害
 * \param notify 是否通知客户端
 * \author fqnewman
 * \return 伤害值
 */
SWORD SceneEntryPk::directDamage(SceneEntryPk *pAtt, const SDWORD &dam, bool notify)
{
		pAtt->attackTarget = this;

		pAtt->setCurTarget(this);
		SceneEntryPk * dt = getDefTarget();
		if (!dt || dt->getState()!=zSceneEntry::SceneEntry_Normal
				|| scene->getDistance(getPos(), pAtt->getPos())<scene->getDistance(getPos(), dt->getPos()))
		{
			if (!(pAtt->getType()==zSceneEntry::SceneEntry_NPC &&(((SceneNpc *)pAtt)->npc->kind == NPC_TYPE_SURFACE)))
			{
				setDefTarget(pAtt);
				if (getType()==zSceneEntry::SceneEntry_NPC)
					setCurTarget(pAtt);
			}
		}
		setEndBattleTime(SceneTimeTick::currentTime, 10*1000);
		return 0;
}
void SceneEntryPk::sendCmdToSelected(const void *pstrCmd, const unsigned int nCmdLen)
{
	//selected_lock.lock();
	SelectedSet_iterator iter = selected.begin();
	for(; iter != selected.end() ;)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(*iter);
		if(pUser)
		{
			if(this->scene->checkTwoPosIInNine(this->getPosI() , pUser->getPosI()))
			{
				pUser->sendCmdToMe(pstrCmd,nCmdLen);
				iter ++ ;
				continue;
			}
		}
		SelectedSet_iterator iter_del = iter;
		iter_del ++;
		selected.erase(iter);
		iter = iter_del;
	}
	//selected_lock.unlock();
}
