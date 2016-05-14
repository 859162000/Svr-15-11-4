/**
 * \file
 * \version	$Id: QuestAction.h $
 * \author	
 * \date		2005-03-31
 * \brief		�ű���������
 * 
 */

#ifndef __QUEST_ACTION_H__
#define __QUEST_ACTION_H__

#include <string>

#include "Quest.h"
#include "QuestParse.h"

#include "Zebra.h"
#include "zObject.h"
#include "zDatabase.h"
#include "SceneUser.h"
#include "Command.h"

namespace Op {
	/**
	 * \brief ���ӱ���ֵ
	 *
	 * ����һ��������ֵ
	 *
	 */
	template <typename T>
	class Add
	{
	public:
		typedef T value_type;

		/**     
		 * \brief 
		 *
		 * ���ӱ�����ֵ         
		 *      
		 * \param value: ���ı�ı���
		 * \param action: Ҫ�޸ĵ�ֵ
		 * \return ��
		 */     		
		void operator() (T& value, T action, SceneUser *user) const
		{
			value += action;
		}

		//added for debug
		std::string name() const 
		{
			return "Add";
		}		
	};

	/**
	 * \brief �趨����ֵ
	 *
	 * �趨һ��������ֵ
	 *
	 */
	template <typename T>
	class Set
	{
	public:
		typedef T value_type;
		
		/**     
		 * \brief  
		 *
		 * �趨һ��������ֵ         
		 *      
		 * \param value: ���ı�ı���
		 * \param action: Ҫ�趨��ֵ
		 * \return ��
		 */     
		 void operator() (T& value, T action, SceneUser *user) const
		{
			value = action;
		}

		//added for debug
		std::string name() const 
		{
			return "Set";
		}		
		
	};

	/**
	 * \brief ���ٱ���ֵ
	 *
	 * ����һ��������ֵ
	 *
	 */	
	template <typename T>
	class Sub
	{
	public:
		typedef T value_type;
		
		/**     
		 * \brief 
		 *
		 * ���ӱ�����ֵ         
		 *      
		 * \param value: ���ı�ı���
		 * \param action: Ҫ���ٵ�ֵ
		 * \return ��
		 */     
		void operator() (T& value, T action, SceneUser *user) const
		{
			value -= action;
		}

		//added for debug
		std::string name() const 
		{
			return "Sub";
		}		
	};

	/**
	 * \brief ��һ������ֵ
	 *
	 * ��һ��������ֵ
	 *
	 */
	template <typename T>
	class Mul
	{
	public:
		typedef T value_type;
		
		/**     
		 * \brief  
		 *
		 * �趨һ��������ֵ         
		 *      
		 * \param value: ���ı�ı���
		 * \param action: Ҫ�趨��ֵ
		 * \return ��
		 */     
		 void operator() (T& value, T action, SceneUser *user) const
		{
			value *= action;
		}

		//added for debug
		std::string name() const 
		{
			return "Mul";
		}		
	};

	/**
	 * \brief ����ֵ�˷�
	 *
	 * ����ֵ�˷�
	 *
	 */
	template <typename T>
	class Pow
	{
	public:
		typedef T value_type;
		
		/**     
		 * \brief  
		 *
		 * �趨һ��������ֵ         
		 *      
		 * \param value: ���ı�ı���
		 * \param action: Ҫ�趨��ֵ
		 * \return ��
		 */     
		 void operator() (T& value, T action, SceneUser *user) const
		{
			value *= value;
		}

		//added for debug
		std::string name() const 
		{
			return "Pow";
		}		
	};

	/**
	 * \brief ����ֵ�˷�
	 *
	 * ����ֵ�˷�
	 *
	 */
	template <typename T>
	class Div
	{
	public:
		typedef T value_type;
		
		/**     
		 * \brief  
		 *
		 * �趨һ��������ֵ         
		 *      
		 * \param value: ���ı�ı���
		 * \param action: Ҫ�趨��ֵ
		 * \return ��
		 */     
		 void operator() (T& value, T action, SceneUser *user) const
		{
			value /= action;
		}

		//added for debug
		std::string name() const 
		{
			return "Div";
		}		
	};

	/**
	 * \brief ����ֵ�˷�
	 *
	 * ����ֵ�˷�
	 *
	 */
	template <typename T>
	class GetP
	{
	public:
		typedef T value_type;
		
		/**     
		 * \brief  
		 *
		 * �趨һ��������ֵ         
		 *      
		 * \param value: ���ı�ı���
		 * \param action: Ҫ�趨��ֵ
		 * \return ��
		 */     
		 void operator() (T& value, T action, SceneUser *user) const
		{
			if (user)
			{
				switch(action)
				{
					case 1:
						value = user->charbase.level;
						break;
					case 2:
						value = time(NULL)%2000000000;
						break;
					case 3:
						break;
					default:
						break;
				}
			}
		}

		//added for debug
		std::string name() const 
		{
			return "GetP";
		}		
	};
}

class SceneUser;

/**
 * \brief ������������
 *
 * ���ඨ���˽ű����������Ľӿڡ�
 *
 */	
class Action
{
public:	
	typedef Action Base;
	
	enum {
		SUCCESS = 0,
		FAILED = 1,
		DISABLE = 2,		
	};

	virtual int do_it (SceneUser* user, Vars* vars);
	

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~Action() { }

protected:
	virtual bool check_args(SceneUser* user, Vars* vars) const;
	
	/**     
	 * \brief  ִ�нű�����Ķ���
	 *
	 * �麯��,�̳�����Ҫ���ش˺����ṩ�Ը��ִ���������֧��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     
	virtual int done(SceneUser* user, Vars* vars) = 0;
};

/**
 * \brief ��Ӵ�����������
 *
 * �����ṩ�˶�����������֧��
 *
 */	
class TeamAction : public Action
{
public:	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	TeamAction(Parse& p)
	{
		p.value("team", _team);	
	}
		
	int do_it (SceneUser* user, Vars* vars);

protected:	
	/**     
	 * \brief  ִ�нű�����Ķ���
	 *
	 * �麯��,�̳�����Ҫ���ش˺����ṩ�Ը��ִ���������֧��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     
	int done(SceneUser* user, Vars* vars) = 0;

private:
	
	int _team;
};

/**
 * \brief ����
 *
 * �����ṩ�˶�������ű����йر������޸ĵ�֧��
 *
 */	
template <typename Operation>
class VarAction : public TeamAction
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	VarAction(Parse& p) : TeamAction(p)
	{ 
		p.value("name", _name);
		p.value("value", _action);
		p.value("id", _id);
		p.value("tmp", _tmp);		
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~VarAction()
	{ }

	/**     
	 * \brief  ִ�б����޸�
	 *
	 * ������done����,ʵ�ֶ���ر������޸�
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
//		Zebra::logger->debug("id:%d\tname:%s\taction:%d\toperation:%s", _id, _name.c_str(), _action, op.name().c_str());					
		if (_id) {
			Vars* v = user->quest_list.vars(_id);
			if (v) {
				v->set_value(op, _name, _action, _tmp, user);
			}
		}else {
			vars->set_value(op, _name, _action, _tmp, user);
		}
		return Action::SUCCESS;
	}
	
private:
	std::string _name;
	value_type _action;
	int _tmp;
	int _id;
};

/**
 * \brief �����䶯��
 *
 * �����ṩ�˶�������ű��б���֮��Ĳ�����֧��
 *
 */	
template <typename Operation>
class VarsAction : public TeamAction
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	VarsAction(Parse& p) : TeamAction(p)
	{ 
		p.value("name1", _name1);
		p.value("id1", _id1);
		p.value("name2", _name2);
		p.value("id2", _id2);
		p.value("tmp", _tmp);		
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~VarsAction()
	{ }

	/**     
	 * \brief  ִ�б����޸�
	 *
	 * ������done����,ʵ�ֶ���ر������޸�
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
		value_type _action;
//		Zebra::logger->debug("id:%d\tname:%s\taction:%d\toperation:%s", _id, _name.c_str(), _action, op.name().c_str());					
		if (_id1)
		{
			Vars* v = user->quest_list.vars(_id1);
			if (v)
			{
				if (_id2)
				{
					Vars* tmpvars= user->quest_list.vars(_id2);
					tmpvars->get_value(_name2, _action);
				}
				else
				{
					vars->get_value(_name2,_action);
				}
				v->set_value(op, _name1, _action, _tmp, user);

			}
		}
		else
		{
			if (_id2)
			{
				Vars* tmpvars= user->quest_list.vars(_id2);
				tmpvars->get_value(_name2, _action);
			}
			else
			{
				vars->get_value(_name2,_action);
			}
			vars->set_value(op, _name1, _action, _tmp, user);
		}
		return Action::SUCCESS;
	}
	
private:
	int _tmp;
	int _id1;
	std::string _name1;
	int _id2;
	std::string _name2;
};

/**
 * \brief �û�����
 *
 * �����ṩ�˶�������ű�����Ҫ�������û����ϱ�����֧��
 *
 */	
template <typename Operation>
class UserVarAction : public TeamAction
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	UserVarAction(Parse& p) : TeamAction(p)
	{ 
		p.value("name", _name);
		p.value("value", _action);
		p.value("id", _id);
		p.value("tmp", _tmp);		
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~UserVarAction()
	{ }

	/**     
	 * \brief  ִ�б����޸�
	 *
	 * ������done����,ʵ�ֶ���ر������޸�
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
		
		if (!_id) _id = vars->quest_id();
				
		Vars* v = UserVar::instance().add(_id, ((QWORD)user->charbase.accid << 32) | user->charbase.id);
		v->set_value(op, _name, _action, _tmp, user);
		
		return Action::SUCCESS;
	}
	
private:
	std::string _name;
	value_type _action;
	int _tmp;
	int _id;
};

/**
 * \brief ϵͳ��Ϣ
 *
 * �����ṩ�˶��û��ṩϵͳ������Ϣ��֧��.
 *
 */	
class NotifyAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	NotifyAction(Parse& p) : TeamAction(p)
	{ 
		p.value("content", _info);	
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~NotifyAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	std::string _info;	
};

/**
 * \brief ��־��Ϣ
 *
 * �����ṩ�˶���־��Ϣ��֧��.
 *
 */	
class LogAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	LogAction(Parse& p) : TeamAction(p)
	{ 
		p.value("content", _info);	
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~LogAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	std::string _info;	
};

class BulletinAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	BulletinAction(Parse& p) : TeamAction(p)
	{ 
		p.value("content", _info);	
		p.value("kind", _kind);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~BulletinAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	std::string _info;	
	int _kind; 
};

/**
 * \brief ϵͳ��Ϣ
 *
 * �����ṩ�˶��û��ṩϵͳ������Ϣ��֧��.
 *
 */	
class Notify1Action : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	Notify1Action(Parse& p) : TeamAction(p)
	{ 
		p.value("content", _info);	
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~Notify1Action() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	std::string _info;	
};

/**
 * \brief �˵�
 *
 * �����ṩ�˶Կͻ��˶�̬�˵���֧��.
 *
 */	
class MenuAction : public Action
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param menu: �˵�����
	 */     	
	MenuAction(const std::string& menu) : _menu(menu)
	{ }
	
	virtual ~MenuAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	std::string _menu;	
};

/**
 * \brief �˵�
 *
 * �����ṩ�˶Կͻ��˶�̬�˵���֧��.
 *
 */	
class SubMenuAction : public Action
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param menu: �˵�����
	 */     	
	SubMenuAction(const std::string& menu) : _menu(menu)
	{ }
	
	virtual ~SubMenuAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	std::string _menu;	
};

/**
 * \brief �˵�
 *
 * �����ṩ�˶Կͻ��˶�̬�˵���֧��.
 *
 */	
class MainMenuAction : public Action
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param menu: �˵�����
	 */     	
	MainMenuAction(const std::string& menu) : _menu(menu)
	{ }
	
	virtual ~MainMenuAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	std::string _menu;	
};


/**
 * \brief ����ˢ��
 *
 * ����ʵ���˶���������ļ�ʱˢ��
 *
 */	
class RefreshAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	RefreshAction(Parse& p) : TeamAction(p)
	{ 
		p.value("name", _name);	
		p.value("id", _id);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~RefreshAction() { }	
	
	int done (SceneUser* user, Vars* vars);
private:
	std::string _name;
	int _id;
};

/**
 * \brief ����
 *
 * �����ṩ�������ض��û�����Ľӿ�
 *
 */
class ExpAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	ExpAction(Parse& p) : TeamAction(p)
	{ 
		p.value("value", _exp);	
		p.value("name", _name);
		p.value("id", _id);
		
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~ExpAction() { }	
	
	int done (SceneUser* user, Vars* vars);
protected:
	int _exp;
	std::string _name;
	int _id;
};



class Exp1Action : public ExpAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	Exp1Action(Parse& p) : ExpAction(p)
	{ 
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~Exp1Action() { }	
	
	int done (SceneUser* user, Vars* vars);
};


/**
 * \brief ����
 *
 * �����ṩ���޸��û��������������Ľӿ�
 *
 */	
template <typename Operation>
class GoldAction : public TeamAction
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	GoldAction(Parse& p)  : TeamAction(p)
	{ 
		p.value("value", _value);
		
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~GoldAction() { }	

	/**     
	 * \brief  �������
	 *
	 * ������done����,�����û�����Я��������
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;

		
		if (op.name() == "Add") {
			user->packs.addMoney(_value,"�������");
			return Action::SUCCESS;
		}

		if (op.name() == "Sub") {
			if (!user->packs.removeMoney(_value,"�������")) {
				Zebra::logger->fatal("�������Ӽ������: �û�(%s), ����(%d)", user->name, vars->quest_id());
			}
			
			return Action::SUCCESS;
		}
		
		return Action::FAILED;
	}
	
private:
	value_type _value;
};

class EnterSeptGuardAction : public Action
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	EnterSeptGuardAction()
	{ 
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~EnterSeptGuardAction() { }	
	
	int done (SceneUser* user, Vars* vars);
private:
};

class FinishSeptGuardAction : public Action
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	FinishSeptGuardAction()
	{ 
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~FinishSeptGuardAction() { }	
	
	int done (SceneUser* user, Vars* vars);
};

/**
 * \brief ����
 *
 * �����ṩ�������ض��û����۵Ľӿ�,��δʵ��
 *
 */
template <typename Operation>
class ScoreAction : public TeamAction
{
public:

	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	ScoreAction(Parse& p) : TeamAction(p)
	{ 
		p.value("value", _value);	
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~ScoreAction() { }	
	
	int done (SceneUser* user, Vars* vars)
	{
		return Action::SUCCESS;
	}
	
private:
	value_type _value;
};

/**
 * \brief ����
 *
 * �����ṩ�������ض��û����ܵȼ��Ľӿ�.
 *
 */
class SkillAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	SkillAction(Parse& p) : TeamAction(p)
	{ 
		p.value("id", _id);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~SkillAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	value_type _id;
};

/**
 * \brief ��ֹ
 *
 * �����ṩ�˽�ֹ�û�ִ��ĳ����ķ���,��ǰ֧�ֵ�������ʹ����Ʒ,ʰȡ��Ʒ��������.
 *
 */
class DisableAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	DisableAction(Parse& p)	: TeamAction(p)
	{ 
	
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~DisableAction() { }
	
	int done (SceneUser* user, Vars* vars);
	
private:

};

/**
 * \brief ��ͼ��ת
 *
 * �����ṩ���û��ڵ�ͼ����ת�Ľӿ�
 *
 */
class GotoAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	GotoAction(Parse& p) : TeamAction(p)
	{ 
		p.value("name", _name);	
		p.value("pos", _pos);
		p.value("pos1", _pos1);
		p.value("pos2", _pos2);
		p.value("pos3", _pos3);
		p.value("pos4", _pos4);
		p.value("cpos", _cpos);
		p.value("rlen", _rlen);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~GotoAction() { }
	
	int done (SceneUser* user, Vars* vars);
	
private:
	std::string _name;
	std::string _pos;
	std::string _pos1;
	std::string _pos2;
	std::string _pos3;
	std::string _pos4;
	std::string _cpos;
	std::string _rlen;
};

/**
 * \brief ����
 *
 * �����ṩ��ʹ�û�����Ľӿ�
 *
 */
class RideDownAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	RideDownAction(Parse& p) : TeamAction(p)
	{ }
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~RideDownAction() { }
	
	int done (SceneUser* user, Vars* vars);
	
private:

};

/**
 * \brief �����Ʒ
 *
 * �����ṩ�˸��û����ĳ����Ʒ�Ľӿ�
 *
 */
class AddItemAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	AddItemAction(Parse& p) : TeamAction(p)
	{ 
		p.value("id", _id);
		p.value("value", _value);	
		p.value("odds", _odds);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~AddItemAction() { }
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _id;
	int _value;
	int _odds;
};

/**
 * \brief ��Ӱ���Ʒ
 *
 * �����ṩ�˸��û����ĳ����Ʒ�Ľӿ�
 *
 */
class AddBindItemAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	AddBindItemAction(Parse& p) : TeamAction(p)
	{ 
		p.value("id", _id);
		p.value("value", _value);	
		p.value("odds", _odds);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~AddBindItemAction() { }
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _id;
	int _value;
	int _odds;
};

/**
 * \brief �����ɫ����Ʒ
 *
 * �����ṩ�˸��û����ĳ����Ʒ�Ľӿ�
 *
 */
class AddGreenBindItemAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	AddGreenBindItemAction(Parse& p) : TeamAction(p)
	{ 
		p.value("id", _id);
		p.value("value", _value);	
		p.value("odds", _odds);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~AddGreenBindItemAction() { }
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _id;
	int _value;
	int _odds;
};

/**
 * \brief ɾ����Ʒ
 *
 * �����ṩ��ɾ���û�����ĳ����Ʒ�Ľӿ�
 *
 */
class RemoveItemAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	RemoveItemAction(Parse& p) : TeamAction(p)
	{ 
		p.value("id", _id);
		p.value("value", _value);			
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~RemoveItemAction() { }
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _id;
	int _value;
};

/**
 * \brief ������Ʒ
 *
 * �����ṩ�˶����û�����ĳ����Ʒ�Ľӿ�
 *
 */
class DropItemAction : public TeamAction
{
public:
	typedef int value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	DropItemAction(Parse& p) : TeamAction(p)
	{ 
		p.value("id", _id);
		p.value("level", _level);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~DropItemAction() { }
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _id;
	int _level;
};

/**
 * \brief δʵ��
 *
 * 
 *
 */
class DropAction : public Action
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	DropAction(Parse& p)
	{
		p.value("id", _id);
		p.value("value", _value);	
		p.value("odds", _odds);
		p.value("guard", _guard);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~DropAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _id;
	int _value;
	int _odds;
	int _guard;
};

/**
 * \brief ��ʱ��
 *
 * �����ṩ�˶���Ҫʱ�����Ƶ������֧��
 *
 */
class TimeoutsAction : public Action
{
public:

	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	TimeoutsAction(Parse& p)
	{
		//p.value("value", _timeout);
		p.value("id", _id);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~TimeoutsAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	//int _timeout;
	int _id;
};

/**
 * \brief ����״̬
 *
 * �����ṩ�������û�����ĳ��״̬�Ľӿ�
 *
 */
class SetStateAction : public TeamAction
{
public:

	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	SetStateAction(Parse& p) : TeamAction(p)
	{
		p.value("value", _state);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~SetStateAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _state;
};

/**
 * \brief ���״̬
 *
 * �����ṩ������û�����ĳ��״̬�Ľӿ�
 *
 */
class ClearStateAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	ClearStateAction(Parse& p) : TeamAction(p)
	{
		p.value("value", _state);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~ClearStateAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _state;
};

/**
 * \brief ��ƥ
 *
 * �����ṩ�˸����û���ƥ�Ľӿ�
 *
 */
class HorseAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	HorseAction(Parse& p) : TeamAction(p)
	{
		p.value("value", _id);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~HorseAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _id;
};


/**
 * \brief ȫ�ֱ���
 *
 * �����ṩ�˶�������ű�����Ҫ�������û��ɼ�������֧��
 *
 */	
template <typename Operation>
class GlobalAction : public Action
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	GlobalAction(Parse& p) : _id(0)
	{ 
		p.value("name", _name);
		p.value("value", _action);
		p.value("id", _id);
		p.value("tmp", _tmp);		
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~GlobalAction()
	{ }
	
	bool check_args(SceneUser* user, Vars* vars) const
	{
		if (!vars) return false;
		
		return true;		
	}

	/**     
	 * \brief  ִ�б����޸�
	 *
	 * ������done����,ʵ�ֶ���ر������޸�
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
		if (!_id) _id = vars->quest_id();

//		Zebra::logger->debug("id:%d\tname:%s\taction:%d\toperation:%s", _id, _name.c_str(), _action, op.name().c_str());
				
		Vars* v = GlobalVar::instance().add(_id);
		v->set_value(op, _name, _action, _tmp, user);

		return Action::SUCCESS;
	}
	
protected:
	std::string _name;
	value_type _action;
	int _tmp;
	int _id;
};

template <typename Operation>
class TongVarAction : public GlobalAction<Operation>
{
public:
	typedef typename Operation::value_type value_type;
	
	TongVarAction(Parse& p) : GlobalAction<Operation>(p)
	{ 
	}

	virtual ~TongVarAction()
	{ }
	
	bool check_args(SceneUser* user, Vars* vars) const
	{
		if (!user) return false;
		
		return true;		
	}

	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
				
		Vars* v = TongVar::instance().add(user->charbase.unionid);
		v->set_value(op, GlobalAction<Operation>::_name, GlobalAction<Operation>::_action, GlobalAction<Operation>::_tmp, user);

		return Action::SUCCESS;
	}	
};

#include "SessionClient.h"
template <typename Operation>
class FamilyLevelAction : public TongVarAction<Operation>
{
public:
	typedef typename Operation::value_type value_type;
	
	FamilyLevelAction(Parse& p) : TongVarAction<Operation>(p),_level(0)
	{ 
		p.value("value" ,_level);
		Zebra::logger->debug("��ʼ��_level%d", _level);
	}

	virtual ~FamilyLevelAction()
	{ }

	int done (SceneUser* user, Vars* vars)
	{
		Cmd::Session::t_OpLevel_SceneSession uplevel;
		uplevel.dwSeptID=user->charbase.septid;
		uplevel.dwLevel=_level;
		Zebra::logger->debug("����_level%d", _level);
		sessionClient->sendCmd(&uplevel,sizeof(uplevel));
		return Action::SUCCESS;
	}
	
private:
	value_type _level;
};

template <typename Operation>
class FamilyVarAction : public TongVarAction<Operation>
{
public:
	typedef typename Operation::value_type value_type;
	
	FamilyVarAction(Parse& p) : TongVarAction<Operation>(p)
	{ 
	}

	virtual ~FamilyVarAction()
	{ }
	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
				
		Vars* v = FamilyVar::instance().add(user->charbase.septid);
		v->set_value(op, GlobalAction<Operation>::_name, GlobalAction<Operation>::_action, GlobalAction<Operation>::_tmp, user);

		return Action::SUCCESS;
	}	
};

template <typename Operation>
class UsersVarAction : public TongVarAction<Operation>
{
public:
	typedef typename Operation::value_type value_type;
	
	UsersVarAction(Parse& p) : TongVarAction<Operation>(p)
	{ 
	}

	virtual ~UsersVarAction()
	{ }
	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
				
		Vars* v = UsersVar::instance().add(((QWORD)user->charbase.accid << 32) |user->charbase.id);
		v->set_value(op, GlobalAction<Operation>::_name, GlobalAction<Operation>::_action, GlobalAction<Operation>::_tmp, user);

		return Action::SUCCESS;
	}	
};

#include "zRegex.h"
#include "SceneNpc.h"
#include "SceneManager.h"

/**
 * \brief  NPC
 *
 * �����ṩ�˶Գ�����NPC���ʵ�֧��
 *
 */
class NpcAction : public Action
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	NpcAction(Parse& p)
	{ 
		p.value("id", _id);
		std::string map;
		p.value("map", map);
		_s = SceneManager::getInstance().getSceneByName(map.c_str());
		if (!_s) {
			Zebra::logger->debug("NpcActionʱ����ĳ���(%s)������", map.c_str());
		}
		
		std::string pos;		
		p.value("ltpos", pos);	
		zRegex regex;
		if((regex.compile("(.*)[ ,](.*)") && regex.match(pos.c_str()))) {
			std::string s;
			regex.getSub(s, 1);
			_ltpos.x = atoi(s.c_str());
			regex.getSub(s, 2);
			_ltpos.y = atoi(s.c_str());
		}

		p.value("rbpos", pos);	
		if((regex.compile("(.*)[ ,](.*)") && regex.match(pos.c_str()))) {
			std::string s;
			regex.getSub(s, 1);
			_rbpos.x = atoi(s.c_str());
			regex.getSub(s, 2);
			_rbpos.y = atoi(s.c_str());
		}
		
		p.value("num", _num);
		if (!_num) _num = 1;
		if (_num>100) _num = 100;
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~NpcAction()
	{ }
	
	bool check_args(SceneUser* user, Vars* vars) const
	{
		return true;
	}
		
protected:
	Scene* _s;
	int _id;
	zPos _ltpos;
	zPos _rbpos;
	int _num;
	
};

/**
 * \brief ����NPC
 *
 * �����ṩ���ڵ�ͼ������һ��NPC�Ľӿ�
 *
 */
class AddNpcAction : public NpcAction
{
public:

	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	AddNpcAction(Parse& p) : NpcAction(p)
	{ }
	
	/**     
	 * \brief ��������
	 *
	 */     
	~AddNpcAction()
	{ }
	
	int done(SceneUser* user, Vars* vars);	
	
};

/**
 * \brief �����ڳ�
 *
 * �����ṩ�˶Ի��������֧��
 *
 */
class AddGuardAction : public NpcAction
{
public:

	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	AddGuardAction(Parse& p) : NpcAction(p)
	{ 
		p.value("id2", _id2);	
		p.value("id3", _id3);	
		p.value("odds1", _odds1);	
		p.value("odds2", _odds2);	
		p.value("odds3", _odds3);	
		p.value("gold", _gold);	
		p.value("path", _path);
		p.value("exp", _exp);
		p.value("script", _script);
		p.value("map", _map);

		if (_odds1+_odds2+_odds3==0)
			_odds1 = 100;

		std::string pos;		
		p.value("dest", pos);	
		zRegex regex;
		if((regex.compile("(.*)[ ,](.*)") && regex.match(pos.c_str()))) {
			std::string s;
			regex.getSub(s, 1);
			_dest.x = atoi(s.c_str());
			regex.getSub(s, 2);
			_dest.y = atoi(s.c_str());
			//Zebra::logger->debug("AddGuardAction::AddGuardAction(): _dest=%u,%u", _dest.x, _dest.y);
		}
		else
			Zebra::logger->trace("AddGuardAction::AddGuardAction(): _destƥ��ʧ�� %s", pos.c_str());
		
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	~AddGuardAction()
	{ }
	
	int done(SceneUser* user, Vars* vars);	

	bool check_args(SceneUser* user, Vars* vars) const
	{
		if (user) return true;
		
		return false;
	}
	
	private:
		std::string _path;
		int _gold;
		int _exp;
		int _script;
		zPos _dest;
		std::string _map;
		DWORD _id2, _id3;
		DWORD _odds1, _odds2, _odds3;
};

/**
 * \brief ɾ��NPC
 *
 * �����ṩ���ڵ�ͼ��ɾ��һ��NPC�Ľӿ�
 *
 */
class RemoveNpcAction : public NpcAction
{
public:

	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	RemoveNpcAction(Parse& p) : NpcAction(p), _remove(_s, _id, _ltpos, _rbpos)
	{ }
	
	/**     
	 * \brief ��������
	 *
	 */     
	~RemoveNpcAction()
	{ }
	
	int done(SceneUser* user, Vars* vars);		
private:

	class RemoveNpc : public removeEntry_Pred<SceneNpc>
	{
	public:		
		RemoveNpc(Scene* s, int id, const zPos& ltpos, const zPos& rbpos) : _s(s), _id(id), _ltpos(ltpos), _rbpos(rbpos)
		{
		}

		bool isIt(SceneNpc *npc)
		{
			if ((int)npc->id == _id && (npc->getPos() >= _ltpos && npc->getPos() <= _rbpos) ) {
				npc->setState(zSceneEntry::SceneEntry_Hide);
				Cmd::stAddMapNpcMapScreenUserCmd addNpc;
				npc->full_t_MapNpcData(addNpc.data);
				_s->sendCmdToNine(npc->getPosI(), &addNpc, sizeof(addNpc));
				return true;
			}

			return false;
		}
	private:
		Scene* _s;
		int _id;
		const zPos& _ltpos;
		const zPos& _rbpos;
	};
	
	RemoveNpc _remove;
};

/**
 * \brief ����
 *
 * �����ṩ�˶�������ű����й����е������޸ĵ�֧��
 *
 */	
template <typename Operation>
class FiveLevelAction : public TeamAction
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	FiveLevelAction(Parse& p) : TeamAction(p)
	{ 
		p.value("value", _level);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~FiveLevelAction()
	{ }

	/**     
	 * \brief  ִ�б����޸�
	 *
	 * ������done����,ʵ�ֶ���ر������޸�
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
		op((int &)user->charbase.fivelevel, _level, user);

		Cmd::stMainUserDataUserCmd  userinfo;
		user->full_t_MainUserData(userinfo.data);
		user->sendCmdToMe(&userinfo,sizeof(userinfo));

		return Action::SUCCESS;
	}
	
private:
	value_type _level;
};

/**
 * \brief ������������
 *
 * �����ṩ�������������͵Ľӿ�
 *
 */
class FiveTypeAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	FiveTypeAction(Parse& p) : TeamAction(p)
	{
		p.value("value", _type);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~FiveTypeAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _type;
};

template <typename Operation>
class HonorAction : public TeamAction
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	HonorAction(Parse& p) : TeamAction(p)
	{ 
		p.value("value", _honor);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~HonorAction()
	{ }

	/**     
	 * \brief  ִ�б����޸�
	 *
	 * ������done����,ʵ�ֶ���ر������޸�
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
		op((int &)user->charbase.honor, _honor, user);

		Cmd::stMainUserDataUserCmd  userinfo;
		user->full_t_MainUserData(userinfo.data);
		user->sendCmdToMe(&userinfo,sizeof(userinfo));

		return Action::SUCCESS;
	}
	
private:
	value_type _honor;
};

template <typename Operation>
class MaxHonorAction : public TeamAction
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	MaxHonorAction(Parse& p) : TeamAction(p)
	{ 
		p.value("value", _honor);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~MaxHonorAction()
	{ }

	/**     
	 * \brief  ִ�б����޸�
	 *
	 * ������done����,ʵ�ֶ���ر������޸�
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     	
	int done (SceneUser* user, Vars* vars)
	{
		Operation op;
		op((int &)user->charbase.maxhonor, _honor, user);

		Cmd::stMainUserDataUserCmd  userinfo;
		user->full_t_MainUserData(userinfo.data);
		user->sendCmdToMe(&userinfo,sizeof(userinfo));

		return Action::SUCCESS;
	}
	
private:
	value_type _honor;
};


template <typename Operation>
class ActionPointAction : public TeamAction
{
public:
	typedef typename Operation::value_type value_type;
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	ActionPointAction(Parse& p) : TeamAction(p)
	{ 
		p.value("value", _ap);
	}

	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~ActionPointAction()
	{ }

	/**     
	 * \brief  ִ�б����޸�
	 *
	 * ������done����,ʵ�ֶ���ر������޸�
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return SUCCESS��ʾ�ɹ���FAILED��ʾʧ�ܣ�DISABLE��ʾ����ĳ���
	 */     	
	int done (SceneUser* user, Vars* vars)
	{
		Cmd::Session::t_ChangeAP  cmd;;
		cmd.id = user->charbase.unionid;
		cmd.point = _ap;
		sessionClient->sendCmd(&cmd, sizeof(cmd));
		
		return Action::SUCCESS;
	}
	
private:
	value_type _ap;
};

class UseSkillAction : public TeamAction
{
public:
	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	UseSkillAction(Parse& p) : TeamAction(p)
	{
		p.value("id", _id);
		p.value("level", _level);
	}
	
	/**     
	 * \brief ��������
	 *
	 */     
	virtual ~UseSkillAction() { }	
	
	int done (SceneUser* user, Vars* vars);
	
private:
	int _id;
	int _level;
};



#endif
