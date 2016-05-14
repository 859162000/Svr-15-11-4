#pragma once
#include "MiniGame.h"
#include <map>
#include <vector>
#include "CardPattern.h"

class DDZCardGame : public MiniGame
{
	public:
		DDZCardGame(Cmd::MiniGameID id, BYTE userNum, DWORD money);
		bool v_start();
		void v_userLeave(MiniUser *u);
		bool v_parseGameCmd(MiniUser *u, Cmd::stMiniGameUserCmd *cmd, DWORD len);
		void v_timer();

	private:
		enum DDZ_State
		{
			DDZS_POINT,//�з�
			DDZS_PLAY//��ʼ
		}ddz_state;

		std::vector<Cmd::Card> allCardList;
		std::vector<CardList> userCardList;

		BYTE packNum;//������
		BYTE point;//����
		BYTE pointTime;//����
		BYTE rCardNum;//�������ŵ���
		BYTE lordSeat;//������λ��

		BYTE lordPutTime;//��������������
		BYTE otherPutTime;//�����˳���������

		BYTE curPutSeat;//��ǰ���Ƶ���
		BYTE lastPutSeat;//���һ�����Ƶ���
		CardPattern lastPattern;//��������

		BYTE curPointSeat;//��ǰ���Ƶ���
		BYTE lastPointSeat;//��һ���Ƚ��Ƶ���

		DWORD countdown;//��ʱ

		void clean();
		void shuffle();
		void deal();
		void initCards();
		void showReserveCards();//��ʾ����
		BYTE nextPointSeat();
		void nextPutUser(BYTE seat=0);
		void judge(BYTE seat);

		void auto_put();
		void auto_point();

		void calcNormalScore(BYTE seat);//����÷�
		void calcFleeScore(BYTE seat);//������ܼƷ�

		bool canPut(BYTE seat, const Cmd::stPutCardMiniGameCmd *cmd, DWORD len);
		//int compare(const std::list<Cmd::Card> &list1, const std::list<Cmd::Card> &list2);
		void putCards(BYTE seat, Cmd::stPutCardMiniGameCmd *cmd, DWORD len);

		BYTE bombCount();
};
