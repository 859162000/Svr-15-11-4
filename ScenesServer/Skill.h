#ifndef _SKILL_H_
#define _SKILL_H_

#include "zDatabaseManager.h"
#include "Command.h"
#include "zSceneEntry.h"
#include "zUniqueID.h"
#include "SkillBase.h"
#include "zTime.h"
#include "zType.h"

/**
 * \brief ����ѹ���ṹ
 */
struct ZlibSkill
{
	DWORD count;
	unsigned char data[0];
	ZlibSkill()
	{
		count = 0;
	}
};

/**
 * \brief ���ܴ浵��Ԫ
 */
struct SaveSkill
{
	DWORD type;
	DWORD level;
};

/**
 * \brief ���ܷ���
 *
 */
enum SkillType
{
	SKILL_TYPE_DAMAGE = 1,	///ֱ���˺�
	SKILL_TYPE_RECOVER,	///�ָ�ϵ
	SKILL_TYPE_BUFF,	///����Ч��
	SKILL_TYPE_DEBUFF,	///����Ч��
	SKILL_TYPE_SUMMON,	///�ٻ�
	SKILL_TYPE_RELIVE	///����
};

class SceneUser;
class SceneEntryPk;
class SceneNpc;

/**
 * \brief �����࣬���弼�ܵĻ������Լ�����ִ�з���
 */
struct zSkill : zEntry
{
	static const DWORD maxUniqueID = 100000;
	public:
		t_Skill data;
		const zSkillB *base;
		const zSkillB *actionbase;

		static zSkill *create(SceneEntryPk *pEntry , DWORD id , DWORD level);
		static zSkill *createTempSkill(SceneEntryPk *pEntry , DWORD id , DWORD level);
		static zSkill *load(SceneEntryPk *pEntry , const SaveSkill *s);
		bool canUpgrade(SceneEntryPk *pEntry);
		bool setupSkillBase(SceneEntryPk *pEntry);
		bool getSaveData(SaveSkill *save);
		bool canUse();
		bool checkSkillBook(bool nextbase=false);
		bool checkSkillStudy(bool nextbase=false);
		void resetUseTime();
		void clearUseTime();
		void refresh(bool ignoredirty=false);

		~zSkill();
		unsigned long long lastUseTime;
	private:
		bool dirty;
		bool istemp;
		bool needSave;
		bool inserted;
		SceneEntryPk *_entry;
		const Cmd::stAttackMagicUserCmd *curRevCmd;
		unsigned int curRevCmdLen;
		zSkill();

		//static DWORD uniqueID;

// �����ӵļ��ܷ���
	public:
		bool action(const Cmd::stAttackMagicUserCmd *rev, const unsigned int cmdLen);
		inline void doOperation(const SkillStatus *pSkillStatus);
		inline void doPassivenessSkill();
		inline bool showMagicToAll();
		inline bool findAttackTarget(const SkillStatus *pSkillStatus, zPos &pd, DWORD &count, SWORD rangDamageBonus=0);
		inline const zSkillB *getNewBase();
};

#endif
