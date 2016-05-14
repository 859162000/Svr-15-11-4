/**
 * \file
 * \version  $Id: SceneNpcManager.h  $
 * \author  
 * \date 
 * \brief Npc��������
 *
 * 
 */

#ifndef _SceneNpcManager_h_
#define _SceneNpcManager_h_

#include "SceneNpc.h"
#include "Chat.h"
#include "TimeTick.h"
#include "zEntryManager.h"

/**
 * \brief ��ÿ������npcִ�еĻص�
 *
 */
struct specialNpcCallBack
{
	public:
		virtual bool exec(SceneNpc *npc)=0;
		virtual ~specialNpcCallBack(){};
};

/**
 * \brief npc������
 *
 */
class SceneNpcManager : public zEntryManager< zEntryTempID >
{
	public: 

		bool init();
		bool addSceneNpc(SceneNpc *sceneNpc);
		bool addSpecialNpc(SceneNpc *, bool=false);
		void removeSceneNpc(SceneNpc *sceneNpc);

		SceneNpc *getNpcByTempID(DWORD tempid);
		static SceneNpcManager &getMe();
		static void destroyMe();
		/**
		 * \brief ��ÿ��npcִ�лص�����
		 *
		 *
		 * \param exec �ص�����
		 * \return �Ƿ����ִ��
		 */
		template <class YourNpcEntry>
		bool execEveryNpc(execEntry<YourNpcEntry> &exec)
		{
			rwlock.rdlock();
			bool ret=execEveryEntry<>(exec);
			rwlock.unlock();
			return ret;
		}

		/**
		 * \brief ɾ������������npc
		 *
		 * \param pred �ж�����
		 */
		template <class YourNpcEntry>
		void removeNpc_if(removeEntry_Pred<YourNpcEntry> &pred)
		{
			rwlock.wrlock();
			removeEntry_if<>(pred);
			rwlock.unlock();
		}

		/**
		 * \brief ɾ��һ�����ڵ�npc
		 *
		 *
		 * \param scene ����
		 */
		void removeNpcInOneScene(Scene *scene);
		void SpecialAI();
		/**
		 * \brief ��ÿ������npcִ�лص�������
		 *
		 *
		 * \param callback �ص�����
		 * \return 
		 */
		void execAllSpecialNpc(specialNpcCallBack &callback)
		{
			rwlock.wrlock();
			for (MonkeyNpcs::iterator it=specialNpc.begin(); it!=specialNpc.end(); it++)
			{
				if (!callback.exec(*it))
				{
					rwlock.unlock();
					return;
				}
			}
			rwlock.unlock();
		}

		MonkeyNpcs &getSepcialNpc()
		{
			return specialNpc;
		}
		bool getNpcCommonChat(DWORD type, char * content);
		void removeSpecialNpc(SceneNpc *);
		void closeFunctionNpc();
	private:
		///specialNpc��ָ��ʹû������ڸ���ҲҪ�������NPC
		///�������boss������NPC
		MonkeyNpcs specialNpc;

		SceneNpcManager();
		~SceneNpcManager();

		///SceneNpcManager��Ψһʵ��
		static SceneNpcManager *snm;
		///��д��
		zRWLock rwlock;

		bool getUniqeID(DWORD &tempid) { return true; }       
		void putUniqeID(const DWORD &tempid) {}

		///npc���˵��������
		std::map<DWORD, std::vector<std::string> > NpcCommonChatTable;
		///npc���˵���ĸ���
		std::map<DWORD, int> NpcCommonChatRate;

		bool loadNpcCommonChatTable();
};

#endif

