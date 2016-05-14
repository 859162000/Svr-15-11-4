#ifndef _TEAMMANAGER_H_
#define _TEAMMANAGER_H_

#include "Team.h"
#include "SessionCommand.h"
#include "zRWLock.h"

struct SceneUser;

/// �Ѻöȳ�Ա
struct stDegreeMember{
	DWORD dwUserID;
	WORD  wdDegree;
	WORD  wdTime;
};

/// �Ѻö���ʱ�ṹ
struct stTempDegreeMember{
	DWORD dwUserID;
	WORD  wdDegree;
	WORD  wdTime;
	BYTE  byType;
};

/// �Ѻöȷ�Χ 
const DWORD FRIENDDEGREE_RANGE_BTM = 3*60;

/// �Ѻöȷ�Χ
const DWORD FRIENDDEGREE_RANGE_MED = 15*60;
// �Ѻöȷ�Χ
const DWORD FRIENDDEGREE_RANGE_TOP = 55*60;

/// ���޷�������ٷֱ�1
const DWORD FRIENDDEGREE_CONSORT_DEFPLUS_RATE_TOP	= 8;	
/// ���޷�������ٷֱ�2
const DWORD FRIENDDEGREE_CONSORT_DEFPLUS_RATE_MED	= 5;	
/// ���޷�������ٷֱ�3
const DWORD FRIENDDEGREE_CONSORT_DEFPLUS_RATE_BTM	= 3;	

/// ͽ�ܷ�������ٷֱ�1
const DWORD FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_TOP	= 2;	
/// ͽ�ܷ�������ٷֱ�2
const DWORD FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_MED	= 2;	
/// ͽ�ܷ�������ٷֱ�3
const DWORD FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_BTM	= 2;	
/// ͽ�ܷ�������ٷֱ�100
const DWORD FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_FULL    = 100;

/// ʦ����������ٷֱ�1
const DWORD FRIENDDEGREE_TEACHER_ATTPLUS_RATE_TOP	= 4;	
/// ʦ����������ٷֱ�2
const DWORD FRIENDDEGREE_TEACHER_ATTPLUS_RATE_MED	= 4;	
/// ʦ����������ٷֱ�3
const DWORD FRIENDDEGREE_TEACHER_ATTPLUS_RATE_BTM	= 2;	

/// ���ѹ�������ٷֱ�1
const DWORD FRIENDDEGREE_FRIEND_ATTPLUS_RATE_TOP	= 8;	
/// ���ѹ�������ٷֱ�2
const DWORD FRIENDDEGREE_FRIEND_ATTPLUS_RATE_MED	= 5;	
/// ���ѹ�������ٷֱ�3
const DWORD FRIENDDEGREE_FRIEND_ATTPLUS_RATE_BTM	= 3;	

/// ��������ٷֱ�
const DWORD FRIENDDEGREE_EXPPLUS_RATE	= 5;	
/// ��Ǯ����ٷֱ�
const DWORD FRIENDDEGREE_MONEYPLUS_RATE = 20;  

/// ���ѹ�ϵ�������1
const DWORD FRIENDDEGREE_FRIEND_RATE_TOP	= 100;	
/// ���ѹ�ϵ�������2
const DWORD FRIENDDEGREE_FRIEND_RATE_MED	= 100;	
/// ���ѹ�ϵ�������3
const DWORD FRIENDDEGREE_FRIEND_RATE_BTM	= 100;	

/// ���޹�ϵ�������1
const DWORD FRIENDDEGREE_CONSORT_RATE_TOP	= 100;	
/// ���޹�ϵ�������2
const DWORD FRIENDDEGREE_CONSORT_RATE_MED	= 100;	
/// ���޹�ϵ�������3
const DWORD FRIENDDEGREE_CONSORT_RATE_BTM	= 100;	

/// ʦͽ��ϵ�������1
const DWORD FRIENDDEGREE_TEACHER_RATE_TOP	= 40;	
/// ʦͽ��ϵ�������2
const DWORD FRIENDDEGREE_TEACHER_RATE_MED	= 20;	
/// ʦͽ��ϵ�������3
const DWORD FRIENDDEGREE_TEACHER_RATE_BTM	= 20;	

/// �Ѻö�������Ч����
const DWORD FRIENDDEGREE_VALIDATION_DISTANCE_WIDE = SCREEN_WIDTH; 
/// �Ѻö�������Ч����
const DWORD FRIENDDEGREE_VALIDATION_DISTANCE_HEIGHT = SCREEN_HEIGHT; 


/**
 * \brief  ���������
 */
class TeamManager
{
	private:
		//��Ʒ����ģʽ
		BYTE obj_mode;
		//�������ģʽ
		BYTE exp_mode;
		// /�������
		Team team;

		/// ���򷽷�
		struct ltword
		{
			bool operator()(const WORD s1, const WORD s2) const
			{
				return s1>s2;
			}
		};

		/// �����б�
		std::map<WORD, struct stDegreeMember,ltword> friendList;

		/// �����б�
		std::map<WORD, struct stDegreeMember,ltword> consortList;

		/// ʦ���б�
		std::map<WORD, struct stDegreeMember,ltword> teacherList;

		/// ͽ���б�
		std::map<WORD, struct stDegreeMember,ltword> prenticeList;

		/// ����ϵ��д��
		zRWLock relationlock;

		/// ���Ͷ���
		typedef std::map<WORD, struct stDegreeMember, ltword>::value_type insValueType;

		/// ������ӵ����
		SceneUser * me;

		bool giveupstatus; //�󷨷�����־
		zRTime giveuptime; //�󷨷���ʱ�� 

	public:

		void setNextObjOwnerID(DWORD id)
		{
			team.setNextObjOwnerID(id);
		}
		DWORD getNextObjOwnerID()
		{
			return team.getNextObjOwnerID();
		}
		bool isSpecialObj()
		{
			return obj_mode == Cmd::TEAM_OBJ_MODE_SPECIAL;
		}
		bool isNormalObj()
		{
			return obj_mode == Cmd::TEAM_OBJ_MODE_NORMAL;
		}
		BYTE getObjMode()
		{
			return obj_mode;
		}
		void setObjMode(BYTE mode)
		{
			obj_mode = mode;
		}
		bool isSpecialExp()
		{
			return exp_mode == Cmd::TEAM_EXP_MODE_SPECIAL;
		}
		bool isNormalExp()
		{
			return exp_mode == Cmd::TEAM_EXP_MODE_NORMAL;
		}
		BYTE  getExpMode()
		{
			return exp_mode;
		}
		void setExpMode(BYTE mode)
		{
			exp_mode = mode;
		}
		/**
		 * \brief  �����ʼ��
		 * \param  pUser ���������ӵ����
		 */
		TeamManager(SceneUser *pUser)
		{
			me = pUser;
			giveupstatus = false;
			obj_mode=0;
			exp_mode=0;
		}

		/**
		 * \brief  �ж϶����Ƿ��Ѿ��ﵽ�������
		 * \return true �����Ѿ��ﵽ��������� false ��û��
		 */
		bool IsFull()
		{
			return team.getSize() == Team::MaxMember;
		}

		/**
		 * \brief  ��ȡ�����Ա����
		 * \return ����
		 */
		int getSize();
		
		int getExpSize(zPosI pos , DWORD sceneid);

		/**
		 * \brief  �Ƿ��������
		 * \return true ������� false ���������
		 */
		bool IsOpen()
		{
			return team.isOpen;
		}

		/**
		 * \brief  ����������ӱ�־
		 * \param  is ������ӱ�־
		 */
		void setOpen(bool is)
		{
			team.isOpen = is;
		}

		/**
		 * \brief  �Ƿ����
		 * \return true ��ӣ� false û�����
		 */
		bool IsTeamed()
		{
			return team.leader != 0;
		}

		/**
		 * \brief  ��ȡ�ӳ�����ʱid
		 * \return �ӳ���ʱid
		 */
		DWORD getLeader() const
		{
			return team.leader;
		}

		/**
		 * \brief  ��¼�ӳ�����ʱid
		 * \param  tempid �ص�����
		 */
		bool setLeader(DWORD tempid)
		{
			team.leader = tempid;
			return true;
		}

		/**
		 * \brief  ��ɫ�������ݱ������
		 * \param  pUser �����Ľ�ɫ
		 */
		void userupgrade(SceneUser *pUser)
		{
			team.userupgrade(pUser);
		}

		/**
		 * \brief  ��ö���ƽ�����侭��
		 * \return ƽ������
		 */
		DWORD getAverageExp()
		{
			return team.averageExp;
		}

		/**
		 * \brief  ��ö������
		 * \return �������
		 */
		const Team& getTeam() const
		{
			return team;
		}
		bool delMemberToSession(DWORD leaderid , DWORD userid);
		bool loadTeam(SceneUser *leader , TempArchiveMember *data);
		bool addNewLeader(SceneUser *pUser);
		bool IsOurTeam(SceneUser *pUser);
		bool IsOurTeam(DWORD dwID);
		bool addNewMember(SceneUser *pUser , Cmd::stAnswerTeamUserCmd *rev);
		bool addNewMember(SceneUser *leader , SceneUser *pUser);
		void removeTeam(SceneUser *pUser , DWORD tempid);
		void deleteTeam(SceneUser *leader);
		void removeMember(SceneUser *pUser , Cmd::stRemoveTeamMemberUserCmd *rev);
		void kickoutMember(SceneUser *pUser , Cmd::stRemoveTeamMemberUserCmd *rev);
		void removeMember(SceneUser *pUser , SceneUser *mem);
		void checkOffline();
		void sendtoTeamCharData(SceneUser * leader , SceneUser *user);
		void sendtoTeamCharData(SceneUser *user);
		void sendCmdToTeam(SceneUser *user , void *cmd , DWORD cmdLen);
		void checkOffline(SceneUser *pUser);
		bool changeLeader(SceneUser *pUser);
		void execEveryOne(TeamMemExec &callback);
		void execEveryOneExceptMe(TeamMemExec &callback , DWORD tempid);
		void decreaseAverageExp(SceneUser *leader , SceneUser *pUser);
		void decreaseAverageExp(DWORD leaderid , SceneUser *pUser);
		void calAverageExp(zPosI pos,DWORD sceneid);

		// ���ø��û����Ѻö��������ϵ
		void setFriendDegree(Cmd::Session::t_ReturnFriendDegree_SceneSession * = NULL);
		DWORD getExpPlus(WORD wdExp);
		DWORD getAttPlus();
		DWORD getDefPlus();
		void putMoneyPlus(WORD wdMoney);
		void countFriendDegree();
		void requestFriendDegree();
		bool canPutSkill();
	private:
		void sendFriendDegreeToSession();

		SceneUser *findNewLeader(SceneUser *pUser);
		bool addMemberByID(SceneUser *pUser , DWORD id);
		bool addMemberByTempID(SceneUser *pUser , DWORD tempid);
		void removeMemberByID(DWORD id);
		void removeMemberByTempID(DWORD tempid);
		bool addMemberToSession(DWORD leaderid , DWORD userid);


};



#endif



