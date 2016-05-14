/**
 * \file
 * \version  $Id: zUserManager.h  $
 * \author  
 * \date 
 * \brief ��ɫ����������
 */

#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include "zUser.h"
#include "zEntryManager.h"

/**
 * \brief ��ɫ������
 *
 * ʵ����ID����ʱID�����ֵ�����,������Щֵ�����ظ�
 */
class zUserManager:public zEntryManager< zEntryID, zEntryTempID, zEntryName>
{
	protected:
		/**
		 * \brief ���������ʻ�����
		 */
		zRWLock rwlock;

	public:
		/**
		 * \brief ���캯��
		 */
		zUserManager()
		{
		}

		/**
		 * \brief ��������
		 */
		virtual ~zUserManager()
		{
			clear();
		}

		/**
		 * \brief ���ݽ�ɫ���ֵõ���ɫ
		 * \param name ��ɫ����
		 * \return ��ɫָ��,�������NULL��ʾû�ҵ���ɫ
		 */
		zUser * getUserByName( const char * name)
		{
			rwlock.rdlock();
			zUser *ret =(zUser *)getEntryByName(name);
			rwlock.unlock();
			return ret;
		}

		/**
		 * \brief ���ݽ�ɫID�õ���ɫ
		 * \param id ��ɫID
		 * \return ��ɫָ��,�������NULL��ʾû�ҵ���ɫ
		 */
		zUser * getUserByID( DWORD id)
		{
			rwlock.rdlock();
			zUser *ret =(zUser *)getEntryByID(id);
			rwlock.unlock();
			return ret;
		}

		/**
		 * \brief ���ݽ�ɫ��ʱID�õ���ɫ
		 * \param tempid ��ɫ��ʱID
		 * \return ��ɫָ��,�������NULL��ʾû�ҵ���ɫ
		 */
		zUser * getUserByTempID( DWORD tempid)
		{
			rwlock.rdlock();
			zUser *ret =(zUser *)getEntryByTempID(tempid);
			rwlock.unlock();
			return ret;
		}

		/**
		 * \brief ��ӽ�ɫ
		 * \param user ��ɫ
		 * \return ����Ƿ�ɹ�
		 */
		bool addUser(zSceneEntry *user)
		{
			rwlock.wrlock();
//			Zebra::logger->debug("%s(%x) really insert into user manager", user->name, user);			
			bool ret =addEntry((zEntry *)user);
			rwlock.unlock();
			return ret;
		}

		/**
		 * \brief �Ƴ���ɫ
		 * \param user ��ɫ
		 */
		void removeUser(zSceneEntry *user)
		{
			rwlock.wrlock();
//			Zebra::logger->debug("%s(%x) really removed from user manager", user->name, user);
			removeEntry((zEntry *)user);
			rwlock.unlock();
		}

		/**
		 * \brief �Ƴ����������Ľ�ɫ
		 * \param pred ��������
		 */
		template <class YourUserEntry>
		void removeUser_if(removeEntry_Pred<YourUserEntry> &pred)
		{
			rwlock.wrlock();
			removeEntry_if<>(pred);
			rwlock.unlock();
		}

		/**
		 * \brief ��ÿ���û�ִ��
		 * \param exec ִ�нӿ�
		 */
		template <class YourUserEntry>
		bool execEveryUser(execEntry<YourUserEntry> &exec)
		{
			rwlock.rdlock();
			bool ret=execEveryEntry<>(exec);
			rwlock.unlock();
			return ret;
		}
};
#endif
