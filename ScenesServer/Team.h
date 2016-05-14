#ifndef _SCENETEAM_H_
#define _SCENETEAM_H_


#include <vector>
#include "zType.h"
#include "zEntry.h"
#include "zRWLock.h"
#include "zSceneEntry.h"

//����ģʽ��Ҫ�ĵȼ�����
#define TEAM_HONOR_MEMBER_LEVEL 50
#define TEAM_HONOR_LEADER_LEVEL 31

/**
 * \brief ��ӳ�Ա�ṹ
 */
struct TeamMember 
{
	DWORD id;
	DWORD tempid;
	char name[MAX_NAMESIZE + 1];
	DWORD offtime;
	DWORD begintime;

	/**
	 * \brief �����ʼ���ṹ
	 */
	TeamMember()
	{
		id = 0;
		tempid = 0;
		bzero(name , sizeof(name));
		offtime = 0;
	}
};

/**
 * \brief �����������
 */
struct TeamMemExec
{
	virtual ~TeamMemExec(){};
	virtual bool exec(TeamMember &member) = 0;
};

struct SceneUser;

struct Team
{
	/// ��������Ա��
	static const  int MaxMember = 6;

	/**
	 * \brief ���캯����ʼ���������
	 */
	Team()
	{
		isOpen = true;
		leader = 0;
		averageExp = 0;
		current_obj_owner=0; 
	}

	/// ������ӱ�־
	bool isOpen;

	/// ����Ƕӳ���tempid ���Ҫ�ı���֪ͨ�����ȫ���Ա
	DWORD leader; 
	//������Ʒ����ʱ��ǰ��������
	DWORD current_obj_owner;

	/// ƽ���������
	DWORD averageExp;
	private:
	friend class TeamCondition;
	friend class TeamedCondition;
	friend class TeamAction;
	friend class QuestList;
	friend class SceneNpc;

	/// �����Ա��
	std::vector<TeamMember> member;

	/// ���߳�Ա��ʱ�ǼǱ�
	std::vector<TeamMember> offline_member;

	/// �����д��
	zRWLock rwlock;
	
	public:
	/**
	 * \brief ��ȡ��������
	 */
	int getSize()
	{
		return member.size();
	}

	/**
	 * \brief ������еĶ�Ա
	 */
	void Clear()
	{
		//rwlock.wrlock();
		member.clear();
		//rwlock.unlock();
	}
	DWORD getNextObjOwnerID();
	void setNextObjOwnerID(DWORD id)
	{
		current_obj_owner=id;
	}
	void userupgrade(SceneUser *pUser);
	void decreaseAverageExp(SceneUser *pUser);
	void increaseAverageExp(SceneUser *pUser);
	void calAverageExp(zPosI pos,DWORD sceneid);
	bool addMember(DWORD id , DWORD tempid , const char *pname);
	void removeMember(DWORD id , DWORD tempid);
	void removeMemberByID(DWORD id);
	DWORD removeMemberByName(const char *pname);
	void removeMemberByTempID(DWORD tempid);
	void execEvery(TeamMemExec &callback);
	void execEveryExceptMe(TeamMemExec &callback , DWORD tempid);
	void requestFriendDegree();
};



#endif
