visit_npc_list60 = { {"��˳ǵ����Ľ�", 425},{"��β�����˹", 421},{"��Դ��ĸ߽���", 440},{"�׹Ƕ�������ϣʥ", 447} };
visit_npc_list602 = { {"��β�������", 422},{"��˳ǵ�����", 434},{"�����ĵ����Ƿ�", 453},{"������Ļ���", 451} };

get_weapon_list60 = { {"85��Ѫ����(��ɫ)", 10309, 0}, {"85��������(��ɫ)", 10509, 0}, {"85������(��ɫ)", 10559, 0}, {"85��������(��ɫ)", 10609, 0}, {"85�������(��ɫ)", 10659, 0}}; 
get_weapon_list602 = { {"85��Ѫ����(��ɫ)", 10309, 1}, {"85��������(��ɫ)", 10509, 1}, {"85������(��ɫ)", 10559, 1}, {"85��������(��ɫ)", 10609, 1}, {"85�������(��ɫ)", 10659, 1}}; 

kill_npc_list60 = { {"���ֶ��Ų��90��Ы����", 16461}, {"�׹Ƕ�ʮ���90������׹�", 16486}, {"����̶��90��������", 15150}, {"�ʳǵ�90���Ѿ�����", 16309} };
kill_npc_list602 = { {"���ֶ��Ų��95��ͨħ�㾫", 16465}, {"����̶��95�������", 15156}, {"������һ���95��������", 16110}, {"�ʳǵ�95���Ѿ�����", 16313} };

get_item_list60 = { {"���۹�", 1881}, {"����", 1974}, {"����", 1908} };
get_item_list602 = { {"���߲�", 1835}, {"Ұɽ��", 1882}, {"����", 1974} };

--kill_get_list60 = { {"ɱ�������������ʦ���10��ҹ����", 16178, 2050}, {"ɱ�׹Ƕ�����Ŀ�צ��ʬ���10��ڲ�", 16024, 2065}, {"ɱ�������������ʦ���10��ȾѪ�ĵ�", 16178, 2062} };

--kill_get_list602 = { {"ɱ�Ͻ�60�������ǹ�����10����ҩ", 16215, 2088}, {"ɱ�Ĳ�60���İ׹Ǹ��ֻ��10����������", 16033, 2023}, {"ɱ�׹Ƕ��Ĳ��ʳԹ��ʬ��ý�ʬ����", 16034, 2125} };

kill_boss_list = { {"����ɽһկ�ϵ�ɽ���̿�ID:16440", 16440} };
kill_boss_list2 = { {"����ɽ��կ�ϵ�ɽ����ʿID:16441", 16441} };

menu_822_1 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_822", 21102, 1)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_822", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������900�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_822", 21102, 99)
end
function Dialog45()
this:AddTalk("���Ұ�����Ž���#place#��Ȼ������Ҹ��㽱����");
end
function MoveCanClose()
	return false
end

]]

menu_822_2 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_822", 21102, 2)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_822", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_822", 21102, 99)
end
function Dialog45()
this:AddTalk("����ɱ#npc#5ֻ��Ȼ����������ҡ�");
end
function MoveCanClose()
	return false
end

]]

--�ռ���Ʒ����
menu_822_3 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_822", 21102, 3)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_822", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_822", 21102, 99)
end
function Dialog45()
this:AddTalk("����Ҫ3��#name#��ȫ���ҵ����������ҡ�");
end
function MoveCanClose()
	return false
end

]]

menu_822_13 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_822", 21102, 13)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_822", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_822", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

--�ռ�װ��������
menu_822_15 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_822", 21102, 15)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_822", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_822", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

--ɱBOSS������Ի�
menu_822_17 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_822", 21102, 17)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_822", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_822", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

menu_822_23 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("ȥ#name#����ҡ�");
this:AddCloseItem("֪����")
end

]]

--�ռ�װ��֪ͨ
menu_822_45 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("ȥ�ռ�һ��#name#���ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_822_4 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����Ҫ5��#name#��ȫ���ҵ����������ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_822_5 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("���Ұ�����Ž���#place#���������һ���㽱���ġ�");
this:AddCloseItem("֪����")
end

]]

menu_822_6 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����ɱ#name#30ֻ��Ȼ����������ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_822_7 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����ɱ#name#��Ȼ����������ҡ�");
this:AddCloseItem("֪����")
end

]]


menu_822_11 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("��ϲ���������������������ȡ����ô��");
this:AddTaskCmdItem("���ܽ���", "accept_822", 21102, 11)
end

]]

menu_822_8 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("5��#name#�����Ѿ��ռ�����ô��");
this:AddTaskCmdItem("�ó���Ʒ", "accept_822", 21102, 8)
end

]]

menu_822_31 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("10����ô���Ū����������̫�ܸ��ˣ�����ҡ�");
this:AddTaskCmdItem("�ó���Ʒ", "accept_822", 21102, 31)
end

]]

--�ռ�װ����������Ի�
menu_822_35 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("#name#��ô���Ū����������̫�ܸ��ˣ�����ҡ�");
this:AddTaskCmdItem("�ó���Ʒ", "accept_822", 21102, 35)
end

]]


menu_822_80 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("�����������ˣ����ȥ���߻�ʽ����ʹ��(90)��֪���ˡ�");
this:AddCloseItem("�����ȥ������")
end

]]

menu_822_90 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ���������ɵĻ����Ѿ������ޣ�������ܼ�����");
this:AddCloseItem("�뿪")
end

]]

menu_822_91 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    �㻹û�м��壬�޷��Ӽ��廷ʽ����");
this:AddCloseItem("�뿪")
end

]]

menu_822_92 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    �����������Ѿ�����������������ʼ����Ļ�ʽ����");
this:AddCloseItem("�뿪")
end

]]


menu_822_94 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ���ڹ���������ɼ�����ҫ������������տ���������һ�μ��廷ʽ�����Ƿ�Ҫ���ڿ�ʼ�أ�");
this:AddTaskCmdItem("�ٴο�ʼ���ռ��廷ʽ����", "accept_822", 21102, 94)
end

]]

menu_822_100 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ��ȷ��Ҫ����ȡ����Ļ�ʽ����浵ô��");
this:AddTaskCmdItem("ȡ���浵", "accept_822", 21102, 100)
end

]]

menu_822_101 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����������ʱ���������Ҫ������������");
this:AddTaskCmdItem("������������", "accept_822", 21102, 101)
end

]]

function visit_npc(npc)
	--print("visit");
	player = me();
	local v = player.quest:vars(21102);
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
		show_dialog(npc, menu_822_80);	
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
	
	local v = player.quest:vars(21102);
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
	local v = player.quest:vars(21102);
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
			if kill < 1 then 
				v:set("kills", 1); 
				kill=1;
				--print("killtest");
				--print(kill);
				
				local xxx="��ʽ����Ŀ�꣺#num#/30";
				xxx = string.gsub(xxx, "#num#", kill);	
				v:sets("num", xxx);
				--print(xxx);
				v:refresh(player,"num")
					
				player.quest:add(21102, v, player, true);
				
			end		
		
			v:set("kills", kill+1);
			
			if kill >= 1 then

				local xxx="��ʽ����Ŀ�꣺#num#/30";
				xxx = string.gsub(xxx, "#num#", kill);	
				v:sets("num", xxx);
				--print(xxx);
				v:refresh(player,"num")
				
				player.quest:add(21102, v, player, true);

			end
						
			if kill >= 30 then

				local xxx="��ʽ����Ŀ�꣺#num#/30";
				xxx = string.gsub(xxx, "#num#", kill);	
				v:sets("num", xxx);
				--print(xxx);
				v:refresh(player,"num")
				
				player.quest:add(21102, v, player, true);

				sys(player, 5, "��ʽ�������");
				v:set("state", -3);
				local tempid = v:get("npc");
				refresh_npc(tempid);
			end
				
		return 1;
	end
	
	return 0;
end

function get_object(object)
	--print("get_ob");
	player = me();
	local v = player.quest:vars(21102);
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
				sys(player, 5, "��û�ʽ������Ʒ");
			else		
				v:set("item_nums", get+1);
				sys(player, 5, "��û�ʽ������Ʒ");
				if get >= 5 then
					sys(player, 5, "��ϲ���ռ����㹻����Ʒ");
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
		
			--print(s);
			--print(type);
			--print(item);
	
		--if s == 1 and id==npc:id() and item==npc:item() and type == 4 then
		--print("��ʼ�ж�");
		if s == 1 and type == 4 then

			local get = v:get("item_nums");
			
			if get < 1 then 
				v:set("item_nums", 1); 
				get=1;
				local xxx="��ʽ����Ŀ�꣺#num#/10";
				xxx = string.gsub(xxx, "#num#", get);	
				v:sets("num", xxx);
				--print(xxx);
				v:refresh(player,"num")
				player.quest:add(21102, v, player, true);
			end
			
			if get >= 1 then 
				v:set("item_nums", get+1); 
				local xxx="��ʽ����Ŀ�꣺#num#/10";
				xxx = string.gsub(xxx, "#num#", get);	
				v:sets("num", xxx);
				--print(xxx);
				v:refresh(player,"num")
				player.quest:add(21102, v, player, true);
			end
			
			if get >= 10 then 
					v:set("item_nums", get+1); 
					local xxx="��ʽ����Ŀ�꣺#num#/10";
					xxx = string.gsub(xxx, "#num#", get);	
					v:sets("num", xxx);
					--print(xxx);
					v:refresh(player,"num")
					player.quest:add(21102, v, player, true);
			
					sys(player, 5, "��ϲ���ռ����㹻����Ʒ");
					v:set("state", -3);
					local tempid = v:get("id");
					refresh_npc(tempid);
			end
				
			return 1;
		end
	
	return 0;
end

function gain_exp(player, qs, qss)
	local baseexp = 1.6*player.charbase.level*player.charbase.level + 5*player.charbase.level;
	local exp = 7*baseexp*(0.6+0.2*(qs + qss -1));
	player:add_exp(exp, false, 0, 0, false);
end
