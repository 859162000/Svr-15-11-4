/**
 * \file	zState.h
 * \version  	$Id$
 * \author  	
 * \date 	
 * \brief 	��������״̬����״̬�ӿ�
 *
 * 
 */

#ifndef _STATE_H
#define _STATE_H
#include <vector>
#include <string>
#include <map>

#include "zEventHandler.h"
#include "zEvent.h"
//#include "zStateMachine.h"
template<typename T> class StateMachine;

//template<typename T, typename EventHandler = EventHandler<T> >
template<typename T>
class State
{
	public:

		/**
		 * \brief ״̬���� 
		 *
		 */
		std::string name;

		/**
		 * \brief State���캯�� 
		 *
		 *
		 * \param timeout ��״̬������ֵ,�����
		 * \return 
		 */
		State(int timeout=0)
		{
		}


		/**
		 * \brief ����״̬��ָ�� 
		 *
		 * \param m ӵ�и�״̬��״̬��ָ��
		 */
		void set_machine(StateMachine<T>* m)
		{
			this->machine = m;
		}

		/**
		 * \brief �����¼������� 
		 *
		 *
		 * \param ename �¼�����
		 * \param eh �¼������Wrap��
		 * \return ��ӳɹ�,����TRUE, ����, ����FALSE
		 */
		bool addHandler(const std::string& ename, EventHandler<T> eh)
		{
			std::pair<EventIter, bool> retval = handlers.insert(typename EventMap::value_type(ename, eh));
			return retval.second;
		}

		/**
		 * \brief �����¼� 
		 *
		 *
		 * \param ev �¼�ָ��
		 * \return �¼���������TRUE, û�б�������FALSE
		 */
		bool dispatch_event(Event* ev)
		{
			EventIter et = handlers.find(ev->get_name());	
			if (et!=handlers.end())
			{
				// ִ�е�ǰ״̬�յ����¼�ʱ���¼�������
				EventHandler<T> eh = (EventHandler<T>)et->second;
				eh.action(ev);
				
				// ��ת����һ״̬
				TnsIter ti = transitions.find(ev->get_name());
				machine->set_act(ti->second);
#ifdef _ZJW_DEBUG				
				std::cout << "cur_state:[" << this->name.c_str() << "] " 
					<< "receive event:[" << ev->get_name() 	<< "] "
					<< "nextstate:[" << ti->second << "]" << std::endl;
#endif				
				return true;
			}
			else
			{
				std::cout << "state:dispatch_event:no find event" << std::endl;
			}

			return false;
		}
		

		/**
		 * \brief ���״̬ת������
		 *
		 *
		 * \param evname �¼�����
		 * \param eh  �¼�������
		 * \param nextstatename ��һ״̬����,Ĭ��Ϊover״̬
		 * \return ��ӳɹ�����TRUE, ����ΪFALSE 
		 */
		bool add_transition(const std::string& evname, EventHandler<T> eh, 
				const std::string& nextstatename="over")
		{
			std::pair< TnsIter, bool > pr;

			pr = transitions.insert(typename TnsMap::value_type(evname, nextstatename));

			if (pr.second)
			{
				pr.second = addHandler(evname, eh);
			}

			return pr.second;
		}


		/**
		 * \brief �жϸ�״̬�Ƿ�ʱ 
		 *
		 * \return ��ʱ����TRUE, ����ΪFALSE 
		 */
		bool is_timeout()
		{
			
		}

	protected:
		typedef std::map<std::string, EventHandler<T> > EventMap;
		typedef typename EventMap::iterator EventIter;
		EventMap handlers;

	
		typedef std::map<std::string, std::string> TnsMap;
		typedef typename TnsMap::iterator TnsIter;
		TnsMap transitions;
		StateMachine<T>* machine;
		int timeout;
};

#endif
