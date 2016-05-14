/**
 * \file
 * \version  $Id: SceneNpcAI.cpp  $
 * \author  
 * \date 
 * \brief AI���ֵ�ʵ��
 * ����AI��������SceneNpc��AI��صķ���
 *
 * 
 */

#include "SceneNpcAI.h"
#include "SceneNpc.h"
#include "Scene.h"
#include "TimeTick.h"
#include "Chat.h"
#include "SessionClient.h"
#include "SceneManager.h"

/**
 * \brief ����NPC��AI
 *
 * \param ai Ҫ���õ�AI�ṹ������
 */
void SceneNpc::setAI(const t_NpcAIDefine &ai)
{
	AIDefine = ai;
}

/*
   const t_NpcAIDefine & SceneNpc::getAI()
   {
   return AIDefine;
   }
   */

/**
 * \brief ��ͨNpc����
 * \return ����ִ���Ƿ�ɹ�
 */
bool SceneNpc::normalAction()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
#ifdef _XWL_DEBUG
	//Channel::sendNine(this, "AIDefine.type=%u", AIDefine.type);
#endif
	//TODOֻ���ڷ����仯ʱ�ŷ���
	if(hp!=lasthp)
	{
		//��Ѫ���¼����Ѫʱ��
		if (hp<lasthp)
			setRecoverTime(SceneTimeTick::currentTime, 3000);

		sendtoSelectedHpAndMp();
		lasthp = hp;
		if (Cmd::PET_TYPE_NOTPET!=getPetType())
			((ScenePet *)this)->sendHpExp();
	}

	recover();

	if (_half_sec(SceneTimeTick::currentTime))
	{
		if (dreadProcess()) return true;
	}

	switch (AIDefine.type)
	{
		case NPC_AI_NORMAL:
			{
				int ret = doNormalAI();
				return ret;
			}
			break;
		case NPC_AI_SAY:
			{
				return doSayAI();
			}
			break;
		case NPC_AI_MOVETO:
			{
				return doMovetoAI();
			}
			break;
		case NPC_AI_ATTACK:
			{
				return doAttackAI();
			}
			break;
		case NPC_AI_GO_ON_PATH:
			{
				return doGoOnPathAI();
			}
			break;
		case NPC_AI_PATROL:
			{
				return doPatrolAI();
			}
			break;
		case NPC_AI_FLEE:
			{
				return doFleeAI();
			}
			break;
		case NPC_AI_RETURN_TO_REGION:
			{
				return doReturnToRegionAI();
			}
			break;
		case NPC_AI_CHANGE_MAP:
			{
				return doChangeMapAI();
			}
			break;
		case NPC_AI_WARP:
			{
				return doWarpAI();
			}
			break;
		case NPC_AI_CLEAR:
			{
				return doClearAI();
			}
			break;
		case NPC_AI_DROP_ITEM:
			{
				return doDropItemAI();
			}
			break;
		case NPC_AI_RANDOM_CHAT:
			{
				return doRandomChatAI();
			}
			break;
		case NPC_AI_WAIT:
			{
				return true;
			}
			break;
		default:
			{
				return false;
			}
			break;
	}
#ifdef _XWL_DEBUG
	//t = SceneTimeTick::currentTime - t;
	//Zebra::logger->debug("���� %s ��������ʱ %u", t);
#endif
}

/**
 * \brief �л���ͼ��AI����
 * \return ִ��AI�Ƿ�ɹ�
 */
bool SceneNpc::doChangeMapAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	std::string string = AIDefine.str;
	std::string mapName(string);
	if (std::string::npos==string.find("��", 0))
	{
		mapName = SceneManager::getInstance().getCountryNameByCountryID(scene->getCountryID());
		mapName += "��" + string;
	}

	Scene * s = SceneManager::getInstance().getSceneByName(mapName.c_str());
	if (!s)
	{
		Zebra::logger->info("doChangeMapAI:npc��ת��ͼʧ��,δ�ҵ���ͼ %s", mapName.c_str());
		return false;
	}

	if (s!=scene)
		changeMap(s, AIDefine.pos);
	else
		warp(AIDefine.pos);

	AIC->setActRegion(getPos());
	t_NpcAIDefine ad;
	ad.type = NPC_AI_WAIT;
	AIC->setAI(ad, false);
	return true;
}

/**
 * \brief ˲���ƶ���AI����
 * \return ִ��AI�Ƿ�ɹ�
 */
bool SceneNpc::doWarpAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	warp(AIDefine.pos);
	AIC->setActRegion(getPos());
	t_NpcAIDefine ad;
	ad.type = NPC_AI_WAIT;
	AIC->setAI(ad, false);
	return true;
}

/**
 * \brief ��ͨAI����
 * \return ִ��AI�Ƿ�ɹ�
 */
bool SceneNpc::doNormalAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	//if ((Cmd::PET_TYPE_NOTPET!=getPetType())&&(0==getMaster())) return true;

	if (dwStandTimeCount>0)
	{
		if (dwStandTime <SceneTimeTick::currentTime.sec())
		{
			SceneUser * pMaster = (SceneUser *)this->getMaster();
			if (pMaster && pMaster->killOnePet((ScenePet *)this))
			{
				setState(SceneEntry_Death);
				setClearState();
			}
#ifdef _DEBUGLOG
			Zebra::logger->debug("��ʱNPC����timeCount=%u ����ʱ��[%u]��ǰʱ��[%u]", dwStandTimeCount, dwStandTime, SceneTimeTick::currentTime.sec());
#endif
			return true;
		}
#ifdef _DEBUGLOG
		Zebra::logger->debug("��ʱNPC����timeCount=%u ����ʱ��[%u]��ǰʱ��[%u]", dwStandTimeCount, dwStandTime, SceneTimeTick::currentTime.sec());
#endif
	}

	switch (npc->kind)
	{
		case NPC_TYPE_TOTEM:			/// ͼ������
		case NPC_TYPE_SURFACE:			/// �ر�����
			{
				if (canFight())
				{
					SceneEntryPk_vec enemies;

					//����뾶
					int r = npc_search_region;
					if (aif&AIF_DOUBLE_REGION) r *= 2;
					BYTE at = getAType();
					if (NPC_ATYPE_FAR==at || NPC_ATYPE_MFAR==at)
						r +=3;
					if (npc->kind == NPC_TYPE_SURFACE) r=10;

					getEntries(r, enemies, 1);

					this->setPetAI((Cmd::petAIMode)(Cmd::PETAI_MOVE_STAND|Cmd::PETAI_ATK_ACTIVE));
					SceneEntryPk * enemy = chooseEnemy(enemies);
					if (enemy)
					{
						//sendMeToNine();
						/*
						   Cmd::stAddMapNpcMapScreenUserCmd addNpc;
						   full_t_MapNpcData(addNpc.data);
						   scene->sendCmdToNine(getPosI(), &addNpc, sizeof(addNpc));
						   */
						npcSkill skill;
						npc->getRandomSkillByType(SKILL_TYPE_DAMAGE, skill);
						return useSkill(enemy,skill.id);
					}
				}
			}
			break;
		case NPC_TYPE_TRAP:				/// ��������
			{
				if (canFight())
				{
					SceneEntryPk_vec enemies;

					getEntries(2, enemies, 1);

					this->setPetAI((Cmd::petAIMode)(Cmd::PETAI_MOVE_STAND|Cmd::PETAI_ATK_ACTIVE));

					SceneEntryPk * enemy = chooseEnemy(enemies);
					if (enemy)
					{
						npcSkill skill;
						if (npc->getRandomSkillByType(SKILL_TYPE_DAMAGE, skill))
						{
							//FunctionTimes times(41,"NPC_TYPE_TRAP");
							sendMeToNine();
							/*
							   Cmd::stAddMapNpcMapScreenUserCmd addNpc;
							   full_t_MapNpcData(addNpc.data);
							   scene->sendCmdToNine(getPosI(), &addNpc, sizeof(addNpc));
							   */
							if (useSkill(enemy,skill.id))
							{
								SceneUser * pMaster = (SceneUser *)this->getMaster();
								if (pMaster && pMaster->killOnePet((ScenePet *)this))
								{
									setState(SceneEntry_Death);
									setClearState();
								}
							}
							return true;
						}
						else
						{
							Zebra::logger->error("[����AI]NPC[%s]�ļ���û�м����޷�ʹ�ã�", this->name);
						}
					}
				}
			}
			break;
		case NPC_TYPE_HUMAN:			///����
		case NPC_TYPE_TASK:			///��������
		case NPC_TYPE_NORMAL:			/// ��ͨ����
		case NPC_TYPE_BBOSS:			/// ��Boss����
		case NPC_TYPE_PBOSS:			/// ��Boss����
		case NPC_TYPE_LBOSS:			/// СBoss����
		case NPC_TYPE_BACKBONE:			/// ��Ӣ����
		case NPC_TYPE_GOLD:				/// �ƽ�����
		case NPC_TYPE_AGGRANDIZEMENT:	/// ǿ������
		case NPC_TYPE_BACKBONEBUG:		/// ��������
		case NPC_TYPE_ABERRANCE:		/// ��������
		case NPC_TYPE_PET:	/// ��������
		case NPC_TYPE_GUARD:	/// ʿ������
		case NPC_TYPE_SUMMONS:			/// �ٻ�����
		case NPC_TYPE_LIVENPC:			/// ����npc
		case NPC_TYPE_SOLDIER:	/// ʿ������
		case NPC_TYPE_CARTOONPET:	/// ��ͨ����
		case NPC_TYPE_UNIONGUARD:	/// ��ս����
		case NPC_TYPE_UNIONATTACKER:	/// ����ʿ��
			if ((checkAttackTime(SceneTimeTick::currentTime))
					||(checkMoveTime(SceneTimeTick::currentTime)))
			{
				if (npc->kind != NPC_TYPE_TOTEM && npc->kind != NPC_TYPE_TRAP)
				{
					if (moveToMaster()) return true;
				}
				if (healSelf()) return true;

				if (canFight())
				{
					SceneEntryPk * enemy = 0;
					SceneEntryPk_vec enemies;

					//��ǰ��Ŀ��
					checkChaseAttackTarget(enemy);

					//��������
					if (!enemy && isActive())
					{

						//����뾶
						int r = npc_search_region;
						if (aif&AIF_DOUBLE_REGION) r *= 2;
						BYTE at = getAType();
						if (NPC_ATYPE_FAR==at || NPC_ATYPE_MFAR==at)
							r +=3;

						getEntries(r, enemies, 1);
					}
					//���б���
					if (enemies.size())
					{
						//�������
						if (runOffEnemy(enemies)) return true;

						//�������
						if (catchme) 
						{
							SceneEntryPk_vec::iterator it;
							for(it = enemies.begin(); it!= enemies.end(); it++)
							{
								if (zMisc::selectByPercent(catchme)) (*it)->setCurTarget(this->tempid, this->getType());
							}
						}

						//ѡ�����
						enemy = chooseEnemy(enemies);
					}
					//���ҵ��ĵ��˴���
					if (enemy)
					{
						switch (zMisc::randBetween(1,10))
						{
							case 1:
								if (buffSelf()) return true;
								break;
							case 2:
								if (debuffEnemy(enemy)) return true;
								break;
							default:
								break;
						}
						return attackEnemy(enemy);
					}
					//��ͬ��Ĵ���
					if (aif&(AIF_HEAL_FELLOW_5|AIF_BUFF_FELLOW_5|AIF_HELP_FELLOW_5))
					{
						SceneEntryPk_vec fellows;
						int r = npc_search_region;
						if (aif&AIF_DOUBLE_REGION)
							r *= 2;
						if (getEntries(r, fellows, 0))
						{
							switch (zMisc::randBetween(1,20))
							{
								case 0:
								case 1:
									//Zebra::logger->debug("%u ��������ͬ��", tempid);
									if (healFellow(fellows)) return true;
									break;
								case 2:
								case 3:
									//Zebra::logger->debug("%u ����buffͬ��", tempid);
									if (buffFellow(fellows)) return true;
									break;
								case 4:
									//Zebra::logger->debug("%u ���԰���ͬ��", tempid);
									if (helpFellow(fellows)) return true;
									break;
								default:
									break;
							}
						}
					}
				}

				if (NPC_AI_PATROL==AIDefine.type)
					return doMovetoAI();
				else
					return randomMove();
			}
			break;
		default:
			break;
	}
	return true;
}

/**
 * \brief �ƶ�AI����
 * ÿ�ƶ�һ�ξ����û��Χ������NPC�������Χ
 * \return ִ���Ƿ�ɹ�
 */
bool SceneNpc::doMovetoAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	//check();
	if (!AIC->phaseTimeOver())
		if (checkMoveTime(SceneTimeTick::currentTime) && canMove())
		{
			if (!gotoFindPath(getPos(), AIDefine.pos))//��Ŀ���ƶ�
				goTo(AIDefine.pos);//��һ�ַ�ʽ��Ŀ���ƶ�
			if (AIC)
				if (NPC_AI_PATROL==AIDefine.type)
					AIC->setActRegion(getPos(), 5, 5);
				else
					AIC->setActRegion(getPos());
		}
	return true;
}

/**
 * \brief ����AI����
 * ���Ǽ���ʱ�����Ƶ���ͨ�ж�
 * \return ִ��AI�Ƿ�ɹ�
 */
bool SceneNpc::doAttackAI()
{
	return doNormalAI();
}

/**
 * \brief ����npc��ͨAI
 * ��ͨAIʱAIDefine��������������Ч
 */
void NpcAIController::setNormalAI()
{
	curAI.type = NPC_AI_NORMAL;
	setAI(curAI);
}

/**
 * \brief NPC���������¼�����
 *
 * \param pAtk ������
 */
void NpcAIController::on_hit(SceneEntryPk *pAtk)
{
	if (0>=npc->hp) return;

	npc->randomChat(NPC_CHAT_ON_HIT);

	//npc->delayMoveTime(npc->npc->distance/2);//�����������ӳ�
	switch(npc->npc->kind)
	{
		case NPC_TYPE_HUMAN:			///����
		case NPC_TYPE_NORMAL:			/// ��ͨ����
		case NPC_TYPE_BBOSS:			/// ��Boss����
		case NPC_TYPE_PBOSS:			/// ��Boss����
		case NPC_TYPE_LBOSS:			/// СBoss����
		case NPC_TYPE_BACKBONE:			/// ��Ӣ����
		case NPC_TYPE_GOLD:				/// �ƽ�����
		case NPC_TYPE_SUMMONS:			/// �ٻ�����
		case NPC_TYPE_AGGRANDIZEMENT:	/// ǿ������
		case NPC_TYPE_ABERRANCE:		/// ��������
		case NPC_TYPE_BACKBONEBUG:		/// ��������
			{
				//hp1/3����ʱ����
				if (npc->aif&AIF_FLEE_30_HP)
				{
					if ((npc->hp*3<npc->getMaxHP()) && (false==curAI.flee))
					{
						t_NpcAIDefine ad;
						ad.type = NPC_AI_FLEE;
						ad.flee = true;
						ad.fleeCount = npc_flee_distance;
						ad.fleeDir = pAtk->getPos().getDirect(npc->getPos());
						setAI(ad, false);

						//Channel::sendNine(this, "hp����1/3�ˣ�����~");
					}
				}

				//��3������Χ������
				if (npc->aif&AIF_FLEE_3_ENEMY_4)
				{
					int side = 0;
					int direct = 0;
					int clockwise = 1;
					int enemyCount = 0;
					int count = 0;//��������ֹ��ѭ��
					zPos pos;
					npc->scene->getNextPos(side, direct, npc->getPos(), clockwise, pos);
					do                      
					{                       
						SceneUser *sceneUser = npc->scene->getSceneUserByPos(pos);
						if (sceneUser && sceneUser->getState() == zSceneEntry::SceneEntry_Normal)
						{
							if (npc->isAttackMe(sceneUser))
								enemyCount++;
						}    
						if (++count>=8)
							break;
					} while(npc->scene->getNextPos(side, direct, npc->getPos(), clockwise, pos) && side <= 1);

					if (enemyCount>=3)
					{
						t_NpcAIDefine ad;
						ad.type = NPC_AI_FLEE;
						ad.flee = true;
						ad.fleeCount = npc_flee_distance;
						ad.fleeDir = pAtk->getPos().getDirect(npc->getPos());
						setAI(ad, false);
						//Channel::sendNine(this, "%d ����Χ���ң�", enemyCount);
					}
				}

				//����20%�����ƶ��ٶ����
				if ((npc->aif&AIF_SPD_UP_HP20)&&(npc->hp*5<npc->getMaxHP()))
					npc->speedUpUnder20 = true;
				else
					npc->speedUpUnder20 = false;
				npc->setSpeedRate(npc->getSpeedRate());

				//����50%���¹����ٶ����
				if ((npc->aif&AIF_ASPD_UP_HP50)&&(npc->hp*2<npc->getMaxHP()))
					npc->aspeedUpUnder50 = true;
				else
					npc->aspeedUpUnder50 = false;
				npc->resetAspeedRate();

				npc->setSecondTarget(pAtk);

				//����60%�����ٻ�
				if (!npc->define->summonList.empty() && npc->hp*10<npc->getMaxHP()*6 &&!npc->summoned)
				{
					npc->summonByNpcMap(npc->define->summonList);
					npc->summoned = true;
				}
			}
			break;
		case NPC_TYPE_GUARD:	/// ʿ������
		case NPC_TYPE_PET:	/// ��������
		case NPC_TYPE_TASK:		/// ��������
		case NPC_TYPE_TOTEM:			/// ͼ������
		case NPC_TYPE_TRADE:	/// ��������
			break;
		default:
			break;
	}
	/*
	   switch (curAI.type)
	   {
	   case NPC_AI_MOVETO:
	   {
	   npc->delayMoveTime(npc_onhit_stop_time*1000);
	   }
	   break;
	   default:
	   break;
	   }
	   */
}

/**
 * \brief �̶�·�����ߵ�AI����
 * \return ִ��AI�Ƿ�ɹ�
 */
bool SceneNpc::doGoOnPathAI()
{
	/*
	   if (!AIC) return true;
	   check();
	   if (AIC->phaseTimeOver() && moveAction )
	   warp(AIDefine.pos);

	//����Ŀ�긽��
	if (scene->zPosShortRange(getPos(), AIDefine.pos, AIDefine.regionX))
	{
	if (dstPos<define->path.size()-1) 
	dstPos++;
	else {
	on_reached();
	dstPos = 0;
	}
	AIDefine.pos = define->path[dstPos];
	AIC->delayPhaseTime(npc_one_checkpoint_time);
	}

	if (checkMoveTime(SceneTimeTick::currentTime) && moveAction)
	{
	if (!gotoFindPath(getPos(), AIDefine.pos))//��Ŀ���ƶ�
	goTo(AIDefine.pos);//��һ�ַ�ʽ��Ŀ���ƶ�
	}
	*/
	return true;
}

/**
 * \brief Ѳ��AI����
 * Ѳ�߻����Ǵ��������ƶ�
 * ֻ���������Ķ�������ڹ����˵Ĺ���
 *
 * \return ִ��AI�Ƿ�ɹ�
 */
bool SceneNpc::doPatrolAI()
{
	return doNormalAI();
	/*
	   SceneEntryPk *entry = NULL;

	   if ((checkChaseAttackTarget(entry)||((aif&AIF_ACTIVE_MODE) && findAndChaseUser(blind ?1:npc_chase_region, entry))))
	   {

	   if (inRange(entry))
	   {
	   if (canAttack(entry))
	   {
	   if (attackTarget(entry))
	   AIC->delayPhaseTime(npc->adistance);
	   else
	   {
	   if (!((aif&AIF_ACTIVE_MODE)&&chasedFindFrontUser(getPos().getDirect(entry->getPos()))))
	   if (checkMoveTime(SceneTimeTick::currentTime) && moveAction)
	   {
	   if (!gotoFindPath(getPos(), entry->getPos()))
	   goTo(entry->getPos());
	   AIC->delayPhaseTime(npc->adistance);
	   }
	   }
	   }
	   }
	   else
	   if (checkMoveTime(SceneTimeTick::currentTime) && moveAction)
	   {
	   if (!gotoFindPath(getPos(), entry->getPos()))//��Ŀ���ƶ�
	   goTo(entry->getPos());//��һ�ַ�ʽ��Ŀ���ƶ�
	   AIC->delayPhaseTime(npc->adistance);
	   }
	   }
	   else
	   {
	   if (checkMoveTime(SceneTimeTick::currentTime) && moveAction)
	   {
	   if (!gotoFindPath(getPos(), AIDefine.pos))//��Ŀ���ƶ�
	   goTo(AIDefine.pos);//��һ�ַ�ʽ��Ŀ���ƶ�
	   AIC->setActRegion(getPos());//���û��Χ

	   }
	   }
	   return true;
	   */
}

/**
 * \brief ����AI�Ĵ���
 *
 * \return ִ��AI�Ƿ�ɹ�
 */
bool SceneNpc::doFleeAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if (checkMoveTime(SceneTimeTick::currentTime) && canMove())
	{
		if (AIDefine.flee&&(AIDefine.fleeCount>0))
		{
			int clockwise = zMisc::selectByPercent(50)?-1:1;
			int tryDir = AIDefine.fleeDir + 8;
			for (int i=0; i<8; i++)
			{
				if (shiftMove(tryDir%8))
				{
					AIDefine.fleeCount--;
					if (0==AIDefine.fleeCount)
					{
						delayMoveTime(3000);
						AIC->switchAI(false);
					}
					return true;
				}
				tryDir += clockwise;
			}
			return doNormalAI();
		}
	}
	return true;
}

/**
 * \brief �ص����Χ��AI����
 *
 * \return ִ��AI�Ƿ�ɹ�
 */
bool SceneNpc::doReturnToRegionAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if (!AIC->phaseTimeOver())
		if (checkMoveTime(SceneTimeTick::currentTime) && canMove())
		{
			if (aif&AIF_WARP_MOVE)
				warp(AIDefine.pos);
			else
				if (!gotoFindPath(getPos(), AIDefine.pos))//��Ŀ���ƶ�
					goTo(AIDefine.pos);//��һ�ַ�ʽ��Ŀ���ƶ�
		}
	return true;
}

/**
 * \brief NPC���ֵ���ʱ���¼�����
 *
 * \param pFound ���ֵĵ���
 */
void NpcAIController::on_find_enemy(const SceneEntryPk *pFound)
{
	if (0>=npc->hp) return;

	int side = 0;
	if (npc->aif&AIF_CALL_FELLOW_7)
		side = 3;
	if (npc->aif&AIF_CALL_FELLOW_9)
		side = 4;

	SceneEntryPk_vec fellows;
	if (npc->getEntries(side, fellows, 0))
	{
		for (unsigned int i=0;i<fellows.size();i++)
		{
			if (fellows[i]->getType()!=zSceneEntry::SceneEntry_NPC) continue;
			//if (SceneNpc::CHASE_NONE != ((SceneNpc *)fellows[i])->getChaseMode()) continue;
			if (0 != fellows[i]->curTargetID) continue;
			//if (!zMisc::selectByPercent(50)) continue;
			if (npc->aif&AIF_CALL_BY_ATYPE)
				if (!(npc->getAType()==((SceneNpc *)fellows[i])->getAType()))
					continue;
			((SceneNpc *)fellows[i])->chaseSceneEntry(pFound->getType(), pFound->tempid);
		}
	}

	npc->randomChat(NPC_CHAT_ON_FIND_ENEMY);

	/*
	   switch(npc->npc->kind)
	   {
	   case NPC_TYPE_HUMAN:			///����
	   case NPC_TYPE_NORMAL:			/// ��ͨ����
	   case NPC_TYPE_BBOSS:			/// ��Boss����
	   case NPC_TYPE_PBOSS:			/// ��Boss����
	   case NPC_TYPE_LBOSS:			/// СBoss����
	   case NPC_TYPE_BACKBONE:			/// ��Ӣ����
	   case NPC_TYPE_GOLD:				/// �ƽ�����
	   case NPC_TYPE_SUMMONS:			/// �ٻ�����
	   case NPC_TYPE_AGGRANDIZEMENT:	/// ǿ������
	   case NPC_TYPE_ABERRANCE:		/// ��������
	   case NPC_TYPE_BACKBONEBUG:		/// ��������
	   {
	   if (zMisc::selectByPercent(npc_call_fellow_rate))
	   {
	//�ٻ�ͬ��
	int direct = zMisc::randBetween(0, 7);
	int clockwise = zMisc::selectByPercent(50)?-1:1;
	int fellowCount = 0;
	int region = 7;//�ٻ�ͬ��ķ�Χ
	int trycount = 0;//���Ҵ������ƣ���ֹ��ѭ��
	zPos pos;
	npc->scene->getNextPos(side, direct, npc->getPos(), clockwise, pos);
	do                      
	{                       
	if ((++trycount)>(region*region))
	break;
	SceneNpc * sceneNpc = npc->scene->getSceneNpcByPos(pos);
	if (sceneNpc&&(0==sceneNpc->getChaseSceneEntry()))
	{
	if (sceneNpc->canChaseTarget(pFound))
	if (sceneNpc->chaseSceneEntry(pFound->getType(), pFound->tempid))
	fellowCount++;
	}                               
	} while(npc->scene->getNextPos(side, direct, npc->getPos(), clockwise, pos) && side <= 7);
	//if (fellowCount>0)
	//Channel::sendNine(npc, "�ҽ� %d ��ͬ�������Ҵ� %s", fellowCount, pFound->name);
	}
	}
	break;
	case NPC_TYPE_GUARD:	/// ʿ������
	case NPC_TYPE_PET:	/// ��������
	case NPC_TYPE_TASK:		/// ��������
	case NPC_TYPE_TOTEM:			/// ͼ������
	case NPC_TYPE_TRADE:	/// ��������
	break;
	default:
	break;
	}
	*/
}

/**
 * \brief �ж�Npc�Ƿ��߳���������з�Χ֮��
 * ���û���ڸ����û�״̬����Ҫ��Χ��������
 * ׷��ʱ��Χ����10
 * \return �Ƿ񳬳����Χ
 */
bool NpcAIController::outOfRegion() const
{
	if (!(npc->canMove())) return false;
	const zPos &pos = npc->getPos();
	//if (npc->getChaseMode() == SceneNpc::CHASE_NONE)
	if (0==npc->curTargetID)
		return !((pos.x>=actPos.x-actRegionX)
				&&(pos.x<=actPos.x+actRegionX)
				&&(pos.y>=actPos.y-actRegionY)
				&&(pos.y<=actPos.y+actRegionY));
	else
		return !((pos.x>=actPos.x-(actRegionX+10))
				&&(pos.x<=actPos.x+(actRegionX+10))
				&&(pos.y>=actPos.y-(actRegionY+10))
				&&(pos.y<=actPos.y+(actRegionY+10)));
}

//�ص����Χ��׷�ٵ��˳�����Χʱ��5���ٷ���
/**
 * \brief ���ûص���Χ��AIʹNPC�ص����Χ
 * ���NPC�Ǹ��ٵ��˶������˻��Χ������5���ٷ��ػ��Χ�ڣ�ͬʱ��������Ŀ��
 *
 */
void NpcAIController::returnToRegion()
{
	//����
	/*	
		SceneEntryPk * master = npc->getMaster();
		if (master)
		{
		if (!npc->scene->zPosShortRange(master->getPos(), npc->getPos(), SceneNpc::npc_pet_warp_region))
		{
		npc->warp(master->getPos());
		return;
		}
		npc->setSpeedRate(2.0);
		return;
		}
		*/

	t_NpcAIDefine def = curAI;
	def.type = NPC_AI_RETURN_TO_REGION;
	def.pos = actPos;
	def.regionX = zMisc::randBetween(2, actRegionX);
	def.regionY = zMisc::randBetween(2, actRegionY);
	def.lasttime = 10;
	setAI(def);
	//if (npc->getChaseMode() != SceneNpc::CHASE_NONE)
	if (0!=npc->curTargetID)
	{
		npc->randomChat(NPC_CHAT_ON_RETURN);
		npc->unChaseUser();
		npc->setSpeedRate(npc->getSpeedRate()*4.0);
	}
	//Channel::sendNine(npc, "�ҵûص�(%d,%d)��Χ%d,%d����", curAI.pos.x, curAI.pos.y, def.regionX, def.regionY);
}

/**
 * \brief ����һ�ι���
 *
 * \return �Ƿ񴴽��ɹ�
 */
bool SceneNpc::createRush()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	Rush * rush = new Rush(define->rushID, define->rushDelay, scene->getCountryID());
	if (rush)
	{
		if (rush->init())
		{
			setMoveTime(SceneTimeTick::currentTime, (define->rushDelay+rush->lasttime+define->interval) * 1000);//���ǽ����ٹ�һ��������
			mayRush = false;

			//��������
			Cmd::Session::t_cityRush_SceneSession send;
			bzero(send.bossName, MAX_NAMESIZE);
			bzero(send.rushName, MAX_NAMESIZE);
			bzero(send.mapName, MAX_NAMESIZE);
			strncpy(send.bossName, rush->bossName, MAX_NAMESIZE-1);
			strncpy(send.rushName, rush->rushName, MAX_NAMESIZE-1);
			strncpy(send.mapName, rush->mapName, MAX_NAMESIZE-1);
			send.delay = rush->rushDelay;
			send.countryID = scene->getCountryID();
			sessionClient->sendCmd(&send, sizeof(send));
			return true;
		}
		Zebra::logger->debug("��ʼ����������ʧ��");
		SAFE_DELETE(rush);
	}
	return false;
}

/**
 * \brief ����Ƿ�ﵽ��������
 *
 * \return �Ƿ���Թ���
 */
bool SceneNpc::canRush()
{
	if (mayRush && (define->rushID) && (zSceneEntry::SceneEntry_Death==getState()))
	{
		Zebra::logger->trace("%s ���﹥�ǵļ��� %d%%", name, define->rushRate);
		if (define->rushRate >= zMisc::randBetween(0,100))
			return true;
	}
	return false;
}

const int NpcAIController::npc_call_fellow_rate = 30;///NPC�ٻ�ͬ��ļ���
const int NpcAIController::npc_one_checkpoint_time = 60;///NPC����·���ƶ�ʱ����һ��·����ʱ��
const int NpcAIController::npc_checkpoint_region = 2;///NPC�ƶ�������һ��·����ж���Χ
const int NpcAIController::npc_onhit_stop_time = 2;///����NPC�ƶ��б�����ʱ��ֹͣ��ʱ��
const int NpcAIController::npc_flee_distance = 4;///NPC���빥���ߵľ���
const int NpcAIController::npc_min_act_region = 5;///NPC���빥���ߵľ���

/*------------NpcAIController-------------------*/
/*------------NpcAIController-------------------*/
/*------------NpcAIController-------------------*/
/*------------NpcAIController-------------------*/

/**
 * \brief ����NPC�Ļ��Χ
 *
 * \param pos ����λ��
 * \param x,y ��Χ�Ŀ�͸�
 * \return 
 */
void NpcAIController::setActRegion(zPos pos, int x, int y)
{
	if (pos==zPos(0,0)) pos = npc->getPos();
	actPos = pos;
	if (0<=x) actRegionX = x>=npc_min_act_region?x:npc_min_act_region;
	if (0<=y) actRegionY = y>=npc_min_act_region?y:npc_min_act_region;
	//Channel::sendNine(npc, "���Χ:(%d,%d) x=%d y=%d", actPos.x, actPos.y, actRegionX, actRegionY);
}

/**
 * \brief �õ�NPC�Ļ��Χ
 *
 * \param pos ���������λ��
 * \param x,y �������Χ�Ŀ�͸�
 */
void NpcAIController::getActRegion(zPos &pos, int &x, int &y)
{
	pos = actPos;
	x = actRegionX;
	y = actRegionY;
}

/**
 * \brief ���캯��
 *
 * \param sn Ҫ���Ƶ�npcָ��
 */
	NpcAIController::NpcAIController(SceneNpc * sn)
:curPhase(0),repeat(-1),active(false),npc(sn),reached(false)
{
	bzero(dstMap, sizeof(dstMap));
	if (sn)
		strncpy(dstMap, sn->scene->name, MAX_NAMESIZE-1);
	dstPos = zPos(0,0);
	curAI.type = NPC_AI_NORMAL;
}

/**
 * \brief ��ȡNPC���ܽű�
 *
 * \param id �ű�ID
 * \return �Ƿ���سɹ�
 */
bool NpcAIController::loadScript(unsigned int id)
{
	if ((!npc)||(!id)) return false;

	if (!phaseVector.empty())
		unloadScript();

	zXMLParser xml;
	if (!xml.initFile(Zebra::global["mapdir"] + "NpcAIScript.xml"))
	{
		Zebra::logger->error("��NPC�ű��ļ�ʧ�� %s", (Zebra::global["mapdir"] + "NpcAIScript.xml").c_str());
		return false;
	}

	xmlNodePtr root = xml.getRootNode("info");
	if (!root) return false;
	xmlNodePtr scriptNode = xml.getChildNode(root, "script");

	while (scriptNode)
	{
		unsigned int num=0;
		xml.getNodePropNum(scriptNode, "id", &num, sizeof(num));
		if (num==id)
		{
			xml.getNodePropNum(scriptNode, "repeat", &repeat, sizeof(repeat));

			xmlNodePtr phaseNode = xml.getChildNode(scriptNode, "phase");
			char action[32];
			t_NpcAIDefine ad;
			while (phaseNode)
			{
				bzero(action, sizeof(action));
				xml.getNodePropStr(phaseNode, "action", action, sizeof(action));
				ad.type = parseAction(action);
				if ((ad.type==NPC_AI_MOVETO)||(ad.type==NPC_AI_PATROL))
					dstPos = ad.pos;
				xml.getNodePropStr(phaseNode, "str", ad.str, sizeof(ad.str));
				//Zebra::logger->debug("loadScript:%s", ad.str);
				if (ad.type==NPC_AI_CHANGE_MAP)
					strncpy(dstMap, ad.str, MAX_NAMESIZE-1);
				xml.getNodePropNum(phaseNode, "x", &ad.pos.x, sizeof(ad.pos.x));
				xml.getNodePropNum(phaseNode, "y", &ad.pos.y, sizeof(ad.pos.y));
				xml.getNodePropNum(phaseNode, "regionX", &ad.regionX, sizeof(ad.regionX));
				xml.getNodePropNum(phaseNode, "regionY", &ad.regionY, sizeof(ad.regionY));
				xml.getNodePropNum(phaseNode, "lasttime", &ad.lasttime, sizeof(ad.lasttime));

				phaseVector.push_back(ad);
				phaseNode = xml.getNextNode(phaseNode, "phase");
			}

			if (!phaseVector.empty())
			{
				if (id>500) active = true;//500�����ǹ���ű�������������npc
				nextPhase(0);
				//Zebra::logger->info("[AI]%s ��ȡNPC���ܽű� id=%d phase=%d", npc->name, id, phaseVector.size());
				return true;
			}
			else
			{
				Zebra::logger->info("��ȡ�����ܽű� id=%d", id);
				return false;
			}
		}
		scriptNode = xml.getNextNode(scriptNode, "script");
	}

	Zebra::logger->debug("δ�ҵ�AI�ű� id=%d", id);
	return false;
}

/**
 * \brief ж�ؽű�
 *
 */
void NpcAIController::unloadScript()
{
	active = false;
	phaseVector.clear();
}

/**
 * \brief ���ýű�ѭ������
 * ������3����Χ��
 * -1������ѭ��
 *  0���Ѿ�����
 *  >0���ݼ�
 *
 * \param re ����
 */
void NpcAIController::setRepeat(int re)
{
	repeat = re;
}

/**
 * \brief �õ�ѭ������
 *
 * \return ѭ������
 */
int NpcAIController::getRepeat()
{
	return repeat;
}

/**
 * \brief ����Ƿ������˽ű�
 *
 * \return �Ƿ������˽ű�
 */
bool NpcAIController::isActive()
{
	return active;
}

/**
 * \brief �����ַ����õ�������ʶ
 *
 * \param action ������ַ���
 * \return �������ı�ʶ
 */
SceneNpcAIType NpcAIController::parseAction(char * action)
{
	if (!strcmp(action, "moveto")) return NPC_AI_MOVETO;
	if (!strcmp(action, "say")) return NPC_AI_SAY;
	if (!strcmp(action, "patrol")) return NPC_AI_PATROL;
	if (!strcmp(action, "attack")) return NPC_AI_ATTACK;
	//if (!strcmp(action, "recover")) return NPC_AI_RECOVER;
	if (!strcmp(action, "changemap")) return NPC_AI_CHANGE_MAP;
	if (!strcmp(action, "warp")) return NPC_AI_WARP;
	if (!strcmp(action, "clear")) return NPC_AI_CLEAR;
	if (!strcmp(action, "wait")) return NPC_AI_WAIT;
	if (!strcmp(action, "dropitem")) return NPC_AI_DROP_ITEM;
	if (!strcmp(action, "randomchat")) return NPC_AI_RANDOM_CHAT;

	Zebra::logger->error("parseAction : δ֪�Ľű����� %s", action);
	return NPC_AI_NORMAL;
}

/**
 * \brief ���øý׶εĽ���ʱ��
 *
 * \param delay �����ڿ�ʼ���ӳ٣�����Ϊ��λ
 */
void NpcAIController::setPhaseTime(const int delay)
{
	phaseEndTime = SceneTimeTick::currentTime;
	phaseEndTime.addDelay(delay);
}

/**
 * \brief �ӳ��ý׶ν�����ʱ��
 *
 * \param delay �ӳ���ʱ�䣬��λ����
 */
void NpcAIController::delayPhaseTime(const int delay)
{
	phaseEndTime.addDelay(delay);
}

/**
 * \brief ���׶�ʱ���Ƿ����
 *
 * \return �Ƿ��˽׶ν���ʱ��
 */
bool NpcAIController::phaseTimeOver()
{
	if (NPC_AI_NORMAL!=curAI.type)
		return SceneTimeTick::currentTime >= phaseEndTime;
	else
		return false;
}

/**
 * \brief ������һ�׶�
 *
 * \param index ����ָ��Ҫ����Ľ׶α�ţ�-1��ʾ��һ�׶�
 */
void NpcAIController::nextPhase(int index = -1)
{
	/* //��������npcҲҪִ�нű�
	   if (!active)
	   {
	   setNormalAI();
	   return;
	   }
	   */

	if (index>-1)
		curPhase = index;
	else
	{
		if (curPhase==phaseVector.size()-1)
		{
			if (-2==repeat)
			{
				curPhase = zMisc::randBetween(0, phaseVector.size()-1);
			}
			else if (-1==repeat)
			{
				curPhase = 0;
			}
			else if (1==repeat)
			{
				active = false;
				phaseVector.clear();
				repeat--;
				npc->on_reached();
				setNormalAI();
				return;
			}
			else if (repeat>1)
			{
				curPhase = 0;
				repeat--;
				//Channel::sendNine(npc, "repeat=%d", repeat);
			}
		}
		else
			curPhase++;
	}

	//setPhaseTime(phaseVector[curPhase].lasttime*1000);

	if (phaseVector.size())
		setAI(phaseVector[curPhase]);
	//Channel::sendNine(npc, "����׶�%d", curPhase);
}

/**
 * \brief ����׶ν������¼�
 * ��ͬ�׶ν���ʱ����ͬ
 * �ƶ�/Ѳ��/�ص���Χ�����ʱ�����δ����Ŀ�ĵ���˲�ƹ�ȥ
 *
 */
void NpcAIController::on_phaseEnd()
{
	switch (curAI.type)
	{
		case NPC_AI_PATROL:
			{
				if (!(npc->checkMoveTime(SceneTimeTick::currentTime)&&npc->canMove())) return;

				npc->warp(curAI.pos);
				//if (!arrived(curAI.pos))
				setActRegion(npc->getPos());
			}
			break;
		case NPC_AI_MOVETO:
			{
				if (!(npc->checkMoveTime(SceneTimeTick::currentTime)&&npc->canMove())) return;

				if (npc->warp(curAI.pos))
					setActRegion(npc->getPos());
				else
					return;
			}
			break;
		case NPC_AI_RETURN_TO_REGION:
			{
				if (!(npc->checkMoveTime(SceneTimeTick::currentTime)&&npc->canMove())) return;

				if (NPC_AI_NORMAL!=oldAI.type)
					npc->warp(curAI.pos);
				setAI(oldAI);
				npc->resetSpeedRate();
			}
			break;
		default:
			break;
	}
	nextPhase();
}

/**
 * \brief �������¼������л���Ӧ״̬
 * �÷�����SceneNpc::action��ִ��
 *
 */
void NpcAIController::processPhase()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	//if (!active) return;
	if (zSceneEntry::SceneEntry_Death==npc->getState()) return;
#ifdef _XWL_DEBUG
	//Channel::sendNine(npc, "ai=%u speed=%f skillValue.movespeed=%d", curAI.type, npc->speedRate, npc->skillValue.movespeed);
#endif

	if (phaseTimeOver()&&curAI.type!=NPC_AI_NORMAL&&curAI.type!=NPC_AI_FLEE)
		on_phaseEnd();

	if (curAI.type!=NPC_AI_RETURN_TO_REGION
			&&curAI.type!=NPC_AI_MOVETO
			//&&curAI.type!=NPC_AI_PATROL
			&&curAI.type!=NPC_AI_FLEE
			&&curAI.type!=NPC_AI_WAIT
			&&(Cmd::PET_TYPE_NOTPET==npc->getPetType())//���ﲻ�жϷ�Χ
			&&((NPC_TYPE_GUARD==npc->npc->kind)||(npc->aif&AIF_ATK_REDNAME)||curAI.type==NPC_AI_PATROL||npc->aif&AIF_LIMIT_REGION)//������Ѳ�߲Żص�ԭλ
			&&outOfRegion())
		returnToRegion();

	//if (npc->getChaseMode() != SceneNpc::CHASE_NONE)
	if (0!=npc->curTargetID)
	{
		if (npc->checkEndBattleTime(SceneTimeTick::currentTime))
			if (!(npc->lockTarget || npc->aif&AIF_LOCK_TARGET))
				npc->unChaseUser();
		/*
		   SceneEntryPk * entry = npc->getChaseSceneEntry();
		   if (entry)
		   if (!npc->scene->zPosShortRange(npc->getPos(), entry->getPos(), SceneNpc::npc_lost_target_region))
		   npc->unChaseUser();
		   */
	}

	switch (curAI.type)
	{ 
		case NPC_AI_MOVETO:
			{ 
				/*
				   if (dstReached())
				   {
				   if (!reached)
				   {
				   npc->on_reached();
				   reached = true;
				   break;
				   }
				   else
				   reached = false;
				   }
				   */
				if (arrived(curAI.pos))
				{
					//Channel::sendNine(npc, "����(%d,%d)", curAI.pos.x, curAI.pos.y);
					setActRegion(curAI.pos, curAI.regionX, curAI.regionY);
					nextPhase();
				}
			}
			break;
		case NPC_AI_PATROL:
			{
				/*
				   if (dstReached())
				   {
				   if (!reached)
				   {
				   npc->on_reached();
				   reached = true;
				   break;
				   }
				   else
				   reached = false;
				   }
				   */
				if (arrived(curAI.pos))
					nextPhase();
			}
			break;
		case NPC_AI_RETURN_TO_REGION:
			{
				if (arrived())
				{
					setAI(oldAI);
					npc->resetSpeedRate();
				}
			}
			break;
		default:
			break;
	}
}

/**
 * \brief npc�����¼��Ĵ���
 *
 */
void NpcAIController::on_relive()
{
	//if (!active) return;

	if (curAI.type==NPC_AI_FLEE)
		setNormalAI();

	setActRegion(npc->getPos(), npc->define->width/2, npc->define->height/2);
	//setNormalAI();

	nextPhase(0);

}

/**
 * \brief ����npc��AI�����ý׶�ʱ��
 *
 * \param ai Ҫ���õ�AI���������û�������
 * \param setTime �Ƿ�ͬʱ����ʱ��,Ĭ��Ϊ��
 */
void NpcAIController::setAI(const t_NpcAIDefine ai, const bool setTime)
{
	oldAI = curAI;
	npc->setAI(ai);
	curAI = ai;
	if (setTime)
		setPhaseTime(ai.lasttime*1000);

	switch (curAI.type)
	{
		case NPC_AI_ATTACK:
			{
				setActRegion(curAI.pos, curAI.regionX, curAI.regionY);
			}
			break;
		case NPC_AI_FLEE:
			{
				npc->randomChat(NPC_CHAT_ON_FLEE);
			}
			break;
		default:
			break;
	}
	//Zebra::logger->debug("setAI(): %s AI=%d", npc->name, ai.type);
}


/**
 * \brief ����oldAI��curAI
 *
 *
 * \param setTime �Ƿ�ͬʱ����ʱ��,Ĭ��Ϊ��
 */
void NpcAIController::switchAI(const bool setTime)
{
	if (oldAI.type!=NPC_AI_FLEE)
		setAI(oldAI, setTime);
	else
		setNormalAI();
}

/**
 * \brief �ж�npc�Ƿ񵽴�ĳλ�õ�ĳ��Χ��
 *
 * \param pos ����λ�ã�Ĭ���ǵ�ǰAI��Ŀ��λ��
 * \param regionX ��Χ��Ĭ���ǵ�ǰAI�ķ�Χ��
 * \param regionY ��Χ�ߣ�Ĭ���ǵ�ǰAI�ķ�Χ��
 * \return �Ƿ��ڷ�Χ��
 */
bool NpcAIController::arrived(zPos pos, int regionX, int regionY)
{
	if (pos == zPos(0,0)) pos = curAI.pos;
	if (-1==regionX) regionX = curAI.regionX;
	if (-1==regionY) regionY = curAI.regionY;

	//zPos npcPos = npc->getPos();
	return (npc->getPos().x>=pos.x-regionX)   
		&&(npc->getPos().x<=pos.x+regionX)  
		&&(npc->getPos().y>=pos.y-regionY)  
		&&(npc->getPos().y<=pos.y+regionY); 
}

/**
 * \brief �Ƿ񵽴�Ŀ�ĵ�
 * Ŀ�ĵصĶ����ǽű������һ���ƶ���λ��
 * ��ͼ��ͬ���㵽��
 *
 */
bool NpcAIController::dstReached()
{
	if (strcmp(dstMap, npc->scene->name)) return false;

	if (arrived(dstPos, npc_checkpoint_region, npc_checkpoint_region))
		return true;

	return false;
}

/**
 * \brief ����npc�������¼�
 *
 */
void NpcAIController::on_die()
{
	npc->randomChat(NPC_CHAT_ON_DIE);
	npc->summoned = false;
}

/**
 * \brief ִ�����AI
 *
 * \return �Ƿ�ִ�гɹ�
 */
bool SceneNpc::doClearAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if (!clearMe)
	{
		setClearState();

		if (npc->id==ALLY_GUARDNPC)//�˹��ڳ�
		{
			Cmd::Session::t_allyNpcClear_SceneSession send;
			send.dwCountryID = scene->getCountryID();
			sessionClient->sendCmd(&send, sizeof(send));

			Zebra::logger->debug("%s �˹��ڳ� %u ����", 
							SceneManager::getInstance().getCountryNameByCountryID(scene->getCountryID()),
							tempid);
		}
	}
	//Channel::sendNine(this, "clear me~");
	return true;
}

/**
 * \brief ִ�ж�����AI
 *
 * \return �Ƿ�ִ�гɹ�
 */
bool SceneNpc::doDropItemAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	zObjectB *ob = objectbm.get(AIDefine.pos.x);
	if (!ob)
	{
		Zebra::logger->debug("%s �Ҳ���Ҫ������Ʒ id=%u num=%u", name, AIDefine.pos.x, AIDefine.pos.y);
		return false;
	}

	for (DWORD i=0; i<AIDefine.pos.y; i++)
		scene->addObject(ob, 1, getPos(), 0, 0);

	t_NpcAIDefine ad;
	ad.type = NPC_AI_WAIT;
	AIC->setAI(ad, false);
	return true;
}

/**
 * \brief ִ��˵��AI
 *
 * \return �Ƿ�ִ�гɹ�
 */
bool SceneNpc::doSayAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	Channel::sendNine(this, AIDefine.str);
	t_NpcAIDefine ad;
	ad.type = NPC_AI_WAIT;
	AIC->setAI(ad, false);
	return true;
}

/**
 * \brief ִ�����˵��AI��˵��������NpcCommonChat.xml�������9
 *
 * \return �Ƿ�ִ�гɹ�
 */
bool SceneNpc::doRandomChatAI()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	randomChat(NPC_CHAT_RANDOM);

	t_NpcAIDefine ad;
	ad.type = NPC_AI_WAIT;
	AIC->setAI(ad, false);
	return true;
}

/**
 * \brief ѡ�񹥻�Ŀ��
 *
 * \param enemies �����б�
 * \return �ҵ��ĵ��ˣ�û֪��������0
 */
SceneEntryPk * SceneNpc::chooseEnemy(SceneEntryPk_vec& enemies)
{
	//��ս��npc
	if (!canFight()) return false;

	SceneEntryPk * ret = 0;

	//�ȼ�����е�Ŀ��
	if (!ret)
		checkChaseAttackTarget(ret);

	//BOSS��10%�ĸ�������Ѱ��Ŀ��
	if ((npc->kind==NPC_TYPE_BBOSS||npc->kind==NPC_TYPE_LBOSS||npc->kind==NPC_TYPE_PBOSS) && zMisc::selectByPercent(10))
		ret = 0;

	if (!ret)
	{
		//����npc
		if (enemies.empty()) return 0;
		if (!isActive()) return 0;
		//if ((!(aif&AIF_ACTIVE_MODE))||(petAI&Cmd::PETAI_ATK_PASSIVE)) return 0;

		/*
		//ȡ�ø���npc
		SceneEntryPk_vec enemies;
		int r = npc_search_region;
		if (aif&AIF_DOUBLE_REGION)
		r *= 2;
		if (!getEntries(r, enemies, 1)) return 0;
		*/

		//û�����������򹥻������Ŀ��
		/*
		   if (!(aif&(AIF_ATK_PDEF|AIF_ATK_MDEF|AIF_ATK_HP)))
		   ret = enemies[0];
		   else
		   */
		{
			//�ж���������
			DWORD minValue = 0xffffffff;
			for (unsigned int i=0; i<enemies.size(); i++)
			{
				if (getTopMaster()->getType()==zSceneEntry::SceneEntry_Player
						&& getPetType()!=Cmd::PET_TYPE_NOTPET)
				{
					//���ﲻ��������������
					if (enemies[i]->getType()==zSceneEntry::SceneEntry_NPC)
					{
						SceneNpc * n = (SceneNpc *)enemies[i];
						if ((NPC_TYPE_GUARD==n->npc->kind)||(n->aif&AIF_ATK_REDNAME))
							continue;
					}

					//������
					SceneEntryPk *tar = enemies[i]->getCurTarget();
					if (tar && 0!=isEnemy(tar))//�Ǳ��˵Ĺ�
						continue;

					//��Χû�����ʱ�Ź�������
					if (enemies[i]->getTopMaster()->getType()==zSceneEntry::SceneEntry_NPC
							&& i<enemies.size()-1)
						continue;
				}

				unsigned int value = minValue;
				if (!(aif&(AIF_ATK_PDEF|AIF_ATK_MDEF|AIF_ATK_HP)))
				{
					int x2 = abs(getPos().x-enemies[i]->getPos().x);
					int y2 = abs(getPos().y-enemies[i]->getPos().y);
					value = x2*x2+y2*y2;
				}
				else
				{
					switch (enemies[i]->getType())
					{
						case zSceneEntry::SceneEntry_Player:
							{
								if (aif&AIF_ATK_PDEF)
									value = ((SceneUser *)enemies[i])->charstate.pdefence;
								if (aif&AIF_ATK_MDEF)
									value = ((SceneUser *)enemies[i])->charstate.mdefence;
								if (aif&AIF_ATK_HP)
									value = enemies[i]->getHp();
							}
							break;
						case zSceneEntry::SceneEntry_NPC:
							{
								if (aif&AIF_ATK_PDEF)
									value = ((SceneNpc *)enemies[i])->getMinPDefence();
								if (aif&AIF_ATK_MDEF)
									value = ((SceneNpc *)enemies[i])->getMinMDefence();
								if (aif&AIF_ATK_HP)
									value = enemies[i]->getHp();
							}
							break;
						default:
							continue;
					}
				}
				if (value<minValue)
				{
					minValue = value;
					ret = enemies[i];
				}
			}
		}
	}

	//ֱ�ӹ������������
	if (ret&&(aif&AIF_ATK_MASTER))
	{
		SceneEntryPk * tmp = ret->getMaster();
		if (tmp)
			if (scene->zPosShortRange(getPos(), tmp->getPos(), 6))
				ret = tmp;
	}

	if (ret) chaseSceneEntry(ret->getType(), ret->tempid);
	return ret;
}

/**
 * \brief ���Լ�����
 *
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::healSelf()
{
	if (hp*2>this->getMaxHP()) return false;

	npcSkill skill;
	if (npc->getRandomSkillByType(SKILL_TYPE_RECOVER, skill))
	{
		if (zMisc::selectByPercent(skill.rate))
		{
			return useSkill(this, skill.id);
		}
	}
	return false;
}

/**
 * \brief ���Լ���buff
 *
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::buffSelf()
{
	npcSkill skill;
	if (npc->getRandomSkillByType(SKILL_TYPE_BUFF, skill))
		if (zMisc::selectByPercent(skill.rate))
		{
			//Channel::sendNine(this, "�Ҹ��Լ�buff");
			if ((211==skill.id)&&(summon))
				return false;
			return useSkill(this, skill.id);
		}
	return false;
}

/**
 * \brief ������debuff
 *
 *
 * \param enemy ����ָ��
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::debuffEnemy(SceneEntryPk * enemy)
{
	npcSkill skill;
	if (npc->getRandomSkillByType(SKILL_TYPE_DEBUFF, skill))
		if (zMisc::selectByPercent(skill.rate))
		{
			//Channel::sendNine(this, "�Ҹ� %s debuff", enemy->name);
			return useSkill(enemy, skill.id);
		}
	return false;
}


/**
 * \brief ��������
 *
 *
 * \param enemy ��������
 * \return �����Ƿ�ɹ�
 */
bool SceneNpc::attackEnemy(SceneEntryPk * enemy)
{
	//����ʹ�ü���
	npcSkill skill;
	if (npc->getRandomSkillByType(SKILL_TYPE_DAMAGE, skill))
		if (zMisc::selectByPercent(skill.rate))
			return useSkill(enemy, skill.id);
	/*
	*/

	if (inRange(enemy))
		return attackTarget(enemy);
	else
		return moveToEnemy(enemy);
}


/**
 * \brief ��Ŀ���ƶ�
 *
 *
 * \param enemy �ƶ�Ŀ�����
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::moveToEnemy(SceneEntryPk * enemy)
{
	if (!canMove()) return false;
	if (aif&AIF_RUN_AWAY) return false;

	if (scene->zPosShortRange(getPos(), enemy->getPos(), 1)) return false;
	if (checkMoveTime(SceneTimeTick::currentTime) && canMove())
	{
		if (aif&AIF_WARP_MOVE)
			warp(enemy->getPos());
		else
			if (!gotoFindPath(getPos(), enemy->getPos()))
			{
#ifdef _XWL_DEBUG
				//Zebra::logger->debug("%s Ѱ·ʧ�� closeCount=%u", name, closeCount);
#endif
				if ((getPetType()==Cmd::PET_TYPE_NOTPET)
						&&(npc->kind==NPC_TYPE_BBOSS||npc->kind==NPC_TYPE_LBOSS||npc->kind==NPC_TYPE_PBOSS))
				{
					closeCount++;
					if (closeCount>=5)
					{
						t_NpcAIDefine ad;
						ad.type = NPC_AI_FLEE;
						ad.flee = true;
						ad.fleeCount = 20;
						ad.fleeDir = enemy->getPos().getDirect(getPos());
						AIC->setAI(ad, false);

						closeCount = 0;
					}
				}
				return goTo(enemy->getPos());
			}
	}
	return true;
}


/**
 * \brief �����Լ���ͬ��
 *
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::healFellow(SceneEntryPk_vec &fellows)
{
	if (!(aif&AIF_HEAL_FELLOW_5)) return false;

	for (unsigned int i=0;i<fellows.size();i++)
	{
		if (fellows[i]->getHp()*2<fellows[i]->getMaxHp())
		{
			npcSkill skill;
			if (npc->getRandomSkillByType(SKILL_TYPE_RECOVER, skill))
				if (zMisc::selectByPercent(skill.rate))
				{
					((SceneNpc *)fellows[i])->randomChat(NPC_CHAT_ON_BE_HELP);
					return useSkill(fellows[i], skill.id);
				}
		}
	}

	return false;
}

/**
 * \brief ��ͬ��buff
 *
 *
 * \param fellows ͬ���б�
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::buffFellow(SceneEntryPk_vec &fellows)
{
	if (!(aif&AIF_BUFF_FELLOW_5)) return false;

	for (unsigned int i=0;i<fellows.size();i++)
	{
		switch (fellows[i]->getType())
		{
			case zSceneEntry::SceneEntry_Player:
				{
				}
				break;
			case zSceneEntry::SceneEntry_NPC:
				{
					//if (CHASE_NONE != ((SceneNpc *)fellows[i])->getChaseMode())
					if (fellows[i]->isFighting())
					{
						npcSkill skill;
						if (npc->getRandomSkillByType(SKILL_TYPE_BUFF, skill))
							if (zMisc::selectByPercent(skill.rate))
							{
								((SceneNpc *)fellows[i])->randomChat(NPC_CHAT_ON_BE_HELP);
								return useSkill(fellows[i], skill.id);
							}
					}
				}
				break;
			default:
				break;
		}
	}
	return false;
}

/**
 * \brief ������ͬ�鹥��
 *
 *
 * \param fellows ͬ���б�
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::helpFellow(SceneEntryPk_vec &fellows)
{
	if (!(aif&AIF_HELP_FELLOW_5)) return false;

	for (unsigned int i=0;i<fellows.size();i++)
	{
		switch (fellows[i]->getType())
		{
			case zSceneEntry::SceneEntry_Player:
				{
				}
				break;
			case zSceneEntry::SceneEntry_NPC:
				{
					//if (CHASE_NONE != ((SceneNpc *)fellows[i])->getChaseMode())
					if (fellows[i]->isFighting())
					{
						SceneEntryPk * pk = ((SceneNpc *)fellows[i])->getChaseSceneEntry();
						if (pk)
						{
							chaseSceneEntry(pk->getType(), pk->tempid);
							randomChat(NPC_CHAT_ON_HELP);
							((SceneNpc *)fellows[i])->randomChat(NPC_CHAT_ON_BE_HELP);
							return true;
						}
					}
				}
				break;
			default:
				break;
		}
	}

	return false;
}

/**
 * \brief ����ƶ�
 *
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::randomMove()
{
	if (!canMove()) return false;

	if (checkMoveTime(SceneTimeTick::currentTime)
			&& zMisc::selectByPercent(2))
	{
		int dir = zMisc::randBetween(0, 7);
		zPos newPos;
		scene->getNextPos(pos, dir, newPos);
		zPosI newPosI = 0;
		scene->zPos2zPosI(newPos, newPosI);
		if (getPosI()==newPosI)
			return shiftMove(dir);//����ƶ�
	}
	return false;
}

/**
 * \brief �ж�һ��entry�ǲ��ǵ���
 * �����������ж�һ�������ǲ����ѷ�
 * ������������ǵ���
 * ����Է��������������������ͬ
 * ��������Ȼ�״̬���෴
 *
 * ���Ĭ���ǵ���
 * npcĬ�����ѷ�
 *
 * \param entry ����ָ��
 * \param notify �Ƿ�֪ͨ
 * \return 0:�ѷ� 1���ǵ��� -1������`
 */
int SceneNpc::isEnemy(SceneEntryPk * entry, bool notify, bool good)
{
	if (!entry) return -1;
	if (this==entry) return 0;
	if ((entry->frenzy)||(frenzy)) return 1;

	/*
	   SceneEntryPk * etm = entry->getTopMaster();
	   if (etm && etm!=entry)
	   return (isEnemy(etm));
	   */

	if (npc->kind==NPC_TYPE_SOLDIER)//ʿ����ֻ���������
	{
		if (entry->getCurTarget()==this) return 1;
		if (entry->getTopMaster()->getType()!=zSceneEntry::SceneEntry_Player) return -1;
		SceneUser * pUser = (SceneUser *)entry->getTopMaster();
		if (pUser->mask.is_masking() && !pUser->isSpecWar(Cmd::COUNTRY_FORMAL_DARE)) return -1;

		//������
		if (6==scene->getCountryID()) return -1;
	
		if (pUser->charbase.country!=scene->getCountryID())	return 1;

		return -1;
	}

	if (npc->kind==NPC_TYPE_UNIONATTACKER)//����ʿ��
	{
		if (entry->getCurTarget()==this) return 1;
		if (entry->getTopMaster()->getType()!=zSceneEntry::SceneEntry_Player) return -1;
		SceneUser * pUser = (SceneUser *)entry->getTopMaster();
		if (pUser->isAtt(Cmd::UNION_CITY_DARE))
			return -1;
		else
			return 1;
	}

	if (npc->kind==NPC_TYPE_UNIONGUARD)//��ս����
	{
		if (entry->getCurTarget()==this) return 1;
		if (entry->getTopMaster()->getType()!=zSceneEntry::SceneEntry_Player) return -1;
		SceneUser * pUser = (SceneUser *)entry->getTopMaster();
		if (pUser->isAtt(Cmd::UNION_CITY_DARE))//�Ƿ񹥷�
			return 1;
		else
			if (pUser->scene->getUnionDare() && !pUser->isSpecWar(Cmd::UNION_CITY_DARE))//��ս�ڼ��ֲ����ս��
				return 1;
			else
				return -1;//��ս�ڼ���ս���Ҳ��ǹ����������ط�
	}

	//����
	if ((NPC_TYPE_GUARD==npc->kind)||(aif&AIF_ATK_REDNAME))
	{
		if (entry->getState()!=zSceneEntry::SceneEntry_Normal) return -1;
		if (entry->getTopMaster()->getType()==zSceneEntry::SceneEntry_NPC) return -1;
		if (entry->isRedNamed()) return 1;
		if (entry->getType()==zSceneEntry::SceneEntry_NPC
				&& ((NPC_TYPE_GUARD==((SceneNpc *)entry)->npc->kind)||(((SceneNpc *)entry)->aif&AIF_ATK_REDNAME)))
			return 0;

		SceneEntryPk * t = entry->getCurTarget();
		if (t)
		{
			if (t==this) return 1;
			if (t->getState() == zSceneEntry::SceneEntry_Normal)
			{
				if (t->isRedNamed()) return 0;
				if ((t->frenzy)||(frenzy)) return 1;

				switch (t->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							SceneUser * user = (SceneUser *)t;
							if (user->charbase.country!=scene->getCountryID())
								return 0;
							if (user->charbase.goodness&Cmd::GOODNESS_ATT)
								return 0;

							//���˵Ĺ���
							if (entry->getType()==zSceneEntry::SceneEntry_NPC)
								return 1;
							/*
							   if (entry->getType()==zSceneEntry::SceneEntry_Player)
							//����Ҵ��ս��״̬��ûȡ���ᵽ����
							return -1;
							else if (entry->getType()==zSceneEntry::SceneEntry_NPC)
							return 1;
							*/
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							SceneNpc * n = (SceneNpc *)t;
							if ((NPC_TYPE_GUARD==n->npc->kind)||(n->aif&AIF_ATK_REDNAME))
								return 1;
						}
						break;
					default:
						break;
				}
			}
		}

		switch (entry->getType())
		{
			case zSceneEntry::SceneEntry_Player:
				{
					SceneUser * user = (SceneUser *)entry;
					if (user->mask.is_masking() && !user->isSpecWar(Cmd::COUNTRY_FORMAL_DARE)) return -1;
					if (user->charbase.country!=scene->getCountryID() && user->scene
							&& user->charbase.unionid!=user->scene->getHoldUnion())
						return 1;
					if (user->charbase.goodness&Cmd::GOODNESS_ATT)
						return 1;
					return 0;
				}
				break;
			case zSceneEntry::SceneEntry_NPC:
				{
					return -1;
				}
				break;
			default:
				return -1;
				break;
		}
	}

	switch (entry->getType())
	{
		case zSceneEntry::SceneEntry_Player:
			{
				if (entry==getMaster())
					return 0;
				else
					return 1;
			}
			break;
		case zSceneEntry::SceneEntry_NPC:
			{
				if (!((SceneNpc *)entry)->isBugbear()) return -1;
				if (((SceneNpc *)entry)->getPetType()==Cmd::PET_TYPE_CARTOON) return -1;
				switch(((SceneNpc *)entry)->npc->kind)
				{
					case NPC_TYPE_HUMAN:			///����
					case NPC_TYPE_NORMAL:			/// ��ͨ����
					case NPC_TYPE_BBOSS:			/// ��Boss����
					case NPC_TYPE_PBOSS:			/// ��Boss����
					case NPC_TYPE_LBOSS:			/// СBoss����
					case NPC_TYPE_BACKBONE:			/// ��Ӣ����
					case NPC_TYPE_GOLD:				/// �ƽ�����
					case NPC_TYPE_SUMMONS:			/// �ٻ�����
					case NPC_TYPE_AGGRANDIZEMENT:	/// ǿ������
					case NPC_TYPE_ABERRANCE:		/// ��������
					case NPC_TYPE_BACKBONEBUG:		/// ��������
					case NPC_TYPE_PET:	/// ��������
					case NPC_TYPE_TOTEM:			/// ͼ������
						{
							SceneEntryPk * hisMaster = ((SceneNpc *)entry)->getTopMaster();
							if (hisMaster->getType()==zSceneEntry::SceneEntry_Player)
								return 1;
							else if (hisMaster->getType()==zSceneEntry::SceneEntry_NPC)
								return 0;
							/*
							   SceneEntryPk * hisMaster = ((SceneNpc *)entry)->getMaster();
							   if (hisMaster)
							   {
							   Zebra::logger->debug("isEnemy 2166 %s(N)->%s(N) ʱ���� %s(Entry)", name, entry->name, hisMaster->name);
							   return isEnemy(hisMaster);
							   }
							   else
							   return 0;
							   */
						}
						break;
					case NPC_TYPE_GUARD:	/// ʿ������
						//return 1;
						//break;
					case NPC_TYPE_TRADE:	/// ��������
					case NPC_TYPE_TASK:		/// ��������
					default:
						return -1;
						break;
				}
			}
			break;
		default:
			return -1;
			break;
	}

	return -1;
}

/**
 * \brief ����ָ���ͼ��������������ҵĻص�����
 *
 */
struct getEntriesCallBack : public zSceneEntryCallBack
{
	SceneNpc * npc;
	int radius;
	SceneEntryPk_vec& entries;
	const int state;
	getEntriesCallBack(SceneNpc * npc, int radius, SceneEntryPk_vec& entries, const int state) : npc(npc), radius(radius), entries(entries), state(state)
	{
		entries.reserve(4 * radius * radius);
	}
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ�������������Ҹ�npc 
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		SceneEntryPk * e = (SceneEntryPk *)entry;
		if (entry
				&& entry->getState() == zSceneEntry::SceneEntry_Normal
				&& npc->scene->zPosShortRange(npc->getPos(), e->getPos(), radius)
				&& npc->isEnemy(e)==state
				&& npc->canReach(e)
				&& !((SceneEntryPk *)entry)->hideme)
		{
			entries.push_back(e);
		}
		return true;
	}
};

/**
 * \brief �õ�һ����Χ�ڵ��ѷ�
 *
 * \param radius ��Χ�뾶
 * \param entries �������ŵõ���ָ�������
 * \param state Ҫ�õ��Ķ����״̬ 0:�ѷ� 1:���� -1:����
 * \return �Ƿ��ҵ�
 */
bool SceneNpc::getEntries(int radius, SceneEntryPk_vec & entries, int state)
{
#ifdef _XWL_DEBUG
	//if (id==50004 || id==15001)
	//	Zebra::logger->debug("%s getEntries", name);
#endif
	//FunctionTime func_time(0,__PRETTY_FUNCTION__,name,32);
	const zPosIVector &pv = scene->getScreenByRange(pos, radius);

	getEntriesCallBack cb(this, radius, entries, state);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		scene->execAllOfScreen(zSceneEntry::SceneEntry_Player, *it, cb);
		scene->execAllOfScreen(zSceneEntry::SceneEntry_NPC, *it, cb);
	}
	return !entries.empty();
}


/**
 * \brief ������˵�λ�ò��������ƶ�
 *
 */
bool SceneNpc::moveToMaster()
{
	return false;
}

/**
 * \brief ��������ĵ���
 *
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::runOffEnemy(SceneEntryPk_vec & enemies)
{
	if (!(aif&AIF_RUN_AWAY)) return false;

	if (!(checkMoveTime(SceneTimeTick::currentTime) && canMove())) return false;

	if (enemies.empty()) return false;
	SceneEntryPk * enemy = enemies[0];
	if (!enemy) return false;

	int dir = enemy->getPos().getDirect(getPos());
	int clockwise = zMisc::selectByPercent(50)?-1:1;
	int tryDir = dir + 8;
	for (int i=0; i<8; i++)
	{
		if (shiftMove(tryDir%8))
			return true;
		tryDir += clockwise;
	}
	return false;
}

/**
 * \brief ���npc�Ƿ񱻰�Χ
 *
 * \return �Ƿ񱻰�Χ
 * 
 */
bool SceneNpc::isSurrounded()
{
	int region = 1;

	int side = 0;
	int direct = 0;
	int clockwise = 1;
	int blockCount = 0;
	int count = 0;
	zPos pos;
	scene->getNextPos(side, direct, getPos(), clockwise, pos);
	do
	{
		if (scene->checkBlock(pos))
			blockCount++;
		if (++count>=(region*2+1)*(region*2+1))
			break;
	} while(scene->getNextPos(side, direct, getPos(), clockwise, pos) && side <= region);
	return blockCount==8;
}
