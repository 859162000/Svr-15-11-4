/**
 * \file
 * \version  $Id: zSceneManager.h $
 * \author  
 * \date 
 * \brief ��������������
 */

#ifndef _ZSCENEMANAGER_H_
#define _ZSCENEMANAGER_H_

#include "zScene.h"
#include "zEntryManager.h"

/**
 * \brief ����������
 *
 * �����ֺ���ʱID����,û��ID��������Ϊ���������ظ�
 */
class zSceneManager:public zEntryManager<zEntryID, zEntryTempID, zEntryName>
{
	protected:
		/**
		 * \brief ���ʹ������Ļ�����
		 */
		zRWLock rwlock;

		zScene * getSceneByName( const char * name)
		{
			rwlock.rdlock();
			zScene *ret =(zScene *)getEntryByName(name);
			rwlock.unlock();
			return ret;
		}

		zScene * getSceneByID(DWORD id)
		{
			rwlock.rdlock();
			zScene *ret =(zScene *)getEntryByID(id);
			rwlock.unlock();
			return ret;
		}
		
		zScene * getSceneByTempID( DWORD tempid)
		{
			rwlock.rdlock();
			zScene *ret =(zScene *)getEntryByTempID(tempid);
			rwlock.unlock();
			return ret;
		}

		template <class YourSceneEntry>
		bool execEveryScene(execEntry<YourSceneEntry> &exec)
		{
			rwlock.rdlock();
			bool ret=execEveryEntry<>(exec);
			rwlock.unlock();
			return ret;
		}

		/**
		 * \brief �Ƴ����������Ľ�ɫ
		 * \param pred ��������
		 */
		template <class YourSceneEntry>
		void removeScene_if(removeEntry_Pred<YourSceneEntry> &pred)
		{
			rwlock.wrlock();
			removeEntry_if<>(pred);
			rwlock.unlock();
		}

	public:
		/**
		 * \brief ���캯��
		 */
		zSceneManager()
		{
		}

		/**
		 * \brief ��������
		 */
		virtual ~zSceneManager()
		{
			clear();
		}

};
#endif
