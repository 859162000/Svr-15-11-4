/**
 * \file 
 * \version  $Id: Rush.h  $
 * \author  
 * \date 
 * \brief ������﹥�ǵ��࣬�����ű������ݽű����ƹ���boss
 *
 * 
 */

#ifndef _RUSH_H_
#define _RUSH_H_
#include "SceneNpc.h"
#include "SceneDefine.h"
#include <vector>

/**
 * \brief ������﹥�ǵ��࣬�����ű������ݽű����ƹ���boss
 *
 */
class Rush
{
	private:
		//DWORD summonCount;

		/**
		 * \brief boss �Ķ��壬�����ٻ�boss
		 * ���Զ�����boss���ٻ�ʱ���ѡһ��
		 *
		 */
		struct bossDefine
		{
			///boss��ţ���Ӧnpc��
			DWORD id;
			///����λ��
			zPos pos;
			///������Χ
			int region;
			///����
			int num;
			///�ű�ID
			DWORD script;
		};
		/**
		 * \brief ���˶���
		 *  ������boss�ٻ�������С�֣�Э��bos����
		 *  ���˲���Rush����ƣ���Ϊ��ʽ����ͨ������ͬ
		 *  Rush��ֻ�������ǵ�ָ���Ա��ڹ��ǽ�����ɾ��
		 *
		 */
		struct servantDefine
		{
			///����id����Ӧnpc��
			DWORD id;
			///һ���ٻ�������
			int num;
			///�ٻ��ļ���
			int rate;
			///�ٻ����
			int interval;
		};
		///��Ŷ��boss������ֻ�ٻ�һ��
		std::vector<bossDefine> bossVector;
		///��Ŷ�����˶���
		std::vector<servantDefine> servantVector;
		///�ٻ��������˵�ָ�룬����ɾ��
		std::list<SceneNpc *> servants;

		/**
		 * \brief �������壬ÿ���׶���һ������
		 *
		 */
		enum rushAction
		{
			///��ͨ
			RUSH_NORMAL,
			///����֮ǰ�ĸ���׶�
			RUSH_RELIVE,
			///�ƶ�
			RUSH_MOVETO,
			///����
			RUSH_ATTACK,
			///�ٻ�����
			RUSH_SUMMON,
			///�ٻ�����boss������������
			RUSH_SUMMON_RUSH,
			///���ٻ�
			RUSH_SUMMON_ON,
			///�ر��ٻ����رպ󲻻����ٻ�����
			RUSH_SUMMON_OFF,
			///������ǹ������boss������
			RUSH_CLEAR,
			///��boss����hp/sp/mp
			RUSH_RECOVER,
			///�ٻ�����boss�ĳ���
			RUSH_SUMMON_PET,
			///����㲥
			RUSH_BROADCAST,
			///���ǽ���
			RUSH_END
		};
		/**
		 * \brief �׶ζ��壬���ǵĽ׶�
		 * Rush���Ŀ�ľ��ǿ���boss���չ��ǽ׶�һ��������
		 *
		 */
		struct phaseDefine
		{
			///�׶ζ���
			rushAction action;
			///λ�ã���ÿ���׶������Բ���ͬ
			///�ƶ�ʱ��ʾĿ�ģ�����ʱ��ʾ���Χ����λ��
			zPos pos;
			///��Χ �ƶ�ʱ��ʾ����Ŀ����ж����룬����ʱ��ʾ���Χ��С
			int region;
			///����ʱ�� һ���׶εĳ���ʱ�䣬��ʱ�ᴥ��onPhaseEnd�¼�
			int lasttime;
			///boss��˵����ÿ�׶ο�ͷ˵��
			char say[MAX_CHATINFO];
			///3������
			DWORD x,y,z;
		};
		///��Ž׶��б�
		std::vector<phaseDefine> phaseVector;
		///��ǰ�Ľ׶�����
		unsigned int curPhase;
		///�׶ν���ʱ��
		zRTime nextPhaseTime;
		///���ǽ�����ʱ��
		zRTime endTime;
		
		///���ǽű��ı��
		DWORD id;
		///Ŀ����ҵı��
		DWORD countryID;

		///����boss��id
		DWORD bossID;
		///bossָ��
		SceneNpc * boss;

		///�´ο��ٻ���ʱ��
		zRTime summonTime;
		///�ϴ�ѭ��ʱboss��hp
		DWORD lastBossHp;
		///�ٻ����˵Ŀ���
		bool canSummon;
		///����ʱ���������boss����������������й���npc
		int clearDelay;
		///�Ƿ����
		bool end;
		///����ʱ����ʾ��
		char text[128];
		std::string startText;
		std::string endText;

		///�ٻ����˵ķ�Χ��λ������boss�ĵ�ǰλ��Ϊ����
		static const int summon_servant_region = 4;
		bool checkSummonTime();
		
		bool loadRushData(DWORD rushID, DWORD rushDelay, DWORD countryID);
		void onPhaseTimeOver();
		void onTimeOver();
		void setEndTime(int delay);
		bool checkEndTime();
		void setPhaseTime(const int delay);
		bool checkPhaseTime();

		void enterNextPhase();
		bool summonBoss();
		bool summonServant();
		bool summonPet();
		void deleteBoss();
		void clearServants();

		rushAction parseAction(const char *);
	public:
		///�����ܳ���ʱ��
		int lasttime;
		///�೤ʱ���ʼ���ǣ�Ҳ����boss�ĸ���ʱ��
		DWORD rushDelay;
		///���ι��ǵ�����
		char rushName[MAX_NAMESIZE];
		///Ŀ���ͼ����
		char mapName[MAX_NAMESIZE];
		///boss������
		char bossName[MAX_NAMESIZE];

		Rush(DWORD rushID, DWORD rushDelay, DWORD countryID);
		bool init(Scene * =0);
		~Rush();
		void process();
		bool isEnd();

		void terminate();
};
#endif
