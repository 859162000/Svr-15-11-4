#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Scene.h"
#include "zSceneManager.h"
#include "zUniqueID.h"

/**
 * \brief ��ÿ��sceneִ�еĻص�����
 *
 */
struct SceneCallBack
{
	virtual bool exec(Scene *scene)=0;
	virtual ~SceneCallBack(){};
};

/**
 * \brief ����������
 *
 */
class SceneManager:public zSceneManager
{
	public:
		typedef std::vector<std::pair<DWORD,DWORD> > NewZoneVec;
		typedef NewZoneVec::iterator newZoneVec_iter;
	private:
		///Ψһʵ��
		static SceneManager * sm;
		///ΨһID������
		zUniqueDWORDID *sceneUniqeID;
		///�Ƿ��ѳ�ʼ��
		bool inited;

		SceneManager();
		~SceneManager();

		bool getUniqeID(DWORD &tempid);
		void putUniqeID(const DWORD &tempid);
		bool newzone;
		NewZoneVec newzon_vec; 

	public:
		static SceneManager &getInstance();
		static void delInstance();
		bool init();
		void final();

		NewZoneVec &queryNewZonePos();
		bool isNewZoneConfig();
		bool randzPosNewZone(Scene *intoScene,zPos &findedPos);
		void addNewZonePos(DWORD x,DWORD y); 
		void setNewZoneConfig(bool type);
		Scene * getSceneByFileName( const char * name);
		Scene * getSceneByName( const char * name);
		Scene * getSceneByTempID( DWORD tempid);
		Scene * getSceneByID( DWORD id);
		Scene * loadScene(Scene::SceneType type,DWORD countryid  , DWORD mapid);
		void unloadScene(std::string &name);
		void unloadScene(Scene * &scene);
		void unloadAllScene();

		void freshEverySceneField();
		void execEveryScene(SceneCallBack &callback);
		//void execEveryMap(MapCallBack &callback);
		template <class YourEntry>
		void execEveryMap(execEntry<YourEntry> &callback)
		{
			SceneManager::MapMap_iter map_iter = SceneManager::getInstance().map_info.begin();
			for(; map_iter != SceneManager::getInstance().map_info.end() ; map_iter ++)
			{
				callback.exec(&map_iter->second);
			}
		}
		/*
		template <class YourEntry>
		void execEveryCountryMap(DWORD country_id , execEntry<YourEntry> &exec)
		{
			for(CountryMap_iter iter = country_info.begin() ; iter != country_info.end() ; iter ++)
			{
				MapMap_value_type iter_map = map_info.find(iter->second.mapid);
				if(iter_map != map_info.end())
				{
					exec.exec(iter_map->second);
				}
			}
		}
		// */
		void checkUnloadOneScene();

		/**
		 * \brief ������Ϣ
		 *
		 */
		struct CountryInfo
		{
			///���
			DWORD id;
			///����
			char name[MAX_NAMESIZE];
			///���ڵĵ�ͼ����
			DWORD mapid;
			///���ҹ��ܱ�ʶ�ֶ�
			DWORD function;
		};
		/**
		 * \brief ��ͼ��Ϣ
		 *
		 */
		struct MapInfo
		{
			///���
			DWORD id;
			///����
			char name[MAX_NAMESIZE];
			///��Ӧ�ĵ�ͼ�ļ���
			char filename[MAX_NAMESIZE];
			///����ڸõ�ͼ����ص��ĵ�ͼ
			DWORD backto;
			///����ڸõ�ͼ����ص��ĳ���
			DWORD backtoCity;
			///����������ͼ����ص��ĳ���
			DWORD foreignbackto;
			///����ڹ�������ͼ����ص��ĳ���
			DWORD commoncountrybackto;
			///�޹������������ͼ����ص��ĳ���
			DWORD commonuserbackto;
			///��սĿ�ĵ�
			DWORD backtodare;
			///��ս�ڼ䣬�ڹ�սս����Ŀǰ�����ǣ������󣬹����������
			DWORD countrydarebackto;
			///��ս�ڼ䣬�ڹ�սս����Ŀǰ�����ǣ�������, �ط��������
			DWORD countrydefbackto;
			///���໥pk�ĵȼ�
			DWORD pklevel;
			///��ʾ�õ�ͼĳЩ�����Ƿ���ã������
			DWORD function;
			///��ʾ�õ�ͼ�����level�������ҽ���
			BYTE level;
			///��ʾ�õ�ͼ�ľ���ӳ�(ʹ��ʱ/100�������)
			BYTE exprate;
		};
		typedef std::map<DWORD , CountryInfo> CountryMap;
		typedef CountryMap::iterator CountryMap_iter;
		typedef CountryMap::value_type CountryMap_value_type;
		///������Ϣ��ID��ӳ��
		CountryMap country_info;
		DWORD getCountryIDByCountryName(const char *name);
		const char * getCountryNameByCountryID(DWORD);
		DWORD getMapIDByMapName(const char *name);
		const char *getMapMapNameByMapID(DWORD mapid);
		DWORD buildMapID(DWORD countryid , DWORD mapid);
		bool buildMapName(DWORD countryid , const char *in ,char *out);
		bool buildMapName(DWORD countryid, DWORD mapid, char *out);
		typedef std::map<DWORD , MapInfo> MapMap;
		typedef MapMap::value_type MapMap_value_type;
		typedef MapMap::iterator MapMap_iter;
		///��ͼ��Ϣ��ID��ӳ��
		MapMap map_info;
};
#endif
