/**
 * \file
 * \version  $Id: GateUser.h  $
 * \author  
 * \date 
 * \brief ���������û���
 */

#ifndef _GATEUSER_H_
#define _GATEUSER_H_

#include <set>
#include "zUser.h"
#include "Command.h"
#include "RecordCommand.h"
#include "Zebra.h"
#include "zRWLock.h"

class GateUserManager;

/**
 * \brief ��ɫѡ��
 *
 */
class GateSelectUserSession:private zNoncopyable
{
	friend class GateUserAccountID;
	protected:
	Cmd::SelectUserInfo userinfo[Cmd::MAX_CHARINFO];

	GateSelectUserSession(DWORD accid)
	{
		this->accid=accid;
		bzero(userinfo,sizeof(userinfo));
	}
	public:
	DWORD accid;
	WORD face;

	void setSelectUserInfo(const Cmd::Record::t_Ret_SelectInfo_GateRecord *ptCmd);

	void putSelectUserInfo(const Cmd::SelectUserInfo &info);

	/**
	 * \brief ɾ��ѡ��Ľ�ɫ
	 *
	 *
	 * \param charid: ��ɫid
	 * \return ɾ���Ƿ�ɹ�
	 */
	bool delSelectUserInfo(DWORD charid)
	{
		bool empty=true;
		for(int i = 0; i < Cmd::MAX_CHARINFO; i++)
		{
			if(userinfo[i].id== charid && userinfo[i].id!=0 && userinfo[i].id!=(DWORD)-1)
			{
				bzero(&userinfo[i],sizeof(Cmd::SelectUserInfo));
			}
			if(userinfo[i].id!=0 && userinfo[i].id!=(DWORD)-1)
				empty=false;
		}
		return empty;
	}

	/**
	 * \brief ���ݽ�ɫ��ŵõ�һ����ɫ��Ϣ
	 *
	 *
	 * \param num: ��ɫ���
	 * \return ��ɫ��Ϣ
	 */
	Cmd::SelectUserInfo *getSelectUserInfo(WORD num)
	{
		if(num>=Cmd::MAX_CHARINFO)
			return NULL;
		return &userinfo[num];
	}

	/**
	 * \brief �жϽ�ɫ�Ƿ�ﵽ����ɫ����
	 *
	 *
	 * \return ��ɫ������ture,���򷵻�false
	 */
	bool charInfoFull()
	{
		bool retval = false;
		for(int i = 0; i < Cmd::MAX_CHARINFO; i++)
		{
			if (userinfo[i].id != 0 && userinfo[i].id != (DWORD)-1)
			{
				retval = true;
			}
		}
		/*
		bool retval = true;
		for(int i = 0; i < Cmd::MAX_CHARINFO; i++)
		{
			if (userinfo[i].id == 0 || userinfo[i].id == (DWORD)-1)
			{
				retval = false;
			}
		}
		// */
		return retval;
	}
};

class GatewayTask;
class SceneClient;
/**
 * \brief �����û�
 *
 */
class GateUser:public zUser,public GateSelectUserSession
{
	friend class GatewayTask;
	private:

	/// �û������˳�״̬(socket�Ѿ��Ͽ�)
	bool logout;
	GatewayTask *gatewaytask;
	/// �ĸ�״̬�����ص�
	enum Systemstate
	{
		SYSTEM_STATE_INITING,		/// ��ʼ״̬
		SYSTEM_STATE_CREATING,		/// ������ɫ״̬
		SYSTEM_STATE_SELECT,		/// ѡ���ɫ״̬
		SYSTEM_STATE_PLAY,			/// ��Ϸ״̬
		SYSTEM_WAIT_STATE_PLAY,		/// �ȴ���Ϸ״̬
		SYSTEM_WAIT_STATE_UNREG		/// �ȴ��˳���ɫ����
	};
	volatile Systemstate systemstate;

	bool quiz;
	/// ��֤��
	char jpegPassport[5];
	/// �������б� 
	std::set<std::string> blacklist; 
	typedef std::set<std::string>::value_type blackListValueType;
	zRWLock rwlock;

	public:
	/// ����һ�´�����ɫָ��
	Cmd::Record::t_CreateChar_GateRecord createCharCmd;

	bool backSelect;
	SceneClient *scene;
	DWORD sceneTempID;

	DWORD countryID;
	DWORD sceneID;

	GateUser(DWORD accID,GatewayTask *histask);
	~GateUser();

	/**
	 * \brief ��ʼ��״̬����
	 *
	 */
	void initState()
	{
		systemstate = SYSTEM_STATE_INITING;
	}
	/**
	 * \brief �Ƿ���ɳ�ʼ��
	 *
	 *
	 * \return �����ɷ���ture,����false
	 */
	bool isInitState() const
	{
		return SYSTEM_STATE_INITING == systemstate;
	}
	/**
	 * \brief ������ɫ״̬
	 *
	 *
	 */
	void createState()
	{
		systemstate = SYSTEM_STATE_CREATING;
	}
	/**
	 * \brief �Ƿ��ڴ�����ɫ״̬
	 *
	 *
	 * \return ����ڴ�����ɫ״̬����ture,���򷵻�false
	 */
	bool isCreateState() const
	{
		return SYSTEM_STATE_CREATING == systemstate;
	}
	/**
	 * \brief ����ѡ���ɫ״̬
	 *
	 *
	 */
	void selectState()
	{
		systemstate=SYSTEM_STATE_SELECT;
	}
	/**
	 * \brief �Ƿ��ڵȴ��˳���ɫ״̬
	 *
	 *
	 * \return ���˳���ɫ״̬����ture,���򷵻�false
	 */
	bool isWaitUnregState() const
	{
		return SYSTEM_WAIT_STATE_UNREG == systemstate;
	}

	
	/**
	 * \brief �����˳��ȴ�״̬
	 *
	 *
	 */
	void waitUnregState()
	{
		systemstate = SYSTEM_WAIT_STATE_UNREG;
	}
	/**
	 * \brief �Ƿ���ѡ���ɫ״̬
	 *
	 *
	 * \return ��ѡ��״̬����ture,���򷵻�false
	 */
	bool isSelectState() const
	{
		return SYSTEM_STATE_SELECT == systemstate;
	}

	
	void playState(SceneClient *s=NULL , DWORD scene_tempid=0);

	/**
	 * \brief ���þ���״̬
	 *
	 */
	void quizState()
	{
		quiz = true;
	}
	
	/**
	 * \brief �������״̬
	 *
	 */
	void clearQuizState()
	{
		quiz = false;
	}
	
	/**
	 * \brief �Ƿ��ھ���״̬
	 *
	 */
	bool isQuizState()
	{
		return quiz;
	}

	/**
	 * \brief �Ƿ�����Ϸ״̬
	 *
	 *
	 * \return ����Ϸ״̬����ture,���򷵻�false
	 */
	bool isPlayState() const
	{
		return SYSTEM_STATE_PLAY == systemstate;
	}

	/**
	 * \brief �Ƿ��ڵȴ���Ϸ״̬
	 * ����һ��״̬:���ظո��յ�ѡ���ɫָ����յ��˳�ָ��,���ܳ���"id����ʹ��"�����
	 *
	 *
	 * \return ����Ϸ״̬����ture,���򷵻�false
	 */
	bool isWaitPlayState() const
	{
		return SYSTEM_WAIT_STATE_PLAY == systemstate;
	}
	/**
	 * \brief ���õȴ���Ϸ״̬
	 *
	 *
	 */
	void waitPlayState()
	{
		systemstate = SYSTEM_WAIT_STATE_PLAY;
	}

	void Terminate();
	void refreshCharInfo();
	bool checkPassport(const char *passport);
	/**
	 * \brief ֪ͨ�ͻ���û�д����Ľ�ɫ
	 *
	 */
	void noCharInfo()
	{
		using namespace Cmd;
		stServerReturnLoginFailedCmd cmd;
		cmd.byReturnCode=LOGIN_RETURN_USERDATANOEXIST;
		if (sendCmd(&cmd, sizeof(cmd)))
		{
			selectState();
		}
	}

	/**
	 * \brief ֪ͨ�ͻ��������ظ�
	 *
	 */
	void nameRepeat()
	{
		using namespace Cmd;
		stServerReturnLoginFailedCmd cmd;
		cmd.byReturnCode=LOGIN_RETURN_CHARNAMEREPEAT;
		if (sendCmd(&cmd, sizeof(cmd)))
		{
			selectState();
		}
	}
	
	/**
	 * \brief ��ȡ�ʺ�
	 *
	 */
	const char* getAccount();

	/**
	 * \brief ����Ϣת��������
	 *
	 */
	bool forwardScene(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
	bool forwardSceneBill(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
	bool forwardBill(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
	bool forwardBillScene(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
	void setVip(bool b);
		
	bool beginSelect();
	void reg(int charno);
	void unreg(bool out=false);
	bool beginGame();
	bool sendCmd(const void *pstrCmd, const unsigned int nCmdLen,const unsigned int type=0,const char *strName=NULL,const bool hasPacked=false);
	static void cmdFilter(Cmd::stNullUserCmd *cmd,DWORD &type,char *name,DWORD &cmdLen);
	void final();
	void addBlackList(const char *);
	void removeBlackList(const char *);
	bool checkChatCmd(DWORD type, const char *strName);
	public:
	/// ��ͼ������hash key
	DWORD mapScreenIndex;
	/// �Ƿ�����
	bool hide;

	/// �Ƿ��Ѿ���ӵ�����
	bool inserted;

	/// ϵͳ����
	BYTE sysSetting[20];

	public:
	/**
	 * \brief ˢ���û�������
	 *
	 *
	 * \param screen �µ�������hashkey
	 * \return 
	 */
	void setIndexKey(const DWORD screen)
	{
		mapScreenIndex = screen; 	 
	}
	const DWORD getIndexKey() const
	{
		return mapScreenIndex;
	}
	bool isHide() const
	{
		return hide;
	}
};
#endif
