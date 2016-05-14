/**
 * \file
 * \version  $Id: SkillStatusManager.cpp  $
 * \author 
 * \date 
 * \brief ���弼��״̬������ͷ�ļ�
 *
 */

#include "SkillBase.h"
#include "SkillStatusManager.h"
#include "SceneUser.h"
#include "ScenePk.h"
#include "SceneUserManager.h"
#include "Scene.h"
#include "Chat.h"

/**
 * \brief	״̬0 ��״̬�������κβ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_0(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;
	return SKILL_RETURN;
}

/**
 * \brief	״̬1 ��״̬�������˺����ӹ̶���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_1(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 1;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
//					pEntry->skillValue.dvalue += sse.value;
					pEntry->pkValue.dvalue += sse.value;
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
//				pEntry->skillValue.dvalue += sse.value;
				pEntry->pkValue.dvalue += sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬2 ��״̬�������˺����Ӱٷֱȣ���׼��ֵΪ��ɫ��������װ�����������ܺ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_2(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 1;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dvaluep = sse.value;
					pEntry->pkValue.dvaluep = sse.value;
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dvaluep = sse.value;
				pEntry->pkValue.dvaluep = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬3 ���ܵ���������[��״̬�����Ѿ�������]
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_3(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 2;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
//					pEntry->skillValue.pdam = sse.value;
					pEntry->pkValue.pdamage = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
//				pEntry->skillValue.pdam += sse.value;
				pEntry->pkValue.pdamage = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬4 ָ��״̬�����ߵ������������ֵ[�����ϲ��������ּ������Դ�״̬����Ҳ�Ѿ�������]
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_4(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 3;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
//					pEntry->skillValue.pdef = sse.value;
					pEntry->pkValue.pdefence = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->pkValue.pdefence = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬5 ��ʾ״̬������������������,װ���������������ܺ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_5(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 10;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
//					pEntry->skillValue.mdam = sse.value;
					pEntry->pkValue.mdamage = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->pkValue.mdamage = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬6 ��ʾ״̬������������������,װ���������������ܺ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_6(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 11;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					//pEntry->skillValue.mdef = sse.value;
					pEntry->pkValue.mdefence = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->pkValue.mdefence = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬7 ����״̬�����ߵĹ̶�ֵ����ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_7(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 4;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.5f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeMP(0-sse.value-value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeMP(0-sse.value-value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬8 ����״̬�����ߵİٷֱȷ���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_8(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 4;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					SDWORD value = (SDWORD)(pEntry->getMaxMP()*(sse.value/100.0f));
					pEntry->changeMP(value);
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SDWORD value = (SDWORD)(pEntry->getMaxMP()*(sse.value/100.0f));
				pEntry->changeMP(value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬9 ����״̬�����ߵĹ̶�ֵ����ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_9(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 5;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(1*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					//pEntry->changeHP(0-sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					pEntry->directDamage(pAtt, sse.value+value,true);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬10 ����״̬�����ߵİٷֱ�����ֵ����׼Ϊ�������ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_10(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 5;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					//SDWORD value = (SDWORD)(pEntry->getMaxHP()*(sse.value/100.0f));
					//pEntry->changeHP(0-value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					SDWORD value = (SDWORD)(pEntry->getMaxHP()*(sse.value/100.0f));
					pEntry->directDamage(pAtt, value, true);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬11 ���ĺ�״̬�����ߵĹ̶�ֵ����ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_11(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 6;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeSP(0-sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeSP(0-sse.value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬12 ����״̬�����ߵİٷֱ�����ֵ����׼���������ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_12(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 6;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					SDWORD value = (SDWORD)(pEntry->getMaxSP()*(sse.value/100.0f));
					pEntry->changeSP(value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SDWORD value = (SDWORD)(pEntry->getMaxSP()*(sse.value/100.0f));
				pEntry->changeSP(value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬13 ��ʾ״̬�����ߵ�ħ�������������̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_13(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.umdef = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.umdef = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.umdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬14 ��ʾ״̬�����ߵ�ħ�������������ٷֱ�(����ר��)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_14(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.ice_umdefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.ice_umdefp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.ice_umdefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬15 ��ʾ��״̬�����ߴ˴��ܵ��Ĺ�����ɵ��˺�ֵ���㣨��������ĺͷ����ģ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_15(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 8;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->ignoreDam = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->ignoreDam = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->ignoreDam = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬16 ��ʾ����״̬�����ߵ��ƶ��ٶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_16(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 9;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.1f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->reSendOther = true;
					pEntry->skillValue.movespeed = sse.value+value;

					//xwl add
					if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
						((SceneNpc *)pEntry)->resetSpeedRate();
					//pEntry->reSendMyMapData();
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->reSendOther = true;
				pEntry->skillValue.movespeed = 0;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

				//pEntry->reSendMyMapData();
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬17 ����״̬�������ٴ�ʩ�Ŵ˼��ܵ���ȴʱ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_17(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 12;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.mgspeed = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.mgspeed = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.mgspeed = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬18 ����״̬�������������״̬�ļ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_18(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 13;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.coldp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.coldp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.coldp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬19 ����״̬�����������ж�״̬����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_19(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 14;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.poisonp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.poisonp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.poisonp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬20 ����״̬����������ʯ��״̬�ļ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_20(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 15;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.petrifyp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.petrifyp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.petrifyp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬21 ����״̬����������ʧ��״̬����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_21(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 16;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.blindp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.blindp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬22 ����״̬�������������״̬����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_22(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 17;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.chaosp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.chaosp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.chaosp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬23 ����״̬���������������̶�ֵ(һ���Թ���������ʱ��������2��)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_23(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.updam = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.updam = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.updam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬24 ����״̬���������������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_24(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.updamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.updamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.updamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬25 ����״̬�����߷����������̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_25(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 21;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.umdam = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.umdam = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.umdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬26 ����״̬�����߷����������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_26(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 211;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.umdamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.umdamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.umdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬27 ����״̬����������ֵ���޹̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_27(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

//	pEntry->skillValue.maxhpup = sse.value;
	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.maxhp = (SWORD)(sse.value);
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.maxhp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬28 ����״̬����������ֵ���ްٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_28(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.maxhp = (SWORD)(pEntry->getBaseMaxHP()*(sse.value/100.0f));
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.maxhp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬29 ����״̬�����߷���ֵ���޹̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_29(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 23;
	pEntry->reSendData = true;

//	pEntry->skillValue.maxmpup = sse.value;
	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.maxmp = (SWORD)(pEntry->getBaseMaxMP()+sse.value);
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.maxmp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬30 ����״̬�����߷���ֵ���ްٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_30(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 23;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.maxmp = (SWORD)(pEntry->getBaseMaxMP()*(sse.value/100.0f));
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.maxmp =0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬31 ����״̬����������ֵ���޹̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_31(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 24;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.maxsp = (SWORD)(pEntry->getMaxSP()+sse.value);
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.maxsp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬32 ����״̬����������ֵ���ްٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_32(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 24;
	pEntry->reSendData = true;

	//pEntry->skillValue.maxspupp = sse.value;
	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.maxsp = (SWORD)(pEntry->getMaxSP()*(sse.value/100.0f));
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.maxsp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬33 ����״̬�����ߵ�������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_33(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 25;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.atrating = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.atrating = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.atrating = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬34 ����״̬����������ֵ�ظ��ٶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_34(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 26;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.hpspeedup = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.hpspeedup = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.hpspeedup = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬35 ����״̬�����߷���ֵ�ظ��ٶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_35(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 27;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.mpspeedup = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.mpspeedup = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.mpspeedup = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬36 ����״̬����������ֵ�ظ��ٶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_36(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 28;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.spspeedup = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.spspeedup = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.spspeedup = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬37 ����״̬������������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_37(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 29;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.akdodge = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.akdodge = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.akdodge = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬38 ����״̬����������ֵ�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_38(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 30;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeHP(sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeHP(sse.value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬39 ����״̬����������ֵ�ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_39(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 30;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.04f*pUser->charstate.wdMen);
		}
	}
	value +=sse.value;
	if (value>100) value =100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeHP((SDWORD)(pEntry->getMaxHP()*(value/100.0f)));
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeHP((SDWORD)(pEntry->getMaxHP()*(value/100.0f)));
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬40 ����״̬�����߷���ֵ�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_40(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 31;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeMP(sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeMP(sse.value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬41 ����״̬�����߷���ֵ�ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_41(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 31;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeMP((SDWORD)(pEntry->getMaxMP()*(sse.value/100.0f)));
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeMP((SDWORD)(pEntry->getMaxMP()*(sse.value/100.0f)));
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬42 ����״̬����������ֵ�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_42(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 32;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeSP(sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeSP(sse.value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬43 ����״̬����������ֵ�ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_43(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 32;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeSP((SDWORD)(pEntry->getMaxSP()*(sse.value/100.0f)));
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeSP((SDWORD)(pEntry->getMaxSP()*(sse.value/100.0f)));
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬44 ״̬�����߱�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_44(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 33;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->relive();
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬45 ״̬�������ܵ��˺�ʱ,�����̶�ֵ�˺�(������Զ�̷�����Զ�̹���)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_45(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 8;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reflect = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reflect = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reflect = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬46 ״̬�������ܵ��˺�ʱ,�����ٷֱ��˺�(������Զ�̷�����Զ�̹���)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_46(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 8;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reflectp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reflectp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reflectp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬47 ״̬�����߱�����3��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_47(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 64;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->standBack(sse.dwTempID, 3);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->standBack(sse.dwTempID, 3);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬48 ����Ŀ����������ڳ���ʱ���ڵ�ħ��Ч��(���渺��Ч����ȫ������)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_48(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 65;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillStatusM.clearActiveSkillStatusOnlyUseToStatus48();
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬49 ������ɫ����Ŀ��������ϵ����в���״̬(����״̬�������)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_49(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 66;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillStatusM.clearBadActiveSkillStatus();
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬50 ����ɫ��Ŀ���ܵ��˺�ʱ,�����ٷֱ��˺�(���Է������༼�ܹ����˺�)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_50(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 8;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reflect2 = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reflect2 = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reflect2 = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬51 �ж�״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_51(SceneEntryPk *pEntry, SkillStatusElement &sse) // ԭ����timer����
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 34;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent - pEntry->skillValue.poisonp-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent - pEntry->skillValue.poisonp-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getPoisondef()))
					{
						pEntry->sendMessageToMe("�ж�״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						return SKILL_ACTIVE;
					}
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					pEntry->directDamage(pAtt, sse.value, true);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬52 ʯ��״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_52(SceneEntryPk *pEntry, SkillStatusElement &sse) // ԭ����timer����
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 35;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent - pEntry->skillValue.petrifyp-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent - pEntry->skillValue.petrifyp-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getPetrifydef()))
					{
						pEntry->sendMessageToMe("ʯ��״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						pEntry->attackAction = false;
						//pEntry->ignoreDam = true;
						pEntry->moveAction = false;
						pEntry->useRes = false;
						//pEntry->stone = true;
						return SKILL_RECOVERY;
					}
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->attackAction = false;
				//pEntry->ignoreDam = true;
				pEntry->moveAction = false;
				pEntry->useRes = false;
				//pEntry->stone = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->attackAction = true;
				//pEntry->ignoreDam = false;
				pEntry->moveAction = true;
				pEntry->useRes = true;
				//pEntry->stone = false;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}


/**
 * \brief	״̬53 ʧ��״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_53(SceneEntryPk *pEntry, SkillStatusElement &sse)  //ԭ����timer����
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 36;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent - pEntry->skillValue.blindp-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent - pEntry->skillValue.blindp-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getBlinddef()))
					{
						pEntry->sendMessageToMe("ʧ��״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						pEntry->blind = true;
						return SKILL_ACTIVE;
					}
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->blind = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->blind = false;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}



/**
 * \brief	״̬54 ����״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_54(SceneEntryPk *pEntry, SkillStatusElement &sse) // ԭ����timer����
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 37;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent - pEntry->skillValue.chaosp-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent - pEntry->skillValue.chaosp-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getChaosdef()))
					{
						pEntry->sendMessageToMe("����״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						pEntry->dread = true;
						return SKILL_ACTIVE;
					}
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
/*				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					pEntry->directDamage(pAtt, sse.value, true);
					pEntry->processDeath(pAtt);
				}*/
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->dread = false;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}


/**
 * \brief	״̬55 ����״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_55(SceneEntryPk *pEntry, SkillStatusElement &sse)  // ԭ����time����
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 38;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent - pEntry->skillValue.coldp-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent - pEntry->skillValue.coldp-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getColddef()))
					{
						pEntry->sendMessageToMe("����״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						pEntry->reSendOther = true;
						pEntry->useRes = false;
						pEntry->attackAction = false;
						pEntry->skillAction = false;
						pEntry->reSendOther = true;
						pEntry->skillValue.movespeed = -900;
	
						//xwl add
						if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
							((SceneNpc *)pEntry)->resetSpeedRate();
						return SKILL_ACTIVE;
					}
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->reSendOther = true;
				pEntry->useRes = false;
				pEntry->attackAction = false;
				pEntry->skillAction = false;
				if(pEntry->skillValue.movespeed != -900)
				{
					pEntry->reSendOther = true;
				}
				pEntry->skillValue.movespeed = -900;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->reSendOther = true;
				pEntry->useRes = true;
				pEntry->attackAction = true;
				pEntry->skillAction = true;
				pEntry->reSendOther = true;
				pEntry->skillValue.movespeed = 0;
	
				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬56 ʹ��ɫ��Ŀ���ƶ��ٶȽ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_56(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 39;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getSlowdef()))
					{
						pEntry->sendMessageToMe("����״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						pEntry->reSendOther = true;
						pEntry->skillValue.movespeed = -900;
						//xwl add
						if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
							((SceneNpc *)pEntry)->resetSpeedRate();
						//pEntry->reSendMyMapData();
						return SKILL_RECOVERY;
					}
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if(pEntry->skillValue.movespeed != -900)
				{
					pEntry->reSendOther = true;
				}
				pEntry->skillValue.movespeed = -900;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

				//pEntry->reSendMyMapData();
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->reSendOther = true;
				pEntry->skillValue.movespeed = 0;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

				//pEntry->reSendMyMapData();
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬57 ����Ŀ����������̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_57(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 40;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.3f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dpdef = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dpdef = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dpdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬58 ����Ŀ����������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_58(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 40;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.04f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dpdefp = value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dpdefp = value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dpdefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬59 ����Ŀ�귨�������̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_59(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 41;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.3f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dmdef = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dmdef = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dmdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬60 �����������Ͱٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_60(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 41;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.04f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dmdefp = value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dmdefp = value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dmdefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬61 ʹ��ɫ��Ŀ������ֵ�����½�,�½��ٶȱ��ж���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_61(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 5;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					//pEntry->changeHP(0-sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					pEntry->directDamage(pAtt, sse.value, true);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬62 ʹ��ɫ��Ŀ�귨��ֵ�����½�,�ٶ�������ֵ���������൱
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_62(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 4;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeMP(0-sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeMP(0-sse.value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬63 ʹ��ɫ��Ŀ������ֵ�����½�,�ٶ�������ֵ���������൱
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_63(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 6;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeSP(0-sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeSP(0-sse.value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬64 ����Ŀ��ȫ��������,��������������,���෨������,Զ�̷���������Զ��������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_64(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 42;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reduce_atrating = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reduce_atrating = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reduce_atrating = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬65 �����˵�ע����������ʩ��������(ת������Ŀ��Ϊʩ����.����ʥ��ʿ�����ڹ�������Ŀ��Ĺ���ʩ��,ʹ���ｫ����Ŀ��תΪʥ��ʿ)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_65(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 43;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeAttackTarget(sse.dwTempID);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->changeAttackTarget(sse.dwTempID);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬66 ��Ŀ�����������Ϊ0
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_66(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 44;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->pdeftozero = true;   // ����������0
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->pdeftozero = false;   // ����������0
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬67 ��Ŀ�귨��������Ϊ0
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_67(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 44;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->mdeftozero = true;   // ����������0
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->mdeftozero = false;   // ����������0
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬68 ����Ŀ�겻��״̬����ʱ��̶�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_68(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 45;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillStatusM.addBadSkillStatusPersistTime(sse.value);
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬69 ����Ŀ�겻��״̬����ʱ��ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_69(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 45;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillStatusM.addBadSkillStatusPersistTimePercent(sse.value);
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬70 ʹĿ�겻�ܹ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_70(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 46;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->attackAction = false;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->attackAction = false;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->attackAction = true;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬71 ʹĿ�겻���ƶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_71(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 62;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getStabledef()))
					{
						pEntry->sendMessageToMe("����״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						pEntry->moveAction = false;
						return SKILL_RECOVERY;
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->moveAction = false;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->moveAction = true;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬72 ���ͽ�ɫ��Ŀ������ֵ�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_72(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	return SkillStatus_9(pEntry, sse);
}

/**
 * \brief	״̬73 ���ͽ�ɫ��Ŀ������ֵ�ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_73(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	return SkillStatus_10(pEntry, sse);
}

/**
 * \brief	״̬74 ���ͽ�ɫ��Ŀ�귨��ֵ�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_74(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	return SkillStatus_7(pEntry, sse);
}

/**
 * \brief	״̬75 ���ͽ�ɫ��Ŀ������ֵ�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_75(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	return SkillStatus_8(pEntry, sse);
}

/**
 * \brief	״̬76 ���ͽ�ɫ��Ŀ������ֵ�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_76(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	return SkillStatus_11(pEntry, sse);
}

/**
 * \brief	״̬77 ���ͽ�ɫ��Ŀ������ֵ�ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_77(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	return SkillStatus_12(pEntry, sse);
}

/**
 * \brief	״̬78 ʹ��ɫ��Ŀ������ֵ��Ϊ0,����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_78(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 33;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->toDie(sse.dwTempID);
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬79 ʹĿ���Ϊ���С����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_79(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 50;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.topet = sse.value;
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬80 �����������Ŀ�깥���ٶȰٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_80(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 51;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->reSendOther = true;
					pEntry->skillValue.uattackspeed = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if(pEntry->skillValue.uattackspeed != sse.value)
				{
					pEntry->reSendOther = true;
				}
				pEntry->skillValue.uattackspeed = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->reSendOther = true;
				pEntry->skillValue.uattackspeed = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬81 ����Ŀ�깥���ٶȰٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_81(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 52;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->reSendOther = true;
					pEntry->skillValue.dattackspeed = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if(pEntry->skillValue.dattackspeed != sse.value)
				{
					pEntry->reSendOther = true;
				}
				pEntry->skillValue.dattackspeed = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->reSendOther = true;
				pEntry->skillValue.dattackspeed = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬82 �����������Ŀ���������,�ж�,ʯ��,����,ʧ��,��Ժ���Ѫ�ļ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_82(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 53;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.sevendownp = 0-sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.sevendownp = 0-sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.sevendownp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬83 ��Ŀ����������˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_83(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 63;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.appenddam += sse.value;
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					/*SWORD wdHP = */pEntry->directDamage(pAtt,pEntry->skillValue.appenddam, true);
					//ScenePk::attackRTCmdToNine(rev , pAtt , this , wdHP , 0);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬84 ��ɫ��Ŀ������ƶ�,���ǲ���ʹ�ü��ܹ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_84(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 47;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getLulldef()))
					{
						pEntry->sendMessageToMe("���״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						pEntry->skillAction = false;
						pEntry->attackAction = false;
						return SKILL_RECOVERY;
					}
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillAction = false;
				pEntry->attackAction = false;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillAction = true;
				pEntry->attackAction = true;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬85 ��ɫ��Ŀ������ֵ�����½�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_85(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 48;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent - pEntry->skillValue.sevendownp)) > 0 ? (sse.percent - pEntry->skillValue.sevendownp):0))
				{
					//pEntry->changeHP(0-sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					pEntry->directDamage(pAtt, sse.value, true);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬86 ��ɫ��Ŀ�����������������̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_86(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.updef = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.updef = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.updef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬87 ��ɫ��Ŀ�����������������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_87(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{

					pEntry->skillValue.updefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.updefp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.updefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬88 ����ɫ�Ĺ������İٷֱ�ת���ɷ������ӵ���ɫ��������,ͬʱ������������Ӧ��ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_88(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 55;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					DWORD temp = (DWORD)(pEntry->pkValue.mdamage*(sse.value/100.0f));
					pEntry->pkValue.mdamage-=temp;
					pEntry->pkValue.mdefence+=temp;
					temp = (DWORD) (pEntry->pkValue.pdamage*(sse.value/100.0f));
					pEntry->pkValue.pdamage-=temp;
					pEntry->pkValue.pdefence+=temp;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬89 ����ɫ��Ŀ��������������͹̶���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_89(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 56;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(1*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dpdam = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dpdam = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dpdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬90 ����ɫ��Ŀ��������������Ͱٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_90(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 56;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.02f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dpdamp = value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dpdamp = value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dpdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬91 ����ɫ��Ŀ��ķ������������͹̶���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_91(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 57;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(1*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dmdam = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dmdam = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dmdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬92 ����ɫ��Ŀ��ķ������������Ͱٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_92(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 57;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.02f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dmdamp = value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dmdamp = value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dmdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬93 ���ڴ�״̬�µ�Ŀ�겻����ʩ����,Ŀ����״̬����ʱʼ�ճ�����ʩ���߱���3�����ϵľ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_93(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 58;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->standBack(sse.dwTempID, 3);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->standBack(sse.dwTempID, 3);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬94 ���ڴ�״̬�µ�Ŀ�겻��ʹ�ü���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_94(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 59;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillAction = false;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillAction = true;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬95 ����ɫ��Ŀ��ķ��������������̶���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_95(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.umdef = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.umdef = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.umdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬96 �������������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_96(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.umdefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.umdefp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.umdefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}




/**
 * \brief	״̬97 ����ɫ��Ŀ���ܵ����˺��̶�ֵת��������Ŀ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_97(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 61;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.tsfdam =sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.tsfdam =sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.tsfdam =0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬98 ����ɫ��Ŀ���ܵ����˺��ٷֱ�ת��������Ŀ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_98(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 61;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.tsfdamp =sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.tsfdamp =sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.tsfdamp =0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬99 ˲���ƶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_99(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 67;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->goToRandomScreen();
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬100 �Ƹ�����״̬��ħ�﹥������300������ʱ��������ø�������״̬)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_100(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.erupt = sse.value;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
			{
				pEntry->skillValue.erupt = 0;
				if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
				{
					SceneUser *pUser = (SceneUser *)pEntry;
					if (pUser->charbase.reliveWeakTime >0)
					{
						pUser->charbase.reliveWeakTime = (pUser->charbase.reliveWeakTime + sse.dwTime)%10000;
					}
					else
					{
						pUser->charbase.reliveWeakTime = (SceneTimeTick::currentTime.sec() + sse.dwTime)%10000;
						// ����Ԥ����������������
						pUser->setupCharBase();

						Cmd::stMainUserDataUserCmd  userinfo;
						pUser->full_t_MainUserData(userinfo.data);
						pUser->sendCmdToMe(&userinfo,sizeof(userinfo));

						pUser->showCurrentEffect(Cmd::USTATE_RELIVEWEAK, true);
						pUser->sendtoSelectedReliveWeakState();
					}
				}

			}
			break;
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.erupt = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬101 δ֪״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_101(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬102 Զ�̹���ȡ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_102(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->maxattack = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->maxattack = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->maxattack = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬110 ������ȡ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_110(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->maxattack = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->maxattack = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->maxattack = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬111 ����״̬������������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_111(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 29;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.akdodge = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.akdodge = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.akdodge = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬113 �ٻ��ٻ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_113(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	pEntry->summonPet(sse.value, Cmd::PET_TYPE_SUMMON, sse.dwTime, 0, "", 0, pEntry->keepPos);
	return SKILL_RETURN;
}

/**
 * \brief	״̬114 ���¼������м��ܵ���ȴʱ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_114(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->usm.resetAllUseTime();
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬115 ���ҹ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_115(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->frenzy = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->frenzy = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬116 ����ٻ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_116(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->killAllPets();
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬117 �����������˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_117(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 63;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.passdam += sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬119 ���ܻظ�HP,MP,SP
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_119(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->isAutoRestitute = false;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->isAutoRestitute = true;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬120 ����ʹ����Ʒ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_120(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 63;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->useRes = false;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->useRes = true;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬121 ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_121(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					//pEntry->assaultMe(sse.attacktype, sse.dwTempID);
					pEntry->reSendData = true;
					pEntry->assault = true;
					pEntry->reSendOther = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->assault = false;
				pEntry->reSendData = true;
				pEntry->reSendOther = true;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬122 ѣ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_122(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent-pEntry->skillValue.sevendownp):0))
				{
					if (zMisc::selectByPercent(pEntry->getReeldef()))
					{
						pEntry->sendMessageToMe("ѣ��״̬���ֿ�");
						return SKILL_RETURN;
					}
					else
					{
						pEntry->attackAction = false;
						pEntry->skillAction = false;
						pEntry->reSendOther = true;
						pEntry->skillValue.movespeed = -1900;

					//xwl add
						if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
							((SceneNpc *)pEntry)->resetSpeedRate();
						return SKILL_RECOVERY;
					}
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->attackAction = false;
				pEntry->skillAction = false;
				if(pEntry->skillValue.movespeed != -1900)
				{
					pEntry->reSendOther = true;
				}
				pEntry->skillValue.movespeed = -1900;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->attackAction = true;
				pEntry->skillAction = true;
				pEntry->reSendOther = true;
				pEntry->skillValue.movespeed = 0;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬123 ����ӡ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_123(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->huntermark = true;
					pEntry->skillValue.brappenddam = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->huntermark = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬124 ����״̬���������������̶�ֵ(����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_124(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 120;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.pupdam = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.pupdam = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pupdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬125 ����״̬�����߷����������̶�ֵ(����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_125(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 120;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.pumdam = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.pumdam = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pumdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬126 ����״̬����������������̶�ֵ(����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_126(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 121;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.pupdef = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.pupdef = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pupdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬127 ����״̬�����߷��������̶�ֵ(����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_127(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 121;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->checkWeapon(108))
						pEntry->skillValue.pumdef = sse.value;
					else
						pEntry->skillValue.pumdef = 0;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->checkWeapon(108))
					pEntry->skillValue.pumdef = sse.value;
				else
					pEntry->skillValue.pumdef = 0;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pumdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬128 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_128(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬129 ���ӽ�ɫȫ�����Ե�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_129(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 121;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.upattribute = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.upattribute = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.upattribute = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬130 ���������ʣ�������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_130(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 25;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.patrating = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.patrating = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.patrating = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬131 ���ӹ��������ٶȣ�������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_131(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 52;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->reSendOther = true;
					pEntry->skillValue.pattackspeed = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if(pEntry->skillValue.pattackspeed != sse.value)
				{
					pEntry->reSendOther = true;
				}
				pEntry->skillValue.pattackspeed = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->reSendOther = true;
				pEntry->skillValue.pattackspeed = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬132 ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_132(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	return SKILL_RECOVERY;
}

/**
 * \brief	״̬133 ȡ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_133(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	pEntry->isPhysics = true;
	return SKILL_RETURN;
}

/**
 * \brief	״̬134 ȡ��������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_134(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	pEntry->isPhysics = false;
	return SKILL_RETURN;
}

/**
 * \brief	״̬135 �������������������̶�ֵ(����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_135(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 120;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
						pEntry->skillValue.spupdam = sse.value;
					else
						pEntry->skillValue.spupdam = 0;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
					pEntry->skillValue.spupdam = sse.value;
				else
					pEntry->skillValue.spupdam = 0;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.spupdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬136 �������������������̶�ֵ(����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_136(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 120;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->checkWeapon(111))
						pEntry->skillValue.rpupdam = sse.value;
					else
						pEntry->skillValue.rpupdam = 0;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->checkWeapon(111))
					pEntry->skillValue.rpupdam = sse.value;
				else
					pEntry->skillValue.rpupdam = 0;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.rpupdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬137 ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_137(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 58;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->dread = true;
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->dread = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬138 ����������������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_138(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 25;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
						pEntry->skillValue.satrating = sse.value;
					else
						pEntry->skillValue.satrating = 0;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
					pEntry->skillValue.satrating = sse.value;
				else
					pEntry->skillValue.satrating = 0;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.satrating = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬139 �����ǿ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_139(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 25;
	pEntry->skillValue.tuling = sse.value;

	return SKILL_RETURN;
}

/**
 * \brief	״̬140 ���ù��ּ�ǿ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_140(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 25;
	pEntry->skillValue.kulou = sse.value;

	return SKILL_RETURN;
}

/**
 * \brief	״̬141 �����ǿ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_141(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 25;
	pEntry->skillValue.tianbing = sse.value;

	return SKILL_RETURN;
}

/**
 * \brief	״̬142 �ٻ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_142(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;
	pEntry->summonPet(sse.value, Cmd::PET_TYPE_SUMMON, sse.dwTime, (DWORD)(pEntry->skillValue.tuling), "", 0, pEntry->keepPos);
	return SKILL_RETURN;
}

/**
 * \brief	״̬143 �ٻ����ù���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_143(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	pEntry->summonPet(sse.value, Cmd::PET_TYPE_SUMMON, sse.dwTime, (DWORD)(pEntry->skillValue.kulou), "", 0, pEntry->keepPos);
	return SKILL_RETURN;
}

/**
 * \brief	״̬144 �ٻ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_144(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	pEntry->summonPet(sse.value, Cmd::PET_TYPE_SUMMON, sse.dwTime, (DWORD)(pEntry->skillValue.tianbing), "", 0, pEntry->keepPos);
	return SKILL_RETURN;
}

/**
 * \brief	״̬145 ����״̬���������������̶�ֵ(ֻ���������м���ʱ���״̬����Ч)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_145(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->checkReduce(BOW_ARROW_ITEM_TYPE,1))
					{
						pEntry->skillValue.lupdam = sse.value;
					}
					else
					{
						pEntry->skillValue.lupdam = 0;
					}
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.lupdam = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->skillValue.lupdam >0)
					pEntry->reduce(BOW_ARROW_ITEM_TYPE,1);
				pEntry->skillValue.lupdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬146 ����״̬���������������̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_146(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.lupdam = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.lupdam = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.lupdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬147 �������������������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_147(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.weaponupdamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.weaponupdamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.weaponupdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬148 ���������ķ����������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_148(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.weaponumdamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.weaponumdamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.weaponumdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬149 �����������ٻ��޵�λ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_149(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	
	pEntry->exchangeMeAndSummonPet();

	return SKILL_RETURN;
}

/**
 * \brief	״̬150 ����״̬����������ֵ���� ���Ӳ���Ϊϵ�������˵ķ���ֵʣ��������ٻ���ר��״̬��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_150(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

//	pEntry->skillValue.maxhpup = sse.value;
	switch(sse.byStep)
	{
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.maxhp=(SWORD)sse.value;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_DOPASS:
			break;
		case ACTION_STEP_START:
			{
				//if (pEntry->skillValue.uppetdamage >0) break;
				pEntry->skillValue.maxhp = (DWORD)(pEntry->getMasterMana()*sse.value/1000.0f);
				pEntry->changeHP(pEntry->getMaxHP());
				sse.value = (WORD)pEntry->skillValue.maxhp;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.maxhp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬151 ���������ӹ����� ����ֵ�������˵ķ���ֵ������ϵ�����ٻ���ר��״̬��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_151(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

//	pEntry->skillValue.maxhpup = sse.value;
	switch(sse.byStep)
	{
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.uppetdamage = (SWORD)sse.value;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_DOPASS:
			break;
		case ACTION_STEP_START:
			{
				//if (pEntry->skillValue.uppetdamage >0) break;
				pEntry->skillValue.uppetdamage = (DWORD)(pEntry->getMasterMana()*(sse.value/1000.0f));
				sse.value = (WORD)pEntry->skillValue.uppetdamage;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.uppetdamage = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬152 ����״̬�����߷����� ���Ӳ���Ϊϵ�������˵ķ���ֵʣ��������ٻ���ר��״̬��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_152(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

//	pEntry->skillValue.maxhpup = sse.value;
	switch(sse.byStep)
	{
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.uppetdefence = (SWORD)sse.value;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_DOPASS:
			break;
		case ACTION_STEP_START:
			{
				//if (pEntry->skillValue.uppetdefence >0) break;
				pEntry->skillValue.uppetdefence = (DWORD)(pEntry->getMasterMana()*(sse.value/1000.0f));
				sse.value = (WORD)pEntry->skillValue.uppetdefence;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.uppetdefence = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬153 ����״̬����������ֵ���� ���Ӳ���Ϊϵ�������˵ķ���ֵʣ��������ٻ���ר��״̬��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_153(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

	pEntry->clearMana();
	return SKILL_RETURN;
}

/**
 * \brief	״̬154 �������������������ٷֱ�(����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_154(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 120;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
						pEntry->skillValue.spupdamp = sse.value;
					else
						pEntry->skillValue.spupdamp = 0;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
					pEntry->skillValue.spupdamp = sse.value;
				else
					pEntry->skillValue.spupdamp = 0;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.spupdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬155 �������������������̶�ֵ(����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_155(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 120;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
						pEntry->skillValue.supdam = sse.value;
					else
						pEntry->skillValue.supdam = 0;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
					pEntry->skillValue.supdam = sse.value;
				else
					pEntry->skillValue.supdam = 0;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.supdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬156 (����)����ֵ��������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_156(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.pmaxhp = (SWORD)(sse.value);
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pmaxhp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬157 ��������ʩ�ų��ػ��ļ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_157(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.bang = (SWORD)(sse.value);
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.bang = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬158 ��������״̬���������������̶�ֵ(��Ч)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_158(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.8f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_updam = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_updam = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_updam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬159 �������ӽ�ɫ��Ŀ�����������������̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_159(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.5f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_updef = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_updef = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_updef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬160 �������ӽ�ɫ��Ŀ��ķ��������������̶���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_160(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.5f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_umdef = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_umdef = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_umdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬161 ��Ŀ����������˺�,�˺�ֵȡ����������ֵ��X%
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_161(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 63;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					SceneEntryPk *pAtt = NULL;
					switch(sse.attacktype)
					{
						case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
							{
								pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
							}
							break;
						case zSceneEntry::SceneEntry_NPC:		// NPC
							{
								pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
							}
							break;
						default:
							break;
					}
					if (!pAtt) return SKILL_RETURN;
					pEntry->skillValue.appenddam += (SWORD)(pAtt->getMaxHP()*(sse.value/100.0f));
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					/*SWORD wdHP = */pEntry->directDamage(pAtt,pEntry->skillValue.appenddam, true);
					//ScenePk::attackRTCmdToNine(rev , pAtt , this , wdHP , 0);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬163 ȥ���������������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_163(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 60;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->dropweapon = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->dropweapon = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->dropweapon = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬165 ����-�ٻ����ܵ����˺�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_165(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->summon)
					{
						pEntry->summon->dwReduceDam = sse.value;
						return SKILL_RECOVERY;
					}
					else
						return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon)
				{
					pEntry->summon->dwReduceDam = sse.value;
					return SKILL_RECOVERY;
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
					if (pEntry->summon)
					{
						pEntry->summon->dwReduceDam = 0;
						return SKILL_RECOVERY;
					}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬168 �ٻ�������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_168(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.summonrelive = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.summonrelive = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.summonrelive = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬169 ��������״̬���������������̶�ֵ(��Ч)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_169(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.8f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_umdam = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_umdam = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_umdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬170 (����)����ֵ��������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_170(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.pmaxhp = (SWORD)(sse.value);
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.pmaxhp = (SWORD)(sse.value);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pmaxhp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬172 �������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_172(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;

	SceneUser *pUser = NULL;
	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(1*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->watchTrap = true;
					if (ACTION_STEP_START == sse.byStep) sse.dwTime += value;
					if (pUser) pUser->sendNineToMe();
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->watchTrap = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->watchTrap = false;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬173 �ٻ��ں�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_173(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_RELOAD:
			{
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
			{
				if (pEntry->summon && ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->skillValue.introject_maxmdam = (WORD)(pEntry->summon->getMaxMDamage()*sse.value/100.0f); // �ٻ���������ħ������
					pEntry->skillValue.introject_maxpdam = (WORD)(pEntry->summon->getMaxPDamage()*sse.value/100.0f); // �ٻ���������������
					pEntry->skillValue.introject_mdam	 = (WORD)(pEntry->summon->getMinMDamage()*sse.value/100.0f); // �ٻ���������ħ������
					pEntry->skillValue.introject_pdam	 = (WORD)(pEntry->summon->getMinPDamage()*sse.value/100.0f); // �ٻ���������������
					pEntry->skillValue.introject_mdef	 = (WORD)((pEntry->summon->getMaxMDefence()*sse.value/100.0f)*0.5f); // �ٻ���������ħ������
					pEntry->skillValue.introject_pdef	 = (WORD)((pEntry->summon->getMaxPDefence()*sse.value/100.0f)*0.5f); // �ٻ����������������
					pEntry->skillValue.introject_maxhp	 = (WORD)((pEntry->summon->getMaxHP()*sse.value/100.0f)); // �ٻ����������������
					pEntry->killSummon();
					if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
					{
						SceneUser *pUser = (SceneUser *)pEntry;
						pUser->setupCharBase();
						pUser->changeMP(pUser->getMaxMP());
						pUser->changeHP(pUser->getMaxHP());						
					}
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.introject_maxmdam = 0;
				pEntry->skillValue.introject_maxpdam = 0;
				pEntry->skillValue.introject_mdam	 = 0;
				pEntry->skillValue.introject_pdam	 = 0;
				pEntry->skillValue.introject_mdef	 = 0;
				pEntry->skillValue.introject_pdef	 = 0;
				pEntry->skillValue.introject_maxhp   = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}


/**
 * \brief	״̬176 ����һ������״̬�������Լ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_176(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reflect_ardor = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reflect_ardor = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reflect_ardor = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬177 ѵ��Ұ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_177(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}
				if (!pAtt) return SKILL_RETURN;

				int percent=0;
				percent = (pAtt->getLevel() - pEntry->getLevel() +1)*sse.percent;
				if (percent<0) percent =0;
				if (percent>100) percent =100;
				if(zMisc::selectByPercent(percent))
				{
					SceneUser *pUser = (SceneUser *)pAtt;
					if (pEntry->getType() == zSceneEntry::SceneEntry_NPC)
					{
						pUser->captureIt((SceneNpc *)pEntry, sse.value);
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬178 �������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_178(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->relivePet();
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬179 ����ѵ��(���ﱻ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_179(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->pet) pEntry->pet->catchme = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->pet) pEntry->pet->catchme = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->pet) pEntry->pet->catchme = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬180 ������ǿ(���ﱻ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_180(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->pet && (pEntry->pet->boostupPet==0) && (pEntry->pet->npc->bear_type==0))
				{
					pEntry->pet->boostupPet = (int)sse.value;
					pEntry->pet->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->pet&&pEntry->pet->npc->bear_type==0) pEntry->pet->boostupPet = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->pet&&pEntry->pet->npc->bear_type==0)
				{
					pEntry->pet->boostupPet = 0;
					pEntry->pet->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬182 �������ʹ���˵��ȫΪ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_182(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 60;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.2f*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->tosh = true;
					if (ACTION_STEP_START == sse.byStep) sse.dwTime+=value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->tosh = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->tosh = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬184 սʿ�������������ӣ�������Ҫ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_184(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->needWeapon(sse.dwSkillID))
						pEntry->skillValue.hpupbylevel = sse.value;
					else
						pEntry->skillValue.hpupbylevel = 0;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->needWeapon(sse.dwSkillID))
					pEntry->skillValue.hpupbylevel = sse.value;
				else
					pEntry->skillValue.hpupbylevel = 0;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.hpupbylevel = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬185 ����һ���ж�״̬�������Լ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_185(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reflect_poison = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reflect_poison = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reflect_poison = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬186 ����״̬���������������ٷֱ�(����ĳ����ר��)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_186(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_umdamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_umdamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_umdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬187 ����ɫ��������ת�������������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_187(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 57;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.04f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.pdamtodef = (DWORD)(pEntry->getMaxPDamage()*value/100.0f);
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.pdamtodef = (DWORD)(pEntry->getMaxPDamage()*value/100.0f);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pdamtodef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬188 ����ɫ�۳�������������ǿ����������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_188(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 57;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.04f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.mdamtodef = (DWORD)(pEntry->getMaxMDamage()*value/100.0f);
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.mdamtodef = (DWORD)(pEntry->getMaxPDamage()*value/100.0f);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.mdamtodef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

//-----------

/**
 * \brief	״̬189 ����ɫ�۳����������ת����������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_189(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 57;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.04f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.pdeftodam = (DWORD)(pEntry->getPDefence()*value/100.0f);
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.pdeftodam = (DWORD)(pEntry->getPDefence()*value/100.0f);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pdeftodam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬190 ����ɫ�۳�����������ת�ɷ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_190(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 57;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.04f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.mdeftodam = (DWORD)(pEntry->getMDefence()*value/100.0f);
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.mdeftodam = (DWORD)(pEntry->getMDefence()*value/100.0f);
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.mdeftodam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬191 ���ӳ����ƶ��ٶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_191(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->pet)
						pEntry->pet->setSpeedRate(640.0f/((640-sse.value)<=0?1:(640-sse.value)));
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->pet)
					pEntry->pet->setSpeedRate(640.0f/((640-sse.value)<=0?1:(640-sse.value)));
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->pet)
					pEntry->pet->resetSpeedRate();
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬192 ÿ���ͷ�һ�������˺���Χ5*5�ķ�Χ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_192(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Null;//Ani_Num;
				cmd.byDirect = pEntry->getDir();

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬193 ״̬����ʱ�˺�״̬�����߼�����Χ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_193(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				Cmd::stAttackMagicUserCmd cmd;

				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}
				if (!pAtt) return SKILL_RETURN;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2U;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_N2U; //·Ҫ���
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2U;
						}
						break;
				}
		
				cmd.dwDefenceTempID = pEntry->tempid;
				cmd.dwUserTempID = pAtt->tempid;
				cmd.wdMagicType = 291;
				ScenePk::attackUserCmdToNine(&cmd , pEntry);
				if (pAtt)
				{
					pEntry->directDamage(pAtt, sse.value, true);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬194 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_194(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬195 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_195(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬196 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_196(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬197 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_197(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬198 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_198(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬199 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_199(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬200 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_200(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬201 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_201(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬202 ��ǿ(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_202(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬203 �������ӽ�ɫ��Ŀ�����������������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_203(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(0.02f*pUser->charstate.wdMen);
		}
	}
	value+=sse.value;
	if (value >100) value=100;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_updefp = value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_updefp = value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_updefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬204 ����һ�����״̬�������Լ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_204(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reflect_lull = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reflect_lull = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reflect_lull = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

//--����--------------------------------------------

/**
 * \brief	״̬205 ����״̬���������������ٷֱȣ����壩
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_205(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.sept_updamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.sept_updamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.sept_updamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬206 ����״̬�����߷����������ٷֱȣ����壩
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_206(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 211;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.sept_umdamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.sept_umdamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.sept_umdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬207 ��ɫ��Ŀ�����������������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_207(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{

					pEntry->skillValue.sept_updefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.sept_updefp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.sept_updefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬208 �������������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_208(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.sept_umdefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.sept_umdefp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.sept_umdefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬209 ����״̬����������ֵ���ްٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_209(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.sept_maxhp = (SWORD)(pEntry->getBaseMaxHP()*(sse.value/100.0f));
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.sept_maxhp = 0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬210 ����״̬�����߷���ֵ���ްٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_210(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 23;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.sept_maxmp = (SWORD)(pEntry->getBaseMaxMP()*(sse.value/100.0f));
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.sept_maxmp =0;
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬216 �ٻ�ͼ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_216(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	pEntry->summonPet(sse.value, Cmd::PET_TYPE_TOTEM, sse.dwTime, 0, "", 0, pEntry->keepPos, pEntry->keepDir);
	return SKILL_RETURN;
}

/**
 * \brief	״̬217 ÿ�ι�����ʱ�򸽼ӷ��������������ķ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_217(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 120;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.magicattack = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.magicattack = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬218 ת��ʩ���߷���ֵ�ٷֱȸ�״̬������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_218(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 31;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}
				if (!pAtt) return SKILL_RETURN;
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					SDWORD vchange = (SDWORD)(pAtt->getMP()*sse.value/100.0f);
					pAtt->changeMP(0-vchange);
					pEntry->changeMP(vchange);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬219 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_219(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬220 ˲���ƶ������ָ��λ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_220(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 67;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->jumpTo(pEntry->keepPos);
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬221 �������������԰ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_221(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 67;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.addmenp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.addmenp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬222 ����״̬��״̬���������ܵ����˺�ȫ��ת�Ʋ�����ʩ�ӵ�״̬ʩ��������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_222(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 67;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->swapdamcharid = sse.dwTempID;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->swapdamcharid = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬223 ��߷���֮����ϵ���ܵ��˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_223(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.pblazeappend = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pblazeappend = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬224 ���������﹥����ʱ���л�����Ŀ��ѣ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_224(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->pet)
					{
						pEntry->pet->giddy = sse.value;
					}
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->pet)
				{
					pEntry->pet->giddy = 0;
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬225 ����һ������״̬�������Լ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_225(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reflect_frost = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reflect_frost = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reflect_frost = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬226 ����һ������״̬�������Լ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_226(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->mhpd = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->mhpd = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬227 ��ɫ��Ŀ���������������͹̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_227(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_dpdef = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_dpdef = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_dpdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬228 ��ɫ��Ŀ��ķ����������½��̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_228(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_dmdef = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_dmdef = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_dmdef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬229 �����ķ����⻷״̬������ͷ��������������̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_229(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.udef = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.udef = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.udef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬230 ������ÿ���ͷ�һ����������Ӷ�Ա����״̬�ӳ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_230(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
					{
						SceneUser *pUser = (SceneUser *)pEntry;
						if (pUser->team.IsTeamed() && pUser->team.getLeader() == pUser->tempid)
							return SKILL_ACTIVE;
					}
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
				{
					SceneUser *pUser = (SceneUser *)pEntry;
					if (!pUser->team.IsTeamed())
					{
						sse.dwTime = 0;
						return SKILL_RETURN;
					}
					else
					{
						if (!pUser->team.canPutSkill())
						{
							sse.dwTime = 0;
							return SKILL_RETURN;
						}
					}

					sse.dwTime = 100;

					cmd.byAttackType = Cmd::ATTACKTYPE_U2P;

					cmd.dwDefenceTempID = 0;
					cmd.dwUserTempID = pEntry->tempid;
					cmd.wdMagicType = sse.value;
					cmd.byAction = Cmd::Ani_Num;
					cmd.byDirect = pEntry->getDir();

					zSkill *s = NULL;

					s = zSkill::createTempSkill(pEntry , sse.value , 1);
					if(s)
					{
						s->action(&cmd, sizeof(cmd));
						SAFE_DELETE(s);
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬231 ������ר��״̬����˫��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_231(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
					{
						SceneUser *pUser = (SceneUser *)pEntry;
						int num = pUser->team.getSize();
						if (num>=2) num-=2;
						else num=0;
						pEntry->skillValue.array_udamp = sse.value+num*3;
						return SKILL_RECOVERY;
					}
					else
						return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.array_udamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬232 ������ר��״̬�����ƶ��ٶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_232(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 9;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.array_dmvspeed = sse.value;

					//xwl add
					if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
						((SceneNpc *)pEntry)->resetSpeedRate();
					//pEntry->reSendMyMapData();
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.array_dmvspeed = 0;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

				//pEntry->reSendMyMapData();
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬233 ������ר��״̬����˫��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_233(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.array_ddefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.array_ddefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬234 ��������������ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_234(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.upcon = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.upcon = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬235 ����״̬��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_235(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->changeface = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->changeface = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬236 �����������������������ڳ��й��������
	�����Ƕ�Ч����Ͷ�͵ļ���״̬���Բ��������������ڼӳɵĵط�ֱ�Ӽӳɵķ�ʽ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_236(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->checkWeapon(109)&&pEntry->checkWeapon(21))
					{
						pEntry->skillValue.sword_udam = sse.value;
						return SKILL_RECOVERY;
					}
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.sword_udam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬237 ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_237(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
					{
						SceneUser *pUser = (SceneUser *)pEntry;
						pUser->relive(Cmd::ReliveSkill, 0, sse.value);
					}
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬238 ����ÿ���ͷ�һ����������Ӷ�Ա����״̬�ӳ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_238(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
					{
						SceneUser *pUser = (SceneUser *)pEntry;
						if (pUser->team.IsTeamed() && pUser->team.getLeader() == pUser->tempid && pUser->team.getSize()>=3)
							return SKILL_ACTIVE;
					}
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
				{
					SceneUser *pUser = (SceneUser *)pEntry;
					if (!(pUser->team.IsTeamed() && pUser->team.getSize()>=3))
					{
						sse.dwTime = 0;
						return SKILL_RETURN;
					}
					else
					{
						pUser->changeMP(-7);
						if (!(pUser->team.canPutSkill() && pUser->getMP()>=7))
						{
							sse.dwTime = 0;
							return SKILL_RETURN;
						}
					}

					sse.dwTime = 100;

					cmd.byAttackType = Cmd::ATTACKTYPE_U2P;

					cmd.dwDefenceTempID = 0;
					cmd.dwUserTempID = pEntry->tempid;
					cmd.wdMagicType = sse.value;
					cmd.byAction = Cmd::Ani_Num;
					cmd.byDirect = pEntry->getDir();

					zSkill *s = NULL;

					s = zSkill::createTempSkill(pEntry , sse.value , 1);
					if(s)
					{
						s->action(&cmd, sizeof(cmd));
						SAFE_DELETE(s);
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬239 ������ר��״̬�����ƶ��ٶ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_239(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 9;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					SceneEntryPk *pAtt = NULL;
					switch(sse.attacktype)
					{
						case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
							{
								pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
							}
							break;
						default:
							break;
					}
					if (pAtt)
					{
						SceneUser *pUser = (SceneUser *)pAtt;
						int num = pUser->team.getSize();
						if (num>=3) num-=3;
						else num=0;
						pEntry->skillValue.array_dmvspeed = sse.value+num*5;
	
						//xwl add
						if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
							((SceneNpc *)pEntry)->resetSpeedRate();
						//pEntry->reSendMyMapData();
						return SKILL_RECOVERY;
					}
					else
						return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.array_dmvspeed = 0;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

				//pEntry->reSendMyMapData();
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬240 ��������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_240(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬241 ����״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_241(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->hideme = true;
					pEntry->skillAction = false;
					pEntry->attackAction = false;
					if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
					{
						Cmd::stRemoveUserMapScreenUserCmd remove;
						remove.dwUserTempID=pEntry->tempid;
						((SceneUser *)pEntry)->scene->sendCmdToNineUnWatch(pEntry->getPosI(),&remove,sizeof(remove));
					}
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->hideme = false;
				pEntry->skillAction = true;
				pEntry->attackAction = true;
				if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
				{
					//FunctionTimes times(39,__FUNCTION__);
					((SceneUser *)pEntry)->sendMeToNine();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬242 �м��ʴӶԵ��˵��˺��ϻ�ȡ����ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_242(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.attackaddhpnum = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.attackaddhpnum = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬243 �м������ܵ��˵Ĺ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_243(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dodge = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dodge = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬244 ����һ�����״̬�������Լ������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_244(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.reflect_icelull = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.reflect_icelull = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.reflect_icelull = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬245 �����״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_245(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->icebox = true;
					pEntry->moveAction = false;
					pEntry->attackAction = false;
					pEntry->skillAction = false;
					pEntry->useRes = false;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->icebox = false;
				pEntry->moveAction = true;
				pEntry->attackAction = true;
				pEntry->skillAction = true;
				pEntry->useRes = true;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬246 ���ɣ�����ɫ��Ŀ��������������͹̶���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_246(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 56;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(1*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_dpdam = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_dpdam = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_dpdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬247 ����ɫ��Ŀ��ķ������������͹̶���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_247(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 57;
	pEntry->reSendData = true;

	WORD value=0;
	if (sse.attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = NULL;
		pUser = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
		if (pUser)
		{
			value= (WORD)(1*pUser->charstate.wdMen);
		}
	}

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_dmdam = sse.value+value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_dmdam = sse.value+value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_dmdam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬248 Һ��״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_248(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->liquidState = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->liquidState = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬249 ״̬������ʱ��Ͷ�ͼ���,��״̬Ͷ���ߵ����Ͷ�ͼ��ܡ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_249(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
			}
			break;
		case ACTION_STEP_STOP:
			{
				Cmd::stAttackMagicUserCmd cmd;

				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					switch (pAtt->getType())
					{
						case zSceneEntry::SceneEntry_Player:
							{
								cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
							}
							break;
						case zSceneEntry::SceneEntry_NPC:
							{
								cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
							}
							break;
						default:
							{
								cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
							}
							break;
					}
		
					cmd.dwDefenceTempID = pEntry->tempid;
					cmd.dwUserTempID = pAtt->tempid;
					cmd.wdMagicType = sse.value;
					cmd.byAction = Cmd::Ani_Num;
					cmd.byDirect = pAtt->getDir();
					cmd.xDes = (WORD)pEntry->getPos().x;
					cmd.yDes = (WORD)pEntry->getPos().y;

					zSkill *s = NULL;

					s = zSkill::createTempSkill(pAtt , sse.value , 1);
					if(s)
					{
						s->action(&cmd, sizeof(cmd));
						SAFE_DELETE(s);
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_CLEAR:
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬250 ״̬������ʱ��Ͷ�ͼ���,��״̬Ͷ���ߵ����Ͷ�ͼ��ܡ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_250(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->bombskillId = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->bombskillId = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬251 ��������������ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_251(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.upint = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.upint = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬252 ��������״̬�����߷����������ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_252(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 211;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.pumdamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.pumdamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.pumdamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬253 �۳�����ֵ�̶�ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_253(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 211;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}
				if (pAtt)
				{
					SDWORD bkMp= pEntry->getMP();
					pEntry->changeMP(0-sse.value);
					bkMp-=pEntry->getMP();
					if (bkMp<0)bkMp=0;
					SDWORD dam = (SDWORD)(bkMp*(sse.percent/100.0f));
					pEntry->directDamage(pAtt, dam,true);
					pEntry->processDeath(pAtt);

					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬254 ������ǿ(���ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_254(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->pet && (pEntry->pet->boostupPet==0) && (pEntry->pet->npc->bear_type==2))
				{
					pEntry->pet->boostupPet = (int)sse.value;
					pEntry->pet->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->pet&&pEntry->pet->npc->bear_type==2) pEntry->pet->boostupPet = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->pet&&pEntry->pet->npc->bear_type==2)
				{
					pEntry->pet->boostupPet = 0;
					pEntry->pet->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬255 �׼�״̬��ɱ�����ｫ��������ֵ�İٷ�֮X�ӵ��Լ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_255(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 22;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->pet)
				{
					pEntry->changeHP((SDWORD)(pEntry->pet->hp*(sse.value/100.0f)));
					pEntry->pet->toDie(pEntry->tempid);
					pEntry->killOnePet(pEntry->pet);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬256 ��߷���֮�׵�ϵ���ܵ��˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_256(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.levinappend = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.levinappend = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬257 �ٻ��ٻ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_257(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 0;

	zNpcB *base = npcbm.get(sse.value);
	if (base)
	{
		t_NpcDefine define;
		zPos pos = pEntry->getPos();
		define.id = base->id;
		strcpy(define.name, base->name);
		define.pos = pEntry->getPos();
		define.width = SCREEN_WIDTH;
		define.height = SCREEN_HEIGHT;
		define.num = 1;
		define.interval = 30;
		define.initstate = zSceneEntry::SceneEntry_Normal;
		//define.scriptID = atoi(prop["script"].c_str());

		pEntry->scene->zPosRevaluate(define.pos);
		define.pos -= zPosHalfScreen;
		pEntry->scene->zPosRevaluate(define.pos);
		pos -= zPosHalfScreen;
		pEntry->scene->zPosRevaluate(pos);
		pEntry->scene->initRegion(define.region, define.pos, define.width, define.height);
		pEntry->scene->summonNpc(define, pos, base);
	}
	return SKILL_RETURN;
}

/**
 * \brief	״̬258 ��������������ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_258(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 7;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.updex = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.updex = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬259 �м��ʺ���һ���˺�״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_259(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->resist = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->resist = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬260 ��ħ����ר��״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_260(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->dmpbyhp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->dmpbyhp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬261 ���﷨��������ǿ(���ﱻ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_261(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->pet && (pEntry->pet->boostupPetMDef==0))
				{
					pEntry->pet->boostupPetMDef = (int)sse.value;
					pEntry->pet->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->pet) pEntry->pet->boostupPetMDef = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->pet)
				{
					pEntry->pet->boostupPetMDef = 0;
					pEntry->pet->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬262 �������ר��״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_262(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->icedam ==0) pEntry->icedam = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->icedam = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬263 ����״̬������ר�ã�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_263(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.protectdam = (WORD)(pEntry->getMaxHP()*(sse.percent/100.0f));
				pEntry->skillValue.protectUpAtt = sse.value;
				return SKILL_ACTIVE;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					pEntry->directDamage(pAtt, pEntry->skillValue.protectdam,true);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.protectdam = 0;
				pEntry->skillValue.protectUpAtt = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬264 һԪ��ÿ���ͷ�һ����������Ӷ�Ա�ӳ�״̬
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_264(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
					{
						SceneUser *pUser = (SceneUser *)pEntry;
						if (pUser->team.IsTeamed() && pUser->team.getLeader() == pUser->tempid && pUser->team.getSize()>=2)
						{
							zRTime ctv;
							pUser->unitaryBattleKeepTime = ctv.msecs()+sse.dwTime*1000;
							return SKILL_ACTIVE;
						}
					}
					return SKILL_RETURN;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
				{
					SceneUser *pUser = (SceneUser *)pEntry;
					if (!(pUser->team.IsTeamed() && pUser->team.getSize()>=2 && (SceneTimeTick::currentTime.msecs() <pEntry->unitaryBattleKeepTime)))
					{
						sse.dwTime = 0;
						return SKILL_RETURN;
					}
//					else
//					{
//						if (!(pUser->team.canPutSkill()))
//						{
//							sse.dwTime = 0;
//							return SKILL_RETURN;
//						}
//					}

//					sse.dwTime = 100;

					cmd.byAttackType = Cmd::ATTACKTYPE_U2P;

					cmd.dwDefenceTempID = 0;
					cmd.dwUserTempID = pEntry->tempid;
					cmd.wdMagicType = sse.value;
					cmd.byAction = Cmd::Ani_Num;
					cmd.byDirect = pEntry->getDir();

					zSkill *s = NULL;

					s = zSkill::createTempSkill(pEntry , sse.value , 1);
					if(s)
					{
						s->action(&cmd, sizeof(cmd));
						SAFE_DELETE(s);
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
//				pEntry->skillValue.unitarybattle = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬265 һԪ��Ͷ�͵��ӳ������ĵ������ϣ���Ӷ�Ա������ʱ�����״̬�����������˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_265(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->skillValue.teamappend = sse.dwTempID;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			{
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.teamappend = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬266 һԪ��Ͷ��һ����Χ�ڵ���Ӷ�Ա����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_266(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->getType() == zSceneEntry::SceneEntry_Player)
				{
					SceneUser *pUser = (SceneUser *)pEntry;
					if (pUser->team.IsTeamed() && pUser->team.getLeader() == pUser->tempid)
						pEntry->skillValue.unitarybattle = 696; //�����ڹ�����ʱ���Ͷ��һ������˺�״̬�����
				}
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			{
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.unitarybattle = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬268 �������˺�ת����ħ��ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_268(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->mdmtompper = sse.percent;
				pEntry->mdmtomp = sse.value;
				return SKILL_RECOVERY;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->mdmtompper=0;
				pEntry->mdmtomp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬270 �ذ�״̬��״̬���������ܵ����˺�ȫ��ת��ʩ�ӵ�״̬ʩ��������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_270(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 67;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->swapdamcharid2 = sse.dwTempID;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->swapdamcharid2 = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬271 �������֮����ϵ���ܵ��˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_271(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.iceappend = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.iceappend = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬272 �������֮����ϵ���ܵ��˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_272(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.trapappend = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.trapappend = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬273 ����״̬���������������ٷֱ�(ĳ����ר��)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_273(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.nsc_updamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.nsc_updamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.nsc_updamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬274 ��ɫ��Ŀ�����������������̶�ֵ������֮�꣩
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_274(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.tgzh_updef = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.tgzh_updef = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.tgzh_updef = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬275 �عⷵ�ռ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_275(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->ignoreDam = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->ignoreDam = true;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->ignoreDam = false;
				pEntry->toDie(pEntry->tempid);
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬276 ��ɫ��Ŀ�����������������ٷֱȣ��������壩
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_276(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 54;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.tgyt_updefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.tgyt_updefp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.tgyt_updefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬277 ����״̬���������������ٷֱ�(³ç����ר��)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_277(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.lm_updamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.lm_updamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.lm_updamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬278 ��߷���֮�׵�ϵ���ܵ��˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_278(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.plevinappend = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.plevinappend = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬279 ������ɱ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_279(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->dietodam = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->dietodam=0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬280 ��߷���֮�׵�ϵ���ܵ��˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_280(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
//				if(zMisc::selectByPercent((int)(sse.percent)))
//				{
					pEntry->skillValue.relive = sse.percent;
					return SKILL_RECOVERY;
//				}
//				else
//					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
			{
				pEntry->skillValue.relive = 0;
			}
			break;
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬281 ��߷���֮�׵�ϵ���ܵ��˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_281(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬282 ��߷���֮����ϵ���ܵ��˺�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_282(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.blazeappend = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.blazeappend = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬283 ʹ��ɫ��Ŀ�귨��ֵ�����½�,�ٶ�������ֵ���������൱
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_283(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 4;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->ignorePhysicDam = true;
					pEntry->changeMP(0-sse.value);
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->getMP() >= sse.value)
				{
					pEntry->changeMP(0-sse.value);
				}
				else
				{
					pEntry->ignorePhysicDam = false;
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->ignorePhysicDam = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬284 ����״̬���������������ٷֱ�(����ĳ����ר��)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_284(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 201;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_updamp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_updamp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_updamp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬285 �������������ٷֱ�(����ĳ����ר��)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_285(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.theurgy_umdefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.theurgy_umdefp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.theurgy_umdefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬286 ��ũ����ר��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_286(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->useRes = false;
					pEntry->moveAction = false;
					pEntry->skillAction = false;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
					pEntry->useRes = true;
					pEntry->moveAction = true;
					pEntry->skillAction = true;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬287 PKֵ���ٽ���
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_287(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬288 ѣ�Σ����ר�ã�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_288(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 0;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent(((int)(sse.percent-pEntry->skillValue.sevendownp)) > 0 ? (sse.percent-pEntry->skillValue.sevendownp):0))
				{
					pEntry->attackAction = false;
					pEntry->skillAction = false;
					pEntry->reSendOther = true;
					pEntry->skillValue.movespeed = -1900;

					//xwl add
					if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
						((SceneNpc *)pEntry)->resetSpeedRate();
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->attackAction = false;
				pEntry->skillAction = false;
				if(pEntry->skillValue.movespeed != -1900)
				{
					pEntry->reSendOther = true;
				}
				pEntry->skillValue.movespeed = -1900;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->attackAction = true;
				pEntry->skillAction = true;
				pEntry->reSendOther = true;
				pEntry->skillValue.movespeed = 0;

				//xwl add
				if (pEntry->getType()==zSceneEntry::SceneEntry_NPC)
					((SceneNpc *)pEntry)->resetSpeedRate();

			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬289 �����������԰ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_289(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.upstr = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.upstr = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬290 ������������ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_290(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.upallattrib = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.upallattrib = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬292 ��������������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_292(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.nowrelive = 100;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.nowrelive = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬293 ��������ֵ�½���ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_293(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dnallattrib = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dnallattrib = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬294 ��������ֵ�½��ٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_294(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dpallattrib = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dpallattrib = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬295 �������5������ֵ�е�һ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_295(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					switch(zMisc::randBetween(1,5))
					{
						case 1:
							pEntry->skillValue.upattribstr = sse.value;
							break;
						case 2:
							pEntry->skillValue.upattribcon = sse.value;
							break;
						case 3:
							pEntry->skillValue.upattribmen = sse.value;
							break;
						case 4:
							pEntry->skillValue.upattribdex = sse.value;
							break;
						case 5:
							pEntry->skillValue.upattribint = sse.value;
							break;
					}
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.upattribstr = 0;
				pEntry->skillValue.upattribcon = 0;
				pEntry->skillValue.upattribmen = 0;
				pEntry->skillValue.upattribdex = 0;
				pEntry->skillValue.upattribint = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬296 ������ȡ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_296(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->attacklow = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->attacklow = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬298 ������ȡ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_298(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->attacklow = true;
					pEntry->skillValue.enervation = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->attacklow = false;
				pEntry->skillValue.enervation = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬299 ����������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_299(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->ignorePhysicDam = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->ignorePhysicDam = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬300 ħ����������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_300(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->ignoreMagicDam = true;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->ignoreMagicDam = false;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬301 ��ǿħ��(�ٻ��ޱ�����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_301(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) 
				{
					pEntry->summon->boostupSummon = (int)sse.value;
					pEntry->summon->sendData();
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				if (pEntry->summon&& ((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10))) pEntry->summon->boostupSummon = (int)sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon &&((pEntry->summon->id >= (DWORD)sse.percent)&&(pEntry->summon->id < (DWORD)sse.percent+10)))
				{
					pEntry->summon->boostupSummon = 0;
					pEntry->summon->sendData();
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬302 �ٻ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_302(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					sse.qwTime = sse.value*sse.dwTime;
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					pEntry->directDamage(pAtt, sse.value, true);
					pEntry->processDeath(pAtt);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					case zSceneEntry::SceneEntry_NPC:		// NPC
						{
							pAtt = SceneNpcManager::getMe().getNpcByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					pEntry->directDamage(pAtt, sse.qwTime, true);
					pEntry->processDeath(pAtt);
				}

				Cmd::stAttackMagicUserCmd cmd;
				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2U;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
					default:
						{
							return SKILL_RETURN;
						}
						break;
				}
				cmd.dwDefenceTempID = pEntry->tempid;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = 33;
				cmd.byAction = Cmd::Ani_Null;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , 33 , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬303 Ԫ��֮��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_303(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon)
				{
					switch(pEntry->summon->id)
					{
						case 42221 ... 42230://ʯ��
						case 42241 ... 42250://����
						case 45001 ... 45010://����
						case 45041 ... 45050://��ħ����
							{
								if ((pEntry->summon->getHp() == pEntry->summon->getMaxHP())&&(pEntry->summon->boostupHpMaxP==0))
								{
								   pEntry->summon->boostupHpMaxP = (int)sse.value;
								   pEntry->summon->changeHP(pEntry->summon->getMaxHP());
								   pEntry->summon->sendData();
								}
								else
								{
								   pEntry->summon->boostupHpMaxP = (int)sse.value;
								   pEntry->summon->sendData();
								}
								return SKILL_RECOVERY;
							}
							break;
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon)
				{
					switch(pEntry->summon->id)
					{
						case 42221 ... 42230://ʯ��
						case 42241 ... 42250://����
						case 45001 ... 45010://����
						case 45041 ... 45050://��ħ����
							{
								pEntry->summon->boostupHpMaxP = 0;
								pEntry->summon->sendData();
								return SKILL_RETURN;
							}
							break;
					}
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬304 ڤ��֮�ţ��������������ֵ����)
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_304(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon)
				{
					switch(pEntry->summon->id)
					{
						case 42061 ... 42070://����սʿ
						case 42081 ... 42090://�Ļ�
						case 44061 ... 44070://��������
						case 45081 ... 45090://��������
							{
								if ((pEntry->summon->getHp() == pEntry->summon->getMaxHP())&&(pEntry->summon->boostupHpMaxP==0))
								{
								   pEntry->summon->boostupHpMaxP = (int)sse.value;
								   pEntry->summon->changeHP(pEntry->summon->getMaxHP());
								   pEntry->summon->sendData();
								}
								else
								{
								   pEntry->summon->boostupHpMaxP = (int)sse.value;
								   pEntry->summon->sendData();
								}
								return SKILL_RECOVERY;
							}
							break;
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon)
				{
					switch(pEntry->summon->id)
					{
						case 42061 ... 42070://����սʿ
						case 42081 ... 42090://�Ļ�
						case 44061 ... 44070://��������
						case 45081 ... 45090://��������
							{
								pEntry->summon->boostupHpMaxP = 0;
								pEntry->summon->sendData();
								return SKILL_RETURN;
							}
							break;
					}
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬305 ���֮�ż��ٻ��������ޣ�����ϵ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_305(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if (pEntry->summon)
				{
					switch(pEntry->summon->id)
					{
						case 42181 ... 42190://���
						case 42161 ... 42170://�콫
						case 42121 ... 42130://����
						case 46021 ... 46030://��������
							{
								if ((pEntry->summon->getHp() == pEntry->summon->getMaxHP())&&(pEntry->summon->boostupHpMaxP==0))
								{
								   pEntry->summon->boostupHpMaxP = (int)sse.value;
								   pEntry->summon->changeHP(pEntry->summon->getMaxHP());
								   pEntry->summon->sendData();
								}
								else
								{
								   pEntry->summon->boostupHpMaxP = (int)sse.value;
								   pEntry->summon->sendData();
								}
								return SKILL_RECOVERY;
							}
							break;
					}
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon)
				{
					switch(pEntry->summon->id)
					{
						case 42181 ... 42190://���
						case 42161 ... 42170://�콫
						case 42121 ... 42130://����
						case 46021 ... 46030://��������
							{
								pEntry->summon->boostupHpMaxP = 0;
								pEntry->summon->sendData();
								return SKILL_RETURN;
							}
							break;
					}
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬306 �м���ʹ�Ե��˵��˺�����Լ�������ֵ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_306(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				pEntry->damtohpper = sse.percent;
				pEntry->damtohpvalper = sse.value;
				return SKILL_ACTIVE;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->damtohpper = 0;
				pEntry->damtohpvalper = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬307 ���͵��˵������;���ٷֱȣ���ħ����ר�ã�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_307(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dpintmen = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dpintmen = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬308 ���͵��˵����������ݰٷֱȣ���ħ����ר�ã�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_308(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dpstrdex = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dpstrdex = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬309 һ����ɱ
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_309(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->killnow = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->killnow = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬311 ���������ȡͬ���������е�����Ч����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_311(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->switchdie = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->switchdie = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬312 ѪծѪ��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_312(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->diewithme = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->diewithme = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬315 ��ˮ֮��
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_315(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				SceneEntryPk *pAtt = NULL;
				switch(sse.attacktype)
				{
					case zSceneEntry::SceneEntry_Player:	// ��ҽ�ɫ
						{
							pAtt = SceneUserManager::getMe().getUserByTempID(sse.dwTempID);
						}
						break;
					default:
						break;
				}

				if (pAtt)
				{
					Cmd::stAttackMagicUserCmd cmd;

					cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
					cmd.dwDefenceTempID = pEntry->tempid;
					cmd.dwUserTempID = pAtt->tempid;
					cmd.wdMagicType = 176;
					cmd.byAction = Cmd::Ani_Num;
					cmd.byDirect = pEntry->getDir();
					cmd.xDes = (WORD)pEntry->getPos().x;
					cmd.yDes = (WORD)pEntry->getPos().y;

					zSkill *s = NULL;

					s = pAtt->usm.findSkill(176);
					if(s)
					{
						s->action(&cmd, sizeof(cmd));
					}
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬316 ����
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_316(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 60;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					if (pEntry->summon)
					{
						switch(pEntry->summon->npc->id)
						{
							case 45041 ... 45050:
								{
									pEntry->summon->summonsplit = sse.value;
								}
								break;
						}
					}
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				if (pEntry->summon)
				{
					switch(pEntry->summon->npc->id)
					{
						case 45041 ... 45050:
							{
								pEntry->summon->summonsplit = 0;
							}
							break;
					}
				}
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}


/**
 * \brief	״̬317 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_317(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬318 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_318(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬319 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_319(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬320 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_320(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬321 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_321(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬322 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_322(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬323 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_323(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬324 ÿ���ͷ�һ��������һ����Χ�ڵ��Ѿ���������
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_324(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_GOOD;
	sse.byMutexType = 20;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					return SKILL_ACTIVE;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				Cmd::stAttackMagicUserCmd cmd;

				switch (pEntry->getType())
				{
					case zSceneEntry::SceneEntry_Player:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					case zSceneEntry::SceneEntry_NPC:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
					default:
						{
							cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
						}
						break;
				}
		
				cmd.dwDefenceTempID = 0;
				cmd.dwUserTempID = pEntry->tempid;
				cmd.wdMagicType = sse.value;
				cmd.byAction = Cmd::Ani_Num;
				cmd.byDirect = pEntry->getDir();
				cmd.xDes = (WORD)pEntry->getPos().x;
				cmd.yDes = (WORD)pEntry->getPos().y;

				zSkill *s = NULL;

				s = zSkill::createTempSkill(pEntry , sse.value , 1);
				if(s)
				{
					s->action(&cmd, sizeof(cmd));
					SAFE_DELETE(s);
				}
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

/**
 * \brief	״̬325 �����������Ͱٷֱ�
 * \param pEntry ״̬������
 * \param sse ����״̬
 * \return ����״̬����ֵ���μ�ͷ�ļ��еļ���״̬������ֵö��
 */
BYTE SkillStatus_325(SceneEntryPk *pEntry, SkillStatusElement &sse)
{
	sse.byGoodnessType = SKILL_BAD;
	sse.byMutexType = 41;
	pEntry->reSendData = true;

	switch(sse.byStep)
	{
		case ACTION_STEP_DOPASS:
		case ACTION_STEP_START:
		case ACTION_STEP_RELOAD:
			{
				if(zMisc::selectByPercent((int)(sse.percent)))
				{
					pEntry->skillValue.dmdefp = sse.value;
					return SKILL_RECOVERY;
				}
				else
					return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_TIMER:
			{
				pEntry->skillValue.dmdefp = sse.value;
				return SKILL_RETURN;
			}
			break;
		case ACTION_STEP_STOP:
		case ACTION_STEP_CLEAR:
			{
				pEntry->skillValue.dmdefp = 0;
			}
			break;
		default:
			break;
	}

	return SKILL_RETURN;
}

//--------------------------------------------------

/**
 * \brief ����״̬���������캯��
 * \author fqnewman
 */
SkillStatusManager::SkillStatusManager()
{
	entry = NULL;
	bclearActiveSkillStatus = false;
}


/**
 * \brief ����״̬��������������
 * \author fqnewman
 */
SkillStatusManager::~SkillStatusManager()
{
	//if (entry->getType() == zSceneEntry::SceneEntry_Player)  saveSkillStatus();
}

/**
 * \brief ����״̬��������������
 * \param pEntry ���뼼�ܹ�������ʹ�ö���
 * \author fqnewman
 */
void SkillStatusManager::initMe(SceneEntryPk *pEntry)
{
	entry = pEntry;
	funlist[0].func = SkillStatus_0;
	funlist[1].func = SkillStatus_1;
	funlist[2].func = SkillStatus_2;
	funlist[3].func = SkillStatus_3;
	funlist[4].func = SkillStatus_4;
	funlist[5].func = SkillStatus_5;
	funlist[6].func = SkillStatus_6;
	funlist[7].func = SkillStatus_7;
	funlist[8].func = SkillStatus_8;
	funlist[9].func = SkillStatus_9;
	funlist[10].func = SkillStatus_10;
	funlist[11].func = SkillStatus_11;
	funlist[12].func = SkillStatus_12;
	funlist[13].func = SkillStatus_13;
	funlist[14].func = SkillStatus_14;
	funlist[15].func = SkillStatus_15;
	funlist[16].func = SkillStatus_16;
	funlist[17].func = SkillStatus_17;
	funlist[18].func = SkillStatus_18;
	funlist[19].func = SkillStatus_19;
	funlist[20].func = SkillStatus_20;
	funlist[21].func = SkillStatus_21;
	funlist[22].func = SkillStatus_22;
	funlist[23].func = SkillStatus_23;
	funlist[24].func = SkillStatus_24;
	funlist[25].func = SkillStatus_25;
	funlist[26].func = SkillStatus_26;
	funlist[27].func = SkillStatus_27;
	funlist[28].func = SkillStatus_28;
	funlist[29].func = SkillStatus_29;
	funlist[30].func = SkillStatus_30;
	funlist[31].func = SkillStatus_31;
	funlist[32].func = SkillStatus_32;
	funlist[33].func = SkillStatus_33;
	funlist[34].func = SkillStatus_34;
	funlist[35].func = SkillStatus_35;
	funlist[36].func = SkillStatus_36;
	funlist[37].func = SkillStatus_37;
	funlist[38].func = SkillStatus_38;
	funlist[39].func = SkillStatus_39;
	funlist[40].func = SkillStatus_40;
	funlist[41].func = SkillStatus_41;
	funlist[42].func = SkillStatus_42;
	funlist[43].func = SkillStatus_43;
	funlist[44].func = SkillStatus_44;
	funlist[45].func = SkillStatus_45;
	funlist[46].func = SkillStatus_46;
	funlist[47].func = SkillStatus_47;
	funlist[48].func = SkillStatus_48;
	funlist[49].func = SkillStatus_49;
	funlist[50].func = SkillStatus_50;
	funlist[51].func = SkillStatus_51;
	funlist[52].func = SkillStatus_52;
	funlist[53].func = SkillStatus_53;
	funlist[54].func = SkillStatus_54;
	funlist[55].func = SkillStatus_55;
	funlist[56].func = SkillStatus_56;
	funlist[57].func = SkillStatus_57;
	funlist[58].func = SkillStatus_58;
	funlist[59].func = SkillStatus_59;
	funlist[60].func = SkillStatus_60;
	funlist[61].func = SkillStatus_61;
	funlist[62].func = SkillStatus_62;
	funlist[63].func = SkillStatus_63;
	funlist[64].func = SkillStatus_64;
	funlist[65].func = SkillStatus_65;
	funlist[66].func = SkillStatus_66;
	funlist[67].func = SkillStatus_67;
	funlist[68].func = SkillStatus_68;
	funlist[69].func = SkillStatus_69;
	funlist[70].func = SkillStatus_70;
	funlist[71].func = SkillStatus_71;
	funlist[72].func = SkillStatus_72;
	funlist[73].func = SkillStatus_73;
	funlist[74].func = SkillStatus_74;
	funlist[75].func = SkillStatus_75;
	funlist[76].func = SkillStatus_76;
	funlist[77].func = SkillStatus_77;
	funlist[78].func = SkillStatus_78;
	funlist[79].func = SkillStatus_79;
	funlist[80].func = SkillStatus_80;
	funlist[81].func = SkillStatus_81;
	funlist[82].func = SkillStatus_82;
	funlist[83].func = SkillStatus_83;
	funlist[84].func = SkillStatus_84;
	funlist[85].func = SkillStatus_85;
	funlist[86].func = SkillStatus_86;
	funlist[87].func = SkillStatus_87;
	funlist[88].func = SkillStatus_88;
	funlist[89].func = SkillStatus_89;
	funlist[90].func = SkillStatus_90;
	funlist[91].func = SkillStatus_91;
	funlist[92].func = SkillStatus_92;
	funlist[93].func = SkillStatus_93;
	funlist[94].func = SkillStatus_94;
	funlist[95].func = SkillStatus_95;
	funlist[96].func = SkillStatus_96;
	funlist[97].func = SkillStatus_97;
	funlist[98].func = SkillStatus_98;
	funlist[99].func = SkillStatus_99;
	funlist[100].func = SkillStatus_100;
	funlist[101].func = SkillStatus_101;
	funlist[102].func = SkillStatus_102;
	funlist[103].func = SkillStatus_101;
	funlist[104].func = SkillStatus_101;
	funlist[105].func = SkillStatus_101;
	funlist[106].func = SkillStatus_101;
	funlist[107].func = SkillStatus_101;
	funlist[108].func = SkillStatus_101;
	funlist[109].func = SkillStatus_101;
	funlist[110].func = SkillStatus_110;
	funlist[111].func = SkillStatus_111;
	funlist[112].func = SkillStatus_101;
	funlist[113].func = SkillStatus_113;
	funlist[114].func = SkillStatus_114;
	funlist[115].func = SkillStatus_115;
	funlist[116].func = SkillStatus_116;
	funlist[117].func = SkillStatus_117;
	funlist[118].func = SkillStatus_101;
	funlist[119].func = SkillStatus_119;
	funlist[120].func = SkillStatus_120;
	funlist[121].func = SkillStatus_121;
	funlist[122].func = SkillStatus_122;
	funlist[123].func = SkillStatus_123;
	funlist[124].func = SkillStatus_124;
	funlist[125].func = SkillStatus_125;
	funlist[126].func = SkillStatus_126;
	funlist[127].func = SkillStatus_127;
	funlist[128].func = SkillStatus_128;
	funlist[129].func = SkillStatus_129;
	funlist[130].func = SkillStatus_130;
	funlist[131].func = SkillStatus_131;
	funlist[132].func = SkillStatus_132;
	funlist[133].func = SkillStatus_133;
	funlist[134].func = SkillStatus_134;
	funlist[135].func = SkillStatus_135;
	funlist[136].func = SkillStatus_136;
	funlist[137].func = SkillStatus_137;
	funlist[138].func = SkillStatus_138;
	funlist[139].func = SkillStatus_139;
	funlist[140].func = SkillStatus_140;
	funlist[141].func = SkillStatus_141;
	funlist[142].func = SkillStatus_142;
	funlist[143].func = SkillStatus_143;
	funlist[144].func = SkillStatus_144;
	funlist[145].func = SkillStatus_145;
	funlist[146].func = SkillStatus_146;
	funlist[147].func = SkillStatus_147;
	funlist[148].func = SkillStatus_148;
	funlist[149].func = SkillStatus_149;
	funlist[150].func = SkillStatus_150;
	funlist[151].func = SkillStatus_151;
	funlist[152].func = SkillStatus_152;
	funlist[153].func = SkillStatus_153;
	funlist[154].func = SkillStatus_154;
	funlist[155].func = SkillStatus_155;
	funlist[156].func = SkillStatus_156;
	funlist[157].func = SkillStatus_157;
	funlist[158].func = SkillStatus_158;
	funlist[159].func = SkillStatus_159;
	funlist[160].func = SkillStatus_160;
	funlist[161].func = SkillStatus_161;
	funlist[162].func = SkillStatus_0;
	funlist[163].func = SkillStatus_163;
	funlist[164].func = SkillStatus_0;
	funlist[165].func = SkillStatus_165;
	funlist[166].func = SkillStatus_0;
	funlist[167].func = SkillStatus_0;
	funlist[168].func = SkillStatus_168;
	funlist[169].func = SkillStatus_169;
	funlist[170].func = SkillStatus_170;
	funlist[171].func = SkillStatus_0;
	funlist[172].func = SkillStatus_172;
	funlist[173].func = SkillStatus_173;
	funlist[174].func = SkillStatus_0;
	funlist[175].func = SkillStatus_0;
	funlist[176].func = SkillStatus_176;
	funlist[177].func = SkillStatus_177;
	funlist[178].func = SkillStatus_178;
	funlist[179].func = SkillStatus_179;
	funlist[180].func = SkillStatus_180;
	funlist[181].func = SkillStatus_0;
	funlist[182].func = SkillStatus_182;
	funlist[183].func = SkillStatus_0;
	funlist[184].func = SkillStatus_184;
	funlist[185].func = SkillStatus_185;
	funlist[186].func = SkillStatus_186;
	funlist[187].func = SkillStatus_187;
	funlist[188].func = SkillStatus_188;
	funlist[189].func = SkillStatus_189;
	funlist[190].func = SkillStatus_190;
	funlist[191].func = SkillStatus_191;
	funlist[192].func = SkillStatus_192;
	funlist[193].func = SkillStatus_193;
	funlist[194].func = SkillStatus_194;
	funlist[195].func = SkillStatus_195;
	funlist[196].func = SkillStatus_196;
	funlist[197].func = SkillStatus_197;
	funlist[198].func = SkillStatus_198;
	funlist[199].func = SkillStatus_199;
	funlist[200].func = SkillStatus_200;
	funlist[201].func = SkillStatus_201;
	funlist[202].func = SkillStatus_202;
	funlist[203].func = SkillStatus_203;
	funlist[204].func = SkillStatus_204;
	funlist[205].func = SkillStatus_205;
	funlist[206].func = SkillStatus_206;
	funlist[207].func = SkillStatus_207;
	funlist[208].func = SkillStatus_208;
	funlist[209].func = SkillStatus_209;
	funlist[210].func = SkillStatus_210;
	funlist[211].func = SkillStatus_0;
	funlist[212].func = SkillStatus_0;
	funlist[213].func = SkillStatus_0;
	funlist[214].func = SkillStatus_0;
	funlist[215].func = SkillStatus_0;
	funlist[216].func = SkillStatus_216;
	funlist[217].func = SkillStatus_217;
	funlist[218].func = SkillStatus_218;
	funlist[219].func = SkillStatus_219;
	funlist[220].func = SkillStatus_220;
	funlist[221].func = SkillStatus_221;
	funlist[222].func = SkillStatus_222;
	funlist[223].func = SkillStatus_223;
	funlist[224].func = SkillStatus_224;
	funlist[225].func = SkillStatus_225;
	funlist[226].func = SkillStatus_226;
	funlist[227].func = SkillStatus_227;
	funlist[228].func = SkillStatus_228;
	funlist[229].func = SkillStatus_229;
	funlist[230].func = SkillStatus_230;
	funlist[231].func = SkillStatus_231;
	funlist[232].func = SkillStatus_232;
	funlist[233].func = SkillStatus_233;
	funlist[234].func = SkillStatus_234;
	funlist[235].func = SkillStatus_235;
	funlist[236].func = SkillStatus_236;
	funlist[237].func = SkillStatus_237;
	funlist[238].func = SkillStatus_238;
	funlist[239].func = SkillStatus_239;
	funlist[240].func = SkillStatus_240;
	funlist[241].func = SkillStatus_241;
	funlist[242].func = SkillStatus_242;
	funlist[243].func = SkillStatus_243;
	funlist[244].func = SkillStatus_244;
	funlist[245].func = SkillStatus_245;
	funlist[246].func = SkillStatus_246;
	funlist[247].func = SkillStatus_247;
	funlist[248].func = SkillStatus_248;
	funlist[249].func = SkillStatus_249;
	funlist[250].func = SkillStatus_250;
	funlist[251].func = SkillStatus_251;
	funlist[252].func = SkillStatus_252;
	funlist[253].func = SkillStatus_253;
	funlist[254].func = SkillStatus_254;
	funlist[255].func = SkillStatus_255;
	funlist[256].func = SkillStatus_256;
	funlist[257].func = SkillStatus_257;
	funlist[258].func = SkillStatus_258;
	funlist[259].func = SkillStatus_259;
	funlist[260].func = SkillStatus_260;
	funlist[261].func = SkillStatus_261;
	funlist[262].func = SkillStatus_262;
	funlist[263].func = SkillStatus_263;
	funlist[264].func = SkillStatus_264;
	funlist[265].func = SkillStatus_265;
	funlist[266].func = SkillStatus_266;
	funlist[267].func = SkillStatus_0;
	funlist[268].func = SkillStatus_268;
	funlist[269].func = SkillStatus_0;
	funlist[270].func = SkillStatus_270;
	funlist[271].func = SkillStatus_271;
	funlist[272].func = SkillStatus_272;
	funlist[273].func = SkillStatus_273;
	funlist[274].func = SkillStatus_274;
	funlist[275].func = SkillStatus_275;
	funlist[276].func = SkillStatus_276;
	funlist[277].func = SkillStatus_277;
	funlist[278].func = SkillStatus_278;
	funlist[279].func = SkillStatus_279;
	funlist[280].func = SkillStatus_280;
	funlist[281].func = SkillStatus_281;
	funlist[282].func = SkillStatus_282;
	funlist[283].func = SkillStatus_283;
	funlist[284].func = SkillStatus_284;
	funlist[285].func = SkillStatus_285;
	funlist[286].func = SkillStatus_286;
	funlist[287].func = SkillStatus_287;
	funlist[288].func = SkillStatus_288;
	funlist[289].func = SkillStatus_289;
	funlist[290].func = SkillStatus_290;
	funlist[291].func = SkillStatus_0;
	funlist[292].func = SkillStatus_292;
	funlist[293].func = SkillStatus_293;
	funlist[294].func = SkillStatus_294;
	funlist[295].func = SkillStatus_295;
	funlist[296].func = SkillStatus_296;
	funlist[297].func = SkillStatus_0;
	funlist[298].func = SkillStatus_298;
	funlist[299].func = SkillStatus_299;
	funlist[300].func = SkillStatus_300;
	funlist[301].func = SkillStatus_301;
	funlist[302].func = SkillStatus_302;
	funlist[303].func = SkillStatus_303;
	funlist[304].func = SkillStatus_304;
	funlist[305].func = SkillStatus_305;
	funlist[306].func = SkillStatus_306;
	funlist[307].func = SkillStatus_307;
	funlist[308].func = SkillStatus_308;
	funlist[309].func = SkillStatus_309;
	funlist[310].func = SkillStatus_0;
	funlist[311].func = SkillStatus_0;
	funlist[312].func = SkillStatus_312;
	funlist[313].func = SkillStatus_0;
	funlist[314].func = SkillStatus_0;
	funlist[315].func = SkillStatus_315;
	funlist[316].func = SkillStatus_316;
	funlist[317].func = SkillStatus_0;
	funlist[318].func = SkillStatus_0;
	funlist[319].func = SkillStatus_0;
	funlist[320].func = SkillStatus_0;
	funlist[316].func = SkillStatus_0;
	funlist[317].func = SkillStatus_317;
	funlist[318].func = SkillStatus_318;
	funlist[319].func = SkillStatus_319;
	funlist[320].func = SkillStatus_320;
	funlist[321].func = SkillStatus_321;
	funlist[322].func = SkillStatus_322;
	funlist[323].func = SkillStatus_323;
	funlist[324].func = SkillStatus_324;
	funlist[325].func = SkillStatus_325;





	//if (entry->getType() == zSceneEntry::SceneEntry_Player) loadSkillStatus();
}

/**
 * \brief ֪ͨĳ��״̬��ֵ
 * \author ������
 */
void SkillStatusManager::sendSelectStates(SceneEntryPk *pThis , DWORD state , WORD value , WORD time)
{
	pThis->sendtoSelectedState(state , value , time);
	pThis->sendSevenStateToMe(state , value , time);
}

/**
 * \brief �õ���ǰ7��״̬����ֵ
 * \author ������
 */
void SkillStatusManager::getSelectStates(Cmd::stSelectReturnStatesPropertyUserCmd *buf , unsigned long maxSize)
{

	using namespace Cmd;
	std::map<DWORD, SkillStatusElement>::iterator tIterator;
	zRTime ctv;
	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end(); tIterator++)
	{
		//Zebra::logger->debug("Ŀǰ״̬%u , %u , %u" , tIterator->first , tIterator->second.id , tIterator->second.state);
/*		switch(tIterator->second.state)
		{
			case USTATE_POISON:
			case USTATE_FREEZE:
			case USTATE_CONFUSION:
			case USTATE_BLED:
			case USTATE_PALSY:
			case USTATE_BLIND:
			case USTATE_ROCK:*/
		if (tIterator->second.state >0 && 
				(sizeof(stSelectReturnStatesPropertyUserCmd) + sizeof(buf->states[0]) * buf->size)<= maxSize)
		{
			buf->states[buf->size].state = tIterator->second.state;
			buf->states[buf->size].result = tIterator->second.value;
			buf->states[buf->size].time = tIterator->second.dwTime;
			buf->size ++;
		}
/*				break;
			default:
				break;
		}*/
	}

	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end(); tIterator++)
	{

		if (tIterator->second.state >0 &&
				(sizeof(stSelectReturnStatesPropertyUserCmd) + sizeof(buf->states[0]) * buf->size)<= maxSize)
		{
			buf->states[buf->size].state = tIterator->second.state;
			buf->states[buf->size].result = tIterator->second.value;
			buf->states[buf->size].time = (tIterator->second.qwTime- ctv.msecs())/1000;
			buf->size ++;
		}
	}

}

/**
 * \brief ������ʷ�ļ���״̬���ϴ����ߵ�ʱ���ܻ����������ϵļ���״̬������Ч
 * \author fqnewman
 */
void SkillStatusManager::loadSkillStatus(char *buf, DWORD length)
{
	SkillStatusElement *value;
//	char buf[zSocket::MAX_DATASIZE];
	unsigned int count;
	DWORD statelen;
	bool notify;
	/*DWORD length;

	count = 0; 
	length=0;
	bzero(buf,zSocket::MAX_DATASIZE);*/

	SkillState *pState = (SkillState *)buf;
	statelen = sizeof(SkillState);
	memcpy(&(entry->skillValue), pState, statelen);
	length-=statelen;

#ifdef _DEBUGLOG
	Zebra::logger->debug("[Ϊ��ɫ(%s)(%d)���ر���ļ���״̬]", entry->name,entry->id);
#endif
	//COfflineSkillStatus::getOfflineSkillStatus(entry->id,buf,length);
	count = length/sizeof(SkillStatusElement);
	notify = false;
#ifdef _DEBUGLOG
	Zebra::logger->debug("[��%d������״̬��Ҫ����]", count);
#endif
	value = (SkillStatusElement *)(buf+statelen);

	SceneEntryPk *pAtt = NULL;
	if (value->attacktype == zSceneEntry::SceneEntry_Player)
	{
		pAtt = SceneUserManager::getMe().getUserByID(value->dwAttackerID);
		if (pAtt) value->dwTempID = pAtt->tempid;
	}

	for(unsigned int i=0; i<count; i++)
	{
		value->byStep = ACTION_STEP_RELOAD;
		switch(runStatusElement(*value))
		{
			case SKILL_RECOVERY:
				{
#ifdef _DEBUGLOG
					Zebra::logger->debug("[%d�ż���״̬�����ص���ʱ��������]", value->id);
#endif
					//value->refresh = 1;
					//value->qwTime = SceneTimeTick::currentTime.msecs()+value->dwTime *1000;
					_recoveryElement[value->id/*value->byMutexType*/]=*value;
					_recoveryElement[value->id].qwTime = SceneTimeTick::currentTime.msecs()+_recoveryElement[value->id].dwTime *1000;
					if (value->state >0)
					{
						sendSelectStates(entry, value->state, value->value, value->dwTime);
						//entry->setEffectStatus(value->state);
						entry->showCurrentEffect(value->state, true);
					}
					notify = true;
				}
				break;
			case SKILL_ACTIVE:
				{
#ifdef _DEBUGLOG
					Zebra::logger->debug("[%d�ż���״̬�����ص���������]", value->id);
#endif
					_activeElement[value->id/*value->byMutexType*/]=*value;
					_recoveryElement[value->id].qwTime = SceneTimeTick::currentTime.msecs()+_recoveryElement[value->id].dwTime *1000;
					if (value->state >0)
					{
						sendSelectStates(entry, value->state, value->value, value->dwTime);
						//entry->setEffectStatus(value->state);
						entry->showCurrentEffect(value->state, true);
					}
					notify = true;
				}
				break;
			default:
#ifdef _DEBUGLOG
					Zebra::logger->debug("[%d�ż���״̬�޷����ӵ���Ӧ�ı���]", value->id);
#endif
				break;
		}
		value++;
	}

	if (notify&&entry)
	{
		entry->changeAndRefreshHMS(true, true);
	}
}

/**
 * \brief �洢��ʷ�ļ���״̬���ϴ����ߵ�ʱ���ܻ����������ϵļ���״̬������Ч
 * \author fqnewman
 */
void SkillStatusManager::saveSkillStatus(char *buf , DWORD &size)
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator;
	SkillStatusElement *value;
	unsigned int count;
	DWORD statelen;
	DWORD length;

	length = 0;
	count = 0;
	statelen = 0;

	SkillState *pState = (SkillState *)buf;
	statelen = sizeof(SkillState);
	memcpy(pState, &(entry->skillValue), statelen);

	value = (SkillStatusElement *)(buf+statelen);
	length = sizeof(SkillStatusElement);
	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end();tIterator ++)
	{
		memcpy(value,&tIterator->second, length);
#ifdef _DEBUGLOG
		Zebra::logger->debug("[����״̬%d���洢]", tIterator->second.id);
#endif
		count++;
		value++;
		if (count>3000) break;
	}

	if (count<3000)
	{
		for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end();tIterator ++)
		{
			memcpy(value,&tIterator->second, length);
#ifdef _DEBUGLOG
		Zebra::logger->debug("[����״̬%d���洢]", tIterator->second.id);
#endif
			count++;
			value++;
			if (count>3000) break;
		}
	}
	//if (count >0) COfflineSkillStatus::writeOfflineSkillStatus(entry->id,buf,count*length);
	size = statelen + count*length;
#ifdef _DEBUGLOG
	Zebra::logger->debug("[��%d������״̬��Ҫ�洢]", count);
#endif
}

/**
 * \brief ��һ�����ܲ���ʩ�����ҵ�����
 * \param carrier ���ܲ���Ͷ��������������˼���״̬
 * \param good ��־��������ǲ���һ������ļ���
 * \author fqnewman
 * \return true Ϊ������һ��������falseΪֹͣ�����ء�
 */
bool SkillStatusManager::putOperationToMe(const SkillStatusCarrier &carrier, const bool good, SWORD rangDamageBonus)
{
	std::vector<SkillElement>::const_iterator tIterator;
	bool sendData = false;

	entry->isPhysics = false;
	if (!entry->preAttackMe(carrier.attacker, &carrier.revCmd, false, good)) return true;

	for (tIterator = carrier.status->_StatusElementList.begin();
		tIterator != carrier.status->_StatusElementList.end();
		tIterator++)
	{
		SkillStatusElement element;
		element.byStep	=	ACTION_STEP_START;
		element.id		=	tIterator->id;
		element.percent =	tIterator->percent;
		element.value	=	tIterator->value;
		element.state	=	tIterator->state;

		element.dwSkillID = carrier.skillbase->skillid;//carrier.skillID;
		element.dwTime = tIterator->time;
		element.qwTime = SceneTimeTick::currentTime.msecs()+element.dwTime *1000;

		SceneEntryPk *pAtt = carrier.attacker->getTopMaster();
		element.dwTempID = pAtt->tempid;
		element.dwAttackerID = pAtt->id;
		element.attacktype = pAtt->getType();
		if (element.dwTime <=2)
		{
			element.refresh = 0;
		}
		else
		{
			element.refresh = 1;
			sendData = true;
		}

		entry->curMagicManType = element.attacktype;
		entry->curMagicManID = element.dwTempID;
		entry->keepPos.x = (DWORD)carrier.revCmd.xDes;
		entry->keepPos.y = (DWORD)carrier.revCmd.yDes;
		entry->keepDir = carrier.revCmd.byDirect;

		switch(runStatusElement(element))
		{
			case SKILL_RECOVERY:
				{
#ifdef _DEBUGLOG
					Zebra::logger->debug("[��ʱ����]��[%u]��״̬��ʩ�������ϳ���ʱ��Ϊ[%u]", element.id,element.dwTime);
					Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS, "[��ʱ����]��[%u]��״̬��ʩ�������ϳ���ʱ��Ϊ[%u]", element.id,element.dwTime);
#endif
					element.qwTime = SceneTimeTick::currentTime.msecs()+element.dwTime *1000;
					if (element.state >0)
					{
						clearMapElement(element.id/*element.byMutexType*/,_recoveryElement, element.id,false);
						_recoveryElement[element.id/*element.byMutexType*/]=element;
						sendSelectStates(entry, element.state, element.value, element.dwTime);
						//entry->setEffectStatus(element.state);
						entry->showCurrentEffect(element.state, true);
					}
					else
					{
						clearMapElement(element.id/*element.byMutexType*/,_recoveryElement, element.id);
						_recoveryElement[element.id/*element.byMutexType*/]=element;
					}
				}
				break;
			case SKILL_BREAK:
				{
					return false;
				}
				break;
			case SKILL_ACTIVE:
				{
#ifdef _DEBUGLOG
					Zebra::logger->debug("[�˺�״̬]��[%u]��״̬��ʩ�������ϳ���ʱ��Ϊ[%u]",element.id,element.dwTime);
					Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS, "[����״̬]��[%u]��״̬��ʩ�������ϳ���ʱ��Ϊ[%u]",element.id,element.dwTime);
#endif
					if (element.dwTime==0) break;
					clearMapElement(element.id/*element.byMutexType*/,_activeElement, element.id);
					_activeElement[element.id/*element.byMutexType*/]=element;
					if (element.state >0)
					{
						sendSelectStates(entry, element.state, element.value, element.dwTime);
						//entry->setEffectStatus(element.state);
						entry->showCurrentEffect(element.state, true);
					}
				}
				break;
			case SKILL_RETURN:
			default:
				break;
		}
	}

	if (carrier.status->isInjure == 1)
	{
		entry->AttackMe(carrier.attacker, &carrier.revCmd, entry->isPhysics, rangDamageBonus);
		carrier.attacker->setPetsChaseTarget(entry);
#ifdef _DEBUGLOG
		Zebra::logger->debug("[�߹������㹫ʽ]");
		Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS,"[�˺�����]");
#endif
	}
	else
	{
		if (entry->notifyHMS)
		{
			entry->attackRTHpAndMp();
		}
		SceneEntryPk *pAtt = carrier.attacker->getTopMaster();
		if (pAtt&&pAtt->isEnemy(entry)&&carrier.status->range!=22)
		{
			SceneEntryPk * aMaster = pAtt->getTopMaster();
			SceneEntryPk * pMaster = entry->getTopMaster();
			if (aMaster->getType()==zSceneEntry::SceneEntry_Player)
			{
				SceneUser * a = (SceneUser *)aMaster;
				if (pMaster->getType()==zSceneEntry::SceneEntry_Player)
				{
					SceneUser * p = (SceneUser *)pMaster;
					ScenePk::checkProtect(a, p);
				}
			}
		}
		//entry->processDeath(carrier.attacker);
#ifdef _DEBUGLOG
		Zebra::logger->debug("[���߹������㹫ʽ]");
		Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS,"[����Ѫ�Է���]");
#endif
	}
	entry->isPhysics = false;
	if (entry->reSendData) entry->changeAndRefreshHMS(true, sendData);
	return true;
}

/**
 * \brief ��ʱ��ˢ��
 * \author fqnewman
 */
void SkillStatusManager::timer()
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator, delIterator;
	bool dirty=false;
	bool sendData =false;

	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end() ; )
	{
#ifdef _DEBUGLOG
		Zebra::logger->debug("[��ʱ.��]ʩ�������ϵĵ�[%u]��״̬ʣ��ʱ��[%u]",tIterator->second.id,tIterator->second.dwTime);
		Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS,"[��ʱ.��]ʩ�������ϵĵ�[%u]��״̬ʣ��ʱ��[%u]",tIterator->second.id, tIterator->second.dwTime);
#endif
		if (tIterator->second.dwTime>0)
		{
			tIterator->second.dwTime --;
		}
		else 
		{
			tIterator->second.dwTime = 0;
		}

		tIterator->second.byStep = ACTION_STEP_TIMER;
		runStatusElement(tIterator->second);

		if(0 == tIterator->second.dwTime)
		{
			tIterator->second.byStep = ACTION_STEP_STOP;
			runStatusElement(tIterator->second);
			if (tIterator->second.refresh) sendData = true;
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
#ifdef _DEBUGLOG
		Zebra::logger->debug("[�˺�״̬]ʩ�������ϵĵ�[%u]��״̬��ɾ��",tIterator->second.id);
		Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS,"[�˺�״̬]ʩ�������ϵĵ�[%u]��״̬��ɾ��",tIterator->second.id);
#endif
			delIterator = tIterator;
			tIterator ++;
			_activeElement.erase(delIterator->first);
			dirty = true;
		}
		else
		{
			tIterator ++;
		}
	}

	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end();)
	{
#ifdef _DEBUGLOG
		Zebra::logger->debug("[��ʱ.��]ʩ�������ϵĵ�[%u]��״̬ʣ��ʱ��Ϊ[%u]",tIterator->second.id,tIterator->second.dwTime);
		time_t tempValue = (time_t)(tIterator->second.qwTime/1000);
		Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS,"[��ʱ.��]ʩ�������ϵĵ�[%u]��״̬ʣ��ʱ��Ϊ[%s]",tIterator->second.id,ctime(&tempValue));
#endif
		QWORD curQtime = SceneTimeTick::currentTime.msecs();
		if(curQtime >= tIterator->second.qwTime)
		{
			tIterator->second.byStep = ACTION_STEP_STOP;
			runStatusElement(tIterator->second);
			if (tIterator->second.refresh) sendData = true;
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
#ifdef _DEBUGLOG
			Zebra::logger->debug("[��ʱ����]ʩ�������ϵĵ�[%u]��״̬��ɾ��",tIterator->second.id);
			Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS,"[��ʱ����]ʩ�������ϵĵ�[%u]��״̬��ɾ��",tIterator->second.id);
#endif
			delIterator = tIterator;
			tIterator ++;
			_recoveryElement.erase(delIterator->first);
			dirty = true;
		}
		else
		{
			tIterator->second.dwTime = (tIterator->second.qwTime - curQtime)/1000;
			tIterator ++;
		}
	}
	if (entry->notifyHMS)
	{
		entry->attackRTHpAndMp();
	}
	if (dirty||entry->reSendData)
	{
		entry->changeAndRefreshHMS(false, sendData);
	}
	//std::map<DWORD, SkillStatusElement>::iterator tIterator, delIterator;
	if (bclearActiveSkillStatus)
	{
		for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end() ; )
		{
			tIterator->second.byStep = ACTION_STEP_CLEAR;
			runStatusElement(tIterator->second);
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
			delIterator = tIterator;
			tIterator ++;
			_activeElement.erase(delIterator->first);
	
		}

		for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end();)
		{
			tIterator->second.byStep = ACTION_STEP_CLEAR;
			runStatusElement(tIterator->second);
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
			delIterator = tIterator;
			tIterator ++;
			_recoveryElement.erase(delIterator->first);
		}
		bclearActiveSkillStatus = false;
	}
}

/**
 * \brief �������б���״̬���������ñ�������ʱ����
 * \author fqnewman
 */
void SkillStatusManager::processPassiveness()
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator;

	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end() ; tIterator ++)
	{
		tIterator->second.byStep = ACTION_STEP_DOPASS;
		if (tIterator->second.percent<100) tIterator->second.percent=100;
		runStatusElement(tIterator->second);
	}

	for(tIterator = _passivenessElement.begin() ; tIterator !=_passivenessElement.end() ; tIterator ++)
	{
		tIterator->second.byStep = ACTION_STEP_DOPASS;
		if (tIterator->second.percent<100&&tIterator->second.id!=180) tIterator->second.percent=100;
		runStatusElement(tIterator->second);
	}
}

/**
 * \brief ִ��һ�������״̬
 * \author ��������fqnewman
 * \return ״̬����ֵ
			SKILL_ACTIVE		//	�ӵ��MAP��
			SKILL_RECOVERY		//	�ӵ���ʱ����MAP��
			SKILL_PASSIVENESS	//	�ӵ����ñ���MAP��
			SKILL_RETURN		//	���ز����κβ���
			SKILL_DONOW			//	����ִ������ֵ�۳�����
 */
BYTE SkillStatusManager::runStatusElement(SkillStatusElement &element)
{
	return  funlist[element.id>SKILLSTATENUMBER?0:element.id].func(entry,element);
}

/**
 * \brief  ����������ϵĲ����ķ�����״̬
 * \author fqnewman
 */
void SkillStatusManager::clearBadActiveSkillStatus()
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator, delIterator;

	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end() ; )
	{
		if (tIterator->second.byGoodnessType == SKILL_BAD)
		{
			tIterator->second.byStep = ACTION_STEP_CLEAR;
			runStatusElement(tIterator->second);
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
			delIterator = tIterator;
			tIterator ++;
			_activeElement.erase(delIterator->first);
		}
		else
		{
			tIterator ++;
		}
	}

	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end();)
	{
		if (tIterator->second.byGoodnessType == SKILL_BAD)
		{
			tIterator->second.byStep = ACTION_STEP_CLEAR;
			runStatusElement(tIterator->second);
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
			delIterator = tIterator;
			tIterator ++;
			_recoveryElement.erase(delIterator->first);
		}
		else
		{
			tIterator ++;
		}
	}
}

/**
 * \brief  ����������ϵķ�������״̬
 * \author fqnewman
 */
void SkillStatusManager::clearActiveSkillStatus()
{
	bclearActiveSkillStatus = true;
}

/**
 * \brief  ����������ϵķ�������״̬
 * \author fqnewman
 */
void SkillStatusManager::clearActiveSkillStatusOnlyUseToStatus48()
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator, delIterator;

	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end() ; )
	{
		tIterator->second.byStep = ACTION_STEP_CLEAR;
		runStatusElement(tIterator->second);
		if (tIterator->second.state >0)
		{
			//entry->clearEffectStatus(tIterator->second.state);
			entry->showCurrentEffect(tIterator->second.state, false);
		}
		delIterator = tIterator;
		tIterator ++;
		_activeElement.erase(delIterator->first);

	}

	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end();)
	{
		if (tIterator->second.id == 173)
		{ 
			tIterator++;
			continue; //������˼���⴦��һ��
		}
		tIterator->second.byStep = ACTION_STEP_CLEAR;
		runStatusElement(tIterator->second);
		if (tIterator->second.state >0)
		{
			//entry->clearEffectStatus(tIterator->second.state);
			entry->showCurrentEffect(tIterator->second.state, false);
		}
		delIterator = tIterator;
		tIterator ++;
		_recoveryElement.erase(delIterator->first);
	}
	this->entry->changeAndRefreshHMS();
}


/**
 * \brief  ���ָ������ID�ļ���״̬
 * \author fqnewman
 */
void SkillStatusManager::clearSkill(DWORD dwSkillID)
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator, delIterator;

	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end() ; )
	{
		if (tIterator->second.dwSkillID == dwSkillID)
		{
			tIterator->second.byStep = ACTION_STEP_CLEAR;
			runStatusElement(tIterator->second);
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
			delIterator = tIterator;
			tIterator ++;
			_activeElement.erase(delIterator->first);
		}
		else
		{
			tIterator++;
		}

	}

	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end();)
	{
		if (tIterator->second.dwSkillID == dwSkillID)
		{
			tIterator->second.byStep = ACTION_STEP_CLEAR;
			runStatusElement(tIterator->second);
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
			delIterator = tIterator;
			tIterator ++;
			_recoveryElement.erase(delIterator->first);
		}
		else
		{
			tIterator++;
		}
	}
	this->entry->changeAndRefreshHMS();
}


/**
 * \brief �����������������������
 * \param skillid ����id
 * \param pSkillStatus �����еĲ���
 * \author fqnewman
 */
void SkillStatusManager::putPassivenessOperationToMe(const DWORD skillid, const SkillStatus *pSkillStatus)
{
	std::vector<SkillElement>::const_iterator tIterator;

	for (tIterator = pSkillStatus->_StatusElementList.begin();
		tIterator != pSkillStatus->_StatusElementList.end();
		tIterator++)
	{
		SkillStatusElement element;
		element.id		=	tIterator->id;
		element.percent =	tIterator->percent;
		element.value	=	tIterator->value;
		element.state	=	tIterator->state;
		element.dwSkillID = skillid;
		element.dwTime = tIterator->time;
		_passivenessElement[element.id]=element;

#ifdef _DEBUGLOG
			Zebra::logger->debug("[���ñ���]֮[%u]��״̬��ʩ��������",element.id);
			Channel::sendSys(entry->tempid, Cmd::INFO_TYPE_SYS,"[���ñ���]֮[%u]��״̬��ʩ��������",element.id);
#endif
	}
}

/**
 * \brief  ���Ӳ���״̬�ĳ���ʱ����ֵ
 * \author fqnewman
 */
void SkillStatusManager::addBadSkillStatusPersistTime(const DWORD &value)
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator;

	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end() ; tIterator ++)
	{
		if (tIterator->second.byGoodnessType == SKILL_BAD)
		{
			tIterator->second.dwTime += value;
		}
	}

	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end(); tIterator++)
	{
		if (tIterator->second.byGoodnessType == SKILL_BAD)
		{
			tIterator->second.dwTime += value;
			tIterator->second.qwTime += value;
		}
	}
}

/**
 * \brief  ���Ӳ���״̬�ĳ���ʱ��ٷֱ�
 * \author fqnewman
 */
void SkillStatusManager::addBadSkillStatusPersistTimePercent(const DWORD &value)
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator;

	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end() ; tIterator ++)
	{
		if (tIterator->second.byGoodnessType == SKILL_BAD)
		{
			tIterator->second.dwTime = (DWORD)(tIterator->second.dwTime*(value/100.0f));
		}
	}

	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end(); tIterator++)
	{
		if (tIterator->second.byGoodnessType == SKILL_BAD)
		{
			tIterator->second.dwTime = (DWORD)(tIterator->second.dwTime*(value/100.0f));
			tIterator->second.qwTime = (DWORD)(tIterator->second.qwTime*(value/100.0f));
		}
	}
}

/**
 * \brief  �������״̬�е�ָ�����
 * \param byMutexType ���ܴ���
 * \param myMap ������״̬map
 * \param dwID ����״̬id
 * \author fqnewman
 */
void SkillStatusManager::clearMapElement(const BYTE &byMutexType, std::map<DWORD, SkillStatusElement> &myMap, DWORD dwID,bool notify)
{
/*	std::map<DWORD, SkillStatusElement>::iterator tIterator, delIterator;

	for(tIterator = myMap.begin() ; tIterator !=myMap.end();)
	{
		if (tIterator->second.byMutexType == byMutexType)
		{
			if (tIterator->second.id != dwID)
			{
				tIterator->second.byStep = ACTION_STEP_CLEAR;
				runStatusElement(tIterator->second);
			}
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false);
			}
			delIterator = tIterator;
			tIterator ++;
			myMap.erase(delIterator->first);
		}
		else
		{
			tIterator ++;
		}
	}*/
	std::map<DWORD, SkillStatusElement>::iterator tIterator, delIterator;

	for(tIterator = myMap.begin() ; tIterator !=myMap.end();)
	{
		if (tIterator->second.id == byMutexType)
		{
			if (tIterator->second.id != dwID)
			{
				tIterator->second.byStep = ACTION_STEP_CLEAR;
				runStatusElement(tIterator->second);
			}
			if (tIterator->second.state >0)
			{
				//entry->clearEffectStatus(tIterator->second.state);
				entry->showCurrentEffect(tIterator->second.state, false,notify);
			}
			delIterator = tIterator;
			tIterator ++;
			myMap.erase(delIterator->first);
		}
		else
		{
			tIterator ++;
		}
	}

}

void SkillStatusManager::clearRecoveryElement(DWORD value)
{
	clearMapElement(value,_recoveryElement, 0);
}

void SkillStatusManager::clearActiveElement(DWORD value)
{
	clearMapElement(value,_activeElement, 0);
}

void SkillStatusManager::processDeath()
{
	if (entry->bombskillId >0)
	{
		Cmd::stAttackMagicUserCmd cmd;

		cmd.byAttackType = Cmd::ATTACKTYPE_U2P;
		cmd.dwDefenceTempID = 0;
		cmd.dwUserTempID = entry->tempid;
		cmd.wdMagicType = entry->bombskillId;
		cmd.byAction = Cmd::Ani_Num;
		cmd.byDirect = entry->getDir();
		cmd.xDes = (WORD)entry->getPos().x;
		cmd.yDes = (WORD)entry->getPos().y;

		zSkill *s = NULL;

		s = zSkill::createTempSkill(entry , entry->bombskillId , 1);
		if(s)
		{
			s->action(&cmd, sizeof(cmd));
			SAFE_DELETE(s);
		}
	}

	clearActiveSkillStatus();
}


/**
 * \brief  ���Ժ���������ʾ����״̬��ǰֵ
 * \author fqnewman
 */
void SkillStatusManager::showValue()
{
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��ɫ���ϵ�[SkillValue]�����б�:---------------------------");
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�˺�ֵ���ӹ̶���ֵ1=%ld",entry->skillValue.dvalue);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�˺�ֵ����x%2=%ld",entry->skillValue.dvaluep);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "���������ֵ��� 57��86=%ld",entry->skillValue.pdefence);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�����������ٷֱ�=%ld",entry->skillValue.pdefencep);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��Ϊ���С����79=%ld",entry->skillValue.topet);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "���������˺�83=%ld",entry->skillValue.appenddam);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�ƶ��ٶȱ���ٷֱ� 16��56=%ld",entry->skillValue.movespeed);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "���ټ���ʩ�ż��17=%ld",entry->skillValue.mgspeed);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�����������״̬����18=%ld",entry->skillValue.coldp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "���������ж�״̬����19=%ld",entry->skillValue.poisonp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��������ʯ��״̬����20=%ld",entry->skillValue.petrifyp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��������ʧ��״̬����21=%ld",entry->skillValue.blindp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�����������״̬����22=%ld",entry->skillValue.chaosp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����������33,64=%ld",entry->skillValue.atrating);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����������33,64=%ld",entry->skillValue.reduce_atrating);

	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����ֵ�ָ��ٶ�����34=%ld",entry->skillValue.hpspeedup);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����ֵ�ָ��ٶ�����35=%ld",entry->skillValue.mpspeedup);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����ֵ�ָ��ٶ�����36=%ld",entry->skillValue.spspeedup);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����������37=%ld",entry->skillValue.akdodge);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����45=%ld",entry->skillValue.reflect);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����x%46=%ld",entry->skillValue.reflectp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "Ϊ�����ٷ�֮���ĵ����˺�50=%ld",entry->skillValue.reflect2);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�����������59��=%ld",entry->skillValue.mdefence);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "������������ٷֱ�x%=%ld",entry->skillValue.mdefencep);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�����ٶȱ��80��81=%ld",entry->skillValue.uattackspeed);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "���������ߴ�״̬����82=%ld",entry->skillValue.sevendownp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�˺�ת�ưٷֱ�=%ld",entry->skillValue.tsfdamp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����:�����˺�=%ld",entry->skillValue.passdam);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��ɫ���ϵ�[PkValue]�����б�:---------------------------");
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��������=%ld",entry->pkValue.pdamage);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "���������=%ld",entry->pkValue.pdefence);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����������=%ld",entry->pkValue.mdamage);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����������=%ld",entry->pkValue.mdefence);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "���ķ���ֵ=%ld",entry->pkValue.mcost);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��������ֵ=%ld",entry->pkValue.hpcost);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��������ֵ=%ld",entry->pkValue.spcost);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��������=%ld",entry->pkValue.exp);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�˺�ֵ=%ld",entry->pkValue.dvalue);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�˺�ֵ���Ӱٷֱ�=%ld",entry->pkValue.dvaluep);
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "�б����:----------------------------------------------");
}

/**
 * \brief  ��ӡ����״̬��ǰֵ��log�ļ���
 * \author fqnewman
 */
void SkillStatusManager::showValueToLog()
{
	Zebra::logger->debug("��ɫ���ϵ�[SkillValue]�����б�:---------------------------");
	Zebra::logger->debug("�˺�ֵ���ӹ̶���ֵ1=%ld",entry->skillValue.dvalue);
	Zebra::logger->debug("�˺�ֵ����x%2=%ld",entry->skillValue.dvaluep);
	Zebra::logger->debug("���������ֵ��� 57��86=%ld",entry->skillValue.pdefence);
	Zebra::logger->debug("�����������ٷֱ�=%ld",entry->skillValue.pdefencep);
	Zebra::logger->debug("��Ϊ���С����79=%ld",entry->skillValue.topet);
	Zebra::logger->debug("���������˺�83=%ld",entry->skillValue.appenddam);
	Zebra::logger->debug("�ƶ��ٶȱ���ٷֱ� 16��56=%ld",entry->skillValue.movespeed);
	Zebra::logger->debug("���ټ���ʩ�ż��17=%ld",entry->skillValue.mgspeed);
	Zebra::logger->debug("�����������״̬����18=%ld",entry->skillValue.coldp);
	Zebra::logger->debug("���������ж�״̬����19=%ld",entry->skillValue.poisonp);
	Zebra::logger->debug("��������ʯ��״̬����20=%ld",entry->skillValue.petrifyp);
	Zebra::logger->debug("��������ʧ��״̬����21=%ld",entry->skillValue.blindp);
	Zebra::logger->debug("�����������״̬����22=%ld",entry->skillValue.chaosp);
	Zebra::logger->debug("����������33,64=%ld",entry->skillValue.atrating);
	Zebra::logger->debug("����ֵ�ָ��ٶ�����34=%ld",entry->skillValue.hpspeedup);
	Zebra::logger->debug("����ֵ�ָ��ٶ�����35=%ld",entry->skillValue.mpspeedup);
	Zebra::logger->debug("����ֵ�ָ��ٶ�����36=%ld",entry->skillValue.spspeedup);
	Zebra::logger->debug("����������37=%ld",entry->skillValue.akdodge);
	Zebra::logger->debug("����45=%ld",entry->skillValue.reflect);
	Zebra::logger->debug("����x%46=%ld",entry->skillValue.reflectp);
	Zebra::logger->debug("Ϊ�����ٷ�֮���ĵ����˺�50=%ld",entry->skillValue.reflect2);
	Zebra::logger->debug("�����������59��=%ld",entry->skillValue.mdefence);
	Zebra::logger->debug("������������ٷֱ�x%=%ld",entry->skillValue.mdefencep);
	Zebra::logger->debug("�����ٶȱ��80��81=%ld",entry->skillValue.uattackspeed);
	Zebra::logger->debug("���������ߴ�״̬����82=%ld",entry->skillValue.sevendownp);
	Zebra::logger->debug("�˺�ת�ưٷֱ�=%ld",entry->skillValue.tsfdamp);
	Zebra::logger->debug("����:�����˺�=%ld",entry->skillValue.passdam);
	Zebra::logger->debug("��ɫ���ϵ�[PkValue]�����б�:---------------------------");
	Zebra::logger->debug("��������=%ld",entry->pkValue.pdamage);
	Zebra::logger->debug("���������=%ld",entry->pkValue.pdefence);
	Zebra::logger->debug("����������=%ld",entry->pkValue.mdamage);
	Zebra::logger->debug("����������=%ld",entry->pkValue.mdefence);
	Zebra::logger->debug("���ķ���ֵ=%ld",entry->pkValue.mcost);
	Zebra::logger->debug("��������ֵ=%ld",entry->pkValue.hpcost);
	Zebra::logger->debug("��������ֵ=%ld",entry->pkValue.spcost);
	Zebra::logger->debug("��������=%ld",entry->pkValue.exp);
	Zebra::logger->debug("�˺�ֵ=%ld",entry->pkValue.dvalue);
	Zebra::logger->debug("�˺�ֵ���Ӱٷֱ�=%ld",entry->pkValue.dvaluep);
	Zebra::logger->debug("�б����:----------------------------------------------");
}

/**
 * \brief  ��ʾ��ɫ���ϵ���������״̬
 * \author fqnewman
 */
void SkillStatusManager::showActive()
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator;
	SkillStatusElement *element;

	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��ɫ���ϵ�����״̬�б�:");
	for(tIterator = _activeElement.begin() ; tIterator !=_activeElement.end(); tIterator++)
	{
		std::string myname;
		char buf [45];
		element = &tIterator->second;
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(element->dwTempID);
		if (pUser)
			myname = pUser->name;
		else
		{
			sprintf(buf,"��ʱID:%u", element->dwTempID);
			myname = buf;
		}
		Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����:%u ����:%u ״̬���:%u %s ʣ��ʱ��:%u ������:%s ����:%u ��ʱ��:%u ״ֵ̬%u ��Чֵ:%u",element->dwSkillID,element->byMutexType,element->id, element->byGoodnessType==1?"�˺�":"��ƽ",element->dwTime,myname.c_str(), element->percent, element->dwTime,element->value, element->state);
	}
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "----------------------------------------------");
}

/**
 * \brief  ��ʾ��ɫ���ϵ���ʱ��������״̬
 * \author fqnewman
 */
void SkillStatusManager::showRecovery()
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator;
	SkillStatusElement *element;

	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��ɫ���ϵ���ʱ����״̬�б�:");
	for(tIterator = _recoveryElement.begin() ; tIterator !=_recoveryElement.end(); tIterator++)
	{
		std::string myname;
		char buf [45];
		element = &tIterator->second;
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(element->dwTempID);
		if (pUser)
			myname = pUser->name;
		else
		{
			sprintf(buf,"��ʱID:%u", element->dwTempID);
			myname = buf;
		}
		Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����:%u ����:%u ״̬���:%u %s ʣ��ʱ��:%u ������:%s ����:%u ��ʱ��:%u ״ֵ̬%u ��Чֵ:%u",element->dwSkillID,element->byMutexType,element->id, element->byGoodnessType==1?"�˺�":"��ƽ",element->dwTime,myname.c_str(), element->percent, element->dwTime,element->value, element->state);
	}
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "----------------------------------------------");
}

/**
 * \brief  ��ʾ��ɫ���ϵ����ñ�������״̬
 * \author fqnewman
 */
void SkillStatusManager::showPassiveness()
{
	std::map<DWORD, SkillStatusElement>::iterator tIterator;
	SkillStatusElement *element;

	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "��ɫ���ϵ����ñ���״̬�б�:");
	for(tIterator = _passivenessElement.begin() ; tIterator !=_passivenessElement.end(); tIterator++)
	{
		std::string myname;
		char buf [45];
		element = &tIterator->second;
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(element->dwTempID);
		if (pUser)
			myname = pUser->name;
		else
		{
			sprintf(buf,"��ʱID:%u", element->dwTempID);
			myname = buf;
		}
		Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "����:%u ����:%u ״̬���:%u %s ʣ��ʱ��:%u ������:%s ����:%u ��ʱ��:%u ״ֵ̬%u ��Чֵ:%u",element->dwSkillID,element->byMutexType,element->id, element->byGoodnessType==1?"�˺�":"��ƽ",element->dwTime,myname.c_str(), element->percent, element->dwTime,element->value, element->state);
	}
	Channel::sendSys((SceneUser *)entry, Cmd::INFO_TYPE_SYS, "----------------------------------------------");
}

/**
 * \brief  ��õ�ǰ��ɫ��Я���ļ���״̬��Ŀ
 */
WORD SkillStatusManager::getSaveStatusSize()
{
	WORD ret =0;

	ret = _recoveryElement.size()+_activeElement.size();
	return ret; 
}
