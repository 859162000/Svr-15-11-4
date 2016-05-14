/**
 * \file
 * \version	$Id: QuestMaker.h  $
 * \author	
 * \date		2005-04-07
 * \brief		��Ʒ��������
 * 
 */
 
#ifndef __QUEST_MAKER_H__
#define __QUEST_MAKER_H__

/**
 * \brief ��Ʒ����
 *
 * ��װ�˹���ģʽ
 *
 */
template <typename I, typename C>
class ProductFactory
{
public:
	typedef ProductFactory<I, C> self_t;
	
	/**     
	 * \brief 
	 *
	 * ʵ���˵���ģʽ
	 *      
	 * \return �����Ψһʵ��
	 */     	
	static self_t& instance()
	{
		if (!_instance) {
			static self_t new_instance;
			_instance = &new_instance;
		}
		
		return *_instance;
	}

	/**     
	 * \brief ȡ�ô�����
	 *
	 * Ѱ���ض���Ʒ��־�Ĵ�����
	 *      
	 * \param id: ��Ʒ��ʶ
	 * \return �ҵĲ�Ʒ������,û�ҵ�����NULL
	 */     	
	C* get_creator(const I& id) const
	{		
		const_list_iterator it = _list.find(id);
		if ( it!= _list.end() ) {
			return it->second;
		}
		
		return NULL;		
	}
	
	/**     
	 * \brief ע�᷽��
	 *
	 * ע��ĳ���Ʒ�Ĵ�����
	 *      
	 * \param id: ��Ʒ��ʶ
	 * \param creator: ��Ʒ������
	 * \return ��
	 */     	
	void register_creator(const I& id, C* creator)	
	{
		_list[id] = creator;
	}
private:
	/**     
	 * \brief ���캯��
	 *
	 */     
	ProductFactory()
	{ }
	
	/**     
	 * \brief ��������
	 *
	 */     
	~ProductFactory()
	{ }
	
	static self_t* _instance;
	
	typedef std::map<I, C*> LIST;
	typedef typename LIST::iterator list_iterator;
	typedef typename LIST::const_iterator const_list_iterator;
	LIST _list;
};

template<typename I, typename C>
ProductFactory<I, C>* ProductFactory<I, C>::_instance = NULL;

#include "QuestParse.h"
#include "QuestCondition.h"
#include "QuestAction.h"
#include "zXMLParser.h"

namespace MakeFunction {
	/**
	 * \brief ��Ʒ��������
	 *
	 *�����˴�����Ʒ�Ľӿ�
	 *
	 */
	template <typename A>
	class Maker
	{
	public:	
		typedef typename A::Base Base;

		/**     
		 * \brief  ��Ʒ�����ӿ�
		 *
		 * �����˲�Ʒ���� �Ľӿ�,�̳�����Ҫʵ�ִ˺����ṩ�����Ʒ�Ĵ�������.
		 *      
		 * \param xml: �ű��ļ�
		 * \param node: �ڵ�����
		 * \return �����Ĳ�Ʒ
		 */     	
		virtual Base* make ( zXMLParser& xml, xmlNodePtr& node) = 0;

		/**     
		 * \brief ��������
		 *
		 */     
		virtual ~Maker() { }
	};
	
	/**
	 * \brief �ڵ����ݴ�����
	 *
	 *��װ����Ҫ���ʽڵ����ݵĽű�����������������Ĵ���
	 *
	 */	
	template <typename A>
	class Content : public Maker<typename A::Base>
	{
	public:	
		typedef typename A::Base Base;
		
		/**     
		 * \brief ��������
		 *
		 */     
		virtual ~Content() { }
		
		/**     
		 * \brief  ��Ʒ�����ӿ�
		 *
		 *ʵ�ֶԷ��ʽڵ����ݵĽű�����������������Ĵ���
		 *      
		 * \param xml: �ű��ļ�
		 * \param node: �ڵ�����
		 * \return �����Ĳ�Ʒ
		 */     	
		Base* make (zXMLParser& xml, xmlNodePtr& node)
		{
			std::string content;
			if (!xml.getNodeContentStr(node, content)) {
				return NULL;
			}
			
			return new A(content);			
		}
	};
	
	/**
	 * \brief �����Դ�����
	 *
	 *��װ�˲���Ҫ���ʽڵ����ԵĽű�����������������Ĵ���
	 *
	 */	
	template <typename A>
	class Void : public Maker<Action>
	{
	public:	
		typedef Maker<Action>::Base Base;
		
		/**     
		 * \brief  ���캯��
		 *
		 * ��ʼ����ر���
		 *      
		 */     	
		Void()
		{ }
		
		/**     
		 * \brief ��������
		 *
		 */     
		virtual ~Void() { }
		
		/**     
		 * \brief  ��Ʒ�����ӿ�
		 *
		 *      
		 * \param xml: �ű��ļ�
		 * \param node: �ڵ�����
		 * \return �����Ĳ�Ʒ
		 */     	
		Base* make (zXMLParser& xml, xmlNodePtr& node)
		{
			return  new A();
		}
	};

	/**
	 * \brief �ڵ����Դ�����
	 *
	 *��װ����Ҫ���ʽڵ����ԵĽű�����������������Ĵ���
	 *
	 */	
	template <typename A>
	class Prop : public Maker<typename A::Base>
	{
	public:	
		typedef typename A::Base Base;
		
		/**     
		 * \brief  ���캯��
		 *
		 * ��ʼ����ر���
		 *      
		 * \param p: �ؼ����б�
		 */     	
		Prop(Parse& p) : _p(p)
		{ }
		
		/**     
		 * \brief ��������
		 *
		 */     
		virtual ~Prop() { }
		
		/**     
		 * \brief  ��Ʒ�����ӿ�
		 *
		 *ʵ�ֶԷ��ʽڵ����ԵĽű�����������������Ĵ���
		 *      
		 * \param xml: �ű��ļ�
		 * \param node: �ڵ�����
		 * \return �����Ĳ�Ʒ
		 */     	
		Base* make (zXMLParser& xml, xmlNodePtr& node)
		{
			if (! _p.parse(xml, node) ) return NULL;

			return new A(_p);			
		}
	private:
	
		Parse _p;
	};

	/**
	 * \brief ��������������
	 *
	 *��װ����Ҫ���������͸��µĽű�����������Ĵ���
	 *
	 */	
	template < template <typename> class A, typename T = int >
	class Update : public Maker<Action>
	{
	public:	
		typedef Maker<Action>::Base Base;
		
		/**     
		 * \brief  ���캯��
		 *
		 * ��ʼ����ر���
		 *      
		 * \param p: �ؼ����б�
		 */     	
		Update(Parse& p) : _p(p)
		{
			
		}
		
		/**     
		 * \brief ��������
		 *
		 */     
		virtual ~Update() { }
		
		/**     
		 * \brief  ��Ʒ�����ӿ�
		 *
		 *ʵ�ֶ԰��������͸��µĽű�����������Ĵ���
		 *      
		 * \param xml: �ű��ļ�
		 * \param node: �ڵ�����
		 * \return �����Ĳ�Ʒ
		 */     	
		Base* make (zXMLParser& xml, xmlNodePtr& node)
		{
			if (! _p.parse(xml, node) ) return NULL;

			std::string type;
			_p.value("type", type);

			if (!type.compare("Set")) {
				return new A< Op::Set<T> >(_p);	
			}else if (!type.compare("Add")) {
				return new A< Op::Add<T> >(_p);											
			}else if (!type.compare("Sub")) {
				return new A< Op::Sub<T> >(_p);									
			}else if (!type.compare("Mul")) {
				return new A< Op::Mul<T> >(_p);									
			}else if (!type.compare("Pow")) {
				return new A< Op::Pow<T> >(_p);									
			}else if (!type.compare("GetP")) {
				return new A< Op::GetP<T> >(_p);									
			}else if (!type.compare("Div")) {
				return new A< Op::Div<T> >(_p);									
			}

			return NULL;
		}
		
		private:
			Parse _p;
	};

	/**
	 * \brief ��������������
	 *
	 *��װ����Ҫ���������ͱȽϵĽű�����������Ĵ���
	 *
	 */	
	template <template <typename> class A, typename T = int >
	class Compare : public Maker<Condition>
	{
	public:	
		typedef Maker<Condition>::Base Base;
		
		/**     
		 * \brief  ���캯��
		 *
		 * ��ʼ����ر���
		 *      
		 * \param p: �ؼ����б�
		 */     	
		Compare(Parse& p) : _p(p)
		{ 
			
		}
		
		/**     
		 * \brief ��������
		 *
		 */     
		virtual ~Compare() { }
		
		/**     
		 * \brief  ��Ʒ�����ӿ�
		 *
		 *ʵ�ֶ԰��������ͱȽϵĽű�����������Ĵ���
		 *      
		 * \param xml: �ű��ļ�
		 * \param node: �ڵ�����
		 * \return �����Ĳ�Ʒ
		 */ 
		Base* make (zXMLParser& xml, xmlNodePtr& node)
		{
			if (! _p.parse(xml, node) ) return NULL;
			
			std::string type;
			_p.value("type", type);
			
			if (!type.compare("Equal")) {
				return new A< Op::Equal<T> >(_p);
			}else if (!type.compare("Great")) {
				return new A< Op::Great<T> >(_p);											
			}else if (!type.compare("Less")) {
				return new A< Op::Less<T> >(_p);											
			}else if (!type.compare("Differ")) {
				return new A< Op::Differ<T> >(_p);											
			}
			
			return NULL;
		}
		
		private:
			Parse _p;
	};

}
	
typedef ProductFactory<std::string, MakeFunction::Maker<Action> > ActionFactory;
typedef ProductFactory<std::string, MakeFunction::Maker<Condition> > ConditionFactory;

#endif
