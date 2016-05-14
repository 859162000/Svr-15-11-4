/**
 * \file
 * \version  $Id: SessionClient.cpp$
 * \author 
 * \date 
 * \brief ����Session���������ӿͻ���
 *
 */

#include <unistd.h>
#include <iostream>
#include "zTCPClient.h"
#include "SessionCommand.h"
#include "SessionClient.h"
#include "ScenesServer.h"
#include "Zebra.h"
#include "SceneManager.h"
#include "SceneTaskManager.h"
#include "RecordClient.h"
#include "Chat.h"
#include "School.h"
#include "ObjectManager.h"
#include "QuestEvent.h"
#include "QuestTable.h"
#include "NpcDare.h"
#include "ScenesServer.h"
#include "CartoonPet.h"
#include "SceneUserManager.h"
#include "CountryTech.h"
#include "CountryAlly.h"
#include "RebuildObject.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

SessionClient *sessionClient = NULL;

/**
 * \brief ͳ�ƴ���ָ��NPC�ĸ���
 */
struct ClearAllNotUnion : public zSceneEntryCallBack
{
	DWORD dwUnionID;
	ClearAllNotUnion(DWORD unionid) : dwUnionID(unionid){};
	std::vector<SceneUser*> _remove_list;

	/**
	 * \brief �ص�����
	 * \param entry ��ͼ���,���������
	 * \return �ص��Ƿ��ͳɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		if (((SceneUser *)entry)->charbase.unionid != dwUnionID)
		{
			_remove_list.push_back((SceneUser*)entry);
		}

		return true;
	}

	void reliveAll()
	{
		std::vector<SceneUser*>::iterator tIterator;

		for (tIterator = _remove_list.begin(); tIterator!=_remove_list.end(); tIterator++)
		{
			(*tIterator)->relive();
		}
	}
};
/**
 * \brief  ��Ʒ�Ƚϻص�����������Ѱ��ĳ�ض����͵���Ʒ
 */
class SessionItemObjectCompare:public UserObjectCompare 
{
	public:
		DWORD  dwObjectID;

		bool isIt(zObject *object)
		{
			if (object->base->id == dwObjectID) return true;
			return false;
		}
};

/**
 * \brief  ��¼�Ự������
 * \return  true ��¼��Ϣ���ͳɹ��� false �޷�������Ϣ
 */
bool SessionClient::connectToSessionServer()
{
	if (!connect())
	{
		Zebra::logger->error("����Session������ʧ��");
		return false;
	}

	using namespace Cmd::Session;
	t_LoginSession tCmd;
	tCmd.wdServerID = ScenesService::getInstance().getServerID();
	tCmd.wdServerType = ScenesService::getInstance().getServerType();

	return sendCmd(&tCmd, sizeof(tCmd));
}

/**
 * \brief ����zThread�еĴ��麯�������̵߳����ص����������ڴ�����յ���ָ��
 *
 */
void SessionClient::run()
{
	zTCPBufferClient::run();

	//��Session֮������ӶϿ�����Ҫ�رշ�����
	ScenesService::getInstance().Terminate();
	while(!ScenesService::getInstance().isSequeueTerminate())
	{
		zThread::msleep(10);
	}

}

/**
 * \brief  ������������е�����ϵ�б������Ѻö��ã�һ�����ڽ�ɫ����ϵ�����仯�������
 * \param  pUser ����ϵ�����仯�Ľ�ɫ
 */
void SessionClient::requestFriendDegree(SceneUser *pUser)
{
#ifdef _DEBUGLOG
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "Ϊ���������������ϵ�б�");
#endif
	if (pUser->team.getLeader() == 0) return;
	SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
	if (leader)
	{
#ifdef _DEBUGLOG
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "Ϊ���������������ϵ�б�ɹ�");
#endif
		leader->team.requestFriendDegree();
	}

}

/**
 * \brief  ����Ự���͹�������Ϣ�������ݰ���
 1 ����ϵս
 2 ������ע��ע��
 3 ��ɫ��ע��ע��
 4 ��̬���أ�ж�ص�ͼ
 5 �û���ʱ�浵
 6 ��ᣬ���壬���ɵ���ش���
 7 ˽����Ϣ����
 * \param  
 * \return  
 */
bool SessionClient::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}

bool SessionClient::cmdMsgParse_Gem(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	switch (ptNullCmd->para)
	{
		case Cmd::Session::SUMMON_GEMNPC_SCENE_PARA:
			{
				Cmd::Session::t_SummonGemNPC_SceneSession* rev = 
					(Cmd::Session::t_SummonGemNPC_SceneSession*)ptNullCmd;
				Scene * scene = SceneManager::getInstance().getSceneByID(rev->dwMapID);
				if (!scene)
				{
					Zebra::logger->debug("SUMMON_GEMNPC: �ٻ�npcʱ�Ҳ�����ͼ mapid=%u", rev->dwMapID);
					return false;
				}
				zNpcB *base = npcbm.get(rev->dwBossID);
				if (NULL == base)
				{
					Zebra::logger->debug("SUMMON_GEMNPC: �ٻ�npcʱ�Ҳ���NPC id=%d", rev->dwBossID);
					return false;
				}

				t_NpcDefine define;
				define.id = base->id;
				strcpy(define.name, base->name);
				define.pos = zPos(rev->x, rev->y);
				define.num = 1;
				define.interval = 5;
				define.initstate = zSceneEntry::SceneEntry_Normal;
				define.width = 2;
				define.height = 2;
				define.scriptID = 0;
				scene->initRegion(define.region, define.pos, define.width, define.height);

				SceneNpc * boss = scene->summonOneNpc<SceneNpc>(define, zPos(rev->x, rev->y), base);
				if (!boss)
				{
					Zebra::logger->debug("SUMMON_GEMNPC: �ٻ�NPCʧ�� id=%d", base->id);
					return false;
				}

				scene->bossMap[rev->dwBossID] = boss;
				return true;
			}
			break;
		case Cmd::Session::SET_GEMSTATE_SCENE_PARA:
			{
				Cmd::Session::t_SetGemState_SceneSession* rev = 
					(Cmd::Session::t_SetGemState_SceneSession*)ptNullCmd;

				SceneUser* pUser = NULL;
				pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (pUser)
				{
					if (rev->dwState==1)
					{
						pUser->showCurrentEffect(Cmd::USTATE_TOGETHER_WITH_DRAGON, true);
						pUser->skillStatusM.clearActiveSkillStatus();
						pUser->mask.on_gem();
						pUser->leaveTeam();
					}
					else if (rev->dwState==2)
					{
						pUser->showCurrentEffect(Cmd::USTATE_TOGETHER_WITH_TIGER, true);
						pUser->skillStatusM.clearActiveSkillStatus();
						pUser->mask.on_gem();
						pUser->leaveTeam();
					}
					else if (rev->dwState==0)
					{
						pUser->showCurrentEffect(Cmd::USTATE_TOGETHER_WITH_DRAGON, false);
						pUser->showCurrentEffect(Cmd::USTATE_TOGETHER_WITH_TIGER, false);
					}

					pUser->setupCharBase();
					Cmd::stMainUserDataUserCmd  userinfo;
					pUser->full_t_MainUserData(userinfo.data);
					pUser->sendCmdToMe(&userinfo,sizeof(userinfo));
				}
			}
			break;
		case Cmd::Session::CLEAR_GEMNPC_SCENE_PARA:
			{
				Cmd::Session::t_ClearGemNPC_SceneSession *rev = (Cmd::Session::t_ClearGemNPC_SceneSession *)ptNullCmd;

				Scene * scene = SceneManager::getInstance().getSceneByID(rev->dwMapID);
				if (!scene)
				{
					Zebra::logger->debug("CLEAR_GEMNPC: ���npcʱ�Ҳ�����ͼ mapid=%u", rev->dwMapID);
					return false;
				}

				if (scene->bossMap[rev->dwBossID])
					scene->bossMap[rev->dwBossID]->setClearState();

				return true;
			}
			break;
		case Cmd::Session::BLAST_GEMNPC_SCENE_PARA:
			{
				Cmd::Session::t_BlastGemNPC_SceneSession * rev = (Cmd::Session::t_BlastGemNPC_SceneSession *)ptNullCmd;

				SceneUser * pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (!pUser)
				{
					Zebra::logger->debug("BLAST_GEMNPC: npc����Ʒʱ�Ҳ������ userID=%u", rev->dwUserID);
					return false;
				}

				pUser->showCurrentEffect(Cmd::USTATE_TOGETHER_WITH_DRAGON, false);
				pUser->showCurrentEffect(Cmd::USTATE_TOGETHER_WITH_TIGER, false);
				pUser->setupCharBase();

				Cmd::stMainUserDataUserCmd  userinfo;
				pUser->full_t_MainUserData(userinfo.data);
				pUser->sendCmdToMe(&userinfo,sizeof(userinfo));

				zNpcB *base = npcbm.get(rev->dwBossID);
				if (NULL == base)
				{
					Zebra::logger->debug("BLAST_GEMNPC: �ٻ�npcʱ�Ҳ���NPC id=%d", rev->dwBossID);
					return false;
				}

				t_NpcDefine define;
				define.id = base->id;
				strcpy(define.name, base->name);
				define.pos = pUser->getPos();
				define.num = 1;
				define.interval = 5;
				define.initstate = zSceneEntry::SceneEntry_Normal;
				define.width = 2;
				define.height = 2;
				define.scriptID = 0;
				pUser->scene->initRegion(define.region, define.pos, define.width, define.height);

				SceneNpc * boss = pUser->scene->summonOneNpc<SceneNpc>(define, pUser->getPos(), base);
				if (!boss)
				{
					Zebra::logger->debug("SUMMON_GEMNPC: �ٻ�NPCʧ�� id=%d", base->id);
					return false;
				}

				boss->toDie(0);

				return true;
			}
			break;
		default:
			break;
	}


	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool SessionClient::cmdMsgParse_Army(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	switch (ptNullCmd->para)
	{
		case Cmd::Session::SEND_USER_ARMY_INFO_PARA:
			{
				Cmd::Session::t_sendUserArmyInfo_SceneSession* rev = 
					(Cmd::Session::t_sendUserArmyInfo_SceneSession*)ptNullCmd;

				SceneUser* pUser = NULL;
				pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (pUser)
				{
					strncpy(pUser->armyName, rev->title, MAX_NAMESIZE);

					pUser->dwArmyState = rev->byType;
					pUser->sendMeToNine();
					// zjw modify by 2006-3-2
					//pUser->sendNineToMe();
				}

				return true;
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool SessionClient::cmdMsgParse_Recommend(const Cmd::t_NullCmd* ptNullCmd, const unsigned int nCmdLen)
{
	switch (ptNullCmd->para)
	{
		case Cmd::Session::PICKUP_RECOMMEND_SCENE_PARA:
			{
				Cmd::Session::t_PickupRecommend_SceneSession* rev = 
					(Cmd::Session::t_PickupRecommend_SceneSession*)ptNullCmd;
				
				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (!pUser)
				{
					return true;
				}

				if (rev->byType == 0)
				{
					pUser->packs.addMoney(rev->dwMoney, "�Ƽ�����ȡ����", NULL, true);
				}
				else
				{
					pUser->packs.addMoney(rev->dwMoney, "���Ƽ�����ȡ����", NULL, true);
				}

				return true;
			}
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;

}

bool SessionClient::cmdMsgParse_Dare(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	switch (ptNullCmd->para)
	{
		case Cmd::Session::PARA_SET_EMPEROR_DARE:
			{
				Cmd::Session::t_setEmperorDare_SceneSession* rev =
					(Cmd::Session::t_setEmperorDare_SceneSession*)ptNullCmd;

#ifdef _ZJW_DEBUG				
				Zebra::logger->debug("�յ����ûʳ�����ս����: state:%u defcountry:%u", 
						rev->byState, rev->dwDefCountryID);
#endif				
				DWORD map_id = (6 << 16) + 134;
				Scene* pScene = SceneManager::getInstance().getSceneByID(map_id);

				if (pScene)
				{
					pScene->setEmperorDare(rev->byState, rev->dwDefCountryID);
					if (rev->byState == 1)
					{
						SceneUserManager::getMe().setEmperorDare(pScene);
					}
					else
					{
						SceneUserManager::getMe().clearEmperorDare(pScene);
					}
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SET_ANTI_ATT_FLAG:
			{
				Cmd::Session::t_setAntiAttFlag_SceneSession* rev = 
					(Cmd::Session::t_setAntiAttFlag_SceneSession*)ptNullCmd;
#ifdef _ZJW_DEBUG				
				Zebra::logger->debug("�յ����÷�������: from:%d to:%d", 
						rev->dwToRelationID, rev->dwToRelationID);
#endif				

				SceneUserManager::getMe().setAntiAtt(rev->dwType, 
						rev->dwFromRelationID, rev->dwToRelationID);
				return true;

			}
			break;
		case Cmd::Session::PARA_ENTERWAR:
			{
				Cmd::Session::t_enterWar_SceneSession* rev = 
					(Cmd::Session::t_enterWar_SceneSession*)ptNullCmd;

#ifdef _ZJW_DEBUG				
				Zebra::logger->debug("�յ������ս����:type:%d fromrelation:%d torelation:%d state:%d isatt:%d isAntiAtt:%d", 							rev->dwWarType, rev->dwFromRelationID, rev->dwToRelationID, 
						rev->dwStatus, rev->isAtt, rev->isAntiAtt);
#endif				

				SceneUser* pUser = NULL;

				pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (rev->dwStatus == 1)
				{
					if (rev->dwWarType == Cmd::COUNTRY_FORMAL_DARE 
							|| rev->dwWarType == Cmd::COUNTRY_FORMAL_ANTI_DARE)
					{
						//rev->dwWarType = Cmd::COUNTRY_FORMAL_DARE;

						if (rev->dwUserID == 0)
						{
							SceneUserManager::getMe().enterWar(rev);
						}
						else if(pUser)
						{
							pUser->addWarRecord(rev->dwWarType, rev->dwToRelationID, 
									rev->isAtt);

							if (rev->isAntiAtt)
							{
								pUser->setAntiAttState(rev->dwWarType, rev->dwToRelationID);
							}

							if (pUser->scene->getRealMapID() == 139 
									&& rev->dwToRelationID==pUser->scene->getCountryID()
									&& pUser->scene->getCountryDareBackToMapID())
							{
								pUser->deathBackToMapID =  (rev->dwToRelationID << 16) + 
									pUser->scene->getCountryDareBackToMapID();
							}
						}
					}
					else
					{
						if (pUser == NULL)
						{
							Zebra::logger->error("��Ч���û�ID(%d)�� ȡ�������ս����", 
									rev->dwUserID);
							return true;
						}
						if (rev->dwWarType == Cmd::SEPT_NPC_DARE && pUser->charbase.level >=60)
						{
							DWORD mapid = pUser->scene->getRealMapID();
							if (mapid == 101 ||
								mapid == 102 ||
								mapid == 104)
							{
								/// ����59����Ҳ������ڷ�˳ǣ���β�����Դ�����NPC����ս��
								return true;
							}
						}

						pUser->addWarRecord(rev->dwWarType, rev->dwToRelationID, rev->isAtt);
						if (rev->isAntiAtt)
						{
							pUser->setAntiAttState(rev->dwWarType, rev->dwToRelationID);
						}

						if (rev->dwWarType == Cmd::UNION_CITY_DARE)
						{
							pUser->scene->setUnionDare(true);
						}
					}
				}
				else
				{	
					if (rev->dwWarType == Cmd::COUNTRY_FORMAL_DARE
						|| rev->dwWarType == Cmd::COUNTRY_FORMAL_ANTI_DARE)
					{
						//rev->dwWarType = Cmd::COUNTRY_FORMAL_DARE;

						if (pUser == NULL)
						{
							SceneUserManager::getMe().enterWar(rev);
						}
						else
						{
							pUser->removeWarRecord(rev->dwWarType, rev->dwToRelationID);
							pUser->setDeathBackToMapID(pUser->scene);
						}
					}
					else
					{
						if (pUser == NULL)
						{
							Zebra::logger->error("��Ч���û�ID(%d)�� ȡ�������ս����", 
									rev->dwUserID);
							return true;
						}

						if (rev->dwWarType == Cmd::UNION_CITY_DARE)
						{
							pUser->scene->setUnionDare(false);
						}

						if (rev->dwWarType == Cmd::SEPT_NPC_DARE)
						{
							pUser->notifySessionNpcDareResult();
						}

						pUser->removeWarRecord(rev->dwWarType);
					}
				}

#ifdef _ZJW_DEBUG
				if (pUser)
				{
					Zebra::logger->debug("%s ���� %d ����ս��¼", pUser->name, pUser->warSize());
				}
#endif				
				if (pUser != NULL)
				{
					pUser->sendNineToMe(); // ��ʱ���¶�ս״̬
					pUser->setStateToNine(Cmd::USTATE_WAR);
				}
				
				return true;
			}
			break;
		case Cmd::Session::PARA_DARE_GOLD:
			{
				Cmd::Session::t_dareGold_SceneSession* rev = (Cmd::Session::t_dareGold_SceneSession*)ptNullCmd;
				Cmd::Session::t_activeDare_SceneSession send;
#ifdef _ZJW_DEBUG				
				Zebra::logger->debug("%ld ���Ӳ��� : %d", rev->dwWarID, rev->dwNum);
#endif				

				SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (pUser == NULL)
				{
					return true;
				}

				send.dwWarID = rev->dwWarID;
				if (rev->dwNum>=0)
				{
					pUser->packs.addMoney(rev->dwNum, "��ս����",NULL, false);
					send.dwStatus = Cmd::Session::SCENE_ACTIVEDARE_SUCCESS;

					if (rev->dwType == Cmd::Session::RETURN_DARE_GOLD)
					{
						if (rev->dwWarType == Cmd::UNION_DARE 
								|| rev->dwWarType == Cmd::UNION_CITY_DARE)
						{
							Zebra::logger->trace("[���ս]: %s(%d) ������ս�� %d ��", 
									pUser->name, pUser->charbase.unionid, rev->dwNum);
						}
						else if (rev->dwWarType == Cmd::SEPT_DARE)
						{
							Zebra::logger->trace("[������־]: %s(%d) ������ս�� %d ��", 
									pUser->name, pUser->charbase.septid, rev->dwNum);
						}
					}
					else if (rev->dwType == Cmd::Session::WINNER_GOLD)
					{
						if (rev->dwWarType == Cmd::UNION_DARE 
								|| rev->dwWarType == Cmd::UNION_CITY_DARE)
						{
							Zebra::logger->trace("[���ս]: %s(%d) ��ս���� %d ��", 
									pUser->name, pUser->charbase.unionid, rev->dwNum);
						}
						else if (rev->dwWarType == Cmd::SEPT_DARE)
						{
							Zebra::logger->trace("[������־]: %s(%d) ��ս���� %d ��", 
									pUser->name, pUser->charbase.septid, rev->dwNum);
						}
					}
					else if (rev->dwType == Cmd::Session::EMPEROR_GOLD)
					{
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��ȡÿ�ս����� 5 ��");

						Zebra::logger->trace("[����]: %s ÿ�ս��� %d ��", 
								pUser->name, rev->dwNum);
					}
				}
				else if(rev->dwNum<0)
				{
					if (pUser->packs.checkMoney(abs(rev->dwNum)) && pUser->packs.removeMoney(abs(rev->dwNum),"�۳���ս��")) 
					{
						//Zebra::logger->trace("��ս���Ӹ��٣�%s([%s]�� [%s]����) �۳� %d ��", 
						//		pUser->name, pUser->unionName, pUser->septName, rev->dwNum);
						if (rev->dwType == Cmd::Session::DARE_GOLD)
						{
							if (rev->dwWarType == Cmd::UNION_DARE 
									|| rev->dwWarType == Cmd::UNION_CITY_DARE)
							{
								Zebra::logger->trace("[���ս]: %s(%d) �۳���ս�� %d ��", 
										pUser->name, pUser->charbase.unionid, rev->dwNum);
							}
							else if (rev->dwWarType == Cmd::SEPT_DARE)
							{
								Zebra::logger->trace("[������־]: %s(%d) �۳���ս�� %d ��", 
										pUser->name, pUser->charbase.septid, rev->dwNum);
							}
						}

						send.dwStatus = Cmd::Session::SCENE_ACTIVEDARE_SUCCESS;
					}
					else 
					{
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "���Ӳ���,���ܽ����ս");

						send.dwStatus = Cmd::Session::SCENE_ACTIVEDARE_FAIL;
					}
				}

				if (rev->dwWarID !=0)
				{
					sendCmd(&send, sizeof(Cmd::Session::t_activeDare_SceneSession));
				}

				return true;
			}
			break;
			case Cmd::Session::PARA_GOTO_LEADER:
			{
				Cmd::Session::t_GoTo_Leader_SceneSession *rev=(Cmd::Session::t_GoTo_Leader_SceneSession*)ptNullCmd;
				SceneUser *pUser=SceneUserManager::getMe().getUserByTempID(rev->leaderTempID);
				if(pUser && !pUser->guard)
				{
					char buffer[256];
					bzero(buffer,sizeof(buffer));
					sprintf(buffer,"name=%s pos=%d,%d",rev->mapName,rev->x,rev->y);
					Gm::gomap(pUser,buffer);
				}
				return true;
			}
			break;
			case Cmd::Session::PARA_GOTO_LEADER_CHECK:
			{
				Cmd::Session::t_GoTo_Leader_Check_SceneSession * rev =(Cmd::Session::t_GoTo_Leader_Check_SceneSession *)ptNullCmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(rev->userTempID);
				if(pUser && !pUser->guard)
				{
					Cmd::stDareCallDuty ret;
					ret.leaderTempID = rev->leaderTempID;
					ret.byCallerType = rev->type;
					strncpy(ret.mapName,rev->mapName,sizeof(ret.mapName));
					ret.x = rev->x;
					ret.y = rev->y;
					pUser->sendCmdToMe(&ret,sizeof(ret));
				}
				return true;
			}
			break;
			case Cmd::Session::PARA_RETURN_CALLTIMES_LEADER:
			{
				Cmd::Session::t_Return_CallTimes_SceneSession *rev = (Cmd::Session::t_Return_CallTimes_SceneSession*)ptNullCmd;
				SceneUser *pUser=SceneUserManager::getMe().getUserByTempID(rev->leaderTempID);
				if(pUser)
				{
					if ((pUser->privatestore.step() != PrivateStore::NONE)||
							(pUser->tradeorder.hasBegin())||
							(pUser->getState() == SceneUser::SceneEntry_Death))
					{
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "ʹ�õ��ߺ������׺Ͱ�̯");
						return false;
					}
					if(!rev->times)
					{
						switch(rev->type)
						{
							case Cmd::CALL_DUTY_KING:
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "���첻�����ù�������");
								}
								break;
							case Cmd::CALL_DUTY_UNION:
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "���첻�����ð������");
								}
								break;
							case Cmd::CALL_DUTY_SEPT:
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "���첻�����ü�������");
								}
								break;
						}
					}
					else
					{
						zObject *srcobj=pUser->packs.uom.getObjectByThisID(rev->qwThisID);
						if(srcobj && srcobj->data.pos.loc() ==Cmd::OBJECTCELLTYPE_COMMON)
						{
							pUser->useCallObj(srcobj);
						}
					}
				}
				return true;
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool SessionClient::cmdMsgParse_Country(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	switch (ptNullCmd->para)
	{
		case Cmd::Session::PARA_SET_CATCHER_STATE:
			{
				Cmd::Session::t_setCatcherState_SceneSession* rev =
					(Cmd::Session::t_setCatcherState_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				
				if (pUser)
				{
					pUser->setCatcherState(rev->byState);
					pUser->reSendMyMapData();
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_SET_DIPLOMAT_STATE:
			{
				Cmd::Session::t_setDiplomatState_SceneSession* rev = 
					(Cmd::Session::t_setDiplomatState_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (pUser)
				{
					pUser->setDiplomatState(rev->byState);
					pUser->reSendMyMapData();
				}
				
				return true;
			}
			break;
		case Cmd::Session::PARA_CHECK_USER:
			{
				Cmd::Session::t_checkUser_SceneSession* rev = 
					(Cmd::Session::t_checkUser_SceneSession*)ptNullCmd;

				SceneUser * pUser = SceneUserManager::getMe().getUserByID(rev->dwCheckedID);
				if (pUser->getGoodnessState() == Cmd::GOODNESS_2_1)
				{      
					rev->byState = 1;
				}
				else
				{
					rev->byState = 0;
				}

				sessionClient->sendCmd(rev, nCmdLen);
				
				return true;
			}
			break;
		case Cmd::Session::PARA_COUNTRY_POWER_SORT:
			{
				Cmd::Session::t_countryPowerSort_SceneSession * rev = (Cmd::Session::t_countryPowerSort_SceneSession *)ptNullCmd;
				for(int i=0; i<13;i++) ScenesService::getInstance().countryPower[i] = rev->country[i];
				return true;
			}
			break;
		case Cmd::Session::PARA_REFRESH_GEN:
			{
				Cmd::Session::t_refreshGen_SceneSession *rev = (Cmd::Session::t_refreshGen_SceneSession *)ptNullCmd;
				Scene * s = SceneManager::getInstance().getSceneByID((rev->dwCountryID<<16)+139);
				if (s)
				{
					SceneNpc * old = s->bossMap[COUNTRY_MAIN_GEN];

					if (0==rev->level) rev->level = 1;//0����1��
					zNpcB *base = npcbm.get(COUNTRY_MAIN_GEN+(rev->level-1)*10);
					//zNpcB *base = npcbm.get(COUNTRY_MAIN_GEN);
					if (0==base)
					{
						Zebra::logger->error("PARA_REFRESH_GEN: �ٻ�npcʱ�Ҳ���NPC id=%d", COUNTRY_MAIN_GEN+rev->level);
						return true;
					}

					t_NpcDefine define;
					define.id = base->id;
					strcpy(define.name, base->name);
					define.pos = old?old->getPos():zPos(492, 494);
					define.num = 1;
					define.interval = 5;
					define.initstate = zSceneEntry::SceneEntry_Normal;
					define.width = 2;
					define.height = 2;
					define.scriptID = 0;
					s->initRegion(define.region, define.pos, define.width, define.height);

					SceneNpc * boss = s->summonOneNpc<SceneNpc>(define, define.pos, base);
					if (!boss)
					{
						Zebra::logger->debug("PARA_REFRESH_GEN: �ٻ�NPCʧ�� id=%d", base->id);
						return true;
					}

					if (old) old->setClearState();
					s->bossMap[COUNTRY_MAIN_GEN] = boss;

					Zebra::logger->trace("ˢ�´󽫾� map=%s level=%u pos=(%u,%u)", s->name, rev->level, boss->getPos().x, boss->getPos().y);
				}
				else
					Zebra::logger->error("ˢ�´󽫾�ʱû�ҵ���ͼ mapID=%u", (rev->dwCountryID<<16)+139);

				return true;
			}
			break;
		case Cmd::Session::PARA_SET_EMPEROR_HOLD:
			{
				Cmd::Session::t_setEmperorHold_SceneSession* rev = 
					(Cmd::Session::t_setEmperorHold_SceneSession*)ptNullCmd;

				Scene* pScene = SceneManager::getInstance().getSceneByID(SceneManager::getInstance().buildMapID(6, 134));
				
				if (pScene)
				{
					pScene->setHoldCountry(rev->dwCountryID);
				}
				
				return true;
			}
			break;
		case Cmd::Session::PARA_SUMMON_ALLY_NPC:
				{
					Cmd::Session::t_summonAllyNpc_SceneSession* rev = (Cmd::Session::t_summonAllyNpc_SceneSession*)ptNullCmd;

					char mapName[MAX_NAMESIZE];
					bzero(mapName, MAX_NAMESIZE);
					snprintf(mapName, MAX_NAMESIZE, "%s������", SceneManager::getInstance().getCountryNameByCountryID(rev->dwCountryID));

					Scene * scene = SceneManager::getInstance().getSceneByName(mapName);
					if (!scene)
					{
							Zebra::logger->debug("PARA_SUMMON_ALLY_NPC: �ٻ�npcʱ�Ҳ�����ͼ mapName=%s", mapName);
							return false;
					}
					zNpcB *base = npcbm.get(ALLY_GUARDNPC);
					if (NULL == base)
					{
							Zebra::logger->debug("PARA_SUMMON_ALLY_NPC: �ٻ�npcʱ�Ҳ���NPC id=%d", ALLY_GUARDNPC);
							return false;
					}

					t_NpcDefine define;
					define.id = base->id;
					strcpy(define.name, base->name);
					define.pos = zPos(472, 474);
					define.num = 5;
					define.interval = 5;
					define.initstate = zSceneEntry::SceneEntry_Normal;
					define.width = 2;
					define.height = 2;
					define.scriptID = 9001;
					scene->initRegion(define.region, define.pos, define.width, define.height);

					int num = scene->summonNpc(define, define.pos, base);
					if (num<5)
					{
							define.num = 5-num;
							scene->summonNpc(define, define.pos, base);
					}

					Zebra::logger->debug("%s �˹��ڳ�����",
									SceneManager::getInstance().getCountryNameByCountryID(scene->getCountryID()));
					return true;
				}
				break;

		case Cmd::Session::PARA_UPDATE_ALLY:
			{
				Cmd::Session::t_updateAlly_SceneSession* rev =
					(Cmd::Session::t_updateAlly_SceneSession*)ptNullCmd;
				CountryAllyM::getMe().processUpdate(rev);
				
				return true;
			}
			break;
		case Cmd::Session::PARA_UPDATE_SCENE_UNION:
			{
				Cmd::Session::t_updateSceneUnion_SceneSession* rev = 
					(Cmd::Session::t_updateSceneUnion_SceneSession*)ptNullCmd;

				Scene* pScene = SceneManager::getInstance().getSceneByID(rev->dwSceneID);
				if (pScene)
				{
					pScene->setHoldUnion(rev->dwUnionID);
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_UPDATE_TECH:
			{
				Cmd::Session::t_updateTech_SceneSession* rev = 
					(Cmd::Session::t_updateTech_SceneSession*)ptNullCmd;
				CountryTech* pCountryTech = CountryTechM::getMe().getCountryTech(rev->dwCountryID);

				if (pCountryTech == NULL)
				{
					CountryTechM::getMe().addCountryTech(rev->dwCountryID);
					pCountryTech = CountryTechM::getMe().getCountryTech(rev->dwCountryID);
				}

				if (pCountryTech)
				{
					pCountryTech->init(rev);
				}
				else
				{
					Zebra::logger->trace("[����]: %d δ�ҵ���Ӧ���ҿƼ���Ϣ��", rev->dwCountryID);
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_SEL_TRANS_COUNTRY_WAR:
			{
				Cmd::Session::t_selTransCountryWar_SceneSession* rev =
					(Cmd::Session::t_selTransCountryWar_SceneSession*)ptNullCmd;
				SceneUserManager::getMe().countryTrans(rev->dwCountryID, rev->dwLevel);
				return true;
			}
			break;
		case Cmd::Session::PARA_COUNTRY_PUNISH_USER:
			{
				Cmd::Session::t_countryPunishUser_SceneSession * rev = (Cmd::Session::t_countryPunishUser_SceneSession *)ptNullCmd;
				SceneUser * pUser = SceneUserManager::getMe().getUserByName(rev->name);
				if (!pUser) return true;

				if (1==rev->method)//����
				{
					pUser->delayForbidTalk(3600);
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�㱻��������һСʱ, һСʱ֮���޷����κ�Ƶ������");
				}
				else if (2==rev->method)//�ؼ���
				{
					if (pUser->scene->getRealMapID()!=189 && pUser->scene->getRealMapID()!=203)
					{
						pUser->charbase.punishTime = 60;
						Scene * s=SceneManager::getInstance().getSceneByName("������������");
						if (s)
							pUser->changeMap(s, zPos(80,70));
						else
						{
							Cmd::Session::t_changeScene_SceneSession cmd;
							cmd.id = pUser->id;
							cmd.temp_id = pUser->tempid;
							cmd.x = 80;
							cmd.y = 70;
							cmd.map_id = 0;
							cmd.map_file[0] = '\0';
							strncpy((char *)cmd.map_name, "������������", MAX_NAMESIZE);
							sessionClient->sendCmd(&cmd, sizeof(cmd));
						}
					}
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_SET_COUNTRY_WAR:
			{
					Cmd::Session::t_setCountryWar_SceneSession* rev = 
							(Cmd::Session::t_setCountryWar_SceneSession*)ptNullCmd;

					Scene* pScene = SceneManager::getInstance().getSceneByID(		                                                SceneManager::getInstance().buildMapID(rev->dwCountryID, rev->dwAreaID));

					if (pScene)
					{
							if (rev->byStatus == 1)
							{

								pScene->setCountryDare(true/*, rev->dwAttCountryID*/);
							}
							else
							{
								pScene->setCountryDare(false);
								pScene->reliveSecGen();
							}
					}

					return true;
			}
			break;
		case Cmd::Session::PARA_WINNER_EXP_SCENE_COUNTRY:
			{
					Cmd::Session::t_updateWinnerExp_SceneSession *rev = 
							(Cmd::Session::t_updateWinnerExp_SceneSession*)ptNullCmd;
					struct WinnerExpSceneExec :public SceneCallBack
					{
							WinnerExpSceneExec(DWORD id,bool type)
							{
									_countryid = id;
									_type = type;
							}
							DWORD _countryid;
							bool _type;
							bool exec(Scene *scene)
							{
									if(_countryid == scene->getCountryID())
									{
											scene->winner_exp = _type;
									}
									return true;
							}
					};
					WinnerExpSceneExec exec(rev->countryID , rev->type); 
					SceneManager::getInstance().execEveryScene(exec);
					return true;
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool SessionClient::cmdMsgParse_Temp(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Session;
	using namespace Cmd::Record;

	switch (ptNullCmd->para)
	{
		case Cmd::Session::GET_CREATE_UNION_ITEM_PARA:
			{
				Cmd::Session::t_GetCreateUnionItem_SceneSession* rev = 
					(Cmd::Session::t_GetCreateUnionItem_SceneSession*)ptNullCmd;

				Cmd::Session::t_ReturnCreateUnionItem_SceneSession send;
				send.dwUserID = rev->dwUserID;

				zObject* item = zObject::create(objectbm.get(UnionDef::CREATE_UNION_NEED_ITEM_ID), 1); 

				if (item)
				{
					zObject::logger(item->createid,item->data.qwThisID,item->data.strName,item->data.dwNum,item->data.dwNum,1,0,"��Ὠ������",rev->dwUserID,NULL,"����",item->base,item->data.kind,item->data.upgrade);
					zObject::logger(item->createid,item->data.qwThisID,item->data.strName,item->data.dwNum,item->data.dwNum,0,0,"��Ὠ������",rev->dwUserID,NULL,"�ʼĵ�����",item->base,item->data.kind,item->data.upgrade);
					item->getSaveData((SaveObject *)&send.item);
					sessionClient->sendCmd(&send, sizeof(send));
				}

				zObject::destroy(item);

				return true;
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool SessionClient::cmdMsgParse_Union(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Session;
	switch (ptNullCmd->para)
	{
		case Cmd::Session::PARA_CHECK_USER_CITY:
			{
				Cmd::Session::t_checkUserCity_SceneSession* rev = 
					(Cmd::Session::t_checkUserCity_SceneSession*)ptNullCmd;

				SceneUser * pUser = SceneUserManager::getMe().getUserByID(rev->dwCheckedID);
				if (pUser->getGoodnessState() == Cmd::GOODNESS_2_1)
				{      
					rev->byState = 1;
				}
				else
				{
					rev->byState = 0;
				}

				sessionClient->sendCmd(rev, nCmdLen);

		Zebra::logger->debug("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
		Zebra::logger->debug("��������%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
				return true;
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool SessionClient::cmdMsgParse_Sept(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Session;
	switch (ptNullCmd->para)
	{
		case Cmd::Session::SEND_SEPT_NORMAL_PARA:
			{
				t_SendSeptNormal_SceneSession* rev = (t_SendSeptNormal_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				
				if (pUser)
				{
					pUser->dwSeptRepute = rev->dwRepute;
					pUser->sendMeToNine();
				}
				
				return true;
			}
			break;
		case Cmd::Session::GET_SEPT_EXP_PARA:
			{
				t_GetSeptExp_SceneSession* rev = (t_GetSeptExp_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (pUser)
				{
					pUser->addNineSeptExp(rev->dwSeptID);
				}

				return true;
			}
			break;
		case Cmd::Session::GET_SEPT_NORMAL_EXP_PARA:
			{
				t_GetSeptNormalExp_SceneSession* rev = (t_GetSeptNormalExp_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (pUser)
				{
					pUser->addNineSeptNormalExp(rev->dwSeptID);
				}

				return true;
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool SessionClient::cmdMsgParse_Other(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Session;
	using namespace Cmd::Record;
	typedef std::map<DWORD, DWORD>::value_type valType;

	switch (ptNullCmd->para)
	{
			/*
		case PARA_SCENE_LOAD_PROCESS:
			{
				Zebra::logger->debug("�����������ļ�...%u �ֽ�", ScenesService::updateStampData());
				return true;
			}
			break;
			*/
		case Cmd::Session::PICKUP_MASTER_SCENE_PARA:
			{
				Cmd::Session::t_PickupMaster_SceneSession* rev = 
					(Cmd::Session::t_PickupMaster_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (!pUser)
				{
					return true;
				}

				pUser->packs.addMoney(rev->dwMoney, "�Ƽ�����ȡ����", NULL, true);

				return true;
			}
			break;
		case Cmd::Session::PARA_CLOSE_NPC:
			{
				SceneNpcManager::getMe().closeFunctionNpc();
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_SEND_CMD:
			{
				Cmd::Session::t_sendCmd_SceneSession * rev = (t_sendCmd_SceneSession *)ptNullCmd;
				Scene * s = SceneManager::getInstance().getSceneByID(rev->mapID);
				if (!s) return false;

				s->sendCmdToScene(rev->cmd, rev->len);
				return true;
			}
			break;
		case Cmd::Session::PARA_SET_SERVICE:
			{
				Cmd::Session::t_SetService_SceneSession *rev = (Cmd::Session::t_SetService_SceneSession *)ptNullCmd;

				DWORD old = atoi(Zebra::global["service_flag"].c_str());

				char buf[32];
				bzero(buf, sizeof(buf));
				snprintf(buf, 32, "%u", rev->flag);
				Zebra::global["service_flag"] = buf;
				Zebra::logger->debug("���÷��� flag=%s", buf);

				if (((old^rev->flag)&Cmd::Session::SERVICE_PROCESS) && ScenesService::pStampData)
				{
					if (rev->flag & Cmd::Session::SERVICE_PROCESS)
						ScenesService::updateStampData();
					else
						ScenesService::pStampData->dwChannelID = false;
					Zebra::logger->debug("%sprocess����",ScenesService::pStampData->dwChannelID?"��":"�ر�");
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_GUARD_FAIL:
			{
				Cmd::Session::t_guardFail_SceneSession * rev = (Cmd::Session::t_guardFail_SceneSession *)ptNullCmd;
				SceneUser *pUser=SceneUserManager::getMe().getUserByTempID(rev->userID);
				if (pUser)
				{
					OnOther event(2);
					EventTable::instance().execute(*pUser, event);
					Channel::sendSys(pUser, Cmd::INFO_TYPE_EXP, "��Ļ���Ŀ������");
				}
			}
			break;
		case Cmd::Session::PARA_UNION_DARE_NOTIFY:
			{
				Cmd::Session::t_unionDareNotify_SceneSession *rev = (Cmd::Session::t_unionDareNotify_SceneSession*)ptNullCmd;
				Scene * scene = SceneManager::getInstance().getSceneByID(rev->sceneID);
				if(!scene) return true;

				scene->setUnionDare(rev->state);
				Zebra::logger->debug("�յ����״̬��Ϣ state=%u", rev->state);
				return true;
			}
			break;
			// ��ʦ���������
		case Cmd::Session::OVERMAN_TICKET_ADD:
			{
				Cmd::Session::t_OvermanTicketAdd *rev = (Cmd::Session::t_OvermanTicketAdd*)ptNullCmd;
				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->id);
				if(pUser)
				{
					std::string disc = "���ѽ���:"; 
					disc += rev->name;
					pUser->packs.addTicket(rev->ticket,disc.c_str(),rev->name);
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_CLEAR_UNION_MANOR:
			{
				Cmd::Session::t_clearUnionManor_SceneSession* rev = 
					(Cmd::Session::t_clearUnionManor_SceneSession*)ptNullCmd;

				Scene* pScene = SceneManager::getInstance().getSceneByID(
						SceneManager::getInstance().buildMapID(rev->dwCountryID, rev->dwAreaID));

				if (pScene)
				{
					ClearAllNotUnion clearMaron(rev->dwUnionID);
					pScene->execAllOfScene(zSceneEntry::SceneEntry_Player, clearMaron);
					clearMaron.reliveAll();
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_RETURN_ENTER_UNION_MANOR:
			{
				Cmd::Session::t_returnEnterUnionManor_SceneSession* rev = 
					(Cmd::Session::t_returnEnterUnionManor_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				Cmd::Session::t_changeScene_SceneSession cmd;
				Scene* pScene= SceneManager::getInstance().getSceneByID(SceneManager::getInstance().buildMapID(rev->dwCountryID, rev->dwAreaID));

				if (pUser) 
				{
					if (pUser->isAtt(Cmd::UNION_CITY_DARE))
					{
						pUser->charbase.gomaptype = ZoneTypeDef::ZONE_PRIVATE_DARE_UNION;
					}
					else
					{
						if (pUser->isSpecWar(Cmd::UNION_CITY_DARE))
						{//��սʱ���ط���������
							pUser->charbase.gomaptype = ZoneTypeDef::ZONE_PRIVATE_UNION; // �����������ת������
						}
						else
						{
							if (pUser->scene->getUnionDare())
							{// ��սʱ����������������
								pUser->charbase.gomaptype = 
									ZoneTypeDef::ZONE_PRIVATE_THIRD_UNION;
							}
							else
							{
								// �����������ת������
								pUser->charbase.gomaptype = ZoneTypeDef::ZONE_PRIVATE_UNION;
							}
						}
					}
				}

				if (pScene && pUser)
				{//����
					zPos Pos;
					Pos.x = 0;
					Pos.y = 0;

					pUser->changeMap(pScene, Pos);
				}
				else if (pUser)
				{

					cmd.id = pUser->id;
					cmd.temp_id = pUser->tempid;
					cmd.x = 0;
					cmd.y = 0;
					cmd.map_id = SceneManager::getInstance().buildMapID(rev->dwCountryID, rev->dwAreaID);
					bzero(cmd.map_file, sizeof(cmd.map_file));
					bzero(cmd.map_name, sizeof(cmd.map_file));
					sessionClient->sendCmd(&cmd, sizeof(cmd));
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_TRANS_DARE_COUNTRY:
			{
				Cmd::Session::t_transDareCountry_SceneSession* rev = 
					(Cmd::Session::t_transDareCountry_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (rev->dwMoney<=0 || (pUser->packs.checkMoney(abs(rev->dwMoney)) 
							&& pUser->packs.removeMoney(abs(rev->dwMoney),"��ս����"))) 
				{
					Cmd::Session::t_changeScene_SceneSession cmd;
					pUser->charbase.gomaptype = ZoneTypeDef::ZONE_PRIVATE_DARE; // ��ս��ת����

					cmd.id = pUser->id;
					cmd.temp_id = pUser->tempid;
					cmd.x = 0;
					cmd.y = 0;
					cmd.map_id = SceneManager::getInstance().buildMapID(rev->dwCountry, 139); //��˫��

					bzero(cmd.map_file, sizeof(cmd.map_file));
					bzero(cmd.map_name, sizeof(cmd.map_file));
					sessionClient->sendCmd(&cmd, sizeof(cmd));
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_RETURN_CHANGE_COUNTRY:
			{
				Cmd::Session::t_returnChangeCountry_SceneSession* rev = (Cmd::Session::
						t_returnChangeCountry_SceneSession*)ptNullCmd;

				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (pUser) pUser->relive();
				return true;
			}
			break;
		case Cmd::Session::PARA_QUIZ_AWARD:
			{
				Cmd::Session::t_quizAward_SceneSession* rev = (Cmd::Session::t_quizAward_SceneSession*)ptNullCmd;
				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (pUser)
				{
					DWORD addExp = rev->dwExp;

					if (rev->byType == 1)
					{
						int cur_answer_count = 0;
						if ((int)pUser->charbase.answerCount>1)
						{
							cur_answer_count = pUser->charbase.answerCount - 1;
						}
						else
						{
							cur_answer_count = 0;
						}
						//int(0.14*�������*��ɫ�ȼ�^2*(0.75+0.25*N)+200)
				//		addExp = (DWORD)(addExp*(0.75+0.25*cur_answer_count)) + 200;
				
					//int(0.2*�������*��ɫ�ȼ�^2*(0.8+0.2*N)+200)
					//	addExp = (DWORD)(addExp*(0.8+0.2*cur_answer_count)) + 200;

//ǰ4�δ�����������=int(��0.12+0.05*n��*�������*��ɫ�ȼ�^2 +200)
//  ����δ�����������=int(1*�������*��ɫ�ȼ�^2 +200)
//addExp=�������*��ɫ�ȼ�^2
						if ( pUser->charbase.answerCount > 5 )
							addExp = (DWORD)(addExp) + 200;
						else
							addExp = (DWORD)(addExp*(0.12+0.05*cur_answer_count)) + 200;
						Zebra::logger->trace("[���˴���]: %s(%u) ��þ��� %d", pUser->name, pUser->id, addExp);
						struct tm tv1;
						time_t timValue = time(NULL);
						zRTime::getLocalTime(tv1, timValue);

/*
						//tv1.tm_wday = (1-7)
						//��������
	                                        if (pUser->charbase.answerCount==5 && tv1.tm_wday ==3)
        	                                {
							
                	                                zObjectB *base = NULL;
                        	                        base = objectbm.get(1986);
                                	                if(!base)
                                        	                Zebra::logger->error("�޷���ȡ��Ʒ");
                                                	zObject* o = NULL;
	                                                o = zObject::create(base,1, 0);
        	                                        if(o)
                	                                {
                        	                                Zebra::logger->debug("create");
                                	                        zObject::logger(o->createid,o->data.qwThisID,o->data.strName,o->data.dwNum,o->data.dwNum,1,0,NULL,pUser->id,pUser->name,"fetch",o->base,o->data.kind,o->data.upgrade);
                                        	                if(o->data.dwNum!=0 && pUser->packs.addObject(o,true, AUTO_PACK))
                                                	        {
                                                        	        Cmd::stAddObjectPropertyUserCmd send;
                                                                	bcopy(&o->data,&send.object,sizeof(t_Object));
                                                                	pUser->sendCmdToMe(&send,sizeof(send));
                                                                	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�õ���Ʒ%s%ld��",o->name,o->data.dwNum);

                  	                                        }
                                                	}
                                        	}*/
					}
					else
					{
						Zebra::logger->trace("[ȫ������]: %s(%u) ��þ��� %d", pUser->name, pUser->id, addExp);
					}

					pUser->addExp(addExp);
					Channel::sendSys(pUser, Cmd::INFO_TYPE_EXP, "�õ�����ֵ %d", addExp);

					if (rev->dwMoney>0)
					{
						pUser->packs.addMoney(rev->dwMoney, "��������",NULL);
					}


					if (pUser->charbase.country != PUBLIC_COUNTRY)
					{
						pUser->charbase.grace += rev->dwGrace;
					}

					// �ָ��ɼ�
					pUser->setState(zSceneEntry::SceneEntry_Normal);
					zPos curPos = pUser->getPos();
					pUser->goTo(curPos);
					pUser->isQuiz = false;
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_QUESTION_OBJECT:
			{
				Cmd::Session::t_questionObject_SceneSession* rev = 
					(Cmd::Session::t_questionObject_SceneSession*)ptNullCmd;
				Cmd::Session::t_returnObject_SceneSession send;

				SceneUser* pUser = SceneUserManager::getMe().getUserByName(rev->from_name);
#ifdef _ZJW_DEBUG
				Zebra::logger->debug("�յ���Ʒ��ѯ����: %s ��ѯ %s ��(%d)", rev->to_name, rev->from_name, 
						rev->dwObjectTempID);
#endif				

				if (pUser)
				{
					zObject* tempObject = pUser->packs.uom.getObjectByThisID(rev->dwObjectTempID);
					strncpy(send.from_name, rev->from_name, MAX_NAMESIZE);
					strncpy(send.to_name, rev->to_name, MAX_NAMESIZE);

					if (tempObject)
					{
						memcpy(&send.object, &tempObject->data, sizeof(t_Object));
					}
					sendCmd(&send, sizeof(Cmd::Session::t_returnObject_SceneSession));
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_UPDATE_COUNTRY_STAR:
			{
				/*Cmd::Session::t_updateCountryStar* rev = (Cmd::Session::t_updateCountryStar*)ptNullCmd;
				  SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				  if (pUser)
				  {
				  pUser->star = rev->dwCountryStar;
				  pUser->reSendMyMapData();
				  }
				  else
				  {
				  Zebra::logger->error("��Ч���û�ID��ȡ��������ż");
				  }*/

				return true;
			}
			break;
		case Cmd::Session::PARA_UPDATE_CONSORT:
			{
				Cmd::Session::t_updateConsort* rev = (Cmd::Session::t_updateConsort*)ptNullCmd;
				SceneUser* pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);

				if (pUser)
				{
					pUser->charbase.consort = rev->dwConsort;
					pUser->kingConsort = rev->byKingConsort;

					if (pUser->kingConsort!=0)
					{
						pUser->sendMeToNine();
						pUser->sendNineToMe();
					}
				}
				else
				{
					//Zebra::logger->error("��Ч���û�ID��ȡ��������ż");
				}

				return true;
			}
			break;
		case Cmd::Session::PARA_NOTIFY_NPC_HOLD_DATA:
			{
				Cmd::Session::t_notifyNpcHoldData* rev = (Cmd::Session::t_notifyNpcHoldData*)ptNullCmd;
				SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (pUser == NULL)	return true;
				pUser->setNpcHoldData(rev);
				return true;
			}
			break;
		case Cmd::Session::PARA_NOTIFY_ADD_INTEGRAL:
			{
				Cmd::Session::t_notifyAddIntegral* rev = (Cmd::Session::t_notifyAddIntegral*)ptNullCmd;
				SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwUserID);
				SceneUser *pGoldUser=SceneUserManager::getMe().getUserByID(rev->dwGoldUser);
				if (pUser == NULL || pGoldUser == NULL)	return true;
				pUser->packs.addTicket(rev->dwNum,std::string(std::string(pGoldUser->name)+std::string("���Ľ�Ҽ����͵�")).c_str(),pGoldUser->name);
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_REGSCENE_RET:
			{
				Cmd::Session::t_regScene_ret_SceneSession *rev=(Cmd::Session::t_regScene_ret_SceneSession *)ptNullCmd;
				Scene * scene=SceneManager::getInstance().getSceneByTempID(rev->dwTempID);
				if (scene)
				{
					if(rev->byValue==Cmd::Session::REGSCENE_RET_REGOK)
						Zebra::logger->info("ע�� %s �ɹ�",scene->name);
					else if(rev->byValue==Cmd::Session::REGSCENE_RET_REGERR)
					{
						Zebra::logger->error("ע�� %s ʧ��",scene->name);
						SceneManager::getInstance().unloadScene(scene);
					}
					return true;
				}
				else
				{
					Zebra::logger->error("ע�᳡��%ldʱδ�ҵ���",rev->dwTempID);
					if(rev->byValue==Cmd::Session::REGSCENE_RET_REGOK)
					{
						Cmd::Session::t_regScene_ret_SceneSession ret;
						ret.dwTempID=rev->dwTempID;
						ret.byValue=Cmd::Session::REGSCENE_RET_REGERR;
						sendCmd(&ret,sizeof(ret));
					}
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_UNREGUSER:
			{
				t_unregUser_SceneSession *rev=(t_unregUser_SceneSession *)ptNullCmd;
				Scene *scene=SceneManager::getInstance() .getSceneByTempID(rev->dwSceneTempID);
				if(scene)
				{
					SceneUser *pUser=SceneUserManager::getMe().getUserByIDOut(rev->dwUserID);
					if(pUser)
					{
						Cmd::Record::t_RemoveUser_SceneRecord rec_ret;
						rec_ret.accid = pUser->accid;
						rec_ret.id = pUser->id;
						recordClient->sendCmd(&rec_ret, sizeof(rec_ret));
						if(rev->retcode==Cmd::Session::UNREGUSER_RET_LOGOUT)
						{
							Cmd::Scene::t_Unreg_LoginScene retgate;
							retgate.dwUserID=pUser->id;
							retgate.dwSceneTempID=rev->dwSceneTempID;
							retgate.retcode=Cmd::Scene::UNREGUSER_RET_LOGOUT;
							pUser->gatetask->sendCmd(&retgate,sizeof(retgate));
							// */
						}

						pUser->unreg();
						Zebra::logger->debug("Session����ע��(%s,%u)",pUser->name , pUser->id);
						Zebra::logger->debug("�û�%ldע��ʱδ�ҵ����ڵ�ͼ%s,�������û����������ҵ�",rev->dwUserID,scene->name);
					}
					else 
					{
						pUser=SceneUserManager::getMe().getUserByID(rev->dwUserID);
						if(pUser)
						{
							OnQuit event(1);
							EventTable::instance().execute(*pUser, event);
//							execute_script_event(pUser,"quit");

							Zebra::logger->debug("Session����ע��(%s,%u)",pUser->name , pUser->id);
							pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
							//pUser->killAllPets();
							if(rev->retcode==Cmd::Session::UNREGUSER_RET_LOGOUT)
							{
								Cmd::Scene::t_Unreg_LoginScene retgate;
								retgate.dwUserID=pUser->id;
								retgate.dwSceneTempID=rev->dwSceneTempID;
								retgate.retcode=Cmd::Scene::UNREGUSER_RET_LOGOUT;
								pUser->gatetask->sendCmd(&retgate,sizeof(retgate));
								// */
							}
							else if(rev->retcode==Cmd::Session::UNREGUSER_RET_ERROR)
							{
								Zebra::logger->debug("�յ�Session�㲥��Ϣע���û�(name=%s,id=%u,tempid=%u",pUser->name , pUser->id , pUser->tempid);
							}

							pUser->unreg();
							//SAFE_DELETE(pUser);

							//						static_cast<SceneUserVisitor *>(pUser)->destroy();
							//						SceneUserManager::getMe().unlock();
						}
					}
					return true;
				}
				else
					Zebra::logger->error("δ�ҵ���ͼ%ld",rev->dwSceneTempID);
				if(rev->retcode==Cmd::Session::UNREGUSER_RET_LOGOUT)
				{
					// ֪ͨSession ע��ʧ��
					Cmd::Session::t_unregUser_SceneSession ret;
					Zebra::logger->debug("Session����ע��,��δ�ҵ���ͼ");
					ret.dwSceneTempID=rev->dwSceneTempID;
					ret.dwUserID=rev->dwUserID;
					ret.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
					sendCmd(&ret,sizeof(ret));
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_REGUSER:
			{
				t_regUser_SceneSession *rev=(t_regUser_SceneSession *)ptNullCmd;
				Scene *scene=SceneManager::getInstance() .getSceneByName((char *)rev->byMapName);
				SceneTask *gate=SceneTaskManager::getInstance().uniqueGet(rev->dwGatewayServerID);
				if(!scene)
				{       
					char map[MAX_NAMESIZE+1];
					bzero(map,sizeof(map));
					bcopy(rev->byMapName,map,6);
					bcopy("��Դ��",&map[6],6);
					scene=SceneManager::getInstance().getSceneByName(map);
				}   
				if(scene)
				{
					if(gate)
					{
						// ����û�
						//						SceneUser *pUser=new SceneUserVisitor(rev->accid);
						SceneRecycleUserManager::getInstance().refresh();
						if(SceneRecycleUserManager::getInstance().canReg(rev->dwID))
						{
							SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwID);
							if(pUser)
							{
								OnQuit event(1);
								EventTable::instance().execute(*pUser, event);
	//							execute_script_event(pUser,"quit");

								pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
								//pUser->killAllPets();
								//֪ͨsession
								Cmd::Session::t_unregUser_SceneSession rets;
								rets.dwUserID=pUser->id;
								rets.dwSceneTempID=pUser->scene->tempid;
								rets.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
								sessionClient->sendCmd(&rets,sizeof(rets));

								//֪ͨ����
								Cmd::Scene::t_Unreg_LoginScene retgate;
								retgate.dwUserID = pUser->id;
								retgate.dwSceneTempID = pUser->scene->tempid;
								retgate.retcode = Cmd::Scene::UNREGUSER_RET_ERROR;
								//pUser->gatetask->sendCmd(&retgate,sizeof(retgate));
								SceneTaskManager::getInstance().broadcastCmd(&retgate,sizeof(retgate));
								Zebra::logger->debug("�����ظ��û�(%s,%u)",pUser->name , pUser->id);

								pUser->unreg();
								//SAFE_DELETE(pUser);

								//						static_cast<SceneUserVisitor *>(pUser)->destroy();
								//						SceneUserManager::getMe().unlock();
								return true;
							}
							pUser=SceneUserManager::getMe().getUserByIDOut(rev->dwID);
							if(pUser)
							{
								// ֪ͨRecord ��ȡʧ��
								Cmd::Record::t_RemoveUser_SceneRecord rec_ret;
								rec_ret.accid = pUser->accid;
								rec_ret.id = pUser->id;
								recordClient->sendCmd(&rec_ret, sizeof(rec_ret));
								// ֪ͨSession ע��ʧ��
								Cmd::Session::t_unregUser_SceneSession ret_session;
								ret_session.dwSceneTempID=rev->dwMapID;
								ret_session.dwUserID=pUser->id;
								ret_session.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
								sessionClient->sendCmd(&ret_session,sizeof(ret_session));
								// ֪ͨGateway ע��ʧ��
								Cmd::Scene::t_Unreg_LoginScene ret_gate;
								ret_gate.dwUserID=pUser->id;
								ret_gate.dwSceneTempID=rev->dwMapID;
								ret_gate.retcode=Cmd::Scene::UNREGUSER_RET_ERROR;
								//pUser->gatetask->sendCmd(&ret_gate,sizeof(ret_gate));
								SceneTaskManager::getInstance().broadcastCmd(&ret_gate,sizeof(ret_gate));
								SceneUserManager::getMe().removeUser(pUser);
								Zebra::logger->debug("�����ظ��û�,���Ҵ������ڶ���(%s,%u)",pUser->name , pUser->id);
								pUser->destroy();
								SAFE_DELETE(pUser);

								//						static_cast<SceneUserVisitor *>(pUser)->destroy();
								return true;
							}
							pUser=new SceneUser(rev->accid);
							pUser->id=rev->dwID;
							pUser->tempid=rev->dwTempID;
							strncpy(pUser->name,(char *)rev->byName,MAX_NAMESIZE);
							//pUser->scene=scene;
							pUser->gatetask=gate;
							if(SceneUserManager::getMe().addUser(pUser))
							{
								// ������
								t_ReadUser_SceneRecord send;
								send.accid=pUser->accid;
								send.id=pUser->id;
								send.dwMapTempID=scene->tempid;
								recordClient->sendCmd(&send,sizeof(send));
								//Zebra::logger->debug("��ʼ��ȡ%ld(%s)����",pUser->id,pUser->name);

								//����Ȩ��
#ifdef _ALL_SUPER_GM
								if (Zebra::global["gm_mode"]=="true")
										pUser->setPriv(Gm::debug_mode);//�����˶��ǳ���GM
								else if (Zebra::global["gm_mode"]=="100_debug"
												&& pUser->id>0
												&& pUser->id<=100)
										pUser->setPriv(Gm::debug_mode);//ǰ100���ǳ���GM
								else
#endif
								{
										if (SUPER_GM_ID == pUser->id)
												pUser->setPriv(Gm::super_mode);//����GM
										else if (pUser->id>=2&&pUser->id<=15)
												pUser->setPriv(Gm::captain_mode);//�鳤
										else if (pUser->id>=16&&pUser->id<=100)
												pUser->setPriv(Gm::gm_mode);//��ͨGM
										else
												pUser->setPriv(Gm::normal_mode);//����ģʽ
								}
								return true;
							}
							else
							{
								//scene->removeUser(pUser);
								Zebra::logger->fatal("�ظ��û�,������δ�����û����� (%ld,%ld,%ld,%s)",
										pUser->tempid,pUser->accid,pUser->id,pUser->name);
								SceneUser *u = SceneUserManager::getMe().getUserByID(pUser->id);
								if(u)
								{
									Zebra::logger->debug("id�ظ�(%u,%u,%s)",u->id,u->tempid,u->name);
								}
								u = SceneUserManager::getMe().getUserByTempID(pUser->tempid);
								if(u)
								{
									Zebra::logger->debug("tempid�ظ�(%u,%u,%s)",u->id,u->tempid,u->name);
								}
								u = SceneUserManager::getMe().getUserByName(pUser->name);
								if(u)
								{
									Zebra::logger->debug("name�ظ�(%u,%u,%s)",u->id,u->tempid,u->name);
								}

								pUser->destroy();
								SAFE_DELETE(pUser);

								//							static_cast<SceneUserVisitor *>(pUser)->destroy();
							}
						}
						else
						{
							Zebra::logger->trace("�˳��ȴ����յ����ɵ�ѡ��ָ�� %d",rev->dwID);
						}
					}
					else
					{
						Zebra::logger->fatal("���ضϵ��� %d",rev->dwGatewayServerID);
					}
				}
				else
					Zebra::logger->fatal("δ�ҵ�ͼ %s",(char *)rev->byMapName);
				// ֪ͨSession������ע��ʧ��

				Cmd::Session::t_unregUser_SceneSession ret;
				ret.dwUserID=rev->dwID;
				if(scene)
					ret.dwSceneTempID=scene->tempid;
				else
					ret.dwSceneTempID=0;
				ret.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
				Zebra::logger->debug("ע��ʧ��ע��");
				sendCmd(&ret,sizeof(ret));
				// ֪ͨGatewayע��ʧ��
				if(gate)
				{
					Cmd::Scene::t_Unreg_LoginScene retgate;
					retgate.dwUserID=rev->dwID;
					if(scene)
						retgate.dwSceneTempID=scene->tempid;
					else
						retgate.dwSceneTempID=0;
					retgate.retcode=Cmd::Scene::UNREGUSER_RET_ERROR;
					gate->sendCmd(&retgate,sizeof(retgate));
				}
				return true;
			}
			//���û���ʱ��������
		case Cmd::Session::PARA_USER_ARCHIVE_READ:
			{
				t_ReadUser_SceneArchive *rev=(t_ReadUser_SceneArchive *)ptNullCmd;
				Scene *scene=SceneManager::getInstance() .getSceneByTempID(rev->dwMapTempID);
				if (scene)
				{
					SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->id);
					if (pUser)
					{
						pUser->setupTempArchive(rev->data , rev->dwSize);
					}
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_REQ_ADD_SCENE:
			{
				Cmd::Session::t_reqAddScene_SceneSession *rev = (Cmd::Session::t_reqAddScene_SceneSession*)ptNullCmd;
				Zebra::logger->debug("�յ����ص�ͼ��Ϣ(%u,%u,%u)",rev->dwServerID ,rev->dwCountryID , rev->dwMapID);
				if(rev->dwServerID == ScenesService::getInstance().getServerID())
				{
					Scene *loaded = SceneManager::getInstance().loadScene(Scene::STATIC, rev->dwCountryID , rev->dwMapID);
					if(loaded)
					{
						using namespace Cmd::Session;
						Zebra::logger->info("����%s(%ld,%ld)�ɹ�",loaded->name,loaded->id,loaded->tempid);
						t_regScene_SceneSession regscene;

						regscene.dwID=loaded->id;
						regscene.dwTempID=loaded->tempid;
						strncpy(regscene.byName, loaded->name, MAX_NAMESIZE);
						strncpy(regscene.fileName, loaded->getFileName(), MAX_NAMESIZE);
						regscene.dwCountryID = rev->dwCountryID;
						sessionClient->sendCmd(&regscene,sizeof(regscene));
					}
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_UNLOAD_SCENE:
			{
				Cmd::Session::t_unloadScene_SceneSession *cmd = (Cmd::Session::t_unloadScene_SceneSession*)ptNullCmd;
				Scene *scene= SceneManager::getInstance().getSceneByID(cmd->map_id);
				if(scene)
				{
					scene->setRunningState(SCENE_RUNNINGSTATE_UNLOAD);
					Zebra::logger->info("��ͼ%sĿǰ��������%u",scene->name 
							, scene->countUser());
					//, SceneUserManager::getMe().countUserInOneScene(scene));
					//if(SceneUserManager::getMe().countUserInOneScene(scene) == 0)
					if(scene->countUser() == 0)
					{
						scene->setRunningState(SCENE_RUNNINGSTATE_REMOVE);
						Cmd::Session::t_removeScene_SceneSession rem;
						rem.map_id = scene->id;
						sessionClient->sendCmd(&rem , sizeof(rem));
					}
					SceneUserManager::getMe().removeUserInOneScene(scene);
					//SceneManager::getInstance().unloadScene(scene);
				}
			}
			break;
		case Cmd::Session::PARA_UNION_ADDUNION:
			{
				Cmd::Session::t_addUnion_SceneSession *rev=(Cmd::Session::t_addUnion_SceneSession *)ptNullCmd;
				//Scene *scene=SceneManager::getInstance().getSceneByTempID(rev->dwMapTempID);
				//if (NULL != scene)
				{
					SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->info.dwCharID);
					if (NULL !=pUser)
					{
						if (1 == rev->byRetcode)
						{
							// ɾ���������
							zObject *itemobj = pUser->packs.uom.getObjectByThisID(rev->dwItemID);
							if (itemobj)
							{
								pUser->packs.removeObject(itemobj); //notify and delete
							}

							// �۳�����
							if (!pUser->packs.removeMoney(UnionDef::CREATE_UNION_NEED_PRICE_GOLD,"�������")) {
								Zebra::logger->fatal("�û�(%s)�������ʱ���Ӽ������", pUser->name);
							}

							// ��ʼ���û��Ĺ�����Ϣ
							//because vote
							//pUser->charbase.unionid = rev->info.dwUnionID;
							//pUser->save(Cmd::Record::TIMETICK_WRITEBACK);
							if (rev->info.byVote>0)
							{
								Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, 
										"��ϲ��,%s�����ͶƱ�׶�!",rev->info.name);
							}
							else
							{
								Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, 
										"��ϲ��, %s �ﴴ���ɹ�!",rev->info.name);

							}

							Cmd::stServerReturnUnionCheckCmd send;
							pUser->sendCmdToMe(&send, sizeof(send));
						}
						else
						{
							Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "���������ظ�");
						}
					}
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SEPT_ADDSEPT:
			{
				Cmd::Session::t_addSept_SceneSession *rev=(Cmd::Session::t_addSept_SceneSession *)ptNullCmd;
				//Scene *scene=SceneManager::getInstance().getSceneByTempID(rev->dwMapTempID);
				//if (NULL != scene)
				{
					SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->info.dwCharID);
					if (NULL !=pUser)
					{
						if (1 == rev->byRetcode)
						{
#ifdef CREATE_SEPT_NEED_ITEM_ID
							// ɾ��������ߣ��۳�Ǯ ---------------------
							SessionItemObjectCompare found;
							found.dwObjectID = CREATE_SEPT_NEED_ITEM_ID;
							zObject *itemobj = pUser->packs.uom.getObject(found);// ���ҵ���
							if (itemobj)
							{
								pUser->packs.rmObject(itemobj);
								SAFE_DELETE(itemobj);
								Cmd::stRemoveObjectPropertyUserCmd send;
								send.qwThisID=rev->dwItemID;
								pUser->sendCmdToMe(&send,sizeof(send));
							}
#endif
							/*
							   zObject *gold=pUser->packs.getGold();
							   if (gold)
							   {
							   gold->data.dwNum-=SeptDef::CREATE_SEPT_NEED_PRICE_GOLD;
							   if (gold->data.dwNum ==0)
							   {
							   Cmd::stRemoveObjectPropertyUserCmd rmgold;
							   rmgold.qwThisID=gold->data.qwThisID;
							   pUser->sendCmdToMe(&rmgold,sizeof(rmgold));
							   pUser->packs.rmObject(gold);
							   SAFE_DELETE(gold);
							   }
							   else
							   {
							//֪ͨ���Ӹı�
							Cmd::stRefCountObjectPropertyUserCmd setgold;
							setgold.qwThisID=gold->data.qwThisID;
							setgold.dwNum=gold->data.dwNum;
							pUser->sendCmdToMe(&setgold,sizeof(setgold));
							}
							}
							*/
							if (!pUser->packs.removeMoney(SeptDef::CREATE_SEPT_NEED_PRICE_GOLD,"��������")) 							     {
								Zebra::logger->fatal("�û�(%s)��������ʱ���Ӽ������", 
										pUser->name);
							}

							// ��ʼ���û��ļ�����Ϣ
							//pUser->charbase.septid = rev->info.dwSeptID;
							//pUser->save(Cmd::Record::OPERATION_WRITEBACK);
							if (rev->info.byVote>0)
							{
								Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, 
										"��ϲ��,%s�����ͶƱ�׶�!",rev->info.name);
							}
							else
							{
								Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, 
										"��ϲ��,%s�崴���ɹ�!",rev->info.name);

							}


							Cmd::stServerReturnSeptCheckCmd send;
							pUser->sendCmdToMe(&send, sizeof(send));
						}
						else
						{
							Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "����������ظ�");
						}
					}
				}
				return true;
			}
			break;
		case PARA_UNION_FIREMEMBER:
			{
				Cmd::Session::t_fireUnionMember_SceneSession *rev=(Cmd::Session::t_fireUnionMember_SceneSession *)ptNullCmd;
				//Scene *scene=SceneManager::getInstance().getSceneByTempID(rev->dwMapTempID);
				//if (NULL != scene)
				//{
					SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwCharID);
					if (NULL !=pUser)
					{
						if (pUser->charbase.unionid != 0)
						{
							Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "���Ѿ��뿪�˰��");
						}

						pUser->charbase.unionid = 0;
						bzero(pUser->unionName, sizeof(pUser->unionName));
						bzero(pUser->caption, sizeof(pUser->caption));
						pUser->removeWarRecord(Cmd::UNION_DARE);
						pUser->removeWarRecord(Cmd::UNION_CITY_DARE);

						pUser->save(Cmd::Record::OPERATION_WRITEBACK);
						Cmd::stUnionMemberLeaveUnionCmd send;
						pUser->sendCmdToMe(&send,sizeof(Cmd::stUnionMemberLeaveUnionCmd));
						pUser->reSendMyMapData();
					}
				//}
				return true;
			}
			break;
		case PARA_SEPT_FIREMEMBER:
			{
				Cmd::Session::t_fireSeptMember_SceneSession *rev=(Cmd::Session::t_fireSeptMember_SceneSession *)ptNullCmd;
				//Scene *scene=SceneManager::getInstance().getSceneByTempID(rev->dwMapTempID);
				//if (NULL != scene)
				{
					SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwCharID);
					if (NULL !=pUser)
					{
						if (pUser->charbase.septid != 0)
						{
							Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "���Ѿ��뿪�˼���");
						}
						pUser->charbase.septid = 0;
						pUser->charbase.levelsept = time(NULL);
						bzero(pUser->septName, sizeof(pUser->septName));

						pUser->removeWarRecord(Cmd::SEPT_DARE);
						pUser->removeWarRecord(Cmd::SEPT_NPC_DARE);

						pUser->save(Cmd::Record::OPERATION_WRITEBACK);
						Cmd::stSeptMemberLeaveSeptCmd send;
						pUser->sendCmdToMe(&send,sizeof(Cmd::stSeptMemberLeaveSeptCmd));
						pUser->reSendMyMapData();
					}
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_FRIENDDEGREE_RETURN:
			{
				Cmd::Session::t_ReturnFriendDegree_SceneSession *rev=(Cmd::Session::t_ReturnFriendDegree_SceneSession *)ptNullCmd;
				//Scene *scene=SceneManager::getInstance().getSceneByTempID(rev->dwMapTempID);
				//if (NULL != scene)
				{
					SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwID);
					if (NULL !=pUser)
					{
#ifdef _DEBUGLOG
						Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "������������е�����ϵ�б�");
#endif
						pUser->team.setFriendDegree(rev);
					}
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SCHOOL_CREATE_SUCCESS:
			{
				Cmd::Session::t_SchoolCreateSuccess_SceneSession *rev=(Cmd::Session::t_SchoolCreateSuccess_SceneSession *)ptNullCmd;
				SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwID);
				if (NULL !=pUser)
				{
					// ɾ��������ߣ��۳�Ǯ ---------------------
					SessionItemObjectCompare found;
					found.dwObjectID = CREATE_SCHOOL_REQUEST_ITEM_ID;
					zObject *itemobj = pUser->packs.uom.getObject(found);// ���ҵ���
					if (itemobj)
					{
						pUser->packs.removeObject(itemobj); //notify and delete
					}
					/*
					   zObject *gold=pUser->packs.getGold();
					   if (gold)
					   {
					   gold->data.dwNum-=CREATE_SCHOOL_REQUEST_PRICE_GOLD;
					   if (gold->data.dwNum<=0) // ��ʵ�� == ���У���ֹ����
					   {
					   Cmd::stRemoveObjectPropertyUserCmd rmgold;
					   rmgold.qwThisID=gold->data.qwThisID;
					   pUser->sendCmdToMe(&rmgold,sizeof(rmgold));
					   pUser->packs.rmObject(gold);
					   SAFE_DELETE(gold);
					   }
					   else
					   {
					//֪ͨ���Ӹı�
					Cmd::stRefCountObjectPropertyUserCmd setgold;
					setgold.qwThisID=gold->data.qwThisID;
					setgold.dwNum=gold->data.dwNum;
					pUser->sendCmdToMe(&setgold,sizeof(setgold));
					}
					}
					*/
					if (!pUser->packs.removeMoney(CREATE_SCHOOL_REQUEST_PRICE_GOLD,"����ʦ��")) {
						Zebra::logger->fatal("�û�(%s)��������ʱ���Ӽ������", pUser->name);
					}

					pUser->charbase.schoolid = rev->dwSchoolID;
					pUser->save(Cmd::Record::OPERATION_WRITEBACK);
					Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "��ϲ��,%s�ɴ����ɹ�!",rev->schoolName);
					requestFriendDegree(pUser);
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SEND_USER_RELATION_ID:
			{
				Cmd::Session::t_sendUserRelationID *rev=(Cmd::Session::t_sendUserRelationID *)ptNullCmd;
				SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (pUser)
				{
					switch(rev->type)
					{
						case Cmd::Session::RELATION_TYPE_COUNTRY:
							{
								if (pUser->charbase.country != rev->dwID)
								{
									pUser->charbase.country = rev->dwID;
									pUser->save(Cmd::Record::OPERATION_WRITEBACK); //�浵
								}
							}
							break;
						case Cmd::Session::RELATION_TYPE_SEPT:
							{
								if (pUser->charbase.septid != rev->dwID)
								{
									pUser->charbase.septid = rev->dwID;
									pUser->sendNineToMe();
									pUser->save(Cmd::Record::OPERATION_WRITEBACK); //�浵
								}

								strncpy(pUser->septName, rev->name, 
										sizeof(pUser->septName));
								pUser->septMaster = rev->septmaster;

								pUser->dwSeptRepute = rev->dwRepute;
								pUser->dwSeptLevel = rev->dwSeptLevel;
							}
							break;
						case Cmd::Session::RELATION_TYPE_SCHOOL:
							{
								pUser->charbase.schoolid = rev->dwID;
								pUser->myOverMan = rev->caption;//ʦ����ID;
								//pUser->save(Cmd::Record::OPERATION_WRITEBACK); //�浵
								requestFriendDegree(pUser);
							}
							break;
						case Cmd::Session::RELATION_TYPE_UNION:
							{
								bool oldKing = pUser->king;
								bool oldEmperor = pUser->emperor;
								if (pUser->charbase.unionid != rev->dwID)
								{
									pUser->charbase.unionid = rev->dwID;
									pUser->sendNineToMe();
									pUser->save(Cmd::Record::OPERATION_WRITEBACK); //�浵
								}

								strncpy(pUser->unionName, rev->name, 
										sizeof(pUser->unionName));

								pUser->unionMaster = rev->unionmaster;
								pUser->king = rev->king;
								pUser->emperor = rev->emperor;

								if (oldEmperor != rev->emperor)
								{
									if (HORSE_TYPE_SUPER==pUser->horse.horseType())
										if (rev->emperor)
											pUser->horse.data.speed += 50;
										else if (pUser->horse.data.speed>=50)
											pUser->horse.data.speed -= 50;
								}

								if (oldKing!=pUser->king)
									pUser->horse.sendData();

								if (pUser->unionMaster)
								{
									pUser->dwUnionActionPoint = rev->dwActionPoint;
									sprintf(pUser->caption, "����");
								}

								if (pUser->king)
								{
									sprintf(pUser->caption, "����");
								}
								else if(rev->caption>100)
								{
									SceneManager::MapMap_iter map_iter = 
										SceneManager::getInstance().map_info.
										find(rev->caption);

									if(map_iter != SceneManager::getInstance().map_info.end())
									{               
										if (pUser->unionMaster)
										{
											sprintf(pUser->caption, "%s ����",
													map_iter->second.name);
										}
										else
										{
											sprintf(pUser->caption, "%s",
													map_iter->second.name);
										}

										//sprintf(pUser->cityName, "%s", 
										//	map_iter->second.name);
									}
								}

								if (pUser->emperor)
								{
									sprintf(pUser->caption, "�ʵ�");
								}
							}
							break;
						case Cmd::Session::RELATION_TYPE_NOTIFY: //֪ͨ��ζ��ֻ��������ϵ��
							{
								requestFriendDegree(pUser);
							}
							break;
						default:
							break;
					}
					//pUser->sendNineToMe();
					pUser->reSendMyMapData();
				}
				return true;

			}
			break;
		case Cmd::Session::PARA_SCENE_CHANEG_SCENE:
			{
				Cmd::Session::t_changeScene_SceneSession *rev=(Cmd::Session::t_changeScene_SceneSession *)ptNullCmd;

				//				SceneUserManager::getMe().lock();
				SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->id);
				if (!pUser || rev->temp_id == (DWORD)-1) {
					return false;
				}
				if (pUser->scene->getRealMapID()==189 && pUser->isRedNamed())//����������
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�����ڷ����ڼ䣬�޷����ͣ�");
					return true;
				}
				if (pUser->scene->getRealMapID()==203 && pUser->charbase.punishTime)//��ץ�ڼ���
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�����ڷ����ڼ䣬�޷����ͣ�");
					return true;
				}
				pUser->clearUState(Cmd::USTATE_XIXINGEMIA);

				/*
				   OnQuit event(1);
				   EventTable::instance().execute(*pUser, event);
				   */
				//execute_script_event("quit");

				pUser->charbase.mapid = rev->temp_id;
				strncpy(pUser->charbase.mapName, (char *)rev->map_name, MAX_NAMESIZE);
				Zebra::logger->trace("%s(%d)����л�����(%s-->%s)",pUser->name,pUser->id,pUser->scene->name,pUser->charbase.mapName);
				pUser->charbase.x = rev->x;
				pUser->charbase.y = rev->y;
				//				Zebra::logger->debug("�洢��һ����(%s, %d, %d)", pUser->charbase.mapName, pUser->charbase.x, pUser->charbase.y);
				pUser->save(Cmd::Record::CHANGE_SCENE_WRITEBACK);
				//pUser->killAllPets();
				Cmd::Scene::t_Unreg_LoginScene retgate;
				retgate.dwUserID = pUser->id;
				retgate.dwSceneTempID = rev->temp_id;
				retgate.retcode = Cmd::Scene::UNREGUSER_RET_CHANGE_SCENE;
				strncpy((char *)retgate.map, (char *)rev->map_file, MAX_NAMESIZE);
				pUser->gatetask->sendCmd(&retgate, sizeof(retgate));
				pUser->unreg();

				if (rev->temp_id==189)
					Zebra::logger->trace("%s PKֵ %u����������", pUser->name, pUser->charbase.goodness);
				if (rev->temp_id==203)
					Zebra::logger->trace("%s ����ɱ�ˣ�����������ʱ�� %u ����", pUser->name, pUser->charbase.punishTime);

				//SAFE_DELETE(pUser);
				//				SceneUserManager::getMe().unlock();

				//				static_cast<SceneUserVisitor *>(pUser)->destroy();
				return true;
			}
			break;
		case Cmd::Session::PARA_SEPT_EXP_DISTRIBUTE:
			{
				Cmd::Session::t_distributeSeptExp_SceneSession *rev = (Cmd::Session::t_distributeSeptExp_SceneSession *)ptNullCmd;
				SceneUser *pUser=SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (pUser)
				{
					pUser->addExp(rev->dwExp, false);
					/*
					   pUser->charbase.exp+=rev->dwExp;
					   if(pUser->charbase.exp >= pUser->charstate.nextexp)
					   {
					   if(!pUser->upgrade())
					   {
					   ScenePk::sendChangedUserData(pUser);
					   }
					   }
					   ScenePk::attackRTExp(pUser , rev->dwExp);
					   */
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_GM_COMMAND:
			{
				return (doGmCmd(ptNullCmd, nCmdLen));
				break;
			}
		case Cmd::Session::PARA_SCENE_FORBID_TALK:
			{
				Cmd::Session::t_forbidTalk_SceneSession * rev = (Cmd::Session::t_forbidTalk_SceneSession *)ptNullCmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByName(rev->name);
				if (pUser)
				{
					switch (rev->operation)
					{
						case 1://����
							{
								pUser->delayForbidTalk(rev->delay);
								if (rev->delay>0)
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�㱻GM���� %d ��", rev->delay);
									ScenesService::gm_logger->trace("��� %s ������ %d ��", pUser->name, rev->delay);
								}
								else
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�㱻GM������ԣ����ڿ���˵����");
									ScenesService::gm_logger->trace("��� %s ���������", pUser->name);
								}
							}
							break;
						case 2://�ؽ���
							break;
						case 3://������
							{
								OnQuit event(1);
								EventTable::instance().execute(*pUser, event);
//								execute_script_event(pUser , "quit");

								pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
								//pUser->killAllPets();
								Cmd::Session::t_unregUser_SceneSession ret;
								ret.dwUserID=pUser->id;
								ret.dwSceneTempID=pUser->scene->tempid;
								ret.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
								sessionClient->sendCmd(&ret,sizeof(ret));
								Cmd::Scene::t_Unreg_LoginScene retgate;
								retgate.dwUserID = pUser->id;
								retgate.dwSceneTempID = pUser->scene->tempid;
								retgate.retcode = Cmd::Scene::UNREGUSER_RET_ERROR;
								pUser->gatetask->sendCmd(&retgate,sizeof(retgate));

								pUser->unreg();
							}
							break;
						case 4://����
							{
								Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, rev->reason);
							}
							break;
						default:
							return true;
					}
					sendCmd(rev, nCmdLen);
				}
				else
					Zebra::logger->debug("����ʱδ�ҵ������ id=%s", rev->name);
				return true;
				break;
			}

		case Cmd::Session::PARA_SCENE_PRIVATE_CHAT:
			{
				Cmd::Session::t_privateChat_SceneSession * rev = (Cmd::Session::t_privateChat_SceneSession *)ptNullCmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByName((char *)rev->dst_name);

				if (pUser)
				{
					switch (rev->act)
					{
						/*
						   case PRIVATE_CHAT_ACT_JOIN:
						   {
						   Cmd::stJoin_ChannelUserCmd *join=(Cmd::stJoin_ChannelUserCmd *)rev->chat_cmd;
						   SceneUser *pHost=SceneUserManager::getMe().getUserByName(join->host_name);
						   if (pHost)
						   {
						   Channel *cl=ChannelM::getMe().get(join->dwChannelID);
						   if(cl)
						   cl->add(this);
						   }
						   }
						   break;
						   case PRIVATE_CHAT_ACT_LEAVE:
						   {
						   Cmd::stLeave_ChannelUserCmd *leave=(Cmd::stLeave_ChannelUserCmd *)rev->chat_cmd;
						   SceneUser *pHost=SceneUserManager::getMe().getUserByName(leave->host_name);
						   if (pHost)
						   {
						   Channel *cl=ChannelM::getMe().get(leave->dwChannelID);
						   if(cl)
						   if (!cl->remove(leave->name))
						   ChannelM::getMe().remove(cl->tempid);
						   }
						   }
						   break;
						   */
						default:
							{
								switch (rev->err_code)
								{
									case 0:
										{
											if (pUser->checkUserCmd((Cmd::stChannelChatUserCmd *)rev->chat_cmd, rev->cmd_size))
												pUser->sendCmdToMe(rev->chat_cmd, rev->cmd_size);
											else
											{
												rev->err_code = Cmd::Session::PRIVATE_CHAT_ERR_FILTER;
												strncpy((char *)rev->dst_name, (char *)rev->src_name, MAX_NAMESIZE-1);
												strncpy((char *)rev->src_name, pUser->name, MAX_NAMESIZE-1);

												sessionClient->sendCmd(rev, sizeof(Cmd::Session::t_privateChat_SceneSession)+rev->cmd_size);
											}
										}
										break;
									case Cmd::Session::PRIVATE_CHAT_ERR_NOUSER:
										{
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������",(char *)rev->src_name);
										}
										break;
									case Cmd::Session::PRIVATE_CHAT_ERR_FILTER:
										{
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s �Ѿ��ر�˽��Ƶ��",(char *)rev->src_name);
										}
										break;
								}
								return true;
							}
					}
				}
				break;
			}
		case Cmd::Session::PARA_SEPT_NPCDARE_NOTIFYSCENE:
			{
				Cmd::Session::t_NpcDare_NotifyScene_SceneSession * rev = (Cmd::Session::t_NpcDare_NotifyScene_SceneSession *)ptNullCmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (pUser)
				{
					pUser->checkNpcDare(rev);
					pUser->reSendMyMapData();
					return true;
				}
			}
			break;
		case Cmd::Session::PARA_SEPT_NPCDARE_GETGOLD:
			{
				Cmd::Session::t_NpcDare_GetGold_SceneSession * rev = (Cmd::Session::t_NpcDare_GetGold_SceneSession *)ptNullCmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (pUser)
				{
					pUser->npcDareGetGold(rev->dwGold);
					return true;
				}
			}
			break;
		case Cmd::Session::PARA_SEPT_NPCDARE_ITEMBACK:
			{
				Cmd::Session::t_NpcDare_ItemBack_SceneSession * rev = (Cmd::Session::t_NpcDare_ItemBack_SceneSession *)ptNullCmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByID(rev->dwUserID);
				if (pUser)
				{
					if (pUser->packs.removeMoney(4000,"npc����ս��ս��"))//��Ǯ
					{
						Zebra::logger->trace("[��������NPC]�۳���ɫ%s40�����ӳɹ�", pUser->name);
					}
					else
					{
						Zebra::logger->trace("[��������NPC]�۳���ɫ%s40������ʧ�ܣ�����������", pUser->name);
					}
					return true;
					/*
					   zObject* item = zObject::create(objectbm.get(NpcDareDef::CREATE_NPCDARE_NEED_ITEM), 1);
					   if(item)
					   {
					   if (pUser->packs.addObject(item, true, AUTO_PACK)) 
					   {
					   Cmd::stAddObjectPropertyUserCmd ret;
					   ret.byActionType = Cmd::EQUIPACTION_REFRESH;
					   bcopy(&item->data, &ret.object, sizeof(t_Object));
					   pUser->sendCmdToMe(&ret, sizeof(ret));
					   Zebra::logger->trace("����ɫ%s�������ߵ�����id=[%u]", pUser->name, item->data.qwThisID);
					   }
					   else 
					   {
					   Zebra::logger->fatal("����ɫ%s�������ߵ�����ʧ��!", pUser->name);
					   }
					   }*/
				}
			}
			break;
			/*
			   case Cmd::Session::PARA_SCENE_CHECKSEND:
			   {
			   using namespace Cmd::Session;
			   t_checkSend_SceneSession * rev = (t_checkSend_SceneSession *)ptNullCmd;
			   SceneUser * pUser = SceneUserManager::getMe().getUserByName(rev->mail.fromName);
			   if (!pUser)
			   {
			   Zebra::logger->trace("%s: �ʼ����ʱ������ %s ����", __PRETTY_FUNCTION__, rev->mail.fromName);
			   return false;
			   }
			   pUser->isSendingMail = false;

			   if (!pUser->packs.checkMoney(rev->mail.sendMoney?rev->mail.sendMoney+Cmd::mail_postage:Cmd::mail_postage))
			   {
			   Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "������Ӳ���");
			   Zebra::logger->trace("%s �ʼ����ɹ������Ӳ���", pUser->name);

			   return false;
			   }

			   t_sendMail_SceneSession sm;
			   bcopy(&rev->mail, &sm.mail, sizeof(mailInfo));
			   if (rev->itemID && rev->itemID != 0xffffffff)//INVALID_THISID
			   {
			   zObject * srcobj=pUser->packs.uom.getObjectByThisID(rev->itemID);
			   if (!srcobj)
			   {
			   Zebra::logger->trace("%s �����ʼ�ʱδ�ҵ�����Ʒ��Ʒ id=%u", pUser->name, rev->itemID);
			   return false;
			   }

			   pUser->packs.removeObject(srcobj, true, false); //notify but not delete
			   srcobj->getSaveData((SaveObject *)&sm.item);

			   zObject::destroy(srcobj);
			   }
			   pUser->packs.removeMoney(rev->mail.sendMoney+Cmd::mail_postage);//��Ǯ

			   if (sendCmd(&sm, sizeof(t_sendMail_SceneSession)))
			   {
			   pUser->save(OPERATION_WRITEBACK);//���̴浵
			   Zebra::logger->trace("�����ʼ� %s->%s",rev->mail.fromName, rev->mail.toName);
			   return true;
			   }
			   else
			   {
			   Zebra::logger->error("�ʼ�����ʧ�� %s->%s",rev->mail.fromName, rev->mail.toName);
			   return false;
			   }
			   }
			   break;
			   */
		case Cmd::Session::PARA_SCENE_GET_MAIL_ITEM_RETURN:
			{
				using namespace Cmd::Session;
				t_getMailItemReturn_SceneSession * rev = (t_getMailItemReturn_SceneSession *)ptNullCmd;
				SceneUser * pUser = SceneUserManager::getMe().getUserByTempID(rev->userID);
				if (!pUser)
				{
					Zebra::logger->error("[�ʼ�]%s: ��ȡ��Ʒʱ���������� mailID=%u", __PRETTY_FUNCTION__, rev->mailID);
					return false;
				}

				pUser->isGetingMailItem = false;

				zObject *gold = pUser->packs.getGold();
				if (!gold)
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��İ�����û��Ǯ");
					return true;
				}
				if (gold->base->maxnum-gold->data.dwNum<rev->sendMoney)
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�޷���ȡ��������ӳ���������");

					return true;
				}
				if (10000000-pUser->charbase.gold<rev->sendGold)
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�޷���ȡ����Ľ��ӳ���������");

					return true;
				}
				if (rev->recvMoney)
				{
					if (!pUser->packs.checkMoney(rev->recvMoney) || !pUser->packs.removeMoney(rev->recvMoney,"�����ʼ�"))
					{
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "������Ӳ���");
						return true;
					}
				}
				if (rev->recvGold)
				{
					if (!pUser->packs.checkGold(rev->recvGold) || !pUser->packs.removeGold(rev->recvGold,"�����ʼ�"))
					{
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��Ľ��Ӳ���");
						return true;
					}
				}

				if (rev->item.object.dwObjectID)
				{
					zObject * ob = zObject::load((const SaveObject *)&rev->item);
					if (0==ob)
					{
						Zebra::logger->error("[�ʼ�]%s �����ʼ�����ʧ�� mailID=%u", pUser->name, rev->mailID);
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��ȡʧ�ܣ����������Ƿ�����Ʒ");
						return false;
					}
					/*
					   ob->data.pos.dwLocation = Cmd::OBJECTCELLTYPE_COMMON;
					   if (!pUser->packsaddObject(ob, true))
					   */
					//������ǰ���ʼ�
					ob->data.pos = Object::INVALID_POS;

					if (!pUser->packs.addObject(ob, true, AUTO_PACK) )
					{
						Zebra::logger->error("[�ʼ�]%s ����ʼ�����ʧ�� mailID=%u", pUser->name, rev->mailID);
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�����Ʒʧ�ܣ��������");
						return false;
					}

					Cmd::stAddObjectPropertyUserCmd status;
					status.byActionType = Cmd::EQUIPACTION_OBTAIN;
					bcopy(&ob->data, &status.object, sizeof(t_Object));
					pUser->sendCmdToMe(&status, sizeof(status));
					zObject::logger(ob->createid,ob->data.qwThisID,ob->data.strName,ob->data.dwNum,ob->data.dwNum,1,0,NULL,pUser->id,pUser->name,"������õ�",ob->base,ob->data.kind,ob->data.upgrade);
					//Zebra::logger->trace("[�ʼ�]%s ͨ���ʼ������Ʒ %s", pUser->name, ob->data.strName);
				}
				if (rev->sendMoney)
				{
					pUser->packs.addMoney(rev->sendMoney,"�ʼ��õ�");
					Zebra::logger->trace("[�ʼ�]%s ͨ���ʼ�������� %u", pUser->name, rev->sendMoney);
				}
				/*
				   if (rev->sendGold)
				   {
				   pUser->packs.addGold(rev->sendGold,"�ʼ��õ�");
				   Zebra::logger->trace("[�ʼ�]%s ͨ���ʼ���ý��� %u", pUser->name, rev->sendGold);
				   }
				   */
				pUser->save(OPERATION_WRITEBACK);//���̴浵

				t_getMailItemConfirm_SceneSession gmic;
				gmic.userID = pUser->tempid;
				gmic.mailID = rev->mailID;
				sendCmd(&gmic, sizeof(gmic));

				Cmd::stRemoveItemMail ri;
				ri.mailID = rev->mailID;
				pUser->sendCmdToMe(&ri, sizeof(ri));
				return true;
			}
			break;
		case Cmd::Session::PARA_AUCTION_CMD:
			{
				return doAuctionCmd((Cmd::Session::t_AuctionCmd *)ptNullCmd, nCmdLen);
			}
			break;
		case Cmd::Session::PARA_CARTOON_CMD:
			{
				return doCartoonCmd((Cmd::Session::t_CartoonCmd *)ptNullCmd, nCmdLen);
			}
			break;
		case Cmd::Session::PARA_SCENE_CREATE_RUSH:
			{
				t_createRush_SceneSession * rev = (t_createRush_SceneSession *)ptNullCmd;

				Rush * rush = new Rush(rev->rushID, rev->delay, rev->countryID);
				if (rush)
				{
					if (rush->init())
					{
						/*
						   Cmd::Session::t_cityRush_SceneSession send;
						   bzero(send.bossName, MAX_NAMESIZE);
						   bzero(send.rushName, MAX_NAMESIZE);
						   bzero(send.mapName, MAX_NAMESIZE);
						   strncpy(send.bossName, rush->bossName, MAX_NAMESIZE-1);
						   strncpy(send.rushName, rush->rushName, MAX_NAMESIZE-1);
						   strncpy(send.mapName, rush->mapName, MAX_NAMESIZE-1);
						   send.delay = rush->rushDelay;
						   sessionClient->sendCmd(&send, sizeof(send));
						   */

						Zebra::logger->trace("Session�������﹥�� %s", rush->rushName);
						return true;
					}

					SAFE_DELETE(rush);
					Zebra::logger->trace("Session�����Ĺ��ǳ�ʼ��ʧ��");
					return true;
				}
			}
			break;
		case Cmd::Session::PARA_TAX_COUNTRY:
			{
				t_taxCountry_SceneSession * rev = (t_taxCountry_SceneSession *)ptNullCmd;
				Scene * scene=SceneManager::getInstance().getSceneByTempID(rev->dwTempID);
				if (scene&&scene->getCountryID() == rev->dwCountryID)
				{
					scene->setTax(rev->byTax);
				}
				return true;
			}
			break;
		case Cmd::Session::PARA_SCENE_SEND_GIFT:
			{
					t_sendGift_SceneSession * rev = (t_sendGift_SceneSession *)ptNullCmd;

					if (rev->info.itemID)
					{
							zObjectB *base = objectbm.get(rev->info.itemID);
							if (base)
							{
									zObject* o = zObject::create(base, rev->info.itemNum, 0);
									if (o)
									{
											if (rev->info.itemType && 1==base->make)
											{
													EquipMaker maker(NULL);
													maker.assign(NULL, o, base, false, rev->info.itemType);
											}

											o->data.bind = rev->info.bind;
											sendMail("��;�����", 0, rev->info.name, rev->info.charID, Cmd::Session::MAIL_TYPE_SYS, rev->info.money, o, rev->info.mailText);
											zObject::logger(o->createid, o->data.qwThisID, o->base->name, o->data.dwNum, 1, 2, 0, NULL, rev->info.charID, rev->info.name, "���Ʒ",o->base,o->data.kind,o->data.upgrade);
											zObject::destroy(o);
									}
							}
							else
									Zebra::logger->error("[Gift]���ͽ�Ʒʱ��û�ҵ���Ʒ itemID=%u user=%s(%u)", rev->info.itemID, rev->info.name, rev->info.charID);
					}
					else
							sendMail("��;�����", 0, rev->info.name, rev->info.charID, Cmd::Session::MAIL_TYPE_SYS, rev->info.money, 0, rev->info.mailText);

					return true;
			}
			break;
		case PARA_BROADCAST_SCENE:
			{
				t_broadcastScene_SceneSession * rev= (t_broadcastScene_SceneSession *)ptNullCmd;
				Scene *scene = SceneManager::getInstance().getSceneByID(rev->mapID);
				if (scene)
				{
					zRTime ctv;
					Cmd::stChannelChatUserCmd send;
					send.dwType=Cmd::CHAT_TYPE_SYSTEM;
					send.dwSysInfoType=Cmd::INFO_TYPE_SCROLL;
					send.dwCharType = 0;
					send.dwChatTime = ctv.sec();
					send.dwChannelID=0;
					bzero(send.pstrName, sizeof(send.pstrName));
					bzero(send.pstrChat, sizeof(send.pstrChat));
					strncpy((char *)send.pstrChat, rev->info, MAX_CHATINFO-1);
					strncpy((char *)send.pstrName, rev->GM, MAX_NAMESIZE);

					scene->sendCmdToScene(&send, sizeof(send));
#ifdef _XWL_DEBUG
					Zebra::logger->debug("%s����:%s mapID=%u", send.pstrName, send.pstrChat, rev->mapID);
#endif
				}
#ifdef _XWL_DEBUG
				else
					Zebra::logger->debug("%s����:%s mapID=%u û�ҵ���ͼ", rev->GM, rev->info, rev->mapID);
#endif
				return true;
			}
			break;
		default:
			break;
	}//switch para

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}


bool SessionClient::cmdMsgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	switch(ptNullCmd->cmd)
	{
		case Cmd::Session::CMD_SCENE_ARMY:
			{
				return cmdMsgParse_Army(ptNullCmd, nCmdLen);
			}
			break;
		case Cmd::Session::CMD_SCENE_COUNTRY:
			{
				return cmdMsgParse_Country(ptNullCmd, nCmdLen);
			}
			break;
		case Cmd::Session::CMD_SCENE_DARE:
			{
				return cmdMsgParse_Dare(ptNullCmd, nCmdLen);
			}
			break;
		case Cmd::Session::CMD_SCENE_RECOMMEND:
			{
				return cmdMsgParse_Recommend(ptNullCmd, nCmdLen);
			}
			break;	
		case Cmd::Session::CMD_SCENE_TMP:
			{
				return cmdMsgParse_Temp(ptNullCmd, nCmdLen);
			}
			break;
		case Cmd::Session::CMD_SCENE_SEPT:
			{
				return cmdMsgParse_Sept(ptNullCmd, nCmdLen);
			}
			break;
		case Cmd::Session::CMD_SCENE_UNION:
			{
				return cmdMsgParse_Union(ptNullCmd, nCmdLen);
			}
			break;
		case Cmd::Session::CMD_SCENE_GEM:
			{
				return cmdMsgParse_Gem(ptNullCmd, nCmdLen);
			}
			break;
		default:
			return cmdMsgParse_Other(ptNullCmd, nCmdLen);
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief  ��������ָ���ڳ��������Ķ���
 * \param  cmd ��Ϣ��
 * \param  cmdLen ��Ϣ����
 * \return ��Ϣ������
 */
bool SessionClient::doAuctionCmd(const Cmd::Session::t_AuctionCmd * cmd, const DWORD cmdLen)
{
	using namespace Cmd::Session;
	using namespace Cmd::Record;

	switch (cmd->auctionPara)
	{
		case PARA_AUCTION_CHECK_BID:
			{
				t_checkBidAuction_SceneSession * rev = (t_checkBidAuction_SceneSession *)cmd;

				SceneUser * pUser = SceneUserManager::getMe().getUserByTempID(rev->userID);
				if (!pUser)
				{
					Zebra::logger->debug("SessionClient::doAuctionCmd(PARA_AUCTION_CHECK_BID): �����������Ѿ�����");
					return true;
				}

				if (!pUser->packs.checkMoney(rev->money) || !pUser->packs.checkGold(rev->gold))
				{
					Zebra::logger->debug("SessionClient::doAuctionCmd(PARA_AUCTION_CHECK_BID): ���������� %s ���Ӳ���", pUser->name);
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��Ľ�Ǯ����");
					return true;
				}

				if (0==rev->bidType)
					pUser->packs.removeMoney(rev->money, "����");
				else
					pUser->packs.removeGold(rev->gold, "��Ҿ���");

				t_bidAuction_SceneSession ba;
				ba.userID = pUser->tempid;
				ba.auctionID = rev->auctionID;
				ba.money = rev->money;
				ba.gold = rev->gold;

				sendCmd(&ba, sizeof(ba));
				pUser->save(OPERATION_WRITEBACK);//���̴浵
				return true;
			}
			break;
		case PARA_AUCTION_CHECK_CANCEL:
			{
				t_checkCancelAuction_SceneSession * rev = (t_checkCancelAuction_SceneSession *)cmd;

				SceneUser * pUser = SceneUserManager::getMe().getUserByTempID(rev->userID);
				if (!pUser)
				{
					Zebra::logger->debug("SessionClient::doAuctionCmd(PARA_AUCTION_CHECK_CANCEL): ȡ��������������Ѿ�����");
					return true;
				}

				/*
				   zObjectB *base = objectbm.get(rev->itemID);
				   if (!base)
				   {
				   Zebra::logger->error("[����]%s ȡ������ʱ��ƷID���� itemID=%u auctionID=%u", pUser->name, rev->itemID, rev->auctionID);
				   return false;
				   }
				   DWORD charge = base->price*3/10;
				   if (0==charge) charge = 1;
				   */
				if (!pUser->packs.checkMoney(rev->charge)
						|| !pUser->packs.removeMoney(rev->charge,"ȡ������"))
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "������Ӳ�����ȡ������");
					return true;
				}

				t_cancelAuction_SceneSession ca;
				ca.userID = pUser->tempid;
				ca.auctionID = rev->auctionID;
				ca.charge = rev->charge;

				sendCmd(&ca, sizeof(ca));
				pUser->save(OPERATION_WRITEBACK);//���̴浵
				return true;
			}
			break;
		default:
			break;
	}
	return false;
}

/**
 * \brief  ����GMָ���ڳ��������Ķ���
 * \param  ptNullCmd ��Ϣ��
 * \param  nCmdLen ��Ϣ����
 * \return true ��Ϣ�Ѿ������� false��Ϣδ������
 */
bool SessionClient::doGmCmd(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Cmd::Session::t_gmCommand_SceneSession * rev = (Cmd::Session::t_gmCommand_SceneSession *)ptNullCmd;
	SceneUser *pUser = SceneUserManager::getMe().getUserByName((char *)rev->dst_name);

	switch(rev->gm_cmd)
	{
		/*----------------�õ���ҵ�λ��--------------------------------*/
		case Cmd::Session::GM_COMMAND_FINDUSER:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)//һ���У���Ϊ��session�жϹ���
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);
								if (rev->src_priv>pUser->getPriv())
								{
									strncpy((char *)ret.map_name, pUser->scene->name, MAX_NAMESIZE);
									ret.x = pUser->getPos().x;
									ret.y = pUser->getPos().y;
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;
								}
								else
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_PRIV;
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "��� %s ������:%s λ��:(%d,%d)", (char *)rev->src_name, (char *)rev->map_name, rev->x, rev->y);
									ScenesService::gm_logger->trace("GM %s ʹ��finduserָ��,�õ� %s ��λ��", pUser->name, (char *)rev->src_name);
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��finduserָ��ʧ�ܣ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��ֻ�ܲ������Լ�Ȩ�޵͵����");
											ScenesService::gm_logger->trace("GM %s ʹ��finduserָ���ͼ�õ� %s ��λ�ã�Ȩ�޲���", pUser->name, (char *)rev->src_name);
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��finduserָ���Ϣ����ʱGM�Ѿ�������");
							break;	
						}//case RET
				}//switch cmd_state
				break;
			}//case FINDUSER
			/*----------------ȥ��������--------------------------------*/
		case Cmd::Session::GM_COMMAND_GOTOUSER:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);
								if (rev->src_priv>pUser->getPriv())
								{
									strncpy((char *)ret.map_name, pUser->scene->name, MAX_NAMESIZE);
									ret.x = pUser->getPos().x;
									ret.y = pUser->getPos().y;
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;
								}
								else
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_PRIV;
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
									Cmd::Session::t_changeScene_SceneSession cmd;
									cmd.id = pUser->id;
									cmd.temp_id = pUser->tempid;
									cmd.x = rev->x;
									cmd.y = rev->y;
									cmd.map_id = 0;
									cmd.map_file[0] = '\0';
									strncpy((char *)cmd.map_name, (char *)rev->map_name, MAX_NAMESIZE-1);
									sendCmd(&cmd, sizeof(cmd));
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��gotouserָ��ʧ�ܣ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��ֻ�ܲ������Լ�Ȩ�޵͵����");
											ScenesService::gm_logger->trace("GM %s ʹ��gotouserָ���ͼ�ҵ� %s��Ȩ�޲���", pUser->name, (char *)rev->src_name);
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��gotouserָ���Ϣ����ʱGM�Ѿ�������");
							break;
						}
				}
				break;
			}

			/*----------------����Ҵ���GM���--------------------------------*/
		case Cmd::Session::GM_COMMAND_CATCHUSER:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);
								if (rev->src_priv>pUser->getPriv())
								{
									Cmd::Session::t_changeScene_SceneSession cmd;
									cmd.id = pUser->id;
									cmd.temp_id = pUser->tempid;
									cmd.x = rev->x;
									cmd.y = rev->y;
									cmd.map_id = 0;
									cmd.map_file[0] = '\0';
									strncpy((char *)cmd.map_name, (char *)rev->map_name, MAX_NAMESIZE);
									sessionClient->sendCmd(&cmd, sizeof(cmd));

									strncpy((char *)ret.map_name, pUser->scene->name, MAX_NAMESIZE);
									ret.x = pUser->getPos().x;
									ret.y = pUser->getPos().y;
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;

									ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
									sendCmd(&ret, sizeof(ret));
								}
								else
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_PRIV;
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��catchuserָ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��ֻ�ܲ������Լ�Ȩ�޵͵����");
											ScenesService::gm_logger->trace("GM %s ʹ��catchuserָ���ͼ��׽ %s��Ȩ�޲���", pUser->name, (char *)rev->src_name);
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��catchuserָ���Ϣ����ʱGM�Ѿ�������");
							break;
						}
				}//switch cmd_state
				break;
			}//case CATCHUSER 

			/*----------------��������������--------------------------------*/
		case Cmd::Session::GM_COMMAND_EMBAR:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);
								if (rev->src_priv>pUser->getPriv())
								{
									pUser->killAllPets();
									Cmd::Session::t_changeScene_SceneSession cmd;
									cmd.id = pUser->id;
									cmd.temp_id = pUser->tempid;
									cmd.x = rev->x;
									cmd.y = rev->y;
									cmd.map_id = 0;
									cmd.map_file[0] = '\0';
									strncpy((char *)cmd.map_name, (char *)rev->map_name, MAX_NAMESIZE);
									sessionClient->sendCmd(&cmd, sizeof(cmd));

									strncpy((char *)ret.map_name, pUser->scene->name, MAX_NAMESIZE);
									ret.x = pUser->getPos().x;
									ret.y = pUser->getPos().y;
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;

									ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
									sendCmd(&ret, sizeof(ret));
								}
								else
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_PRIV;
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��embarָ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��ֻ�ܲ������Լ�Ȩ�޵͵����");
											ScenesService::gm_logger->trace("GM %s ʹ��embarָ���ͼ��׽ %s��Ȩ�޲���", pUser->name, (char *)rev->src_name);
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��embarָ���Ϣ����ʱGM�Ѿ�������");
							break;
						}
				}//switch cmd_state
				break;
			}//case CATCHUSER 

			/*----------------ʹ��ҽ���--------------------------------*/
		case Cmd::Session::GM_COMMAND_DONTTALK:
		case Cmd::Session::GM_COMMAND_TALK:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);
								if (rev->src_priv>pUser->getPriv())
								{
									pUser->delayForbidTalk(rev->x);
									if (0<rev->x)
										Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�㱻 %s ���� %d ��", (char *)rev->src_name, rev->x);
									else
										Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"%s �������Ľ���״̬�����ڿ���˵����", (char *)rev->src_name);
									ret.x = rev->x;
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;

									ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
									sendCmd(&ret, sizeof(ret));
								}
								else
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_PRIV;
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
									if (0<rev->x)
										Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "%s ������ %d ��", (char *)rev->src_name, rev->x);
									else
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "�ѽ�� %s �Ľ���״̬", (char *)rev->src_name);
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��donttalkָ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��ֻ�ܲ������Լ�Ȩ�޵͵����");
											ScenesService::gm_logger->trace("GM %s ʹ��donttalkָ���ͼʹ %s ���ԣ�Ȩ�޲���", pUser->name, (char *)rev->src_name);
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��donttalkָ���Ϣ����ʱGM�Ѿ�������");
							break;
						}//case RET
				}//switch cmd_state

				break;
			}//case TALK

			/*----------------�ߵ����--------------------------------*/
		case Cmd::Session::GM_COMMAND_KICK:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);

								if (rev->src_priv>pUser->getPriv())
								{
									//SceneUserManager::getMe().lock();

									OnQuit event(1);
									EventTable::instance().execute(*pUser, event);
//									execute_script_event(pUser,"quit");

									pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
									//pUser->killAllPets();//ɾ������

									//֪ͨsession
									Cmd::Session::t_unregUser_SceneSession rets;
									rets.dwUserID=pUser->id;
									rets.dwSceneTempID=pUser->scene->tempid;
									rets.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
									sessionClient->sendCmd(&rets,sizeof(rets));

									//֪ͨ����
									Cmd::Scene::t_Unreg_LoginScene retgate;
									retgate.dwUserID = pUser->id;
									retgate.dwSceneTempID = pUser->scene->tempid;
									retgate.retcode = Cmd::Scene::UNREGUSER_RET_ERROR;
									pUser->gatetask->sendCmd(&retgate,sizeof(retgate));

									pUser->unreg();
									//SAFE_DELETE(pUser);

									//SceneUserManager::getMe().unlock();

									ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;
								}
								else
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_PRIV;
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s ���߳�������", (char *)rev->src_name);
									ScenesService::gm_logger->trace("GM %s ʹ��kickָ��,�� %s �߳�������", rev->dst_name, rev->src_name);
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��kickָ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��ֻ�ܲ������Լ�Ȩ�޵͵����");
											ScenesService::gm_logger->trace("GM %s ʹ��kickָ���ͼ�� %s �߳���������Ȩ�޲���", pUser->name, (char *)rev->src_name);
											break;
										default:
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��kickָ���Ϣ����ʱGM�Ѿ�������");
							break;
						}
				}//switch state

				break;
			}//case KICK

			/*----------------�������Ȩ��--------------------------------*/
		case Cmd::Session::GM_COMMAND_SETPRIV:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);
								ret.x = rev->x;

								//setpriv���Ƚ�Ȩ��
								switch (rev->x)
								{
									case 0:
										pUser->setPriv(rev->x);
										Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�㱻 %s ȡ������Ȩ��", (char *)rev->src_name);
										ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;
										break;
									case 1:
										pUser->setPriv(rev->x);
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "����������ͨ���");
										ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;
										break;
									case 2:
										if (pUser->id>100)
										{
											//�ö��󲻿��ܳ�ΪGM
											ret.err_code = Cmd::Session::GM_COMMAND_ERR_PRIV;
											ret.y = pUser->id;
											break;
										}
										pUser->setPriv(rev->x);
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "���ΪGM��!");
										ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;
										break;
									default:
										//��������
										ret.err_code = Cmd::Session::GM_COMMAND_ERR_PARAM;
										ret.x = rev->x;
										break;
								}
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "���óɹ���priv=%d", rev->x);
									ScenesService::gm_logger->trace("GM %s ʹ��setprivָ��,���� %s ��Ȩ�ޣ�priv=%d", pUser->name, (char *)rev->src_name, rev->x);
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��setprivָ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "%s ���ܳ�ΪGM", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��setprivָ���ͼʹ %s ΪGM��ID̫�� priv=%d id=%d", pUser->name, (char *)rev->src_name, rev->x, rev->y);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PARAM:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��������");
											ScenesService::gm_logger->trace("GM %s ʹ��setprivָ���������name=%s priv=%d", pUser->name, (char *)rev->src_name, rev->x);
											break;
										default:
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��setprivָ���Ϣ����ʱGM�Ѿ�������");
							break;
						}
				}//switch state

				break;
			}//case SETPRIV 

			/*----------------������ֵ--------------------------------*/
		case Cmd::Session::GM_COMMAND_LOCKVALUE:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);
								ret.x = rev->x;
								ret.y = rev->y;
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;

								//Ҫ��������ֵ
								switch (rev->x)
								{
									case 1:
										pUser->hplock = true;
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s �������������ֵ", (char *)rev->src_name);
										break;
									case 2:
										pUser->mplock = true;
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s ���������ħ��ֵ", (char *)rev->src_name);
										break;
									case 3:
										pUser->splock = true;
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s �������������ֵ", (char *)rev->src_name);
										break;
									case 4:
										pUser->hplock = true;
										pUser->mplock = true;
										pUser->splock = true;
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s ���������������ħ��������ֵ", (char *)rev->src_name);
										break;
									default:
										break;
								}
								//Ҫ�����������ֵ
								switch (rev->y)
								{
									case 1:
										pUser->hplock = false;
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s ������������ֵ����", (char *)rev->src_name);
										break;
									case 2:
										pUser->mplock = false;
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s ��������ħ��ֵ����", (char *)rev->src_name);
										break;
									case 3:
										pUser->splock = false;
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s ������������ֵ����", (char *)rev->src_name);
										break;
									case 4:
										pUser->hplock = false;
										pUser->mplock = false;
										pUser->splock = false;
										Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%s ��������������ħ��������ֵ����", (char *)rev->src_name);
										break;
									default:
										break;
								}
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							if ((5==ret.x)&&(5==ret.y))
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_PARAM;
							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "���óɹ���lock=%d unlock=%d", rev->x, rev->y);
									ScenesService::gm_logger->trace("GM %s ʹ��lockvalueָ��,���� %s ����ֵ��lock=%d unlock=%d", pUser->name, (char *)rev->src_name, rev->x, rev->y);
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��lockvalueָ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��û���㹻��Ȩ��");
											ScenesService::gm_logger->trace("GM %s ʹ��lockvalueָ���ͼ���� %s ����ֵ��Ȩ�޲��㡣 lock=%d unlock=%d", pUser->name, (char *)rev->src_name, rev->x, rev->y);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PARAM:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��������");
											ScenesService::gm_logger->trace("GM %s ʹ��lockvalueָ���������name=%s lock=%d unlock=%d", pUser->name, (char *)rev->src_name, rev->x, rev->y);
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��lockvalueָ���Ϣ����ʱGM�Ѿ�������");
							break;
						}
				}//switch state

				break;
			}//case LOACKVALUE 

			/*----------------�����ȼ�--------------------------------*/
		case Cmd::Session::GM_COMMAND_LEVELUP:
			{
				switch (rev->cmd_state)
				{
					case Cmd::Session::GM_COMMAND_STATE_REQ:
						{
							Cmd::Session::t_gmCommand_SceneSession ret;
							ret.gm_cmd = rev->gm_cmd;
							strncpy((char *)ret.dst_name, (char *)rev->src_name, MAX_NAMESIZE);
							if (pUser)
							{
								strncpy((char *)ret.src_name, pUser->name, MAX_NAMESIZE);
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOERR;
								ret.x = rev->x;

								if (!pUser->upgrade(rev->x))
									ret.err_code = Cmd::Session::GM_COMMAND_ERR_FAIL;
							}
							else
								ret.err_code = Cmd::Session::GM_COMMAND_ERR_NOUSER;

							ret.cmd_state = Cmd::Session::GM_COMMAND_STATE_RET;
							sendCmd(&ret, sizeof(ret));
							break;
						}

					case Cmd::Session::GM_COMMAND_STATE_RET:
						{
							if (pUser)
							{
								if (Cmd::Session::GM_COMMAND_ERR_NOERR==rev->err_code)
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "�����ɹ���name=%s num=%d", (char *)rev->src_name, rev->x);
									ScenesService::gm_logger->trace("GM %s ʹ��levelupָ��,���� %s �ĵȼ� %d ", pUser->name, (char *)rev->src_name, rev->x);
								}
								else
								{
									switch (rev->err_code)
									{
										case Cmd::Session::GM_COMMAND_ERR_NOUSER:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s ������", (char *)rev->src_name);
											ScenesService::gm_logger->trace("GM %s ʹ��levelupָ���� %s ������", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_PRIV:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��û���㹻��Ȩ��");
											ScenesService::gm_logger->trace("GM %s ʹ��levelupָ���ͼ���� %s �ĵȼ���Ȩ�޲��㡣", pUser->name, (char *)rev->src_name);
											break;
										case Cmd::Session::GM_COMMAND_ERR_FAIL:
											Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "ָ��ִ��ʧ��");
											ScenesService::gm_logger->trace("GM %s ʹ��levelupָ�ִ��ʧ�ܡ�name=%s num=%d", pUser->name, (char *)rev->src_name, rev->x);
											break;
									}
								}
							}
							else
								ScenesService::gm_logger->trace("GM ʹ��levelupָ���Ϣ����ʱGM�Ѿ�������");
							break;
						}
				}//switch state

				break;
			}//case LEVELUP

			/*----------------����process.dat�ļ�--------------------------------*/
		case Cmd::Session::GM_COMMAND_LOAD_PROCESS:
			{
				Zebra::logger->debug("�����������ļ�...%u �ֽ�", ScenesService::updateStampData());
				break;
			}//case LOAD_PROCESS
			break;
			//��������GMָ��
		case Cmd::Session::GM_COMMAND_NEWZONE:
			{
				if(rev->cmd_state)
				{
					if(!SceneManager::getInstance().isNewZoneConfig())
					{
						SceneManager::getInstance().setNewZoneConfig(true);
						SceneManager::getInstance().addNewZonePos(151,211); 
						SceneManager::getInstance().addNewZonePos(250,175); 
						SceneManager::getInstance().addNewZonePos(212,330); 
						SceneManager::getInstance().addNewZonePos(159,343); 
					}
					else
					{
						SceneManager::getInstance().setNewZoneConfig(true);
					}
				}
				else
				{
					SceneManager::getInstance().setNewZoneConfig(false);
					return true;
				}
				if(rev->x && rev->y)
				{
					SceneManager::getInstance().addNewZonePos(rev->x,rev->y); 
				}
			}
			break;

	}//switch gm_cmd
	return true;
}

/* \brief ��������������Ϣ
 * \param cmd ��Ϣ
 * \param nCmdLen ��Ϣ����
 *
 * \return ���
 */
bool SessionClient::doCartoonCmd(const Cmd::Session::t_CartoonCmd *cmd, const unsigned int nCmdLen)
{
	Cmd::Session::t_CartoonCmd * rev = (Cmd::Session::t_CartoonCmd *)cmd;

	using namespace Cmd::Session;
	switch(rev->cartoonPara)
	{
		case PARA_CARTOON_DRAW:
			{
				t_drawCartoon_SceneSession * rev = (t_drawCartoon_SceneSession *)cmd;
				SceneUser *pMaster = SceneUserManager::getMe().getUserByID(rev->userID);
				if (!pMaster)
				{
					Zebra::logger->error("[����]��ȡ���鷵��ʱ���Ҳ������� userID=%u cartoonID=%u exp=%u", rev->userID, rev->cartoonID, rev->num);
					return false;
				}
				if (pMaster->cartoonList.find(rev->cartoonID)==pMaster->cartoonList.end()) return false;

				if (pMaster->cartoon && pMaster->cartoon->getCartoonID()==rev->cartoonID)
					pMaster->cartoonList[rev->cartoonID] = pMaster->cartoon->getCartoonData();
				pMaster->cartoonList[rev->cartoonID].addExp = 0;
				if (pMaster->cartoon && pMaster->cartoon->getCartoonID()==rev->cartoonID)
					pMaster->cartoon->setCartoonData(pMaster->cartoonList[rev->cartoonID]);
				Cmd::stAddCartoonCmd ac;
				ac.isMine = true;
				ac.cartoonID = rev->cartoonID;
				ac.data = pMaster->cartoonList[rev->cartoonID];
				pMaster->sendCmdToMe(&ac, sizeof(ac));

				pMaster->addExp(rev->num, true, 0, 0, false);
				Zebra::logger->trace("[����]%s(%u) ��ȡ���� %u cartoonID=%u", pMaster->name, pMaster->id, rev->num, rev->cartoonID);
				return true;
				return true;
			}
			break;
		case PARA_CARTOON_LEVEL_NOTIFY:
			{
				t_levelNotifyCartoon_SceneSession * rev = (t_levelNotifyCartoon_SceneSession *)cmd;
				SceneUser *pAdopter = SceneUserManager::getMe().getUserByID(rev->userID);
				if (!pAdopter || pAdopter->adoptList.find(rev->cartoonID)==pAdopter->adoptList.end()) return false;

				CartoonPet * p = pAdopter->adoptList[rev->cartoonID];
				if (!p) return false;

				Cmd::t_CartoonData d = p->getCartoonData();
				d.masterLevel = rev->level;
				p->setCartoonData(d);

				Channel::sendNine(p, "ŶҲ~������ %s ����%u���ˣ�������Ҳ�����~", d.masterName, rev->level);
				return true;
			}
			break;
		case PARA_CARTOON_CHARGE_NOTIFY:
			{
				t_chargeNotifyCartoon_SceneSession * rev = (t_chargeNotifyCartoon_SceneSession *)cmd;

				SceneUser *pAdopter = SceneUserManager::getMe().getUserByName(rev->adopter);
				if (!pAdopter)
				{
					Zebra::logger->error("[����]�����ֵ����ʱ�Ҳ��������� adopter=%s cartoonID=%u time=%u", rev->adopter, rev->cartoonID, rev->time);
					return false;
				}

				if (pAdopter->adoptList.find(rev->cartoonID)==pAdopter->adoptList.end())
				{
					t_correctCartoon_SceneSession c;
					c.cartoonID = rev->cartoonID;
					sessionClient->sendCmd(&c, sizeof(c));
					return false;
				}

				t_saveCartoon_SceneSession send;
				send.cartoonID = rev->cartoonID;
				send.data = pAdopter->adoptList[rev->cartoonID]->getCartoonData();
				send.data.time += rev->time;
				send.type = SAVE_TYPE_SYN;
				sessionClient->sendCmd(&send, sizeof(send));

				pAdopter->adoptList[rev->cartoonID]->setCartoonData(send.data);
				return true;
			}
			break;
		case PARA_CARTOON_SALE:
			{
				t_saleCartoon_SceneSession * rev = (t_saleCartoon_SceneSession *)cmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByID(rev->userID);
				if (!pUser)
				{
					Zebra::logger->error("[����]���۳��ﷵ��ʱ�Ҳ������� userID=%u cartoonID=%u", rev->userID, rev->cartoonID);
					return false;
				}

				if (pUser->cartoonList.find(rev->cartoonID)==pUser->cartoonList.end()) return true;

				if (pUser->cartoon && pUser->cartoon->getCartoonID()==rev->cartoonID)
					pUser->cartoon->putAway(Cmd::Session::SAVE_TYPE_DONTSAVE);
				pUser->packs.addMoney(3, "�����������",NULL);

				Cmd::stRemoveCartoonCmd send;
				send.cartoonID = rev->cartoonID;
				pUser->sendCmdToMe(&send, sizeof(send));

				Zebra::logger->trace("[����]%s ���۳��� %s", pUser->name, pUser->cartoonList[rev->cartoonID].name);
				pUser->cartoonList.erase(rev->cartoonID);

				pUser->refreshPetPackSize();
				return true;
			}
			break;
		case PARA_CARTOON_ADD:
			{
				t_addCartoon_SceneSession * rev = (t_addCartoon_SceneSession *)cmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByID(rev->userID);
				if (!pUser) return false;
				pUser->cartoonList[rev->cartoonID] = rev->data;

				if (pUser->cartoon && pUser->cartoon->getCartoonID()==rev->cartoonID)
				{
					if (rev->data.state==Cmd::CARTOON_STATE_WAITING
							|| rev->data.state==Cmd::CARTOON_STATE_ADOPTED)
					{
						CartoonPet * tmp = pUser->cartoon;
						pUser->killOnePet(pUser->cartoon);
						tmp->setClearState();
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "%s ���ڵȴ����Ѿ������������ܷų����������������µ�¼��Ϸ", rev->data.name);
						Zebra::logger->debug("[����]%s(%u) �ĳ��� %s(%u) �ڸ���ʱ��ɾ����session״̬ %u", pUser->name, pUser->id, rev->data.name, rev->cartoonID, rev->data.state);
					}
					else
						pUser->cartoon->setCartoonData(pUser->cartoonList[rev->cartoonID]);
				}

				Cmd::stAddCartoonCmd ac;
				ac.isMine = true;
				ac.cartoonID = rev->cartoonID;
				ac.data = rev->data;
				pUser->sendCmdToMe(&ac, sizeof(ac));

				if (rev->data.state==Cmd::CARTOON_STATE_FOLLOW && !pUser->cartoon)
				{
					CartoonPet * cp = (CartoonPet *)pUser->summonPet(rev->data.npcID, Cmd::PET_TYPE_CARTOON);
					if (cp)
					{
						cp->setCartoonID(rev->cartoonID);
						cp->setCartoonData(rev->data);
					}
				}
				else if (rev->data.state==Cmd::CARTOON_STATE_PUTAWAY && !pUser->cartoon && pUser->charbase.petPoint)
				{
					//���������ĳ�������ʱ�䣬һ���Լӵ���������
					pUser->cartoonList[rev->cartoonID].time += pUser->charbase.petPoint;

					/*
					Cmd::Session::t_saveCartoon_SceneSession s;
					strncpy(s.userName, pUser->name, MAX_NAMESIZE);
					s.type = SAVE_TYPE_CHARGE;
					s.cartoonID = rev->cartoonID;
					s.data = pUser->cartoonList[rev->cartoonID];
					sessionClient->sendCmd(&s, sizeof(s));
					*/
					Cmd::Session::t_chargeCartoon_SceneSession send;
					send.masterID = pUser->id;
					send.cartoonID = rev->cartoonID;
					send.time = pUser->charbase.petPoint;
					sessionClient->sendCmd(&send, sizeof(send));

					Cmd::stAddCartoonCmd ac;
					ac.isMine = true;
					ac.cartoonID = rev->cartoonID;
					ac.data = pUser->cartoonList[rev->cartoonID];
					pUser->sendCmdToMe(&ac, sizeof(ac));

					Zebra::logger->trace("%s �ĳ���%s(%u)���%u������ʱ��", pUser->name, pUser->cartoonList[rev->cartoonID].name, rev->cartoonID, pUser->charbase.petPoint);
					pUser->charbase.petPoint = 0;
				}

				pUser->refreshPetPackSize();
				return true;
			}
			break;
		case PARA_CARTOON_GET_BACK:
			{
				t_getBackCartoon_SceneSession * rev = (t_getBackCartoon_SceneSession *)cmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByID(rev->userID);

				if (!pUser || pUser->adoptList.find(rev->cartoonID)==pUser->adoptList.end())//��������û���������Ǳ��Ϊ�������������
				{
					t_correctCartoon_SceneSession c;
					c.cartoonID = rev->cartoonID;
					sessionClient->sendCmd(&c, sizeof(c));
					return true;
				}

				if (pUser) pUser->adoptList[rev->cartoonID]->putAway(SAVE_TYPE_RETURN);
				return true;
			}
			break;
		case PARA_CARTOON_NOTIFY:
			{
				t_notifyCartoon_SceneSession * rev = (t_notifyCartoon_SceneSession *)cmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByName(rev->adopter);
				if (!pUser) return true;
				if (pUser->adoptList.find(rev->cartoonID)==pUser->adoptList.end()) return true;

				pUser->adoptList[rev->cartoonID]->setExpRate(rev->state?3:2);

				if (rev->state)
					Channel::sendNine(pUser->adoptList[rev->cartoonID], "HOHO~���������ˣ���Ҫ���ͣ����1.5���ľ���!");
				else
					Channel::sendNine(pUser->adoptList[rev->cartoonID], "���������ˣ�������Ҳû����...");
				return true;
			}
			break;
			/*
			   case PARA_CARTOON_ASK:
			   {
			   t_askCartoon_SceneSession * rev = (t_askCartoon_SceneSession *)cmd;
			   SceneUser *pUser = SceneUserManager::getMe().getUserByID(rev->masterID);
			   if (!pUser) return false;

			   for (SceneUser::cartoon_it it=pUser->cartoonList.begin(); it!=pUser->cartoonList.end(); it++)
			   {
			   if (it->second.state==Cmd::CARTOON_STATE_WAITING)
			   {
			   Cmd::stAddCartoonCmd add;
			   add.isMine = false;
			   add.cartoonID = it->first;
			   add.data = it->second;
			   sendCmdByID(rev->askerID, &add, sizeof(add));
			   return true;
			   }
			   }
			   return true;
			   }
			   break;
			   */
		case PARA_CARTOON_ADOPT:
			{
				t_adoptCartoon_SceneSession * rev = (t_adoptCartoon_SceneSession *)cmd;
				SceneUser *pUser = SceneUserManager::getMe().getUserByID(rev->userID);
				if (!pUser) return false;

				if (pUser->adoptList.size()>=5)
				{
					Cmd::Session::t_saveCartoon_SceneSession send;
					strncpy(send.userName, pUser->name, MAX_NAMESIZE);
					send.type = Cmd::Session::SAVE_TYPE_RETURN;
					send.cartoonID = rev->cartoonID;
					send.data = rev->data;
					send.data.state = Cmd::CARTOON_STATE_WAITING;
					bzero(send.data.adopter, MAX_NAMESIZE);
					sessionClient->sendCmd(&send, sizeof(send));

					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "�벻Ҫ�ظ�����");
					return true;
				}

				zNpcB *base = npcbm.get(rev->data.npcID);
				if (!base) return false;

				for(SceneUser::adopt_it it=pUser->adoptList.begin(); it!=pUser->adoptList.end(); it++)
					if (it->first==rev->cartoonID)
					{
						it->second->setClearState();
						pUser->adoptList.erase(it);
					}

				t_NpcDefine define;
				define.id = base->id;
				strncpy(define.name, base->name, MAX_NAMESIZE-1);
				define.pos = pUser->getPos();
				define.num = 1;
				define.interval = 5;
				define.initstate = zSceneEntry::SceneEntry_Normal;
				define.width = SceneUser::CALL_PET_REGION;
				define.height = SceneUser::CALL_PET_REGION;
				define.pos -= zPos(SceneUser::CALL_PET_REGION/2,SceneUser::CALL_PET_REGION/2);
				define.scriptID = 0;
				pUser->scene->initRegion(define.region, define.pos, define.width, define.height);

				CartoonPet * newPet = pUser->scene->summonOneNpc<CartoonPet>(define, zPos(0,0), base, 0, 0, 0);
				if (newPet)
				{
					strncpy(newPet->name, rev->data.name, MAX_NAMESIZE);
					newPet->setPetType(Cmd::PET_TYPE_CARTOON);
					newPet->setMaster(pUser);
					SceneNpcManager::getMe().addSpecialNpc(newPet);

					newPet->setCartoonID(rev->cartoonID);
					newPet->setCartoonData(rev->data);
					newPet->setExpRate(rev->masterState?3:2);
					pUser->adoptList[rev->cartoonID] = newPet;

					//Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "��������һֻ�����������ڿ�ʼ���Ի��%5����ľ���ֵ");
					Channel::sendNine(newPet, "лл���������~�ҿ��԰�������5%����ľ���ֵ:)");
					Zebra::logger->debug("[����]%s(%u��)������ %s(%u��) �ĳ���(%u),������%uֻ addExp=%u", pUser->name, pUser->charbase.level, rev->data.masterName, rev->data.masterLevel, rev->cartoonID, pUser->adoptList.size(), rev->data.addExp);
				}

				return true;
			}
			break;
		case PARA_CARTOON_CONSIGN:
			{
				t_consignCartoon_SceneSession *rev = (t_consignCartoon_SceneSession *)cmd;
				SceneUser * pUser = SceneUserManager::getMe().getUserByID(rev->userID);
				if (!pUser) return false;

				if (pUser->adoptList.size()>=5)
				{
					t_consignRetCartoon_SceneSession send;
					send.userID = pUser->id;
					send.ret = 2;
					send.cartoonID = rev->cartoonID;
					sendCmd(&send, sizeof(send));

					return true;
				}

				Cmd::stConsignCartoonCmd ccc;
				strncpy(ccc.name, rev->name, MAX_NAMESIZE);
				ccc.cartoonID = rev->cartoonID;
				pUser->sendCmdToMe(&ccc, sizeof(ccc));
				return true;
			}
			break;
		default:
			break;
	}
	return false;
}
