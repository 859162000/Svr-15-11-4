#ifndef _MINI_GAME_H_
#define _MINI_GAME_H_

#include "MiniUserCommand.h"
#include <vector>

class MiniUser;
class MiniRoom;

struct Seat
{
	MiniUser *user;
	BYTE open;
	Seat():user(0),open(1){}
};

class MiniGame
{
	public:
		MiniGame(Cmd::MiniGameID id, BYTE userNum, DWORD money);
		virtual ~MiniGame(){};

		bool userEnter(MiniUser *u, Cmd::MiniUserPosition seatID);
		void userLeave(MiniUser *u);
		bool toggleReady(MiniUser *u);

		BYTE find(MiniUser *u);
		void full_MiniSeatData(Cmd::stSeatStateCommonMiniGameCmd *cmd);
		Cmd::MiniGameState getState();

		bool empty();
		bool full();
		bool canStart();
		bool start();
		bool end();
		void timer();

		bool parseGameCmd(MiniUser *u, Cmd::stMiniGameUserCmd *cmd, DWORD len);

		void sendCmdToAll(const void *cmd, const unsigned int len) const;
		void sendInfoToAll(const int type, const char *info, ...) const;

		void toggleSeat(MiniUser *host, Cmd::MiniSeatID seatID);
		void kickUser(MiniUser *h, MiniUser *u);

		MiniRoom *getRoom();
	protected:
		virtual void v_userEnter(MiniUser *u, Cmd::MiniUserPosition seatID){}
		virtual void v_userLeave(MiniUser *u){}
		virtual bool v_start(){return true;}
		virtual bool v_end(){return true;}
		virtual void v_timer(){}
		virtual bool v_parseGameCmd(MiniUser *u, Cmd::stMiniGameUserCmd *cmd, DWORD len){return false;}

		Cmd::MiniSeatID makeSeatID(BYTE seat){return Cmd::MiniSeatID(id.type, id.room, id.game, seat);}
		void setHost(BYTE seat);

		void enableAllSeats();
		BYTE nextUserSeat(BYTE from);//得到下一个有人的座位
	public:
		Cmd::MiniGameID id;

	protected:
		Cmd::MiniGameState state;

		BYTE minUserNum;//最少人数
		BYTE maxUserNum;//最多人数
		BYTE curUserNum;//当前人数
		//BYTE hostSeat;//房主的座位ID
		std::vector<Seat> seatList;

		DWORD money;//游戏币基数
};

#endif
