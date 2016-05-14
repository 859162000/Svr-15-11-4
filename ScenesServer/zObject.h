#ifndef _ZOBJECT_H_
#define _ZOBJECT_H_
#include <vector>
#include <set>
#include "zDatabaseManager.h"
#include "Command.h"
#include "zSceneEntry.h"
#include "zTime.h"

//#define BINARY_VERSION 	0
//#define BINARY_VERSION 	20051018
//#define BINARY_VERSION	20051225
//#define BINARY_VERSION 		20060124
#define BINARY_VERSION 		20060313

struct ZlibObject
{
	DWORD 	version;
	DWORD	count;
	unsigned char data[0];
	ZlibObject()
	{
		count = 0;
		version = 0;
	}
};

struct SaveObject
{
	union
	{
		struct 
		{
			DWORD createtime;
			DWORD dwCreateThisID;	
		};
		unsigned long long createid;
	};
	// */
	t_Object object;
};
class GlobalObjectIndex;
class SceneUser;

namespace luabind
{
	namespace detail {
		template<class T>
		struct delete_s;
	}
}

struct zObject:zEntry
{
	friend class GlobalObjectIndex;
	public:
		t_Object data;
		zObjectB *base;

		//only for RebuildObject::make, it's ugly, but .....sigh
		WORD* _p1[5];
		WORD* _p2[18];
		
		static zObject *create(zObjectB *objbase, unsigned int num=1, BYTE level = 0);
		static void destroy(zObject* ob);
		static void logger(QWORD createid,DWORD objid,char *objname,DWORD num,DWORD change,DWORD type,DWORD srcid,char *srcname,DWORD dstid,char *dstname,const char *action, zObjectB *base,BYTE kind,BYTE upgrade);
		static DWORD RepairMoney2RepairGold(DWORD dwObjectRepair);
		static zObject *create(zObject *objsrc);
		static zObject *load(const SaveObject *o);
		bool getSaveData(SaveObject *save);
		int foundSocket();
		bool canMail();
		void checkBind();
		
		zCountryMaterialB* canContribute(); // �Ƿ��ܱ�����
		DWORD getMaterialKind(); // ���ԭ�����: 0,��ͨ���ʣ�1,˿��,2,��ʯ,3,���,4,ľ�ģ�5,Ƥë,6,��ҩ

		const stObjectLocation &reserve() const;
		void restore(const stObjectLocation &loc);
			
		union
		{
			struct 
			{
				DWORD createtime;
				DWORD dwCreateThisID;	
			};
			unsigned long long createid;
		};
		// */
	private:
		friend class Package;
		friend class Packages;
		friend class MainPack;
		friend class zSceneObject;
		friend class luabind::detail::delete_s<zObject>;

		zObject();
		~zObject();

		bool free() const;
		void free(bool flag);

		void fill(t_Object& d);		
		void generateThisID();
		bool inserted;
};

struct zSceneObject:public zSceneEntry
{
	static zSceneObject *create(zObject *obj, const zRTime &ct)
	{
		if(obj==NULL) return NULL;
		zSceneObject *ret=new zSceneObject(obj,ct);
		return ret;
	}
	/**
	 * \brief ��������Ʒ�Ƿ����
	 * ���ڵĵ�����Ʒ����ʧ��
	 * \param ct ��ǰʱ��
	 * \return �Ƿ����
	 */
	bool checkOverdue(const zRTime &ct) const
	{
		return ct >= this->ct;
	}

	bool checkProtectOverdue(const zRTime &ct)
	{
		if(ownerID == 0)
		{
			return false;
		}
		if(ct > protectTime)
		{
			protectTime = 0;
			ownerID = 0;
			return true;
		}
		return false;
	}

	void setOwner(DWORD dwID, int protime=10)
	{
		if(dwID)
		{
			ownerID = dwID;
			zRTime ctv;
			protectTime = ctv;
			this->protectTime.addDelay(protime * 1000);
		}
	}

	void setOverDueTime(zRTime &ct_1)
	{
		ct = ct_1;
	}
	DWORD getOwner()
	{
		return ownerID;
	}

	~zSceneObject()
	{
		if (o) o->free();
		zObject::destroy(o);
	}

	void clear()
	{
		o=NULL;
	}

	zObject *getObject()
	{
		return o;
	}

	private:
	DWORD ownerID;
	zRTime protectTime;
	zObject *o;
	zRTime ct;
	zSceneObject(zObject *obj, const zRTime &ct):zSceneEntry(SceneEntry_Object),ct(ct)
	{
		this->ct.addDelay(600 * 1000);
		o=obj;
		ownerID=0;
		id = obj->data.qwThisID;
		tempid = obj->base->id;
		strncpy(name,obj->data.strName,MAX_NAMESIZE);
	}
};

#define DECLARE(prop, type) type get_##prop() const {return prop;}
#define RESET(prop) prop = 0;
class Equips 
{
public:
	Equips()
	{
		reset();
	}
	
	void reset()
	{
		RESET(maxhp)					// �������ֵ
		RESET(maxmp)					// �����ֵ
		RESET(maxsp)					// �������ֵ
	
		RESET(pdamage)					// ��С������
		RESET(maxpdamage)				// ��󹥻���
		RESET(mdamage)					// ��С����������
		RESET(maxmdamage)				// �����������
		RESET(appendminpet)           // ������ǿ��Сֵ
		RESET(appendmaxpet)				// ������ǿ���ֵ
	
		RESET(pdefence)				// ���
		RESET(mdefence)				// ħ��
		RESET(damagebonus)			// �˺��ӳ�
		RESET(damage)				// �����˺�ֵx��
		
		RESET(akspeed)				// �����ٶ�
		RESET(mvspeed)				// �ƶ��ٶ�
		RESET(atrating)				// ������
		RESET(akdodge)				// �����
	
		RESET(str)				  	// ����
		RESET(inte)  					// ����
		RESET(dex)  					// ����
		RESET(spi)  					// ����
		RESET(con)  					// ����
		
		RESET(hpr)					// ����ֵ�ָ�
		RESET(mpr)  					// ����ֵ�ָ�
		RESET(spr)  					// ����ֵ�ָ�
		
		RESET(holy)						//��ʥһ��
		RESET(bang)						//�ػ���
		RESET(pdam) 					// ������������
		RESET(pdef)  					// �������������
		RESET(mdam)  					// ����ħ��������
		RESET(mdef)  					// ����ħ��������
		
		RESET(poisondef) 				//��������
		RESET(lulldef) 				//���������
		RESET(reeldef) 				//��ѣ������
		RESET(evildef) 				//����ħ����
		RESET(bitedef) 				//����������
		RESET(chaosdef) 				//����������
		RESET(colddef) 				//����������
		RESET(petrifydef) 			//��ʯ������
		RESET(blinddef) 				//��ʧ������
		RESET(stabledef) 				//����������
		RESET(slowdef)				 //����������
		RESET(luredef) 				//���ջ�����
		
		RESET(poison) 				//�ж�����
		RESET(lull) 					//�������
		RESET(reel) 					//ѣ������
		RESET(evil) 					//��ħ����
		RESET(bite)					//��������
		RESET(chaos) 					//��������
		RESET(cold) 					//��������
		RESET(petrify) 				//ʯ������
		RESET(blind) 					//ʧ������
		RESET(stable) 				//��������
		RESET(slow) 					//��������
		RESET(lure) 					//�ջ�����
		RESET(hpleech.odds) 
		RESET(hpleech.effect) 
		RESET(mpleech.odds) 			//x%��������ֵy
		RESET(mpleech.effect) 		//x%���շ���ֵy

		RESET(hptomp)					//ת������ֵΪ����ֵx��
		RESET(dhpp) 					//�����˺�����x%	
		RESET(dmpp)					//�����˺�ֵ����x%		
	
		RESET(incgold)				//���ӽ�Ǯ����x%
		RESET(doublexp)				//x%˫������		
		RESET(mf)						 //���ӵ�����x%

		//������װ�������
		RESET(dpdam)			//�����˺�����%x
		RESET(dmdam)			//�����˺�����%x
		RESET(bdam)				//�����˺�x%
		RESET(rdam)				//�˺�����%x
		RESET(ignoredef)		//%x����Ŀ�����
		
//		RESET(aftype)
		aftype = FIVE_NONE;
		RESET(afpoint)
//		RESET(dftype)
		dftype = FIVE_NONE;
		RESET(dfpoint)
		RESET(maxhprate)
		RESET(maxmprate)
		
		skill.clear();
		skills.clear();
	}
	
	DECLARE( maxhp, WORD )				// �������ֵ
	DECLARE( maxmp, WORD )				// �����ֵ
	DECLARE( maxsp, WORD )				// �������ֵ

	DECLARE( pdamage, WORD )			// ��С������
	DECLARE( maxpdamage, WORD )		// ��󹥻���
	DECLARE( mdamage, WORD )			// ��С����������
	DECLARE( maxmdamage, WORD )		// �����������
	DECLARE( appendminpet, WORD )  // ��С������ǿ
	DECLARE( appendmaxpet, WORD )  // ��������ǿ

	DECLARE( pdefence, WORD )			// ���
	DECLARE( mdefence, WORD )			// ħ��
	DECLARE( damagebonus, BYTE )		// �˺��ӳ�
	DECLARE( damage, BYTE )				// �����˺�ֵx��
	
	DECLARE( akspeed, WORD )			// �����ٶ�
	DECLARE( mvspeed, WORD )			// �ƶ��ٶ�
	DECLARE( atrating, WORD )			// ������
	DECLARE( akdodge, WORD )			// �����

	DECLARE( str, WORD )  				// ����
	DECLARE( inte, WORD ) 				// ����
	DECLARE( dex, WORD ) 				// ����
	DECLARE( spi, WORD )  				// ����
	DECLARE( con, WORD )  				// ����
	
	DECLARE( hpr, WORD )  				// ����ֵ�ָ�
	DECLARE( mpr, WORD )  				// ����ֵ�ָ�
	DECLARE( spr, WORD )  				// ����ֵ�ָ�
	
	DECLARE( holy, WORD )				//��ʥһ��
	DECLARE( bang, WORD )				//�ػ���
	DECLARE( pdam, WORD )  				// ������������
	DECLARE( pdef, WORD )  				// �������������
	DECLARE( mdam, WORD )  				// ����ħ��������
	DECLARE( mdef, WORD )  				// ����ħ��������
	
	DECLARE( poisondef, WORD ) 			//��������
	DECLARE( lulldef, WORD ) 			//���������
	DECLARE( reeldef, WORD ) 			//��ѣ������
	DECLARE( evildef, WORD ) 			//����ħ����
	DECLARE( bitedef, WORD ) 			//����������
	DECLARE( chaosdef, WORD ) 			//����������
	DECLARE( colddef, WORD ) 			//����������
	DECLARE( petrifydef, WORD ) 		//��ʯ������
	DECLARE( blinddef, WORD ) 			//��ʧ������
	DECLARE( stabledef, WORD ) 			//����������
	DECLARE( slowdef, WORD ) 			//����������
	DECLARE( luredef, WORD ) 			//���ջ�����
		
	DECLARE( poison, WORD ) 			//�ж�����
	DECLARE( lull, WORD ) 				//�������
	DECLARE( reel, WORD ) 				//ѣ������
	DECLARE( evil, WORD ) 				//��ħ����
	DECLARE( bite, WORD ) 				//��������
	DECLARE( chaos, WORD ) 				//��������
	DECLARE( cold, WORD ) 				//��������
	DECLARE( petrify, WORD ) 			//ʯ������
	DECLARE( blind, WORD ) 				//ʧ������
	DECLARE( stable, WORD ) 			//��������
	DECLARE( slow, WORD ) 				//��������
	DECLARE( lure, WORD ) 				//�ջ�����

	DECLARE( hptomp, BYTE ) 			//ת������ֵΪ����ֵx��
	DECLARE( dhpp, BYTE ) 				//�����˺�����x%	
	DECLARE( dmpp, BYTE ) 				//�����˺�ֵ����x%		

	DECLARE( incgold, BYTE ) 			//���ӽ�Ǯ����x%
	DECLARE( doublexp, BYTE ) 			//x%˫������		
	DECLARE( mf, BYTE ) 				//���ӵ�����x%

	//������װ�������
	DECLARE(dpdam, BYTE)			//�����˺�����%x
	DECLARE(dmdam, BYTE)			//�����˺�����%x
	DECLARE(bdam, BYTE)				//�����˺�x%
	DECLARE(rdam, BYTE)				//�˺�����%x
	DECLARE(ignoredef, BYTE)		//%x����Ŀ�����
	DECLARE(maxhprate, BYTE)		//���ٷֱ��������hp
	DECLARE(maxmprate, BYTE)		//���ٷֱ��������mp

	BYTE get_hpleech_odds()		const	{return hpleech.odds;}
	WORD get_hpleech_effect()	const	{return hpleech.effect;}
	BYTE get_mpleech_odds()		const	{return mpleech.odds;} 			//x%��������ֵy
	WORD get_mpleech_effect()	const	{return mpleech.effect;} 		//x%���շ���ֵy

	/**
	 * \brief ��ȡ��������
	 * \return ������������
	 */
	DWORD getAttFive() const {return aftype;}

	/**
	 * \brief ��ȡ�������е���
	 * \return ���е���
	 */
	DWORD getAttFivePoint() const {return afpoint;}

	/**
	 * \brief ��ȡ��������
	 * \return ������������
	 */
	DWORD getDefFive() const {return dftype;}

	/**
	 * \brief ��ȡ�������е���
	 * \return ���е���
	 */
	DWORD getDefFivePoint() const {return dfpoint;}
	
	/**
	 * \brief ��ȡװ���Ե������ܵļӳ�
	 *
	 * \param id: ����id
	 * \return �ӳɵȼ�
	 */	
	DWORD getMaxSkill(WORD id) const
	{
		const_iterator it = skill.find(id);
		if (it != skill.end()) {
			return it->second;
		}
		
		return 0;		
	}
	
	/**
	 * \brief ��ȡװ����ĳϵ���ܵļӳ�
	 *
	 * \param id: ����ϵ��
	 * \return �ӳɵȼ�
	 */	
	DWORD getMaxSkills(WORD id) const
	{
		const_iterator it = skills.find(id);
		if (it != skills.end()) {
			return it->second;
		}
		
		return 0;		
	}

private:
	friend class EquipPack;
	WORD maxhp;					// �������ֵ
	WORD maxmp;					// �����ֵ
	WORD maxsp;					// �������ֵ

	WORD pdamage;				// ��С������
	WORD maxpdamage;			// ��󹥻���
	WORD mdamage;				// ��С����������
	WORD maxmdamage;			// �����������
	WORD appendminpet;         // �����ٻ���������ǿ��Сֵ
	WORD appendmaxpet;         // �����ٻ���������ǿ���ֵ

	WORD pdefence;				// ���
	WORD mdefence;				// ħ��
	BYTE damagebonus;			// �˺��ӳ�
	BYTE damage;				// �����˺�ֵx��
	
	WORD akspeed;				// �����ٶ�
	WORD mvspeed;				// �ƶ��ٶ�
	WORD atrating;				// ������
	WORD akdodge;				// �����

	WORD str;  // ����
	WORD inte;  // ����
	WORD dex;  // ����
	WORD spi;  // ����
	WORD con;  // ����
	
	
//			WORD fivetype;  // ��������
//			WORD fivepoint; // ��������
	
	WORD hpr;  // ����ֵ�ָ�
	WORD mpr;  // ����ֵ�ָ�
	WORD spr;  // ����ֵ�ָ�
	
	WORD holy;  //��ʥһ��	
	WORD bang;	//�ػ���
	WORD pdam;  // ������������
	WORD pdef;  // �������������
	WORD mdam;  // ����ħ��������
	WORD mdef;  // ����ħ��������
	
	WORD poisondef; //��������
	WORD lulldef; //���������
	WORD reeldef; //��ѣ������
	WORD evildef; //����ħ����
	WORD bitedef; //����������
	WORD chaosdef; //����������
	WORD colddef; //����������
	WORD petrifydef; //��ʯ������
	WORD blinddef; //��ʧ������
	WORD stabledef; //����������
	WORD slowdef; //����������
	WORD luredef; //���ջ�����
		
	WORD poison; //�ж�����
	WORD lull; //�������
	WORD reel; //ѣ������
	WORD evil; //��ħ����
	WORD bite; //��������
	WORD chaos; //��������
	WORD cold; //��������
	WORD petrify; //ʯ������
	WORD blind; //ʧ������
	WORD stable; //��������
	WORD slow; //��������
	WORD lure; //�ջ�����
	
	struct leech
	{
		BYTE odds;    //x
		WORD effect;	//y
	};
	leech hpleech; //x%��������ֵy
	leech mpleech; //x%���շ���ֵy
	
	BYTE hptomp; //ת������ֵΪ����ֵx��
	BYTE dhpp; //�����˺�����x%	
	BYTE dmpp; //�����˺�ֵ����x%		

	BYTE incgold; //���ӽ�Ǯ����x%
	BYTE doublexp; //x%˫������		
	BYTE mf; //���ӵ�����x%

	union {
		BYTE _five_props[5];
		struct {
			//������װ�������
			BYTE dpdam; //�����˺�����%x
			BYTE dmdam; //�����˺�����%x
			BYTE bdam; //�����˺�x%
			BYTE rdam; //�˺�����%x
			BYTE ignoredef; //%x����Ŀ�����
		};
	};
	
	WORD aftype;  
	WORD afpoint;
	WORD dftype;
	WORD dfpoint;
	
	typedef std::map<WORD, WORD> SKILL;
	typedef SKILL::const_iterator const_iterator;
	SKILL skill;  //�������ܼӳ�
	SKILL skills; //ȫϵ���ܼӳ� 
	BYTE maxhprate;  //���ٷֱ��������hp
	BYTE maxmprate;  //���ٷֱ��������mp
};	


struct PackageCallback
{
	virtual bool exec(zObject * o)=0;
	virtual ~PackageCallback(){};
};

class Package:private zNoncopyable
{
	protected:
		zObject** container;

		virtual bool add(zObject *object ,bool find);
		virtual bool remove(zObject *object);
		virtual bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);		
	public:
		Package(WORD type,DWORD id, WORD w, WORD h);
		virtual ~Package();

		virtual bool getObjectByZone(zObject **ret,WORD x,WORD y);
		virtual bool getObjectByID(zObject **ret,DWORD id);
		virtual void execEvery(PackageCallback &callback);

		virtual WORD space() const;
		virtual WORD size() const;
		WORD type() const;
		DWORD id() const;
		
		void setSpace(WORD);
	private:
		friend class Packages;
		
		virtual void removeAll();	
		bool find_space(WORD &x,WORD &y) const;
		virtual int position(WORD x, WORD y) const;

		WORD _type;
		DWORD _id;
		WORD _width;
		WORD _height;

		WORD _space;
		WORD _size;

};

#if 0
class MultiPack:public Package
{
	protected:
		WORD width;
		WORD height;
		std::vector<std::vector<zObject *> >grid;
		std::set<zObject *> allset;
	public:
		MultiPack(WORD type,DWORD id ,WORD width,WORD height);
		virtual ~MultiPack();
		virtual bool add(zObject *object ,bool find);
		virtual void remove(zObject *object);
		virtual void removeAll();
		virtual bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);
		virtual bool getObjectByZone(zObject **ret,WORD x,WORD y,WORD width,WORD height);
		virtual void execEvery(PackageCallback &callback);
		virtual bool findSpace(WORD owidth,WORD oheight,WORD &x,WORD &y);
		std::set<zObject *> &getAllset()
		{
			return allset;
		}
		
		WORD get_width() const 
		{
			return width;
		}
		
		WORD get_height() const 
		{
			return height;
		}

};
#endif

class ObjectPack : public Package
{
public:
	ObjectPack(zObject* ob, int type = Cmd::OBJECTCELLTYPE_PACKAGE, bool consume = true);
	~ObjectPack();
	
	bool empty() const 
	{
		return space() == size();
	}
	
	zObject* object()
	{
		return _ob;
	}

	void consume_dur_by(SceneUser* user, const zRTime& current);
	bool can_input()
	{
		if (!_ob || _ob->data.dur == 0) {
			return false;
		}
		return true;
	}
	
private:	
	bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);
	//bool add(zObject *object ,bool find);
	
	zObject* _ob;
	bool _cosume_by_time;
	Timer _one_min;
};

		
class EquipPack : public Package
{
	public:

		enum EQUIPNO{		
			BODY,
			HEAD,
			HANDL,
			HANDR,
			BELT,
			NECKLACE, //5
			
			RINGL,
			RINGR,
			BANGL,
			BANGR,
			BOOTS, //10
			
			OTHERS,	
			OTHERS1,	
			OTHERS2,	
			OTHERS3,
			OTHERS4,	//15	
		};
		
		enum PACKNO{
			R_PACK,
			L_PACK,
			R_MAKE,
			L_MAKE,
		};
		EquipPack(SceneUser* user);
		~EquipPack();
		
		DWORD doubleexp_obj;
		DWORD doubleexp_obj_time;
		DWORD family_obj_times;
		DWORD family_obj_time;
		DWORD tong_obj_times;
		DWORD tong_obj_time;
		DWORD king_obj_times;
		DWORD king_obj_time;
		const Equips &getEquips() const;
		ObjectPack* pack(PACKNO no) const;
		zObject* equip(EQUIPNO no) const;
			
		bool getObjectByZone(zObject **ret,WORD x,WORD y);
		zObject *getObjectByEquipNo(EQUIPNO no);
		zObject *getObjectByEquipPos(DWORD no);

		bool costAttackDur(SceneUser *pThis);
		bool costDefenceDur(SceneUser *pThis);
		bool costAttackDurByPet(SceneUser *pThis);
		bool costDefenceDurByPet(SceneUser *pThis);
		void restituteDurability(SceneUser *pThis , const zRTime &ct);
		void updateDurability(SceneUser *pThis , DWORD value);
		DWORD fullAllEquiped(char *buf);

		bool needRecalc;
		int effectCount;

		bool skillReduceObject(SceneUser* pThis, DWORD kind, WORD num);
		bool skillCheckReduceObject(SceneUser* pThis, DWORD id, WORD num);
		bool canChangeColor();
		void calcAll();
		int reduceDur(SceneUser *pThis , DWORD which , DWORD type , DWORD num , bool needFresh=true , bool needCal=true);
		bool isTonic();

		void obtain_exp(SceneUser* user, DWORD exp, bool force = false);
	private:
		bool add(zObject *object ,bool find);
		bool remove(zObject *object);
		bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);

		virtual int position(WORD x, WORD y) const;
		
		bool reduceDur(SceneUser *pThis , DWORD which);
		inline void freshDurability(SceneUser *pThis , zObject *o);
		inline void freshUserData(SceneUser *pThis);

		bool process_extra_add(zObject* ob);
		bool process_extra_remove(zObject* ob);
		bool process_extra_get(zObject** ob, WORD x, WORD y);
		
		SceneUser* owner;
		ObjectPack** packs;
		Equips equips;
		const static int _poses[60];

};

class SceneUser;

class MainPack:public Package
{
	public:
		enum {
			WIDTH = 7,
			HEIGHT = 6,
		};
		
		MainPack();
		~MainPack();
		
		bool getObjectByZone(zObject **ret,WORD x,WORD y);
		zObject * getGold();
		DWORD getGoldNum();
		
		bool skillReduceObject(SceneUser* pThis, DWORD kind, DWORD num);

	private:
		bool add(zObject *object ,bool find);
		bool remove(zObject *object);
		bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);
		
		zObject *gold;
};

class StorePack : public Package
{
public:
	enum {
		W = 7,
		H = 6,
	};
	
	std::vector<BYTE> days;
	
	StorePack();
	~StorePack();
		
	void goldstore(SceneUser *pThis,bool notify=true);
	int load(unsigned char* dest);
	int save(unsigned char* dest);
	
	void notify(SceneUser* user);
	
private:
	bool checkAdd(SceneUser* pUser, zObject* object, WORD x, WORD y);
	
};

class PetPack : public Package
{
public:
	static const BYTE W = 8;
	static const BYTE H = 10;

	PetPack();
	~PetPack();

	WORD size() const;//������������
	void setAvailable(WORD);//������������

	bool isEmpty() const;

private:
	WORD available;//���õ�����

	bool checkAdd(SceneUser* pUser, zObject* object, WORD x, WORD y);
};

#include "ObjectManager.h"

template<typename T>
class Type2Type
{
public:
	typedef T BASE;
};


class Packages
{
	private:
		SceneUser *owner;

		Package** getPackage(int packs);
		
	public:
		enum {
			MAIN_PACK = 1,
			LEFT_PACK = 2,
			RIGHT_PACK = 4, 
			EQUIP_PACK = 8, 
			STORE_PACK = 16,
			LM_PACK = 32, 
			RM_PACK = 64,
			PET_PACK = 128,
		};
		
		UserObjectM uom;
		MainPack main;
		EquipPack equip;
		StorePack store;
		PetPack petPack;

		Packages(SceneUser* user);
		~Packages();
		void addMoney(DWORD num, const char *disc , const char *note = NULL, bool notify = true);
		void addGold(DWORD num, const char *disc , const char *note = NULL, bool notify = true,bool pack=false);
		void addTicket(DWORD num, const char *disc , const char *note = NULL, bool notify = true);
		bool checkMoney(DWORD need);
		bool checkTicket(DWORD need);
		bool checkGold(DWORD need);
		bool removeMoney(DWORD num ,const char *disc);
		bool removeGold(DWORD num ,const char *disc , bool need=true);
		bool removeTicket(DWORD num ,const char *disc);
		
		bool moveObject(SceneUser *pUser,zObject *srcObj,stObjectLocation &dst);
		bool moveObjectToScene(zObject *o , const zPos &pos , DWORD overdue_msecs=0 , const unsigned long dwID=0);
		bool removeObject(zObject *srcObj, bool notify = true, bool del = true);
		bool addObject(zObject *srcObj, bool needFind, int packs = 0);
		Package* getPackage(DWORD type,DWORD id);
		SceneUser *getOwner()
		{
			return owner;
		}
		
		template<typename T>
		void execEvery(Package* pack, T t)
		{
			typename T::BASE cb(this);
			pack->execEvery(cb);
		}
		
		template<typename T, typename P1>
		void execEvery(Package* pack, T t, P1 p1)
		{
			typename T::BASE cb(this, p1);
			pack->execEvery(cb);
		}
		
		zObject *getGold();
		DWORD getGoldNum();
};

#define AUTO_PACK Packages::MAIN_PACK |Packages::LEFT_PACK| Packages::RIGHT_PACK

struct Combination : public PackageCallback
{
public:
	Combination(SceneUser* user, zObject* get) : _user(user), _get(get), _num(0)
	{ }

	virtual bool exec(zObject* o);

	int num() const
	{
		return _num;
	}

private:
	SceneUser* _user;
	zObject* _get;
	int _num;
};

struct ClearPack : public PackageCallback
{
public: 
	ClearPack(const Packages* ps) : _ps(ps)
	{ }
	
	virtual bool exec(zObject* p);
private:
	const Packages* _ps;
};

struct DropFromPack : public PackageCallback
{
public:
	struct Param
	{
		Package* pack;
		int drop_num;
		zPos pos;

		Param(Package* p, int d, const zPos& s) : pack(p), drop_num(d), pos(s)
		{ }
	};
	
	DropFromPack(const Packages* ps, const Param&  p) ;
	virtual bool exec(zObject* ob);
private:
	const Packages* _ps;
	Param _p;

	int _begin;
	int _pos;
};


#endif

