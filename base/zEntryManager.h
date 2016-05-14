/**
 * \file
 * \version  $Id: zEntryManager.h  $
 * \author  
 * \date 
 * \brief entry�����������ļ�
 */

#ifndef _ZENTRYMANAGER_H_
#define _ZENTRYMANAGER_H_
#include "zMisc.h"
#include "zEntry.h"
#include <ext/hash_map>
#include "Zebra.h"

/**
 * \brief keyֵ��ֵ�Ƚ�,Ŀǰ֧�� (DWORD , char *)����������
 */
template <class keyT>
struct my_key_equal : public std::binary_function<keyT, keyT, bool>
{
	inline bool operator()(const keyT s1, const keyT s2) const;
};

/**
 * \brief ģ��ƫ�ػ�
 * ���ַ������бȽ�
 */
template<>
inline bool my_key_equal<const char *>::operator()(const char * s1, const char * s2) const
{
	return strcmp(s1, s2) == 0;
}

/**
 * \brief ģ��ƫ�ػ�
 * ���������бȽ�
 */
template<>
inline bool my_key_equal<DWORD>::operator()(const DWORD s1, const DWORD s2) const
{
	return s1  == s2;
}

/**
 * \brief ����ͰHash����ģ��,���̰߳�ȫ
 *
 * Ŀǰ֧������key����(DWORD , char *),value���Ͳ�������,��������Ҫ��copy�ġ�
 * \param keyT key����(DWORD , char *)
 * \param valueT value����
 */
template <class keyT,class valueT>
class LimitHash:private zNoncopyable
{
	protected:

		/**
		 * \brief hash_map����
		 */
		typedef __gnu_cxx::hash_map<keyT, valueT, __gnu_cxx::hash<keyT>, my_key_equal<keyT> > hashmap;
		typedef typename hashmap::iterator iter;
		typedef typename hashmap::const_iterator const_iter;
		hashmap ets;

		/**
		 * \brief �������ݣ����ԭ��������ͬkeyֵ�����ݣ�ԭ�����ݽ��ᱻ�滻
		 * \param key keyֵ
		 * \param value Ҫ���������
		 * \return �ɹ�����true�����򷵻�false
		 */
		inline bool insert(const keyT &key,valueT &value)
		{
			ets[key]=value;
			return true;
		}

		/**
		 * \brief ����keyֵ���Ҳ��õ�����
		 * \param key ҪѰ�ҵ�keyֵ
		 * \param value ���ؽ��������˴�,δ�ҵ�������ı��ֵ
		 * \return ���ҵ�����true��δ�ҵ�����false
		 */
		inline bool find(const keyT &key,valueT &value) const
		{
			const_iter it = ets.find(key);
			if(it != ets.end())
			{
				value = it->second;
				return true;
			}
			else
				return false;
		}

		/**
		 * \brief ���Ҳ��õ�һ������
		 * \param value ���ؽ��������˴�,δ�ҵ�������ı��ֵ
		 * \return ���ҵ�����true��δ�ҵ�����false
		 */
		inline bool findOne(valueT &value) const
		{
			if(!ets.empty())
			{
				value=ets.begin()->second;
				return true;
			}
			return false;
		}

		/**
		 * \brief ���캯��
		 *
		 */
		LimitHash()
		{
		}

		/**
		 * \brief ��������,�����������
		 */
		~LimitHash()
		{
			clear();
		}

		/**
		 * \brief �Ƴ�����
		 * \param key Ҫ�Ƴ���keyֵ
		 */
		inline void remove(const keyT &key)
		{
			ets.erase(key);
		}

		/**
		 * \brief �����������
		 */
		inline void clear()
		{
			ets.clear();
		}

		/**
		 * \brief ͳ�����ݸ���
		 */
		inline unsigned int size() const
		{
			return ets.size();
		}

		/**
		 * \brief �ж������Ƿ�Ϊ��
		 */
		inline bool empty() const
		{
			return ets.empty();
		}
};

/**
 * \brief ����ͰMultiHash����ģ��,���̰߳�ȫ
 *
 * Ŀǰ֧������key����(DWORD , char *),value���Ͳ�������,��������Ҫ��copy�ġ�
 * \param keyT key����(DWORD , char *)
 * \param valueT value����
 */
template <class keyT,class valueT>
class MultiHash:private zNoncopyable
{
	protected:

		/**
		 * \brief hash_multimap����
		 */
		typedef __gnu_cxx::hash_multimap<keyT, valueT, __gnu_cxx::hash<keyT>, my_key_equal<keyT> > hashmap;
		typedef typename hashmap::iterator iter;
		typedef typename hashmap::const_iterator const_iter;
		hashmap ets;

		/**
		 * \brief �������ݣ����ԭ��������ͬkeyֵ�����ݣ�ԭ�����ݽ��ᱻ�滻
		 * \param key keyֵ
		 * \param value Ҫ���������
		 * \return �ɹ�����true�����򷵻�false
		 */
		inline bool insert(const keyT &key,valueT &value)
		{
			ets.insert(std::pair<keyT, valueT>(key, value));
			return true;
		}

		/**
		 * \brief ���캯��
		 *
		 */
		MultiHash()
		{
		}

		/**
		 * \brief ��������,�����������
		 */
		~MultiHash()
		{
			clear();
		}

		/**
		 * \brief �����������
		 */
		inline void clear()
		{
			ets.clear();
		}

		/**
		 * \brief ͳ�����ݸ���
		 */
		inline unsigned int size() const
		{
			return ets.size();
		}

		/**
		 * \brief �ж������Ƿ�Ϊ��
		 */
		inline bool empty() const
		{
			return ets.empty();
		}
};

/**
 * \brief Entry����ʱIDΪkeyֵ��ָ����������Ҫ�̳�ʹ��
 */
class zEntryTempID:public LimitHash<DWORD,zEntry *>
{
	protected:

		zEntryTempID() {}
		virtual ~zEntryTempID() {}

		/**
		 * \brief ��Entry����������,tempid�ظ����ʧ��
		 * \param e Ҫ�����Entry
		 * \return �ɹ�����true,���򷵻�false
		 */
		inline bool push(zEntry * e)
		{
			if(e!=NULL && getUniqeID(e->tempid))
			{
				zEntry *temp;
				if(!find(e->tempid,temp))
				{
					if(insert(e->tempid,e))
						return true;
				}
				putUniqeID(e->tempid);
			}
			return false;
		}

		/**
		 * \brief �Ƴ�Entry
		 * \param e Ҫ�Ƴ���Entry
		 */
		inline void remove(zEntry * e)
		{
			if(e!=NULL)
			{
				putUniqeID(e->tempid);
				LimitHash<DWORD,zEntry *>::remove(e->tempid);
			}
		}

		/**
		 * \brief ͨ����ʱID�õ�Entry
		 * \param tempid Ҫ�õ�Entry����ʱID
		 * \return ����Entryָ��,δ�ҵ�����NULL
		 */
		inline zEntry * getEntryByTempID(const DWORD tempid) const
		{
			zEntry *ret=NULL;
			LimitHash<DWORD,zEntry *>::find(tempid,ret);
			return ret;
		}

		/**
		 * \brief �õ�һ����ʱID
		 * \param tempid ���Ҫ�õ�����ʱID
		 * \return �õ�����true,���򷵻�false
		 */
		virtual bool getUniqeID(DWORD &tempid) =0;
		/**
		 * \brief �Ż�һ����ʱID
		 * \param tempid Ҫ�Żص���ʱID
		 */
		virtual void putUniqeID(const DWORD &tempid) =0;
};

/**
 * \brief Entry��IDΪkeyֵ��ָ����������Ҫ�̳�ʹ��
 */
class zEntryID:public LimitHash<DWORD,zEntry *>
{
	protected:
		/**
		 * \brief ��Entry����������
		 * \param e Ҫ�����Entry
		 * \return �ɹ�����true,���򷵻�false
		 */
		inline bool push(zEntry * &e)
		{
			zEntry *temp;
			if(!find(e->id,temp))
				return insert(e->id,e);
			else
				return false;
		}

		/**
		 * \brief �Ƴ�Entry
		 * \param e Ҫ�Ƴ���Entry
		 */
		inline void remove(zEntry * e)
		{
			if(e!=NULL)
			{
				LimitHash<DWORD,zEntry *>::remove(e->id);
			}
		}

		/**
		 * \brief ͨ��ID�õ�Entry
		 * \param id Ҫ�õ�Entry��ID
		 * \return ����Entryָ��,δ�ҵ�����NULL
		 */
		inline zEntry * getEntryByID(const DWORD id) const
		{
			zEntry *ret=NULL;
			LimitHash<DWORD,zEntry *>::find(id,ret);
			return ret;
		}
};

/**
 * \brief Entry������Ϊkeyֵ��ָ����������Ҫ�̳�ʹ��
 */
class zEntryName:public LimitHash<const char *,zEntry *>
{
	protected:
		/**
		 * \brief ��Entry����������,�������������ͬkeyֵ�����ʧ��
		 * \param e Ҫ�����Entry
		 * \return �ɹ�����true,���򷵻�false
		 */
		inline bool push(zEntry * &e)
		{
			zEntry *temp;
			if(!find(e->name,temp))
				return insert(e->name,e);
			else
				return false;
		}

		/**
		 * \brief �Ƴ�Entry
		 * \param e Ҫ�Ƴ���Entry
		 */
		inline void remove(zEntry * e)
		{
			if(e!=NULL)
			{
				LimitHash<const char *,zEntry *>::remove(e->name);
			}
		}
		
		/**
		 * \brief ͨ�����ֵõ�Entry
		 * \param name Ҫ�õ�Entry������
		 * \return ����Entryָ��,δ�ҵ�����NULL
		 */
		inline zEntry * getEntryByName( const char * name) const
		{
			zEntry *ret=NULL;
			LimitHash<const char *,zEntry *>::find(name,ret);
			return ret;
		}

		/**
		 * \brief ͨ�����ֵõ�Entry
		 * \param name Ҫ�õ�Entry������
		 * \return ����Entryָ��,δ�ҵ�����NULL
		 */
		inline zEntry * getEntryByName(const std::string  &name) const
		{
			return getEntryByName(name.c_str());
		}
};

/**
 * \brief Entry������Ϊkeyֵ��ָ����������Ҫ�̳�ʹ��
 */
class zMultiEntryName:public MultiHash<const char *,zEntry *>
{
	protected:
		/**
		 * \brief ��Entry����������,�������������ͬkeyֵ�����ʧ��
		 * \param e Ҫ�����Entry
		 * \return �ɹ�����true,���򷵻�false
		 */
		inline bool push(zEntry * &e)
		{
			return insert(e->name,e);
		}

		/**
		 * \brief ��Entry���������Ƴ�
		 * \param e ��Ҫ�Ƴ���Entry
		 */
		inline void remove(zEntry * &e)
		{
			__gnu_cxx::pair<iter,iter> its = ets.equal_range(e->name);
			for(iter it = its.first; it != its.second; it++)
			{
				if (it->second == e)
				{
					ets.erase(it);
					return;
				}
			}
		}

		/**
		 * \brief ����keyֵ���Ҳ��õ�����
		 * \param name ҪѰ�ҵ�nameֵ
		 * \param e ���ؽ��������˴�,δ�ҵ�������ı��ֵ
		 * \param r ����ж���ƥ�䣬�Ƿ����ѡ��
		 * \return ���ҵ�����true��δ�ҵ�����false
		 */
		inline bool find(const char * &name,zEntry * &e,const bool r=false) const
		{
			int rd = ets.count(name);
			if(rd > 0)
			{
				int mrd = 0, j = 0;
				if (r)
					zMisc::randBetween(0, rd - 1);
				__gnu_cxx::pair<const_iter,const_iter> its = ets.equal_range(name);
				for(const_iter it = its.first; it != its.second && j < rd; it++, j++)
				{
					if (mrd == j)
					{
						e = it->second;
						return true;
					}
				}
			}
			return false;
		}

};

template<int i>
class zEntryNone
{
	protected:
		inline bool push(zEntry * &e) { return true; }
		inline void remove(zEntry * &e) { }
		inline void clear(){}
};

/**
 * \brief Entry����ӿ�,��<code>zEntryManager::execEveryEntry</code>ʹ��
 */
template <class YourEntry>
struct execEntry
{
	virtual bool exec(YourEntry *entry) =0;
	virtual ~execEntry(){}
};

/**
 * \brief Entryɾ�������ӿ�,��<code>zEntryManager::removeEntry_if</code>ʹ��
 */
template <class YourEntry>
struct removeEntry_Pred
{
	/**
	 * \brief ��ɾ����entry�洢������
	 */
	std::vector<YourEntry *> removed;
	/**
	 * \brief �����Ƿ�Ҫɾ����entry,��Ҫʵ��
	 * \param Ҫ�����Ե�entry
	 */
	virtual bool isIt(YourEntry *entry) =0;
	/**
	 * \brief ��������
	 */
	virtual ~removeEntry_Pred(){}
};

/**
 * \brief Entry�������ӿ�,�û�Ӧ�ø��ݲ�ͬʹ������̳���
 */

template<typename e1,typename e2=zEntryNone<1>, typename e3=zEntryNone<2> >
class zEntryManager:protected e1,protected e2,protected e3
{
	protected:

		/**
		 * \brief ���Entry,�����ظ�������Entry���ʧ��
		 * \param e ����ӵ� Entryָ��
		 * \return �ɹ�����true�����򷵻�false 
		 */
		inline bool addEntry(zEntry * e)
		{
			if(e1::push(e))
			{
				if(e2::push(e))
				{
					if(e3::push(e))
						return true;
					else
					{
						e2::remove(e);
						e1::remove(e);
					}
				}
				else
					e1::remove(e);
			}
			return false;
		}

		/**
		 * \brief ɾ��Entry
		 * \param e ��ɾ����Entryָ��
		 */
		inline void removeEntry(zEntry * e)
		{
			e1::remove(e);
			e2::remove(e);
			e3::remove(e);
		}

		/**
		 * \brief ����������
		 */
		~zEntryManager() { };

		/**
		 * \brief ͳ�ƹ�������Entry�ĸ���
		 * \return ����Entry����
		 */
		inline int size() const
		{
			return e1::size();
		}

		/**
		 * \brief �ж������Ƿ�Ϊ��
		 */
		inline bool empty() const
		{
			return e1::empty();
		}

		/**
		 * \brief �������Entry
		 */
		inline void clear()
		{
			e1::clear();
			e2::clear();
			e3::clear();
		}

		/**
		 * \brief ��ÿ��Entry���д���
		 * ������ĳ��Entry����falseʱ������ϴ�����
		 * \param eee ����ӿ�
		 * \return ���ȫ��ִ����Ϸ���true,���򷵻�false
		 */
		template <class YourEntry>
		inline bool execEveryEntry(execEntry<YourEntry> &eee)
		{
			typedef typename e1::iter my_iter;
			for(my_iter it=e1::ets.begin();it!=e1::ets.end();it++)
			{
				if(!eee.exec((YourEntry *)it->second))
					return false;
			}
			return true;
		}

		/**
		 * \brief ɾ������������Entry
		 * \param pred ���������ӿ�
		 */
		template <class YourEntry>
		inline void removeEntry_if(removeEntry_Pred<YourEntry> &pred)
		{
			typedef typename e1::iter my_iter;
			my_iter it=e1::ets.begin();
			while(it!=e1::ets.end())
			{
				if(pred.isIt((YourEntry *)it->second))
				{
					pred.removed.push_back((YourEntry *)it->second);
				}
				it++;
			}

			for(unsigned int i=0;i<pred.removed.size();i++)
			{
				removeEntry(pred.removed[i]);
			}
		}
};
#endif
