/**
 * \file
 * \version  $Id: CountryTech.h $
 * \author   
 * \date 
 * \brief 国家科技管理器
 *
 * 
 */

#ifndef _COUNTRY_TECH
#define _COUNTRY_TECH
#include "zTime.h"
#include <time.h>       
#include <sys/time.h> 
#include <vector>
#include <map>
#include "zSingleton.h"
#include "zRWLock.h"
#include "SessionCommand.h"

class Scene;

class CTech
{
	public:
		CTech()
		{
			dwType = 0;
			dwLevel = 0;
		}
		
		~CTech(){};
		void init(Cmd::Session::_techItem* rec);
		bool canProduce(DWORD dwObjectLevel);

		DWORD dwType;
		DWORD dwLevel;
};

class CountryTech
{
	public:
		CountryTech(){};
		~CountryTech(){};
		typedef std::map<DWORD, CTech*> CTechMap;

		void init(Cmd::Session::t_updateTech_SceneSession* rev);
		/// 科技索引
		CTechMap techIndex;
		CTech* getTech(DWORD dwType);
		void   addTech(DWORD dwType, CTech* pTech);
		DWORD id;

		zRWLock rwlock;
};

class CountryTechM : public Singleton<CountryTechM>
{
	friend class SingletonFactory<CountryTechM>;
	public:
		~CountryTechM(){}

		/**
		 * \brief 初始化
		 *
		 * 初始化该场景所有国家的国家科技信息
		 *
		 */
		void init();
		CountryTech* getCountryTech(DWORD dwCountryID);
		void   addCountryTech(DWORD dwCountryID);
		bool canProduce(DWORD dwCountry, DWORD dwObjectType, DWORD dwObjectLevel);
		
	protected:
		typedef std::map<DWORD, CountryTech*> CountryTechMap;
		/// 科技索引
		CountryTechMap countryTechIndex;
		CountryTechM(){}
		std::map<DWORD, DWORD> TechMap;
		zRWLock rwlock;
};

#endif

