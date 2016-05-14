/**
 * \file
 * \version  $Id: CDareSeptNpc.h  $
 * \author  
 * \date 
 * \brief ����NPC����ս
 *
 * 
 */

#ifndef _CDARE_SEPT_NPC_H
#define _CDARE_SEPT_NPC_H
#include "CDare.h"
#include "CDareSept.h"

/**
 * \brief ����NPC����ս 
 *
 */
class CDareSeptNpc : public CDareSept
{
	public:
		CDareSeptNpc(DWORD active_time, DWORD ready_time);
		virtual ~CDareSeptNpc();

	public:		
		virtual void setSecondID(DWORD dwID){CDareSept::setSecondID(dwID);}
		virtual void addFirstID(DWORD dwID){CDareSept::addFirstID(dwID);}
		
		virtual void sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, unsigned int cmdLen, unsigned int relationID)
		{
			CDareSept::sendCmdToAllDarePlayer(cmd, cmdLen, relationID);
		}
		
		virtual void sendActiveStateToScene(UserSession* pUser)
		{
			CDareSept::sendActiveStateToScene(pUser);
		}

		/**
  		  * \brief ��ս���֪ͨ
		  *
		  * \param winner_type �������:0 ��ս��ʤ�� 1 Ӧս��ʤ�� 2 սƽ
		  *		  
		  */
 		virtual void notifyWarResult(int winner_type){};

		/**
		 * \brief ս���ӷ�
		 *
		 *  ����һ�Σ�����Ӧ������ŶӼ���PK����һ�ˡ��÷ֹ�ʽ���£�
		 *  grade = grade + user.level/10;
		 *
		 * \param pAtt ������
		 * \param pDef ���ط�
		 */
		virtual void addGrade(UserSession* pAtt, UserSession* pDef){};
		
		virtual void setReadyOverState();
};
#endif

