#pragma once
#include "MiniUserCommand.h"
#include "Zebra.h"

typedef std::map<Cmd::Card, DWORD> CardList;
typedef std::map<Cmd::Card, DWORD>::iterator card_iter;

class CardPattern
{
	public:
		static bool match_pattern(const DWORD &packNum, const Cmd::Card *cards, const DWORD &num, CardPattern &pat);

		CardList list;
		DWORD serialNum;//��������
		DWORD unitNum;//һ�鼸��
		Cmd::Card value;//ֵ��������������
		BYTE bomb;
		BYTE missile;
		BYTE add;//�м��Ÿ�����(3˳)

		CardPattern():serialNum(1),unitNum(1),bomb(0),missile(0),add(0),_valid(false){}
		CardPattern &operator=(const CardPattern &c);
		bool operator>(const CardPattern &c);

		void clear(){_valid = false;}
		bool valid()const{return _valid;}
	private:
		bool _valid;
};

