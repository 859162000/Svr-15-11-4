/**
 * \file
 * \version	$Id: QuestEvent.h  $
 * \author
 * \date	2005-03-31
 * \brief	����ϵͳ
 * 
 */

#ifndef __QUEST_EVENT_H__
#define __QUEST_EVENT_H__

#include "Zebra.h"
#include "zXMLParser.h"

#include <string>
#include <ext/hash_map>
#include <vector>

class SceneUser;
class Vars;
class Condition;
class Action;

/**
 * \brief �����¼�
 *
 * ��װ�˶�����ű��������¼��Ĵ���
 *
 */
class Event
{
public:
	enum {
		START_QUEST = 0,
		QUESTING = 1,
	};
	
	Event();
	~Event();


	int execute();
	int execute(SceneUser& user);
	int execute(SceneUser& user, DWORD quest_id, DWORD offset);
	
	
	int state(SceneUser& user);

	/**     
	 * \brief   ȡ���¼�id
	 *      
	 * \return �¼�id
	 */
	DWORD id() const
	{
		return _id;
	}
	
	bool parse(const std::string& file_name);

	const char * npc_name() const
	{
		return _npc_name;
	}
private:
	
	class FreeMemory 
	{
	public:
		template <typename T>
		void operator () (T target)
		{
			delete target;	
		}	
	};

	class Embranchment
	{
	public:
		Embranchment();

		~Embranchment();

		bool is_valid(SceneUser* user, const Vars* vars);
		int do_it(SceneUser* user, Vars* vars);

		void add(Condition* condition);
		void add(Action* action);
		bool active() const;
		void active(bool flag);
	private:
		bool _active;
		std::vector<Condition *> _conditions;
		std::vector<Action *> _actions;
	};
	
	typedef __gnu_cxx::hash_map<DWORD, Embranchment *> EMBRANCHMENT; //embranchment_id
	typedef EMBRANCHMENT::iterator embranchment_iterator;
	typedef EMBRANCHMENT::const_iterator const_embranchment_iterator;	
	
	//EMBRANCHMENT _embranchments;
	
	//we use the higest bit to indicate if it is a new quest
	template <typename T>
	class LessIgnoreHB : public std::binary_function<T, T, bool>
	{
	public:
		bool operator() (T preceding, T posterior) const
		{
			preceding &= ~(0x1 << 31);
			posterior &= ~(0x1 << 31);
			
			return preceding < posterior;
		}
	};
	
	typedef std::map<DWORD, EMBRANCHMENT *, LessIgnoreHB<DWORD> > QUESTS; //quest_id
	typedef QUESTS::iterator quest_iterator;
	
	QUESTS _quests;
	
	DWORD _id;
	char _npc_name[MAX_NAMESIZE];
};

/**
 * \brief �¼�������
 *
 *�����¼�ģ��,�ṩ�Բ�ͬ�����¼����͵�ͳһ����
 *
 */
template<typename T>
class EventTrigger
{
public:
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 */    
	EventTrigger(DWORD id) : _id(id)
	{ }

	/**     
	 * \brief   ȡ���¼�id
	 *      
	 * \return �¼�id
	 */		
	DWORD id() const
	{
		return _id;
	}
	
	/**     
	 * \brief   ȡ���¼�Ŀ¼
	 *      
	 * \return �¼�Ŀ¼
	 */
	static const std::string& dir()
	{
		return T::DIR;
	}
	
private:
	DWORD _id;		
};

namespace Trigger {
	/**
	 * \brief ʹ�ô����¼�
	 *
	 *��ʹ����Ʒʱ������
	 *
	 */	
	class Use
	{
	public:
		static const std::string DIR;
	};
	
	/**
	 * \brief ɱ�������¼�
	 *
	 *��ɱ��NPCʱ������
	 *
	 */	
	class Kill
	{
	public:
		static const std::string DIR;
	};

	/**
	 * \brief ɱ�������¼�
	 *
	 *��ɱ��NPCʱ������
	 *
	 */	
	class KillByLevel
	{
	public:
		static const std::string DIR;
	};
	
	/**
	 * \brief ɱ�������¼�(����ɱ��)
	 *
	 *��ɱ��NPCʱ������
	 *
	 */	
	class KillBySelf
	{
	public:
		static const std::string DIR;
	};

	/**
	 * \brief ʰȡ�����¼�
	 *
	 *��ʰȡ��Ʒʱ������
	 *
	 */	
	class Get
	{
	public:	
		static const std::string DIR;
	};
		
	/**
	 * \brief ���ʴ����¼�
	 *
	 *�ڷ���NPCʱ������
	 *
	 */	
	class Visit
	{
	public:	
		static const std::string DIR;
	};

	/**
	 * \brief ���������¼�
	 *
	 *���û�����ʱ������
	 *
	 */	
	class Die
	{
	public:	
		static const std::string DIR;
	};

	/**
	 * \brief �������¼�
	 *
	 *���û�����ʱ������
	 *
	 */	
	class Ride
	{
	public:	
		static const std::string DIR;
	};

	/**
	 * \brief �˳������¼�
	 *
	 *���û��˳�ʱ������
	 *
	 */	
	class Quit
	{
	public:	
		static const std::string DIR;
	};

	/**
	 * \brief ��ʱ�����¼�
	 *
	 *��Ҫ��ʱ���������
	 *
	 */	
	class Timer
	{
	public:	
		static const std::string DIR;
	};
	
	/**
	 * \brief ���������¼�
	 *
	 *���û�������Ʒʱ������
	 *
	 */	
	class Drop
	{
	public:	
		static const std::string DIR;
	};

	/**
	 * \brief ������Ϸ�����¼�
	 *
	 *���û�������Ϸʱ������
	 *
	 */	
	class Enter
	{
	public:	
		static const std::string DIR;
	};

	/**
	 * \brief ���������¼�
	 *
	 */	
	class Other
	{
	public:	
		static const std::string DIR;
	};	
}


//typedef EventTrigger<Trigger::Use> OnUse;
typedef EventTrigger<Trigger::Kill> OnKill;
typedef EventTrigger<Trigger::KillByLevel> OnKillByLevel;
typedef EventTrigger<Trigger::KillBySelf> OnKillBySelf;
typedef EventTrigger<Trigger::Get> OnGet;
//typedef EventTrigger<Trigger::Visit> OnVisit;
typedef EventTrigger<Trigger::Die> OnDie;
typedef EventTrigger<Trigger::Ride> OnRide;
typedef EventTrigger<Trigger::Quit> OnQuit;
typedef EventTrigger<Trigger::Timer> OnTimer;
typedef EventTrigger<Trigger::Drop> OnDrop;
typedef EventTrigger<Trigger::Enter> OnEnter;
typedef EventTrigger<Trigger::Other> OnOther;

/**
 * \brief ���ʴ����¼�
 *
 *�ڷ���NPCʱ������
 *
 */	
template <typename T>
class EventTrigger2 : public EventTrigger<T> 
{
public:	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *  
	 * param id : ���ʵ�npc id
	 * param quest_id : ����id
	 * param offset : �����֧ 
	 */    
	EventTrigger2(DWORD id, DWORD quest_id = 0, DWORD offset = 0) : EventTrigger<T>(id), _quest_id(quest_id), _offset(offset)
	{ }

	/**     
	 * \brief   ȡ���¼���Ӧ������id
	 *      
	 * \return ����id
	 */	
	DWORD quest_id() const
	{
		return _quest_id;
	}
	
	/**     
	 * \brief   ȡ���¼���Ӧ�������֧
	 *      
	 * \return �����֧
	 */	
	DWORD offset() const
	{
		return _offset;
	}
		
private:
	DWORD _quest_id;
	DWORD _offset;
};

typedef EventTrigger2<Trigger::Use> OnUse;
typedef EventTrigger2<Trigger::Visit> OnVisit;

/**
 * \brief �¼�������
 *
 *����ͬ���͵��¼��б�,���ṩͳһ�Ĵ���ʽ
 *
 */	
template <typename E>
class EventManager
{
public:
	typedef EventManager<E> self_t;

	/**     
	 * \brief  ����ģʽ,��֤�¼���������Ψһʵ��
	 *      
	 * \return �¼���������Ψһʵ��
	 */		
	static self_t& instance() 
	{
		static self_t new_instance;			
		return new_instance;
	}

	/**     
	 * \brief  ִ������
	 *
	 * ���Ҷ�Ӧ�������¼���ִ��
	 *      
	 * \param user: ������û�
	 * \param event:�����¼�
	 * \return ������
	 */	
	int execute(SceneUser& user, E& event)
	{
		__gnu_cxx::hash_map<DWORD, Event>::iterator it = _events.find(event.id());
		if (it!=_events.end()) {
			return it->second.execute(user);
		}
		
		//return Action::SUCCESS;
		return 0;
	}

	/**     
	 * \brief  ִ������
	 *
	 * ���Ҷ�Ӧ�������¼���ִ��
	 *      
	 * \param event:�����¼�
	 * \return ������
	 */		
	int execute(E& event);

	/**     
	 * \brief  ����״̬
	 *
	 * ȡ���û�ĳ�����״̬
	 *      
	 * \param user: ������û�
	 * \param event: ��ѯ������
	 * \return ����״̬
	 */	
	int state(SceneUser& user, E& event);

	/**     
	 * \brief  ��������ű�
	 *
	 * ��������ű�,���ɶ�Ӧ���¼��ṹ
	 *      
	 * \param file_name: �ļ���
	 * \return true��ʾ�����ű��ɹ�,false��ʾ�����ű�ʧ��
	 */  	
	bool parse(const std::string& file_name) 
	{
		Event event;
		
		std::string name = Zebra::global["questdir"]+E::dir()+file_name;
		if ( !event.parse(name) ) {
			return false;
		}
		
		_events[event.id()] = event;
		return true;
	}
	
	void get_valid_quest(SceneUser &user, bool=false);
	void get_valid_quest_str(SceneUser &user);
private:
	__gnu_cxx::hash_map<DWORD, Event> _events;	

	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *  
	 */    
	EventManager()
	{ }

	/**     
	 * \brief ��������
	 *
	 */
	~EventManager()
	{ }
	
	static EventManager<E>* _instance;
};

template<typename E>
EventManager<E>* EventManager<E>::_instance = NULL;

/**     
 * \brief  ִ������
 *
 * ģ��ƫ�ػ�,ִ���ض��ķ������ʹ��������¼�
 *      
 * \param user: ������û�
 * \param event:�����¼�
 * \return ������
 */	
template<>
inline int EventManager<OnVisit>::execute(SceneUser& user, OnVisit& event)
{
	__gnu_cxx::hash_map<DWORD, Event>::iterator it = _events.find(event.id());
	if (it!=_events.end()) {
		if ( event.quest_id() && event.offset() ) {
			return it->second.execute(user, event.quest_id(), event.offset());
		}else {
			return it->second.execute(user);	
		}
	}
	
	return 0;
}

template<>
inline int EventManager<OnUse>::execute(SceneUser& user, OnUse& event)
{
	__gnu_cxx::hash_map<DWORD, Event>::iterator it = _events.find(event.id());
	if (it!=_events.end()) {
		if ( event.quest_id() && event.offset() ) {
			return it->second.execute(user, event.quest_id(), event.offset());
		}else {
			return it->second.execute(user);	
		}
	}
	
	return 0;
}

/**     
 * \brief  ����״̬
 *
 * ȡ���û�ĳ�����״̬
 *      
 * \param user: ������û�
 * \param event: ��ѯ������
 * \return ����״̬
 */	
template<>
inline int EventManager<OnVisit>::state(SceneUser& user, OnVisit& event)
{
	__gnu_cxx::hash_map<DWORD, Event>::iterator it = _events.find(event.id());
	if (it!=_events.end()) {
		return it->second.state(user);	
	}
	
	return -1;
}

/**     
 * \brief  ִ������
 *
 * ģ��ƫ�ػ�,ִ���ض��Ķ�ʱ���������¼�
 *      
 * \param event:�����¼�
 * \return ������
 */	
template<>
inline int EventManager<OnTimer>::execute(OnTimer& event)
{
	__gnu_cxx::hash_map<DWORD, Event>::iterator it = _events.find(event.id());
	if (it!=_events.end()) {
		return it->second.execute();	
	}
	
	return 0;
}

#endif
