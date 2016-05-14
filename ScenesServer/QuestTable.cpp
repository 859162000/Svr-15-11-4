/**
 * \file
 * \version	$Id: QuestTable.cpp  $
 * \author	
 * \date	2005-03-30
 * \brief	任务系统
 * 
 */
 
#include "QuestTable.h"

#include "Quest.h"
#include "QuestMaker.h"
#include "QuestEvent.h"

#include "Zebra.h"
#include "zXMLParser.h"


EventTable* EventTable::_instance = NULL;

/**     
 * \brief 构造函数
 *
 */     
EventTable::EventTable()
{
	
}

/**     
 * \brief 析构函数
 *
 */     
EventTable::~EventTable()
{
	
}

/**     
 * \brief  单件模式,保证事件驱动表格的唯一实例
 *
 * 注册触发条件和动作,生成 事件驱动表格的唯一实例
 *      
 * \return 事件驱动表格的唯一实例
 */	
EventTable& EventTable::instance()
{
	static EventTable new_instance;
	
	if (!_instance) {
		_instance = &new_instance;
		
		//register condition functor
		{
			Parse p;
			p.key("team");
			p.key("type");
			p.key("value");
			p.key("name");
			p.key("id");
			ConditionFactory::instance().register_creator("var", new MakeFunction::Compare<VarCondition>(p));
			ConditionFactory::instance().register_creator("global", new MakeFunction::Compare<GlobalCondition>(p));
			ConditionFactory::instance().register_creator("tongvar", new MakeFunction::Compare<TongVarCondition>(p));
			ConditionFactory::instance().register_creator("familyvar", new MakeFunction::Compare<FamilyVarCondition>(p));
			ConditionFactory::instance().register_creator("uservar", new MakeFunction::Compare<UsersVarCondition>(p));			
//			ConditionFactory::instance().register_creator("user", new MakeFunction::Compare<UserVarCondition>(p));
			ConditionFactory::instance().register_creator("gold", new MakeFunction::Compare<GoldCondition>(p));			
			ConditionFactory::instance().register_creator("mapcountry", new MakeFunction::Prop<MapCountryCondition>(p));			
		}
		{
			Parse p;
			p.key("team");			
			p.key("type");
			p.key("value");
			ConditionFactory::instance().register_creator("level", new MakeFunction::Compare<LevelCondition>(p));
			ConditionFactory::instance().register_creator("septid", new MakeFunction::Compare<SeptIDCondition>(p));
			ConditionFactory::instance().register_creator("unionid", new MakeFunction::Compare<UnionIDCondition>(p));
			ConditionFactory::instance().register_creator("skillnum", new MakeFunction::Compare<HaveSkillCondition>(p));
			ConditionFactory::instance().register_creator("checksex", new MakeFunction::Compare<CheckSexCondition>(p));
			ConditionFactory::instance().register_creator("fivelevel", new MakeFunction::Compare<FiveLevelCondition>(p));
			ConditionFactory::instance().register_creator("repute", new MakeFunction::Compare<ReputeCondition>(p));
			ConditionFactory::instance().register_creator("familylevel", new MakeFunction::Compare<FamilyLevelCondition>(p));
			ConditionFactory::instance().register_creator("actionpoint", new MakeFunction::Compare<ActionPointCondition>(p));	
		}
		
		{
			Parse p;
			p.key("team");
			p.key("value");
			ConditionFactory::instance().register_creator("isguard", new MakeFunction::Prop<IsGuardCondition>(p));
			ConditionFactory::instance().register_creator("nation", new MakeFunction::Prop<NationCondition>(p));
			ConditionFactory::instance().register_creator("innation", new MakeFunction::Prop<InNationCondition>(p));
			ConditionFactory::instance().register_creator("confraternity", new MakeFunction::Prop<ConfraternityCondition>(p));
			ConditionFactory::instance().register_creator("profession", new MakeFunction::Prop<ProfessionCondition>(p));
			ConditionFactory::instance().register_creator("fivetype", new MakeFunction::Prop<FiveTypeCondition>(p));
			ConditionFactory::instance().register_creator("horse", new MakeFunction::Prop<HorseCondition>(p));
			ConditionFactory::instance().register_creator("grade", new MakeFunction::Prop<GradeCondition>(p));			
			ConditionFactory::instance().register_creator("honor", new MakeFunction::Prop<HonorCondition>(p));
			ConditionFactory::instance().register_creator("maxhonor", new MakeFunction::Prop<MaxHonorCondition>(p));
			ConditionFactory::instance().register_creator("selfcountry", new MakeFunction::Prop<SelfCountryCondition>(p));
			ConditionFactory::instance().register_creator("countrypower", new MakeFunction::Prop<CountryPowerCondition>(p));
			ConditionFactory::instance().register_creator("week", new MakeFunction::Prop<WeekCondition>(p));
			ConditionFactory::instance().register_creator("caption", new MakeFunction::Prop<CaptionCondition>(p));
		}

		{
			Parse p;
			p.key("team");
			p.key("long");
			p.key("id");
			p.key("less");
			ConditionFactory::instance().register_creator("timeouts", new MakeFunction::Prop<TimeoutsCondition>(p));
		}
		
		{
			Parse p;
			p.key("team");
			p.key("size");
			ConditionFactory::instance().register_creator("space", new MakeFunction::Prop<SpaceCondition>(p));
		}

		{
			Parse p;
			p.key("start");
			p.key("end");
			ConditionFactory::instance().register_creator("timer", new MakeFunction::Prop<TimerCondition>(p));
		}

		{
			Parse p;
			p.key("id");
			p.key("value");	
			p.key("level");
			p.key("team");
			ConditionFactory::instance().register_creator("item", new MakeFunction::Prop<ItemCondition>(p));
		}

		{
			Parse p;
			p.key("number");
			p.key("male");	
			p.key("female");
			ConditionFactory::instance().register_creator("teamed", new MakeFunction::Prop<TeamedCondition>(p));
		}

		//register action functor
		{
			Parse p;
			p.key("team");
			p.key("type");
			p.key("value");
			p.key("name");
			p.key("id");
			p.key("tmp");
			ActionFactory::instance().register_creator("var", new MakeFunction::Update<VarAction>(p));
			ActionFactory::instance().register_creator("global", new MakeFunction::Update<GlobalAction>(p));
			ActionFactory::instance().register_creator("tongvar", new MakeFunction::Update<TongVarAction>(p));
			ActionFactory::instance().register_creator("familyvar", new MakeFunction::Update<FamilyVarAction>(p));
			ActionFactory::instance().register_creator("familylevel", new MakeFunction::Update<FamilyLevelAction>(p));
			ActionFactory::instance().register_creator("uservar", new MakeFunction::Update<UsersVarAction>(p));
//			ActionFactory::instance().register_creator("user", new MakeFunction::Update<UserVarAction>(p));
		}

		{
			Parse p;
			p.key("team");
			p.key("type");
			p.key("name1");
			p.key("id1");
			p.key("name2");
			p.key("id2");
			p.key("tmp");
			ActionFactory::instance().register_creator("vars", new MakeFunction::Update<VarsAction>(p));
		}

		{
			Parse p;
			p.key("team");
			p.key("type");
			p.key("value");
			ActionFactory::instance().register_creator("gold", new MakeFunction::Update<GoldAction>(p));
			ActionFactory::instance().register_creator("score", new MakeFunction::Update<ScoreAction>(p));
			ActionFactory::instance().register_creator("fivelevel", new MakeFunction::Update<FiveLevelAction>(p));
			ActionFactory::instance().register_creator("honor", new MakeFunction::Update<HonorAction>(p));
			ActionFactory::instance().register_creator("maxhonor", new MakeFunction::Update<MaxHonorAction>(p));
			ActionFactory::instance().register_creator("actionpoint", new MakeFunction::Update<ActionPointAction>(p));
		}

		ActionFactory::instance().register_creator("menu", new MakeFunction::Content<MenuAction>);	
		ActionFactory::instance().register_creator("submenu", new MakeFunction::Content<SubMenuAction>);			
		ActionFactory::instance().register_creator("mainmenu", new MakeFunction::Content<MainMenuAction>);	
		
		{
			Parse p;
			p.key("team");
			p.key("content");
			p.key("kind");
			ActionFactory::instance().register_creator("notify", new MakeFunction::Prop<NotifyAction>(p));
			ActionFactory::instance().register_creator("notify1", new MakeFunction::Prop<Notify1Action>(p));
			ActionFactory::instance().register_creator("bulletin", new MakeFunction::Prop<BulletinAction>(p));
			ActionFactory::instance().register_creator("log", new MakeFunction::Prop<LogAction>(p));
		}
		{
			Parse p;
			p.key("team");
			p.key("name");
			p.key("id");
			ActionFactory::instance().register_creator("refresh", new MakeFunction::Prop<RefreshAction>(p));
		}

		{
			ActionFactory::instance().register_creator("enterseptguard", new MakeFunction::Void<EnterSeptGuardAction>);
			ActionFactory::instance().register_creator("finishseptguard", new MakeFunction::Void<FinishSeptGuardAction>);
		}

		{
			Parse p;
			p.key("team");
			p.key("id");
			p.key("name");
			p.key("value");
			ActionFactory::instance().register_creator("exp", new MakeFunction::Prop<ExpAction>(p));
			ActionFactory::instance().register_creator("exp1", new MakeFunction::Prop<Exp1Action>(p));
		}
		
		{
			Parse p;
			p.key("team");
			p.key("id");
			ActionFactory::instance().register_creator("timeouts", new MakeFunction::Prop<TimeoutsAction>(p));
		}
		
		{
			Parse p;
			p.key("team");
			p.key("value");
			ActionFactory::instance().register_creator("setstate", new MakeFunction::Prop<SetStateAction>(p));
			ActionFactory::instance().register_creator("clearstate", new MakeFunction::Prop<ClearStateAction>(p));
			ActionFactory::instance().register_creator("fivetype", new MakeFunction::Prop<FiveTypeAction>(p));
			ActionFactory::instance().register_creator("horse", new MakeFunction::Prop<HorseAction>(p));
		}
		{
			Parse p;
			p.key("team");
			p.key("id");
			p.key("level");
			ActionFactory::instance().register_creator("skill", new MakeFunction::Prop<SkillAction>(p));
			ActionFactory::instance().register_creator("useskill", new MakeFunction::Prop<UseSkillAction>(p));			
		}
		{
			Parse p;
			p.key("team");
			ActionFactory::instance().register_creator("disable", new MakeFunction::Prop<DisableAction>(p));
			ActionFactory::instance().register_creator("ridedown", new MakeFunction::Prop<RideDownAction>(p));
		}
		{
			Parse p;
			p.key("team");
			p.key("name");
			p.key("pos");
			p.key("pos1");
			p.key("pos2");
			p.key("pos3");
			p.key("pos4");
			ActionFactory::instance().register_creator("goto", new MakeFunction::Prop<GotoAction>(p));
		}
	
		{
			Parse p;
			p.key("team");
			p.key("id");
			p.key("value");
			p.key("odds");
			ActionFactory::instance().register_creator("additem", new MakeFunction::Prop<AddItemAction>(p));	
		}

		{
			Parse p;
			p.key("team");
			p.key("id");
			p.key("value");
			p.key("odds");
			ActionFactory::instance().register_creator("addbinditem", new MakeFunction::Prop<AddBindItemAction>(p));	
		}

		{
			Parse p;
			p.key("team");
			p.key("id");
			p.key("value");
			p.key("odds");
			ActionFactory::instance().register_creator("addgreenbinditem", new MakeFunction::Prop<AddGreenBindItemAction>(p));	
		}

		{
			Parse p;
			p.key("id");
			p.key("value");
			p.key("odds");
			p.key("guard");
			ActionFactory::instance().register_creator("drop", new MakeFunction::Prop<DropAction>(p));	
		}

		{
			Parse p;
			p.key("team");
			p.key("id");
			p.key("value");
			ActionFactory::instance().register_creator("removeitem", new MakeFunction::Prop<RemoveItemAction>(p));
		}
		{
			Parse p;
			p.key("team");
			p.key("id");
			p.key("level");
			ActionFactory::instance().register_creator("dropitem", new MakeFunction::Prop<DropItemAction>(p));
		}

		{
			Parse p;
			p.key("id");
			p.key("id2");
			p.key("id3");
			p.key("odds1");
			p.key("odds2");
			p.key("odds3");
			p.key("map");
			p.key("ltpos");
			p.key("rbpos");
			p.key("num");
			p.key("path");
			p.key("gold");
			p.key("exp");
			p.key("script");
			p.key("dest");
			ActionFactory::instance().register_creator("addnpc", new MakeFunction::Prop<AddNpcAction>(p));
			ActionFactory::instance().register_creator("addguard", new MakeFunction::Prop<AddGuardAction>(p));			
			ActionFactory::instance().register_creator("removenpc", new MakeFunction::Prop<RemoveNpcAction>(p));
		}
	}	
	
	return *_instance;
}

/**     
 * \brief 初始化驱动表格
 *      
 * 按照触发事件类型初始化相应的驱动事件结构
 
 * \param type: 触发事件类型
 * \param file: 触发事件定义脚本
 * \return 初始化成功返回true,否则返回false
 */
bool EventTable::init(const std::string& type, const std::string& file)
{
	if ( !type.compare("on_use") ) {
		return EventManager<OnUse>::instance().parse(file);
	}

	if ( !type.compare("on_kill") ) {
		return EventManager<OnKill>::instance().parse(file);
	}

	if ( !type.compare("on_kill_by_level") ) {
		return EventManager<OnKillByLevel>::instance().parse(file);
	}

	if ( !type.compare("on_kill_by_self") ) {
		return EventManager<OnKillBySelf>::instance().parse(file);
	}

	if ( !type.compare("on_get") ) {
		return EventManager<OnGet>::instance().parse(file);
	}

	if ( !type.compare("on_visit") ) {
		return EventManager<OnVisit>::instance().parse(file);
	}

	if ( !type.compare("on_die") ) {
		return EventManager<OnDie>::instance().parse(file);
	}
	
	if ( !type.compare("on_ride") ) {
		return EventManager<OnRide>::instance().parse(file);
	}
	if ( !type.compare("on_quit") ) {
		return EventManager<OnQuit>::instance().parse(file);
	}
	if ( !type.compare("on_timer") ) {
		return EventManager<OnTimer>::instance().parse(file);
	}
	if ( !type.compare("on_drop") ) {
		return EventManager<OnDrop>::instance().parse(file);
	}
	if ( !type.compare("on_enter") ) {
		return EventManager<OnEnter>::instance().parse(file);
	}
	if ( !type.compare("on_other") ) {
		return EventManager<OnOther>::instance().parse(file);
	}

	return false;
}


QuestTable* QuestTable::_instance = NULL;
const std::string QuestTable::_quest_file = "quests.xml";

/**     
 * \brief 构造函数
 *
 */    
QuestTable::QuestTable()
{
	
}

/**     
 * \brief 析构函数
 *
 */     
QuestTable::~QuestTable()
{
	
}

/**     
 * \brief  单件模式,保证任务表格的唯一实例
 *
 *      
 * \return 任务表格的唯一实例
 */	
QuestTable& QuestTable::instance()
{
	if (!_instance) {
		static QuestTable new_instance;
		_instance = &new_instance;
	}	
	
	return *_instance;
}

/**     
 * \brief 初始化任务表格
 *      
 * 解析任务脚本,生成任务表格和相应的事件驱动表格
 *
 * \return 初始化成功返回true,否则返回false
 */
bool QuestTable::init()
{
	zXMLParser xml;
	Zebra::global["questfile"] = Zebra::global["questdir"]+_quest_file;
	
	if (!xml.initFile(Zebra::global["questfile"]))
	{
		Zebra::logger->error("加载任务文件 %s 失败", Zebra::global["questfile"].c_str());
		return false;
	}

	xmlNodePtr root = xml.getRootNode("quests");
	if (root)
	{
		xmlNodePtr node = xml.getChildNode(root, NULL);	
		while (node) {
			//parse quest
			if (!strcmp((char *)node->name, "quest")) {
				
				Quest quest;
				if (!xml.getNodePropNum(node, "id", &quest._id, sizeof(quest._id))) {
					Zebra::logger->error("加载任务文件 %s 失败，任务缺少id", Zebra::global["questfile"].c_str());					
					return false;
				}
				xml.getNodePropStr(node, "name", quest._title);

				xmlNodePtr d_e_node = xml.getChildNode(node, NULL);
				while (d_e_node) {										
					if (!strcmp((char *)d_e_node->name, "description")) {
						xmlNodePtr description_node = xml.getChildNode(d_e_node, NULL);
						if (xml.getNodeContentStr(description_node, quest._description,true))
							Zebra::logger->debug("任务(id=%d title=%s)\n", quest._id, quest._title.c_str());
					}					
					_quests[quest._id] = quest;
					Zebra::logger->debug("任务(id=%d ) descr=%s\n", quest._id, quest._description.c_str());
					if (!strcmp((char *)d_e_node->name, "events")) {	

						xmlNodePtr event_node = xml.getChildNode(d_e_node, NULL);
						while (event_node) {
							if ( !strcmp((char *)event_node->name, "event")	) {
								//parse events
								std::string type;
								if (!xml.getNodePropStr(event_node, "type", type)) {
									
									return false;	
								}
								std::string file;
								if (!xml.getNodePropStr(event_node, "source", file)) {
									
									return false;	
								}
								if (!EventTable::instance().init(type, file)) {
									return false;
								}
							}
							event_node = xml.getNextNode(event_node, NULL);
						}
					}
					
					d_e_node = xml.getNextNode(d_e_node, NULL);
				}
			}
			
			node = xml.getNextNode(node, NULL);
		}
		
		Zebra::logger->info("初始化任务系统成功");
		return true;
	}
	
	return false;

}

/**     
 * \brief 查找任务信息
 *      
 * 在任务表格中查找特定id的任务信息
 
 * \param id:任务id
 * \return 查找到的任务信息,没找到返回NULL
 */
const Quest* QuestTable::quest(DWORD id) const
{
	const_quest_iterator it = _quests.find(id);
	if ( it != _quests.end()) {
		return &it->second;	
	}	
	
	return NULL;
}
