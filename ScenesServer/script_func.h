/**
 * \file
 * \version	$Id: script_func.h  $
 * \author	
 * \date	
 * \brief	脚本辅助函数
 * 
 */

#ifndef __QUEST_SCRIPT_FUNC_H__
#define __QUEST_SCRIPT_FUNC_H__

#include <string>
#include <vector>

class SceneUser;
class SceneNpc;

SceneUser* me();
void set_me(SceneUser* user);

bool sys(SceneUser* target, int type, const char* msg);

void show_dialog(SceneNpc* npc, const char* menu);

class Vars;

void set_var(Vars* vars, const char* name, int value);
void set_varS(Vars* vars, const char* name, const char * value);

int get_var(Vars* vars, const char* name);
const char * get_varS(Vars* vars, const char* name);

void refresh_status(SceneNpc* npc);

void refresh_npc(int id);

int npc_tempid(SceneNpc* npc);

int npc_id(SceneNpc* npc);

void refresh_quest(int id);



/** brief 提供对脚本中全局变量的支持
	
*/
class GlobalVars
{
public:
	static Vars* add_g();
	static Vars* add_t();
	static Vars* add_f();
};

bool check_money(SceneUser* user, int money);
bool remove_money(SceneUser* user, int money);
void add_money(SceneUser* user, int money);

bool have_ob(SceneUser* user, int id, int num, int level,int type);
unsigned int get_ob(SceneUser* user, int id, int level );
bool del_ob(SceneUser* user, unsigned int id);
int  space(SceneUser* user);

Vars * get_familyvar(SceneUser* user, int dummy);
Vars * get_uservar(SceneUser* user, int dummy);
Vars * get_tongvar(SceneUser* user, int dummy);

void add_exp(SceneUser* user,unsigned int num, bool addPet, unsigned int dwTempID, unsigned char byType, bool addCartoon);
int get_time();
double diff_time(int, int);

#endif
