/**
 * \file
 * \version  $Id: zSingleton.h  $
 * \author  
 * \date 
 * \brief 
 *
 * 
 */

#ifndef _SINGLETON_H
#define _SINGLETON_H

template <typename T>
class SingletonFactory
{
	public:
		static T* instance()
		{
			return new T();
		}
};

template <typename T, typename MANA = SingletonFactory<T> > 
class Singleton
{
	private:
		/**
		 * \brief �������캯����û��ʵ�֣����õ���
		 *
		 */
		Singleton(const Singleton&);

		/**
		 * \brief ��ֵ�������ţ�û��ʵ�֣����õ���
		 *
		 */
		const Singleton & operator= (const Singleton &);
	protected:

		static T* ms_Singleton;
		Singleton( void )
		{
		}

		~Singleton( void )
		{
		}

	public:
	
		static void delMe(void)
		{//�����������destoryMe�б�����
			if (ms_Singleton)
			{
				delete ms_Singleton;
				ms_Singleton = 0;
			}
		}

		static T* instance( void )
		{
			if (!ms_Singleton)
			{
				ms_Singleton = MANA::instance();
			}
			
			return ms_Singleton;
		}

		static T& getMe(void)
		{
			return *instance();
		}

};

template <typename T, typename MANA>
T* Singleton<T,MANA>::ms_Singleton = 0;

#endif

