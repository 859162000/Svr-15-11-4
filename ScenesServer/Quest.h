/**
 * \file
 * \version	$Id: Quest.h $
 * \author	
 * \date		
 * \brief		����ϵͳ
 * 
 */
 
#ifndef __QUEST_H__
#define __QUEST_H__

#include "zType.h"
#include "Command.h"

#include <map>
#include <ext/hash_map>
#include <string>
#include <list>

class SceneUser;

/**
 * \brief ����
 *
 * �����װ�˶�����ĳ���
 *
 */
class Quest
{
public:
	enum {
		FINISHED_SAVE = -1,
		FINISHED_NOT_SAVE = -2,
		FINISHED = -3,
		DOING = 0,	
	};
	
	static bool execute(SceneUser& user, Cmd::stQuestUserCmd* cmd, unsigned int len);
	
	static int load(SceneUser& user, unsigned char* dest , unsigned long &dest_size);
	
	static int save(SceneUser& user, unsigned char* dest);
	
	static int notify(SceneUser& user);
	
	static int abandon(SceneUser& user, DWORD id);

	/**     
	 * \brief  ������������
	 *
	 * \return ��������
	 */	
	const std::string& title() const
	{
		return _title;
	}
	
	/**     
	 * \brief  ������������
	 *
	 * \return ��������
	 */	
	const std::string& description() const	
	{
		return _description;
	}

	static const std::string FINISHED_NAME;
		
private:
	friend class QuestTable;
		
	DWORD _id;
	
	std::string _title;
	std::string _description;
};

#include <sstream>

/**
 * \brief �������
 *
 * �����װ�˶���������ĳ���
 *
 */
class Vars
{
public:
	typedef 	void (*callback_func) ();

	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param quest_id: ����id
	 */    	
	Vars(DWORD quest_id = 0) : _quest_id(quest_id), _timeout(0), _start_time(0), _update(0)
	{ }
	
	/**     
	 * \brief ��������
	 *
	 */    
	~Vars()
	{ }

	void callback(callback_func func)
	{
		_func = func;
	}

	template<typename T>
	bool get_value(const std::string& name, T& value)
	{
		var_iterator it = _vars.find(name);
		if (it != _vars.end()) {
			std::stringstream os(it->second.value());
			os >> value;
			return true;
		}

		return false;
	}
	/**     
	 * \brief  ���ñ���
	 *
	 * ����Operation�������ñ���ֵ
	 *      
	 * \param op: ��������
	 * \param name: ��������
	 * \param action: Ҫ�ı�ı���ֵ
	 * \param tmp: �����Ƿ���Ҫ�洢
	 * \return ��ǰ���Ƿ���0
	 */	
	template <typename Operation>
	int set_value(Operation op, const std::string& name, typename Operation::value_type const & action, int tmp=0, SceneUser *user=NULL)
	{
		std::stringstream os;
		
		var_iterator it = _vars.find(name);
		if (it != _vars.end()) {
			os << it->second.value();
			tmp = it->second.tmp();

		}else {
			//haven't this var before, give a default value	
			os << 0;
		}
				
		typename Operation::value_type value;
		os >> value;

		//value=action;
		op(value, action, user);

		os.clear();
		
		os << value;
		std::string new_value;
		os >> new_value;

		_vars[name] = VAR(new_value, tmp);
		
		return 0;
	}

	/**     
	 * \brief  �жϱ����Ƿ���Ч
	 *
	 * ����Operation�����жϱ���ֵ�Ƿ���������Ҫ��
	 *      
	 * \param op: ��������
	 * \param name: ��������
	 * \param condition: ����ֵ
	 * \return ��Ч����true,���򷵻�false
	 */		
	template <typename Operation>
	bool is_valid(Operation op, const std::string& name, typename Operation::value_type const & condition) const
	{
		const_var_iterator it = _vars.find(name);
		if (it != _vars.end()) {
			std::stringstream os(it->second.value());
			
			typename Operation::value_type value;
			os >> value;
						
			return op(value, condition);
		}
		

		return !Operation::NEED_EXIST;			
	}
	
	/**     
	 * \brief  ��������id
	 *
	 * \return ����id
	 */	
	DWORD quest_id() const
	{
		return _quest_id;
	}
	
	int set_timer();
	int set_timer(int start);
	
	bool is_timeout(int timeout) const;
	int start_time() const;
	
	int save_timer(unsigned char* dest) const;
	int load_timer(unsigned char* dest);

	int save(unsigned char* dest) const;
	int load(unsigned char* dest);
	
	int notify(SceneUser& user) const;
	int notify(SceneUser& user, const std::string& name) const;
	
	int state() const;

	int update() const;
	void update(int value);

	std::string info() const;

	bool reserve(); 
private:
	class VAR
	{
	public:
		VAR(const std::string& value="", int tmp=0) : _value(value), _tmp(tmp)
		{ }
		
		~VAR()
		{ }
		
		const std::string& value() const
		{
			return _value;
		}
		
		bool is_tmp() const
		{
			return _tmp==1;	
		}
		
		bool tmp() const
		{
			return _tmp;
		}
		
	private:
		std::string _value;
		int _tmp;	
	};	
	struct key_hash : public std::unary_function<const std::string, size_t>
	{
		size_t operator()(const std::string &x) const
		{
			__gnu_cxx::hash<const char *> H;
			return H(x.c_str());
		}
	};
	typedef __gnu_cxx::hash_map<std::string, VAR, key_hash> VARS;
	typedef VARS::iterator var_iterator;
	typedef VARS::const_iterator const_var_iterator;	
		
	VARS _vars;	
	DWORD _quest_id;
	int _timeout;
	int _start_time;
	int _update;

	callback_func _func;
};


#include <fstream>
#include "Zebra.h"

/**
 * \brief ȫ�ֱ���
 *
 * �����װ�˶�ȫ����������ĳ���
 *
 */

#define VARS_SAVE_PATH "/tmp/zebra_save_vars_"

template <int dummy>
class GlobalVarImpl
{
public:
	typedef GlobalVarImpl<dummy> self_t;
	enum {
		MAX_BUF_SIZE = 4096*8,
	};
	
	static self_t& instance()
	{
		if (!_instance) {
			/***********************
			static self_t new_instance; 
			_instance = &new_instance;
			************************/
			//here can not use as above
			_instance = new self_t;
		}
		
		return *_instance;
	}		

	Vars* vars(QWORD id) const
	{
		const_vars_iterator it = _vars.find(id);
		if 	(it != _vars.end() ) {
			return it->second;
		}
		
		return NULL;	
	}

	Vars* add(QWORD id)
	{
		Vars* v = vars(id);

		if (!v) {
			v = new Vars(id);
			_vars[id] = v;
		}

		return v;
	}

	void update()
	{
		time_t t = time(NULL);
		const struct tm * const now = localtime(&t);

		if (_day != now->tm_mday) {
			_day = now->tm_mday;
			destroy();
		}

	}

	bool save() const
	{
		std::ofstream of(_file.c_str(), std::ios::binary);

		char buf[MAX_BUF_SIZE*30];
		bzero(buf, sizeof(buf));

		int count = _vars.size();
		memcpy(buf, &count, sizeof(count));
		int len = sizeof(count);
		
		for (const_vars_iterator it=_vars.begin(); it!=_vars.end(); ++it) {
			QWORD id = it->first;
			memcpy(buf+len, &id, sizeof(id));
			len += sizeof(id);

			len += it->second->save((unsigned char*)buf+len);
			if (len >= (MAX_BUF_SIZE*30 - 1024))
			{
				Zebra::logger->fatal("�洢ȫ�ֱ���ʱ������������Խ�磨%u, %u��", MAX_BUF_SIZE, len);
				of.write(buf, len);
				bzero(buf, sizeof(buf));
				len = 0;
			}
		}

		if (len > 0) of.write(buf, len);
			
		of.flush();
		of.close();
		
		return true;
	}

	bool load()
	{
		bool ret = false;
		std::ifstream inf(_file.c_str(), std::ios::binary);

		//����ļ���С
		inf.seekg(0, std::ios::end);
		int length = inf.tellg();
		inf.seekg(0, std::ios::beg);

		if (length > 0)
		{
			char *buf = new char[length];
			if (buf)
			{
				bzero(buf, length);

				inf.read(buf, length);
				int size =*((int*)buf);
				int len = sizeof(int);

				while (size-- > 0) {
					QWORD id = *((QWORD*)(buf+len));
					len += sizeof(id);
					Vars* vars = new Vars;
					len += vars->load((unsigned char*)buf+len);
					_vars[id] = vars;
				}
				SAFE_DELETE_VEC(buf);
				ret = true;
			}
			else
				Zebra::logger->fatal("������������ļ������ڴ�ʧ�ܣ�%u", length);
		}

		inf.close();
		return ret;
	}

/*
	void set(QWORD id, const std::string& name, int value)
	{
		Vars* v = vars(id);

		if (!v) {
			v = new Vars(id);
			_vars[id] = v;
		}

		Op::Set<int> op;
		v->set_value(op, name, value);
	}
*/

	std::string show() const
	{
		std::ostringstream os;
		for (const_vars_iterator it=_vars.begin(); it!=_vars.end(); ++it) {
			os << "����(" << it->first << ")" << "\n";
			os << it->second->info() << "\n";
		}
		return os.str();
	}

	static void server_id(int id) 
	{
		SERVER_ID = id;
	}

	static int server_id()
	{
		return SERVER_ID;
	}
	
private:	
	void destroy()
	{	
/*
		for (vars_iterator it=_vars.begin(); it!=_vars.end(); ++it) {
			SAFE_DELETE(it->second);
		}
		_vars.clear();
*/
		for (vars_iterator it=_vars.begin(); it!=_vars.end(); ) {
			if (it->second->reserve()) ++it;
			else {
				vars_iterator tmp = it;
				++it;
				SAFE_DELETE(tmp->second);
				_vars.erase(tmp);
			}
		}
	}
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 */ 
	GlobalVarImpl()
	{ 
		time_t t = time(NULL);
		const struct tm * const now = localtime(&t);
		_day = now->tm_mday;

		std::ostringstream os;
//		os << ScenesService::getInstance().getServerID();
		os << VARS_SAVE_PATH;
		os << SERVER_ID << "_";
		os << dummy << ".sv";
		_file = os.str();
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	~GlobalVarImpl()
	{ 
		destroy();
	}
	
	typedef std::map<QWORD, Vars*> VARS;	//
	typedef VARS::iterator vars_iterator;
	typedef VARS::const_iterator const_vars_iterator;
	
	VARS _vars;
	
	static self_t* _instance;
	int _day;
	std::string _file;

	static int SERVER_ID;
};

template <int dummy>
GlobalVarImpl<dummy>* GlobalVarImpl<dummy>::_instance = NULL;

template <int dummy>
int GlobalVarImpl<dummy>::SERVER_ID = 0;

typedef GlobalVarImpl<1> GlobalVar;
typedef GlobalVarImpl<2> TongVar;
typedef GlobalVarImpl<3> FamilyVar;
typedef GlobalVarImpl<4> UsersVar;

#define ALLVARS(function) \
	GlobalVar::instance().function(); \
	TongVar::instance().function(); \
	FamilyVar::instance().function(); \
	UsersVar::instance().function(); \
	UserVar::instance().function();

/*************************************************************
NOTE: This macro used initial the filename, according by server id,
           it must be call FIRST, even BEFORE the INSTANCE.
*************************************************************/
#define ALLVARS1(function, arg1) \
	GlobalVar::function(arg1); \
	TongVar::function(arg1); \
	FamilyVar::function(arg1); \
	UsersVar::function(arg1); \
	UserVar::function(arg1);


/**
 * \brief �û�����
 *
 * �����װ�˶��û���������ĳ���
 *
 */
class UserVar
{
public:
	enum {
		MAX_BUF_SIZE = 4096*8,
	};
	static UserVar& instance();

	Vars* vars(DWORD id, QWORD key) const;
	
	Vars* add(DWORD id, QWORD key);

	void update()
	{
//		time_t t = time(NULL);
//		const struct tm * const now = localtime(&t);
//
//		if (_day != now->tm_mday) {
//			_day = now->tm_mday;
//			destroy();
//		}

	}

	bool save() const;
	bool load();

/*
	void set(QWORD id, const std::string& name, int value)
	{
		Vars* v = vars(id);

		if (!v) {
			v = new Vars(id);
			_vars[id] = v;
		}

		Op::Set<int> op;
		v->set_value(op, name, value);
	}
*/

	std::string show() const
	{
		std::ostringstream os;
		for (const_vars_iterator it=_vars.begin(); it!=_vars.end(); ++it) {
			os << "����(" << it->first << ")" << "\n";
			//os << it->second->info() << "\n";
		}
		return os.str();
	}

	static void server_id(int id) 
	{
		SERVER_ID = id;
	}

	static int server_id()
	{
		return SERVER_ID;
	}
private:	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 */ 
	UserVar()
	{
		std::ostringstream os;
//		os << ScenesService::getInstance().getServerID();
		os << VARS_SAVE_PATH;
		os << SERVER_ID << "_";
		os << "userv" << ".sv";
		_file = os.str();
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	~UserVar()
	{ }
	
	class VAR
	{
	public:	
		Vars* vars(QWORD key) const
		{
			const_var_iterator it = _vars.find(key);
			if 	(it != _vars.end() ) {
				return it->second;
			}
			
			return NULL;
		}		
		
		Vars* add(DWORD id,QWORD key)
		{
			Vars* v = vars(key);
			if (!v) v = new Vars(id);
		
			_vars[key] = v;
		
			return v;				
		}
		
		/**     
		 * \brief �洢����
		 *
		 *�洢�������
		 *      
		 * \param dest: ���񵵰�
		 * \return �洢�Ķ����Ƶ�������
		 */
		int save(unsigned char* dest) const;

		/**     
		 * \brief ��ȡ����
		 *
		 *��ȡ�������
		 *      
		 * \param dest: ���񵵰�
		 * \return ��ȡ�Ķ����Ƶ�������
		 */
		int load(unsigned char* dest);
	private:
		std::map<QWORD, Vars*> _vars;
		typedef std::map<QWORD, Vars*>::const_iterator const_var_iterator;
	};
	
	typedef __gnu_cxx::hash_map<DWORD, VAR*> VARS;	//
	typedef VARS::iterator vars_iterator;
	typedef VARS::const_iterator const_vars_iterator;
	std::string _file;
	
	VARS _vars;
	
	static UserVar* _instance;
	static int SERVER_ID;
};

/**
 * \brief �����б�
 *
 * �����װ�˶��û����ϵ������б�Ĳ���
 *
 */
class QuestList
{
public:
	enum {
		MAX_NUM = 20,
	};
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 */ 
	QuestList() : _menu("")
	{ }

	/**     
	 * \brief ��������
	 *
	 */    
	~QuestList()
	{ }
	
	Vars* vars(DWORD id) const;
	
	void add_quest(DWORD id, const Vars& vars, SceneUser& user, bool notify = true);
	int count() const;
	
	bool set_menu(const std::string& menu);
	int get_menu(char* menu, int& status);
	
	void add_menu(const std::string& menu);
	
	int save(unsigned char* dest) const;
	int load(unsigned char* dest , unsigned long &dest_size);
	
	int state(DWORD id) const;
	int start_time(DWORD id) const;
	
	int notify(SceneUser& user) const;
	
	int abandon(SceneUser& user, DWORD id, bool force = false, bool destroy = true);

	// �����������
	void clear(SceneUser* pUser);

	int update(SceneUser& user, bool refresh);

	std::string info(int id=0) const;

private:
	
	std::string _menu;
	std::list<std::string> _subs;
	
	typedef __gnu_cxx::hash_map<DWORD, Vars> QUESTS;	//questid, vars
	typedef QUESTS::iterator quest_iterator;
	typedef QUESTS::const_iterator const_quest_iterator;
	
	QUESTS _quests;

};

#endif

