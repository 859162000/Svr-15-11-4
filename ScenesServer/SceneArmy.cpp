/**
 * \file
 * \version  $Id: SceneArmy.cpp  $
 * \author  
 * \date 
 * \brief ʵ�ֹ������ָ��Ĵ���
 *
 * 
 */
#include <math.h>
#include "SceneUser.h"
#include "Scene.h"
#include "RecordClient.h"
#include "Chat.h"
#include "SessionCommand.h"							
#include "SessionClient.h"
#include "SceneManager.h"

/**
 * \brief ִ�о����������
 *
 *
 * \param rev ��սָ��
 * \param cmdLen ��Ϣ����
 * \return �Ƿ�ɹ�
 */
bool SceneUser::doArmyCmd(const Cmd::stArmyUserCmd *rev,unsigned int cmdLen)
{
	Zebra::logger->debug("doArmyCmd receive byParam:[%d]", rev->byParam);

	switch (rev->byParam)
	{
		case REQ_ARMY_LIST_PARA:
			{
				Cmd::stReqArmyListUserCmd* cmd = (Cmd::stReqArmyListUserCmd*)rev;
				Cmd::Session::t_ReqArmyList_SceneSession  send;

				send.byType = cmd->byType;
				send.dwUserID = this->id;
				send.dwCityID = this->scene->getRealMapID();
				sessionClient->sendCmd(&send, sizeof(send));
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

