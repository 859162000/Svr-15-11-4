/**
 * \file
 * \version  $Id: SceneNpc.cpp $
 * \author  
 * \date 
 * \brief ����Npc
 *
 * 
 */
#include <list>
#include <vector>
#include <algorithm>

#include "zSceneEntry.h"
#include "SceneNpc.h"
#include "Zebra.h"
#include "Chat.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TeamManager.h"
#include "TimeTick.h"
#include "SkillStatusManager.h"
#include "QuestEvent.h"
#include "QuestTable.h"
#include "Rush.h"
#include "CountryDare.h"
#include "SessionClient.h"
#include "TimeTick.h"
#include "ScenesServer.h"
#include "Command.h"
#include "CartoonPet.h"

DWORD SceneNpc::serialID = SceneNpc::maxUniqueID;
zUniqueDWORDID SceneNpc::uniqueID(1, SceneNpc::maxUniqueID);

/**
 * \brief �жϸ�npc�Ƿ���Ա�����
 * ����false����ȫ���ܱ�����
 *
 */
bool SceneNpc::canBeAttack()
{
	if (getPetType()==Cmd::PET_TYPE_RIDE//��ƥҲ���ܱ�����
			|| getPetType()==Cmd::PET_TYPE_CARTOON)
		return false;

	switch(npc->kind)
	{
		case NPC_TYPE_STORAGE:
		case NPC_TYPE_ROADSIGN:
		case NPC_TYPE_TREASURE:
		case NPC_TYPE_MOBILETRADE:
		case NPC_TYPE_LIVENPC:
		case NPC_TYPE_MAILBOX:
		case NPC_TYPE_AUCTION:
		case NPC_TYPE_SURFACE:
		case NPC_TYPE_CARTOONPET:
			{
				return false;
			}
			break;

		default:
			break;
	}
	return true;
}

/**
 * \brief �жϸ�npc�Ƿ���Ա�����(�Ƿ��ǹ���)
 * ��������NPC_TYPE_TRADE NPC_TYPE_TASK �������Ϳ��Ա�����˹���
 */
bool SceneNpc::isBugbear()
{
	if (getPetType()==Cmd::PET_TYPE_RIDE//��ƥҲ���ܱ�����
			|| getPetType()==Cmd::PET_TYPE_CARTOON)
		return false;

	switch(npc->kind)
	{
		case NPC_TYPE_TRADE:
		case NPC_TYPE_TASK:
		case NPC_TYPE_STORAGE:
		case NPC_TYPE_ROADSIGN:
		case NPC_TYPE_TREASURE:
		case NPC_TYPE_MOBILETRADE:
		case NPC_TYPE_LIVENPC:
		case NPC_TYPE_MAILBOX:
		case NPC_TYPE_AUCTION:
		case NPC_TYPE_SURFACE:
		case NPC_TYPE_CARTOONPET:
			{
				return false;
			}
			break;

		default:
			break;
	}
	return true;
}
/**
 * \brief Npc����
 *
 */
void SceneNpc::death(const zRTime &ct)
{
	this->skillStatusM.processDeath();

	killAllPets();
	petDeath();
	//����赲��Ϣ
	scene->clearBlock(getPos());

	setState(SceneEntry_Death);
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("npc %s(%u) ����", name, tempid);
#endif
	summonByNpcMap(define->deathSummonList);

	//������Ʒ�ӳ�
	setMoveTime(ct, 1000);
	setAttackTime(ct, 1000);

	if (Cmd::PET_TYPE_NOTPET==getPetType())//���ﲻ������
	{
		lostObject = true;
		mayRush = true;
	}

	if (ALLY_GUARDNPC==npc->id)
		Zebra::logger->trace("%s �˹��ڳ���ɱ��",
				SceneManager::getInstance().getCountryNameByCountryID(scene->getCountryID()));
}

/**
 * \brief ������
 * ����Գ�����������������֤�����߶Գ���������������
 * \param direct ���˷���
 * \param step ���˲���
 */
void SceneNpc::backoff(const int direct, const int step)
{
	const int walk_adjust[9][2]= { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, 0} };
	int i = 0;

	zPosI oldPosI = getPosI();
	zPos oldPos = getPos(),  newPos = getPos();
	for(i = 1; i <= step; i++)
	{
		newPos.x += walk_adjust[direct][0];
		newPos.y += walk_adjust[direct][1];
		if(scene->checkBlock(newPos))
			break;
	}
	if (i > 1)
	{
		newPos.x -= walk_adjust[direct][0];
		newPos.y -= walk_adjust[direct][1];

		if (scene->refresh(this, newPos))
		{
			scene->setBlock(newPos);
			scene->clearBlock(oldPos);
			setDir(scene->getReverseDirect(direct));

			Cmd::stBackOffMagicUserCmd cmd;
			cmd.dwTempID = tempid;
			cmd.byType = Cmd::MAPDATATYPE_NPC;
			cmd.byDirect = direct;
			cmd.x = newPos.x;
			cmd.y = newPos.y;
			scene->sendCmdToNine(oldPosI, &cmd, sizeof(cmd));

			if (oldPosI != getPosI())
			{
				Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
				removeNpc.dwMapNpcDataPosition = tempid;
				scene->sendCmdToReverseDirect(oldPosI,
						scene->getScreenDirect(oldPosI,getPosI()), &removeNpc, sizeof(removeNpc));
				Cmd::stAddMapNpcMapScreenUserCmd addNpc;
				full_t_MapNpcData(addNpc.data);
				scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()),
						&addNpc, sizeof(addNpc));
				Cmd::stRTMagicPosUserCmd ret;
				full_stRTMagicPosUserCmd(ret);
				scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), &ret, sizeof(ret));
				attackRTHpAndMp();
			}
		}
	}
	backOffing = 1000;
}

/**
 * \brief ��龭���б�����ҵĹ����Ƿ�ʱ
 * \param pAtt ������
 */
void SceneNpc::refreshExpmapAttackTime(SceneUser* pAtt)
{
}

/**
 * \brief ���� user��hp
 *
 * \param pAtt ������
 * \param wdHP ���ٵ�hp
 * \return 
 */
void SceneNpc::reduceHP(SceneUser *pAtt , DWORD wdHP)
{

}

/**
 * \brief �����Ǯ�Ļص�
 *
 */
struct MoneyTeamExecExceptMe : public TeamMemExec
{
	///�ӳ�
	SceneUser *leader;
	///�Լ�
	SceneUser *me;
	///Ǯ��
	WORD money;

	/**
	 * \brief ���캯��
	 *
	 * \param pMe �Լ�
	 * \param u �ӳ�
	 * \param mon Ǯ��
	 */
	MoneyTeamExecExceptMe(SceneUser *pMe, SceneUser *u , WORD mon)
	{
		me = pMe;
		leader = u;
		money = mon;
	}
	/**
	 * \brief ��Ǯ
	 *
	 * \param member ����
	 * \return �Ƿ�����ص�
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser && pUser != me)
		{
			if(pUser->scene->checkTwoPosIInNine(pUser->getPosI(),me->getPosI()))
			{
				if(pUser->getState() != SceneUser::SceneEntry_Death)
				{
					pUser->packs.addMoney((DWORD)(money),"�����Ǯ");
					pUser->team.putMoneyPlus(money);
				}
			}
		}
		return true;
	}

};
//���ӷ���
/**
 * \brief �����Ǯ
 *
 * \param money ��Ǯ��
 */
void SceneNpc::distributeMoney(DWORD money)
{
	SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
	if(pUser)
	{
		if(pUser->team.getLeader())
		{
			SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
			if(leader)
			{
				WORD mon = 0;
				WORD myMon = (WORD)(money*0.2f + 0.9f);
				if(leader->team.getExpSize(pUser->getPosI(),this->scene->id))
				{
					mon = (WORD)((money - myMon) / leader->team.getExpSize(pUser->getPosI(),this->scene->id) + 0.9f);
				}
				myMon +=mon;
				pUser->packs.addMoney(myMon,"�����Ǯ");
				pUser->team.putMoneyPlus(myMon);

				MoneyTeamExecExceptMe exec(pUser, leader , mon);
				leader->team.execEveryOne(exec);
			}
		}
		else
		{
			pUser->packs.addMoney(money,"�����Ǯ");
		}
	}
}

struct CountSeptTeamExec : public TeamMemExec
{
	///�������
	int count;
	///�־����ɫ�ļ���ID
	DWORD septid;
	///ɱ����npc
	SceneNpc *npc;

	CountSeptTeamExec(DWORD dwSeptid, SceneNpc *pnpc)
	{
		septid = dwSeptid;
		count=0;
		npc = pnpc;
	}

	/**
	 * \brief ͬһ��������ͳ�ƺ���
	 *
	 *
	 * \param member ����
	 * \return �Ƿ�����ص�
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			if (pUser->scene->tempid == npc->scene->tempid &&
					pUser->scene->checkTwoPosIInNine(pUser->getPosI(),npc->getPosI()))
			{
				if (pUser->charbase.septid == septid)
				{
					count++;
					if (count >1) return false;
				}
			}
		}
		return true;
	}
};

/**
 * \brief ����־���Ļص�
 *
 */
struct ExpTeamExec : public TeamMemExec
{
	///�ӳ�
	SceneUser *leader;
	///ɱ����npc
	SceneNpc *npc;
	///����ֵ
	DWORD exp;
	///����ID

	/**
	 * \brief ���캯��
	 *
	 * \param u �ӳ�
	 * \param n ɱ����npc
	 * \param wdExp ����
	 */
	ExpTeamExec(SceneUser *u , SceneNpc *n , DWORD wdExp)
	{
		leader = u;
		npc = n;
		exp = wdExp;
	}
	/**
	 * \brief ��Ǯ�ĺ���
	 *
	 *
	 * \param member ����
	 * \return �Ƿ�����ص�
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			// ����ģʽ�ӳ���������ֵ
			if((pUser == leader && pUser->team_mode == Cmd::TEAM_HONOR) || (leader->team_mode == Cmd::TEAM_HONOR &&pUser->charbase.level >= TEAM_HONOR_MEMBER_LEVEL))
			{
				return true;
			}
			if(pUser->getState() != SceneUser::SceneEntry_Death)
			{
				//bool checkUserInNine(SceneUser *pMe, SceneUser *pOhter, const bool locker = true);
				if(leader->team.getAverageExp() > 0 
						&& pUser->scene->tempid == npc->scene->tempid
						&& pUser->scene->checkTwoPosIInNine(pUser->getPosI(),npc->getPosI()))
				{
					//Zebra::logger->debug("ƽ������=%d,����=%d",leader->team.getAverageExp() , leader->team.getExpSize(npc->getPosI()));
					//DWORD wdExp=(DWORD)(exp * (((pUser->charbase.level / 30)+1.0f) / leader->team.getAverageExp()) + 0.9); 
					DWORD exp_level=0;
					if(leader->team_mode == Cmd::TEAM_HONOR)
					{
						if(leader->scene->id == pUser->scene->id 
								&& pUser->scene->checkTwoPosIInNine(pUser->getPosI(),leader->getPosI())
								&& (int)(leader->team.getSize() - 1 > 0))
						{
							exp_level = (DWORD)(leader->charbase.level / (leader->team.getSize() - 1));
						}
					}
					exp_level += pUser->charbase.level; 
					DWORD early_level=pUser->charbase.level;
					DWORD wdExp = (DWORD)(exp *((float)(exp_level) / (float)leader->team.getAverageExp()) + 0.9); 
					wdExp += pUser->team.getExpPlus(wdExp);
					if (pUser->pet) pUser->pet->addExp(wdExp);//���ﾭ�鲻���
					for (SceneUser::adopt_it it=pUser->adoptList.begin(); it!=pUser->adoptList.end(); it++)
						it->second->releaseExp(wdExp*15/100);
					wdExp = npc->levelExp(wdExp , pUser->charbase.level);
					wdExp = npc->addOtherExp(wdExp , pUser);
					if (wdExp >0)
					{
						if (pUser->scene&&(pUser->charbase.country == pUser->scene->getCountryID())) wdExp = (DWORD)(wdExp*(pUser->wdTirePer/100.0f));
						if (pUser->isSpecWar(Cmd::COUNTRY_FORMAL_DARE))
						{//��ս�ڣ���־����50%
							wdExp = wdExp/2;
						}
						//TODO �������
						if (wdExp==0) wdExp=1;

						pUser->sendExpToSept(wdExp);
						if (pUser->charbase.septid>0)
						{
							CountSeptTeamExec execCount(pUser->charbase.septid, npc);
							leader->team.execEveryOne(execCount);
							if (execCount.count>1)	wdExp= (DWORD)(wdExp*1.2f);
						}
						DWORD oldExp = wdExp;
						wdExp += pUser->scene->sceneExp(wdExp);
						if(pUser->charbase.country == pUser->scene->getCountryID())
						{
							wdExp += pUser->scene->winnerExp(oldExp);
						}
						wdExp = wdExp*(100+5*pUser->adoptList.size())/100;	//����һ����5%
						pUser->addExp(wdExp, false, npc->tempid, Cmd::MAPDATATYPE_NPC, true);
						pUser->packs.equip.obtain_exp(pUser, wdExp);
					}
					//Ϊ�ӳ�������������
					if(leader->team_mode == Cmd::TEAM_HONOR && early_level < pUser->charbase.level 
							&& pUser->charbase.level <= TEAM_HONOR_MEMBER_LEVEL
							&& leader->scene->id == pUser->scene->id
							&& pUser->scene->checkTwoPosIInNine(pUser->getPosI(),leader->getPosI()))
					{
						DWORD get_honor=0;
						for(;early_level < pUser->charbase.level ; early_level ++)
						{
							zHonorB *hb = honorbm.get(early_level);
							if(hb)
							{
								int honor_temp=0;
								honor_temp = leader->packs.equip.reduceDur(leader , Cmd::EQUIPCELLTYPE_ADORN , ItemType_Honor , hb->value , true , false);
								if(!honor_temp)
								{
									honor_temp = leader->packs.equip.reduceDur(leader , Cmd::EQUIPCELLTYPE_ADORN + 1 , ItemType_Honor , hb->value , true , false);
								}
								get_honor += honor_temp; 
							}
						}
						if(get_honor)
						{
							leader->charbase.honor += get_honor;
							leader->charbase.maxhonor += get_honor;
							zObject::logger(0,0,"����ֵ",leader->charbase.honor,get_honor,1,pUser->id , pUser->name ,leader->id,leader->name,"�������ֵ",NULL,0,0);
							Channel::sendSys(leader, Cmd::INFO_TYPE_GAME,"����������%d",get_honor);
							Cmd::stMainUserDataUserCmd  userinfo;
							leader->full_t_MainUserData(userinfo.data);
							leader->sendCmdToMe(&userinfo,sizeof(userinfo));
						}
					}
				}
			}
		}
		return true;
	}

};
/**
 * \brief ��������־���Ļص�
 *
 */
struct SpecialExpTeamExec : public TeamMemExec
{
	///��Ա
	SceneUser *user;
	///����ֵ
	SceneNpc *npc;
	DWORD exp;
	///����ID

	/**
	 * \brief ���캯��
	 *
	 * \param u �õ�������û�
	 * \param wdExp ����
	 */
	SpecialExpTeamExec(SceneUser *u , SceneNpc *n , DWORD wdExp)
	{
		user = u;
		npc=n;
		exp = wdExp/10;
		exp = exp?exp:1;
	}
	/**
	 * \brief ��Ǯ�ĺ���
	 *
	 *
	 * \param member ����
	 * \return �Ƿ�����ص�
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			// ����־��鲿�ֲ����Լ���
			if(pUser == user)
			{
				return true;
			}
			if(pUser->getState() != SceneUser::SceneEntry_Death)
			{
				if(user->scene->id == pUser->scene->id
						&& pUser->scene->checkTwoPosIInNine(pUser->getPosI(),user->getPosI()))
				{
					pUser->addExp(exp, false, npc->tempid, Cmd::MAPDATATYPE_NPC);
				}
			}
		}
		return true;
	}

};
/**
 * \brief ����־���
 *
 */
void SceneNpc::distributeExp()
{
	NpcHpHashmap_iterator iter;
	for(iter = expmap.begin() ; iter != expmap.end() ; iter ++)
	{
		if (abs(iter->second.attack_time.sec() - SceneTimeTick::currentTime.sec()) >=10)
		{
#ifdef _ZJW_DEBUG
			Zebra::logger->debug("%d ���һ�ι�������ʮ��", iter->first);
#endif			
			continue;
		}

		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(iter->first);
		if(pUser)
		{
			if(pUser->team.getLeader() && pUser->team.isNormalExp())
			{
				SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
				if(leader)
				{
					if(this->getMaxHP() > 0)
					{
						//float per = (((pUser->team.getExpSize(this->getPosI()) - 1.0f)/2.0f) * 5.0f + 5.0f)/100.0f;
						float per = ((leader->team.getExpSize(this->getPosI() , this->scene->id) - 1.0f) *20.0f)/100.0f;
						leader->team.calAverageExp(this->getPosI(),this->scene->id);
						DWORD exp = (DWORD)(((float)npc->exp * ((float)iter->second.wdHP / (float)(this->getMaxHP()))) * (1.0f + per)  + 0.9f);

						ExpTeamExec exec(leader , this , exp);
						leader->team.execEveryOne(exec);
					}
				}
			}
			else
			{
				//Zebra::logger->debug("npc����ֵ%u",npc->exp);
				DWORD wdExp = (DWORD)(npc->exp * ((float)iter->second.wdHP / (this->getMaxHP())) + 0.9f);
				//Zebra::logger->debug("����ֵ%u",wdExp);
				//����������Ӿ���ֵ
				wdExp +=pUser->pkValue.exp;
				//����
				if (pUser->pet) pUser->pet->addExp(wdExp);
				//����
				for (SceneUser::adopt_it it=pUser->adoptList.begin(); it!=pUser->adoptList.end(); it++)
					it->second->releaseExp(wdExp*15/100);
				wdExp = levelExp(wdExp , pUser->charbase.level);
				wdExp = addOtherExp(wdExp , pUser);
				if (wdExp >0)
				{
					if (pUser->scene &&(pUser->charbase.country == pUser->scene->getCountryID())) wdExp = (DWORD)(wdExp*(pUser->wdTirePer/100.0f));

					if (pUser->isSpecWar(Cmd::COUNTRY_FORMAL_DARE))
					{//��ս�ڣ���־����50%
						wdExp = wdExp/2;
					}

					if (wdExp==0) wdExp=1;
					pUser->sendExpToSept(wdExp);
					pUser->packs.equip.obtain_exp(pUser, wdExp);
					//�������־��鷽ʽ
					if(pUser->team.IsTeamed() && pUser->team.isSpecialExp())
					{
						SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
						if(leader)
						{
							SpecialExpTeamExec exec(pUser, this,wdExp);
							leader->team.execEveryOne(exec);
						}
					}
					DWORD oldExp = wdExp;
					wdExp += pUser->scene->sceneExp(wdExp);
					if(pUser->charbase.country == pUser->scene->getCountryID())
					{
						wdExp += pUser->scene->winnerExp(oldExp);
					}
					wdExp = wdExp*(100+5*pUser->adoptList.size())/100;	//����һ����5%
					pUser->addExp(wdExp, false, this->tempid, Cmd::MAPDATATYPE_NPC, true);
				}
			}
		}
	}
	expmap.clear();
}

DWORD SceneNpc::addOtherExp(DWORD wdExp , SceneUser *pUser)
{
	if(pUser)
	{
		if(pUser->issetUState(Cmd::USTATE_GUARD))
		{
			wdExp = (DWORD)(wdExp * 1.5f);
		}
		BYTE per = pUser->packs.equip.getEquips().get_doublexp();
		if ((per>20)&&(per<100)) per=20;
		else if (per>=100) per=100;

		if(ScenesService::getInstance().double_exp||zMisc::selectByPercent(per))
		{
			wdExp <<= 1;
		}
	}
	return wdExp;
}

DWORD SceneNpc::levelExp(DWORD wdExp , DWORD char_level)
{
	int diff = (int)(char_level - this->npc->level);
	if(diff >= 6 && diff < 10)
	{
		wdExp = (DWORD)((float)wdExp * 0.5f);
	}
	else if(diff >= 10)
	{
		wdExp = 1;
	}
	return wdExp;
}

void SceneNpc::changeHP(const SDWORD &curHp)
{
	SDWORD changeValue = 0;

	if (((int)this->hp)+(int)curHp>=0)
	{
		changeValue = this->hp;
		this->hp += curHp;
		if (this->hp > this->getMaxHP()) this->hp = this->getMaxHP();
		changeValue = (int)this->hp-changeValue;
	}
	else
	{
		changeValue= this->hp;
		this->hp=0;
	}

	notifyHMS = true;

	if (changeValue !=0)
	{
		Cmd::stObjectHpMpPopUserCmd ret;
		ret.dwUserTempID = this->tempid;
		ret.byTarget = Cmd::MAPDATATYPE_NPC;
		ret.vChange = (int)changeValue;
		ret.type = Cmd::POP_HP;
		this->scene->sendCmdToNine(getPosI(), &ret , sizeof(ret));
	}
}


void SceneNpc::changeMP(const SDWORD &mp)
{
    notifyHMS = true;
}

void SceneNpc::changeSP(const SDWORD &sp)
{
    notifyHMS = true;
}


bool SceneNpc::preAttackMe(SceneEntryPk *pEntry, const Cmd::stAttackMagicUserCmd *rev, bool physics, const bool good)
{
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::preAttackMe %s hp=%u", name, hp);
#endif
	if(getState() != zSceneEntry::SceneEntry_Normal
			|| !canBeAttack())  
	{
		if (rev)
			ScenePk::attackFailToMe(rev, pEntry, true);

		return false;
	}

	if (this->npc->kind == NPC_TYPE_RESOURCE)
	{
		if (pEntry->getType() != zSceneEntry::SceneEntry_Player)
		{
			if (rev)
				ScenePk::attackFailToMe(rev, pEntry, true);
			return false;
		}
		else
		{
			if (rev)
			{
				if (rev->wdMagicType != SKILLNORMAL)
				{
					return false;
				}
			}
			SceneUser *pUser = (SceneUser *)pEntry;
			zObject *temp=NULL;
			bool ret=false;
			if(pUser->packs.equip.getObjectByZone(&temp , 0 , Cmd::EQUIPCELLTYPE_HANDR))
			{
				if(temp)
				{
					if (0 != temp->data.dur)
					{
						if (temp->base->id == 876) //�ɼ�����
						{
							ret=true;
						}
					}
				}
			}
			if (!ret)
			{
				if (rev)
					ScenePk::attackFailToMe(rev, pEntry, true);
				return false;
			}
		}
	}

	if (!good)
	{
		DWORD attackRating = 0; // ������
		DWORD attLevel = 0;
		DWORD attCountryID = 0;
		bool  isUnionCityWar = false;
		bool  isCountryFormal = false; // �жϴ󽫾��������빥�����Ƿ����ڵж�
		bool  isAtt = true; // Ĭ��Ϊ�������ս�еĹ���
		bool  isAttCountry = true; // ��ս�еĹ���
		bool  isAntiCountry = false;

		SceneEntryPk * m = ((SceneEntryPk *)pEntry)->getMaster();
		while (m && m->getType()!=zSceneEntry::SceneEntry_Player)
			m = m->getMaster();

		SceneUser * master = NULL;

		if (m)
		{
			master = (SceneUser*)m;
			attackRating = master->charstate.attackrating;
			attLevel = master->charbase.level;
			attCountryID = master->charbase.country;
			isUnionCityWar = master->isSpecWar(Cmd::UNION_CITY_DARE);

			isCountryFormal = master->isWarRecord(Cmd::COUNTRY_FORMAL_DARE, this->scene->getCountryID());
			isAtt	= master->isAtt(Cmd::UNION_CITY_DARE);
			isAttCountry = master->isAtt(Cmd::COUNTRY_FORMAL_DARE);
			isAntiCountry = master->isAntiAtt(Cmd::COUNTRY_FORMAL_DARE, this->scene->getCountryID());

			refreshExpmapAttackTime(master);
		}
		else
		{
			attackRating = ((SceneNpc*)pEntry)->npc->rating;
			attLevel = ((SceneNpc *)pEntry)->getLevel();
			attCountryID = ((SceneNpc *)pEntry)->scene->getCountryID();
		}

		if (this->id == 58101) // �����ID
		{
//			if (!isUnionCityWar || 	(this->scene->getCountryID() != attCountryID && this->scene->getCountryID()!=6) || !isAtt)
			if (!isUnionCityWar || !isAtt)

			{
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "�����ڶ��ս�����ǹ���,���ܹ���58101");
#endif			

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id == COUNTRY_MAIN_FLAG)
		{
			if (!CountryDareM::getMe().isAttackMainFlag(this->scene, COUNTRY_SEC_FLAG))
			{
				if(master)  
				{
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "�����ڸ��죬���ܹ�������"); 
				}
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "��������ʧ�ܣ������ڸ���");
#endif			

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id==COUNTRY_MAIN_FLAG || this->id==COUNTRY_SEC_FLAG)
		{

			if (this->scene->getCountryID() ==attCountryID)
			{
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "�����˲�����������");
#endif			
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);
				return false;
			}

			if (attLevel>50)
			{
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "����50������������˳��ڵĹ���");
#endif			

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id==COUNTRY_KING_MAIN_FLAG || this->id==COUNTRY_KING_SEC_FLAG)
		{
			if (this->scene->getCountryID() ==attCountryID)
			{
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "�����˲�����������");
#endif			
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;

			}

			if (isCountryFormal)
			{
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id == COUNTRY_KING_MAIN_FLAG)
		{
			if (!CountryDareM::getMe().isAttackMainFlag(this->scene, COUNTRY_KING_SEC_FLAG))
			{
				if(master)  
				{
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "�����ڸ��죬���ܹ�������"); 
				}
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "��������ʧ�ܣ������ڸ���");
#endif			
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->isMainGeneral() || this->id==COUNTRY_SEC_GEN)
		{//�����˲��������󽫾����齫
			if (this->scene->getCountryID() ==attCountryID)
			{
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);
				return false;
			}
		}
		if (this->isMainGeneral())
		{//�ж��Ƿ��������󽫾�
			if (!isCountryFormal || !CountryDareM::getMe().isAttackMainGen(this->scene) ||
					(!isAttCountry && !isAntiCountry))
			{
				if (master)
				{
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "���ڹ�ս�ڼ���ǽ����������ܹ����󽫾�");
				}

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id==COUNTRY_EMPEROR_MAIN_GEN || this->id==COUNTRY_EMPEROR_SEC_GEN)
		{//ռ����˲��������ʳǴ󽫾��ͻʳ��齫
			if (this->scene->getEmperorDare())
			{
				if (this->scene->getEmperorDareDef() == attCountryID)
				{// �Լ��˲����Լ���
					if (rev)
						ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);
					return false;
				}
			}
			else
			{//���ڻʳ�����ս�ڼ�,˭�����ܴ�ʳǴ󽫾����齫
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);
				return false;
			}
		}

		if (this->id==COUNTRY_EMPEROR_MAIN_GEN)
		{//�ж��Ƿ��������ʳǴ󽫾�
			if (!this->scene->getEmperorDare() 
					|| !CountryDareM::getMe().isAttackMainFlag(this->scene, COUNTRY_EMPEROR_SEC_GEN))
			{
				if (master) 
				{
					//Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "�����ڽ����ӳ����ǲ�ս�������ܹ����󽫾�"); 
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "���ڻʳ�ս�ڼ���н����ӳ������ܹ����󽫾�");
				}

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if ((this->id == COUNTRY_SEC_GEN) && !this->scene->getCountryDare())
		{//�ж��Ƿ��������齫
			if (master) 
			{
				Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "���ڹ�ս�ڼ䣬���ܹ��������ӳ�"); 
			}

			if (rev)
				ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

			return false;
		}
	}

	//��ʼ��pk����
	this->pkValue.init();
	this->skillValue.init();

	this->skillStatusM.processPassiveness();	// �����ҵı���״̬Ӱ��

	if (pEntry->getType() == SceneEntry_Player)
	{
		SceneUser *pAtt = (SceneUser *)pEntry;
		ScenePk::calpdamU2N(rev , pAtt , this);
	}
	else
	{
		SceneNpc *pAtt = (SceneNpc *)pEntry;
		ScenePk::calpdamN2N(rev, pAtt , this);
	}
	if (pEntry->getType() == SceneEntry_Player)
	{
		SceneUser *pAtt = (SceneUser *)pEntry;
		ScenePk::calmdamU2N(rev , pAtt , this);
	}
	else
	{
		SceneNpc *pAtt = (SceneNpc *)pEntry;
		ScenePk::calmdamN2N(rev, pAtt , this);
	}

	SceneEntryPk *pMaster = this->getTopMaster();
	if (pMaster&&pMaster->getType() == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = (SceneUser *)pMaster;
		pUser->packs.equip.costDefenceDurByPet(pUser);
	}
	return true;
}

bool SceneNpc::AttackMe(SceneEntryPk *pAtt, const Cmd::stAttackMagicUserCmd *rev, bool physics, SWORD rangDamageBonus)
{
	using namespace Cmd;

	SceneEntryPk * am = pAtt->getTopMaster();
	if (am->getType()==zSceneEntry::SceneEntry_Player)
	{
		SceneUser * a = (SceneUser *)am;
		//������˴��һ��ʱ����֪ͨ
		if (hp==getMaxHp() && scene->getCountryID()!=a->charbase.country)
		{
			char buf[MAX_CHATINFO];
			bzero(buf, sizeof(buf));
			snprintf(buf, MAX_CHATINFO-1, "%s(%s) ���� %s(%u,%u) ���� %s,����֧Ԯ"
					, a->name, SceneManager::getInstance().getCountryNameByCountryID(a->charbase.country)
					, scene->name, getPos().x, getPos().y, name);

			//��������NPC��ȫ��֪ͨ
			if (!isBugbear() && canBeAttack())
				Channel::sendCountryInfo(scene->getCountryID(), Cmd::INFO_TYPE_EXP, buf);

			//�����󽫾��ͽ����ӳ��������ǡ��߾����������Ͻ�֪ͨ
			if (isMainGeneral() || id==COUNTRY_SEC_GEN)
			{
				DWORD high = (scene->getCountryID()<<16);
				Channel::sendMapInfo(high+WANGCHENG_MAP_ID, Cmd::INFO_TYPE_EXP, buf);
				Channel::sendMapInfo(high+DONGJIAO_MAP_ID, Cmd::INFO_TYPE_EXP, buf);
				Channel::sendMapInfo(high+NANJIAO_MAP_ID, Cmd::INFO_TYPE_EXP, buf);
				Channel::sendMapInfo(high+BIANJING_MAP_ID, Cmd::INFO_TYPE_EXP, buf);
			}
		}
	}

	if (pAtt->getMaster())
	{
		this->setLockUser(pAtt->getMaster()->id);
	}
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::AttackMe %s hp=%u", name, hp);
#endif
	SceneEntryPk::AttackMe(pAtt,rev,physics,rangDamageBonus);

	if (Cmd::PET_TYPE_NOTPET!=getPetType())
	{
		((ScenePet *)this)->sendHpExp();
	}
	//����AI
	if (AIC)
		AIC->on_hit(pAtt);
	if (aif&AIF_GIVEUP_6_SEC)
		setEndBattleTime(SceneTimeTick::currentTime, 6*1000);
	if (aif&AIF_GIVEUP_3_SEC)
		setEndBattleTime(SceneTimeTick::currentTime, 3*1000);
	//setAttackTime(SceneTimeTick::currentTime,1200);
	return true;
}


struct HpTeamExec : public TeamMemExec
{
	const Cmd::stNPCHPMapScreenUserCmd &ret;
	HpTeamExec(const Cmd::stNPCHPMapScreenUserCmd &cmd) : ret(cmd)
	{
	}
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			pUser->sendCmdToMe(&ret , sizeof(ret));
			return true;
		}
		else
		{
			return true;
		}
	}
};

/**
 * \brief �ж�һ�������Ƿ��ڹ����Լ�
 * ���ݾ����б����ж�
 * ֻ�������ж���Һͳ���
 *
 * \param entry Ҫ�жϵĶ���
 * \return �Ƿ��ڹ����Լ�
 */
bool SceneNpc::isAttackMe(SceneEntryPk *entry)
{
	if(!entry) return false;

	if (zSceneEntry::SceneEntry_NPC==entry->getType())
	{
		if (Cmd::PET_TYPE_NOTPET!=((SceneNpc *)entry)->getPetType())
			return isAttackMe(((SceneNpc *)entry)->getMaster());
		else
			return false;
	}
	else if (zSceneEntry::SceneEntry_Player==entry->getType())
	{
		SceneUser * pUser = (SceneUser *)entry;
		if(pUser->team.getLeader())
		{
			NpcHpHashmap_iterator iter;
			for(iter = expmap.begin() ; iter != expmap.end() ; iter ++)
			{
				SceneUser *u = SceneUserManager::getMe().getUserByTempID(iter->first);
				if(u)
				{
					if (pUser->team.getLeader() == u->team.getLeader())
					{
						return true;
					}
				}
			}
		}
		else
		{
			NpcHpHashmap_iterator iter;
			iter = expmap.find(pUser->tempid);
			if(iter != expmap.end())
			{
				return true;
			}
		}
	}
	return false;
}
void SceneNpc::showHP(SceneUser *pUser, DWORD npchp)
{
	if(!pUser)
	{
		return ;
	}
	Cmd::stNPCHPMapScreenUserCmd ret;
	ret.dwMapNpcDataPosition = tempid;
	ret.dwHP = npchp;//this->hp;				
	ret.dwMaxHP =getMaxHP();				
	pUser->sendCmdToMe(&ret , sizeof(ret));
}
/**
 * \brief ֪ͨ�ͻ�������ֵ�ı仯
 * \author fqnewman
 */
void SceneNpc::attackRTHpAndMp()
{
    notifyHMS = false;
}


/**
 * \brief ���캯��
 * \param scene npc���ڵĳ���
 * \param npc ��������
 * \param define ��������
 * \param type ����
 * \param entrytype �������(��ҡ�npc��������)
 * \param anpc ����ǿ����npc base
 */
SceneNpc::SceneNpc(Scene *scene, zNpcB *npc, const t_NpcDefine *define, const SceneNpcType type, const SceneEntryType entrytype, zNpcB *anpc) : SceneEntryPk(entrytype, SceneEntry_Hide),npc(npc), anpc(anpc), define(define), _half_sec(0.3f), _one_sec(1), _3_sec(3), lockedUserTime(), nextMoveTime(npc->distance), nextAttackTime(npc->adistance), type(type), isUse(false)
{
	id = npc->id;
	this->scene = scene;
	if (type == STATIC)
	{
		tempid = ++serialID;
	}
	else if (type == GANG)
	{
		tempid = uniqueID.get();
		if (tempid==uniqueID.invalid())
			Zebra::logger->fatal("%s ��ʱ��ŷ������", __PRETTY_FUNCTION__);
	}
	if ((char)define->name[0]) {
		strncpy(name, define->name, MAX_NAMESIZE);
	}else {
		strncpy(name, npc->name, MAX_NAMESIZE);
	}

	catchme = 0; //�������˹�����־
	boostupPet =0; // ������ǿ
	boostupPetMDef = 0; //��ǿ����ķ�������
	boostupSummon = 0; //�ٻ��޹�����ǿ
	dwReduceDam = 0; //�ٻ����˺��ۼ�
	giddy =0; //ŭ��ѵ��ʹ�Է�ѣ�εļ���
	boostupHpMaxP =0; // ��ǿ����������ֵ�ٷֱ�

	hp = this->getMaxHP();//npc->hp+(anpc?anpc->hp:0);
	chaseMode = CHASE_NONE;
	curTargetType = 0;
	curTargetID = 0;
	defTargetType = 0;
	defTargetID = 0;
	dwNpcLockedUser = 0;
	lostObject = false;
	notifyHMS = false;
	clearMe = false;
	backOffing = 0;
	dwStandTime =SceneTimeTick::currentTime.sec();
	dwStandTimeCount=0;
	skillStatusM.initMe(this);

	speedRate = 1.0;
	aspeedRate = 1.0;
	speedUpUnder20 = false;
	aspeedUpUnder50 = false;
	recoverUnder30 = false;
	//master = NULL;

	appendMinDamage = 0;
	appendMaxDamage = 0;

	AIC = new NpcAIController(this);

	AIC->setActRegion(define->region.c, define->width/2, define->height/2);
//	AIC->loadScript(define->scriptID);

	isRushNpc = false;
	lockTarget = false;

	needRecover = false;

	notifystep=0; // ��boss ֪ͨ����

	//petAI = 0;
	aif = npc->ai;
#ifdef _XWL_DEBUG
	//aif = npc->ai | AIF_LIMIT_REGION;
#endif
	//����
	pet = 0;
	summon = 0;
	totems.clear();

	summoned = false;

	switch(this->npc->kind)
	{
		case NPC_TYPE_TOTEM:
			{
				switch(this->npc->id)
				{
					case 300:
						{
							this->setUState(39);
						}
						break;
					default:
						{
							this->setUState(39);
						}
						break;
				}
			}
			break;
		default:
			break;
	}

	///����npc����
	if (!npc->skillMap.empty())
	{
		std::map<int, std::vector<npcSkill> >::iterator map_it;
		for (map_it=npc->skillMap.begin();map_it!=npc->skillMap.end();map_it++)
		{
			std::vector<npcSkill>::iterator skill_it;
			for (skill_it=map_it->second.begin();skill_it!=map_it->second.end();skill_it++)
			{
				//zSkill::create(this , skill_it->id , 1);

				zSkill *skill = zSkill::create(this , skill_it->id , 1);
				if(!skill)
					Zebra::logger->error("SceneNpc::SnceneNpc():�޷����ؼ��� name=%s skill=%u level=%u",npc->name, skill_it->id, npc->level);
#ifdef _DEBUGLOG
				else
					Zebra::logger->error("NPC[%s]���ؼ���skill=[%u] level=[%u]",npc->name, skill_it->id, npc->level);
#endif
			}
		}
	}
	if (npc->skill>0)
	{
		zSkill::create(this , npc->skill , npc->level) ;
	}
}

/**
 * \brief ��������
 *
 */
SceneNpc::~SceneNpc()
{
	if (type == GANG)
	{
		uniqueID.put(tempid);
		SAFE_DELETE(define);
	}
	SAFE_DELETE(AIC);
}

/**
 * \brief ����Npc����״̬
 * \return ����״̬
 */
SceneNpc::SceneNpcChase SceneNpc::getChaseMode() const
{
    return chaseMode;
}

/**
 * \brief ��ȡ�����û����
 * \return �û����
 */
SceneEntryPk* SceneNpc::getChaseSceneEntry() const
{
	switch (curTargetType)
	{
		case zSceneEntry::SceneEntry_Player:
			{
				return SceneUserManager::getMe().getUserByTempID(curTargetID);
			}
			break;
		case zSceneEntry::SceneEntry_NPC:
			{
				return SceneNpcManager::getMe().getNpcByTempID(curTargetID);
			}
			break;
		default:
			{
				return NULL;
			}
			break;
	}
	return NULL;
}

/**
 * \brief ���ø����û�
 * \param type ���ٶ��������
 * \param entryid Ҫ�����û��ı��
 * \return �Ƿ���ٳɹ�
 */
bool SceneNpc::chaseSceneEntry(const DWORD type, const DWORD entryid)
{
    //�������Լ�
    if (type==(DWORD)getType()&&entryid==tempid) return false;

	if (curTargetID!=0)
			return false;
	else
	{
			chaseMode = CHASE_ATTACK;
			//curTargetType = type;
			//curTargetID = entryid;
			setCurTarget(entryid, type);
			if (aif&AIF_GIVEUP_10_SEC)
					setEndBattleTime(SceneTimeTick::currentTime, 10*1000);
			return true;
	}
}

/**
 * \brief ǿ�Ƹ����û���������Ѿ��ڸ����û�����ô��45%�ļ��ʽ�Ŀ��ת����Ŀǰ���û�
 * \param userid Ҫ�����û��ı��
 * \return �Ƿ���ٳɹ�
 */
bool SceneNpc::forceChaseUser(SceneEntryPk *pAtt)
{
	//if (chaseMode != CHASE_NONE)
	if (curTargetID!=0)
	{
		if (zMisc::selectByPercent(30))
		{
			chaseMode = CHASE_ATTACK;
			setCurTarget(pAtt);
			//Channel::sendNine(this, "��������Ŀ��%s", pAtt->name);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		chaseMode = CHASE_ATTACK;
		setCurTarget(pAtt);
		AIC->on_find_enemy(pAtt);
		if (aif&AIF_GIVEUP_10_SEC)
			setEndBattleTime(SceneTimeTick::currentTime, 10*1000);

		return true;
	}
}

/**
 * \brief ȡ�����û����и���
 *
 */
void SceneNpc::unChaseUser()
{
	if (0!=curTargetID)
	{
		chaseMode = CHASE_NONE;
		leaveBattle();
		lockTarget = false;
#ifdef _XWL_DEBUG
		//Zebra::logger->debug("%s ����Ŀ��", name);
#endif
	}
}

/**
 * \brief ���������û�������Ʒ������ʱ��ʹ��
 * \param ct ��ǰʱ��
 */
bool SceneNpc::checkLockUserOverdue(const zRTime &ct)
{
	if (getState() == zSceneEntry::SceneEntry_Normal
			&& dwNpcLockedUser
			&& ct > lockedUserTime)
	{
		dwNpcLockedUser = 0;
		lockedUserTime = ct;
		return true;
	}
	return false;
}

/**
 * \brief ���������û�
 * \param dwID ���������û�
 */
void SceneNpc::setLockUser(const DWORD dwID)
{
    checkLockUserOverdue(SceneTimeTick::currentTime);
    if (dwID == dwNpcLockedUser && dwID != 0)
    {
	lockedUserTime = SceneTimeTick::currentTime;
	lockedUserTime.addDelay(10000);
    }
    else if(dwNpcLockedUser == 0)
    {
	dwNpcLockedUser = dwID;
	lockedUserTime = SceneTimeTick::currentTime;
	lockedUserTime.addDelay(10000);
    }
}

/**
 * \brief ���Npc�Ƿ��Ѿ���������ƶ�ʱ��
 * \param ct ��ǰʱ��
 * \return �ж��Ƿ�ɹ�
 */
bool SceneNpc::checkMoveTime(const zRTime &ct)
{
    return ct >= nextMoveTime;
}

/**
 * \brief ����Npc��һ���ƶ�ʱ��
 * \param ct ��ǰʱ��
 */
void SceneNpc::setMoveTime(const zRTime &ct)
{
    nextMoveTime = ct;
    nextMoveTime.addDelay((int)(npc->distance/speedRate));
    //Channel::sendNine(this, "�ƶ��ӳ�1 %u ����", (int)(npc->distance/speedRate));
}

/**
 * \brief ����Npc��һ���ƶ�ʱ��
 * \param ct ��ǰʱ��
 * \param delay �ӳ�ʱ�䣬����
 */
void SceneNpc::setMoveTime(const zRTime &ct, const int delay)
{
    nextMoveTime = ct;
    nextMoveTime.addDelay((int)(delay/speedRate));
    //Channel::sendNine(this, "�ƶ��ӳ�2 %u ����", (int)(delay/speedRate));
}

/**
 * \brief �ӳ��´��ƶ���ʱ��
 *
 *
 * \return 
 */
void SceneNpc::delayMoveTime(const int delay)
{
    nextMoveTime.addDelay(delay);
    if (getState()==zSceneEntry::SceneEntry_Death)
	Zebra::logger->debug("%s �ӳٸ���ʱ�� %u", name, delay);
    //Channel::sendNine(this, "�ƶ��ӳ�3 %u ����", delay);
}

/**
 * \brief ���Npc�Ƿ��Ѿ�������Թ���ʱ��
 * \param ct ��ǰʱ��
 * \return �ж��Ƿ�ɹ�
 */
bool SceneNpc::checkAttackTime(const zRTime &ct) const
{
    return ct >= nextAttackTime;
}

/**
 * \brief ����Npc��һ�ι���ʱ��
 * \param ct ��ǰʱ��
 */
void SceneNpc::setAttackTime(const zRTime &ct)
{
    //����960���벥�Ŷ���ʱ��
    nextAttackTime = ct;
    nextAttackTime.addDelay((int)(npc->adistance/aspeedRate));
#ifdef _XWL_DEBUG
    //if (!isSpecialNpc())
    //	Zebra::logger->debug("%s �´ι��� %d ����", name, (int)(npc->adistance/aspeedRate));
#endif
    //nextAttackTime.addDelay(npc->distance + 960);
}

/**
 * \brief ����Npc��һ�ι���ʱ��
 * \param ct ��ǰʱ��
 * \param delay �ӳ�ʱ�䣬����
 */
void SceneNpc::setAttackTime(const zRTime &ct, const int delay)
{
    nextAttackTime = ct;
    nextAttackTime.addDelay((int)(delay/aspeedRate));
}

/**
 * \brief �ж��Ƿ���Ե�����Ʒ
 * \param ct ��ǰʱ��
 * \return �Ƿ���Ե�����Ʒ
 */
bool SceneNpc::canLostObject(const zRTime &ct)
{
	if (lostObject)
	{
		reliveTime = SceneTimeTick::currentTime;
		reliveTime.addDelay(define->interval * 1000);
		if (NPC_TYPE_BBOSS==npc->kind || NPC_TYPE_PBOSS==npc->kind)
			Zebra::logger->debug("[BOSS]%s(%u) ����", name, tempid);
		//setMoveTime(ct, define->interval * 1000);//��������ʱ��
		//setAttackTime(ct, define->interval * 1000);
		lostObject = false;
		return true;
	}
	else
		return false;
}

/**
 * \brief �ж��Ƿ��������
 * \param ct ��ǰʱ��
 * \return �Ƿ��������
 */
bool SceneNpc::canRelive(const zRTime &ct)
{
	if (!lostObject
			&& ct > reliveTime)
	{
		//if (npc->kind==NPC_TYPE_BBOSS || npc->kind==NPC_TYPE_LBOSS || npc->kind==NPC_TYPE_PBOSS)
		//	Zebra::logger->debug("boss %s(%u) ����", name, tempid);
		setMoveTime(ct);
		setAttackTime(ct);
		notifystep=0;
		if (this->npc->kind == NPC_TYPE_BBOSS)
		{
			Cmd::stRefreshBossUserCmd send;
			strncpy(send.mapName,this->scene->getName(),sizeof(send.mapName));
			//send.x = this->getPos().x;
			//send.y = this->getPos().y;
			send.time = 0;
			send.npcid = this->npc->id;
			send.country = (BYTE)this->scene->getCountryID();
			this->forwardSession(&send, sizeof(send));
		}
		return true;
	}
	else
	{
		switch(this->npc->kind)
		{
			case NPC_TYPE_BBOSS:
				{
					int timegap=500;
					switch(notifystep)
					{
						case 0:
							timegap = 20;
							break;
						case 1:
							timegap = 10;
							break;
						case 3:
							timegap = 500;
							break;
						default:
							break;
					}
					if ((int)((reliveTime.sec()-ct.sec())/60) == timegap)
					{
						Cmd::stRefreshBossUserCmd send;
						strncpy(send.mapName,this->scene->getName(),sizeof(send.mapName));
						//send.x = this->getPos().x;
						//send.y = this->getPos().y;
						send.time = timegap;
						send.npcid = this->npc->id;
						send.country = (BYTE)this->scene->getCountryID();
						this->forwardSession(&send, sizeof(send));
						notifystep++;
					}
				}
				break;
			default:
				break;
		}
		return false;
	}
}

/**
 * \brief ������ת����Session������
 *
 * \param ptNullCmd ��ת��������
 * \param nCmdLen �����
 */
bool SceneNpc::forwardSession(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(nCmdLen > zSocket::MAX_USERDATASIZE)
	{
		Zebra::logger->debug("��ϢԽ��(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
	}
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::Session::t_Session_ForwardUser *sendCmd=(Cmd::Session::t_Session_ForwardUser *)buf;
	constructInPlace(sendCmd);
	sendCmd->dwID=0;
	sendCmd->size=nCmdLen;
	bcopy(ptNullCmd,sendCmd->data,nCmdLen);
	return sessionClient->sendCmd(buf,sizeof(Cmd::Session::t_Session_ForwardUser)+nCmdLen);
}

/**
 * \brief ��ȡNpc����
 * ��̬�Ļ��Ƕ�̬�����
 * \return Npc����
 */
const SceneNpc::SceneNpcType &SceneNpc::getSceneNpcType() const
{
    return type;
}

/**
 * \brief ���NPC�Լ�����Ϣ���ṹ��
 *
 * \param data Ҫ���Ľṹ
 */
void SceneNpc::full_t_NpcData(Cmd::t_NpcData &data)
{
    data.dwMapNpcDataPosition=tempid;
    data.dwNpcDataID=id;
    data.byDirect=dir;
    data.movespeed = (DWORD)(npc->distance/speedRate);
    //data.movespeed = (DWORD)(640/speedRate);
    //if (skillValue.movespeed!=0)
    //setSpeedRate(getSpeedRate()*640.0f/((float)data.movespeed));
    //bcopy(byState, data.byState, sizeof(byState));
    data.level = getLevel();
}
/**
 * \brief ���NPC�Լ�����Ϣ���ṹ��
 *
 * \param data Ҫ���Ľṹ
 */
void SceneNpc::full_t_MapNpcData(Cmd::t_MapNpcData &data)
{
	full_t_NpcData(*((Cmd::t_NpcData*)&data));
	full_all_UState(data.byState);
}
/**
 * \brief ���NPC�Լ�����Ϣ���ṹ��
 *
 * \param data Ҫ���Ľṹ
 */
void SceneNpc::full_t_MapNpcDataAndPos(Cmd::t_MapNpcDataPos &data)
{
	full_t_MapNpcData(*(Cmd::t_MapNpcData*)&data);
	//����
	data.x=getPos().x;
	data.y=getPos().y;
	data.byDir=getDir();
}
/**
 * \brief ���NPC�Լ�����Ϣ���ṹ��
 *
 * \param data Ҫ���Ľṹ
 */
void SceneNpc::full_t_MapNpcDataState(Cmd::t_MapNpcDataState &data)
{
	full_t_NpcData(*((Cmd::t_NpcData*)&data));
	data.num = full_UState(data.state);
}
/**
 * \brief ���NPC�Լ�����Ϣ���ṹ��
 *
 * \param data Ҫ���Ľṹ
 */
void SceneNpc::full_t_MapNpcDataAndPosState(Cmd::t_MapNpcDataPosState &data)
{
	full_t_NpcData(*((Cmd::t_NpcData*)&data));
	//����
	data.x=getPos().x;
	data.y=getPos().y;
	data.byDir=getDir();
	data.num = full_UState(data.state);
}
/**
 * \brief ���ħ��λ����Ϣ���ṹ��
 *
 */
void SceneNpc::full_stRTMagicPosUserCmd(Cmd::stRTMagicPosUserCmd &ret) const
{
    ret.pos.dwTempID=tempid;
    ret.pos.byDir=getDir();
    ret.pos.x=getPos().x;
    ret.pos.y=getPos().y;
    ret.byTarget=Cmd::MAPDATATYPE_NPC;
}

/**
 * \brief Npc����
 * \return ����ִ���Ƿ�ɹ�
 */
void SceneNpc::action(const zRTime& ctv)
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if(backOffing > 0)
	{
		backOffing -= 50;
	}

	if (_one_sec(ctv)) 
	{
		this->skillStatusM.timer();
		//TODOֻ���ڷ����仯ʱ�ŷ���
		if(hp != this->getMaxHP() && getState() == zSceneEntry::SceneEntry_Normal)
		{
			sendtoSelectedHpAndMp();
		}
	}

	if (AIC) AIC->processPhase();

	switch(getState())
	{
		case zSceneEntry::SceneEntry_Normal:
			//��ͨ��Npc
			normalAction();
			break;
		case zSceneEntry::SceneEntry_Death:
			//������Npc
			{
				deathAction();
			}
			break;
		case zSceneEntry::SceneEntry_Hide:
			hideAction();
			//���ص�Npc
			//do nothing
			break;
	}
}

/**
 * \brief ������Npc��AI��������
 * \param ctv ��ǰʱ��
 * \param affectNpc Ҫ�����npc�б�
 */
void SceneNpc::AI(const zRTime& ctv, MonkeyNpcs &affectNpc,const DWORD group , const bool every)
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	MonkeyNpcs::iterator oldit;
	for(MonkeyNpcs::iterator it=affectNpc.begin();it!=affectNpc.end();)
	{
		/// ������ڵ�ǰ���鴦���Ҳ���Ҫȫ�������ʱ��,�ͷ�������ѭ���Ը�npc�Ĵ���
		if (every || ((*it)->tempid % MAX_NPC_GROUP == group))
		{
			if ((*it)->needClear())
			{
				oldit = it;
				it++;
				SceneNpc * delNpc = *oldit;
				affectNpc.erase(oldit);
				Scene *mScene = delNpc->scene;

				//֪ͨ�ͻ���ɾ��NPC
				Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
				removeNpc.dwMapNpcDataPosition = delNpc->tempid;
				mScene->sendCmdToNine(delNpc->getPosI(), &removeNpc, sizeof(removeNpc));
				//�ӵ�ͼ�͹�������ɾ�����NPC
				mScene->removeNpc(delNpc);
				SceneNpcManager::getMe().removeSceneNpc(delNpc);
				//SceneNpcManager::getMe().removeSpecialNpc(delNpc); //�Ѿ���ɾ���ˣ�û�б�Ҫ�ظ�����
				//Zebra::logger->trace("%s, ɾ��npc %s(%u)", __FUNCTION__, delNpc->name, delNpc->tempid);
				SAFE_DELETE(delNpc);
			}
			else
			{
				(*it)->action(ctv);
				it++;
			}
		}
		else
		{
			it++;
		}
	}
}

/**
 * \brief �������ڸ��ٵĵ���
 *
 * \param entry ������ҵ��Ķ���
 * \return �Ƿ���ҳɹ�
 */
bool SceneNpc::checkChaseAttackTarget(SceneEntryPk *&entry)
{
	if (getPetType()!=Cmd::PET_TYPE_NOTPET && getPetType()!=Cmd::PET_TYPE_SEMI)
		((ScenePet *)this)->checkMasterTarget(entry);
	if (entry) return true;

	if (curTargetID!=0 || defTargetID!=0)
	{
		entry = getChaseSceneEntry();
		SceneEntryPk * dt = 0;
		bool switchTarget = false;
		if (getPetType()==Cmd::PET_TYPE_NOTPET || getPetType()==Cmd::PET_TYPE_SEMI)
		{
			dt = getDefTarget();
			if (!entry || (dt && zMisc::selectByPercent(1)))
			{
				entry = dt;
				switchTarget = true;
			}
		}
		if (NULL == entry//�Ҳ���Ŀ�꣬���߻��߻���ͼ
				|| entry->getState() != zSceneEntry::SceneEntry_Normal//���������
				|| (!scene->zPosShortRange(getPos(), entry->getPos(), npc_lost_target_region) && !lockTarget)
				|| !canReach(entry) //��ʹ��ǰ��Ҳ��������
				//|| ((master==NULL) ?false:!scene->zPosShortRange(getPos(), master->getPos(), npc_chase_region+1))//�������ٷ�Χ
				|| (NPC_AI_PATROL==AIDefine.type&&!entry->isRedNamed())
				|| (!(canFight()))
				|| entry->hideme)	//����ս��
		{
#ifdef _XWL_DEBUG
			//if (entry && !scene->zPosShortRange(getPos(), entry->getPos(), npc_lost_target_region))
			//Zebra::logger->debug("%s->%s ���볬�� %u ��", name, entry->name, npc_lost_target_region);
#endif
			if (!chaseSecondTarget()&&!chaseItsMaster())
				unChaseUser();//����׷��
		}
		else
		{
			if (1==isEnemy(entry))
			{
#ifdef _XWL_DEBUG
				//Channel::sendNine(this, "%s ", entry->name);
#endif
				if (switchTarget)
				{
					setCurTarget(entry, true);
					clearDefTarget();
#ifdef _XWL_DEBUG
					//Zebra::logger->debug("%s �л���������Ŀ�� %s", name, entry->name);
#endif
				}
				return true;
			}
			else
			{
				unChaseUser();//����׷��
			}
		}
	}

	entry = NULL;
	return false;
}


/**
 * \brief �жϵ����Ƿ��ڹ�����Χ��
 *
 *
 * \param entry ����ָ��
 * \return �Ƿ��ڹ�����Χ��
 */
bool SceneNpc::inRange(SceneEntryPk * entry)
{
	BYTE atype = 0;
	BYTE action = 0;
	npc->getATypeAndAction(atype,action);

	switch(atype)
	{
		case NPC_ATYPE_NEAR:	/// �����빥��
		case NPC_ATYPE_MNEAR:	/// �����뷨������
			if (!scene->zPosShortRange(getPos(), entry->getPos(), 1))
			{
				//Zebra::logger->debug("�����빥�����ɹ�");
				return false;
			}
			break;
		case NPC_ATYPE_MFAR:	/// ����Զ���빥��
		case NPC_ATYPE_FAR:		/// Զ���빥��
			if (!scene->zPosShortRange(getPos(), entry->getPos(), 6))
			{
				if (getPetType()!=Cmd::PET_TYPE_NOTPET && scene->zPosShortRange(getPos(), entry->getPos(), 10))
					return true;

				return false;
			}
			else
			{
				if ((Cmd::PET_TYPE_NOTPET==getPetType())&&
						(!scene->zPosShortRange(getPos(), entry->getPos(), 1))&&
						zMisc::selectByPercent(1)) return false;
			}
			break;
		default:
			return false;
	}
	return true;
}

/**
 * \brief �ж��ƶ�������Ƿ��ڹ�����Χ��
 *
 *
 * \param entry ����ָ��
 * \return �Ƿ��ڹ�����Χ��
 */
bool SceneNpc::canReach(SceneEntryPk *entry)
{
	//ֻ�г����ж������Χ
	switch (getPetType())
	{
		case Cmd::PET_TYPE_NOTPET:
		case Cmd::PET_TYPE_SEMI:
		case Cmd::PET_TYPE_TOTEM:
			{
				switch (this->npc->kind)
				{
					case NPC_TYPE_TRAP:
						break;
					default:
						{
							return true;
						}
						break;
				}
			}
			break;
		default:
			break;
	}
	if (!entry) return false;

	zPos c(0,0);
	int x=0, y=0;
	AIC->getActRegion(c, x, y);
	/*
	   if (getPetType()!=Cmd::PET_TYPE_NOTPET)
	   {
	   x += 10;
	   y += 10;
	   }
	   else
	   */
	{
		x = 7;//�������7
		y = 7;
	}

	//����Χ���޷��߶�
	if (!canMove() || isSurrounded())
	{
		c = getPos();
		x = 0;
		y = 0;
	}

	int atkRange = 0;
	BYTE atype=0, action=0;
	npc->getATypeAndAction(atype,action);
	switch(atype)
	{
		case NPC_ATYPE_NEAR:	/// �����빥��
		case NPC_ATYPE_MNEAR:	/// ���������빥��
			atkRange = 1;
			break;
		case NPC_ATYPE_MFAR:	/// ����Զ���빥��
		case NPC_ATYPE_FAR:	/// Զ���빥��
			atkRange = 6;
			break;
		default:
			return false;
	}

	x += atkRange;
	y += atkRange;

	const zPos &pos = entry->getPos();
	return (pos.x>=c.x-x)
		&&(pos.x<=c.x+x)
		&&(pos.y>=c.y-y)
		&&(pos.y<=c.y+y);
}

/**
 * \brief �ж��Ƿ���Թ����ö���
 *
 *
 * \param entry ����
 * \return �Ƿ���Թ���
 */
bool SceneNpc::canAttack(SceneEntryPk *entry)
{
	//���Npc�Ƿ���Թ���
	if (!checkAttackTime(SceneTimeTick::currentTime))// || !zMisc::selectByPercent(50))
		return false;

	if (!attackAction) return false; //����������׷��
	return true;
}

/**
 * \brief Npc�������
 * \param entry ���
 * \return �����Ƿ�ɹ�
 */
bool SceneNpc::attackTarget(SceneEntryPk *entry)
{
	if (this->assault)
	{
		this->skillStatusM.clearRecoveryElement(121);
	}
	if (!canAttack(entry)) return false;

	if (entry->getTopMaster() &&
			entry->getTopMaster()->getType() == zSceneEntry::SceneEntry_Player &&
			this->getTopMaster() &&
			this->getTopMaster()->getType() == zSceneEntry::SceneEntry_Player)
	{
		if (!(this->isPkZone(entry) && entry->isPkZone(this))) // �¼�&&this->isPkZone(pDef)
		{
			return false;
		}
	}

	BYTE atype = 0;
	BYTE action = 0;
	npc->getATypeAndAction(atype,action);

	//�ɹ������û�
	if (entry && this->npc->kind != NPC_TYPE_SURFACE) 
	{
		setDir(getPos().getDirect(entry->getPos()));
#ifdef _XWL_DEBUG
		//if (getPetType()!=Cmd::PET_TYPE_NOTPET)
		//	Zebra::logger->debug("%s ���÷��� %s", name, entry->name);
#endif
	}

	using namespace Cmd;
	stAttackMagicUserCmd att;
	att.dwUserTempID = tempid;
	if (entry) att.dwDefenceTempID = entry->tempid;

	switch(atype)
	{
		case NPC_ATYPE_NEAR:	/// �����빥��
			{
				if (npc->skill)
					att.wdMagicType = npc->skill;
				else
					att.wdMagicType = SKILLNORMAL;
				att.byAction = Ani_Attack;
			}
			break;
		case NPC_ATYPE_FAR:		/// Զ���빥��
			{
				if (npc->skill)
					att.wdMagicType = npc->skill;
				else
					att.wdMagicType = NORMALBOW;
				//att.wdMagicType = SKILLNORMAL;
				att.byAction = Ani_Attack;
				//att.byAction = Ani_Attack;
			}
			break;
		case NPC_ATYPE_MFAR:
		case NPC_ATYPE_MNEAR:
			{
				if (!this->skillAction) return false;
				att.wdMagicType = npc->skill;
				att.byAction = 0;
			}
			break;
		default:
			break;
	}
	//att.byAction = action;
	/*
	*/

	if (entry)
	{
		att.xDes = (WORD)entry->getPos().x;
		att.yDes = (WORD)entry->getPos().y;
		att.byDirect = getDir();
		switch (entry->getType())
		{
			case zSceneEntry::SceneEntry_Player:
				{
					att.byAttackType = ATTACKTYPE_N2U;
				}
				break;
			case zSceneEntry::SceneEntry_NPC:
				{
					att.byAttackType = ATTACKTYPE_N2N;
				}
				break;
			default:
				{
					att.byAttackType = ATTACKTYPE_N2U;
				}
				break;
		}

	}
	else
	{
		att.xDes = (WORD)getPos().x;
		att.yDes = (WORD)getPos().y;
		att.byDirect = getDir();
	}
	//scene->sendCmdToNine(getPosI(), &att, sizeof(att), false);

	this->skillValue.init();
	this->skillStatusM.processPassiveness();// �����ҵı���״̬Ӱ��

	//if (zMisc::selectByPercent(this->npc->rating))
	{
		//ScenePk::physicalMagicN2U(this, sceneUser);

		switch(atype)
		{
			case NPC_ATYPE_NEAR:	/// �����빥��
			case NPC_ATYPE_FAR:		/// Զ���빥��
				{
					if (entry)
					{
						if (this->checkMagicFlyRoute(entry,(atype==2||atype==3)?AttackFly:AttackNear))
						{
							if (entry->preAttackMe(this,&att))
							{
								scene->sendCmdToNine(getPosI(), &att, sizeof(att));
								entry->AttackMe(this,&att);
							}
						}
					}
				}
				break;
			case NPC_ATYPE_MFAR:
			case NPC_ATYPE_MNEAR:
				{
					//��ħ������
					if (entry)
					{
						if (this->checkMagicFlyRoute(entry,(atype==2||atype==3)?AttackFly:AttackNear))
						{
							if (entry->preAttackMe(this,&att,false))
							{
								scene->sendCmdToNine(getPosI(), &att, sizeof(att));
								entry->AttackMe(this,&att,false);
							}
						}
						/*
						   else
						   {
						   if (getPetType()==Cmd::PET_TYPE_NOTPET)
						   scene->sendCmdToNine(getPosI(), &att, sizeof(att), false);
						   }
						   */
					}

					/*
					   zSkill *s = this->usm.findSkill(att.wdMagicType);
					   if(s)
					   {
					   if (skillAction)
					   {
					   if (s->action(&att))
					   {
					   break;
					   }
					   }
					   }
					   if (entry)
					   {
					   entry->preAttackMe(this,NULL);
					   entry->AttackMe(this,NULL);
					   }
					   */
					//sceneUser->preAttackMe(this,NULL,false,npc->five);
					//sceneUser->AttackMe(this,NULL,false);
				}
				break;
			default:
				break;
		}

		if (entry)
		{
			if (this->giddy>0) //ŭ��ѵ������
			{
				if (zMisc::selectByPercent(giddy))
				{
					Cmd::stAttackMagicUserCmd cmd;

					if (entry->getType() == zSceneEntry::SceneEntry_Player)
					{
						cmd.dwDefenceTempID = entry->tempid;
						cmd.byAttackType = Cmd::ATTACKTYPE_N2U;
						cmd.byAction = Cmd::Ani_Null;
					}
					else
					{
						cmd.dwDefenceTempID = entry->tempid;
						cmd.byAttackType = Cmd::ATTACKTYPE_N2N;
						cmd.byAction = Cmd::Ani_Null;
					}

					cmd.dwUserTempID = this->tempid;
					cmd.wdMagicType = 386;
					cmd.byDirect = this->getDir();

					zSkill *s = NULL;

					s = zSkill::createTempSkill(this , 386 , 1);
					if(s)
					{
						s->action(&cmd, sizeof(cmd));
						SAFE_DELETE(s);
					}
				}
			}



			if (entry->getType() == zSceneEntry::SceneEntry_Player) ((SceneUser *)entry)->mask.on_defence();
		}
	}

	SceneEntryPk *pMaster = this->getTopMaster();
	if (pMaster&&pMaster->getType() == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = (SceneUser *)pMaster;
		pUser->packs.equip.costAttackDurByPet(pUser);
	}

	//������һ�ι�����ʱ��
	setAttackTime(SceneTimeTick::currentTime);
	if (nextMoveTime>SceneTimeTick::currentTime)
		delayMoveTime(720);
	else
		setMoveTime(SceneTimeTick::currentTime, 720);
	setEndBattleTime(SceneTimeTick::currentTime, 10*1000);

	return true;
}

#include "RebuildObject.h"

bool is_boss(int type)
{
	switch (type)
	{
		case NPC_TYPE_BACKBONE:
		case NPC_TYPE_BBOSS:
		case NPC_TYPE_LBOSS:
		case NPC_TYPE_GOLD:
		case NPC_TYPE_PBOSS:
		case NPC_TYPE_DUCKHIT:
		case NPC_TYPE_RESOURCE:
			return true;
			break;	
		default:
			break;
	}
	return false;
}

int drop_odds(WORD player_level, WORD npc_level, DWORD npc_kind)
{
	int odds = 100;

	if (is_boss(npc_kind))
	{
		if (player_level>npc_level+20)
		{
			DWORD sub = (player_level-(npc_level+20))*10;
			odds = (sub>=100)?0:(100-sub);
		}
	}
	else
	{
		int diff = player_level - npc_level;
		if(diff >= 6 && diff < 10)
		{
			odds = 50;
		}
		else if(diff >= 10)
		{
			odds = 0;
		}
	}
	if (odds>100) odds=100;
	if (npc_kind==NPC_TYPE_DUCKHIT || npc_kind==NPC_TYPE_RESOURCE) odds = 100;
#ifdef _XWL_DEBUG
	Zebra::logger->debug("������� %u", 100-odds);
#endif
	return odds;
}

/**
 * \brief ����Npc�Ķ���
 * \return ����ִ���Ƿ�ɹ�
 */
bool SceneNpc::deathAction()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if (canLostObject(SceneTimeTick::currentTime))
	{
		//Zebra::logger->debug("%s ����", name);
		int value=1;
		int value1=0;
		int value2=0;
		int player_level = 0;
		int vcharm = 0;
		int vlucky = 0;
		SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
		if (pUser)
		{

			DWORD leader_id = pUser->team.getLeader();
			SceneUser* leader = SceneUserManager::getMe().getUserByTempID(leader_id);
			if (leader) {
				Team& team = const_cast<Team&>(leader->team.getTeam());

				team.rwlock.rdlock();
				std::vector<TeamMember>::iterator it = team.member.begin();;
				for(; it!=team.member.end(); ++it) {
					SceneUser* member = SceneUserManager::getMe().getUserByTempID(it->tempid);
					if (member && (member->charbase.level < 20 || 
								scene->checkTwoPosIInNine(pUser->getPosI() , member->getPosI()) ||
								scene->checkTwoPosIInNine(this->getPosI() , member->getPosI()) )) {
						//moved from attackDeathNpc by lqy, avoid unfair bonus
						OnKill event(id) ;
						EventTable::instance().execute(*member, event);
						OnKillByLevel e(npc->level);
						EventTable::instance().execute(*member, e);
						if (dwNpcLockedUser == member->id)
						{
							OnKillBySelf es(id);
							EventTable::instance().execute(*member, es);
						}

///						if (ScriptQuest::get_instance().has(ScriptQuest::NPC_KILL, npc->id)) { 
//							char func_name[32];
//							sprintf(func_name, "%s_%d", "kill", npc->id);
//						}
					}
				}
				team.rwlock.unlock();
			}else {
				OnKill event(id) ;
				EventTable::instance().execute(*pUser, event);
				OnKillByLevel e(npc->level);
				EventTable::instance().execute(*pUser, e);
				OnKillBySelf es(id);
				EventTable::instance().execute(*pUser, es);

//				if (ScriptQuest::get_instance().has(ScriptQuest::NPC_KILL, npc->id)) { 
//					char func_name[32];
//					sprintf(func_name, "%s_%d", "kill", npc->id);
//				}
			}

			value1=pUser->packs.equip.getEquips().get_mf();
			value2=pUser->packs.equip.getEquips().get_incgold();
			vcharm = pUser->charstate.charm;
			vlucky = pUser->charstate.lucky;
			player_level = pUser->charbase.level;
		}

		NpcLostObject nlo;
		if (npc->kind == NPC_TYPE_BBOSS && zMisc::selectByPercent(50))
		{
			npc->nco.lostGreen(nlo, value, value1, value2, vcharm, vlucky);
		}
		else if (zMisc::selectByOneHM(100))
		{
			npc->nco.lostAll(nlo);
		}
		else
		{
			npc->nco.lost(nlo, value, value1, value2, vcharm, vlucky, player_level, ScenesService::getInstance().double_drop, ScenesService::getInstance().levelDoubleDrop);
		}

		for(NpcLostObject::const_iterator it = nlo.begin(); it != nlo.end(); it++)
		{
			if (!((npc->id>=21501 && npc->id<=21511) ||
				  (npc->id>=26501 && npc->id<=26511) ||
				  (npc->id>=30051 && npc->id<=30102)))
			{
				if (!zMisc::selectByPercent(drop_odds(player_level, npc->level, npc->kind))) continue;
			}

			zObjectB *ob = objectbm.get((*it).id);
			if (ob)
			{
				int num = (*it).minnum;
				if ((*it).minnum != (*it).maxnum)
					num = zMisc::randBetween((*it).minnum, (*it).maxnum);
				if (num > 0)
				{
					if(ob->id == 665)
					{
						//��������
						distributeMoney(num);
					}
					else
					{
						SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
						if(pUser && pUser->team.getLeader()&& this->npc->kind != NPC_TYPE_RESOURCE)
						{
							SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
							if(leader && leader->team.isSpecialObj())
							{
								scene->addObject(ob, num, getPos(), leader->team.getNextObjOwnerID(), id);
							}
							else
							{
								scene->addObject(ob, num, getPos(), dwNpcLockedUser, id);
							}
						}
						else
						{
							if (this->npc->kind == NPC_TYPE_RESOURCE && pUser)
							{
								zObject *o = zObject::create(ob,num);
								if (o)
								{
									DWORD addnum=o->data.dwNum;
									Combination callback(pUser, o);
									pUser->packs.main.execEvery(callback);
									if (pUser->packs.equip.pack(EquipPack::L_PACK) && pUser->packs.equip.pack(EquipPack::L_PACK)->can_input()) pUser->packs.equip.pack(EquipPack::L_PACK)->execEvery(callback);
									if (pUser->packs.equip.pack(EquipPack::R_PACK) && pUser->packs.equip.pack(EquipPack::R_PACK)->can_input()) pUser->packs.equip.pack(EquipPack::R_PACK)->execEvery(callback);

									bool added = false;
									if (o->data.dwNum)
									{
										if (pUser->packs.uom.space(pUser) >= 1 && pUser->packs.addObject(o, true, AUTO_PACK))
										{
											//�����˫��������ߺ�����������Ҫ��
											if(o->base->kind == ItemType_DoubleExp || o->base->kind == ItemType_Honor || o->base->kind == ItemType_ClearProperty)
											{
												o->data.bind=1;
											}
											added = true;
											Cmd::stAddObjectPropertyUserCmd status;
											status.byActionType = Cmd::EQUIPACTION_OBTAIN;
											bcopy(&o->data, &status.object, sizeof(t_Object));
											pUser->sendCmdToMe(&status, sizeof(status));
										}
										else
										{
											addnum -= o->data.dwNum;
											Channel::sendSys(pUser , Cmd::INFO_TYPE_FAIL, "��İ�������");
											scene->addObject(ob, o->data.dwNum, getPos(), dwNpcLockedUser, id);



											if (callback.num() || added)
											{
												zObject::logger(o->createid,o->data.qwThisID,o->data.strName,addnum,addnum,1,this->scene->id,pUser->scene->name,pUser->id,pUser->name,"�ɼ��õ�",o->base,o->data.kind,o->data.upgrade);
												OnGet event(o->data.dwObjectID);
												EventTable::instance().execute(*pUser, event);
//												if (ScriptQuest::get_instance().has(ScriptQuest::OBJ_GET, o->data.dwObjectID))
//												{ 
//													char func_name[32];
//													sprintf(func_name, "%s_%d", "get", o->data.dwObjectID);
//												}								
											}
										}
									}
									if (!o->data.dwNum || !added)
									{
										zObject::destroy(o);
									}
								}
							}
							else
							{
								scene->addObject(ob, num, getPos(), dwNpcLockedUser, id);
							}
						}
					}
				}
			}
		}
		//���ǵ���
		if (zMisc::/*selectByPercent*//*Ҫ��ĳ����֮����*/selectByTenTh(npc->soulrate) && zMisc::selectByPercent(drop_odds(player_level, npc->level, npc->kind)) ) {

			//zObjectB *base = objectbm.get(SoulStone::id());
			zObjectB *base = objectbm.get(SoulStone::id(npc->trait));
			if (!base) return false;

			zObject *ob = zObject::create(base, 1);
			if (ob) {
				SoulStone::assign(ob, npc->trait);
				scene->addObject(ob, getPos(), false , dwNpcLockedUser);
				zObject::logger(ob->createid,ob->data.qwThisID,ob->data.strName,ob->data.dwNum,ob->data.dwNum,1,0,NULL,scene->id,scene->name,"npc_lost",ob->base,ob->data.kind,ob->data.upgrade);
			}
		}

		//��������
		if (canRush())
			createRush();

	}
	else if (canRelive(SceneTimeTick::currentTime))
	{
		zPos pos;

		if (getSceneNpcType() == SceneNpc::GANG)
		{
			if (!isRushNpc)
				setClearState();
			//Zebra::logger->debug("��Ƕ�̬npc %s", name);
			//��̬�����Npc��Ҫ������Դ
			//Zebra::logger->debug("TODO ��̬Npc����Ҫɾ��");
			/*TODO ��̬Npc����Ҫɾ��
			  scene->removeNpc(sceneNpc, false);
			  SceneNpcManager::getMe().removeSceneNpc(sceneNpc);
			  SAFE_DELETE(sceneNpc);
			// */
			//����С�ģ�����ڴ��Ѿ��ͷ��ˣ���֮������ʹ�����������
		}
		else if (scene->randPosByRegion(define->region.index, pos))
		{
			//����
			isUse = false; // ��ʬ��ʹ��״̬ϴ����
			clearUState(Cmd::USTATE_DEATH);
			Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
			removeNpc.dwMapNpcDataPosition = tempid;
			scene->sendCmdToNine(getPosI(), &removeNpc, sizeof(removeNpc));

			hp = this->getMaxHP();
			recoverUnder30 = false;
			//��ͷ��ʼ����·���ƶ�
			if (AIC) AIC->on_relive();

			if (define->initstate == zSceneEntry::SceneEntry_Normal)
			{
				//���ҷ��赲��ɹ�
				scene->setBlock(pos);
				setState(zSceneEntry::SceneEntry_Normal);
			}
			else
				setState(zSceneEntry::SceneEntry_Hide);

			if (scene->refresh(this, pos))
			{
				if (getState() == zSceneEntry::SceneEntry_Normal)
				{
					this->sendMeToNine();
				}
				//�ٻ�npc�ĳ���
				if (!define->petList.empty())
				{
					for (std::map<DWORD, std::pair<DWORD, DWORD> >::const_iterator it=define->petList.begin(); it!=define->petList.end(); it++)
					{
						for (DWORD i=0; i<it->second.first; i++)
						{
							if (zMisc::selectByPercent(it->second.second))
							{
								ScenePet * pet = summonPet(it->first, Cmd::PET_TYPE_TOTEM, 0, 0, 0, 0);
								if (pet)
									pet->setPetAI(Cmd::PETAI_ATK_ACTIVE);
							}
						}
					}
				}
				//�����
				if (!define->dieList.empty())
				{
					for (std::list< std::pair<DWORD,zPos> >::const_iterator it=define->dieList.begin(); it!=define->dieList.end(); it++)
					{
						ScenePet * pet = summonPet(it->first, Cmd::PET_TYPE_SEMI, 0, 0, 0, 0, it->second);
						if (pet)
						{
							pet->setPetAI(Cmd::PETAI_ATK_ACTIVE);
							Zebra::logger->debug("%s �ٻ����� %s", name, pet->name);
						}
					}
				}

				if (npc->kind==NPC_TYPE_BBOSS || npc->kind==NPC_TYPE_LBOSS || npc->kind==NPC_TYPE_PBOSS)
					Zebra::logger->debug("[BOSS]%s(%u) ���� pos=(%u,%u)", name, tempid, pos.x, pos.y);
			}
			else
			{
				setState(zSceneEntry::SceneEntry_Death);
				Zebra::logger->debug("[BOSS]%s(%u) ����ʧ�� pos=(%u,%u)", name, tempid, pos.x, pos.y);
			}

			if (this->id == COUNTRY_MAIN_FLAG)
			{
				CountryDareM::getMe().reliveSecondFlag(this->scene, COUNTRY_SEC_FLAG);
			}

			if (this->id == COUNTRY_KING_MAIN_FLAG)
			{
				CountryDareM::getMe().reliveSecondFlag(this->scene, COUNTRY_KING_SEC_FLAG);
			}


		}
	}

	return true;
}

/*
 * \brief ����npc
 *
 * \showDelay ���ص�ʱ��
 */
void SceneNpc::hideMe(int showDelay)
{
    Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
    removeNpc.dwMapNpcDataPosition = tempid;
    scene->sendCmdToNine(getPosI(), &removeNpc, sizeof(removeNpc));

    setState(zSceneEntry::SceneEntry_Hide);
    showTime = SceneTimeTick::currentTime;
    showTime.addDelay(showDelay);
#ifdef _XWL_DEBUG
    Zebra::logger->debug("npc %s ����%d����", npc->name, showDelay);
#endif
}

/*
 * \brief npc����״̬����Ϊ
 *
 * \return �Ƿ�ִ�гɹ�
 */
bool SceneNpc::hideAction()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
    if (showTime<=SceneTimeTick::currentTime)
	{
		setState(zSceneEntry::SceneEntry_Normal);
		scene->setBlock(getPos());
		sendMeToNine();
#ifdef _XWL_DEBUG
		Zebra::logger->debug("npc %s ��������", name);
#endif
	}
    return true;
}

/**
 * \brief Ѱ·�������ж��м���Ƿ�ɴ�Ŀ�ĵ�
 * \param tempPos Ѱ·���̵��м��
 * \param destPos Ŀ�ĵ�����
 * \param radius Ѱ·��Χ��������Χ����ΪĿ�ĵز��ɴ�
 * \return �����Ƿ�ɵ���Ŀ�ĵ�
 */
bool SceneNpc::moveable(const zPos &tempPos, const zPos &destPos,  int radius)
{
	return (scene->zPosShortRange(tempPos, destPos, radius)
			&& (!scene->checkBlock(tempPos) //Ŀ���ɴ����������Ŀ���
				|| tempPos == destPos));
}

/**
 * \brief Npc��ĳһ�������ƶ�
 * \param direct ����
 * \param step �ƶ��Ĳ���
 * \return �ƶ��Ƿ�ɹ�
 */
bool SceneNpc::move(const int direct, const int step)
{
	if (!canMove()) return false;
	//if ((aif&AIF_NO_MOVE)||(petAI&Cmd::PETAI_MOVE_STAND)) return false;

	const int walk_adjust[9][2]= { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, 0} };

	if(backOffing)
	{
		return true;
	}
	//Zebra::logger->debug("�ƶ�������%u", step);
	lastPos2 = lastPos1;
	lastPos1 = pos;
	zPosI oldPosI = getPosI();
	zPos oldPos = getPos(),  newPos = getPos();
	newPos.x += (step*walk_adjust[direct][0]);
	newPos.y += (step*walk_adjust[direct][1]);
	//���Ŀ����赲�Լ��Ƿ�Ϸ�
	if (scene->checkBlock(newPos))
	{
		//Zebra::logger->debug("%u, %u, %s, %u, %u, %u", id, tempid, name, direct, getPos().x, getPos().y);
		return false;
	}
	if (scene->refresh(this, newPos))
	{
		if (getPetType()!=Cmd::PET_TYPE_CARTOON && getPetType()!=Cmd::PET_TYPE_RIDE)
			scene->setBlock(newPos);
		scene->clearBlock(oldPos);
		setDir(direct);

		Cmd::stNpcMoveMoveUserCmd cmd;
		cmd.dwNpcTempID = tempid;
		cmd.byDirect = direct;
		cmd.bySpeed = step;
		if (Cmd::PET_TYPE_RIDE==getPetType()&&speedRate>1.5)
		{
			cmd.bySpeed = 3;
#ifdef _XWL_DEBUG
			//Zebra::logger->debug("%s �ƶ����� 2", name);
#endif
		}
		cmd.x = newPos.x;
		cmd.y = newPos.y;
		scene->sendCmdToNine(oldPosI, &cmd, sizeof(cmd));

		if (oldPosI != getPosI())
		{
			Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
			removeNpc.dwMapNpcDataPosition = tempid;
			scene->sendCmdToReverseDirect(oldPosI,
					scene->getScreenDirect(oldPosI,getPosI()), &removeNpc, sizeof(removeNpc));
			BUFFER_CMD(Cmd::stAddMapNpcMapScreenStateUserCmd ,send , zSocket::MAX_USERDATASIZE);
			this->full_t_MapNpcDataState(send->data);
			scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), send,send->size());
			Cmd::stRTMagicPosUserCmd ret;
			full_stRTMagicPosUserCmd(ret);
			scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), &ret, sizeof(ret));
			attackRTHpAndMp();
		}
	}

	//������һ���ƶ�ʱ��
	setMoveTime(SceneTimeTick::currentTime);
	//���´ι���ʱ����ɵ�Ӱ�죺��ͨ���ƶ�ʱ�䣻 ����ƶ�ʱ���һ��
	if (getPetType()==Cmd::PET_TYPE_NOTPET || getPetType()==Cmd::PET_TYPE_SEMI)
	{
		if (nextAttackTime>SceneTimeTick::currentTime)
			nextAttackTime.addDelay((int)(npc->distance/speedRate));
		else
			setAttackTime(SceneTimeTick::currentTime);
	}
	else
	{
		if (nextAttackTime>SceneTimeTick::currentTime)
			nextAttackTime.addDelay((int)(npc->distance/speedRate/2));
		else
			setAttackTime(SceneTimeTick::currentTime, (int)(npc->distance/speedRate/2));
	}

	return true;
}

/**
 * \brief ˲���ƶ�
 *
 *
 * \param newPos Ŀ��λ��
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::warp(const zPos &newPos,bool ignore)
{
    if (!ignore)
	if (!canMove()) return false;

    zPosI oldPosI = getPosI();
    zPos oldPos = getPos();

    zPos findedPos;
    bool founded = scene->findPosForUser(newPos, findedPos);
    if(scene->refresh(this,founded ? findedPos : newPos))
	{
		if (SceneEntry_Normal==getState() && getPetType()!=Cmd::PET_TYPE_CARTOON && getPetType()!=Cmd::PET_TYPE_RIDE)
			scene->setBlock(newPos);
		scene->clearBlock(oldPos);


		Cmd::stRTMagicPosUserCmd ret;
		full_stRTMagicPosUserCmd(ret);
		if (oldPosI != getPosI())
		{
			Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
			removeNpc.dwMapNpcDataPosition = tempid;
			Cmd::stAddMapNpcMapScreenUserCmd addNpc;
			full_t_MapNpcData(addNpc.data);
			if(scene->checkTwoPosIInNine(oldPosI,getPosI()))
			{
				scene->sendCmdToReverseDirect(oldPosI,scene->getScreenDirect(oldPosI,getPosI()),&removeNpc,sizeof(removeNpc));
				scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), &addNpc, sizeof(addNpc));
				//scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), &ret, sizeof(ret));
			}
			else
			{
				scene->sendCmdToNine(oldPosI,&removeNpc, sizeof(removeNpc));
				sendMeToNine();
			}
		}
		scene->sendCmdToNine(getPosI(), &ret, sizeof(ret));
		//setMoveTime(SceneTimeTick::currentTime);
		return true;
	}
    else return false;
}

/**
 * \brief ʹ�����ĳһ�����ƶ�
 * ��Ѱ·�㷨���ƶ�����������ֻ�Ƿ�װ��һ�£����ڲ�ͬ��npc������ͬ����������ò�ͬ������A*�㷨
 * \param srcPos �������
 * \param destPos Ŀ�ĵ�����
 * \return �ƶ��Ƿ�ɹ�
 */
bool SceneNpc::gotoFindPath(const zPos &srcPos, const zPos &destPos)
{
    if(id==27 || id==28 || id==29)
	//������һ�����������Բ���Ϊ2
	return zAStar<2>::gotoFindPath(srcPos, destPos);
    else
	//��ͨ����һ��ֻ����һ�������Բ���Ϊ1
	return zAStar<>::gotoFindPath(srcPos, destPos);
}

/**
 * \brief Npc��ĳһ�����ƶ�
 * ����Ѱ·�㷨���ƶ�����������ֻ�Ƿ�װ��һ�£����ڲ�ͬ��npc������ͬ����������ò�ͬ������A*�㷨
 * \param pos Ŀ�ĵ�����
 * \return �ƶ��Ƿ�ɹ�
 */
bool SceneNpc::goTo(const zPos &pos)
{
    if(id==27 || id==28 || id==29)
	//������һ�����������Բ���Ϊ2
	return zAStar<2>::goTo(getPos(), pos);
    else
	//��ͨ����һ��ֻ����һ�������Բ���Ϊ1
	return zAStar<>::goTo(getPos(), pos);
}

/**
 * \brief Npc�����ĳһ�������ƶ�
 * ��������ֻ�Ƿ�װ��һ�£����ڲ�ͬ��npc������ͬ����������ò�ͬ������A*�㷨
 * \param direct �������
 * \return �ƶ��Ƿ�ɹ�
 */
bool SceneNpc::shiftMove(const int direct)
{
    if(id==27 || id==28 || id==29)
	//������һ�����������Բ���Ϊ2
	return zAStar<2>::shiftMove(direct);
    else
	//��ͨ����һ��ֻ����һ�������Բ���Ϊ1
	return zAStar<>::shiftMove(direct);
}

void SceneNpc::clearStateToNine(WORD state)
{
	if(!scene) return;
	Cmd::stClearStateMapScreenUserCmd send;
	send.type=Cmd::MAPDATATYPE_NPC;
	send.dwTempID = this->tempid;
	send.wdState =state;
	if (npc->kind == NPC_TYPE_TRAP)
	{
		SceneEntryPk *entry = getMaster();
		if (entry && entry->getType() == zSceneEntry::SceneEntry_Player)
		{
			((SceneUser*)entry)->sendCmdToMe(&send, sizeof(send));
		}		
	}
	else
	{
		this->scene->sendCmdToNine(getPosI(),&send,sizeof(send));
	}
}
/**
 * \brief ����ĳ��״̬���û�
 *
 * \author zjw
 */

void SceneNpc::setStateToNine(WORD state)
{
	Cmd::stSetStateMapScreenUserCmd send;
	send.type=Cmd::MAPDATATYPE_NPC;
	send.dwTempID = this->tempid;
	send.wdState =state;
	if (npc->kind == NPC_TYPE_TRAP)
	{
		SceneEntryPk *entry = getMaster();
		if (entry && entry->getType() == zSceneEntry::SceneEntry_Player)
		{
			((SceneUser*)entry)->sendCmdToMe(&send, sizeof(send));
		}		
	}
	else
	{
		this->scene->sendCmdToNine(getPosI(),&send,sizeof(send));
	}
}
/**
 * \brief ֪ͨ�ͻ�����ʾ״̬��Ч
 * \param state ״̬��Ч���
 * \param isShow ���ֻ�����ʧ
 */
void SceneNpc::showCurrentEffect(const WORD &state, bool isShow,bool notify)
{
	if (isShow)
	{
		if(this->setUState(state) && notify)
			this->setStateToNine(state);
	}
	else
	{
		if(this->clearUState(state) && notify)
			this->clearStateToNine(state);
	}
}

/**
 * \brief ��ɫ������N��
 * \param dwAttTempID �����ߵ���ʱID
 * \param grids
 * \author fqnewman
 */
void SceneNpc::standBack(const DWORD dwAttTempID, DWORD grids)
{
	SceneUser *att = SceneUserManager::getMe().getUserByTempID(dwAttTempID);
	if (att)
	{
		backoff(Scene::getCompDir(att->getPos(), this->pos), grids);
	}
	else
	{
		backoff(getDir(), grids);
	}
}

/**
 * \brief ������Ŀ�껻��dwTempID��ָ��Ľ�ɫ���
 * \param dwTempID Ŀ���ɫ����ʱID
 * \author fqnewman
 */
void SceneNpc::changeAttackTarget(const DWORD &dwTempID)
{
	SceneUser *targetUser = SceneUserManager::getMe().getUserByTempID(dwTempID);
	if (targetUser) 
	{
		if (canChaseTarget(targetUser))
		{
			unChaseUser();
			chaseSceneEntry(targetUser->getType(), targetUser->tempid);
		}
	}
}

/**
 * \brief �ý�ɫ����
 * \author fqnewman
 */
void SceneNpc::toDie(const DWORD &dwTempID)
{
	if (0==dwTempID)
		ScenePk::attackDeathNpc(0, this);
	else
	{
		SceneUser *att = SceneUserManager::getMe().getUserByTempID(dwTempID);
		if (att)
		{
			hp = 0;
			processDeath(att);
			//ScenePk::attackDeathNpc(att, this);
		}
	}
}

/**
 * \brief �жϽ�ɫ�Ƿ�����
 * \author fqnewman
 * \return trueΪ����
 */
bool SceneNpc::isDie()
{
    if (this->getState() == SceneEntry_Death) return true;
    return false;
}


/**
 * \brief �õ�npc�ĵȼ�
 *
 *
 * \return �ȼ�
 */
DWORD SceneNpc::getLevel() const
{
    return npc->level;
}

/**
 * \brief ��Ҫ��ְҵ���ͣ���������ʹ�õļ�������
 * \author fqnewman
 */
bool SceneNpc::needType(const DWORD &needtype)
{
    return true;
}

/**
 * \brief ��Ҫ��ְҵ���ͣ���������ʹ�õļ�������
 * \author fqnewman
 */
bool SceneNpc::addSkillToMe(zSkill *skill)
{
    return usm.addSkill(skill);;
}

/**
 * \brief �Ƿ��иü�����Ҫ������
 * \author fqnewman
 * \return true �� false û��
 */
bool SceneNpc::needWeapon(DWORD skillid)
{
    return true;
}

/**
 * \brief �Ƿ�Pk����
 * \param other PK�����
 * \author fqnewman
 * \return true �� false ��
 */
bool SceneNpc::isPkZone(SceneEntryPk *other)
{
    return true;
}

/**
 * \brief ������Ʒ�����ͷ���
 * \param object ������Ʒ������
 * \param num ������Ʒ������
 * \author fqnewman
 * \return true ���ĳɹ� false ʧ��
 */
bool SceneNpc::reduce(const DWORD &object, const BYTE num)
{
    return true;
}

/**
 * \brief ����������Ʒ�Ƿ��㹻
 * \param object ������Ʒ������
 * \param num ������Ʒ������
 * \author fqnewman
 * \return true �㹻 false ����
 */
bool SceneNpc::checkReduce(const DWORD &object, const BYTE num)
{
    return true;
}

/**
 * \brief ʩ�ż��������µ�����MP,HP,SP
 * \param base ���ܻ������Զ���
 * \author fqnewman
 * \return true ���ĳɹ� false ʧ��
 */
bool SceneNpc::doSkillCost(const zSkillB *base)
{
    return true;
}

/**
 * \brief ���ʩ�ż��������µ�����MP,HP,SP�Ƿ��㹻
 * \param base ���ܻ������Զ���
 * \author fqnewman
 * \return true ���ĳɹ� false ʧ��
 */
bool SceneNpc::checkSkillCost(const zSkillB *base)
{
    return true;
}

/**
 * \brief ��������ʩ�ųɹ����ʣ�������μ����Ƿ����ʩ��
 * \author fqnewman
 * \return true �ɹ� false ʧ��
 */
bool SceneNpc::checkPercent()
{
    return true;
}

/**
 * \brief �ı��ɫ��hp
 * \param hp �����HP
 * \author fqnewman
 */
SWORD SceneNpc::directDamage(SceneEntryPk *pAtt, const SDWORD &dam, bool notify)
{

	SceneEntryPk::directDamage(pAtt, dam, notify);

	SDWORD reduceHP=0;
	if ((SDWORD)hp-dam>=0)
	{
		hp -= dam;
		reduceHP = dam;
	}
	else
	{
		reduceHP = hp;
		hp=0;
	}

	if (reduceHP !=0 && notify)
	{
		Cmd::stObjectHpMpPopUserCmd ret;
		ret.dwUserTempID = this->tempid;
		ret.byTarget = Cmd::MAPDATATYPE_NPC;
		ret.vChange = 0-(int)reduceHP;
		ret.type = Cmd::POP_HP;
		this->scene->sendCmdToNine(getPosI(), &ret , sizeof(ret));
	}


	SceneEntryPk *pEntry = pAtt->getMaster();
	if (pEntry&&(pEntry->getType() == zSceneEntry::SceneEntry_Player))
	{
		SceneUser * master = (SceneUser *)pEntry;
		if (master)
		{
			this->reduceHP(master , reduceHP);
			this->setLockUser(master->id);
		}
	}
	return reduceHP;
}

/**
 * \brief ����npc����
 *
 *
 * \param pAtt ����
 * \return �Ƿ�����
 */
bool SceneNpc::processDeath(SceneEntryPk *pAtt)
{
	SceneEntryPk *mymaster=NULL;
	if (hp ==0 && getState()!=zSceneEntry::SceneEntry_Death)
	{
		SceneEntryPk *temp = NULL;
		DWORD attCountryID = 0;
		DWORD attUnionID   = 0;
		DWORD dwAttUserID = 0;
		bool isAntiAtt = false;

		// ����ٻ�
		mymaster = this->getTopMaster();
		if (mymaster->summon != this) mymaster = NULL;
		switch(pAtt->getType())
		{
			case zSceneEntry::SceneEntry_NPC:
				{
					temp = pAtt->getTopMaster();
					if (!temp) temp = pAtt;

					if (((SceneUser*)temp)->getType() == zSceneEntry::SceneEntry_Player)
					{
						attUnionID   = ((SceneUser*)temp)->charbase.unionid;
						attCountryID = ((SceneUser*)temp)->charbase.country;
						dwAttUserID = ((SceneUser*)temp)->id;
						isAntiAtt = ((SceneUser*)temp)->isWarRecord(Cmd::COUNTRY_FORMAL_ANTI_DARE, 
								this->scene->getCountryID());
					}
				}
				break;
			case zSceneEntry::SceneEntry_Player:
				{
					temp = pAtt;
					attCountryID = ((SceneUser*)temp)->charbase.country;
					attUnionID = ((SceneUser*)temp)->charbase.unionid;
					dwAttUserID = ((SceneUser*)temp)->id;

					isAntiAtt = ((SceneUser*)temp)->isWarRecord(Cmd::COUNTRY_FORMAL_ANTI_DARE, 
								this->scene->getCountryID());

					switch ( npc->kind)
					{
						case NPC_TYPE_TRADE:
						case NPC_TYPE_TASK:
						case NPC_TYPE_LIVENPC:
						case NPC_TYPE_MAILBOX:
						case NPC_TYPE_AUCTION:
							if (attCountryID!=scene->getCountryID())
							{
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "%sNPC %s(%u,%u) �� %s(%s) ɱ��", scene->getRealName(), name, pos.x, pos.y, temp->name, SceneManager::getInstance().getCountryNameByCountryID(attCountryID));
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
						default:
							break;
					}

					if (npc->id==30034)//ɱ��������
					{
						SceneUser * u = (SceneUser *)pAtt;
						if (u->isRedNamed())
						{
							u->charbase.goodness = (DWORD)Cmd::GOODNESS_2_1;
							while (!u->pkState.cancelProtect(u));

							Channel::sendSys(u, Cmd::INFO_TYPE_EXP, "��ɱ�������٣�������Լ����еķ����¼");
							Zebra::logger->debug("[����]%s ɱ��������", u->name);
						}
					}
				}
				break;
			default:
				{
					temp = pAtt;
				}
				break;
		}

		resetSpeedRate();
		resetAspeedRate();

		if (AIC) AIC->on_die();

		if (this->id == COUNTRY_MAIN_FLAG || this->isMainGeneral() || this->id == COUNTRY_KING_MAIN_FLAG || this->id == COUNTRY_EMPEROR_MAIN_GEN)
		{
			Cmd::Session::t_countryDareResult_SceneSession send;
			send.dwAttCountryID = attCountryID;
			send.dwDefCountryID = this->scene->getCountryID();

			if (attCountryID>0)
			{
				strncpy(send.attCountryName, 
						SceneManager::getInstance().getCountryNameByCountryID(attCountryID), 
						sizeof(send.attCountryName));
			}
			else
			{
				bzero(send.attCountryName, sizeof(send.attCountryName));
			}

			strncpy(send.defCountryName, 
					SceneManager::getInstance().getCountryNameByCountryID(this->scene->getCountryID()), 
					sizeof(send.defCountryName));

			if (this->id == COUNTRY_MAIN_FLAG || this->id == COUNTRY_KING_MAIN_FLAG)
			{
				send.byType = Cmd::Session::COUNTRY_ANNOY_DARE;
				send.dwAttUserID = dwAttUserID;
			}
			else if (this->isMainGeneral())
			{
				if (isAntiAtt)
				{
					send.byType = Cmd::Session::COUNTRY_FORMAL_ANTI_DARE;
				}
				else
				{
					send.byType = Cmd::Session::COUNTRY_FORMAL_DARE;
				}

				send.dwAttUserID = dwAttUserID;
			}
			else if (this->id == COUNTRY_EMPEROR_MAIN_GEN)
			{
				send.byType = Cmd::Session::EMPEROR_DARE;
				send.dwAttUserID = dwAttUserID;
			}

			sessionClient->sendCmd(&send, sizeof(send));
		}

		if (this->id == COUNTRY_SEC_GEN)
		{// ֪ͨ�ط����󽫾����ܵ�����
			Cmd::Session::t_countryNotify_SceneSession send;
			bzero(send.info, sizeof(send.info));
			sprintf(send.info, "%s", "�����ӳ��������󽫾����ܵ�����!");
			send.dwCountryID = this->scene->getCountryID();
			sessionClient->sendCmd(&send, sizeof(send));
		}

		if (this->id == COUNTRY_SEC_FLAG)
		{// ֪ͨ�ط��������ܵ�����
			Cmd::Session::t_countryNotify_SceneSession send;
			bzero(send.info, sizeof(send.info));
			sprintf(send.info, "���� %s ����(%u,%u)����", this->scene->getRealName(), this->getPos().x, this->getPos().y);
			send.dwCountryID = this->scene->getCountryID();
			sessionClient->sendCmd(&send, sizeof(send));
		}

		if (this->id == 58101 && attUnionID>0 && attCountryID > 0) //����������ǳ��죬��֪ͨ�Ự���г����任
		{
			Cmd::Session::t_UnionCity_DareResult_SceneSession send;
			send.dwUserID = pAtt->id;
			send.dwCountryID = this->scene->getCountryID();
			send.dwCityID = pAtt->scene->getRealMapID();
			send.dwUnionID = attUnionID;
			sessionClient->sendCmd(&send, sizeof(send));
		}

		ScenePk::attackDeathNpc(temp, this);
		if (this->summonsplit)
		{
			if (mymaster&&zMisc::selectByPercent(this->summonsplit))
			{
				mymaster->summonPet(this->npc->id, Cmd::PET_TYPE_TOTEM, 30, 0, 0, 0);
				mymaster->summonPet(this->npc->id, Cmd::PET_TYPE_SUMMON, 0, 0, 0, 0);
			}
		}
		return true;
	}
	return false;
}

#include "Chat.h"

/**
 * \brief ��������״̬
 *
 *
 * \param user ���
 */
void SceneNpc::set_quest_status(SceneUser* user)
{
	clearUState(Cmd::USTATE_START_QUEST);
	clearUState(Cmd::USTATE_DOING_QUEST);	
	clearUState(Cmd::USTATE_FINISH_QUEST);	

//	if (ScriptQuest::get_instance().has(ScriptQuest::NPC_VISIT, id)) { 
//		char func_name[32];
//		sprintf(func_name, "%s_%d", "state", id);
//		if (state) {
//			setUState(state);
//			return;
//		}
//	}

	OnVisit event(id);
	int state = EventManager<OnVisit>::instance().state(*user, event) ;
	if (state != -1) {
		setUState(state);
		//Zebra::logger->debug("NPC(%s), ����(%d, %d) ", name, id, state);

	}
}

/**
 * \brief ��9�������Լ�����Ϣ
 */
void SceneNpc::reSendMyMapData()
{
    this->sendMeToNine();
}

/**
 * \brief npc�л���ͼ
 * ֻ����ͬһ������
 *
 *
 * \param newScene Ҫȥ�ĵ�ͼ
 * \param pos Ҫȥ��λ��
 * \return �Ƿ��л��ɹ�
 */
bool SceneNpc::changeMap(Scene * newScene, const zPos &pos)
{
	if (!newScene)
	{
		Zebra::logger->error("SceneNpc::changeMap(): ������ת�����ڵĵ�ͼ newScene=0");
		return false;
	}
	scene->removeNpc(this);
	//֪ͨ�ͻ���ɾ��NPC
	zPosI oldPosI = getPosI();
	Scene *oldScene=scene;

	//this->pos = pos;


	if (!newScene->refreshNpc(this, pos))
	{
		Zebra::logger->debug("%s ��ת��ͼ %s ʧ��", name, newScene->name);
		return false;
	}
	scene = newScene;
	//ɾ���ɵ�ͼ�ϵ���
	Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
	removeNpc.dwMapNpcDataPosition = tempid;
	oldScene->sendCmdToNine(oldPosI, &removeNpc, sizeof(removeNpc));
	//����µ�ͼ����
	this->sendMeToNine();

	Zebra::logger->debug("%s ��ת��ͼ %s (x=%u,y=%u)", name, newScene->name, getPos().x, getPos().y);
	return true;
}

/**
 * \brief ��ѡ�и�npc����ҷ��͸�npc��״̬��Ϣ
 *
 *
 * \param state npc״̬
 * \param value ������ֵ
 * \param time ����ʱ��
 * \return 
 */
void SceneNpc::sendtoSelectedState(DWORD state , WORD value , WORD time)
{
	//Zebra::logger->debug("NPC(%s), ״̬(%d, %d , %d)", name, state , value , time);
	using namespace Cmd;
	char Buf[200]; 
	bzero(Buf , sizeof(Buf));
	stSelectReturnStatesPropertyUserCmd *srs=(stSelectReturnStatesPropertyUserCmd*)Buf;
	constructInPlace(srs);
	srs->byType = MAPDATATYPE_NPC;
	srs->dwTempID = this->tempid;
	srs->states[0].state = state;
	srs->states[0].result = value;
	srs->states[0].time = time;
	srs->size=1;
	//selected_lock.lock();
	SelectedSet_iterator iter = selected.begin();
	for(; iter != selected.end() ;)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(*iter);
		if(pUser)
		{
			if(this->scene->checkTwoPosIInNine(this->getPosI() , pUser->getPosI()))
			{
				pUser->sendCmdToMe(srs , sizeof(stSelectReturnStatesPropertyUserCmd) + sizeof(srs->states[0]));
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

/**
 * \brief ��ѡ�ָ�npc����ҷ��͸�npc��hp��np
 */
void SceneNpc::sendtoSelectedHpAndMp()
{
	Cmd::stRTSelectedHpMpPropertyUserCmd ret;
	ret.byType = Cmd::MAPDATATYPE_NPC;
	ret.dwTempID = this->tempid;//��ʱ���
	ret.dwHP = this->hp;//��ǰѪ
	ret.dwMaxHp = getMaxHP();//���hp
	ret.dwMP = 0;//this->charbase.mp;//��ǰmp
	ret.dwMaxMp = 0;//this->charstate.maxmp;//���mp
	//selected_lock.lock();
	SelectedSet_iterator iter = selected.begin();
	for(; iter != selected.end() ;)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(*iter);
		if(pUser)
		{
			if(this->scene->checkTwoPosIInNine(this->getPosI() , pUser->getPosI()))
			{
				pUser->sendCmdToMe(&ret ,sizeof(ret));
				//Zebra::logger->debug("sendtoSelectedHpAndMp(): ����hp��mp�� %s", pUser->name);
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

/**
 * \brief ����ͼ��ϵ�ĳ���ʱ��
 *
 * \param standTime �ӳ���ʱ��
 */
void SceneNpc::setStandingTime(DWORD standTime)
{
    dwStandTime += standTime;
    dwStandTimeCount=standTime;
#ifdef _DEBUGLOG
    if (standTime>0) Zebra::logger->debug("ͼ��ϵ�ĳ���ʱ��%u ��ǰʱ��[%u]", standTime, dwStandTime);
#endif
}


/**
 * \brief �ж�npc���������
 *
 *
 * \param five Ҫ�жϵ�������ֵ
 * \return 1���� 2������ 0���޹�
 */
int SceneNpc::IsOppose(DWORD five)
{
	if(this->npc->five == (five + 4)%5)
	{
		return 1;
	}
	else
	{
		if(this->npc->five == (five + 1)%5)
		{
			return 2;
		}

	}
	if ((this->npc->five != 5) &&(five == 5))
		return 1;
	if ((this->npc->five == 5) &&(five != 5))
		return 2;
	return 0;
}

/**
 * \brief ����npc���ƶ��ٶȱ���
 *
 * \param rate ����
 */
void SceneNpc::setSpeedRate(float rate)
{
	speedRate = rate;

	//hp20%�����ٶȼӱ�
	if (speedUpUnder20)
		speedRate *= 2.0;

	//this->sendMeToNine();
	//Channel::sendNine(this, "�ƶ��ٶ� %f", speedRate);
}

/**
 * \brief ����npcԭʼ�ٶ�
 */
void SceneNpc::resetSpeedRate()
{
	DWORD value = (DWORD)((640-skillValue.movespeed)*(1+this->skillValue.array_dmvspeed/100.0f));
	if (value == 0)
	{
		speedRate = 6;
	}
	else
	{
		speedRate = 640.0f/((float)value);
	}
	if (this->assault) speedRate =4;

	//hp20%�����ٶȼӱ�
	if (speedUpUnder20)
		speedRate *= 2.0;

	//this->sendMeToNine();
	//Channel::sendNine(this, "�ƶ��ٶ� %f", speedRate);
}

/**
 * \brief ����npc�Ĺ����ٶȱ���
 *
 * \param rate ����
 */
void SceneNpc::setAspeedRate(float rate)
{
    aspeedRate = rate;

    //hp50%���¹����ٶȼӱ�
    if (aspeedUpUnder50)
		aspeedRate *= 2.0;

    //Channel::sendNine(this, "�����ٶ� %f", aspeedRate);
    //Zebra::logger->debug("%s ���ù����ٶȱ��� %f", name, rate);
}

/**
 * \brief ����npcԭʼ�����ٶ�
 */
void SceneNpc::resetAspeedRate()
{
    aspeedRate = 1.0;

    //hp50%���¹����ٶȼӱ�
    if (aspeedUpUnder50)
		aspeedRate *= 2.0;

    //Channel::sendNine(this, "�����ٶ� %f", aspeedRate);
}

/**
 * \brief ȡ��npc������
 */
void SceneNpc::clearMaster()
{
    //master = NULL;
}

//����Ѱ��ʱ�ж�
/**
 * \brief �ж��Ƿ������Ŀ��
 *
 *
 * \param entry Ҫ�жϵ�Ŀ��
 * \return �Ƿ��������
 */
bool SceneNpc::canChaseTarget(const SceneEntryPk * entry)
{
	switch (entry->getType())
	{
		case zSceneEntry::SceneEntry_Player:
			{
				if (NPC_AI_PATROL==AIDefine.type)
				{
					if (((SceneUser *)entry)->isRedNamed())
						return true;
					else
						return false;
				}
				else
					return true;
			}
		case zSceneEntry::SceneEntry_NPC:
			{
				//������ʱ����
				if (NPC_TYPE_PET!=/* ((SceneNpc *)entry)-> */npc->kind) return false;
				if (NPC_AI_PATROL==AIDefine.type)
				{
					if (((SceneNpc *)entry)->isRedNamed()) return true;

					SceneEntryPk * pk = ((SceneNpc *)entry)->getChaseSceneEntry();
					if ((pk) && (zSceneEntry::SceneEntry_Player==pk->getType())
							&&(NPC_TYPE_PET!=((SceneNpc *)entry)->npc->kind))
						return true;
				}
				else
					if (NPC_TYPE_PET==((SceneNpc *)entry)->npc->kind)
						return true;
			}
		default:
			return false;
	}
	return false;
}

/**
 * \brief npc�Ƿ����
 *
 * \return �Ƿ����
 */
bool SceneNpc::isRedNamed(bool allRedMode)
{
    return false;
}

/**
 * \brief ����npc�ű�
 *
 * \param id �ű�id
 * \return �Ƿ����óɹ�
 */
bool SceneNpc::setScript(int id)
{
    if (!AIC) return false;

//    AIC->loadScript(id);
    return AIC->isActive();
}

/**
 * \brief ���npc�ű�
 *
 */
void SceneNpc::clearScript()
{
    if (!AIC) return;
//    AIC->unloadScript();
}

/**
 * \brief �����߳�����
 * \param attacktype �����ߵ�����
 * \param tempid �����ߵ���ʱid
 * \author fqnewman
 */
void SceneNpc::assaultMe(BYTE attacktype, DWORD tempid)
{
	if (attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(tempid);
		if (pUser)
		{
			zPos tmppos = pos;
			const zPos &pos1 = pUser->getPos();
			if (pos1.x>pos.x)
			{
				tmppos.x++;
			}
			else if (tmppos.x>0)
			{
				tmppos.x--;
			}

			if (pos1.y>pos.y)
			{
				tmppos.y++;
			}
			else if (tmppos.y>0)
			{
				tmppos.y--;
			}
			pUser->goTo(tmppos);
		}
	}
}

/**
 * \brief ��Ŀ��ʹ�ü���
 *
 *
 * \param target Ŀ��
 * \param id ����id
 * \return �Ƿ�ʹ�óɹ�
 */
bool SceneNpc::useSkill(SceneEntryPk * target, DWORD id)
{
	//return false;
	if (!target) return false;
	if (!canAttack(target)) return false;

	zSkill *s = usm.findSkill(id);
	if (!s) return false;
	if (!s->canUse()) return false;

	BYTE atype = 0;
	BYTE action = 0;
	npc->getATypeAndAction(atype, action);

	if (this->npc->kind != NPC_TYPE_SURFACE) setDir(getPos().getDirect(target->getPos()));

	using namespace Cmd;
	stAttackMagicUserCmd att;
	att.dwUserTempID = tempid;
	att.dwDefenceTempID = target->tempid;

	att.wdMagicType = id;
	switch(atype)
	{
		case NPC_ATYPE_NEAR:	/// �����빥��
			{
				att.byAction = Ani_Attack;
			}
			break;
		case NPC_ATYPE_FAR:		/// Զ���빥��
			{
				att.byAction = Ani_Attack;
			}
			break;
		case NPC_ATYPE_MFAR:
		case NPC_ATYPE_MNEAR:
			{
				att.byAction = Ani_Null;
			}
			break;
		case NPC_ATYPE_NOACTION:
			{
				att.byAction = Ani_Num;
			}
			break;
		default:
			break;
	}
	//att.byAction = action;

	att.xDes = (WORD)target->getPos().x;
	att.yDes = (WORD)target->getPos().y;
	att.byDirect = getDir();
	switch (target->getType())
	{
		case zSceneEntry::SceneEntry_Player:
			{
				att.byAttackType = ATTACKTYPE_N2U;
			}
			break;
		case zSceneEntry::SceneEntry_NPC:
			{
				att.byAttackType = ATTACKTYPE_N2N;
			}
			break;
		default:
			{
				att.byAttackType = ATTACKTYPE_N2U;
			}
			break;
	}

	s->action(&att, sizeof(att));
#ifdef _XWL_DEBUG
	//Channel::sendNine(this, "%s!", s->name);
#endif
	setAttackTime(SceneTimeTick::currentTime);
	setMoveTime(SceneTimeTick::currentTime);
	setEndBattleTime(SceneTimeTick::currentTime, 10*1000);

	return true;
}

/**
 * \brief npc���˵��
 *
 * \param type ˵��������
 */
void SceneNpc::randomChat(NpcChatType type)
{
	if ((npc->kind!=0)
			&&(npc->kind!=6)
			&&(npc->kind!=7)
			&&(npc->kind!=19)
			&&(npc->kind!=20)
			&&(npc->kind!=24)
			&&(npc->kind!=25))
		return;

	char str[MAX_CHATINFO];
	if (SceneNpcManager::getMe().getNpcCommonChat(type, str))
		Channel::sendNine(this, str);
	//else
	//	Zebra::logger->debug("ȡ��˵������ʧ�� type=%d", type);
}

/**
 * \brief ��ɫ���˿־�Ч��
 */
bool SceneNpc::dreadProcess()
{
	if (dread)
	{
		int count=10;
		int curDir=0;
		do {
			if (count==10)
				curDir = getDir()+zMisc::randBetween(-1,1);
			else
				curDir++;
			if (curDir <0) curDir = 7;
			count--;
		}while(!move(curDir%8, 1)&&count>0);
		return true;
	}
	return false;
}

/**
 * \brief �õ�npc�Ĺ�������
 *
 * \return ��������
 */
BYTE SceneNpc::getAType()
{
    BYTE atype = 0;
    BYTE action = 0;
    npc->getATypeAndAction(atype,action);
    return atype;
}

/**
 * \brief �ж��Ƿ����
 *
 * \return �Ƿ����
 */
bool SceneNpc::isRedNamed(bool allRedMode) const
{
    return false;
}
/**
 * \brief ��������Ϣ�Ľṹ
 *
 *
 * \param data �ṹ��ַ
 */     
void SceneNpc::full_PetDataStruct(Cmd::t_PetData & data)
{
}

/**
 * \brief �Ƿ���������
 *
 * \return �Ƿ���������
 */
bool SceneNpc::isActive()
{
    return aif&AIF_ACTIVE_MODE;
}

/**
 * \brief �Ƿ����ս��
 *
 * \return �Ƿ����ս��
 */
bool SceneNpc::canFight()
{
    return !(aif&AIF_NO_BATTLE);
}

/**
 * \brief �Ƿ�����ƶ�
 *
 * \return �Ƿ�����ƶ�
 */
bool SceneNpc::canMove()
{
    return ( !(aif&AIF_NO_MOVE) && moveAction);
}

/**
 * \brief �õ����������
 *
 * \return ��������
 */
Cmd::petType SceneNpc::getPetType()
{
    return Cmd::PET_TYPE_NOTPET;
}

/**
 * \brief ���npcΪ���״̬
 * �´�ѭ��ʱ���
 */
void SceneNpc::setClearState()
{
    //if (isSpecialNpc())
    //	SceneNpcManager::getMe().removeSpecialNpc(this);
    clearMe = true;

	if (isMainGeneral())
		scene->bossMap.erase(COUNTRY_MAIN_GEN);
	if (scene->bossMap[id]==this)
		scene->bossMap.erase(id);
    //Zebra::logger->trace("���npc %s(%u)", name, tempid);
}

/**
 * \brief ����npc�Ƿ�Ҫ���
 *
 * \return �Ƿ�Ҫ���
 */
bool SceneNpc::needClear()
{
    return clearMe;
}

/**
 * \brief �ж��Ƿ�������npc
 *
 * \return �Ƿ�������npc
 */
bool SceneNpc::isTaskNpc()
{
    if(npc->kind == NPC_TYPE_TRADE 
	    || npc->kind == NPC_TYPE_TASK 
	    || npc->kind == NPC_TYPE_LIVENPC)
    {
	return true;
    }
    return false;
}
/**
 * \brief �ж��Ƿ��ǹ���npc
 *
 * \return �Ƿ��ǹ���npc
 */
bool SceneNpc::isFunctionNpc()
{
    if(npc->kind == NPC_TYPE_ROADSIGN 
	    || npc->kind == NPC_TYPE_TRADE 
	    || npc->kind == NPC_TYPE_TASK 
	    || npc->kind == NPC_TYPE_MOBILETRADE
	    || npc->kind == NPC_TYPE_MAILBOX
	    || npc->kind == NPC_TYPE_AUCTION)
    {
	return true;
    }
    return false;
}
/**
 * \brief �ж��Ƿ�������npc
 *
 * ����npc�������boss���й̶��ű���npc
 * \return �Ƿ�������npc
 */
bool SceneNpc::isSpecialNpc()
{
	if (Cmd::PET_TYPE_NOTPET!=getPetType())
	{
		SceneEntryPk *p = getTopMaster();
		if (0==p) return false;
		if (p==this) return false;
		if (p->getType() == zSceneEntry::SceneEntry_Player) return true;
		if (p->getType() == zSceneEntry::SceneEntry_NPC) return ((SceneNpc *)p)->isSpecialNpc();
	}
	if (AIC->isActive()) return true;
	if (NPC_TYPE_BBOSS==npc->kind || NPC_TYPE_PBOSS==npc->kind) return true;
	return false;
}

/**
 * \brief �õ��ٶȱ���
 *
 * \return �ٶȱ���
 */
float SceneNpc::getSpeedRate()
{
    return speedRate;
}

/**
 * \brief �ı䲢ˢ�½�ɫ����
 * \author fqnewman
 */
void  SceneNpc::changeAndRefreshHMS(bool lock, bool sendData)
{
    this->resetSpeedRate();
    this->reSendData = false;
}

/**
 * \brief ���ù���Ŀ��
 * 
 *
 * \param target ����Ŀ��
 * \param force ǿ������Ŀ��
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::setCurTarget(SceneEntryPk * target, bool force)
{
	if (!target) return false;
	if (!canFight()) return false;
	if (!canReach(target)) return false;
	if (curTargetID!=0 && !force)
	{
		if (lockTarget||aif&AIF_LOCK_TARGET) return false;
		return forceChaseUser(target);
	}

	if (aif&AIF_CALL_FELLOW_7 || aif&AIF_CALL_FELLOW_9)
		AIC->on_find_enemy(target);

	if (SceneEntryPk::setCurTarget(target))
	{
		closeCount = 0;
		return true;
	}
	return false;
}

/**
 * \brief ���ù���Ŀ��
 *
 *
 * \param type ����Ŀ������
 * \param id ����Ŀ����ʱID
 * \param force ǿ������Ŀ��
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::setCurTarget(DWORD id, DWORD type, bool force)
{
	//if (!canFight()) return false;
	SceneEntryPk * target = 0;
	switch (type)
	{
		case zSceneEntry::SceneEntry_Player:
			{
				target = scene->getUserByTempID(id);
				break;
			}
		case zSceneEntry::SceneEntry_NPC:
			{
				target = SceneNpcManager::getMe().getNpcByTempID(id);
				break;
			}
		default:
			return false;
	} 
	return setCurTarget(target);
}

/**
 * \brief �л�����
 *
 * \param id ����ID
 * \param type ��������
 * \param standTime ��npc�ĳ���ʱ��
 * \param sid �ű�ID
 * \param petName ָ�����������
 * \param anpcid ǿ���ĸ���npcID
 *
 * \return ����ó�����ڣ����س�������ָ�룬����ΪNULL
 */
ScenePet * SceneNpc::summonPet(DWORD id, Cmd::petType type, DWORD standTime, DWORD sid, const char * petName, DWORD anpcid, zPos pos, BYTE vdir)
{
	if ((Cmd::PET_TYPE_PET>type)||(Cmd::PET_TYPE_SEMI<type))
	{
		Zebra::logger->trace("SceneNpc::summonPet(): %s �ٻ�δ֪���͵ĳ��� type=%d", name, type);
		return 0;
	}

	zNpcB *base = npcbm.get(id);
	zNpcB *abase = NULL;
	if (anpcid>0) abase = npcbm.get(anpcid);
	if (NULL == base) return 0;

	t_NpcDefine define;
	//	zPos pos = getPos();
	define.id = base->id;
	strncpy(define.name, base->name, MAX_NAMESIZE-1);
	define.pos = getPos();
	define.num = 1;
	define.interval = 5;
	define.initstate = zSceneEntry::SceneEntry_Normal;
	define.width = SceneUser::CALL_PET_REGION;
	define.height = SceneUser::CALL_PET_REGION;
	define.pos -= zPos(SceneUser::CALL_PET_REGION/2,SceneUser::CALL_PET_REGION/2);
	define.scriptID = sid;
	scene->initRegion(define.region, define.pos, define.width, define.height);

	ScenePet * newPet = scene->summonOneNpc<ScenePet>(define, pos, base, standTime, abase, vdir);

	if (newPet)
	{
		/*
		   char n[MAX_NAMESIZE];
		   bzero(n, MAX_NAMESIZE);
		   if (0==strncmp(petName, "", MAX_NAMESIZE))
		   snprintf(n, MAX_NAMESIZE-1, "(%s)%s", name, newPet->name);
		   else
		   snprintf(n, MAX_NAMESIZE-1, "(%s)%s", name, petName);
		   strncpy(newPet->name, , MAX_NAMESIZE-1);
		   */

		newPet->setMaster(this);
		newPet->setPetType(type);
		newPet->setDir(vdir);
		if (isSpecialNpc())
			SceneNpcManager::getMe().addSpecialNpc(newPet);

		using namespace Cmd;
		switch (type)
		{
			case PET_TYPE_PET:
				{
					if (pet)
					{
						pet->death(SceneTimeTick::currentTime);
						killOnePet(pet);
					}
					if (summon)
					{
						summon->death(SceneTimeTick::currentTime);
						killOnePet(summon);
					}
					pet = newPet;
				}
				break;
			case PET_TYPE_SUMMON:
				{
					if (pet)
					{
						pet->death(SceneTimeTick::currentTime);
						killOnePet(pet);
					}
					if (summon)
					{
						summon->death(SceneTimeTick::currentTime);
						killOnePet(summon);
					}
					summon = newPet;
				}
				break;
			case PET_TYPE_TOTEM:
#ifdef _XWL_DEBUG
				//Zebra::logger->debug("SceneNpc::summonPet(): lock %s", name);
#endif
				totems.push_back(newPet);
#ifdef _XWL_DEBUG
				//Zebra::logger->debug("SceneNpc::summonPet(): unlock %s", name);
#endif
				break;
			case PET_TYPE_SEMI:
				semipetList.push_back(newPet);
				break;
			default:
				Zebra::logger->trace("SceneNpc::summonPet(): δ֪�ĳ������� %d", type);
				break;
		}

		Zebra::logger->debug("%s ���ӳ��� %s ���� %d", name, newPet->name, type);
	}

	return newPet;
}


/**
 * \brief ɾ��һ������
 *
 */
bool SceneNpc::killOnePet(ScenePet * kill)
{
	if (!kill) return false;
	if (kill->getMaster()!=this)
	{
		Zebra::logger->error("[����]%s(%u) ����NPC %s(%u) �ĳ��kill->getMaster()=%u", kill->name, kill->tempid, name, tempid, kill->getMaster());
		return false;
	}

	using namespace Cmd;

#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::killOnePet(): lock %s", name);
#endif
	switch (kill->getPetType())
	{
		case PET_TYPE_PET:
			{
				pet = 0;
			}
			break;
		case PET_TYPE_SUMMON:
			{
				summon = 0;
			}
			break;
		case PET_TYPE_TOTEM:
			{
				totems.remove(kill);
			}
			break;
		case PET_TYPE_SEMI:
			{
				semipetList.remove(kill);
			}
			break;
		default:
			break;
	}

	kill->clearMaster();

	return true;
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::killOnePet(): unlock %s", name);
#endif
}

/**
 * \brief ɾ�����г���
 *
 */
void SceneNpc::killAllPets()
{
	//ɾ�����г���
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::killAllPets(): lock %s type=%u", name, getPetType());
#endif
	std::list<ScenePet *> copy(totems);
	for (std::list<ScenePet *>::iterator it=copy.begin(); it!=copy.end(); it++)
	{
		(*it)->skillStatusM.clearActiveSkillStatus();

		(*it)->killAllPets();
		//petDeath();
		(*it)->leaveBattle();
		(*it)->scene->clearBlock((*it)->getPos());
		(*it)->setState(SceneEntry_Death);

		Cmd::stNpcDeathUserCmd death;
		death.dwNpcTempID = (*it)->tempid;
		(*it)->scene->sendCmdToNine((*it)->getPosI() , &death , sizeof(death));

		(*it)->clearMaster();
	}
	totems.clear();
	copy.clear();
	copy = semipetList;
	for (std::list<ScenePet *>::iterator it=copy.begin(); it!=copy.end(); it++)
	{
		(*it)->skillStatusM.clearActiveSkillStatus();

		(*it)->killAllPets();
		(*it)->leaveBattle();
		(*it)->scene->clearBlock((*it)->getPos());
		(*it)->setState(SceneEntry_Death);

		Cmd::stNpcDeathUserCmd death;
		death.dwNpcTempID = (*it)->tempid;
		(*it)->scene->sendCmdToNine((*it)->getPosI() , &death , sizeof(death));

		(*it)->clearMaster();
	}
	semipetList.clear();
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::killAllPets(): unlock %s type=%u", name, getPetType());
#endif

	if (pet)
	{
		pet->skillStatusM.clearActiveSkillStatus();

		pet->killAllPets();
		//petDeath();
		pet->leaveBattle();
		pet->scene->clearBlock(pet->getPos());
		pet->setState(SceneEntry_Death);

		Cmd::stNpcDeathUserCmd death;
		death.dwNpcTempID = pet->tempid;
		pet->scene->sendCmdToNine(pet->getPosI() , &death , sizeof(death));

		pet->clearMaster();
		pet = 0;

	}
	if (summon)
	{
		summon->skillStatusM.clearActiveSkillStatus();

		summon->killAllPets();
		//petDeath();
		summon->leaveBattle();
		summon->scene->clearBlock(summon->getPos());
		summon->setState(SceneEntry_Death);

		Cmd::stNpcDeathUserCmd death;
		death.dwNpcTempID = summon->tempid;
		summon->scene->sendCmdToNine(summon->getPosI() , &death , sizeof(death));

		summon->clearMaster();
		summon = 0;

	}
}

/**
 * \brief ����Ƿ��˻�Ѫ��ʱ��
 *
 *
 * \param ct ���ڱȽϵ�ʱ��
 * \return �Ƿ�ʱ��
 */
bool SceneNpc::checkRecoverTime(const zRTime& ct)
{
    return ct>=rcvTimePet;
}

/**
 * \brief �����´λ�Ѫ��ʱ��
 *
 * \param ct �ӳٿ�ʼ��ʱ��
 * \param delay �ӳ�
 */
void SceneNpc::setRecoverTime(const zRTime& ct, int delay)
{
    rcvTimePet = ct;
    rcvTimePet.addDelay(delay);
}

/**
 * \brief �������˹��฽�ӹ�����
 * \param mindamage ��С������
 * \param maxdamage ��󹥻���
 */
void SceneNpc::setAppendDamage(WORD mindamage, WORD maxdamage)
{
    appendMinDamage = mindamage;
    appendMaxDamage = maxdamage;
}

/**
 * \brief ��ȡ��С����������
 * \return ��С����������
 */
DWORD SceneNpc::getMinMDamage()
{
    SDWORD value = (SDWORD)(this->npc->mdamage+appendMinDamage+skillValue.uppetdamage+skillValue.theurgy_umdam-skillValue.dmdam-skillValue.theurgy_dmdam);
    if (value <0) value =0;
    return value;
}

/**
 * \brief ��ȡ�����������
 * \return �����������
 */
DWORD SceneNpc::getMaxMDamage() 
{
    SDWORD value = (SDWORD)(this->npc->maxmdamage+appendMaxDamage+skillValue.uppetdamage+skillValue.theurgy_umdam-skillValue.dmdam-skillValue.theurgy_dmdam);
    if (value <0) value =0;
    return value;
}

/**
 * \brief ��ȡ��С��������
 * \return ��С��������
 */
DWORD SceneNpc::getMinPDamage()
{
    SDWORD value = (SDWORD)(this->npc->damage+appendMinDamage+skillValue.uppetdamage+skillValue.theurgy_updam-skillValue.dpdam-skillValue.theurgy_dpdam);
    if (value <0) value =0;
    return value;
}

/**
 * \brief ��ȡ�����������
 * \return �����������
 */
DWORD SceneNpc::getMaxPDamage()
{
    SDWORD value = (SDWORD)(this->npc->maxdamage+appendMaxDamage+skillValue.uppetdamage+skillValue.theurgy_updam-skillValue.dpdam-skillValue.theurgy_dpdam);
    if (value <0) value =0;
    return value;
}

/**
 * \brief ��ȡ��С����������
 * \return ��С����������
 */
DWORD SceneNpc::getMinMDefence()
{
    SDWORD value = this->npc->mdefence+skillValue.uppetdefence+skillValue.umdef+skillValue.theurgy_umdef - skillValue.theurgy_dmdef;
    value = (SDWORD)(value *( 1 - skillValue.dmdefp/100.0f));
    if (value < 0) value = 0;
    return value;
}

/**
 * \brief ��ȡ�����������
 * \return �����������
 */
DWORD SceneNpc::getMaxMDefence() 
{
    SDWORD value = this->npc->maxmdefence+skillValue.uppetdefence+skillValue.umdef+skillValue.theurgy_umdef - skillValue.theurgy_dmdef;
    value = (SDWORD)(value *( 1 - skillValue.dmdefp/100.0f));
    if (value < 0) value = 0;
    return value;
}

/**
 * \brief ��ȡ��С���������
 * \return ��С���������
 */
DWORD SceneNpc::getMinPDefence()
{
    SDWORD value = this->npc->pdefence+skillValue.uppetdefence+skillValue.updef+skillValue.theurgy_updef - skillValue.theurgy_dpdef;
    if (value < 0) value = 0;
    return value;
}

/**
 * \brief ��ȡ������������
 * \return ������������
 */
DWORD SceneNpc::getMaxPDefence()
{
    SDWORD value = this->npc->maxpdefence+skillValue.uppetdefence+skillValue.updef+skillValue.theurgy_updef - skillValue.theurgy_dpdef;
    if (value < 0) value = 0;
    return value;
}

/**
 * \brief �������hp
 * \author fqnewman
 * \return �������ֵ
 */
DWORD SceneNpc::getMaxHP()
{
#ifdef _XWL_DEBUG
	if (boostupHpMaxP)
		Zebra::logger->debug("boostupHpMaxP=%u", boostupHpMaxP);
#endif
    return (DWORD)((npc->hp+(anpc?anpc->hp:0)+skillValue.maxhp)*(1.0f+(float)(boostupHpMaxP/100.0f)));;
}

/**
 * \brief �������hp
 * \author fqnewman
 * \return �������ֵ
 */
DWORD SceneNpc::getBaseMaxHP()
{
    return npc->hp+(anpc?anpc->hp:0);
}

/**
 * \brief ��9�������Լ�������
 *
 */
void SceneNpc::sendMeToNine()
{
	if (npc->kind == NPC_TYPE_TRAP)
	{
		SceneEntryPk *entry = getMaster();
		if (entry && entry->getType() == zSceneEntry::SceneEntry_Player)
		{
			SceneUser *pUser = (SceneUser *)entry;
		BUFFER_CMD(Cmd::stAddMapNpcAndPosMapScreenStateUserCmd ,send , zSocket::MAX_USERDATASIZE);
		this->full_t_MapNpcDataAndPosState(send->data);
			pUser->sendCmdToMe(send, send->size());
		}		
	}
	else
	{
		BUFFER_CMD(Cmd::stAddMapNpcAndPosMapScreenStateUserCmd ,send , zSocket::MAX_USERDATASIZE);
		this->full_t_MapNpcDataAndPosState(send->data);
		this->scene->sendCmdToNine(getPosI(),send,send->size());
	}
}

/**
 * \brief npc��Ѫ
 *
 */
bool SceneNpc::recover()
{
	if (!npc->recover.num) return 0;

	bool ret = false;
	DWORD tMaxHP = getMaxHP();

	if (!needRecover && (npc->recover.start*tMaxHP/100 > hp))
		needRecover = true;
	if (!needRecover) return 0;

	if (_3_sec(SceneTimeTick::currentTime))
	{
		if (npc->recover.type==1)//���ٷֱȻ�
			hp += tMaxHP*npc->recover.num/100;
		else if (npc->recover.type==2)//������
			hp += npc->recover.num;

		ret = true;
	}

	if (hp>tMaxHP) hp = tMaxHP;
	if (hp==tMaxHP)
		needRecover = false;

	return ret;
}

/**
 * \brief ����ս�� 
 *
 */
void SceneNpc::leaveBattle()
{
	if (aif&AIF_RCV_REST)//����ս����Ѫ
	{
		rcvTimeRest.now();
		rcvTimeRest.addDelay(30*1000);
	}

	clearDefTarget();
	lockTarget = false;
	SceneEntryPk::leaveBattle();
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("%s ����ս��", name);
#endif
}

/**
 * \brief ���õڶ�����Ŀ��
 * \param target Ŀ��ָ��
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::setSecondTarget(SceneEntryPk * target)
{
	if (!target) return false;
	if (!canFight()) return false;
	if (curTargetType==(DWORD)target->getType() && curTargetID==target->tempid) return false;
	if (!canReach(target)) return false;

	if (target==this) return false;//�������Լ�
	if (!isEnemy(target)) return false;//����������

	secondTargetType = target->getType();
	secondTargetID = target->tempid;
#ifdef _XWL_DEBUG
	//	Zebra::logger->debug("%s ���õڶ�Ŀ�� id=%u type=%u", name, secondTargetID, secondTargetType);
#endif
	return true;
}

/**
 * \brief ���õڶ�����Ŀ��
 * \param id ��ʱid
 * \param type ����
 * \return �Ƿ�ɹ�
 */
bool SceneNpc::setSecondTarget(DWORD id, DWORD type)
{
	SceneEntryPk * def = NULL;
	if (zSceneEntry::SceneEntry_Player==type)
		def = scene->getUserByTempID(tempid);
	else if (zSceneEntry::SceneEntry_NPC==type)
		def = SceneNpcManager::getMe().getNpcByTempID(tempid);

	if (def)
		return setSecondTarget(def);
	else
		return false;
}

/**
 * \brief �õ��ι���Ŀ��
 *
 * \return �ι���Ŀ��
 * 
 */
SceneEntryPk * SceneNpc::getSecondTarget()
{
	switch (secondTargetType)
	{
		case zSceneEntry::SceneEntry_Player:
			return scene->getUserByTempID(secondTargetID);
		case zSceneEntry::SceneEntry_NPC:
			return SceneNpcManager::getMe().getNpcByTempID(secondTargetID);
		default:
			return 0;
	}     
}

/**
 * \brief ���õڶ�����Ŀ��Ϊ��ǰĿ��
 *
 * \return �Ƿ�ɹ�
 * 
 */
bool SceneNpc::chaseSecondTarget()
{
    if (secondTargetID==0) return false;
    //setCurTarget(secondTargetID, secondTargetType);
    curTargetID = secondTargetID;
    curTargetType = secondTargetType;
#ifdef _XWL_DEBUG
    //Zebra::logger->debug("%s �����ڶ�Ŀ�� id=%u type=%u", name, secondTargetID, secondTargetType);
#endif
    secondTargetID = 0;
    secondTargetType = 0;
    return true;
}

/**
 * \brief ���õ�ǰĿ������˵�ǰĿ��
 *
 * \return �Ƿ�ɹ�
 * 
 */
bool SceneNpc::chaseItsMaster()
{
    SceneEntryPk * c = getCurTarget();
    if (!c) return false;

    SceneEntryPk * m = c->getTopMaster();
    if (!m || m==c) return false;

#ifdef _XWL_DEBUG
    //Zebra::logger->debug("%s ���������� id=%u type=%u", name, m->tempid, m->getType());
#endif
    return setCurTarget(m, true);
}

void SceneNpc::goToRandomScreen()
{
	zPos randPos;
	if(this->scene->randzPosOnRect(getPos() , randPos , SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		warp(randPos);
	}
}

bool SceneNpc::isMainGeneral()
{
	return (id>=58200 && id<58300 && 0==id%10);
}

/**
 * \brief ����npcMap�ٻ�npc�����HP60%���¡��������ٻ���npc��
 *
 * \return �ٻ�������
 */
int SceneNpc::summonByNpcMap(std::map<DWORD, std::pair<DWORD, DWORD> > map)
{
	unsigned int count = 0;
	for (std::map<DWORD, std::pair<DWORD, DWORD> >::iterator it=map.begin(); it!=map.end(); it++)
	{
		zNpcB *base = npcbm.get(it->first);

		if (base)
		{
			t_NpcDefine define;
			//const zPos &pos = getPos();
			define.id = base->id;
			strncpy(define.name, base->name, MAX_NAMESIZE-1);
			define.pos = getPos();
			define.num = 1;
			define.interval = 5;
			define.initstate = zSceneEntry::SceneEntry_Normal;
			define.width = 10;
			define.height = 10;
			define.pos -= zPos(5,5);
			define.scriptID = 0;
			scene->initRegion(define.region, define.pos, define.width, define.height);

			for (unsigned int i=0; i<it->second.first; i++)
			{
				if (zMisc::selectByPercent(it->second.second))
				{
					SceneNpc * newPet = scene->summonOneNpc<SceneNpc>(define, zPos(0, 0), base, 0, 0);    
					if (newPet)
						count++;
				}
			}
		}
	}
	return count;
}

	/**
	 * \brief ����Ļ�ڶ����ƶ�
	 * \author fqnewman
	 */
	void SceneNpc::jumpTo(zPos &newPos)
	{
		warp(newPos);
	}
