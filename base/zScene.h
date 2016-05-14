/**
 * \file
 * \version  $Id: zScene.h  $
 * \author  
 * \date 
 * \brief 场景定义
 */

#ifndef _ZSCENE_H_
#define _ZSCENE_H_

#include "zEntry.h"

enum enumSceneRunningState{
	SCENE_RUNNINGSTATE_NORMAL,//正常运行
	SCENE_RUNNINGSTATE_UNLOAD,//正在卸载
	SCENE_RUNNINGSTATE_REMOVE,//正在卸载
};
/**
 * \brief 场景基本信息定义
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
