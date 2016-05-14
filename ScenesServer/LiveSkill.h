/**
 * \file
 * \version	$Id: LiveSkill.h  $
 * \author	
 * \date	2005-03-09
 * \brief	ʵ�������
 * 
 */
 
#ifndef __LIVE_SKILL_H__
#define __LIVE_SKILL_H__

#include "zType.h"
#include "zNullCmd.h"
#include "zDatabaseManager.h"
#include "Command.h"

#include <strings.h> //for bzero in Command.h
#include <stdlib.h> //for random
#include <assert.h>

class SceneUser;
class zLiveSkillB;
class zObject;

struct LiveSkill
{
public:
	WORD id; //���ܱ�ʶ
	WORD level; //�ȼ�
	DWORD point; //�ȼ���ֵ��С��

	enum {
		DEFAULT_START_LEVEL	= 1, //�����Ĭ�Ͽ�ʼ�ȼ�
		WORKING_TIME = 5, //����ʱ��
		ADVANCE_WORK_BONUS = 500, //�����Ʒ���ʼӳ�
		MIN_NEED_SP = 4, 
		MAX_NEED_SP = 8, 
		ADVANCE_LEVEL = 22, //���׼�����Ҫ�ȼ�
		ODDS_BENCHMARK = 10000, 
	};

	enum {
		BASIC_WORK = 1,
		MAKE_WORK = 2,
		ADVANCE_WORK = 3,
	};

	int bonus(SceneUser* user, int points = 0);
	int bonus_items(SceneUser* user, zLiveSkillB* base_skill);
	static int bonus_exp(SceneUser* user, DWORD exp);
	
	static int consume_sp();
	
	const static int odds[];
};

#include <map>

class LiveSkillsManager;

class LiveSkills
{
public:
	LiveSkills();
	~LiveSkills();
		

	std::size_t count()
	{
		return skills.size();
	}
	
	void add(const LiveSkill& skill)
	{
		skills[skill.id] = skill;	
	}
	
	enum {
		START_SKILL_ID = 321,
	};
	
	int state() const
	{
		/*
		int state = skill_id - LiveSkills::START_SKILL_ID + Cmd::LIVE_SKILL_STATE_START + 1;

		//the following case should not be occured, just for safe
		state = (state<0)?0:state;
		state = (state>Cmd::MAX_STATE)?Cmd::MAX_STATE:state;
		*/
		int state = 0;
		switch (skill_id)
		{
			case 321:
				state = Cmd::USTATE_PLANTING;
				break;
			case 324:
				state = Cmd::USTATE_DIGGING;
				break;
			case 327:
				state = Cmd::USTATE_PROSPECTING;
				break;
			case 330:
				state = Cmd::USTATE_CUTTING;
				break;
			case 333:
				state = Cmd::USTATE_GATHERING;
				break;
			case 336:
				state = Cmd::USTATE_FISHING;
				break;
			case 337:
				state = Cmd::USTATE_HUNTING;
				break;
		}			
		
		return state;
	}
	
	//notice, unsafe function, ensure length of buf greater than 6
	int titles(BYTE* buf, int len) const
	{
		assert( len>=6 );
		memset(buf, 0, len);
		
		const_iterator it = skills.begin();
		while (it != skills.end()) {
			switch (it->second.id) 
			{
				case 323: //����
					buf[0] = (BYTE) it->second.level;
					break;					
				case 326: //����
					buf[1] = (BYTE) it->second.level;
					break;					
				case 329: //����
					buf[2] = (BYTE) it->second.level;
					break;					
				case 331: //ľ��
					buf[3] = (BYTE) it->second.level;
					break;					
				case 335: //��ҩ
					buf[4] = (BYTE) it->second.level;
					break;					
				case 338: //���
					buf[5] = (BYTE) it->second.level;
					break;					
			}			
			++it;	
		}
		
		return 0;
	}
	
	bool is_upgrade() const
	{
		bool result = true;
		
		switch (skill_id)
		{
			case 333:
				result = false;
				break;
			case 336:
				result = false;
				break;
			case 337:
				result = false;
				break;
		}
		
		return result;
	}
	
private:
	friend class LiveSkillsManager;
	friend class LiveSkill;
	friend class SceneUser;
	friend class EquipMaker;
	typedef std::map<WORD, LiveSkill> skill_t;
	typedef skill_t::iterator iterator;
	typedef skill_t::const_iterator const_iterator;
	
	skill_t skills; //�û���ǰ��ü���
	unsigned long working_time; //�û��ϴι���ʱ��
	WORD skill_id; //�û��ϴι������
	zObject* tool; //�Ͷ�����
	/**
	 * \brief ���ݼ���id�����û��Ƿ���иü���
	 * \return �ҵ��ļ��ܣ�û�з���NULL
	 */
	LiveSkill* skill(WORD id);
	
};

class zObject;

#include <ext/hash_set>
#include <map>

class LiveSkillsManager
{
public:

	/**
	 * \brief �������ص���Ϣ������
	 * 
	 */
	//static int execute(SceneUser* user, const Cmd::t_NullCmd *command, unsigned int len);

	/**
	 * \brief �Ͷ�״ָ̬�����
	 * 
	 */
	static bool command_filter(SceneUser* user, const Cmd::t_NullCmd *ptNullCmd, const unsigned int cmdLen);


	/**
	 * \brief ��ʼ�Ͷ�������
	 * 
	 */	
	int execute(SceneUser* user, zObject* ob);

	/**
	 * \brief �����û��Ͷ�״̬
	 * 
	 */
	static bool update(SceneUser* user);
	
	/**
	 * \brief ����ĳ�û����������Ϣ
	 * 
	 */
	static int load(SceneUser* user, const LiveSkill& live_skill);

	/**
	 * \brief �����ض���ʽ�洢�û��������Ϣ
	 * \param user:Ҫ�浵���û�
	 * \param dest:Ŀ���ַ
	 * \return �����Ļ���������
	 */
	static int save(SceneUser* user, unsigned char* dest);	
	
	/**
	 * \brief ֪ͨ�ͻ���ĳ�û����������Ϣ����SceneUser::sendInitToMe������
	 * 
	 */
	static void notify(SceneUser* user);

	/**
	 * \brief ʵ��sigletonģʽ
	 * 
	 */	
	static LiveSkillsManager& instance();

private:
	static LiveSkillsManager* instance_;
	
	LiveSkillsManager();
	~LiveSkillsManager();

	class Factory : public zDatabaseCallBack<zLiveSkillB> 
	{
	public:	
		typedef __gnu_cxx::hash_set<DWORD> hash_set;
		typedef hash_set::iterator iterator;

		hash_set identifies; //all possiable work tools
		std::map<DWORD, DWORD> skills; //
		
		bool exec(zLiveSkillB* entry)
		{
			//identifies.insert((entry->map_kind << 16) | (entry->weapon_kind ));
			if (entry->weapon_kind) {
				identifies.insert( entry->weapon_kind );
				skills[(entry->map_kind<<16) | entry->weapon_kind] = entry->skill_id;
			}
			return true;
		}
		
	};
	
	Factory factory;
};


#endif
