/**
 * \file
 * \version  $Id: CDare.cpp  $
 * \author  
 * \date 
 * \brief ʵ�ֶ�ս������
 *
 */

#include "zRWLock.h"
#include "SchoolManager.h"
#include "CUnion.h"
#include "CSept.h"
#include "CDare.h"
#include "Command.h"
#include "zDBConnPool.h"
#include "SessionServer.h"
#include "SessionTaskManager.h"
#include "Session.h"
#include "CRelationManager.h"
#include "zTime.h"
#include <vector>
#include "CCityManager.h"
#include "CCountryManager.h"
#include "CDareRecordManager.h"
#include "SessionChat.h"
#include "Dare.h"
#include "CDareSept.h"
#include "CDareSeptNpc.h"
#include "CDareEmperor.h"
#include "TimeTick.h"

using namespace DareDef;

CDare::CDare(DWORD active_time, DWORD ready_time)
{
	count = 0;
	secondID = 0;
	userid1 = 0;
	userid2 = 0;
	grade1 = 0;
	grade2 = 0;
	pk1 = 0;
	pk2 = 0;
	attList.clear();
	dwWinnerID = 0;
	isAntiAtt = false;

	if (active_time == 0)
	{
		this->active_time = ::dare_active_time;
	}
	else 
	{
		this->active_time = active_time;
	}

	if (ready_time == 0)
	{
		this->ready_time = ::dare_ready_time;
	}
	else 
	{
		this->ready_time = ready_time;
	}

	start_time = time(NULL);
}

CDare::~CDare()
{
}

void CDare::timer()
{
}

void CDare::printState()
{
	for (DWORD i=0; i<attList.size(); i++)
	{
		Zebra::logger->trace("[��ս����]:%d(%d,%d,%s), %s", this->tempid, attList[i], secondID, str_type[this->type],
				str_state[this->state]);
	}
}

void CDare::setReadyState()
{
	rwlock.wrlock();
	this->state = CDare::DARE_READY;
	rwlock.unlock();
	this->printState();
}

void CDare::setReadyActiveState(UserSession* pUser)
{
	Cmd::Session::t_dareGold_SceneSession send;

	if (this->type == Cmd::UNION_DARE)
	{
		send.dwNum = (-dare_need_gold);
	}
		
	send.dwWarID = this->tempid;
	if (pUser && pUser->scene)
	{
		send.dwUserID = pUser->id; // Ӧս��ID
		this->userid2 = pUser->id;

		send.dwType = Cmd::Session::DARE_GOLD;
		send.dwWarType = this->type;

		pUser->scene->sendCmd(&send, sizeof(send));
		rwlock.wrlock();
		this->count = 0;
		this->state = CDare::DARE_READY_ACTIVE;
		rwlock.unlock();
		
		Zebra::logger->trace("��ս���Ӹ��٣�%s(%d) �۳� %d ����ս��", pUser->name, pUser->unionid, send.dwNum);

		this->printState();
	}
	else
	{
		// TODO:����Ҳ���Ӧս��
		setReturnGoldState();
	}
}

void CDare::setReadyOverState()
{
	// TODO:֪ͨ���������ң������佻ս״̬���������ҷ���ϵͳ��Ϣ����֮��ս����������Ӯ�Ҽ�������
	// �ж�����Ƿ�����

	Cmd::Session::t_dareGold_SceneSession send;     // ��ʤ�߽������
	Cmd::Session::t_enterWar_SceneSession exit_war; // ֪ͨ�������˳���ս״̬

	UserSession* pUser1 = NULL;
	UserSession* pUser2 = NULL;

	bool wait_bound = false;  // �Ƿ����DARE_WAIT_BOUND״̬
	int winner_type = 0;  // Ϊ0����ս��ʤ��Ϊ1��Ӧս��ʤ��2��սƽ
	send.dwWarID = this->id;

	rwlock.wrlock();
	this->state = CDare::DARE_READY_OVER;
	rwlock.unlock();
	
	if (this->type == Cmd::UNION_DARE)
	{
		send.dwNum = dare_winner_gold; // ʤ����������������
	}
	
	send.dwWarType = this->type;
	send.dwType = Cmd::Session::WINNER_GOLD;


	this->printState();
	
	exit_war.dwWarType = this->type;
	exit_war.dwStatus = 0;

	for (DWORD i=0; i<attList.size(); i++)
	{
		this->sendCmdToAllDarePlayer(&exit_war, sizeof(exit_war), attList[i]);
	}
	
	this->sendCmdToAllDarePlayer(&exit_war, sizeof(exit_war), this->secondID);

	pUser1 = UserSessionManager::getInstance()->getUserByID(this->userid1);
	pUser2 = UserSessionManager::getInstance()->getUserByID(this->userid2);

	if (grade1>grade2)
	{
		send.dwUserID = this->userid1;
		winner_type = 0;

		if (pUser1)
		{
			pUser1->scene->sendCmd(&send, sizeof(send));
		}
		else
		{// Ӯ�����������ߣ�����DARE_WAIT_BOUND״̬
			wait_bound = true;
		}
	}
	else if (grade1<grade2)
	{
		send.dwUserID = this->userid2;
		winner_type = 1;

		if (pUser2)
		{
			pUser2->scene->sendCmd(&send, sizeof(send));
		}
		else
		{
			wait_bound = true;
		}
	}
	else if (grade1 == grade2)
	{
		if (this->type == Cmd::UNION_DARE)
		{
			send.dwNum = dare_winner_gold/2;
		}
			
		winner_type = 2;

		if (pUser1 && pUser2)
		{
			send.dwUserID = this->userid1;
			pUser1->scene->sendCmd(&send, sizeof(send));

			send.dwUserID = this->userid2;
			pUser2->scene->sendCmd(&send, sizeof(send));
		}
		else
		{
			wait_bound = true;
		}
	}

	// ͨ���ս���
	this->notifyWarResult(winner_type);
	
	if (wait_bound)
	{
		rwlock.wrlock();
		this->state = CDare::DARE_WAIT_BOUNTY;
		rwlock.unlock();
		this->printState();
	}
	else
	{
		this->setOverState();
	}
}

void CDare::setWaitBountyState()
{
	if (this->computeResult())
	{
		this->setOverState();
	}
	// �����δ�����ɹ����򱣳ָ�״̬����
}

bool CDare::computeResult()
{
	Cmd::Session::t_dareGold_SceneSession send;

	UserSession* pUser1 = NULL;
	UserSession* pUser2 = NULL;

	send.dwWarID = this->id;
	if (this->type == Cmd::UNION_DARE)
	{
		send.dwNum = dare_winner_gold;
	}

	send.dwType = Cmd::Session::WINNER_GOLD;
	send.dwWarType = this->type;

	
	pUser1 = UserSessionManager::getInstance()->getUserByID(this->userid1);
	pUser2 = UserSessionManager::getInstance()->getUserByID(this->userid2);

	if (pUser1 == NULL || pUser2 == NULL)
	{
		return false;
	}

	if (grade1>grade2)
	{
		send.dwUserID = this->userid1;

		if (pUser1 && pUser1->scene)
		{
			pUser1->scene->sendCmd(&send, sizeof(send));
		}
		else
		{// Ӯ�����������ߣ�����DARE_WAIT_BOUND״̬
			return false;
		}
	}
	else if (grade1<grade2)
	{
		send.dwUserID = this->userid2;

		if (pUser2 && pUser2->scene)
		{
			pUser2->scene->sendCmd(&send, sizeof(send));
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (this->type == Cmd::UNION_DARE)
		{
			send.dwNum = dare_winner_gold/2;
		}
	
		if (pUser1 && pUser2 && pUser1->scene && pUser2->scene)
		{
			send.dwUserID = this->userid1;
			pUser1->scene->sendCmd(&send, sizeof(send));

			send.dwUserID = this->userid2;
			pUser2->scene->sendCmd(&send, sizeof(send));
		}
		else
		{
			return false;
		}
	}


	return true;

}

void CDare::setOverState()
{
	rwlock.wrlock();
	this->state = CDare::DARE_OVER;
	rwlock.unlock();
	this->printState();
}

void CDare::setReturnGoldState()
{// �����ս������Ѿ������ߣ��ʹ���DARE_RETURN_GOLD״̬�����������ߣ�����Ǯ��������OVER״̬
	Cmd::Session::t_dareGold_SceneSession send;
	UserSession* pUser = UserSessionManager::getInstance()->getUserByID(this->userid1);

#ifdef _ZJW_DEBUG
	Zebra::logger->debug("Ӧս�߾ܾ����������㣬���ؽ�Ҹ���ս��:%d", dare_need_gold);
#endif	
	rwlock.wrlock();

	if (pUser && pUser->scene)
	{
		send.dwUserID = this->userid1;
		if (this->type == Cmd::UNION_DARE)
		{
			send.dwNum = dare_need_gold;
		}
		send.dwType = Cmd::Session::RETURN_DARE_GOLD;
		send.dwWarType = this->type;
		
		send.dwWarID = this->tempid; // ����Ҫ�ٴ���
		pUser->scene->sendCmd(&send, sizeof(send));
		Zebra::logger->trace("��ս���Ӹ��٣�%s(%d) ���� %d ����ս��", pUser->name, pUser->unionid, send.dwNum);

		this->state = CDare::DARE_OVER;
	}
	else
	{

		this->state = CDare::DARE_RETURN_GOLD;
	}

	rwlock.unlock();

	this->printState();
}

void CDare::setActiveState()
{
	Cmd::Session::t_enterWar_SceneSession send;

	send.dwWarType = this->type;
	
	send.dwToRelationID = this->secondID;
	send.isAtt = true;
	
	for (DWORD i=0; i<attList.size(); i++)
	{
		this->sendCmdToAllDarePlayer(&send, sizeof(send), attList[i]);
	}
	
	send.isAtt = false;
	
	for (DWORD i=0; i<attList.size(); i++)
	{
		send.dwToRelationID = attList[i];
		this->sendCmdToAllDarePlayer(&send, sizeof(send), this->secondID);
	}

	rwlock.wrlock();
	this->state = CDare::DARE_ACTIVE;
	rwlock.unlock();

	this->printState();
}

void CDare::setReadyQuestionState()
{
	DWORD toUserID = 0;
	char  fromRelationName[MAX_NAMESIZE+1];
	bzero(fromRelationName, sizeof(fromRelationName));

	strncpy(fromRelationName, this->getFirstName(), MAX_NAMESIZE);
	toUserID = getSecondUserID();
	
	rwlock.wrlock();

	if (toUserID != 0)
	{
		UserSession* pDareUser = UserSessionManager::getInstance()->getUserByID(this->userid1);
		if (this->type==Cmd::UNION_DARE)
		{
			CUnion* pUnion = CUnionM::getMe().getUnionByID(this->secondID);
			if (pUnion && pDareUser)
			{
				pUnion->sendUnionDare(pDareUser, fromRelationName, this->tempid);
			}
		}
		else
		{
			userid2 = toUserID;               // ����Ӧս��ID
			UserSession* pUser = UserSessionManager::getInstance()->getUserByID(this->userid2);
			UserSession* pDareUser = UserSessionManager::getInstance()->getUserByID(this->userid1);

			if (pUser && pDareUser)
			{//�ҵ���ЧӦս��,���䷢����սѯ������
				Cmd::stActiveDareCmd send;

				send.dwMsgType = Cmd::DARE_QUESTION;
				send.dwWarID   = this->tempid;
				send.dwDareType = this->type;
				
				strncpy(send.name, pDareUser->name, MAX_NAMESIZE);
				strncpy(send.fromRelationName, fromRelationName, MAX_NAMESIZE);
				pUser->sendCmdToMe(&send, sizeof(send));
#ifdef _ZJW_DEBUG		    
				Zebra::logger->debug("��Ӧս�߷���ѯ�ʡ�");
#endif		    
			}
			else
			{//Ӧս��
#ifdef _ZJW_DEBUG		    
				Zebra::logger->debug("Ӧս��δ����");
#endif		    
			}
		}
	}
	else
	{
#ifdef _ZJW_DEBUG	    
		Zebra::logger->debug("δ�ҵ���Ч��Ӧս��");
#endif	    
	}

	state = CDare::DARE_READY_QUESTION;
	rwlock.unlock();
	this->printState();
}

bool CDare::isAtt(DWORD dwID)
{
	for (DWORD i=0; i<attList.size(); i++)
	{
		if (attList[i] == dwID)
		{
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------------------------------
CDareUnion::CDareUnion() : CDare(0,0)
{
	//pFirst = NULL;
	//pSecond = NULL;
}

CDareUnion::CDareUnion(DWORD active_time, DWORD ready_time) : CDare(active_time, ready_time)
{
}

CDareUnion::~CDareUnion()
{
}

void CDareUnion::setSecondID(DWORD dwID)
{
	this->secondID = dwID;
}

void CDareUnion::addFirstID(DWORD dwID)
{
	attList.push_back(dwID);
}

void CDareUnion::addGrade(UserSession* pAtt, UserSession* pDef)
{
	if (pAtt==NULL || pDef == NULL)
		return;

	if (pAtt->unionid == secondID)
	{
		pk2 = pk2 + 1;
		grade2 = pDef->level/10 + grade2;
	}
	else
	{
		for (DWORD i=0; i<attList.size(); i++)
		{
			if (pAtt->unionid == attList[i])
			{
				pk1 = pk1 + 1; //�ۼ�PK����
				grade1 = pDef->level/10 + grade1;
			}
		}
	}
}

void CDareUnion::sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, unsigned int cmdLen, unsigned int relationID)
{
	if (cmd->isAtt)
	{// ����Ǹ�������������Ϣ
		CUnion *pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(relationID);
		CUnion *pDefUnion = (CUnion*)CUnionM::getMe().getUnionByID(cmd->dwToRelationID);

		if (pAttUnion && pDefUnion) 
		{
			if (cmd->dwStatus ==1)
			{
				for (int i=0; i<3; i++)
				{
					pAttUnion->sendUnionNotify("�Ұ�� %s �﷢���˰��ս�����λ����ע�ⰲȫ", 
							pDefUnion->name);
				}
			}

			pAttUnion->sendCmdToAllMemberScene(cmd, cmdLen);
		}
	}
	else
	{
		CUnion *pDefUnion = (CUnion*)CUnionM::getMe().getUnionByID(relationID);
		CUnion *pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(cmd->dwToRelationID);

		if (pAttUnion && pDefUnion) 
		{
			if (cmd->dwStatus ==1)
			{
				for (int i=0; i<3; i++)
				{
					pDefUnion->sendUnionNotify("�Ұ�� %s �﷢���˰��ս�����λ����ע�ⰲȫ", 
							pAttUnion->name);
				}
			}

			pDefUnion->sendCmdToAllMemberScene(cmd, cmdLen);	
		}
	}
}

void CDareUnion::sendActiveStateToScene(UserSession* pUser)
{
	Cmd::Session::t_enterWar_SceneSession send;
        send.dwWarType = type;
	
	if (secondID != pUser->unionid)
	{
		send.dwToRelationID = secondID;
		send.isAtt = true;
		send.dwUserID = pUser->id;
		send.dwSceneTempID = pUser->scene->tempid;
		pUser->scene->sendCmd(&send, sizeof(send));
	}
	else
	{
		for (DWORD i=0; i<attList.size(); i++)
		{
			send.dwToRelationID = attList[i];
			send.isAtt = false;
			send.dwUserID = pUser->id;
			send.dwSceneTempID = pUser->scene->tempid;
			pUser->scene->sendCmd(&send, sizeof(send));
		}
	}
}

void CDareUnion::notifyWarResult(int winner_type)
{
	CUnion *pDefUnion = (CUnion*)CUnionM::getMe().getUnionByID(this->secondID);
	CUnion *pAttUnion = NULL;

	if (winner_type == 0)
	{
		if (pDefUnion)
		{
			for (DWORD i=0; i<attList.size(); i++)
			{
				pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[i]);
				if (pAttUnion)
				{
					pAttUnion->sendUnionNotify("\n����ս������ ��ս��� %s ��ʤ\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
					pAttUnion->name, pAttUnion->name,
					this->pk1, this->grade1,
					pDefUnion->name,
					this->pk2, this->grade2);
					
					pDefUnion->sendUnionNotify("\n����ս������ ��ս��� %s ʧ��\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
					pDefUnion->name, pAttUnion->name,
					this->pk1, this->grade1,
					pDefUnion->name,
					this->pk2, this->grade2);

					SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, pAttUnion->dwCountryID,
							"%s ����ձ� %s ������ꪳ�ɳ�������ף�Ŭ���ܶ�����",
							pDefUnion->name, pAttUnion->name);
				}
			}
		}
	}
	else if (winner_type == 1)
	{
		if (pDefUnion)
		{
			for (DWORD i=0; i<attList.size(); i++)
			{
				pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[i]);
				if (pAttUnion)
				{
					pAttUnion->sendUnionNotify("\n����ս������ ��ս��� %s ʧ��\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
					pAttUnion->name, pAttUnion->name,
					this->pk1, this->grade1,
					pDefUnion->name,
					this->pk2, this->grade2);
				
				pDefUnion->sendUnionNotify("\n����ս������ ��ս��� %s ��ʤ\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
					pDefUnion->name, pAttUnion->name,
					this->pk1, this->grade1,
					pDefUnion->name,
					this->pk2, this->grade2);

				SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, pAttUnion->dwCountryID,
							"%s ����ձ� %s ������ꪳ�ɳ�������ף�Ŭ���ܶ�����",
							pAttUnion->name, pDefUnion->name);

				}
			}
		}
	}
	else
	{
		if (pDefUnion)
		{
			 for (DWORD i=0; i<attList.size(); i++)
			{
				pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[i]);
				if (pAttUnion)
				{
					pAttUnion->sendUnionNotify("\n����ս������ ��ս��� %s սƽ\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
							pAttUnion->name, pAttUnion->name,
							this->pk1, this->grade1,
							pDefUnion->name,
							this->pk2, this->grade2);

					pDefUnion->sendUnionNotify("\n����ս������ ��ս��� %s սƽ\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
							pDefUnion->name, pAttUnion->name,
							this->pk1, this->grade1,
							pDefUnion->name,
							this->pk2, this->grade2);
				}
			}
		}
	}
}

char* CDareUnion::getFirstName()
{
	if (attList.size()>0)
	{
		CUnion *pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[0]);
		if (pAttUnion)	
		{
			return pAttUnion->name;
		}
	}
	
	return NULL;
}

DWORD CDareUnion::getSecondUserID()
{
	CUnion *pSecond = (CUnion*)CUnionM::getMe().getUnionByID(this->secondID);
	if (pSecond && pSecond->master && pSecond->master->byStatus)
	{
		return pSecond->master->id;
	}

	return 0;
}

void CDareUnion::timer()
{
	rwlock.wrlock();
	CUnion *pSecond = (CUnion*)CUnionM::getMe().getUnionByID(this->secondID);
	CUnion *pAttUnion = NULL;
	
	if (this->state == CDare::DARE_ACTIVE)
	{
		if (this->count%900 == 0 && this->count!=active_time)
		{
			if (pSecond)
			{
				for (DWORD i=0; i<attList.size(); i++)
				{
					pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[i]);
					if (pAttUnion)
					{
						pAttUnion->sendUnionNotify("�Ұ�� %s �﷢���˰��ս�����λ����ע�ⰲȫ", 
								pSecond->name);

						pSecond->sendUnionNotify("�Ұ�� %s �﷢���˰��ս�����λ����ע�ⰲȫ", 
								pAttUnion->name);
					}
				}
			}
		}
		else if (this->count==3000)
		{
			if (pSecond) 
			{
				for (DWORD i=0; i<attList.size(); i++)
				{
					pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[i]);
					
					if (pAttUnion)
					{	
						pAttUnion->sendUnionNotify("�Ұ��� %s ���ս���� 10 �����ڽ���", 
								pSecond->name);
						
						pSecond->sendUnionNotify("�Ұ��� %s ���ս���� 10 �����ڽ���", 
								pAttUnion->name);
					}
				}
			}
		}
		else if (this->count>=3300 && (this->count%60 ==0))
		{
			if (pSecond) 
			{
				for (DWORD i=0; i<attList.size(); i++)
				{
					 pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[i]);

					 if (pAttUnion)
					 {    
						 pAttUnion->sendUnionNotify("�Ұ��� %s ���ս���� %d �����ڽ���", 
								 pSecond->name, 
							(this->active_time-this->count)/60);

						 pSecond->sendUnionNotify("�Ұ��� %s ���ս���� %d �����ڽ���", 
								 pAttUnion->name, 
							(this->active_time-this->count)/60);
					 }
				}
			}
		}
	}
	
	rwlock.unlock();
}

bool CDareUnion::isInvalid()
{
	return false;
}

//-------------------------------------------------------------------------------------
CDareSchool::CDareSchool() : CDare(0, 0)
{
}

CDareSchool::~CDareSchool()
{
}

void CDareSchool::setSecondID(DWORD dwID)
{
	this->secondID = dwID;
}

void CDareSchool::addFirstID(DWORD dwID)
{
	attList.push_back(dwID);
}

void CDareSchool::addGrade(UserSession* pAtt, UserSession* pDef)
{
	if (pAtt==NULL || pDef == NULL)
		return;

	if (pAtt->schoolid == secondID)
	{
		pk2 = pk2 + 1;
		grade2 = pDef->level/10 + grade2;
	}
	else
	{
		for (DWORD i=0; i<attList.size(); i++)
		{
			if (pAtt->schoolid == attList[i])
			{
				pk1 = pk1 + 1; //�ۼ�PK����
				grade1 = pDef->level/10 + grade1;
			}
		}
	}
}

void CDareSchool::sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, unsigned int cmdLen, unsigned int relationID)
{
	if (cmd->isAtt)
	{// ����Ǹ�������������Ϣ
		CSchool *pAttSchool = (CSchool*)CSchoolM::getMe().getSchool(relationID);
		pAttSchool->sendCmdToAllMemberScene(cmd, cmdLen);
	}
	else
	{
		CSchool *pDefSchool = (CSchool*)CSchoolM::getMe().getSchool(relationID);
		pDefSchool->sendCmdToAllMemberScene(cmd, cmdLen);
	}
}

void CDareSchool::sendActiveStateToScene(UserSession* pUser)
{
	Cmd::Session::t_enterWar_SceneSession send;
	send.dwWarType = type;

	if (secondID != pUser->schoolid)
	{
		send.dwToRelationID = secondID;
		send.isAtt = true;
		send.dwUserID = pUser->id;
		send.dwSceneTempID = pUser->scene->tempid;
		pUser->scene->sendCmd(&send, sizeof(send));
	}
	else
	{
		for (DWORD i=0; i<attList.size(); i++)
		{
			send.dwToRelationID = attList[i];
			send.isAtt = false;
			send.dwUserID = pUser->id;
			send.dwSceneTempID = pUser->scene->tempid;
			pUser->scene->sendCmd(&send, sizeof(send));
		}
	}
}

void CDareSchool::notifyWarResult(int winner_type)
{
	 CSchool *pDefSchool = (CSchool*)CSchoolM::getMe().getSchool(this->secondID);
         CSchool *pAttSchool = NULL;
	 
	if (winner_type == 0)
	{
		if (pDefSchool)
		{
			for (DWORD i=0; i<attList.size(); i++)
			{
				 pAttSchool = (CSchool*)CSchoolM::getMe().getSchool(attList[i]);
                                 if (pAttSchool)
				 {
					 pAttSchool->notifyWarResult("ʦ�Ŷ�ս������ ��ս���: %s ��ʤ\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
							 pAttSchool->getSchoolName(), pAttSchool->getSchoolName(),
							 this->pk1, this->grade1,
							 pDefSchool->getSchoolName(), 
							 this->pk2, this->grade2);

					 pDefSchool->notifyWarResult("ʦ�Ŷ�ս������ ��ս��� %s ʧ��\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
							 pDefSchool->getSchoolName(), pAttSchool->getSchoolName(),
							 this->pk2, this->grade2,
							 pDefSchool->getSchoolName(),
							 this->pk2, this->grade2);
				 }
			}
		}
	}
	else if (winner_type == 1)
	{
		if (pDefSchool)
		{
			for (DWORD i=0; i<attList.size(); i++)
			{
				pAttSchool = (CSchool*)CSchoolM::getMe().getSchool(attList[i]);
				if (pAttSchool)
				{
					pAttSchool->notifyWarResult("ʦ�Ŷ�ս������ ��ս���: %s ս��\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
							pAttSchool->getSchoolName(), pAttSchool->getSchoolName(),
							this->pk1, this->grade1,
							pDefSchool->getSchoolName(), 
							this->pk2, this->grade2);

					pDefSchool->notifyWarResult("ʦ�Ŷ�ս������ ��ս��� %s ��ʤ\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
							pDefSchool->getSchoolName(), pAttSchool->getSchoolName(),
							this->pk2, this->grade2,
							pDefSchool->getSchoolName(),
							this->pk2, this->grade2);
				}
			}
		}
	}
	else 
	{
		if (pDefSchool)
		{
			for (DWORD i=0; i<attList.size(); i++)
			{
				pAttSchool = (CSchool*)CSchoolM::getMe().getSchool(attList[i]);
				if (pAttSchool)
				{
					pAttSchool->notifyWarResult("ʦ�Ŷ�ս������ ��ս���: %s սƽ\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
							pAttSchool->getSchoolName(), pAttSchool->getSchoolName(),
							this->pk1, this->grade1,
							pDefSchool->getSchoolName(), 
							this->pk2, this->grade2);

					pDefSchool->notifyWarResult("ʦ�Ŷ�ս������ ��ս��� %s սƽ\n %s PK����:%d �÷�:%d\n %s PK����:%d �÷�:%d", 
							pDefSchool->getSchoolName(),pAttSchool->getSchoolName(),
							this->pk2, this->grade2,
							pDefSchool->getSchoolName(),
							this->pk2, this->grade2);
				}
			}
		}
	}
}


char* CDareSchool::getFirstName()
{
	if (attList.size()>0)
	{
		CSchool *pAttSchool = (CSchool*)CSchoolM::getMe().getSchool(attList[0]);
		if (pAttSchool)
		{
			return pAttSchool->name;
		}
	}
	
	return NULL;
}

DWORD CDareSchool::getSecondUserID()
{
	CSchool* pSecond = CSchoolM::getMe().getSchool(this->secondID);
	if (pSecond && pSecond->getMasterNode())
	{
		return pSecond->getMasterNode()->getCharID();	
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
CDareUnionCity::CDareUnionCity(DWORD active_time, DWORD ready_time) : CDareUnion(active_time, ready_time)
{
	last_fulltime = 0;
}

CDareUnionCity::~CDareUnionCity()
{
}
		
/**
 * \brief ���ؽ�������
 *
 * ���ؽ�������ֻ�����˳���ս��֪ͨ
 *
 * \param 
 * \return 
 */
void CDareUnionCity::setReadyOverState()
{
	Cmd::Session::t_enterWar_SceneSession exit_war; // ֪ͨ�������˳���ս״̬
	Cmd::Session::t_clearUnionManor_SceneSession send;
	
	exit_war.dwWarType = this->type;
	exit_war.dwStatus = 0;
	
	for (DWORD i=0; i<attList.size(); i++)
	{
		this->sendCmdToAllDarePlayer(&exit_war, sizeof(exit_war), attList[i]);
	}
	
	this->sendCmdToAllDarePlayer(&exit_war, sizeof(exit_war), this->secondID);

	CCity* pCity = CCityM::getMe().findByUnionID(this->secondID);
	CUnion *pSecond = (CUnion*)CUnionM::getMe().getUnionByID(this->secondID);

	// ���������������ʧ���߰����Ա���������
	if (this->grade1>this->grade2)
	{
		if (pSecond) pSecond->changeAllSeptRepute(-5);
		send.dwUnionID = this->dwWinnerID;

		CUnion *pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(this->dwWinnerID);
		if (pAttUnion) 
		{
			if (pSecond && pSecond->dwCountryID!=pAttUnion->dwCountryID && pCity)
			{
				pSecond->sendUnionNotify(
						"ϵͳ���棺�ҹ� %s �� %s �� %s ���ռ��",   
						pCity->getName(), CCountryM::getMe().find(pAttUnion->dwCountryID)->name, 
						pAttUnion->name);
				
				pAttUnion->sendUnionNotify(
						"ϵͳ���棺�ҹ� %s ���ɹ�ռ�� %s �� %s", 
						pAttUnion->name, CCountryM::getMe().find(pSecond->dwCountryID)->name,
						pCity->getName());
				
				if (pCity->dwCountry != pAttUnion->dwCountryID)
				{//��ռ���������,�������繫��
					CCountry* pAttCountry = CCountryM::getMe().find(pAttUnion->dwCountryID);
					CCountry* pDefCountry = CCountryM::getMe().find(pSecond->dwCountryID);

					SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
							"%s %s �ﾭ��һ����ս�ɹ�ռ�� %s %s ���� %s ��",
							pAttCountry->name, pAttUnion->name,
							pDefCountry->name, pSecond->name,
							pCity->getName());
				}
			}
		}
	}
	else
	{
		CCountry* pDefCountry = CCountryM::getMe().find(pSecond->dwCountryID);
		for (DWORD i=0; i<attList.size(); i++)
		{
			CUnion *pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[i]);
			if (pAttUnion) 
			{
				pAttUnion->changeAllSeptRepute(-5);
				//�ҹ�X���ɹ��������ҹ���˳�/����/��β��δ��X��ռ��
				if (pSecond && pSecond->dwCountryID!=pAttUnion->dwCountryID && pCity)
				{
					pSecond->sendUnionNotify(
					"ϵͳ���棺�ҹ� %s ���ɹ������� %s δ�� %s ռ��",   
		    					pSecond->name, pCity->getName(), 
							CCountryM::getMe().find(pAttUnion->dwCountryID)->name);

					pAttUnion->sendUnionNotify(
					"ϵͳ���棺�ҹ� %s ������ %s�� %s ʧ�ܣ����´�Ŭ��",   
		    					pAttUnion->name, CCountryM::getMe().find(pSecond->dwCountryID)->name,
							pCity->getName());

					if (pCity->dwCountry != pAttUnion->dwCountryID)
					{//��ռ���������,�������繫��
						CCountry* pAttCountry = CCountryM::getMe().find(pAttUnion->dwCountryID);

						SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
								"%s %s�﹥ռ %s %s���� %s �ǵ�ս��ʧ��",
								pAttCountry->name, pAttUnion->name,
								pDefCountry->name, pSecond->name,
								pCity->getName());
					}
				}
			}
		}

		send.dwUnionID = this->secondID;
	}
	
	if (pCity)
	{
		send.dwCountryID = pCity->dwCountry;
		send.dwAreaID = pCity->dwCityID*10+1;

		DWORD map_id = (pCity->dwCountry << 16) + pCity->dwCityID*10+1;

		SceneSession *pScene = SceneSessionManager::getInstance()->getSceneByID(map_id);
		if (pScene)
		{
			Cmd::Session::t_unionDareNotify_SceneSession u;
			u.sceneID = map_id;
			u.state = 0;
			pScene->sendCmd(&u, sizeof(u));

			pScene->sendCmd(&send, sizeof(send));
		}
	}


	/*send.dwAreaID = pCity->dwCityID*10+2;
	map_id = (pCity->dwCountry << 16) + pCity->dwCityID*10+2;
	pScene = SceneSessionManager::getInstance()->getSceneByID(map_id);
	if (pScene)
	{
		pScene->sendCmd(&send, sizeof(send));
	}*/

	if (pCity)
	{
		pCity->endDare();
		pCity->writeDatabase();
	}

	this->setOverState();
}

void CDareUnionCity::timer()
{
}

void CDareUnionCity::setReadyState()
{
	CDareUnion::setReadyState();
	
	CCity* pCity = CCityM::getMe().findByUnionID(this->secondID);
	DWORD map_id = (pCity->dwCountry << 16) + pCity->dwCityID*10+1;
	SceneSession *pScene = SceneSessionManager::getInstance()->getSceneByID(map_id);
	if (pScene)
	{
		Cmd::Session::t_unionDareNotify_SceneSession u;
		u.sceneID = map_id;
		u.state = 1;
		pScene->sendCmd(&u, sizeof(u));
	}

	CUnion *pSecond = (CUnion*)CUnionM::getMe().getUnionByID(this->secondID);
	for (DWORD i=0; i<attList.size(); i++)
	{
		CUnion *pAttUnion = (CUnion*)CUnionM::getMe().getUnionByID(attList[i]);
		if (pSecond && pAttUnion) {
			if (pAttUnion->dwCountryID != pSecond->dwCountryID)
			{
				SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_GAME,
						pSecond->dwCountryID, "%s �� %s �����ҹ� %s �Ķ��ս��ʼ����֧Ԯ",
						CCountryM::getMe().find(pAttUnion->dwCountryID)->name,
						pAttUnion->name, pCity->getName());

				SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_GAME,
						pAttUnion->dwCountryID, "�ҹ� %s ��� %s���� %s �Ķ��ս��ʼ����֧Ԯ",
						pAttUnion->name, 
						CCountryM::getMe().find(pSecond->dwCountryID)->name,
						pCity->getName());
			}
		}
	}
}

//-------------------------------------------------------------------------------------


CDareM::CDareM()
{
	channelUniqeID =new zUniqueDWORDID(1000);
}

CDareM::~CDareM()
{
	rwlock.wrlock();
	for(zEntryTempID::hashmap::iterator it=zEntryTempID::ets.begin();it!=zEntryTempID::ets.end();it++)
	{
		CDare *temp =(CDare *)it->second;
		SAFE_DELETE(temp);
	}
	clear();
	rwlock.unlock();
	SAFE_DELETE(channelUniqeID);
}

void CDareM::destroyMe()
{
	delMe();
}

bool CDareM::getUniqeID(DWORD &tempid)
{
	tempid=channelUniqeID->get();
	return (tempid!=channelUniqeID->invalid());
}

void CDareM::putUniqeID(const DWORD &tempid)
{
	channelUniqeID->put(tempid);
}

bool CDareM::init()
{
	return true;
}


/**
 * \brief �������ظ���t_activeDare����
 * \param dare ��ս��¼
 */
void CDareM::activeDare_sceneSession(Cmd::Session::t_activeDare_SceneSession* cmd)
{
	CDare* pDare = (CDare*)CDareM::getMe().getEntryByTempID(cmd->dwWarID);
	Zebra::logger->debug("�����ս %ld ��ACTIVEDARE��Ϣ", cmd->dwWarID);

	if (pDare)
	{//�ҵ���Ӧ��ս��ID
		if (cmd->dwStatus == Cmd::Session::SCENE_ACTIVEDARE_SUCCESS)
		{
			switch (pDare->state)
			{
				case CDare::DARE_READY:
					{// �յ��ɹ��۳���ս�����ӵ���Ϣ,����ѯ��Ӧս��״̬
						Zebra::logger->debug("��ս %ld ���ڴ��� DARE_READY ״̬���յ� ACTIVEDARE_SUCCESS", pDare->tempid);
						pDare->setReadyQuestionState();
					}
					break;
				case CDare::DARE_READY_ACTIVE:
					{ // �յ��ɹ��۳�Ӧս�����ӵ���Ϣ�������ս״̬
						Zebra::logger->debug(
							"��ս %ld ���ڴ��� DARE_READY_ACTIVE ״̬�� �յ� ACTIVEDARE_SUCCESS", 								pDare->tempid);
						pDare->setActiveState();
					}
					break;
				default:
					break;
			}
		}
		else if (cmd->dwStatus == Cmd::Session::SCENE_ACTIVEDARE_FAIL)
		{
			switch (pDare->state)
			{
				case CDare::DARE_READY:
					{// �յ��۳���ս������ʧ�ܵ���Ϣ�������ս����״̬
						Zebra::logger->debug("��ս %ld ���ڴ��� DARE_READY ״̬���յ�ACTIVEDARE_FAIL", pDare->tempid);
						pDare->setOverState();
					}
					break;
				case CDare::DARE_READY_ACTIVE:
					{// �յ��۳�Ӧս������ʧ�ܵ���Ϣ�������˻���ս�߽�ҵ�״̬
						Zebra::logger->debug("��ս %ld ���ڴ��� DARE_READY_ACTIVE ״̬���յ�ACTIVEDARE_FAIL", pDare->tempid);
						pDare->setReturnGoldState();
					}
					break;
				default:
					break;

			}
		}
	}
	else
	{//�����ڸ�ս�����ս���ѱ�ϵͳȡ��
		Zebra::logger->error("ս��%ld�����ڣ�����ȡ��", cmd->dwWarID);
	}

}

/**
 * \brief ���������͹�����t_darePk_SceneSession����
 * \param cmd ����ָ��
 */

void CDareM::darePk_sceneSession(Cmd::Session::t_darePk_SceneSession* cmd)
{
	UserSession* pAtt = UserSessionManager::getInstance()->getUserByID(cmd->attID);
	UserSession* pDef = UserSessionManager::getInstance()->getUserByID(cmd->defID);
	CDare* pDare = NULL;

#ifdef _ZJW_DEBUG
	Zebra::logger->debug("��ս״̬��PK�Ƿ�");
#endif    

	if (pAtt==NULL || pDef==NULL)
	{
		Zebra::logger->debug("�û�ID��Ч");
		return;
	}

	pDare = findDareRecord(Cmd::UNION_DARE, pAtt->unionid, pDef->unionid);

	if (pDare)
	{
		pDare->addGrade(pAtt, pDef);

#ifdef _ZJW_DEBUG	
		Zebra::logger->debug(" union grade(%d,%d)", pDare->grade1, pDare->grade2);
#endif	
		pDare = NULL;
	}


	pDare = findDareRecord(Cmd::SCHOOL_DARE, pAtt->schoolid, pDef->schoolid);

	if (pDare)
	{
#ifdef _ZJW_DEBUG	
		Zebra::logger->debug("school grade(%d,%d)", pDare->grade1, pDare->grade2);
#endif	

		pDare->addGrade(pAtt, pDef);
		pDare = NULL;
	}

	pDare = findDareRecord(Cmd::SEPT_DARE, pAtt->septid, pDef->septid);

	if (pDare)
	{
#ifdef _ZJW_DEBUG	
		Zebra::logger->debug("sept grade(%d,%d)", pDare->grade1, pDare->grade2);
#endif	

		pDare->addGrade(pAtt, pDef);
	}
}

void CDareM::createDare_sceneSession(Cmd::Session::t_createDare_SceneSession* pCmd, std::vector<DWORD>& dare_list)
{
	if (findDareRecordByFull(pCmd->type, pCmd->relationID2, dare_list) != NULL) return;

	CDare* pDare = NULL;

	switch (pCmd->type)
	{
		case Cmd::SEPT_NPC_DARE:
			{
				pDare = new CDareSeptNpc(pCmd->active_time, pCmd->ready_time);
			}
			break;
		case Cmd::COUNTRY_FORMAL_DARE:
			{
				pDare = new CDareCountry(pCmd->active_time, pCmd->ready_time);
			}
			break;
		case Cmd::EMPEROR_DARE:
			{
				pDare = new CDareEmperor(pCmd->active_time, pCmd->ready_time);
			}
			break;
		case Cmd::COUNTRY_FORMAL_ANTI_DARE:
			{
				pDare = new CDareAntiCountry(pCmd->active_time, pCmd->ready_time);
			}
			break;
		case Cmd::UNION_CITY_DARE:
			{
				pDare = new CDareUnionCity(pCmd->active_time, pCmd->ready_time);
			}
			break;
		default:
			break;
	}

	if (pDare == NULL) return;

	pDare->type = pCmd->type;

	for (DWORD i=0; i<dare_list.size(); i++)
	{
		pDare->addFirstID(dare_list[i]);
	}
	
	pDare->setSecondID(pCmd->relationID2);

	pDare->count = 0;
	pDare->setReadyState();
	pDare->setActiveState();

	rwlock.wrlock();
	addEntry(pDare);
	rwlock.unlock();
	return;
}

void CDareM::createUnionCityDare_sceneSession(Cmd::Session::t_UnionCity_Dare_SceneSession* pCmd)
{
	CCity* pCity = CCityM::getMe().find(pCmd->dwToCountryID, pCmd->dwCityID); 
	CUnion* pUnion = CUnionM::getMe().getUnionByID(pCmd->dwFromUnionID);
	UserSession* pUser = UserSessionManager::getInstance()->getUserByID(pCmd->dwFromUserID);
	int need_money = 0;

	//�жϹ���ĳ����ǲ����������ĳ���
	if ( pCmd->dwToCountryID == PUBLIC_COUNTRY)
		need_money = CREATE_UNION_NEUTRAL_CITY_DARE_NEED_PRICE_MONEY;
	else
		need_money = (pCmd->dwCityID==KING_CITY_ID)?CREATE_UNION_KING_CITY_DARE_NEED_PRICE_MONEY:CREATE_UNION_CITY_DARE_NEED_PRICE_MONEY;

	if (pUser == NULL)
	{
		Zebra::logger->error("[���ս]:��ս���û��Ѳ����ҵ���");
		return;
	}

	if (pUnion==NULL)
	{
		if (pUser) 
		{
			Cmd::Session::t_dareGold_SceneSession send;
			send.dwUserID = pCmd->dwFromUserID;
			send.dwNum = need_money;
			send.dwWarID = 0;
			send.dwType = Cmd::Session::RETURN_DARE_GOLD;
			send.dwWarType = Cmd::UNION_CITY_DARE;
			
			if (pUser && pUser->scene) 
				pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

			Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(����û�н������)", pUser->name, 
					pUser->unionid, send.dwNum);

			pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "����û�н�����ᣬ���ܷ�����ս");
		}
		
		return;
	}
	
	if (pUnion && pUnion->master)
	{
		if (pUnion->master->id != pCmd->dwFromUserID)
		{
			if (pUser)
			{
				Cmd::Session::t_dareGold_SceneSession send;
				send.dwUserID = pCmd->dwFromUserID;
				send.dwNum = need_money;
				send.dwWarID = 0;
				send.dwType = Cmd::Session::RETURN_DARE_GOLD;
				send.dwWarType = Cmd::UNION_CITY_DARE;

				if (pUser && pUser->scene) 
					pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

				Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(�����ǰ���)", pUser->name, 
						pUser->unionid, send.dwNum);

				pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "ֻ�а������ܷ�������ս");
			}
			return;
		}
		else
		{
			if (pUnion->getActionPoint()<1)
			{
				if (pUser)
				{
					Cmd::Session::t_dareGold_SceneSession send;
					send.dwUserID = pCmd->dwFromUserID;
					send.dwNum = need_money;
					send.dwWarID = 0;
					send.dwType = Cmd::Session::RETURN_DARE_GOLD;
					send.dwWarType = Cmd::UNION_CITY_DARE;

					if (pUser && pUser->scene) 
						pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

					Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(�ж�������)", 
							pUser->name, pUser->unionid, send.dwNum);
					pUser->sendSysChat(Cmd::INFO_TYPE_MSG, "�ж������㣬���ܷ�����ս, ���Ұ�����Ա���˽��ж��������Ϣ");
				}
				return;
			}
			
			CCity* pAttCity = CCityM::getMe().findByUnionID(pUser->unionid); 
			if (pAttCity)
			{
				pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "����ӵ�г��У���������ս��");
				Cmd::Session::t_dareGold_SceneSession send;
				send.dwUserID = pCmd->dwFromUserID;
				send.dwNum = need_money;
				send.dwWarID = 0;
				send.dwType = Cmd::Session::RETURN_DARE_GOLD;
				send.dwWarType = Cmd::UNION_CITY_DARE;
				
				Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(��ӵ�г���)", 
							pUser->name, pUser->unionid, send.dwNum);

				if (pUser && pUser->scene) 
					pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

				return;
			}
			
			if (pUnion->dwCountryID != pCmd->dwToCountryID && pCmd->dwCityID==KING_CITY_ID)
			{
				pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "�����ܶ�ȡ�������ǡ�");
				Cmd::Session::t_dareGold_SceneSession send;
				send.dwUserID = pCmd->dwFromUserID;
				send.dwNum = need_money;
				send.dwWarID = 0;
				send.dwType = Cmd::Session::RETURN_DARE_GOLD;
				send.dwWarType = Cmd::UNION_CITY_DARE;
				
				Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(������ս��������)", 
							pUser->name, pUser->unionid, send.dwNum);

				if (pUser && pUser->scene) 
					pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

				return;
			}
		}
	}
	else
	{
		if (pUser) 
		{
			Cmd::Session::t_dareGold_SceneSession send;
			send.dwUserID = pCmd->dwFromUserID;
			send.dwNum = need_money;
			send.dwWarID = 0;
			send.dwType = Cmd::Session::RETURN_DARE_GOLD;
			send.dwWarType = Cmd::UNION_CITY_DARE;
	
			if (pUser && pUser->scene) 
				pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

			Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(��᲻����)", pUser->name, 
					pUser->unionid, send.dwNum);
			pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "��᲻����");
		}
		
		return;
	}
                         
	struct tm tv1;
	time_t timValue = time(NULL);
	zRTime::getLocalTime(tv1, timValue);

#ifndef _ALL_SUPER_GM
	if (tv1.tm_hour<18 || (tv1.tm_hour>21 && tv1.tm_min>30))
	{
#endif		
		if (pCity == NULL)
		{
			if (pUser && CCityM::getMe().addNewCity(pCmd))
			{//����µ�ӵ���ߣ����Ϳ�Ǯ��Ϣ������
				Zebra::logger->trace("[���ս]: %s �ѻ�øó�����Ȩ", pUser->name);
				pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "���ѻ�øóǵ�����Ȩ");
				return;
			}
		}
		else
		{	if (pCmd->dwFromUnionID == pCity->dwUnionID)
			{//�жϸó��Ƿ��������Լ�
				if (pUser) pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "�������Լ����������ս");
				Cmd::Session::t_dareGold_SceneSession send;
				send.dwUserID = pCmd->dwFromUserID;
				send.dwNum = need_money;
				send.dwWarID = 0;
				send.dwType = Cmd::Session::RETURN_DARE_GOLD;
				send.dwWarType = Cmd::UNION_CITY_DARE;
				
				if (pUser && pUser->scene) 
				{
					pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

					Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(�������Լ����������ս)", 
							pUser->name, pUser->unionid, send.dwNum);
				}
				return;
			}

			// �жϽ����Ƿ�����ս����
			if (CCityM::getMe().findDareUnionByID(pUser->unionid) != NULL)
			{
				if (pUser) pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "��������ս��İ��");

				Cmd::Session::t_dareGold_SceneSession send;
				send.dwUserID = pCmd->dwFromUserID;
				send.dwNum = need_money;
				send.dwWarID = 0;
				send.dwType = Cmd::Session::RETURN_DARE_GOLD;
				send.dwWarType = Cmd::UNION_CITY_DARE;
				
				if (pUser && pUser->scene) 
				{
					pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));
				
					Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(��������ս��İ��)", 
							pUser->name, pUser->unionid, send.dwNum);
				}

				return;
			}
			
			// �ж�ӵ�����Ƿ���Ч,�п�����ӵ���ڼ䣬��ɢ�˰��
			CUnion* pOwn = CUnionM::getMe().getUnionByID(pCity->dwUnionID);
			if (pOwn == NULL)
			{
				if (pCity)
				{
					pCity->changeUnion(pCmd->dwFromUnionID);
					if (pCity->dwCityID == KING_CITY_ID) // ��˫��
					{//����ʳǸı��˰�ᣬ��ͬ�����¹�����Ϣ
						CCountry* pCountry = CCountryM::getMe().find(pUser->country);
						if (pCountry)
						{
							pCountry->changeKing(pUser);
						}
					}
					
					if (pUser) pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "������˸óǵ�����Ȩ");
				}
				return;
			}
			
			// �ó��Ѿ������ӵ�У�������ս
			if (pCity->addDareList(pCmd->dwFromUnionID))
			{
				pCity->writeDatabase();
				pUnion->changeActionPoint(-1);
				
				if (pOwn && pUnion)
				{
					Zebra::logger->trace("[���ս]:��ս�ɹ�:%s �� %s ��19:30�ֿ�ʼ���ս", 
							pUnion->name, pOwn->name);
					
					
					pOwn->sendUnionNotify(
					"ϵͳ���棺%s ��� %s ������ ���� ��ս�����ڽ���\n19��30��ʼ����׼ʱ�μӡ�",   
		    					pUnion->name, pOwn->name);

					pUnion->sendUnionNotify(
					"ϵͳ���棺%s ��� %s ������ ���� ��ս�����ڽ���\n19��30��ʼ����׼ʱ�μӡ�",
							pUnion->name, pOwn->name);

					if (pOwn->dwCountryID != pUnion->dwCountryID)
					{
						SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_GAME,
								pOwn->dwCountryID, "%s �� %s �����ҹ� %s ������ս",
								CCountryM::getMe().find(pUnion->dwCountryID)->name,
								pUnion->name, pCity->getName());
							
						SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_GAME,
								pUnion->dwCountryID, "�ҹ� %s ���� %s�� %s �����˶��ս",
							pUnion->name,CCountryM::getMe().find(pOwn->dwCountryID)->name,
								pCity->getName());
					}
				}
			}
			else
			{
				if (pUser) 
				{
					Cmd::Session::t_dareGold_SceneSession send;
					send.dwUserID = pCmd->dwFromUserID;
					send.dwNum = need_money;
					send.dwWarID = 0;
					send.dwType = Cmd::Session::RETURN_DARE_GOLD;
					send.dwWarType = Cmd::UNION_CITY_DARE;
						
					if (pUser && pUser->scene) 
						pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

					Zebra::logger->trace("���Ӹ��٣�%s(%d) ���� %d ����ս��(�ѽ������������ս)", 
							pUser->name, pUser->unionid, send.dwNum);

					pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "�ѽ������������ս");
				}
				
				return;
			}
		}
		
#ifndef _ALL_SUPER_GM
	}
	else
	{
		if (pUser) 
		{
			Cmd::Session::t_dareGold_SceneSession send;
			send.dwUserID = pCmd->dwFromUserID;
			send.dwNum = need_money;
			send.dwWarID = 0;
			send.dwType = Cmd::Session::RETURN_DARE_GOLD;
			send.dwWarType = Cmd::UNION_CITY_DARE;
	
			if (pUser && pUser->scene) 
				pUser->scene->sendCmd(&send, sizeof(Cmd::Session::t_dareGold_SceneSession));

			pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "����21��30�ֵ��ڶ���18��֮������ս");
			return;
		}
	}
#endif	
}

bool CDareM::processSceneMessage(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen)
{
	if (cmd)
	{
		switch (cmd->para)
		{
			case Cmd::Session::PARA_BEGIN_EMPEROR_DARE:
				{
					CCountry* pCountry = CCountryM::getMe().find(NEUTRAL_COUNTRY_ID);
					
					if (pCountry)
					{
						pCountry->beginDare();
					}
					return true;
				}
				break;
			case Cmd::Session::PARA_GM_CREATE_UNION_CITY:
				{
					Cmd::Session::t_GMCreateUnionCity_SceneSession* rev = 
						(Cmd::Session::t_GMCreateUnionCity_SceneSession*)cmd;
					
					if (rev->byOpType==1)
					{
						CCity* pCity = CCityM::getMe().find(rev->dwCountryID, rev->dwCityID);
						if (pCity) pCity->beginDare();
					}
				}
				break;
			case Cmd::Session::PARA_ACTIVEDARE:
				{
					activeDare_sceneSession((Cmd::Session::t_activeDare_SceneSession*)cmd);
					return true;
				}

				break;
			case Cmd::Session::PARA_DARE_PK:
				{
					darePk_sceneSession((Cmd::Session::t_darePk_SceneSession*)cmd);
					return true;
				}
				break;
			case Cmd::Session::PARA_UNION_CITY_DARE:
				{
					createUnionCityDare_sceneSession((Cmd::Session::t_UnionCity_Dare_SceneSession*)cmd);
					return true;
				}
				break;
			case Cmd::Session::PARA_UNION_CITY_DARE_RESULT:
				{
					Cmd::Session::t_UnionCity_DareResult_SceneSession* rev = 
						(Cmd::Session::t_UnionCity_DareResult_SceneSession*)cmd;
	
					UserSession *pUser = UserSessionManager::getInstance()->getUserByID(rev->dwUserID);
					CDare* pDare = findDareRecordByID(Cmd::UNION_CITY_DARE, rev->dwUnionID);
					
					if (pDare)
					{
						pDare->grade1 = 1;
						pDare->dwWinnerID= rev->dwUnionID;
						pDare->setReadyOverState();
					}
						
					CCity* pCity = NULL;
					
					if (rev->dwCityID >1000)
					{
						pCity = CCityM::getMe().find(rev->dwCountryID, rev->dwCityID/10);
					}
					else
					{
						pCity = CCityM::getMe().find(rev->dwCountryID, rev->dwCityID);
					}

					if (pCity)
					{
						pCity->changeUnion(rev->dwUnionID);
						if (pCity->dwCityID == KING_CITY_ID && pUser) // ��˫��
						{//������Ǹı��˰�ᣬ��ͬ�����¹�����Ϣ
							CCountry* pCountry = CCountryM::getMe().find(pUser->country);
							if (pCountry)
							{
								pCountry->changeKing(pUser);
							}
						}
												
						if (pUser) pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "������˸óǵ�����Ȩ");
					}

					return true;
				}
				break;
			case Cmd::Session::PARA_CHECK_CALLTIMES_LEADER:
				{
					Cmd::Session::t_Check_CallTimes_SceneSession *rev = (Cmd::Session::t_Check_CallTimes_SceneSession*)cmd;
					UserSession *leader = UserSessionManager::getInstance()->getUserByTempID(rev->leaderTempID);
					if(leader)
					{
						Cmd::Session::t_Return_CallTimes_SceneSession ret;
						ret.type = rev->type;
						ret.leaderTempID = rev->leaderTempID;
						ret.qwThisID = rev->qwThisID;
						ret.times=0;
						
						switch(rev->type)
						{
							case Cmd::CALL_DUTY_KING:
								{
									if(CCountryM::getMe().isKing(leader))
									{
										CCountry* pCountry = CCountryM::getMe().find(leader->country);
										if(pCountry)
										{
											if(pCountry->calltimes < 2)
											{
												ret.times=2-pCountry->calltimes;
												pCountry->calltimes++;
												pCountry->writeDatabase();
											}
										}
									}
								}
								break;
							case Cmd::CALL_DUTY_UNION:
								{

									CUnion* pUnion = CUnionM::getMe().getUnionByID(leader->unionid);
									if (pUnion && pUnion->tempid == leader->id)
									{
										if(pUnion->calltimes < 2)
										{
											ret.times=2-pUnion->calltimes;
											pUnion->calltimes++;
											pUnion->writeDatabase();
										}
									}
								}
								break;
							case Cmd::CALL_DUTY_SEPT:
								{
									CSept* pSept = CSeptM::getMe().getSeptByID(leader->septid);
									if (pSept && pSept->tempid == leader->id)
									{
										struct tm tv1,tv2;
										zRTime::getLocalTime(tv1,SessionTimeTick::currentTime.sec());
										zRTime::getLocalTime(tv2, pSept->calldaytime);
										if(tv1.tm_mday == tv2.tm_mday)
										{
											if(pSept->calltimes < 5)
											{
												ret.times=5-pSept->calltimes;
												pSept->calltimes++;
												pSept->writeDatabase();
											}
										}
										else
										{
											pSept->calldaytime = SessionTimeTick::currentTime.sec();
											pSept->calltimes = 0;
											ret.times=5-pSept->calltimes;
											pSept->calltimes ++;
											pSept->writeDatabase();
										}
									}
								}
								break;
						}
						leader->scene->sendCmd(&ret,sizeof(ret));
					}
					return true;
				}
				break;
			case Cmd::Session::PARA_GOTO_LEADER:
				{
					Cmd::Session::t_GoTo_Leader_SceneSession * rev =(Cmd::Session::t_GoTo_Leader_SceneSession *)cmd;
					UserSession *leader = UserSessionManager::getInstance()->getUserByTempID(rev->leaderTempID);
					if(leader)
					{
						Cmd::Session::t_GoTo_Leader_Check_SceneSession ret;
						ret.leaderTempID = leader->tempid;
						ret.type = rev->type;
						strncpy(ret.mapName,rev->mapName,sizeof(ret.mapName));
						ret.x = rev->x;
						ret.y = rev->y;
						switch(rev->type)
						{
							case Cmd::CALL_DUTY_KING:
								{
									struct execAll : public execEntry<CUnion>
									{
										execAll(Cmd::Session::t_GoTo_Leader_Check_SceneSession &cmd,DWORD country)
										{
											_ret.leaderTempID = cmd.leaderTempID;
											_ret.type = cmd.type;
											strncpy(_ret.mapName,cmd.mapName,sizeof(_ret.mapName));
											_ret.x = cmd.x;
											_ret.y = cmd.y;
											_country = country;
										}
										Cmd::Session::t_GoTo_Leader_Check_SceneSession _ret;
										DWORD _country;
										bool exec(CUnion *u)
										{
											UserSession *pUser=NULL; 
											if (u && u->master)
											{
												pUser= UserSessionManager::getInstance()->getUserByID(u->master->id);
											}
											if(pUser && pUser->country == _country)
											{
												_ret.userTempID = pUser->tempid;
												pUser->scene->sendCmd(&_ret,sizeof(_ret));
											}
											return true;
										}
									};
									execAll exec(ret,leader->country);
									CUnionM::getMe().execEveryOne(exec);
								}
								break;
							case Cmd::CALL_DUTY_UNION:
								{
									CUnion* pUnion = CUnionM::getMe().getUnionByID(leader->unionid);
									if (pUnion)
									{
										pUnion->sendCallCmdToAllMemberScene(&ret,sizeof(ret),leader->tempid);
									}
								}
								break;
							case Cmd::CALL_DUTY_SEPT:
								{
									
									CSept* pSept = CSeptM::getMe().getSeptByID(leader->septid);
									if (pSept)
									{
										pSept->sendCallCmdToAllMemberScene(&ret,sizeof(ret),leader->tempid);
									}
								}
								break;
							default:
								{
								}
								break;
						}
					}
					else
					{
						Zebra::logger->debug("ʹ������ʱ�Ҳ����쵼");
					}
					return true;
				}
				break;
			case Cmd::Session::PARA_RESET_CALLTIMES_LEADER:
				{
					Cmd::Session::t_Reset_CallTimes_SceneSession *rev = (Cmd::Session::t_Reset_CallTimes_SceneSession*)cmd;
					UserSession *leader = UserSessionManager::getInstance()->getUserByTempID(rev->leaderTempID);
					if(leader)
					{
						CCountry* pCountry = CCountryM::getMe().find(leader->country);
						if(pCountry)
						{
							pCountry->calltimes=0;
							pCountry->writeDatabase();
						}
						CUnion* pUnion = CUnionM::getMe().getUnionByID(leader->unionid);
						if (pUnion && pUnion->tempid == leader->id)
						{
							pUnion->calltimes=0;
							pUnion->writeDatabase();
						}
						CSept* pSept = CSeptM::getMe().getSeptByID(leader->septid);
						if (pSept && pSept->tempid == leader->id)
						{
							pSept->calldaytime = SessionTimeTick::currentTime.sec();
							pSept->calltimes = 0;
							pSept->writeDatabase();
						}
					}
					return true;
				}
			default:
				break;

		}

	}

	return false;
}

void CDare::sendActiveStateToScene(UserSession* pUser)
{
	rwlock.wrlock();
	this->state = CDare::DARE_ACTIVE;
	rwlock.unlock();
}

void CDareM::userOnlineCountry(UserSession* pUser)
{
	struct execAll : public execEntry<CDare>
	{
		UserSession* u;
		execAll(UserSession* pUser) : u(pUser) {}
		
		bool exec(CDare *pDare)
		{
			if ((pDare->type == Cmd::COUNTRY_FORMAL_DARE 
					|| pDare->type == Cmd::COUNTRY_FORMAL_ANTI_DARE)
					&& (pDare->secondID== u->country || pDare->isAtt(u->country)))
			{
				pDare->sendActiveStateToScene(u);
			}

			return true;
		}
	};

	execAll myList(pUser);
	execEveryOne(myList);
}

void CDareM::userOnline(UserSession* pUser)
{// �жϸ��û��Ƿ��ڽ�ս״̬������ǣ������������ͽ�ս��¼
	CDare* pDare = NULL;

	/*if (pUser->unionid<=0 && pUser->schoolid<=0 && pUser->septid<=0)
	{
		return;
	}*/

	if (pUser->unionid>0 && ((pDare = findDareRecordByID(Cmd::UNION_DARE, pUser->unionid)) != NULL) )
	{
		if (pDare->state == CDare::DARE_ACTIVE)
		{
			pDare->sendActiveStateToScene(pUser);
		}
		else if (pDare->state == CDare::DARE_READY_QUESTION)
		{
			pDare->setReadyQuestionState();
		}
		else if (pDare->state == CDare::DARE_WAIT_BOUNTY)
		{
			pDare->setWaitBountyState();
		}
		else if (pDare->state == CDare::DARE_RETURN_GOLD)
		{
			pDare->setReturnGoldState();
		}

		pDare = NULL;
	}

	/*if (pUser->schoolid>0 && ((pDare = findDareRecordByID(Cmd::SCHOOL_DARE, pUser->schoolid)) != NULL))
	{
		if (pDare->state == CDare::DARE_ACTIVE)
		{
			pDare->sendActiveStateToScene(pUser);
		}
		else if (pDare->state == CDare::DARE_READY_QUESTION)
		{
			pDare->setReadyQuestionState();
		}
		else if (pDare->state == CDare::DARE_WAIT_BOUNTY)
		{
			pDare->setWaitBountyState();
		}
		else if (pDare->state == CDare::DARE_RETURN_GOLD)
		{
			pDare->setReturnGoldState();
		}

		pDare = NULL;
	}*/

	if (pUser->septid>0 && ((pDare = findDareRecordByID(Cmd::SEPT_DARE, pUser->septid)) != NULL))
	{
		if (pDare->state == CDare::DARE_ACTIVE)
		{
			pDare->sendActiveStateToScene(pUser);
		}
		else if (pDare->state == CDare::DARE_READY_QUESTION)
		{
			pDare->setReadyQuestionState();
		}
		else if (pDare->state == CDare::DARE_WAIT_BOUNTY)
		{
			pDare->setWaitBountyState();
		}
		else if (pDare->state == CDare::DARE_RETURN_GOLD)
		{
			pDare->setReturnGoldState();
		}

		pDare = NULL;
	}

	if (pUser->unionid>0 && ((pDare = findDareRecordByID(Cmd::UNION_CITY_DARE, pUser->unionid)) != NULL))
	{
		if (pDare->state == CDare::DARE_ACTIVE)
		{
			pDare->sendActiveStateToScene(pUser);
		}
		pDare = NULL;
	}

	/*struct onlineCallback : public CCityM::cityCallback
	{
		UserSession* _pUser;
		onlineCallback(UserSession* pUser) : _pUser(pUser){}
		
		void exec(CCity * pCity)
		{
			if (pCity && _pUser && (pCity->dwUnionID == _pUser->unionid) && pCity->dwDareUnionID>0)
			{
				if (pCity->getName()!=NULL)
				{
					_pUser->sendSysChat(Cmd::INFO_TYPE_EXP, 
							"��%s �����ս����20:30�ֿ�ʼ����׼ʱ�μ�",
							pCity->getName());
				}
			}
		}
	};

	if (pUser->unionid>0)
	{
		onlineCallback oc(pUser);
		CCityM::getMe().execEveryCity(oc);
	}*/

	if (pUser->unionid>0)
	{
		CCity* pCity = CCityM::getMe().findByUnionID(pUser->unionid);

		if (pCity && pCity->dareSize()>0)
		{
			pUser->sendSysChat(Cmd::INFO_TYPE_EXP, "�������ս����19:30�ֿ�ʼ����׼ʱ�μ�");
		}
		else
		{
			pCity = CCityM::getMe().findDareUnionByID(pUser->unionid);
			if (pCity)
			{
				pUser->sendSysChat(Cmd::INFO_TYPE_EXP, "�������ս����19:30�ֿ�ʼ����׼ʱ�μ�");
			}
		}
	}

	CCountry* pCountry = CCountryM::getMe().find(pUser->country);
	if (pCountry && pCountry->dwDareCountryID>0)
	{
		CCountry* pAttCountry = CCountryM::getMe().find(pCountry->dwDareCountryID);
		if (pAttCountry)
		{
			struct tm tv1, tv2;
			time_t timValue = time(NULL);
			zRTime::getLocalTime(tv1, pAttCountry->dwDareTime);
			zRTime::getLocalTime(tv2, timValue);

			if (tv1.tm_mday!=tv2.tm_mday)
				pUser->sendSysChat(Cmd::INFO_TYPE_EXP, "%s ���ڽ���20:40�ֹ����ҹ��������÷���׼��", pAttCountry->name);
			else
				pUser->sendSysChat(Cmd::INFO_TYPE_EXP, "%s ��������20:40�ֹ����ҹ��������÷���׼��", pAttCountry->name);
		}
	}
	
	CCountry* pAttCountry = CCountryM::getMe().findByDare(pUser->country);
	if (pAttCountry)
	{
		CCountry* pDefCountry = CCountryM::getMe().findByDare(pUser->country, false);
		if (pDefCountry)
		{
			struct tm tv1, tv2;
			time_t timValue = time(NULL);
			zRTime::getLocalTime(tv1, pDefCountry->dwLastDareTime);
			zRTime::getLocalTime(tv2, timValue);

			if (tv1.tm_mday!=tv2.tm_mday)
				pUser->sendSysChat(Cmd::INFO_TYPE_EXP, "�ҹ����ڽ���20:40�ֹ��� %s�������ý���׼��", pDefCountry->name);
			else
				pUser->sendSysChat(Cmd::INFO_TYPE_EXP, "�ҹ���������20:40�ֹ��� %s�������ý���׼��", pDefCountry->name);
		}
	}
	
	if (CDareM::getMe().findDareRecord(Cmd::EMPEROR_DARE, 6, 0) != NULL)
	{
		Cmd::stEnterEmperorDareActive send;
		pUser->sendCmdToMe(&send, sizeof(send));
	}
}

bool CDareM::processUserMessage(UserSession *pUser,const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen)
{
	switch (ptNullCmd->byParam)
	{
		case Cmd::ACTIVE_DARE_PARA:
			{
				Cmd::stActiveDareCmd *pCmd = (Cmd::stActiveDareCmd *)ptNullCmd;
#ifdef _ZJW_DEBUG							
				Zebra::logger->debug("���ܵ� %s ��ACTIVE_DARE����", pUser->name);
#endif							
				if (pCmd->dwMsgType == Cmd::DARE_QUESTION)
				{
					if (strcmp(pCmd->fromRelationName, 
								pCmd->toRelationName) == 0)
					{// ��ֹ���Լ�������ս
						if (pUser)
						{
							pUser->sendSysChat(
									Cmd::INFO_TYPE_FAIL, 
									"�������Լ�������ս");
						}
						return true;
					}

					if (findDareRecordByRelationName(pCmd->dwDareType, 
								pCmd->fromRelationName, NULL) != NULL) 
					{
						if (pUser)
						{
							pUser->sendSysChat(
									Cmd::INFO_TYPE_FAIL, 
									"���Ѵ��ڶ�ս״̬");
						}

						return true;
					}

					if (findDareRecordByRelationName(pCmd->dwDareType, 
								pCmd->toRelationName, NULL) != NULL) 
					{
						if (pUser)
						{
							pUser->sendSysChat(
									Cmd::INFO_TYPE_FAIL, 
									"�Է��Ѵ��ڶ�ս״̬");
						}

						return true;
					}

					switch (pCmd->dwDareType)
					{
						case Cmd::SCHOOL_DARE:
							{
								CSchool *pSchool1 = (CSchool *)CSchoolM::
									getMe().getSchoolByName
									(pCmd->fromRelationName);

								CSchool *pSchool2 = (CSchool *)CSchoolM::
									getMe().getSchoolByName
									(pCmd->toRelationName);

								Cmd::Session::t_dareGold_SceneSession 
									ready_send;

								if (pSchool1 == NULL || pSchool2 == NULL)
								{
									pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "��Ч��ʦ��ս��ս���룡");
									return true;
								}

								if (pSchool1->getMasterNode()== NULL)
								{
#ifdef _ZJW_DEBUG											    
									Zebra::logger->debug("ʦ�� %s û��ʦ��", pCmd->fromRelationName);
#endif											       
									return true;
								}

								if (pSchool2->getMasterNode() == NULL)
								{
#ifdef _ZJW_DEBUG											    
									Zebra::logger->debug("ʦ�� %s û��ʦ��",
											pCmd->toRelationName);
#endif
									return true;												
								}


								if (strncmp(pSchool1->getMasterNode()->name,
											pCmd->name, MAX_NAMESIZE)!=0) 
									return true;

								if (addNewDareRecord(pCmd, Cmd::SCHOOL_DARE,
											pSchool1->id, pSchool2->id, 
											pUser->id))
								{
									ready_send.dwWarID = pCmd->dwWarID;
									ready_send.dwUserID = pUser->id;
									ready_send.dwNum = (-dare_need_gold);
									if (pUser->scene->sendCmd(&ready_send, sizeof(Cmd::Session::t_dareGold_SceneSession)))
									{
#ifdef _ZJW_DEBUG													
										Zebra::logger->debug("���Ϳ�Ǯ�ɹ�");
#endif													
									}


								}
								else
								{
									pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�޷�������ս");
								}
							}
							break;
						case Cmd::SEPT_DARE:
							{
								return processSeptDare(pUser, pCmd, cmdLen);	
							}
							break;
						case Cmd::UNION_DARE:
							{
								return processUnionDare(pUser, pCmd, cmdLen);
							}	
							break;
					}
					return true;
				}

				if (pCmd->dwMsgType == Cmd::DARE_YES)
				{
					// TODO:������ս����Scene������Ϣ���۳���Ӧ����������
					//      �۳��ɹ���Scene��һ����Ϣ��ȷ��ĳ��ս��ID��Ǯ�ɹ���
#ifdef _ZJW_DEBUG								
					Zebra::logger->debug("%s ͬ����ս��", pUser->name);
#endif								
					CDare* pDare = (CDare*)CDareM::getMe().getEntryByTempID(pCmd->dwWarID);
					if (pDare && ((pDare->userid2<=0 
									&& pDare->type==Cmd::UNION_DARE) 
								|| pDare->type!=Cmd::UNION_DARE)
					   )
					{
						pDare->setReadyActiveState(pUser);
					}
					else
					{
						pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, 
								"�ȴ���ʱ����ս��ȡ��,�����ɱ��˽�����ս");
					}

					return true;
				}

				if (pCmd->dwMsgType == Cmd::DARE_NO)
				{
					CDare* pDare = (CDare*)CDareM::getMe().getEntryByTempID(
							pCmd->dwWarID);

					if (pDare)
					{
						if (pDare->type != Cmd::UNION_DARE) {
							UserSession *pFromUser = UserSessionManager::getInstance()->
								getUserByID(pDare->userid1);
#ifdef _ZJW_DEBUG								
							Zebra::logger->debug("%s �ܾ���ս��", pUser->name);
#endif								

							if (pFromUser)
							{
								pFromUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "��ս���ܾ�");
							}

							pDare->setReturnGoldState();
						}

						pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, 
								"���ܾ�����ս");
					}
					else
					{
						pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, 
								"�ȴ���ʱ����ս��ȡ��");

					}

					return true;
				}
			}
			break;
		case Cmd::QUERY_DARE_LIST_PARA:
			{
				Cmd::stQueryDareList *pCmd = (Cmd::stQueryDareList *)ptNullCmd;

				if (pCmd->byType == Cmd::UNION_DARE)
				{
					CUnionM::getMe().sendDareListToUser(pUser);
				}
				else if (pCmd->byType == Cmd::SEPT_DARE)
				{
					CSeptM::getMe().sendDareListToUser(pUser);
				}

				return true;
			}
			break;
		case Cmd::GET_UNION_CITY_TAX_PARA:
			{
				CCityM::getMe().awardTaxGold(pUser);
				return true;
			}
			break;
		case Cmd::DARE_CALL_DUTY:
			{
				Cmd::stDareCallDuty *pCmd = (Cmd::stDareCallDuty *)ptNullCmd;
				UserSession *leader = UserSessionManager::getInstance()->getUserByTempID(pCmd->leaderTempID);
				if(leader)
				{
					if(pCmd->byPipeline == Cmd::PIPELINE_AGREE)
					{
						Cmd::Session::t_GoTo_Leader_SceneSession ret;
						ret.leaderTempID = pUser->tempid;//pCmd->leaderTempID;
						strncpy(ret.mapName,pCmd->mapName,sizeof(ret.mapName));
						ret.x=pCmd->x;
						ret.y=pCmd->y;
						pUser->scene->sendCmd(&ret,sizeof(ret));
					}
					else
					{
						leader->sendSysChat(Cmd::INFO_TYPE_GAME, "%s�ܾ�����ٻ�",pUser->name);
					}
				}
				else
				{
					Zebra::logger->debug("����������ʱδ�ҵ��쵼:%d",pCmd->leaderTempID);
				}
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

bool CDareM::processSeptDare(UserSession *pUser, Cmd::stActiveDareCmd *pCmd, const unsigned int cmdLen)
{
	CSept *pSept1 = (CSept *)CSeptM::getMe().getSeptByName(pCmd->fromRelationName);
	CSept *pSept2 = (CSept *)CSeptM::getMe().getSeptByName(pCmd->toRelationName);

	int vDareRepute[] = {0, 1, 5, 10, 20};

	if (pSept1 == NULL || pSept2 == NULL)
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "��Ч�ļ���ս��ս���룡");
		return true;
	}

	if (pSept1->master == NULL)
	{
#ifdef _ZJW_DEBUG											    
		Zebra::logger->debug("���� %s û���峤", pCmd->fromRelationName);
#endif											       
		return true;
	}

	if (pSept2->master == NULL)
	{
#ifdef _ZJW_DEBUG											    
		Zebra::logger->debug("���� %s û���峤",
				pCmd->toRelationName);
#endif											    
		return true;
	}

	if (strncmp(pSept1->master->name, 
				pCmd->name, MAX_NAMESIZE)!=0) 
		return true;

	
	if (pCmd->byDareRepute<1 || pCmd->byDareRepute>4  
			|| ((int)pSept1->getRepute() < vDareRepute[pCmd->byDareRepute]) || ((int)pSept2->getRepute() < vDareRepute[pCmd->byDareRepute]))
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "���������Է��������㣬��սʧ��");
		return true;
	}

	if (addNewDareRecord(pCmd, Cmd::SEPT_DARE, 
				pSept1->id, pSept2->id, 
				pUser->id))
	{
		pSept1->changeRepute(-vDareRepute[pCmd->byDareRepute]);
		CDare* pDare = findDareRecord(Cmd::SEPT_DARE, pSept1->id, pSept2->id);
		((CDareSept*)pDare)->dwDareRepute = pCmd->byDareRepute;
		 pDare->setReadyQuestionState();
	}
	else
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�޷�������ս");
	}

	return true;
}

bool CDareM::processUnionDare(UserSession *pUser, Cmd::stActiveDareCmd *pCmd, const unsigned int cmdLen)
{
	CUnion* pUnion1 = NULL;
	CUnion* pUnion2 = NULL;
	Cmd::Session::t_dareGold_SceneSession ready_send;
#ifdef _ZJW_DEBUG										       
	Zebra::logger->debug("%s ��������ս.", pUser->name);
	Zebra::logger->debug("���� %s ���ڰ��", pCmd->fromRelationName);
#endif											
	pUnion1 = (CUnion *)CUnionM::getMe().getUnionByName(pCmd->fromRelationName);
	if (pUnion1 == NULL)
	{
		Zebra::logger->debug("%s ��᲻���ڡ�",
				pCmd->fromRelationName);
		return true;
	}

	pUnion2 = (CUnion *)CUnionM::getMe().getUnionByName(pCmd->toRelationName);
	if (pUnion2 == NULL)
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "%s ��᲻����", pCmd->toRelationName);
		Zebra::logger->debug("%s ��᲻����", pCmd->toRelationName);
		return true;
	}

	if (pUnion1 == NULL || pUnion2 == NULL)
	{
		pUser->sendSysChat(
				Cmd::INFO_TYPE_FAIL, 
				"��Ч�İ��ս��ս���룡");
		return true;
	}

	if (pUnion1->master == NULL)
	{
#ifdef _ZJW_DEBUG											    
		Zebra::logger->debug("��� %s û�а���", pCmd->fromRelationName);
#endif											       
		return true;
	}

	if (pUnion2->master == NULL)
	{
#ifdef _ZJW_DEBUG											    
		Zebra::logger->debug("��� %s û�а���",
				pCmd->toRelationName);
#endif											    
	}


	if (!CUnionM::getMe().havePowerByName(pCmd->name, Cmd::DARE)) 
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL,
				"����Ȩ������ս");
		return true;
	}

	if (pUnion1 && 
			pUnion1->getActionPoint()<1)
	{
		pUser->sendSysChat(
				Cmd::INFO_TYPE_MSG, 
				"����ж������㣬���ܷ�����ս, ���Ұ�����Ա���˽��ж��������Ϣ");
		return true;
	}
	pUnion1->changeActionPoint(-1);

	if (addNewDareRecord(pCmd, 
				Cmd::UNION_DARE, pUnion1->id, 
				pUnion2->id, pUser->id))
	{
		ready_send.dwWarID = pCmd->dwWarID;
		ready_send.dwUserID = pUser->id;
		ready_send.dwNum = (-dare_need_gold);
		ready_send.dwType = Cmd::Session::DARE_GOLD;
		ready_send.dwWarType = Cmd::UNION_DARE;

		if (pUser->scene->sendCmd(&ready_send, sizeof(Cmd::Session::t_dareGold_SceneSession)))
		{

			Zebra::logger->trace("���Ӹ��٣�%s(%d) �۳� %d ����ս��", pUser->name, pUser->unionid, ready_send.dwNum);
#ifdef _ZJW_DEBUG													
			Zebra::logger->debug("���Ϳ�Ǯ�ɹ�");
#endif													
		}
	}
	else
	{
		pUser->sendSysChat(Cmd::INFO_TYPE_FAIL, "�޷�������ս");
	}

	return true;
}

void  CDareM::timer()
{
	struct execAll : public execEntry<CDare>
	{
		std::vector<CDare *> _removeList;

		execAll(){}

		void removeList()
		{
			std::vector<CDare *>::iterator tIterator;
			for (tIterator = _removeList.begin(); tIterator != _removeList.end(); tIterator++)
			{
				CDare *cd = *tIterator;
				CDareM::getMe().removeEntry(cd);
				SAFE_DELETE(cd);
			}
			_removeList.clear();
		}

		~execAll(){}

		bool exec(CDare *pDare)
		{
			pDare->updateTime();
			//unsigned int active_time = atoi(Zebra::global["dare_active_time"].c_str());
			//unsigned int ready_time  = atoi(Zebra::global["dare_ready_time"].c_str());
			
			if (pDare->type == Cmd::UNION_CITY_DARE)
			{
				if (!pDare->isActivePeriod() && pDare->state == CDare::DARE_ACTIVE)
				{
					pDare->setReadyOverState();
				}
			}
			else if (pDare->type == Cmd::SEPT_NPC_DARE 
				|| pDare->type == Cmd::COUNTRY_FORMAL_DARE 
				|| pDare->type == Cmd::COUNTRY_FORMAL_ANTI_DARE
				|| pDare->type == Cmd::EMPEROR_DARE)
			{
				if (!pDare->isActivePeriod() && pDare->state == CDare::DARE_ACTIVE)
				{
					// TODO ��ս��������
					pDare->setReadyOverState();
				}
			}
			else
			{
				pDare->timer();

				if (!pDare->isReadyPeriod() && (pDare->state == CDare::DARE_READY))
				{
					// TODO ɾ��������ڵĽ�ս��¼
					pDare->setReturnGoldState();
					//_removeList.push_back(pDare);
				}
				else if (!pDare->isReadyPeriod() && (pDare->state == CDare::DARE_READY_QUESTION))
				{
					pDare->setReturnGoldState();
				}
				else if (!pDare->isActivePeriod() && pDare->state == CDare::DARE_ACTIVE)
				{
					// TODO ��ս��������
					pDare->setReadyOverState();
				}
			}

			if (pDare->state == CDare::DARE_OVER)
			{
				// TODO ��������
				_removeList.push_back(pDare);
			}

			return true;
		}
	};

	//rwlock.wrlock();
	execAll myList;
	execEveryOne(myList);
	myList.removeList();
	//rwlock.unlock();
}

bool CDareM::addNewDareRecord(Cmd::stActiveDareCmd *pCmd, DWORD dwType, DWORD dwID1, DWORD dwID2, DWORD dwUserID)
{
	if (findDareRecord(dwType, dwID1, dwID2) != NULL) return false;

	CDare* pDare = NULL;

	switch (dwType)
	{
		case Cmd::UNION_DARE:
			{
				pDare = new CDareUnion();
				break;
			}
		case Cmd::SEPT_DARE:
			{
				pDare = new CDareSept();
				break;
			}
		default:
			break;
	}

	if (pDare == NULL) return false;

	pDare->type = dwType;

	pDare->addFirstID(dwID1);
	pDare->setSecondID(dwID2);

	pDare->userid1 = dwUserID;
	pDare->count = 0;
	rwlock.wrlock();
	addEntry(pDare);
	pDare->setReadyState();
	pCmd->dwWarID = pDare->tempid;
	rwlock.unlock();
	
	return true;
}

CDare* CDareM::findDareRecordByUser(DWORD dwType, DWORD dwUserID1, DWORD dwUserID2)
{
	struct execAll : public execEntry<CDare>
	{
		DWORD _type;
		DWORD _id1;
		DWORD _id2;
		CDare* _dare;

		execAll(DWORD dwType, DWORD dwID1, DWORD dwID2)
		{
			_type = dwType;
			_id1 = dwID1;
			_id2 = dwID2;
			_dare = NULL;
		}
		~execAll(){}

		bool exec(CDare* pDare)
		{
#ifdef _ZJW_DEBUG	    
			Zebra::logger->debug("findDareByUser(%d, %d, %d)(%d, %d, %d)", _type, _id1, _id2, pDare->type, pDare->userid1, pDare->userid2);
#endif	    
			if ((pDare->type == _type 
						&& pDare->userid1 == _id1 && pDare->userid2 == _id2)
					||
					(pDare->type == _type && pDare->userid1 == _id2 && pDare->userid2 == _id1)
					||
					(pDare->type == _type && _id2==0 && (pDare->userid1 == _id1 || pDare->userid2 == _id1))
			   )
			{
				_dare = pDare;
				return false;
			}

			return true;
		}
	};

	//rwlock.wrlock();
	execAll myList(dwType, dwUserID1, dwUserID2);
	execEveryOne(myList);
	//rwlock.unlock();

	return myList._dare;

}

CDare * CDareM::findDareRecordByRelationName(DWORD dwType, const char* r1, const char* r2)
{
	CDare* pDare = NULL;
	
	switch (dwType)
	{
		case Cmd::UNION_DARE:
			{
				CUnion* pRelation1 = NULL;
				CUnion* pRelation2 = NULL;
				
				if (r1)
				{
					pRelation1 = CUnionM::getMe().getUnionByName(r1);
				}

				if (r2)
				{
					pRelation2 = CUnionM::getMe().getUnionByName(r2);
						
				}

				if (pRelation1)
				{
					if (pRelation2)
					{
						pDare = this->findDareRecord(dwType, pRelation1->id, pRelation2->id);
					}
					else
					{
						pDare = this->findDareRecordByID(dwType, pRelation1->id);
					}
				}
			}
			break;
		case Cmd::SCHOOL_DARE:
			//pDare = new CDareSchool();
			break;
		case Cmd::SEPT_DARE:
			{
				CSept* pRelation1 = NULL;
				CSept* pRelation2 = NULL;

				if (r1)
				{
					pRelation1 = CSeptM::getMe().getSeptByName(r1);
				}

				if (r2)
				{
					pRelation2 = CSeptM::getMe().getSeptByName(r2);

				}

				if (pRelation1)
				{
					if (pRelation2)
					{
						pDare = this->findDareRecord(dwType, pRelation1->id, pRelation2->id);
					}
					else
					{
						pDare = this->findDareRecordByID(dwType, pRelation1->id);
					}
				}
			}
			break;
		default:
			break;
	}

	return pDare;

}

CDare * CDareM::findDareRecordByFull(DWORD dwType, DWORD dwDefID, std::vector<DWORD>& vDareList)
{
	struct execAll : public execEntry<CDare>
	{
		DWORD _type;
		DWORD _dwDefID;
		std::vector<DWORD> &_vDareList;
		CDare *_dare;

		execAll(DWORD dwType, DWORD dwDefID, std::vector<DWORD>& vDareList) : _type(dwType), _dwDefID(dwDefID), _vDareList(vDareList), _dare(NULL)
		{
		}
		~execAll(){}

		bool exec(CDare *pDare)
		{
			bool find = true;
			if (pDare)
			{
				if (pDare->type == _type)
				{
					if (pDare->secondID == _dwDefID)
					{
						for (DWORD i=0; i<_vDareList.size(); i++)
						{
							if (!pDare->isAtt(_vDareList[i]))
							{
								find = false;
								break;
							}
						}

						if (find)
						{
							_dare = pDare;
							return false;
						}
					}
				}
			}

			return true;
		}
	};

	execAll myList(dwType, dwDefID, vDareList);
	execEveryOne(myList);
	return myList._dare;

}
CDare * CDareM::findDareRecord(DWORD dwType, DWORD dwID1, DWORD dwID2)
{
	struct execAll : public execEntry<CDare>
	{
		DWORD _type;
		DWORD _id1;
		DWORD _id2;
		CDare *_dare;
		execAll(DWORD dwType, DWORD dwID1, DWORD dwID2)
		{
			_type = dwType;
			_id1 = dwID1;
			_id2 = dwID2;
			_dare = NULL;
		}
		~execAll(){}
		bool exec(CDare *pDare)
		{
			if (pDare)
			{
#ifdef _ZJW_DEBUG			
				Zebra::logger->debug("findDare(%d, %d, %d)", _type, _id1, _id2);
#endif			
				if (_type == Cmd::EMPEROR_DARE && pDare->type == _type)
				{// ֻ����һ���ʳ�����ս����
					_dare = pDare;
					return false;
				}
				else if (pDare->type == _type)
				{
					if (pDare->secondID == _id2 && pDare->isAtt(_id1)
					|| (pDare->secondID == _id1 && pDare->isAtt(_id2)))
					{
						_dare = pDare;
						return false;
					}
				}
			}

			return true;
		}
	};

	if (dwID2!=0)
	{
		execAll myList(dwType, dwID1, dwID2);
		execEveryOne(myList);
		return myList._dare;
	}
	
	return this->findDareRecordByID(dwType, dwID1);
}

CDare*   CDareM::findDareRecordByID(DWORD dwType, DWORD dwID)
{
	struct execAll : public execEntry<CDare>
	{
		DWORD _type;
		DWORD _id1;
		
		CDare *_dare;
		execAll(DWORD dwType, DWORD dwID1)
		{
			_type = dwType;
			_id1 = dwID1;
			_dare = NULL;
		}
		~execAll(){}
		bool exec(CDare *pDare)
		{
			if (pDare)
			{
#ifdef _ZJW_DEBUG			
				Zebra::logger->debug("findDare(%d, %d, %d)", _type, _id1);
#endif			
				if (pDare->type == _type)
				{
					if (pDare->secondID == _id1 || pDare->isAtt(_id1))
					{
						_dare = pDare;
						return false;
					}
				}
			}

			return true;
		}
	};

	execAll myList(dwType, dwID);
	execEveryOne(myList);
	return myList._dare;
}

//---------------------------CDareCountry----------------------------------------------------------
CDareCountry::CDareCountry() : CDare(0,0)
{
}

CDareCountry::CDareCountry(DWORD active_time, DWORD ready_time) : CDare(active_time, ready_time)
{
}

CDareCountry::~CDareCountry()
{
}

void CDareCountry::setActiveState()
{
	Cmd::Session::t_enterWar_SceneSession send;
	Cmd::Session::t_setCountryWar_SceneSession setCountryCmd;
	setCountryCmd.byStatus = 1;
	send.dwWarType = this->type;

	// ����״̬
	rwlock.wrlock();
	this->state = CDare::DARE_ACTIVE;
	rwlock.unlock();

	send.dwToRelationID = this->secondID;
	send.isAtt = true;

	for (DWORD i=0; i<attList.size(); i++)
	{
		this->sendCmdToAllDarePlayer(&send, sizeof(send), attList[i]);
	}

	send.isAtt = false;

	for (DWORD i=0; i<attList.size(); i++)
	{
		send.dwToRelationID = attList[i];
		this->sendCmdToAllDarePlayer(&send, sizeof(send), this->secondID);
	}
	
	DWORD map_id = (this->secondID << 16) + KING_CITY_ID;
	setCountryCmd.dwCountryID = this->secondID;
	//setCountryCmd.dwAttCountryID = attList[0];
	setCountryCmd.dwAreaID = KING_CITY_ID;
	

	SceneSession *pScene = SceneSessionManager::getInstance()->getSceneByID(map_id);
	if (pScene)
	{
		pScene->sendCmd(&setCountryCmd, sizeof(setCountryCmd));
	}
	
	this->printState();
}

void CDareCountry::setReadyOverState()
{
	Cmd::Session::t_enterWar_SceneSession exit_war; // ֪ͨ�������˳���ս״̬
	Cmd::Session::t_setCountryWar_SceneSession setCountryCmd;
	setCountryCmd.byStatus = 0;

	exit_war.dwWarType = this->type;
	exit_war.dwStatus = 0;
	exit_war.dwToRelationID = this->secondID;

	for (DWORD i=0; i<attList.size(); i++)
	{
		this->sendCmdToAllDarePlayer(&exit_war, sizeof(exit_war), attList[i]);
	}
	
	for (DWORD i=0; i<attList.size(); i++)
	{
		exit_war.dwToRelationID = attList[i];
		this->sendCmdToAllDarePlayer(&exit_war, sizeof(exit_war), this->secondID);
	}

	CCountry* pDefCountry = CCountryM::getMe().find(this->secondID);
	CCountry* pAttCountry = NULL;
		
	if (grade1==0 && grade2==0)
	{//relationID2���سɹ�����ս����սʧ�ܴ�����һ���Ǻż���,���ع�սʤ������һ,�Ǻż�һ
		//ƽ�ִ���
		pDefCountry->dwFormalWin++;
		pDefCountry->changeStar(1);

		for (DWORD i=0; i<attList.size(); i++)	
		{
			CDareRecordM::getMe().addNewDareRecord(attList[i], this->secondID, 1);
			pAttCountry = CCountryM::getMe().find(attList[i]);
			if (pAttCountry==NULL || pDefCountry==NULL)
			{
				continue;
			}

			pAttCountry->dwFormalFail++;
			pAttCountry->changeStar(-2);
			pAttCountry->swapMaterialByPer(pDefCountry, 3);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, attList[i], "�ҹ� ���� %s ����սʧ��",
								pDefCountry->name);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, this->secondID, 
					"�ҹ� �� %s �Ĺ����У����سɹ�����սʤ��",
					pAttCountry->name);

			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
					"%s ���� %s �� %s ���� %s ���׳Ƴ�",
					pAttCountry->name, pAttCountry->kingName, pDefCountry->name, pDefCountry->kingName);

/*			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
					"%s �������ʱ� %s �Ӷᣬ%s 3\%�����������ʱ��˵� %s��\n %s ��Χ�� %s ��ս��δȡ�óɹ���%s ��������ʧ�ܸ��ա�\n %s �� %s ��Χ���£���ס�����ǣ� %s ʤ����",
					pAttCountry->name, pDefCountry->name, pAttCountry->name, pDefCountry->name,
					pAttCountry->name, pDefCountry->name, pAttCountry->name, 
					pDefCountry->name, pAttCountry->name, pDefCountry->name);
					// */

	/*		SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
					"%s ��Χ�� %s ��ս��δȡ�óɹ���%s ��������ʧ�ܸ��ա�", 
					pAttCountry->name, 
					pDefCountry->name,
					pAttCountry->name);

			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, "%s �� %s ��Χ���£���ס�����ǣ� %s ʤ����", 
					pDefCountry->name, 
					pAttCountry->name,
					pDefCountry->name);*/

			CSeptM::getMe().changeAllRepute(attList[i], -10);
		}
	}
	else if (grade1==2)
	{//relationID1��ս�ɹ�����ս����սʤ��������һ���ǺżӶ�,���ع���սʧ�ܴ�����һ���Ǻż���
		//��ս��ʤ��
		pDefCountry->dwFormalFail++;
		pDefCountry->changeStar(-2);	
		CSeptM::getMe().changeAllRepute(pDefCountry->dwID, -10);

		for (DWORD i=0; i<attList.size(); i++)
		{
			CDareRecordM::getMe().addNewDareRecord(attList[i], this->secondID, 0);
			pAttCountry = CCountryM::getMe().find(attList[i]);
			//սʤ������ӳɱ�־
			pAttCountry->setWinnerExp();

			if (pAttCountry==NULL || pDefCountry==NULL)
			{
				continue;
			}		    

			pAttCountry->dwFormalWin++;
			pAttCountry->changeStar(2);

			pDefCountry->swapMaterialByPer(pAttCountry, 3);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, attList[i], "�ҹ� ���� %s �ɹ�����սʤ��.�������ڻ�ɱ���ﾭ�������ٷ�֮��ʮ",
					pDefCountry->name);

			pDefCountry->generalLevelDown();
			pDefCountry->gen_refreshTime = SessionTimeTick::currentTime.sec()+5;
			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, this->secondID, 
					"�ҹ� �� %s �Ĺ����У�����ʧ�ݣ���սʧ��.�������Ǵ󳼴����뷴��.\n",
					pAttCountry->name, pDefCountry->gen_level);
			
			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
					"%s ���� %s �� %s ���� %s ���׳Ƴ�",
					pDefCountry->name, pDefCountry->kingName, pAttCountry->name, pAttCountry->kingName);

/*			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
				"%s �������ʱ� %s �Ӷᣬ%s 3\%�����������ʱ��˵� %s��\n %s ��Χ�� %s ��ս���й��ƶԷ�����, %s ȡ�ù�ս��ʤ����\n %s �� %s ��Χ���£��������䣬 %s ս�ܡ�",
					pDefCountry->name, pAttCountry->name, pDefCountry->name, pAttCountry->name,
					pAttCountry->name, pDefCountry->name, pAttCountry->name,
					pDefCountry->name, pAttCountry->name, pDefCountry->name
					);*/

			CSeptM::getMe().changeAllRepute(pAttCountry->dwID, 6);
		}
	}
	else if (grade2==2)
	{//relationID2�����ɹ�����ս��ʧ�ܴ�����һ���Ǻż��������ع���սʤ��������һ���ǺżӶ�
		//���ط�ʤ��
		//սʤ������ӳɱ�־
		pDefCountry->setWinnerExp();
		pDefCountry->dwFormalWin++;
		pDefCountry->changeStar(2);
		CSeptM::getMe().changeAllRepute(pDefCountry->dwID, 6);

		for (DWORD i=0; i<attList.size(); i++)
		{	
			CDareRecordM::getMe().addNewDareRecord(attList[i], this->secondID, 2);
			pAttCountry = CCountryM::getMe().find(attList[i]);

			if (pAttCountry==NULL || pDefCountry==NULL)
			{
				continue;
			}

			pAttCountry->dwFormalFail++;
			pAttCountry->changeStar(-2);
			pAttCountry->swapMaterialByPer(pDefCountry, 3);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, attList[i], 
					"�ҹ� �� %s ͵Ϯ�ɹ�����սʧ��",
					pDefCountry->name);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, this->secondID, 
					"�ҹ� �ڶ� %s �����У�ռ��Է����ǣ���սʤ��",
					pAttCountry->name);

			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
				"%s ��Χ�� %s ��ս�����⵽�Է�͵Ϯ, %s �����⵽�ƻ�, %s ս�ܡ�\n %s �� %s ��Χ���£�͵Ϯ %s ���ǳɹ��� %s ʤ����\n %s �������ʱ� %s �Ӷᣬ%s 3\%�����������ʱ��˵� %s", 
					pAttCountry->name, 
					pDefCountry->name,
					pAttCountry->name,
					pAttCountry->name,
					pDefCountry->name, pAttCountry->name, pAttCountry->name, pDefCountry->name,
					pAttCountry->name, pDefCountry->name, pAttCountry->name, pDefCountry->name
					);

			/*SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, "%s �� %s ��Χ���£�͵Ϯ %s ���ǳɹ��� %s ʤ����", 
					pDefCountry->name, 
					pAttCountry->name,
					pAttCountry->name,
					pDefCountry->name);

			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, "%s �������ʱ� %s �Ӷᣬ%s 3\%�����������ʱ��˵� %s",
					pAttCountry->name, pDefCountry->name, pAttCountry->name, pDefCountry->name);
					*/

			CSeptM::getMe().changeAllRepute(pAttCountry->dwID, -10);
		}
	}

	if (attList.size()>0)
	{
/*		DWORD map_id = (attList[0] << 16) + KING_CITY_ID;
		setCountryCmd.dwCountryID = this->firstID;
		setCountryCmd.dwAreaID = KING_CITY_ID;

		SceneSession *pScene = SceneSessionManager::getInstance()->getSceneByID(map_id);
		if (pScene)
		{
			pScene->sendCmd(&setCountryCmd, sizeof(setCountryCmd));
		}*/
	}
	
	DWORD map_id = (this->secondID << 16) + KING_CITY_ID;
	setCountryCmd.dwCountryID = this->secondID;
	setCountryCmd.dwAreaID = KING_CITY_ID;

	SceneSession *pScene = SceneSessionManager::getInstance()->getSceneByID(map_id);
	if (pScene)
	{
		pScene->sendCmd(&setCountryCmd, sizeof(setCountryCmd));
	}

	if (pDefCountry) pDefCountry->endDare();
	if (pAttCountry) pAttCountry->writeDatabase();
	if (pDefCountry )pDefCountry->writeDatabase();
	this->setOverState();
}

void CDareCountry::setSecondID(DWORD dwID)
{
	this->secondID = dwID;
}

void CDareCountry::addFirstID(DWORD dwID)
{
	attList.push_back(dwID);
}

void CDareCountry::sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, 
		unsigned int cmdLen, unsigned int relationID)
{
	//OneCountryScene ocs;
//	ocs.init(cmd, cmdLen, relationID);
//	UserSessionManager::getInstance()->execEveryUser(ocs);
	cmd->dwFromRelationID = relationID;
	SessionTaskManager::getInstance().broadcastScene(cmd, cmdLen);

	Cmd::stSetCountryDareUserCmd send;
	send.byStatus = cmd->dwStatus;
	if (send.byStatus == 0)
	{//�˳���սʱ���ж�ʤ��
		if (grade1==grade2 || grade2>grade1)	
		{//����ʧ��
			if (this->isAtt(relationID))
			{
				send.byResult = 0;
			}
			else
			{
				send.byResult = 1;
			}
		}
		else
		{
			if (this->isAtt(relationID))
			{
				send.byResult = 1;
			}
			else
			{
				send.byResult = 0;
			}
		}
	}
		
	send.dwCountry = cmd->dwToRelationID;
	SessionTaskManager::getInstance().sendCmdToCountry(relationID, &send, sizeof(send));
}

void CDareCountry::sendActiveStateToScene(UserSession* pUser)
{
	Cmd::Session::t_enterWar_SceneSession send;
	send.dwWarType = type;
	send.dwFromRelationID = pUser->country;

	Cmd::stSetCountryDareUserCmd enter_send;
	enter_send.byStatus = 1;

	if (secondID != pUser->country)
	{
		enter_send.dwCountry = secondID;
		send.dwToRelationID = secondID;

		send.isAtt = true;
		send.dwUserID = pUser->id;
		send.dwSceneTempID = pUser->scene->tempid;
		pUser->scene->sendCmd(&send, sizeof(send));

		pUser->sendCmdToMe(&enter_send, sizeof(enter_send));
	}
	else
	{
		for (DWORD i=0; i<attList.size(); i++)
		{
			send.dwToRelationID = attList[i];
			send.isAtt = false;
			send.isAntiAtt = this->isAntiAtt;
			send.dwUserID = pUser->id;
			send.dwSceneTempID = pUser->scene->tempid;
			pUser->scene->sendCmd(&send, sizeof(send));

			enter_send.dwCountry = attList[i];
			pUser->sendCmdToMe(&enter_send, sizeof(enter_send));
		}
	}
}

void CDareCountry::timer()
{
}

bool CDareCountry::isInvalid()
{
	return false;
}

void CDareCountry::sendAntiAtt()
{
	Cmd::Session::t_setAntiAttFlag_SceneSession send_anti; // ֪ͨ�����ôι�ս���뷴��ģʽ״̬

	send_anti.dwType = this->type;
	send_anti.dwFromRelationID = this->secondID;

	for (DWORD i=0; i<attList.size(); i++)
	{
		send_anti.dwToRelationID = attList[i];
		SessionTaskManager::getInstance().broadcastScene(&send_anti, sizeof(send_anti));
	}
}

//---------------------------CDareAntiCountry----------------------------------------------------------
CDareAntiCountry::CDareAntiCountry() : CDareCountry(0,0)
{
}

CDareAntiCountry::CDareAntiCountry(DWORD active_time, DWORD ready_time) : CDareCountry(active_time, ready_time)
{
}

CDareAntiCountry::~CDareAntiCountry()
{
}

void CDareAntiCountry::setReadyOverState()
{
	Cmd::Session::t_enterWar_SceneSession exit_war; // ֪ͨ�������˳���ս״̬
	Cmd::Session::t_setCountryWar_SceneSession setCountryCmd;
	setCountryCmd.byStatus = 0;

	exit_war.dwWarType = this->type;
	exit_war.dwStatus = 0;
	exit_war.dwToRelationID = this->secondID;

	for (DWORD i=0; i<attList.size(); i++)
	{
		this->sendCmdToAllDarePlayer(&exit_war, sizeof(exit_war), attList[i]);
	}
	
	for (DWORD i=0; i<attList.size(); i++)
	{
		exit_war.dwToRelationID = attList[i];
		this->sendCmdToAllDarePlayer(&exit_war, sizeof(exit_war), this->secondID);
	}
	
	CCountry* pDefCountry = CCountryM::getMe().find(this->secondID);
	CCountry* pAttCountry = NULL;
		
	if (grade1==0 && grade2==0)
	{//relationID2���سɹ�����ս����սʧ�ܴ�����һ���Ǻż���,���ع�սʤ������һ,�Ǻż�һ
		//ƽ�ִ���
		pDefCountry->dwFormalWin++;
		pDefCountry->changeStar(1);

		for (DWORD i=0; i<attList.size(); i++)	
		{
			pAttCountry = CCountryM::getMe().find(attList[i]);
			if (pAttCountry==NULL || pDefCountry==NULL)
			{
				continue;
			}

			pAttCountry->dwFormalFail++;
			pAttCountry->changeStar(-2);
			pAttCountry->swapMaterialByPer(pDefCountry, 3);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, attList[i], "�ҹ� ���� %s ����սʧ��",
								pDefCountry->name);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, this->secondID, 
					"�ҹ� �� %s �Ĺ����У����سɹ�����սʤ��",
					pAttCountry->name);

			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
					"%s �������ʱ� %s �Ӷᣬ%s 3\%�����������ʱ��˵� %s��\n %s ��Χ�� %s ��ս��δȡ�óɹ���%s ��������ʧ�ܸ��ա�\n %s �� %s ��Χ���£���ס�����ǣ� %s ʤ����",
					pAttCountry->name, pDefCountry->name, pAttCountry->name, pDefCountry->name,
					pAttCountry->name, pDefCountry->name, pAttCountry->name, 
					pDefCountry->name, pAttCountry->name, pDefCountry->name);

			CSeptM::getMe().changeAllRepute(attList[i], -10);
		}
	}
	else if (grade1==2)
	{//relationID1��ս�ɹ�����ս����սʤ��������һ���ǺżӶ�,���ع���սʧ�ܴ�����һ���Ǻż���
		//��ս��ʤ��
		pDefCountry->dwFormalFail++;
		pDefCountry->changeStar(-2);	
		CSeptM::getMe().changeAllRepute(pDefCountry->dwID, -10);

		for (DWORD i=0; i<attList.size(); i++)
		{
			pAttCountry = CCountryM::getMe().find(attList[i]);
			//սʤ������ӳɱ�־
			//pAttCountry->setWinnerExp();

			if (pAttCountry==NULL || pDefCountry==NULL)
			{
				continue;
			}		    

			pAttCountry->dwFormalWin++;
			pAttCountry->changeStar(2);

			pDefCountry->swapMaterialByPer(pAttCountry, 3);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, attList[i], "�ҹ� ���� %s �ɹ�����սʤ��.",
					pDefCountry->name);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, this->secondID, 
					"�ҹ� �� %s �Ĺ����У�����ʧ�ݣ���սʧ��.",
					pAttCountry->name);
			

			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
				"%s �������ʱ� %s �Ӷᣬ%s 3\%�����������ʱ��˵� %s��\n %s ��Χ�� %s ��ս���й��ƶԷ�����, %s ȡ�ù�ս��ʤ����\n %s �� %s ��Χ���£��������䣬 %s ս�ܡ�",
					pDefCountry->name, pAttCountry->name, pDefCountry->name, pAttCountry->name,
					pAttCountry->name, pDefCountry->name, pAttCountry->name,
					pDefCountry->name, pAttCountry->name, pDefCountry->name
					);

			CSeptM::getMe().changeAllRepute(pAttCountry->dwID, 6);
		}
	}
	else if (grade2==2)
	{//relationID2�����ɹ�����ս��ʧ�ܴ�����һ���Ǻż��������ع���սʤ��������һ���ǺżӶ�
		//���ط�ʤ��
		//սʤ������ӳɱ�־
		//pDefCountry->setWinnerExp();
		pDefCountry->dwFormalWin++;
		pDefCountry->changeStar(2);
		CSeptM::getMe().changeAllRepute(pDefCountry->dwID, 6);

		for (DWORD i=0; i<attList.size(); i++)
		{	
			pAttCountry = CCountryM::getMe().find(attList[i]);

			if (pAttCountry==NULL || pDefCountry==NULL)
			{
				continue;
			}

			pAttCountry->dwFormalFail++;
			pAttCountry->changeStar(-2);
			pAttCountry->swapMaterialByPer(pDefCountry, 3);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, attList[i], 
					"�ҹ� �� %s ͵Ϯ�ɹ�����սʧ��",
					pDefCountry->name);

			SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_EXP, this->secondID, 
					"�ҹ� �ڶ� %s �����У�ռ��Է����ǣ���սʤ��",
					pAttCountry->name);

			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_GAME, 
				"%s ��Χ�� %s ��ս�����⵽�Է�͵Ϯ, %s �����⵽�ƻ�, %s ս�ܡ�\n %s �� %s ��Χ���£�͵Ϯ %s ���ǳɹ��� %s ʤ����\n %s �������ʱ� %s �Ӷᣬ%s 3\%�����������ʱ��˵� %s", 
					pAttCountry->name, 
					pDefCountry->name,
					pAttCountry->name,
					pAttCountry->name,
					pDefCountry->name, pAttCountry->name, pAttCountry->name, pDefCountry->name,
					pAttCountry->name, pDefCountry->name, pAttCountry->name, pDefCountry->name
					);

			CSeptM::getMe().changeAllRepute(pAttCountry->dwID, -10);
		}
	}

	if (attList.size()>0)
	{
/*		DWORD map_id = (attList[0] << 16) + KING_CITY_ID;
		setCountryCmd.dwCountryID = this->firstID;
		setCountryCmd.dwAreaID = KING_CITY_ID;

		SceneSession *pScene = SceneSessionManager::getInstance()->getSceneByID(map_id);
		if (pScene)
		{
			pScene->sendCmd(&setCountryCmd, sizeof(setCountryCmd));
		}*/
	}
	
	DWORD map_id = (this->secondID << 16) + KING_CITY_ID;
	setCountryCmd.dwCountryID = this->secondID;
	setCountryCmd.dwAreaID = KING_CITY_ID;

	SceneSession *pScene = SceneSessionManager::getInstance()->getSceneByID(map_id);
	if (pScene)
	{
		pScene->sendCmd(&setCountryCmd, sizeof(setCountryCmd));
	}

	//if (pDefCountry) pDefCountry->endDare();
	if (pAttCountry) pAttCountry->writeDatabase();
	if (pDefCountry )pDefCountry->writeDatabase();
	this->setOverState();
}

