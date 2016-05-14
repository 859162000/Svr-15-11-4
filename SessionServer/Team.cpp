#include "Team.h"
#include "Session.h"
#include "SessionManager.h"


/**
 * \brief ��ӳ�Ա
 * \param userid ��ɫid
 * \return true �ɹ� false ʧ��
 */
bool Team::addMember(const DWORD userid)
{
	UserSession *pUser=UserSessionManager::getInstance()->getUserByID(userid);
	if(!pUser)
	{
		return false;
	}
	bool bret = false;
	bret = member.insert(userid).second;
	if(bret)
	{
		pUser->teamid = leaderid;
		//Zebra::logger->debug("��Ӷ�Ա(%u , %u)" , pUser->teamid , pUser->id); 
	}
	return bret;
}

/**
 * \brief ɾ����Ա
 * \param userid ��ɫid
 * \return true �ɹ�
 */
bool Team::delMember(const DWORD userid)
{
	member.erase(userid);
	UserSession *pUser=UserSessionManager::getInstance()->getUserByID(userid);
	if(pUser)
	{
		//Zebra::logger->debug("ɾ����Ա(%u , %u)" , pUser->teamid , pUser->id); 
		pUser->teamid = 0;
	}
	return true;
}

/**
 * \brief ���öӳ�
 * \param leader �ӳ�id
 * \return true �ɹ� false ʧ��
 */
bool Team::setLeader(const DWORD leader)
{
	leaderid = leader;
	return addMember(leaderid);
}

/**
 * \brief ɾ������
 * \return true �ɹ� false ʧ��
 */
bool Team::delTeam()
{
	MemberSet_iter iter;
	for(iter = member.begin() ; iter != member.end() ; iter ++)
	{
		UserSession *pUser=UserSessionManager::getInstance()->getUserByID(*iter);
		if(pUser)
		{
			//Zebra::logger->debug("�����ɢɾ����Ա(%u , %u)" , pUser->teamid , pUser->id); 
			pUser->teamid = 0;
		}
	}
	leaderid = 0;
	return true;
}

/// ȫ�ֶ�������
GlobalTeamIndex *GlobalTeamIndex::instance = NULL;

/**
 * \brief ��ȡȫ�ֶ�������Ψһʵ��
 * \return ȫ�ֶ�����������
 */
GlobalTeamIndex *GlobalTeamIndex::getInstance()
{
	if(!instance)
	{
		instance = new GlobalTeamIndex;
	}
	return instance;
}

/**
 * \brief ɾ��ʵ��
 */
void GlobalTeamIndex::delInstance()
{
	SAFE_DELETE(instance);
}

/**
 * \brief ���ӳ�Ա
 * \param leaderid �ӳ�id
 * \param userid ��ԱID
 * \return true �ɹ� false ʧ��
 */
bool GlobalTeamIndex::addMember(const DWORD leaderid , const DWORD userid)
{
	bool bret = false;
	mlock.lock();
	if(leaderid == userid)
	{
		Team t;
		t.setLeader(leaderid);
		bret = team.insert(TeamMap_value_type(leaderid , t)).second;
	}
	else
	{
		TeamMap_iterator iter = team.find(leaderid);
		if(iter != team.end())
		{
			bret = iter->second.addMember(userid);
		}
		else
		{
			bret = false;
		}
	}
	mlock.unlock();
	return bret;
}

/**
 * \brief ɾ����Ա
 * \param leaderid �ӳ�id
 * \param userid ��ԱID
 * \return true �ɹ� false ʧ��
 */
bool GlobalTeamIndex::delMember(const DWORD leaderid , const DWORD userid)
{
	bool bret = false;
	mlock.lock();
	TeamMap_iterator iter = team.find(leaderid);
	if(iter != team.end())
	{
		if(leaderid == userid)
		{
			iter->second.delTeam();
			team.erase(leaderid);
			bret = true;
		}
		else
		{
			bret = iter->second.delMember(userid);
		}
	}
	else
	{
		bret = false;
	}
	mlock.unlock();
	return bret;
}
