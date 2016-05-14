#include <math.h>
#include <sstream>
#include "SceneUser.h"
#include "Scene.h"
#include "Chat.h"
#include "QuestEvent.h"
#include "QuestTable.h"
#include "RebuildObject.h"
#include "SessionClient.h"
#include "Command.h"
#include "SceneManager.h"
//#include "script.h"

/**     
 * \brief  处理stPropertyUserCmd指令
 *
 *
 * 处理stPropertyUserCmd指令
 *      
 * \param rev: 接受到的指令内容
 * \param cmdLen: 接受到的指令长度
 * \return 处理指令成功返回真,否则返回false
 */     	
bool SceneUser::doPropertyCmd(const Cmd::stPropertyUserCmd *rev,unsigned int cmdLen)
{
	switch(rev->byParam)
	{
		case SWAPUSEROBJECT_PROPERTY_USERCMD_PARAMETER:
			{
				Cmd::stSwapObjectPropertyUserCmd *swap=(Cmd::stSwapObjectPropertyUserCmd *)rev;
				//得到原物品
				zObject *srcobj=packs.uom.getObjectByThisID(swap->qwThisID);
				if(srcobj)
				{
					//交易处理
					if(tradeorder.hasBegin() && tradeorder.in_trade(srcobj))
					{
						return true;
					}
				

					if (mask.is_use(srcobj)) {
						return Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "请先解除该蒙面巾!");
					}
					stObjectLocation org=srcobj->data.pos;
					Zebra::logger->trace("[移动物品]%s移动物品%s(%d,%d,%d,%d)->(%d,%u,%d,%d)",name,srcobj->data.strName,
							org.loc(),org.tab(),org.xpos(), org.ypos(),
							swap->dst.loc(),swap->dst.tab(),swap->dst.xpos(),swap->dst.ypos());
					if(org != swap->dst && packs.moveObject(this,srcobj,swap->dst))
					{
						//向客户端发送指令指明新的两个地方的物品
						if(swap->dst.loc()!=Cmd::OBJECTCELLTYPE_NONE)
							sendCmdToMe(swap,sizeof(Cmd::stSwapObjectPropertyUserCmd));

						if(packs.equip.needRecalc/* || recalcBySword(false)*/)
						{
							//FunctionTimes times(42,__FUNCTION__);
							notifyEquipChange();
							setupCharBase();
							Cmd::stMainUserDataUserCmd  userinfo;
							full_t_MainUserData(userinfo.data);
							sendCmdToMe(&userinfo,sizeof(userinfo));

							sendMeToNine();
							this->reSendData = false;
#ifdef _DEBUGLOG
							Zebra::logger->trace("[移动物品]完毕......");
#endif
						}
					}
					else
						Zebra::logger->trace("[移动物品]%s移动物品%s(%d,%d,%d,%d)->(%d,%u,%d,%d)失败",name,srcobj->data.strName,
								org.loc(),org.tab(),org.xpos(), org.ypos(),
								swap->dst.loc(),swap->dst.tab(),swap->dst.xpos(),swap->dst.ypos());
				}
				else
					Zebra::logger->debug("[移动物品]%s未找到移动物品 %ld",name,swap->qwThisID);
				return true;
			}
			break;
		case SPLITUSEROBJECT_PROPERTY_USERCMD_PARAMETER:
			{
				Cmd::stSplitObjectPropertyUserCmd *split=(Cmd::stSplitObjectPropertyUserCmd *)rev;
				zObject *srcobj=packs.uom.getObjectByThisID(split->qwThisID);

				if (!srcobj) return true;


				//fix objects which have num can be equiped via this, it's ugly and should be checked at client too
				//NOTICE: this will lead to theses objects can not be equiped, client shouldn't send this cmd , use 
				//stSwapObjectPropertyUserCmd instead when needed.
				if (split->dst.loc() == Cmd::OBJECTCELLTYPE_EQUIP
						|| split->dst.loc() == Cmd::OBJECTCELLTYPE_PET) return true;
				
				//交易处理
				if(tradeorder.hasBegin() && tradeorder.in_trade(srcobj))
				{
					return true;
				}

				if (split->dwNum > srcobj->data.dwNum) {
					return true;
				}

				bool notify = false;

				//其他物品 
				Package *destpack=packs.getPackage(split->dst.loc(),split->dst.tab());
				zObject *destobj=NULL;
				if(destpack && 
					destpack->getObjectByZone(&destobj, split->dst.xpos(), split->dst.ypos()) )
				{					
					if (!destobj) { //split
						destobj = zObject::create(srcobj);
						if(destobj) 	{
							destobj->data.dwNum = split->dwNum;
							destobj->data.pos = split->dst;

							if(packs.addObject(destobj, false)) {
								srcobj->data.dwNum -= split->dwNum;

								//通知客户端
								Cmd::stSplitObjectPropertyUserCmd ret;
								bcopy(split,&ret,sizeof(ret));
								ret.qwNewThisID=destobj->data.qwThisID;
								sendCmdToMe(&ret,sizeof(ret));
								notify = true;
								zObject::logger(destobj->createid,destobj->data.qwThisID,destobj->data.strName,destobj->data.dwNum,destobj->data.dwNum,1,0,NULL,this->id,this->name,"拆分新增",destobj->base,destobj->data.kind,destobj->data.upgrade);
							}
							else {
								zObject::destroy(destobj);
								return true;
							}
							
						}
					} else if (destobj->data.dwObjectID == srcobj->data.dwObjectID && 
						srcobj->base->maxnum>1 &&
						srcobj->data.dwObjectID==destobj->data.dwObjectID &&
						srcobj->data.upgrade==destobj->data.upgrade ) {

						srcobj->data.dwNum -= split->dwNum;
						destobj->data.dwNum += split->dwNum;

						if(destobj->data.dwNum>destobj->base->maxnum)
						{
							destobj->data.dwNum=destobj->base->maxnum;
						}

						notify = true;
						
						//通知客户端
					
						Cmd::stSplitObjectPropertyUserCmd ret;
						bcopy(split,&ret,sizeof(ret));
						ret.qwNewThisID=destobj->data.qwThisID;
						sendCmdToMe(&ret,sizeof(ret));


					}
					 else {
					}


					if(srcobj->data.dwNum==0)
					{
						zObject::logger(srcobj->createid,srcobj->data.qwThisID,srcobj->data.strName,srcobj->data.dwNum,destobj->data.dwNum,0,0,NULL,this->id,this->name,"拆分删除",srcobj->base,srcobj->data.kind,srcobj->data.upgrade);
						packs.removeObject(srcobj, false, true);
					}
					else
					{
						zObject::logger(srcobj->createid,srcobj->data.qwThisID,srcobj->data.strName,srcobj->data.dwNum,destobj->data.dwNum,0,0,NULL,this->id,this->name,"被拆分",srcobj->base,srcobj->data.kind,srcobj->data.upgrade);
					}

				}

				return true;
			}
			break;
#if 0
		case UNIONUSEROBJECT_PROPERTY_USERCMD_PARAMETER:
			{
				Cmd::stUnionObjectPropertyUserCmd *uobj=(Cmd::stUnionObjectPropertyUserCmd *)rev;
				zObject *srcobj=packs.uom.getObjectByThisID(uobj->qwSrcThisID);
				zObject *destobj=packs.uom.getObjectByThisID(uobj->qwDstThisID);
				if(srcobj && destobj && srcobj->base->maxnum>1 &&
						srcobj->data.dwObjectID==destobj->data.dwObjectID &&
						srcobj->data.upgrade==destobj->data.upgrade /*&&
						srcobj->data.pos.dwLocation==Cmd::OBJECTCELLTYPE_MOUSE*/)
				{
					if(srcobj->data.dwNum+destobj->data.dwNum>srcobj->base->maxnum)
					{
						srcobj->data.dwNum=srcobj->data.dwNum+destobj->data.dwNum-srcobj->base->maxnum;
						destobj->data.dwNum=srcobj->base->maxnum;
					}
					else
					{
						destobj->data.dwNum+=srcobj->data.dwNum;
						srcobj->data.dwNum=0;
					}
					//交易处理
					if(destobj->data.pos.dwLocation==Cmd::OBJECTCELLTYPE_TRADE && tradeorder.hasBegin())
					{
						tradeorder.rollback();
						SceneUser *an=tradeorder.getAnother();
						if(an)
						{
							an->lock();
							an->tradeorder.rollback();
							Cmd::stRefCountObjectPropertyUserCmd ret;
							ret.qwThisID=destobj->data.qwThisID;
							ret.dwNum=destobj->data.dwNum;
							an->sendCmdToMe(&ret,sizeof(ret));
							an->unlock();
						}
					}

					//通知客户端
					Cmd::stRefCountObjectPropertyUserCmd ret;
					ret.qwThisID=destobj->data.qwThisID;
					ret.dwNum=destobj->data.dwNum;
					sendCmdToMe(&ret,sizeof(ret));
					if(srcobj->data.dwNum==0)
					{
						packs.rmObject(srcobj);
						Cmd::stRemoveObjectPropertyUserCmd rm;
						rm.qwThisID=srcobj->data.qwThisID;
						sendCmdToMe(&rm,sizeof(rm));
						SAFE_DELETE(srcobj);
					}
					else
					{
						ret.qwThisID=srcobj->data.qwThisID;
						ret.dwNum=srcobj->data.dwNum;
						sendCmdToMe(&ret,sizeof(ret));
					}
				}
				return true;
			}
			break;
#endif
		case USEUSEROBJECT_PROPERTY_USERCMD_PARAMETER:
			{
				Cmd::stUseObjectPropertyUserCmd *use=(Cmd::stUseObjectPropertyUserCmd *)rev;
				zObject *srcobj=packs.uom.getObjectByThisID(use->qwThisID);
				if(srcobj && srcobj->data.pos.loc() ==Cmd::OBJECTCELLTYPE_COMMON)
				{
					useObject(srcobj);
				}
				return true;
			}
			break;
		case Cmd::SCROLL_MAP_PROPERTY_USERCMD_PARA:
			{
				Cmd::stScrollMapPropertyUserCmd * smp = (Cmd::stScrollMapPropertyUserCmd*)rev;
				zObject *srcobj=packs.uom.getObjectByThisID(smp->qwThisID);
				if(srcobj && (srcobj->base->id==675 || srcobj->base->id==676) && srcobj->data.pos.loc()==Cmd::OBJECTCELLTYPE_COMMON)
				{
					strncpy(srcobj->data.maker, smp->mapname[0].strMapName,MAX_NAMESIZE);
					useObject(srcobj);
				}
				else if(srcobj && srcobj->data.pos.loc()==Cmd::OBJECTCELLTYPE_EQUIP&&srcobj->base->kind == ItemType_Amulet)
				{
					strncpy(srcobj->data.maker, smp->mapname[0].strMapName,MAX_NAMESIZE);
					useAmulet(srcobj);
				}
				return true;
			}
			break;
			//请求增值地宫列表
		case Cmd::REQUEST_INC_MAP_PROPERTY_USERCMD_PARA:
			{
				zObject *obj = this->packs.equip.getObjectByEquipPos(Cmd::EQUIPCELLTYPE_ADORN);
				if(obj && obj->base->kind == ItemType_Amulet)
				{
					char buf[1024];
					bzero(buf,sizeof(buf));
					Cmd::stScrollMapPropertyUserCmd *smp = (Cmd::stScrollMapPropertyUserCmd*)buf;
					smp->qwThisID = obj->data.qwThisID;
					constructInPlace(smp);
					smp->size = this->scene->getIncCity((const char*)smp->mapname);
					sendCmdToMe(smp, sizeof(Cmd::stScrollMapPropertyUserCmd) + MAX_NAMESIZE * smp->size);				
				}
				return true;
			}
			break;
		case PICKUPITEM_PROPERTY_USERCMD_PARA:
			{
				Cmd::stPickUpItemPropertyUserCmd *pick=(Cmd::stPickUpItemPropertyUserCmd *)rev;
				zPos p;
				zSceneObject *ret = NULL;
				p.x=pick->x;
				p.y=pick->y;
				ret = scene->getSceneObjectByPos(p);
				if(ret)
				{
					//int ok = 0;
					zObject *o=ret->getObject();
					if(!o)
					{
						return false;
					}
					//无主物品或者在没有组队情况下的物品
					if((o->base->id!=673 || o->base->id!=674) && (!ret->getOwner() || ret->getOwner() == this->id || this->team.IsOurTeam(ret->getOwner())))
					{
						//bool bret = false;
						if(o->base->id==673)//玄天符
						{
							if (guard)
							{
								Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "你正在运镖，保护好你的镖车！");
								return true;
							}
							if(o->data.maker[0]=='\0')
							{
								zPos newPos;
								if(!this->scene->backtoCityMap())
								{
									//随机重生区坐标
									if (!this->scene->randzPosByZoneType(ZoneTypeDef::ZONE_RELIVE,newPos)) return true;
									if (!this->goTo(newPos)) return true;
								}
								else
								{
									//在其它地图中寻找城市
									this->scene->changeMap(this , false);
								}
							}
							else
							{
								zPos newPos;
								newPos.x=o->data.durpoint;
								newPos.y=o->data.dursecond;


								SceneManager::CountryMap_iter country_iter = SceneManager::getInstance().country_info.find(scene->getCountryID());
								if(country_iter == SceneManager::getInstance().country_info.end()) {
									//unknow country
									return true;
								}


								std::ostringstream os;
								os << "name=" << o->data.maker;
								os << " pos=" << o->data.durpoint << "," << o->data.dursecond;
								Gm::gomap(this, os.str().c_str());
							}
							return true;
						}
						else if(o->base->id==674)//轩辕符
						{
							if(ret->getOwner() == this->id)
							{
								if (guard)
								{
									Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "你正在运镖，保护好你的镖车！");
									return true;
								}
								if(o->data.maker[0]=='\0')
								{
									zPos newPos;
									if(!this->scene->backtoCityMap())
									{
										//随机重生区坐标
										if (!this->scene->randzPosByZoneType(ZoneTypeDef::ZONE_RELIVE,newPos)) return true;
										if (!this->goTo(newPos)) return true;
									}
									else
									{
										//在其它地图中寻找城市
										this->scene->changeMap(this , false);
									}
								}
								else
								{
									zPos newPos;
									newPos.x=o->data.durpoint;
									newPos.y=o->data.dursecond;


									SceneManager::CountryMap_iter country_iter = SceneManager::getInstance().country_info.find(scene->getCountryID());
									if(country_iter == SceneManager::getInstance().country_info.end()) {
										//unknow country
										return true;
									}


									std::ostringstream os;
									os << "name=" << o->data.maker;
									os << " pos=" << o->data.durpoint << "," << o->data.dursecond;
									Gm::gomap(this, os.str().c_str());
								}
							}
							return true;
						}
					}
					if(!ret->getOwner() || ret->getOwner() == this->id ||(this->team.IsOurTeam(ret->getOwner()) && this->team.isNormalObj()))
					{
						//如果是银子
						/*
						   if(o->base->kind==ItemType_Money)
						   {
						   if(!this->team.IsTeamed())
						   {
						   packs.addMoney(o->data.dwNum,"拣");
						   }
						   else
						   {
						//自己先得到20%
						packs.addMoney((DWORD)(o->data.dwNum * 0.2f),"拣");
						//组队平分银子
						struct TeamMoneyExec :public TeamMemExec
						{
						DWORD num;
						TeamMoneyExec(DWORD n)
						{
						num = n;
						}
						bool exec(TeamMember &member)
						{
						SceneUser *pUser = SceneUserManager::getMe().
						getUserByTempID(member.tempid);
						if(pUser)
						{
						pUser->packs.addMoney(num,"拣");
						return true;
						}
						return false;
						}
						};
						SceneUser *leader = SceneUserManager::getMe().
						getUserByTempID(this->team.getLeader());
						if(leader)
						{
						TeamMoneyExec tm((DWORD)(o->data.dwNum * 0.8f / leader->team.getSize()));
						leader->team.execEveryOne(tm);
						}
						}
						ok |= 1;
						}
						else
						{
						// */
						/*
						   bret = packsaddObject(o,true);
						   if(bret)
						   {
						   Cmd::stAddObjectPropertyUserCmd ao;
						   ao.byActionType=Cmd::EQUIPACTION_OBTAIN;
						   bcopy(&o->data,&ao.object,sizeof(t_Object));
						   sendCmdToMe(&ao,sizeof(ao));
						   }
						   */
						Combination callback(this, o);
						packs.main.execEvery(callback);
						if (packs.equip.pack(EquipPack::L_PACK) && packs.equip.pack(EquipPack::L_PACK)->can_input()) packs.equip.pack(EquipPack::L_PACK)->execEvery(callback);
						if (packs.equip.pack(EquipPack::R_PACK) && packs.equip.pack(EquipPack::R_PACK)->can_input()) packs.equip.pack(EquipPack::R_PACK)->execEvery(callback);

						bool added = false;
						if (o->data.dwNum) {

							if (this->packs.uom.space(this) >= 1 && packs.addObject(o, true, AUTO_PACK)) {
								//如果是双倍经验道具和荣誉道具需要绑定
								if(o->base->kind == ItemType_DoubleExp || o->base->kind == ItemType_Honor || o->base->kind == ItemType_ClearProperty)
								{
									o->data.bind=1;
								}
								added = true;
								Cmd::stAddObjectPropertyUserCmd status;
								status.byActionType = Cmd::EQUIPACTION_OBTAIN;
								bcopy(&o->data, &status.object, sizeof(t_Object));
								sendCmdToMe(&status, sizeof(status));
							}else {
								Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "你的包裹已满");
								Cmd::stAddMapObjectMapScreenUserCmd status;
								status.action = Cmd::OBJECTACTION_DROP;
								status.data.dwMapObjectTempID = o->data.qwThisID;
								status.data.dwObjectID = o->data.dwObjectID;
								strncpy(status.data.pstrName, o->data.strName, MAX_NAMESIZE);
								status.data.x = p.x;
								status.data.y = p.y;
								status.data.wdNumber = o->data.dwNum;
								status.data.wdLevel = o->base->level;
								status.data.upgrade = o->data.upgrade;
								status.data.kind = o->data.kind;
								scene->sendCmdToNine(ret->getPosI(), &status, sizeof(status));	
							}
						}

						if (callback.num() || added) {
							OnGet event(o->data.dwObjectID);
							EventTable::instance().execute(*this, event);
							zObject::logger(o->createid,o->data.qwThisID,o->data.strName,o->data.dwNum,o->data.dwNum,1,this->scene->id,this->scene->name,this->id,this->name,"拣东西",o->base,o->data.kind,o->data.upgrade);
//							if (ScriptQuest::get_instance().has(ScriptQuest::OBJ_GET, o->data.dwObjectID)) { 
//								char func_name[32];
//								sprintf(func_name, "%s_%d", "get", o->data.dwObjectID);
//								execute_script_event(this,func_name, o);
//							}								
							/*
							Zebra::logger->debug("%s(%u)捡到%s(%u)在(%u,%u)",
									name,id,o->name,o->id,ret->getPos().x,ret->getPos().y);
							// */
						}

						if (added) {
							ret->clear();
						}
						if (!o->data.dwNum || added) {
							scene->removeObject(ret);

							Cmd::stRemoveMapObjectMapScreenUserCmd re;
							re.dwMapObjectTempID=ret->id;
							scene->sendCmdToNine(getPosI(),&re,sizeof(re));

							SAFE_DELETE(ret);
						}

						return true;
						// }
					}
					else
					{
						zObject *o=ret->getObject();
						if(o && o->base->id!=673 && o->base->id!=674)//轩辕符
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "对不起,该物品不属于你");
						}
					}
				}
			}
			break;

		case FOUNDITEM_PROPERTY_USERCMD_PARA:
			if(!RebuildObject::instance().make(*this, rev))
			{
				RebuildObject::instance().response(*this, 1, Base::MAKE);
			}
			break;
		case UPGRADEITEM_PROPERTY_USERCMD_PARA:
			if(!RebuildObject::instance().upgrade(*this, rev))
			{
				RebuildObject::instance().response(*this, 1, Base::UPGRADE);
			}
			break;
		case COMPOSEITEM_PROPERTY_USERCMD_PARA:
			if(!RebuildObject::instance().compose(*this, rev))
			{
				RebuildObject::instance().response(*this, 1, Base::COMPOSE);
			}
			break;
		case COMPOSE_SOUL_STONE_PROPERTY_USERCMD_PARA:
			if(!RebuildObject::instance().compose_soul_stone(*this, rev))
			{
				RebuildObject::instance().response(*this, 1, Base::COMPOSE);
			}
			break;
		case HOLE_PROPERTY_USERCMD_PARA:
			if(!RebuildObject::instance().hole(*this, rev))
			{
				RebuildObject::instance().response(*this, 1, Base::HOLE);
			}
			break;
		case ENCHASEL_PROPERTY_USERCMD_PARA:
			if(!RebuildObject::instance().enchance(*this, rev))
			{
				RebuildObject::instance().response(*this, 1, Base::ENCHANCE);
			}
			break;
		case DECOMPOSE_PROPERTY_USERCMD_PARA:
			if(!RebuildObject::instance().decompose(*this, rev))
			{
				RebuildObject::instance().response(*this, 1, Base::DECOMPOSE);
			}
			break;


		//更新用户快捷键	
		case Cmd::ACCELKEY_PROPERTY_USERCMD_PARA:
			{
				Cmd::stAccekKeyPropertyUserCmd *acc = (Cmd::stAccekKeyPropertyUserCmd *)rev;
				int len = sizeof(Cmd::stAccekKeyPropertyUserCmd) + acc->accelNum * sizeof(Cmd::stGameAccelKey);
				if(acc->accelNum > 0 && len < 1024)
				{
					memcpy(accelData , rev , len);
				}
				else
				{
					bzero(accelData,sizeof(accelData));
				}
				//Zebra::logger->debug("(%s , %ld)更新用户快捷键成功" , this->name , this->tempid);
				return true;
			}
			break;

		//添加用户技能数据
		case ADDUSERSKILL_PROPERTY_USERCMD_PARA:
			{
				if(!addSkillData((Cmd::stAddUserSkillPropertyUserCmd *)rev))
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "不能再次学习已经学习到技能");
				}
				return true;
			}
			break;
		
		//删除用户技能
		case REMOVEUSERSKILL_PROPERTY_USERCMD_PARAMETER:
			{
				if(removeSkill((Cmd::stRemoveUserSkillPropertyUserCmd *)rev))
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "删除技能失败");
				}
				return true;
			}
			break;

		//技能升级	
		case ADDSKILLPOINT_PROPERTY_USERCMD:
			{
				if(!upgradeSkill(((Cmd::stAddSkillPointPropertyUserCmd *)rev)->dwSkillID))
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "不能升级该技能");
				}
				return true;
			}
			break;
		case SYSTEMSETTINGS_PROPERTY_USERCMD_PARA:
			{
				Cmd::stSystemSettingsUserCmd *st = (Cmd::stSystemSettingsUserCmd *)rev;

				//允许组队
				if(isset_state(st->data.bySettings , Cmd::USER_SETTING_TEAM))
				{
					set_state(sysSetting , Cmd::USER_SETTING_TEAM);
					this->team.setOpen(true);
				}
				else
				{
					clear_state(sysSetting , Cmd::USER_SETTING_TEAM);
					this->team.setOpen(false);
				}
				//this->team.setOpen(true);

				bcopy(&st->data.bySettings[1], setting, sizeof(setting));
				bcopy(st->data.dwChatColor, chatColor, sizeof(chatColor));
				/*
				for (int i=Cmd::USER_SETTING_FIREND; i<Cmd::USER_SETTING_CHAT_PRIVATE; i++)
				{
					if(isset_state(st->data.bySettings , i))
						set_state(sysSetting , i);
					else
						clear_state(sysSetting , i);
				}
				*/

#ifdef _XWL_DEBUG
				Zebra::logger->debug("收到系统设置消息:%x %x %x %x,%u %u %u %u %u %u %u %u", st->data.bySettings[0],st->data.bySettings[1],st->data.bySettings[2],st->data.bySettings[3],st->data.dwChatColor[0],st->data.dwChatColor[1],st->data.dwChatColor[2],st->data.dwChatColor[3],st->data.dwChatColor[4],st->data.dwChatColor[5],st->data.dwChatColor[6],st->data.dwChatColor[7]);
#endif
				
				//通知session
				Cmd::Session::t_sysSetting_SceneSession send;
				bcopy(sysSetting, send.sysSetting, sizeof(send.sysSetting));
				strncpy((char *)send.name, name, MAX_NAMESIZE-1);
				sessionClient->sendCmd(&send, sizeof(send));

				//通知网关
				Cmd::Scene::t_sysSetting_GateScene gate_send;
				bcopy(sysSetting, gate_send.sysSetting, sizeof(gate_send.sysSetting));
				gate_send.id=this->id;
				this->gatetask->sendCmd(&gate_send, sizeof(gate_send));


				/*
				 *
				//允许好友
				if(isset_state(st->data.bySettings , Cmd::USER_SETTING_FIREND))
				{
					set_state(sysSetting , Cmd::USER_SETTING_FIREND);
				}
				else
				{
					clear_state(sysSetting , Cmd::USER_SETTING_FIREND);
				}

				//允许交易
				if(isset_state(st->data.bySettings , Cmd::USER_SETTING_TRADE))
				{
					set_state(sysSetting , Cmd::USER_SETTING_TRADE);
				}
				else
				{
					clear_state(sysSetting , Cmd::USER_SETTING_TRADE);
				}

				if(isset_state(st->data.bySettings , Cmd::USER_SETTING_SCHOOL))
				{
					set_state(sysSetting , Cmd::USER_SETTING_SCHOOL);
				}
				else
				{
					clear_state(sysSetting , Cmd::USER_SETTING_SCHOOL);
				}

				if(isset_state(st->data.bySettings , Cmd::USER_SETTING_FAMILY))
				{
					set_state(sysSetting , Cmd::USER_SETTING_FAMILY);
				}
				else
				{
					clear_state(sysSetting , Cmd::USER_SETTING_FAMILY);
				}

				if(isset_state(st->data.bySettings , Cmd::USER_SETTING_UNION))
				{
					set_state(sysSetting , Cmd::USER_SETTING_UNION);
				}
				else
				{
					clear_state(sysSetting , Cmd::USER_SETTING_UNION);
				}

				if(isset_state(st->data.bySettings , Cmd::USER_SETTING_COLOR_NINE))
				{
					set_state(sysSetting , Cmd::USER_SETTING_COLOR_NINE);
				}
				else
				{
					clear_state(sysSetting , Cmd::USER_SETTING_COLOR_NINE);
				}
				*/
				
				return true;
			}
			break;
/*
		//分配五行点数
		case DISTRIBUTEUFIVEPOINT_PROPERTY_USERCMD_PARA:
			{
				Zebra::logger->debug("(%s , %ld)分配五行点数指令" , this->name , this->tempid);
				Cmd::stDistributeFivePointPropertyUserCmd *dis = (Cmd::stDistributeFivePointPropertyUserCmd *)rev;
				if(IsJoin(dis->fiveType) && charbase.points > 0 || (charbase.five == dis->fiveType && charbase.points > 0))
				{
					charbase.fivevalue[dis->fiveType] ++;
					charstate.fivevalue[dis->fiveType] ++;
					int type = dis->fiveType;
				charstate.fivedefence[(type + 1) % 5] = 
					(WORD)sqrt(sqrt((charbase.fivevalue[type] * charbase.fivevalue[type] * charbase.fivevalue[type])));
					if(charbase.fivevalue[type] % 4 == 0)
					{
						type =(type + 3) % 5;
						if(charbase.fivevalue[type] > 0)
						{
							charbase.fivevalue[type] ++;
							charstate.fivevalue[type] ++;
							charstate.fivedefence[(type + 1) % 5] = 
								(WORD)sqrt(sqrt((charbase.fivevalue[type] * charbase.fivevalue[type] * charbase.fivevalue[type])));
							if(charbase.fivevalue[type] % 4 == 0)
							{
								type =(type + 3) % 5;
								charbase.fivevalue[type] ++;
								charstate.fivevalue[type] ++;
								charstate.fivedefence[(type + 1) % 5] = 
									(WORD)sqrt(sqrt((charbase.fivevalue[type] * charbase.fivevalue[type] * charbase.fivevalue[type])));
							}
						}
					}
					charbase.points --;

					//装备改变攻击力预处理
					calPreValue();

					//刷新用户数据
					Cmd::stMainUserDataUserCmd  userinfo;
					full_t_MainUserData(userinfo.data);
					sendCmdToMe(&userinfo,sizeof(userinfo));
					break;
*/
		//分配属性点数
		case DISTRIBUTEUPOINT_PROPERTY_USERCMD_PARA:
			{
				if (charbase.points>0)
				{
					Cmd::stDistributePointPropertyUserCmd *dis = (Cmd::stDistributePointPropertyUserCmd *)rev;
					switch(dis->type)
					{
						case Cmd::PROPERTY_CON:	//体质
						case Cmd::PROPERTY_STR:	//体力
						case Cmd::PROPERTY_DEX:	//敏捷
						case Cmd::PROPERTY_INT:	//智力
						case Cmd::PROPERTY_MEN:	//精神
							{
								charbase.wdProperty[dis->type]++;
								charbase.points--;
								this->setupCharBase();
								//刷新用户数据
								Cmd::stMainUserDataUserCmd  userinfo;
								full_t_MainUserData(userinfo.data);
								sendCmdToMe(&userinfo,sizeof(userinfo));
							}
							break;
						default:
							{
							}
							break;
					}
				}
				return true;
			}
			break;
		case CLEARPOINT_PROPERTY_USERCMD_PARA:
			{
				Cmd::stClearPointPropertyUserCmd *cmd = (Cmd::stClearPointPropertyUserCmd*)rev;
				switch (cmd->dwItemID)
				{
					case 752:
						if (this->charbase.level<40)
						{
							if (this->reduceObjectNum(752,1)==-1)
							{
								Zebra::logger->trace("角色[%s]使用洗属性点功能失败" , this->name);
								return true;
							}
						}
						else
						{
							Zebra::logger->trace("角色[%s]使用洗属性点功能失败" , this->name);
							return true;
						}
						break;
					case 760:
						if (this->charbase.level>=40)
						{
							if (this->reduceObjectNum(760,1)==-1)
							{
								Zebra::logger->trace("角色[%s]使用洗属性点功能失败" , this->name);
								return true;
							}
						}
						else
						{
							Zebra::logger->trace("角色[%s]使用洗属性点功能失败" , this->name);
							return true;
						}
						break;
					case 756:
						if (this->charbase.level>=40)
						{
							if (this->reduceObjectNum(756,1)==-1)
							{
								Zebra::logger->trace("角色[%s]使用洗属性点功能失败" , this->name);
								return true;
							}
						}
						else
						{
							Zebra::logger->trace("角色[%s]使用洗属性点功能失败" , this->name);
							return true;
						}
						break;
					default:
						{
							Zebra::logger->trace("角色[%s]使用洗属性点功能失败" , this->name);
							return true;
						}
						break;
				}
				charbase.points =charbase.points
						+charbase.wdProperty[0]
						+charbase.wdProperty[1]
						+charbase.wdProperty[2]
						+charbase.wdProperty[3]
						+charbase.wdProperty[4];
				charbase.wdProperty[0]=0;
				charbase.wdProperty[1]=0;
				charbase.wdProperty[2]=0;
				charbase.wdProperty[3]=0;
				charbase.wdProperty[4]=0;
				this->setupCharBase();
				//刷新用户数据
				Cmd::stMainUserDataUserCmd  userinfo;
				full_t_MainUserData(userinfo.data);
				sendCmdToMe(&userinfo,sizeof(userinfo));
				Zebra::logger->trace("角色[%s]使用洗属性点功能成功" , this->name);
				return true;
			}
			break;
		case CLEARPOINT_LIMIT_PROPERTY_USERCMD_PARA:
			{
				Cmd::stClearPointLimitPropertyUserCmd *cmd = (Cmd::stClearPointLimitPropertyUserCmd*)rev;
				zObject *srcobj=packs.uom.getObjectByThisID(cmd->qwThisID);
				if(srcobj && srcobj->data.pos.loc() ==Cmd::OBJECTCELLTYPE_COMMON)
				{
					if (srcobj->base->kind == ItemType_ClearProperty && srcobj->base->id == 755)
					{
						BYTE num = cmd->byProperty%5;
						if (charbase.wdProperty[num]>5)
						{
							charbase.points +=5;
							charbase.wdProperty[num]=charbase.wdProperty[num]-5;
						}
						else
						{
							charbase.points += charbase.wdProperty[num];
							charbase.wdProperty[num]=0;
						}
						this->setupCharBase();
						//刷新用户数据
						Cmd::stMainUserDataUserCmd  userinfo;
						full_t_MainUserData(userinfo.data);
						sendCmdToMe(&userinfo,sizeof(userinfo));

						zObject::logger(srcobj->createid,srcobj->data.qwThisID,srcobj->data.strName,srcobj->data.dwNum,srcobj->data.dwNum,0,this->id,this->name,0,NULL,"用洗5点属性宝石",NULL,0,0);
						Zebra::logger->trace("角色[%s]使用洗5点属性功能成功" , this->name);
						packs.removeObject(srcobj); //notify and delete
					}
				}
				return true;
			}
			break;
		case CLEARUSERSKILLPOINT_PROPERTY_USERCMD_PARAMETER:
			{
				Cmd::stClearUserSkillPointPropertyUserCmd *cmd = (Cmd::stClearUserSkillPointPropertyUserCmd*)rev;
				switch (cmd->dwItemID)
				{
					case 753:
						if (this->charbase.level<40)
						{
							if (this->reduceObjectNum(753,1)==-1)
							{
								Zebra::logger->trace("角色[%s]使用洗技能点功能失败" , this->name);
								return true;
							}
						}
						else
						{
							Zebra::logger->trace("角色[%s]使用洗技能点功能失败" , this->name);
							return true;
						}
						break;
					case 761:
						if (this->charbase.level>=40)
						{
							if (this->reduceObjectNum(761,1)==-1)
							{
								Zebra::logger->trace("角色[%s]使用洗技能点功能失败" , this->name);
								return true;
							}
						}
						else
						{
							Zebra::logger->trace("角色[%s]使用洗技能点功能失败" , this->name);
							return true;
						}
						break;
					case 757:
						if (this->charbase.level>=40)
						{
							if (this->reduceObjectNum(757,1)==-1)
							{
								Zebra::logger->trace("角色[%s]使用洗技能点功能失败" , this->name);
								return true;
							}
						}
						else
						{
							Zebra::logger->trace("角色[%s]使用洗技能点功能失败" , this->name);
							return true;
						}
						break;
					default:
						{
							Zebra::logger->trace("角色[%s]使用洗技能点功能失败" , this->name);
							return true;
						}
						break;
				}
				charbase.skillpoint = charbase.level;
				usm.clear();
				Cmd::stClearSkillUserCmd send;
				sendCmdToMe(&send,sizeof(send));
				skillValue.init();
				this->setupCharBase();
				//刷新用户数据
				Cmd::stMainUserDataUserCmd  userinfo;
				full_t_MainUserData(userinfo.data);
				sendCmdToMe(&userinfo,sizeof(userinfo));
				Zebra::logger->trace("角色[%s]使用洗技能点功能成功" , this->name);
				return true;
			}
			break;
		case BODYCOLOR_PROPERTY_USERCMD_PARA:
			{
				using namespace Cmd;
				stBodyColorPropertyUserCmd *bcp = (stBodyColorPropertyUserCmd*)rev;
				if((bcp->dwBodyColorCustom != charbase.bodyColor) && packs.equip.canChangeColor())
				{
					DWORD cost = 1000;
/*
					zObject *gold=packs.getGold();					
					if(!gold)
					{
						Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"对不起,您银子不够");
						return true;
					}

					if(cost > gold->data.dwNum)
					{
						Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"对不起,您银子不够");
						return true;
					}
					gold->data.dwNum -= cost;
					if(gold->data.dwNum==0)
					{
						stRemoveObjectPropertyUserCmd rmgold;
						rmgold.qwThisID=gold->data.qwThisID;
						sendCmdToMe(&rmgold,sizeof(rmgold));
						packs.rmObject(gold);
						SAFE_DELETE(gold);
					}
					else
					{
						stRefCountObjectPropertyUserCmd setgold;
						setgold.qwThisID=gold->data.qwThisID;
						setgold.dwNum=gold->data.dwNum;
						sendCmdToMe(&setgold,sizeof(setgold));
					}
*/
					if (!packs.checkMoney(cost) || !packs.removeMoney(cost,"服装染色")) {
						Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"对不起,您银子不够");
						return true;
					}
					Channel::sendMoney(this, Cmd::INFO_TYPE_GAME,cost,"服装染色成功 , 花费银子");
					if(packs.equip.equip(EquipPack::OTHERS3)  && packs.equip.equip(EquipPack::OTHERS3)->base->kind == ItemType_FashionBody)
					{
						packs.equip.equip(EquipPack::OTHERS3)->data.color = bcp->dwBodyColorCustom;
					}
					else if(packs.equip.equip(EquipPack::OTHERS2)  && packs.equip.equip(EquipPack::OTHERS2)->base->kind == ItemType_FashionBody)
					{
						packs.equip.equip(EquipPack::OTHERS2)->data.color = bcp->dwBodyColorCustom;
					}
					else if (packs.equip.equip(EquipPack::BODY)) 
					{
						packs.equip.equip(EquipPack::BODY)->data.color = bcp->dwBodyColorCustom;
					}
					else
					{
						charbase.bodyColor = bcp->dwBodyColorCustom;
					}
					//Cmd::stAddUserMapScreenUserCmd cmd;
					//full_t_MapUserData(cmd.data);
					//scene->sendCmdToNine(getPosI() , &cmd,sizeof(cmd) , false);
					reSendMyMapData();
				}
				else
				{
					Zebra::logger->trace("用户(%u,%s,%u)更换不可更换颜色的衣服" , id , name , tempid);
				}
				return true;
			}
			break;
		case FACE_PROPERTY_USERCMD_PARA:
			{
				using namespace Cmd;
				stFacePropertyUserCmd *fp = (stFacePropertyUserCmd*)rev;
				if(fp->dwFace == charbase.face)
				{
					return true;
				}
				DWORD cost=0;
				zHeadListB *base=headlistbm.get(fp->dwFace);
				if(base)
				{
					cost = base->cost;
				}
				else
				{
					return true;
				}
				if (!packs.checkMoney(cost) || !packs.removeMoney(cost,"更换头像")) {
					Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"对不起,您银子不够");
					return true;
				}
				
				charbase.face = fp->dwFace;
				Channel::sendMoney(this, Cmd::INFO_TYPE_GAME,cost,"头像更换成功,花费银子");
				//Cmd::stAddUserMapScreenUserCmd cmd;
				//full_t_MapUserData(cmd.data);
				//scene->sendCmdToNine(getPosI() , &cmd,sizeof(cmd) , false);
				reSendMyMapData();
				return true;
			}
			break;
		case HAIR_PROPERTY_USERCMD_PARA:
			{
				using namespace Cmd;
				stHairPropertyUserCmd *hp = (stHairPropertyUserCmd*)rev;
				if(hp->dwHairColor == charbase.hair && hp->byHairType == getHairType())
				{
					return true;
				}
				DWORD cost = 0;
				int isChange = 0;
				if(hp->byHairType != getHairType())
				{
					zHairStyleB *base=hairstylebm.get(hp->byHairType);
					if(base)
					{
						cost = base->cost;
						isChange = 1;
					}
					/*
					switch(hp->byHairType)
					{
						case HairStype_1:
							{
								isChange = 1;
								cost = 2000;
							}
							break;
						case HairStype_2:
							{
								isChange = 1;
								cost = 2000;
							}
							break;
						case HairStype_3:
							{
								isChange = 1;
								cost = 2000;
							}
							break;
						case HairStype_4:
							{
								isChange = 1;
								cost = 2000;
							}
							break;
						case HairStype_5:
							{
								isChange = 1;
								cost = 2000;
							}
							break;
						case HairStype_6:
							{
								isChange = 1;
								cost = 2000;
							}
							break;
						default:
							break;
					}
					// */
				}
				if((hp->dwHairColor & 0X00FFFFFF) != getHairColor())
				{
					zHairColourB *base=haircolourbm.get(hp->dwHairColor & 0X00FFFFFF);
					if(base)
					{
						/// 如果不是光头换发色才需要银子
						if(hp->byHairType != 1)
						{
							cost += base->cost;
						}
						isChange = 2;
					}
					/*
					switch(hp->dwHairColor)
					{
						case HairColor_black:
							{
								isChange = 2;
								cost += 1000;
							}
							break;
						case HairColor_drink:
							{
								isChange = 2;
								cost += 2000;
							}
							break;
						case HairColor_purple:
							{
								isChange = 2;
								cost += 3000;
							}
							break;
					}
					// */
				}
/*
				zObject *gold=packs.getGold();
				if(!gold)
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"对不起,您银子不够");
					return true;
				}
				if(cost > gold->data.dwNum)
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"对不起,您银子不够");
					return true;
				}
				gold->data.dwNum -= cost;
				if(gold->data.dwNum==0)
				{
					stRemoveObjectPropertyUserCmd rmgold;
					rmgold.qwThisID=gold->data.qwThisID;
					sendCmdToMe(&rmgold,sizeof(rmgold));
					packs.rmObject(gold);
					SAFE_DELETE(gold);
				}
				else
				{
					stRefCountObjectPropertyUserCmd setgold;
					setgold.qwThisID=gold->data.qwThisID;
					setgold.dwNum=gold->data.dwNum;
					sendCmdToMe(&setgold,sizeof(setgold));
				}
*/
				if (!packs.checkMoney(cost) || !packs.removeMoney(cost,"美发")) {
					Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"对不起,您银子不够");
					return true;
				}
				
				if(isChange)
				{
					Channel::sendMoney(this, Cmd::INFO_TYPE_GAME,cost,"美发成功,花费银子");
				}
				else
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"所选服务雷同，请选择其他服务");
				}
				setHairType(hp->byHairType);
				setHairColor(hp->dwHairColor);
				//Cmd::stAddUserMapScreenUserCmd cmd;
				//full_t_MapUserData(cmd.data);
				//scene->sendCmdToNine(getPosI() , &cmd,sizeof(cmd) , false);
				reSendMyMapData();
				return true;
			}
			break;
		case SELECT_PROPERTY_USERCMD_PARA:
			{
				using namespace Cmd;
				stSelectPropertyUserCmd * spu = (stSelectPropertyUserCmd*)rev;
				//Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"收到选择指令");
				if(spu->dwOldTempID)
				{
					if(spu->byOldType == MAPDATATYPE_USER)
					{
						SceneUser *pDel = SceneUserManager::getMe().getUserByTempID(spu->dwOldTempID);
						if(pDel)
						{
							//pDel->selected_lock.lock();
							pDel->selected.erase(this->tempid);
							//pDel->selected_lock.unlock();
						}
					}
					else
					{
						SceneNpc *pDel = SceneNpcManager::getMe().getNpcByTempID(spu->dwOldTempID);
						if(pDel)
						{
							//pDel->selected_lock.lock();
							pDel->selected.erase(this->tempid);
							//pDel->selected_lock.unlock();
						}
					}
				}
				//如果目标为-1则表示不再选中别的对象
				if(spu->dwTempID == (DWORD)-1)
				{
					break;
				}
				switch(spu->byType)
				{
					case MAPDATATYPE_NPC:
						{
							SceneNpc *pNpc = SceneNpcManager::getMe().getNpcByTempID(spu->dwTempID);
							if(pNpc)
							{
								//pNpc->selected_lock.lock();
								pNpc->selected.insert(SelectedSet_value_type(this->tempid));
								Cmd::stRTSelectedHpMpPropertyUserCmd ret;
								ret.byType = Cmd::MAPDATATYPE_NPC;
								ret.dwTempID = pNpc->tempid;//临时编号
								ret.dwHP = pNpc->hp;//当前血
								ret.dwMaxHp = pNpc->getMaxHP();//最大hp
								ret.dwMP = 0;//this->charbase.mp;//当前mp
								ret.dwMaxMp = 0;//this->charstate.maxmp;//最大mp
								//pNpc->selected_lock.unlock();
								this->sendCmdToMe(&ret , sizeof(ret));
								char Buf[200]; 
								bzero(Buf , sizeof(Buf));
								stSelectReturnStatesPropertyUserCmd *srs=(stSelectReturnStatesPropertyUserCmd*)Buf;
								constructInPlace(srs);
								srs->byType = MAPDATATYPE_NPC;
								srs->dwTempID = spu->dwTempID;
								pNpc->skillStatusM.getSelectStates(srs , sizeof(Buf));
								if(srs->size > 0)
								{
									this->sendCmdToMe(srs , sizeof(stSelectReturnStatesPropertyUserCmd) + 
											sizeof(srs->states[0]) * srs->size);
								}
							}
						}
						break;
					case MAPDATATYPE_USER:
						{
							using namespace Cmd;
							SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(spu->dwTempID);
							if(pUser)
							{
								//Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"%s收到选择%s指令",this->name , pUser->name);
								//pUser->selected_lock.lock();
								pUser->selected.insert(SelectedSet_value_type(this->tempid));
								Cmd::stRTSelectedHpMpPropertyUserCmd ret;
								ret.byType = Cmd::MAPDATATYPE_USER;
								ret.dwTempID = pUser->tempid;//临时编号
								ret.dwHP = pUser->charbase.hp;//当前血
								ret.dwMaxHp = pUser->charstate.maxhp;//最大hp
								ret.dwMP = pUser->charbase.mp;//当前mp
								ret.dwMaxMp = pUser->charstate.maxmp;//最大mp
								//pUser->selected_lock.unlock();
								this->sendCmdToMe(&ret , sizeof(ret));
								char Buf[200]; 
								bzero(Buf , sizeof(Buf));
								stSelectReturnStatesPropertyUserCmd *srs=(stSelectReturnStatesPropertyUserCmd*)Buf;
								constructInPlace(srs);
								srs->byType = MAPDATATYPE_USER;
								srs->dwTempID = spu->dwTempID;
								pUser->skillStatusM.getSelectStates(srs , sizeof(Buf));
								if(srs->size > 0)
								{
									this->sendCmdToMe(srs , sizeof(stSelectReturnStatesPropertyUserCmd) + 
											sizeof(srs->states[0]) * srs->size);
								}
								//临时发送pk附加状态,等待以后策划修改^_^处理
								pUser->sendtoSelectedPkAdditionStateToUser(this);
								pUser->sendtoSelectedReliveWeakStateToUser(this);
								pUser->sendtoSelectedTrainStateToUser(this);
								/*
								bzero(Buf , sizeof(Buf));
								constructInPlace(srs);
								srs->byType = MAPDATATYPE_USER;
								srs->dwTempID = spu->dwTempID;
								srs->size=1;
								srs->states[0].state = Cmd::USTATE_PK;
								srs->states[0].result=charbase.pkaddition;
								srs->states[0].time=0XFFFF;
								this->sendCmdToMe(srs , sizeof(stSelectReturnStatesPropertyUserCmd) + 
										sizeof(srs->states[0]) * srs->size);
								// */
							}
						}
						break;
					default:
						break;
				}
			}
			break;
			//请求观察某个用户
		case SELECT_MAINUSER_PROPERTY_USERCMD_PARA:
			{
				using namespace Cmd;
				stSelectMainUserPropertyUserCmd *smu = (stSelectMainUserPropertyUserCmd*)rev;
				switch(smu->byType)
				{
					case MAPDATATYPE_USER:
						{
							SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(smu->dwTempID);
							if(pUser)
							{
								if (!pUser->mask.is_masking())
								{
									char Buf[sizeof(stSelectReturnMainUserPropertyUserCmd) + sizeof(EquipedObject) * 16];
									bzero(Buf , sizeof(Buf));
									stSelectReturnMainUserPropertyUserCmd *srm = (stSelectReturnMainUserPropertyUserCmd*)Buf;
									constructInPlace(srm);
									srm->dwTempID = pUser->tempid;
									pUser->full_t_MainUserData(srm->mainuser_data);
									pUser->full_t_MapUserData(srm->mapuser_data);
								
									srm->dwSize = pUser->packs.equip.fullAllEquiped((char*)srm->object_data);
									this->sendCmdToMe(srm , sizeof(stSelectReturnMainUserPropertyUserCmd) 
											+ sizeof(EquipedObject) * srm->dwSize);

									if (pUser->horse.horse())
									{
											stSelectReturnHorsePropertyUserCmd send;
											pUser->horse.full_HorseDataStruct(&send.data);
											sendCmdToMe(&send,sizeof(send));
									}
								}
								else
								{
									Channel::sendSys(this, Cmd::INFO_TYPE_MSG, "他是蒙面人无法观察");
								}
							}
						}
						break;
					case MAPDATATYPE_NPC:
						{
						}
						break;
					default:
						break;
				}
			}
			break;
		case HONOR_TO_PKVALUE_PROPERTY_USERCMD_PARA:
			{
				short good = this->charbase.goodness & 0x0000FFFF;
				short old_good = good;
				DWORD old_honor = this->charbase.honor;
				if(good > 0 && good < (short)Cmd::GOODNESS_7 && this->charbase.honor > 0)
				{
					if(good <= (short)(this->charbase.honor / 5))
					{
						this->charbase.honor -= good * 5;
						good = 0;
					}
					else
					{
						good -= (short)(this->charbase.honor / 5);
						this->charbase.honor = this->charbase.honor % 5;
					}
					this->charbase.goodness = this->charbase.goodness & 0XFFFF0000 + good;
					Cmd::stMainUserDataUserCmd  userinfo;
					full_t_MainUserData(userinfo.data);
					sendCmdToMe(&userinfo,sizeof(userinfo));
					reSendMyMapData();
					zObject::logger(0,0,"荣誉值",this->charbase.honor,old_honor - this->charbase.honor,0,this->id,this->name,0,NULL,"洗PK值扣除荣誉值",NULL,0,0);
					return Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "你用%u荣誉点缩短了%u分钟的罪恶时间",old_honor - this->charbase.honor , old_good - good);
				}
				return true;
			}
			break;
		case GO_TRAIN_PROPERTY_USERCMD_PARA:
			{
				if((privatestore.step() != PrivateStore::NONE) || tradeorder.hasBegin())//正在交易
					return true;

				Cmd::stGoTrainPropertyUserCmd * cmd = (Cmd::stGoTrainPropertyUserCmd *)rev;

				char mname[MAX_NAMESIZE];
				bzero(mname, MAX_NAMESIZE);
				DWORD mid = 192+cmd->level;
				SceneManager::getInstance().buildMapName(6, mid, mname);
				zPos p;
				switch (cmd->level)
				{
					case 1:
						p = zPos(172, 265);
						break;
					case 2:
						p = zPos(257, 144);
						break;
					case 3:
						p = zPos(34, 188);
						break;
					case 4:
						p = zPos(139, 270);
						break;
					case 5:
						p = zPos(69, 192);
						break;
					case 6:
						p = zPos(217, 208);
						break;
					case 7:
						p = zPos(245, 200);
						break;
					case 8:
						p = zPos(78, 187);
						break;
					case 9:
						p = zPos(107, 204);
						break;
					default:
						Zebra::logger->error("%s 进入修炼地图层数不对 level=%u", name, cmd->level);
						return true;
						break;
				}

				zObject * o = 0;
				if (!packs.main.getObjectByID(&o, 798))
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "你缺少一件必要的道具");
					return true;
				}

				packs.removeObject(o, true, true);
				charbase.trainTime = 86399;
				sendtoSelectedTrainState();

				Scene * s=SceneManager::getInstance().getSceneByName(mname);
				if (s)
				{
					if (changeMap(s, p))
						Zebra::logger->trace("%s 进入练级地图 %s(%u,%u)", name, s->name, getPos().x, getPos().y);
					else
						Zebra::logger->trace("%s 进入练级地图失败 %s(%u,%u)", name, s->name, p.x, p.y);
				}
				else
				{       
					Cmd::Session::t_changeScene_SceneSession cmd;
					cmd.id = id;
					cmd.temp_id = tempid;
					cmd.x = p.x;
					cmd.y = p.y;
					cmd.map_id = 0; 
					cmd.map_file[0] = '\0';
					strncpy((char *)cmd.map_name, mname, MAX_NAMESIZE);
					sessionClient->sendCmd(&cmd, sizeof(cmd));
				}
			}
			break;
		default:
			break;
	}
//	Zebra::logger->debug("SceneUser::doPropertyCmd\tparam:%d", rev->byParam);
	return false;
}

