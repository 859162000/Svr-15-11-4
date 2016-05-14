/**
 * \file
 * \version  $Id: CountryAlly.h  $
 * \author  
 * \date 
 * \brief ����ͬ�˹�����
 *
 * 
 */

#ifndef _COUNTRY_ALLY
#define _COUNTRY_ALLY
#include "zTime.h"
#include <time.h>       
#include <sys/time.h> 
#include <vector>
#include <map>
#include "zSingleton.h"
#include "zRWLock.h"
#include "SessionCommand.h"

class Scene;

class CAlly
{
	public:
		CAlly()
		{
			dwCountryID = 0;
			dwAllyCountryID = 0;
			dwFriendDegree = 0;
		}

		~CAlly(){};

		DWORD dwCountryID;
		DWORD dwAllyCountryID;
		DWORD dwFriendDegree;
};

class CountryAllyM : public Singleton<CountryAllyM>
{
	friend class SingletonFactory<CountryAllyM>;
	public:
	~CountryAllyM(){}

	/**
	 * \brief ��ʼ��
	 *
	 * ��ʼ���ó������й��ҵĹ���ͬ����Ϣ
	 *
	 */
	void init();
	CAlly* getAlly(DWORD dwCountryID1, DWORD dwCountryID2);
	void   processUpdate(Cmd::Session::t_updateAlly_SceneSession *rev);
	bool   isAlly(DWORD dwCountryID1, DWORD dwCountryID2);
	DWORD  getFriendLevel(DWORD dwCountryID1, DWORD dwCountryID2);

	protected:
	std::vector<CAlly*> allies;
	CountryAllyM(){}

	zRWLock rwlock;
};

#endif

