/**
 * \file	zStateMachine.h
 * \version  	$Id$
 * \author  	
 * \date 	
 * \brief 	FSM����
 *
 * 
 */

#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H
#include <string>
#include <map>
#include "zType.h"
#include "zState.h"


template<typename T>
class StateMachine
{   
	public:
		StateMachine()
		{
			act_state = NULL;
		}
		
		virtual ~StateMachine()
		{
			StateIter st;

			for(st=states.begin(); st!=states.end(); st++)
			{
				SAFE_DELETE(st->second);
			}
		}

		/**
		 * \brief �����¼�
		 *
		 *
		 * \param ev �¼�ָ�� 
		 */
		virtual void dispatch_event(Event* ev)
		{
			if (act_state)
			{
				std::cout << "machine:dispatch_event" << std::endl;
				act_state->dispatch_event(ev);
			}
		}
		
		/**
		 * \brief ͨ��state_machine.xml��ʼ��״̬��,����,����ʵ�ֵķ���
		 *
		 * \param name ״̬������, ������state_machine.xml���Ҷ�Ӧ������
		 */
		virtual void init(const std::string& name)
		{
			State<T>* over_state  = new State<T>;
			this->add_state("over", over_state);
		}
		
		/**
		 * \brief ����[],ͨ��״̬��,��ȡһ��״̬
		 *
		 *
		 * \param name ״̬�� 
		 * \return ָ��״̬��ָ��, ���ָ��״̬������,����NULL
		 */
		virtual State<T>* operator[](std::string name)
		{
			StateIter st;
			st = states.find(name);
			if ( st == states.end() )
				return 0; // δ�ҵ�,����NULL 
			return st->second; // �ҵ�,���ض�Ӧ��״ָ̬��
		}

		/**
		 * \brief ���õ�ǰ�Ļ�Ծ״̬ 
		 *
		 * \param ״̬��
		 * \return ���óɹ�,����TRUE,����,����FALSE
		 */
		bool set_act(std::string statename = "default")
		{
			State<T> *st = (*this)[statename];
			if(st == 0)
				return false;

			act_state = st;
			return true;
		}
		
		/**
		 * \brief �õ���ǰ�Ļ�Ծ״̬ 
		 *
		 * \return ���ص�ǰ״̬
		 */
		State<T>* get_act()
		{
			return act_state;
		}

		bool is_over()
		{
			if (act_state && act_state->name=="over") return true;
			return false;
		}

	protected:
		
		State<T>* act_state;//��ǰ״̬ 
	
		typedef std::map<std::string, State<T>* > StateMap;
		typedef typename StateMap::iterator StateIter;
			
		/// ״̬��
		StateMap states;

		/**
		 * \brief ���һ����״̬ 
		 *
		 *
		 * \param name ״̬��
		 * \param new_state �µ�״̬
		 * \return 
		 */
		void add_state(const std::string& name, State<T>* new_state)
		{
			//std::pair<StateIter, bool> retval = states.insert(StateMap::value_type(name, new_state));
			new_state->name = name;
			states.insert(typename StateMap::value_type(name, new_state));
			//return retval.second;
		}

		std::string name;
};

#endif
