visit_npc_list110 = { {"ǧ������Ը�", 461},{"����̶������", 469},{"����������������", 464} };
visit_npc_list1102 = { {"ǧ�����������", 462},{"�����������Ķſ�", 467},{"����̶������", 470} };

get_weapon_list110 = { {"105����ħ��(��ɫ)", 10311, 0}, {"105����ħ��(��ɫ)", 10511, 0}, {"105����ħ��(��ɫ)", 10561, 0},{"105����ħ��(��ɫ)", 10611, 0}, {"105����ħ��(��ɫ)", 10661, 0}}; 
get_weapon_list1102 = { {"105����ħ��(��ɫ)", 10311, 1}, {"105����ħ��(��ɫ)", 10511, 1}, {"105����ħ��(��ɫ)", 10561, 1}, {"105����ħ��(��ɫ)", 10611, 1}, {"105����ħ��(��ɫ)", 10661, 1}}; 

kill_npc_list110 = { {"����Ӫ�ص�110�������ӱ�", 16583}, {"��е���������110����������", 10219},{"��е���������110����ͭħ��", 10221} };
kill_npc_list1102 = { {"��е�����߲��115������ʯ��", 10229},{"��е�����߲��115������ͭ��", 10230},{"����Ӫ�ص�115����������", 16589} }; 

get_item_list110 = { {"����", 1914}, {"ѩ����", 1884}, {"����", 1976} };
get_item_list1102 = { {"�ڵ�", 1917}, {"����Ĺ�ʵ", 1834}, {"����", 1976} };

kill_get_list = { {"ɱ�������ϻ����10����ͷ", 15014, 2098}, {"ɱɽկǰ�ڵ�ɽ���ڱ����10��ɽ��ȾѪ��", 10097, 2015}, {"ɱɽկǰ�ڵ�ɽ��սʿ���10��ɽ������", 10098, 2016} };

kill_get_list2 = { {"ɱ����ɽһկ��ɽ�����ֻ��10��ɽ������", 10100, 2017}, {"ɱ����ɽһկ��ɽ���������10��ɽ��ȾѪ��", 10113, 2015}, {"ɱ����ɽ��կ���ؼ�ɽ�����10��ɽ������", 10109, 2017} };

kill_boss_list = { {"����ɽһկ�ϵ�ɽ���̿�ID:16440", 16440} };

kill_boss_list2 = { {"����ɽ��կ�ϵ�ɽ����ʿID:16441", 16441} };

menu_825_1 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_825", 21102, 1)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_825", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������900�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_825", 21102, 99)
end
function Dialog45()
this:AddTalk("���Ұ�����Ž���#place#��Ȼ������Ҹ��㽱����");
end
function MoveCanClose()
	return false
end

]]

menu_825_2 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_825", 21102, 2)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_825", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_825", 21102, 99)
end
function Dialog45()
this:AddTalk("����ɱ#npc#5ֻ��Ȼ����������ҡ�");
end
function MoveCanClose()
	return false
end

]]

--�ռ���Ʒ����
menu_825_3 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_825", 21102, 3)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_825", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_825", 21102, 99)
end
function Dialog45()
this:AddTalk("����Ҫ3��#name#��ȫ���ҵ����������ҡ�");
end
function MoveCanClose()
	return false
end

]]

menu_825_13 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_825", 21102, 13)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_825", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_825", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

--�ռ�װ��������
menu_825_15 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_825", 21102, 15)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_825", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_825", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

--ɱBOSS������Ի�
menu_825_17 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����һ����ʽ����һ�����˾Ͳ���ɾ����������������ҽ������NPC��ȡ������ÿ�εĽ���������ɵ������������������ڵĻ���������\n    ���������;���߳���1Сʱ����ô���������Զ�������\n\n    ���Ǳ�����#qs#������\n    ���ǽ���ĵ�#round#����\n\n���齱����#exp#");
this:AddTaskCmdItem("��������", "accept_825", 21102, 17)
this:AddDialogItem("������浵(0)", "Dialog99")
this:AddTaskCmdItem("�������ջ�����", "accept_825", 21102, 102)
end
function Dialog99()
this:AddTalk("    ��ȷ��Ҫ��������ɵĻ����浵ô��\n    һ���浵���������Ƚ�����ȡ�����ܼ�����ʽ�����⽫������100�������㡣")
this:AddTaskCmdItem("ȷ���浵-900����ֵ{10001}", "accept_825", 21102, 99)
end
function MoveCanClose()
	return false
end

]]

menu_825_23 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("ȥ#name#����ҡ�");
this:AddCloseItem("֪����")
end

]]

--�ռ�װ��֪ͨ
menu_825_45 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("ȥ�ռ�һ��#name#���ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_825_4 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����Ҫ5��#name#��ȫ���ҵ����������ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_825_5 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("���Ұ�����Ž���#place#���������һ���㽱���ġ�");
this:AddCloseItem("֪����")
end

]]

menu_825_6 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����ɱ#name#30ֻ��Ȼ����������ҡ�");
this:AddCloseItem("֪����")
end

]]

menu_825_7 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("����ɱ#name#��Ȼ����������ҡ�");
this:AddCloseItem("֪����")
end

]]


menu_825_11 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("��ϲ���������������������ȡ����ô��");
this:AddTaskCmdItem("���ܽ���", "accept_825", 21102, 11)
end

]]

menu_825_8 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("5��#name#�����Ѿ��ռ�����ô��");
this:AddTaskCmdItem("�ó���Ʒ", "accept_825", 21102, 8)
end

]]

menu_825_31 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("10����ô���Ū����������̫�ܸ��ˣ�����ҡ�");
this:AddTaskCmdItem("�ó���Ʒ", "accept_825", 21102, 31)
end

]]

--�ռ�װ����������Ի�
menu_825_35 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("#name#��ô���Ū����������̫�ܸ��ˣ�����ҡ�");
this:AddTaskCmdItem("�ó���Ʒ", "accept_825", 21102, 35)
end

]]


menu_825_80 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("�����������ˣ����ȥ���߻�ʽ����ʹ��(60)��֪���ˡ�");
this:AddCloseItem("�����ȥ������")
end

]]

menu_825_90 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ���������ɵĻ����Ѿ������ޣ�������ܼ�����");
this:AddCloseItem("�뿪")
end

]]

menu_825_91 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    �㻹û�м��壬�޷��Ӽ��廷ʽ����");
this:AddCloseItem("�뿪")
end

]]

menu_825_92 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    �����������Ѿ�����������������ʼ����Ļ�ʽ����");
this:AddCloseItem("�뿪")
end

]]


menu_825_94 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ���ڹ���������ɼ�����ҫ������������տ���������һ�μ��廷ʽ�����Ƿ�Ҫ���ڿ�ʼ�أ�");
this:AddTaskCmdItem("�ٴο�ʼ���ռ��廷ʽ����", "accept_825", 21102, 94)
end

]]

menu_825_100 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ��ȷ��Ҫ����ȡ����Ļ�ʽ����浵ô��");
this:AddTaskCmdItem("ȡ���浵", "accept_825", 21102, 100)
end

]]

menu_825_101 = [[
function IsHasTask()
return true
end
function TaskDialog()
this:AddTalk("    ����������ʱ���������Ҫ������������");
this:AddTaskCmdItem("������������", "accept_825", 21102, 101)
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
		show_dialog(npc, menu_825_80);	
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
	
	
		local s = v:get("state");
		local id = v:get("id");
		local item = v:get("item");
		local type = v:get("type");
		
	return 0;
end

function gain_exp(player, qs, qss)
	local baseexp = 1.6*player.charbase.level*player.charbase.level + 5*player.charbase.level;
	local exp = 7*baseexp*(0.6+0.2*(qs + qss -1));
	player:add_exp(exp, false, 0, 0, false);
end
