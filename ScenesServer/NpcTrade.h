/**
 * \file
 * \version  $Id: NpcTrade.h $
 * \author  
 * \date 
 * \brief Npc�����Ի���
 *
 * 
 */


#ifndef _NpcTrade_h_
#define _NpcTrade_h_

#include "zType.h"
#include "zRWLock.h"
#include "zDatabase.h"

/**
 * \brief NPC����
 *
 */
class NpcTrade
{

	public:

		enum
		{
			NPC_BUY_OBJECT		= 1,	///��
			NPC_SELL_OBJECT		= 2,	///��
			NPC_REPAIR_OBJECT	= 4,	///����
			NPC_MAKE_OBJECT		= 8,	///����
			NPC_UPDATE_OBJECT	= 16,	///����
			NPC_MERGE_OBJECT	= 32, 	///�ϳ�
			NPC_ENCHANCE_OBJECT = 64,  //��Ƕ
			NPC_MERGE_SOUL_OBJECT = 128,  //���Ǻϳ�
			NPC_HOLE_OBJECT = 256,  //���
			NPC_STORE_OBJECT = 512,  //�ֿ�
			NPC_DECOMPOSE_OBJECT = 1024,  //�ֽ�
		};

		struct NpcItem
		{
			DWORD id;					///��Ʒ���
			WORD  kind;					///��Ʒ����
			WORD  lowLevel;				///��͵ȼ�
			WORD  level;				///��ߵȼ�
			WORD  itemlevel;			///������Ʒ�ĵȼ�
			WORD  action;				///��������
			NpcItem()
			{
				id = 0;
				kind = 0;
				lowLevel = 0;
				level = 0;
				itemlevel = 0;
				action = 0;
			}
		};

		~NpcTrade()
		{
			final();
		}

		/**
		 * \brief �õ�Ψһʵ��
		 *
		 *
		 * \return npc����ϵͳ
		 */
		static NpcTrade &getInstance()
		{
			if (NULL == instance)
				instance = new NpcTrade();

			return *instance;
		}

		/**
		 * \brief ж��Ψһʵ��
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		bool init();
		bool getNpcMenu(const DWORD npcid, char *menuTxt);
		bool verifyNpcAction(const DWORD npcid, const NpcItem &item);

	private:

		NpcTrade() {};

		void final();

		static NpcTrade *instance;

		typedef __gnu_cxx::hash_multimap<DWORD, NpcItem> NpcItemMultiMap;

		/**
		 * \brief npc�Ի���
		 *
		 */
		struct NpcDialog
		{
			DWORD npcid;			///Npc���
			char menu[6144];		///�˵�����
			NpcItemMultiMap items;	///��Ʒ����
			NpcDialog()
			{
				npcid = 0;
				bzero(menu, sizeof(menu));
			}
			NpcDialog(const NpcDialog& nd)
			{
				npcid = nd.npcid;
				bcopy(nd.menu, menu, sizeof(menu));
				for(NpcItemMultiMap::const_iterator it = nd.items.begin(); it != nd.items.end(); it++)
				{
					items.insert(*it);
				}
			}
		};

		typedef __gnu_cxx::hash_multimap<DWORD, NpcDialog> NpcDialogMultiMap;

		NpcDialogMultiMap dialogs;
		zRWLock rwlock;

};

#endif

