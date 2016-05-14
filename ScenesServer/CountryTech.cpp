/**
 * \file
 * \version  $Id: CountryTech.cpp $
 * \author 
 * \date 
 * \brief 国家科技管理器
 *
 * 
 */
#include "CountryTech.h"
#include "SceneManager.h"
#include "SceneNpc.h"
#include "TimeTick.h"
#include "SessionClient.h"

void CountryTechM::init()
{
/*	if (!countryTechIndex.empty())
	{
		return;
	}

	for (SceneManager::CountryMap_iter iter=SceneManager::getInstance().country_info.begin(); 
			iter!=SceneManager::getInstance().country_info.end(); iter++)
	{
		addCountryTech(iter->second.id);
	}
*/
	TechMap.insert(std::map<DWORD, DWORD>::value_type(101, 1));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(102, 2));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(103, 3));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(104, 4));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(105, 4));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(106, 4));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(107, 4));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(112, 4));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(109, 5));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(111, 6));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(108, 7));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(110, 8));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(117, 9));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(118, 10));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(115, 11));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(114, 12));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(116, 13));
	TechMap.insert(std::map<DWORD, DWORD>::value_type(113, 14));
}

CountryTech* CountryTechM::getCountryTech(DWORD dwCountryID)
{
	CountryTech* pCountryTech = NULL;

	rwlock.rdlock();
	CountryTechMap::iterator pos;
	pos = countryTechIndex.find(dwCountryID);

	if (pos != countryTechIndex.end())
	{
		pCountryTech = pos->second;
	}       	
	rwlock.unlock();
		
	return pCountryTech;
}

void CountryTechM::addCountryTech(DWORD dwCountryID)
{
	CountryTech* pCountryTech = new CountryTech();
	rwlock.wrlock();
	if (pCountryTech)
	{
		pCountryTech->id = dwCountryID;
		countryTechIndex[dwCountryID] = pCountryTech;
	}
	rwlock.unlock();
}

bool CountryTechM::canProduce(DWORD dwCountry, DWORD dwObjectType, DWORD dwObjectLevel)
{
	CountryTech* pCountryTech = this->getCountryTech(dwCountry);
	if (pCountryTech)
	{
		std::map<DWORD, DWORD>::iterator pos;
		pos = TechMap.find(dwObjectType);
		DWORD dwTechType = 0;

		if (pos != TechMap.end())
		{               
			 dwTechType = pos->second;
		}      	
		
		CTech* pTech = pCountryTech->getTech(dwTechType);
		if (pTech)
		{
			return pTech->canProduce(dwObjectLevel);
		}
		else {
			return true;
		}
	}
	
	return false;
}

//------------------------CountryTech-----------------------
void CountryTech::init(Cmd::Session::t_updateTech_SceneSession* rev)
{
	if (rev)
	{
		for (int i=0; i<14; i++)
		{
			CTech *pTech = this->getTech(rev->data[i].dwType);
			if (!pTech)
			{
				pTech = new CTech();
				this->addTech(rev->data[i].dwType, pTech);
			}

			pTech->init(&rev->data[i]);
		}
	}
	
#ifdef _ZJW_DEBUG
	
	for (CTechMap::iterator mIter=techIndex.begin(); mIter!=techIndex.end(); mIter++)
	{
		CTech* temp = mIter->second;
		Zebra::logger->debug("国家:%d 科技类型:%d 科技等级:%d", this->id, temp->dwType, temp->dwLevel);
	}
#endif	

}

CTech* CountryTech::getTech(DWORD dwType)
{
	CTech* pTech = NULL;

	rwlock.rdlock();
	CTechMap::iterator pos;
	pos = techIndex.find(dwType);

	if (pos != techIndex.end())
	{
		pTech = pos->second;
	}       	
	rwlock.unlock();
	
	return pTech;
}

void CountryTech::addTech(DWORD dwType, CTech* pTech)
{
	rwlock.wrlock();
	if (pTech)
	{
		techIndex.insert(CTechMap::value_type(dwType, pTech));
	}
	rwlock.unlock();

}

//------------------------CTech----------------------
void CTech::init(Cmd::Session::_techItem* rec)
{
	if (rec)
	{
		dwType = rec->dwType;
		dwLevel = rec->dwLevel;
	}
}

bool CTech::canProduce(DWORD dwObjectLevel)
{
	int level_tab[] = {0, 80, 90, 100, 110, 120, 130, 140, 150};

	if ((int)dwObjectLevel>=level_tab[this->dwLevel])
	{
		return false;
	}
	
	return true;	
}

