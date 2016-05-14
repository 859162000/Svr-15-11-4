/**
 * \file
 * \version  $Id: Scene.cpp  $
 * \author  
 * \date 
 * \brief �������ʵ��
 *
 * 
 */

#include <string>
#include <vector>
#include <set>

#include "Scene.h"
#include "LoadMap.h"
#include "Zebra.h"
#include "zRegex.h"
#include "zMisc.h"
#include "zDatabaseManager.h"
#include "SceneNpc.h"
#include "SceneNpcManager.h"
#include "TimeTick.h"
#include "Command.h"
#include "zString.h"
#include "zObject.h"
#include "ScenePk.h"
#include "SessionClient.h"
#include "SkillBase.h"
#include "Zebra.h"
#include "SceneManager.h"
#include "SceneTaskManager.h"
#include "RebuildObject.h"

/**
 * \brief ���캯��
 *
 */
Scene::Scene(): _one_sec(1)
{
	userCount=0;
	countryID=0;
	function=0;
	backtoMapID=0;
	backtoCityMapID=0;
	foreignerBacktoMapID=0; 
	countryDareBackToMapID=0;
	commonCountryBacktoMapID=0; 
	commonUserBacktoMapID=0;
	inited=false;
	execGroup = 0;
	isUnionDare = false;
	isCountryFormalDare = false;
	//dwAttCountryID = 0;
	level =0;
	exprate=0.0f;
	winner_exp=false;
	pklevel=0;
	isEmperorDare = false;
	dwEmperorDareDef = 0;
}

/**
 * \brief ��������
 *
 */
Scene::~Scene()
{
	final();
}

void Scene::freshGateScreenIndex(SceneUser *pUser, const DWORD screen)
{
	Cmd::Scene::t_fresh_ScreenIndex index;
	index.dwMapTempID = tempid;
	index.dwScreen = screen;
	index.dwUserTempID = pUser->tempid;
	pUser->gatetask->sendCmd(&index , sizeof(index));
}

/**
 * \brief ��ȡ��ͼ������������
 * \return ��ͼ������������
 */
const char * Scene::getCountryName() const
{
	return SceneManager::getInstance().getCountryNameByCountryID(countryID);
}

/**
 * \brief Ϊ����Ʒ����һ�������
 * \param pos ���ĵ�
 * \param findedPos ���ҵ��ĵ�
 * \return �����Ƿ�ɹ�
 */
bool Scene::findPosForObject(const zPos &pos, zPos &findedPos)
{
	int side = 0;
	int direct = zMisc::randBetween(0, 7);
	int clockwise = zMisc::selectByPercent(50) ? -1 : 1;
	int count = 0;
	findedPos = pos;
	while (checkObjectBlock(findedPos))
	{
		if (side > 14)
		{
			return false;
		}
		getNextPos(side, direct, pos, clockwise, findedPos);
		if (++count>=28*28) break;
	}
	setObjectBlock(findedPos);
	return true;
}

/**
 * \brief Ϊ�������һ�������
 * \param pos ���ĵ�
 * \param findedPos ���ҵ��ĵ�
 * \return �����Ƿ�ɹ�
 */
bool Scene::findPosForUser(const zPos &pos, zPos &findedPos)
{
	int side = 0;
	int direct = zMisc::randBetween(0, 7);
	int clockwise = zMisc::selectByPercent(50) ? -1 : 1;
	int count = 0;
	findedPos = pos;
	while (checkBlock(findedPos))
	{
		if (side > 14)
		{
			return false;
		}
		getNextPos(side, direct, pos, clockwise, findedPos);
		if (++count>=28*28) return false;
	}
	setBlock(findedPos);
	Zebra::logger->debug("%s, count = %u", __PRETTY_FUNCTION__, count);
	return true;
}

/**
 * \brief �������꣬��ȡվ�������������������
 * \param pos �����
 * \param bState �Ƿ��޶��ҵ�����ҵ�״̬
 * \param byState ��ҵ�״̬
 * \return ���
 */
SceneUser *Scene::getSceneUserByPos(const zPos &pos, const bool bState, const zSceneEntry::SceneEntryState byState)
{
	return (SceneUser *)getSceneEntryByPos(zSceneEntry::SceneEntry_Player, pos, bState, byState);
}

/**
  * \brief ͨ�����ID��ȡ��������
  *
  * \param userid ���ID
  *
  * \return ���ʵ��
  *
  */
SceneUser *Scene::getUserByID(DWORD userid)
{
	SceneUser *ret=SceneUserManager::getMe().getUserByID(userid);
	if(ret)
	{
		if(ret->scene!=this) ret=NULL;
	}
	return ret;
}

/**
 * \brief ������ʱid�õ����ָ��
 *
 *
 * \param usertempid �����ʱid
 * \return ���ָ�룬ʧ�ܷ���0
 */
SceneUser *Scene::getUserByTempID(DWORD usertempid)
{
	SceneUser *ret=SceneUserManager::getMe().getUserByTempID(usertempid);
	if(ret)
	{
		if(ret->scene!=this) ret=NULL;
	}
	return ret;
}

/**
 * \brief �������ֵõ����ָ��
 *
 *
 * \param username �������
 * \return ���ָ�룬ʧ�ܷ���0
 */
SceneUser *Scene::getUserByName(const char *username)
{
	SceneUser *ret=SceneUserManager::getMe().getUserByName(username);
	if(ret)
	{
		if(ret->scene!=this) ret=NULL;
	}
	return ret;
}

/**
 * \brief �������꣬��ȡվ���������������Npc
 * \param pos �����
 * \param bState �Ƿ��޶�npc������״̬
 * \param byState npc��������״̬
 * \return Npc
 */
SceneNpc *Scene::getSceneNpcByPos(const zPos &pos, const bool bState, const zSceneEntry::SceneEntryState byState)
{
	return (SceneNpc *)getSceneEntryByPos(zSceneEntry::SceneEntry_NPC, pos, bState, byState);
}

/**
 * \brief ������ʱid�õ�NPCָ��
 *
 *
 * \param npctempid �����ʱid
 * \return NPCָ�룬ʧ�ܷ���0
 */
SceneNpc *Scene::getNpcByTempID(DWORD npctempid)
{
	SceneNpc *ret=SceneNpcManager::getMe().getNpcByTempID(npctempid);
	if(ret)
	{
		if(ret->scene!=this) ret=NULL;
	}
	return ret;
}

struct AICallback : public zSceneEntryCallBack
{
	const zRTime &ctv;
	MonkeyNpcs delNpc;
	AICallback(const zRTime &ctv) : ctv(ctv) {}
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ���������ȫ����npc
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
		SceneNpc *npc = (SceneNpc *)entry;
		if (npc->needClear())
			delNpc.insert(npc);
		else
			npc->action(ctv);
		return true;
	}
};
struct GetAffectScreen : public zSceneEntryCallBack
{
	DWORD group;
	/**
	 * \brief ���캯��
	 * \param scene ����
	 */
	GetAffectScreen(DWORD g) :group(g) {};
	/**
	 * \brief �ص�����
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{

		My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
		SceneUser *pUser = (SceneUser *)entry;
		if(group == pUser->tempid%MAX_NPC_GROUP && pUser->scene)
			pUser->refreshMe();

		return true;
	}
};

bool Scene::SceneEntryAction(const zRTime& ctv, const DWORD group)
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	////FunctionTimes(0,__FUNCTION__);
	if(_one_sec(ctv))
	{
		//����
		processRush();
		//���µر���Ʒ
		updateSceneObject();
		if(this->getRealMapID() == 190 && userCount)
		{
			struct tm tm_1;
			time_t timValue = time(NULL);
			tm_1=*localtime(&timValue);
			/*
			if((tm_1.tm_hour%2) && (tm_1.tm_min >= 55))
			{
			}
			else if(((tm_1.tm_hour%2) == 0) && tm_1.tm_min == 30)
			// */
			if(((tm_1.tm_hour%2) == 0) && tm_1.tm_min == 30 && tm_1.tm_sec < 3)
			{
				//SceneUserManager::getMe().removeUserToHuangcheng(this);
			}
		}
	}

	//�����ռ���Ӱ�������ţ��Ե�ͼ�����еĽ�ɫ�ָ�
	GetAffectScreen affectScreen(group);
	execAllOfScene(zSceneEntry::SceneEntry_Player, affectScreen);
	if(userCount>400)
	{
		SceneTaskManager::getInstance().execEvery();
	}
	AICallback callback(ctv);
	execAllOfEffectNpcScreen(group,callback);
	//���մ�ɾ����npc
	if (!callback.delNpc.empty())
	{
		//FunctionTime func_alltime(0,__PRETTY_FUNCTION__,"���ն�̬npc��Ҫ��ʱ��" , 32);
		for(MonkeyNpcs::iterator it=callback.delNpc.begin();it!=callback.delNpc.end();++it)
		{
			SceneNpc *npc = *it;

			//֪ͨ�ͻ���ɾ��NPC
			Cmd::stRemoveMapNpcMapScreenUserCmd removeMapNpc;
			removeMapNpc.dwMapNpcDataPosition = npc->tempid;
			sendCmdToNine(npc->getPosI(), &removeMapNpc, sizeof(removeMapNpc));
			//�ӵ�ͼ�͹�������ɾ�����NPC
			removeNpc(npc);
			SceneNpcManager::getMe().removeSceneNpc(npc);
			SceneNpcManager::getMe().removeSpecialNpc(npc);
			//Zebra::logger->trace("%s, ɾ��npc %s(%u)", __FUNCTION__, npc->name, npc->tempid);
			SAFE_DELETE(npc);
		}
	}
	static int allcount=0;
	allcount += userCount;
	if(allcount >= 100)
	{
		allcount=0;
		return true;
	}
	return false;
}

/**
 * \brief �������꣬��ȡվ�����������������Ʒ
 * \param pos �����
 * \return ��Ʒ
 */
zSceneObject *Scene::getSceneObjectByPos(const zPos &pos)
{
	return (zSceneObject *)getSceneEntryByPos(zSceneEntry::SceneEntry_Object, pos);
}

/**
 * \brief �ڵ�ͼ�����Ƴ�ĳһ����Ʒ
 * \param so ��ͼ��Ʒ
 */
void Scene::removeObject(zSceneObject *so)
{
	if (removeSceneEntry(so))
	{
		clearObjectBlock(so->getPos());
	}
}

/**
 * \brief �ڵ�ͼ�����Ƴ�ĳһ��Npc
 * \param sn ��ͼNpc
 */
void Scene::removeNpc(SceneNpc *sn)
{
	if (removeSceneEntry(sn)
			&& sn->getState() == zSceneEntry::SceneEntry_Normal)
	{
		clearBlock(sn->getPos());
	}
}

/**
 * \brief ��������������������
 * \param pos ����������
 * \param type ��Ҫ������������
 * \return �Ƿ�ɹ�
 */
bool Scene::checkZoneType(const zPos &pos, const int type) const
{
	int ret = ZoneTypeDef::ZONE_NONE;
	for(ZoneTypeDefVector::const_iterator it = zoneTypeDef.begin(); it != zoneTypeDef.end(); it++)
	{
		if ((*it).region.isIn(pos))
		{
			ret |= (*it).type;
		}
	}
	return ZoneTypeDef::ZONE_NONE != (ret & type);
}

/**
 * \brief ��������������������
 * \param pos ����������
 * \return ��������
 */
int Scene::getZoneType(const zPos &pos) const
{
	int ret = ZoneTypeDef::ZONE_NONE;
	for(ZoneTypeDefVector::const_iterator it = zoneTypeDef.begin(); it != zoneTypeDef.end(); it++)
	{
		if ((*it).region.isIn(pos))
		{
			ret |= (*it).type;
		}
	}
	return ret;
}

/**
 * \brief �ڵ�ͼָ�����͵��������������������
 * \param type ��������
 * \param pos ����������
 * \return �Ƿ�ɹ�
 */
bool Scene::randzPosByZoneType(const int type, zPos &pos) const
{
	if (ZoneTypeDef::ZONE_NONE == type) {
#if 0
		int i=0;
		do {
			pos.x = zMisc::randBetween(1, width() -1);
			pos.y = zMisc::randBetween(1, height() -1);
		} while (checkBlock(pos, TILE_BLOCK) && ++i<10000);

		Zebra::logger->debug("%s, i = %u", __PRETTY_FUNCTION__, i);
		return true;
#else
		return randPosByRegion(_index, pos);
#endif
	}

	for(ZoneTypeDefVector::const_iterator it = zoneTypeDef.begin(); it != zoneTypeDef.end(); it++)
	{
		if (ZoneTypeDef::ZONE_NONE != ((*it).type & type))
		{
			return randPosByRegion((*it).region.index, pos);
		}
	}
	return false;
}



/**
 * \brief ����л���ͼ
 *
 *
 * \param pUser ���ָ��
 * \param deathBackto �Ƿ��������س�
 * \param ignoreWar ������ת�еĹ�ս������
 * \return 0 ������ʧ�ܣ�1 �ڱ����������� 2���������
 */
int Scene::changeMap(SceneUser *pUser , bool deathBackto, bool ignoreWar)
{
	if(!pUser)
	{
		return 0;
	}
	//TODO
	//�ڱ�ĵ�ͼѰ��������
	//Scene *toscene=SceneManager::getInstance().getSceneByName(backtoMapName);
	Scene *toscene=NULL;

	if(deathBackto)
	{
		if(pUser->charbase.country == this->countryID)
		{
			toscene=SceneManager::getInstance().getSceneByID(backtoMapID);
		}
		else if(pUser->deathBackToMapID)
		{
			toscene=SceneManager::getInstance().getSceneByID(pUser->deathBackToMapID);
		}
		else
		{
			if(this->getCountryID() == 6)
			{
				if (commonCountryBacktoMapID)
				{
					toscene=SceneManager::getInstance().getSceneByID((this->countryID<<16) + commonCountryBacktoMapID);
				}
				else
				{
					toscene=SceneManager::getInstance().getSceneByID((pUser->charbase.country<<16) + 
							foreignerBacktoMapID);
				}
			}
			else
			{
				if(pUser->charbase.country == 6)
				{
					toscene=SceneManager::getInstance().getSceneByID((pUser->charbase.country << 16 ) + commonUserBacktoMapID);
				}
				else
				{
					toscene=SceneManager::getInstance().getSceneByID((pUser->charbase.country << 16 ) + foreignerBacktoMapID);
				}
			}
		}
	}
	else
	{
		toscene=SceneManager::getInstance().getSceneByID(backtoCityMapID);
	}

	if(toscene)
	{
		if ((!toscene->checkUserLevel(pUser))&&(pUser->scene!=toscene)) return true;

		zPos newpos;
		newpos.x=0,newpos.y=0;
		if(pUser->charbase.country == this->countryID)
		{
			if(toscene->randzPosByZoneType(ZoneTypeDef::ZONE_RELIVE , newpos))
			{
				pUser->changeMap(toscene,newpos);
				return 1;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			if (pUser->isSpecWar(Cmd::COUNTRY_FORMAL_DARE) && pUser->scene->getCountryDare() && !ignoreWar)
			{
				if (toscene->randzPosByZoneType(ZoneTypeDef::ZONE_PRIVATE_RELIVE , newpos))
				{
					pUser->changeMap(toscene, newpos);
					return 1;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				if(toscene->randzPosByZoneType(ZoneTypeDef::ZONE_RELIVE, newpos))
				{
					pUser->changeMap(toscene,newpos);
					return 1;
				}
				else
				{
					return 1;
				}
			}
		}
	}
	else
	{
		Cmd::Session::t_changeScene_SceneSession cmd;
		cmd.id = pUser->id;
		cmd.temp_id = pUser->tempid;
		cmd.x = 0;
		cmd.y = 0;
		bzero(cmd.map_file ,sizeof(cmd.map_file));
		bzero(cmd.map_name ,sizeof(cmd.map_name));
		if(deathBackto)
		{
			if(pUser->deathBackToMapID)
			{
				cmd.map_id = pUser->deathBackToMapID;
			}
			else
			{
				if (this->getCountryID() == 6 && !this->commonCountryBacktoMapID)
				{
					cmd.map_id = (pUser->charbase.country<<16) + this->foreignerBacktoMapID;
				}
				else
				{
					cmd.map_id = this->backtoMapID;
				}
			}
		}
		else
		{
			cmd.map_id = this->backtoCityMapID;
		}
		//bcopy(backtoMapName , cmd.map_name , sizeof(cmd.map_name));
		sessionClient->sendCmd(&cmd, sizeof(cmd));
		std::string mapname;
		SceneManager::MapMap_iter map_iter = SceneManager::getInstance().map_info.find(cmd.map_id & 0X0000FFFF);
		SceneManager::CountryMap_iter country_iter = SceneManager::getInstance().country_info.find(cmd.map_id >> 16);
		if(map_iter != SceneManager::getInstance().map_info.end() && 
				country_iter != SceneManager::getInstance().country_info.end())
		{
			mapname = country_iter->second.name;
			mapname+= "��";
			mapname+= map_iter->second.name;
			strncpy(pUser->wait_gomap_name,mapname.c_str(),MAX_NAMESIZE);
			Zebra::logger->debug("�ڱ�ĳ���Ѱ��������,��ͼ����:%s",pUser->wait_gomap_name);
		}
		else
		{
			Zebra::logger->debug("�ڱ�ĳ���Ѱ��������,��ͼID:%d",cmd.map_id);
		}
		return 2;
	}
	// */
	return 0;
}

/**
 * \brief �ڵ�ͼָ�����͵��������������������
 * \param type ��������
 * \param pos ����������
 * \param orign ԭʼ����,�������������������
 * \return �Ƿ�ɹ�
 */
bool Scene::randzPosByZoneType(const int type, zPos &pos , const zPos orign)
{
	if (ZoneTypeDef::ZONE_NONE == type) {
		return randPosByRegion(_index, pos);
	}

	ZoneTypeDef *near = NULL;
	for(ZoneTypeDefVector::iterator it = zoneTypeDef.begin(); it != zoneTypeDef.end(); it++)
	{
		if (ZoneTypeDef::ZONE_NONE != ((*it).type & type))
		{
			if (NULL == near
					|| abs((*it).pos.x - orign.x) + abs((*it).pos.y - orign.y) < abs(near->pos.x - orign.x) + abs(near->pos.y - orign.y))
			{
				near = &(*it);
			}
		}
	}
	if(near)
	{
		return randPosByRegion(near->region.index, pos);
	}
	return false;
}

/**
 * \brief ��ĳλ��һ����Χ�������һ��
 * ����10�Σ�10�����Ҳ������������false
 *
 *
 * \param center ����λ��
 * \param pos �����ҵ���λ��
 * \param rectx ��Χ��
 * \param recty ��Χ��
 * \return �����Ƿ�ɹ�
 */
bool Scene::randzPosOnRect(const zPos &center , zPos &pos , WORD rectx , WORD recty) const
{
	int randx = 0;
	int randy = 0;
	int times = 10;
	while(times)
	{
		times --;
		randx = zMisc::randBetween(1 , rectx);
		randy = zMisc::randBetween(1 , recty);
		pos.x = center.x + randx - (rectx + 1)/2;
		pos.y = center.y + randy - (recty + 1)/2;
		if(pos.x < this->width() || pos.y < this->height()) 
		{
			return true;
		}
	}
	return false;
}

/**
 * \brief �����ͼ���ͷŵ�ͼ��Դ
 *
 */
void Scene::final()
{
	if(inited)
	{
		allTiles.clear();
		sceneWH.x=0;
		sceneWH.y=0;
	}
}

/**
 * \brief ��ʼ����ͼ
 * \param countryid ����id
 * \param mapid ��ͼid
 */
bool Scene::init(DWORD countryid , DWORD mapid)
{
	if(inited)
	{
		Zebra::logger->error("�ظ���ʼ��");
		return inited;
	}
	//std::string s;
	/*
	zXMLParser parser;
	parser.initStr(xmlConfig);
	xmlNodePtr root=parser.getRootNode("map");
	// */
	std::string mapname;
	std::string countryname;
	std::string filename;
	SceneManager::MapMap_iter map_iter = SceneManager::getInstance().map_info.find(mapid);
	if(map_iter == SceneManager::getInstance().map_info.end())
	{
		Zebra::logger->error("�õ���ͼ����ʧ��");
		return inited;
	}
	SceneManager::CountryMap_iter country_iter = SceneManager::getInstance().country_info.find(countryid);
	if(country_iter == SceneManager::getInstance().country_info.end())
	{
		Zebra::logger->error("�õ�������Ϣʧ��");
		return inited;
	}

	mapname = country_iter->second.name;
	mapname+= "��";
	mapname+= map_iter->second.name;
	strncpy(this->name,mapname.c_str(),MAX_NAMESIZE);
	char temp[20];
	bzero(temp , sizeof(temp));
	sprintf(temp , "%d" , country_iter->second.id);
	fileName = temp;
	fileName+= ".";
	fileName+= map_iter->second.filename;
	this->id = (country_iter->second.id << 16) + map_iter->second.id;
	this->countryID = countryid;
	this->function = map_iter->second.function;
	this->level = map_iter->second.level;
	this->exprate = map_iter->second.exprate/100.0f;
	 

	//����õ�ͼû��������
	if(map_iter->second.backto)
	{
		this->backtoMapID = (country_iter->second.id << 16) + map_iter->second.backto;
	}
	//���������������������ͼ
	if(map_iter->second.foreignbackto)
	{
		this->foreignerBacktoMapID = /*(country_iter->second.id << 16) + */map_iter->second.foreignbackto;
	}
	//�ڹ���������������ͼ
	if(map_iter->second.commoncountrybackto)
	{
		this->commonCountryBacktoMapID = /*(country_iter->second.id << 16) + */map_iter->second.commoncountrybackto;
	}
	//�޹��������������������ͼ
	if(map_iter->second.commonuserbackto)
	{
		this->commonUserBacktoMapID = /*(country_iter->second.id << 16) + */map_iter->second.commonuserbackto;
	}
	//����õ�ͼ��������
	if(map_iter->second.backtoCity)
	{
		this->backtoCityMapID = (country_iter->second.id << 16) + map_iter->second.backtoCity;
	}

	/// ��սս�������󣬹�������������
	if (map_iter->second.countrydarebackto)
	{
		this->countryDareBackToMapID = map_iter->second.countrydarebackto;
	}

	if (map_iter->second.countrydefbackto)
	{
		this->countryDefBackToMapID = map_iter->second.countrydefbackto;
	}
	if (map_iter->second.pklevel)
	{
		this->pklevel = map_iter->second.pklevel;
	}
	
	//����õ�ͼ���ǹ�սĿ�ĵ�
	if(map_iter->second.backtodare)
	{
		this->backtoDareMapID = map_iter->second.backtodare;
	}
	if(!loadMapFile())return inited;
	screenx = (sceneWH.x+SCREEN_WIDTH -1)/SCREEN_WIDTH;
	screeny = (sceneWH.y+SCREEN_HEIGHT -1)/SCREEN_HEIGHT;
	screenMax=screenx*screeny;
	setSceneWH(sceneWH, screenx, screeny, screenMax);

	//Ԥ�Ƚ�����ͼ���赲������
	{
		zPos tempPos;
		for(tempPos.x = 0; tempPos.x < sceneWH.x; tempPos.x++)
		{
			for(tempPos.y = 0; tempPos.y < sceneWH.y; tempPos.y++)
			{
				if (!checkBlock(tempPos, TILE_BLOCK))
					_index.push_back(tempPos);
			}
		}
	}

	for(ZoneTypeDefVector::iterator it = zoneTypeDef.begin(); it != zoneTypeDef.end(); it++)
	{
		ZoneTypeDef &zone = *it;
		zPosRevaluate(zone.pos);
		/*
		Zebra::logger->debug("zonedef: %u, %u, %u, %u, %u, %u",
				zone.pos.x, zone.pos.y, zone.width, zone.height, zone.type, zone.initstate);
				// */
		initRegion(zone.region, zone.pos, zone.width, zone.height);
	}

	//�ӽű��ļ���ȡ�ù���ű�id��npcid�Ķ�Ӧ
	std::map<int,int> scriptMap;
	zXMLParser xp;
	if (!xp.initFile(Zebra::global["mapdir"] + "NpcAIScript.xml"))
	{
		Zebra::logger->error("���ܶ�ȡnpc�ű��ļ� %s", (Zebra::global["mapdir"] + "NpcAIScript.xml").c_str());
	}
	xmlNodePtr infoNode = xp.getRootNode("info");
	if (infoNode)
	{
		xmlNodePtr scriptNode = xp.getChildNode(infoNode, NULL);
		while (scriptNode)
		{
			int shared;//�Ƿ���ű�
			xp.getNodePropNum(scriptNode, "shared", &shared, sizeof(shared));
			if (shared)
			{
				int sID;//�ű�id
				int nID;//npcid
				xp.getNodePropNum(scriptNode, "id", &sID, sizeof(sID));
				xp.getNodePropNum(scriptNode, "npcid", &nID, sizeof(nID));
				scriptMap[nID] = sID;
			}
			scriptNode = xp.getNextNode(scriptNode, NULL);
		}
	}

	for(NpcDefineVector::iterator it = npcDefine.begin(); it != npcDefine.end(); it++)
	{
		t_NpcDefine *define = &(*it);
		zPosRevaluate(define->pos);
		/*
		Zebra::logger->debug("define: %u, %s, %u, %u, %u, %u, %u, %u, %u",
				define->id, define->name, define->pos.x, define->pos.y,
				define->width, define->height, define->num, define->interval, define->initstate);
				// */
		initRegion(define->region, define->pos, define->width, define->height);
		
		//����Ƿ��й���ű�
		if ((0==define->scriptID)&&(scriptMap.end()!=scriptMap.find(define->id)))
		{
			define->scriptID = scriptMap[define->id];
			//Zebra::logger->debug("�� %u ���ù���ű� %u ��", define->id, define->scriptID);
		}
		
		if (!initByNpcDefine(define))
		{
			Zebra::logger->debug("��ʼ��NPCʧ��");
			return false;
		}
	}

	countryTax = 0; ///������ͼ�Ĺ���˰�ʳ�ʼ����0��
	inited=true;
	return inited;
}

/**
 * \brief ��ʼ��һ�����η�Χ���������ƶ�������
 * \param reg һ�����η�Χ
 * \param pos ���η�Χ�����ϵ�����
 * \param width ���η�Χ�Ŀ�
 * \param height ���η�Χ�ĸ�
 * \return ��ʼ���Ƿ�ɹ�
 */
void Scene::initRegion(zRegion &reg, const zPos &pos, const WORD width, const WORD height)
{
	zPos tempPos;
	reg.s = pos;
	reg.e.x = pos.x + width;
	reg.e.y = pos.y + height;
	zPosRevaluate(reg.e);
	reg.c.x = reg.s.x + (reg.e.x - reg.s.x) / 2;
	reg.c.y = reg.s.y + (reg.e.y - reg.s.y) / 2;
	for(tempPos.x = reg.s.x; tempPos.x < reg.e.x; tempPos.x++)
	{
		for(tempPos.y = reg.s.y; tempPos.y < reg.e.y; tempPos.y++)
		{
			if (!checkBlock(tempPos, TILE_BLOCK))
				reg.index.push_back(tempPos);
		}
	}
}

/**
 * \brief ��һ�����η�Χ�������ȡһ������
 * \param reg һ�����η�Χ
 * \param pos �������
 * \return ��ȡ�����Ƿ�ɹ�
 */
bool Scene::randPosByRegion(const zPosIndex &index, zPos &pos) const
{
	if (index.empty())
		return false;
	else
	{
		int r = zMisc::randBetween(0, index.size() - 1);
		int i=0;
		while (checkBlock(index[r]) && i++<5)
			r = zMisc::randBetween(0, index.size() - 1);
			
		pos = index[r];
		return true;
	}
}

/**
 * \brief ��ʼ��һ��Npc
 * \param sceneNpc ��Ҫ��ʼ����Npc
 * \param init_region Npc��ʼ���귶Χ
 */
void Scene::initNpc(SceneNpc *sceneNpc, zRegion *init_region, zPos myPos)
{
	zPos pos;
	SceneNpcManager::getMe().addSceneNpc(sceneNpc);
	if (sceneNpc->isSpecialNpc())
		SceneNpcManager::getMe().addSpecialNpc(sceneNpc);
	if (myPos.x != 0 && myPos.y !=0)
	{
		if (!refreshNpc(sceneNpc,myPos))
		{
			if(randPosByRegion(init_region == NULL ? sceneNpc->define->region.index : init_region->index, pos))
				refreshNpc(sceneNpc,pos);
			//else
			//	Zebra::logger->debug("%s ���ҿո�ʧ�� %s:(%u,%u)", sceneNpc->name, name, pos.x, pos.y);
		}
	}
	else
	{
		if(randPosByRegion(init_region == NULL ? sceneNpc->define->region.index : init_region->index, pos))
			refreshNpc(sceneNpc,pos);
		//else
		//	Zebra::logger->debug("%s ���ҿո�ʧ�� %s:(%u,%u)", sceneNpc->name, name, pos.x, pos.y);
	}
}

/**
 * \brief ˢ�µ�ͼ�ϵ�NPC����,û�о����,���������ǰ���赲
 * \param sceneNpc ��Ҫˢ�µ�Npc
 * \param pos ˢ�µ�����
 * \return �ɹ�����true,���򷵻�false
 */
bool Scene::refreshNpc(SceneNpc *sceneNpc,const zPos & pos)
{
	if (sceneNpc)
	{
		zPos oldPos=sceneNpc->getPos();
		bool newnpc=sceneNpc->hasInScene();
		if(sceneNpc->define->initstate == zSceneEntry::SceneEntry_Normal)
		{
			//��ʼ��Ϊ��ͨ״̬
			if (refresh(sceneNpc, pos))
			{
				switch (sceneNpc->npc->kind)
				{
					case NPC_TYPE_TOTEM:
					case NPC_TYPE_TRAP:
						if(!newnpc) clearBlock(oldPos);
						break;
					default:
						{
							//���ҷ��赲��ɹ�
							setBlock(pos);
							//�����ǰ�赲
							if(!newnpc) clearBlock(oldPos);
						}
						break;
				}
				sceneNpc->setState(zSceneEntry::SceneEntry_Normal);
				return true;
			}
		}
		else
		{
			//��ʼ��Ϊ����״̬
			if(refresh(sceneNpc, pos))
			{
				//�����ǰ�赲
				if(!newnpc) clearBlock(oldPos);
				return true;
			}
		}
	}
	return false;
}

/**
 * \brief �ٻ�npc
 * �ٻ��Ķ��Ƕ�̬npc����Ҫ�ֶ�ɾ��
 *
 * \param define npc����
 * \param pos Ŀ��λ��
 * \param base npc������Ϣ
 * \return �ٻ���npc������
 */
int Scene::summonNpc(const t_NpcDefine &define, const zPos &pos, zNpcB *base)
{
	zRegion init_region;
	initRegion(init_region, pos, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Zebra::logger->debug("*******%u, %s, %u, %u, %u", define.id, define.name, define.pos.x, define.pos.y, define.num);
	unsigned int count = 0;
	for(WORD i = 0; i < define.num; i++)
	{
		t_NpcDefine *pDefine = new t_NpcDefine(define);
		if (pDefine)
		{
			//Zebra::logger->debug("%u, %s, %u, %u, %u", pDefine->id, pDefine->name, pDefine->pos.x, pDefine->pos.y, pDefine->num);
			SceneNpc *sceneNpc = new SceneNpc(this, base, pDefine, SceneNpc::GANG);
			if (sceneNpc)
			{
				initNpc(sceneNpc, &init_region);
				if (sceneNpc->getState() == zSceneEntry::SceneEntry_Normal)
				{
					Cmd::stAddMapNpcMapScreenUserCmd addNpc;
					sceneNpc->full_t_MapNpcData(addNpc.data);
					sendCmdToNine(sceneNpc->getPosI(), &addNpc, sizeof(addNpc));
					Cmd::stRTMagicPosUserCmd ret;
					sceneNpc->full_stRTMagicPosUserCmd(ret);
					sendCmdToNine(sceneNpc->getPosI(), &ret,sizeof(ret));
				}
				count++;
			}
			else
			{
				Zebra::logger->fatal("%s:SceneNpc�����ڴ�ʧ��", __FUNCTION__);
				SAFE_DELETE(pDefine);
			}
		}
		else
		{
			Zebra::logger->fatal("%s:t_NpcDefine�����ڴ�ʧ��", __FUNCTION__);
		}
	}
	return count;
}

/**
 * \brief ���ݶ����ʼ�����е�Npc
 * \param pDefine Npc��������
 * \return ��ʼ���Ƿ�ɹ�
 */
bool Scene::initByNpcDefine(const t_NpcDefine *pDefine)
{
	zNpcB *base = npcbm.get(pDefine->id);

	bool retval = true;
	if (base)
	{
		for(DWORD i = 0; i < pDefine->num; i++)
		{
			SceneNpc *sceneNpc = new SceneNpc(this, base, pDefine, SceneNpc::STATIC);
			if (sceneNpc)
			{
				initNpc(sceneNpc, NULL);
				if (!pDefine->petList.empty()||!pDefine->dieList.empty())
				{
					sceneNpc->setState(zSceneEntry::SceneEntry_Death);
					clearBlock(sceneNpc->getPos());
					sceneNpc->setMoveTime(SceneTimeTick::currentTime, 10000);
				}
				/*
				switch (base->kind)
				{
					case NPC_TYPE_HUMAN:
					case NPC_TYPE_NORMAL:
					case NPC_TYPE_BBOSS:
					case NPC_TYPE_LBOSS:
					case NPC_TYPE_PBOSS:
					case NPC_TYPE_BACKBONE:
					case NPC_TYPE_GOLD:
					case NPC_TYPE_BACKBONEBUG:
					case NPC_TYPE_ABERRANCE:
					case NPC_TYPE_GUARD:
						{
							sceneNpc->setState(zSceneEntry::SceneEntry_Death);
							clearBlock(sceneNpc->getPos());
							sceneNpc->setMoveTime(SceneTimeTick::currentTime, 10000);
						}
						break;
					default:
						break;
				}
				*/
			}
			else
				retval = false;
		}
	}
	else
		retval = false;
	return retval;
}

/**
 * \brief �����ת��
 * \return �����ת���Ƿ�ɹ�
 */
bool Scene::initWayPoint(zXMLParser *parser,const xmlNodePtr node , DWORD countryid)
{
	WayPoint wp;
	if(wp.init(parser,node , countryid) && wpm.addWayPoint(wp))
	{
		for(int i=0;i<wp.pointC;i++)
			setBlock(wp.point[i], TILE_DOOR);
		return true;
	}
	return false;
}

/**
 * \brief ��ȡ��ͼ�Լ��������ļ�
 *
 * 
 * \return ��ȡ�Ƿ�ɹ�
 */
bool Scene::loadMapFile()
{
	if (!LoadMap((Zebra::global["mapdir"] + getRealFileName()+".mps").c_str(),allTiles,sceneWH.x,sceneWH.y))
	{
		Zebra::logger->error("���� %s ʧ��",(Zebra::global["mapdir"] + getRealFileName()+".mps").c_str());
		return false;
	}

	zXMLParser xml;
	if (!xml.initFile(Zebra::global["mapdir"] + getRealFileName()+".xml"))
	{
		Zebra::logger->error("���ܶ�ȡ���������ļ� %s", (Zebra::global["mapdir"] + getRealFileName()+".xml").c_str());
		return false;
	}

	xmlNodePtr root = xml.getRootNode("map");
	if (root)
	{
		xmlNodePtr node = xml.getChildNode(root, NULL);
		while(node)
		{
			//Zebra::logger->debug(":::::::::::::::::::::: %s", node->name);
			if (strcmp((char *)node->name, "zonedef") == 0)
			{
				ZoneTypeDef zone;
				xml.getNodePropNum(node, "x", &zone.pos.x, sizeof(zone.pos.x));
				xml.getNodePropNum(node, "y", &zone.pos.y, sizeof(zone.pos.y));
				xml.getNodePropNum(node, "width", &zone.width, sizeof(zone.width));
				xml.getNodePropNum(node, "height", &zone.height, sizeof(zone.height));
				xml.getNodePropNum(node, "type", &zone.type, sizeof(zone.type));
				char tempChar[32];
				bzero(tempChar, sizeof(tempChar));
				xml.getNodePropStr(node, "initstate", tempChar, sizeof(tempChar));
				if (0 == strcasecmp(tempChar, "hide"))
					zone.initstate = zSceneEntry::SceneEntry_Hide;
				else
					zone.initstate = zSceneEntry::SceneEntry_Normal;
				zone.state = zone.initstate;
				//Zebra::logger->debug("zonedef: x = %u, y = %u, width = %u, height = %u, type = %u, initstate = %u", zone.pos.x, zone.pos.y, zone.width, zone.height, zone.type, zone.initstate);
				zoneTypeDef.push_back(zone);
			}
			else if (strcmp((char *)node->name, "npc") == 0)
			{
				t_NpcDefine define;
				xml.getNodePropNum(node, "id", &define.id, sizeof(define.id));
				//xml.getNodePropStr(node, "name", &define.name, sizeof(define.name));
				xml.getNodePropNum(node, "x", &define.pos.x, sizeof(define.pos.x));
				xml.getNodePropNum(node, "y", &define.pos.y, sizeof(define.pos.y));
				xml.getNodePropNum(node, "width", &define.width, sizeof(define.width));
				xml.getNodePropNum(node, "height", &define.height, sizeof(define.height));
				xml.getNodePropNum(node, "num", &define.num, sizeof(define.num));
				xml.getNodePropNum(node, "interval", &define.interval, sizeof(define.interval));
				xml.getNodePropNum(node, "rush", &define.rushID, sizeof(define.rushID));
				xml.getNodePropNum(node, "rushrate", &define.rushRate, sizeof(define.rushRate));
				xml.getNodePropNum(node, "delay", &define.rushDelay, sizeof(define.rushDelay));
				xml.getNodePropNum(node, "script", &define.scriptID, sizeof(define.scriptID));
				//xml.getNodePropNum(node, "summonid", &define.summonID, sizeof(define.summonID));
				//xml.getNodePropNum(node, "summonnum", &define.summonNum, sizeof(define.summonNum));
				char tempChar[512];
				bzero(tempChar, sizeof(tempChar));
				xml.getNodePropStr(node, "pet", tempChar, sizeof(tempChar));
				if (strcmp(tempChar, ""))
					define.fillNpcMap(tempChar, define.petList);

				bzero(tempChar, sizeof(tempChar));
				xml.getNodePropStr(node, "summon", tempChar, sizeof(tempChar));
				if (strcmp(tempChar, ""))
					define.fillNpcMap(tempChar, define.summonList);

				bzero(tempChar, sizeof(tempChar));
				xml.getNodePropStr(node, "deathsummon", tempChar, sizeof(tempChar));
				if (strcmp(tempChar, ""))
					define.fillNpcMap(tempChar, define.deathSummonList);

				bzero(tempChar, sizeof(tempChar));
				xml.getNodePropStr(node, "die", tempChar, sizeof(tempChar));
				if (strcmp(tempChar, ""))
				{
					std::vector<std::string> vs;
					std::vector<std::string> sub_vs;
					vs.clear();
					Zebra::stringtok(vs, tempChar, ";");
					for (DWORD i=0; i<vs.size(); i++)
					{
						sub_vs.clear();
						Zebra::stringtok(sub_vs, vs[i].c_str(), "-");
						if (sub_vs.size()==3)
							define.dieList.push_back(std::make_pair(atoi(sub_vs[0].c_str()), zPos(atoi(sub_vs[1].c_str()), atoi(sub_vs[2].c_str()))));
					}
				}

				bzero(tempChar, sizeof(tempChar));
				xml.getNodePropStr(node, "initstate", tempChar, sizeof(tempChar));
				if (0 == strcasecmp(tempChar, "hide"))
					define.initstate = zSceneEntry::SceneEntry_Hide;
				else
					if (!define.petList.empty()||!define.dieList.empty())
						define.initstate = zSceneEntry::SceneEntry_Death;
					else
						define.initstate = zSceneEntry::SceneEntry_Normal;

				if (npcbm.get(define.id))
				{
					npcDefine.push_back(define);
				}
				else
				{
					if(strlen(define.name)!=0)
						Zebra::logger->warn("�������ݱ����û�����Npc %s", define.name);
				}
			}
			else if (strcmp((char *)node->name, "waypoint") == 0)
			{
				if(!initWayPoint(&xml,node,countryID))
				{
					Zebra::logger->error("������ת�����");
					return false;
				}
			}
			else if (strcmp((char *)node->name, "fixedrush") == 0)
			{
				char tempChar[32];
				bzero(tempChar, sizeof(tempChar));
				xml.getNodePropNum(node, "id", &fixedRush.id, sizeof(fixedRush.id));
				if (fixedRush.id)
				{
					fixedRush.nextTime = SceneTimeTick::currentTime.sec() + 10;

					struct tm tm;
					xml.getNodePropStr(node, "allStart", tempChar, sizeof(tempChar));
					if(strptime(tempChar,"%Y%m%d %H:%M:%S", &tm)!=NULL)
					{
						time_t t=mktime(&tm);
						if(t!=(time_t)-1) fixedRush.allStart = t;
					}
					bzero(tempChar, sizeof(tempChar));
					xml.getNodePropStr(node, "allEnd", tempChar, sizeof(tempChar));
					if(strptime(tempChar,"%Y%m%d %H:%M:%S", &tm)!=NULL)
					{
						time_t t=mktime(&tm);
						if(t!=(time_t)-1) fixedRush.allEnd = t;
					}

					bzero(tempChar, sizeof(tempChar));
					xml.getNodePropStr(node, "startTime", tempChar, sizeof(tempChar));
					strptime(tempChar,"%H:%M:%S", &fixedRush.startTime);

					bzero(tempChar, sizeof(tempChar));
					xml.getNodePropStr(node, "endTime", tempChar, sizeof(tempChar));
					strptime(tempChar,"%H:%M:%S", &fixedRush.endTime);

					xml.getNodePropNum(node, "weekDay", &fixedRush.weekDay, sizeof(fixedRush.weekDay));
					xml.getNodePropNum(node, "delay", &fixedRush.delay, sizeof(fixedRush.delay));
				}
			}
			/*
			else if (strcmp((char *)node->name, "params") == 0)
			{
				char tempChar[32];
				bzero(tempChar, sizeof(tempChar));
				xml.getNodePropNum(node, "id", &fixedRush.id, sizeof(fixedRush.id));
				if (fixedRush.id)
				{
					fixedRush.nextTime = SceneTimeTick::currentTime.sec() + 10;

					struct tm tm;
					xml.getNodePropStr(node, "start", tempChar, sizeof(tempChar));
					if(strptime(tempChar,"%Y%m%d %H:%M:%S", &tm)!=NULL)
					{
						time_t t=mktime(&tm);
						if(t!=(time_t)-1) fixedRush.startTime = t;
					}
					xml.getNodePropStr(node, "end", tempChar, sizeof(tempChar));
					if(strptime(tempChar,"%Y%m%d %H:%M:%S", &tm)!=NULL)
					{
						time_t t=mktime(&tm);
						if(t!=(time_t)-1) fixedRush.endTime = t;
					}

					xml.getNodePropNum(node, "startHour", &fixedRush.startHour, sizeof(fixedRush.startHour));
					xml.getNodePropNum(node, "endHour", &fixedRush.endHour, sizeof(fixedRush.endHour));
				}
			}
			*/
			node = xml.getNextNode(node, NULL);
		}
	}
	else
	{
		Zebra::logger->warn("���ص�ͼ����%sʧ��", (Zebra::global["mapdir"] + fileName+".xml").c_str());
		return false;
	}

	return true;
}

/**
 * \brief ����ָ���ͼ��������������ҵĻص�����
 *
 */
struct SceneSendToEveryOne : public zSceneEntryCallBack
{
	const void *cmd;		/// �����͵�ָ��
	const int len;	/// ������ָ��Ĵ�С
	SceneSendToEveryOne(const void *cmd, const int len) : cmd(cmd), len(len) {};
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ��������������
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		((SceneUser *)entry)->sendCmdToMe(cmd,len);
		return true;
	}
};

/**
 * \brief ����ָ�����ͼ�������û�
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��ĳ���
 * \param locker �����������Ƿ�Ե�ͼ�������м�������
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::sendCmdToScene(const void *pstrCmd, const int nCmdLen)
{
	/// whj 
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::Scene::t_User_ForwardMap *sendCmd=(Cmd::Scene::t_User_ForwardMap *)buf;
	constructInPlace(sendCmd);
	sendCmd->maptempid=tempid;
	sendCmd->size=nCmdLen;
	bcopy(pstrCmd,sendCmd->data,nCmdLen);
	SceneTaskManager::getInstance().broadcastCmd(sendCmd,sizeof(Cmd::Scene::t_User_ForwardMap)+nCmdLen); 
	return true;
}
/**
 * \brief ����ָ�ĳһ����Χ�������������û�
 * \param posi ������
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��ĳ���
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::sendCmdToNine(const zPosI posi, const void *pstrCmd, const int nCmdLen)
{
#if 1
	/// whj ���ط���9������
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::Scene::t_Nine_ForwardScene *sendCmd=(Cmd::Scene::t_Nine_ForwardScene *)buf;
	constructInPlace(sendCmd);
	sendCmd->maptempid=tempid;
	sendCmd->screen=posi;
	sendCmd->size=nCmdLen;
	bcopy(pstrCmd,sendCmd->data,nCmdLen);
	SceneTaskManager::getInstance().broadcastCmd(sendCmd,sizeof(Cmd::Scene::t_Nine_ForwardScene)+nCmdLen); 
	return true;
#else
	SceneSendToEveryOne sendeveryone(pstrCmd, nCmdLen);
	const zPosIVector &pv = getNineScreen(posi);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		//Zebra::logger->debug("%s: %u", __FUNCTION__, *it);
		execAllOfScreen(zSceneEntry::SceneEntry_Player,*it,sendeveryone);
	}
	return true;
#endif
}


/**
 * \brief ����ָ���ͼ��������������ҵĻص�����
 *
 */
struct SceneSendToWatchTrap : public zSceneEntryCallBack
{
	const void *cmd;		/// �����͵�ָ��
	const int len;	/// ������ָ��Ĵ�С
	SceneSendToWatchTrap(const void *cmd, const int len) : cmd(cmd), len(len) {};
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ��������������
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		if (((SceneUser *)entry)->watchTrap)((SceneUser *)entry)->sendCmdToMe(cmd,len);
		return true;
	}
};
/**
 * \brief ����ָ�ĳһ����Χ�����������о�����������������û�
 * \param posi ������
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��ĳ���
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::sendCmdToWatchTrap(const zPosI posi, const void *pstrCmd, const int nCmdLen)
{
	SceneSendToWatchTrap sendeveryone(pstrCmd, nCmdLen);
	const zPosIVector &pv = getNineScreen(posi);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		//Zebra::logger->debug("%s: %u", __FUNCTION__, *it);
		execAllOfScreen(zSceneEntry::SceneEntry_Player,*it,sendeveryone);
	}
	return true;
}

/**
 * \brief ����ָ���ͼ��������������ҵĻص�����
 *
 */
struct SceneSendToUnWatchTrap : public zSceneEntryCallBack
{
	const void *cmd;		/// �����͵�ָ��
	const int len;	/// ������ָ��Ĵ�С
	SceneSendToUnWatchTrap(const void *cmd, const int len) : cmd(cmd), len(len) {};
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ��������������
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		if (!((SceneUser *)entry)->watchTrap)((SceneUser *)entry)->sendCmdToMe(cmd,len);
		return true;
	}
};
/**
 * \brief ����ָ�ĳһ����Χ�����������о�����������������û�
 * \param posi ������
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��ĳ���
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::sendCmdToNineUnWatch(const zPosI posi, const void *pstrCmd, const int nCmdLen)
{
	SceneSendToUnWatchTrap sendeveryone(pstrCmd, nCmdLen);
	const zPosIVector &pv = getNineScreen(posi);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		//Zebra::logger->debug("%s: %u", __FUNCTION__, *it);
		execAllOfScreen(zSceneEntry::SceneEntry_Player,*it,sendeveryone);
	}
	return true;
}

/**
 * \brief ����ָ���ͼ��������������ҵĻص�����
 *
 */

struct findPosInNine : public zSceneEntryCallBack
{
	const zPos &_pos;		///ʩ������������
	zPosVector &_range;	///������б�
	findPosInNine(const zPos &pos, zPosVector &range) : _pos(pos), _range(range)
	{
		_range.reserve( 9 * SCREEN_WIDTH * SCREEN_HEIGHT);
	}
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ��������������
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		if ((abs(_pos.x - entry->getPos().x) <=SCREEN_WIDTH) && (abs(_pos.y - entry->getPos().y) <=SCREEN_HEIGHT))
		{
			_range.push_back(entry->getPos());
		}
		return true;
	}
};

/**
 * \brief ���Ҿ����ڵ����н�ɫ������
 * \param posi ������
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::findEntryPosInNine(const zPos &vpos, zPosI vposi, zPosVector &range)
{
	findPosInNine findpos(vpos, range);
	const zPosIVector &pv = getNineScreen(vposi);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		//Zebra::logger->debug("%s: %u", __FUNCTION__, *it);
		execAllOfScreen(zSceneEntry::SceneEntry_Player,*it,findpos);
	}
	return true;
}

/**
 * \brief ���Ҿ����ڵ����н�ɫ��NPC������
 * \param posi ������
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::findEntryPosInOne(const zPos &vpos, zPosI vposi, zPosVector &range)
{
	findPosInNine findpos(vpos, range);
	const zPosIVector &pv = getScreenByRange(vpos, 19);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		execAllOfScreen(zSceneEntry::SceneEntry_Player,*it,findpos);
		execAllOfScreen(zSceneEntry::SceneEntry_NPC,*it,findpos);
	}
	return true;
}

/**
 * \brief ����ָ���ͼ��������������ҵĻص�����
 *
 */
struct SendToEveryOneExceptMe : public zSceneEntryCallBack
{
	const zSceneEntry *me;
	const void *cmd;		/// �����͵�ָ��
	const int len;	/// ������ָ��Ĵ�С
	SendToEveryOneExceptMe(const zSceneEntry *pEntry, const void *cmd, const int len) : me(pEntry), cmd(cmd), len(len){};
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ��������������
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		if (entry != me) ((SceneUser *)entry)->sendCmdToMe(cmd,len);
		return true;
	}
};

/**
 * \brief ����ָ�ĳһ����Χ�������������û������˶�����
 * \param pEntry ������
 * \param posi ������
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��ĳ���
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::sendCmdToNineExceptMe(zSceneEntry *pEntry, const zPosI posi, const void *pstrCmd, const int nCmdLen)
{
#if 1
	/// whj ���ط���9������
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::Scene::t_Nine_ExceptMe_ForwardScene *sendCmd=(Cmd::Scene::t_Nine_ExceptMe_ForwardScene *)buf;
	constructInPlace(sendCmd);
	sendCmd->maptempid=tempid;
	sendCmd->screen=posi;
	sendCmd->exceptme_id=pEntry->id;
	sendCmd->size=nCmdLen;
	bcopy(pstrCmd,sendCmd->data,nCmdLen);
	SceneTaskManager::getInstance().broadcastCmd(sendCmd,sizeof(Cmd::Scene::t_Nine_ExceptMe_ForwardScene)+nCmdLen); 
	return true;
#else
	SendToEveryOneExceptMe sendeveryone(pEntry, pstrCmd, nCmdLen);
	const zPosIVector &pv = getNineScreen(posi);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		//Zebra::logger->debug("%s: %u", __FUNCTION__, *it);
		execAllOfScreen(zSceneEntry::SceneEntry_Player,*it,sendeveryone);
	}
	return true;
#endif
}

/**
 * \brief �㲥ָ�����3������5���������û�
 * \param posi ������
 * \param direct ����
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��ĳ���
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::sendCmdToDirect(const zPosI posi, const int direct, const void *pstrCmd, const int nCmdLen)
{
#if 1
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::Scene::t_Nine_dir_ForwardScene *sendCmd=(Cmd::Scene::t_Nine_dir_ForwardScene *)buf;
	constructInPlace(sendCmd);
	sendCmd->maptempid=tempid;
	sendCmd->screen=posi;
	sendCmd->dir=direct;
	sendCmd->size=nCmdLen;
	bcopy(pstrCmd,sendCmd->data,nCmdLen);
	SceneTaskManager::getInstance().broadcastCmd(sendCmd,sizeof(Cmd::Scene::t_Nine_dir_ForwardScene)+nCmdLen); 
	return true;
#else
	SceneSendToEveryOne sendeveryone(pstrCmd, nCmdLen);
	const zPosIVector &pv = getDirectScreen(posi, direct);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		//Zebra::logger->debug("%s: %u", __FUNCTION__, *it);
		execAllOfScreen(zSceneEntry::SceneEntry_Player,*it,sendeveryone);
	}
	return true;
#endif
}

/**
 * \brief �㲥ָ�����3������5���������û�
 * \param posi ������
 * \param direct ����
 * \param pstrCmd �����͵�ָ��
 * \param nCmdLen ������ָ��ĳ���
 * \return �㲥ָ���Ƿ�ɹ�
 */
bool Scene::sendCmdToReverseDirect(const zPosI posi, const int direct, const void *pstrCmd, const int nCmdLen)
{
#if 1
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::Scene::t_Nine_rdir_ForwardScene *sendCmd=(Cmd::Scene::t_Nine_rdir_ForwardScene *)buf;
	constructInPlace(sendCmd);
	sendCmd->maptempid=tempid;
	sendCmd->screen=posi;
	sendCmd->dir=direct;
	sendCmd->size=nCmdLen;
	bcopy(pstrCmd,sendCmd->data,nCmdLen);
	SceneTaskManager::getInstance().broadcastCmd(sendCmd,sizeof(Cmd::Scene::t_Nine_rdir_ForwardScene)+nCmdLen); 
	return true;
#else
	SceneSendToEveryOne sendeveryone(pstrCmd, nCmdLen);
	const zPosIVector &pv = getReverseDirectScreen(posi, direct);
	for(zPosIVector::const_iterator it = pv.begin(); it != pv.end(); it++)
	{
		//Zebra::logger->debug("%s: %u", __FUNCTION__, *it);
		execAllOfScreen(zSceneEntry::SceneEntry_Player,*it,sendeveryone);
	}
	return true;
#endif
}

/**
 * \brief �ڵ�ͼ�����Ƴ����
 * \param so ���
 */
void Scene::removeUser(SceneUser *so)
{
	if (removeSceneEntry(so))
	{
		clearBlock(so->getPos());
	}
	//SceneUserManager::getMe().removeUser(so);
}

/**
 * \brief ����Ʒ��ӵ����ϣ���֪ͨ��Χ9�������
 * \param ob ��Ʒ
 * \param pos ����
 * \param dwID ��Ʒ������
 * \return �����Ʒ�Ƿ�ɹ�
 */
bool Scene::addObject(zObject *ob,const zPos &pos,const unsigned long overdue_msecs,const unsigned long dwID, int protime)
{
	bool retval = false;
	zSceneObject *so = zSceneObject::create(ob, SceneTimeTick::currentTime);
	if(overdue_msecs)
	{
		zRTime ct = SceneTimeTick::currentTime;
		ct.addDelay(overdue_msecs);
		so->setOverDueTime(ct);
	}
	if (so)
	{
		zPos findedPos;
		zPosI posi;
		if(findPosForObject(pos, findedPos))
		{
			if (refresh(so, findedPos))
			{
				//���ñ���
				if(dwID)
					so->setOwner(dwID, protime);
				zPos2zPosI(findedPos, posi);
				stObjectLocation pos(Cmd::OBJECTCELLTYPE_NONE, 0, findedPos.x, findedPos.y);
				so->getObject()->data.pos = pos;
/*				
				so->getObject()->data.pos.dwLocation=Cmd::OBJECTCELLTYPE_COMMON;
				so->getObject()->data.pos.x = findedPos.x;
				so->getObject()->data.pos.y = findedPos.y;
*/
				Cmd::stAddMapObjectMapScreenUserCmd add;
				if(so->getObject()->base->id == 672 || so->getObject()->base->id == 673)
				{
					add.data.dwOwner = 0;
					add.action = Cmd::OBJECTACTION_UPDATE;
				}
				else
				{
					add.action = Cmd::OBJECTACTION_DROP;
					SceneUser *pUser = SceneUserManager::getMe().getUserByID(so->getOwner());
					if(pUser)
					{
						add.data.dwOwner = pUser->tempid;
					}
					else
					{
						add.data.dwOwner = 0;
					}
				}
				add.data.dwMapObjectTempID = so->getObject()->data.qwThisID;
				add.data.dwObjectID = so->getObject()->base->id;
				strncpy(add.data.pstrName, so->getObject()->data.strName, MAX_NAMESIZE);
				add.data.x = so->getPos().x;
				add.data.y = so->getPos().y;
				add.data.wdNumber = so->getObject()->data.dwNum;
				add.data.wdLevel = so->getObject()->base->level;
				add.data.upgrade = so->getObject()->data.upgrade;
				add.data.kind = so->getObject()->data.kind;				
				sendCmdToNine(posi, &add, sizeof(add));
				retval = true;
			}
			else
				clearObjectBlock(findedPos);
		}
		if (!retval)
		{
//			so->clear();
			SAFE_DELETE(so);
		}
	}
	return retval;
}

/**
 * \brief ����Ʒ��ӵ����ϣ���֪ͨ��Χ9�������
 * \param ob ��Ҫ��ӵ���Ʒ�Ļ�������
 * \param num ��Ʒ����
 * \param pos ����
 * \param dwID ��Ʒ������
 * \param npc_id  ����ʯ��Ӧ��npc id
 * \return �����Ʒ�Ƿ�ɹ�
 */
bool Scene::addObject(zObjectB *ob, const int num, const zPos &pos,const unsigned long dwID, int npc_id)
{
	bool retval = false;
	zObject *o = zObject::create(ob,num);
	if (o)
	{
		//������ƷҲ���ܲ����ǰ�ɫװ��
		//o->found(NULL,true);
		EquipMaker maker(NULL);
		maker.assign(NULL, o, o->base,true);
		int protime=10;
		if ((npc_id>=21501 && npc_id<=21511) ||
			(npc_id>=26501 && npc_id<=26511) ||
			(npc_id>=30051 && npc_id<=30102))
		{
			protime=5;
		}

		if (addObject(o, pos, 0, dwID, protime))
		{
			zObject::logger(o->createid,o->data.qwThisID,o->data.strName,o->data.dwNum,o->data.dwNum,1,0,NULL,this->id,this->name,"��������",o->base,o->data.kind,o->data.upgrade);
			retval = true;
		}
		//removed by lqy, addobject will clean this object auto
/*		
		else
			SAFE_DELETE(o);
*/
	}
	return retval;
}

/**
 * \brief ��ʱ��תȦ
 * \param side �뾶
 * \param X ��ʼ����X
 * \param Y ��ʼ����Y
 * \param CX ��������X
 * \param CY ��������Y
 */
void Scene::runCircle_anti_clockwise(
		const int side,
		const DWORD X,
		const DWORD Y,
		DWORD &CX,
		DWORD &CY) const
{
	if (CX + side == X)
	{
		//�������
		if (Y + side == CY)
		{
			//���¶���
			CX++;
		}
		else
		{
			CY++;
		}
	}
	else if (Y + side == CY)
	{
		//�±�����
		if (X + side == CX)
		{
			//���¶���
			CY--;
		}
		else
		{
			CX++;
		}
	}
	else if (X + side == CX)
	{
		//�ұ�����
		if (CY + side == Y)
		{
			//���϶���
			CX--;
		}
		else
		{
			CY--;
		}
	}
	else if (CY + side == Y)
	{
		//��������
		if (CX + side == X)
		{
			//���϶���
			CY++;
		}
		else
		{
			CX--;
		}
	}
	else
		Zebra::logger->trace("Scene::runCircle_anti_clockwise(): ����Ĳ��� side=%u, X=%u Y=%u CX=%u CY=%u", side, X, Y, CX, CY);
}

/**
 * \brief ˳ʱ��תȦ
 * \param side �뾶
 * \param X ��ʼ����X
 * \param Y ��ʼ����Y
 * \param CX ��������X
 * \param CY ��������Y
 */
void Scene::runCircle_clockwise(
		const int side,
		const DWORD X,
		const DWORD Y,
		DWORD &CX,
		DWORD &CY) const
{
	if (CX + side == X)
	{
		//�������
		if (CY + side == Y)
		{
			//���϶���
			CX++;
		}
		else
		{
			CY--;
		}
	}
	else if (Y + side == CY)
	{
		//�±�����
		if (CX + side == X)
		{
			//���¶���
			CY--;
		}
		else
		{
			CX--;
		}
	}
	else if (X + side == CX)
	{
		//�ұ�����
		if (Y + side == CY)
		{
			//���¶���
			CX--;
		}
		else
		{
			CY++;
		}
	}
	else if (CY + side == Y)
	{
		//��������
		if (CX + side == X)
		{
			//���϶���
			CY++;
		}
		else
		{
			CX++;
		}
	}
	else
		Zebra::logger->trace("Scene::runCircle_clockwise(): ����Ĳ��� side=%u, X=%u Y=%u CX=%u CY=%u", side, X, Y, CX, CY);
}

/**
 * \brief �ҳ�һ������ָ�������ϵ�����λ��
 * \param dir ����
 * \param orgPos ��ʼ����
 * \return �ҵ�������
 */
void Scene::getNextPos(const zPos &orgPos, const int dir, zPos &newPos) const
{
	newPos = orgPos;
	switch (dir)
	{
		case 0:
			newPos.y = newPos.y>1?newPos.y-1:0;
			break;
		case 1:
			newPos.x = newPos.x+1>sceneWH.x?sceneWH.x:newPos.x+1;
			newPos.y = newPos.y>1?newPos.y-1:0;
			break;
		case 2:
			newPos.x = newPos.x+1>sceneWH.x?sceneWH.x:newPos.x+1;
			break;
		case 3:
			newPos.x = newPos.x+1>sceneWH.x?sceneWH.x:newPos.x+1;
			newPos.y = newPos.y+1>sceneWH.y?sceneWH.y:newPos.y+1;
			break;
		case 4:
			newPos.y = newPos.y+1>sceneWH.y?sceneWH.y:newPos.y+1;
			break;
		case 5:
			newPos.x = newPos.x>1?newPos.x-1:0;
			newPos.y = newPos.y+1>sceneWH.y?sceneWH.y:newPos.y+1;
			break;
		case 6:
			newPos.x = newPos.x>1?newPos.x-1:0;
			break;
		case 7:
			newPos.x = newPos.x>1?newPos.x-1:0;
			newPos.y = newPos.y>1?newPos.y-1:0;
			break;
		default:
			break;
	}
}

/**
 * \brief ��sideΪ�뾶����orgPosΪԭ�㣬�ҵ�ͼλ��
 * \param side �뾶
 * \param direct ����
 * \param orgPos ��ʼ����
 * \param clockwise ˳ʱ�뻹����ʱ��
 * \param crtPos �õ�������
 * \return �����Ƿ�ɹ�
 */
bool Scene::getNextPos(
		int &side,
		const int direct,
		const zPos &orgPos,
		const int clockwise,
		zPos &crtPos) const
{
	const int walk_adjust[][2]= { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, 0} };

	//������ĵ��Ƿ񳬳���ͼ�߽�
	if (!zPosValidate(orgPos))
	{
		crtPos.x = 0;
		crtPos.y = 0;
		++side; //added by lqy, fix infinite loop bug, see  Scene::findPosForUser please!
		return false;
	}

	do {
		//���յ�
		if (side == 0)
		{
			side++;
			crtPos.x = orgPos.x + walk_adjust[direct][0] * side;
			crtPos.y = orgPos.y + walk_adjust[direct][1] * side;
		}
		else
		{
			//תȦ
			if (clockwise == 1)
			{
				//��ʱ��
				runCircle_anti_clockwise(side, orgPos.x, orgPos.y, crtPos.x, crtPos.y);
			}
			else if (clockwise == -1)
			{
				//˳ʱ��
				runCircle_clockwise(side, orgPos.x, orgPos.y, crtPos.x, crtPos.y);
			}
			else
			{
				Zebra::logger->trace("Scene::getNextPos(): ����Ĳ��� clockwise=%u", clockwise);

				//����˳ʱ�봦��
				runCircle_clockwise(side, orgPos.x, orgPos.y, crtPos.x, crtPos.y);
			}
			if ((crtPos.x == orgPos.x + walk_adjust[direct][0] * side
						&& crtPos.y == orgPos.y + walk_adjust[direct][1] * side))
			{
				//�ư뾶һ�����
				side++;
				crtPos.x = orgPos.x + walk_adjust[direct][0] * side;
				crtPos.y = orgPos.y + walk_adjust[direct][1] * side;
			}
		}

		//�߽���
		if ((crtPos.x & 0x80000000) == 0 
				&& (crtPos.y & 0x80000000) == 0
				&& zPosValidate(crtPos))
		{
			break;
		}
		else
		{
			//���������߽�ĵ�
			Zebra::logger->error("���ĵ�(%u, %u), �����߽��(%u, %u)", orgPos.x, orgPos.y, crtPos.x, crtPos.y);
		}
	} while(true);

	return true;
}

/**
 * \brief ���µ�ͼ��Ʒ�Ļص�����
 *
 */
struct UpdateSceneObjectCallBack : public zSceneEntryCallBack
{
	/**
	 * \brief ����ָ��
	 *
	 */
	Scene *scene;
#ifdef _POOL_ALLOC_	
	typedef std::set<zSceneObject *, std::less<zSceneObject *>, __gnu_cxx::__pool_alloc<zSceneObject *> > set;
#else
	typedef std::set<zSceneObject *, std::less<zSceneObject *> > set;
#endif

	set delObject;
	/**
	 * \brief ���캯��
	 * \param scene ����
	 */
	UpdateSceneObjectCallBack(Scene *scene) : scene(scene) {};
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ���������ȫ���ǵ�ͼ��Ʒ
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		zSceneObject *so = (zSceneObject *)entry;
		zObject *o = so->getObject();
		if (NULL == o)
		{
			delObject.insert(so);
			return true;
		}
		if(o->base->id != 673 && o->base->id != 674)
		{
			if(so->checkProtectOverdue(SceneTimeTick::currentTime))
			{
				Cmd::stClearObjectOwnerMapScreenUserCmd  ret;
				ret.dwMapObjectTempID=so->id;
				scene->sendCmdToNine(so->getPosI(),&ret,sizeof(ret));
			}
		}
#ifndef _DEBUGLOG
		if (so->checkOverdue(SceneTimeTick::currentTime))
#endif
		{
			delObject.insert(so);
		}

		return true;
	}
	void clearObject()
	{
		Cmd::stRemoveMapObjectMapScreenUserCmd re;
		for(set::iterator it = delObject.begin(); it != delObject.end(); ++it)
		{
			zSceneObject *so = *it;
			zObject *o = so->getObject();
			if (o)
				zObject::logger(o->createid,o->data.qwThisID,o->data.strName,o->data.dwNum,o->data.dwNum,0,0,NULL,0,NULL,"��������",NULL,0,0);
			re.dwMapObjectTempID=so->id;
			scene->sendCmdToNine(so->getPosI(),&re,sizeof(re));

			//ʵ��ɾ����Ʒ
			scene->removeObject(so);
			SAFE_DELETE(so);
		}
	}
};

/**
 * \brief ���µ�ͼ�������Ʒ�����������Ʒ�������ԡ���Ʒ��ʧ��
 *
 */
void Scene::updateSceneObject()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	UpdateSceneObjectCallBack usocb(this);

	execAllOfScene(zSceneEntry::SceneEntry_Object, usocb);
	usocb.clearObject();
}

/**
 * \brief ɾ����ͼ��Ʒ�Ļص�����
 */
struct RemoveSceneObjectCallBack : public zSceneEntryCallBack
{
	Scene *scene;
#ifdef _POOL_ALLOC_	
	typedef std::set<zSceneObject *, std::less<zSceneObject *>, __gnu_cxx::__pool_alloc<zSceneObject *> > set;
#else
	typedef std::set<zSceneObject *, std::less<zSceneObject *> > set;
#endif	
	set delObject;
	/**
	 * \brief ���캯��
	 */
	RemoveSceneObjectCallBack(Scene *scene) : scene(scene) {};
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ���������ȫ���ǵ�ͼ��Ʒ
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		zSceneObject *so = (zSceneObject *)entry;
		delObject.insert(so);
		return true;
	}
	void clearObject()
	{
		Cmd::stRemoveMapObjectMapScreenUserCmd re;
		for(set::iterator it = delObject.begin(); it != delObject.end(); ++it)
		{
			zSceneObject *so = *it;
			zObject *o = so->getObject();
			if (o)
				zObject::logger(o->createid,o->data.qwThisID,o->data.strName,o->data.dwNum,o->data.dwNum,0,0,NULL,0,NULL,"��������",NULL,0,0);
			re.dwMapObjectTempID=so->id;
			scene->sendCmdToNine(so->getPosI(),&re,sizeof(re));

			//ʵ��ɾ����Ʒ
			scene->removeObject(so);
			SAFE_DELETE(so);
		}
	}
};

/**
 * \brief ж�ص�ͼʱж�����е�ͼ�ϵ���Ʒ
 *
 */
void Scene::removeSceneObjectInOneScene()
{
	RemoveSceneObjectCallBack usocb(this);

	execAllOfScene(zSceneEntry::SceneEntry_Object, usocb);
	usocb.clearObject();
}

/**
 * \brief ���澲̬����
 *
 *
 */
bool StaticScene::save()
{
	return true;
}

/////////////////////////////////////
// 	����̳�
/////////////////////////////////////
/**
 * \brief ���캯��
 *
 */
StaticScene::StaticScene():Scene()
{
}

/**
 * \brief ��������
 *
 *
 */
StaticScene::~StaticScene()
{
}

/**
 * \brief ���涯̬����
 *
 *
 */
bool GangScene::save()
{
	//TODO  ���浽���ݿ�
	return true;
}

/**
 * \brief ���캯��
 *
 *
 */
GangScene::GangScene():Scene()
{
}

/**
 * \brief ��������
 *
 *
 */
GangScene::~GangScene()
{
	save();
}

/**
 * \brief ���������һ�����Ƕ���
 *
 *
 * \param rush ����ָ��
 * \return �Ƿ���ӳɹ�
 */
bool Scene::addRush(Rush *rush)
{
	rushList.push_back(rush);
	//Zebra::logger->debug("���Rush , ���� %s �� %u ��Rush", name, rushList.size());
	return true;
}

/**
 * \brief ������鹥���Ƿ������������ɾ���ö���
 *
 *
 */
void Scene::processRush()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	std::list<Rush *> del;
	for (std::list<Rush *>::iterator it=rushList.begin(); it!=rushList.end(); it++)
	{
		(*it)->process();
		if ((*it)->isEnd())
		{
			del.push_back((*it));
			Zebra::logger->debug("%s ɾ��Rush %s", name, (*it)->rushName);
		}
	}
	if (!del.empty())
	{
		for (std::list<Rush *>::iterator it=del.begin(); it!=del.end(); it++)
		{
			rushList.remove(*it);
			Rush *ru = *it;
			SAFE_DELETE(ru);
		}
		//Zebra::logger->debug("ɾ��Rush , ���� %s �� %u ��Rush", name, rushList.size());
	}

	DWORD sec = SceneTimeTick::currentTime.sec();
	//�̶�ˢ��
	if (fixedRush.id)
	{
		if (sec+30>=fixedRush.nextTime && sec<fixedRush.nextTime)
		{
			if (!rushList.empty())
			{
				Zebra::logger->debug("%s ������й��﹥��", name);
				for (std::list<Rush *>::iterator it=rushList.begin(); it!=rushList.end(); it++)
					(*it)->terminate();
			}
		}
		else if (sec>=fixedRush.nextTime || rushList.empty())
		{
			if (fixedRush.allStart && sec<fixedRush.allStart) return;
			if (fixedRush.allEnd && sec>fixedRush.allEnd) return;
			struct tm tv1;
			time_t timValue = time(NULL);
			zRTime::getLocalTime(tv1, timValue);
			if (fixedRush.weekDay && tv1.tm_wday!=fixedRush.weekDay) return;
			if (fixedRush.startTime.tm_hour)
			{
				if (tv1.tm_hour<fixedRush.startTime.tm_hour) return;
				if (tv1.tm_hour==fixedRush.startTime.tm_hour && tv1.tm_min<fixedRush.startTime.tm_min) return;
			}
			if (fixedRush.endTime.tm_hour)
			{
				if (tv1.tm_hour>fixedRush.endTime.tm_hour) return;
				if (tv1.tm_hour==fixedRush.endTime.tm_hour && tv1.tm_min>fixedRush.endTime.tm_min) return;
			}

			Rush * rush = new Rush(fixedRush.id, fixedRush.delay, countryID);
			if (rush)
			{
				if (rush->init(this))
				{
					fixedRush.nextTime = sec;
					fixedRush.nextTime += rush->rushDelay+rush->lasttime+30;
					Zebra::logger->error("%s ��ʼ�����﹥�� id=%u countryID=%u delay=%u", name, fixedRush.id, countryID, fixedRush.delay);
					return;
				}
				else
					Zebra::logger->error("%s ��ʼ�����﹥��ʧ�� id=%u countryID=%u delay=%u", name, fixedRush.id, countryID, fixedRush.delay);

				SAFE_DELETE(rush);
				return;
			}
		}
	}
	/*

	DWORD sec = SceneTimeTick::currentTime.sec();
	//�̶�ˢ��
	if (nextRushTime && sec>=1145592000 && sec<=1146024000)
	{
		if (sec+30>=fixedRush.nextTime && sec<fixedRush.nextTime)
		{
			if (!rushList.empty())
			{
				Zebra::logger->debug("%s ������й��﹥��", name);
				for (std::list<Rush *>::iterator it=rushList.begin(); it!=rushList.end(); it++)
					(*it)->terminate();
			}
		}
		else if (sec>=fixedRush.nextTime || rushList.empty())
		{
			if (fixedRush.startTime && sec<fixedRush.startTime) return;
			if (fixedRush.endTime && sec>fixedRush.endTime) return;
			struct tm tv1;
			time_t timValue = time(NULL);
			zRTime::getLocalTime(tv1, timValue);
			if (fixedRush.startHour && tv1.tm_hour<fixedRush.startHour) return;
			if (fixedRush.endHour && tv1.tm_hour>fixedRush.endHour) return;

			Rush * rush = new Rush(fixedRush.id, 0, countryID);
			if (rush)
			{
				if (rush->init(this))
				{
					fixedRush.nextTime = sec;
					fixedRush.nextTime += rush->lasttime+30;
					return;
				}
				else
					Zebra::logger->error("%s ��ʼ�����﹥��ʧ�� id=%u countryID=%u", name, atoi(params["fixedRush"].c_str()), countryID);

				SAFE_DELETE(rush);
				return;
			}
		}
	}
	*/
}

bool Scene::checkUserLevel(SceneUser *pUser)
{
	if (!pUser) return true;
	if (this->level >0)
	{
		if (pUser->charbase.level < this->level)
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "%s���Եȼ�����%d������ҿ��ţ�",this->name, this->level);
			Zebra::logger->trace("[GOMAP]���%s�ȼ�����������ͼ[%s]ʧ��!" , pUser->name, this->name);
			return false;
		}
	}
	return true;
}


struct SecGenCallback : public zSceneEntryCallBack
{
	SecGenCallback(){}
	/**
	 * \brief �ص�����
	 * \param entry ��ͼ���������ȫ����npc
	 * \return �ص��Ƿ�ɹ�
	 */
	bool exec(zSceneEntry *entry)
	{
		SceneNpc *npc = (SceneNpc *)entry;
		if (npc->getState()==zSceneEntry::SceneEntry_Death)
		{
			npc->reliveTime = SceneTimeTick::currentTime;

			Zebra::logger->debug("%s ��ս��������������ӳ�(%u,%u)", npc->scene->name, npc->getPos().x, npc->getPos().y);
		}
		return true;
	}
};

void Scene::reliveSecGen()
{
	SecGenCallback c;
	execAllOfScene_npc(COUNTRY_SEC_GEN, c);
}

