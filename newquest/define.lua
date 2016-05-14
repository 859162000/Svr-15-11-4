--visit_npc_list = { {"��Դ���56001", 56001}, {"��Դ���56003", 56003}, {"��Դ���56006", 56006} };
--kill_npc_list = { {"����50007", 50007}, {"����50021", 50021}, {"����10038", 10038} };
--get_item_list = { {"�޶�", 1800}, {"��ë", 1801}, {"��ë", 1802} };
--kill_get_list = { {"ɱ50007���1800�޶�", 50007, 1800}, {"ɱ50021���1801��ë", 50021, 1801}, {"ɱ10038���1802�޶�", 10038, 1802} };

menu_247_1 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ��ֻ�ܽ�һ�Σ����Ƿ�Ҫ��ʼ���������");
this:AddTaskCmdItem("��������", "accept_247", 21101, 1)
end
function Dialog45()
this:AddTalk("���Ұ�����Ž���#place#��Ȼ������һظ��㽱���ġ�");
end
function MoveCanClose()
	return false
end

]]

menu_247_2 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ��ֻ�ܽ�һ�Σ����Ƿ�Ҫ��ʼ���������");
this:AddTaskCmdItem("��������", "accept_247", 21101, 2)
end
function Dialog45()
this:AddTalk("����ɱ#npc#����5ֻ��Ȼ����������ҡ�");
end
function MoveCanClose()
	return false
end

]]


menu_247_3 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ��ֻ�ܽ�һ�Σ����Ƿ�Ҫ��ʼ���������");
this:AddTaskCmdItem("��������", "accept_247", 21101, 3)
end
function Dialog45()
this:AddTalk("����Ҫ3��#name#����ƷIDΪ#id#��ȫ���ҵ����������ҡ�");
end
function MoveCanClose()
	return false
end

]]

menu_247_13 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ��ֻ�ܽ�һ�Σ����Ƿ�Ҫ��ʼ���������");
this:AddTaskCmdItem("��������", "accept_247", 21101, 13)
end
function MoveCanClose()
	return false
end

]]

menu_247_23 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("#name#��Ҫɱ�Ĺ���Ϊ#kill#���õ�3��#item#����ҡ�");
end

]]

menu_247_4 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����Ҫ3��#name#����ƷIDΪ#id#��ȫ���ҵ����������ҡ�");
end

]]

menu_247_5 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("���Ұ�����Ž���#place#��Ȼ������һظ��㽱���ġ�");
end

]]

menu_247_6 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����ɱ#npc#����5ֻ��Ȼ����������ҡ�");
end

]]


menu_247_11 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("��ϲ�����������������Ľ���");
end

]]

menu_247_8 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("3��#name#����ƷIDΪ#id#�����Ѿ��ռ�����ô��");
this:AddTaskCmdItem("������Ʒ", "accept_247", 21101, 8)
end

]]


menu_247_80 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("�����������ˣ����ȥ��������֪���ˡ�");
end

]]

function visit_npc(npc)
	--print("visit");
	player = me();
	local v = player.quest:vars(21101);
	if not v then
		return 0;
	end

	local s = v:get("state");
	local id = v:get("id");
	local type = v:get("type");
	--print(s);
	--print(id);
	--print(type);
	--print(npc:id());

	if s == 1 and id==npc:id() and type == 1 then
		--print(1);
		show_dialog(npc, menu_247_80);	
		--print(2);
		player:remove_ob(2068, 1, 0);
		v:set("state", -3);	
		npc:refresh();
		local tempid = v:get("npc");
		refresh_npc(tempid);		
		return 1;
	end	
	
	return 0;
end

function state_npc(npc)
	player = me();
	if not player then return 0; end
	
	local v = player.quest:vars(21101);
	if not v then
		return 0;
	end

	local s = v:get("state");
	local id = v:get("id");
	local type = v:get("type");
	
	if s == 1 and id==npc:id() and type == 1 then
		return Quest.FINISH;
	end

	return 0;
end


function kill_npc(npc)
	--print("kill");
	player = me();
	local v = player.quest:vars(21101);
	if not v then return 0; end
	
	local s = v:get("state");
	local id = v:get("id");
	local type = v:get("type");
	local item = v:get("item");
	
	--print(npc:id());
	--print(1);
	
	if s == 1 and id==npc:id() and type == 2 then
		local kill = v:get("kills");
		
		--print(2);
		if not kill then 
			v:set("kills", 1); 
		else		
			v:set("kills", kill+1);
			if (kill+1) >= 3 then
				sys(player, 1, "��ϲ�������һ������");
				v:set("state", -3);
				local tempid = v:get("npc");
				refresh_npc(tempid);
			end
		end
				
		return 1;
	end
	
	

	
	return 0;
end




function get_object(object)
	--print("get_ob");
	player = me();
	local v = player.quest:vars(21101);
	if not v then return 0; end
	--print(3);
	
	local s = v:get("state");
	local id = v:get("id");
	local item = v:get("item");
	local type = v:get("type");
	
	--if s == 1 and id==npc:id() and type == 3 then
		if s == 1 and type == 3 then

			local get = v:get("item_nums");
			
			--print(2);
			if not get then 
				v:set("item_nums", 1); 
				--sys(player, 1, "���õ���");
			else		
				v:set("item_nums", get+1);
				--sys(player, 1, "���õ���");
				if get >= 2 then
					sys(player, 1, "��ϲ���ռ����㹻����Ʒ");
					v:set("state", -3);
					local tempid = v:get("id");
					refresh_npc(tempid);
				end
			end
				
			return 1;
		end
		
		
		local s = v:get("state");
		local id = v:get("id");
		local item = v:get("item");
		local type = v:get("type");
		
			--print(5);
			--print(s);
			--print(type);
			--print(id);
			--print(item);
	
		--if s == 1 and id==npc:id() and item==npc:item() and type == 4 then
		if s == 1 and type == 4 then

			local get = v:get("item_nums");
			
			--print(2);
			if not get then 
				v:set("item_nums", 1); 
				sys(player, 5, "�õ���");
			else		
				v:set("item_nums", get+1);
				sys(player, 5, "�õ���");
				if get >= 2 then
					sys(player, 1, "��ϲ���ռ����㹻����Ʒ");
					v:set("state", -3);
					player:remove_ob(item, 3, 0);
					local tempid = v:get("item");
					refresh_npc(tempid);
				end
			end
				
			return 1;
		end
	
	return 0;
end

BASE_EXP = 90;
BASE_NUM = 5;

function gain_exp(player, qs)
	local exp = BASE_EXP*player.charbase.level*(0.8+0.2*math.mod(qs, BASE_NUM)*(qs/BASE_NUM));
	player:add_exp(exp, false, 0, 0, false);
end
