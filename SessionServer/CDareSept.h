/**
 * \file
 * \version  $Id: CDareSept.h $
 * \author  
 * \date 
 * \brief ��������ս
 *
 * 
 */

#ifndef _CDARE_SEPT_H
#define _CDARE_SEPT_H
#include "CDare.h"

class CDareSept : public CDare
{
	public:
		CDareSept();
		CDareSept(DWORD active_time, DWORD ready_time);
		virtual ~CDareSept();

	public:		
		// ����Ϊ���ö�ս״̬�Ĵ���������ս״̬����鿴��սϵͳ״̬ת��ͼ
		virtual void setSecondID(DWORD dwID);
		virtual void addFirstID(DWORD dwID);
		virtual bool isInvalid();

		virtual void setReadyQuestionState();
		virtual void setReadyActiveState(UserSession* pUser);
		virtual void setReturnGoldState();
		virtual void setReadyOverState();
		
		virtual void sendCmdToAllDarePlayer(Cmd::Session::t_enterWar_SceneSession* cmd, unsigned int cmdLen, unsigned int relationID);
		virtual void sendActiveStateToScene(UserSession* pUser);
 		virtual void notifyWarResult(int winner_type);

		/**
		 * \brief ս���ӷ�
		 *
		 *  ����һ�Σ�����Ӧ������ŶӼ���PK����һ�ˡ��÷ֹ�ʽ���£�
		 *  grade = grade + user.level/10;
		 *
		 * \param pAtt ������
		 * \param pDef ���ط�
		 */
		virtual void addGrade(UserSession* pAtt, UserSession* pDef);
		virtual void timer();

		int dwDareRepute;
	protected:		
		virtual char* getFirstName();
		virtual DWORD getSecondUserID();

};

#endif

