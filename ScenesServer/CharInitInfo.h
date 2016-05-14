/**
 * \file
 * \version  $Id: CharInitInfo.h  $
 * \author 
 * \date
 * \brief ��ʼ��ɫ��Ϣ
 *
 * 
 */


#ifndef _CharInitInfo_h_
#define _CharInitInfo_h_

#include <vector>
#include <ext/hash_map>
#include "zType.h"
#include "zRWLock.h"

/**
 * \brief ����װ���ṹ
 *
 */
struct InitObject
{
	///
	DWORD id;
	char name[MAX_NAMESIZE+1];
	DWORD localeID;
	WORD  num;
	WORD  profession;

	InitObject()
	{
		id = 0;
		bzero(name, sizeof(name));
		localeID = 0;
		num = 0;
		profession = 0;
	}

	/**
	 * \brief ���캯��
	 *
	 *
	 * \param object: ����װ��
	 */
	InitObject(const InitObject &object)
	{
		id = object.id;
		bcopy(object.name, name, sizeof(name));
		localeID = object.localeID;
		num = object.num;
		profession = object.profession;
	}

};

typedef std::vector<InitObject> InitObjectVector;

/**
 * \brief ������Ϣ
 *
 */
class CharInitInfo
{

	public:

		~CharInitInfo()
		{
			final();
		}

		/**
		 * \brief �õ�Ψһʵ��
		 *
		 *
		 * \return Ψһʵ��
		 */
		static CharInitInfo &getInstance()
		{
			if (NULL == instance)
				instance = new CharInitInfo();

			return *instance;
		}

		/**
		 * \brief ɾ��Ψһʵ��
		 *
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		bool init();
		void get(const WORD profession, InitObjectVector &objs);
		void final();

	private:

		static CharInitInfo *instance;

		CharInitInfo() {};

		typedef __gnu_cxx::hash_multimap<WORD, InitObject> ObjectsContainer;
		zRWLock rwlock;
		ObjectsContainer objects;

};

#endif

