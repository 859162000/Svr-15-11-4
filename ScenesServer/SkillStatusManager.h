/**
 * \file
 * \version  $Id: SkillStatusManager.h $
 * \author  
 * \date 
 * \brief ���弼��״̬������ͷ�ļ�
 *
 */

#ifndef __SKILLSTATUSMANAGER__
#define __SKILLSTATUSMANAGER__

#include "zType.h"
#include "zDatabase.h"
#include "zSceneEntry.h"

struct SceneEntryPk;
struct PkValue;
struct SkillState;

/// �����ƶ�����ö��
enum {
	SKILL_GOOD=0,
	SKILL_BAD=1
};

/// ����״̬����ö��
enum {
	SKILL_TYPE_INITIAVITE=1,	// ��������
	SKILL_TYPE_RECOVERY=2,		// ��ʱ��������
	SKILL_TYPE_PASSIVENESS=3	// ���ñ�������
};

/// ����״̬������ֵö��
enum {
	SKILL_ACTIVE	=	1,	//	�ӵ��MAP��
	SKILL_RECOVERY,			//  �ӵ���ʱ����MAP��
	SKILL_PASSIVENESS,		//	�ӵ����ñ���MAP��
	SKILL_RETURN,			//	���ӵ��κ�MAP��
	SKILL_BREAK,			//	������Ͷ�Ų���
};

/// ����״ִ̬�в���ö��
enum {
	ACTION_STEP_DOPASS=	0,	// һ������״̬�Ŀ�ʼ����
	ACTION_STEP_START=	1,	// һ������״̬�Ŀ�ʼ����
	ACTION_STEP_TIMER,		// һ������״̬�Ķ�ʱ����
	ACTION_STEP_STOP,		// һ������״̬��ֹͣ����
	ACTION_STEP_CLEAR,		// һ������״̬���ⲿ���
	ACTION_STEP_RELOAD		// һ������״̬����������
};

/// ����״̬�����Ŀ����
#define SKILLSTATENUMBER 325   /// ����״̬�����

/**
 * \brief  ����״̬Ԫ������
 * \author fqnewman
 */
struct SkillStatusCarrier
{
		/// ���ܲ���
		const SkillStatus *status;
		/// �����ֵ�
		const zSkillB *skillbase;
		/// �յ��Ĺ�����Ϣ
		Cmd::stAttackMagicUserCmd revCmd;
		/// �����ߵ�ָ��
		SceneEntryPk *attacker;
		/**
		 * \brief  ���캯������ʼ����������
		 * \author fqnewman
		 */
		SkillStatusCarrier()
		{
			status = NULL;
			skillbase = NULL;
			attacker = NULL;
		}
};

/**
 * \brief  ����״̬Ԫ��
 * \author fqnewman
 */
struct SkillStatusElement
{
	/// ״̬�� id
	WORD  id;

	/// ״̬��������
	WORD percent;

	/// ״̬��Ӱ����ֵ
	WORD value;

	/// ״̬�ĳ���ʱ��
	QWORD qwTime;

	/// ״ִ̬�еĲ����־
	WORD state;


	///�����ߵ���ʱID;
	DWORD dwTempID;

	///�����ߵ�ID
	DWORD dwAttackerID;

	///����ID
	DWORD dwSkillID;

	///ִ��ʱ��
	DWORD dwTime;

	///ִ�в���
	BYTE  byStep;
	
	///�ƶ�����
	BYTE  byGoodnessType;
	
	///���ܵĻ������
	BYTE  byMutexType;

	///�Ƿ�ˢ����������1Ϊˢ��0Ϊ��
	BYTE  refresh;

	///�����ߵ�����
	zSceneEntry::SceneEntryType attacktype;

	/**
	 * \brief  ����״̬Ԫ�ع��캯����ʼ����������
	 * \author fqnewman
	 */
	SkillStatusElement()
	{
		id			= 0;					//״̬��id;
		percent		= 0;
		value		= 0;
		qwTime		= 0;
		state		= 0;
		dwTempID	= 0;
		dwSkillID	= 0;
		dwTime		= 0;
		byStep		= 0;
		refresh    = 0;
	}
};

/**
 * \brief  ����״̬������
 * \author fqnewman
 */
class SkillStatusManager
{
private:
	/// ��������״̬�б�
	std::map<DWORD, SkillStatusElement> _activeElement;

	/// ��ʱ��������״̬�б�
	std::map<DWORD, SkillStatusElement> _recoveryElement;

	/// ���ñ�������״̬�б�
	std::map<DWORD, SkillStatusElement> _passivenessElement;

	/// ���Ͷ���
	typedef std::map<DWORD, SkillStatusElement>::value_type VALUE_TYPE;

	/**
	 * \brief  ����״̬�����б�
	 * \author fqnewman
	 */
	struct{
		BYTE (* func)(SceneEntryPk *, SkillStatusElement &);
	}funlist[SKILLSTATENUMBER+1];

	/// ����״̬����������
	SceneEntryPk *entry;
	bool bclearActiveSkillStatus;

public:
		SkillStatusManager();
		~SkillStatusManager();
		void initMe(SceneEntryPk *pEntry);
		void loadSkillStatus(char *buf, DWORD length);
		void getSelectStates(Cmd::stSelectReturnStatesPropertyUserCmd *buf , unsigned long maxSize);
		void sendSelectStates(SceneEntryPk *pThis , DWORD state , WORD value , WORD time);
		void saveSkillStatus(char *buf, DWORD &size);
		bool putOperationToMe(const SkillStatusCarrier &carrier, const bool good = false, SWORD rangDamageBonus=0);
		void putPassivenessOperationToMe(const DWORD skillid, const SkillStatus *pSkillStatus);
		void processPassiveness();
		inline BYTE runStatusElement(SkillStatusElement &element);
		void timer();
		void clearActiveSkillStatus();
		void clearActiveSkillStatusOnlyUseToStatus48();
		void clearBadActiveSkillStatus();
		void addBadSkillStatusPersistTime(const DWORD &value);
		void addBadSkillStatusPersistTimePercent(const DWORD &value);
		void clearMapElement(const BYTE &byMutexType, std::map<DWORD, SkillStatusElement> &myMap, DWORD dwID,bool notify=true);
		WORD getSaveStatusSize();
		void clearRecoveryElement(DWORD value);
		void clearActiveElement(DWORD value);
		void processDeath();
		void clearSkill(DWORD dwSkillID);

//[test]-----------------------------------------------------------------
		void showValue();
		void showValueToLog();
		void showActive();
		void showRecovery();
		void showPassiveness();
//-----------------------------------------------------------------------
};

#endif

