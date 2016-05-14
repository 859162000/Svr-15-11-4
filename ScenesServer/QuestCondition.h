/**
 * \file
 * \version	$Id: QuestCondition.h  $
 * \author	
 * \date	
 * \brief	�ű���������
 * 
 */
 
#ifndef __QUEST_CONDITION_H__
#define __QUEST_CONDITION_H__

#include <string>

#include "Quest.h"
#include "QuestParse.h"

#include "Zebra.h"
#include "SceneUser.h"
#include "Scene.h"

namespace Op {
	/**
	 * \brief ������С�ж�
	 *
	 * �ж�һ�������Ƿ���ڸ���ֵ
	 *
	 */
	template <typename T>
	class Great
	{
	public:
		enum {
			NEED_EXIST = 1,
		};
		
		typedef T value_type;
		
		/**     
		 * \brief �жϸ��������Ƿ���ڲ���ֵ
		 *
		 * \param value: ���жϱ���
		 * \param condition: ����ֵ
		 * \return ���жϱ������ڲ���ֵ����true,���򷵻�false
		 */     		
		bool operator() (T value, T condition) const
		{
			return (value>condition);
		}
		
		//added for debug
		std::string name() const 
		{
			return "Great";
		}
	};
	
	/**
	 * \brief ������С�ж�
	 *
	 * �ж�һ�������Ƿ�С�ڸ���ֵ
	 *
	 */
	template <typename T>
	class Less
	{
	public:
		enum {
			NEED_EXIST = 0,
		};

		typedef T value_type;
		
		/**     
		 * \brief �жϸ��������Ƿ�С�ڲ���ֵ
		 *
		 * \param value: ���жϱ���
		 * \param condition: ����ֵ
		 * \return ���жϱ���С�ڲ���ֵ����true,���򷵻�false
		 */     		
		bool operator() (T value, T condition) const
		{
			return (value<condition);
		}

		//added for debug
		std::string name() const 
		{
			return "Less";
		}
	};
	
	/**
	 * \brief ������С�ж�
	 *
	 * �ж�һ�������Ƿ���ڸ���ֵ
	 *
	 */
	template <typename T>
	class Equal
	{
	public:
		enum {
			NEED_EXIST = 1,
		};

		typedef T value_type;
		
		/**     
		 * \brief �жϸ��������Ƿ���ڲ���ֵ
		 *
		 * \param value: ���жϱ���
		 * \param condition: ����ֵ
		 * \return ���жϱ������ڲ���ֵ����true,���򷵻�false
		 */     		
		bool operator() (T value, T condition) const
		{
			return (value==condition);
		}

		//added for debug
		std::string name() const 
		{
			return "Equal";
		}
		
	};
	
	/**
	 * \brief ������С�ж�
	 *
	 * �ж�һ��������ͬ�ڸ���ֵ
	 *
	 */
	template <typename T>
	class Differ
	{
	public:
		enum {
			NEED_EXIST = 0,
		};

		typedef T value_type;
		
		/**     
		 * \brief �жϸ��������Ƿ񲻵��ڲ���ֵ
		 *
		 * \param value: ���жϱ���
		 * \param condition: ����ֵ
		 * \return ���жϱ��������ڲ���ֵ����true,���򷵻�false
		 */     		
		bool operator() (T value, T condition) const
		{
			return (value!=condition);
		}

		//added for debug
		std::string name() const 
		{
			return "Differ";
		}
		
	};
}

/**
 * \brief ������������
 *
 * ���ඨ���˽ű����������Ľӿڡ�
 *
 */	
class Condition
{
public:
	typedef Condition Base;

	virtual bool is_valid (const SceneUser* user, const Vars* vars) const;

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~Condition() { }
	
protected:
	virtual bool check_args(const SceneUser* user, const Vars* vars) const;
	
	virtual bool check_valid(const SceneUser* user, const Vars* vars) const = 0;
};

/**
 * \brief ��Ӵ�����������
 *
 * �����ṩ�˶�����������֧��
 *
 */	
class TeamCondition : public Condition
{
public:
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	TeamCondition(Parse& p)
	{ 
		p.value("team", _team);
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~TeamCondition() { }
	
	virtual bool is_valid(const SceneUser* user, const Vars* vars) const;
	
protected:	
	
private:
	int _team;
};

/**
 * \brief ��������
 *
 * �����ṩ�˶�������ű����йر����������ж���֧��
 *
 */
template <typename Operation>
class VarCondition : public TeamCondition
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
	VarCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("name", _name);
		p.value("value", _condition);
		p.value("id", _id);
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~VarCondition()
	{ }

	
	/**     
	 * \brief  �����ж�
	 *
	 * ������check_valid����,�ж�ĳ�������Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;
//		Zebra::logger->debug("id:%d\tname:%s\tcondition:%d\toperation:%s", _id, _name.c_str(), _condition, op.name().c_str());
		if (_id) {
			Vars* v = user->quest_list.vars(_id);
			if (v) {
				return v->is_valid(op, _name, _condition);
			}else {
				return !Operation::NEED_EXIST;
			}
		}
			
		return vars->is_valid(op, _name, _condition);
	};

private:
	std::string _name;
	value_type _condition;
	int _id;
};

/**
 * \brief �û���������
 *
 * �����ṩ�˶�������ű����й��û������������ж���֧��
 *
 */
template <typename Operation>
class UserVarCondition : public TeamCondition
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
	UserVarCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("name", _name);
		p.value("value", _condition);
		p.value("id", _id);
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~UserVarCondition()
	{ }

	/**     
	 * \brief  �û������ж�
	 *
	 * ������check_valid����,�ж�ĳ���û������Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;
		int id = _id?_id:vars->quest_id();

		QWORD key = ((QWORD)user->charbase.accid << 32) | user->charbase.id;
		Vars* v = UserVar::instance().vars(id, key);
		if (v) {
			return v->is_valid(op, _name, _condition);
		}

		return !Operation::NEED_EXIST;
		
	};

private:
	std::string _name;
	value_type _condition;
	int _id;
};

/**
 * \brief ȫ�ֱ�������
 *
 * �����ṩ�˶�������ű����й�ȫ�ֱ����������ж���֧��
 *
 */
template <typename Operation>
class GlobalCondition : public Condition
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
	GlobalCondition(Parse& p)
	{ 
		p.value("name", _name);
		p.value("value", _condition);
		p.value("id", _id);
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~GlobalCondition()
	{ }

	/**     
	 * \brief  �������
	 *
	 * ������check_args����,����������Ҫ�û���Ϣ
	 *      
	 * \param user: NULL
	 * \param vars: ������Ϣ
	 * \return ������Ч����true,���򷵻�false
	 */ 	
	bool check_args(const SceneUser* user, const Vars* vars) const
	{
		if (!vars) return false;
		
		return true;
	}

	/**     
	 * \brief  ȫ�ֱ����ж�
	 *
	 * ������check_valid����,�ж�ĳ��ȫ�ֱ����Ƿ�����ű�Ҫ��
	 *      
	 * \param user: NULL
	 * \param vars: ������Ϣ
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;
		int id = _id?_id:vars->quest_id();

//		Zebra::logger->debug("id:%d\tname:%s\tcondition:%d\toperation:%s", id, _name.c_str(), _condition, op.name().c_str());
		
		Vars* v = GlobalVar::instance().vars(id);
		if (v) {
			return v->is_valid(op, _name, _condition);
		}
		
		return !Operation::NEED_EXIST;
	};

protected:
	std::string _name;
	value_type _condition;
	int _id;
};

template <typename Operation>
class TongVarCondition : public GlobalCondition<Operation>
{
public:
	TongVarCondition(Parse& p) : GlobalCondition<Operation>(p)
	{ 
	}

	virtual ~TongVarCondition()
	{ }

	bool check_args(const SceneUser* user, const Vars* vars) const
	{
		if (!user) return false;
		
		return true;
	}

	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;

		Vars* v = TongVar::instance().vars(user->charbase.unionid);
		if (v) {
			return v->is_valid(op, GlobalCondition<Operation>::_name, GlobalCondition<Operation>::_condition);
		}
		
		return !Operation::NEED_EXIST;
	};
};

template <typename Operation>
class FamilyVarCondition : public TongVarCondition<Operation>
{
public:
	FamilyVarCondition(Parse& p) : TongVarCondition<Operation>(p)
	{ 
	}

	virtual ~FamilyVarCondition()
	{ }

	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;

		Vars* v = FamilyVar::instance().vars(user->charbase.septid);
		if (v) {
			return v->is_valid(op, GlobalCondition<Operation>::_name, GlobalCondition<Operation>::_condition);
		}
		
		return !Operation::NEED_EXIST;
	};
};

template <typename Operation>
class UsersVarCondition : public TongVarCondition<Operation>
{
public:
	UsersVarCondition(Parse& p) : TongVarCondition<Operation>(p)
	{ 
	}

	virtual ~UsersVarCondition()
	{ }

	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;

		Vars* v = UsersVar::instance().vars( ((QWORD)user->charbase.accid << 32) | user->charbase.id);
		if (v) {
			return v->is_valid(op, GlobalCondition<Operation>::_name, GlobalCondition<Operation>::_condition);
		}
		
		return !Operation::NEED_EXIST;
	};
};

/**
 * \brief �����Ƿ�ѧϰ����
 *
 * �����ṩ���ж��û��Ƿ�ѧϰ����
 *
 */
template <typename Operation>
class HaveSkillCondition : public TeamCondition
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
	HaveSkillCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~HaveSkillCondition()
	{ }
	
	/**     
	 * \brief  ���ܸ����ж�
	 *
	 * ������check_valid����,�ж��û��ļ��������Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		Operation op;
		int size=user->usm.size();
		return op(size, _condition);
	}

private:
	value_type _condition;		
};


/**
 * \brief �������Ա�
 *
 * �����ṩ���ж��û����Ա�
 *
 */
template <typename Operation>
class CheckSexCondition : public TeamCondition
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
	CheckSexCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~CheckSexCondition()
	{ }
	
	/**     
	 * \brief  ���ܸ����ж�
	 *
	 * ������check_valid����,�ж��û��ļ��������Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		Operation op;
		int sex = 0;
		switch(user->charbase.type)
		{
			case PROFESSION_1:		//����
			case PROFESSION_3:		//����
			case PROFESSION_5:		//��ʦ
			case PROFESSION_7:		//��ʦ
				sex = 1;
				break;
			case PROFESSION_2:		//��Ů
			case PROFESSION_4:		//����
			case PROFESSION_6:		//��Ů
			case PROFESSION_8:		//��Ů
				sex = 0;
				break;
			case PROFESSION_NONE:	//��ҵ
			default:
				break;
		}
		return op(sex, _condition);
	}

private:
	value_type _condition;		
};

/**
 * \brief �ȼ�����
 *
 * �����ṩ���ж��û��ȼ��Ƿ����,С��,���ڻ򲻵���ĳ��ֵ�Ľӿ�
 *
 */
template <typename Operation>
class LevelCondition : public TeamCondition
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
	LevelCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~LevelCondition()
	{ }
	
	/**     
	 * \brief  �ȼ��ж�
	 *
	 * ������check_valid����,�ж��û��ĵȼ��Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		Operation op;
		return op(user->charbase.level, _condition);
	}

private:
	value_type _condition;		
};


/**
 * \brief ��������
 *
 * �����ṩ���ж��û�����ID�Ƿ����,С��,���ڻ򲻵���ĳ��ֵ�Ľӿ�
 *
 */
template <typename Operation>
class SeptIDCondition : public TeamCondition
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
	SeptIDCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~SeptIDCondition()
	{ }
	
	/**     
	 * \brief  �����ж�
	 *
	 * ������check_valid����,�ж��û��ļ���ID�Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		Operation op;
		return op(user->charbase.septid, _condition);
	}

private:
	value_type _condition;		
};

/**
 * \brief �������
 *
 * �����ṩ���ж��û�����ID�Ƿ����,С��,���ڻ򲻵���ĳ��ֵ�Ľӿ�
 *
 */
template <typename Operation>
class UnionIDCondition : public TeamCondition
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
	UnionIDCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~UnionIDCondition()
	{ }
	
	/**     
	 * \brief  ����ж�
	 *
	 * ������check_valid����,�ж��û��ļ���ID�Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		Operation op;
		return op(user->charbase.unionid, _condition);
	}

private:
	value_type _condition;		
};

#include "Chat.h"

/**
 * \brief ��Ǯ����
 *
 * �����ṩ���ж��û������н�Ǯ�����Ƿ����,С��,���ڻ򲻵���ĳ��ֵ�Ľӿ�
 *
 */
template <typename Operation>
class GoldCondition : public TeamCondition
{
public:	
	/**     
	 * \brief  ���캯��
	 *
	 * ��ʼ����ر���
	 *      
	 * \param p: �ؼ����б�
	 */     	
	GoldCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~GoldCondition()
	{ }

	/**     
	 * \brief  ��Ǯ�ж�
	 *
	 * ������check_valid����,�ж��û������еĽ�Ǯ�Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		zObject* gold = const_cast<SceneUser *>(user)->packs.getGold();
		
		DWORD number = 0;
		if (gold)  number = gold->data.dwNum;
		Operation op;

		//shouldn't be exist, NB CEHUA
		//if (op.name() == "Great" && number < (DWORD)_condition) {
			//Channel::sendSys(const_cast<SceneUser*>(user), Cmd::INFO_TYPE_FAIL, "��Ǯ����");
		//}
		
		return op(number, _condition);
	}

private:
	int _condition;		
};

/**
 * \brief ��Ʒ����
 *
 * �����ṩ���ж��û������е��ض���Ʒ�����Ƿ����,С��,���ڻ򲻵���ĳ��ֵ�Ľӿ�
 *
 */
class ItemCondition : public TeamCondition
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
	ItemCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("id", _id);
		p.value("value", _condition);
		p.value("level", _level);
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~ItemCondition()
	{ }

	/**     
	 * \brief  ��Ʒ�����ж�
	 *
	 * ������check_valid����,�ж��û������ռ��ĳ����Ʒ�����Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		return user->packs.uom.exist(_id, _condition, _level);
	}

private:
	int _id;
	int _level;
	value_type _condition;		
};

/**
 * \brief ��������
 *
 * �����ṩ���ж��û��Ƿ�����һ�����ҵĽӿ�
 *
 */
class NationCondition : public TeamCondition
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
	NationCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~NationCondition()
	{ }
	
	/**     
	 * \brief  �����ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�����ĳ������
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		return (int)user->charbase.country == _condition;
	}

private:
	value_type _condition;		
};

/**
 * \brief �Ƿ��ڱ�������
 *
 * �����ṩ���ж��û��Ƿ��ڱ����Ľӿ�
 *
 */
class InNationCondition : public TeamCondition
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
	InNationCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~InNationCondition()
	{ }
	
	/**     
	 * \brief  �����ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�����ĳ������
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		return user->charbase.country == user->scene->getCountryID();
	}

private:
	value_type _condition;		
};

/**
 * \brief �������
 *
 * �����ṩ���ж��û��Ƿ�����ĳ�����Ľӿ�
 *
 */
class ConfraternityCondition : public TeamCondition
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
	ConfraternityCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~ConfraternityCondition()
	{ }
	
	/**     
	 * \brief  ����ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�����ĳ�����
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		return (int)(user->charbase.unionid & 0x1) == _condition;
	}

private:
	value_type _condition;		
};

/**
 * \brief ְҵ����
 *
 * �����ṩ���ж��û��Ƿ�����ĳ��ְҵ�Ľӿ�
 *
 */
class ProfessionCondition : public TeamCondition
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
	ProfessionCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _condition);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~ProfessionCondition()
	{ }
	
	/**     
	 * \brief  ְҵ�ж�
	 *
	 * ������check_valid����,�ж��û�ְҵ�Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		return user->charbase.type & _condition;
	}

private:
	value_type _condition;		
};

/**
 * \brief �����ռ�����
 *
 * �����ṩ���ж��û������еĿռ��Ƿ�������ĳ����Ʒ�Ľӿ�
 *
 */
class SpaceCondition : public TeamCondition
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
	SpaceCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("size", _size);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~SpaceCondition()
	{ }
	
	/**     
	 * \brief  �����ռ��ж�
	 *
	 * ������check_valid����,�ж��û������ռ��Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		int free = user->packs.uom.space(user);

		if ( free >= _size) 	return true;

		//Channel::sendSys(const_cast<SceneUser*>(user), Cmd::INFO_TYPE_FAIL, "�����ռ䲻��");
		return false;
	}

private:
	value_type _size;		
};

/**
 * \brief �Ƿ񳬹�����ʱ��Ҫ��
 *
 * �����ṩ���ж�����ʱ���Ƿ�����Ҫ��Ľӿ�
 *
 */
class TimeoutsCondition : public TeamCondition
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
	TimeoutsCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("long", _time);	
		p.value("id", _id);	
		p.value("less", _less);
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~TimeoutsCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж�����ʱ���Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		if (_id) {
			Vars* v = user->quest_list.vars(_id);
			if (v) {
				return _less?(v->is_timeout(_time)):(!v->is_timeout(_time));
			}
			return true;
		}
			
		return _less?(vars->is_timeout(_time)):(!vars->is_timeout(_time));
	}

private:
	value_type _time;		
	int _id;
	int _less;
};

/**
 * \brief ʱ������
 *
 * �����ṩ���ж�ϵͳʱ���Ƿ�����Ҫ��Ľӿ�
 *
 */
class TimerCondition : public Condition
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
	TimerCondition(Parse& p)
	{ 
		std::string start;
		p.value("start", start);
		strptime(start.c_str(), "%H:%M:%S", &_start);		
		std::string end;
		p.value("end", end);
		strptime(end.c_str(), "%H:%M:%S", &_end);				
	}


	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~TimerCondition()
	{ }

	/**     
	 * \brief  �������
	 *
	 * ������check_args����,����������Ҫ�û���������Ϣ
	 *      
	 * \param user: NULL
	 * \param vars: NULL
	 * \return ��ǰ���Ƿ���true
	 */ 	
	bool check_args(const SceneUser* user, const Vars* vars) const
	{
		return true;
	}

	/**     
	 * \brief  ʱ�������ж�
	 *
	 * ������check_valid����,�ж�ϵͳʱ���Ƿ�����ű�Ҫ��
	 *      
	 * \param user: NULL
	 * \param vars: NULL
	 * \return true��ʾ��������,false��ʾ����������
	 */   	
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
        	struct tm now;
        	time_t timValue = time(NULL);
        	zRTime::getLocalTime(now, timValue);
		//Zebra::logger->debug("Time_START:%02d:%02d:%02d", _start.tm_hour, _start.tm_min, _start.tm_sec);
		//Zebra::logger->debug("Time_NOW  :%02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
		//Zebra::logger->debug("Time_END  :%02d:%02d:%02d", _end.tm_hour, _end.tm_min, _end.tm_sec);
		if ( ((now.tm_hour > _start.tm_hour) || ( (now.tm_hour == _start.tm_hour) && (now.tm_min > _start.tm_min) ) 
			|| ((now.tm_hour == _start.tm_hour) && (now.tm_min == _start.tm_min) && (now.tm_sec >= _start.tm_sec))) //start
			&& ((now.tm_hour < _end.tm_hour) || ( (now.tm_hour == _end.tm_hour) && (now.tm_min < _end.tm_min) ) 
			|| ((now.tm_hour == _end.tm_hour) && (now.tm_min == _end.tm_min) && (now.tm_sec < _end.tm_sec))) 	 //end
			) {
			return true;
		}	

		return false;	
	}

private:
	struct tm _start;
	struct tm _end;	
};

/**
 * \brief �������
 *
 * �����ṩ���ж��û������Ƿ�����Ҫ��Ľӿ�
 *
 */
class TeamedCondition : public Condition
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
	TeamedCondition(Parse& p) 
	{ 
		p.value("number", _number);	
		p.value("male", _male);
		p.value("female", _female);
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~TeamedCondition()
	{ }
	
	bool check_valid(const SceneUser* user, const Vars* vars) const;

private:
	value_type _number;	
	value_type _male;
	value_type _female;
};

class IsGuardCondition : public TeamCondition
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
	IsGuardCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _need);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~IsGuardCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж�����ʱ���Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		int guard = user->guard?1:0;
		return guard == _need;
	}
private:
	value_type _need;
};

class FiveTypeCondition : public TeamCondition
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
	FiveTypeCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _need);	
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~FiveTypeCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж�����ʱ���Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		return (int)user->charbase.fivetype == _need;
	}
private:
	value_type _need;
};

template <typename Operation>
class FiveLevelCondition : public TeamCondition
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
	FiveLevelCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _level);
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~FiveLevelCondition()
	{ }

	
	/**     
	 * \brief  �����ж�
	 *
	 * ������check_valid����,�ж�ĳ�������Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;
			
		return op((int)user->charbase.fivelevel, _level);
	};

private:
	value_type _level;
};

template <typename Operation>
class FamilyLevelCondition : public TeamCondition
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
	FamilyLevelCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _level);
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~FamilyLevelCondition() { }

	
	/**     
	 * \brief  �����ж�
	 *
	 * ������check_valid����,�ж�ĳ�������Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;
			
		return op((int)user->dwSeptLevel, _level);
	};

private:
	value_type _level;
};

template <typename Operation>
class ReputeCondition : public TeamCondition
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
	ReputeCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _level);
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~ReputeCondition()
	{ }

	
	/**     
	 * \brief  �����ж�
	 *
	 * ������check_valid����,�ж�ĳ�������Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;
			
		return op((int)user->dwSeptRepute, _level);
	};

private:
	value_type _level;
};

template <typename Operation>
class ActionPointCondition : public TeamCondition
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
	ActionPointCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _level);
	}
	
	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~ActionPointCondition()
	{ }

	
	/**     
	 * \brief  �����ж�
	 *
	 * ������check_valid����,�ж�ĳ�������Ƿ�����ű�Ҫ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid (const SceneUser* user, const Vars* vars) const
	{
		Operation op;
			
		return op((int)user->dwUnionActionPoint, _level);
	};

private:
	value_type _level;
};

class HorseCondition : public TeamCondition
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
	HorseCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _id);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~HorseCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�ӵ��������ƥ
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		return user->horse.horse() == (DWORD)_id;
	}
private:
	value_type _id;
};

class GradeCondition : public TeamCondition
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
	GradeCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _grade);	
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~GradeCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�ӵ��������ƥ
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const
	{
		if (_grade == 1) return user->king;
		if (_grade == 2) return user->unionMaster;
		if (_grade == 4) return user->septMaster;
		
		return true;
	}
private:
	value_type _grade;
};

class MapCountryCondition : public TeamCondition
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
	MapCountryCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("name", _name);
		p.value("id", _id);
	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~MapCountryCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�ӵ��������ƥ
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const;
private:
	std::string _name;
	int _id;
};

class HonorCondition : public TeamCondition
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
	HonorCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _value);
 	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~HonorCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�ӵ��������ƥ
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const;
private:
	int _value;
};

class MaxHonorCondition : public TeamCondition
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
	MaxHonorCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _value);
 	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~MaxHonorCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�ӵ��������ƥ
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const;
private:
	int _value;
};

class SelfCountryCondition : public TeamCondition
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
	SelfCountryCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _value);
 	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~SelfCountryCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж��û��Ƿ�ӵ��������ƥ
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const;
private:
	int _value;
};

class CountryPowerCondition : public TeamCondition
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
	CountryPowerCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _value);
 	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~CountryPowerCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж�����ǿ��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const;
private:
	int _value;
};


class WeekCondition : public TeamCondition
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
	WeekCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _value);
 	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~WeekCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж������Ƿ���һ������ָ�����Ǽ��� valueֵ����0-6λ��ʾһ�ܵ�7��
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const;
private:
	int _value;
};


class CaptionCondition : public TeamCondition
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
	CaptionCondition(Parse& p) : TeamCondition(p)
	{ 
		p.value("value", _value);
 	}

	/**     
	 * \brief ��������
	 *
	 */    
	virtual ~CaptionCondition()
	{ }
	
	/**     
	 * \brief  ����ʱ���ж�
	 *
	 * ������check_valid����,�ж���ɫ�Ƿ�������߳���
	 *      
	 * \param user: �����������û�
	 * \param vars: �û������ĸ�������ر���
	 * \return true��ʾ��������,false��ʾ����������
	 */    
	bool check_valid(const SceneUser* user, const Vars* vars) const;
private:
	int _value;
};

#endif

