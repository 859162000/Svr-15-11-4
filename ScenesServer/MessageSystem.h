/**
 * \file
 * \version  $Id: MessageSystem.h 3203 2005-10-03 03:56:05Z fangq $
 * \author  ������,songsiliang@netease.com
 * \date 2005��01��29�� 21ʱ00��38�� CST
 * \brief ϵͳ��ʾ��Ϣ
 *
 * 
 */

#ifndef _MessageSystem_h_
#define _MessageSystem_h_

#include <vector>
#include "zType.h"
#include "Command.h"
#include "zRWLock.h"
#include "SceneUser.h"

/**
 * \brief ��Ϣϵͳ
 *
 */
class MessageSystem
{

	public:

		~MessageSystem()
		{
			final();
		}

		/**
		 * \brief �õ�Ψһʵ��
		 *
		 */
		static MessageSystem &getInstance()
		{
			if (NULL == instance)
				instance = new MessageSystem();

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
		void check(SceneUser *sceneUser, const bool init = false);

	private:

		static MessageSystem *instance;

		MessageSystem() {};
		void final();

		/**
		 * \brief ��Ϣ���ݽṹ
		 *
		 */
		struct t_Message
		{
			time_t starttime;
			time_t endtime;
			bool login;
			WORD interval;
			int count;
			int order;
			Cmd::stChannelChatUserCmd cmd;
		};

		std::vector<t_Message> messages;
		zRWLock rwlock;

};

#endif

