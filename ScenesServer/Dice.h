#ifndef _DICE_H_
#define _DICE_H_

#include "SceneUser.h"
#include "MiniUserCommand.h"

class Dice
{
	public:
		Dice(SceneUser * u1, SceneUser * u2, DWORD money);
		~Dice(){};

		enum DiceState
		{
			DICE_STATE_CREATE,//δ��ʼ������������
			DICE_STATE_ROLLING,//ɫ����ת���ȴ�ֹͣ��Ϣ
			DICE_STATE_END,//һ�ֿ�ʼ֮ǰ�ȴ�˫��׼��
			DICE_STATE_DEL//�ȴ�ɾ��
		};

		bool init();

		bool sendCmdToAll(const void *cmd, const DWORD len);
		bool sendAllInfo(const char *pattern, ...);

		void endGame(SceneUser * user=0);
		bool rotate(DWORD id);
		bool setReady(SceneUser * user);

		DWORD getMoney();
		DWORD getTheOtherTempID(DWORD id);
		bool judge();

		DiceState getState();
		bool timer(DWORD time, SceneUser * u);
	private:
		DWORD startTime;

		DWORD round;//n��Ŀ
		DWORD money;
		DiceState gameState;

		DWORD tempid1, tempid2;
		DWORD value1, value2;
		bool continue1, continue2;
		char name1[MAX_NAMESIZE], name2[MAX_NAMESIZE];
};

#endif
