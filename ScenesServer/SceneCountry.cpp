/**
 * \file
 * \version  $Id: SceneCountry.cpp $
 * \author 
 * \date
 * \brief 实现国家相关指令的处理
 *
 * 
 */
#include <math.h>
#include "SceneUser.h"
#include "Scene.h"
#include "RecordClient.h"
#include "Chat.h"
#include "SessionCommand.h"							
#include "SessionClient.h"
#include "SceneManager.h"
#include "ScenesServer.h"


//const unsigned int CHANGE_COUNTRY_NEED_MONEY = 5 * 10000; // 变更国籍，所需银两5锭
//const unsigned int CANCEL_COUNTRY_NEED_MONEY = 5 *  10000; // 取消国籍，所需银两5锭
extern unsigned int cancel_country_need_money;
extern unsigned int is_cancel_country;

/**
 * \brief 执行国家相关命令
 *
 *
 * \param rev 挑战指令
 * \param cmdLen 消息长度
 * \return 是否成功
 */
bool SceneUser::doCountryCmd(const Cmd::stCountryUserCmd *rev,unsigned int cmdLen)
{
	Zebra::logger->debug("doCountryCmd receive byParam:[%d]", rev->byParam);

	switch (rev->byParam)
	{
		case REQUEST_COUNTRY_POWER_PARA:
			{
				Cmd::stReturnCountryPowerCmd send;
				send.country[0]=0;
				send.country[1]=0;
				for(int i=0; i<13; i++) if (ScenesService::getInstance().countryPower[i]==1) Cmd::set_state(send.country,i);
				this->sendCmdToMe(&send,sizeof(send));
				return true;
			}
			break;
		case Cmd::ANSWER_COUNTRY_DARE_PARA:
			{
				Cmd::stAnswerCountryDareUserCmd* cmd = (Cmd::stAnswerCountryDareUserCmd*)rev;
				if (cmd->byStatus == Cmd::ANSWER_COUNTRY_DARE_YES)
				{
					Cmd::Session::t_changeScene_SceneSession cmd;
					Scene* pScene= SceneManager::getInstance().
						getSceneByID(SceneManager::getInstance().
								buildMapID(this->charbase.country, 137));//边境

					if (this->scene->getRealMapID() == 137)
					{
						return true;
					}

					this->charbase.exploit = this->charbase.exploit+(2*exploit_arg);
					this->charbase.gomaptype = ZoneTypeDef::ZONE_COUNTRY_WAR;

					if (pScene)            
					{//本服                         
						zPos Pos;
						Pos.x = 0;
						Pos.y = 0;     

						this->changeMap(pScene, Pos);
					}       
					else
					{               

						cmd.id = this->id;
						cmd.temp_id = this->tempid;
						cmd.x = 0;              
						cmd.y = 0;
						cmd.map_id = SceneManager::getInstance().buildMapID
							(this->charbase.country, 137);
						//边境

						bzero(cmd.map_file, sizeof(cmd.map_file));
						bzero(cmd.map_name, sizeof(cmd.map_file));
						sessionClient->sendCmd(&cmd, sizeof(cmd));
					}
				}
				return true;
			}
			break;
		case Cmd::CONTRIBUTE_COUNTRY_MATERIAL:
			{
				Cmd::stContributeCountryMaterialCmd* cmd = (Cmd::stContributeCountryMaterialCmd*)rev;
				if (cmd->itemID && cmd->itemID != 0xffffffff)//INVALID_THISID
				{
					zObject * srcobj=this->packs.uom.getObjectByThisID(cmd->itemID);
					if (!srcobj)
					{
						Zebra::logger->trace("%s 国家捐献时未找到该物品 id=%u", 
								this->name, cmd->itemID);
						return true;
					}

					zCountryMaterialB* country_material = srcobj->canContribute();
					if (country_material==NULL)
					{
						Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "你不能捐赠这件物品");
						return true;
					}
						
					Cmd::Session::t_ContributeCountry_SceneSession send;
					send.byType = srcobj->getMaterialKind();
					
					if (send.byType>0)
					{
						if (country_material->dwMaterialKind == 1)	
						{//打造出来的高等级材料
							send.dwValue = (DWORD)pow(5, srcobj->data.upgrade+1) * srcobj->data.dwNum;	
						}
						else
						{//普通材料
							send.dwValue = srcobj->data.dwNum;
						}
						send.byType = 1;
					}
					else
					{
						if (country_material->dwMaterialKind == 3)
						{
							send.dwValue = srcobj->data.dwNum;
							send.byType = 3;
						}
						else
						{
							send.dwValue = (srcobj->base->price*srcobj->data.dwNum)/100;
						}
					}

					if (send.dwValue>0)
					{
						if (send.byType !=3)
						{
						DWORD add_exploit = (DWORD)((((float)(send.dwValue)/10)/5) * exploit_arg);
						
						Zebra::logger->trace("[国家捐献]: 当前功勋:%d 本次获得功勋:%d 当前荣誉值:%d 本次获得荣誉值:%d", 
								this->charbase.exploit, add_exploit,
								this->charbase.honor, (send.dwValue/5));

						this->charbase.exploit = this->charbase.exploit + add_exploit;
						
						BUFFER_CMD(Cmd::stAddUserAndPosMapScreenStateUserCmd ,send2 , zSocket::MAX_USERDATASIZE);
						this->full_t_MapUserDataPosState(send2->data);
						sendCmdToMe(send2,send2->size());
						//又不要加荣誉值了,nb策划
						//this->charbase.honor += send.dwValue/5;
						//this->charbase.maxhonor += send.dwValue/5;
						Cmd::stMainUserDataUserCmd send1;
						this->full_t_MainUserData(send1.data);
						this->sendCmdToMe(&send1, sizeof(send1));
						}
						else if (send.byType == 3)
						{
							this->charbase.exploit = this->charbase.exploit + 
								((send.dwValue*2) * exploit_arg);
							/*Cmd::stMainUserDataUserCmd send1;
							  this->full_t_MainUserData(send1.data);
							  this->sendCmdToMe(&send1, sizeof(send1));*/

							BUFFER_CMD(Cmd::stAddUserAndPosMapScreenStateUserCmd ,send2 , zSocket::MAX_USERDATASIZE);
							this->full_t_MapUserDataPosState(send2->data);
							sendCmdToMe(send2,send2->size());
						}

						send.dwCountry = this->charbase.country;
						sessionClient->sendCmd(&send, sizeof(send));
					}
					
					zObject::logger(srcobj->createid,srcobj->data.qwThisID,srcobj->base->name,
							srcobj->data.dwNum,srcobj->data.dwNum,0,this->id,this->name,0,"国家","捐献到国库",NULL,0,0);

					this->packs.removeObject(srcobj);//移除物品

					this->save(Cmd::Record::OPERATION_WRITEBACK);
					return true;
				}
			}
			break;
		case Cmd::CANCEL_COUNTRY_PARA:
			{
				//Cmd::stCancelCountryCmd * cmd = (Cmd::stCancelCountryCmd *)rev;
				Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "该功能暂时未开放");
				return true;

				SceneManager::CountryMap_iter src_pos = SceneManager::getInstance().
					country_info.find(this->charbase.country);

				if (this->charbase.country == PUBLIC_COUNTRY)	
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "您已经是无国籍人士。");
					return true;
				}

				this->charbase.country = PUBLIC_COUNTRY;
				if (this->scene->getCountryID() == 6)
				{
					deathBackToMapID = (this->charbase.country << 16 ) + 
						this->scene->getCommonCountryBacktoMapID();
				}       
				else
				{
					if(this->charbase.country == 6)
					{
						deathBackToMapID = (this->charbase.country << 16 ) + 
							this->scene->getCommonUserBacktoMapID();                              					     }
					else
					{
						deathBackToMapID = (this->charbase.country << 16 ) + 
							this->scene->getForeignerBacktoMapID();
					}
				}             

				// 清除钱庄所有物品
//				this->packs.store.removeAll();	
//				packs.clearPackage(&packs.store);
				packs.execEvery(&packs.store, Type2Type<ClearPack>());

				// 清除所有任务
				this->quest_list.clear(this);

				// 清除功勋和文采值
				this->charbase.grace = 0;
				this->charbase.exploit = 0;
				this->save(Cmd::Record::OPERATION_WRITEBACK);
				Cmd::Session::t_changeCountry_SceneSession send;

				Channel::sendSys(this, Cmd::INFO_TYPE_GAME, 
						"您已经离开 %s，成为无国籍人士，原有社会关系、钱庄物品均清空，希望您在这个国家能愉快的生活",
						src_pos->second.name);

				send.dwUserID = this->id;
				send.dwToCountryID = this->charbase.country;
				sessionClient->sendCmd(&send, sizeof(send));

				return true;
			}
			break;
		case Cmd::APPLY_COUNTRY_PARA:
			{
				Cmd::stApplyCountryCmd* cmd = (Cmd::stApplyCountryCmd*)rev;
				
				Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "该功能暂时未开放");
				return true;

				if (this->charbase.country != PUBLIC_COUNTRY)	
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "您必须首先成为无国籍人士，才能申请加入新的国家");
					return true;
				}

				if (cmd->dwToCountryID == PUBLIC_COUNTRY)
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, 
							"不能申请成为无国籍人士。");
					return true;
				}

				if (is_cancel_country)
				{       
					if (this->packs.checkMoney(cancel_country_need_money)				                                                && this->packs.removeMoney(cancel_country_need_money, "改变国籍"))
					{
						this->charbase.country = cmd->dwToCountryID;

						if (this->scene->getCountryID() == 6)
						{
							deathBackToMapID = (this->charbase.country << 16 ) + 
								this->scene->getCommonCountryBacktoMapID();
						}       
						else
						{
							if(this->charbase.country == 6)
							{
								deathBackToMapID = (this->charbase.country << 16 ) + 
									this->scene->getCommonUserBacktoMapID();                              					     }
							else
							{
								deathBackToMapID = (this->charbase.country << 16 ) + 
									this->scene->getForeignerBacktoMapID();
							}
						}            

						this->relive();	
					}
					else
					{
						Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "金钱不足5锭,不能申请加入新国家");
					}
				}
				else
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "暂时未开放叛国功能。");
				}

				return true;
			}
			break;
		case Cmd::CHANGE_COUNTRY_PARA:
			{
				Cmd::stChangeCountryCmd* cmd = (Cmd::stChangeCountryCmd*)rev;

				if (this->charbase.country == cmd->dwToCountryID)
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "您已经是该国国籍。无需变更");
					return true;
				}
				time_t cur_time = time(NULL);

				if ((cur_time - this->lastChangeCountryTime)< 24*60*60*3)
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "叛国后三天以后才能再次叛国");
					return true;
				}
				
				if (6 == cmd->dwToCountryID)
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "不能变更到中立区");
					return true;
				}

				SceneManager::CountryMap_iter src_pos = SceneManager::getInstance().
					country_info.find(this->charbase.country);

				SceneManager::CountryMap_iter cur_pos = SceneManager::getInstance().
					country_info.find(cmd->dwToCountryID);

				if (src_pos == SceneManager::getInstance().country_info.end() 
						|| cur_pos == SceneManager::getInstance().country_info.end())
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "国家不存在，无法变更");
					return true;
				}

				if (this->charbase.unionid>0 || this->charbase.septid>0)
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "请您先退出帮会或家族再变更国籍");
					return true;
				}

				const DWORD change_country_need_gold = 5000;

				if (packs.checkGold(change_country_need_gold)
						&& packs.removeGold(change_country_need_gold, "改变国籍"))
				{       
					this->charbase.country = cmd->dwToCountryID;
					this->deathBackToMapID  = (this->charbase.country << 16 ) + 102;

					// 清除钱庄所有物品
					//					this->packs.store.removeAll();	
					//					packs.clearPackage(&packs.store);
					packs.execEvery(&packs.store, Type2Type<ClearPack>());					
					// 清除所有任务
					//this->quest_list.clear(this);

					// 清除功勋和文采值
					this->charbase.grace = 0;
					this->charbase.exploit = 0;
					this->save(Cmd::Record::OPERATION_WRITEBACK);
					Cmd::Session::t_changeCountry_SceneSession send;

					Channel::sendSys(this, Cmd::INFO_TYPE_GAME, 
							"您已经离开 %s 加入了 %s ，原有社会关系、钱庄物品均清空，希望您在这个国家能愉快的生活",
							src_pos->second.name, cur_pos->second.name);

					send.dwUserID = this->id;
					send.dwToCountryID = cmd->dwToCountryID;
					sessionClient->sendCmd(&send, sizeof(send));
					this->lastChangeCountryTime = cur_time;
				}
				else
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "金币不足50两, 不能变更国籍");
				}

				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

