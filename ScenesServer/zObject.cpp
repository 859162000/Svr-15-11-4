#include "Zebra.h"
#include "zMisc.h"
#include "SceneUser.h"
#include "Scene.h"
#include "TimeTick.h"
#include "ObjectManager.h"
#include "Chat.h"
#include "ScenesServer.h"
#include "SessionClient.h"

/**
 * \brief 构造函数
 */
zObject::zObject():zEntry()
{
	createtime=time(NULL);
	base=NULL;
	inserted=false;
	bzero(&data,sizeof(data));
	data.fivetype = FIVE_NONE;
	fill(data);
}

/**
 * \brief 生成对象ID
 */
void zObject::generateThisID()
{
	id=zMisc::randBetween(0,1)?zMisc::randBetween(-1000,0x80000000):zMisc::randBetween(1000,0x7FFFFFFE);
	data.qwThisID=id;
}

/**
 * \brief 析构从全局物品索引中删除自己，并清空相关属性
 */
zObject::~zObject()
{
	if(inserted)
	{
		goi->removeObject(this->id);
		inserted=false;
		createtime=0;
		base=NULL;
		bzero(&data,sizeof(data));
	}
}

void zObject::fill(t_Object& d)
{
	for (int i=0; i<5; ++i) {
		_p1[i] = &d.str + i;
	}

	for(int i=0; i<16; ++i) {
		_p2[i] = &d.pdam + i;
	}
	
	for(int i=0; i<2; ++i) {
		_p2[i+16] = &d.atrating + i;
	}

}

void zObject::checkBind()
{
	switch(base->kind)
	{
		case ItemType_DoubleExp:
		case ItemType_Honor:
		case ItemType_ClearProperty:
		case ItemType_KING:
		case ItemType_FAMILY:
		case ItemType_TONG:
			{
				data.bind=1;
			}
			break;
		default:
			break;
	}
	if(base->id== 881 || base->id == 882)
	{
		data.bind=1;
	}
}
DWORD zObject::RepairMoney2RepairGold(DWORD dwObjectRepair)
{
	return (DWORD)((float)(dwObjectRepair/50)+0.99);
}

/**
 * \brief 物品log
 * \param createid 	物品创建id
 * \param objid 	物品Thisid
 * \param objname 	物品名称
 * \param num 		物品数量
 * \param change	物品数量
 * \param type	 	变化类型(2表示上线加载,1表示增,0表示减)
 * \param srcid 	源id
 * \param srcname  	源名称
 * \param dstid 	目的id
 * \param dstname 	目的名称
 * \param action 	操作
 *
void zObject::logger(QWORD createid,DWORD objid,char *objname,DWORD num,DWORD change,DWORD type,DWORD srcid,char *srcname,DWORD dstid,char *dstname,const char *action)
{
	*
	char _objname[MAX_NAMESIZE + 1];
	char _srcname[MAX_NAMESIZE + 1];
	char _dstname[MAX_NAMESIZE + 1];
	char _action[MAX_NAMESIZE + 1];
	bzero(_objname,sizeof(_objname));
	bzero(_srcname,sizeof(_srcname));
	bzero(_dstname,sizeof(_dstname));
	bzero(_action,sizeof(_action));
	if(objname)
	{
		strncpy(_objname,objname,MAX_NAMESIZE +1);
	}
	else
	{
		strncpy(_objname,"NULL",MAX_NAMESIZE +1);
	}
	if(srcname)
	{
		strncpy(_srcname,srcname,MAX_NAMESIZE +1);
	}
	else
	{
		strncpy(_srcname,"NULL",MAX_NAMESIZE +1);
	}
	if(dstname)
	{
		strncpy(_dstname,dstname,MAX_NAMESIZE +1);
	}
	else
	{
		strncpy(_dstname,"NULL",MAX_NAMESIZE +1);
	}
	if(action)
	{
		strncpy(_action,action,MAX_NAMESIZE +1);
	}
	else
	{
		strncpy(_action,"NULL",MAX_NAMESIZE +1);
	}
	// /
	ScenesService::objlogger->debug("%llu,%u,%s,%u,%u,%u,%u,%s,%u,%s,%s",createid,objid,objname,num,change,type,srcid,srcname,dstid,dstname,action);
}*/
/**
 * \brief 物品log
 * \param createid      物品创建id
 * \param objid         物品Thisid
 * \param objname       物品名称
 * \param num           物品数量
 * \param change        物品数量
 * \param type          变化类型(2表示上线加载,1表示增,0表示减)
 * \param srcid         源id
 * \param srcname       源名称
 * \param dstid         目的id
 * \param dstname       目的名称
 * \param action        操作
 * \param base          物品基本表里的指针
 * \param kind          物品的种类
 * \param upgrade       物品的等级（升级的次数）
 * \brief		其中后三个参数是用来打印，追加的日志的，包括装备的颜色，星级，材料的等级，宝石的等级
 */
void zObject::logger(QWORD createid,DWORD objid,char *objname,DWORD num,DWORD change,DWORD type,DWORD srcid,char *srcname,DWORD dstid,char *dstname,const char *action, zObjectB *base,BYTE kind,BYTE upgrade)
{
        char tmpInfo[60] = {0};
        char *p = tmpInfo;

        if(srcname != NULL)
                strcat(tmpInfo, srcname);

	if(base != NULL)
	{
        	switch (base->kind)
        	{
                	case ItemType_ClothBody:                //101代表布质加生命类服装
                	case ItemType_FellBody:                 //102代表皮甲加魔防类服装
                	case ItemType_MetalBody:                //103代表金属铠甲加物防类服装
                	case ItemType_Blade:                    //104代表武术刀类武器
                	case ItemType_Sword :                   //105代表武术剑类武器
               		case ItemType_Axe:                      //106代表武术斧类武器
                	case ItemType_Hammer:                   //107代表武术斧类武器
                	case ItemType_Staff:                    //108代表法术杖类武器
                	case ItemType_Crossbow:                 //109代表箭术弓类武器
                	case ItemType_Fan:                      //110代表美女扇类
                	case ItemType_Stick:                    //111代表召唤棍类武器
                	case ItemType_Shield:                   //112代表盾牌类
                	case ItemType_Helm:                     //113代表角色头盔类
                	case ItemType_Caestus:                  //114代表角色腰带类
                	case ItemType_Cuff:                     //115代表角色护腕类
                	case ItemType_Shoes:                    //116代表角色鞋子类
                	case ItemType_Necklace:                 //117代表角色项链类
                	case ItemType_Fing:                     //118代表角色戒指类
                        	{
                                	switch(kind)
                                	{
                                        	case 0:
                                                	strcat(p + strlen(tmpInfo), ":");
                                                	strcat(tmpInfo, "白色:");
                                                	break;
                                        	case 1:
                                                	strcat(p + strlen(tmpInfo), ":");
                                                	strcat(tmpInfo, "蓝色:");
                                                	break;
                                        	case 2:
                                                	strcat(p + strlen(tmpInfo), ":");
                                                	strcat(tmpInfo, "黄色:");
                                                	break;
                                        	case 4:
                                                	strcat(p + strlen(tmpInfo), ":");
                                                	strcat(tmpInfo, "绿色:");
                                                	break;
                                        	default:
                                               		break;
                                	}
                                	sprintf(p + strlen(tmpInfo), "%d",upgrade);
                        	}
                        	break;

                	case ItemType_Resource: //16代表原料类 
                        	{
                                	switch(base->id)
                                	{
                                        	//下面是需要显示等级的物品ID
                                        	case 506:
                                        	case 507:
                                        	case 516:
                                        	case 517:
                                        	case 526:
                                        	case 527:
                                        	case 536:
                                        	case 537:
                                        	case 546:
                                        	case 547:
                                        	case 877:
                                        	{
                                                	strcat(p + strlen(tmpInfo), ":");
                                                	strcat(tmpInfo, "材料:");
                                                	sprintf(p + strlen(tmpInfo), "%d",(upgrade+1));

                                        	}
                                        	default:
                                               	 	break;

                                	}
                        	}
                        	break;
                	case ItemType_LevelUp:       //27代表道具升级需要的材料类
                        	{
                                	switch(base->id)
                                	{
                                        	case 678:
                                        	case 679:
                                        	{
                                                	strcat(p + strlen(tmpInfo), ":");
                                                	strcat(tmpInfo, "宝石:");
                                                	sprintf(p + strlen(tmpInfo), "%d",(upgrade+1));
                                        	}
                                        	default:
                                                	break;
                                	}
                        	}
                        	break;
                	default:
                        	break;
        	}
	}

        ScenesService::objlogger->debug("%llu,%u,%s,%u,%u,%u,%u,%s,%u,%s,%s",createid,objid,objname,num,change,type,srcid,tmpInfo,dstid,dstname,action);
}
/**
 * \brief 根据物品对象复制一个新的物品对象
 * \param objsrc 参照对象
 * \return 失败返回NULL 否则返回生成的对象
 */
zObject *zObject::create(zObject *objsrc)
{
	if(objsrc==NULL) return NULL;
	zObject *ret=new zObject();
	if(ret)
	{
		strncpy(ret->name,objsrc->name,MAX_NAMESIZE);
		ret->tempid=objsrc->id;
		ret->base=objsrc->base;
		bcopy(&objsrc->data,&ret->data,sizeof(ret->data));
		ret->generateThisID();
		ret->free(true);

		if(!goi->addObject(ret))
		{
			SAFE_DELETE(ret);
		}
		else
		{
			ret->dwCreateThisID=ret->data.qwThisID;
			ret->inserted=true;
		}
	}
	return ret;
}

void  zObject::destroy(zObject* ob)
{
	assert(!ob || ob->free());
	SAFE_DELETE(ob);
}

const stObjectLocation &zObject::reserve() const
{
	return data.pos;
}

void zObject::restore(const stObjectLocation &loc)
{
	data.pos = loc;
}

bool zObject::free() const
{
	return data.pos == Object::INVALID_POS || data.pos.loc() == Cmd::OBJECTCELLTYPE_NONE;
}

void zObject::free(bool flag)
{
	data.pos = Object::INVALID_POS;
}

/**
 * \brief 根据物品字典创建一个物品对象
 * \param objbase 物品字典
 * \param num 物品的数量
 * \param level 物品的级别
 * \return 物品对象
 */
zObject *zObject::create(zObjectB *objbase, unsigned int num, BYTE level)
{
	if(objbase==NULL) return NULL;

	//only money can have 0
	if ( (num == 0 && objbase->id != 665) || num > objbase->maxnum) {
		return NULL;
	}
	
	zObject *ret=new zObject();
	if(ret)
	{
		ret->base = objbase;
		ret->generateThisID();
		strncpy(ret->name, objbase->name, MAX_NAMESIZE);
		strncpy(ret->data.strName, objbase->name, MAX_NAMESIZE);
/*		
		ret->data.pos.dwLocation = Cmd::OBJECTCELLTYPE_COMMON;
*/
		ret->data.dwObjectID = objbase->id;

		ret->free(true);
		ret->tempid = objbase->id;
		ret->data.dwNum = num;
		ret->data.color = objbase->color;
		switch (objbase->kind)
		{
			case ItemType_ClothBody:		//101代表布质加生命类服装
			case ItemType_FellBody:		    //102代表皮甲加魔防类服装
			case ItemType_MetalBody:		//103代表金属铠甲加物防类服装
			case ItemType_Blade:		    //104代表武术刀类武器
			case ItemType_Sword :	        //105代表武术剑类武器
			case ItemType_Axe:	           //106代表武术斧类武器
			case ItemType_Hammer:	        //107代表武术斧类武器
			case ItemType_Staff:		    //108代表法术杖类武器
			case ItemType_Crossbow:	        //109代表箭术弓类武器
			case ItemType_Fan:	           //110代表美女扇类
			case ItemType_Stick:	        //111代表召唤棍类武器
			case ItemType_Shield:	//112代表盾牌类
			case ItemType_Helm:		//113代表角色头盔类
			case ItemType_Caestus:	//114代表角色腰带类
			case ItemType_Cuff:		//115代表角色护腕类
			case ItemType_Shoes:		//116代表角色鞋子类
			case ItemType_Necklace:	//117代表角色项链类
			case ItemType_Fing:		//118代表角色戒指类
			case ItemType_FashionBody:		//119代表时装
					ret->data.color  = zMisc::randBetween(0xFF000000, 0xFFFFFFFF);
					break;		
		}
		
		ret->data.needlevel = objbase->needlevel;				// 需要等级
	
		ret->data.maxhp = objbase->maxhp;					// 最大生命值
		ret->data.maxmp = objbase->maxmp;					// 最大法术值
		ret->data.maxsp = objbase->maxsp;					// 最大体力值
	
		ret->data.pdamage = objbase->pdamage;				// 最小攻击力
		ret->data.maxpdamage = objbase->maxpdamage;			// 最大攻击力
		ret->data.mdamage = objbase->mdamage;				// 最小法术攻击力
		ret->data.maxmdamage = objbase->maxmdamage;			// 最大法术攻击力
	
		ret->data.pdefence = objbase->pdefence;				// 物防
		ret->data.mdefence = objbase->mdefence;				// 魔防
		ret->data.damagebonus = objbase->damagebonus;			// 伤害加成
		
		ret->data.akspeed = objbase->akspeed;				// 攻击速度
		ret->data.mvspeed = objbase->mvspeed;				// 移动速度
		ret->data.atrating = objbase->atrating;				// 命中率
		ret->data.akdodge = objbase->akdodge;				// 躲避率
		ret->data.bang = objbase->bang;
		ret->data.dur = objbase->durability;
		ret->data.maxdur = objbase->durability;
		ret->data.price = objbase->price;
		ret->data.cardpoint = objbase->cardpoint;
		
		ret->data.upgrade = level;								// 初始等级

		if(!goi->addObject(ret))
		{
			SAFE_DELETE(ret);
		}
		else
		{
			ret->dwCreateThisID=ret->data.qwThisID;
			ret->inserted = true;
                        Zebra::logger->debug("创建物品%s成功等级%d", objbase->name,ret->data.upgrade);
                        ret->data.upgrade = level;
		}
	}else {
		Zebra::logger->debug("创建物品%s失败", objbase->name);
	}
	
	return ret;
}

/**
 * \brief 从档案服务器读物品
 *
 * \param o 从档案服务器中读到的物品
 *
 * \return load成功返回该物品,否则返回NULL
 */
zObject *zObject::load(const SaveObject *o)
{
	if(o==NULL) return NULL;
	zObjectB *objbase = objectbm.get(o->object.dwObjectID);
	if(objbase==NULL) 
	{
		Zebra::logger->debug("加载物品失败,道具基本表中不存在:%d",o->object.dwObjectID);
		return NULL;
	}
	int i=0;
	zObject *ret=NULL; 
	while(!ret && i < 100)
	{
		ret=new zObject();
		if( i > 1)
		{
			Zebra::logger->debug("尝试new出新的物品对象次数:%d",i);
		}
		i ++;
	}
	if(ret == NULL) 
	{
		Zebra::logger->debug("加载物品失败,new物品对象失败:%d",o->object.dwObjectID);
		return ret; 
	}
	bcopy(&o->object , &ret->data , sizeof(t_Object));
	ret->createid = o->createid;
	ret->id = ret->data.qwThisID;
	ret->tempid = ret->data.dwObjectID;
	strncpy(ret->name,ret->data.strName,MAX_NAMESIZE);
	ret->base=objbase;

	if(!goi->addObject(ret))
	{
		SAFE_DELETE(ret);
	}
	else
		ret->inserted=true;
	return ret;
}
/**
 * \brief 得到物品创建时间,存档时使用
 *
 * \return 物品创建时间
 */
bool zObject::getSaveData(SaveObject *save)
{
	bcopy(&data , &save->object , sizeof(t_Object));
	save->createid =  createid;
	return true;
}

/**
 * \brief 返回物品孔的数目

 * \return 孔的数目
 */
int zObject::foundSocket()
{
	WORD socket = 0;
	if (zMisc::selectByPercent(base->hole.odds)) {
		socket = zMisc::randBetween(base->hole.min, base->hole.max);
		for(int i=0; i<socket; ++i)
			data.socket[i] = (DWORD)-1;
	}
	
	return socket;
}

/**
 * \brief 返回物品是否可以邮寄

 * \return 是否可以
 */
bool zObject::canMail()
{
	if (data.bind || data.dwObjectID == 800 || base->kind == ItemType_Quest || data.dwObjectID == 734 )
		return false;
	if (data.pos.loc() !=Cmd::OBJECTCELLTYPE_COMMON 
			&& data.pos.loc() !=Cmd::OBJECTCELLTYPE_PACKAGE
			&& data.pos.loc() !=Cmd::OBJECTCELLTYPE_PET)
		return false;
	return true;
}

/**
 * \brief 返回物品是否可以被捐献

 * \return 是否可以
 */
zCountryMaterialB* zObject::canContribute()
{
	zCountryMaterialB* country_material = NULL;
	country_material = countrymaterialbm.get(data.dwObjectID+base->kind);
	return country_material;
}

/**
 * \brief 返回物品原料类别

 * \return 类别
 */
DWORD zObject::getMaterialKind()
{
	DWORD ret = 0;
	
	if (base->kind == ItemType_Resource)
	{
		if (data.dwObjectID == 501 || data.dwObjectID == 502 || data.dwObjectID == 506 || data.dwObjectID == 507)
		{
			ret = 1;
		}
		else if (data.dwObjectID == 511 || data.dwObjectID == 512 
			|| data.dwObjectID == 516 || data.dwObjectID == 517)
		{
			ret = 2;
		}
		else if (data.dwObjectID == 521 || data.dwObjectID == 522 
			|| data.dwObjectID == 526 || data.dwObjectID == 527)
		{
			ret = 3;
		}
		else if (data.dwObjectID == 531 || data.dwObjectID == 532 
			|| data.dwObjectID == 536 || data.dwObjectID == 537)
		{
			ret = 4;
		}
		else if (data.dwObjectID == 541 || data.dwObjectID == 542
			|| data.dwObjectID == 546 || data.dwObjectID == 547)
		{
			ret = 5;
		}
		else if (data.dwObjectID == 554 || data.dwObjectID == 555
			|| data.dwObjectID == 559 || data.dwObjectID == 560)
		{
			ret = 6;
		}
	}

	return ret;
}


/**
 * \brief 包裹构造
 */
Package::Package(WORD type,DWORD id, WORD w, WORD h):_type(type),_id(id), _width(w), _height(h), _space(w*h), _size(_space)

{
	WORD cap = _size;
	if (cap == 0) cap = 1;
	container = new zObject* [cap];
	memset(container, 0, sizeof(zObject*)*cap);
}

Package::~Package()
{
	removeAll();
	SAFE_DELETE_VEC(container);
}

bool Package::checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y)
{
	if (object==NULL) return true;
	zObject *temp;
	return getObjectByZone(&temp, x, y);
}

bool Package::add(zObject* object, bool find)
{
	if(!object  || object->base->kind==ItemType_Money)
		return false;

	if(find)	{
		if(find_space(object->data.pos.x, object->data.pos.y))	{
			return add(object,false);
		}else {
			//Zebra::logger->warn("包裹[%d:%d]中找不到空间存放物品[%x]", _type, _id, object);
			return false;
		}	
	}

	if(!find)	{
		int pos = position(object->data.pos.xpos(), object->data.pos.ypos());
		if (pos == -1 || pos >= size()) {
			Zebra::logger->warn("包裹[%d:%d]中添加物品[%x]时索引[%d]错误", _type, _id, object, pos);
			return false;
		}
		if (container[pos]) { 
			//shouldn't be reached at all
			Zebra::logger->warn("包裹[%d]中[%d, %d]已有物品%x，不能存放物品%x", _id, object->data.pos.x, object->data.pos.y, container[pos], object);
			return false;
		}else {

		//	assert(!container[pos]);
			container[pos] = object;
			--_space;
			return true;
		}
	}

	//never reach
	return false;

}

bool Package::find_space(WORD &x,WORD &y) const
{
	for (int i=0; i<_width; ++i)
		for(int j=0; j<_height; ++j) 
			if (!container[j*_width+i]) {
				x = i;
				y = j;
				return true;
			}

	return false;
}

int Package::position(WORD x, WORD y) const
{
	return y*_width + x;
}


bool Package::remove(zObject *object)
{
	if(object)
	{
		int pos = position(object->data.pos.xpos(), object->data.pos.ypos());
		if (pos == -1 || pos >= size() || container[pos] != object) {
			Zebra::logger->warn("包裹[%d:%d]中删除物品[%x]时索引[%d]错误", _type, _id, object, pos);
			return false;
		}

		object->free(true);
		container[pos] = NULL;
		++_space;
		return true;
	}

	return false;
}

void Package::removeAll()
{
	for (int i=0; i<size(); ++i) {
		SAFE_DELETE(container[i]);
	};
}

bool Package::getObjectByZone(zObject **ret, WORD x, WORD y)
{
	int pos = position(x, y);
	if (pos >= size() || pos == -1) return false;
	
	*ret = container[pos];
	return true;
}

bool Package::getObjectByID(zObject **ret, DWORD id)
{
	for (int i=0; i<size(); ++i)
	{
		if (!container[i]) continue;

		if (container[i]->base->id==id)
		{
			*ret = container[i];
			return true;
		}
	}
	return false;
}

void Package::execEvery(PackageCallback &callback)
{
	for (int i=0; i<size(); ++i) {
		if (!container[i]) continue;
		if (!callback.exec(container[i])) break;
	}
}

WORD Package::space() const
{
	return _space;
}

WORD Package::size() const
{
	return _size;
}

/**
 * \brief 获取包裹类型
 * \return 包裹类型
 */
WORD Package::type() const
{
	return _type;
}

/**
 * \brief 获取包裹ID
 * \return 包裹ID
 */
DWORD Package::id() const
{
	return _id;
}

void Package::setSpace(WORD s)
{
	_space = s;
}
#if 0
/**
 * \brief 多格包
 * \param type 类型
 * \param id 编号
 * \param width 宽度
 * \param height 高度
 */
MultiPack::MultiPack(WORD type,DWORD id ,WORD width,WORD height):Package(type,id)
{
	this->width=width;
	this->height=height;
	grid.resize(height);
	for(int i = 0; i < height; i++)
	{
		grid[i].resize(width);
		for(int j=0;j<width;j++)
			grid[i][j]=NULL;
	}
}

/**
 * \brief 析构函数
 */
MultiPack::~MultiPack()
{
	for(std::set<zObject *>::iterator it=allset.begin();it!=allset.end();it++)
	{
		zObject *o = *it;
		SAFE_DELETE(o);
	}
	allset.clear();
	for(int i = 0; i < height; i++)
		for(int j=0;j<width;j++)
			grid[i][j]=NULL;
}

/**
 * \brief 添加装备
 * \param object 物品对象
 * \param find 是否自动寻找位置
 * \return 添加成功返回true 否则返回false
 */
bool MultiPack::add(zObject *object,bool find)
{ 
	if(object==NULL || object->base->kind==ItemType_Money)
		return false;
	if(find)
	{
		if(findSpace(object->base->width,object->base->height,object->data.pos.x,object->data.pos.y))
		{
			return add(object,false);
		}
	}
	else if(object && object->data.pos.x+object->base->width<=width  && object->data.pos.y+object->base->height <=height)
	{
		object->data.pos.dwLocation=type;
		object->data.pos.dwTableID=id;
		WORD i = 0;
		WORD j = 0;
		for(i=object->data.pos.x;i<object->data.pos.x + object->base->width;i++)
			for(j=object->data.pos.y;j<object->data.pos.y + object->base->height;j++) 
				grid[j][i]=object;
		allset.insert(object); 
		/*
		if(!allset.insert(object).second && object->base->width == 1 && object->base->height == 1)
		{
			Zebra::logger->debug("重复加入物品");
			grid[j][i]=0;
		}
		// */
		/*
		for(WORD j=0;j<height;j++)
		{
			printf("\n");
			for(WORD i=0;i<width;i++)
				if(grid[j][i]==NULL)
					printf("0");
				else
					printf("*");
		}
		printf("\n");
		// */
		return true;
	}
	return false;
}

/**
 * \brief 检查物品
 * \param pUser 角色
 * \param object 物品对象
 * \param x 横坐标
 * \param y 纵坐标
 * \return true 装备有此物品
 */
bool MultiPack::checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y)
{
	if (object==NULL) return true;
	zObject *temp;
	return getObjectByZone(&temp, x, y,object->base-> width,object->base-> height);
}

/**
 * \brief 从包裹里删除物品
 * \param object 要处理的对象
 */
void MultiPack::remove(zObject *object)
{
	if(object)
	{
		object->data.pos.dwLocation=Cmd::OBJECTCELLTYPE_NONE;
		object->data.pos.dwTableID=0;
		allset.erase(object);
		int count=0;
		for(WORD i=0;i<width;i++)
			for(WORD j=0;j<height;j++) {
				if(grid[j][i]==object)
				{
					count++;
					grid[j][i]=NULL;
				}
			}
		if (count>1) {
			Zebra::logger->fatal("物品(%s)坐标(%d,%d)在主包裹中有镜像", object->name, object->data.pos.x, object->data.pos.y);
			return;
		}
		/*
				if (i>=width || j >=height) {
					Zebra::logger->fatal("物品(%s)坐标错误(%d,d)", object->name, object->data.pos.x, object->data.pos.y);
					return;
				}
		for(WORD i=object->data.pos.x;i<object->data.pos.x+object->base->width;i++)
			for(WORD j=object->data.pos.y;j<object->data.pos.y+object->base->height;j++) {
				if (i>=width || j >=height) {
					Zebra::logger->fatal("物品(%s)坐标错误(%d,d)", object->name, object->data.pos.x, object->data.pos.y);
					return;
				}
				if(grid[j][i]==object) grid[j][i]=NULL;
			}
			*/
	}
}

/**
 * \brief 清空包裹
 * \param object 要处理的对象
 */
void MultiPack::removeAll()
{
	for (std::set<zObject *>::iterator it=allset.begin(); it!=allset.end(); ++it) 
	{
		remove(*it);
	}
	allset.clear();
}

/**
 * \brief  根据位置拿到对象
 * \param ret 回传拿到的对象
 * \param x   横坐标
 * \param y   纵坐标
 * \param w   宽度
 * \param h   高度
 * \return true 得到物品， false 该区域很复杂不只一个物品
 */
bool MultiPack::getObjectByZone(zObject **ret,WORD x,WORD y,WORD w,WORD h)
{
	*ret=NULL;
	if(x+w>width || y+h>height) return false;
	int count=0;
	for(WORD i=x;i<x+w;i++)
		for(WORD j=y;j<y+h;j++)
			if(grid[j][i]!=*ret && grid[j][i]!=NULL)
			{
				count++;
				*ret=grid[j][i];
			}
	return (count <=1);
}

/**
 * \brief 查找背包空间
 * \param owidth 物品宽
 * \param oheight 物品高
 * \param 查找到的x坐标
 * \param 查找到的y坐标
 * \return true 找到合适的位置，false 没找到适合的位置
 */
bool MultiPack::findSpace(WORD owidth,WORD oheight,WORD &x,WORD &y)
{
		bool finded=false;
		WORD i=0,j=0;
		for(i=0;i<=width-owidth;i++)
		{
			for(j=0;j<=height-oheight;j++)
			{
				bool space=true;
				for(WORD w=0;w<owidth;w++)
				{
					for(WORD h=0;h<oheight;h++)
						if(grid[j+h][i+w]!=NULL)
						{
							space=false;
							break;
						}
					if(!space) break;
				}
				if(space)
				{
					finded=true;
					break;
				}
			}
			if(finded) break;
		}

		if(finded)
		{
			x=i;
			y=j;
		}
		return finded;
}

/**
 * \brief 遍历，空方法
 * \param callback 回调
 */
void MultiPack::execEvery(PackageCallback &callback)
{
	for(std::set<zObject *>::iterator it=allset.begin(); it!=allset.end(); ++it) {
		if (!callback.exec(*it)) break;
	}
}
#endif

/**
 * \brief 装备包裹构造
 * \param user 包裹的主人
 */
EquipPack::EquipPack(SceneUser* user):Package(Cmd::OBJECTCELLTYPE_EQUIP,0, 1, 20) , owner(user)
{
	needRecalc=true;
	packs = new ObjectPack*[4];
	memset(packs, 0, 4*sizeof(ObjectPack*));
	doubleexp_obj=0; 
	doubleexp_obj_time=0; 
	family_obj_times=5;
	family_obj_time=0;
	tong_obj_times=2;
	tong_obj_time=0;
	king_obj_times=2;
	king_obj_time=0;
	effectCount=0;
}

/**
 * \brief 析构函数
 */
EquipPack::~EquipPack()
{
	for (int i=0; i<4; ++i) {
		SAFE_DELETE(packs[i]);
	}
	SAFE_DELETE_VEC(packs);
}

bool EquipPack::isTonic()
{
	if(container[13] && container[13]->base->kind == ItemType_Tonic)
	{
		return true;
	}
	return false;
}
/**
 * \brief 更新装备的耐久度
 * \param pThis 包裹主人
 * \param value 装备耐久值
 */
void EquipPack::updateDurability(SceneUser *pThis , DWORD value)
{
	for (int i=0;i<16;i++)
	{
		if(container[i]!=NULL)
		{
			container[i]->data.dur = value>container[i]->data.maxdur?container[i]->data.maxdur:value;
			if(container[i]->data.dur == 0)
			{
				container[i]->data.dur = 1;
			}

			freshDurability(pThis, container[i]);

			/*
			if(equip[i]!=NULL && equip[i]->data.dur < equip[i]->data.maxdur)
			{
				equip[i]->data.dur = equip[i]->data.maxdur;
			}
			*/
		}
	}
}

/**
 * \brief 将装备信息填到buf中
 * \param buf 返回空间
 * \return 装备数量
 */
DWORD EquipPack::fullAllEquiped(char *buf)
{
	using namespace Cmd;
	EquipedObject *data = (EquipedObject*)buf;
	int num = 0;
	for (int i =0;i<16;i++)
	{
		if(container[i]!=NULL)
		{
			bcopy(&container[i]->data , &data[num].object, sizeof(t_Object));
			data[i].byWhere = i;
			num++;
		}
	}
	return num;
}

/**
 * \brief 恢复耐久度
 * \param pThis 角色
 * \param ct 时间
 */
void EquipPack::restituteDurability(SceneUser *pThis , const zRTime &ct)
{

	for (int i=0;i<12;i++)
	{
		if(container[i]!=NULL)
		{
			if(container[i]!=NULL && container[i]->data.dur < container[i]->data.maxdur)
			{
				if(container[i]->data.dursecond > 0 && container[i]->data.durpoint > 0)
				{
					if(!(ct.sec() % container[i]->data.dursecond))
					{
						bool need = container[i]->data.dur == 0 ? true : false;
						container[i]->data.dur +=container[i]->data.durpoint;
						if(container[i]->data.dur > container[i]->data.maxdur)
						{
							container[i]->data.dur = container[i]->data.maxdur;
						}
						if(need)
						{
							freshDurability(pThis , container[i]);
							calcAll();
							freshUserData(pThis);
						}
					}
				}
			}
		}
	}

}

/**
 * \brief 重算装备对人物的影响并通知客户端
 * \param 
 * \return 
 */
void EquipPack::freshUserData(SceneUser *pThis)
{
	pThis->setupCharBase();
	Cmd::stMainUserDataUserCmd  userinfo;
	pThis->full_t_MainUserData(userinfo.data);
	pThis->sendCmdToMe(&userinfo,sizeof(userinfo));
}

/**
 * \brief 刷新耐久度，通知客户端耐久度变化
 * \param pThis 角色
 * \param o 物品
  */
void EquipPack::freshDurability(SceneUser *pThis , zObject *o)
{
	Cmd::stDurabilityUserCmd std;
	std.dwThisID = o->data.qwThisID;
	std.dwDur = o->data.dur;
	std.dwMaxDur = o->data.maxdur;
	pThis->sendCmdToMe(&std , sizeof(std));
}

/**
 * \brief 削减耐久度 
 * \param pThis 角色
 * \param which 消耗目标
 */
bool EquipPack::reduceDur(SceneUser *pThis , DWORD which)
{
	if(which > 15 || container[which] == NULL)
	{
		return false;
	}
	WORD olddur = (WORD)(container[which]->data.dur / 50) + 1;
	container[which]->data.dur --;
	if((olddur != (WORD)(container[which]->data.dur / 50) + 1) || (container[which]->data.dur == 0))
	{
		freshDurability(pThis , container[which]);
	}
	if(container[which]->data.dur == 0)
	{
		calcAll();

		if( container[which]->base->kind <= 118 && container[which]->base->kind >= 101 && container[which]->data.bind)
		{
			return false;
		}
		else
		{
			//临时增加,便于QA测试
			//再次根据策划文档修改
			zObject::logger(container[which]->createid,container[which]->data.qwThisID,container[which]->data.strName,container[which]->data.dwNum,1,0,pThis->id,pThis->name,0,NULL,"耐久用完删除",NULL,0,0);
			if(pThis->packs.removeObject(container[which]))
			{
				//FunctionTimes times(1,"reduceDur");
				container[which]=NULL; 
				pThis->setupCharBase();
				Cmd::stMainUserDataUserCmd  userinfo;
				pThis->full_t_MainUserData(userinfo.data);
				pThis->sendCmdToMe(&userinfo,sizeof(userinfo));
				pThis->sendMeToNine();
			}

			return false;
		}
	}
	return true;
}
zObject *EquipPack::getObjectByEquipPos(DWORD no)
{
	if(no < 16)
	{
		return container[no];
	}
	else
	{
		return NULL;
	}
}
zObject *EquipPack::getObjectByEquipNo(EQUIPNO no)
{
	return container[no];
}
/**
 * \brief 削减耐久值 
 * \param pThis 角色
 * \param which 消耗目标
 *
 * \return 实际消耗的耐久值
 */
int EquipPack::reduceDur(SceneUser *pThis , DWORD which , DWORD type , DWORD num , bool needFresh , bool needCal)
{
	int ret = 0;
	if(which > 15 || container[which] == NULL)
	{
		return 0;
	}
	if(container[which]->base->kind != type)
	{
		return 0;
	}
	WORD olddur = (WORD)(container[which]->data.dur / 50) + 1;
	ret = (int)(container[which]->data.dur - num);
	if(ret > 0)
	{
		container[which]->data.dur = ret;
		ret = num;
	}
	else
	{
		ret = container[which]->data.dur;
		container[which]->data.dur = 0;
	}
	if(needFresh)
	{
			freshDurability(pThis , container[which]);
	}
	else
	{
		if(olddur != (WORD)(container[which]->data.dur / 50) + 1)
		{
			freshDurability(pThis , container[which]);
		}
	}
	if(container[which]->data.dur == 0)
	{
		if(needCal)
		{
			calcAll();
		}

		//临时增加,便于QA测试
		//再次根据策划文档修改
		if( container[which]->base->kind <= 118 && container[which]->base->kind >= 101 && container[which]->data.bind)
		{
			return false;
		}
		else
		{
			zObject::logger(container[which]->createid,container[which]->data.qwThisID,container[which]->data.strName,container[which]->data.dwNum,1,0,pThis->id,pThis->name,0,NULL,"耐久用完删除",NULL,0,0);
			if(pThis->packs.removeObject(container[which])) //notify and delete
			{
				container[which]=NULL; 
				pThis->setupCharBase();
				Cmd::stMainUserDataUserCmd  userinfo;
				pThis->full_t_MainUserData(userinfo.data);
				pThis->sendCmdToMe(&userinfo,sizeof(userinfo));
				pThis->sendMeToNine();
			}
		}

	}
	return ret;
}

/**
 * \brief 消耗攻击性装备耐久度
 * \param pThis 角色
 */
bool EquipPack::costAttackDur(SceneUser *pThis)
{
	bool bret = false;
	//int jewelry = 0;
	//int which[3] ={0};
	//
	for (int i=0;i<16;i++)
	{
		if(container[i]!=NULL && container[i]->data.dur != 0)
		{
			switch(container[i]->base->kind)
			{
				case ItemType_Blade:		    //104代表武术刀类武器
				case ItemType_Sword :	        //105代表武术剑类武器
				case ItemType_Axe:	    	    //106代表武术斧类武器
				case ItemType_Hammer:	        //107代表武术斧类武器
				case ItemType_Staff:		    //108代表法术杖类武器
				case ItemType_Crossbow:	        //109代表箭术弓类武器
				case ItemType_Fan:	   	        //110代表美女扇类
				case ItemType_Stick:	        //111代表召唤棍类武器
				case ItemType_Flower:			//120代表鲜花,采集手套...
					{
						if(zMisc::selectByPercent(10))
						{
							if (container[i] && container[i]->base->recast && container[i]->data.dur<=50)
								pThis->petAutoRepair(container[i]);

							if(!reduceDur(pThis , i))
							{
								bret = true;
							}
						}
					}
					break;
					/*
				case ItemType_Necklace:
				case ItemType_Fing:
					{
						jewelry ++;
						if(jewelry <= 3)
						{
							which[jewelry - 1] = i;
						}
					}
					break;
					// */
				default:
					break;
			}
		}
	}

	/*
	for(int ii = 0 ; ii < jewelry ; ii ++)
	{
		if(zMisc::selectByPercent(60))
		{
			if(!reduceDur(pThis , which[ii]))
			{
				bret = true;
			}
		}
	}
	// */
	/*
	switch(jewelry)
	{
		case 1:
			{
				if(!reduceDur(pThis , which[jewelry -1]))
				{
					bret = true;
				}
			}
			break;
		case 2:
			{
				jewelry -= zMisc::randBetween(0 , 1);
				if(!reduceDur(pThis , which[jewelry - 1]))
				{
					bret = true;
				}
			}
			break;
		case 3:
			{
				jewelry -= zMisc::randBetween(0 , 2);
				if(!reduceDur(pThis , which[jewelry - 1]))
				{
					bret = true;
				}
			}
			break;
		default:
			break;
	}
	// */
	if(bret)
	{
		freshUserData(pThis);
	}
	return bret;
}

/**
 * \brief 宠物攻击消耗攻击性装备耐久度
 * \param pThis 角色
 */
bool EquipPack::costAttackDurByPet(SceneUser *pThis)
{
	bool bret = false;
	//int jewelry = 0;
	//int which[3] ={0};
	//
	for (int i=0;i<16;i++)
	{
		if(container[i]!=NULL && container[i]->data.dur != 0)
		{
			switch(container[i]->base->kind)
			{
				case ItemType_Stick:	        //111代表召唤棍类武器
					{
						if(zMisc::selectByPercent(10))
						{
							if (container[i])
							{
								if (container[i]->base->level <= 70) continue;
								if (container[i]->data.dur<=50 && container[i]->base->recast)
									pThis->petAutoRepair(container[i]);
							}

							if(!reduceDur(pThis , i))
							{
								bret = true;
							}
						}
					}
					break;
				default:
					break;
			}
		}
	}

	if(bret)
	{
		freshUserData(pThis);
	}
	return bret;
}

/**
 * \brief 消耗防御性装备耐久度
 * \param pThis 角色
 */
bool EquipPack::costDefenceDur(SceneUser *pThis)
{
	bool bret = false;
	int clothing = 0;
	int which[5] = {0};
	for (int i=0;i<16;i++)
	{
		if(container[i]!=NULL && container[i]->data.dur != 0)
		{
			switch(container[i]->base->kind)
			{
				case ItemType_ClothBody:		//101代表布质加生命类服装
				case ItemType_FellBody:		    //102代表皮甲加魔防类服装
				case ItemType_MetalBody:		//103代表金属铠甲加物防类服装
				case ItemType_Helm:		//113代表角色头盔类
				case ItemType_Caestus:	//114代表角色腰带类
				case ItemType_Cuff:		//115代表角色护腕类
				case ItemType_Shoes:		//116代表角色鞋子类			
					{
						/*
						if(clothing < 5)
						{
							which[clothing] = i;
						}
						// */
						which[clothing] = i;
						clothing ++;
					}
					break;
				case ItemType_Shield:
				case ItemType_Necklace:
				case ItemType_Fing:
					{
						if(zMisc::selectByPercent(3))
						{
							if (container[i] && container[i]->base->recast && container[i]->data.dur<=50)
								pThis->petAutoRepair(container[i]);
							if(!reduceDur(pThis , i))
							{
								bret = true;
							}
						}
					}
					break;
				default:
					break;
			}
		}
	}
	for(int ii=0 ; ii < clothing ; ii ++)
	{
		if(zMisc::selectByPercent(3))
		{
			if (container[which[ii]] && container[which[ii]]->base->recast && container[which[ii]]->data.dur<=50)
				pThis->petAutoRepair(container[which[ii]]);

			if(!reduceDur(pThis , which[ii]))
			{
				bret = true;
			}
		}
	}
	/*
	switch(clothing)
	{
		case 5:
			{
				int except_1 = zMisc::randBetween(0 , 4);
				int except_2 = zMisc::randBetween(0 , 4);
				while(except_2 == except_1)
				{
					except_2 = zMisc::randBetween(0 , 4);
				}
				for(int i = 0 ; i < clothing ; i ++)
				{
					if(i != except_1 && i != except_2)
					{
						if(!reduceDur(pThis , which[i]))
						{
							bret = true;
						}
					}
				}
			}
			break;
		case 4:
			{
				int except = zMisc::randBetween(0 , 3);
				for(int i = 0 ; i < clothing ; i ++)
				{
					if(i != except)
					{
						if(!reduceDur(pThis , which[i]))
						{
							bret = true;
						}
					}
				}
			}
			break;
		case 3:
		case 2:
		case 1:
			{
				for(int i = 0 ; i < clothing ; i ++)
				{
					if(!reduceDur(pThis , which[i]))
					{
						bret = true;
					}
				}
			}
			break;
		default:
			break;
	}
	// */
	if(bret)
	{
		freshUserData(pThis);
	}
	return bret;
}

/**
 * \brief 宠物攻击消耗防御性装备耐久度
 * \param pThis 角色
 */
bool EquipPack::costDefenceDurByPet(SceneUser *pThis)
{
	bool bret = false;

	for (int i=0;i<16;i++)
	{
		if(container[i]!=NULL && container[i]->data.dur != 0)
		{
			switch(container[i]->base->kind)
			{
				case ItemType_Necklace:
				case ItemType_Fing:
					{
						if(zMisc::selectByPercent(3))
						{
							if (container[i])
							{
								if (container[i]->base->level <=70) continue;
								if (container[i]->data.dur<=50 && container[i]->base->recast)
									pThis->petAutoRepair(container[i]);
							}
							if(!reduceDur(pThis , i))
							{
								bret = true;
							}
						}
					}
					break;
				default:
					break;
			}
		}
	}

	if(bret)
	{
		freshUserData(pThis);
	}
	return bret;
}

/**
 * \brief 获取虚拟装备
 * \return 虚拟装备对象
 */
const Equips& EquipPack::getEquips() const
{
	return equips;
}

#define CALCUTE(prop) equips.prop += container[i]->data.prop;
 
/**
 * \brief 重算虚拟装备数值
 */
void EquipPack::calcAll() 
{
	bool calcmdamage;
	equips.reset();
//	Zebra::logger->debug("reset");
	typedef std::map<std::string,int> SuitNmaeMap;
	typedef SuitNmaeMap::iterator SuitNameMap_iter;
	SuitNmaeMap suitmap[3];
	std::pair<int,int> suitnum;
	suitnum.first=-1;
	suitnum.second=0;

	calcmdamage = true;
	if (container[position(0,Cmd::EQUIPCELLTYPE_HANDR)])
	{
		if (container[position(0,Cmd::EQUIPCELLTYPE_HANDR)]->base->kind == ItemType_Stick)
		{
			calcmdamage = false;
		}
	}
	for (int i=0;i<16;i++)
	{
		if(container[i]!=NULL && container[i]->data.dur != 0)
		{
			//如果是时装,生命值增加5%
			if(container[i]->base->kind == ItemType_FashionBody)
			{
				equips.maxhprate +=10;
				equips.maxmprate +=10;
			}
			if(container[i]->data.bind && container[i]->data.maker[0] && container[i]->base->kind != ItemType_Shield)
			{
				int color = -1;
				if(container[i]->data.kind & 1)
				{
					color = 0;
				}
				if (container[i]->data.kind & 2)
				{
					color = 1;
				}
				if (container[i]->data.kind & 4)
				{
					color = 2;
				}
				if(color != -1)
				{
					SuitNameMap_iter iter = suitmap[color].find(container[i]->data.maker);
					if(iter != suitmap[color].end())
					{
						iter->second ++;
						if( iter->second >=6 )
						{
							suitnum.first = color;
							suitnum.second = iter->second;
						}
					}
					else
					{
						suitmap[color].insert(std::make_pair(container[i]->data.maker,1));
					}
				}
			}
			CALCUTE(maxhp)					// 最大生命值
			CALCUTE(maxmp)					// 最大法术值
			CALCUTE(maxsp)					// 最大体力值

			if (container[i]->base->kind != ItemType_Crossbow || (container[i]->base->kind == ItemType_Crossbow && equip(HANDR)/*arrow must*/ )) {
				CALCUTE(pdamage)				// 最小攻击力
				CALCUTE(maxpdamage)				// 最大攻击力
				CALCUTE(damagebonus)			// 伤害加成
			}

			if (calcmdamage)
			{
				CALCUTE(mdamage)				// 最小法术攻击力
				CALCUTE(maxmdamage)				// 最大法术攻击力
			}
			else
			{
				equips.appendminpet+=container[i]->data.mdamage;
				equips.appendmaxpet+=container[i]->data.maxmdamage;
			}
		
			CALCUTE(pdefence)				// 物防
			CALCUTE(mdefence)				// 魔防
			CALCUTE(damage)					// 增加伤害值x％
			
			CALCUTE(akspeed)				// 攻击速度
			CALCUTE(mvspeed)				// 移动速度
			CALCUTE(atrating)				// 命中率
			CALCUTE(akdodge)				// 躲避率
		
			CALCUTE(str)				  	// 力量
			CALCUTE(inte)  					// 智力
			CALCUTE(dex)  					// 敏捷
			CALCUTE(spi)  					// 精神
			CALCUTE(con)  					// 体质
			
			CALCUTE(hpr)					// 生命值恢复
			CALCUTE(mpr)  					// 法术值恢复
			CALCUTE(spr)  					// 体力值恢复
			
			CALCUTE(holy) 					//神圣一击
			CALCUTE(bang) 					//重击
			CALCUTE(pdam) 					// 增加物理攻击力
			CALCUTE(pdef)  					// 增加物理防御力
			CALCUTE(mdam)  					// 增加魔法攻击力
			CALCUTE(mdef)  					// 增加魔法防御力
			
			CALCUTE(poisondef) 				//抗毒增加
			CALCUTE(lulldef) 				//抗麻痹增加
			CALCUTE(reeldef) 				//抗眩晕增加
			CALCUTE(evildef) 				//抗噬魔增加
			CALCUTE(bitedef) 				//抗噬力增加
			CALCUTE(chaosdef) 				//抗混乱增加
			CALCUTE(colddef) 				//抗冰冻增加
			CALCUTE(petrifydef) 			//抗石化增加
			CALCUTE(blinddef) 				//抗失明增加
			CALCUTE(stabledef) 				//抗定身增加
			CALCUTE(slowdef)				 //抗减速增加
			CALCUTE(luredef) 				//抗诱惑增加
			
			CALCUTE(poison) 				//中毒增加
			CALCUTE(lull) 					//麻痹增加
			CALCUTE(reel) 					//眩晕增加
			CALCUTE(evil) 					//噬魔增加
			CALCUTE(bite)					//噬力增加
			CALCUTE(chaos) 					//混乱增加
			CALCUTE(cold) 					//冰冻增加
			CALCUTE(petrify) 				//石化增加
			CALCUTE(blind) 					//失明增加
			CALCUTE(stable) 				//定身增加
			CALCUTE(slow) 					//减速增加
			CALCUTE(lure) 					//诱惑增加
			CALCUTE(hpleech.odds) 
			CALCUTE(hpleech.effect) 
			CALCUTE(mpleech.odds) 			//x%吸收生命值y
			CALCUTE(mpleech.effect) 		//x%吸收法术值y

			CALCUTE(hptomp)					//转换生命值为法术值x％
			CALCUTE(dhpp) 					//物理伤害减少x%	
			CALCUTE(dmpp)					//法术伤害值减少x%		
		
			CALCUTE(incgold)				//增加银子掉落x%
			CALCUTE(doublexp)				//x%双倍经验		
			if (container[i]->base->kind == ItemType_DoubleExp) {
				equips.doublexp=100;				//x%双倍经验		
			}
			CALCUTE(mf)						 //增加掉宝率x%

			switch ( container[i]->base->kind) 
			{
				case ItemType_Blade:		    //104代表武术刀类武器
				case ItemType_Sword :	        //105代表武术剑类武器
				case ItemType_Axe:	           //106代表武术斧类武器
				case ItemType_Hammer:	        //107代表武术斧类武器
				case ItemType_Staff:		    //108代表法术杖类武器
				case ItemType_Crossbow:	        //109代表箭术弓类武器
				case ItemType_Fan:	           //110代表美女扇类
				case ItemType_Stick:	        //111代表召唤棍类武器			
					equips.aftype = container[i]->data.fivetype;
					equips.afpoint = container[i]->data.fivepoint;

					break;
				case ItemType_Necklace:	//117代表角色项链类
				case ItemType_Fing:		//118代表角色戒指类
					//note: be careful, weapon must be computed before this
					if (container[i]->data.fivetype == equips.aftype) {
						equips.afpoint += container[i]->data.fivepoint;
					}
					break;
				case ItemType_ClothBody:		//101代表布质加生命类服装
				case ItemType_FellBody:		    //102代表皮甲加魔防类服装
				case ItemType_MetalBody:		//103代表金属铠甲加物防类服装
					equips.dftype = container[i]->data.fivetype;
					equips.dfpoint = container[i]->data.fivepoint;

					break;
				case ItemType_Shield:	//112代表盾牌类
				case ItemType_Helm:		//113代表角色头盔类
				case ItemType_Caestus:	//114代表角色腰带类
				case ItemType_Cuff:		//115代表角色护腕类
				case ItemType_Shoes:		//116代表角色鞋子类
					//note: be careful, armor must be computed before this
					if (container[i]->data.fivetype == equips.dftype) {
						equips.dfpoint += container[i]->data.fivepoint;
					}
					break;
			}
			
			int j = 0;
		
			while (j<10 && container[i]->data.skill[j].id && equips.getMaxSkill(container[i]->data.skill[j].id) < container[i]->data.skill[j].point) {
				equips.skill[container[i]->data.skill[j].id] = container[i]->data.skill[j].point;
				++j;
			}

			if (container[i]->data.skills.id && equips.getMaxSkill(container[i]->data.skills.id) < container[i]->data.skills.point) {
				equips.skills[container[i]->data.skills.id] = container[i]->data.skills.point;
			}
			
			int index = 0;
			int found[16];
			memset(found, 0, sizeof(int)*16);
			
			if (container[i]->data.fivetype != FIVE_NONE) {
				for (int j=0; j<16; ++j)	{
/*
					if (container[j] && container[j]->data.fivetype != FIVE_NONE) {
						Zebra::logger->debug("!!![%s]\t[%d]", container[j]->data.strName, j);
					}
*/
					if ( i!=j && container[j]!=NULL && container[j]->data.dur != 0 && container[i]->data.fiveset[index]==container[j]->data.dwObjectID  && !found[j] &&
						( ((container[i]->data.fivetype+3*(index+1))%5) == container[j]->data.fivetype) ) {				
/*						
							switch (index) 
							{
								case 0:
									CALCUTE(dpdam)			//物理伤害减少%x
									break;
								case 1:
									CALCUTE(dmdam)			//法术伤害减少%x
									break;
								case 2:
									CALCUTE(bdam)				//增加伤害x%
									break;
								case 3:
									CALCUTE(rdam)				//伤害反射%x
									break;
								case 4:
									CALCUTE(ignoredef)		//%x忽视目标防御
									break;
							}
*/
							int k = 0;
							while (k < 5) {
								if (container[i]->data._five_props[k] ) {
									if (k>=index) break;
								}
								++k;
							}
							equips._five_props[k] += container[i]->data._five_props[k];
//							Zebra::logger->debug("found");																
							++index;
							found[j] = 1;
							j = 0; //loop again
					}
				}				
			}
		}
	}
	switch(suitnum.first)
	{
		case 0:
			{
				if(suitnum.second >= 10)
				{
					equips.pdam+=5; 					// 增加物理攻击力
					equips.pdef+=5;  					// 增加物理防御力
					equips.mdam+=5;  					// 增加魔法攻击力
					equips.mdef+=5;  					// 增加魔法防御力
					equips.maxhprate +=8;
					equips.maxmprate +=8;
				}
				else
				{
					equips.pdam+=2; 					// 增加物理攻击力
					equips.pdef+=2;  					// 增加物理防御力
					equips.mdam+=2;  					// 增加魔法攻击力
					equips.mdef+=2;  					// 增加魔法防御力
					equips.maxhprate +=4;
					equips.maxmprate +=4;
				}
			}
			break;
		case 1:
			{
				if(suitnum.second >= 10)
				{
					equips.pdam+=5; 					// 增加物理攻击力
					equips.pdef+=5;  					// 增加物理防御力
					equips.mdam+=5;  					// 增加魔法攻击力
					equips.mdef+=5;  					// 增加魔法防御力
					equips.maxhprate +=8;
					equips.maxmprate +=8;
				}
				else
				{
					equips.pdam+=2; 					// 增加物理攻击力
					equips.pdef+=2;  					// 增加物理防御力
					equips.mdam+=2;  					// 增加魔法攻击力
					equips.mdef+=2;  					// 增加魔法防御力
					equips.maxhprate +=4;
					equips.maxmprate +=4;
				}
			}
			break;
		case 2:
			{
				if(suitnum.second >= 10)
				{
					equips.pdam+=5; 					// 增加物理攻击力
					equips.pdef+=5;  					// 增加物理防御力
					equips.mdam+=5;  					// 增加魔法攻击力
					equips.mdef+=5;  					// 增加魔法防御力
					equips.maxhprate +=8;
					equips.maxmprate +=8;
				}
				else
				{
					equips.pdam+=2; 					// 增加物理攻击力
					equips.pdef+=2;  					// 增加物理防御力
					equips.mdam+=2;  					// 增加魔法攻击力
					equips.mdef+=2;  					// 增加魔法防御力
					equips.maxhprate +=4;
					equips.maxmprate +=4;
				}
			}
			break;
		default:
			break;
	}
	//Zebra::logger->debug("装备攻击五行属性(%d:%d)，防御五行(%d:%d)", equips.aftype, equips.afpoint, equips.dftype, equips.dfpoint);
}

int EquipPack::position(WORD x, WORD y) const
{
	int pos = x*10 + y;
	if (pos < (int)sizeof(_poses) && _poses[pos] != -1) {
		return _poses[pos];
	}

	return -1;
}		

bool EquipPack::process_extra_add(zObject* ob)
{
	if (ob && (ob->data.pos.ypos() == Cmd::EQUIPCELLTYPE_MAKE || ob->data.pos.ypos() == Cmd::EQUIPCELLTYPE_PACKAGE)) {
		int pos = ((ob->data.pos.ypos() - Cmd::EQUIPCELLTYPE_PACKAGE) << 1)  + ob->data.pos.xpos();
		assert(pos >= 0 && pos <4);
		SAFE_DELETE(packs[pos]);
		packs[pos] = new ObjectPack(ob, ob->data.pos.ypos() -1 );
		return true;
	}

	return false;
}

bool EquipPack::process_extra_remove(zObject* ob)
{
	if (ob && (ob->data.pos.ypos() == Cmd::EQUIPCELLTYPE_MAKE || ob->data.pos.ypos() == Cmd::EQUIPCELLTYPE_PACKAGE)) {
		int pos = ((ob->data.pos.ypos() - Cmd::EQUIPCELLTYPE_PACKAGE) << 1)  + ob->data.pos.xpos();
		assert(pos >= 0 && pos <4);
		SAFE_DELETE(packs[pos]);
		return true;
	}

	return false;
}

bool EquipPack::process_extra_get(zObject** ob, WORD x, WORD y)
{
	if (y == Cmd::EQUIPCELLTYPE_MAKE ||y == Cmd::EQUIPCELLTYPE_PACKAGE) {
		int pos = ((y - Cmd::EQUIPCELLTYPE_PACKAGE) << 1)  + x;
		if (pos<4) {
			if (packs[pos] && packs[pos]->object() ) *ob = packs[pos]->object();
			return true;
		}
	}

	return false;
}

ObjectPack* EquipPack::pack(PACKNO no) const
{
	return packs[no];
}

zObject* EquipPack::equip(EQUIPNO no) const
{
	return container[no];
}

const int EquipPack::_poses[] = { 	-1, 	1, 	0, 	3, 	2, 	5, 	8, 	6, 	4, 	10,
									11, 	16,	18,	13,	-1,	-1,	9,	7,	-1,	-1,
									12, 	17,	19,	14,	-1,	-1,	-1,	-1,	-1,	-1,
									13, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
									14, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
									15, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1};
/**
 * \brief 添加装备
 * \param object 物品对象
 * \param find 是否自动寻找位置
 * \return 添加成功返回true 否则返回false
 */
bool EquipPack::add(zObject *object ,bool find)
{
	if(process_extra_add(object)) return Package::add(object, false);
		
	if (Package::add(object, false))
	{
		if (owner&&object &&(11 <= object->data.upgrade))
		{
			effectCount++;
			if (1==effectCount)	owner->showCurrentEffect(Cmd::USTATE_ULTRA_EQUIPMENT, true); // 更新客户端状态
		}
		calcAll();
		needRecalc=true;
		return true;
	}
	return false;
}

/**
 * \brief 删除装备
 * \param object 物品对象
 */
bool EquipPack::remove(zObject *object)
{
	if (process_extra_remove(object)) return Package::remove(object);

	bool ret = Package::remove(object);
	calcAll();
	if (owner&&object &&(11 <= object->data.upgrade))
	{
		if (effectCount>0)
		{
			effectCount--;
			if (0>=effectCount)	owner->showCurrentEffect(Cmd::USTATE_ULTRA_EQUIPMENT, false); // 更新客户端状态
		}
	}
	if(ret && object && object->base->kind == ItemType_Amulet && owner->scene->isIncScene())
	{
		std::ostringstream os;
		os << "name=" << object->data.maker;
		os << " pos=" << object->data.durpoint << "," << object->data.dursecond;
		bzero(object->data.maker,sizeof(object->data.maker));
		object->data.durpoint=0;
		object->data.dursecond=0;
		Cmd::stAddObjectPropertyUserCmd ret;
		ret.byActionType = Cmd::EQUIPACTION_REFRESH;
		bcopy(&object->data, &ret.object, sizeof(t_Object));
		owner->sendCmdToMe(&ret, sizeof(ret));				
		Gm::gomap(owner, os.str().c_str());
	}
	needRecalc=true;

	return ret;
}

/**
 * \brief 检查物品
 * \param pUser 角色
 * \param object 物品对象
 * \param x 横坐标
 * \param y 纵坐标
 * \return true 装备有此物品
 */
bool EquipPack::checkAdd(SceneUser *pUser,zObject *ob,WORD x,WORD y)
{
	if (ob && ob->base->setpos == y && (y == Cmd::EQUIPCELLTYPE_MAKE || y == Cmd::EQUIPCELLTYPE_PACKAGE)) {
		int pos = ((y- Cmd::EQUIPCELLTYPE_PACKAGE) << 1)  + x;
		if (pos >= 0 && pos < 4 && packs[pos] && !packs[pos]->empty()) return false;
	}
	
	zObject* tmp;
	bool ret = getObjectByZone(&tmp, x, y);
	if (ob==NULL) return true;
	
	using namespace Cmd;
	if(!(pUser->charbase.type&ob->base->sex) || pUser->charbase.level<ob->data.needlevel)
		return false;

	if (ob->base->setpos && ob->base->setpos == y) 
	{
		switch(ob->base->kind)
		{
			case ItemType_DoubleExp:
				{
					if(ob->data.dur<= 60)
					{
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "剩余时间太短,不能装备"); 
						return false;
					}
					if(doubleexp_obj_time/86400 == SceneTimeTick::currentTime.sec()/86400)
					{
						if(doubleexp_obj != ob->data.qwThisID)
						{
							Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "一天只能使用一个双倍经验物品！");
							return false;
						}
					}
					else
					{
						doubleexp_obj = ob->data.qwThisID;
						doubleexp_obj_time = SceneTimeTick::currentTime.sec(); 
					}
				}
				break;
			case ItemType_FashionBody:
				{
					if(ob->data.dur<= 1)
					{
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "剩余时间太短,不能装备"); 
						return false;
					}
				}
				break;
			case ItemType_Tonic:
			case ItemType_Amulet:
				{
					if(ob->data.dur<= 60)
					{
						Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "剩余时间太短,不能装备"); 
						return false;
					}
				}
				break;
			default:
				break;
		}
		//双倍经验道具一天用一次
		if ( (ob->base->setpos == EQUIPCELLTYPE_HANDL && container[HANDR] && container[HANDR]->base->kind != ob->base->needobject && ob->base->kind != container[HANDR]->base->needobject) ||(ob->base->setpos == EQUIPCELLTYPE_HANDR && container[HANDL]  && container[HANDL]->base->kind  != ob->base->needobject && ob->base->kind != container[HANDL]->base->needobject ) ) 
		{
			
			zObject* obj = container[HANDR] ;
			if (ob->base->setpos == EQUIPCELLTYPE_HANDR) obj = container[HANDL] ;

			if (!obj) return ret;
			
			if (pUser->packs.uom.space(pUser) > 0 ) {
				pUser->packs.removeObject(obj, true, false); //notify but not delete

/*				
				obj->data.pos.dwLocation = Cmd::OBJECTCELLTYPE_COMMON;
				pUser->packsaddObject(obj, true);
*/
				pUser->packs.addObject(obj, true, AUTO_PACK);

				Cmd::stAddObjectPropertyUserCmd ret1;
				ret1.byActionType = Cmd::EQUIPACTION_OBTAIN;
				bcopy(&obj->data, &ret1.object, sizeof(t_Object));
				pUser->sendCmdToMe(&ret1, sizeof(ret1));

/*			
				if (ob->base->setpos == EQUIPCELLTYPE_HANDL) {
					container[HANDR] = NULL;
				}else {
					container[HANDL] = NULL;
				}
*/
				return ret;
			}
			return false;
		}
						
		return ret;
	}
	
	return false;
}

/**
 * \brief 根据位置大小获取物品
 * \param ret 返回的物品对象
 * \param x 横坐标
 * \param y 纵坐标
 * \param width 宽度
 * \param height 高度
 * \return true 成功返回
 */
bool EquipPack::getObjectByZone(zObject **ret,WORD x,WORD y)
{
	if (process_extra_get(ret, x, y)) return true;
	
	return Package::getObjectByZone(ret, x, y);
}

/**
 * \brief 经验绑定
 * \param user 角色
 * \param exp 绑定到装备身上的经验数量
 * \param force 不判断几率强制绑定
 */
void EquipPack::obtain_exp(SceneUser* user, DWORD exp, bool force)
{
	if (force || zMisc::selectByPercent(20)) {
		int index = zMisc::randBetween(0, 15);
		int current = index;
		do {
			if (container[current] != NULL)
			{
				switch ( container[current]->base->kind)
				{
					case ItemType_Blade:		    //104代表武术刀类武器
					case ItemType_Sword :	        //105代表武术剑类武器
					case ItemType_Axe:	           //106代表武术斧类武器
					case ItemType_Hammer:	        //107代表武术斧类武器
					case ItemType_Staff:		    //108代表法术杖类武器
					case ItemType_Crossbow:	        //109代表箭术弓类武器
					case ItemType_Fan:	           //110代表美女扇类
					case ItemType_Stick:	        //111代表召唤棍类武器			
					case ItemType_Necklace:	//117代表角色项链类
					case ItemType_Fing:		//118代表角色戒指类
					case ItemType_ClothBody:		//101代表布质加生命类服装
					case ItemType_FellBody:		    //102代表皮甲加魔防类服装
					case ItemType_MetalBody:		//103代表金属铠甲加物防类服装
					case ItemType_Shield:	//112代表盾牌类
					case ItemType_Helm:		//113代表角色头盔类
					case ItemType_Caestus:	//114代表角色腰带类
					case ItemType_Cuff:		//115代表角色护腕类
					case ItemType_Shoes:		//116代表角色鞋子类
						{
							container[current] ->data.exp += static_cast<int>(exp*0.05);
							Cmd::stObjectExpUserCmd cmd;
							cmd.id = container[current]->data.qwThisID;
							cmd.exp = container[current]->data.exp;
							user->sendCmdToMe(&cmd , sizeof(cmd));
						}
						break;
					default:
						break;
				}
				break;
			}

		}while ( ( current = (++current % 16) ) != index ) ;
	}		
}

/**
 * \brief 箭支消耗数量是以耐久度计算的,当耐久为0时删除箭桶
 * \param pThis 主人
 * \param kind 物品类型
 * \param num 消耗数量
 * \author whj 
 * \return 消耗是否成功
 */
bool EquipPack::skillCheckReduceObject(SceneUser* pThis, DWORD kind, WORD num)
{
/*
	std::set<zObject *>::iterator it = allset.begin();
	int count=0;
	while (it != allset.end()) 
*/
	int count=0;	

	for (int i=0; i<16; ++i) 
	{
		if (!container[i]) continue;
		
		switch(kind)
		{
			case BOW_ARROW_ITEM_TYPE:
				{
					if ( container[i]->base->kind == kind) 
					{
						if((short)container[i]->data.dur < num)
						{
							return false;
						}
						else
						{
							return true;
						}
					}
					break;
				}
			default:
				{
					if ( container[i]->base->kind == kind) 
					{
						count++;
					}
				}
		}
//		++it;		
	}
	if (count>=num) return true;
	return false;
}

/**
 * \brief 物品消耗处理（用于技能的物品消耗）
 * \param pThis 主人
 * \param kind 物品的类型
 * \param num 消耗物品的数目
 * \author whj 
 * \return 消耗是否成功
 */
bool EquipPack::skillReduceObject(SceneUser* pThis, DWORD kind, WORD num)
{
//	std::set<zObject *>::iterator it = allset.begin();
	int count =0;
//	while (it != allset.end()) 
	for (int i=0; i<16; ++i) 
	{
		if (!container[i]) continue;
		
		switch(kind)
		{
			case BOW_ARROW_ITEM_TYPE:
				{
					if ( container[i]->base->kind == kind) 
					{
						if((short)container[i]->data.dur <= num)
						{
/*						
							zObject * obj = *it;
							Cmd::stRemoveObjectPropertyUserCmd rm;
							rm.qwThisID=obj->data.qwThisID;
							pThis->sendCmdToMe(&rm,sizeof(rm));
							zObject::logger((*it)->createid,(*it)->data.qwThisID,(*it)->data.strName,(*it)->data.dwNum,pThis->id,pThis->name,0,NULL,"技能消耗");
							pThis->packs.rmObject(obj);
							SAFE_DELETE(obj);
*/
							zObject::logger(container[i]->createid,container[i]->data.qwThisID,container[i]->data.strName,container[i]->data.dwNum,1,0,pThis->id,pThis->name,0,NULL,"技能消耗",NULL,0,0);
							pThis->packs.removeObject(container[i]);
							return true;
						}
						else
						{
							container[i]->data.dur -=num;
							Cmd::stDurabilityUserCmd std;
							std.dwThisID = container[i]->data.qwThisID;
							std.dwDur = container[i]->data.dur;
							pThis->sendCmdToMe(&std , sizeof(std));
							//Zebra::logger->debug("消耗箭支id=%u,thisid=%u,dur=%u",id ,std.dwThisID , std.dwDur);
							return true;
						}
					}
					break;
				}
			default:
				{
					if ( container[i]->base->kind == kind) 
					{
/*
						std::set<zObject *>::iterator temp = it;
						it++;
						zObject * obj = *temp;
						Cmd::stRemoveObjectPropertyUserCmd rm;
						rm.qwThisID=obj->data.qwThisID;
						pThis->sendCmdToMe(&rm,sizeof(rm));
						zObject::logger((*it)->createid,(*it)->data.qwThisID,(*it)->data.strName,(*it)->data.dwNum,pThis->id,pThis->name,0,NULL,"技能消耗");
						count++;
						pThis->packs.rmObject(obj);
						SAFE_DELETE(obj);
*/						
						zObject::logger(container[i]->createid,container[i]->data.qwThisID,container[i]->data.strName,container[i]->data.dwNum,1,0,pThis->id,pThis->name,0,NULL,"技能消耗",NULL,0,0);
						pThis->packs.removeObject(container[i]);
						if (count >= num) return true;
						continue;
					}
				}
		}
		//++it;		
	}
	if (count>=num) return true;
	
	return false;
}


/**
 * \brief 构造函数
 * \param ob 物品对象
 * \param type 类型
 */
ObjectPack::ObjectPack(zObject* ob, int type, bool consume)	:
	Package(type, ob->data.qwThisID, ob->data.maxpdamage, ob->data.maxmdamage), 
	_ob(ob), _cosume_by_time(consume), _one_min(60, SceneTimeTick::currentTime)
{
	if (_ob && _ob->data.maxdur ==0) //应对之前没有耐久的物品
	{
		_ob->data.maxdur = 43200;
		_ob->data.dur = 43200;
	}
	if (ob->base->setpos == Cmd::EQUIPCELLTYPE_PACKAGE  && !_ob->data.bind) {
		_ob->data.bind = 1;
		_ob->data.color = time(NULL);
	}
}

/**
 * \brief 析构函数
 */
ObjectPack::~ObjectPack()
{
	//SAFE_DELETE(_ob);
}

/*
bool ObjectPack::add(zObject *object ,bool find)
{
	if (find && (!_ob || _ob->data.dur == 0)) {
		//only can takeout
		return false;
	}
	return Package::add(object, find);
}
// */
bool ObjectPack::checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y)
{
	if (!_ob || _ob->data.dur == 0) {
		//only can takeout
		return object == NULL;
	}
	if(object && _ob->data.qwThisID == object->data.qwThisID)
	{
		Zebra::logger->debug("把自己添加到自己的包裹%s(%d),%s(%u)",pUser->name,pUser->id,object->data.strName,object->data.qwThisID);
		return false;
	}

	return Package::checkAdd(pUser, object, x, y);
}

void ObjectPack::consume_dur_by(SceneUser* user, const zRTime& current)
{
/*
	if (_cosume_by_time && _ob && _ob->data.dur > 0 && _one_min(current) ) {
		
		--_ob->data.dur;
		Cmd::stDurabilityUserCmd cmd;
		cmd.dwThisID = _ob->data.qwThisID;
		cmd.dwDur = _ob->data.dur;
		user->sendCmdToMe(&cmd , sizeof(cmd));
	}
*/
}

/**
 * \brief 构造函数初始化人物主包裹
 */
MainPack::MainPack():Package(Cmd::OBJECTCELLTYPE_COMMON,0,MainPack::WIDTH,MainPack::HEIGHT)
{
	gold=NULL;
}

/**
 * \brief 析构函数
 */
MainPack::~MainPack()
{
	SAFE_DELETE(gold);
}

/**
 * \brief 向包裹中添加对象
 * \param object 物品对象
 * \param find 寻找标志
 * \return true 添加成功 ， false 添加失败
 */
bool MainPack::add(zObject *object ,bool find)
{
	if(object && object->base->kind==ItemType_Money)
	{
		if(gold)
		{
			return false;
		}
		else
		{
			gold=object;
/*
			gold->data.pos.dwLocation=Cmd::OBJECTCELLTYPE_COMMON;
			gold->data.pos.dwTableID=0;
			gold->data.pos.x=(WORD)-1;
			gold->data.pos.y=(WORD)-1;
*/
			gold->data.pos = stObjectLocation(Cmd::OBJECTCELLTYPE_COMMON, 0, (WORD)-1, (WORD)-1);
			return true;
		}

	}
	else
		return Package::add(object, find);
}

/**
 * \brief 从包裹中删除物品
 * \param object  物品对象
 */
bool MainPack::remove(zObject *object)
{
	if(object && object->base->kind==ItemType_Money)
	{
		if(gold==object) gold=NULL;
		return true;
	}
	else
		return Package::remove(object);
}

/**
 * \brief 检查并添加
 * \param pUser 角色
 * \param object 物品
 * \param x,y 坐标
 * \return true 有该物品，false 没有
 */
bool MainPack::checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y)
{
	if(object && object->base->kind==ItemType_Money)
		return (gold==NULL);
	else
		return  Package::checkAdd(pUser,object,x,y);
}

/**
 * \brief 根据物品位置和大小获取物品
 * \param ret 返回找到的物品对象
 * \param x 横坐标
 * \param y 纵坐标
 * \param width 宽度
 * \param height 高度
 * \return true 有此物品 false 无此物品
 */
bool MainPack::getObjectByZone(zObject **ret,WORD x,WORD y)
{
	if(x==(WORD)-1 && y==(WORD)-1)
	{
		*ret=gold;
		return true;
	}
	else
		return  Package::getObjectByZone(ret,x,y);
}

/**
 * \brief 获得金子数目
 * \return 金子树目
 */
DWORD MainPack::getGoldNum()
{
	if(gold)
	{
		return gold->data.dwNum;
	}
	else
	{
		return 0;
	}
}

/**
 * \brief 获得金子对象
 * \return  物品对象或NULL
 */
zObject * MainPack::getGold()
{
	return gold;
}

/**
 * \brief 箭支消耗数量是以耐久度计算的,当耐久为0时删除箭桶
 * \param pThis 主人
 * \param id 物品的objectid
 * \param num　技能消耗物品的数量
 * \author whj 
 * \return 消耗是否成功
 */
bool MainPack::skillReduceObject(SceneUser* pThis, DWORD id, DWORD num)
{
/*
	std::set<zObject *>::iterator it = allset.begin();
	while (it != allset.end()) 
*/
	for (int i=0; i<16; ++i)
	{
		if (!container[i]) continue;
		
		if ( container[i]->data.dwObjectID == id) 
		{
			if((short)container[i]->data.dur <= 1)
			{
/*
				pThis->packs.rmObject(*it);
				Cmd::stRemoveObjectPropertyUserCmd rm;
				rm.qwThisID=(*it)->data.qwThisID;
				pThis->sendCmdToMe(&rm,sizeof(rm));
*/
				pThis->packs.removeObject(container[i], true, false/*?*/);
				return true;
			}
			else
			{
				container[i]->data.dur -=1;
				Cmd::stDurabilityUserCmd std;
				std.dwThisID = container[i]->data.qwThisID;
				std.dwDur = container[i]->data.dur;
				std.dwMaxDur = container[i]->data.maxdur;
				pThis->sendCmdToMe(&std , sizeof(std));
				//Zebra::logger->debug("消耗箭支id=%u,thisid=%u,dur=%u",id ,std.dwThisID , std.dwDur);
				return true;
			}
			break;
		}
		///++it;		
	}

	return false;
}


/**
 * \brief 仓库构造函数，初始化仓库类型并定义仓库大小
 */
StorePack::StorePack() : Package(Cmd::OBJECTCELLTYPE_STORE, 0,  W, H*3)
{

}

/**
 * \brief 仓库构造函数（空）
 */
StorePack::~StorePack()
{


}

#include "NpcTrade.h"

/**
 * \brief 检查指定位置上是否有指定的对象
 * \param pUser 仓库拥有者
 * \param object 物品对象
 * \param x 横坐标
 * \param y 纵坐标
 * \return true 在指定位置上有指定对象 false 检查失败
 */
bool StorePack::checkAdd(SceneUser * pUser, zObject * object, WORD x, WORD y)
{
	if (!object) return true;

	//check if npc can do this?
	NpcTrade::NpcItem item;
	item.id = object->data.dwObjectID;
	item.kind = object->base->kind;
	item.lowLevel = 0;
	item.level = object->data.needlevel;
	item.action = NpcTrade::NPC_STORE_OBJECT;
	if ( !NpcTrade::getInstance().verifyNpcAction(pUser->npc_dwNpcDataID, item) ) {
		return false;
	}

	//check if npc near by user?
	SceneNpc * npc = SceneNpcManager::getMe().getNpcByTempID(pUser->npc_dwNpcTempID);
	if (!npc  || !npc->scene || npc->scene != pUser->scene 
			|| !pUser->scene->zPosShortRange(pUser->getPos(), npc->getPos(), SCREEN_WIDTH , SCREEN_HEIGHT)
	   	) 	{
		return false;
	}

	BYTE page = y / H;
	if (page < days.size() && days[page]) {
		return Package::checkAdd(pUser, object, x, y);
	}
	return false;
}
void StorePack::goldstore(SceneUser *pThis,bool notify)
{
	//如果已经有两个包裹并且是vip用户则赠送第三个
	if(pThis->packs.store.days.size()<=2 && (pThis->charbase.bitmask & CHARBASE_VIP))
	{
		pThis->packs.store.days.push_back(1);
		if(notify)
		{
			pThis->packs.store.notify(pThis);
			Channel::sendSys(pThis, Cmd::INFO_TYPE_GAME, "恭喜,您获得系统赠送的一个仓库"); 
		}
	}
}

/**
 * \brief 仓库加载
 * \param dest 目标数据
 * \return 包大小
 */
int StorePack::load(unsigned char* dest)
{
	BYTE size = *(BYTE *)dest;
	dest += sizeof(BYTE);
	
	if (size > 0) {
		days.resize(size);
		memcpy(&days[0], dest, sizeof(BYTE)*size);
	}
	
	return sizeof(BYTE) + sizeof(BYTE)*size;
}

/**
 * \brief 仓库存储
 * \param dest 目标数据
 * \return 包大小
 */
int StorePack::save(unsigned char* dest)
{
	BYTE count = days.size();
	memcpy(dest, &count, sizeof(count));
	dest += sizeof(count);
	
	memcpy(dest, &days[0], sizeof(BYTE)*count);
	
	return sizeof(count) + sizeof(BYTE)*count;
}

/**
 * \brief 发送仓库更新
 * \param user 角色
 */
void StorePack::notify(SceneUser* user)
{
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::stStoreInfoNpcTradeUserCmd* info = (Cmd::stStoreInfoNpcTradeUserCmd*)buf;
	constructInPlace(info);
	info->page = days.size();
	memcpy(&info->day[0], &days[0], days.size());
	
	user->sendCmdToMe(info, sizeof(Cmd::stStoreInfoNpcTradeUserCmd) + sizeof(BYTE)*days.size());
}

/**
 * \brief 能否换色
 * \return true 能 false 否
 */
bool EquipPack::canChangeColor()
{
	/// 王霆要求,目的是为了方便通过颜色字段做装备特效
	/*
	if(equip(BODY) && (equip(BODY)->base->color == 0XFFFFFFFF))
	{
		return true;
	}
	else
	{
		return false;
	}
	// */
	return true;
}

/**
 * \brief 构造函数
 */
Packages::Packages(SceneUser* user) : owner(user), equip(user)
{

}

/**
 * \brief  析构函数
 */
Packages::~Packages()
{

}

/**
 * \brief 根据类型获取包裹
 * \param type 包裹类型
 * \param id 目前未使用
 * \return 包裹对象
 */
Package * Packages::getPackage(DWORD type,DWORD id)
{
	switch(type)
	{
		case Cmd::OBJECTCELLTYPE_COMMON:
			return (Package *)&main;
		case Cmd::OBJECTCELLTYPE_EQUIP:
			return (Package *)&equip;
		case Cmd::OBJECTCELLTYPE_STORE:
			return (Package *)&store;
		case Cmd::OBJECTCELLTYPE_PACKAGE:
			if (equip.pack(EquipPack::L_PACK) && equip.pack(EquipPack::L_PACK)->object()->data.qwThisID == id) {
				return (Package *)equip.pack(EquipPack::L_PACK);
			}
			if (equip.pack(EquipPack::R_PACK) && equip.pack(EquipPack::R_PACK)->object()->data.qwThisID == id) {
				return (Package *)equip.pack(EquipPack::R_PACK);
			}
		case Cmd::OBJECTCELLTYPE_MAKE:
			if (equip.pack(EquipPack::L_MAKE) && equip.pack(EquipPack::L_MAKE)->object()->data.qwThisID == id) {
				return (Package *)equip.pack(EquipPack::L_MAKE);
			}
			if (equip.pack(EquipPack::R_MAKE) && equip.pack(EquipPack::R_MAKE)->object()->data.qwThisID == id) {
				return (Package *)equip.pack(EquipPack::R_MAKE);
			}
			break;
		case Cmd::OBJECTCELLTYPE_PET:
			return (Package *)&petPack;
			break;
		default:
			break;
	}
	return NULL;
}

/*
void Packages::clearPackage(Package* pack)
{
	ClearPack cp(this);
	pack->execEvery(cp);
}
*/

Package** Packages::getPackage(int packs)
{
	Package** p = new Package*[8];
	memset(p, 0, 8*sizeof(Package *));

	//notice the sequence
	int i = 0;
	if (packs & MAIN_PACK) p[i++] = (Package *)&main;
	if ((packs & LEFT_PACK) && equip.pack(EquipPack::L_PACK) ) p[i++] =  (Package *)equip.pack(EquipPack::L_PACK);	
	if ((packs & RIGHT_PACK) && equip.pack(EquipPack::R_PACK) ) p[i++] = (Package *)equip.pack(EquipPack::R_PACK);	
	
	if (packs & EQUIP_PACK) p[i++] = (Package *)&equip;
	if (packs & STORE_PACK) p[i++] = (Package *)&store;
	
	if ((packs & LM_PACK) && equip.pack(EquipPack::L_MAKE) ) p[i++] = (Package *)equip.pack(EquipPack::L_MAKE);	
	if ((packs & RM_PACK) && equip.pack(EquipPack::R_MAKE) ) p[i++] = (Package *)equip.pack(EquipPack::R_MAKE);

	if (packs & PET_PACK) p[i++] = (Package *)&petPack;

	return p;
}

/**
 * \brief 获得当前金子数量
 * \return 金子数量
 */
DWORD Packages::getGoldNum()
{
	return main.getGoldNum();
}

/**
 * \brief 获取身上的金子
 * \return 物品对象，或NULL
 */
zObject *Packages::getGold()
{
	return main.getGold();
}

/**
 * \brief 将物品丢到地上
 * \param o 目标物品 
 * \param pos 位置
 * \return true 无聊的返回值
 */
bool Packages::moveObjectToScene(zObject *o , const zPos &pos , DWORD overdue_msecs, const unsigned long dwID)
{
	removeObject(o, true, false); //notify but not delete
	zObject::logger(o->createid,o->data.qwThisID,o->data.strName,o->data.dwNum,o->data.dwNum,0,owner->id,owner->name,owner->scene->id,owner->scene->name,"掉落",o->base,o->data.kind,o->data.upgrade);
	//到地上
	if(!owner->scene->addObject(o , pos , overdue_msecs , dwID))
	{
		Zebra::logger->trace("%s(%ld)凋落的装备添加倒场景失败",
				owner->name,owner->id);
		//zObject::destroy(o);
	}
	return true;
}

/**
 * \brief 移动物品
 * \param pUser 角色对象
 * \param srcObj 被移动对象
 * \param dst 对象的目的位置
 * \return true 移动成功， false 移动失败
 */
bool Packages::moveObject(SceneUser *pUser,zObject *srcObj,stObjectLocation &dst)
{
	using namespace Cmd;

	Package *srcpack = getPackage(srcObj->data.pos.loc(), srcObj->data.pos.tab());
	if (!srcpack)  {
		Zebra::logger->warn("物品%s[%x]定位错误，不能移动", srcObj->name, srcObj);		
		return false;
	}
	if(srcpack->type() == Cmd::OBJECTCELLTYPE_EQUIP)
	{
		if (
				(equip.pack(EquipPack::L_PACK) && 
				 equip.pack(EquipPack::L_PACK)->object()->data.qwThisID == srcObj->data.qwThisID && 
				 !equip.pack(EquipPack::L_PACK)->empty()) ||
				(equip.pack(EquipPack::R_PACK) && 
				 equip.pack(EquipPack::R_PACK)->object()->data.qwThisID == srcObj->data.qwThisID &&
				 !equip.pack(EquipPack::R_PACK)->empty()) ||
				(equip.pack(EquipPack::L_MAKE) && 
				 equip.pack(EquipPack::L_MAKE)->object()->data.qwThisID == srcObj->data.qwThisID &&
				 !equip.pack(EquipPack::L_MAKE)->empty()) ||
				(equip.pack(EquipPack::R_MAKE) && 
				 equip.pack(EquipPack::R_MAKE)->object()->data.qwThisID == srcObj->data.qwThisID &&
				 !equip.pack(EquipPack::R_MAKE)->empty())
		   )
		{
			Zebra::logger->warn("不能移动装有物品的包裹%s(%u),%s(%u)",pUser->name,pUser->id,srcObj->data.strName,srcObj->data.qwThisID);
			return false;
		}
	}


	if (dst.loc() == Cmd::OBJECTCELLTYPE_NONE) {
		zObject::logger(srcObj->createid,srcObj->data.qwThisID,srcObj->base->name,srcObj->data.dwNum,srcObj->data.dwNum,0,0,NULL,pUser->id,pUser->name,"扔东西",srcObj->base,srcObj->data.kind,srcObj->data.upgrade);
		removeObject(srcObj);
		return true;
	}
	
	Package *destpack = getPackage(dst.loc(), dst.tab());
	if(!destpack) return false;

	if(srcpack->type() == Cmd::OBJECTCELLTYPE_STORE && destpack->type() != Cmd::OBJECTCELLTYPE_STORE)
	{
		if (srcObj->data.pos.ypos()>=6 && pUser->isSafety(Cmd::SAFE_THIRD_PACK))
		{// 仓库中,第六行以后,就是第二.三仓库
			return false;
		}
	}

	if(destpack->type() == Cmd::OBJECTCELLTYPE_EQUIP)
	{
		if (
				(equip.pack(EquipPack::L_PACK) && 
				 equip.pack(EquipPack::L_PACK)->object()->data.qwThisID == srcObj->data.qwThisID && 
				 !equip.pack(EquipPack::L_PACK)->empty()) ||
				(equip.pack(EquipPack::R_PACK) && 
				 equip.pack(EquipPack::R_PACK)->object()->data.qwThisID == srcObj->data.qwThisID &&
				 !equip.pack(EquipPack::R_PACK)->empty()) ||
				(equip.pack(EquipPack::L_MAKE) && 
				 equip.pack(EquipPack::L_MAKE)->object()->data.qwThisID == srcObj->data.qwThisID &&
				 !equip.pack(EquipPack::L_MAKE)->empty()) ||
				(equip.pack(EquipPack::R_MAKE) && 
				 equip.pack(EquipPack::R_MAKE)->object()->data.qwThisID == srcObj->data.qwThisID &&
				 !equip.pack(EquipPack::R_MAKE)->empty())
		   )
		{
			Zebra::logger->warn("不能移动装有物品的包裹%s(%u),%s(%u)",pUser->name,pUser->id,srcObj->data.strName,srcObj->data.qwThisID);
			return false;
		}
	}

	zObject *destObj = NULL;
	if(destpack->getObjectByZone(&destObj,dst.xpos(),dst.ypos()) && destObj != srcObj)
	{
		if(destpack->checkAdd(pUser,srcObj,dst.xpos(),dst.ypos()))
		{
			if(srcpack->checkAdd(pUser,destObj,srcObj->data.pos.xpos(),srcObj->data.pos.ypos()))
			{
				//宠物包裹只能拿出不能放入
				if (dst.loc()==Cmd::OBJECTCELLTYPE_PET
						|| (srcObj->data.pos.loc()==Cmd::OBJECTCELLTYPE_PET && destObj))
					return false;

				destpack->remove(destObj);
				if(destObj) destObj->data.pos=srcObj->data.pos;

				srcpack->remove(srcObj);
				srcpack->add(destObj,false);

				srcObj->data.pos=dst;
				destpack->add(srcObj,false);

				if(dst.loc() ==Cmd::OBJECTCELLTYPE_EQUIP)//装备时消耗耐久
				{
					switch(srcObj->base->kind)
					{
						case ItemType_DoubleExp:
							{
								if(srcObj->data.dur> 60)
								{
									if(!pUser->packs.equip.reduceDur(pUser , Cmd::EQUIPCELLTYPE_ADORN , ItemType_DoubleExp , 60 , true , true))
									{
										pUser->packs.equip.reduceDur(pUser , Cmd::EQUIPCELLTYPE_ADORN + 1 , ItemType_DoubleExp , 60 , true , true);
									}
								}
								/*
								   else
								   {
								   Channel::sendSys(&user, Cmd::INFO_TYPE_FAIL, "耐久度太低,不能装备"); 
								   return false;
								   }
								// */
							}
							break;
						case ItemType_Tonic:
							{
								if(srcObj->data.dur> 60)
								{
									if(!pUser->packs.equip.reduceDur(pUser , Cmd::EQUIPCELLTYPE_ADORN , ItemType_Tonic , 60 , true , false))
									{
										pUser->packs.equip.reduceDur(pUser , Cmd::EQUIPCELLTYPE_ADORN + 1 , ItemType_Tonic , 60 , true , false);
									}
								}
								/*
								   else
								   {
								   Channel::sendSys(&user, Cmd::INFO_TYPE_FAIL, "耐久度太低,不能装备"); 
								   return false;
								   }
								// */
							}
							break;
						case ItemType_Amulet:
							{
								if(srcObj->data.dur> 60)
								{
									if(!pUser->packs.equip.reduceDur(pUser , Cmd::EQUIPCELLTYPE_ADORN , ItemType_Amulet , 60 , true , false))
									{
									pUser->packs.equip.reduceDur(pUser , Cmd::EQUIPCELLTYPE_ADORN + 1 , ItemType_Amulet , 60 , true , false);
									}
								}
								/*
								   else
								   {
								   Channel::sendSys(&user, Cmd::INFO_TYPE_FAIL, "耐久度太低,不能装备"); 
								   return false;
								   }
								// */
							}
							break;
						case ItemType_FashionBody:
							{
								if(!pUser->packs.equip.reduceDur(pUser , Cmd::EQUIPCELLTYPE_ADORN , ItemType_FashionBody,1 , true ,false))
								{
								pUser->packs.equip.reduceDur(pUser , Cmd::EQUIPCELLTYPE_ADORN + 1, ItemType_FashionBody,1 , true ,false);
								}
							}
							break;
						case ItemType_GreatLeechdomMp:
							{
								//检查是否有自动补魔道具
								pUser->checkAutoMP();
							}
							break;
						default:
							break;
					}
				}

				return true;
			}
		}
	}
	else
	{
		if(destObj == srcObj )
		{
			Zebra::logger->debug("自己移动到自己的位置bug");
		}
	}
  
	return false;
}

/**
 * \brief	删除包裹中的对象
 * \param   srcObj 目标物品
 */
bool Packages::removeObject(zObject *srcObj, bool notify, bool del)
{
	if(!srcObj) return false;

	if (notify) {
		Cmd::stRemoveObjectPropertyUserCmd send;
		send.qwThisID=srcObj->data.qwThisID;
		owner->sendCmdToMe(&send,sizeof(send));
	}

	uom.removeObject(srcObj);
	Package *p=getPackage(srcObj->data.pos.loc(),srcObj->data.pos.tab());
	if(p && p->remove(srcObj)) {
		//only delete when remove from package and del is true
		if (del) zObject::destroy(srcObj);
		return true;
	}

	Zebra::logger->warn("物品%s[%x]定位错误，不能删除", srcObj->name, srcObj);		
	return false;
}

#include "QuestEvent.h"
#include "QuestTable.h"

/**
 * \brief 增加物品
 * \param srcObj 物品对象
 * \param needFind 是否要查找位置
 * \param from_record 是否来自记录
 * \return false 添加失败 true 添加成功
 */
bool Packages::addObject(zObject *srcObj, bool needFind, int packs)
{
	if(srcObj)
	{
		switch (srcObj->base->kind)
		{
			case ItemType_Blade:                //104代表武术刀类武器
                        case ItemType_Sword :           //105代表武术剑类武器
                        case ItemType_Axe:                 //106代表武术斧类武器
                        case ItemType_Hammer:           //107代表武术斧类武器
                        case ItemType_Staff:                //108代表法术杖类武器
                        case ItemType_Crossbow:         //109代表箭术弓类武器
                        case ItemType_Fan:                 //110代表美女扇类
                        case ItemType_Stick:            //111代表召唤棍类武器                   
                        case ItemType_Necklace: //117代表角色项链类
                        case ItemType_Fing:             //118代表角色戒指类
                        case ItemType_ClothBody:                //101代表布质加生命类服装
                        case ItemType_FellBody:             //102代表皮甲加魔防类服装
                        case ItemType_MetalBody:                //103代表金属铠甲加物防类服装
                        case ItemType_Shield:   //112代表盾牌类
                        case ItemType_Helm:             //113代表角色头盔类
                        case ItemType_Caestus:  //114代表角色腰带类
                        case ItemType_Cuff:             //115代表角色护腕类
                        case ItemType_Shoes:            //116代表角色鞋子类
                             break;
                        default:
			     srcObj->data.exp = 0;
                             break;
                }

		assert( !needFind || srcObj->free() ); //be sure a object do not be added into 2 diff package
		if(uom.addObject(srcObj))	{
			if (!packs) {
				Package *p = getPackage(srcObj->data.pos.loc(),srcObj->data.pos.tab());
				if(p && p->add(srcObj, needFind)) return true;
			}

			if (packs) {
				//save location infomation
				stObjectLocation loc = srcObj->reserve();

				Package** p = getPackage(packs);
				int i = 0;
				while (p && p[i]) {
					srcObj->data.pos = stObjectLocation(p[i]->type(), p[i]->id(), (WORD)-1, (WORD)-1);
					if (p[i]->add(srcObj, needFind)) {
						SAFE_DELETE_VEC(p);
						return true;
					}
					++i;
				}
				SAFE_DELETE_VEC(p);

				//can not be add, resotre the location
				srcObj->restore(loc);
			}
			//Zebra::logger->warn("物品%s[%x]定位错误，不能添加", srcObj->name, srcObj);
			uom.removeObject(srcObj);
			
			return false;
		}else {
			//Zebra::logger->warn("物品%s[%x]索引重复，不能添加", srcObj->name, srcObj);
		}
	}
	return false;
}

/**     
 * \brief 包裹积分验证
 *      
 * 验证用户包裹中的积分是否满足要求
 
 * \param need: 需要积分
 * \return 验证通过返回true,否则返回false
 */
bool Packages::checkTicket(DWORD need)
{
	if(owner->charbase.ticket >= need)
	{
		return true;
	}
	
	return false;	
}
/**     
 * \brief 包裹金币验证
 *      
 * 验证用户包裹中的金币是否满足要求
 
 * \param need: 需要金币
 * \return 验证通过返回true,否则返回false
 */
bool Packages::checkGold(DWORD need)
{
	if(owner->charbase.gold >= need)
	{
		return true;
	}
	
	return false;	
}
/**     
 * \brief 包裹银子验证
 *      
 * 验证用户包裹中的银子是否满足要求
 
 * \param need: 需要银子
 * \return 验证通过返回true,否则返回false
 */
bool Packages::checkMoney(DWORD need)
{
	if (!need) return true;
	
	zObject* gold = getGold();
	//temp solution, just for record before
	if (!gold) {
		gold = zObject::create(objectbm.get(665), 0);
		if (gold) {
			owner->packs.addObject(gold, true, MAIN_PACK);
		}else {
			Zebra::logger->error("创建银子失败");
		}

	}

	if (!gold) return false;
	if ( gold->data.dwNum < need ) {
		//Channel::sendSys(&user, Cmd::INFO_TYPE_FAIL, "银子不足"); 
		return false;
	}
	
	return true;	
}

/**     
 * \brief 扣除银子
 *      
 * 从用户包裹扣除银子,该函数不会检查扣除的金额,注意防止溢出
 
 * \param num: 扣除的银子数量
 * \return 成功返回true,否则返回false
 */
bool Packages::removeMoney(DWORD num ,const char *disc)
{	
#ifdef _DEBUGLOG
	Zebra::logger->debug("用户(%s)银子扣除操作进入，期望(%d)", owner->name, num);
#endif
	if (!num) return true;
	
	zObject* gold = getGold();
	//temp solution, just for record before
	if (!gold) {
		gold = zObject::create(objectbm.get(665), 0);
		if (gold) {
			owner->packs.addObject(gold, true, MAIN_PACK);
		}else {
			Zebra::logger->error("创建银子失败");
		}
	}

	if (!gold) return false;

#ifdef _DEBUGLOG
	Zebra::logger->debug("用户(%s)银子扣除操作，期望(%d),现有(%d)", owner->name, num, gold->data.dwNum);
#endif
	
	if (gold->data.dwNum < num) {
		Zebra::logger->debug("用户(%s)银子扣除失败，期望(%d),现有(%d)", owner->name, num, gold->data.dwNum);
		return false;
	}
	
	gold->data.dwNum -= num;
	zObject::logger(owner->charbase.accid,gold->data.qwThisID,gold->data.strName,gold->data.dwNum,num,0,0,NULL,owner->id,owner->name,disc,NULL,0,0);
	
	Cmd::stRefCountObjectPropertyUserCmd ret;
	ret.qwThisID = gold->data.qwThisID;
	ret.dwNum = gold->data.dwNum;
	owner->sendCmdToMe(&ret, sizeof(ret));
	
	return true;
}

/**     
 * \brief 扣除积分
 *      
 
 * \param num: 扣除的积分数量
 * \return 成功返回true,否则返回false
 */
bool Packages::removeTicket(DWORD num ,const char *disc)
{
	if(owner->charbase.ticket < num)
	{
		Zebra::logger->debug("%s(%d)积分不足,扣除失败,需要%d,现有%d,描述:%s",owner->name,owner->id,num,owner->charbase.ticket,disc);
		return false;
	}
	owner->charbase.ticket-=num;
	zObject::logger(0,0,"积分",owner->charbase.ticket,num,0,owner->id,owner->name,0,NULL,disc,NULL,0,0);
	Cmd::stMainUserDataUserCmd send;
	owner->full_t_MainUserData(send.data);
	owner->sendCmdToMe(&send,sizeof(send));
	
	return true;
}
/**
 * \brief 添加积分
 * \param num 积分数量
 * \param note 提示信息
 */
void Packages::addTicket(DWORD num, const char *disc ,  const char *note, bool notify)
{       
	using namespace Cmd;
	owner->charbase.ticket+=num;
	zObject::logger(0,0,"积分",owner->charbase.ticket,num,1,0,NULL,owner->id,owner->name,disc,NULL,0,0);
	Cmd::stMainUserDataUserCmd send;
	owner->full_t_MainUserData(send.data);
	owner->sendCmdToMe(&send,sizeof(send));
	if (notify)
	{
		if (note == NULL)
		{
			Channel::sendSys(owner, Cmd::INFO_TYPE_GAME, "你得到积分%d",num);
		}       
		else                    
		{       
			Channel::sendSys(owner, Cmd::INFO_TYPE_GAME, "%s消费使你得到积分%d",note,num);
		}
	}
}
/**     
 * \brief 扣除金币
 *      
 
 * \param num: 扣除的金子数量
 * \param need: 是否需要加成(股票扣钱不能有任何加成)
 * \return 成功返回true,否则返回false
 */
bool Packages::removeGold(DWORD num ,const char *disc , bool need)
{
	if(owner->charbase.gold < num)
	{
		Zebra::logger->debug("%s(%d)金币不足,扣除失败,需要%d,现有%d,描述:%s",owner->name,owner->id,num,owner->charbase.gold,disc);
		return false;
	}
	owner->charbase.gold-=num;
	zObject::logger(owner->charbase.accid,owner->charbase.level,"金币",owner->charbase.gold,num,0,owner->id,owner->name,0,NULL,disc,NULL,0,0);
	Cmd::stMainUserDataUserCmd send;
	owner->full_t_MainUserData(send.data);
	owner->sendCmdToMe(&send,sizeof(send));
	//如果是股票扣钱则存储,否则不用
	if(!need)
		owner->save(Cmd::Record::OPERATION_WRITEBACK);
	//暂时去掉加成效果
	/*
	//DWORD overman_id=owner->getOverMan();
	//加成处理部分
	if(need)
	{
		if(overman_id)
		{
			DWORD tick=(DWORD)(num*0.2); 
			SceneUser *pUser = SceneUserManager::getMe().getUserByID(overman_id);
			if(pUser)
			{
				std::string disc = "消费奖励:"; 
				disc += owner->name;
				pUser->packs.addTicket(tick,disc.c_str(),owner->name);
			}
			else
			{
				Cmd::Session::t_OvermanTicketAdd add;
				add.id=overman_id;
				add.ticket=tick;
				strncpy(add.name,owner->name,MAX_NAMESIZE);
				sessionClient->sendCmd(&add, sizeof(add));
			}
		}
		Cmd::Session::t_SpendGold_SceneSession s;
		s.userID = owner->id;
		s.gold = num;
		sessionClient->sendCmd(&s, sizeof(s));
	}
	// */


	return true;
}
/**
 * \brief 添加金币
 * \param num 金币数量
 * \param note 提示信息
 */
void Packages::addGold(DWORD num, const char *disc ,  const char *note, bool notify,bool pack)
{       
	using namespace Cmd;
	owner->charbase.gold+=num;
	zObject::logger(owner->charbase.accid,owner->charbase.level,"金币",owner->charbase.gold,num,1,0,NULL,owner->id,owner->name,disc,NULL,0,0);
	Cmd::stMainUserDataUserCmd send;
	owner->full_t_MainUserData(send.data);
	owner->sendCmdToMe(&send,sizeof(send));
	owner->save(Cmd::Record::OPERATION_WRITEBACK);
	if (notify)
	{
		if (note == NULL)
		{
			//Channel::sendSys(owner, Cmd::INFO_TYPE_GAME, "你得到金子%d",num);
			Channel::sendGold(owner, Cmd::INFO_TYPE_GAME , num, "你得到金子");
		}       
		else                    
		{       
			Channel::sendGold(owner, Cmd::INFO_TYPE_GAME , num, disc);
		}
	}
	if(pack && !(owner->charbase.bitmask & CHARBASE_VIP))
	{
		owner->charbase.bitmask |= CHARBASE_VIP;
		//vip用户赠送一个包裹
		owner->packs.store.goldstore(owner);
	}
}
/**
 * \brief 添加银子
 * \param num 银子数量
 * \param note 提示信息
 */
void Packages::addMoney(DWORD num, const char *disc ,  const char *note, bool notify)
{       
	using namespace Cmd;
	DWORD realGet = 0;
	zObject *gold = getGold();
	//temp solution, just for record before
	if (!gold) {
		gold = zObject::create(objectbm.get(665), 0);
		if (gold) {
			owner->packs.addObject(gold, true, MAIN_PACK);
		}else {
			Zebra::logger->error("创建银子失败");
		}
	}

	if (!gold) return;
	if(gold->base->maxnum - gold->data.dwNum < num)
	{
		realGet = gold->base->maxnum - gold->data.dwNum;
		gold->data.dwNum = gold->base->maxnum;
	}
	else
	{
		gold->data.dwNum += num;
		realGet = num;
	}
	zObject::logger(owner->charbase.accid,gold->data.qwThisID,gold->data.strName,gold->data.dwNum,realGet,1,0,NULL,owner->id,owner->name,disc,NULL,0,0);
	stRefCountObjectPropertyUserCmd setgold;
	setgold.qwThisID=gold->data.qwThisID;
	setgold.dwNum=gold->data.dwNum;
	owner->sendCmdToMe(&setgold,sizeof(setgold));

	if (notify)
	{
		if (note == NULL)
		{
			Channel::sendMoney(owner, Cmd::INFO_TYPE_GAME , realGet, "你得到银子");
		}       
		else                    
		{       
			Channel::sendMoney(owner, Cmd::INFO_TYPE_GAME, realGet, "%s" , note);
		}
	}
}

/**
 * \brief 遍历查找并合并物品，将结果通知客户端
 * \param o 物品对象
 * \return true 继续遍历， false 终止遍历
 */
bool Combination::exec(zObject* o)
{
	if (o->data.dwObjectID == _get->data.dwObjectID && o->data.upgrade == _get->data.upgrade && o->data.dwNum < o->base->maxnum) {
		o->data.dwNum += _get->data.dwNum;
		if (o->data.dwNum > o->base->maxnum) {
			_num += (o->base->maxnum - (o->data.dwNum - _get->data.dwNum) );
			_get->data.dwNum = o->data.dwNum - o->base->maxnum;
			o->data.dwNum = o->base->maxnum;

			Cmd::stRefCountObjectPropertyUserCmd status;
			status.qwThisID = o->data.qwThisID;
			status.dwNum = o->data.dwNum;
			_user->sendCmdToMe(&status, sizeof(status));	
			return true;
		}


		_num += _get->data.dwNum;
		_get->data.dwNum = 0;

		Cmd::stRefCountObjectPropertyUserCmd status;
		status.qwThisID = o->data.qwThisID;
		status.dwNum = o->data.dwNum;
		_user->sendCmdToMe(&status, sizeof(status));	

		return false;
	}

	return true;
}

/*
 * \param kind		说明删除物品的原因
 *			如：改造，升级，etc
 *
 *
bool ClearPack::exec(zObject* ob, char *kind)
{
	char event[128]={0};
	
	if (kind == NULL)
		strcpy(event,"改造物品删除");
	else
	{
		strcpy(event, kind);
		strcat(event, "物品删除");
	}

	zObject::logger(ob->createid,ob->data.qwThisID,ob->data.strName,ob->data.dwNum,ob->data.dwNum,0,const_cast<Packages* >(_ps)->getOwner()->id,const_cast<Packages* >(_ps)->getOwner()->name,0,NULL,event,ob->base,ob->data.kind,ob->data.upgrade);
	const_cast<Packages* >(_ps)->removeObject(ob); //notify and delete
	return true;
}*/
bool ClearPack::exec(zObject* ob)
{
	zObject::logger(ob->createid,ob->data.qwThisID,ob->data.strName,ob->data.dwNum,ob->data.dwNum,0,const_cast<Packages* >(_ps)->getOwner()->id,const_cast<Packages* >(_ps)->getOwner()->name,0,NULL,"改造物品删除",ob->base,ob->data.kind,ob->data.upgrade);
	const_cast<Packages* >(_ps)->removeObject(ob); //notify and delete
	return true;
}


DropFromPack::DropFromPack(const Packages* ps, const Param& p) : _ps(ps), _p(p)
{ 
	int num = p.pack->size() -p.pack->space();
	_begin = (num < p.drop_num)?0:zMisc::randBetween(0, num - p.drop_num);
	_pos = 0;
}
	

bool DropFromPack::exec(zObject* ob)
{
/*
//TODOBYLQY
	std::vector<zObject *> temp_vec;
	int begin = 0;
	std::set<zObject *>::iterator iter;
	if(mainpack > 0)
	{
		for(iter = packs.main.getAllset().begin(); iter != packs.main.getAllset().end() ; iter ++)
		{
			//TODO 其它不可掉落物品
			if((*iter)->data.upgrade > 5 || (*iter)->data.bind || (*iter)->data.pos.y == Cmd::EQUIPCELLTYPE_PACKAGE || (*iter)->data.pos.y == Cmd::EQUIPCELLTYPE_MAKE || (*iter)->base->kind==ItemType_MASK)
			{
				continue;
			}
			temp_vec.push_back(*iter);
		}
		if(mainpack < (int)temp_vec.size())
		{
			begin = zMisc::randBetween(0 , temp_vec.size() - mainpack);
		}
		else
		{
			mainpack = temp_vec.size();
		}
		for(int i = begin; i < mainpack ;  i ++)
		{
			this->packs.moveObjectToScene(&*temp_vec[i] , this->getPos());
		}
	}
*/	
	if( /*ob->data.upgrade > 5 || */ob->data.bind || 
		ob->data.pos.ypos() == Cmd::EQUIPCELLTYPE_PACKAGE || 
		ob->data.pos.ypos() == Cmd::EQUIPCELLTYPE_MAKE || ob->base->kind==ItemType_MASK ||
		ob->base->kind==ItemType_Quest) {
		//didn't drop
		return true;
	}
	++_pos;
	if (_pos >= _begin  && _p.drop_num > 0) {
		const_cast<Packages*>(_ps)->moveObjectToScene(ob, _p.pos);
		--_p.drop_num;
	}
	
	return true;
}

//////////////宠物包裹//////////////
PetPack::PetPack()
:Package(Cmd::OBJECTCELLTYPE_PET, 0, 8, 10),available(0)
{
	setSpace(0);
}
PetPack::~PetPack() {}

bool PetPack::checkAdd(SceneUser* pUser, zObject* object, WORD x, WORD y)
{
	if (x>W || y>H) return false;

	int pos = W*y + x;
	if (pos>available) return false;

	return Package::checkAdd(pUser, object, x, y);
}

void PetPack::setAvailable(WORD s)
{
	WORD old = available;
	available = s;

	WORD n = space();
	if (s>old)
		n += s-old;
	else if (old>s)
		n -= old-s;

	setSpace(n);
}

WORD PetPack::size() const
{
	return available;
}

bool PetPack::isEmpty() const
{
	return space()==size();
}
