/**
 * \file
 * \version	$Id: QuestTable.h $
 * \author	
 * \date		
 * \brief		����ϵͳ
 * 
 */

#ifndef __QUEST_TABLE_H__
#define __QUEST_TABLE_H__

#include "zType.h"

#include <string>
#include <map>

template <typename T>
class EventManager;

class SceneUser;

/**
 * \brief �¼��������
 *
 *����ͬ���͵��¼��б�,���ṩͳһ�Ĵ���ʽ
 *
 */	
class EventTable
{
public:
	
	static EventTable& instance();

	/**     
	 * \brief  ִ�������¼�
	 *
	 * ������Ӧ���¼�������ִ�и������¼�
	 *      
	 * \param user: ������û�
	 * \param event:�����¼�
	 * \return ������
	 */		
	template <typename E>
	int execute (SceneUser& user, E& event)
	{
		return EventManager<E>::instance().execute(user, event);
	}

	/**     
	 * \brief  ִ�������¼�
	 *
	 * ������Ӧ���¼�������ִ�и������¼�
	 *      
	 * \param event:�����¼�
	 * \return ������
	 */		
	template <typename E>
	int execute (E& event)
	{
		return EventManager<E>::instance().execute(event);
	}

	bool init(const std::string& type, const std::string& file);
	
private:	

	EventTable();
	~EventTable();
	
	static EventTable* _instance;
};

#include "Quest.h"
#include <ext/hash_map>

/**
 * \brief ������
 *
 *���񼯺�,�洢�����������ϸ��Ϣ
 *
 */	
class QuestTable
{
public:
	static QuestTable& instance();
	
	bool init();
	
	const Quest* quest(DWORD id) const;
	
private:
	static const std::string _quest_file;
	
	QuestTable();

	~QuestTable();

	static QuestTable* _instance;	
	
	typedef __gnu_cxx::hash_map<DWORD, Quest> QUESTS;
	typedef QUESTS::iterator quest_iterator;
	typedef QUESTS::const_iterator const_quest_iterator;
	
	QUESTS _quests;
};

#endif



