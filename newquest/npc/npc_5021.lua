local sq = the_script();
sq:add(ScriptQuest.NPC_VISIT, 5021);

require "define5021"

function check_time(npc,v)
	player = me();
	--print("ʱ����");
	local qs = v:get("finish_quests"); 
	local qss = v:get("finish_rounds");
	local qstime = v:get("qstime"); 
	v:set("nowtime", time());
	local nowtime = v:get("nowtime");
	v:set("qstimetemp", math.mod(qstime, 86400)); 
	v:set("nowtimetemp", math.mod(time(), 86400)); 
	if qs and (((time()- math.mod(time(), 86400))/86400) > (qstime - math.mod(qstime, 86400))/86400 or (time() - qstime >= 86400)) then
	  v:set("finish_quests", 0); 
	  v:set("finish_rounds", 0); 
	  v:set("state",2);
	  show_dialog(npc, menu_5021_92);	
	  sys(player, 5, "��ʼ���컷����");
	  return 1;
	end 
end

function select_quest70(npc,v)
	player = me();
	local tempnum = math.random(1, 100);
	--print("40~50���");

		if tempnum <= 20 then v:set("type", 1); end
		if tempnum > 20 and tempnum <= 40 then v:set("type", 2); end
		if tempnum > 40 and tempnum <= 60 then v:set("type", 3); end
		if tempnum > 60 and tempnum <= 80 then v:set("type", 4); end
		if tempnum > 80 and tempnum <= 100 then v:set("type", 5); end
		--if tempnum > 90 and tempnum <= 100 then v:set("type", 6); end
		
		local type = v:get("type"); 	
		--����һ�����ͺͱ�����ͬʱ������+1
		local temptype = v:get("temptype"); 
		if temptype == type then 
			v:set("type", temptype+1);
			local type = v:get("type");
				if type == 6 then 
					v:set("type", 3); 
					local type = v:get("type"); 
				end
		end 
			
		if type == 4 then v:set("type", 2); end
		
		local type = v:get("type"); 		
		
	--print(type);
	local id = 0;
	local r = 0;
	local menu = "";
	
		local qs = v:get("finish_quests"); 
		if not qs then
			v:set("finish_quests", 1);
		end

		local qss = v:get("finish_rounds");
		if not qss then					v:set("finish_quests", 1);		end

		local baseexp = 1.4*player.charbase.level*player.charbase.level + 5*player.charbase.level;
		local exp = 7*baseexp*(0.6+0.2*(qs + qss));
		v:set("showexp", exp); 
		local showexp = v:get("showexp");

--��������1����ʱ
	if type == 1 then
	--print("40~50����");

	  if qss < 4 then 
			r = math.random(1, table.getn(visit_npc_list70));
			menu = string.gsub(menu_5021_1, "#place#", visit_npc_list70[r][1]);	
			menu = 	string.gsub(menu, "#npc#", visit_npc_list70[r][2]);
			id = visit_npc_list70[r][2];
		end
		if qss >= 4 then 
			r = math.random(1, table.getn(visit_npc_list702));
			menu = string.gsub(menu_5021_1, "#place#", visit_npc_list702[r][1]);	
			menu = 	string.gsub(menu, "#npc#", visit_npc_list702[r][2]);	
			id = visit_npc_list702[r][2];
		end
		
		menu = string.gsub(menu, "#exp#", showexp);
		menu = string.gsub(menu, "#qs#", (qs+1));
		menu = string.gsub(menu, "#round#", (qss+1));
		
		v:set("questnpc", r);
		--print("type=1 npc");
	end


--ɱ������1����ʱ
	if type == 2 then
	
		if qss < 4 then 
			r = math.random(1, table.getn(kill_npc_list70));
			menu = string.gsub(menu_5021_2, "#name#", kill_npc_list70[r][1]);
			menu = 	string.gsub(menu, "#npc#", kill_npc_list70[r][2]);
			id = kill_npc_list70[r][2]; 
		end
		
		if qss >= 4 then 
			r = math.random(1, table.getn(kill_npc_list702));
			menu = string.gsub(menu_5021_2, "#name#", kill_npc_list702[r][1]);
			menu = 	string.gsub(menu, "#npc#", kill_npc_list702[r][2]);
			id = kill_npc_list702[r][2]; 
		end
		
		v:set("questkill", r);
		
		menu = string.gsub(menu, "#exp#", showexp);
		menu = string.gsub(menu, "#qs#", (qs+1));
		menu = string.gsub(menu, "#round#", (qss+1));
		
	end
	
--�ռ���Ʒ����	
	if type == 3 then
		if qss < 4 then 
			r = math.random(1, table.getn(get_item_list70));
			menu = string.gsub(menu_5021_3, "#name#", get_item_list70[r][1]);	
			menu = 	string.gsub(menu, "#id#", get_item_list70[r][2]);
			id = get_item_list70[r][2];
		end
		if qss >= 4 then 
			r = math.random(1, table.getn(get_item_list702));
			menu = string.gsub(menu_5021_3, "#name#", get_item_list702[r][1]);	
			menu = 	string.gsub(menu, "#id#", get_item_list702[r][2]);
			id = get_item_list702[r][2];
		end
		
		menu = string.gsub(menu, "#exp#", showexp);
		menu = string.gsub(menu, "#qs#", (qs+1));
		menu = string.gsub(menu, "#round#", (qss+1));
	  v:set("questobject", r);
	  v:set("id", id);
	end

--ɱ���ռ���Ʒ����
	if type == 4 then
		if qss < 4 then 
			r = math.random(1, table.getn(kill_get_list70));
			menu = string.gsub(menu_5021_13, "#name#", kill_get_list70[r][1]);	
			menu = 	string.gsub(menu, "#kill#", kill_get_list70[r][2]);
			menu = 	string.gsub(menu, "#item#", kill_get_list70[r][3]);	
			id = kill_get_list70[r][2];
			item = kill_get_list70[r][3];
		end
		if qss >= 4 then 
			r = math.random(1, table.getn(kill_get_list702));
			menu = string.gsub(menu_5021_13, "#name#", kill_get_list702[r][1]);	
			menu = 	string.gsub(menu, "#kill#", kill_get_list702[r][2]);
			menu = 	string.gsub(menu, "#item#", kill_get_list702[r][3]);	
			id = kill_get_list702[r][2];
			item = kill_get_list702[r][3];
		end
		
		menu = string.gsub(menu, "#exp#", showexp);
		menu = string.gsub(menu, "#qs#", (qs+1));
		menu = string.gsub(menu, "#round#", (qss+1));
		v:set("item",item);
	  v:set("questkillobject", r);
	end

--�ռ�װ������
	if type == 5 then
		if qss < 4 then 
			r = math.random(1, table.getn(get_weapon_list70));
			menu = string.gsub(menu_5021_15, "#name#", get_weapon_list70[r][1]);	
			menu = string.gsub(menu, "#id#", get_weapon_list70[r][2]);
			menu = string.gsub(menu, "#color#", get_weapon_list70[r][3]);
			id = get_weapon_list70[r][2];
			color = get_weapon_list70[r][3];
		end
		if qss >= 4 then 
			r = math.random(1, table.getn(get_weapon_list702));
			menu = string.gsub(menu_5021_15, "#name#", get_weapon_list702[r][1]);	
			menu = string.gsub(menu, "#id#", get_weapon_list702[r][2]);
			menu = string.gsub(menu, "#color#", get_weapon_list702[r][3]);
			id = get_weapon_list702[r][2];
			color = get_weapon_list702[r][3];
		end
		
		menu = string.gsub(menu, "#exp#", showexp);
		menu = string.gsub(menu, "#qs#", (qs+1));
		menu = string.gsub(menu, "#round#", (qss+1));
		v:set("color",color);
	  v:set("weaponrequire", r);
	end
	
--ɱBOSS����	
	--print("ɱBOSS�ж�");
	if type == 6 then
	--print("ɱBOSS�ж�ͨ��");
		if qss < 4 then 
			r = math.random(1, table.getn(kill_boss_list));
			menu = string.gsub(menu_5021_17, "#name#", kill_boss_list[r][1]);	
			menu = string.gsub(menu, "#id#", kill_boss_list[r][2]);
			id = kill_boss_list[r][2];
			color = kill_boss_list[r][3];
		end
		if qss >= 4 then 
			r = math.random(1, table.getn(kill_boss_list2));
			menu = string.gsub(menu_5021_17, "#name#", kill_boss_list2[r][1]);	
			menu = string.gsub(menu, "#id#", kill_boss_list2[r][2]);
			id = kill_boss_list2[r][2];
			color = kill_boss_list2[r][3];
		end
		--print("ɱBOSS�ж϶�");
		menu = string.gsub(menu, "#exp#", showexp);
		menu = string.gsub(menu, "#qs#", (qs+1));
		menu = string.gsub(menu, "#round#", (qss+1));
		v:set("name",name);
	  v:set("killboss", r);
	end	
	
	
	show_dialog(npc, menu);	

	return type, id;
end

--����NPC��ʼ
function visit_5021(npc)
--print("40~50����NPC��ʼ");

--�ȼ��ж�
	player = me();
	--print("40~50�ȼ��ж�2");
	if player.charbase.level < 80 or player.charbase.level >= 90 then
			return 0;
	end

--�����ж�
	local v = player.quest:vars(21102);
	if not v then
	--print("no vars");
	v = Vars(21102);
	end

--�����ж�
	local qss = v:get("finish_rounds");
	if not qss then
		v:set("finish_rounds", 1);
	end
	local qs = v:get("finish_quests"); 
	if not qs then
		v:set("finish_quests", 1);
	end

--�����ж�
	if string.len(player.sept_name) == 0 then
		show_dialog(npc, menu_5021_91);
	  return 1;
	end
	
--��ȡ�浵�Ի�����
  local savegame = v:get("savegame");
	if savegame == 1 then
		show_dialog(npc, menu_5021_100);
		return 1;
	end

--����ȼ��жϻ���
	local fl=player.sept_level * 2;
	if  fl == 0 then
		fl=1;
	end
	
--ÿ�ջ�������
	local v = player.quest:vars(21102);
	--��ȡ���˱���
	local uv = player:get_uservar(99);

	if v then
		local qss = v:get("finish_rounds");
		local qs = v:get("finish_quests");
		--print(qs);
	
--�������ջ�����Ի�		
		local uv = player:get_uservar(99);
		local todayagain = uv:get("todayagain");
		--print(todayagain);
		if todayagain == 1 then 
				show_dialog(npc, menu_5021_94);
			return 1;
		end
		
--���ջ�������ʾ	
		local uv = player:get_uservar(99);
		local todayend = uv:get("todayend");
		if todayend == 1 then 
				show_dialog(npc, menu_5021_90);
			return 1;
	  end

--���������ޣ����ø��˱���	
		local uv = player:get_uservar(99);
		local todayend = uv:get("todayend");
		if qss and qss >= fl and todayend ~= 1 then 
				local uv = player:get_uservar(99);
				local todayend = uv:get("todayend");
				if not todayend then 	
					uv:set("todayend", 1);
					else
					uv:set("todayend", 1);
				end
			return 1;
		end

	end
		
		
	if not v then
		v = Vars(21102);
		local type, id = select_quest70(npc,v);
		
		v:set("type", type);
		v:set("id", id);
		v:set("state", 2);
		v:set("npc", npc:tempid());
		player.quest:add(21102, v, player, true);
--		sys(player, 1, "������һ������");
		npc:refresh();
		return 1;
	end

	local v = player.quest:vars(21102);
	local s = v:get("state");
	local type = v:get("type");
	local qss = v:get("finish_rounds");
	if not qss then
		v:set("finish_rounds", 1);
	end
	
	local num = v:gets("num");
	local xxx="null";
	v:sets("num", xxx);
	v:refresh(player,"num")
	player.quest:add(21102, v, player, true);
	
--ÿ������30����������ж�	
	--if (s == 1 or s == -3) and (time()-v:get("starttime")) >= 3600 then
	--			show_dialog(npc, menu_5021_101);
	--		return 1;	
	--end		
	
--��������1������ʾ
	if s == 1 and type == 1 then
			local r=v:get("questnpc");
			if qss < 4 then 
				menu = string.gsub(menu_5021_5, "#place#", visit_npc_list70[r][1]);	
				menu = 	string.gsub(menu, "#npc#", visit_npc_list70[r][2]);
				show_dialog(npc, menu);
			end
			if qss >= 4 then 
				menu = string.gsub(menu_5021_5, "#place#", visit_npc_list702[r][1]);	
				menu = 	string.gsub(menu, "#npc#", visit_npc_list702[r][2]);
			end
			show_dialog(npc, menu);
		return 1;	
	end	

--ɱ��������ʾ
	if s == 1 and type == 2 then
			local r=v:get("questkill");
			if qss < 4 then 
				menu = string.gsub(menu_5021_6, "#name#", kill_npc_list70[r][1]);
				menu = 	string.gsub(menu, "#npc#", kill_npc_list70[r][2]);
			end 
			if qss >= 4 then 
				menu = string.gsub(menu_5021_6, "#name#", kill_npc_list702[r][1]);
				menu = 	string.gsub(menu, "#npc#", kill_npc_list702[r][2]);
			end 
			show_dialog(npc, menu);
		return 1;	
	end	
	
--��Ʒ�ռ�������
	if s == 1 and type == 3 then
			local r=v:get("questobject");
			if qss < 4 then 
				menu = string.gsub(menu_5021_8, "#name#", get_item_list70[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_item_list70[r][2]);
			end 
			if qss >= 4 then 
				menu = string.gsub(menu_5021_8, "#name#", get_item_list702[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_item_list702[r][2]);
			end 
			show_dialog(npc, menu);
		return 1;	
	end	

--��Ʒ�ռ�������	
	if s == 1 and type == 3 then
			local r=v:get("questobject");
			if qss < 4 then 
				menu = string.gsub(menu_5021_4, "#name#", get_item_list70[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_item_list70[r][2]);
			end
			if qss >= 4 then 
				menu = string.gsub(menu_5021_4, "#name#", get_item_list702[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_item_list702[r][2]);
			end
			show_dialog(npc, menu);
		return 1;	
	end	

--ɱ���ռ���Ʒ������
	if s == 1 and type == 4 then
			local r=v:get("questkillobject");
			if qss < 4 then 
				menu = string.gsub(menu_5021_23, "#name#", kill_get_list70[r][1]);	
				menu = 	string.gsub(menu, "#kill#", kill_get_list70[r][2]);
				menu = 	string.gsub(menu, "#item#", kill_get_list70[r][3]);
			end 
			if qss >= 4 then 
				menu = string.gsub(menu_5021_23, "#name#", kill_get_list702[r][1]);	
				menu = 	string.gsub(menu, "#kill#", kill_get_list702[r][2]);
				menu = 	string.gsub(menu, "#item#", kill_get_list702[r][3]);
			end 
			show_dialog(npc, menu);
		return 1;	
	end	

--ɱ���ռ���Ʒ������	
	if s == -3 and type == 4 then
			local r=v:get("questkillobject");
			if qss < 4 then
				menu = string.gsub(menu_5021_31, "#name#", kill_get_list70[r][1]);	
				menu = 	string.gsub(menu, "#kill#", kill_get_list70[r][2]);
				menu = 	string.gsub(menu, "#item#", kill_get_list70[r][3]);
			end 
			if qss >= 4 then
				menu = string.gsub(menu_5021_31, "#name#", kill_get_list702[r][1]);	
				menu = 	string.gsub(menu, "#kill#", kill_get_list702[r][2]);
				menu = 	string.gsub(menu, "#item#", kill_get_list702[r][3]);
			end 
			show_dialog(npc, menu);
		return 1;		
	end
	
--�ռ�װ��������Ի�
	local id = v:get("id");
	local color = v:get("color");
	local weapon = player:get_ob(id, color);
	local weapon2 = player:get_ob((id+10000), color);
	local qs = v:get("finish_quests"); 
	local qss = v:get("finish_rounds");
	--print("timetest");
	
	if s == 1 and type == 5 and (weapon ~= 0 or weapon2 ~= 0) then
		if qss < 4 then	
			local r=v:get("weaponrequire");
			menu = string.gsub(menu_5021_35, "#name#", get_weapon_list70[r][1]);	
			menu = 	string.gsub(menu, "#id#", get_weapon_list70[r][2]);
			menu = 	string.gsub(menu, "#color#", get_weapon_list70[r][3]);
			show_dialog(npc, menu);
			return 1;
		end
		if qss >= 4 then	
			local r=v:get("weaponrequire");
			menu = string.gsub(menu_5021_35, "#name#", get_weapon_list702[r][1]);	
			menu = 	string.gsub(menu, "#id#", get_weapon_list702[r][2]);
			menu = 	string.gsub(menu, "#color#", get_weapon_list702[r][3]);
			show_dialog(npc, menu);
			return 1;
		end 
	end		
	
--�ռ�װ��������Ի�
	if s == 1 and type == 5 then
			local r=v:get("weaponrequire");
			if qss < 4 then
				menu = string.gsub(menu_5021_45, "#name#", get_weapon_list70[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_weapon_list70[r][2]);
				menu = 	string.gsub(menu, "#color#", get_weapon_list70[r][3]);
			end
			if qss >= 4 then
				menu = string.gsub(menu_5021_45, "#name#", get_weapon_list702[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_weapon_list702[r][2]);
				menu = 	string.gsub(menu, "#color#", get_weapon_list702[r][3]);
			end
			show_dialog(npc, menu);
		return 1;	
	end	
	
--ɱBOSS������Ի�
	--print("�жϣ���");
	if type == 6 then
	--print("�ж�ͨ������");
			local r=v:get("killboss");
			if qss < 4 then
				menu = string.gsub(menu_5021_7, "#name#", kill_boss_list[r][1]);	
				menu = 	string.gsub(menu, "#id#", kill_boss_list[r][2]);
				show_dialog(npc, menu);
			end
			if qss >= 4 then
				menu = string.gsub(menu_5021_7, "#name#", kill_boss_list2[r][1]);	
				menu = 	string.gsub(menu, "#id#", kill_boss_list2[r][2]);
			end
			show_dialog(npc, menu);
		return 1;	
	end		
	
	
--ɱBOSS������ʾ
	if s == 1 and type == 6 then
			local r=v:get("killboss");
			if qss < 4 then 
				menu = string.gsub(menu_5021_7, "#name#", kill_boss_list[r][1]);	
				menu = 	string.gsub(menu, "#id#", kill_boss_list[r][2]);
				show_dialog(npc, menu);
			end
			if qss >= 4 then 
				menu = string.gsub(menu_5021_7, "#name#", kill_boss_list2[r][1]);	
				menu = 	string.gsub(menu, "#id#", kill_boss_list2[r][2]);
			end
			show_dialog(npc, menu);
		return 1;	
	end		

	if s == -3 then
		show_dialog(npc, menu_5021_11);
		return 1;		
	end
	
	if s == 2 then
		local type, id = select_quest70(npc,v);
		v:set("type", type);
		v:set("id", id);
--		v:set("state", 1);
		v:set("npc", npc:tempid());
--		npc:refresh();	
		return 1;
	end
	
	return 0;
end

--******************�Ի�����ж�ѡ��********************************

function accept_5021(npc, id, flag)
		player = me();
		local v = player.quest:vars(id);
		local qss = v:get("finish_rounds");
			if not qss then
				v:set("finish_rounds", 1);
			end

--��������1���������ж�
--print("40~50��������");
		if( v:get("state") == 2) and flag == 1 then
			local r=v:get("questnpc");
			
			if qss < 4 then 
				menu = string.gsub(menu_5021_5, "#place#", visit_npc_list70[r][1]);	
				menu = 	string.gsub(menu, "#npc#", visit_npc_list70[r][2]);
				show_dialog(npc, menu);
				local xxxx="���Ŵ���#place#Ȼ�����Դ�㱨";
				xxxx = string.gsub(xxxx, "#place#", visit_npc_list70[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			
			if qss >= 4 then 
				menu = string.gsub(menu_5021_5, "#place#", visit_npc_list702[r][1]);	
				menu = 	string.gsub(menu, "#npc#", visit_npc_list702[r][2]);
				show_dialog(npc, menu);
				local xxxx="���Ŵ���#place#Ȼ�����Դ�㱨";
				xxxx = string.gsub(xxxx, "#place#", visit_npc_list702[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			
			v:set("state",1);
			player:add_ob(2068, 1, 0, 0);
			sys(player, 1, "�����˸�����");	
			v:set("starttime", time());
		
			--����ʼʱ���ʱ
			local qs = v:get("finish_quests");
			local qss = v:get("finish_rounds");
      if qs == 0 and qss == 0 then
      	v:set("qstime", time());
			end
					
			player.quest:add(21102, v, player, true);
			
			npc:refresh();
			return 1;
		end
	  
--ɱ������1���������ж�
	  if( v:get("state") == 2) and flag == 2 then
			local r=v:get("questkill");
			
			if qss < 4 then 
				menu = string.gsub(menu_5021_6, "#name#", kill_npc_list70[r][1]);
				menu = 	string.gsub(menu, "#npc#", kill_npc_list70[r][2]);
				local xxxx="����#name#30ֻȻ�����Դ�㱨";
				xxxx = string.gsub(xxxx, "#name#", kill_npc_list70[r][1]);
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			if qss >= 4 then 
				menu = string.gsub(menu_5021_6, "#name#", kill_npc_list702[r][1]);
				menu = 	string.gsub(menu, "#npc#", kill_npc_list702[r][2]);
				local xxxx="����#name#30ֻȻ�����Դ�㱨";
				xxxx = string.gsub(xxxx, "#name#", kill_npc_list702[r][1]);
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			
			show_dialog(npc, menu);
			v:set("state",1);
			sys(player, 5, "������ɱ������");	
			v:set("starttime", time());
	
			--����ʼʱ���ʱ
			local qs = v:get("finish_quests");
			local qss = v:get("finish_rounds");
      if qs == 0 and qss == 0 then
      	v:set("qstime", time());
			end	
	
			player.quest:add(21102, v, player, true);
				
			npc:refresh();
			return 1;
		end
		
--���ʡ�ɱ����������		
--print("40~50���˽�����");

		if ( v:get("state") == -3) and flag == 11 then
		
				v:set("state", 2);
				v:set("kills", 0);
				v:set("item_nums", 0);
				npc:refresh();
				
				local qs = v:get("finish_quests");
				if not qs then
					v:set("finish_quests", 1);
				else
					v:set("finish_quests", qs+1);
				end
				
				if math.mod(qs+1, 5) == 0 then
					sys(player, 5, "��ϲ�������һ����");
						if not qss then
							v:set("finish_rounds", 1);
							v:set("finish_quests", 0);
						else
							v:set("finish_rounds", qss+1);
							v:set("finish_quests", 0);
						end
					npc:refresh();
				end
				
				local type = v:get("type"); 	
				v:set("temptype", type);
				local temptype = v:get("temptype"); 	

				gain_exp(player, qs, qss+1);
				check_time(npc,v)
				visit_5021(npc);
			return 1;		
		end		
		
--�ռ���Ʒ1���������ж�	
		if( v:get("state") == 2) and flag == 3 then
			local r=v:get("questobject");
			if qss < 4 then 
				menu = string.gsub(menu_5021_4, "#name#", get_item_list70[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_item_list70[r][2]);
				local xxxx="ȥ�ռ�#name#5������Դ";
				xxxx = string.gsub(xxxx, "#name#", get_item_list70[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			if qss >= 4 then 
				menu = string.gsub(menu_5021_4, "#name#", get_item_list702[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_item_list702[r][2]);
				local xxxx="ȥ�ռ�#name#5������Դ";
				xxxx = string.gsub(xxxx, "#name#", get_item_list702[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			
			show_dialog(npc, menu);	
			v:set("state",1);
			sys(player, 1, "�������ռ���Ʒ����");	
			v:set("starttime", time());

			--����ʼʱ���ʱ
			local qs = v:get("finish_quests");
			local qss = v:get("finish_rounds");
      if qs == 0 and qss == 0 then
      	v:set("qstime", time());
			end
			
			player.quest:add(21102, v, player, true);
				
			npc:refresh();
			return 1;
		end
		
--�ռ���Ʒ������
		local id = v:get("id"); 
		local type = v:get("type"); 	
		
		if ( v:get("state") == 1) and type == 3 and flag == 8 and player:have_ob(id, 5, 0, 0) then
		--if ( v:get("state") == 1) and type == 3 and flag == 8 then
			sys(player, 1, "������ռ���Ʒ����");		
			player:remove_ob(id, 5, 0);
		
			v:set("state", 2);
			v:set("kills", 0);
			v:set("id", 0);
			v:set("item_nums", 0);
			npc:refresh();
		
			local qs = v:get("finish_quests");
				if not qs then
					v:set("finish_quests", 1);
				else
					v:set("finish_quests", qs+1);
				end
				
				if math.mod(qs+1, 5) == 0 then
					sys(player, 5, "��ϲ�������һ����");
						if not qss then
							v:set("finish_rounds", 1);
							v:set("finish_quests", 0);
						else
							v:set("finish_rounds", qss+1);
							v:set("finish_quests", 0);
						end
					--print("finish round");
	
					npc:refresh();
				end
				
			v:set("temptype", 3);
			gain_exp(player, qs, qss+1);
			
			player.quest.refresh(id);	
			npc:refresh();
			check_time(npc,v)	
			visit_5021(npc);
			return 1;
		end		
		
--ɱ���ռ���Ʒ�������ж�  
		if( v:get("state") == 2) and flag == 13 then
			local r=v:get("questkillobject");
			if qss < 4 then
				menu = string.gsub(menu_5021_23, "#name#", kill_get_list70[r][1]);	
				menu = 	string.gsub(menu, "#kill#", kill_get_list70[r][2]);
				menu = 	string.gsub(menu, "#item#", kill_get_list70[r][3]);
				local xxxx="#name#��Ȼ�����Դ�㱨";
				xxxx = string.gsub(xxxx, "#name#", kill_get_list70[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			if qss >= 4 then
				menu = string.gsub(menu_5021_23, "#name#", kill_get_list702[r][1]);	
				menu = 	string.gsub(menu, "#kill#", kill_get_list702[r][2]);
				menu = 	string.gsub(menu, "#item#", kill_get_list702[r][3]);
				local xxxx="#name#��Ȼ�����Դ�㱨";
				xxxx = string.gsub(xxxx, "#name#", kill_get_list702[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			
			show_dialog(npc, menu);
			sys(player, 5, "�����ռ���Ʒ����");		
			v:set("state",1);
			v:set("starttime", time());

			--����ʼʱ���ʱ
			local qs = v:get("finish_quests");
			local qss = v:get("finish_rounds");
      if qs == 0 and qss == 0 then
      	v:set("qstime", time());
			end
	
			player.quest:add(21102, v, player, true);
							
			npc:refresh();
			return 1;
		end
		
		player = me();
				
		local s = v:get("state");
		local type = v:get("type");
		local id = v:get("id");

--ɱ���ռ���Ʒ�������ж�	
		local item = v:get("item");
		if ( v:get("state") == -3) and type == 4 and flag == 31 and player:have_ob(item, 6, 0, 0) then
			sys(player, 5, "������ռ���Ʒ����");		
			player:remove_ob(item, 10, 0);
			v:set("state", 2);
			v:set("kills", 0);
			v:set("id", 0);
			v:set("item_nums", 0);
			npc:refresh();
			
			local qs = v:get("finish_quests");
				if not qs then
					v:set("finish_quests", 1);
				else
					v:set("finish_quests", qs+1);
				end
				
				if math.mod(qs+1, 5) == 0 then
					sys(player, 5, "��ϲ�������һ����");
						if not qss then
							v:set("finish_rounds", 1);
							v:set("finish_quests", 0);
						else
							v:set("finish_rounds", qss+1);
							v:set("finish_quests", 0);
						end
					--print("finish round");
	
					npc:refresh();
				end
			
			v:set("temptype", 4);

			gain_exp(player, qs, qss+1);
			player.quest.refresh(id);	
			npc:refresh();
			check_time(npc,v)
		  visit_5021(npc);
			return 1;
		end

--�ռ�װ���������ж�  
		if( v:get("state") == 2) and type == 5 and flag == 15 then
			local r=v:get("weaponrequire");
		  if qss < 4 then 
				menu = string.gsub(menu_5021_45, "#name#", get_weapon_list70[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_weapon_list70[r][2]);
				menu = 	string.gsub(menu, "#color#", get_weapon_list70[r][3]);
				local xxxx="�ռ�һ��#name#������Դ";
				xxxx = string.gsub(xxxx, "#name#", get_weapon_list70[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
				v:set("starttime", time());
			end
			
			if qss >= 4 then 
				menu = string.gsub(menu_5021_45, "#name#", get_weapon_list702[r][1]);	
				menu = 	string.gsub(menu, "#id#", get_weapon_list702[r][2]);
				menu = 	string.gsub(menu, "#color#", get_weapon_list702[r][3]);
				show_dialog(npc, menu);
				local xxxx="�ռ�һ��#name#������Դ";
				xxxx = string.gsub(xxxx, "#name#", get_weapon_list702[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			
			show_dialog(npc, menu);
			sys(player, 5, "�������ռ�װ������");	
			v:set("starttime", time());
			v:set("state",1);
			
			--����ʼʱ���ʱ
			local qs = v:get("finish_quests");
			local qss = v:get("finish_rounds");
      if qs == 0 and qss == 0 then
      	v:set("qstime", time());
			end
			
			player.quest:add(21102, v, player, true);
							
			npc:refresh();
			return 1;
		end
		
--�ռ�װ���������ж�
		local id = v:get("id");
		local color = v:get("color");
		local weapon = player:get_ob(id, color);
		local weapon2 = player:get_ob((id+10000), color);
		if ( v:get("state") == 1) and type == 5 and flag == 35 and (weapon ~= 0 or weapon2 ~= 0) then
			sys(player, 5, "������ռ�װ������");		
			player:del_ob(weapon);
			v:set("state", 2);
			v:set("color", 0);
			v:set("id", 0);
			v:set("weapon", 0);
			npc:refresh();
		
			local qs = v:get("finish_quests");
				if not qs then
					v:set("finish_quests", 1);
				else
					v:set("finish_quests", qs+1);
				end
				
				if math.mod(qs+1, 5) == 0 then
					sys(player, 5, "��ϲ�������һ����");
						if not qss then
							v:set("finish_rounds", 1);
							v:set("finish_quests", 0);
						else
							v:set("finish_rounds", qss+1);
							v:set("finish_quests", 0);
						end
					--print("finish round");
	
					npc:refresh();
				end
				
			v:set("temptype", 5);

			gain_exp(player, qs, qss+1);
			
			player.quest.refresh(id);	
			npc:refresh();
			
			check_time(npc,v)
		  visit_5021(npc);
			return 1;
		end		
	
		local s = v:get("state");
		--if s == -3 then
		--	sys(player, 1, "����˸�����");		
		--	player:add_exp(100, false, 0, 0);	
		--	v:set("state", -1);	
		--	player.quest.refresh(id);	
		--	npc:refresh();	
		--	return 1;
		--end
		
--ɱBOSS����������ж�
		if( v:get("state") == 2) and type == 6 and flag == 17 then
			local r=v:get("killboss");
			
			if qss < 4 then 
				menu = string.gsub(menu_5021_17, "#name#", kill_boss_list[r][1]);	
				menu = string.gsub(menu, "#id#", kill_boss_list[r][2]);
				show_dialog(npc, menu);
				local xxxx="ȥɱ#name#Ȼ�����Դ�㱨";
				xxxx = string.gsub(xxxx, "#name#", kill_boss_list[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			
			if qss >= 4 then 
				menu = string.gsub(menu_5021_17, "#name#", kill_boss_list2[r][1]);	
				menu = string.gsub(menu, "#id#", kill_boss_list2[r][2]);
				show_dialog(npc, menu);
				local xxxx="ȥɱ#name#Ȼ�����Դ�㱨";
				xxxx = string.gsub(xxxx, "#name#", kill_boss_list2[r][1]);	
				v:sets("text", xxxx);
				v:refresh(player,"text")
			end
			
			v:set("state",1);
			sys(player, 1, "�����˸�����");	
			v:set("starttime", time());
		
			--����ʼʱ���ʱ
			local qs = v:get("finish_quests");
			local qss = v:get("finish_rounds");
      if qs == 0 and qss == 0 then
      	v:set("qstime", time());
			end
					
			player.quest:add(21102, v, player, true);
			
			npc:refresh();
			return 1;
		end		
		
		
--�����浵		
		if s > 0 and flag == 99 and player.charbase.honor < 900 then
			sys(player, 5, "���������������浵ʧ��");		
			return 1;
		end

		if s > 0 and flag == 99 and player.charbase.honor >= 900 then
			sys(player, 5, "��ʽ����浵�ɹ�");		
			local qs = v:get("finish_quests");
			local qss = v:get("finish_rounds");
			v:set("saveqs", qs);	
			v:set("saveqss", qss);	
			v:set("savegame", 1);
			local saveqs = v:get("saveqs");
			local saveqss = v:get("saveqss");
			
			player.charbase.honor = player.charbase.honor - 900
			
			player.quest.refresh(id);	
			npc:refresh();	
			return 1;
		end


--����ȡ��	
		local savegame = v:get("savegame");
		local saveqs = v:get("saveqs");
		local saveqss = v:get("saveqss");
		if savegame == 1 and flag == 100 then
			sys(player, 5, "�ɹ�ȡ����ʽ����浵");		
			v:set("finish_quests", saveqs);	
			v:set("finish_rounds", saveqss);	
			v:set("savegame", 0);
			v:set("starttime", time());
			v:set("qstime", time());
			
			v:set("saveqs", 0);	
			v:set("saveqss", 0);	
			v:set("savegame", 0);
			local saveqs = v:get("saveqs");
			local saveqss = v:get("saveqss");
			
			player.quest.refresh(id);	
			npc:refresh();	
			return 1;
		end
	
--���û�����	
		if flag == 94 then
			sys(player, 5, "���¿�ʼ���ջ�ʽ����");		

			v:set("finish_rounds", 0);
			v:set("finish_quests", 0);
			
			local uv = player:get_uservar(98);
			if not todayagain then 	
				uv:set("todayend", 0);
				uv:set("todayagain", 0);
			else
				uv:set("todayend", 0);
				uv:set("todayagain", 0);
			end

			local qs = v:get("finish_quests");
			local qss = v:get("finish_rounds");
		
			player.quest.refresh(id);	
			npc:refresh();	
			visit_5021(npc);
			return 1;
		end		
		
--��ʱ�ؽ�����
    if flag == 101 then 
  	  sys(player, 5, "�ؽӱ�����");
    	--����ʼʱ���ʱ
			v:set("starttime", time());
			v:set("state", 2);
			visit_5021(npc);
			return 1;
		end
		
--������������
    if flag == 102 then 
  	  sys(player, 5, "�������˽�����������������");
    	--����ʼʱ���ʱ
    	
    	local uv = player:get_uservar(98);
				if not todayend then 	
					uv:set("todayend", 1);
				else
					uv:set("todayend", 1);
				end
    	
			v:set("starttime", time());
			v:set("state", 2);
			return 1;
		end	
	
	return 0;
end

function state_5021(npc)
	player = me();
		if not player then return 0; end
		--print("40~50�ȼ��ж�1");
		if player.charbase.level < 80 or player.charbase.level >= 90 then
			return 0;
		end
		
	local v = player.quest:vars(21102);
	if not v then
		--print("start");
		return Quest.START;
	end

	local s = v:get("state");
	
	if s == 1 then
		--print("doing");
		return Quest.DOING;
	end

	if s == -3 then
		--print("finish");
		return Quest.FINISH;
	end

	return 0;
end