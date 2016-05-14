#include "SceneUser.h"
#include "SceneNpcManager.h"
#include "NpcTrade.h"
#include "RebuildObject.h"
#include <string>





bool SceneUser::npcTradeGold(Cmd::stBuyObjectNpcTradeUserCmd *ptCmd,zObjectB *base, BYTE itemlevel)
{
	SceneNpc * n = SceneNpcManager::getMe().getNpcByTempID(npc_dwNpcTempID);
	if (!n)
	{
		Zebra::logger->debug("[交易:玩家<------商店]%s 交易时，找不到该npc tempID=%u", name, npc_dwNpcTempID);
		return false;
	}
	DWORD need_gold=0;
	DWORD need_ticket=0;
	if(base && (charbase.gold + charbase.ticket) >= base->price*ptCmd->dwNum)
	{
		NpcTrade::NpcItem item;
		item.id = base->id;
		item.kind = base->kind;
		item.lowLevel = 0;
		item.level = base->needlevel;
		item.itemlevel = itemlevel;
		item.action = NpcTrade::NPC_BUY_OBJECT;
		if (NpcTrade::getInstance().verifyNpcAction(npc_dwNpcDataID, item))
		{
			//zObject *o = zObject::create(base, ptCmd->dwNum);
			zObject* o = NULL;
			if (base->recast)
			{
				o = zObject::create(base, ptCmd->dwNum, 0);
				if (o && itemlevel)
				{
					do
					{
						//Upgrade::upgrade(*this, o, 0); //must success
					}
					while (--itemlevel>0);
				}
			}else
			{
				o = zObject::create(base, ptCmd->dwNum, itemlevel);
			}
			if(o)
			{
				Combination callback(this, o);
				packs.main.execEvery(callback);
				if (packs.equip.pack(EquipPack::L_PACK)) packs.equip.pack(EquipPack::L_PACK)->execEvery(callback);
				if (packs.equip.pack(EquipPack::R_PACK)) packs.equip.pack(EquipPack::R_PACK)->execEvery(callback);

				int free = 0;
				if (o->data.dwNum) {

					if (packs.addObject(o, true, AUTO_PACK)) {
						free = o->data.dwNum;
						//如果是双倍经验道具和荣誉道具需要绑定
						o->checkBind();
						Cmd::stAddObjectPropertyUserCmd status;
						status.byActionType = Cmd::EQUIPACTION_OBTAIN;
						bcopy(&o->data, &status.object, sizeof(t_Object));
						sendCmdToMe(&status, sizeof(status));
					}
				}



				if (callback.num() || free) 
				{
					//get object
					int count = callback.num() + free;
					DWORD need=base->price*count;
					if(charbase.gold >=need)
					{
						need_gold=need;
						//charbase.gold -=need_gold;
					}
					else
					{
						need_gold=charbase.gold;
						//charbase.gold=0;
						need_ticket=need;
						//charbase.ticket-=need_ticket;
					}
					if(need_gold)
					{
						std::string disc = "买东西:"; 
						disc += o->base->name;
						if (!packs.removeGold(need_gold,disc.c_str())) {
							Zebra::logger->fatal("[交易:玩家<------商店]用户(%s)买%s时金币计算错误!", name, o->base->name);
						}
					}
					if(need_ticket)
					{
						std::string disc = "买东西:"; 
						disc += o->base->name;
						if (!packs.removeTicket(need_ticket,disc.c_str())) {
							Zebra::logger->fatal("[交易:玩家<------商店]用户(%s)买%s时点券算错误!", name, o->base->name);
						}
					}
					zObject::logger(o->createid,o->data.qwThisID,o->data.strName,o->data.dwNum,count,1,n->id,n->name,this->id,this->name,"buy_npc",o->base,o->data.kind,o->data.upgrade);
					if(need_gold && !need_ticket)
					{
						//Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "得到物品 %s(%d)个 , 花费金子%d个,",o->name , count,need_gold);
						Channel::sendGold(this, Cmd::INFO_TYPE_GAME , need_gold,"得到物品 %s(%d)个 , 花费金子",o->name , count);
					}
					else if(need_gold && need_ticket)
					{
						Channel::sendGold(this, Cmd::INFO_TYPE_GAME , need_gold,"得到物品 %s(%d)个 , 花费积分%d,花费金子",o->name , count,need_ticket);
						//Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "得到物品 %s(%d)个 , 花费金子%d个,花费积分%d个",o->name , count,need_gold);
					}
					else
					{
						Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "得到物品 %s(%d)个 , 花费积分%d个",o->name , count,need_ticket);
					}
				}
				if (!free) { //package is full
					//Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "你的包裹已满");
					zObject::destroy(o);
				}									
			}
		}
	}
	else
	{
		Channel::sendSys(this,Cmd::INFO_TYPE_GAME,"您的金子不足");
	}
	return true;
}
