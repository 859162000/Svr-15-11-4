/**
 * \file
 * \version  $Id: zDatabaseManager.h  $
 * \author  
 * \date 
 * \brief 游戏基本数据管理器 声明
 */

#ifndef _ZDATABASEMANAGER_H_
#define _ZDATABASEMANAGER_H_
#include "zDatabase.h"
#include "zEntryManager.h"
#include "zRWLock.h"
#include "Zebra.h"

template <class data>
class zDatabaseCallBack
{
	public:
		virtual bool exec(data *entry)=0;
		virtual ~zDatabaseCallBack(){};
};
typedef zEntryManager<zEntryID,zMultiEntryName> zDataManager;
template <class data,class datafile>
class  zDataBM:public zDataManager
{

	private:
		static zDataBM<data,datafile> *me;
		zRWLock rwlock;

		zDataBM()
		{
		}

		class deleteEvery:public zDatabaseCallBack<data>
		{
			bool exec(data *entry)
			{
				delete entry;
				return true;
			}
		};

		~zDataBM()
		{
			deleteEvery temp;
			execAll(temp);
			rwlock.wrlock();
			clear();
			rwlock.unlock();
		}

		zEntry * getEntryByID( DWORD id)
		{
			zEntry * ret=NULL;
			zEntryID::find(id,ret);
			return ret;
		}

		void removeEntryByID(DWORD id)
		{
			zEntry * ret=NULL;
			if(zEntryID::find(id,ret))
				removeEntry(ret);
		}

		zEntry * getEntryByName( const char * name)
		{
			zEntry * ret=NULL;
			zMultiEntryName::find(name,ret,true);
			return ret;
		}

		void removeEntryByName(const char * name)
		{
			zEntry * ret=NULL;
			if(zMultiEntryName::find(name,ret))
				removeEntry(ret);
		}

		bool refresh(datafile &base)
		{
			data *o=(data *)getEntryByID(base.getUniqueID());
			if(o==NULL)
			{
				o=new data();
				if(o==NULL)
				{
					Zebra::logger->fatal("无法分配内存");
					return false;
				}
				o->fill(base);
				if(!zDataManager::addEntry(o))
				{
					Zebra::logger->fatal("添加Entry错误(%ld)(id=%ld, name=%s)" , base.dwField0,o->id, o->name);
					SAFE_DELETE(o);
					return false;
				}
			}
			else
			{
				o->fill(base);
				//重新调整名字hash中的位置，这样即使名称改变也可以查询到
				zMultiEntryName::remove((zEntry * &)o);
				zMultiEntryName::push((zEntry * &)o);
			}
			return true;
		}

	public:
		static zDataBM & getMe()
		{
			if(me==NULL)
				me=new zDataBM();
			return *me;
		}

		static void delMe()
		{
			SAFE_DELETE(me);
		}

		bool refresh(const char *filename)
		{
			FILE* fp = fopen(filename,"rb");
			bool ret=false;
			if(fp)
			{
				DWORD size;
				datafile ob;
				bzero(&ob,sizeof(ob));
				if (fread(&size,sizeof(size),1,fp)==1)
				{
					rwlock.wrlock();
					for(DWORD i =0;i<size;i++)
					{
						if(fread(&ob,sizeof(ob),1,fp)==1)
						{
							refresh(ob);
							bzero(&ob,sizeof(ob));
						}
						else
						{
							Zebra::logger->error("读到未知大小结构，文件%s可能损坏",filename);
							break;
						}
						if(feof(fp)) break;
					}
					rwlock.unlock();
					ret=true;
				}
				else
				{
					Zebra::logger->error("读取记录个数失败");
				}
				fclose(fp);
			}
			else
			{
				Zebra::logger->error("打开文件失败 %s",filename);
			}
			if(ret)
				Zebra::logger->info("刷新基本表成功 %s", filename);
			else
				Zebra::logger->error("刷新基本表失败 %s", filename);
			return ret;
		}

		data *get(DWORD dataid)
		{
			rwlock.rdlock();
			data *ret=(data *)getEntryByID(dataid);
			rwlock.unlock();
			return ret;
		}

		data *get(const char *name)
		{
			rwlock.rdlock();
			data *ret=(data *)getEntryByName(name);
			rwlock.unlock();
			return ret;
		}

		void execAll(zDatabaseCallBack<data> &base)
		{
			rwlock.rdlock();
			for(zEntryID::hashmap::iterator it=zEntryID::ets.begin();it!=zEntryID::ets.end();it++)
			{
				if(!base.exec((data *)it->second))
				{
					rwlock.unlock();
					return;
				}
			}
			rwlock.unlock();
		}

		void listAll()
		{
			class listevery:public zDatabaseCallBack<data>
			{
				public:
					int i;
					listevery()
					{
						i=0;
					}
					bool exec(data *zEntry)
					{
						i++;
						Zebra::logger->debug("%ld\t%s",zEntry->id,zEntry->name);
						return true;
					}
			};
			listevery le;
			execAll(le);
			Zebra::logger->debug("Total %d",le.i);
		}
};

extern zDataBM<zObjectB,ObjectBase> &objectbm;
extern zDataBM<zBlueObjectB,BlueObjectBase> &blueobjectbm;
extern zDataBM<zGoldObjectB,GoldObjectBase> &goldobjectbm;
extern zDataBM<zDropGoldObjectB,DropGoldObjectBase> &dropgoldobjectbm;
extern zDataBM<zSetObjectB,SetObjectBase> &setobjectbm;
extern zDataBM<zFiveSetB, FiveSetBase> &fivesetbm;
extern zDataBM<zHolyObjectB,HolyObjectBase> &holyobjectbm;
extern zDataBM<zUpgradeObjectB,UpgradeObjectBase> &upgradeobjectbm;
extern zDataBM<zNpcB,NpcBase> &npcbm;
//extern zDataBM<zCharacterB , CharacterBase> &characterbm;
extern zDataBM<zExperienceB , ExperienceBase> &experiencebm;
extern zDataBM<zHonorB , HonorBase> &honorbm;
extern zDataBM<zSkillB , SkillBase> &skillbm;
extern zDataBM<zLiveSkillB , LiveSkillBase> &liveskillbm;
extern zDataBM<zSoulStoneB , SoulStoneBase> &soulstonebm;
extern zDataBM<zHairStyleB , HairStyle> &hairstylebm;
extern zDataBM<zHairColourB , HairColour> &haircolourbm;
extern zDataBM<zCountryMaterialB, CountryMaterial> &countrymaterialbm;
extern zDataBM<zHeadListB , HeadList> &headlistbm;
extern zDataBM<zPetB , PetBase> &petbm;

extern bool loadAllBM();
extern void unloadAllBM();

#endif
