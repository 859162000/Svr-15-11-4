/**
 * \file
 * \version  $Id: Scene.h  $
 * \author  
 * \date 
 * \brief �����ͼ����
 *
 * 
 */

#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <map>
#include <list>
#include <ext/hash_map>

#include "zScene.h"
#include "LoadMap.h"
#include "zSceneEntryIndex.h"
#include "zXMLParser.h"
#include "SceneUserManager.h"
#include "SceneCommand.h"
#include "Command.h"
#include "SceneDefine.h"
#include "SceneNpc.h"
#include "SceneNpcManager.h"
#include "zTime.h"
#include "WayPoint.h"
#include "Rush.h"

const DWORD NANJIAO_MAP_ID = 135;
const DWORD DONGJIAO_MAP_ID = 136;
const DWORD BIANJING_MAP_ID = 137;
const DWORD WANGCHENG_MAP_ID = 139;
const DWORD PK_MAP_ID = 213;

/**
 * \brief ��ͼ����
 */
class Scene:public zScene, public zSceneEntryIndex
{

	protected:

		Scene();

	public:

		//��Session����ˢ�µ�npc
		std::map<DWORD, SceneNpc *> bossMap;

		/**
		 * \brief �������Ͷ���
		 */
		enum SceneType
		{
			STATIC,		/// ��̬��ͼ
			GANG		/// ��̬��ͼ
		};

		virtual ~Scene();
		virtual bool save() =0;

		bool init(DWORD countryid , DWORD mapid);
		void final();
		void freshGateScreenIndex(SceneUser *pUser , const DWORD screen);

		bool sendCmdToWatchTrap(const zPosI screen, const void *pstrCmd, const int nCmdLen);
		bool sendCmdToNine(const zPosI screen, const void *pstrCmd, const int nCmdLen);
		bool sendCmdToNineUnWatch(const zPosI screen, const void *pstrCmd, const int nCmdLen);
		bool sendCmdToScene(const void *pstrCmd, const int nCmdLen);
		bool sendCmdToNineExceptMe(zSceneEntry *pEntry, const zPosI screen, const void *pstrCmd, const int nCmdLen);
		bool sendCmdToDirect(const zPosI screen, const int direct, const void *pstrCmd, const int nCmdLen);
		bool sendCmdToReverseDirect(const zPosI screen, const int direct, const void *pstrCmd, const int nCmdLen);
		bool findEntryPosInNine(const zPos &vpos, zPosI vposi, zPosVector &range);
		bool findEntryPosInOne(const zPos &vpos, zPosI vposi, zPosVector &range);

		int summonNpc(const t_NpcDefine &define, const zPos &pos, zNpcB *base);
		template <typename Npc>
		Npc* summonOneNpc(const t_NpcDefine &define, const zPos &pos, zNpcB *base, DWORD standTime=0, zNpcB* abase = NULL, BYTE vdir=4);

		bool refreshNpc(SceneNpc *sceneNpc,const zPos & newPos);

		/**
		 * \brief ��ȡ������ͼ
		 * \return ������ͼID
		 */
		DWORD backtoMap() const { return backtoMapID; }
		/**
		 * \brief ��ȡ�سǵ�ͼ
		 * \return �سǵ�ͼID
		 */
		DWORD backtoCityMap() const { return backtoCityMapID; }
		/**
		 * \brief �����໥pk�ĵȼ�
		 * \return ��true
		 */
		DWORD getPkLevel() const { return pklevel; }
		/**
		 * \brief ���õ�ͼ�Ƿ��������
		 * \return ������ʱtrue
		 */
		bool canRide() const { return !(function & 0x1); }
		/**
		 * \brief ����ʹ�þ���
		 * \return �����Ƿ���true
		 */
		bool canUserScroll() const { return !(function & 0x8); }
		/**
		 * \brief ���õ�ͼ�Ƿ�������
		 * \return ������ʱtrue
		 */
		bool noTeam() const { return (function & 0x40); }
		/**
		 * \brief ���õ�ͼ�Ƿ����ʹ������
		 * \return �Ƿ����
		 */
		bool checkCallObj() const { return (function & 0x80); }
		/**
		 * \brief �Ƿ�������
		 * \return �����Ƿ���true
		 */
		bool isMainCity() const { return (function & 0x2); }
		/**
		 * \brief �Ƿ��ǵض���
		 * \return �����Ƿ���true
		 */
		bool isField() const { return (function & 0x4); }
		/**
		 * \brief �Ƿ�����ֵ��ͼ
		 * \return �����Ƿ���true
		 */
		bool isIncScene() const { return (function & 0x20); }
		/**
		 * \brief �Ƿ��ǲ�������ͼ
		 * \return ��true
		 */
		bool isNoRedScene() const { return (function & 0x10); }
		/**
		 * \brief �Ƿ���PK��ͼ
		 * \return ��true
		 */
		bool isPkMap() const { return getRealMapID()>=213 && getRealMapID()<=215; }
		/**
		 * \brief ��ȡ��ͼ���
		 * \return ��ͼ���
		 */
		const DWORD getRealMapID() const { return id & 0x0000FFFF; }
		/**
		 * \brief ��ȡ��ͼ��������
		 * \return ��ͼ��������
		 */
		const DWORD getCountryID() const { return countryID; }

		const char *getCountryName() const;

		/**
		 * \brief ��ȡ��ͼ����
		 * \return ��ͼ����(���ڷ����)
		 */
		const char *getName() const { return name; }
		/**
		 * \brief ��ȡ��ͼ����
		 * \return ��ͼ����(δ������ϵ�����)
		 */
		const char *getRealName() const
		{
			const char *real = strstr(name , "��");
			if( real != NULL)
				return real + 2;
			else
				return name;
		}
		/**
		 * \brief ��ȡ��ͼ�ļ�����
		 * ���Ʋ�����ǰ׺
		 * \return ��ͼ�ļ�����(���ڷ����)
		 */
		const char *getFileName() const { return fileName.c_str(); }
		/**
		 * \brief ��ȡ��ͼ�ļ�����
		 * ���Ʋ�����ǰ׺
		 * \return ��ͼ�ļ�����(Ϊ������ϵ�����)
		 */
		const char *getRealFileName() const { return fileName.c_str() + fileName.find(".") + 1; }
		/**
		 * \brief ��ȡָ����ͼ�ļ�����
		 * ���Ʋ�����ǰ׺
		 * \param file �ļ���
		 * \return ��ͼ�ļ�����(Ϊ������ϵ�����)
		 */
		const char *getRealFileName(std::string file) const { return file.c_str() + file.find(".") + 1; }
		/**
		 * \brief ��������赲��Ϣ
		 * \param pos ����
		 * \param block �赲���
		 * \return �Ƿ��赲��
		 */
		const bool checkBlock(const zPos &pos, const BYTE block) const
		{
			if (zPosValidate(pos))
				return (0 != (allTiles[pos.y * width() + pos.x].flags & block));
			else
				return true;
		}
		/**
		 * \brief ��������赲��Ϣ
		 * \param pos ����
		 * \return �Ƿ��赲��
		 */
		const bool checkBlock(const zPos &pos) const { return checkBlock(pos, TILE_BLOCK | TILE_ENTRY_BLOCK); }
		/**
		 * \brief ����Ŀ���赲����
		 * \param pos ����
		 * \param block �赲���
		 */
		void setBlock(const zPos &pos, const BYTE block)
		{
			if (zPosValidate(pos))
				allTiles[pos.y * width() + pos.x].flags |= block;
		}
		/**
		 * \brief ����Ŀ���赲����
		 * \param pos ����
		 */
		void setBlock(const zPos &pos) { setBlock(pos, TILE_ENTRY_BLOCK); }
		/**
		 * \brief ���Ŀ���赲����
		 * \param pos ����
		 * \param block �赲���
		 */
		void clearBlock(const zPos &pos, const BYTE block)
		{
			if (zPosValidate(pos))
				allTiles[pos.y * width() + pos.x].flags &= ~block;
		}
		/**
		 * \brief ���Ŀ���赲����
		 * \param pos ����
		 */
		void clearBlock(const zPos &pos) { clearBlock(pos, TILE_ENTRY_BLOCK); }
		/**
		 * \brief ��������赲��Ϣ
		 * ��Ҫ�ڶ���Ʒ��ʱ��ʹ��
		 * \param pos ����
		 * \return �Ƿ��赲��
		 */
		const bool checkObjectBlock(const zPos &pos) const { return checkBlock(pos, TILE_BLOCK | TILE_OBJECT_BLOCK); }
		/**
		 * \brief ����Ŀ���赲����
		 * ��Ҫ�ڶ���Ʒ��ʱ��ʹ��
		 * \param pos ����
		 */
		void setObjectBlock(const zPos &pos) { setBlock(pos, TILE_OBJECT_BLOCK); }
		/**
		 * \brief ���Ŀ���赲����
		 * ��Ҫ�ڶ���Ʒ��ʱ��ʹ��
		 * \param pos ����
		 */
		void clearObjectBlock(const zPos &pos) { clearBlock(pos, TILE_OBJECT_BLOCK); }
		/**
		 * \brief ��ȡ�ر�����
		 * \param pos ����
		 * \return ���صر�����
		 */
		const Tile* getTile(const zPos &pos) const
		{
			if (zPosValidate(pos))
				return &allTiles[pos.y * width() + pos.x];
			else
				return NULL;
		}
		/**
		 * \brief ����λ�õõ�·��
		 * \param pos Ҫ���ҵ�λ��
		 * \return �ҵ���·�㣬ʧ�ܷ���0
		 */
		const WayPoint *getWayPoint(const zPos &pos) const { return wpm.getWayPoint(pos); }
		/**
		 * \brief ����Ŀ���ҵ�·��
		 * \param filename Ŀ���ͼ�ļ���
		 * \return �ҵ���·�㣬ʧ�ܷ���0
		 */
		const WayPoint *getWayPoint(const char *filename) const { return wpm.getWayPoint(filename); }
		/**
		 * \brief ���ѡһ��·��
		 * \return �ҵ���·��
		 */
		const WayPoint *getRandWayPoint() const { return wpm.getRandWayPoint(); }
		/**
		 * \brief ���ص�ͼ�ϵ�����
		 * \return һ�ŵ�ͼ�ϵ�����
		 */
		const DWORD countUser() const { return userCount; }
		/**
		 * \brief ��������
		 * \return ���Ӻ������
		 */
		const DWORD addUserCount() { return ++userCount; }
		/**
		 * \brief ��������
		 * \return ���ٺ������
		 */
		const DWORD subUserCount() { return --userCount; }

		bool findPosForObject(const zPos &pos, zPos &findedPos);
		bool findPosForUser(const zPos &pos, zPos &findedPos);
		SceneUser *getSceneUserByPos(const zPos &pos, const bool bState = true, const zSceneEntry::SceneEntryState byState = zSceneEntry::SceneEntry_Normal);
		SceneUser *getUserByID(DWORD userid);
		SceneUser *getUserByTempID(DWORD usertempid);
		SceneUser *getUserByName(const char *username);
		SceneNpc *getSceneNpcByPos(const zPos &pos, const bool bState = true, const zSceneEntry::SceneEntryState byState = zSceneEntry::SceneEntry_Normal);
		SceneNpc *getNpcByTempID(DWORD npctempid);
		zSceneObject *getSceneObjectByPos(const zPos &pos);
		bool addObject(zObject *ob, const zPos &pos, const unsigned long overdue_msecs=0,const unsigned long dwID=0, int protime=10);
		bool addObject(zObjectB *ob, const int num, const zPos &pos,const unsigned long dwID=0, int npc_id = 0);
		void removeUser(SceneUser *so);
		void removeObject(zSceneObject *so);
		void removeNpc(SceneNpc *sn);
		bool checkZoneType(const zPos &pos, const int type) const;
		int getZoneType(const zPos &pos) const;
		bool randzPosByZoneType(const int type, zPos &pos) const;
		int changeMap(SceneUser *pUser , bool deathBackto=true, bool ignoreWar=false);
		bool randzPosByZoneType(const int type, zPos &pos , const zPos orign);
		bool randzPosOnRect(const zPos &center , zPos &pos , WORD rectx = SCREEN_WIDTH , WORD recty = SCREEN_HEIGHT) const;

		bool getNextPos(int &side, const int direct, const zPos &orgPos, const int clockwise, zPos &crtPos) const;
		void getNextPos(const zPos &orgPos, const int dir, zPos &newPos) const;
		bool SceneEntryAction(const zRTime& ctv, const DWORD group);
		void removeSceneObjectInOneScene();

		bool addRush(Rush *);
		void processRush();

		void setUnionDare(bool flag){this->isUnionDare = flag;}
		bool getUnionDare() const { return this->isUnionDare; }
		void setHoldUnion(DWORD dwUnionID) { this->dwHoldUnionID = dwUnionID; }
		DWORD getHoldUnion() const { return this->dwHoldUnionID; }
		void setHoldCountry(DWORD dwCountryID) { this->dwHoldCountryID = dwCountryID; }
		DWORD getHoldCountry() const { return this->dwHoldCountryID; }
		DWORD getCommonCountryBacktoMapID() const { return this->commonCountryBacktoMapID; }
		DWORD getForeignerBacktoMapID() const { return this->foreignerBacktoMapID; }
		DWORD getCountryDareBackToMapID() const { return this->countryDareBackToMapID; }
		DWORD getCountryDefBackToMapID() const { return this->countryDefBackToMapID; }
		DWORD getCommonUserBacktoMapID() const { return this->commonUserBacktoMapID; }
		void setCountryDare(bool flag) { this->isCountryFormalDare = flag; }
		bool getCountryDare() const { return this->isCountryFormalDare; }
		void setEmperorDare(bool flag, DWORD dwDefCountryID) { this->isEmperorDare = flag; this->dwEmperorDareDef = dwDefCountryID; }
		void reliveSecGen();
		bool getEmperorDare() const { return this->isEmperorDare; }
		DWORD getEmperorDareDef() const { return this->dwEmperorDareDef; }
		void setTax(DWORD dwTax) { countryTax = dwTax; }
		const DWORD getTax() const { return countryTax; }
		void addFieldMapName(const char *name)
		{
			stMapName mapname;
			strncpy(mapname.strMapName,name,MAX_NAMESIZE);
			fieldMapName.push_back(mapname);
		}
		void clearFieldMapName() { fieldMapName.clear(); }
		DWORD getField(const char *ou) const
		{
			stMapName *out = (stMapName *)ou; 
			int i=0;
			for(std::vector<stMapName>::const_iterator iter = fieldMapName.begin();iter!=fieldMapName.end();iter++)
			{
				strncpy((&out[i])->strMapName , (*iter).strMapName,MAX_NAMESIZE);
				i++;
			}
			return i;
		}
		bool checkField(const char *out) const
		{
			for(std::vector<stMapName>::const_iterator iter = fieldMapName.begin();iter!=fieldMapName.end();iter++)
			{
				if (strncmp((char *)out, (*iter).strMapName,MAX_NAMESIZE) == 0)
				{
					return true;
				}
			}
			return false;
		}
		void addMainMapName(const char *name)
		{
			stMapName mapname;
			strncpy(mapname.strMapName,name,MAX_NAMESIZE);
			mainMapName.push_back(mapname);
		}

		void clearMainMapName() { mainMapName.clear(); }
		DWORD getMainCity(const char *ou) const
		{
			stMapName *out = (stMapName *)ou; 
			int i=0;
			for(std::vector<stMapName>::const_iterator iter = mainMapName.begin();iter!=mainMapName.end();iter++)
			{
				strncpy((&out[i])->strMapName , (*iter).strMapName,MAX_NAMESIZE);
				i++;
			}
			return i;
		}
		bool checkMainCity(const char *out) const
		{
			for(std::vector<stMapName>::const_iterator iter = mainMapName.begin();iter!=mainMapName.end();iter++)
			{
				if (strncmp((char *)out, (*iter).strMapName,MAX_NAMESIZE) == 0)
				{
					return true;
				}
			}
			return false;
		}

		void addIncMapName(const char *name)
		{
			stMapName mapname;
			strncpy(mapname.strMapName,name,MAX_NAMESIZE);
			incMapName.push_back(mapname);
		}

		void clearIncMapName() { incMapName.clear(); }
		DWORD getIncCity(const char *ou) const
		{
			stMapName *out = (stMapName *)ou; 
			int i=0;
			for(std::vector<stMapName>::const_iterator iter = incMapName.begin();iter!=incMapName.end();iter++)
			{
				strncpy((&out[i])->strMapName , (*iter).strMapName,MAX_NAMESIZE);
				i++;
			}
			return i;
		}
		bool checkIncCity(const char *out) const
		{
			for(std::vector<stMapName>::const_iterator iter = incMapName.begin();iter!=incMapName.end();iter++)
			{
				if (strncmp((char *)out, (*iter).strMapName,MAX_NAMESIZE) == 0)
				{
					return true;
				}
			}
			return false;
		}


		DWORD sceneExp(DWORD exp) const { return (DWORD)(exp * exprate); }
		DWORD winnerExp(DWORD exp) const
		{
			if(winner_exp)
			{
				return (DWORD)(exp * 0.5f);
			}
			else
			{
				return 0;
			}
		}
		BYTE getLevel() const { return level; }
		bool checkUserLevel(SceneUser *pUser);
		///սʤ������ӳɱ�־ 
		bool winner_exp; 

		/// ��սս�������󣬹������������
		DWORD countryDareBackToMapID;


		/**
		 * \brief �Ƿ����շѵ�ͼ
		 * \return �Ƿ����շѵ�ͼ
		 */
		bool isTrainingMap()
		{
			DWORD i = id & 0x0000FFFF;
			return (i>=193 && i<=202);
		}
		void initRegion(zRegion &reg, const zPos &pos, const WORD width, const WORD height);

		bool randPosByRegion(const zPosIndex &index, zPos &pos) const;
	private:
		struct stMapName
		{
			stMapName()
			{
				bzero(strMapName,sizeof(strMapName));
			}
			char strMapName[MAX_NAMESIZE];
		};
		//�������ͼ
		std::vector<stMapName> fieldMapName;
		//���ǵ�ͼ
		std::vector<stMapName> mainMapName;
		//��ֵ��ͼ
		std::vector<stMapName> incMapName;
		std::map<std::string, std::string> params;

		///��npc���鴦��
		DWORD execGroup;

		/// �����ս���б�־
		bool isUnionDare;
		
		/// �ó����������
		DWORD dwHoldUnionID;

		/// �ó���ռ���߹���ID
		DWORD dwHoldCountryID;


		/// ��ʽ��ս���ڸó������еı�־
		bool isCountryFormalDare;

		/// �ʳ�ս���ڸó������еı�־
		bool isEmperorDare;
		
		/// �ʳ�ս���ط�
		DWORD dwEmperorDareDef;

		///�������Ĺ����б�
		std::list<Rush *> rushList;

		///һ�붨ʱ��
		Timer _one_sec;

		///�Ѿ���ʼ��
		bool inited;

		///���еĵ�ͼ����
		zTiles allTiles;
		///���е�npc����
		NpcDefineVector npcDefine;
		///�����и�������Ķ���
		ZoneTypeDefVector zoneTypeDef;

		///�������û���
		DWORD userCount;
		///�������ͼû��������ʱ��Ҫ��ת���ĵ�ͼ
		DWORD backtoMapID;
		///�ص����ǵĵ�ͼid
		DWORD backtoCityMapID;
		///�ص���սĿ�ĵ�
		DWORD backtoDareMapID;
		/// �����������Ӧ�ûص��ĵ�ͼid(û�й�����Ϣ)
		DWORD foreignerBacktoMapID;
		/// �ڹ�����������Ӧ�ûص��ĵ�ͼid(û�й�����Ϣ)
		DWORD commonCountryBacktoMapID; 
		/// �޹��������������������
		DWORD commonUserBacktoMapID; 
		/// ��սս���������ط����������
		DWORD countryDefBackToMapID;
		///��ͼ����˵��
		DWORD function;
		/// ���໥pk�ĵȼ�
		DWORD pklevel;
		///����id
		DWORD countryID;
		///������Ӧ���ļ���
		std::string fileName;
		///·�������
		WayPointM wpm;
		///����ͼ��ȡ˰��
		DWORD countryTax;
		///����ͼ����������С��ҵȼ�
		BYTE level;
		///������ͼ�ӳ�
		float exprate;

		struct FixedRush
		{
			DWORD id;//ID
			DWORD nextTime;//�´ε�ʱ��
			DWORD allStart;//�ܿ�ʼʱ��
			DWORD allEnd;//�ܽ���ʱ��
			int weekDay;//���ڼ�
			tm startTime;//һ���п�ʼ��ʱ��
			tm endTime;//һ���н�����ʱ��
			DWORD delay;//��ʼ�ӳ�

			FixedRush()
			{
				id = 0;//ID
				nextTime = 0;//�´ε�ʱ��
				allStart = 0;//�ܿ�ʼʱ��
				allEnd = 0;//�ܽ���ʱ��
				weekDay = 0;//���ڼ�
				delay = 0;//��ʼ�ӳ�
			}
		} fixedRush;

		bool initWayPoint(zXMLParser *parser,const xmlNodePtr node , DWORD countryid);
		bool loadMapFile();
		void initNpc(SceneNpc *sceneNpc, zRegion *init_region, zPos myPos=zPos(0,0));
		bool initByNpcDefine(const t_NpcDefine *pDefine);

		void runCircle_anti_clockwise(const int side, const DWORD X, const DWORD Y, DWORD &CX, DWORD &CY) const;
		void runCircle_clockwise(const int side, const DWORD X, const DWORD Y, DWORD &CX, DWORD &CY) const;

		void updateSceneObject();

		zPosIndex _index;	/// ���赲������
};

/**
 * \brief �ٻ�һ��npc
 * \param define npc����ṹ
 * \param pos �ٻ�λ��
 * \param base npc������Ϣ
 * \param standTime ͼ��ϵ�ĳ���ʱ��
 * \param abase ��ǿnpc�Ļ�����Ϣ
 * \return �ٻ���npc��ָ�룬ʧ�ܷ���0
 */
	template <typename Npc>
Npc* Scene::summonOneNpc(const t_NpcDefine &define, const zPos &pos, zNpcB *base, DWORD standTime, zNpcB* abase, BYTE vdir)
{
	t_NpcDefine *pDefine = new t_NpcDefine(define);
	if (pDefine)
	{
		Npc *sceneNpc = new Npc(this, base, pDefine, SceneNpc::GANG, zSceneEntry::SceneEntry_NPC, abase);
		if (sceneNpc)
		{
			sceneNpc->setDir(vdir);
			sceneNpc->setStandingTime(standTime);
			initNpc(sceneNpc, NULL, pos);//zPos(0,0));//��NULL����define.region��Χ��ѡ��λ��
			if (sceneNpc->getState() == zSceneEntry::SceneEntry_Normal)
			{
				if (base->kind != NPC_TYPE_TRAP)
				{
					Cmd::stAddMapNpcMapScreenUserCmd addNpc;
					sceneNpc->full_t_MapNpcData(addNpc.data);
					sendCmdToNine(sceneNpc->getPosI(), &addNpc, sizeof(addNpc));
					Cmd::stRTMagicPosUserCmd ret;
					sceneNpc->full_stRTMagicPosUserCmd(ret);
					sendCmdToNine(sceneNpc->getPosI(), &ret,sizeof(ret));
				}
				else
				{
					SceneEntryPk *entry = sceneNpc->getMaster();
					if (entry&& entry->getType() == zSceneEntry::SceneEntry_Player)
					{
						SceneUser *pUser = (SceneUser *)entry;
						Cmd::stAddMapNpcMapScreenUserCmd addNpc;
						sceneNpc->full_t_MapNpcData(addNpc.data);
						pUser->sendCmdToMe(&addNpc, sizeof(addNpc));
						Cmd::stRTMagicPosUserCmd ret;
						sceneNpc->full_stRTMagicPosUserCmd(ret);
						pUser->sendCmdToMe(&ret,sizeof(ret));
					}
				}
			}
#ifdef _XWL_DEBUG
			else
				Zebra::logger->debug("%s ��ʼ״̬ %u",sceneNpc->name, sceneNpc->getState());
#endif
			return sceneNpc;
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
	return NULL;
}		

/**
 * \brief ��̬����
 *
 */
class StaticScene:public Scene
{
	public:
		StaticScene();
		~StaticScene();
		bool save();
};

/**
 * \brief ��̬����
 *
 */
class GangScene:public Scene
{
	public:
		GangScene();
		~GangScene();
		bool save();
};

#endif
