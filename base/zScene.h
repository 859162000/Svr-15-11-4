/**
 * \file
 * \version  $Id: zScene.h  $
 * \author  
 * \date 
 * \brief ��������
 */

#ifndef _ZSCENE_H_
#define _ZSCENE_H_

#include "zEntry.h"

enum enumSceneRunningState{
	SCENE_RUNNINGSTATE_NORMAL,//��������
	SCENE_RUNNINGSTATE_UNLOAD,//����ж��
	SCENE_RUNNINGSTATE_REMOVE,//����ж��
};
/**
 * \brief ����������Ϣ����
 */
struct zScene:public zEntry
{
	private:
		DWORD running_state;
	public:
		zScene():running_state(SCENE_RUNNINGSTATE_NORMAL){}
		DWORD getRunningState() const
		{
			return running_state;
		}
		DWORD setRunningState(DWORD set)
		{
			running_state = set;
			return running_state;
		}
};

#endif
