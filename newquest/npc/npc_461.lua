local sq = the_script();
sq:add(ScriptQuest.NPC_VISIT, 461);

require "define825"

function visit_461(npc)
	return visit_npc(npc);
end

function state_461(npc)
	return state_npc(npc);
end