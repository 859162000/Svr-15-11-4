/**
 * \file
 * \version  $Id: zUniqueID.h  $
 * \author  
 * \date 
 * \brief Ψһ���������ģ�嶨��
 *
 * 
 */

#ifndef _ZUNIQEID_
#define _ZUNIQEID_

#include <pthread.h>
#include <list>
#include <ext/pool_allocator.h>

#include "zMutex.h"
#include "zNoncopyable.h"
/**
 * \brief zUniqueIDģ��
 * ��ģ��ʵ����ΨһID������������֤�̰߳�ȫ��
 * �����ø��ֳ��ȵ��޷���������ΪID��
 */
template <class T>
class zUniqueID:private zNoncopyable
{
	private:
		zMutex mutex;
		std::list<T, __gnu_cxx::__pool_alloc<T> > ids;
		T maxID;
		T minID;
		T curMaxID;
		void init(T min,T max)
		{
			minID=min;
			maxID=max;
			curMaxID=minID;
		}

	public:
		/**
		 * \brief Ĭ�Ϲ��캯�� 
		 * ��ʼIDΪ1�������ЧIDΪ(T)-2,��ЧIDΪ(T)-1
		 */
		zUniqueID()
		{
			init(1,(T)-1);
		}

		/**
		 * \brief ���캯�� 
		 * �û��Զ�����ʼID�������ЧIDΪ(T)-2,��ЧIDΪ(T)-1
		 * \param startID �û��Զ������ʼID
		 */
		zUniqueID(T startID)
		{
			init(startID,(T)-1);
		}

		/**
		 * \brief ���캯�� 
		 * �û��Զ�����ʼID���������ЧID,�����ЧIDΪ�����ЧID-1
		 * \param startID �û��Զ������ʼID
		 * \param endID �û��Զ���������ЧID
		 */
		zUniqueID(T startID,T endID)
		{
			init(startID,endID);
		}

		/**
		 * \brief �������� 
		 * �����ѷ����ID�ڴ档
		 */
		~zUniqueID()
		{
			mutex.lock();
			ids.clear();
			mutex.unlock();
		}

		/**
		 * \brief �õ������ЧID 
		 * \return ���������ЧID
		 */
		T invalid()
		{
			return maxID;
		}

		/**
		 * \brief �������ID�Ƿ񱻷����ȥ
		 * \return �������ȥ����true,��ЧID��δ����ID����false
		 */
		bool hasAssigned(T testid)
		{
			mutex.lock();
			if(testid<maxID && testid>=minID)
			{
				typename std::list<T, __gnu_cxx::__pool_alloc<T> >::iterator iter = ids.begin();
				for(;iter != ids.end() ; iter ++)
				{
					if(*iter == testid)
					{
						mutex.unlock();
						return false;
					}
				}
				/*
				for(int i=0,n=ids.size() ;i<n;i++)
				{
					if(ids[i]==testid)
					{
						mutex.unlock();
						return false;
					}
				}
				// */
				mutex.unlock();
				return true;
			}
			mutex.unlock();
			return false;
		}

		/**
		 * \brief �õ�һ��ΨһID 
		 * \return ����һ��ΨһID��������������ЧID���ȱ�ʾ����ID���ѱ��ã��޿���ID��
		 */
		T get()
		{
			T ret;
			mutex.lock();
				if(maxID>curMaxID)
				{
					ret=curMaxID;
					curMaxID++;
				}
				else
					ret=maxID;
			if(ret == maxID && !ids.empty())
			{
				ret=ids.back();
				ids.pop_back();
			}
			mutex.unlock();
			return ret;
		}

		/**
		 * \brief һ�εõ����ID����ЩID�������ڵ�,���Ҳ��ر��Ż�ȥ 
		 * \param size Ҫ�����ID����
		 * \param count ʵ�ʷ���ID�ĸ���
		 * \return ���ص�һ��ID��������������ЧID���ȱ�ʾ����ID���ѱ��ã��޿���ID��
		 */
		T get(int size,int & count)
		{
			T ret;
			mutex.lock();
			if(maxID>curMaxID)
			{
				count=(maxID-curMaxID)>size?size:(maxID-curMaxID);
				ret=curMaxID;
				curMaxID+=count;
			}
			else
			{
				count=0;
				ret=maxID;
			}
			mutex.unlock();
			return ret;
		}

		/**
		 * \brief ��ID�Ż�ID�أ��Ա��´�ʹ�á� 
		 * 
		 * �Żص�ID��������get�����õ��ġ����Ҳ��ܱ�֤�Żص�ID,û�б������߳�ʹ�á�
		 * �����û�Ҫ�Լ���֤����ʹ�õ�ID���ᱻ�Ż�ȥ���������ID�ظ�����
		 * \param id ��get�õ���ID.
		 */
		void put(T id)
		{
			mutex.lock();
			if(id<maxID && id>=minID)
			{
				bool hasID=false;
				typename std::list<T, __gnu_cxx::__pool_alloc<T> >::iterator iter = ids.begin();
				for(;iter != ids.end() ; iter ++)
				{
					if(*iter == id)
					{
						hasID=true;
						break;
					}
				}
				/*
				for(int i=0,n=ids.size() ;i<n;i++)
				{
					if(ids[i]==id)
					{
						hasID=true;
						break;
					}
				}
				// */
				if(!hasID) ids.push_front(id);
				//if(!hasID) ids.insert(ids.begin() , id);
				//if(!hasID) ids.push_back(id);
			}
			mutex.unlock();
		}
};

typedef zUniqueID<DWORD> zUniqueDWORDID;
#endif
