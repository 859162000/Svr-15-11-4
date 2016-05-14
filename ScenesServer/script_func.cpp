/**
 * \file
 * \version	$Id: script_func.cpp $
 * \author	
 * \date	
 * \brief	�ű���������
 * 
 */

#include "script_func.h"
#include "SceneUser.h"

SceneUser* current_user = NULL;


/**     
 * \brief ȡ�õ�ǰ�û�
 *
 * \return: ��ǰ�û�
 */  
SceneUser* me()
{
	return current_user;
}

/**     
 * \brief �趨��ǰ�û�
 *
 * \param user: Ŀ���û�
 */  
void set_me(SceneUser* user)
{
	current_user = user;
}

#include "Chat.h"


/**     
 * \brief �ű��ӿڣ�������Ϣ���û�����
 *
 * \param target : Ŀ���û�
 * \param type : ѶϢ���ͣ�see enumSysInfoType from Command.h for details
 * \param msg: ��Ϣ����
 * \return: ���ͳɹ�����true�����򷵻�false
 */  
bool sys(SceneUser* target, int type, const char* msg)
{
	return Channel::sendSys(target, type, msg);
}

#include "Command.h"
#include "SceneNpc.h"
#include "NpcTrade.h"

/**     
 * \brief �ű��ӿڣ������Ի���
 *
 * \param npc : �Ի���npc
 * \param menu : �Ի�������
 */  
void show_dialog(SceneNpc* npc, const char* menu)
{
	BYTE buf[4096];
	Cmd::stVisitNpcTradeUserCmd *cmd=(Cmd::stVisitNpcTradeUserCmd *)buf;
	memset(buf, 0, sizeof(buf));
	constructInPlace(cmd);
	
	strncpy(cmd->menuTxt, menu, strlen(menu));	
	if (npc && NpcTrade::getInstance().getNpcMenu(npc->id, cmd->menuTxt+strlen(menu))) {
		//intended to be blank
	}
	cmd->byReturn = 1;
	
	if (current_user) {
		current_user->sendCmdToMe(cmd, sizeof(Cmd::stVisitNpcTradeUserCmd) + strlen(cmd->menuTxt));
	}
}

#include "Quest.h"
#include "QuestAction.h"

/**     
 * \brief �ű��ӿڣ��趨һ������ֵ
 *
 * \param vars : Ŀ�����
 * \param name : ��������
 * \param value: ����ֵ
 */ 
void set_var(Vars* vars, const char* name, int value)
{
	Op::Set<int> op;
	vars->set_value(op, name, value, 0, current_user);
	
}

/**     
 * \brief �ű��ӿڣ��趨һ������ֵ
 *
 * \param vars : Ŀ�����
 * \param name : ��������
 * \param value: ����ֵ
 */ 
void set_varS(Vars* vars, const char* name, const char * value)
{
	Op::Set< std::string> op;
	std::string s=value;
	vars->set_value(op, name, s, 0, current_user);
}

/**     
 * \brief �ű��ӿڣ�ȡ��һ������ֵ
 *
 * \param vars : Ŀ�����
 * \param name : ��������
 * \return: ȡ�õı���ֵ�������������ڷ���0
 */ 
int get_var(Vars* vars, const char* name)
{
	int value;
	if (vars->get_value(name, value)) return value;
	
	return 0;
}

/**     
 * \brief �ű��ӿڣ�ȡ��һ������ֵ
 *
 * \param vars : Ŀ�����
 * \param name : ��������
 * \return: ȡ�õı���ֵ�������������ڷ���0
 */ 
const char *  get_varS(Vars* vars, const char* name)
{
	std::string value;
	if (vars->get_value(name, value)) return value.c_str();
	
	return 0;
}

/**     
 * \brief �ű��ӿڣ�ˢ��npc״̬
 *
 * \param npc : Ŀ��npc
 */ 
void refresh_status(SceneNpc* npc)
{
	npc->set_quest_status(current_user);

	Cmd::stAddMapNpcMapScreenUserCmd cmd;
	npc->full_t_MapNpcData(cmd.data);
	current_user->sendCmdToMe(&cmd, sizeof(Cmd::stAddMapNpcMapScreenUserCmd));
}

/**     
 * \brief �ű��ӿڣ�ˢ��npc״̬
 *
 * \param npc : Ŀ��npc��tempid
 */ 
void refresh_npc(int id)
{
	SceneNpc *sceneNpc = SceneNpcManager::getMe().getNpcByTempID((DWORD)id);	
	if (sceneNpc) refresh_status(sceneNpc);

}

/**     
 * \brief �ű��ӿڣ�ȡ��ĳnpc��tempid
 *
 * \param npc : Ŀ��npc
 *\return : ��npc��tempid
 */ 
int npc_tempid(SceneNpc* npc)
{
	return npc->tempid;
}

/**     
 * \brief �ű��ӿڣ�ȡ��ĳnpc��id
 *
 * \param npc : Ŀ��npc
 *\return : ��npc��id
 */ 
int npc_id(SceneNpc* npc)
{
	return npc->id;
}

/**     
 * \brief �ű��ӿڣ�ˢ��������Ϣ
 *
 * \param npc : Ŀ������id
 */ 
void refresh_quest(int id)
{
	Cmd::stAbandonQuestUserCmd ret;
	ret.id = id;
	current_user->sendCmdToMe(&ret, sizeof(ret));
}

#define U_ID 98765

/**     
 * \brief �ű��ӿڣ�ȡ��ȫ�ֱ����������������ڣ����½�
 *
 *\return : �õ���ȫ�ֱ���
 */ 
Vars* GlobalVars::add_g()
{
	return GlobalVar::instance().add(U_ID);
}

/**     
 * \brief �ű��ӿڣ�ȡ�ð������������������ڣ����½�
 *
 *\return : �õ��İ�����
 */ 
Vars* GlobalVars::add_t()
{
	return TongVar::instance().add(current_user->charbase.unionid);
}

/**     
 * \brief �ű��ӿڣ�ȡ�ü�������������������ڣ����½�
 *
 *\return : �õ��ļ������
 */ 
Vars* GlobalVars::add_f()
{
	return FamilyVar::instance().add(current_user->charbase.septid);
}

/**     
 * \brief �ű��ӿڣ��ж��û����ϴ��еĽ�Ǯ�Ƿ�����Ҫ��
 *
 * \param user: Ŀ���û�
 * \param money: ��Ҫ�Ľ�Ǯ
 * \return : ������������ture�����򷵻�false
 */ 
bool check_money(SceneUser* user, int money)
{
	return user->packs.checkMoney(money);
}

/**     
 * \brief �ű��ӿڣ������û����ϴ��еĽ�Ǯ����
 *
 * \param user: Ŀ���û�
 * \param money: ��Ҫ�Ľ�Ǯ
 * \return : ���ٳɹ�����ture�����򷵻�false
 */ 
bool remove_money(SceneUser* user, int money)
{
	return user->packs.removeMoney(money, "����");
}

/**     
 * \brief �ű��ӿڣ������û����ϴ��еĽ�Ǯ����
 *
 * \param user: Ŀ���û�
 * \param money: ���ӵĽ�Ǯ
 */ 
void add_money(SceneUser* user, int money)
{
	user->packs.addMoney(money, "����");
}

/**     
 * \brief �ű��ӿڣ��ж��û������Ƿ����ĳ��Ʒ
 *
 * \param user: Ŀ���û�
 * \param id: ��Ʒ��objectid
 * \param number: Ҫ�������
 * \param level: ��Ʒ�ȼ�������
 * \return ��������Ʒ�������ڵ��ڸ�������������true�����򷵻�false
 */ 
bool have_ob(SceneUser* user, int id, int num, int level ,int type)
{
	return user->packs.uom.exist(id, num, level ,type);
}

/**     
 * \brief �ű��ӿڣ��ж��û������Ƿ����ĳ��Ʒ
 *
 * \param user: Ŀ���û�
 * \param id: ��Ʒ��objectid
 * \param level: ��Ʒ����
 * \return ����������Ʒ������Ʒthisid�����򷵻�0
 */ 
unsigned int get_ob(SceneUser* user, int id, int level )
{
	return user->packs.uom.exist(id, 1, level ,1);
}

/**     
 * \brief �ű��ӿڣ�ɾ����Ʒ
 *
 * \param user: Ŀ���û�
 * \param id: ��Ʒ��thisid
 * \return ɾ���Ƿ�ɹ�
 */ 
bool del_ob(SceneUser* user, unsigned int id)
{
	zObject *delObj=user->packs.uom.getObjectByThisID(id);
	return user->packs.removeObject(delObj , true,true);
}

/**     
 * \brief �ű��ӿڣ�ȡ���û������е�ʣ��ռ�
 *
 * \param user: Ŀ���û�
 * \return �û�������ӵ�е�ʣ��ռ�
 */ 
int  space(SceneUser* user)
{
	return user->packs.uom.space(user);
}

/**
 * \brief �ű��ӿڣ�ȡ��ָ���û���������������������ڣ����½�
 *
 *\return : �õ����û��������
 */
Vars * get_familyvar(SceneUser* user, int dummy)
{
	return FamilyVar::instance().add(user->charbase.septid);
}

/**
 * \brief �ű��ӿڣ�ȡ��ָ���û������������������ڣ����½�
 *
 *\return : �õ����û�����
 */
Vars * get_uservar(SceneUser* user, int dummy)
{
	return UsersVar::instance().add(((QWORD)user->charbase.accid << 32) |user->charbase.id);
}

/**
 * \brief �ű��ӿڣ�ȡ��ָ���û��л�����������������ڣ����½�
 *
 *\return : �õ����û��л����
 */
Vars * get_tongvar(SceneUser* user, int dummy)
{
	return TongVar::instance().add(user->charbase.unionid);
}

void add_exp(SceneUser* user,DWORD num, bool addPet, DWORD dwTempID, BYTE byType, bool addCartoon)
{
	if(user)
	{
		user->addExp(num,addPet,dwTempID,byType,addCartoon);
		Zebra::logger->debug("[�õ���ʽ������][%s(%u)][%ld]", user->charbase.name, user->id, num);
	}
}

/**     
 * \brief �ű��ӿڣ�ȡ��ϵͳ��ǰʱ��
 *
 * \return ϵͳ��ǰʱ��
 */ 
int get_time()
{
	tzset();
	return time(NULL)-timezone;
}

/**     
 * \brief �ű��ӿڣ�ȡ������ʱ��Ĳ�
 *
 * \param t1 : ���Ƚ�ʱ��1
 * \param t2 : ���Ƚ�ʱ��2
 * \return ����ʱ��Ĳ�
 */ 
double diff_time(int t1, int t2)
{
	return difftime(t1, t2);
}


