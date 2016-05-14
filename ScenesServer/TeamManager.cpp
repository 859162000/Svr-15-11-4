
#include "SceneUser.h"
#include "Scene.h"
#include "TeamManager.h"
#include "Chat.h"
#include "SessionClient.h"

/**
 * \brief  ������ӽ�ָ��������Աuser�����ݷ��͸��������г�Ա�Ŀͻ���
 */
struct SendTeamDataExec : public TeamMemExec
{

	/// �ӳ�
	SceneUser *leader;

	/// ������ӳ�Ա��Ϣ����
	Cmd::stFreshTeamMemberUserCmd ret;

	/**
	 * \brief  ���캯����ʼ����ʼ����Ҫ���͵���Ϣ
	 * \param  l �ӳ���ָ��
	 * \param  user ������Ա�Ķ���ָ��
	 * \return 
	 */
	SendTeamDataExec(SceneUser * l , SceneUser *user)
	{
		leader = l; 
		ret.dwTempID = user->tempid;
		ret.dwMaxHealth = user->charstate.maxhp;
		ret.dwHealth = user->charbase.hp;
		ret.dwMaxMp = user->charstate.maxmp;
		ret.dwMp = user->charbase.mp;
	}

	/**
	 * \brief  �ص�����
	 * \param  member ������Ա
	 * \return false ������ֹ true �������
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
						getUserByTempID(member.tempid);
		if(pUser)
		{
			if(ret.dwTempID != pUser->tempid)
				pUser->sendCmdToMe(&ret , sizeof(ret));
			return true;
		}
		return false;
	}
};

/**
 * \brief  �������ָ����Ա����
 * \param  leader �ӳ�
 * \param  user ��Ա
 */
void TeamManager::sendtoTeamCharData(SceneUser *leader , SceneUser *user)
{
		if(leader)
		{
			SendTeamDataExec exec(leader , user);
			team.execEvery(exec);
		}
}

/**
 * \brief  �������ָ����Ա����
 * \param  user ��Ҫ���͵ĳ�Ա����
 */
void TeamManager::sendtoTeamCharData(SceneUser *user)
{
	if(user->team.IsTeamed())
	{
		SceneUser *leader = SceneUserManager::getMe().getUserByTempID(user->team.getLeader());
		if(leader)
		{
			leader->team.sendtoTeamCharData(leader , user);
		}
	}
}

/**
 * \brief  �����ṹ����������������ж�Ա�㲥��Ϣ
 */
struct SendCmdExec : public TeamMemExec
{
	/// ��Ҫ�㲥����Ϣ
	void *cmd;

	/// ��Ϣ����
	DWORD cmdLen;

	/**
	 * \brief  �����ʼ������
	 * \param  data ��Ϣ��
	 * \param  dataLen ��Ϣ����
	 */
	SendCmdExec(void *data , DWORD dataLen)
	{
		cmd = data;
		cmdLen = dataLen;
	}

	/**
	 * \brief  �ص�����
	 * \param  member ��Ա
	 * \return false ��ֹ���� true ��������
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
						getUserByTempID(member.tempid);
		if(pUser)
		{
			pUser->sendCmdToMe(cmd , cmdLen);
			return true;
		}
		return true;
	}
};

/**
 * \brief  ������Ϣ�����
 * \param  user   ��Ϣ������
 * \param  cmd    ��Ϣ��
 * \param  cmdLen ��Ϣ����
 */
void TeamManager::sendCmdToTeam(SceneUser *user , void *cmd , DWORD cmdLen)
{
	if(user->team.IsTeamed())
	{
		SceneUser *leader = SceneUserManager::getMe().getUserByTempID(user->team.getLeader());
		if(leader)
		{
			SendCmdExec exec(cmd , cmdLen);
			leader->team.execEveryOne(exec);
		}
	}
}

/**
 * \brief  ����������ж�Ա�Ƿ���һ����
 */
struct CheckAllInOneScreenExec : public TeamMemExec
{
	bool isOk;
	SceneUser *leader;

	/**
	 * \brief  �����ʼ������
	 * \param  data ��Ϣ��
	 * \param  dataLen ��Ϣ����
	 */
	CheckAllInOneScreenExec(SceneUser *pUser)
	{
		isOk = true;
		leader = pUser;
	}

	/**
	 * \brief  �ص�����
	 * \param  member ��Ա
	 * \return false ��ֹ���� true ��������
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
						getUserByTempID(member.tempid);
		if(pUser)
		{
			if (!(pUser->scene == leader->scene && pUser->scene->zPosShortRange(pUser->getPos() , leader->getPos() , 11, 11)))
			{
				isOk = false;
			}
			return true;
		}
		else
		{
			isOk = false;
		}
		return true;
	}
};


bool TeamManager::canPutSkill()
{
	if(me && me->team.IsTeamed())
	{
		SceneUser *leader = SceneUserManager::getMe().getUserByTempID(me->team.getLeader());
		if(leader)
		{
			CheckAllInOneScreenExec exec(leader);
			leader->team.execEveryOne(exec);
			if (!exec.isOk)
			{
				if (!giveupstatus)
				{
					giveupstatus = true;
					giveuptime.now();
					giveuptime.addDelay(120000);
					return true;
				}
				else
				{
					if (SceneTimeTick::currentTime.msecs() > giveuptime.msecs())
					{
						giveupstatus = false;///��״̬��ʼ����ȥ��
						return false;
					}
					else
						return true;
				}
			}
			else
			{
				giveupstatus = false;
				return true;
			}
		}
	}
	return false;
}

/**
 * \brief   ����һ���µĶ�Ա����
 */
struct AddNewMemberExec : public TeamMemExec
{
	/// �ӳ�
	SceneUser *leader;

	/// �³�Ա
	SceneUser *nm;

	/// ��Ա������Ϣʵ��1
	Cmd::stAddTeamMemberUserCmd ret_1;

	/// ��Ա������Ϣʵ��2
	Cmd::stAddTeamMemberUserCmd ret_2;

	/**
	 * \brief  �����ʼ����Ϣʵ��
	 * \param  u �ӳ���ɫ����
	 * \param n ������Ա��ɫ����
	 */
	AddNewMemberExec(SceneUser *u , SceneUser *n)
	{
		leader = u;
		nm = n;
		ret_1.dwTeamID = u->team.getLeader();
		ret_2.dwTeamID = u->team.getLeader();
		ret_2.data.dwTempID = nm->tempid;
		ret_2.data.dwMaxHealth = nm->charstate.maxhp;
		ret_2.data.dwHealth = nm->charbase.hp;
		ret_2.data.dwMaxMp = nm->charstate.maxmp;
		ret_2.data.dwMp = nm->charbase.mp;
		ret_2.data.wdFace = nm->charbase.face;
		strncpy(ret_2.data.pstrName , nm->name , MAX_NAMESIZE);
		ret_2.data.byHead = false;
	}

	/**
	 * \brief  �ص��������³�Ա���͸�ÿ����Ա����ÿ����Ա���͸��³�Ա
	 * \param  member ��Ա
	 * \return false ��ֹ���� true ��������
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
						getUserByTempID(member.tempid);
		if(pUser)
		{
			ret_1.dwTeamID = pUser->team.getLeader();
			strncpy(ret_1.data.pstrName , pUser->name , MAX_NAMESIZE);
			if(leader->tempid == pUser->tempid)
			{
				ret_1.data.byHead = true;
			}
			else
			{
				ret_1.data.byHead = false;
			}
			ret_1.data.dwTempID = pUser->tempid;
			ret_1.data.dwMaxHealth = pUser->charstate.maxhp;
			ret_1.data.dwHealth = pUser->charbase.hp;
			ret_1.data.dwMaxMp = pUser->charstate.maxmp;
			ret_1.data.dwMp = pUser->charbase.mp;
			ret_1.data.wdFace = pUser->charbase.face;
			strncpy(ret_1.data.pstrName , pUser->name , MAX_NAMESIZE);
			pUser->sendCmdToMe(&ret_2 , sizeof(ret_2));
			Channel::sendSys(pUser , Cmd::INFO_TYPE_GAME, "%s�������" , ret_2.data.pstrName);
			nm->sendCmdToMe(&ret_1 , sizeof(ret_1));
			//Zebra::logger->debug("����(%ld)������Ӷ�Աָ��(%s , %ld)" , ret_2.dwTeamID , nm->name , nm->id);
			return true;
		}
		return true;
	}
};

/**
 * \brief  �жϽ�ɫ�Ƿ����Լ��Ķ���
 * \param  pUser ���ж϶���
 * \return true �� false ��
 */
bool TeamManager::IsOurTeam(SceneUser *pUser)
{
	if(!team.leader)
	{
		return false;
	}
	else
	{
		return team.leader == pUser->team.getLeader();
	}
}

/**
 * \brief  �жϽ�ɫ�Ƿ����Լ��Ķ���
 * \param  dwID ���ж϶����id
 * \return true �� false ��
 */
bool TeamManager::IsOurTeam(DWORD dwID)
{
	SceneUser *pUser = SceneUserManager::getMe().
		getUserByID(dwID);
	if(pUser)
	{
		return IsOurTeam(pUser);
	}
	return false;
}

/**
 * \brief  ����һ���µĶ��飬����һ���ӳ�
 * \param  pUser ׼�ӳ�
 * \return true �ɹ� false ʧ��
 */
bool TeamManager::addNewLeader(SceneUser *pUser)
{
	if(team.getSize() == 0)
	{
		/*		
				if(addNewMember(pUser , pUser))
				{
				Zebra::logger->debug("��������%ld,�鳤(%s , %ld)" , pUser->tempid , pUser->name , pUser->id);
				return true;
				}
		// */
		if(addMemberByTempID(pUser , pUser->tempid))
		{
			pUser->team.setLeader(pUser->tempid);
			Cmd::stAddTeamMemberUserCmd ret;
			ret.data.dwTempID = pUser->tempid;
			ret.data.dwMaxHealth = pUser->charstate.maxhp;
			ret.data.dwHealth = pUser->charbase.hp;
			ret.data.dwMaxMp = pUser->charstate.maxmp;
			ret.data.dwMp = pUser->charbase.mp;
			ret.data.wdFace = pUser->charbase.face;
			strncpy(ret.data.pstrName , pUser->name , MAX_NAMESIZE);
			ret.data.byHead = true;
			setLeader(pUser->tempid);
			pUser->sendCmdToMe(&ret , sizeof(ret));
			if(pUser->team_mode == Cmd::TEAM_HONOR)
			{
				Channel::sendSys(pUser , Cmd::INFO_TYPE_GAME, "%s������ģʽ�齨����" , ret.data.pstrName);
			}
			else
			{
				Channel::sendSys(pUser , Cmd::INFO_TYPE_GAME, "%s����ͨģʽ�齨����" , ret.data.pstrName);
			}
			//Zebra::logger->trace("��������%ld,�鳤(%s , %ld)" , pUser->tempid , pUser->name , pUser->id);

			pUser->reSendMyMapData();
			//session����
			addMemberToSession(pUser->id , pUser->id);
			return true;
		}
	}
	return false;
}

/**
 * \brief  ����һ���µĶ�Ա
 * \param  pUser ׼��Ա
 * \param  rev �������Ӧ����Ϣ
 * \return true ����ɹ� false ����ʧ��
 */
bool TeamManager::addNewMember(SceneUser *pUser , Cmd::stAnswerTeamUserCmd *rev)
{
		SceneUser *nm = SceneUserManager::getMe().
						getUserByTempID(rev->dwAnswerUserID);
	if(nm)
	{
		if (nm->issetUState(Cmd::USTATE_TOGETHER_WITH_TIGER)
				||  nm->issetUState(Cmd::USTATE_TOGETHER_WITH_DRAGON))
		{
			return false;
		}
		nm->team.setLeader(pUser->tempid);
		if(addMemberByTempID(pUser , rev->dwAnswerUserID))
		{
			Cmd::stTeamModeUserCmd tmu; 
			tmu.byType=pUser->team_mode;
			nm->sendCmdToMe(&tmu , sizeof(tmu));
			nm->team.setObjMode(pUser->team.getObjMode());
			Cmd::stObjModeTeamMemberUserCmd objmode; 
			objmode.byType=pUser->team.getObjMode();
			nm->sendCmdToMe(&objmode , sizeof(objmode));
			nm->team.setExpMode(pUser->team.getExpMode());
			Cmd::stExpModeTeamMemberUserCmd expmode; 
			expmode.byType=pUser->team.getExpMode();
			nm->sendCmdToMe(&expmode , sizeof(expmode));
			AddNewMemberExec add(pUser , nm);
			team.execEvery(add);
			//FunctionTimes times(103,__FUNCTION__);
			pUser->reSendMyMapData();

			addMemberToSession(pUser->id , nm->id);
			return true;

		}
	}
	return false;
}

/**
 * \brief  ����һ���µĶ�Ա
 * \param  leader �ӳ�
 * \param  pUser ׼��Ա
 * \return true ���ӳɹ�  false ����ʧ��
 */
bool TeamManager::addNewMember(SceneUser *leader , SceneUser *pUser)
{
	if (pUser->issetUState(Cmd::USTATE_TOGETHER_WITH_TIGER)
			||  pUser->issetUState(Cmd::USTATE_TOGETHER_WITH_DRAGON))
	{
		return false;
	}

	pUser->team.setLeader(leader->tempid);
	///Zebra::logger->debug("��Ա(%u)�������:%u",pUser->tempid , leader->tempid);
	AddNewMemberExec add(leader , pUser);
	if(addMemberByTempID(leader , pUser->tempid))
	{
		Cmd::stTeamModeUserCmd tmu; 
		tmu.byType=leader->team_mode;
		pUser->sendCmdToMe(&tmu , sizeof(tmu));
		pUser->team.setObjMode(leader->team.getObjMode());
		Cmd::stObjModeTeamMemberUserCmd objmode; 
		objmode.byType=leader->team.getObjMode();
		pUser->sendCmdToMe(&objmode , sizeof(objmode));
		pUser->team.setExpMode(leader->team.getExpMode());
		Cmd::stExpModeTeamMemberUserCmd expmode; 
		expmode.byType=leader->team.getExpMode();
		pUser->sendCmdToMe(&expmode , sizeof(expmode));
		team.execEvery(add);
		//FunctionTimes times(102,__FUNCTION__);
		pUser->reSendMyMapData();

		addMemberToSession(leader->id , pUser->id);
		return true;

	}

	return false;
}

/**
 * \brief ��ȡ��������
 */
int TeamManager::getSize()
{
	if(me && me->team.IsTeamed())
	{
		if (me->tempid == me->team.getLeader())
		{
			return team.getSize();
		}
		else
		{
			SceneUser *leader = SceneUserManager::getMe().getUserByTempID(me->team.getLeader());
			if(leader)
			{
				return leader->team.getSize();
			}
		}
	}
	return 0;
}

/**
 * \brief  ɾ�����
 * \param  pUser �ӳ�
 * \param  tempid �ӳ�����ʱid
 */
void TeamManager::removeTeam(SceneUser *pUser , DWORD tempid)
{
	if(pUser->team.getLeader() == tempid)
	{
		//changeLeader();
		pUser->team.setLeader(0);
		removeMemberByTempID(tempid);
	}
}

/**
 * \brief  �����ɢ��������ÿ����Ա���˳�����
 */
struct DeleteTeamExec : public TeamMemExec
{
	/// �ӳ�
	SceneUser *leader;

	/// ɾ�������Ϣ
	Cmd::stRemoveTeamUserCmd ret;

	/**
	 * \brief  �����ʼ��
	 * \param  u �ӳ�
	 */
	DeleteTeamExec(SceneUser *u)
	{
		leader = u;
	}

	/**
	 * \brief  �ص����������Ͷ����ɢ��Ϣ��ÿ����Ա
	 * \param  member ��Ա
	 * \return true �������� false ��ֹ����
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
						getUserByTempID(member.tempid);
		if(pUser)
		{
			//FunctionTimes times(100,"�����ɢ");
			pUser->team.setLeader(0);
			pUser->sendCmdToMe(&ret , sizeof(ret));
			pUser->reSendMyMapData();
			Channel::sendSys(pUser , Cmd::INFO_TYPE_GAME, "�����ɢ");
		}
		return true;
	}

};

/**
 * \brief  ɾ�����
 * \param  leader �ӳ�
 */
void TeamManager::deleteTeam(SceneUser *leader)
{
	//��������־
	setLeader(0);
	DeleteTeamExec del(leader);
	team.execEvery(del);
	team.Clear();
	// Session����
	delMemberToSession(leader->id ,leader->id);
}

/**
 * \brief  ɾ����Ա����֪ͨ���ж����Ա
 */
struct RemoveMemberExec : public TeamMemExec
{
	/// �ӳ�
	SceneUser *leader;

	/// ɾ����Ա��Ϣ
	Cmd::stRemoveTeamMemberUserCmd ret;


	/**
	 * \brief  ���캯����ʼ����Աɾ����Ϣ
	 * \param  u �ӳ�
	 * \param  rem  ��Աɾ��֪ͨ��Ϣ
	 */
	RemoveMemberExec(SceneUser *u , const Cmd::stRemoveTeamMemberUserCmd *rem)
	{
		leader = u;
		ret.dwTeamID = rem->dwTeamID;
		//strncpy(ret.pstrName , rem->pstrName , MAX_NAMESIZE);
		ret.dwTempID = rem->dwTempID;
	}

	/**
	 * \brief  �ص�����֪ͨ���ж�Ա�г�Ա�뿪
	 * \param  member ��Ա
	 * \return true �������� false ��ֹ����
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
			getUserByTempID(member.tempid);
		if(pUser)
		{
			pUser->sendCmdToMe(&ret , sizeof(ret));
			//Channel::sendSys(pUser , Cmd::INFO_TYPE_GAME, "%s�뿪����" , ret.pstrName);
		}
		return true;
	}

};

/**
 * \brief  �ڶ���������
 * \param  pUser �ӳ�
 * \param rev ��Աɾ����Ϣ
 */
void TeamManager::kickoutMember(SceneUser *pUser , Cmd::stRemoveTeamMemberUserCmd *rev)
{
	RemoveMemberExec rem(pUser , rev);
	team.execEvery(rem);
	//DWORD tempid = team.removeMemberByName(rev->pstrName);
	team.removeMemberByTempID(rev->dwTempID);
	if(rev->dwTempID)
	{
		SceneUser *u = SceneUserManager::getMe().
						getUserByTempID(rev->dwTempID);
		if(u)
		{
			u->team.setLeader(0);
			u->reSendMyMapData();
			// Session����
			delMemberToSession(pUser->id ,u->id);
		}
	}
}

/**
 * \brief  ��Ӧ��Ϣɾ�������Ա
 * \param  pUser ��ɾ���ĳ�Ա
 * \param  rev ɾ�������Ա��Ϣ
 */
void TeamManager::removeMember(SceneUser *pUser , Cmd::stRemoveTeamMemberUserCmd *rev)
{
	RemoveMemberExec rem(pUser , rev);
	team.execEvery(rem);
	//DWORD tempid = team.removeMemberByName(rev->pstrName);
	team.removeMemberByTempID(rev->dwTempID);
	if(rev->dwTempID)
	{
		SceneUser *u = SceneUserManager::getMe().
						getUserByTempID(rev->dwTempID);
		if(u)
		{
			//FunctionTimes times(101,__FUNCTION__);
			u->team.setLeader(0);
			u->reSendMyMapData();
			// Session����
			delMemberToSession(pUser->id ,u->id);
		}
	}
}

/**
 * \brief  ɾ�������Ա
 * \param  pUser �ӳ�
 * \param  mem ��ɾ����Ա
 */
void TeamManager::removeMember(SceneUser *pUser , SceneUser *mem)
{
	//TODO ɾ������
	Cmd::stRemoveTeamMemberUserCmd ret;
	ret.dwTeamID = pUser->tempid;
	//strncpy(ret.pstrName , mem->name , MAX_NAMESIZE);
	ret.dwTempID=mem->tempid;
	removeMember(pUser , &ret);
	// Session����
	delMemberToSession(pUser->id ,mem->id);
}				

/**
 * \brief  ����
 * \param  callback �ص�����
 */
void TeamManager::execEveryOne(TeamMemExec &callback)
{
	team.execEvery(callback);
}

/**
 * \brief  ���������ض���Ա��������ж�Ա
 * \param  callback �ص�����
 * \param  tempid ���ų���Ա�� id
 */
void TeamManager::execEveryOneExceptMe(TeamMemExec &callback , DWORD tempid)
{
	team.execEveryExceptMe(callback , tempid);
}

/**
 * \brief  ����id����һ���µĳ�Ա
 * \param  pUser ��ɫָ�룬���ض�Ҫ��
 * \param  id ������Ա�� id
 * \return true ���ӳɹ� false ����ʧ��
 */
bool TeamManager::addMemberByID(SceneUser *pUser , DWORD id)
{
	//�ڵز����ж�
	/*
	if(IsFull())
	{
		//TODO ��ʾ��
		return false;
	}
	// */
	if(pUser)
	{
		SceneUser *u = SceneUserManager::getMe().getUserByID(id);
		if(u)
		{
			if(team.addMember(id , u->tempid , u->name))
			{
				return true;
			}
		}
	}
	return false;
}

/**
 * \brief  ����������߳�Ա״̬��δ���߳�Ա��������ʱ�䣬�����涨ʱ������߳�Ա�Ӷ�����ɾ��
 */
struct CheckOfflineExec : public TeamMemExec
{
	/// �ӳ�
	SceneUser *leader;

	/// ���Ͷ���
	typedef std::vector<Cmd::stRemoveTeamMemberUserCmd> Remove_vec;

	/// ���Ͷ���
	typedef Remove_vec::iterator Remove_vec_iterator;

	/// ���������
	Remove_vec del_vec;

	/// ���Ͷ���
	typedef std::vector<DWORD> Online_vec;

	/// �������ָ��
	typedef Online_vec::iterator Online_vec_iterator;

	/// �������߹�����
	Online_vec add_vec;

	/**
	 * \brief  ���캯����ʼ������
	 * \param  u �ӳ�
	 */
	CheckOfflineExec(SceneUser *u)
	{
		leader = u;
	}

	/**
	 * \brief  �����������߳�Ա
	 * \param  member ��Ա
	 * \return true �������� false ��ֹ����
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
			getUserByID(member.id);
		if(pUser)
		{
			if(!pUser->team.IsTeamed())
			{
				add_vec.push_back(pUser->id);
			}
			/*
			//�ո�����
			if(member.offtime != 0)
			{
			if(!pUser->team.IsTeamed())
			{
			add_vec.push_back(pUser->id);
			}
			//leader->team.addNewMember(leader , pUser);
			}
			// */
		}
		else
		{
			member.offtime += 10;
			if(member.offtime >= 120)
			{
				//TODO ɾ������
				Cmd::stRemoveTeamMemberUserCmd ret;
				ret.dwTeamID = leader->tempid;
				//strncpy(ret.pstrName , member.name , MAX_NAMESIZE);
				ret.dwTempID=member.tempid;
				del_vec.push_back(ret);
			}
		}
		return true;
	}
};

/**
 * \brief  ������߳�Ա״̬��ɾ����ʱ�ģ��ָ����ߵ�
 * \param  pUser �ӳ�
 */
void TeamManager::checkOffline(SceneUser *pUser)
{
	CheckOfflineExec exec(pUser);
	team.execEvery(exec);
	if(!exec.del_vec.empty())
	{
		for(CheckOfflineExec::Remove_vec_iterator del_iter=exec.del_vec.begin();del_iter != exec.del_vec.end();del_iter ++)
		{
			pUser->team.removeMember(pUser , &*del_iter);
		}
		exec.del_vec.clear();
		if(pUser->team.getSize() == 1)
		{
			pUser->team.deleteTeam(pUser);
		}
	}
	if(!exec.add_vec.empty())
	{
		for(CheckOfflineExec::Online_vec_iterator add_iter=exec.add_vec.begin();add_iter !=exec.add_vec.end();add_iter ++)
		{
			SceneUser *tuser = SceneUserManager::getMe().
				getUserByID(*add_iter);
			if(tuser)
			{
				pUser->team.addNewMember(pUser , tuser);
			}
		}
		exec.add_vec.clear();
	}
}

/**
 * \brief  ���������������еĳ�Աת�Ƶ��¶ӳ�����
 */
struct ChangeLeaderExec : public TeamMemExec
{
	/// ���ζӳ�
	SceneUser *leader;

	/// �¶ӳ�
	SceneUser *newleader;

	/**
	 * \brief  �����ʼ������
	 * \param  u �����ζӳ�
	 * \param  n δ���ӳ�
	 */
	ChangeLeaderExec(SceneUser *u , SceneUser *n)
	{
		leader = u;
		newleader = n;
	}

	/**
	 * \brief  ���������ж�Աת�Ƶ��¶ӳ�����
	 * \param  member ��Ա
	 * \return  true �������� false ��ֹ����
	 */
	bool exec(TeamMember &member)
	{
		if(member.id != newleader->tempid)
		{
			SceneUser *user = SceneUserManager::getMe().
				getUserByTempID(member.tempid);
			if(user)
			{
				newleader->team.addNewMember(newleader , user);
			}
		}
		return true;
	}
};

/**
 * \brief  �����������һ�����ζӳ�����ĳ�Աʹ���Ϊ�¶ӳ�
 */
struct FindLeaderExec : public TeamMemExec
{
	/// ���ζӳ�
	SceneUser *leader;

	/// �¶ӳ�
	SceneUser *newleader;

	/**
	 * \brief  ���캯����ʼ������
	 * \param  u ��ǰ�ӳ�
	 */
	FindLeaderExec(SceneUser *u)
	{
		leader = u;
		newleader = NULL;
	}

	/**
	 * \brief  ���������µĶӳ�
	 * \param  member ��Ա
	 * \return true ��ʾ�������� false ��ֹ����
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
			getUserByTempID(member.tempid);
		if(pUser && pUser->tempid != leader->tempid)
		{
			newleader = pUser;
			return true;
		}
		else
		{
			return false;
		}
		return true;
	}
};

/**
 * \brief  ����һ���µĶӳ�
 * \param  pUser ��ǰ�ӳ�
 * \return �µĶӳ������ɫָ��
 */
SceneUser *TeamManager::findNewLeader(SceneUser *pUser)
{
	FindLeaderExec exec(pUser);
	team.execEvery(exec);
	return exec.newleader;
}

/**
 * \brief  �ı�ӳ�
 * \param  pUser ��ǰ�ӳ�
 * \return true �ı�ɹ�  false �ı�ʧ��
 */
bool TeamManager::changeLeader(SceneUser *pUser)
{
	SceneUser *newleader = findNewLeader(pUser);
	if(newleader)
	{
		if(newleader->team.addNewLeader(newleader))
		{
			ChangeLeaderExec exec(pUser , newleader);
			team.execEvery(exec);
		}
		return true;
	}
	return false;
}

/**
 * \brief  ������ʱid���Ӷ����Ա
 * \param  pUser δʹ��
 * \param tempid �����ӳ�Ա����ʱid
 * \return true ���ӳɹ� false ����ʧ��
 */
bool TeamManager::addMemberByTempID(SceneUser *pUser , DWORD tempid)
{
	//�ڵز����ж�
	/*
	if(IsFull())
	{
		//TODO ��ʾ��
		return false;
	}
	// */
	if(pUser)
	{
		SceneUser *u = SceneUserManager::getMe().
						getUserByTempID(tempid);
		if(u )
		{
			if(team.addMember(u->id , tempid , u->name))
			{
				return true;
			}
		}
	}
	return false;
}

/**
 * \brief  ����id ɾ����Ա
 * \param  id ��ɫ��id
 */
void TeamManager::removeMemberByID(DWORD id)
{
	team.removeMemberByID(id);
}

/**
 * \brief ������ʱidɾ����Ա 
 * \param  tempid ��ʱid
 */
void TeamManager::removeMemberByTempID(DWORD tempid)
{
	team.removeMemberByTempID(tempid);
}

/**
 * \brief �����Ѻöȹ�ϵ
 */
void TeamManager::requestFriendDegree()
{
	team.requestFriendDegree();
}

/**
 * \brief  ���͵�ǰ�Ѻöȹ�ϵ�б��Ự�浵
 */
void TeamManager::sendFriendDegreeToSession()
{
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::Session::stCountMember * temp = NULL;

	Cmd::Session::t_CountFriendDegree_SceneSession *retCmd=(Cmd::Session::t_CountFriendDegree_SceneSession *)buf;
	strncpy(retCmd->name, me->name, MAX_NAMESIZE);
	constructInPlace(retCmd);
	retCmd->size = 0;
	temp = retCmd->namelist;

	if (!friendList.empty())
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = friendList.begin(); sIterator!= friendList.end(); sIterator++)
		{
			temp->dwUserID = sIterator->second.dwUserID;
			temp->wdDegree = sIterator->second.wdDegree;
			temp->byType   = Cmd::RELATION_TYPE_FRIEND;
			retCmd->size++;
			temp++;
		}
	}

	if (!consortList.empty())
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = consortList.begin(); sIterator!= consortList.end(); sIterator++)
		{
			temp->dwUserID = sIterator->second.dwUserID;
			temp->wdDegree = sIterator->second.wdDegree;
			temp->byType   = Cmd::RELATION_TYPE_LOVE;
			retCmd->size++;
			temp++;
		}
	}

	if (!teacherList.empty())
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = teacherList.begin(); sIterator!= teacherList.end(); sIterator++)
		{
			temp->dwUserID = sIterator->second.dwUserID;
			temp->wdDegree = sIterator->second.wdDegree;
			temp->byType   = Cmd::RELATION_TYPE_TEACHER;
			retCmd->size++;
			temp++;
		}
	}

	if (!prenticeList.empty())
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = prenticeList.begin(); sIterator!= prenticeList.end(); sIterator++)
		{
			temp->dwUserID = sIterator->second.dwUserID;
			temp->wdDegree = sIterator->second.wdDegree;
			temp->byType   = Cmd::RELATION_TYPE_TEACHER;
			retCmd->size++;
			temp++;
		}
	}
	if (0 != retCmd->size) sessionClient->sendCmd(retCmd, sizeof(Cmd::Session::t_CountFriendDegree_SceneSession)+retCmd->size*sizeof(Cmd::Session::stCountMember));
}

/**
 * \brief  �����Ѻö����̰ѵ�ǰ���Ѻö�ֵ�滻���������Ĺ�ϵ�嵥�У���Ϊ��ǰֵ�����µģ�
		   Ȼ�����֮ǰ�Ĺ�ϵ�б��ٰ��յ��Ĺ�ϵ�嵥���뵽��ϵ�б���
 * \param  rev �Ѻöȹ�ϵ�б���Ϣ
 */
void TeamManager::setFriendDegree(Cmd::Session::t_ReturnFriendDegree_SceneSession *rev)
{
	relationlock.wrlock();

	if (NULL == rev)
	{
		friendList.clear();   // �����б�
		consortList.clear();  // �����б�
		teacherList.clear();  // ʦ���б�
		prenticeList.clear(); // ͽ���б�

	}
	else if (0 == rev->size)
	{
		friendList.clear();   // �����б�
		consortList.clear();  // �����б�
		teacherList.clear();  // ʦ���б�
		prenticeList.clear(); // ͽ���б�
	}
	else
	{
		std::list<stTempDegreeMember> templist;

		for(int i=0; i<rev->size; i++)
		{
			stTempDegreeMember unit;
			unit.dwUserID = rev->memberlist[i].dwUserID;
			unit.wdDegree = rev->memberlist[i].wdDegree;
			unit.byType   = rev->memberlist[i].byType;
			unit.wdTime   = 0;
			templist.push_back(unit);
		}

		if (!friendList.empty())   // �����б�
		{
			std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
			for(sIterator = friendList.begin(); sIterator!= friendList.end(); sIterator++)
			{
				std::list<stTempDegreeMember>::iterator tIterator;
				for(tIterator = templist.begin(); tIterator!=templist.end(); tIterator++)
				{
					if ((sIterator->second.dwUserID == tIterator->dwUserID) &&
						(Cmd::Session::TYPE_FRIEND == tIterator->byType))
					{
						tIterator->wdDegree = sIterator->second.wdDegree;
						tIterator->wdTime		= sIterator->second.wdTime;
					}
				}
			}
		}
		if (!consortList.empty())  // �����б�
		{
			std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
			for(sIterator = consortList.begin(); sIterator!= consortList.end(); sIterator++)
			{
				std::list<stTempDegreeMember>::iterator tIterator;
				for(tIterator = templist.begin(); tIterator!=templist.end(); tIterator++)
				{
					if ((sIterator->second.dwUserID == tIterator->dwUserID) &&
						(Cmd::Session::TYPE_CONSORT == tIterator->byType))
					{
						tIterator->wdDegree = sIterator->second.wdDegree;
						tIterator->wdTime		= sIterator->second.wdTime;
					}
				}
			}
		}
		if (!teacherList.empty())  // ʦ���б�
		{
			std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
			for(sIterator = teacherList.begin(); sIterator!= teacherList.end(); sIterator++)
			{
				std::list<stTempDegreeMember>::iterator tIterator;
				for(tIterator = templist.begin(); tIterator!=templist.end(); tIterator++)
				{
					if ((sIterator->second.dwUserID == tIterator->dwUserID) &&
						(Cmd::Session::TYPE_TEACHER == tIterator->byType))
					{
						tIterator->wdDegree = sIterator->second.wdDegree;
						tIterator->wdTime		= sIterator->second.wdTime;
					}
				}
			}
		}
		if (!prenticeList.empty()) // ͽ���б�
		{
			std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
			for(sIterator = prenticeList.begin(); sIterator!= prenticeList.end(); sIterator++)
			{
				std::list<stTempDegreeMember>::iterator tIterator;
				for(tIterator = templist.begin(); tIterator!=templist.end(); tIterator++)
				{
					if ((sIterator->second.dwUserID == tIterator->dwUserID) &&
						(Cmd::Session::TYPE_PRENTICE == tIterator->byType))
					{
						tIterator->wdDegree = sIterator->second.wdDegree;
						tIterator->wdTime		= sIterator->second.wdTime;
					}
				}
			}
		}

		friendList.clear();   // �����б�
		consortList.clear();  // �����б�
		teacherList.clear();  // ʦ���б�
		prenticeList.clear(); // ͽ���б�

		std::list<stTempDegreeMember>::iterator tIterator;
		for(tIterator = templist.begin(); tIterator!=templist.end(); tIterator++)
		{
			stDegreeMember member;
	
			member.dwUserID = tIterator->dwUserID;
			member.wdDegree = tIterator->wdDegree;
			member.wdTime   = tIterator->wdTime;
			switch(tIterator->byType)
			{
				case Cmd::Session::TYPE_FRIEND:
					{
						friendList.insert(insValueType(member.wdDegree, member));
					}
					break;
				case Cmd::Session::TYPE_CONSORT:
					{
						consortList.insert(insValueType(member.wdDegree, member));
					}
					break;
				case Cmd::Session::TYPE_TEACHER:
					{
						teacherList.insert(insValueType(member.wdDegree, member));
					}
					break;
				case Cmd::Session::TYPE_PRENTICE:
					{
						prenticeList.insert(insValueType(member.wdDegree, member));
					}
					break;
				default:
					break;
			}
		}
	}
	relationlock.unlock();
}

/**
 * \brief  �����Ǯ����
 * \param  wdMoney �������ն��
 */
void TeamManager::putMoneyPlus(WORD wdMoney)
{
	if (0 == team.leader) return;

	relationlock.rdlock();
	if (!teacherList.empty())
	{
		if (me && (me->charbase.level <40))
		{
			SceneUser * pMaster = NULL;
			pMaster = SceneUserManager::getMe().getUserByID(teacherList.begin()->second.dwUserID);
			if (pMaster)
			{
				pMaster->packs.addMoney((WORD)(wdMoney*(FRIENDDEGREE_MONEYPLUS_RATE/100.0f)), "������⽱��","��ö����Ǯ����");
			}
		}
	}
	relationlock.unlock();
}


/**
 * \brief  ��ȡ�Ѻö���ظ��Ӿ���ֵ
 * \param  wdExp �������ն��
 */
DWORD TeamManager::getExpPlus(WORD wdExp)
{
	DWORD Exp = 0;
	if (0 == team.leader) return 0;

	relationlock.rdlock();
	if (!teacherList.empty())
	{
		if (me && (me->charbase.level <40))
		{
			SceneUser * pMaster = NULL, *pLeader = NULL;
			pMaster = SceneUserManager::getMe().getUserByID(teacherList.begin()->second.dwUserID);
			pLeader = SceneUserManager::getMe().getUserByTempID(team.leader);
			if (pMaster && pLeader)
			{
				// TODO ������ļ��㹫ʽ�ĳ�һ����
				Exp = (DWORD)((wdExp * (((pMaster->charbase.level / 30.0f)+1.0f) / pLeader->team.getAverageExp()) + 0.9)*(FRIENDDEGREE_EXPPLUS_RATE/100.0f));
				if (0 == Exp) Exp = 1;
			}
		}
	}
	relationlock.unlock();
	return Exp;
}

/**
 * \brief  ��ȡ�Ѻöȹ�������ֵ
 * \return ����ֵ
 */
DWORD TeamManager::getAttPlus()
{
	DWORD teacherAttPlus = 0;
	DWORD prenticeID = 0;
	DWORD friendAttPlus = 0;
	DWORD friendID = 0;

	if (0 == team.leader) return 0;

	relationlock.rdlock();

	if (!prenticeList.empty())
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = prenticeList.begin(); sIterator!= prenticeList.end(); sIterator++)
		{
			SceneUser * otherUser = NULL;
			otherUser = SceneUserManager::getMe().getUserByID(sIterator->second.dwUserID);
			if (otherUser)
			{
				if (otherUser->scene->zPosShortRange(me->getPos(), otherUser->getPos(),FRIENDDEGREE_VALIDATION_DISTANCE_WIDE, FRIENDDEGREE_VALIDATION_DISTANCE_HEIGHT))
				{
					if ((sIterator->second.wdDegree>= FRIENDDEGREE_RANGE_BTM) &&(sIterator->second.wdDegree<FRIENDDEGREE_RANGE_MED))
					{
						if (zMisc::selectByPercent(FRIENDDEGREE_TEACHER_RATE_BTM))
						{
							teacherAttPlus =  FRIENDDEGREE_TEACHER_ATTPLUS_RATE_BTM;
							prenticeID = sIterator->second.dwUserID;
						}
						break;
					}
					else if ((sIterator->second.wdDegree>= FRIENDDEGREE_RANGE_MED) &&(sIterator->second.wdDegree<FRIENDDEGREE_RANGE_TOP))
					{
						if (zMisc::selectByPercent(FRIENDDEGREE_TEACHER_RATE_MED))
						{
							teacherAttPlus =  FRIENDDEGREE_TEACHER_ATTPLUS_RATE_MED;
							prenticeID = sIterator->second.dwUserID;
						}
						break;
					}
					else if (sIterator->second.wdDegree >= FRIENDDEGREE_RANGE_TOP)
					{
						if (zMisc::selectByPercent(FRIENDDEGREE_TEACHER_RATE_TOP))
						{
							teacherAttPlus =  FRIENDDEGREE_TEACHER_ATTPLUS_RATE_TOP;
							prenticeID = sIterator->second.dwUserID;
						}
						break;
					}
				}
			}
		}
	}

	if (!friendList.empty())
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = friendList.begin(); sIterator!= friendList.end(); sIterator++)
		{
			SceneUser * otherUser = NULL;
			otherUser = SceneUserManager::getMe().getUserByID(sIterator->second.dwUserID);
			if (otherUser)
			{
				if (otherUser->scene->zPosShortRange(me->getPos(), otherUser->getPos(),FRIENDDEGREE_VALIDATION_DISTANCE_WIDE,FRIENDDEGREE_VALIDATION_DISTANCE_HEIGHT))
				{
					if ((sIterator->second.wdDegree>= FRIENDDEGREE_RANGE_BTM) &&(sIterator->second.wdDegree<FRIENDDEGREE_RANGE_MED))
					{
						if (zMisc::selectByPercent(FRIENDDEGREE_FRIEND_RATE_BTM))
						{
							friendAttPlus =  FRIENDDEGREE_FRIEND_ATTPLUS_RATE_BTM;
							friendID = sIterator->second.dwUserID;
						}
						break;
					}
					else if ((sIterator->second.wdDegree>= FRIENDDEGREE_RANGE_MED) &&(sIterator->second.wdDegree<FRIENDDEGREE_RANGE_TOP))
					{
						if (zMisc::selectByPercent(FRIENDDEGREE_FRIEND_RATE_MED))
						{
							friendAttPlus =  FRIENDDEGREE_FRIEND_ATTPLUS_RATE_MED;
							friendID = sIterator->second.dwUserID;
						}
						break;
					}
					else if (sIterator->second.wdDegree >= FRIENDDEGREE_RANGE_TOP)
					{
						if (zMisc::selectByPercent(FRIENDDEGREE_FRIEND_RATE_TOP))
						{
							friendAttPlus =  FRIENDDEGREE_FRIEND_ATTPLUS_RATE_TOP;
							friendID = sIterator->second.dwUserID;
						}
						break;
					}
				}
			}
		}
	}

	relationlock.unlock();

	if ((0 == teacherAttPlus) && (0 == friendAttPlus))
	{
		return 0;
	}
	else if ((0 != teacherAttPlus) && (0 == friendAttPlus))
	{
		return teacherAttPlus;
	}
	else if ((0 == teacherAttPlus) && (0 != friendAttPlus))
	{
		return friendAttPlus;
	}
	else if ((0 != teacherAttPlus) && (0 != friendAttPlus))
	{
		if (prenticeID == friendID)
		{
			if (teacherAttPlus > friendAttPlus)
			{
				return teacherAttPlus;
			}
			else
			{
				return friendAttPlus;
			}
		}
		else
		{
			return friendAttPlus+teacherAttPlus;
		}
	}
	return 0;
}

/**
 * \brief  ��ȡ�Ѻöȷ�������ֵ
 * \return ����ֵ
 */
DWORD TeamManager::getDefPlus()
{
	DWORD prenticeAttPlus = 0;
	DWORD teacherID = 0;
	DWORD consortAttPlus = 0;
	DWORD consortID = 0;

	if (0 == team.leader) return 0;

	relationlock.rdlock();

	if (!teacherList.empty())
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		sIterator = teacherList.begin();
		SceneUser * otherUser = NULL;
		otherUser = SceneUserManager::getMe().getUserByID(sIterator->second.dwUserID);
		if (otherUser)
		{
			if (otherUser->scene->zPosShortRange(me->getPos(), otherUser->getPos(),FRIENDDEGREE_VALIDATION_DISTANCE_WIDE, FRIENDDEGREE_VALIDATION_DISTANCE_HEIGHT))
			{
				if ((sIterator->second.wdDegree>= FRIENDDEGREE_RANGE_BTM) &&(sIterator->second.wdDegree<FRIENDDEGREE_RANGE_MED))
				{
					if (zMisc::selectByPercent(FRIENDDEGREE_TEACHER_RATE_BTM))
					{
						prenticeAttPlus =  FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_BTM;
						teacherID = sIterator->second.dwUserID;
					}
				}
				else if ((sIterator->second.wdDegree>= FRIENDDEGREE_RANGE_MED) &&(sIterator->second.wdDegree<FRIENDDEGREE_RANGE_TOP))
				{
					if (zMisc::selectByPercent(FRIENDDEGREE_TEACHER_RATE_MED))
					{
						prenticeAttPlus =  FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_MED;
						teacherID = sIterator->second.dwUserID;
					}
				}
				else if (sIterator->second.wdDegree >= FRIENDDEGREE_RANGE_TOP)
				{
					if (zMisc::selectByPercent(FRIENDDEGREE_TEACHER_RATE_TOP))
					{
						prenticeAttPlus =  FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_TOP;
						teacherID = sIterator->second.dwUserID;
					}
				}
			}
		}
	}

	if (!consortList.empty())
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		sIterator = consortList.begin();

		SceneUser * otherUser = NULL;
		otherUser = SceneUserManager::getMe().getUserByID(sIterator->second.dwUserID);
		if (otherUser)
		{
			if (me->scene->zPosShortRange(me->getPos(), otherUser->getPos(),FRIENDDEGREE_VALIDATION_DISTANCE_WIDE, FRIENDDEGREE_VALIDATION_DISTANCE_HEIGHT))
			{
				if ((sIterator->second.wdDegree>= FRIENDDEGREE_RANGE_BTM) &&(sIterator->second.wdDegree<FRIENDDEGREE_RANGE_MED))
				{
					if (zMisc::selectByPercent(FRIENDDEGREE_CONSORT_RATE_BTM))
					{
						consortAttPlus =  FRIENDDEGREE_CONSORT_DEFPLUS_RATE_BTM;
						consortID = sIterator->second.dwUserID;
					}
				}
				else if ((sIterator->second.wdDegree>= FRIENDDEGREE_RANGE_MED) &&(sIterator->second.wdDegree<FRIENDDEGREE_RANGE_TOP))
				{
					if (zMisc::selectByPercent(FRIENDDEGREE_CONSORT_RATE_MED))
					{
						consortAttPlus =  FRIENDDEGREE_CONSORT_DEFPLUS_RATE_MED;
						consortID = sIterator->second.dwUserID;
					}
				}
				else if (sIterator->second.wdDegree >= FRIENDDEGREE_RANGE_TOP)
				{
					if (zMisc::selectByPercent(FRIENDDEGREE_CONSORT_RATE_TOP))
					{
						consortAttPlus =  FRIENDDEGREE_CONSORT_DEFPLUS_RATE_TOP;
						consortID = sIterator->second.dwUserID;
					}
				}
			}
		}
	}

	relationlock.unlock();

	if ((0 == prenticeAttPlus) && (0 == consortAttPlus))
	{
		return 0;
	}
	else if ((0 != prenticeAttPlus) && (0 == consortAttPlus))
	{
		return prenticeAttPlus;
	}
	else if ((0 == prenticeAttPlus) && (0 != consortAttPlus))
	{
		return consortAttPlus;
	}
	else if ((0 != prenticeAttPlus) && (0 != consortAttPlus))
	{
		if (teacherID == consortID)
		{
			if (prenticeAttPlus > consortAttPlus)
			{
				return prenticeAttPlus;
			}
			else
			{
				return consortAttPlus;
			}
		}
		else
		{
			return consortAttPlus+prenticeAttPlus;
		}
	}
	return 0;
}

/**
 * \brief  ʵʱ�ۼӼ������й�ϵ���Ѻöȹ�ϵ
 */
void TeamManager::countFriendDegree()
{
	if (0 == team.leader) return;

	bool oper = false;

	relationlock.rdlock();
	if (!friendList.empty())   // �����б�
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = friendList.begin(); sIterator!= friendList.end(); sIterator++)
		{
			sIterator->second.wdTime++;
			if (60 <= sIterator->second.wdTime)
			{
				sIterator->second.wdTime=0;
				sIterator->second.wdDegree+=60;
				oper = true;
			}
		}
	}
	if (!consortList.empty())  // �����б�
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = consortList.begin(); sIterator!= consortList.end(); sIterator++)
		{
			sIterator->second.wdTime++;
			if (60 <= sIterator->second.wdTime)
			{
				sIterator->second.wdTime=0;
				sIterator->second.wdDegree+=60;
				oper = true;
			}
		}
	}
	if (!teacherList.empty())  // ʦ���б�
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = teacherList.begin(); sIterator!= teacherList.end(); sIterator++)
		{
			sIterator->second.wdTime++;
			if (60 <= sIterator->second.wdTime)
			{
				sIterator->second.wdTime=0;
				sIterator->second.wdDegree+=60;
				oper = true;
			}
		}
	}
	if (!prenticeList.empty()) // ͽ���б�
	{
		std::map<WORD, struct stDegreeMember,ltword>::iterator sIterator;
		for(sIterator = prenticeList.begin(); sIterator!= prenticeList.end(); sIterator++)
		{
			sIterator->second.wdTime++;
			if (60 <= sIterator->second.wdTime)
			{
				sIterator->second.wdTime=0;
				sIterator->second.wdDegree+=60;
				oper = true;
			}
		}
	}
	if (oper) sendFriendDegreeToSession();
	relationlock.unlock();
}

/**
 * \brief  ������ʱ�浵���ݼ��ض���
 * \param  leader �ӳ�
 * \param  data ��ʱ�浵����
 * \return true ���سɹ�  false ����ʧ��
 */
bool TeamManager::loadTeam(SceneUser *leader , TempArchiveMember *data)
{
	//����ͼ�������
	if(leader->scene->noTeam())
	{
		return false;
	}
	if(!leader->team.addNewLeader(leader))
	{
		return false;
	}
	int size = data->size - sizeof(DWORD) - sizeof(DWORD) - sizeof(BYTE) - sizeof(BYTE);
	leader->team_mode = *(DWORD*)&data->data[size];
	Cmd::stTeamModeUserCmd tmu; 
	tmu.byType=leader->team_mode;
	leader->sendCmdToMe(&tmu , sizeof(tmu));
	leader->team.setNextObjOwnerID(*(DWORD*)&data->data[size+sizeof(DWORD)]);
	leader->team.setObjMode(*(BYTE*)&data->data[size+sizeof(DWORD)+sizeof(DWORD)]);
	leader->team.setExpMode(*(BYTE*)&data->data[size+sizeof(DWORD)+sizeof(DWORD)+sizeof(BYTE)]);
	struct PairIDAndName
	{
		DWORD id;
		char name[MAX_NAMESIZE];
	};
	PairIDAndName *pair = NULL;
	while((int)size > 0)
	{
		pair = (PairIDAndName*)&data->data[data->size -sizeof(DWORD) - sizeof(DWORD) - sizeof(BYTE) - sizeof(BYTE) - size]; 
		size -= sizeof(PairIDAndName);
		SceneUser *u = SceneUserManager::getMe().
			getUserByID(pair->id);
		if(u)
		{
			if(!u->team.IsTeamed())
			{
				//Zebra::logger->debug("����%u�ָ���Ա�ҵ���Ա,id=%u , tempid=%u , name=%s" , leader->tempid , u->id , u->tempid , u->name);
				if(leader->team.IsFull())
				{
					break;
				}
				addNewMember(leader , u);
			}
			//team.addMember(u->id , u->tempid , u->name);
		}
		else if(pair->id)
		{
			team.addMember(pair->id , 0 , pair->name);
		}
		//Zebra::logger->debug("�ָ������Ա,id=%u , name=%s" , pair->id , pair->name);
		//size -= (int)((size - sizeof(DWORD)) > 0) ? size - sizeof(DWORD) : 0;
	}
	if(leader->team.getSize() == 1)
	{
		leader->team.deleteTeam(leader);
	}
	//Zebra::logger->debug("load�����Ա����=%u" , leader->team.getSize());
	return true;
}

/**
 * \brief  ����һ����Ա���Ự
 * \param  leaderid �ӳ� id
 * \param  userid ���ӵĳ�Ա�� id
 * \return true Ϊ��Ϣ���ͳɹ� false ��Ϣ����ʧ��
 */
bool TeamManager::addMemberToSession(DWORD leaderid , DWORD userid)
{
	Cmd::Session::t_Team_AddMember add;
	add.dwLeaderID = leaderid;
	add.dwMemberID = userid;
	sessionClient->sendCmd(&add , sizeof(Cmd::Session::t_Team_AddMember));
	return true;
}

/**
 * \brief  ��Ự���ͳ�Աɾ����Ϣ
 * \param  leaderid �ӳ���id
 * \param  userid ��ɾ����ɫ��id
 * \return true Ϊ��Ϣ���ͳɹ� false ��Ϣ����ʧ��
 */
bool TeamManager::delMemberToSession(DWORD leaderid , DWORD userid)
{
	Cmd::Session::t_Team_DelMember del;
	del.dwLeaderID = leaderid;
	del.dwMemberID = userid;
	sessionClient->sendCmd(&del , sizeof(Cmd::Session::t_Team_DelMember));
	return true;
}
void TeamManager::decreaseAverageExp(SceneUser *leader , SceneUser *pUser)
{
	team.decreaseAverageExp(pUser);
}
void TeamManager::decreaseAverageExp(DWORD leaderid , SceneUser *pUser)
{
	SceneUser *leader = SceneUserManager::getMe().
		getUserByID(leaderid);
	if(leader)
	{
		leader->team.decreaseAverageExp(leader , pUser);
	}
}
struct ExpSizeExec : public TeamMemExec
{
	int size;
	zPosI pos;
	DWORD sceneid;
	ExpSizeExec(zPosI p,DWORD s)
	{
		pos = p;
		size = 0;
		sceneid = s;
	}
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().
						getUserByTempID(member.tempid);
		if(pUser)
		{
			if(pUser->scene->id == sceneid && pUser->scene->checkTwoPosIInNine(pUser->getPosI(),pos))
			{
				//Zebra::logger->debug("���9����Ա����ɹ�(%d,%d)(%d,%d)",pos,pUser->getPosI(),pUser->getPos().x,pUser->getPos().y);
				size++;
			}
			else
			{
				//Zebra::logger->debug("���9����Ա����ʧ��(%d,%d)(%d,%d)",pos,pUser->getPosI(),pUser->getPos().x,pUser->getPos().y);
			}
		}
		return true;
	}
};
int TeamManager::getExpSize(zPosI pos , DWORD sceneid)
{
	ExpSizeExec exec(pos,sceneid);
	team.execEvery(exec);
	return exec.size;
}
void TeamManager::calAverageExp(zPosI pos,DWORD sceneid)
{
	team.calAverageExp(pos,sceneid);
}
