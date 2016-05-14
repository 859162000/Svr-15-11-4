#ifndef _MINI_USER_COMMAND_H_
#define _MINI_USER_COMMAND_H_

#include "zType.h"
#include "Command.h"
#pragma pack(1)

namespace Cmd
{ 

//BEGIN_ONE_CMD
//////////////////////////////////////////////////////////////
/// ����С��Ϸ���ʼ
//////////////////////////////////////////////////////////////

struct stMiniGameUserCmd : public stNullUserCmd
{
	stMiniGameUserCmd()
	{
		byCmd = MINIGAME_USERCMD;
		subParam = 0;
	}
	BYTE subParam;
};

/*����ָ�ʼ***************************************/

enum MiniGameType
{
	DOUDIZHU	= 1,
	CAISHUZI	= 2,
	MAX_GAMETYPE
};

struct MiniUserPosition
{
	BYTE type;
	BYTE room;
	BYTE game;
	BYTE seat;

	MiniUserPosition(DWORD t=0, DWORD r=0, DWORD g=0, DWORD s=0):type(t),room(r),game(g),seat(s){}
	MiniUserPosition(const MiniUserPosition &p):type(p.type),room(p.room),game(p.game),seat(p.seat){}
	MiniUserPosition & operator =(const MiniUserPosition &p)
	{
		type = p.type;
		room = p.room;
		game = p.game;
		seat = p.seat;
		return *this;
	}
	bool operator ==(const MiniUserPosition &p)
	{
		return type==p.type && room==p.room && game==p.game && seat==p.seat;
	}

	DWORD roomID() const{return (type<<24)+(room<<16);}
	DWORD gameID() const{return (type<<24)+(room<<16)+(game<<8);}
	DWORD id() const{return (type<<24)+(room<<16)+(game<<8)+seat;}
};
typedef MiniUserPosition MiniGameID;
typedef MiniUserPosition MiniRoomID;
typedef MiniUserPosition MiniSeatID;

struct MiniGameScore
{
	BYTE gameType;
	DWORD win;
	DWORD lose;
	DWORD draw;
	int score;
	int money;

	MiniGameScore():gameType(0),win(0),lose(0),draw(0),score(0),money(0){}
	MiniGameScore & operator=(const MiniGameScore &s)
	{
		win = s.win;
		lose = s.lose;
		draw = s.draw;
		score = s.score;
		money = s.money;
		return *this;
	}

	MiniGameScore & operator+=(const MiniGameScore &s)
	{
		win += s.win;
		lose += s.lose;
		draw += s.draw;
		score += s.score;
		money += s.money;
		return *this;
	}
	bool operator==(const MiniGameScore &s) const
	{
		return score==s.score && win==s.win && draw==s.draw && lose==s.lose;
	}
	bool operator<(const MiniGameScore &s) const
	{
		if (score==s.score)
			if (win==s.win)
				if (draw==s.draw)
					return lose>s.lose;
				else
					return draw<s.draw;
			else
				return win<s.win;
		else
			return score<s.score;
	}
	bool operator>(const MiniGameScore &s) const
	{
		return !(*this==s) && !(*this<s);
	}
};

enum MiniGameState
{
	MGS_PREPARE = 1,
	MGS_PLAY    = 2
};

#define COMMON_MINI_PARA 1
struct stCommonMiniGameCmd : public stMiniGameUserCmd
{
	stCommonMiniGameCmd() : stMiniGameUserCmd()
	{
		byParam = COMMON_MINI_PARA;
	}
};

/*
#define INVIT_COMMON_MINI_PARA 1
struct stInvitCommonMiniGameCmd : public stCommonMiniGameCmd
{
stInvitCommonMiniGameCmd() : stCommonMiniGameCmd()
{
subParam = INVIT_COMMON_MINI_PARA;
}
DWORD tempID;//��������/������ ��ʱID
DWORD gameID;//��Ϸ����
};

#define INVIT_RET_COMMON_MINI_PARA 2
struct stInvitRetCommonMiniGameCmd : public stCommonMiniGameCmd
{
stInvitRetCommonMiniGameCmd() : stCommonMiniGameCmd()
{
subParam = INVIT_RET_COMMON_MINI_PARA;
}
DWORD tempid;//��������ʱID
BYTE ret;//0:��ͬ�� 1:ͬ�� 2:����
};
*/

#define LOGIN_COMMON_MINI_PARA 3
struct stLoginCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stLoginCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = LOGIN_COMMON_MINI_PARA;
	}
	DWORD ret;//����/��½�Ƿ�ɹ�
};

#define LOGOUT_COMMON_MINI_PARA 4
struct stLogoutCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stLogoutCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = LOGOUT_COMMON_MINI_PARA;
	}
};

struct MiniRoomData
{
	MiniRoomID roomID;
	WORD gameNum;
	WORD oneGameUserNum;
	DWORD userNum;
	DWORD money;//��Ϸ�һ���
};
//�����б�
#define GAME_LIST_COMMON_MINI_PARA 5
struct stGameListCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stGameListCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = GAME_LIST_COMMON_MINI_PARA;
	}
	DWORD num;//��������
	MiniRoomData data[0];
};

//���뷿��
#define ENTER_ROOM_COMMON_MINI_PARA 6
struct stEnterRoomCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stEnterRoomCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ENTER_ROOM_COMMON_MINI_PARA;
	}
	MiniRoomID roomID;//�����
};

enum MiniUserState
{
	MUS_NOTPLAY	= 0,
	MUS_HALL	= 1,
	MUS_ROOM	= 2,
	MUS_SEAT	= 3,
	MUS_READY	= 4,
	MUS_PLAY	= 5
};
struct MiniUserData
{
	DWORD id;
	char name[MAX_NAMESIZE];
	WORD face;//ͷ��
	DWORD countryID;
	MiniUserPosition pos;
	BYTE state;
	MiniGameScore score;
	MiniUserData():id(0),face(0),countryID(0),state(0){bzero(name, MAX_NAMESIZE);}
	MiniUserData(const MiniUserData &d)
	{
		id = d.id;
		bcopy(d.name, name, MAX_NAMESIZE-1);
		face = d.face;
		countryID = d.countryID;
		pos = d.pos;
		state = d.state;
		score = d.score;
	}

	MiniUserData & operator=(const MiniUserData &d)
	{
		id = d.id;
		bcopy(d.name, name, MAX_NAMESIZE-1);
		face = d.face;
		countryID = d.countryID;
		pos = d.pos;
		state = d.state;
		score = d.score;
		return *this;
	}
};
//���/���·�������б�
#define ADD_ROOM_USER_COMMON_MINI_PARA 7
struct stAddRoomUserCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stAddRoomUserCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ADD_ROOM_USER_COMMON_MINI_PARA;
		num = 0;
	}
	MiniRoomID roomID;
	DWORD num;
	MiniUserData data[0];
};
//���/����һ����������б�
#define ADD_ONE_ROOM_USER_COMMON_MINI_PARA 7
struct stAddOneRoomUserCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stAddOneRoomUserCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ADD_ONE_ROOM_USER_COMMON_MINI_PARA;
		num = 1;
	}
	MiniRoomID roomID;
	DWORD num;
	MiniUserData data;
};

struct MiniSeatData
{
	MiniSeatID seatID;
	BYTE state;//0�ر� 1��
	BYTE isHost;//0�� 1�� 
};
//ˢ����λ����״̬
#define SEAT_STATE_COMMON_MINI_PARA 8
struct stSeatStateCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stSeatStateCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = SEAT_STATE_COMMON_MINI_PARA;
		num = 0;
	}
	DWORD num;
	MiniSeatData data[0];
};
//ˢ��һ����λ����״̬
#define ONE_SEAT_STATE_COMMON_MINI_PARA 8
struct stOneSeatStateCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stOneSeatStateCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ONE_SEAT_STATE_COMMON_MINI_PARA;
		num = 1;
	}
	DWORD num;
	MiniSeatData data;
};

//�뿪����
#define LEAVE_ROOM_COMMON_MINI_PARA 9
struct stLeaveRoomCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stLeaveRoomCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = LEAVE_ROOM_COMMON_MINI_PARA;
	}
	MiniRoomID roomID;
};

//������
#define ENTER_GAME_COMMON_MINI_PARA 10
struct stEnterGameCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stEnterGameCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ENTER_GAME_COMMON_MINI_PARA;
	}
	MiniSeatID seatID;//��λ��
};

/*
//���/������Ϸ����б�
#define ADD_GAME_USER_COMMON_MINI_PARA 11
struct stAddGameUserCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stAddGameUserCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ADD_GAME_USER_COMMON_MINI_PARA;
		num = 0;
	}
	MiniGameID gameID;
	DWORD num;
	MiniUserData data[0];
};

//���/����һ����Ϸ����б�
#define ADD_ONE_GAME_USER_COMMON_MINI_PARA 11
struct stAddOneGameUserCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stAddOneGameUserCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ADD_ONE_GAME_USER_COMMON_MINI_PARA;
		num = 1;
	}
	MiniGameID gameID;
	DWORD num;
	MiniUserData data;
};
*/

//�뿪����
#define LEAVE_GAME_COMMON_MINI_PARA 12
struct stLeaveGameCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stLeaveGameCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = LEAVE_GAME_COMMON_MINI_PARA;
	}
	MiniGameID gameID;
};

/*
//��Ϊ��Ϸ������
#define HOST_GAME_COMMON_MINI_PARA 13
struct stHostGameCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stHostGameCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = HOST_GAME_COMMON_MINI_PARA;
		hostID = 0;
	}
	MiniGameID gameID;
	DWORD hostID;
};
*/

//��/�ر�ĳ��λ��
#define TOGGLE_SEAT_COMMON_MINI_PARA 14
struct stToggleSeatCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stToggleSeatCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = TOGGLE_SEAT_COMMON_MINI_PARA;
		open = 1;
	}
	MiniSeatID seatID;
	BYTE open;//0�ر� 1��
};

//��ĳ��
#define KICK_USER_COMMON_MINI_PARA 15
struct stKickUserCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stKickUserCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = KICK_USER_COMMON_MINI_PARA;
		userID = 0;
	}
	MiniGameID gameID;
	DWORD userID;
};

//�л�׼��/δ׼��״̬
#define TOGGLE_READY_COMMON_MINI_PARA 16
struct stToggleReadyCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stToggleReadyCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = TOGGLE_READY_COMMON_MINI_PARA;
	}
	MiniGameID gameID;
};

//�������ȷ����Ϣ
#define LOGIN_RET_COMMON_MINI_PARA 17
struct stLoginRetCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stLoginRetCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = LOGIN_RET_COMMON_MINI_PARA;
		ret = 0;
	}
	BYTE ret;//1�ɹ� 0ʧ��
};

//���뷿��ȷ����Ϣ
#define ENTER_ROOM_RET_COMMON_MINI_PARA 18
struct stEnterRoomRetCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stEnterRoomRetCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ENTER_ROOM_RET_COMMON_MINI_PARA;
		ret = 0;
	}
	BYTE ret;//1�ɹ� 0ʧ�� 2�������� 3�Ѿ��μӸ����͵���Ϸ
	MiniRoomID roomID;
};

//������Ϸȷ����Ϣ
#define ENTER_GAME_RET_COMMON_MINI_PARA 19
struct stEnterGameRetCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stEnterGameRetCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = ENTER_GAME_RET_COMMON_MINI_PARA;
		ret = 0;
	}
	BYTE ret;//1�ɹ� 0ʧ�� 2�������� 3��Ϸ�ѿ�ʼ 4��λ������ 5���Ӳ��� 
	MiniSeatID seatID;
};

//ˢ�����״̬
#define UPDATE_USER_STATE_COMMON_MINI_PARA 20
struct stUpdateUserStateCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stUpdateUserStateCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = UPDATE_USER_STATE_COMMON_MINI_PARA;
		userID = 0;
		state = MUS_NOTPLAY;
	}
	DWORD userID;//���ID
	BYTE state;//���״̬
	MiniUserPosition pos;//��ǰλ��
};

//��Ϸ��ʼ
#define GAME_START_COMMON_MINI_PARA 21
struct stGameStartCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stGameStartCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = GAME_START_COMMON_MINI_PARA;
	}
	MiniGameID gameID;
};

//��Ϸ����
#define GAME_END_COMMON_MINI_PARA 22
struct stGameEndCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stGameEndCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = GAME_END_COMMON_MINI_PARA;
	}
	MiniGameID gameID;
};

//֪ͨ��Ϸ���/����
#define GAME_RESULT_COMMON_MINI_PARA 23
struct stGameResultCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stGameResultCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = GAME_RESULT_COMMON_MINI_PARA;
		userID = 0;
	}
	MiniGameID gameID;
	DWORD userID;
	MiniGameScore score;
};

/*
//������ҵķ���
#define REQ_SCORE_COMMON_MINI_PARA 24
struct stReqScoreCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stReqScoreCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = REQ_SCORE_COMMON_MINI_PARA;
		userID = 0;
		gameType = 1;
	}
	DWORD userID;
	BYTE gameType;
};
*/

//������ҵķ���
#define USER_SCORE_COMMON_MINI_PARA 25
struct stUserScoreCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stUserScoreCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = USER_SCORE_COMMON_MINI_PARA;
		userID = 0;
	}
	DWORD userID;
	MiniGameScore score;
};

enum MiniChatType
{
	MCT_NORMAL,//��ͨ
	MCT_SYS,//ϵͳ
	MCT_POPUP//��������
};

//����
#define CHAT_COMMON_MINI_PARA 26
struct stChatCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stChatCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = CHAT_COMMON_MINI_PARA;
		userID = 0;
		type = MCT_NORMAL;
		bzero(content, sizeof(content));
	}
	DWORD userID;
	MiniUserPosition pos;//˵��λ��
	BYTE type;//��������
	char content[64];
};

//�������������
#define REQ_ROOM_DATA_COMMON_MINI_PARA 27
struct stReqRoomDataCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stReqRoomDataCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = REQ_ROOM_DATA_COMMON_MINI_PARA;
	}
};

//�������а�
#define REQ_TOP_COMMON_MINI_PARA 28
struct stReqTopCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stReqTopCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = REQ_TOP_COMMON_MINI_PARA;
	}
};

//�������а�
#define RET_TOP_COMMON_MINI_PARA 29
struct stRetTopCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stRetTopCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = RET_TOP_COMMON_MINI_PARA;
		num = 0;
	}
	DWORD num;
	MiniUserData data[0];
};

//����Ϸ��
#define DEPOSIT_COMMON_MINI_PARA 30
struct stDepositCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stDepositCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = DEPOSIT_COMMON_MINI_PARA;
		num = 0;
	}
	DWORD num;
};

//�һ�����
#define DRAW_COMMON_MINI_PARA 31
struct stDrawCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stDrawCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = DRAW_COMMON_MINI_PARA;
		num = 0;
	}
	DWORD num;
};

//�һ���������
#define DRAW_RET_COMMON_MINI_PARA 32
struct stDrawRetCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stDrawRetCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = DRAW_RET_COMMON_MINI_PARA;
		ret = 0;
	}
	BYTE ret;//0ʧ�� 1�ɹ� 2���� 3������
};

//��������Լ�����Ϣ
#define USER_DATA_COMMON_MINI_PARA 33
struct stUserDataCommonMiniGameCmd : public stCommonMiniGameCmd
{
	stUserDataCommonMiniGameCmd() : stCommonMiniGameCmd()
	{
		subParam = USER_DATA_COMMON_MINI_PARA;
	}
	MiniUserData data;
};

/*����ָ�����****************************************/

/*����ָ�ʼ**************************************/
class Card
{
	public:
		enum Suit
		{
			Diamond = 0x10,
			Club = 0x20,
			Heart = 0x30,
			Spade = 0x40,
			Joker = 0x50
		};

		Card():value(0){}
		Card(const Card &c):value(c.value){}
		Card(const unsigned char v):value(v){}
		Card(const Suit s, const unsigned char n):value(s|n){}

		unsigned char suit() const {return value & 0xf0;}
		unsigned char number() const {return value & 0x0f;}

		bool valid()
		{
			if (!suit() || suit()>5) return false;
			if (!number() || number()>13) return false;
			if (Joker==suit() && number()<14) return false;
			return true;
		}

		bool operator==(const Card &c) const{return number()==c.number();}
		bool operator<(const Card &c) const{return number()<c.number();}
		bool operator>(const Card &c) const{return number()>c.number();}
	private:
		unsigned char value;
};

#define CARD_MINI_PARA 2
struct stCardMiniGameCmd : public stMiniGameUserCmd
{
	stCardMiniGameCmd() : stMiniGameUserCmd()
	{
		byParam = CARD_MINI_PARA;
	}
	MiniGameID gameID;
};

//����
#define DEAL_CARD_MINI_PARA 1
struct stDealCardMiniGameCmd : public stCardMiniGameCmd
{
	stDealCardMiniGameCmd() : stCardMiniGameCmd()
	{
		subParam = DEAL_CARD_MINI_PARA;
		num = 0;
	}
	BYTE num;
	Card cards[0];
};

//֪ͨĳ�˳���
#define NOTIFY_PUT_CARD_MINI_PARA 2
struct stNotifyPutCardMiniGameCmd : public stCardMiniGameCmd
{
	stNotifyPutCardMiniGameCmd() : stCardMiniGameCmd()
	{
		subParam = NOTIFY_PUT_CARD_MINI_PARA;
		userID = 0;
	}
	DWORD userID;
};

//����
#define PUT_CARD_MINI_PARA 3
struct stPutCardMiniGameCmd : public stCardMiniGameCmd
{
	stPutCardMiniGameCmd() : stCardMiniGameCmd()
	{
		subParam = PUT_CARD_MINI_PARA;
		userID = 0;
		num = 0;
	}
	DWORD userID;
	BYTE num;
	Card cards[0];
};

//֪ͨĳ�˽з�
#define NOTIFY_POINT_DDZ_MINI_PARA 4
struct stNotifyPointDDZMiniGameCmd : public stCardMiniGameCmd
{
	stNotifyPointDDZMiniGameCmd() : stCardMiniGameCmd()
	{
		subParam = NOTIFY_POINT_DDZ_MINI_PARA;
		userID = 0;
	}
	DWORD userID;
};

//�������з�
#define POINT_DDZ_MINI_PARA 5
struct stPointDDZMiniGameCmd : public stCardMiniGameCmd
{
	stPointDDZMiniGameCmd() : stCardMiniGameCmd()
	{
		subParam = POINT_DDZ_MINI_PARA;
		userID = 0;
		num = 0;
	}
	DWORD userID;
	BYTE num;
};

//��ʾ����
#define RESERVE_CARDS_DDZ_MINI_PARA 6
struct stReserveCardsDDZMiniGameCmd : public stCardMiniGameCmd
{
	stReserveCardsDDZMiniGameCmd() : stCardMiniGameCmd()
	{
		subParam = RESERVE_CARDS_DDZ_MINI_PARA;
		userID = 0;
		num = 0;
	}
	DWORD userID;
	BYTE num;
	Card cards[0];
};

//̯��
#define SHOW_HAND_DDZ_MINI_PARA 6
struct stShowHandDDZMiniGameCmd : public stCardMiniGameCmd
{
	stShowHandDDZMiniGameCmd() : stCardMiniGameCmd()
	{
		subParam = SHOW_HAND_DDZ_MINI_PARA;
		userID = 0;
		num = 0;
	}
	DWORD userID;
	BYTE num;
	Card cards[0];
};

/*����ָ�����****************************************/

/*******************************************************/
// ����
#define INVITE_MINI_PARA 11
struct stInviteMiniGame : public stMiniGameUserCmd
{
	stInviteMiniGame()
	{
		byParam = INVITE_MINI_PARA;
	}
	DWORD tempid;//������/�������ߵ���ʱID
	DWORD money;//��ע
};

// ����ظ�
#define INVITE_RET_MINI_PARA 12
struct stInviteRetMiniGame : public stMiniGameUserCmd
{
	stInviteRetMiniGame()
	{
		byParam = INVITE_RET_MINI_PARA;
	}
	DWORD tempid;//�����ߵ���ʱID
	DWORD money;//��ע
	DWORD ret;//1:ͬ�� 0:��ͬ��
};

// ��ʼ��Ϸ
#define START_MINI_PARA 13
struct stStartMiniGame : public stMiniGameUserCmd
{
	stStartMiniGame()
	{
		byParam = START_MINI_PARA;
	}
	DWORD tempid;//�Է�����ʱID
	DWORD money;//��ע
};

// ֹͣɫ��
#define STOP_DICE_MINI_PARA 14
struct stStopDiceMiniGame : public stMiniGameUserCmd
{
	stStopDiceMiniGame()
	{
		byParam = STOP_DICE_MINI_PARA;
	}
};

// ɫ������
#define DICE_NUM_MINI_PARA 15
struct stDiceNumMiniGame : public stMiniGameUserCmd
{
	stDiceNumMiniGame()
	{
		byParam = DICE_NUM_MINI_PARA;
	}
	DWORD tempid;//��ɫ���˵���ʱID
	DWORD num;
};

// ���
#define RESULT_MINI_PARA 16
struct stResultMiniGame : public stMiniGameUserCmd
{
	stResultMiniGame()
	{
		byParam = RESULT_MINI_PARA;
	}
	DWORD res;//0:�� 1:Ӯ 2:ƽ
};

// ��Ϸ���� //�����뿪��Ϸ
#define END_MINI_PARA 17
struct stEndMiniGame : public stMiniGameUserCmd
{
	stEndMiniGame()
	{
		byParam = END_MINI_PARA;

		reason = 0;
	}
	DWORD reason;//����ԭ�� 0:�������� 1:�Է��뿪
};

// ������Ϸ
#define CONTINUE_MINI_PARA 18
struct stContinueMiniGame : public stMiniGameUserCmd
{
	stContinueMiniGame()
	{
		byParam = CONTINUE_MINI_PARA;
	}
	DWORD ret;//0:������ 1:����
};

// ֪ͨ�ͻ��˽�����Ϸ
#define ENTER_MINI_PARA 19
struct stEnterMiniGame : public stMiniGameUserCmd
{
	stEnterMiniGame()
	{
		byParam = ENTER_MINI_PARA;
	}
};
/*******************************************************/

//////////////////////////////////////////////////////////////
/// ����С��Ϸ�������
//////////////////////////////////////////////////////////////

}
#pragma pack()

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
