LUA_PATH = "./newquest/?.lua";

print("可以进游戏啦~~");


require "npc/npc_421";
require "npc/npc_422";
require "npc/npc_423";
require "npc/npc_424";
require "npc/npc_425";
require "npc/npc_426";
require "npc/npc_427";
require "npc/npc_428";
require "npc/npc_429";
require "npc/npc_430";
require "npc/npc_431";
require "npc/npc_432";
require "npc/npc_433";
require "npc/npc_434";
require "npc/npc_435";
require "npc/npc_436";
require "npc/npc_437";
require "npc/npc_440";
require "npc/npc_441";
require "npc/npc_442";
require "npc/npc_443";
require "npc/npc_444";
require "npc/npc_447";
require "npc/npc_448";
require "npc/npc_449";
require "npc/npc_450";
require "npc/npc_451";
require "npc/npc_452";
require "npc/npc_453";
require "npc/npc_454";
require "npc/npc_455";
require "npc/npc_456";
require "npc/npc_457";
require "npc/npc_460";
require "npc/npc_461";
require "npc/npc_462";
require "npc/npc_463";
require "npc/npc_464";
require "npc/npc_467";
require "npc/npc_469";
require "npc/npc_470";
require "npc/npc_497";
require "npc/npc_820";
require "npc/npc_821";
require "npc/npc_822";
require "npc/npc_823";
require "npc/npc_825";



require "npc/npc_10207";
require "npc/npc_10216";
require "npc/npc_10219";
require "npc/npc_10221";
require "npc/npc_10229";
require "npc/npc_10230";
require "npc/npc_15022";
require "npc/npc_15030";
require "npc/npc_15037";
require "npc/npc_15094";
require "npc/npc_15147";
require "npc/npc_15150";
require "npc/npc_15156";
require "npc/npc_15170";
require "npc/npc_15173";
require "npc/npc_15176";
require "npc/npc_16034";
require "npc/npc_16047";
require "npc/npc_16071";
require "npc/npc_16110";
require "npc/npc_16118";
require "npc/npc_16215";
require "npc/npc_16219";
require "npc/npc_16231";
require "npc/npc_16236";
require "npc/npc_16241";
require "npc/npc_16256";
require "npc/npc_16261";
require "npc/npc_16309";
require "npc/npc_16313";
require "npc/npc_16317";
require "npc/npc_16348";
require "npc/npc_16461";
require "npc/npc_16465";
require "npc/npc_16469";
require "npc/npc_16476";
require "npc/npc_16482";
require "npc/npc_16486";
require "npc/npc_16579";
require "npc/npc_16583";
require "npc/npc_16589";




function enter()
	--print("enter");
	player = me();
	sys(player, 1, "进入游戏");		
	
	--每个任务30分钟内完成判断	
	local v = player.quest:vars(21102);

	--print("30分钟内完成");	
	local quittime = v:gets("quittime");
	
	local s = v:get("state");
	if not s then
		v:set("state", 2);
	end
	local s = v:get("state");
	v:set("nowtime", time());
	local quittime=v:get("quittime");
	local nowtime = v:get("nowtime");
	local havequit = v:get("havequit");
	--print(havequit);
	
	if quittime > 1 and havequit == 1 then
		v:set("quittimetmp", math.mod(quittime, 86400)); 
		v:set("nowtimetemp", math.mod(time(), 86400)); 

		if (((time()- math.mod(time(), 86400))/86400) > (quittime - math.mod(quittime, 86400))/86400 or (time() - quittime >= 86400)) then
			--print("时间检测通过");
		  v:set("finish_quests", 0); 
		  v:set("finish_rounds", 0); 
		  v:set("state",2);
	  	v:set("level",1);
	  	v:set("havequit", 2);
	  	local havequit = v:get("havequit");
	  	--print(havequit);
	  	
	  	local uv = player:get_uservar(98);
  		if todayend == 1 then 	
				uv:set("todayend", 2);
			end
	  	
	  	local todaybegin = v:get("todaybegin");
			if todaybegin == 1 then
				v:set("todaybegin", 2);
			end
	  	
		 --show_dialog(npc, menu_497_92);	
		  sys(player, 5, "由于离线时间过长，重新开始今天环任务");
		  return 1;
		end 
		
	end
	
	--print("离线时间过长");	
	local havequit = v:get("havequit");
	if (time()-v:get("quittime")) >= 3600 and havequit == 1 then
			--print("离线时间过长");
				--show_dialog(npc, menu_497_101);
			sys(player, 5, "离线时间过长，今天的环任务结束");

    	local uv = player:get_uservar(98);
  		if not todayend then 	
				uv:set("todayend", 1);
			else
				uv:set("todayend", 1);
			end
			
			v:set("havequit", 2);
			local havequit = v:get("havequit");
	  	--print(havequit);
	  	
			local todaybegin = v:get("todaybegin");
			if todaybegin == 1 then
				v:set("todaybegin", 2);
			end	
				
			v:set("starttime", time());
			v:set("state", 2);
			v:set("level",1);
				
			--return 1;	
	end				
	
	return 1;
end


function quit()
	player = me();
	--sys(player, 1, "退出测试");		
	local v = player.quest:vars(21102);
	
	if not v then
		v = Vars(21102);
	end	
	
	local todaybegin = v:get("todaybegin");
	local havequit = v:get("havequit");
	if todaybegin == 1 then
		v:set("todaybegin", 2);
		v:set("quittime", time());
		if havequit == 0 or havequit == 2 then 
			--print("quittime");
			v:set("havequit", 1);
			local havequit = v:get("havequit");
	  	--print(havequit);
		end
	end
	
	--local timerenew = v:get("timerenew");
	local quittime=v:get("quittime");
	--v:set("timerenew",2);
	--print(quittime);
	
	return 1;
end


function die()
	--print("die");
	--player = me();
	--sys(player, 1, "死亡测试");		
	
	return 1;
end

