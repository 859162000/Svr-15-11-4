visit_npc_list70 = { {"��˳ǵ�κ��", 430},{"�׹Ƕ����Ĺ�����", 448},{"�����ĵ���ȫ��", 454} };
visit_npc_list702 = { {"��˳ǵ�����", 435},{"�������ʯ����", 452},{"ǧ���������", 460},{"����������������", 463} };

get_weapon_list70 = { {"95��ҹħ��(��ɫ)", 10310, 0}, {"95��ҹħ��(��ɫ)", 10510, 0}, {"95��ҹħ��(��ɫ)", 10560, 0}, {"95��ҹħ��(��ɫ)", 10610, 0}, {"95��ҹħ��(��ɫ)", 10660, 0}}; 
get_weapon_list702 = { {"95��ҹħ��(��ɫ)", 10310, 1}, {"95��ҹħ��(��ɫ)", 10510, 1}, {"95��ҹħ��(��ɫ)", 10560, 1}, {"95��ҹħ��(��ɫ)", 10610, 1}, {"95��ҹħ��(��ɫ)", 10660, 1}}; 

kill_npc_list70 = { {"���ֶ�ʮ���100���໢��", 16469}, {"�����������100����ս٪��", 16118}, {"�ʳǵ�100���Ѿ�����", 16317}, {"����Ӫ�ص�100������С��", 16579} };
kill_npc_list702 = { {"��е���������100����������", 10207}, {"�ʳǵ�100���Ѿ�����", 16317}, {"��е�����Ĳ��105���ֲ�ľ��", 10216} };

get_item_list70 = { {"Ԫ��", 1910}, {"ʮ���", 1836}, {"����", 1975} };
get_item_list702 = { {"����", 1911}, {"��֥", 1883}, {"��ͭ", 1913}, {"����", 1975} };


--kill_get_list70 = { {"ɱ�������������ʦ���10��ҹ����", 16178, 2050}, {"ɱ�׹Ƕ�����Ŀ�צ��ʬ���10��ڲ�", 16024, 2065}, {"ɱ�������������ʦ���10��ȾѪ�ĵ�", 16178, 2062} };

--kill_get_list702 = { {"ɱ�Ͻ�60�������ǹ�����10����ҩ", 16215, 2088}, {"ɱ�Ĳ�60���İ׹Ǹ��ֻ��10����������", 16033, 2023}, {"ɱ�׹Ƕ��Ĳ��ʳԹ��ʬ��ý�ʬ����", 16034, 2125} };

kill_boss_list = { {"����ɽһկ�ϵ�ɽ���̿�ID:16440", 16440} };
kill_boss_list2 = { {"����ɽ��կ�ϵ�ɽ����ʿID:16441", 16441} };

menu_823_1 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_823", 21102, 1)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_823", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������900�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_823", 21102, 99)
end
function Dialog45()
this:AddTalk("���Ұ�����Ž���#place#��Ȼ������Ҹ��㽱����");
end
function MoveCanClose()
	return false
end

]]

menu_823_2 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_823", 21102, 2)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_823", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_823", 21102, 99)
end
function Dialog45()
this:AddTalk("����ɱ#npc#5ֻ��Ȼ����������ҡ�");
end
function MoveCanClose()
	return false
end

]]

--�ռ���Ʒ����
menu_823_3 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_823", 21102, 3)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_823", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_823", 21102, 99)
end
function Dialog45()
this:AddTalk("����Ҫ3��#name#��ȫ���ҵ����������ҡ�");
end
function MoveCanClose()
	return false
end

]]

menu_823_13 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_823", 21102, 13)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_823", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_823", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

--�ռ�װ��������
menu_823_15 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_823", 21102, 15)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_823", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_823", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

--ɱBOSS������Ի�
menu_823_17 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_823", 21102, 17)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_823", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_823", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

menu_823_23 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("ȥ#name#����ҡ�");
this:AddCloseItem("֪����")
end

]]

--�ռ�װ��֪ͨ
menu_823_45 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("ȥ�ռ�һ��#name#���ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_823_4 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����Ҫ5��#name#��ȫ���ҵ����������ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_823_5 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("���Ұ�����Ž���#place#���������һ���㽱���ġ�");
this:AddCloseItem("֪����")
end

]]

menu_823_6 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����ɱ#name#30ֻ��Ȼ����������ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_823_7 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����ɱ#name#��Ȼ����������ҡ�");
this:AddCloseItem("֪����")
end

]]


menu_823_11 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("��ϲ���������������������ȡ����ô��");
this:AddTaskCmdItem("���ܽ���", "accept_823", 21102, 11)
end

]]

menu_823_8 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("5��#name#�����Ѿ��ռ�����ô��");
this:AddTaskCmdItem("�ó���Ʒ", "accept_823", 21102, 8)
end

]]

menu_823_31 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("10����ô���Ū����������̫�ܸ��ˣ�����ҡ�");
this:AddTaskCmdItem("�ó���Ʒ", "accept_823", 21102, 31)
end

]]

--�ռ�װ����������Ի�
menu_823_35 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("#name#��ô���Ū����������̫�ܸ��ˣ�����ҡ�");
this:AddTaskCmdItem("�ó���Ʒ", "accept_823", 21102, 35)
end

]]


menu_823_80 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("�����������ˣ����ȥ���߻�ʽ����ʹ��(100)��֪���ˡ�");
this:AddCloseItem("�����ȥ������")
end

]]

menu_823_90 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ���������ɵĻ����Ѿ������ޣ�������ܼ�����");
this:AddCloseItem("�뿪")
end

]]

menu_823_91 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    �㻹û�м��壬�޷��Ӽ��廷ʽ����");
this:AddCloseItem("�뿪")
end

]]

menu_823_92 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    �����������Ѿ�����������������ʼ����Ļ�ʽ����");
this:AddCloseItem("�뿪")
end

]]


menu_823_94 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ���ڹ���������ɼ�����ҫ������������տ���������һ�μ��廷ʽ�����Ƿ�Ҫ���ڿ�ʼ�أ�");
this:AddTaskCmdItem("�ٴο�ʼ���ռ��廷ʽ����", "accept_823", 21102, 94)
end

]]

menu_823_100 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ��ȷ��Ҫ����ȡ����Ļ�ʽ����浵ô��");
this:AddTaskCmdItem("ȡ���浵", "accept_823", 21102, 100)
end

]]

menu_823_101 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����������ʱ���������Ҫ������������");
this:AddTaskCmdItem("������������", "accept_823", 21102, 101)
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
		show_dialog(npc, menu_823_80);	
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
