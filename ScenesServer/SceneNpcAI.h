/**
 * \file
 * \version  $Id: SceneNpcAI.h  $
 * \author  
 * \date 
 * \brief ����NPCAI�������ص����ݽṹ����
 * 
 * 
 */

#ifndef _SCENE_NPC_AI_H_
#define _SCENE_NPC_AI_H_

#include "zSceneEntry.h"
#include "SceneEntryPk.h"
//#include "SceneNpc.h"
#include "zTime.h"

/**
 * \brief AI���ͣ�npc�����Ļ�������
 * �ߡ�������Ѳ�ߡ���ת��
 *
 */
enum SceneNpcAIType
{
	NPC_AI_NORMAL,///��ͨAI
	NPC_AI_SAY,///˵��
	NPC_AI_MOVETO,///�ƶ���ĳλ��
	NPC_AI_ATTACK,///��ĳ��Χ�ڹ���
	NPC_AI_FLEE,///�������
	NPC_AI_RETURN_TO_REGION,///�ص����Χ
	NPC_AI_GO_ON_PATH,///����һ��·���ƶ�
	NPC_AI_CHANGE_MAP,///�л���ͼ��ͬһ�������ڣ�
	NPC_AI_WARP,///ͬһ��ͼ��˲���ƶ�
	NPC_AI_PATROL,///Ѳ��
	NPC_AI_CLEAR,///�����npc
	NPC_AI_WAIT,///�ȴ���ʲôҲ����
	NPC_AI_GUARD_DOOR,///����
	NPC_AI_GUARD_ARCHER,///����
	NPC_AI_GUARD_GUARD,///����
	NPC_AI_GUARD_PATROL,///Ѳ����ʿ
	NPC_AI_DROP_ITEM,///������
	NPC_AI_CALL_PET,///�г���
	NPC_AI_RANDOM_CHAT///���˵��
};

///npcAI��־����
enum NpcAIFlag
{
	AIF_ATK_PDEF            = 0x00000001,   ///���ȹ��������͵ĵ���
	AIF_ATK_MDEF            = 0x00000002,   ///���ȹ���ħ����͵ĵ���
	AIF_ATK_HP              = 0x00000004,   ///���ȹ�������ֵ��͵ĵ���
	AIF_GIVEUP_10_SEC       = 0x00000008,   ///׷��10�����Ŀ��
	AIF_GIVEUP_6_SEC        = 0x00000010,   ///6��δ�ܵ��˺�����Ŀ��
	AIF_GIVEUP_3_SEC        = 0x00000020,   ///3��δ�ܵ��˺�����Ŀ��
	AIF_FLEE_30_HP		= 0x00000040,   ///HP30%��������4��
	AIF_FLEE_3_ENEMY_4	= 0x00000080,   ///��3�����ϵ���Χ������4��
	AIF_NO_BATTLE		= 0x00000100,	///��ս��npc
	AIF_NO_MOVE		= 0x00000200,   ///���ƶ���������������·��ȣ�
	AIF_WARP_MOVE		= 0x00000400,   ///˲�Ʒ�ʽ�ƶ�
	AIF_CALL_FELLOW_7       = 0x00000800,   ///�ٻ�7*7��Χ��δ����npc(����50%)
	AIF_CALL_FELLOW_9       = 0x00001000,   ///�ٻ�9*9��Χ��δ����npc������50%��
	AIF_CALL_BY_ATYPE       = 0x00002000,   ///�ٻ�ͬ�ֹ������͵�ͬ�飨����������־������
	AIF_HEAL_FELLOW_5       = 0x00004000,   ///����5*5��Χ�ڵ�HP50%���µ�ͬ��
	AIF_BUFF_FELLOW_5	= 0x00008000,   ///��5*5��Χ�ڵ�ͬ��buff
	AIF_HELP_FELLOW_5	= 0x00010000,	///����5*5��Χ�ڵ�ͬ�鹥�������ڱ���npc��
	AIF_ATK_MASTER		= 0x00020000,	///ֱ�ӹ������������
	AIF_ATK_REDNAME		= 0x00040000,	///�������������
	AIF_DOUBLE_REGION	= 0x00080000,	///������Χ�ӱ�
	AIF_SPD_UP_HP20		= 0x00100000,	///hp20%�����ƶ��ٶȼӱ�
	AIF_ASPD_UP_HP50	= 0x00200000,	///hp50%���¹����ٶȼӱ�
	AIF_ACTIVE_MODE		= 0x00400000,	///��������
	AIF_RUN_AWAY		= 0x00800000,	///�������
	AIF_LOCK_TARGET		= 0x01000000,	///���л�����Ŀ��ֱ����
	AIF_RCV_UNDER_30	= 0x02000000,	///hp30%���³�����Ѫ1%
	AIF_RCV_REST		= 0x04000000,	///����ս��30����Ѫһ��5%
	AIF_SUMMON_UNDER_60	= 0x08000000,	///hp60%�����ٻ�����
	AIF_LIMIT_REGION	= 0x10000000	///���ƻ��Χ
};

///npc˵��������
enum NpcChatType
{
	NPC_CHAT_ON_FIND_ENEMY = 1,	///���ֵ���
	NPC_CHAT_ON_ATTACK,	///����ʱ˵�Ļ�
	NPC_CHAT_ON_RETURN,	///׷�𷵻�ʱ˵�Ļ�
	NPC_CHAT_ON_DIE,	///����ʱ��˵�Ļ�
	NPC_CHAT_ON_FLEE,	///����ʱ˵�Ļ�
	NPC_CHAT_ON_HIT,	///����ʱ˵�Ļ�
	NPC_CHAT_ON_HELP,	///����ͬ��ʱ˵�Ļ�
	NPC_CHAT_ON_BE_HELP,	///ͬ��������ʱ˵�Ļ�
	NPC_CHAT_RANDOM		///���˵��
};

/**
 * \brief һ��AI�Ķ���
 *
 */
struct t_NpcAIDefine
{
	///���ͣ�NPC�ڸý׶ε���Ҫ����
	SceneNpcAIType type;
	///λ�� ���ݲ�ͬ����λ�õ�����Ҳ�Բ���ͬ
	///�ƶ�ʱ��ʾĿ�ĵأ�������ʾ���Χ����
	zPos pos;
	///��Χ 
	///�ƶ�ʱ��ʾ����Ŀ�ĵص��ж���Χ��������ʾ���Χ
	int regionX, regionY;
	//zRTime endTime;
	///��AI�ĳ���ʱ��
	int lasttime;

	///�Ƿ���������
	bool flee;
	///���ܵķ���
	int fleeDir;
	///���ܼ���
	int fleeCount;

	///�л���ͼʱ��Ҫȥ�ĵ�ͼ
	///˵��ʱ��Ҫ˵�Ļ�
	char str[MAX_CHATINFO];


	/**
	 * \brief Ĭ�Ϲ��캯��
	 *
	 */
	t_NpcAIDefine()
		:type(NPC_AI_NORMAL), pos(zPos(0,0)), regionX(2), regionY(2), lasttime(0), flee(false), fleeDir(-1), fleeCount(0)
		{
			bzero(str, sizeof(str));
		}
	
	/**
	 * \brief ���캯��
	 *
	 *
	 * \param type AI����
	 * \param pos λ��
	 * \param regionX ��Χ��
	 * \param regionY ��Χ��
	 * \param lasttime ����ʱ��
	 * \return 
	 */
	t_NpcAIDefine(SceneNpcAIType type, const zPos &pos, int regionX, int regionY, int lasttime)
		:type(type), pos(pos), regionX(regionX), regionY(regionY), lasttime(lasttime)
		{
			bzero(str, sizeof(str));
		}


	/**
	 * \brief �������캯��
	 *
	 * \param ad Ҫ���ƵĶ���
	 */
	t_NpcAIDefine(const t_NpcAIDefine &ad)
	{
		type = ad.type;
		pos = ad.pos;
		regionX = ad.regionX;
		regionY = ad.regionY;
		lasttime = ad.lasttime;
		flee = ad.flee;
		fleeDir = ad.fleeDir;
		fleeCount = ad.fleeCount;
		strncpy(str, ad.str, sizeof(str)-1);
	}

	/**
	 * \brief ��ֵ
	 *
	 * \param ad Ҫ�����Ķ���
	 * \return ���������ַ
	 */
	t_NpcAIDefine & operator = (const t_NpcAIDefine &ad)
	{
		type = ad.type;
		pos = ad.pos;
		regionX = ad.regionX;
		regionY = ad.regionY;
		lasttime = ad.lasttime;
		flee = ad.flee;
		fleeDir = ad.fleeDir;
		fleeCount = ad.fleeCount;
		strncpy(str, ad.str, sizeof(str)-1);

		return *this;
	}
};

class SceneNpc;

/**
 * \brief AI������
 * ���������Զ�ȡNPC�ű���ʹNPC���սű�������
 * 
 * ��������������¼���ʱ�䡢��������������
 * ���ݲ�ͬ������ΪNPC���ò�ͬ��AI
 *
 */
class NpcAIController
{
	static const int npc_call_fellow_rate;///NPC�ٻ�ͬ��ļ���
	static const int npc_one_checkpoint_time;///NPC����·���ƶ�ʱ����һ��·����ʱ��
	static const int npc_checkpoint_region;///NPC�ƶ�������һ��·����ж���Χ
	static const int npc_onhit_stop_time;///����NPC�ƶ��б�����ʱ��ֹͣ��ʱ��
	static const int npc_flee_distance;///NPC���빥���ߵľ���
	static const int npc_min_act_region;///NPC��С���Χ
	
	///AI������AI�����������е�˳������ִ��
	std::vector<t_NpcAIDefine> phaseVector;
	///��ǰ��AI����
	unsigned int curPhase;
	///��ǰAI�Ľ���ʱ��
	zRTime phaseEndTime;

	///�ű��ظ�����
	///-1������ѭ��  0��ֹͣ  >0��ѭ������
	int repeat;

	///�Ƿ������˽ű�
	bool active;
	void nextPhase(int index);
	void on_phaseEnd();
	SceneNpcAIType parseAction(char *);
	
	///�����������Ƶ�npc
	SceneNpc * npc;

	///��ǰ��AI�ͱ����ǰһ��AI
	t_NpcAIDefine curAI,oldAI;
	///���Χ������
	zPos actPos;
	///���Χ�Ŀ�͸�
	int actRegionX,actRegionY;
	bool outOfRegion() const;
	void returnToRegion();

	bool arrived(zPos pos = zPos(0,0), int x = -1, int y = -1);
	bool dstReached();
	///�Ƿ񵽴�Ŀ�ĵ�
	bool reached;

	///Ŀ�ĵ�ͼ
	char dstMap[32];
	///Ŀ��λ��
	zPos dstPos;

public:
	void setAI(const t_NpcAIDefine ai, const bool setTime = true);
	void switchAI(const bool setTime = true);
	void setNormalAI();
	void setActRegion(zPos pos = zPos(0,0), int x = -1, int y = -1);
	void getActRegion(zPos &, int &, int  &);
	NpcAIController(SceneNpc *);
	bool loadScript(unsigned int id);
	void unloadScript();
	void processPhase();
	void setRepeat(int repeat);
	int getRepeat();
	bool isActive();
	void setPhaseTime(const int delay);
	void delayPhaseTime(const int delay);
	bool phaseTimeOver();

	void on_relive();
	void on_find_enemy(const SceneEntryPk *);
	void on_hit(SceneEntryPk *pAtk);
	void on_die();
};
#endif
