#include <stdarg.h>
#include <fstream>
#include <sstream>
#include "SceneManager.h"
#include "zRegex.h"
#include "Chat.h"
#include "zProperties.h"
#include "Team.h"
#include "SessionClient.h"
#include "QuestEvent.h"
#include "QuestTable.h"
#include "zSceneEntry.h"
#include "RebuildObject.h"
#include "ScenesServer.h"
#include "SceneTaskManager.h"
#include "SceneCommand.h"
#include <string.h>
#include <stdio.h>
#include "GmToolCommand.h"
#include "CartoonPet.h"
#include "BillCommand.h"
#include "Dice.h"

extern unsigned int cancel_country_need_money;          
extern unsigned int is_cancel_country; 


/**
 * \brief GMָ������
 *
 */
Gm GmCmds[]=
{
	//��ͨGM��
	{"help",Gm::help,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"��ʾָ�����"},
	{"levelup",Gm::levelup,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"����"},
	{"goto",Gm::goTo_Gm,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"��תλ��"},
	{"gomap",Gm::gomap_Gm,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"��ת��ͼ"},
	{"finduser",Gm::finduser,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"Ѱ�����"},
	{"gotouser",Gm::gotouser,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�������"},
	{"catchuser",Gm::catchuser,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�ٻ����"},
	{"bczone",Gm::bczone,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"��������㲥"},
	{"bcworld",Gm::bcworld,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"��������㲥"},
	{"bcwgamemsg",Gm::bcwgamemsg,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"������Ϸ������㲥"},
	{"kick",Gm::kick,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�ߵ����"},
	{"donttalk",Gm::donttalk,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"����"},
	{"talk",Gm::talk,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�������"},
	{"gohome",Gm::goHome,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�ص�����"},
	{"hideme",Gm::hideMe,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�����Լ�"},
	{"showme",Gm::showMe,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�ָ��ɼ�״̬"},
	{"god",Gm::god,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�޵�ģʽ"},
	{"normal",Gm::normal,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�ָ���ͨģʽ"},
	{"clearseptunion",Gm::clearSeptUnion,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"����ϰ���ͼ�������"},
	{"clearpoint",Gm::clearPoint,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"����������Ե�"},
	{"clearskillpoint",Gm::clearSkillPoint,Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"������ܵ�"},
	{"studyskill",Gm::studySkill, Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"ѧϰ�¼���"},
	{"settire",Gm::setTire, Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"����ƣ��ʱ��"},
	{"checkcountryinfo",Gm::checkCountryInfo, Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"��������Ϣ"},
	{"loadprocess",Gm::loadProcess, Gm::gm_mode |Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"���¼�������������ļ�"},
	{"cleararea",Gm::clearArea, Gm::gm_mode | Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"��ɢ��Χ���"},
	{"embar",Gm::embar, Gm::gm_mode | Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"����ָ�������"},
	{"refreshgeneral",Gm::refreshGeneral, Gm::gm_mode | Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�ֶ�ˢ�¸����󽫾�"},

	//�鳤��
	{"closenpc",Gm::closenpc, Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�رչ���NPC"},
	{"countrypower",Gm::countrypower, Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"�������ǿ��"},
	{"summon",Gm::summon,Gm::captain_mode | Gm::super_mode | Gm::debug_mode,"GM�ٻ�Npc"},
	{"usleep", Gm::usleep, Gm::super_mode | Gm::debug_mode, "�߳�usleep�ĺ�����"},
	{"createrush", Gm::createRush, Gm::debug_mode, "����һ�ι��﹥��"},

	//����GM��
	{"kickgateuser",Gm::kickGateUser, Gm::super_mode | Gm::debug_mode,"�ߵ������ϵ����"},
	{"loadgift",Gm::loadGift, Gm::super_mode | Gm::debug_mode,"���¼�����ٵ�����"},
	{"enableregister",Gm::enableRegister, Gm::super_mode | Gm::debug_mode,"����ע��"},
	{"enablelogin",Gm::enableLogin, Gm::super_mode | Gm::debug_mode,"�����½"},
	{"setservice",Gm::setService, Gm::super_mode | Gm::debug_mode,"������Ϸ����"},
	{"decitydare", Gm::debugCityDare, Gm::super_mode | Gm::debug_mode, "������ֹͣ�����ս"},
	{"deempdare", Gm::debugEmperorDare, Gm::super_mode | Gm::debug_mode, "������ֹͣ�ʳ�����ս"},
	{"createquiz", Gm::createQuiz, Gm::super_mode | Gm::debug_mode, "����һ��ȫ����������"},
	{"setpriv",Gm::setPriv,Gm::super_mode | Gm::debug_mode,"���ý�ɫȨ��"},
	{"newzone",Gm::newzone,Gm::super_mode | Gm::debug_mode,"�������в���"},
	{"stockconsign", Gm::stockconsign, Gm::super_mode | Gm::debug_mode, "��Ʊί�й���"},
	{"killer",Gm::killer, Gm::super_mode | Gm::debug_mode,"��ɱģʽ"},
	{"tong", Gm::tong, Gm::super_mode |Gm::debug_mode, "������ս-ʹ�÷���://tong t1=XXX t2=XXX"},
	{"clearworld", Gm::clearWorld, Gm::super_mode |Gm::debug_mode, "�����Χ�ڵĹ���"},
	{"showspecialnpc",Gm::showSpecialNpc, Gm::debug_mode,"�鿴��ǰ������NPC"},
	{"qaccount", Gm::qAccount, Gm::super_mode | Gm::debug_mode, "��ѯ�ʺ�"},
	{"userdistribute", Gm::countServerUser, Gm::super_mode | Gm::debug_mode, "�õ�����������������ֲ�"},
	{"shutdown", Gm::shutdown, Gm::super_mode | Gm::debug_mode, "ͣ��ά��"},
	{"systime", Gm::systime, Gm::super_mode | Gm::debug_mode, "�õ�ϵͳ��ǰʱ��"},
	{"goldsystem", Gm::goldsystem, Gm::super_mode | Gm::debug_mode, "���ϵͳ"},
	{"stocksystem", Gm::stocksystem, Gm::super_mode | Gm::debug_mode, "��Ʊϵͳ"},


	//������
	{"viewcountrydare",Gm::viewCountryDare,Gm::debug_mode,"�鿴��ս��¼"},
	{"givegold", Gm::givegold, Gm::debug_mode, "��Ҳ���"},
	{"givemoney", Gm::givemoney, Gm::debug_mode, "���Ҳ���"},
	{"givestock", Gm::givestock, Gm::debug_mode, "��Ʊ����"},
	{"version",Gm::version, Gm::debug_mode,"��̬���÷������汾��"},
	{"createunion", Gm::createUnion, Gm::debug_mode, "����һ�����"},
	{"createfamily", Gm::createFamily, Gm::debug_mode, "����һ������"},
	{"debugvote", Gm::debugVote, Gm::debug_mode, "������ֹͣͶƱ"},
	{"debuggem", Gm::debugGem, Gm::debug_mode, "������ֹͣ��������"},
	{"addsepexp", Gm::addSeptExp, Gm::debug_mode, "��ȡ���徭��"},
	{"setrepute", Gm::setRepute, Gm::debug_mode, "���ü�������"},
	{"setseptlevel", Gm::setSeptLevel, Gm::debug_mode, "���ü���ȼ�"},
	{"setallyfd", Gm::setAllyFriendDegree, Gm::debug_mode, "���������Ѻö�"},
	{"changecountry", Gm::changeCountry, Gm::debug_mode, "������ֹͣ�ѹ�����"},
	{"loadmap",Gm::loadMap, Gm::debug_mode,"���ص�ͼ"},
	{"unloadmap",Gm::unloadMap, Gm::debug_mode,"ж�ص�ͼ"},
	{"callpet",Gm::callPet, Gm::debug_mode,"�ٻ�����"},
	{"lockvalue",Gm::lockValue, Gm::debug_mode,"��������"},
	{"svote",Gm::svote,Gm::debug_mode,"����ͶƱ-ʹ�÷���://svote name=XXX"},
	{"uvote",Gm::uvote,Gm::debug_mode,"���ͶƱ-ʹ�÷���://uvote name=XXX"},       
	{"createschool", Gm::createSchool, Gm::debug_mode, "����һ������"},
	{"fetch",Gm::fetch, Gm::debug_mode,"GM����Ʒ"},
	{"showentries", Gm::showEntries, Gm::debug_mode, "�鿴��Χ��npc�����"},
	{"getsize", Gm::getsize, Gm::debug_mode, "�鿴��ɫ��ռ�ռ�"},
	{"setpetai", Gm::setPetAI, Gm::debug_mode, "���ó�����Ϊģʽ"},
	{"setpetaif", Gm::setPetAIF, Gm::debug_mode, "���ó���AIF"},
	{"bank", Gm::bank, Gm::debug_mode, "���òֿ����"},
	{"backoff",Gm::backoff, Gm::debug_mode,"GM����Ʒ"},
	{"upskill",Gm::upgradeSkill, Gm::debug_mode,"ѧϰħ��"},
	{"abandon",Gm::abandon, Gm::debug_mode,"�������"},
	{"showmaps",Gm::showMaps, Gm::debug_mode,"�鿴�Ѿ����صĵ�ͼ"},
	{"showpets",Gm::showPets, Gm::debug_mode,"�鿴��ǰ�ĳ���"},
	{"killpets",Gm::killPets, Gm::debug_mode,"�鿴��ǰ�ĳ���"},
	{"leechdom",Gm::leechdom, Gm::debug_mode,"ʹ��ҩƷ"},
	{"value",Gm::value, Gm::debug_mode,"����hp,mp,sp"},
	{"goodness",Gm::goodness, Gm::debug_mode,"ȡ�õ�ǰ�ƶ��"},
	{"getvalue",Gm::getvalue, Gm::debug_mode,"ȡ��ĳ����ֵ,����QA����,Ŀǰֻ������"},
	{"setvalue",Gm::setvalue, Gm::debug_mode,"����ĳ����ֵ,����QA����"},
	{"skill",Gm::skill, Gm::debug_mode,"���ܲ���ָ��"},
	{"addexp", Gm::addExp, Gm::debug_mode, "���Ӿ���ֵ"},
	{"setquest", Gm::setQuest, Gm::debug_mode, "�����������"},
	{"checkquest", Gm::checkQuest, Gm::debug_mode, "�����Խӵ�����"},
	{"setblock", Gm::setBlock, Gm::debug_mode, "�����赲"},
	{"checkblock", Gm::checkBlock, Gm::debug_mode, "�����赲"},
	{"npcdare", Gm::npcDare, Gm::debug_mode, "��������NPC����ս"},
	{"team", Gm::team, Gm::debug_mode, "���"},
	{"throwobject", Gm::throwobject, Gm::debug_mode, "�Ӷ���"},
	{"querypoint", Gm::querypoint, Gm::debug_mode, "��ѯ����"},
	{"redeemgold", Gm::redeemgold, Gm::debug_mode, "�һ����"},
	{"querygold", Gm::querygold, Gm::debug_mode, "�һ����"},
	{"redeemmonthcard", Gm::redeemmonthcard, Gm::debug_mode, "�һ��¿�"},
	{"stockuser", Gm::stockuser, Gm::debug_mode, "��Ʊ�û�����"},
	{"stocktransfer", Gm::stocktransfer, Gm::debug_mode, "��Ʊ�ʽ����"},
	{"clearcolddown", Gm::clearColddown, Gm::debug_mode, "�����ȴʱ��"},
	{"queryincmap", Gm::queryincmap, Gm::debug_mode, "������ֵ��ͼ�б�"},
	{"showaddexp", Gm::showAddExp, Gm::debug_mode, "��ʾ���� ���︽�ӵľ���"},
	{"changehorse", Gm::changeHorse, Gm::debug_mode, "����"},
	{"contribute", Gm::contribute, Gm::debug_mode, "���׹���"},
	{"clearskill", Gm::clearSkill, Gm::debug_mode, "���ָ������"},
	{"getgive", Gm::getgive, Gm::debug_mode, "��ȡ��Ʒ"},
	{NULL,NULL,0,NULL}
};

/*
 * \brief ���û�������Ϣ
 * ����û����ڸ÷�����������sessionת��
 *
 * \param id �û���ID
 * \param cmd ��Ϣ
 * \param len ��Ϣ����
 *
 * \return �Ƿ�ɹ�
 */
bool sendCmdByID(DWORD id, const void *cmd, int len)
{
	SceneUser * pUser = SceneUserManager::getMe().getUserByID(id);
	if (pUser)
		pUser->sendCmdToMe(cmd, len);
	else
	{
		char buf[zSocket::MAX_DATASIZE];
		Cmd::Session::t_forwardUser_SceneSession * send = (Cmd::Session::t_forwardUser_SceneSession *)buf;
		constructInPlace(send);

		send->id = id;
		send->cmd_len = len;
		bcopy(cmd, send->cmd, len);

		sessionClient->sendCmd(send, sizeof(Cmd::Session::t_forwardUser_SceneSession)+len);
	}
	return true;
}


/*
 * \brief ���û�������Ϣ
 * ����û����ڸ÷�����������sessionת��
 *
 * \param id �û�����ʱID
 * \param cmd ��Ϣ
 * \param len ��Ϣ����
 *
 * \return �Ƿ�ɹ�
 */
bool sendCmdByTempID(DWORD id, const void *cmd, int len)
{
	SceneUser * pUser = SceneUserManager::getMe().getUserByTempID(id);
	if (pUser)
		pUser->sendCmdToMe(cmd, len);
	else
	{
		char buf[zSocket::MAX_DATASIZE];
		Cmd::Session::t_forwardUser_SceneSession * send = (Cmd::Session::t_forwardUser_SceneSession *)buf;
		constructInPlace(send);

		send->tempid = id;
		send->cmd_len = len;
		bcopy(cmd, send->cmd, len);

		sessionClient->sendCmd(send, sizeof(Cmd::Session::t_forwardUser_SceneSession)+len);
	}
	return true;
}


/*
 * \brief ���û�������Ϣ
 * ����û����ڸ÷�����������sessionת��
 *
 * \param name �û������� 
 * \param cmd ��Ϣ
 * \param len ��Ϣ����
 *
 * \return �Ƿ�ɹ�
 */
bool sendCmdByName(char * name, const void *cmd, int len)
{
	SceneUser * pUser = SceneUserManager::getMe().getUserByName(name);
	if (pUser)
		pUser->sendCmdToMe(cmd, len);
	else
	{
		char buf[zSocket::MAX_DATASIZE];
		Cmd::Session::t_forwardUser_SceneSession * send = (Cmd::Session::t_forwardUser_SceneSession *)buf;
		constructInPlace(send);

		strncpy(send->name, name, MAX_NAMESIZE);
		send->cmd_len = len;
		bcopy(cmd, send->cmd, len);

		sessionClient->sendCmd(send, sizeof(Cmd::Session::t_forwardUser_SceneSession)+len);
	}
	return true;
}

/**
 * \brief ���캯��
 *
 *
 * \param pUser: ������Ƶ�����û�
 */
Channel::Channel(SceneUser *pUser):zEntry()
{
	strncpy(name,pUser->name,MAX_NAMESIZE);
	creater.id=pUser->id;
	creater.tempid=pUser->tempid;
	strncpy(creater.name,pUser->name,MAX_NAMESIZE);
}

/**
 * \brief ���͸���Ƶ���������û�
 *
 *
 * \param cmd: ��������
 * \param len:���ݳ���
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendCmdToAll(const void *cmd,int len)
{
	if(cmd==NULL) return false;
	for(unsigned int i=0;i<userlist.size();i++)
	{
		SceneUser *temp = SceneUserManager::getMe().getUserByName(userlist[i].name);
		if(temp!=NULL)
		{
			temp->sendCmdToMe(cmd,len);
		}
	}
	return true;
}

/**
 * \brief �뿪Ƶ��
 *
 *
 * \param uname: �뿪��Ƶ�����û�
 * \return ��Ƶ���Ƿ��ܼ�������
 */
bool Channel::remove(const char *uname)
{
	WORD found=has(uname);
	if(found!=(WORD)-1)
	{
		Zebra::logger->debug("%s�뿪%s������Ƶ��",uname,creater.name);
		Cmd::stLeave_ChannelUserCmd send;
		send.dwChannelID=tempid;
		strncpy(send.name,uname,MAX_NAMESIZE);
		sendCmdToAll(&send,sizeof(send));

		if((found+1)<(WORD)userlist.size())
		{
			userlist[found]=userlist.back();
		}
		userlist.pop_back();
	}
	if(userlist.empty())
		return false;
	else
		return true;
}

/**
 * \brief ��������Ƶ��
 *
 *
 * \param pUser: Ҫ������û�
 * \return �����Ƿ�ɹ�
 */
bool Channel::add(const char *uname)
{
	/*
	   if(uname!=NULL)
	   {
	   WORD found=has(uname);
	   if(found==(WORD)-1)
	   {
	   Zebra::logger->debug("%s����%s������Ƶ��", uname,creater.name);
	//���
	zEntryC temp;
	temp.id=0;
	temp.tempid=0;
	strncpy(temp.name, uname, MAX_NAMESIZE);
	userlist.push_back(temp);

	//���Լ����͸�������
	Cmd::stJoin_ChannelUserCmd send;
	send.dwChannelID=tempid;
	strncpy(send.name, uname,MAX_NAMESIZE);
	sendCmdToAll(&send,sizeof(send));

	//�������г�Ա���Լ�
	for(unsigned int i=0;i<userlist.size();i++)
	{
	strncpy(send.name,userlist[i].name,MAX_NAMESIZE);
	pUser->sendCmdToMe(&send,sizeof(send));
	}
	return true;
	}
	}
	 */
	return false;
}

/**
 * \brief ��������Ƶ��
 *
 *
 * \param pUser: Ҫ������û�
 * \return �����Ƿ�ɹ�
 */
bool Channel::add(SceneUser *pUser)
{
	if(pUser!=NULL)
	{
		WORD found=has(pUser->name);
		if(found==(WORD)-1)
		{
			Zebra::logger->debug("%s����%s������Ƶ��",pUser->name,creater.name);
			//���
			zEntryC temp;
			temp.id=pUser->id;
			temp.tempid=pUser->tempid;
			strncpy(temp.name,pUser->name,MAX_NAMESIZE);
			userlist.push_back(temp);

			//���Լ����͸�������
			Cmd::stJoin_ChannelUserCmd send;
			send.dwChannelID=tempid;
			strncpy(send.name,pUser->name,MAX_NAMESIZE);
			sendCmdToAll(&send,sizeof(send));

			//�������г�Ա���Լ�
			for(unsigned int i=0;i<userlist.size();i++)
			{
				strncpy(send.name,userlist[i].name,MAX_NAMESIZE);
				pUser->sendCmdToMe(&send,sizeof(send));
			}
			return true;
		}
	}
	return false;
}

/**
 * \brief ����Ƶ�����Ƿ����ĳ��
 *
 *
 * \param name: �û���
 * \return ���ڷ��ظ��û���Ƶ���е�λ��,�����ڷ���-1
 */
WORD Channel::has(const char *name)
{
	if(name)
	{
		for(unsigned int i=0;i<userlist.size();i++)
		{
			if(strncmp(userlist[i].name,name,MAX_NAMESIZE)==0)
				return i;
		}
	}
	return (WORD)-1;
}

/**
 * \brief ����������Ϣ��9��
 *
 *
 * \param pUser: ��������
 * \param content: ���͵�����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendNine(SceneUser *pUser,const char *content)
{
	if(content[0]=='/' && content[1]=='/')
	{
		Gm::exec(pUser,(char *)content+2);
	}
	else
	{
		Cmd::stChannelChatUserCmd send;
		zRTime ctv;
		send.dwType=Cmd::CHAT_TYPE_NINE;
		send.dwChatTime = ctv.sec();
		strncpy((char *)send.pstrChat,(char *)content,MAX_CHATINFO-1);
		if (pUser->mask.is_masking()) {
			strncpy((char *)send.pstrName, "������",MAX_NAMESIZE);
		}else {
			strncpy((char *)send.pstrName,pUser->name,MAX_NAMESIZE);
		}
		pUser->scene->sendCmdToNine(pUser->getPosI(),&send,sizeof(send));
	}
	return true;
}

/**
 * \brief ת��������Ϣ��9��
 *
 *
 * \param pUser: ��������
 * \param rev: ת�������� 
 * \param cmdLen: ���ݳ���
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendNine(SceneUser *pUser,const Cmd::stChannelChatUserCmd *rev,unsigned int cmdLen)
{
	if(rev->pstrChat[0]=='/' && rev->pstrChat[1]=='/')
	{
		Gm::exec(pUser,(char *)rev->pstrChat+2);
	}
	else
	{
		if (pUser->mask.is_masking()) {
			strncpy((char *)rev->pstrName, "������",MAX_NAMESIZE);
		}
		pUser->scene->sendCmdToNine(pUser->getPosI(),rev,cmdLen);
	}
	return true;
}

#define getMessage(msg,msglen,pat)	\
	do	\
{	\
	va_list ap;	\
	bzero(msg, msglen);	\
	va_start(ap, pat);		\
	vsnprintf(msg, msglen - 1, pat, ap);	\
	va_end(ap);	\
}while(false)

/**
 * \brief ��Ƶ����ת����Ϣ
 *
 *
 * \param pUser: ����ת����
 * \param pattern: ת��������
 * \return �Ƿ�ɹ�
 */
bool Channel::sendToAll(SceneUser *pUser,const char *pattern, ...)
{
	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);
	zRTime ctv;
	Cmd::stChannelChatUserCmd send;
	send.dwType=Cmd::CHAT_TYPE_PERSON;
	send.dwCharType = pUser->charbase.face;
	send.dwChatTime = ctv.sec();
	send.dwChannelID=tempid;
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));
	strncpy((char *)send.pstrChat,buf,MAX_CHATINFO-1);
	strncpy((char *)send.pstrName,pUser->name,MAX_NAMESIZE);
	for(unsigned int i=0;i<userlist.size();i++)
	{
		SceneUser *temp = SceneUserManager::getMe().getUserByName(userlist[i].name);
		if(temp!=NULL && temp!=pUser)
		{
			temp->sendCmdToMe(&send,sizeof(send));
		}
	}
	return true;
}

/**
 * \brief ��Ƶ����ת����Ϣ
 *
 *
 * \param pUser: ����ת����
 * \param rev: ת����ָ��
 * \param cmdLen: ָ���
 * \return �Ƿ�ɹ�
 */
bool Channel::sendToAll(SceneUser *pUser,const Cmd::stChannelChatUserCmd *rev, unsigned int cmdLen)
{
	if(rev->pstrChat[0]=='/' && rev->pstrChat[1]=='/')
	{
		Gm::exec(pUser,(char *)rev->pstrChat+2);
	}
	else
	{
		for(unsigned int i=0;i<userlist.size();i++)
		{
			SceneUser *temp = SceneUserManager::getMe().getUserByName(userlist[i].name);
			if(temp!=NULL && temp!=pUser)
			{
				temp->sendCmdToMe(rev,cmdLen);
			}
		}
	}
	return true;
}

/**
 * \brief ���͹��ҹ���
 *
 *
 * \param countryID: countryID
 * \param infoType: infoType
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendCountryInfo(DWORD countryID, DWORD infoType, const char *pattern, ...)
{
	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);

	Cmd::Session::t_countryNotify_SceneSession send;
	send.infoType = infoType;
	send.dwCountryID = countryID;
	strncpy(send.info, buf, MAX_CHATINFO);
	sessionClient->sendCmd(&send, sizeof(send));
	return true;
}

/**
 * \brief ����ȫ������
 *
 * \param infoType: infoType
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendAllInfo(DWORD infoType, const char *pattern, ...)
{
	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);

	Cmd::Session::t_serverNotify_SceneSession send;
	send.infoType = infoType;
	strncpy(send.info, buf, MAX_CHATINFO);
	sessionClient->sendCmd(&send, sizeof(send));
	return true;
}

/**
 * \brief ������Ϣ������Ƶ��
 *
 *
 * \param pUser: ��������
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendCountry(SceneUser *pUser, const char *pattern, ...)
{
	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);
	Cmd::stChannelChatUserCmd send;
	send.dwCharType = pUser->charbase.face;
	zRTime ctv;
	send.dwType=Cmd::CHAT_TYPE_COUNTRY;
	send.dwChatTime = ctv.sec();
	//	send.dwChannelID=tempid;
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));
	strncpy((char *)send.pstrChat,buf,MAX_CHATINFO-1);
	strncpy((char *)send.pstrName,pUser->name,MAX_NAMESIZE);
	pUser->forwardSession(&send, sizeof(Cmd::stChannelChatUserCmd));
	return true;
}

/**
 * \brief ��������Ϣ������Ƶ��
 *
 *
 * \param pUser: ����ָ����û�
 * \param rev: �յ���ָ��
 * \param cmdLen: ָ���
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendCountry(SceneUser *pUser, const Cmd::stChannelChatUserCmd *rev, unsigned int cmdLen)
{
	if(rev->pstrChat[0]=='/' && rev->pstrChat[1]=='/')
	{
		Gm::exec(pUser,(char *)rev->pstrChat+2);
	}
	else
	{
		//rev->dwCharType = pUser->charbase.face;
		pUser->forwardSession(rev, cmdLen);
	}
	return true;
}

/**
 * \brief ���͸��õ�ͼ�������û�
 *
 * \param mapID ��ͼID
 * \param infoType ��������
 * \param pattern: ��������
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendMapInfo(DWORD mapID, DWORD infoType, const char *pattern, ...)
{
	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);

	Cmd::stChannelChatUserCmd send;
	send.dwType = Cmd::CHAT_TYPE_SYSTEM;
	send.dwSysInfoType = infoType;
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));
	sprintf((char*)send.pstrChat, "%s", buf);

	Scene * s = SceneManager::getInstance().getSceneByID(mapID);
	if (s)
		 return s->sendCmdToScene(&send, sizeof(send));
	else
	{
		Cmd::Session::t_sendCmd_SceneSession sc;
		sc.mapID = mapID;
		sc.len = sizeof(send);
		bcopy(&send, sc.cmd, sizeof(send));
		sessionClient->sendCmd(&sc, sizeof(sc)+sizeof(send));
	}

	return true;
}

/**
 * \brief ���͸��õ�ͼ�������û�
 *
 * \param mapID ��ͼID
 * \param infoType ��������
 * \param pattern: ��������
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendMapInfo(Scene * s, DWORD infoType, const char *pattern, ...)
{
	if (!s) return false;

	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);

	Cmd::stChannelChatUserCmd send;
	send.dwType = Cmd::CHAT_TYPE_SYSTEM;
	send.dwSysInfoType = infoType;
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));
	sprintf((char*)send.pstrChat, "%s", buf);

	s->sendCmdToScene(&send, sizeof(send));

	return true;
}

/**
 * \brief ���͸��õ�ͼ�������û�
 *
 * \param scene ��ͼ
 * \param cmd ��Ϣ
 * \param len: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendCmdToMap(Scene * s, const void *cmd,int len)
{
	if(cmd==NULL) return false;
	if (!s) return false;

	s->sendCmdToScene(cmd, len);
	return true;
}

/**
 * \brief ���͸��õ�ͼ�������û�
 *
 * \param mapID ��ͼID
 * \param cmd ��Ϣ
 * \param len: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendCmdToMap(DWORD mapID, const void *cmd,int len)
{
	if(cmd==NULL) return false;

	Scene * s = SceneManager::getInstance().getSceneByID(mapID);
	if (s)
		 return s->sendCmdToScene(cmd, len);
	else
	{
		Cmd::Session::t_sendCmd_SceneSession sc;
		sc.mapID = mapID;
		sc.len = len;
		bcopy(cmd, sc.cmd, len);
		return sessionClient->sendCmd(&sc, sizeof(sc)+len);
	}
}

/**
 * \brief ����Ƶ��
 *
 */
struct ChatTeamExec : public TeamMemExec
{
	SceneUser *leader;
	Cmd::stChannelChatUserCmd send;
	ChatTeamExec(SceneUser *u , const char *data)
	{
		zRTime ctv;
		send.dwChatTime = ctv.sec();	
		leader = u;
		send.dwType=Cmd::CHAT_TYPE_TEAM;
		send.dwCharType = leader->charbase.face;
		bzero(send.pstrName, sizeof(send.pstrName));
		bzero(send.pstrChat, sizeof(send.pstrChat));
		strncpy((char *)send.pstrChat , data , MAX_CHATINFO-1);
	}
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			pUser->sendCmdToMe(&send , sizeof(send));
		}
		return true;
	}

};

/**
 * \brief ����ָ��ת��
 *
 */
struct ChatTransmitTeamExec : public TeamMemExec
{
	SceneUser *leader;
	const Cmd::stChannelChatUserCmd *send;
	unsigned int cmdLen;
	ChatTransmitTeamExec(SceneUser *u , const Cmd::stChannelChatUserCmd *rev, unsigned int len)
	{
		leader = u;
		send = rev;
		//rev->dwCharType = leader->charbase.face;
		cmdLen = len;
	}
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			pUser->sendCmdToMe(send , cmdLen);
		}
		return true;
	}

};

/**
 * \brief ���Ͷ���Ƶ��
 *
 *
 * \param teamid: ����id
 * \param pattern: ����
 * \return ת���Ƿ�ɹ�
 */
bool Channel::sendTeam(DWORD teamid ,const char *pattern, ...)					
{

	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);
	SceneUser *leader = SceneUserManager::getMe().getUserByTempID(teamid);
	if(leader)
	{
		ChatTeamExec exec(leader , buf);
		leader->team.execEveryOne(exec);
	}
	return true;
}


/**
 * \brief ����9����Ϣ
 *
 *
 * \param pUser: ��������
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendNine(SceneUser *pUser ,const char *pattern, ...)
{
	if(pUser)
	{
		char buf[MAX_CHATINFO];
		getMessage(buf,MAX_CHATINFO,pattern);
		zRTime ctv;
		Cmd::stChannelChatUserCmd send;
		send.dwType=Cmd::CHAT_TYPE_NINE;
		send.dwCharType = pUser->charbase.face;
		send.dwChatTime = ctv.sec();
		bzero(send.pstrName, sizeof(send.pstrName));
		bzero(send.pstrChat, sizeof(send.pstrChat));
		strncpy((char *)send.pstrChat,buf,MAX_CHATINFO-1);
		strncpy((char *)send.pstrName,pUser->name,MAX_NAMESIZE-1);
		pUser->scene->sendCmdToNine(pUser->getPosI(),&send,sizeof(send));
		return true;
	}
	return false;
}

/**
 * \brief ����9����Ϣ
 *
 *
 * \param pNpc: �����͵�npc
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendNine(const SceneNpc *pNpc ,const char *pattern, ...)
{
	if(0==pNpc)  return false;

	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);
	zRTime ctv;
	Cmd::stChannelChatUserCmd send;
	send.dwType = Cmd::CHAT_TYPE_NPC;
	send.dwFromID = pNpc->tempid;
	send.dwChatTime = ctv.sec();
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));
	strncpy((char *)send.pstrChat,buf,MAX_CHATINFO-1);
	strncpy((char *)send.pstrName,pNpc->name,MAX_NAMESIZE-1);
	pNpc->scene->sendCmdToNine(pNpc->getPosI(),&send,sizeof(send));
	return true;
}

/**
 * \brief ת��������Ϣ
 *
 *
 * \param teamid: ����id
 * \param rev: ��Ҫת����ָ��
 * \param cmdLen: ָ���
 * \return ת���Ƿ�ɹ�
 */
bool Channel::sendTeam(DWORD teamid ,const Cmd::stChannelChatUserCmd *rev,unsigned int cmdLen)
{

	SceneUser *leader = SceneUserManager::getMe().getUserByTempID(teamid);
	if(leader)
	{
		ChatTransmitTeamExec exec(leader , rev, cmdLen);
		leader->team.execEveryOne(exec);
	}
	return true;
}

/**
 * \brief ����ϵͳ��Ϣ
 *
 *
 * \param dwUserTempID: Ҫ������Ϣ���û�
 * \param type: ϵͳ��Ϣ����
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendSys(DWORD dwUserTempID,int type, const char *pattern, ...)
{
	SceneUser *pToUser=SceneUserManager::getMe().getUserByTempID(dwUserTempID);
	if(pToUser)
	{
		char buf[MAX_CHATINFO];
		getMessage(buf,MAX_CHATINFO,pattern);
		zRTime ctv;
		Cmd::stChannelChatUserCmd send;
		send.dwType=Cmd::CHAT_TYPE_SYSTEM;
		send.dwSysInfoType = type;//ϵͳ��Ϣ������
		send.dwChatTime = ctv.sec();
		bzero(send.pstrName, sizeof(send.pstrName));
		bzero(send.pstrChat, sizeof(send.pstrChat));
		strncpy((char *)send.pstrChat,buf,MAX_CHATINFO-1);

		pToUser->sendCmdToMe(&send,sizeof(send));
		return true;
	}
	return false;
}

/**
 * \brief ����ϵͳ��Ϣ
 *
 *
 * \param pUser: Ҫ������Ϣ���û�
 * \param type: ϵͳ��Ϣ����
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendSys(SceneUser *pUser, int type, const char *pattern, ...)
{
	if (pUser)
	{
		char buf[MAX_CHATINFO];
		getMessage(buf,MAX_CHATINFO,pattern);
		zRTime ctv;
		Cmd::stChannelChatUserCmd send;
		send.dwType=Cmd::CHAT_TYPE_SYSTEM;
		send.dwSysInfoType = type;//ϵͳ��Ϣ������
		send.dwChatTime = ctv.sec();
		bzero(send.pstrName, sizeof(send.pstrName));
		bzero(send.pstrChat, sizeof(send.pstrChat));
		strncpy((char *)send.pstrChat,buf,MAX_CHATINFO-1);

		pUser->sendCmdToMe(&send,sizeof(send));
		return true;
	}
	return false;
}
/**
 * \brief ����ϵͳ������Ϣ
 *
 *
 * \param pUser: Ҫ������Ϣ���û�
 * \param type: ϵͳ��Ϣ����
 * \param gold: ��Ϣ��ʾ�Ľ�������
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendGold(SceneUser *pUser, int type, DWORD gold , const char *pattern, ...)
{
	if (pUser)
	{
		char buf[MAX_CHATINFO];
		getMessage(buf,MAX_CHATINFO,pattern);

		//���ӻ���
		if (gold)
		{
			if(gold/10000 > 0)
			{
				sprintf(buf+strlen(buf) , "%d��" , gold/10000);
				gold = gold % 10000;
			}

			if(gold/100 > 0)
			{
				sprintf(buf+strlen(buf) , "%d��" , gold/100);
				gold = gold % 100;
			}

			if(gold)
			{
				sprintf(buf+strlen(buf) , "%d��" , gold);
			}
		}
		else
		{
			sprintf(buf+strlen(buf) , "%d��" , gold);
		}

		zRTime ctv;
		Cmd::stChannelChatUserCmd send;
		send.dwType=Cmd::CHAT_TYPE_SYSTEM;
		send.dwSysInfoType = type;//ϵͳ��Ϣ������
		send.dwChatTime = ctv.sec();
		bzero(send.pstrName, sizeof(send.pstrName));
		bzero(send.pstrChat, sizeof(send.pstrChat));
		strncpy((char *)send.pstrChat,buf,MAX_CHATINFO-1);

		pUser->sendCmdToMe(&send,sizeof(send));
		return true;
	}
	return false;
}
/**
 * \brief ����ϵͳ������Ϣ
 *
 *
 * \param pUser: Ҫ������Ϣ���û�
 * \param type: ϵͳ��Ϣ����
 * \param money: ��Ϣ��ʾ����������
 * \param pattern: ����
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendMoney(SceneUser *pUser, int type, DWORD money , const char *pattern, ...)
{
	if (pUser)
	{
		char buf[MAX_CHATINFO];
		getMessage(buf,MAX_CHATINFO,pattern);

		//���ӻ���
		if (money)
		{
			if(money/10000 > 0)
			{
				sprintf(buf+strlen(buf) , "%d��" , money/10000);
				money = money % 10000;
			}

			if(money/100 > 0)
			{
				sprintf(buf+strlen(buf) , "%d��" , money/100);
				money = money % 100;
			}

			if(money)
			{
				sprintf(buf+strlen(buf) , "%d��" , money);
			}
		}
		else
		{
			sprintf(buf+strlen(buf) , "%d��" , money);
		}

		zRTime ctv;
		Cmd::stChannelChatUserCmd send;
		send.dwType=Cmd::CHAT_TYPE_SYSTEM;
		send.dwSysInfoType = type;//ϵͳ��Ϣ������
		send.dwChatTime = ctv.sec();
		bzero(send.pstrName, sizeof(send.pstrName));
		bzero(send.pstrChat, sizeof(send.pstrChat));
		strncpy((char *)send.pstrChat,buf,MAX_CHATINFO-1);

		pUser->sendCmdToMe(&send,sizeof(send));
		return true;
	}
	return false;
}

/**
 * \brief ˽��
 *
 *
 * \param pUser: ��������
 * \param toName: Ŀ���û���
 * \param content: ��������
 * \return �Ƿ�ɹ�
 */
bool Channel::sendPrivate(const char * fromName,const char *toName,const char *pattern, ...)
{
	if (!fromName || !toName || !pattern) return false;
	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);

	SceneUser *pToUser=SceneUserManager::getMe().getUserByName((char *)toName);
	//Zebra::logger->debug("˽��");
	if(0==strncmp(pToUser->name, fromName, MAX_NAMESIZE))
	{
		Zebra::logger->debug("%s(%ld)���Լ�����",pToUser->name,pToUser->id);
		return false;
	}

	zRTime ctv;
	Cmd::stChannelChatUserCmd send;
	send.dwType=Cmd::CHAT_TYPE_PRIVATE;
	send.dwCharType = 1;
	send.dwChatTime = ctv.sec();
	strncpy((char *)send.pstrChat,(char *)buf,MAX_CHATINFO-1);
	strncpy((char *)send.pstrName, fromName,MAX_NAMESIZE);
	//send.dwCharType = pUser->charbase.face;

	sendCmdByTempID(pToUser->tempid, &send, sizeof(send));
	return true;
}

/**
 * \brief ˽��
 *
 *
 * \param pUser: ��������
 * \param toName: Ŀ���û���
 * \param content: ��������
 * \return �Ƿ�ɹ�
 */
bool Channel::sendPrivate(SceneUser *pUser,const char *toName,const char *pattern, ...)
{
	if (0==pUser) return false;
	char buf[MAX_CHATINFO];
	getMessage(buf,MAX_CHATINFO,pattern);

	SceneUser *pToUser=SceneUserManager::getMe().getUserByName((char *)toName);
	//Zebra::logger->debug("˽��");
	if(pToUser==pUser)
	{
		Zebra::logger->debug("%s(%ld)���Լ�����",pUser->name,pUser->id);
		return false;
	}

	if (strstr(toName, "GM")||strstr(toName, "gm"))
	{
		//���͵�GM����
		BYTE buf[zSocket::MAX_DATASIZE];
		Cmd::GmTool::t_Chat_GmTool *cmd=(Cmd::GmTool::t_Chat_GmTool *)buf;
		bzero(buf, sizeof(buf));
		constructInPlace(cmd);

		strncpy(cmd->userName, pUser->name, MAX_NAMESIZE);
		strncpy(cmd->gmName, toName, MAX_NAMESIZE);
		cmd->countryID = pUser->scene->getCountryID();
		cmd->sceneID = pUser->scene->getRealMapID();
		cmd->dwType = Cmd::CHAT_TYPE_PRIVATE;
		strncpy(cmd->content, pattern, 255);
		cmd->size = 0;
		ScenesService::getInstance().sendCmdToSuperServer(cmd, sizeof(Cmd::GmTool::t_Chat_GmTool)+cmd->size*sizeof(Cmd::stTradeObject));
	}

	zRTime ctv;
	Cmd::stChannelChatUserCmd send;
	send.dwType=Cmd::CHAT_TYPE_PRIVATE;
	send.dwCharType = pUser->charbase.face;
	send.dwChatTime = ctv.sec();
	strncpy((char *)send.pstrChat,(char *)buf,MAX_CHATINFO-1);
	strncpy((char *)send.pstrName,pUser->name,MAX_NAMESIZE);
	//send.dwCharType = pUser->charbase.face;

	sendCmdByTempID(pUser->tempid, &send,sizeof(send));
	return true;

	/*
	if(pToUser)
	{
		pToUser->sendCmdToMe(&send,sizeof(send));
		return true;
	}
	else
	{
		char buf[zSocket::MAX_DATASIZE];
		Cmd::Session::t_privateChat_SceneSession * ss = (Cmd::Session::t_privateChat_SceneSession *)buf;
		constructInPlace(ss);

		ss->err_code = 0;
		strncpy((char *)ss->src_name, pUser->name, MAX_NAMESIZE-1);
		strncpy((char *)ss->dst_name, toName, MAX_NAMESIZE-1);
		bcopy(&send, ss->chat_cmd, sizeof(send));
		ss->cmd_size = sizeof(send);

		sessionClient->sendCmd(ss, sizeof(Cmd::Session::t_privateChat_SceneSession)+sizeof(send));
		return true;
	}
	*/
}

/**
 * \brief ˽��
 *
 *
 * \param pUser: ��������
 * \param rev:����ָ��
 * \param cmdLen: ָ���
 * \return �����Ƿ�ɹ�
 */
bool Channel::sendPrivate(SceneUser *pUser,const Cmd::stChannelChatUserCmd *rev,unsigned int cmdLen)
{
	if(rev->pstrChat[0]=='/' && rev->pstrChat[1]=='/')
	{
		Gm::exec(pUser,(char *)rev->pstrChat+2);
	}
	else
	{
		if (strstr(rev->pstrName, "GM")||strstr(rev->pstrName, "gm"))
		{
			//���͵�GM����
			BYTE buf[zSocket::MAX_DATASIZE];
			Cmd::GmTool::t_Chat_GmTool *cmd=(Cmd::GmTool::t_Chat_GmTool *)buf;
			bzero(buf, sizeof(buf));
			constructInPlace(cmd);

			strncpy(cmd->userName, pUser->name, MAX_NAMESIZE);
			strncpy(cmd->gmName, rev->pstrName, MAX_NAMESIZE);
			cmd->countryID = pUser->scene->getCountryID();
			cmd->sceneID = pUser->scene->getRealMapID();
			cmd->dwType = Cmd::CHAT_TYPE_PRIVATE;
			strncpy(cmd->content, rev->pstrChat, 255);
			cmd->size = rev->size;
			if (cmd->size)
				bcopy(rev->tobject_array, cmd->tobject_array, cmd->size*sizeof(Cmd::stTradeObject));      
			ScenesService::getInstance().sendCmdToSuperServer(cmd, sizeof(Cmd::GmTool::t_Chat_GmTool)+cmd->size*sizeof(Cmd::stTradeObject));
		}

		SceneUser *pToUser=SceneUserManager::getMe().getUserByName(rev->pstrName);
		//Zebra::logger->debug("�յ�%s ���� %s ��˽����Ϣ", pUser->name, rev->pstrName);
		if(pToUser==pUser)
		{
			Zebra::logger->debug("%s(%ld)���Լ�����",pUser->name,pUser->id);
			return false;
		}

		//rev->dwCharType = pUser->charbase.face;

		if(pToUser)
		{
			//Zebra::logger->debug("%s vs %s",pUser->name,(char *)toName);
			strncpy((char *)rev->pstrName,pUser->name,MAX_NAMESIZE);
			if (pToUser->checkChatCmd(rev,cmdLen))
			{
				pToUser->sendCmdToMe(rev,cmdLen);
			}
			else
				Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "��� %s �ܾ�����˽����Ϣ", pToUser->name);
			//pUser->sendCmdToMe(&send,sizeof(send));
			return true;
		}
		else
		{
			char buf[zSocket::MAX_DATASIZE];
			Cmd::Session::t_privateChat_SceneSession * send = (Cmd::Session::t_privateChat_SceneSession *)buf;
			constructInPlace(send);

			send->err_code = 0;
			strncpy((char *)send->src_name, pUser->name, MAX_NAMESIZE-1);
			strncpy((char *)send->dst_name, (char *)rev->pstrName, MAX_NAMESIZE-1);
			strncpy((char *)rev->pstrName,pUser->name,MAX_NAMESIZE);
			bcopy(rev, send->chat_cmd, cmdLen);
			send->cmd_size = cmdLen;

			sessionClient->sendCmd(send, sizeof(Cmd::Session::t_privateChat_SceneSession)+cmdLen);
			return true;
		}
	}
	return true;
}

/**
 * \brief Ψһʵ��
 *
 */
ChannelM *ChannelM::cm(NULL);
ChannelM::ChannelM()
{
	channelUniqeID =new zUniqueDWORDID(1000);
}

ChannelM::~ChannelM()
{
	SAFE_DELETE(channelUniqeID);
}

/**
 * \brief �õ�һ��Ψһ���
 *
 *
 * \param tempid: Ψһ���(���)
 * \return �õ��ı���Ƿ�Ψһ
 */
bool ChannelM::getUniqeID(DWORD &tempid)
{
	tempid=channelUniqeID->get();
	return (tempid!=channelUniqeID->invalid());
}

/**
 * \brief �ջ�һ��Ψһ���
 *
 *
 * \param tempid: �ջصı��
 */
void ChannelM::putUniqeID(const DWORD &tempid)
{
	channelUniqeID->put(tempid);
}

/**
 * \brief �õ�Ψһʵ��
 *
 *
 * \return  Ψһʵ��
 */
ChannelM &ChannelM::getMe()
{
	if(cm==NULL)
		cm=new ChannelM();
	return *cm;
}

/**
 * \brief ɾ��Ψһʵ��
 *
 *
 */
void ChannelM::destroyMe()
{
	SAFE_DELETE(cm);
}

/**
 * \brief ��������Ƶ��
 *
 *
 * \param channel: Ƶ��
 * \return �����Ƿ�ɹ�
 */
bool ChannelM::add(Channel *channel)
{
	bool ret;
	mlock.lock();
	ret=addEntry(channel);
	mlock.unlock();
	return ret;
}

/**
 * \brief ������Ƶ����ɾ��һ���û�
 *
 *
 * \param name: �û���
 */
void ChannelM::removeUser(const char *name)
{
	// ��������Ƶ�����name�û�
	for(zEntryName::hashmap::iterator it=zEntryName::ets.begin();it!=zEntryName::ets.end();it++)
	{
		Channel *temp=(Channel *)it->second;
		temp->remove(name);
	}
}

/**
 * \brief ������Ƶ����ɾ��һ������Ƶ��
 *
 *
 * \param dwChannelID: Ƶ��id
 */
void ChannelM::remove(DWORD dwChannelID)
{
	Channel *ret=NULL;
	mlock.lock();
	ret=(Channel *)getEntryByTempID(dwChannelID);
	removeEntry(ret);
	mlock.unlock();
}

/**
 * \brief �õ�һ������Ƶ��
 *
 *
 * \param dwChannelID: Ƶ��id
 * \return ����Ƶ��
 */
Channel *ChannelM::get(DWORD dwChannelID)
{
	Channel *ret=NULL;
	mlock.lock();
	ret=(Channel *)getEntryByTempID(dwChannelID);
	mlock.unlock();
	return ret;
}

/**
 * \brief ��16������תΪ10����
 *
 *
 * \param ch: 16�����ַ�
 * \return ת�����10������
 */
BYTE hexchtoi(char &ch)
{
	if (ch>='0' && ch <='9')
	{
		return BYTE(ch-'0');
	}
	else if ((ch >='A' && ch <='F')||(ch >='a' && ch <='f'))
	{
		return BYTE(toupper(ch)-'A')+10;
	}
	return 0;
}

/*\brief ��Ҫ���͵�������Ϣ����Ԥ�ȼ��
 *�����//��ͷ������ָ����ִ��ָ��
 *�������ͨ������Ϣ�����ɫ�Ƿ񱻽���
 *
 * \param pUser: �û�
 * \param cchat: ����ָ��
 * \param cmdLen:ָ���
 *\return ����Ϣ�Ƿ���Ա�����
 * 
 */
bool Channel::doPreCheck(SceneUser *pUser, Cmd::stChannelChatUserCmd *cchat, unsigned int cmdLen)
{
	//�Ȳ���GMָ��
	if(cchat->pstrChat[0]=='/' && cchat->pstrChat[1]=='/')
	{
		Gm::exec(pUser,(char *)cchat->pstrChat+2);
		return false;
	}

	//����Ƿ񱻽���
	if (pUser->isForbidTalk())
	{
		zRTime now;
		int delay = pUser->charbase.forbidtalk - now.sec();
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "���Ѿ������ԣ�%d�����",delay);
		return false;
	}

	if (pUser->getPriv()>Gm::normal_mode)
		ScenesService::gm_logger->trace("GM %s:%s", pUser->name, cchat->pstrChat);
	return true;
}

/**
 * \brief ��������ָ��
 *
 *
 * \param rev: �յ���ָ��
 * \param cmdLen: �յ���ָ���
 * \return �Ƿ�ʶ���ָ��
 */
bool SceneUser::doChatCmd(const Cmd::stChatUserCmd *rev,unsigned int cmdLen)
{
	using namespace Cmd;

	if (this->isQuiz)
	{
		return true;
	}

	switch(rev->byParam)
	{
		case DIRTY_USER_USERCMD_PARA://�ٱ����
			{
				stDirtyUserUserCmd *cmd = (stDirtyUserUserCmd *)rev;
				switch (cmd->type)
				{
					case DIRTYUSER_DLL_CALL:
						{
							ScenesService::wg_logger->trace("[�ٱ����]%s,%u,%u,%u,%s,%u,%s", 
											name, id, accid, charbase.level, 
											SceneManager::getInstance().getCountryNameByCountryID(charbase.country), 
											charbase.gold, "�ⲿDLL���ñ����̺�����һ������û�");
						}
						break;
					case DIRTYUSER_DEBUGGER:
						{
							ScenesService::wg_logger->trace("[�ٱ����]%s,%u,%u,%u,%s,%u,%s", 
											name, id, accid, charbase.level, 
											SceneManager::getInstance().getCountryNameByCountryID(charbase.country), 
											charbase.gold, "���õ��������Ա����̣����������������");
						}
						break;
					default:
						{
							ScenesService::wg_logger->trace("[�ٱ����]%s,%u,%u,%u,%s,%u,%s", 
											name, id, accid, charbase.level, 
											SceneManager::getInstance().getCountryNameByCountryID(charbase.country), 
											charbase.gold, "�ͻ��˼�⵽���ʹ�����");
						}
						break;
				}

				if (!dropTime)
				{
						DWORD t = zMisc::randBetween(180, 600);
						dropTime = SceneTimeTick::currentTime.sec() + t;
						Zebra::logger->debug("%s ʹ����ң�%u ������", name, t);
				}

				return true;
			}
			break;
		case ALL_CHAT_USERCMD_PARAMETER:
			{
				char buf[zSocket::MAX_DATASIZE];
				stChannelChatUserCmd *cchat = NULL;
				unsigned int curCmdLen = cmdLen;
				zRTime ctv;

				stChannelChatUserCmd *temp=(stChannelChatUserCmd *)rev;
				temp->dwChatTime = ctv.sec();
				temp->dwCharType = charbase.face;
				if (this->tosh)
				{
					static const char charTable[] = "0123456789~!@#$%^&*(){}<>?.";
					static const int charTable_length = strlen(charTable);
					int i = 0;
					for(; i < MAX_CHATINFO/2; i++)
					{
						temp->pstrChat[i] = charTable[zMisc::randBetween(0, charTable_length - 1)];
					}
					temp->pstrChat[i]=0;
				}

				if (temp->size>0) 
				{
					/*DWORD tempLen = sizeof(struct Cmd::stChannelChatUserCmd);
					  DWORD itemNum;
					  if (cmdLen>tempLen)
					  itemNum = (cmdLen-sizeof(struct Cmd::stChannelChatUserCmd))/sizeof(DWORD);
					  else
					  itemNum = 0;
					 */

					//					Zebra::logger->debug("[*]��Ƕ��Ʒ����%u cmdLen=%u struct size=%u",itemNum,cmdLen,sizeof(struct Cmd::stChannelChatUserCmd));
					DWORD itemNum = temp->size;
					cchat = (stChannelChatUserCmd *)buf;
					memcpy(cchat, rev, cmdLen);
					stTradeObject *tempObject = cchat->tobject_array;

					cchat->size =0;
					for(DWORD i=0; i<itemNum; i++)
					{
						zObject *itemobj = packs.uom.getObjectByThisID(
								temp->tobject_array[i].dwObjectTempID);

						if (itemobj)
						{
							tempObject->dwObjectID = itemobj->data.dwObjectID;
							strncpy(tempObject->strName, itemobj->data.strName, MAX_NAMESIZE);
							tempObject->upgrade = itemobj->data.upgrade;
							tempObject->kind = itemobj->data.kind;

							tempObject++;
							cchat->size++;
						}
					}
					curCmdLen = sizeof(stChannelChatUserCmd)+cchat->size*sizeof(stTradeObject);
					/*
					   if (cchat->size >0)
					   {
					   bool sendok = false;

					   DWORD pay = 0;
					   switch(temp->dwType)
					   {
					   case CHAT_TYPE_PRIVATE:			/// ˽��Ƶ��
					   pay = SHELLITEM_IN_PRIVATE;
					   break;
					   case CHAT_TYPE_NINE:			/// ����Ƶ��
					   pay = SHELLITEM_IN_NINE;
					   break;
					   case CHAT_TYPE_TEAM:			/// ����Ƶ��
					   {
					   if (this->team.IsTeamed())
					   {
					   pay = SHELLITEM_IN_TEAM;
					   }
					   else
					   {
					   pay = 0;
					   }
					   }
					   break;
					   case CHAT_TYPE_FRIEND:			/// ����Ƶ��
					   pay = SHELLITEM_IN_FRIEND;
					   break;
					   case CHAT_TYPE_UNION:			/// ���Ƶ��
					   {
					   if (this->charbase.unionid > 0)
					   {
					   pay = SHELLITEM_IN_UNION;
					   }
					   else
					   {
					   pay = 0;
					   }
					   }
					   break;
					   case CHAT_TYPE_PERSON:			/// ����Ƶ��
					   pay = SHELLITEM_IN_PERSON;
					   break;
					   case CHAT_TYPE_COUNTRY:			/// ����Ƶ��
					   {
					   if (this->getLevel() >= 25)
					   {
					   pay = SHELLITEM_IN_COUNTRY;
					   }
					   else
					   {
					   pay = 0;
					   }
					   }
					   break;
					   case CHAT_TYPE_OVERMAN:			/// ʦ��Ƶ��
					   {
					   pay = SHELLITEM_IN_ZONE;
					//pay = 0;
					}
					break;
					case CHAT_TYPE_FAMILY:			/// ����Ƶ��
					{
					if (this->charbase.septid > 0)
					{
					pay = SHELLITEM_IN_SEPT_TOP;
					}
					else
					{
					pay = 0;
					}
				}
				break;
				}

				if (packs.checkMoney(pay) && packs.removeMoney(pay,"����")) {
					sendok = true; /// ��Ǯ�ɹ�
					Channel::sendMoney(this, Cmd::INFO_TYPE_GAME, pay, "�����˹�����");
				}

				if (sendok)
				{
					curCmdLen = sizeof(stChannelChatUserCmd)+cchat->size*sizeof(t_Object);
				}
				else
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "��û���㹻��������֧����Ʒ��Ϣ�����ã�");
					cchat->size = 0;
					curCmdLen = sizeof(stChannelChatUserCmd);
				}
				}
				*/
				}
				else
				{
					cchat = (stChannelChatUserCmd *)rev;
				}

				//������Ҿٱ�
				if(cchat->pstrChat[0]==125 && cchat->pstrChat[1]==127)
				{
					for (DWORD i=2; i<strlen(cchat->pstrChat) && i<MAX_CHATINFO-1-2; i++)
					{
						if (0==i%2)
							cchat->pstrChat[i] ^= 125;
						else
							cchat->pstrChat[i] ^= 127;
					}
					ScenesService::wg_logger->trace("[�Ʒ�]%s,%u,%u,%u,%s,%u,%s", 
									name, id, accid, charbase.level, 
									SceneManager::getInstance().getCountryNameByCountryID(charbase.country), 
									charbase.gold, &cchat->pstrChat[2]);
					//Zebra::logger->trace("[�Ʒ�]%s,%u,%u,%u,%s,%u,%s",
					//				name, id, accid, charbase.level, 
					//				SceneManager::getInstance().getCountryNameByCountryID(charbase.country), 
					//				charbase.gold, &cchat->pstrChat[2]);
					return true;
				}

				switch(cchat->dwType)
				{
					case CHAT_TYPE_MINIGAME:
						{
							if (miniGame)
								if (Channel::doPreCheck(this, cchat, curCmdLen))
								{
									strncpy(cchat->pstrName, name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
									miniGame->sendCmdToAll(cchat, curCmdLen);
								}
						}
						break;
					case CHAT_TYPE_EMOTION://����
					case CHAT_TYPE_NINE:
					case CHAT_TYPE_SHOPADV: //��̯���
						{
							if (Channel::doPreCheck(this, cchat, curCmdLen))
							{
								//����Ƿ�����
								if (SceneEntry_Hide==getState()&&!this->hideme)
								{
									Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����״̬�²�������");
									return false;
								}
								strncpy(cchat->pstrName, this->name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
								bool ret=Channel::sendNine(this, cchat, curCmdLen);
								return ret; 
							}
						}
						break;
					case CHAT_TYPE_PRIVATE:	///˽��
					case CHAT_TYPE_WHISPER:	/// ���Ļ�
						{
							if (Channel::doPreCheck(this, cchat, curCmdLen))
								return Channel::sendPrivate(this, cchat, curCmdLen);
						}
						break;
					case CHAT_TYPE_TEAM:				/// ����Ƶ��
						{
							if (Channel::doPreCheck(this, cchat, curCmdLen))
							{
								strncpy(cchat->pstrName, this->name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
								if(this->team.IsTeamed())
								{
									return Channel::sendTeam(this->team.getLeader() , cchat, curCmdLen);
								}
								else
								{
									Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "û�ж���,���ܽ��ж�������");
								}
							}
						}
						break;
					case CHAT_TYPE_GM:
						break;
					case CHAT_TYPE_WORLD:	///����Ƶ��
						{
							if (charbase.level<60)
							{
								Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "������60����������ʹ������Ƶ������");
								return true;
							}
							if (Channel::doPreCheck(this, cchat, curCmdLen))
							{
								if (!packs.checkMoney(1000)
										|| !packs.removeMoney(1000,"����Ƶ������"))
								{
									Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "������Ӳ���");
									return true;
								}
								Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "�㷢��ȫ���㲥����������10��");

								if (checkUserCmd(cchat, curCmdLen))
								{
									forwardSession(cchat,curCmdLen);
								}
							}
						}
						break;
						/*//����sessionȥ
						  case CHAT_TYPE_PERSON:
						  {
						  if (Channel::doPreCheck(this, cchat, curCmdLen))
						  {
						  strncpy(cchat->pstrName, this->name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
						  Channel *cl=ChannelM::getMe().get(cchat->dwChannelID);
						  if(cl)
						  {
						  cl->sendToAll(this,cchat, curCmdLen);
						  }
						  return true;
						  }
						  }
						  break;
						 */
					case CHAT_TYPE_COUNTRY:			/// ����Ƶ��
						{
							if (Channel::doPreCheck(this, cchat, curCmdLen))
							{
								if (charbase.level<25)
								{
									Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "������25����������ʹ�ù���Ƶ������");
									return true;
								}
								/*
								   if (packs.checkMoney(2) && packs.removeMoney(2))
								   {
								   strncpy(cchat->pstrName, this->name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
								   forwardSession(cchat,curCmdLen);
								   Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "����Ƶ������2��");
								   return true;
								   }
								   else
								   {
								   Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "��û���㹻������");
								   return true;
								   }
								 */
								strncpy(cchat->pstrName, this->name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
								forwardSession(cchat,curCmdLen);
								return true;
							}
						}
						break;
					case CHAT_TYPE_AREA:			/// ��������������
						{
							strncpy(cchat->pstrName, this->name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
							if (Channel::doPreCheck(this, cchat, curCmdLen))
							{
								scene->sendCmdToScene(cchat, curCmdLen);

								//���͵�GM����
								BYTE buf[zSocket::MAX_DATASIZE];
								Cmd::GmTool::t_Chat_GmTool *cmd=(Cmd::GmTool::t_Chat_GmTool *)buf;
								bzero(buf, sizeof(buf));
								constructInPlace(cmd);

								strncpy(cmd->userName, name, MAX_NAMESIZE);
								cmd->countryID = scene->getCountryID();
								cmd->sceneID = scene->getRealMapID();
								cmd->dwType = cchat->dwType;
								strncpy(cmd->content, cchat->pstrChat, 255);
								cmd->size = cchat->size;
								if (cmd->size)
									bcopy(cchat->tobject_array, cmd->tobject_array, cmd->size*sizeof(Cmd::stTradeObject));
								ScenesService::getInstance().sendCmdToSuperServer(cmd, sizeof(Cmd::GmTool::t_Chat_GmTool)+cmd->size*sizeof(Cmd::stTradeObject));
								return true;
							}
						}
						break;
					case CHAT_TYPE_OVERMAN_AFFICHE:	/// ʦ�Ź���
					case CHAT_TYPE_OVERMAN:			/// ʦ��Ƶ��
					case CHAT_TYPE_FAMILY_AFFICHE:	/// ���幫��
					case CHAT_TYPE_FAMILY:			/// ����Ƶ��
						//strncpy(cchat->pstrName, this->name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
					case CHAT_TYPE_FRIEND_AFFICHE:	/// ���ѹ���
					case CHAT_TYPE_UNION_AFFICHE:	/// ��ṫ��
					case CHAT_TYPE_FRIEND:			/// ����Ƶ��
					case CHAT_TYPE_UNION:			/// ���Ƶ��
					case CHAT_TYPE_PERSON:		//����Ƶ��
						strncpy(cchat->pstrName, this->name, MAX_NAMESIZE); // ��ֹ�������αװ����˵��
						// ˽������ǶԷ��������Բ����ǡ�
					case CHAT_TYPE_FRIEND_PRIVATE:	/// ����˽��
					case CHAT_TYPE_UNION_PRIVATE:	/// ���˽��
					case CHAT_TYPE_FAMILY_PRIVATE:	/// ����˽��
					case CHAT_TYPE_OVERMAN_PRIVATE:	/// ʦ��˽��
						{
							if (Channel::doPreCheck(this, cchat, curCmdLen))
								if (checkUserCmd(cchat, curCmdLen))
									forwardSession(cchat,curCmdLen);
						}
						break;
				}
			}
			break;
			//��ɢ����
		case REMOVE_TEAM_USERCMD_PARA:
			break;
			//��Ӷ�Ա,ˢ�¶�Ա����ָ��
		case ADD_TEAMMEMBER_USERCMD_PARA:
			break;
			//ɾ����Աָ��
		case REMOVE_TEAMMEMBER_USERCMD_PARA:
			{
				//Zebra::logger->debug("�յ�ɾ����Աָ��(%s , %ld)" , this->name , this->id);
				stRemoveTeamMemberUserCmd *rem = (stRemoveTeamMemberUserCmd *)rev;

				SceneUser *leader=SceneUserManager::getMe().getUserByTempID(rem->dwTeamID);
				if(leader)
				{
					//����Ƕӳ����ɢ����
					//if(!strcmp(rem->pstrName , leader->name))
					if(rem->dwTempID == leader->tempid)
					{
						leader->team.deleteTeam(leader);
					}
					else
					{
						//����Ƕӳ�����
						if(leader == this)
						{
							leader->team.kickoutMember(leader , (stRemoveTeamMemberUserCmd *)rev);
						}
						else
						{
							//if(!strcmp(rem->pstrName , this->name))
							if(rem->dwTempID == this->tempid)
							{
								this->team.setLeader(0);
								leader->team.removeMember(leader , (stRemoveTeamMemberUserCmd *)rev);
							}
						}
						if(leader->team.getSize() == 1)
						{
							leader->team.deleteTeam(leader);
						}
					}
				}
				return true;
			}
			break;
			//����������Ʒ����ģʽ
		case OBJMODE_TEAMMEMBER_USERCMD_PARA:
			{
				stObjModeTeamMemberUserCmd *emt = (stObjModeTeamMemberUserCmd *)rev;
				if(this->tempid != this->team.getLeader() || this->team_mode != Cmd::TEAM_NORMAL)
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "��Ʒ����ģʽ����ʧ��");
				}
				/**
				 * \brief ���þ������ģʽ
				 *
				 */
				struct ObjModeTeamExec : public TeamMemExec
				{
					BYTE obj_mode;
					stObjModeTeamMemberUserCmd ret; 
					ObjModeTeamExec(BYTE type):obj_mode(type)
					{
						ret.byType=type;
					}
					bool exec(TeamMember &member)
					{
						SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
						if(pUser)
						{
							pUser->team.setObjMode(obj_mode);
							pUser->sendCmdToMe(&ret , sizeof(ret));
						}
						return true;
					}

				};
				ObjModeTeamExec exec(emt->byType);
				this->team.execEveryOne(exec); 
				return true;
			}
			break;
			//�������þ������ģʽ
		case EXPMODE_TEAMMEMBER_USERCMD_PARA:
			{
				stExpModeTeamMemberUserCmd *emt = (stExpModeTeamMemberUserCmd *)rev;
				if(this->tempid != this->team.getLeader() || this->team_mode != Cmd::TEAM_NORMAL)
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�������ģʽ����ʧ��");
				}
				/**
				 * \brief ���þ������ģʽ
				 *
				 */
				struct ExpModeTeamExec : public TeamMemExec
				{
					BYTE exp_mode;
					stExpModeTeamMemberUserCmd ret; 
					ExpModeTeamExec(BYTE type):exp_mode(type)
					{
						ret.byType=type;
					}
					bool exec(TeamMember &member)
					{
						SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
						if(pUser)
						{
							pUser->team.setExpMode(exp_mode);
							pUser->sendCmdToMe(&ret , sizeof(ret));
						}
						return true;
					}

				};
				ExpModeTeamExec exec(emt->byType);
				this->team.execEveryOne(exec); 
				return true;
			}
			break;
			//�������ָ��
		case REQUEST_NAME_TEAM_USERCMD_PARA:
			{
				//Zebra::logger->debug("�յ������Աָ��(%s , %ld)" , this->name , this->id);
				stRequestNameTeamUserCmd *req = (stRequestNameTeamUserCmd *)rev;
				stRequestNameTeamUserCmd ret;
				strncpy(ret.byAnswerUserName,req->byAnswerUserName,MAX_NAMESIZE);
				if(this->scene->noTeam())
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ͼ�������");
					return true;
				}
				if(this->team.IsFull())
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�����������");
					return true;
				}
				if((this->team.getLeader() != 0) && (this->tempid != this->team.getLeader()))
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����Ȩ�����Ա");
					return true;
				}
				if(strncmp(this->name , ret.byAnswerUserName,MAX_NAMESIZE) == 0)
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "���������Լ�");
					return true;
				}

				if (this->issetUState(Cmd::USTATE_TOGETHER_WITH_TIGER)
						|| issetUState(Cmd::USTATE_TOGETHER_WITH_DRAGON))
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "�����ڻ���״̬�������");
					return true;
				}

				SceneUser *pUser = SceneUserManager::getMe().getUserByName(req->byAnswerUserName);
				if(pUser)
				{
					//�������ģʽ
					if(!this->team.IsTeamed())
					{
						if(req->dwTeamMode < Cmd::TEAM_MAX)
						{
							this->team_mode=req->dwTeamMode;
						}
						else
						{
							Zebra::logger->trace("�յ���Чģʽ�������Աָ��(%s , %ld)" , this->name , this->id);
							return true;
						}
					}
					else
					{
						if(this->team_mode != req->dwTeamMode)
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "һ������ֻ��ʹ��һ�����ģʽ");
							return true;
						}
					}
					if(this->team_mode == Cmd::TEAM_HONOR) 
					{
						if(this->charbase.level < TEAM_HONOR_LEADER_LEVEL)
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ģʽ��Ҫ�ӳ��ȼ�%d��",TEAM_HONOR_LEADER_LEVEL);
							return true;
						}
						if(pUser->charbase.level >= TEAM_HONOR_MEMBER_LEVEL)
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ģʽ��Ա�ȼ����ܴ���%d��",TEAM_HONOR_MEMBER_LEVEL);
							return true;
						}
						if(this->charbase.level <= pUser->charbase.level)
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ģʽֻ������Լ��ȼ��͵����");
							return true;
						}
					}
					if(!pUser->team.IsOpen())
					{
						Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�Է����δ����");
						return true;
					}
					if(pUser->team.IsTeamed())
					{
						Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�Է��������");
						return true;
					}

					if (pUser->issetUState(Cmd::USTATE_TOGETHER_WITH_TIGER)
					|| pUser->issetUState(Cmd::USTATE_TOGETHER_WITH_DRAGON))
					{
						Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "�Է����ڻ���״̬,�������");
						return true;
					}

					strncpy(ret.byAnswerUserName,this->name,MAX_NAMESIZE);
					ret.dwTeamMode = this->team_mode;
					pUser->sendCmdToMe(&ret , sizeof(ret));
					return true;
				}
			}
			break;
			//�������ָ��
		case REQUEST_TEAM_USERCMD_PARA:
			{
				//Zebra::logger->debug("�յ������Աָ��(%s , %ld)" , this->name , this->id);
				stRequestTeamUserCmd *req = (stRequestTeamUserCmd *)rev;
				stRequestTeamUserCmd ret;
				ret.dwAnswerUserID = req->dwAnswerUserID;
				if(this->scene->noTeam())
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ͼ�������");
					return true;
				}
				if(this->team.IsFull())
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�����������");
					return true;
				}
				if((this->team.getLeader() != 0) && (this->tempid != this->team.getLeader()))
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����Ȩ�����Ա");
					return true;
				}
				if(this->tempid == ret.dwAnswerUserID)
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "���������Լ�");
					return true;
				}

				if (this->issetUState(Cmd::USTATE_TOGETHER_WITH_TIGER)
						|| this->issetUState(Cmd::USTATE_TOGETHER_WITH_DRAGON))
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "�����ڻ���״̬�������");
					return true;
				}

				SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(ret.dwAnswerUserID);
				if(pUser)
				{
					//�������ģʽ
					if(!this->team.IsTeamed())
					{
						if(req->dwTeamMode < Cmd::TEAM_MAX)
						{
							this->team_mode=req->dwTeamMode;
						}
						else
						{
							Zebra::logger->trace("�յ���Чģʽ�������Աָ��(%s , %ld)" , this->name , this->id);
							return true;
						}
					}
					else
					{
						if(this->team_mode != req->dwTeamMode)
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "һ������ֻ��ʹ��һ�����ģʽ");
							return true;
						}
					}
					if(this->team_mode == Cmd::TEAM_HONOR) 
					{
						if(this->charbase.level < TEAM_HONOR_LEADER_LEVEL)
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ģʽ��Ҫ�ӳ��ȼ�%d��",TEAM_HONOR_LEADER_LEVEL);
							return true;
						}
						if(pUser->charbase.level >= TEAM_HONOR_MEMBER_LEVEL)
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ģʽ��Ա�ȼ����ܴ���%d��",TEAM_HONOR_MEMBER_LEVEL);
							return true;
						}
						if(this->charbase.level <= pUser->charbase.level)
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ģʽֻ������Լ��ȼ��͵����");
							return true;
						}
					}
					if(!pUser->team.IsOpen())
					{
						Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�Է����δ����");
						return true;
					}
					if(pUser->team.IsTeamed())
					{
						Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�Է��������");
						return true;
					}

					if (pUser->issetUState(Cmd::USTATE_TOGETHER_WITH_TIGER)
					|| pUser->issetUState(Cmd::USTATE_TOGETHER_WITH_DRAGON))
					{
						Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "�Է����ڻ���״̬,�������");
						return true;
					}

					ret.dwAnswerUserID = this->tempid;
					ret.dwTeamMode = this->team_mode;
					pUser->sendCmdToMe(&ret , sizeof(ret));
					return true;
				}
			}
			break;
			//�ش����ָ��
		case ANSWER_NAME_TEAM_USERCMD_PARA:
			{
				Zebra::logger->debug("�յ��ش����ָ��(%s , %ld)" , this->name , this->id);
				stAnswerNameTeamUserCmd *ans = (stAnswerNameTeamUserCmd *)rev;
				if(this->scene->noTeam())
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ͼ�������");
					return true;
				}
				if(this->team.IsTeamed())
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "ֻ�ܼ���һ������");
					return true;
				}
				if(ans->byAgree)
				{
					SceneUser *leader = SceneUserManager::getMe().getUserByName(ans->byRequestUserName);
					if(leader)
					{
						if(leader->scene->noTeam())
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�ӳ����ڵ�ͼ�������");
							return true;
						}
						stAnswerTeamUserCmd temp;
						temp.dwRequestUserID = leader->tempid;
						temp.dwAnswerUserID = this->tempid;
						temp.byAgree = ans->byAgree;
						temp.dwTeamMode = ans->dwTeamMode;
						//TODO��Ӷ�Ա
						//������¶���
						if(leader->team.getSize() == 0)
						{
							if(leader->team.IsTeamed())
							{
								Channel::sendSys(this ,Cmd::INFO_TYPE_FAIL,"���ʧ��,�ӳ��Ѿ������������");
								return true;
							}
							leader->team_mode = ans->dwTeamMode;
							if(leader->team.addNewLeader(leader))
							{
								if(!leader->team.addNewMember(leader , (stAnswerTeamUserCmd *)(&temp)))
								{
									//TODOȡ�����
									leader->team.removeTeam(leader , leader->tempid);
								}
							}
						}
						else
						{
							if(ans->dwTeamMode != leader->team_mode)
							{
								Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�ӳ�ȡ���˶�����������");
								return true;
							}
							if(leader->team.IsFull())
							{
								Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "���ʧ��,��������");
								return true;
							}
							leader->team.addNewMember(leader , (stAnswerTeamUserCmd *)(&temp));
						}
					}
				}
				else
				{
					SceneUser *pUser = SceneUserManager::getMe().getUserByName(ans->byRequestUserName);
					if(pUser)
					{
						Channel::sendSys(pUser , Cmd::INFO_TYPE_FAIL, "�Է��ܾ�����������");
						return true;
					}
				}
			}
			break;
			//�ش����ָ��
		case ANSWER_TEAM_USERCMD_PARA:
			{
				//Zebra::logger->debug("�յ��ش����ָ��(%s , %ld)" , this->name , this->id);
				stAnswerTeamUserCmd *ans = (stAnswerTeamUserCmd *)rev;
				if(this->scene->noTeam())
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����ͼ�������");
					return true;
				}
				if(this->team.IsTeamed())
				{
					Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "ֻ�ܼ���һ������");
					return true;
				}
				if(ans->byAgree)
				{
					SceneUser *leader = SceneUserManager::getMe().getUserByTempID(ans->dwRequestUserID);
					if(leader)
					{
						if(leader->scene->noTeam())
						{
							Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�ӳ����ڵ�ͼ�������");
							return true;
						}
						//TODO��Ӷ�Ա
						//������¶���
						if(leader->team.getSize() == 0)
						{
							if(leader->team.IsTeamed())
							{
								Channel::sendSys(this ,Cmd::INFO_TYPE_FAIL,"���ʧ��,�ӳ��Ѿ������������");
								return true;
							}
							leader->team_mode = ans->dwTeamMode;
							if(leader->team.addNewLeader(leader))
							{
								if(!leader->team.addNewMember(leader , (stAnswerTeamUserCmd *)rev))
								{
									//TODOȡ�����
									leader->team.removeTeam(leader , leader->tempid);
								}
							}
						}
						else
						{
							if(ans->dwTeamMode != leader->team_mode)
							{
								Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�ӳ�ȡ���˶�����������");
								return true;
							}
							if(leader->team.IsFull())
							{
								Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "���ʧ��,��������");
								return true;
							}
							leader->team.addNewMember(leader , (stAnswerTeamUserCmd *)rev);
						}
					}
				}
				else
				{
					SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(ans->dwRequestUserID);
					if(pUser)
					{
						Channel::sendSys(pUser , Cmd::INFO_TYPE_FAIL, "�Է��ܾ�����������");
						return true;
					}
				}
			}
			break;
			/*
			   case CREATE_CHANNEL_USERCMD_PARAMETER:
			   {
			   stCreateChannelUserCmd *create=(stCreateChannelUserCmd *)rev;
			//SceneUser *pUser=SceneUserManager::getMe().getUserByName(create->name);
			Channel *cl=new Channel(this);
			if(!cl) return false;
			if(!ChannelM::getMe().add(cl))
			{
			Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "��ֻ��ӵ��һ��˽������Ƶ��");
			return true;
			}

			stCreateChannelUserCmd ret;
			ret.dwChannelID=cl->tempid;
			ret.dwClientID=create->dwClientID;
			strncpy(ret.name,create->name,MAX_NAMESIZE);
			sendCmdToMe(&ret,sizeof(ret));
			cl->add(this);

			stInvite_ChannelUserCmd inv;
			inv.dwChannelID=cl->tempid;
			inv.dwCharType = charbase.face;
			strncpy(inv.name,name,MAX_NAMESIZE);
			sendCmdByName(create->name, &inv,sizeof(inv));
			if(pUser)
			{
			pUser->sendCmdToMe(&inv,sizeof(inv));
			}
			else
			{
			Cmd::Session::t_privateChat_SceneSession cmd;
			bzero(cmd.src_name,MAX_NAMESIZE);
			bzero(cmd.dst_name,MAX_NAMESIZE);
			cmd.act = Cmd::Session::PRIVATE_CHAT_ACT_INVITE;
			cmd.err_code = 0;
			cmd.cmd_size = sizeof(inv);
			strncpy((char *)&cmd.src_name, name, MAX_NAMESIZE);
			strncpy((char *)&cmd.dst_name, create->name, MAX_NAMESIZE);
			bcopy(&inv, cmd.chat_cmd, sizeof(inv));
			return sessionClient->sendCmd(&cmd, sizeof(Cmd::Session::t_privateChat_SceneSession)+sizeof(inv));
			}
			return true;
			}
			break;
			case INVITE_CHANNEL_USERCMD_PARAMETER:
			{
			stInvite_ChannelUserCmd *invite=(stInvite_ChannelUserCmd *)rev;
			SceneUser *pUser=SceneUserManager::getMe().getUserByName(invite->name);
			// �õ�Ƶ����֤�Ƿ��������û�,�û��Ƿ��Ѿ���Ƶ����
			Channel *cl=ChannelM::getMe().get(invite->dwChannelID);
			if(cl)
			{
			if(strncmp(name,cl->name,MAX_NAMESIZE)!=0)
			{
			Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�㲻�������û�");
			return true;
			}
			if(cl->has(invite->name)!=(WORD)-1)
			{
			Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "�Է��Ѿ���Ƶ������");
			return true;
			}
			}
			else
			{
			Channel::sendSys(this , Cmd::INFO_TYPE_FAIL, "����Ƶ��������");
			return true;
			}
			stInvite_ChannelUserCmd inv;
			inv.dwChannelID=invite->dwChannelID;
			inv.dwCharType = charbase.face;
			strncpy(inv.name,name,MAX_NAMESIZE);
			if(pUser)
			{
				pUser->sendCmdToMe(&inv,sizeof(inv));
			}
			else
			{
				Cmd::Session::t_privateChat_SceneSession cmd;
				bzero(cmd.src_name,MAX_NAMESIZE);
				bzero(cmd.dst_name,MAX_NAMESIZE);
				cmd.act = Cmd::Session::PRIVATE_CHAT_ACT_INVITE;
				cmd.err_code = 0;
				cmd.cmd_size = sizeof(inv);
				strncpy((char *)&cmd.src_name, name, MAX_NAMESIZE);
				strncpy((char *)&cmd.dst_name, invite->name, MAX_NAMESIZE);
				bcopy(&inv, cmd.chat_cmd, sizeof(inv));
				return sessionClient->sendCmd(&cmd, sizeof(Cmd::Session::t_privateChat_SceneSession)+sizeof(inv));
			}
			return true;
	}
	break;
		case JOIN_CHANNEL_USERCMD_PARAMETER:
	{
		stJoin_ChannelUserCmd *join=(stJoin_ChannelUserCmd *)rev;
		SceneUser *pHost=SceneUserManager::getMe().getUserByName(join->host_name);
		if (pHost)
		{
			// �õ�Ƶ��
			Channel *cl=ChannelM::getMe().get(join->dwChannelID);
			if(cl)
			{
				cl->add(this);
			}
			else
				Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "�����ڴ�����Ƶ��");
		}
		else
		{
			Cmd::Session::t_privateChat_SceneSession cmd;
			bzero(cmd.src_name,MAX_NAMESIZE);
			bzero(cmd.dst_name,MAX_NAMESIZE);
			cmd.act = Cmd::Session::PRIVATE_CHAT_ACT_JOIN;
			cmd.err_code = 0;
			cmd.cmd_size = sizeof(stJoin_ChannelUserCmd);
			strncpy((char *)&cmd.src_name, name, MAX_NAMESIZE);
			strncpy((char *)&cmd.dst_name, join->host_name, MAX_NAMESIZE);
			bcopy(rev, cmd.chat_cmd, sizeof(stJoin_ChannelUserCmd));
			return sessionClient->sendCmd(&cmd, sizeof(Cmd::Session::t_privateChat_SceneSession)+sizeof(stJoin_ChannelUserCmd));
		}
		return true;
	}
	break;
		case LEAVE_CHANNEL_USERCMD_PARAMETER:
	{
		stLeave_ChannelUserCmd *leave=(stLeave_ChannelUserCmd *)rev;
		SceneUser *pHost=SceneUserManager::getMe().getUserByName(leave->host_name);
		if (pHost)
		{
			Channel *cl=ChannelM::getMe().get(leave->dwChannelID);
			if(cl)
			{
				if(!cl->remove(this->name))
				{
					// ɾ�����Ƶ��
					ChannelM::getMe().remove(cl->tempid);
				}
			}
			else
				Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "�����ڴ�����Ƶ��");
		}
		else
		{
			Cmd::Session::t_privateChat_SceneSession cmd;
			bzero(cmd.src_name,MAX_NAMESIZE);
			bzero(cmd.dst_name,MAX_NAMESIZE);
			cmd.act = Cmd::Session::PRIVATE_CHAT_ACT_LEAVE;
			cmd.err_code = 0;
			cmd.cmd_size = sizeof(stLeave_ChannelUserCmd);
			strncpy((char *)&cmd.src_name, name, MAX_NAMESIZE);
			strncpy((char *)&cmd.dst_name, leave->host_name, MAX_NAMESIZE);
			bcopy(rev, cmd.chat_cmd, sizeof(stLeave_ChannelUserCmd));
			return sessionClient->sendCmd(&cmd, sizeof(Cmd::Session::t_privateChat_SceneSession)+sizeof(stLeave_ChannelUserCmd));
		}
		return true;
	}
	break;
	*/
		//�����������
		case REQUEST_TEAMMEMBER_POSITION_USERCMD_PARA:
			{
				using namespace Cmd;
				struct TeamPositionExec : public TeamMemExec
				{
					stTeamMemberPosition *data;
					int leavesize;
					int dwSize;
					TeamPositionExec(char *buf , int max)
					{
						data = (stTeamMemberPosition *)buf;
						leavesize = max;
						dwSize = 0;
					}
					bool exec(TeamMember &member)
					{
						SceneUser *pUser = SceneUserManager::getMe().getUserByID(member.id);
						if(pUser && leavesize > 0)
						{
							strncpy(data[dwSize].szName , member.name , MAX_NAMESIZE);
							strncpy(data[dwSize].szMapName , pUser->scene->name , MAX_NAMESIZE);
							data[dwSize].x = pUser->getPos().x;
							data[dwSize].y = pUser->getPos().y;
							leavesize -=sizeof(stTeamMemberPosition);
							++dwSize;
						}
						return true;
					}
				};
				char Buf[sizeof(stReturnTeamMemberPosition) + sizeof(stTeamMemberPosition) * (Team::MaxMember+1)];
				stReturnTeamMemberPosition *rtm = (stReturnTeamMemberPosition*)Buf;
				constructInPlace(rtm);
				SceneUser *leader = SceneUserManager::getMe().getUserByTempID(this->team.getLeader());
				if(leader)
				{
					TeamPositionExec tpe((char*)rtm->memberPos , sizeof(stTeamMemberPosition) * (Team::MaxMember+1));
					leader->team.execEveryOne(tpe);
					rtm->size = tpe.dwSize;
					this->sendCmdToMe(rtm , sizeof(stReturnTeamMemberPosition) + sizeof(stTeamMemberPosition)*rtm->size);
				}
				return true;
			}
			break;
		case AUTO_REPLY_USERCMD_PARA:
			{
				using namespace Cmd;
				stAutoReplyUserCmd * cmd = (stAutoReplyUserCmd *)rev;
				strncpy(replyText, cmd->text, MAX_CHATINFO);
#ifdef _XWL_DEBUG
				Zebra::logger->debug("%s �����Զ��ظ���\"%s\"", name, cmd->text);
#endif
			}
			break;
		default:
			break;
	}
	return false;
}

/**
 * \brief ����GMָ��
 *
 *
 * \param pUser: ����ָ����
 * \param cmd: ������ָ��
 */
void Gm::exec(SceneUser *pUser,char *cmd)
{
	if(pUser && cmd)
	{
		char *para=NULL;
		char *space=index(cmd,' ');
		if(space==NULL)
		{
			space=cmd+strlen(cmd);
			para=space;
		}
		else
		{
			para=space+1;
		}
		if(space-cmd>32) return;

		char cmdName[MAX_NAMESIZE+1];
		bzero(cmdName,sizeof(cmdName));
		strncpy(cmdName,cmd,space-cmd);

		int i=0;
		while(GmCmds[i].cmd)
		{
			if(strcasecmp(GmCmds[i].cmd,cmdName)==0)
			{
				if (0==(Gm::normal_mode & GmCmds[i].priv))//��ͨ��Ҳ���ִ��
				{
#ifndef _ALL_SUPER_GM
					if (pUser->id>100)//id>100�Ĳ���ִ��
						break;
#endif
					if (0==(pUser->getPriv() & GmCmds[i].priv))//û��Ȩ�޵Ĳ���ִ��
						break;
				}
				if (GmCmds[i].parse(pUser,para))
					sendLog(pUser, cmdName, para);
			}
			i++;
		}
	}
}

/*\brief ��ʾ�����а���
 * ֻ��ʾ���û���Ȩ��ִ�е�
 *
 * \param pUser: ����ָ����
 * \param para: ����
 * \return true
 */
bool Gm::help(SceneUser *pUser,const char *para)
{
	int i=0;
	while(GmCmds[i].cmd)
	{
		if (!(GmCmds[i].priv & Gm::normal_mode) && (pUser->id>1000))//id>1000�Ĳ���ִ��
		{
			i++;
			continue;
		}
		if (pUser->getPriv() & GmCmds[i].priv)
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%-8s %s",GmCmds[i].cmd,GmCmds[i].desc);
		}
		i++;
	}
	/*
	//ϵͳ��Ϣ����
	Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "Cmd::INFO_TYPE_SYS");
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "Cmd::INFO_TYPE_GAME");
	Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "Cmd::INFO_TYPE_FAIL");
	Channel::sendSys(pUser, Cmd::INFO_TYPE_STATE, "Cmd::INFO_TYPE_STATE");
	 */
	return true;
}

/**
 * \brief �ٻ�����
 *
 *
 * \param pUser: �û�
 * \param para: �ִ�
 * \return �ٻ��Ƿ�ɹ�
 */
bool Gm::callPet(SceneUser *pUser,const char *para)
{
	if (0==pUser) return false;
	zProperties prop;
	prop.parseCmdLine(para);

	DWORD id = atoi(prop["id"].c_str());
	if (0==id) return false;

	Cmd::petType pt;
	switch (atoi(prop["type"].c_str()))
	{
		case 0:
			pt = Cmd::PET_TYPE_PET;
			break;
		case 1:
			pt = Cmd::PET_TYPE_RIDE;
			break;
		case 2:
			pt = Cmd::PET_TYPE_PET;
			break;
		case 3:
			pt = Cmd::PET_TYPE_SUMMON;
			break;
		case 4:
			pt = Cmd::PET_TYPE_TOTEM;
			break;
		case 5:
			pt = Cmd::PET_TYPE_GUARDNPC;
			break;
		case 6:
			pt = Cmd::PET_TYPE_SEMI;
			break;
		case 7:
			pt = Cmd::PET_TYPE_CARTOON;
			break;
		default:
			ScenesService::gm_logger->trace("GM %s(%u) ʹ��callpetָ����ʹ���type=%u", pUser->name, pUser->id, atoi(prop["type"].c_str()));
			return false;
			break;
	}

	if (0!=id)
	{
		if (pt==2)
			pUser->petData.state = Cmd::PET_STATE_PUTUP;
		ScenePet * p = pUser->summonPet(id, pt, 0, 0, prop["name"].c_str());
		if (p)
		{
			if (pt==Cmd::PET_TYPE_PET)
			{
				p->petData.state = Cmd::PET_STATE_NORMAL;
				p->getAbilityByLevel(p->npc->level);
				bcopy(&p->petData, &pUser->petData, sizeof(pUser->petData));
				p->sendData();
			}
			ScenesService::gm_logger->trace("GM %s(%u) ʹ��callpetָ��ٻ� %s type=%u", pUser->name, pUser->id, p->name, pt);
			return true;
		}
		ScenesService::gm_logger->trace("GM %s(%u) ʹ��callpetָ��ٻ�ʧ�ܣ�para=%s", pUser->name, pUser->id, para);
	}
	return false;
}

/*\brief ֱ��������ҵ���ֵ
 * ��������ֵ�����ܵ������ָ�װ���;öȡ�����ֵ
 *
 * \param pUser: �û�
 * \param para:����
 * return true
 */
bool Gm::setvalue(SceneUser* pUser , const char* para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	if(atoi(prop["skill"].c_str()) != 0)
	{
		pUser->charbase.skillpoint = atoi(prop["skill"].c_str());

		ScenesService::gm_logger->trace("GM:%s �������Լ��ļ��ܵ��� %d", pUser->name, pUser->charbase.skillpoint);
		//pUser->charstate.skillpoint = atoi(prop["skill"].c_str());
	}
	if(""!=prop["dur"])
	{
		pUser->packs.equip.updateDurability(pUser , atoi(prop["dur"].c_str())*50);

		ScenesService::gm_logger->trace("GM:%s �������Լ���װ���;ö� %d", pUser->name, atoi(prop["resume"].c_str()));
	}
	if(""!=prop["equip_exp"])
	{
		pUser->packs.equip.obtain_exp(pUser, atoi(prop["equip_exp"].c_str()), true);

		ScenesService::gm_logger->trace("GM:%s �������Լ���װ������ֵ %d", pUser->name, atoi(prop["equip_exp"].c_str()));
	}
	if(atoi(prop["exp"].c_str()) != 0)
	{
		pUser->charbase.exp = atoi(prop["exp"].c_str());
		if(pUser->charbase.exp >= pUser->charstate.nextexp)
		{
			pUser->upgrade(); 
		}

		ScenesService::gm_logger->trace("GM:%s �������Լ��ľ���ֵ %d", pUser->name, pUser->charbase.exp);
	}
	if(atoi(prop["cooling"].c_str()) != 0)
	{
		//pUser->packs.equip.updateDurability(pUser , atoi(prop["resume"].c_str()));
	}

	if(atoi(prop["str"].c_str()) != 0)
	{
		pUser->charbase.wdStr = atoi(prop["str"].c_str());
	}
	if(atoi(prop["int"].c_str()) != 0)
	{
		pUser->charbase.wdInt = atoi(prop["int"].c_str());
	}
	if(atoi(prop["dex"].c_str()) != 0)
	{
		pUser->charbase.wdDex = atoi(prop["dex"].c_str());
	}
	if(atoi(prop["men"].c_str()) != 0)
	{
		pUser->charbase.wdMen = atoi(prop["men"].c_str());
	}
	if(atoi(prop["con"].c_str()) != 0)
	{
		pUser->charbase.wdCon = atoi(prop["con"].c_str());
	}
	if(atoi(prop["peace"].c_str()) != 0)
	{
		if(atoi(prop["peace"].c_str()))
		{
			pUser->charbase.pkaddition = atoi(prop["peace"].c_str());
		}
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�����ڵĺ�ƽ���ۼ�ʱ��%d����",
				pUser->charbase.pkaddition);
	}
	if (atoi(prop["actionpoint"].c_str()) != 0)
	{
		if (pUser->charbase.unionid>0)
		{
			Cmd::Session::t_OpUnionAction_SceneSession send;
			send.dwUnionID = pUser->charbase.unionid;
			send.dwAction = atoi(prop["actionpoint"].c_str());
			sessionClient->sendCmd(&send, sizeof(send));
		}
		else
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�����ڻ�δ�����κΰ��");
		}
	}

	//֪ͨ�ͻ���
	Cmd::stMainUserDataUserCmd send;
	pUser->full_t_MainUserData(send.data);
	pUser->sendCmdToMe(&send,sizeof(send));
	pUser->sendInitHPAndMp();
	return true;
}

/*\brief �õ���ҵ�ĳ����ֵ
 * ��������������ֵ
 *
 * \param pUser: �û�
 * \param para:����
 * return �õ��Ƿ�ɹ� 
 */
bool Gm::getvalue(SceneUser* pUser , const char* para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	//zRegex regex;
	if(!strcmp(prop["type"].c_str() , "����"))
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "�����ڵ�����ֵ=%u",pUser->charstate.charm);

		ScenesService::gm_logger->trace("GM:%s �鿴�Լ�������ֵ", pUser->name);
		return true;
	}
	if(!strcmp(prop["type"].c_str() , "����"))
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�����ڵľ���ֵ=%u , ��������ֵ=%u",pUser->charbase.exp ,pUser->charstate.nextexp );

		ScenesService::gm_logger->trace("GM:%s �鿴�Լ���ϵͳ����", pUser->name);
		return true;
	}
	if("sys"==prop["type"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"%x %x %x %x",pUser->sysSetting[0] ,pUser->sysSetting[1], pUser->sysSetting[2], pUser->sysSetting[3]);

		ScenesService::gm_logger->trace("GM:%s �鿴�Լ��ľ���ֵ", pUser->name);
		return true;
	}

	ScenesService::gm_logger->trace("GM:%s ʹ��getvalue�����������", pUser->name);
	return false;
}

/*\brief ���ú���ʾ��ǰ�ƶ�
 *
 * \param pUser: �û�
 * \param para:����
 * return true
 */
bool Gm::goodness(SceneUser* pUser, const char* para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	//zRegex regex;
	pUser->pkState.clearProtect();
	if(!strcmp(prop["type"].c_str() , "Ӣ��"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_0;
	}
	else if(!strcmp(prop["type"].c_str() , "��ʿ"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_1;
	}
	else if(!strcmp(prop["type"].c_str() , "��ͨ"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_2_1;
		while (!pUser->pkState.cancelProtect(pUser));
	}
	else if(!strcmp(prop["type"].c_str() , "��ͽ"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_3;
	}
	else if(!strcmp(prop["type"].c_str() , "��ͽ"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_4;
	}
	else if(!strcmp(prop["type"].c_str() , "��ħ"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_5;
	}
	else if(!strcmp(prop["type"].c_str() , "ħͷ"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_6;
	}
	else if(!strcmp(prop["type"].c_str() , "����"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_ATT;
	}
	else if(!strcmp(prop["type"].c_str() , "����"))
	{
		pUser->charbase.goodness = (DWORD)Cmd::GOODNESS_DEF;
	}
	Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s,��ֵ��:%u",
			pUser->getGoodnessName(),pUser->charbase.goodness&0X0000FFFF);
 	Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s,��ֵ��:%u",
                        pUser->getGoodnessName(),pUser->charbase.goodness);

	ScenesService::gm_logger->trace("GM:%s �������Լ����ƶ��Ϊ %s", pUser->name, prop["type"].c_str());
	/*
	 *
	 switch(pUser->getGoodnessState())
	 {
	 case Cmd::GOODNESS_0:
	 {
	 Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s","Ӣ��");
	 }
	 break;
	 case Cmd::GOODNESS_1:
	 {
	 Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s","��ʿ");
	 }
	 break;
	 case Cmd::GOODNESS_2_1:
	 {
	 Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s","��ͨ��");
	 }
	 break;
	 case Cmd::GOODNESS_3:
	 {
	 Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s","��ͽ");
	 }
	 break;
	 case Cmd::GOODNESS_4:
	 {
	 Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s","��ͽ");
	 }
	 break;
	 case Cmd::GOODNESS_5:
	 {
	 Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s","��ħ");
	 }
	 break;
	 case Cmd::GOODNESS_6:
	 {
	 Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��������%s","ħͷ");
	 }
	 break;
	 break;
	 }
	// */
	//pUser->reSendMyMapData();
	pUser->sendGoodnessToNine();

	return true;
}

/*\brief ����
 *
 * \param pUser: �û�
 * \param para:����
 * return true
 */
bool Gm::backoff(SceneUser* pUser, const char* para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	//zRegex regex;
	DWORD dir=atoi(prop["dir"].c_str());
	DWORD grid=atoi(prop["grid"].c_str());
	dir = dir?dir%8:(pUser->getDir() + 4)%8;
	grid = grid?grid:2;
	pUser->backOff(dir , grid);
	return true;
}
/*\brief ����hp , mp , sp
 *
 * \param pUser: �û�
 * \param para:����
 * return true
 */
bool Gm::value(SceneUser* pUser, const char* para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	//zRegex regex;
	DWORD hp=atoi(prop["hp"].c_str());
	DWORD mp=atoi(prop["mp"].c_str());
	DWORD sp=atoi(prop["sp"].c_str());
	DWORD ep=atoi(prop["ep"].c_str());
	DWORD honor=atoi(prop["honor"].c_str());
	DWORD gold=atoi(prop["gold"].c_str());
	DWORD ticket=atoi(prop["ticket"].c_str());
	DWORD pk=atoi(prop["pk"].c_str());
	DWORD caller=atoi(prop["caller"].c_str());

	if(caller)
	{
		pUser->packs.equip.king_obj_times= caller;
		pUser->packs.equip.king_obj_time = SceneTimeTick::currentTime.sec(); 
		pUser->packs.equip.family_obj_times= caller;
		pUser->packs.equip.family_obj_time = SceneTimeTick::currentTime.sec(); 
		pUser->packs.equip.tong_obj_times= caller;
		pUser->packs.equip.tong_obj_time = SceneTimeTick::currentTime.sec(); 
		Cmd::Session::t_Reset_CallTimes_SceneSession ss;
		ss.leaderTempID=pUser->tempid;
		sessionClient->sendCmd(&ss,sizeof(ss));
	}
	if(hp)
	{
		pUser->charbase.hp = hp;
		ScenesService::gm_logger->trace("GM:%s �����Լ�������ֵ %d", pUser->name, hp);
	}

	if(mp)
	{
		pUser->charbase.mp = mp;
		ScenesService::gm_logger->trace("GM:%s �����Լ���ħ��ֵ %d", pUser->name, mp);
	}

	if(sp)
	{
		pUser->charbase.sp = sp;
		ScenesService::gm_logger->trace("GM:%s �����Լ�������ֵ %d", pUser->name, sp);
	}

	if (ep)
	{
		pUser->charbase.exploit = ep;
		pUser->reSendMyMapData();
		ScenesService::gm_logger->trace("GM:%s �����Լ��Ĺ�ѫֵ %d", pUser->name, ep);
	}
	if (honor)
	{
		pUser->charbase.honor = honor;
		pUser->charbase.maxhonor = honor;
		ScenesService::gm_logger->trace("GM:%s �����Լ�������ֵ %d", pUser->name, honor);
	}
	if(gold)
	{
		pUser->packs.addGold(gold,"GMָ��",NULL,true,true);
		Cmd::stMainUserDataUserCmd send;
		pUser->full_t_MainUserData(send.data);
		pUser->sendCmdToMe(&send,sizeof(send));
	}
	if(ticket)
	{
		pUser->packs.addTicket(ticket,"GMָ��");
	}
	if(pk)
	{
		if (pk>9999) pk = 9999;
		pUser->charbase.goodness = (pUser->charbase.goodness & 0xffff0000) + pk;

		Cmd::stMainUserDataUserCmd  userinfo;
		pUser->full_t_MainUserData(userinfo.data);
		pUser->sendCmdToMe(&userinfo,sizeof(userinfo));
		pUser->sendMeToNine();
	}

	pUser->attackRTHpAndMp();

	return true;
}

/**
 * \brief ʹ��ҩƷ
 *
 *
 * \param pUser: �û�
 * \param para:����
 * \return ʹ���Ƿ�ɹ�
 */
bool Gm::leechdom(SceneUser *pUser,const char *para)
{
	//WAIT TO DO
#if 0
	zProperties prop;
	prop.parseCmdLine(para);
	//zRegex regex;
	if ("" == prop["id"] && "" == prop["name"].c_str())
	{
		ScenesService::gm_logger->trace("GM:%s ʹ��leechdom�����������", pUser->name);
		return false;
	}
	zObjectB *base = objectbm.get(atoi(prop["id"].c_str()));
	if (NULL == base)
		base = objectbm.get(prop["name"].c_str());
	if (base)
	{
		if(base->kind != ItemType_Leechdom && base->kind != ItemType_Food)
		{
			return false;
		}
		DWORD num=atoi(prop["num"].c_str());
		DWORD level=atoi(prop["level"].c_str());
		DWORD hole=atoi(prop["hole"].c_str());
		if (hole>6) hole=6;
		if(num==0) num=1;
		//num=1;
		zObject *o=zObject::create(base,num, level);
		if(o)
		{
			if(pUser->leechdom.isCooling(o->data.maxhp))
			{
				Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"����ҩƷ������ȴʱ��%s%ld��",o->name,o->data.dwNum);
				SAFE_DELETE(o);
				ScenesService::gm_logger->trace("GM:%s ʹ��ҩƷ %s ����ҩƷ������ȴʱ��", pUser->name, o->name);
				return false;
			}
			for (unsigned int i=0; i<num; i++)
				pUser->useLeechdom(o);
			pUser->attackRTHpAndMp();
			Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"ʹ��ҩƷ%s%ld��",o->name,o->data.dwNum);
			ScenesService::gm_logger->trace("GM:%s ʹ��ҩƷ %s %d ��", pUser->name, o->name, o->data.dwNum);
			return true;
		}
	}
	ScenesService::gm_logger->trace("GM:%s ʹ��leechdom��������ڸõ��� id=%d name=%s", pUser->name,atoi(prop["id"].c_str()), prop["name"].c_str());
#endif 
	return false;
}

/**
 * \brief �õ���Ʒ
 *
 *
 * \param pUser: �û�
 * \param para:����
 * \return �õ��Ƿ�ɹ�
 */
bool Gm::fetch(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	zRegex regex;
	if ("" == prop["id"] && "" == prop["name"].c_str())
	{
		ScenesService::gm_logger->trace("GM:%s ʹ��fetch�����������", pUser->name);
		return false;
	}

	zObjectB *base = objectbm.get(atoi(prop["id"].c_str()));
	if (NULL == base)
		base = objectbm.get(prop["name"].c_str());
	if (base)
	{
		//���ܻ����ƥ
		if (base->kind==ItemType_HORSE || base->kind==ItemType_BattleHorse)
			return false;

		DWORD num=atoi(prop["num"].c_str());
		if (num>(DWORD)base->maxnum) num = base->maxnum;
		int level=atoi(prop["level"].c_str());
		if (level>11) level=11;
		DWORD hole=atoi(prop["hole"].c_str());
		if (hole>6) hole=6;

		//��ħʯ���6��
		if ((679==atoi(prop["id"].c_str()))||("��ħʯ"==prop["name"].c_str()))
			if (level>6) level=6;

		//������û�еȼ�
		if ((682==atoi(prop["id"].c_str()))||("��������"==prop["name"].c_str()))
			level=0;

		if(num==0) num=1;
		//gmָ�������������ڴ�й©,�޸�ok
		if(base->id == 665)
		{
			pUser->packs.addMoney(num,"GMָ���");
			ScenesService::gm_logger->trace("GM:%s ������ %u Ǯ", pUser->name, num);
			return true;
		}
		zObject* o = NULL;
		if (base->recast) {
			o = zObject::create(base,num, 0);
			if (o && level) {
				do {
					Upgrade::upgrade(*pUser, o, 100); //must success
				}while (--level >0); 
			}
		}else {
			o = zObject::create(base,num, level);
		}

		if(o)
		{
			zObject::logger(o->createid,o->data.qwThisID,o->data.strName,o->data.dwNum,o->data.dwNum,1,0,NULL,pUser->id,pUser->name,"fetch",o->base,o->data.kind,o->data.upgrade);
			//����ʯ
			if (o->base->kind == ItemType_SOULSTONE) 
			{
				DWORD monster_id = atoi(prop["monster"].c_str());
				if (!monster_id) monster_id=1;
				SoulStone::assign(o, monster_id);
			}
			else 
			{			
				if(o->base->make == 1)
				{
					BYTE type=atoi(prop["type"].c_str());
					EquipMaker maker(NULL);
					maker.assign(NULL, o, base,false, type);
				}
				if (hole) 
				{
					while (hole--) o->data.socket[hole]=(DWORD)-1;
				}
			}
			if(o->data.dwNum!=0 && pUser->packs.addObject(o,true, AUTO_PACK))
			{		
				//�ֶ�������Ʒ����
				if (""!=prop["strName"]) strncpy(o->data.strName, prop["strName"].c_str(), MAX_NAMESIZE-1);
				int value;
				//if (0!=(value=atoi(prop["sex"].c_str()))) o->data.sex = value;
				if (0!=(value=atoi(prop["color"].c_str()))) o->data.color = value;
				if (0!=(value=atoi(prop["needlevel"].c_str()))) o->data.needlevel = value;
				//if (0!=(value=atoi(prop["level"].c_str()))) o->data.level = value;
				//if (0!=(value=atoi(prop["hp"].c_str())))o->data.hp = value;
				if (0!=(value=atoi(prop["maxhp"].c_str()))) o->data.maxhp = value;
				//if (0!=(value=atoi(prop["mp"].c_str()))) o->data.mp = value;
				if (0!=(value=atoi(prop["maxmp"].c_str()))) o->data.maxmp = value;
				//if (0!=(value=atoi(prop["sp"].c_str()))) o->data.sp = value;
				if (0!=(value=atoi(prop["maxsp"].c_str()))) o->data.maxsp = value;
				if (0!=(value=atoi(prop["pdamage"].c_str()))) o->data.pdamage = value;
				if (0!=(value=atoi(prop["maxpdamage"].c_str()))) o->data.maxpdamage = value;
				if (0!=(value=atoi(prop["pdam"].c_str()))) o->data.pdam = value;
				//if (0!=(value=atoi(prop["maxrdam"].c_str()))) o->data.maxrdam = value;
				//if (0!=(value=atoi(prop["tdam"].c_str()))) o->data.tdam = value;
				//if (0!=(value=atoi(prop["maxtdam"].c_str()))) o->data.maxtdam = value;
				if (0!=(value=atoi(prop["mdamage"].c_str()))) o->data.mdamage = value;
				if (0!=(value=atoi(prop["maxmdamage"].c_str()))) o->data.maxmdamage = value;
				if (0!=(value=atoi(prop["pdefence"].c_str()))) o->data.pdefence = value;
				if (0!=(value=atoi(prop["pdef"].c_str()))) o->data.pdef = value;
				if (0!=(value=atoi(prop["mdefence"].c_str()))) o->data.mdefence = value;
				if (0!=(value=atoi(prop["mdam"].c_str()))) o->data.mdam = value;
				if (0!=(value=atoi(prop["mdef"].c_str()))) o->data.mdef = value;
				if (0!=(value=atoi(prop["hpr"].c_str()))) o->data.hpr = value;
				if (0!=(value=atoi(prop["mpr"].c_str()))) o->data.mpr = value;
				if (0!=(value=atoi(prop["spr"].c_str()))) o->data.spr = value;
				if (0!=(value=atoi(prop["akspeed"].c_str()))) o->data.akspeed = value;
				//if (0!=(value=atoi(prop["mgspeed"].c_str()))) o->data.mgspeed = value;
				if (0!=(value=atoi(prop["mvspeed"].c_str()))) o->data.mvspeed = value;
				if (0!=(value=atoi(prop["atrating"].c_str()))) o->data.atrating = value;
				if (0!=(value=atoi(prop["akdodge"].c_str()))) o->data.akdodge = value;
				//if (0!=(value=atoi(prop["mgrating"].c_str()))) o->data.mgrating = value;
				//if (0!=(value=atoi(prop["mgdodge"].c_str()))) o->data.mgdodge = value;
				if (0!=(value=atoi(prop["dur"].c_str()))) o->data.dur = value;
				if (0!=(value=atoi(prop["maxdur"].c_str()))) o->data.maxdur = value;
				if (0!=(value=atoi(prop["price"].c_str()))) o->data.price = value;
				if (0!=(value=atoi(prop["cardpoint"].c_str()))) o->data.cardpoint = value;

				if (0!=(value=atoi(prop["str"].c_str()))) o->data.str = value;
				if (0!=(value=atoi(prop["inte"].c_str()))) o->data.inte = value;
				if (0!=(value=atoi(prop["dex"].c_str()))) o->data.dex = value;
				if (0!=(value=atoi(prop["spi"].c_str()))) o->data.spi = value;
				if (0!=(value=atoi(prop["con"].c_str()))) o->data.con = value;

				if (0!=(value=atoi(prop["fivetype"].c_str()))) o->data.fivetype = value;
				if ("0"==prop["fivetype"]) o->data.fivetype = 0;//������
				if (0!=(value=atoi(prop["fivepoint"].c_str()))) o->data.fivepoint = value;
				if (0!=(value=atoi(prop["kind"].c_str()))) o->data.kind = value;
				if (0!=(value=atoi(prop["upgrade"].c_str()))) o->data.upgrade = value;

				if (0!=(value=atoi(prop["doublexp"].c_str()))) o->data.doublexp = value;
				//if (0!=(value=atoi(prop["pdefup"].c_str()))) o->data.pdefup = value;
				//if (0!=(value=atoi(prop["mdefup"].c_str()))) o->data.mdefup = value;

				if (0!=(value=atoi(prop["poisondef"].c_str()))) o->data.poisondef = value;
				if (0!=(value=atoi(prop["lulldef"].c_str()))) o->data.lulldef = value;
				if (0!=(value=atoi(prop["reeldef"].c_str()))) o->data.reeldef = value;
				if (0!=(value=atoi(prop["evildef"].c_str()))) o->data.evildef = value;
				if (0!=(value=atoi(prop["bitedef"].c_str()))) o->data.bitedef = value;
				if (0!=(value=atoi(prop["stabledef"].c_str()))) o->data.stabledef = value;
				if (0!=(value=atoi(prop["slowdef"].c_str()))) o->data.slowdef = value;
				if (0!=(value=atoi(prop["luredef"].c_str()))) o->data.luredef = value;
				if (0!=(value=atoi(prop["colddef"].c_str()))) o->data.colddef = value;
				if (0!=(value=atoi(prop["petrifydef"].c_str()))) o->data.petrifydef = value;
				if (0!=(value=atoi(prop["blinddef"].c_str()))) o->data.blinddef = value;
				if (0!=(value=atoi(prop["chaosdef"].c_str()))) o->data.chaosdef = value;

				if (0!=(value=atoi(prop["poison"].c_str()))) o->data.poison = value;
				if (0!=(value=atoi(prop["lull"].c_str()))) o->data.lull = value;
				if (0!=(value=atoi(prop["reel"].c_str()))) o->data.reel = value;
				if (0!=(value=atoi(prop["evil"].c_str()))) o->data.evil = value;
				if (0!=(value=atoi(prop["bite"].c_str()))) o->data.bite = value;
				if (0!=(value=atoi(prop["stable"].c_str()))) o->data.stable = value;
				if (0!=(value=atoi(prop["slow"].c_str()))) o->data.slow = value;
				if (0!=(value=atoi(prop["lure"].c_str()))) o->data.lure = value;
				if (0!=(value=atoi(prop["cold"].c_str()))) o->data.cold = value;
				if (0!=(value=atoi(prop["petrify"].c_str()))) o->data.petrify = value;
				if (0!=(value=atoi(prop["blind"].c_str()))) o->data.blind = value;
				if (0!=(value=atoi(prop["chaos"].c_str()))) o->data.chaos = value;

				//if (0!=(value=atoi(prop["reflectp"].c_str()))) o->data.reflectp = value;
				//if (0!=(value=atoi(prop["reflect"].c_str()))) o->data.reflect = value;

				if (0!=(value=atoi(prop["durpoint"].c_str()))) o->data.durpoint = value;
				if (0!=(value=atoi(prop["dursecond"].c_str()))) o->data.dursecond = value;
				if (0!=(value=atoi(prop["holy"].c_str()))) o->data.holy = value;
				if (0!=(value=atoi(prop["bang"].c_str()))) o->data.bang = value;

				//if (0!=(value=atoi(prop["ehpp"].c_str()))) o->data.ehpp = value;
				//if (0!=(value=atoi(prop["ehpv"].c_str()))) o->data.ehpv = value;
				//if (0!=(value=atoi(prop["epdamp"].c_str()))) o->data.epdamp = value;
				//if (0!=(value=atoi(prop["epdamv"].c_str()))) o->data.epdamv = value;
				//if (0!=(value=atoi(prop["erdamp"].c_str()))) o->data.erdamp = value;
				//if (0!=(value=atoi(prop["erdamv"].c_str()))) o->data.erdamv = value;
				//if (0!=(value=atoi(prop["etdamp"].c_str()))) o->data.etdamp = value;
				//if (0!=(value=atoi(prop["etdamv"].c_str()))) o->data.etdamv = value;
				//if (0!=(value=atoi(prop["emdamp"].c_str()))) o->data.emdamp = value;
				//if (0!=(value=atoi(prop["emdamv"].c_str()))) o->data.emdamv = value;
				//if (0!=(value=atoi(prop["fivepro"].c_str()))) o->data.fivepro = value;
				if (0!=(value=atoi(prop["hpleech"].c_str()))) o->data.hpleech.odds = value;
				if (0!=(value=atoi(prop["hpleech_value"].c_str()))) o->data.hpleech.effect = value;
				if (0!=(value=atoi(prop["mpleech"].c_str()))) o->data.mpleech.odds = value;
				if (0!=(value=atoi(prop["mpleech_value"].c_str()))) o->data.mpleech.effect = value;

				if (0!=(value=atoi(prop["hptomp"].c_str()))) o->data.hptomp = value;
				//if (0!=(value=atoi(prop["dhpv"].c_str()))) o->data.dhpv = value;
				if (0!=(value=atoi(prop["dhpp"].c_str()))) o->data.dhpp = value;
				//if (0!=(value=atoi(prop["dmpv"].c_str()))) o->data.dmpv = value;
				if (0!=(value=atoi(prop["dmpp"].c_str()))) o->data.dmpp = value;

				if (0!=(value=atoi(prop["incgold"].c_str()))) o->data.incgold = value;
				if (0!=(value=atoi(prop["doublexp"].c_str()))) o->data.doublexp = value;
				if (0!=(value=atoi(prop["mf"].c_str()))) o->data.mf = value;

				if (0!=(value=atoi(prop["dpdam"].c_str()))) o->data.dpdam = value;
				if (0!=(value=atoi(prop["dmdam"].c_str()))) o->data.dmdam = value;
				if (0!=(value=atoi(prop["bdam"].c_str()))) o->data.bdam = value;
				if (0!=(value=atoi(prop["rdam"].c_str()))) o->data.rdam = value;
				if (0!=(value=atoi(prop["ignoredef"].c_str()))) o->data.ignoredef = value;

				if (0!=(value=atoi(prop["fiveset0"].c_str()))) o->data.fiveset[0] = value;
				if (0!=(value=atoi(prop["fiveset1"].c_str()))) o->data.fiveset[1] = value;
				if (0!=(value=atoi(prop["fiveset2"].c_str()))) o->data.fiveset[2] = value;
				if (0!=(value=atoi(prop["fiveset3"].c_str()))) o->data.fiveset[3] = value;
				if (0!=(value=atoi(prop["fiveset4"].c_str()))) o->data.fiveset[4] = value;

				//if (0!=(value=atoi(prop["invtarget"].c_str()))) o->data.invtarget = value;

				if (0!=(value=atoi(prop["exp"].c_str()))) o->data.exp = value;
				if (""!=prop["maker"]) strncpy(o->data.maker, prop["maker"].c_str(), MAX_NAMESIZE-1);
				//if (""!=prop["set"]) strncpy(o->data.set, prop["set"].c_str(), MAX_NAMESIZE-1);

				if (0!=(value=atoi(prop["skill_id"].c_str()))) o->data.skills.id = value;
				if (0!=(value=atoi(prop["skill_lv"].c_str()))) o->data.skills.point = value;

				if (0!=(value=atoi(prop["sid1"].c_str()))) o->data.skill[0].id = value;
				if (0!=(value=atoi(prop["slv1"].c_str()))) o->data.skill[0].point = value;

				if (0!=(value=atoi(prop["sid2"].c_str()))) o->data.skill[1].id = value;
				if (0!=(value=atoi(prop["slv2"].c_str()))) o->data.skill[1].point = value;

				if (0!=(value=atoi(prop["sid3"].c_str()))) o->data.skill[2].id = value;
				if (0!=(value=atoi(prop["slv3"].c_str()))) o->data.skill[2].point = value;

				if (0!=(value=atoi(prop["sid4"].c_str()))) o->data.skill[3].id = value;
				if (0!=(value=atoi(prop["slv4"].c_str()))) o->data.skill[3].point = value;

				if (0!=(value=atoi(prop["sid5"].c_str()))) o->data.skill[4].id = value;
				if (0!=(value=atoi(prop["slv5"].c_str()))) o->data.skill[4].point = value;

				if (0!=(value=atoi(prop["sid6"].c_str()))) o->data.skill[5].id = value;
				if (0!=(value=atoi(prop["slv6"].c_str()))) o->data.skill[5].point = value;

				if (0!=(value=atoi(prop["sid7"].c_str()))) o->data.skill[6].id = value;
				if (0!=(value=atoi(prop["slv7"].c_str()))) o->data.skill[6].point = value;

				if (0!=(value=atoi(prop["sid8"].c_str()))) o->data.skill[7].id = value;
				if (0!=(value=atoi(prop["slv8"].c_str()))) o->data.skill[7].point = value;

				if (0!=(value=atoi(prop["sid9"].c_str()))) o->data.skill[8].id = value;
				if (0!=(value=atoi(prop["slv9"].c_str()))) o->data.skill[8].point = value;

				if (0!=(value=atoi(prop["sid10"].c_str()))) o->data.skill[9].id = value;
				if (0!=(value=atoi(prop["slv10"].c_str()))) o->data.skill[9].point = value;

				//�����˫��������ߺ�����������Ҫ��
				o->checkBind();
				Cmd::stAddObjectPropertyUserCmd send;
				bcopy(&o->data,&send.object,sizeof(t_Object));
				pUser->sendCmdToMe(&send,sizeof(send));
				Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�õ���Ʒ%s%ld��",o->name,o->data.dwNum);
				ScenesService::gm_logger->trace("GM:%s ��������Ʒ %s %d ��", pUser->name, o->name, o->data.dwNum);
				return true;
			}
			zObject::destroy(o);
		}
		return false;
	}
	ScenesService::gm_logger->trace("GM:%s ��ͼ���첻���ڵ���Ʒ name=%s id=%s ", pUser->name, prop["name"].c_str(), prop["id"].c_str());
	return false;
}

/**
 * \brief �ٻ�npc
 *
 *
 * \param pUser: �û�
 * \param para:����
 * \return �õ��Ƿ�ɹ�
 */
bool Gm::summon(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	//prop.dump(std::cout);

	if ("" == prop["id"] && "" == prop["name"].c_str())
	{
		ScenesService::gm_logger->trace("GM:%s ʹ��summon�����������λ�� %s (%d,%d)", pUser->name, pUser->scene->name, pUser->getPos().x, pUser->getPos().y);
		return false;
	}
	zNpcB *base = npcbm.get(atoi(prop["id"].c_str()));
	if (NULL == base)
		base = npcbm.get(prop["name"].c_str());
	if (base)
	{
		t_NpcDefine define;
		zPos pos = pUser->getPos();
		define.id = base->id;
		strcpy(define.name, base->name);
		if ("" == prop["x"]
				|| "" == prop["y"])
		{
			define.pos = pUser->getPos();
		}
		else
		{
			define.pos.x = atoi(prop["x"].c_str());
			define.pos.y = atoi(prop["y"].c_str());
		}
		define.width = SCREEN_WIDTH;
		define.height = SCREEN_HEIGHT;
		define.num = atoi(prop["num"].c_str());
		define.num = std::max(define.num,(DWORD)1);
		define.num = std::min(define.num,(DWORD)1000);
		define.interval = 30;
		define.initstate = zSceneEntry::SceneEntry_Normal;
		define.scriptID = atoi(prop["script"].c_str());

		pUser->scene->zPosRevaluate(define.pos);
		define.pos -= zPosHalfScreen;
		pUser->scene->zPosRevaluate(define.pos);
		pos -= zPosHalfScreen;
		pUser->scene->zPosRevaluate(pos);
		pUser->scene->initRegion(define.region, define.pos, define.width, define.height);
		int num = pUser->scene->summonNpc(define, pos, base);

		ScenesService::gm_logger->trace("GM:%s �ٻ����� %s %d ����λ�� %s (%d,%d)", pUser->name, define.name, num, pUser->scene->name, define.pos.x, define.pos.y);
		return true;
	}

	ScenesService::gm_logger->trace("GM:%s ʹ��summon�����������λ�� %s (%d,%d)", pUser->name, pUser->scene->name, pUser->getPos().x, pUser->getPos().y);
	return false;
}

/*\brief GM���Լ�����
 * 
 * ����GM�������������������û�����
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::levelup(SceneUser *pUser,const char *para)
{
	if (0==pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	int num=atoi(prop["num"].c_str());
	if (num>MAX_LEVEL) num = MAX_LEVEL;
	if (num <=0) num = 1;

	//���Լ�����
	if (""==prop["name"] || !strncmp(pUser->name, prop["name"].c_str(), MAX_NAMESIZE))
	{
		if (pUser->upgrade(num))
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"��ϲ�㣡�����ˡ�");
			ScenesService::gm_logger->trace("GM %s ʹ��levelupָ� �������Լ��ĵȼ� num=%d", pUser->name, num);
			return true;
		}
		else
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"����ʧ��");
			ScenesService::gm_logger->trace("GM %s ʹ��levelupָ� �����Լ��ĵȼ�ʧ��, num=%d", pUser->name, num);
			return false;
		}
	}
   else//����������
   {
		//ֻ�г���GM�ſ��Ը�����������
		if (!(pUser->getPriv() & Gm::super_mode))
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"�㲻���������˵ĵȼ���Ȩ�޲���");
			ScenesService::gm_logger->trace("GM %s ʹ��levelupָ� ��ͼ���� %s �ĵȼ�, Ȩ�޲���", pUser->name, prop["name"].c_str());
			return false;
		}
		else
		{
			//û����������
			SceneUser * pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());

			if (pTarget)//�������
			{
				if (pUser->getPriv() <= pTarget->getPriv())
				{                       
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ֻ�ܲ������Լ�����͵����!");
					ScenesService::gm_logger->trace("GM %s ʹ����levelup�����ͼʹ %s ������Ȩ�޲���", pUser->name, prop["name"].c_str());                                    
					return false;
				}
				if (pTarget->upgrade(num))
				{
					Channel::sendSys(pTarget, Cmd::INFO_TYPE_GAME,"��ϲ��%s ��������ĵȼ� %d", pUser->name, num);
					Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"���� %s �ĵȼ� %d �ɹ�", pTarget->name, num);
					ScenesService::gm_logger->trace("GM %s ʹ��levelupָ� ������ %s �ĵȼ��� num=%d", pUser->name, prop["name"].c_str(), num);
					return true;
				}
				else
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"����ʧ��");
					ScenesService::gm_logger->trace("GM %s ʹ��levelupָ� ���� %s �ĵȼ�ʧ��, num=%d", pUser->name, prop["name"].c_str(), num);
					return false;
				}
			}
			else//��Ҳ����߻��ڵ�ǰ����������
			{
				Cmd::Session::t_gmCommand_SceneSession cmd;
				cmd.gm_cmd = Cmd::Session::GM_COMMAND_LEVELUP;
				cmd.src_priv = pUser->getPriv();
				cmd.x = num;//������Ҫ�����ĵȼ���
				strncpy((char *)cmd.src_name, pUser->name, MAX_NAMESIZE-1);
				strncpy((char *)cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE-1);

				sessionClient->sendCmd(&cmd, sizeof(cmd));
				return true;
			}
		}
	}

	return false;
}

/*\brief ��ͼ��ת
 * 
 *
 * \param pUser: �û�
 * \param para:����
 * \return ��ת�Ƿ�ɹ�
 */
bool Gm::gomap_Gm(SceneUser *pUser,const char *para)
{
	bool ignoreUserLevel = false;
	zProperties prop;
	prop.parseCmdLine(para);

	if ("true" == prop["ignoreUserLevel"])
	{
		ignoreUserLevel = true;
	}
	if ("" == prop["name"])
	{
		//		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"��ָ����ͼ�����꣬�磺//gomap name=XXX pos=100,100");
		ScenesService::gm_logger->trace("GM %s ʹ��gomap�����������", pUser->name);
		return false;
	}
	pUser->charbase.gomaptype=atoi(prop["type"].c_str());
	std::string name = prop["name"];
	if(strstr(prop["name"].c_str() , "��") == NULL)
	{
		SceneManager::CountryMap_iter country_iter
			=SceneManager::getInstance().country_info.find(pUser->scene->getCountryID());
		if(country_iter != SceneManager::getInstance().country_info.end() && !(country_iter->second.function &0x01)/*��������δ���Ź���*/ && pUser->scene->getCountryID()!=6)
		{
			name = country_iter->second.name;
			name+= "��";
			name+= prop["name"];
		}
		else
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "δ������");
			Zebra::logger->debug("%s(%u)��������δ���Ź���",pUser->name,pUser->id);
			return false;
		}
	}
	else
	{
		char country_name[MAX_NAMESIZE+1];
		bzero(country_name,sizeof(country_name));
		strncpy(country_name,name.c_str(),MAX_NAMESIZE);
		char *real = strstr(country_name , "��");
		if(real)
		{
			*real=0;
		}
		SceneManager::CountryMap_iter country_iter=SceneManager::getInstance().country_info.find(SceneManager::getInstance().getCountryIDByCountryName(country_name));
		if(country_iter != SceneManager::getInstance().country_info.end() && (country_iter->second.function &0x01)/*��������δ���Ź���*/ && country_iter->first!=6)
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "δ������");
			Zebra::logger->debug("%s(%u)��������δ���Ź���",pUser->name,pUser->id);
			return false;
		}
	}
	Scene * scene=SceneManager::getInstance().getSceneByName(name.c_str());

	zPos Pos;
	Pos.x = 0;
	Pos.y = 0;
	bool findPos=true;

	if (""!=prop["pos"])
	{
		zRegex regex;
		if((regex.compile("(.*)[ ,](.*)") && regex.match(prop["pos"].c_str())))
		{
			std::string s;
			regex.getSub(s,1);
			Pos.x=atoi(s.c_str());
			regex.getSub(s,2);
			Pos.y=atoi(s.c_str());
			findPos=false; 
		}
	}
	else if (""!=prop["cpos"])
	{
		zPos cPos;
		zRegex regex;
		if((regex.compile("(.*)[ ,](.*)") && regex.match(prop["cpos"].c_str())))
		{
			std::string s;
			regex.getSub(s,1);
			cPos.x=atoi(s.c_str());
			regex.getSub(s,2);
			cPos.y=atoi(s.c_str());
			int rlen = atoi(prop["rlen"].c_str());
			if(!rlen)
			{
				rlen=6;
			}
			Pos.x=zMisc::randBetween((cPos.x - rlen)>0?(cPos.x - rlen):0,cPos.x + rlen);
			Pos.y=zMisc::randBetween((cPos.y - rlen)>0?(cPos.y - rlen):0,cPos.y + rlen);
			findPos=false; 
		}
	}

	if(scene)
	{
		if ((!ignoreUserLevel)&&(!scene->checkUserLevel(pUser))&&(pUser->scene!=scene)) return false;

		if (Pos.x>scene->width()) Pos.x = scene->width();
		if (Pos.y>scene->height()) Pos.y = scene->height();
		if (!findPos) return pUser->changeMap(scene,Pos, ignoreUserLevel);

		const WayPoint *wp=pUser->scene->getWayPoint(scene->getFileName());
		if(wp==NULL)//û�д�pUser->scene��scene����ת��
		{
			wp=scene->getRandWayPoint();//�����һ��scene�ϵ���ת��
			if(wp)
			{
				const zPos *pos=wp->getRandPoint();
				bool suc = pUser->changeMap(scene,*pos, ignoreUserLevel);
				if (suc)
				{
					ScenesService::gm_logger->trace("GM %s ʹ��gomap���Ŀ�� %s (%d,%d)", pUser->name, scene->name, pos->x, pos->y);
					return true;
				}
				else
				{
					ScenesService::gm_logger->trace("GM %s ʹ��gomap�����ת��ͼʧ�ܣ�Ŀ�� %s (%d,%d)", pUser->name, scene->name, pos->x, pos->y);
					return false;
				}
			}
			else
			{
				//				Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"��ͼ��û����ת��");
				ScenesService::gm_logger->trace("GM %s ʹ��gomap�����ͼ %s û����ת��", pUser->name, name.c_str());
				return false;
			}
		}
		else
		{
			const zPos pos=wp->getRandDest()->pos;
			bool suc = pUser->changeMap(scene,pos, ignoreUserLevel);
			if (suc)
			{
				ScenesService::gm_logger->trace("GM %s ʹ��gomap���Ŀ�� %s (%d,%d)", pUser->name, scene->name, pos.x, pos.y);
				return true;
			}
			else
			{
				ScenesService::gm_logger->trace("GM %s ʹ��gomap�����ת��ͼʧ�ܣ�Ŀ�� %s (%d,%d)", pUser->name, scene->name, pos.x, pos.y);
				return false;
			}
		}
	}
	else
	{
		//ScenesService::gm_logger->debug("�л�����������(%s, %d, %d)", dp->name, dp->pos.x, dp->pos.y);
		if (pUser->guard && pUser->guard->canMove()) pUser->saveGuard = true;//ʹ�ڳ�����ָ��ʹ����
		if (pUser->adoptList.size()) pUser->saveAdopt = true;
		Cmd::Session::t_changeScene_SceneSession cmd;
		cmd.id = pUser->id;
		cmd.temp_id = pUser->tempid;
		cmd.x = Pos.x;
		cmd.y = Pos.y;
		cmd.map_id = 0;
		cmd.map_file[0] = '\0';
		strncpy((char *)cmd.map_name, name.c_str(), MAX_NAMESIZE);
		sessionClient->sendCmd(&cmd, sizeof(cmd));
		return true;
	}
}

/*\brief ��ͼ��ת
 * 
 *
 * \param pUser: �û�
 * \param para:����
 * \return ��ת�Ƿ�ɹ�
 */
bool Gm::gomap(SceneUser *pUser,const char *para)
{
	bool ignoreUserLevel = false;
	zProperties prop;
	prop.parseCmdLine(para);

	if ("true" == prop["ignoreUserLevel"])
	{
		ignoreUserLevel = true;
	}
	if ("" == prop["name"])
	{
		return false;
	}
	pUser->charbase.gomaptype=atoi(prop["type"].c_str());
	std::string name = prop["name"];
	if(strstr(prop["name"].c_str() , "��") == NULL)
	{
		SceneManager::CountryMap_iter country_iter
			=SceneManager::getInstance().country_info.find(pUser->scene->getCountryID());
		if(country_iter != SceneManager::getInstance().country_info.end() && !(country_iter->second.function &0x01)/*��������δ���Ź���*/ && pUser->scene->getCountryID()!=6)
		{
			name = country_iter->second.name;
			name+= "��";
			name+= prop["name"];
		}
		else
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "δ������");
			Zebra::logger->debug("%s(%u)��������δ���Ź���",pUser->name,pUser->id);
			return false;
		}
	}
	else
	{
		char country_name[MAX_NAMESIZE+1];
		bzero(country_name,sizeof(country_name));
		strncpy(country_name,name.c_str(),MAX_NAMESIZE);
		char *real = strstr(country_name , "��");
		if(real)
		{
			*real=0;
		}
		SceneManager::CountryMap_iter country_iter=SceneManager::getInstance().country_info.find(SceneManager::getInstance().getCountryIDByCountryName(country_name));
		if(country_iter != SceneManager::getInstance().country_info.end() && (country_iter->second.function &0x01)/*��������δ���Ź���*/ && country_iter->first!=6)
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "δ������");
			Zebra::logger->debug("%s(%u)��������δ���Ź���",pUser->name,pUser->id);
			return false;
		}
	}
	Scene * scene=SceneManager::getInstance().getSceneByName(name.c_str());

	zPos Pos;
	Pos.x = 0;
	Pos.y = 0;
	bool findPos=true;

	if (""!=prop["pos"])
	{
		zRegex regex;
		if((regex.compile("(.*)[ ,](.*)") && regex.match(prop["pos"].c_str())))
		{
			std::string s;
			regex.getSub(s,1);
			Pos.x=atoi(s.c_str());
			regex.getSub(s,2);
			Pos.y=atoi(s.c_str());
			findPos=false; 
		}
	}
	else if (""!=prop["cpos"])
	{
		zPos cPos;
		zRegex regex;
		if((regex.compile("(.*)[ ,](.*)") && regex.match(prop["cpos"].c_str())))
		{
			std::string s;
			regex.getSub(s,1);
			cPos.x=atoi(s.c_str());
			regex.getSub(s,2);
			cPos.y=atoi(s.c_str());
			int rlen = atoi(prop["rlen"].c_str());
			if(!rlen)
			{
				rlen=6;
			}
			Pos.x=zMisc::randBetween((cPos.x - rlen)>0?(cPos.x - rlen):0,cPos.x + rlen);
			Pos.y=zMisc::randBetween((cPos.y - rlen)>0?(cPos.y - rlen):0,cPos.y + rlen);
			findPos=false; 
		}
	}

	if(scene)
	{
		if ((!ignoreUserLevel)&&(!scene->checkUserLevel(pUser))&&(pUser->scene!=scene)) return false;

		if (Pos.x>scene->width()) Pos.x = scene->width();
		if (Pos.y>scene->height()) Pos.y = scene->height();
		if (!findPos) return pUser->changeMap(scene,Pos, ignoreUserLevel);

		const WayPoint *wp=pUser->scene->getWayPoint(scene->getFileName());
		if(wp==NULL)//û�д�pUser->scene��scene����ת��
		{
			wp=scene->getRandWayPoint();//�����һ��scene�ϵ���ת��
			if(wp)
			{
				const zPos *pos=wp->getRandPoint();
				return pUser->changeMap(scene,*pos, ignoreUserLevel);
			}
			else
				return false;
		}
		else
		{
			const zPos pos=wp->getRandDest()->pos;
			return pUser->changeMap(scene,pos, ignoreUserLevel);
		}
	}
	else
	{
		if (pUser->guard && pUser->guard->canMove()) pUser->saveGuard = true;//ʹ�ڳ�����ָ��ʹ����
		if (pUser->adoptList.size()) pUser->saveAdopt = true;
		Cmd::Session::t_changeScene_SceneSession cmd;
		cmd.id = pUser->id;
		cmd.temp_id = pUser->tempid;
		cmd.x = Pos.x;
		cmd.y = Pos.y;
		cmd.map_id = 0;
		cmd.map_file[0] = '\0';
		strncpy((char *)cmd.map_name, name.c_str(), MAX_NAMESIZE);
		sessionClient->sendCmd(&cmd, sizeof(cmd));
		return true;
	}
}

/*\brief ˲��
 * 
 *
 * \param pUser: �û�
 * \param para:����
 * \return �Ƿ�ɹ�
 */
bool Gm::goTo_Gm(SceneUser *pUser,const char *para)
{
	char para_1[MAX_CHATINFO + 1];
	bzero(para_1,sizeof(para_1));
	strncpy(para_1,para,MAX_CHATINFO);
	while(strlen(para_1) && para_1[strlen(para_1)-1] == 32)
	{
		para_1[strlen(para_1)-1] = 0;
	}
	zRegex regex;
	zPos newPos;
	//Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"���������(%ld,%ld),����������(265,290)",pUser->getPos().x,pUser->getPos().y);
	if((regex.compile("(.*)[ ,](.*)") && regex.match(para_1)))
	{
		std::string s;
		regex.getSub(s,1);
		newPos.x=atoi(s.c_str());
		regex.getSub(s,2);
		//newPos.y=atoi(trim(s.c_str()));
		newPos.y=atoi(s.c_str());
		if(newPos.x>pUser->scene->width() || newPos.y>pUser->scene->height()) return false;
	}
	else
	{
		ScenesService::gm_logger->trace("GM %s ʹ��goto����������� %s", pUser->name, para);
		return false;
	}
	bool suc = pUser->goTo(newPos);
	if (suc)
	{
		ScenesService::gm_logger->trace("GM %s ʹ��goto���Ŀ�� %s (%d,%d)", pUser->name, pUser->scene->name, newPos.x, newPos.y);
		return true;
	}
	else
	{
		ScenesService::gm_logger->trace("GM %s ʹ��goto����ʧ�ܣ�Ŀ�� %s (%d,%d)", pUser->name, pUser->scene->name, newPos.x, newPos.y);
		return false;
	}
}


/*\brief ˲��
 * 
 *
 * \param pUser: �û�
 * \param para:����
 * \return �Ƿ�ɹ�
 */
bool Gm::goTo(SceneUser *pUser,const char *para)
{
	char para_1[MAX_CHATINFO + 1];
	bzero(para_1,sizeof(para_1));
	strncpy(para_1,para,MAX_CHATINFO);
	while(strlen(para_1) && para_1[strlen(para_1)-1] == 32)
	{
		para_1[strlen(para_1)-1] = 0;
	}
	zRegex regex;
	zPos newPos;
	if((regex.compile("(.*)[ ,](.*)") && regex.match(para_1)))
	{
		std::string s;
		regex.getSub(s,1);
		newPos.x=atoi(s.c_str());
		regex.getSub(s,2);
		newPos.y=atoi(s.c_str());
		if(newPos.x>pUser->scene->width() || newPos.y>pUser->scene->height()) return false;
	}
	else
	{
		return false;
	}
	return pUser->goTo(newPos);
}

/*\brief ����������
 * 
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::upgradeSkill(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["id"])
	{
		ScenesService::gm_logger->trace("GM %s ʹ��upskill����������� para=%s", pUser->name, para);
		return false;
	}

	bool suc = pUser->upgradeSkill(atoi(prop["id"].c_str()));
	if (suc)
	{
		ScenesService::gm_logger->trace("GM %s ʹ��upskill���id=%s", pUser->name, prop["id"].c_str());
		return true;
	}
	else
	{
		ScenesService::gm_logger->trace("GM %s ʹ��upskill����ʧ�ܣ�id=%s", pUser->name, prop["id"].c_str());
		return false;
	}
}

/*\brief ��������
 * 
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::abandon(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["id"])
	{
		ScenesService::gm_logger->trace("GM %s ʹ��abandon����������� para=%s", pUser->name, para);
		return false;
	}

	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"�������(%s)", prop["id"].c_str());
	pUser->quest_list.abandon(*pUser, atoi(prop["id"].c_str()), true);
	ScenesService::gm_logger->trace("GM %s ʹ��abandon���ȡ������ id=%s", pUser->name, prop["id"].c_str());
	return true;
}

/**
 * \brief GM�õ�������ڵ�����λ��
 *
 * \param pUser: �û�
 * \param para:����
 * \return �õ��Ƿ�ɹ�
 */
bool Gm::finduser(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��finduser����������� para=%s", pUser->name, para);
		return false;
	}
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget)
	{					
		//�Ƚ�Ȩ�޼���
		if (pUser->getPriv() <= pTarget->getPriv())
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ֻ�ܲ������Լ�����͵����!");
			ScenesService::gm_logger->trace("GM %s ʹ��finduser�����ͼ�õ� %s ��λ�ã�Ȩ�޲���", pUser->name, prop["name"].c_str());
			return false;
		}

		zPos Pos=pTarget->getPos();
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"��� %s ������:%s ����:(%d,%d)",prop["name"].c_str(),pTarget->scene->getName(),Pos.x,Pos.y);
		ScenesService::gm_logger->trace("GM %s ʹ��finduser����õ� %s ��λ��", pUser->name, prop["name"].c_str());
		return true;
	}
	else
	{
		Cmd::Session::t_gmCommand_SceneSession cmd;
		bzero(cmd.src_name,MAX_NAMESIZE);
		bzero(cmd.dst_name,MAX_NAMESIZE);
		bzero(cmd.map_file,MAX_NAMESIZE);
		bzero(cmd.map_name,MAX_NAMESIZE);
		cmd.gm_cmd = Cmd::Session::GM_COMMAND_FINDUSER;
		cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
		cmd.id = pUser->id;
		cmd.src_priv = pUser->getPriv();
		strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
		return sessionClient->sendCmd(&cmd, sizeof(cmd));
	}
}

/**
 * \brief ��GM����һ��������
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::gotouser(SceneUser *pUser,const char *para)
{               
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��gotouser����������� para=%s", pUser->name, para);
		return false;
	}
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget)
	{
		//�Ƚ�Ȩ�޼���
		if (pUser->getPriv() <= pTarget->getPriv())
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "ֻ�ܲ������Լ�����͵����!");
			ScenesService::gm_logger->trace("GM %s ʹ��gotouser�����ͼ���� %s ��λ�ã�Ȩ�޲���", pUser->name, prop["name"].c_str());
			return false;
		}

		//λ��
		std::string mapName = "name=";
		mapName += pTarget->scene->getName();
		//����
		zPos Pos = pTarget->getPos();
		char position[MAX_NAMESIZE];
		bzero(position,sizeof(position));
		sprintf(position,"%d,%d",Pos.x,Pos.y);
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,position);

		//��ת
		gomap(pUser,mapName.c_str());
		goTo(pUser,position);

		ScenesService::gm_logger->trace("GM %s ʹ��gotouser������� %s ��λ��", pUser->name, prop["name"].c_str());
		return true;
	}       
	else    
	{
		Cmd::Session::t_gmCommand_SceneSession cmd;
		bzero(cmd.src_name,MAX_NAMESIZE);
		bzero(cmd.dst_name,MAX_NAMESIZE);
		bzero(cmd.map_file,MAX_NAMESIZE);
		bzero(cmd.map_name,MAX_NAMESIZE);
		cmd.gm_cmd = Cmd::Session::GM_COMMAND_GOTOUSER;
		cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
		cmd.id = pUser->id;
		cmd.src_priv = pUser->getPriv();
		strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
		return sessionClient->sendCmd(&cmd, sizeof(cmd));
	}
}

/**
 * \brief ����һ����ҹ�Ʊ
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::givestock(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��catchuser����������� para=%s", pUser->name, para);
		return false;
	}
	DWORD gold=atoi(prop["gold"].c_str());
	DWORD money=atoi(prop["money"].c_str());
	DWORD accid=atoi(prop["accid"].c_str());
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget && (accid == pTarget->charbase.accid) && (pUser->scene->getCountryID() == pTarget->charbase.country))
	{
		Cmd::Bill::t_Stock_Save save;
		save.dwMoney=money;
		save.dwGold=gold;
		pTarget->sendSceneCmdToBill(&save,sizeof(save));
		ScenesService::gm_logger->trace("GM %s ʹ��givestock��������%s(%d)������Ʊ���:%d,��Ʊ����:%d", pUser->name, pTarget->name,pTarget->id,gold,money);
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�����%s(%d)������Ʊ�����ɹ�",pTarget->name,pTarget->charbase.accid);
		return true;
	}
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "����Ҳ������Ҳ���ʧ��,ԭ��:1.��Ҳ�����;2.���ֺ�accid����Ӧ;3.����������ڹ���");
	return false;
}
/**
 * \brief ����һ���������
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::givemoney(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��catchuser����������� para=%s", pUser->name, para);
		return false;
	}
	DWORD num=atoi(prop["num"].c_str());
	DWORD accid=atoi(prop["accid"].c_str());
	if((strncmp(prop["type"].c_str(),"��Ʊ����",MAX_NAMESIZE)!=0))
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "����Ҳ������Ҳ���ʧ��,ԭ��:���Ͳ���ȷ");
		return false;
	}
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget && (accid == pTarget->charbase.accid) && (pUser->scene->getCountryID() == pTarget->charbase.country))
	{
		pTarget->packs.addMoney(num,prop["type"].c_str());
		ScenesService::gm_logger->trace("GM %s ʹ��givemoney��������%s(%d)��������:%d,����:%s", pUser->name, pTarget->name,pTarget->id,num,prop["type"].c_str());
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�����%s(%d)�������Ҳ����ɹ�",pTarget->name,pTarget->charbase.accid);
		return true;
	}
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "����Ҳ������Ҳ���ʧ��,ԭ��:1.��Ҳ�����;2.���ֺ�accid����Ӧ;3.����������ڹ���");
	return false;
}
/**
 * \brief ����һ����ҽ��
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::givegold(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��catchuser����������� para=%s", pUser->name, para);
		return false;
	}
	DWORD num=atoi(prop["num"].c_str());
	DWORD accid=atoi(prop["accid"].c_str());
	if((strncmp(prop["type"].c_str(),"��Ʊ����",MAX_NAMESIZE)!=0) && (strncmp(prop["type"].c_str(),"�㿨�����",MAX_NAMESIZE)!=0))
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "����Ҳ�����Ҳ���ʧ��,ԭ��:���Ͳ���ȷ");
		return false;
	}
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget && (accid == pTarget->charbase.accid) && (pUser->scene->getCountryID() == pTarget->charbase.country))
	{
		if(strncmp(prop["type"].c_str(),"�㿨�����",MAX_NAMESIZE)==0)
		{
			DWORD temp=num/300;
			if(temp)
			{
				pTarget->charbase.goldgive +=temp;
				Channel::sendSys(pTarget, Cmd::INFO_TYPE_GAME, "��õ���Ʒ����������ʯ%d��,����Ե�����������Ŷ",temp);
			}
		}
		pTarget->packs.addGold(num,prop["type"].c_str());
		ScenesService::gm_logger->trace("GM %s ʹ��givegold��������%s(%d)�������:%d,����:%s", pUser->name, pTarget->name,pTarget->id,num,prop["type"].c_str());
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�����%s(%d)������Ҳ����ɹ�",pTarget->name,pTarget->charbase.accid);
		return true;
	}
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "����Ҳ������Ҳ���ʧ��,ԭ��:1.��Ҳ�����;2.���ֺ�accid����Ӧ;3.����������ڹ���");
	return false;
}

/**
 * \brief ��һ����Ҵ���GM���
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::catchuser(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��catchuser����������� para=%s", pUser->name, para);
		return false;
	}
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget)
	{
		//�Ƚ�Ȩ�޼���
		if (pUser->getPriv() <= pTarget->getPriv())
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ֻ�ܲ������Լ�����͵����!");
			ScenesService::gm_logger->trace("GM %s ʹ��catchuser�����ͼ��׽ %s ��Ȩ�޲���", pUser->name, prop["name"].c_str());
			return false;
		}

		//��ͼ
		std::string mapName = "name=";
		mapName += pUser->scene->getName();
		//����
		zPos Pos = pUser->getPos();
		char position[MAX_NAMESIZE];
		bzero(position,sizeof(position));
		sprintf(position,"%d,%d",Pos.x,Pos.y);
		//Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,position);

		//����
		gomap(pTarget,mapName.c_str());
		goTo(pTarget,position);

		ScenesService::gm_logger->trace("GM %s ʹ��catchuser������� %s ��λ��", pUser->name, prop["name"].c_str());
		return true;
	}
	else
	{
		Cmd::Session::t_gmCommand_SceneSession cmd;
		bzero(cmd.src_name,MAX_NAMESIZE);
		bzero(cmd.dst_name,MAX_NAMESIZE);
		bzero(cmd.map_file,MAX_NAMESIZE);
		bzero(cmd.map_name,MAX_NAMESIZE);
		cmd.gm_cmd = Cmd::Session::GM_COMMAND_CATCHUSER;
		cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
		cmd.id = pUser->id;
		cmd.src_priv = pUser->getPriv();
		cmd.x = pUser->getPos().x;
		cmd.y = pUser->getPos().y;
		strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
		strncpy((char *)&cmd.map_name, pUser->scene->name, MAX_NAMESIZE);
		return sessionClient->sendCmd(&cmd, sizeof(cmd));
	}
}

/**
 * \brief ��һ����Ҷ����������
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::embar(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��catchuser����������� para=%s", pUser->name, para);
		return false;
	}
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget)
	{
		//�Ƚ�Ȩ�޼���
		if (pUser->getPriv() <= pTarget->getPriv())
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ֻ�ܲ������Լ�����͵����!");
			ScenesService::gm_logger->trace("GM %s ʹ��embar�����ͼ��׽ %s ��Ȩ�޲���", pUser->name, prop["name"].c_str());
			return false;
		}

		//��ͼ
		std::string mapName = "name=��� pos=65,54 ignoreUserLevel=true";
		//Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,position);

		//����
		pTarget->killAllPets();
		gomap(pTarget,mapName.c_str());

		ScenesService::gm_logger->trace("GM %s ʹ��embar������� %s ��λ��", pUser->name, prop["name"].c_str());
		return true;
	}
	else
	{
		Cmd::Session::t_gmCommand_SceneSession cmd;
		bzero(cmd.src_name,MAX_NAMESIZE);
		bzero(cmd.dst_name,MAX_NAMESIZE);
		bzero(cmd.map_file,MAX_NAMESIZE);
		bzero(cmd.map_name,MAX_NAMESIZE);
		cmd.gm_cmd = Cmd::Session::GM_COMMAND_CATCHUSER;
		cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
		cmd.id = pUser->id;
		cmd.src_priv = pUser->getPriv();
		cmd.x = 65;
		cmd.y = 54;
		strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
		strncpy((char *)&cmd.map_name, "���", MAX_NAMESIZE);
		return sessionClient->sendCmd(&cmd, sizeof(cmd));
	}
}


/**
 * \brief  ���ϵͳ����
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::goldsystem(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	if(!strcmp(prop["service"].c_str(),"start"))
	{
		Cmd::Scene::t_ServiceGold_GateScene ret;
		ret.byType=Cmd::Scene::SERVICE_START;
		Zebra::logger->debug("%s(%d)������ҷ���",pUser->name,pUser->id);
		SceneTaskManager::getInstance().broadcastCmd(&ret,sizeof(ret));
	}
	if(!strcmp(prop["service"].c_str(),"stop"))
	{
		Cmd::Scene::t_ServiceGold_GateScene ret;
		ret.byType=Cmd::Scene::SERVICE_STOP;
		Zebra::logger->debug("%s(%d)ֹͣ��ҷ���",pUser->name,pUser->id);
		SceneTaskManager::getInstance().broadcastCmd(&ret,sizeof(ret));
	}
	return true;
}
/**
 * \brief  ��Ʊϵͳ����
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::stocksystem(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	if(!strcmp(prop["service"].c_str(),"start"))
	{
		Cmd::Scene::t_ServiceStock_GateScene ret;
		ret.byType=Cmd::Scene::SERVICE_START;
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"������Ʊ����ɹ�");
		Zebra::logger->debug("%s(%d)������Ʊ����",pUser->name,pUser->id);
		SceneTaskManager::getInstance().broadcastCmd(&ret,sizeof(ret));
	}
	if(!strcmp(prop["service"].c_str(),"stop"))
	{
		Cmd::Scene::t_ServiceStock_GateScene ret;
		ret.byType=Cmd::Scene::SERVICE_STOP;
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"ֹͣ��Ʊ����ɹ�");
		Zebra::logger->debug("%s(%d)ֹͣ��Ʊ����",pUser->name,pUser->id);
		SceneTaskManager::getInstance().broadcastCmd(&ret,sizeof(ret));
	}
	return true;
}
/**
 * \brief  ��Ʊί�й���
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::stockconsign(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	if(!strcmp(prop["put"].c_str(),"true"))
	{
		Cmd::stPutListStockUserCmd ret;
		ret.dwNum=atoi(prop["num"].c_str());
		ret.dwPrice=atoi(prop["price"].c_str());
		ret.byType=atoi(prop["type"].c_str());
		pUser->sendCmdToBill(&ret , sizeof(ret));
	}
	if(!strcmp(prop["cancel"].c_str(),"true"))
	{
		Cmd::stRequestCancelListStockUserCmd ret;
		ret.id=atoi(prop["id"].c_str());
		ret.byType=atoi(prop["type"].c_str());
		pUser->sendCmdToBill(&ret , sizeof(ret));
	}
	if(!strcmp(prop["cancelall"].c_str(),"true"))
	{
		Cmd::t_CancelListAllStock_GateScene ret;
		pUser->sendCmdToBill(&ret , sizeof(ret));
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"��Ʊ���гɹ�");
		Zebra::logger->debug("%s(%d)��Ʊ����",pUser->name,pUser->id);
	}
	if(!strcmp(prop["history"].c_str(),"true"))
	{
		Cmd::stRequstHistoryStockUserCmd ret;
		pUser->sendCmdToBill(&ret , sizeof(ret));
	}
	if(!strcmp(prop["myhistory"].c_str(),"true"))
	{
		Cmd::stRequstSelfHistoryStockUserCmd ret;
		ret.begintime=atoi(prop["time"].c_str());
		ret.num=atoi(prop["num"].c_str());
		pUser->sendCmdToBill(&ret , sizeof(ret));
	}
	return true;
}
/**
 * \brief  ��Ʊ�ʽ����
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::stocktransfer(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	if(!strcmp(prop["query"].c_str(),"true"))
	{
		Cmd::stQueryFundStockUserCmd qfs;
		pUser->sendCmdToBill(&qfs , sizeof(qfs));
	}
	if(!strcmp(prop["save"].c_str(),"true"))
	{
		pUser->stockSave(atoi(prop["money"].c_str()),atoi(prop["gold"].c_str()));
		/*
		Cmd::stTransferFundStockSaveUserCmd tfs;
		tfs.dwGold=atoi(prop["gold"].c_str());
		tfs.dwMoney=atoi(prop["money"].c_str());
		pUser->sendCmdToBill(&tfs , sizeof(tfs));
		// */
	}
	else if(!strcmp(prop["fetch"].c_str(),"true"))
	{
		Cmd::stTransferFundStockFetchUserCmd tff;
		tff.dwGold=atoi(prop["gold"].c_str());
		tff.dwMoney=atoi(prop["money"].c_str());
		pUser->sendCmdToBill(&tff , sizeof(tff));
	}
	return true;
}
/**
 * \brief  ��Ʊ�û�
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::stockuser(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	Cmd::stPassowrdStockUserCmd ret;
	strncpy(ret.byPawword,prop["password"].c_str(),sizeof(ret.byPawword));
	strncpy(ret.byNew1,prop["new1"].c_str(),sizeof(ret.byNew1));
	strncpy(ret.byNew2,prop["new2"].c_str(),sizeof(ret.byNew2));
	pUser->sendCmdToBill(&ret , sizeof(ret));
	//pUser->gatetask->sendCmd(&ret , sizeof(ret));
	return true;
}
/**
 * \brief  �һ��¿�
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::redeemmonthcard(SceneUser *pUser,const char *para)
{
	Cmd::Scene::t_Request_RedeemCard rcg;
	rcg.dwUserID = pUser->id;
	pUser->gatetask->sendCmd(&rcg , sizeof(rcg));
	return true;
}
/**
 * \brief �һ���� 
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::redeemgold(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	DWORD point = atoi(prop["point"].c_str());
	Cmd::Scene::t_Request_RedeemGold rcg;
	rcg.dwUserID = pUser->id;
	rcg.dwNum = point>0?point:1;
	pUser->gatetask->sendCmd(&rcg , sizeof(rcg));
	return true;
}
/**
 * \brief  ��ѯ�¿��ͽ��
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::querygold(SceneUser *pUser,const char *para)
{
	Cmd::Scene::t_Request_Bill rcg;
	rcg.dwUserID = pUser->id;
	pUser->gatetask->sendCmd(&rcg , sizeof(rcg));
	return true;
}
/**
 * \brief ��ѯʣ��㿨��
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::querypoint(SceneUser *pUser,const char *para)
{
	Cmd::Scene::t_Request_Point rb;
	rb.dwUserID=pUser->id;
	pUser->gatetask->sendCmd(&rb , sizeof(rb));
	return true;
}
/**
 * \brief �Ӷ���
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::throwobject(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	DWORD type = atoi(prop["type"].c_str());
	
	if(type == 0) {
		pUser->packs.execEvery(&pUser->packs.main, Type2Type<DropFromPack>(), DropFromPack::Param(&pUser->packs.main, pUser->packs.main.size(), pUser->getPos()));
	}
	if(type == 1) {
		pUser->packs.execEvery(&pUser->packs.equip, Type2Type<DropFromPack>(), DropFromPack::Param(&pUser->packs.equip, pUser->packs.equip.size(), pUser->getPos()));
		if(pUser->packs.equip.needRecalc)
		{
			pUser->setupCharBase();
			Cmd::stMainUserDataUserCmd  userinfo;
			pUser->full_t_MainUserData(userinfo.data);
			pUser->sendCmdToMe(&userinfo,sizeof(userinfo));
			pUser->sendMeToNine();
		}
	}

#if 0
//TODOBYLQY
	zProperties prop;
	prop.parseCmdLine(para);

	DWORD type = atoi(prop["type"].c_str());
	std::vector<zObject *> temp_vec;
	int begin = 0;
	std::set<zObject *>::iterator iter;
	if(type == 0)
	{
		for(iter = pUser->packs.main.getAllset().begin(); iter != pUser->packs.main.getAllset().end() ; iter ++)
		{
			//TODO �������ɵ�����Ʒ
			if((*iter)->data.upgrade > 5 || (*iter)->data.bind || (*iter)->data.pos.y == Cmd::EQUIPCELLTYPE_PACKAGE || (*iter)->data.pos.y == Cmd::EQUIPCELLTYPE_MAKE || (*iter)->base->kind==ItemType_MASK)
			{
				continue;
			}
			temp_vec.push_back(*iter);
		}
		int mainpack = temp_vec.size();
		for(int i = begin; i < mainpack ;  i ++)
		{
			pUser->packs.moveObjectToScene(&*temp_vec[i] , pUser->getPos());
		}
	}
	if(type == 1)
	{
		bool needRecalc = false;
		begin = 0;
		temp_vec.clear();
		for(iter = pUser->packs.equip.getAllset().begin(); iter != pUser->packs.equip.getAllset().end() ; iter ++)
		{
			//TODO �������ɵ�����Ʒ
			if((*iter)->data.upgrade > 5 || (*iter)->data.bind || (*iter)->data.pos.y == Cmd::EQUIPCELLTYPE_PACKAGE || (*iter)->data.pos.y == Cmd::EQUIPCELLTYPE_MAKE )
			{
				continue;
			}
			temp_vec.push_back(*iter);
		}
		int	equippack = temp_vec.size();
		for(int i = begin; i < equippack ;  i ++)
		{
			pUser->packs.moveObjectToScene(&*temp_vec[i] , pUser->getPos());
			needRecalc = true;
		}
		if(needRecalc)
		{
			pUser->setupCharBase();
			Cmd::stMainUserDataUserCmd  userinfo;
			pUser->full_t_MainUserData(userinfo.data);
			pUser->sendCmdToMe(&userinfo,sizeof(userinfo));
			pUser->sendMeToNine();
		}
	}
#endif
	return true;
}
/**
 * \brief ���:mode:ģʽ,name:��Ա
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::team(SceneUser *pLeader,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pLeader, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��team����������� para=%s", pLeader->name, para);
		return false;
	}
	SceneUser *pUser=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pUser)
	{
		DWORD mode = atoi(prop["mode"].c_str());
		//�������ģʽ
		if(!pLeader->team.IsTeamed())
		{
			if(mode < Cmd::TEAM_MAX)
			{
				pLeader->team_mode=mode;
			}
			else
			{
				Zebra::logger->trace("�յ���Чģʽ�������Աָ��(%s , %ld)" , pLeader->name , pLeader->id);
				return false;
			}
		}
		else
		{
			if(pLeader->team_mode != mode)
			{
				Channel::sendSys(pLeader , Cmd::INFO_TYPE_FAIL, "һ������ֻ��ʹ��һ�����ģʽ");
				return false;
			}
		}
		if(pLeader->team_mode == Cmd::TEAM_HONOR) 
		{
			if(pLeader->charbase.level < TEAM_HONOR_LEADER_LEVEL)
			{
				Channel::sendSys(pLeader , Cmd::INFO_TYPE_FAIL, "����ģʽ��Ҫ�ӳ��ȼ�%d��",TEAM_HONOR_LEADER_LEVEL);
				return false;
			}
			if(pUser->charbase.level >= TEAM_HONOR_MEMBER_LEVEL)
			{
				Channel::sendSys(pLeader , Cmd::INFO_TYPE_FAIL, "����ģʽ��Ա�ȼ����ܴ���%d��",TEAM_HONOR_MEMBER_LEVEL-1);
				return false;
			}
		}
		if(!pUser->team.IsOpen())
		{
			Channel::sendSys(pLeader , Cmd::INFO_TYPE_FAIL, "�Է����δ����");
			return false;
		}
		if(pUser->team.IsTeamed())
		{
			Channel::sendSys(pLeader , Cmd::INFO_TYPE_FAIL, "�Է��������");
			return false;
		}
		
		if (pUser->issetUState(Cmd::USTATE_TOGETHER_WITH_TIGER)
		||  pUser->issetUState(Cmd::USTATE_TOGETHER_WITH_DRAGON))
		{
			Channel::sendSys(pLeader , Cmd::INFO_TYPE_FAIL, "�Է����ڻ���״̬���������");
			return false;
		}

		Cmd::stRequestTeamUserCmd ret;
		ret.dwAnswerUserID = pLeader->tempid;
		ret.dwTeamMode = pLeader->team_mode;
		pUser->sendCmdToMe(&ret , sizeof(ret));
	}
	return false;
}

/**
 * \brief GM��������㲥��Ϣ
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::bczone(SceneUser *pUser,const char *para)
{
	if ((NULL==pUser)||(NULL==para))
		return false;
	if (!strcmp(para,""))
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��bczone����������� para=%s", pUser->name, para);
		return false;
	}

	//��Ϣ
	Cmd::stChannelChatUserCmd send;
	zRTime ctv;
	send.dwType=Cmd::CHAT_TYPE_GM;
	send.dwChatTime = ctv.sec();
	strncpy((char *)send.pstrChat,para,MAX_CHATINFO-1);

	/*
	//�������
	if (pUser->mask.is_masking()) 
	{
	strncpy((char *)send.pstrName, "������",MAX_NAMESIZE);
	}
	else 
	 */
	{
		strncpy((char *)send.pstrName,pUser->name,MAX_NAMESIZE);
	}

	pUser->scene->sendCmdToScene(&send,sizeof(send));

	ScenesService::gm_logger->trace("GM %s ʹ��bczone���para=%s", pUser->name, para);
	return true;
}

/**
 * \brief GM��������㲥��Ϣ
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::bcworld(SceneUser *pUser,const char *para)
{
	if ((NULL==pUser)||(NULL==para))
		return false;
	if (""==para)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��bcworld����������� para=%s", pUser->name, para);
		return false;
	}

	//��Ϣ
	Cmd::stChannelChatUserCmd send;
	zRTime ctv;
	send.dwType = Cmd::CHAT_TYPE_GM;
	send.dwChatTime = ctv.sec();
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));
	strncpy((char *)send.pstrChat, para,MAX_CHATINFO-1);
	/*
	//�������
	if (pUser->mask.is_masking()) 
	{
	strncpy((char *)send.pstrName, "������", MAX_NAMESIZE);
	}
	else 
	 */
	{
		strncpy((char *)send.pstrName, pUser->name, MAX_NAMESIZE);
	}
	pUser->forwardSession(&send, sizeof(Cmd::stChannelChatUserCmd));

	ScenesService::gm_logger->trace("GM %s ʹ��bcworld���para=%s", pUser->name, para);
	return true;
}

/**
 * \brief GM��������㲥��Ϣ
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::bcwgamemsg(SceneUser *pUser,const char *para)
{
	if ((NULL==pUser)||(NULL==para))
		return false;
	if (""==para)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��bcwgamemsg����������� para=%s", pUser->name, para);
		return false;
	}

	//��Ϣ
	Cmd::stChannelChatUserCmd send;
	zRTime ctv;
	send.dwType = Cmd::CHAT_TYPE_GM;
	send.dwSysInfoType = Cmd::INFO_TYPE_EXP;
	send.dwChatTime = ctv.sec();
	bzero(send.pstrName, sizeof(send.pstrName));
	bzero(send.pstrChat, sizeof(send.pstrChat));
	strncpy((char *)send.pstrChat, para,MAX_CHATINFO-1);
	/*
	//�������
	if (pUser->mask.is_masking()) 
	{
	strncpy((char *)send.pstrName, "������", MAX_NAMESIZE);
	}
	else 
	 */
	{
		strncpy((char *)send.pstrName, pUser->name, MAX_NAMESIZE);
	}
	pUser->forwardSession(&send, sizeof(Cmd::stChannelChatUserCmd));

	ScenesService::gm_logger->trace("GM %s ʹ��bcwgamemsg���para=%s", pUser->name, para);
	return true;
}

/**
 * \brief GMʹһ����Ҳ��ܷ���
 * �磺//donttalk name=xwl time=86400
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::donttalk(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{       
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ���������");
		ScenesService::gm_logger->trace("GM %s ʹ��donttalk����������� para=%s", pUser->name, para);
		return false;
	}

	int delay = atoi(prop["time"].c_str());
	if (86400<delay)
		delay = 86400;//������1��
	if (0>=delay)
		delay = 600;//Ĭ�Ͻ���1Сʱ

	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget)
	{
		//�Ƚ�Ȩ�޼���
		if (pUser->getPriv() <= pTarget->getPriv())
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ֻ�ܲ������Լ�����͵����!");
			ScenesService::gm_logger->trace("GM %s ʹ��donttalk�����ͼʹ %s ���ԣ�Ȩ�޲���", pUser->name, prop["name"].c_str());
			return false;
		}
		pTarget->delayForbidTalk(delay);
		Channel::sendSys(pTarget, Cmd::INFO_TYPE_GAME,"�㱻 %s ���� %d ��", pUser->name, delay);
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"��� %s ������ %d ��", pTarget->name, delay);
		ScenesService::gm_logger->trace("GM %s ʹ��donttalk������ %s ���� %d ��", pUser->name, prop["name"].c_str(), delay);
		return true;
	}
	else    
	{
		Cmd::Session::t_gmCommand_SceneSession cmd;
		bzero(cmd.src_name,MAX_NAMESIZE);
		bzero(cmd.dst_name,MAX_NAMESIZE);
		bzero(cmd.map_file,MAX_NAMESIZE);
		bzero(cmd.map_name,MAX_NAMESIZE);
		cmd.gm_cmd = Cmd::Session::GM_COMMAND_DONTTALK;
		cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
		cmd.id = pUser->id;
		cmd.src_priv = pUser->getPriv();
		cmd.x = delay;
		cmd.y = 0;
		strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
		return sessionClient->sendCmd(&cmd, sizeof(cmd));
	}
}

/**
 * \brief GMʹһ����һָ�����
 * �磺//talk name=xwl
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::talk(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	SceneUser *pTarget=NULL;

	if ("" == prop["name"])
	{       
		/*                Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ���������");
				  ScenesService::gm_logger->trace("GM %s ʹ��talk����������� para=%s", pUser->name, para);
				  return false;*/
		pTarget = pUser;
	}
	else
	{
		pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	}

	if(pTarget)
	{
		//�Ƚ�Ȩ�޼���
		if ((pUser->getPriv() <= pTarget->getPriv()) && (pUser->getPriv()!=Gm::super_mode))
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ֻ�ܲ������Լ�����͵����!");
			ScenesService::gm_logger->trace("GM %s ʹ��talk�����ͼʹ %s �ָ����ԣ�Ȩ�޲���", pUser->name, prop["name"].c_str());
			return false;
		}

		if (pTarget->isForbidTalk())
		{
			pTarget->delayForbidTalk(0);
			Channel::sendSys(pTarget, Cmd::INFO_TYPE_GAME,"%s �������Ľ��ԣ����ڿ���˵����", pUser->name);
			ScenesService::gm_logger->trace("GM %s ʹ��talk��������� %s �Ľ���", pUser->name, prop["name"].c_str());
		}
		return true;
	}
	else    
	{
		Cmd::Session::t_gmCommand_SceneSession cmd;
		bzero(cmd.src_name,MAX_NAMESIZE);
		bzero(cmd.dst_name,MAX_NAMESIZE);
		bzero(cmd.map_file,MAX_NAMESIZE);
		bzero(cmd.map_name,MAX_NAMESIZE);
		cmd.gm_cmd = Cmd::Session::GM_COMMAND_TALK;
		cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
		cmd.id = pUser->id;
		cmd.src_priv = pUser->getPriv();
		cmd.x = 0;
		cmd.y = 0;
		strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
		return sessionClient->sendCmd(&cmd, sizeof(cmd));
	}
}


/**
 * \brief GM�ߵ�һ�����
 * �磺//kick name=xwl
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::kick(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{       
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ���������");
		ScenesService::gm_logger->trace("GM %s ʹ��kick����������� para=%s", pUser->name, para);
		return false;
	}
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if(pTarget && pUser)
	{       
		//�Ƚ�Ȩ�޼���
		if (pUser->getPriv() <= pTarget->getPriv())
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"�����߱��Լ��߼�������!");
			ScenesService::gm_logger->trace("GM %s ʹ��kick�����ͼ�� %s �߳���������Ȩ�޲���", pUser->name, prop["name"].c_str());
			return false;
		}

		//SceneUserManager::getMe().lock();
		Channel::sendSys(pTarget, Cmd::INFO_TYPE_FAIL,"���Ѿ���%s�߳���������",pUser->name);

		OnQuit event(1);
		EventTable::instance().execute(*pTarget, event);

		pTarget->save(Cmd::Record::LOGOUT_WRITEBACK);
		pTarget->killAllPets();

		//֪ͨsession������
		Cmd::Session::t_unregUser_SceneSession ret;
		ret.dwUserID=pTarget->id;
		ret.dwSceneTempID=pTarget->scene->tempid;
		ret.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
		sessionClient->sendCmd(&ret,sizeof(ret));
		/*
		//֪ͨsession������
		//Cmd::Session::t_unregUser_SceneSession ret;
		Cmd::Session::t_unregUser_GateSession ret;
		ret.dwUserID=pTarget->id;
		ret.dwSceneTempID=pTarget->scene->tempid;
		ret.retcode=Cmd::Session::UNREGUSER_RET_LOGOUT;
		sessionClient->sendCmd(&ret,sizeof(ret));
		 */

		//֪ͨ���ط�����
		Cmd::Scene::t_Unreg_LoginScene retgate;
		retgate.dwUserID = pTarget->id;
		retgate.dwSceneTempID = pTarget->scene->tempid;
		retgate.retcode = Cmd::Scene::UNREGUSER_RET_ERROR;
		pTarget->gatetask->sendCmd(&retgate,sizeof(retgate));

		pTarget->unreg();
		//SAFE_DELETE(pTarget);
		//SceneUserManager::getMe().unlock();
		/*
		 */

		ScenesService::gm_logger->trace("GM %s ʹ��kick����� %s �߳�������", pUser->name, prop["name"].c_str());
		return true;
	}
	else    
	{
		Cmd::Session::t_gmCommand_SceneSession cmd;
		bzero(cmd.src_name,MAX_NAMESIZE);
		bzero(cmd.dst_name,MAX_NAMESIZE);
		bzero(cmd.map_file,MAX_NAMESIZE);
		bzero(cmd.map_name,MAX_NAMESIZE);
		cmd.gm_cmd = Cmd::Session::GM_COMMAND_KICK;
		cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
		cmd.src_priv = pUser->getPriv();
		strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
		return sessionClient->sendCmd(&cmd, sizeof(cmd));
	}
}

bool Gm::kickGateUser(SceneUser *pUser,const char *para)
{
	if (!pUser || !para) return false;
	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Scene::t_kickUser_GateScene send;
	send.userID = atoi(prop["userid"].c_str());
	send.accid = atoi(prop["accid"].c_str());

	struct SceneTaskSendCmdExec :public execEntry<SceneTask>
	{       
		void *_data; 
		DWORD _size;
		SceneTaskSendCmdExec(void *data , DWORD size):_data(data),_size(size){} 
		bool exec(SceneTask *task)
		{       
			if(task)
			{       
				task->sendCmd(_data,_size);
			}       
			return true;
		}       
	};      
	SceneTaskSendCmdExec exec(&send , sizeof(Cmd::Scene::t_kickUser_GateScene)); 
	SceneTaskManager::getInstance().execEverySceneTask(exec);
	return true;
}

/**
 * \brief ��ʾ����
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::showPets(SceneUser *pUser,const char *para)
{
	if (0==pUser) return false;
	int count = 0;
	if (pUser->pet)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "����:%s id=%d pos=%d,%d", pUser->pet->name, pUser->pet->tempid, pUser->pet->getPos().x, pUser->pet->getPos().y);
		count++;
	}
	if (pUser->summon)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�ٻ���:%s id=%d pos=%d,%d", pUser->summon->name, pUser->summon->tempid, pUser->summon->getPos().x, pUser->summon->getPos().y);
		count++;
	}
	for (std::list<ScenePet *>::iterator it=pUser->totems.begin(); it!=pUser->totems.end(); it++)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "ͼ�ڣ�%s id=%d pos=%d,%d", (*it)->name, (*it)->tempid, (*it)->getPos().x, (*it)->getPos().y);
		count++;
	} 
	if (pUser->ridepet)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�ڳ�:%s id=%d pos=%d,%d", pUser->ridepet->name, pUser->ridepet->tempid, pUser->ridepet->getPos().x, pUser->ridepet->getPos().y);
		count++;
	}
	if (pUser->guard)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�ڳ�:%s id=%d pos=%d,%d", pUser->guard->name, pUser->guard->tempid, pUser->guard->getPos().x, pUser->guard->getPos().y);
		count++;
	}
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�� %d ��", count);
	return true;
}

/**
 * \brief ��ʾ��ͼ
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::showMaps(SceneUser *pUser,const char *para)
{
	struct showMapsExec : public SceneCallBack
	{
		SceneUser * pUser;
		showMapsExec(SceneUser * p)
		{
			pUser = p;
		}
		~showMapsExec(){}

		bool exec(Scene *s)
		{
			return Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, s->name);
		}
	};
	if (0==pUser) return false;

	showMapsExec sme(pUser);
	SceneManager::getInstance().execEveryScene(sme);
	return true;
}
/**
 * \brief ���ص�ͼ��ֻ�г���GM�ſ���ʹ��
 * �磺//loadmap name=˴������
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::loadMap(SceneUser *pUser,const char *para)
{
	if (0==pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["mapname"])
	{       
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ����ͼ����");
		ScenesService::gm_logger->trace("GM %s ʹ��loadmap����������� para=%s", pUser->name, para);
		return false;
	}
	//Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"��ָ��Ŀǰ������");
	DWORD countryid = 0;
	DWORD mapid = 0;
	if ("" == prop["countryname"])
	{
		SceneManager::CountryMap_iter country_iter
			=SceneManager::getInstance().country_info.find(pUser->scene->getCountryID());
		if(country_iter != SceneManager::getInstance().country_info.end())
		{
			countryid = pUser->scene->getCountryID();
		}
	}
	else
	{
		countryid = SceneManager::getInstance().getCountryIDByCountryName(prop["countryname"].c_str());
	}
	if(countryid == 0)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"����Ĺ������Ʋ���ȷ");
		return false;
	}

	mapid = SceneManager::getInstance().getMapIDByMapName(prop["mapname"].c_str());
	if(mapid == 0)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"����ĵ�ͼ���Ʋ���ȷ");
		return false;
	}
	//�ж��Ƿ��ظ�����
	Scene *loaded = SceneManager::getInstance().getSceneByID(SceneManager::getInstance().buildMapID(countryid , mapid));
	if(loaded)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�����ظ����ص�ͼ");
		return false;
	}

	DWORD serverid=atoi(prop["serverid"].c_str());
	if(!serverid || serverid == ScenesService::getInstance().getServerID())
	{
		loaded = SceneManager::getInstance().loadScene(Scene::STATIC, countryid , mapid);
		if(loaded)
		{
			using namespace Cmd::Session;
			ScenesService::gm_logger->info("����%s(%ld,%ld)�ɹ�",loaded->name,loaded->id,loaded->tempid);
			t_regScene_SceneSession regscene;
			regscene.dwCountryID = countryid;
			regscene.dwID=loaded->id;
			regscene.dwTempID=loaded->tempid;
			strncpy(regscene.byName,loaded->name,MAX_NAMESIZE);
			strncpy(regscene.fileName, loaded->getFileName(), MAX_NAMESIZE);
			sessionClient->sendCmd(&regscene,sizeof(regscene));
			ScenesService::gm_logger->trace("GM %s ʹ��loadmap������ص�ͼ %s��%s", 
					pUser->name, prop["countryname"].c_str() , prop["mapname"].c_str());
		}
	}
	else
	{
		using namespace Cmd::Session;
		t_reqAddScene_SceneSession ass;
		ass.dwServerID = serverid;
		ass.dwCountryID = countryid;
		ass.dwMapID = mapid;
		sessionClient->sendCmd(&ass,sizeof(ass));
		ScenesService::gm_logger->trace("GM %s ʹ��loadmap������ص�ͼ %s��%s", 
				pUser->name, prop["countryname"].c_str() , prop["mapname"].c_str());
	}

	// */
	return true;
}

/**
 * \brief ж�ص�ͼ��ֻ�г���GM�ſ���ʹ��
 * �磺//unloadmap name=˴������
 * 
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::unloadMap(SceneUser *pUser,const char *para)
{
	if (0==pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["mapname"])
	{       
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ����ͼ����");
		ScenesService::gm_logger->trace("GM %s ʹ��unloadmap����������� para=%s", pUser->name, para);
		return false;
	}
	DWORD countryid = 0;
	DWORD mapid = 0;
	if ("" == prop["countryname"])
	{
		SceneManager::CountryMap_iter country_iter
			=SceneManager::getInstance().country_info.find(pUser->scene->getCountryID());
		if(country_iter != SceneManager::getInstance().country_info.end())
		{
			countryid = pUser->scene->getCountryID();
		}
	}
	else
	{
		countryid = SceneManager::getInstance().getCountryIDByCountryName(prop["countryname"].c_str());
	}
	if(countryid == 0)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"����Ĺ������Ʋ���ȷ");
		return false;
	}

	mapid = SceneManager::getInstance().getMapIDByMapName(prop["mapname"].c_str());
	if(mapid == 0)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"����ĵ�ͼ���Ʋ���ȷ");
		return false;
	}
	ScenesService::gm_logger->trace("GM %s ʹ��unloadmap���ж�ص�ͼ %s", pUser->name, prop["mapname"].c_str());
	Cmd::Session::t_unloadScene_SceneSession ret;
	//strncpy((char*)ret.map_name , name.c_str() , MAX_NAMESIZE);
	ret.map_id = SceneManager::getInstance().buildMapID(countryid , mapid);
	sessionClient->sendCmd(&ret, sizeof(ret));
	return false;
}


/**
 * \brief �������Ȩ�ޣ�ֻ�г���GM�ſ�����
 * �磺//setpriv name=xwl priv=2
 * 
 * priv Ŀǰ��3��ֵ�� 0.�κ�ָ�����ִ�� 1.��ͨ���ָ�� 2.��ͨGMָ�� 3.����ָ���ִ��
 * 
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::setPriv(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{       
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ���������");
		ScenesService::gm_logger->trace("GM %s ʹ��setpriv����������� para=%s", pUser->name, para);
		return false;
	}
	SceneUser *pTarget=SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if (pUser==pTarget && (pUser->getPriv()!=Gm::super_mode))
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"���������Լ���");
		ScenesService::gm_logger->trace("GM %s ʹ��setpriv�����ͼ�����Լ���Ȩ��", pUser->name);
		return false;
	}

	int level = atoi(prop["priv"].c_str());
	if(pTarget && pUser)
	{       
		switch (level)
		{
			case 0:
				pTarget->setPriv(level);
				Channel::sendSys(pTarget, Cmd::INFO_TYPE_FAIL,"�㱻 %s ������������",pUser->name);
				break;
			case 1:
				pTarget->setPriv(level);
				Channel::sendSys(pTarget, Cmd::INFO_TYPE_SYS,"��ָ�����ͨ���״̬");
				break;
			case 2:
				if (pTarget->id>1000)
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"%s ���ܳ�ΪGM",pTarget->name);
					ScenesService::gm_logger->trace("GM %s ʹ��setpriv�����ͼ���� %s ��Ȩ�ޣ�id̫�� id=%d", pUser->name, prop["name"].c_str(), pTarget->id);
					return false;
				}
				pTarget->setPriv(level);
				Channel::sendSys(pTarget, Cmd::INFO_TYPE_SYS,"���ΪGM�ˣ�");
				break;
				/*
			case 3:
				{
					pTarget->setPriv(Gm::super_mode);
					Channel::sendSys(pTarget, Cmd::INFO_TYPE_SYS,"��ָ�������GM���״̬");
				}
				break;
				*/
			default:
				{
					Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"Ȩ�޲�������");
					ScenesService::gm_logger->trace("GM %s ʹ��setpriv����������� para=%s", pUser->name, para);
					return false;
				}
		}
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"���óɹ�");
		ScenesService::gm_logger->trace("GM %s ʹ��setpriv������� %s ��Ȩ�ޣ� priv=%d", pUser->name, prop["name"].c_str(), level);
		return true;
	}
	else    
	{
		Cmd::Session::t_gmCommand_SceneSession cmd;
		bzero(cmd.src_name,MAX_NAMESIZE);
		bzero(cmd.dst_name,MAX_NAMESIZE);
		bzero(cmd.map_file,MAX_NAMESIZE);
		bzero(cmd.map_name,MAX_NAMESIZE);
		cmd.gm_cmd = Cmd::Session::GM_COMMAND_SETPRIV;
		cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
		cmd.src_priv = pUser->getPriv();
		cmd.x = level;
		strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
		return sessionClient->sendCmd(&cmd, sizeof(cmd));
	}
}

/**
 * \brief �����Լ� 
 * ����֮���κ��˶��������ý�ɫ
 * 
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::hideMe(SceneUser *pUser,const char *para)
{
	if(pUser)
	{       
		pUser->setState(zSceneEntry::SceneEntry_Hide);

		//֪ͨ9��ɾ���ý�ɫ
		Cmd::stRemoveUserMapScreenUserCmd remove;
		remove.dwUserTempID=pUser->tempid;
		pUser->scene->sendCmdToNine(pUser->getPosI(),&remove,sizeof(remove));

		//�Ƴ�block
		pUser->scene->clearBlock(pUser->getPos());

		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"����ɹ�");
		ScenesService::gm_logger->trace("GM %s ʹ��hideme�������", pUser->name);
		return true;
	}
	return false;
}

/**
 * \brief ȡ������ 
 * �ָ��ɼ�״̬
 * 
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::showMe(SceneUser *pUser,const char *para)
{
	if(pUser)
	{       
		pUser->setState(zSceneEntry::SceneEntry_Normal);

		zPos curPos = pUser->getPos();
		pUser->goTo(curPos);
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�ָ����ɼ�״̬");
		ScenesService::gm_logger->trace("GM %s ʹ��showme����������", pUser->name);
		return true;
	}
	return false;
}

/**
 * \brief �޵�ģʽ 
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::god(SceneUser *pUser,const char *para)
{
	if(pUser)
	{       
		pUser->angelMode = true;

		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�����޵�ģʽ��");
		ScenesService::gm_logger->trace("GM %s ʹ��god������벻��ģʽ", pUser->name);
		return true;
	}
	return false;
}

/**
 * \brief ��ɱģʽ 
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::killer(SceneUser *pUser,const char *para)
{
	if(pUser)
	{       
		pUser->afreetMode = true;

		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�����ɱģʽ��");
		ScenesService::gm_logger->trace("GM %s ʹ��killer��������ɱģʽ", pUser->name);
		return true;
	}
	return false;
}
bool Gm::newzone(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	if(pUser)
	{       
		if(strncmp("true",prop["service"].c_str(),MAX_NAMESIZE)==0)
		{
			if(!SceneManager::getInstance().isNewZoneConfig())
			{
				SceneManager::getInstance().setNewZoneConfig(true);
				SceneManager::getInstance().addNewZonePos(151,211); 
				SceneManager::getInstance().addNewZonePos(250,175); 
				SceneManager::getInstance().addNewZonePos(212,330); 
				SceneManager::getInstance().addNewZonePos(159,343); 
			}
			else
			{
				SceneManager::getInstance().setNewZoneConfig(true);
			}
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�����������óɹ�");
			ScenesService::gm_logger->trace("GM %s ������������", pUser->name);
		}
		if(strncmp("false",prop["service"].c_str(),MAX_NAMESIZE)==0)
		{
			SceneManager::getInstance().setNewZoneConfig(false);
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"ȡ���������óɹ�");
			ScenesService::gm_logger->trace("GM %s ȡ����������", pUser->name);
		}
		DWORD x = atoi(prop["x"].c_str());
		DWORD y = atoi(prop["y"].c_str());
		if(x && y)
		{
			SceneManager::getInstance().addNewZonePos(x,y); 
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"���������������������:x=%d,y=%d",x,y);
			ScenesService::gm_logger->trace("GM %s ���������������������:x=%d,y=%d", pUser->name,x,y);
		}
		Cmd::Session::t_gmCommand_SceneSession send;
		send.gm_cmd=Cmd::Session::GM_COMMAND_NEWZONE;
		send.cmd_state=2;
		send.x=0;
		send.y=0;
		if(strncmp("true",prop["serviceall"].c_str(),MAX_NAMESIZE)==0)
		{
			send.cmd_state=1;
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"����ȫ���������óɹ�");
			ScenesService::gm_logger->trace("GM %s ������������", pUser->name);
		}
		if(strncmp("false",prop["serviceall"].c_str(),MAX_NAMESIZE)==0)
		{
			send.cmd_state=0;
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"ȡ��ȫ���������óɹ�");
			ScenesService::gm_logger->trace("GM %s ȡ����������", pUser->name);
		}
		DWORD xall = atoi(prop["xall"].c_str());
		DWORD yall = atoi(prop["yall"].c_str());
		if(xall && yall)
		{
			send.x=xall;
			send.y=yall;
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"���������������������:x=%d,y=%d",xall,yall);
			ScenesService::gm_logger->trace("GM %s ���������������������:x=%d,y=%d", pUser->name,xall,yall);
		}
		if((send.cmd_state != 2) || (send.x != 0 && send.y != 0))
		{
			sessionClient->sendCmd(&send, sizeof(send));
		}
		if(strncmp("true",prop["query"].c_str(),MAX_NAMESIZE)==0)
		{
			SceneManager::NewZoneVec &vec = SceneManager::getInstance().queryNewZonePos(); 
			SceneManager::newZoneVec_iter iter = vec.begin();
			for(;iter != vec.end() ; iter ++)
			{
				Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�����������������:x=%d,y=%d",(*iter).first,(*iter).second);
			}
		}
		return true;
	}
	return false;
}

/**
 * \brief �ָ���ͨģʽ 
 * ȡ���޵кͱ�ɱģʽ
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::normal(SceneUser *pUser,const char *para)
{
	if(pUser)
	{       
		pUser->angelMode = false;
		pUser->afreetMode = false;
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS,"�ָ���ͨģʽ");
		ScenesService::gm_logger->trace("GM %s ʹ��normal����ָ���ͨģʽ", pUser->name);
		return true;
	}
	return false;
}

/**
 * \brief �ص��������� 
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::goHome(SceneUser *pUser,const char *para)
{
	if(pUser)
	{       
		zXMLParser parser = zXMLParser();
		char str[MAX_NAMESIZE];
		bzero(str,MAX_NAMESIZE);
		if(parser.initFile(Zebra::global["sceneinfofile"].c_str()))
		{
			xmlNodePtr root = parser.getRootNode("ScenesInfo");
			if (0==root) return false;

			xmlNodePtr infoNode = parser.getChildNode(root,"countryinfo");
			if (0==infoNode) return false;

			xmlNodePtr countryNode = parser.getChildNode(infoNode,"country");
			while (countryNode)
			{
				unsigned int id=0;
				parser.getNodePropNum(countryNode, "id", &id, sizeof(id));
				if (id==pUser->charbase.country)
				{
					std::string s = "name=";
					parser.getNodePropStr(countryNode, "name", str, sizeof(str));
					s += str;
					s += "��";

					unsigned int cap_id;//���ǵ�mapID
					parser.getNodePropNum(countryNode, "mapID", &cap_id, sizeof(cap_id));

					infoNode = parser.getChildNode(root,"mapinfo");
					if (0==infoNode) return false;

					xmlNodePtr mapNode = parser.getChildNode(infoNode,"map");

					while (mapNode)
					{
						parser.getNodePropNum(mapNode, "mapID", &id, sizeof(id));
						if (id==cap_id)
						{
							parser.getNodePropStr(mapNode, "name", str, sizeof(str));
							s += str;
							s += " pos=";

							parser.getNodePropStr(mapNode, "fileName", str, sizeof(str));

							zXMLParser parser2 = zXMLParser();
							char mapFile[MAX_NAMESIZE];
							bzero(mapFile, sizeof(mapFile));
							sprintf(mapFile, "map/%s.xml",str);
							if(parser2.initFile(mapFile))
							{
								xmlNodePtr root2 = parser2.getRootNode("map");
								xmlNodePtr zoneNode = parser2.getChildNode(root2,"zonedef");
								int num;
								while (zoneNode)
								{
									parser2.getNodePropNum(zoneNode, "type", &num, sizeof(num));
									if (8==num)
									{
										parser2.getNodePropStr(zoneNode, "x", str, sizeof(str));
										s += str;
										s += ",";
										parser2.getNodePropStr(zoneNode, "y", str, sizeof(str));
										s += str;

										break;
									}
									zoneNode = parser2.getNextNode(zoneNode, "zonedef");;
								}
							}
							else
								ScenesService::gm_logger->trace("GM %s ʹ��gohomeָ��,�� %s �ļ�ʧ��", pUser->name, mapFile);
							break;
						}
						mapNode = parser.getNextNode(mapNode, "map");
					}

					gomap(pUser, s.c_str());

					//Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "���ǣ�country=%d, param=%s", pUser->charbase.country, s.c_str());
					ScenesService::gm_logger->trace("GM %s ʹ��gohome����������� param=%s ", pUser->name, s.c_str());
					return true;
				}
				countryNode = parser.getNextNode(countryNode, "country");
			}
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "δ�ҵ����ǣ�country=%d", pUser->charbase.country);
			ScenesService::gm_logger->trace("GM %s ʹ��gohome����ʧ�ܣ�û�ҵ����ǡ�countryID=%d", pUser->name, pUser->charbase.country);
		}
		return false;
	}
	return false;
}

/**
 * \brief ���ܲ���ָ��
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::skill(SceneUser *pUser,const char *para)
{
	if ((NULL==pUser)||(NULL==para))
		return false;
	if (""==para)
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���");
		ScenesService::gm_logger->trace("GM %s ʹ��skill����������� para=%s", pUser->name, para);
		return false;
	}

	zProperties prop;
	prop.parseCmdLine(para);

	if ("pkvalue" == prop["show"])
	{
		pUser->skillStatusM.showValue();
	}
	else if ("active" == prop["show"])
	{
		pUser->skillStatusM.showActive();
	}
	else if ("recovery" == prop["show"])
	{
		pUser->skillStatusM.showRecovery();
	}
	else if ("passiveness" == prop["show"])
	{
		pUser->skillStatusM.showPassiveness();
	}
	else if ("log" == prop["show"])
	{
		pUser->skillStatusM.showValueToLog();
	}



	if (!("" == prop["defencer"] ||
				"" == prop["magictype"] ||
				"" == prop["attacktype"]))
	{
		Cmd::stAttackMagicUserCmd send;

		SceneUser *defencer = SceneUserManager::getMe().getUserByName(prop["defencer"].c_str());
		if (defencer)
		{
			send.dwUserTempID = pUser->tempid;
			send.dwDefenceTempID = defencer->tempid;
			send.wdMagicType = atoi(prop["magictype"].c_str());
			send.xDes = (WORD)pUser->getPos().x;
			send.yDes = (WORD)pUser->getPos().y;
			send.byDirect = pUser->getDir();
			send.byAttackType = atoi(prop["attacktype"].c_str());
			send.byAction = 0;
			Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�ҹ�����%s",defencer->name);
			pUser->attackMagic(&send, sizeof(send));
			ScenesService::gm_logger->trace("GM %s ʹ��skill��������� %s ", pUser->name, defencer->name);
			return true;
		}
		else
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"��� %s ������",prop["defencer"].c_str());
			ScenesService::gm_logger->trace("GM %s ʹ��skill������ %s ������", pUser->name, prop["defencer"].c_str());
			return false;
		}
	}
	else    
	{
		ScenesService::gm_logger->trace("GM %s ʹ��skill����������� para=%s", pUser->name, para);
		return false;
	}
}


/**
 * \brief ����ҽ�����Ѫ�ȴ���
 * ����
 * //lockvalue name=member1 lock=mp
 * //lockvalue name=member1 unlock=all
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::lockValue(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	/*        if ("" == prop["name"])
		  {       
		  Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ���������");
		  ScenesService::gm_logger->trace("GM %s ʹ��lockvalue����������� para=%s", pUser->name, para);
		  return false;
		  }
	 */
	Cmd::Session::t_gmCommand_SceneSession cmd;
	cmd.err_code = 0;//������Ϣ
	bzero(cmd.src_name,MAX_NAMESIZE);
	bzero(cmd.dst_name,MAX_NAMESIZE);
	bzero(cmd.map_file,MAX_NAMESIZE);
	bzero(cmd.map_name,MAX_NAMESIZE);
	cmd.gm_cmd = Cmd::Session::GM_COMMAND_LOCKVALUE;
	cmd.cmd_state = Cmd::Session::GM_COMMAND_STATE_REQ;
	cmd.src_priv = pUser->getPriv();

	strncpy((char *)&cmd.src_name, pUser->name, MAX_NAMESIZE);

	if ("" == prop["name"])
	{
		strncpy((char*)&cmd.dst_name, pUser->name, MAX_NAMESIZE);
	}
	else
	{
		strncpy((char *)&cmd.dst_name, prop["name"].c_str(), MAX_NAMESIZE);
	}

	if ("mp" == prop["lock"])
	{
		cmd.x = 2;
	}
	else if ("hp" == prop["lock"])
	{
		cmd.x = 1;
	}
	else if ("sp" == prop["lock"])
	{
		cmd.x = 3;
	}
	else if ("all" == prop["lock"])
	{
		cmd.x = 4;
	}
	else
	{
		cmd.x = 5;
	}

	if ("mp" == prop["unlock"])
	{
		cmd.y = 2;
	}
	else if ("hp" == prop["unlock"])
	{
		cmd.y = 1;
	}
	else if ("sp" == prop["unlock"])
	{
		cmd.y = 3;
	}
	else if ("all" == prop["unlock"])
	{
		cmd.y = 4;
	}
	else
	{
		cmd.y = 5;
	}
	return sessionClient->sendCmd(&cmd, sizeof(cmd));

}


/**
 * \brief ģ�����ͶƱ
 * ����
 * //svote name=������
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::svote(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{       
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ����������");
		return false;
	}

	Cmd::stVoteListSeptCmd send1;
	pUser->forwardSession(&send1, sizeof(Cmd::stVoteListSeptCmd));

	Cmd::stVoteSeptCmd send;
	strncpy(send.septName,prop["name"].c_str(),MAX_NAMESIZE);

	pUser->forwardSession(&send, sizeof(Cmd::stVoteSeptCmd));
	return true;
}

/**
 * \brief ģ����ͶƱ
 * ����
 * //uvote name=���ڰ�
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::uvote(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ���������");
		return false;
	}

	Cmd::stVoteListUnionCmd send1;
	pUser->forwardSession(&send1, sizeof(Cmd::stVoteListUnionCmd));

	Cmd::stVoteUnionCmd send;
	strncpy(send.unionName,prop["name"].c_str(),MAX_NAMESIZE);

	pUser->forwardSession(&send, sizeof(Cmd::stVoteUnionCmd));
	return true;
}

/**
 * \brief ǿ�Ƽ�����ս
 * ����
 * //tong t1=���ڰ� t2=ħ�ް�...
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::tong(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["t1"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ���������");
		return false;
	}

	if ("" == prop["t2"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ�������ָ���������");
		return false;
	}


	Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "���ս״̬�Ѽ��");
	return true;

}

/**
 * \brief ��ʾ�ض�npc�ľ���λ����Ϣ
 *
 */
class ShowSpecialNpcCallBack : public specialNpcCallBack
{
	private:
		SceneUser * pUser;

	public:
		int count;
		ShowSpecialNpcCallBack(SceneUser * user):pUser(user),count(0){}
		bool exec(SceneNpc * npc)
		{
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "����NPC: %s λ��:%s (%u,%u)", npc->name, npc->scene->name, npc->getPos().x, npc->getPos().y);
			count++;
			return true;
		}
};

/**
 * \brief ��ʾ�ض�npc��λ����Ϣ
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::showSpecialNpc(SceneUser *pUser,const char *para)
{
	ShowSpecialNpcCallBack ssncb(pUser);
	SceneNpcManager::getMe().execAllSpecialNpc(ssncb);
	Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "�� %u ��", ssncb.count);
	return true;
}

/**
 * \brief ���ܳ���
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::killPets(SceneUser *pUser,const char *para)
{
	pUser->killAllPets();
	return true;
}
/**
 * \brief ����
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::createRush(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["id"])
	{       
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���������//createrush id=1 delay=60");
		ScenesService::gm_logger->trace("GM %s ʹ��createrush����������� para=%s", pUser->name, para);
		return false;
	}

	int id = atoi(prop["id"].c_str());
	int delay = atoi(prop["delay"].c_str());
	zXMLParser parser = zXMLParser();
	unsigned int countryID=0;
	if (""!=prop["country"])
	{
		char str[MAX_NAMESIZE];
		bzero(str,MAX_NAMESIZE);
		if(parser.initFile(Zebra::global["sceneinfofile"].c_str()))
		{
			xmlNodePtr root = parser.getRootNode("ScenesInfo");
			if (0==root) return false;

			xmlNodePtr infoNode = parser.getChildNode(root,"countryinfo");
			if (0==infoNode) return false;

			xmlNodePtr countryNode = parser.getChildNode(infoNode,"country");
			while (countryNode)
			{
				parser.getNodePropStr(countryNode, "name", str, sizeof(str));
				if (!strcmp(str, prop["country"].c_str()))
				{
					parser.getNodePropNum(countryNode, "id", &countryID, sizeof(countryID));
					break;
				}
				countryNode = parser.getNextNode(countryNode, "country");
			}
			if (!id)
			{
				ScenesService::gm_logger->trace("GM %s ʹ��createrushָ�δ�ҵ����� country=%s", pUser->name, prop["country"].c_str());
				return false;
			}
		}
		else
		{
			ScenesService::gm_logger->trace("GM %s ʹ��createrushָ��Ҳ����ļ� %s", pUser->name, Zebra::global["sceneinfofile"].c_str());
			return false;
		}
	}
	else
	{
		countryID = pUser->scene->getCountryID();
	}

	Rush * rush = new Rush(id, delay, countryID);
	if (rush)
	{
		if (rush->init())
		{
			ScenesService::gm_logger->trace("GM %s ʹ��createrush����, �� %s �������﹥��", pUser->name, rush->mapName);
			return true;
		}

		SAFE_DELETE(rush);
		ScenesService::gm_logger->trace("GM %s ʹ��createrush����, ��ʼ����������ʧ�� para=%s", pUser->name, para);
		return false;
	}

	ScenesService::gm_logger->trace("GM %s ʹ��createrush����, ��������ʧ�� para=%s", pUser->name, para);
	return false;
}

/**
 * \brief ����
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::createQuiz(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	Cmd::Session::t_createQuiz_SceneSession send;

	send.active_time  = atoi(prop["subjects"].c_str());
	send.ready_time   = atoi(prop["ready_time"].c_str());
	send.subject_type	  = atoi(prop["type"].c_str());
	send.type = 0;
	sessionClient->sendCmd(&send, sizeof(send));

	ScenesService::gm_logger->trace("GM %s ʹ��createquiz����, ����ȫ���������� para=%s", pUser->name, para);
	return true;
}

/**
 * \brief ���һ����ͼ��ĳ�������npc
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::clearWorld(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	int region = 0;
	if ("" == prop["region"])
		region = 5;
	else
		region = atoi(prop["region"].c_str());

	int side = 0;
	int direct = 0;
	int clockwise = 1;
	int clearCount = 0;
	int count = 0;
	zPos pos;
	pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, pos);
	do
	{
		SceneNpc * sceneNpc = pUser->scene->getSceneNpcByPos(pos);
		if (sceneNpc)
		{
			switch(sceneNpc->npc->kind)
			{
				case NPC_TYPE_NORMAL:
				case NPC_TYPE_BBOSS:
				case NPC_TYPE_LBOSS:
				case NPC_TYPE_PBOSS:
				case NPC_TYPE_BACKBONE:
				case NPC_TYPE_GOLD:
				case NPC_TYPE_SUMMONS:
				case NPC_TYPE_AGGRANDIZEMENT:
				case NPC_TYPE_ABERRANCE:
				case NPC_TYPE_BACKBONEBUG:
				case NPC_TYPE_GUARD:
				case NPC_TYPE_PET:
				case NPC_TYPE_TOTEM:
				case NPC_TYPE_HUMAN:
				case NPC_TYPE_WILDHORSE:
				case NPC_TYPE_DUCKHIT:
				case NPC_TYPE_BANNER:
				case NPC_TYPE_TRAP:
				case NPC_TYPE_UNIONGUARD:
				case NPC_TYPE_SOLDIER:
				case NPC_TYPE_TASK:
				case NPC_TYPE_TRADE:
					{
						if (!(sceneNpc->getMaster()==pUser))
						{
							sceneNpc->toDie(pUser->tempid);
							clearCount++;
						}
					}
					break;
				default:
					break;
			}
		}
		if (++count>=(region*2+1)*(region)*2+1)
			break;
	} while(pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, pos) && side <= region);
	ScenesService::gm_logger->trace("GM %s ʹ�� clearworld ���ɱ���˰뾶 %d ��Χ�ڵ� %d ֻNPC", pUser->name, region, clearCount);
	return true;
}

/**
 * \brief ���һ����ͼ��ĳ�������npc
 * 
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::clearArea(SceneUser *pUser,const char *para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	int region = 0;
	if ("" == prop["region"])
		region = 5;
	else
		region = atoi(prop["region"].c_str());

	int side = 0;
	int direct = 0;
	int clockwise = 1;
	int clearCount = 0;
	int count = 0;
	zPos pos;
	pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, pos);
	do
	{
		SceneUser * sceneUser = pUser->scene->getSceneUserByPos(pos);
		if (sceneUser)
		{
			if (sceneUser!=pUser)
			{
				sceneUser->goToRandomRect(sceneUser->getPos(), SCREEN_WIDTH*2+region, SCREEN_HEIGHT*2+region);
				sceneUser->catchMyPet();
				clearCount++;
			}
		}
		if (++count>=(region*2+1)*(region)*2+1)
			break;
	} while(pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, pos) && side <= region);
	ScenesService::gm_logger->trace("GM %s ʹ�� cleararea �����ɢ��%s(%u,%u)��Χ�����", pUser->name, pUser->scene->name, pUser->getPos().x, pUser->getPos().y);
	return true;
}

/**
 * \brief ���������������
 *
 *
 * \param pUser ָ��ʹ����
 * \param para ����
 * \return ָ��ִ���Ƿ�ɹ�
 */
bool Gm::clearSeptUnion(SceneUser *pUser,const char *para)
{
/*	Cmd::Session::t_ClearRelation_SceneSession send;
	send.dwUserID = pUser->id;

	sessionClient->sendCmd(&send, sizeof(send));
	*/

	Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"�������ѽ���");
	return false;
}

/**
 * \brief ����һ������
 *
 *
 * \param pUser ָ��ʹ����
 * \param para ����
 * \return ָ��ִ���Ƿ�ɹ�
 */
bool Gm::createFamily(SceneUser *pUser,const char *para)
{
	if ((!pUser)||(!para)) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���������//createfamily name=�ҵļ���");
		ScenesService::gm_logger->trace("GM %s ʹ��createfamily����������� para=%s", pUser->name, para);
		return false;
	}

	Cmd::Session::t_addSept_SceneSession send;

	send.dwMapTempID = pUser->scene->tempid;
	send.info.dwUnionID = 0;
	send.info.dwRepute = 0;
	send.info.dwSeptID = 0;			 			// �����ID
	strncpy(send.info.name,prop["name"].c_str(),MAX_NAMESIZE-1); 	// ��������
	send.info.dwCharID = pUser->charbase.id;			// �峤�Ľ�ɫID
	send.info.dwCountryID = pUser->charbase.country;		// ������������
	send.info.byVote = 0;						// Ĭ�Ͻ���ͶƱ�ڼ�
	strncpy(send.info.masterName,pUser->name,MAX_NAMESIZE-1);	// �᳤������
	send.info.dwSpendGold = 0;		// ���廨�ѽ��
	send.info.dwLevel = 1;		// ����ȼ�
	bzero(send.info.note, sizeof(send.info.note));
	sessionClient->sendCmd(&send, sizeof(send));
	return true;
}

/**
 * \brief ����һ�����
 *
 *
 * \param pUser ָ��ʹ����
 * \param para ����
 * \return ָ��ִ���Ƿ�ɹ�
 */
bool Gm::createUnion(SceneUser *pUser,const char *para)
{

	Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"�������ѽ���");
	/*
	if ((!pUser)||(!para)) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���������//createunion name=�ҵİ��");
		ScenesService::gm_logger->trace("GM %s ʹ��createunion����������� para=%s", pUser->name, para);
		return false;
	}

	Cmd::Session::t_addUnion_SceneSession send;

	send.dwMapTempID = pUser->scene->tempid;
	send.dwItemID 	 = 0;
	send.info.dwUnionID = 0;					// ����ID
	strncpy(send.info.name,prop["name"].c_str(),MAX_NAMESIZE-1);		// �������
	send.info.dwCharID = pUser->charbase.id;	                    	// �᳤�Ľ�ɫID
	send.info.dwCountryID = pUser->charbase.country;
	strncpy(send.info.masterName,pUser->name,MAX_NAMESIZE-1);	// �᳤������
	send.info.wdLevel = 1;     		    	       	        // ��ἶ��
	send.info.qwExp = 0;                              	  	// ��ᾭ��
	send.info.byVote = 0; //���ͶƱ��־
	sessionClient->sendCmd(&send, sizeof(send));
	*/
	return false;
}

/**
 * \brief ����һ������
 *
 *
 * \param pUser ָ��ʹ����
 * \param para ����
 * \return ָ��ִ���Ƿ�ɹ�
 */
bool Gm::createSchool(SceneUser *pUser,const char *para)
{
	if ((!pUser)||(!para)) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if ("" == prop["name"])
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL,"ȱ�ٲ���������//createunion name=�ҵİ��");
		ScenesService::gm_logger->trace("GM %s ʹ��createunion����������� para=%s", pUser->name, para);
		return false;
	}

	Cmd::Session::t_createSchool_SceneSession cmd;
	strncpy((char *)cmd.masterName, pUser->name, MAX_NAMESIZE-1);
	strncpy((char *)cmd.schoolName, prop["name"].c_str(), MAX_NAMESIZE-1);//Ҫ��������������

	sessionClient->sendCmd(&cmd, sizeof(cmd));
	return true;
}

/**
 * \brief �鿴���Ҷ�ս��Ϣ
 *
 *
 * \param pUser ָ��ʹ����
 * \param para ����
 * \return ָ��ִ���Ƿ�ɹ�
 */
bool Gm::viewCountryDare(SceneUser *pUser,const char *para)
{
	if ((!pUser)||(!para)) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_viewCountryDare_SceneSession send;
	if ("" == prop["type"])
		send.byType = 3;
	else
		send.byType = atoi(prop["type"].c_str());

	send.dwUserID = pUser->id;

	sessionClient->sendCmd(&send, sizeof(send));
	return true;
}

/**
 * \brief �鿴��Χ��npc�����
 *
 * \param pUser ָ��ʹ����
 * \param para ����
 * radius ���Ұ뾶
 * type ���ҵ����� 1:��� 2:npc 0:ȫ��
 * \return �Ƿ�ִ�гɹ�
 */
bool Gm::showEntries(SceneUser *pUser,const char *para)
{
	if ((!pUser)||(!para)) return false;
	zProperties prop;
	prop.parseCmdLine(para);

	int radius = atoi(prop["radius"].c_str());
	if (0==radius) radius=2;
	int type = atoi(prop["type"].c_str());

	int side = 0;
	int direct = 0;
	int clockwise = 1;
	int count = 0;
	int playerCount = 0,npcCount = 0;
	zPos curPos(0,0);
	pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, curPos);
	do
	{
		SceneEntryPk *entry;
		char str[MAX_CHATINFO];
		std::string state;
		if ((0==type)||(1==type))
		{
			entry = pUser->scene->getSceneUserByPos(curPos);
			if (entry)
			{
				switch (entry->getState())
				{
					case zSceneEntry::SceneEntry_Normal:
						state = "��ͨ";
						break;
					case zSceneEntry::SceneEntry_Death:
						state = "����";
						break;
					case zSceneEntry::SceneEntry_Hide:
						state = "����";
						break;
					default:
						state = "δ֪";
						break;
				}
				playerCount++;
				snprintf(str, MAX_CHATINFO-1,"(%u,%u) ��� %s %s", curPos.x, curPos.y, entry->name, state.c_str());
				Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, str);
			}
		}

		if ((0==type)||(2==type))
		{
			entry = pUser->scene->getSceneNpcByPos(curPos);
			if (entry)
			{
				switch (entry->getState())
				{
					case zSceneEntry::SceneEntry_Normal:
						state = "��ͨ";
						break;
					case zSceneEntry::SceneEntry_Death:
						state = "����";
						break;
					case zSceneEntry::SceneEntry_Hide:
						state = "����";
						break;
					default:
						state = "δ֪";
						break;
				}
				npcCount++;
				snprintf(str, MAX_CHATINFO-1,"(%u,%u) npc %s %s", curPos.x, curPos.y, entry->name, state.c_str());
				Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, str);
			}
		}
		if (++count>=(radius*2+1)*(radius*2+1)) break;
	} while(pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, curPos) && side <= radius);

	if (playerCount||npcCount)
		Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "%d ����� %d ��npc", playerCount, npcCount);

	ScenesService::gm_logger->trace("GM %s ʹ��showentries����(%u,%u) para=\"%s\"", pUser->name, pUser->getPos().x, pUser->getPos().y, para);

	return true;
	//Channel::sendNine(this, "?? %d ???", entries.size());
	//        return !entries.empty();
}

/*\brief ��ѯ��ɫ��С
 * 
 * ��ѯ��ɫ���ڴ�����ռ�Ŀռ��С
 *
 * \param pUser: �û�
 * \param para:����
 * \return �����Ƿ�ɹ�
 */
bool Gm::getsize(SceneUser *pUser,const char *para)
{
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME,"%s��ɫ��ռ�ռ�Ϊ%d",pUser->name, sizeof(*pUser));
	return true;
}

/**
 * \brief ���ó�����Ϊģʽ
 *
 *
 * \param pUser: �û�
 * \param para:����
 * \return �Ƿ����óɹ�
 */
bool Gm::setPetAI(SceneUser *pUser,const char *para)
{
	if ((!pUser)||(!para)) return false;
	zProperties prop;
	prop.parseCmdLine(para);

	int t;
	Cmd::petAIMode m=Cmd::PETAI_MOVE_STAND;
	t = atoi(prop["type"].c_str());
	if ("follow"==prop["mode"]) m = Cmd::PETAI_MOVE_FOLLOW;
	if ("stand"==prop["mode"]) m = Cmd::PETAI_MOVE_STAND;
	if ("active"==prop["mode"]) m = Cmd::PETAI_ATK_ACTIVE;
	if ("passive"==prop["mode"]) m = Cmd::PETAI_ATK_PASSIVE;
	if ("noatk"==prop["mode"]) m = Cmd::PETAI_ATK_NOATK;

	if (2==t) if (pUser->pet) pUser->pet->setPetAI(m);
	if (3==t) if (pUser->summon) pUser->summon->setPetAI(m);
	if (4==t)
	{
		for (std::list<ScenePet *>::iterator it=pUser->totems.begin(); it!=pUser->totems.end(); it++)
			(*it)->setPetAI(m);
	}
	ScenesService::gm_logger->trace("GM %s ʹ��setpetaiָ�����ó����ж���ʽ type=%d ai=%s",pUser->name, t, prop["mode"].c_str());
	return true;
}

/**
 * \brief ���ó���AIF
 *
 *
 * \param pUser: �û�
 * \param para:����
 * \return �Ƿ����óɹ�
 */
bool Gm::setPetAIF(SceneUser *pUser,const char *para)
{
	if ((!pUser)||(!para)) return false;
	zProperties prop;
	prop.parseCmdLine(para);

	int t = atoi(prop["aif"].c_str());

	if (pUser->pet) pUser->pet->aif = t;
	if (pUser->summon) pUser->summon->aif = t;
	return true;
}

bool Gm::bank(SceneUser* pUser, const char* para)
{
	if (!pUser || !para) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	BYTE page = atoi(prop["page"].c_str());
	BYTE day = atoi(prop["day"].c_str());

	if (page >= pUser->packs.store.days.size()) {
		pUser->packs.store.days.push_back(day);
	}else {
		pUser->packs.store.days[page] = day;
	}

	pUser->packs.store.notify(pUser);
	return true;
}

/**
 * \brief ���Ӿ���ֵ
 * ����10w
 *
 * \param pUser ʹ����
 * \param para ����
 * \return �Ƿ�ִ�гɹ�
 */
bool Gm::addExp(SceneUser* pUser, const char* para)
{
	if ((!pUser)||(!para)) return false;
	zProperties prop;
	prop.parseCmdLine(para);

	SceneUser * target = SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if (!target) target = pUser;
	int num = atoi(prop["num"].c_str());
	if (num<=0) return false;
	if (num>1000000000) num = 1000000000;
	target->addExp(num, true, 0, 0, true);
	return true;
}

/**
 * \brief ��ѯ�û��ʺ�
 * 
 *
 * \param pUser ʹ����
 * \param para ����
 * \return �Ƿ�ִ�гɹ�
 */
bool Gm::qAccount(SceneUser* pUser, const char* para)
{
	if ((!pUser)||(!para)) return false;
	zProperties prop;
	prop.parseCmdLine(para);

	SceneUser * target = SceneUserManager::getMe().getUserByName(prop["name"].c_str());
	if (!target) target = pUser;

	Cmd::Scene::t_Query_AccountScene send;
	send.dwUserID = pUser->id;
	send.dwDestID = target->id;

	pUser->gatetask->sendCmd(&send, sizeof(send));

	return true;
}

/**
 * \brief �����߳�sleep��ʱ��
 * 
 *
 * \param pUser ����GM
 * \param para ����
 * \return �Ƿ�ִ�гɹ�
 */
bool Gm::usleep(SceneUser* pUser, const char* para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	int uscene=atoi(prop["scene"].c_str());
	int ugate=atoi(prop["gate"].c_str());
	int uclient=atoi(prop["client"].c_str());
	if(uscene)
	{
		zTCPTaskPool::setUsleepTime(uscene);
		Zebra::logger->info("��������������usleepʱ��:%dus",uscene);
	}
	Cmd::Scene::t_Usleep_GateScene ret;
	if(ugate)
	{
		ret.utask=ugate;
	}
	if(uclient)
	{
		ret.uclient=uclient;
	}
	if(ugate || uclient)
	{
		if(strncmp("true",prop["all"].c_str(),MAX_NAMESIZE)==0)
		{
			SceneTaskManager::getInstance().broadcastCmd(&ret, sizeof(ret));
			if(ugate)
				Zebra::logger->info("�����������ط�����usleepʱ��:%dus",ugate);
			if(uclient)
				Zebra::logger->info("�����������ؿͻ���usleepʱ��:%dus",uclient);
		}
		else
		{
			pUser->gatetask->sendCmd(&ret, sizeof(ret));
			if(ugate)
				Zebra::logger->info("�����������ط�����usleepʱ��:%dus",ugate);
			if(uclient)
				Zebra::logger->info("�����������ؿͻ���usleepʱ��:%dus",uclient);
		}
	}
	return true;
}
/**
 * \brief �õ�ϵͳ��ǰʱ��
 * 
 *
 * \param pUser ����GM
 * \param para ����
 * \return �Ƿ�ִ�гɹ�
 */
bool Gm::systime(SceneUser* pUser, const char* para)
{
	struct tm tm_1;
	time_t timValue = time(NULL);
	zRTime::getLocalTime(tm_1, timValue);
	//tm_1=*localtime(&timValue);
	Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "ϵͳ��ǰʱ��%d��%d��%d��%dʱ%d��%d��",tm_1.tm_year+1900,tm_1.tm_mon+1,tm_1.tm_mday,tm_1.tm_hour,tm_1.tm_min,tm_1.tm_sec);
	return true;
}
/**
 * \brief ͣ��ά��
 * 
 *
 * \param pUser ����GM
 * \param para ����
 * \return �Ƿ�ִ�гɹ�
 */
bool Gm::shutdown(SceneUser* pUser, const char* para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	//zRegex regex;
	Cmd::Session::t_shutdown_SceneSession send;
	struct tm tm_1;
	time_t timValue = time(NULL);
	tm_1=*localtime(&timValue);
	//zRTime::getLocalTime(tm_1, timValue);
	//Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "ϵͳ��ǰʱ��%d��%d��%d��%dʱ%d��%d��",tm_1.tm_year+1900,tm_1.tm_mon+1,tm_1.tm_mday,tm_1.tm_hour,tm_1.tm_min,tm_1.tm_sec);
	tm_1.tm_year=atoi(prop["year"].c_str())!=0?atoi(prop["year"].c_str()):tm_1.tm_year;
	tm_1.tm_mon=atoi(prop["month"].c_str())!=0?atoi(prop["month"].c_str()):tm_1.tm_mon;
	tm_1.tm_mday=atoi(prop["day"].c_str())!=0?atoi(prop["day"].c_str()):tm_1.tm_mday;
	tm_1.tm_hour=atoi(prop["hour"].c_str())!=0?atoi(prop["hour"].c_str()):tm_1.tm_hour;
	tm_1.tm_min=atoi(prop["minute"].c_str());
	tm_1.tm_sec=atoi(prop["second"].c_str());
	send.time=mktime(&tm_1);
	if(send.time<=timValue)
	{
		send.time=0;
		 Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "ȡ��ͣ��ά��");
	}
	strncpy((char *)send.info, prop["info"].c_str(),MAX_CHATINFO-1);
	sessionClient->sendCmd(&send, sizeof(send));
	//Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "ϵͳ����%d��%d��%d��%dʱ%d��%d��ͣ��ά��",tm_1.tm_year+1900,tm_1.tm_mon+1,tm_1.tm_mday,tm_1.tm_hour,tm_1.tm_min,tm_1.tm_sec);
	return true;
}

/**
 * \brief �õ��������������������ֲ�״��
 * 
 *
 * \param pUser ʹ����
 * \param para ����
 * \return �Ƿ�ִ�гɹ�
 */
bool Gm::countServerUser(SceneUser* pUser, const char* para)
{
	struct SceneUserCountExec : public SceneCallBack
	{
		SceneUser *pUser;
		SceneUserCountExec(SceneUser *u):pUser(u) 
		{
		}
		bool exec(Scene *scene)
		{
			if (scene)
			{
				DWORD count = scene->countUser();
				Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "����%s��ǰ����:%d", scene->name, count);
			}
			return true;
		}
	};
	SceneUserCountExec esea(pUser); 
	SceneManager::getInstance().execEveryScene(esea);
	struct SceneTaskCountExec :public execEntry<SceneTask>
	{
		SceneUser *pUser;
		SceneTaskCountExec(SceneUser *u):pUser(u)
		{
		}
		bool exec(SceneTask *task)
		{
			if(task)
			{
				DWORD count = SceneUserManager::getMe().countUserByTask(task);
				Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, "����%u��ǰ����:%d", task->getID(), count);
			}
			return true;
		}
	};
	SceneTaskCountExec stce(pUser); 
	SceneTaskManager::getInstance().execEverySceneTask(stce);

	return true;
}

#include "QuestAction.h"
bool Gm::setQuest(SceneUser* pUser, const char* para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	zRegex regex;

	std::string var_name  = prop["var"];
	std::string value = prop["value"];
	int id = atoi(prop["id"].c_str());
	int type = atoi(prop["type"].c_str());

	std::string info = "Error Type!";
	Vars* vars = NULL;
	if (1)
	{
		switch (type) 
		{
			case 0:
				info = pUser->quest_list.info();
				vars = pUser->quest_list.vars(id);
				break;
			case 1:
				info = GlobalVar::instance().show();
				vars = GlobalVar::instance().add(id);
				break;
			case 2:
				info = TongVar::instance().show();
				vars = TongVar::instance().add(id);
				break;
			case 3:
				info = FamilyVar::instance().show();
				vars = FamilyVar::instance().add(id);
				break;
			case 4:
				info = UsersVar::instance().show();
				vars = UsersVar::instance().add(id);
				break;
		}		
		if (var_name.empty() || value.empty()) {
			Channel::sendSys(pUser, Cmd::INFO_TYPE_SYS, info.c_str());
			return false;
		}
	}


	bool new_vars = false;
	if (!vars) {
		vars = new Vars;
		new_vars = true;
	}

	Op::Set<int> op;
	vars->set_value(op, var_name,  atoi(prop["value"].c_str()), 0, pUser);
	if (type == 0) {
		vars->notify(*pUser, var_name);
		pUser->quest_list.add_quest(id, *vars, *pUser, false);
		pUser->quest_list.update(*pUser, true);
	}

	if (new_vars) SAFE_DELETE(vars);

	return true;
}

bool Gm::setBlock(SceneUser* pUser, const char* para)
{
	int radius = 2;

	int side = 0;
	int direct = 0;
	int clockwise = 1;
	int count = 0;
	zPos curPos(0,0);
	pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, curPos);
	do
	{
		pUser->scene->setBlock(curPos);
		if (++count>=(radius*2+1)*(radius*2+1)) break;
	} while(pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, curPos) && side <= radius);
	ScenesService::gm_logger->debug("GM %s ʹ��clearblock��������赲", pUser->name);

	return true;
}

bool Gm::checkBlock(SceneUser* pUser, const char* para)
{
	int radius = 2;

	int side = 0;
	int direct = 0;
	int clockwise = 1;
	int count = 0;
	int blockCount = 0;
	zPos curPos(0,0);
	pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, curPos);
	do
	{
		if (pUser->scene->checkBlock(curPos))
			blockCount++;
		if (++count>=(radius*2+1)*(radius*2+1)) break;
	} while(pUser->scene->getNextPos(side, direct, pUser->getPos(), clockwise, curPos) && side <= radius);
	Channel::sendSys(pUser,Cmd::INFO_TYPE_FAIL, "��Χ2������ %d ���赲��", blockCount);

	return true;
}

bool Gm::npcDare(SceneUser *pUser, const char * para)
{
	Cmd::Session::t_NpcDare_GMCmd_SceneSession send;
	sessionClient->sendCmd(&send, sizeof(send));
	ScenesService::gm_logger->debug("GM %s ʹ��npcdare�����������NPC����ս", pUser->name);
	return true;
}

bool Gm::clearPoint(SceneUser *pUser, const char * para)
{
	if (pUser)
	{
				pUser->charbase.points =pUser->charbase.points
						+pUser->charbase.wdProperty[0]
						+pUser->charbase.wdProperty[1]
						+pUser->charbase.wdProperty[2]
						+pUser->charbase.wdProperty[3]
						+pUser->charbase.wdProperty[4];
				pUser->charbase.wdProperty[0]=0;
				pUser->charbase.wdProperty[1]=0;
				pUser->charbase.wdProperty[2]=0;
				pUser->charbase.wdProperty[3]=0;
				pUser->charbase.wdProperty[4]=0;
				pUser->setupCharBase();
				//ˢ���û�����
				Cmd::stMainUserDataUserCmd  userinfo;
				pUser->full_t_MainUserData(userinfo.data);
				pUser->sendCmdToMe(&userinfo,sizeof(userinfo));
				ScenesService::gm_logger->debug("GM %s ʹ��clearpoint���Լ����������Ե�", pUser->name);
	}
	return true;
}

bool Gm::clearSkillPoint(SceneUser *pUser, const char * para)
{
	if (pUser)
	{
				pUser->charbase.skillpoint = pUser->charbase.level;
				pUser->usm.clear();
				Cmd::stClearSkillUserCmd send;
				pUser->sendCmdToMe(&send,sizeof(send));
				pUser->skillValue.init();
				pUser->setupCharBase();
				//ˢ���û�����
				Cmd::stMainUserDataUserCmd  userinfo;
				pUser->full_t_MainUserData(userinfo.data);
				pUser->sendCmdToMe(&userinfo,sizeof(userinfo));
				ScenesService::gm_logger->debug("GM %s ʹ��clearskillpoint���Լ��ļ��ܵ�", pUser->name);
	}
	return true;
}

bool Gm::clearSkill(SceneUser *pUser, const char * para)
{
	if (pUser)
	{
		zProperties prop;
		prop.parseCmdLine(para);
		if (prop["id"]!="")
		{
			DWORD skillid = atoi(prop["id"].c_str());

			pUser->usm.clearskill(skillid);
			Cmd::stClearSkillUserCmd send;
			pUser->sendCmdToMe(&send,sizeof(send));
			pUser->usm.refresh();
			pUser->skillValue.init();
			pUser->setupCharBase();
				//ˢ���û�����
			Cmd::stMainUserDataUserCmd  userinfo;
			pUser->full_t_MainUserData(userinfo.data);
			pUser->sendCmdToMe(&userinfo,sizeof(userinfo));
			ScenesService::gm_logger->debug("GM %s ʹ��clearskill���Լ���ָ������", pUser->name);
		}
	}
	return true;
}


bool Gm::studySkill(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	if (pUser)
	{
		std::string skill_name  = prop["name"];
		if (!skill_name.empty())
		{
			zSkillB * skillb = skillbm.get(skill_name.c_str());
			if (skillb)
			{
                pUser->upgradeSkill(skillb->skillid);
			}
		}
	}
	return true;
}

bool Gm::queryincmap(SceneUser *pUser, const char * para)
{
	zObject *obj = pUser->packs.equip.getObjectByEquipPos(Cmd::EQUIPCELLTYPE_ADORN);
	if(obj && obj->base->kind == ItemType_Amulet)
	{
		char buf[1024];
		bzero(buf,sizeof(buf));
		Cmd::stScrollMapPropertyUserCmd *smp = (Cmd::stScrollMapPropertyUserCmd*)buf;
		smp->qwThisID = obj->data.qwThisID;
		constructInPlace(smp);
		smp->size = pUser->scene->getIncCity((const char*)smp->mapname);
		pUser->sendCmdToMe(smp, sizeof(Cmd::stScrollMapPropertyUserCmd) + MAX_NAMESIZE * smp->size);				
	}
	return true;
}
bool Gm::clearColddown(SceneUser *pUser, const char * para)
{
	if (pUser)
	{
		pUser->reSendOther=true;
		pUser->usm.clearAllUseTime();
	}
	return true;
}

bool Gm::debugCityDare(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_GMCreateUnionCity_SceneSession send;
	send.byOpType = atoi(prop["state"].c_str());
	send.dwCityID = pUser->scene->getRealMapID();
	send.dwCountryID = pUser->charbase.country;
	sessionClient->sendCmd(&send, sizeof(send));

	return true;
}

bool Gm::debugEmperorDare(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_beginEmperorDare_SceneSession send;
	sessionClient->sendCmd(&send, sizeof(send));

	return true;
}

bool Gm::changeCountry(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	is_cancel_country = atoi(prop["state"].c_str());
	cancel_country_need_money = atoi(prop["money"].c_str());

	return true;
}

bool Gm::setSeptLevel(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	Cmd::Session::t_OpLevel_SceneSession send;
	
	send.dwLevel = atoi(prop["value"].c_str());
	send.dwSeptID = pUser->charbase.septid;
	sessionClient->sendCmd(&send, sizeof(send));

	return true;
}

bool Gm::setAllyFriendDegree(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);
	Cmd::Session::t_opAllyFrienddegree_SceneSession send;
	
	send.friendDegree = atoi(prop["value"].c_str());
	send.dwCountryID = pUser->charbase.country;
	sessionClient->sendCmd(&send, sizeof(send));
	
	ScenesService::gm_logger->trace("GM %s ʹ��setallyfd������� %s ���������Ѻö�", 
			pUser->name, prop["value"].c_str());                                    

	return true;
}

bool Gm::setRepute(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_OpRepute_SceneSession send;
	send.dwRepute = atoi(prop["value"].c_str());
	send.dwSeptID = pUser->charbase.septid;
	sessionClient->sendCmd(&send, sizeof(send));

	return true;
}

bool Gm::debugVote(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_opTechVote_SceneSession send;
	send.byOpType = atoi(prop["state"].c_str());
	send.dwCountryID = pUser->charbase.country;
	sessionClient->sendCmd(&send, sizeof(send));
	
	return true;
}

bool Gm::debugGem(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_OpGemState_SceneSession send;
	send.dwState = atoi(prop["state"].c_str());
	sessionClient->sendCmd(&send, sizeof(send));

	return true;
}

bool Gm::addSeptExp(SceneUser *pUser, const char * para)
{
	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_opTechVote_SceneSession send;
	sessionClient->sendCmd(&send, sizeof(send));

	return true;
}

bool Gm::checkQuest(SceneUser *pUser, const char * para)
{
	if (pUser)
		EventManager<OnVisit>::instance().get_valid_quest_str(*pUser);
	return true;
}

/*
 * \brief ������Ϸ���ܵĿ���
 * \param pUser ָ��ʹ����
 * \param para ����
 *
 * \return ִ�н��
 *
 */
bool Gm::setService(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;
	using namespace Cmd::Session;

	zProperties prop;
	if (0==prop.parseCmdLine(para))
	{
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "��ǰ����״̬ flag=%s", Zebra::global["service_flag"].c_str());
		return true;
	}

	DWORD f = atoi(Zebra::global["service_flag"].c_str());
	if (prop["mail"]=="on")
		f |= SERVICE_MAIL;
	else if (prop["mail"]=="off")
		f &= ~SERVICE_MAIL;

	if (prop["auction"]=="on")
		f |= SERVICE_AUCTION;
	else if (prop["auction"]=="off")
		f &= ~SERVICE_AUCTION;

	if (prop["process"]=="on")
		f |= SERVICE_PROCESS;
	else if (prop["process"]=="off")
		f &= ~SERVICE_PROCESS;

	if (prop["horse"]=="on")
		f |= SERVICE_HORSE;
	else if (prop["horse"]=="off")
		f &= ~SERVICE_HORSE;

	if (prop["flag"]!="")
		f = atoi(prop["flag"].c_str());

	/*
	char buf[32];
	bzero(buf, sizeof(buf));
	snprintf(buf, 32, "%u", f);
	Zebra::global["service_flag"] = buf;
	*/

	Cmd::Session::t_SetService_SceneSession send;
	send.flag = f;
	sessionClient->sendCmd(&send, sizeof(send));
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "��ǰ����״̬ flag=%u", f);
	ScenesService::gm_logger->trace("GM %s ���÷��� flag=%u", pUser->name, f);

	return true;
}

/*
 * \brief ������Ϸ���ܵĿ���
 * \param pUser ָ��ʹ����
 * \param para ����
 *
 * \return ִ�н��
 *
 */
bool Gm::setTire(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if (prop["oper"]=="clear")
	{
		if (prop["hour"] != "")
		{
			int tm_hour = atoi(prop["hour"].c_str());
			if (tm_hour<0 || tm_hour>23) return true;
			for(int i = 0; i < 60; i++)
			{
				int j = (tm_hour*60+i)/5;
				Cmd::clear_state(((unsigned char *)(pUser->charbase.tiretime)),j);
			}
		}
		else
		{
			bzero(pUser->charbase.tiretime, 36);
			pUser->wdTire = 0;
		}
	}
	else if (prop["oper"]=="set")
	{
		if (prop["hour"] != "")
		{
			int tm_hour = atoi(prop["hour"].c_str());
			if (tm_hour<0 || tm_hour>23) return false;
			for(int i = 0; i < 60; i++)
			{
				int j = (tm_hour*60+i)/5;
				Cmd::set_state(((unsigned char *)(pUser->charbase.tiretime)),j);
			}
		}
		else
		{
			for(int x = 0; x < 24; x++)
			for(int i = 0; i < 60; i++)
			{
				int j = (x*60+i)/5;
				Cmd::set_state(((unsigned char *)(pUser->charbase.tiretime)),j);
			}
		}
	}
	int count=0;
	for(int k=0; k<288;k++)
	{
		if (Cmd::isset_state(((unsigned char *)(pUser->charbase.tiretime)), k))
		{
			count++;
		}
	}
	pUser->wdTire=count;
	pUser->callProcessTire();
	return true;
}

/*
 * \brief ���ÿ���/�ر�ĳ����ע��
 * \param pUser ָ��ʹ����
 * \param para ����
 *
 * \return ִ�н��
 *
 */
bool Gm::enableRegister(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if (prop["countryid"]!="")
	{
		WORD wdCountryID = atoi(prop["countryid"].c_str());
		if (prop["status"]=="true")
		{
			Cmd::Scene::t_ChangeCountryStatus send;
			send.oper = Cmd::Scene::ENABLE_REGISTER;
			send.country = wdCountryID;
			send.dwUserID = pUser->id;
			SceneTaskManager::getInstance().broadcastCmd(&send,sizeof(send));
			ScenesService::gm_logger->trace("GM %s �����˹���%uע������", pUser->name, wdCountryID);
		}
		else if (prop["status"]=="false")
		{
			Cmd::Scene::t_ChangeCountryStatus send;
			send.oper = Cmd::Scene::DISABLE_REGISTER;
			send.country = wdCountryID;
			send.dwUserID = pUser->id;
			SceneTaskManager::getInstance().broadcastCmd(&send,sizeof(send));
			ScenesService::gm_logger->trace("GM %s �����˹���%uע��ر�", pUser->name, wdCountryID);
		}
	}
	return true;
}

/*
 * \brief ���ÿ���/�ر�ĳ����ע��
 * \param pUser ָ��ʹ����
 * \param para ����
 *
 * \return ִ�н��
 *
 */
bool Gm::enableLogin(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if (prop["countryid"]!="")
	{
		WORD wdCountryID = atoi(prop["countryid"].c_str());
		if (prop["status"]=="true")
		{
			Cmd::Scene::t_ChangeCountryStatus send;
			send.oper = Cmd::Scene::ENABLE_LOGIN;
			send.country = wdCountryID;
			send.dwUserID = pUser->id;
			SceneTaskManager::getInstance().broadcastCmd(&send,sizeof(send));
			ScenesService::gm_logger->trace("GM %s �����˹���%u��½����", pUser->name, wdCountryID);
		}
		else if (prop["status"]=="false")
		{
			Cmd::Scene::t_ChangeCountryStatus send;
			send.oper = Cmd::Scene::DISABLE_LOGIN;
			send.country = wdCountryID;
			send.dwUserID = pUser->id;
			SceneTaskManager::getInstance().broadcastCmd(&send,sizeof(send));
			ScenesService::gm_logger->trace("GM %s �����˹���%u��½�ر�", pUser->name, wdCountryID);
		}
	}
	return true;
}

/*
 * \brief ����/��ʾ �������汾��
 * \param pUser ָ��ʹ����
 * \param para ����
 *
 * \return ִ�н��
 *
 */
bool Gm::version(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	if (prop["oper"]=="set")
	{
		if (prop["id"]!="")
		{
			DWORD wdVersionID = atoi(prop["id"].c_str());
			Cmd::Scene::t_ChangeVerifyVersion send;
			send.oper = Cmd::Scene::CHANGECODE;
			send.versionCode = wdVersionID;
			send.dwUserID = pUser->id;
			SceneTaskManager::getInstance().broadcastCmd(&send,sizeof(send));
			ScenesService::gm_logger->trace("GM %s ���÷������汾У���Ϊ%u", pUser->name, wdVersionID);
		}
	}
	else if (prop["oper"]=="show")
	{
		Cmd::Scene::t_ChangeVerifyVersion send;
		send.oper = Cmd::Scene::SHOWCODE;
		send.versionCode = 0;
		send.dwUserID = pUser->id;
		SceneTaskManager::getInstance().broadcastCmd(&send,sizeof(send));
		ScenesService::gm_logger->trace("GM %s �鿴�汾У���", pUser->name);
	}
	return true;
}

/*
 * \brief ��ʾ����ע��/��½����״̬�б�
 * \param pUser ָ��ʹ����
 * \param para ����
 *
 * \return ִ�н��
 *
 */
bool Gm::checkCountryInfo(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	Cmd::Scene::t_ChangeCountryStatus send;
	send.oper = Cmd::Scene::LIST_COUNTRY_INFO;
	send.dwUserID = pUser->id;
	pUser->gatetask->sendCmd(&send,sizeof(send));
	return true;
}

bool Gm::showAddExp(SceneUser *pUser, const char * para)
{
	if (!pUser || !para) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	DWORD id = atoi(prop["id"].c_str());

	if (pUser->cartoon && pUser->cartoon->getCartoonID()==id)
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "npc: %u", pUser->cartoon->getCartoonData().addExp);
	if (pUser->cartoonList.find(id)!=pUser->cartoonList.end())
		Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "cartoonList: %u", pUser->cartoonList[id].addExp);
	return true;
}

bool Gm::changeHorse(SceneUser *pUser, const char * para)
{
	if (!pUser || !para) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	DWORD id = atoi(prop["id"].c_str());

	pUser->horse.horse(id);
	ScenesService::gm_logger->trace("GM %s ��changehorseָ��� id=%u", pUser->name, id);
	return true;
}

bool Gm::loadProcess(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	Cmd::Session::t_gmCommand_SceneSession send;
	strncpy((char *)send.src_name, pUser->name, MAX_NAMESIZE);
	send.gm_cmd = Cmd::Session::GM_COMMAND_LOAD_PROCESS;
	sessionClient->sendCmd(&send, sizeof(send));

	ScenesService::gm_logger->trace("GM %s ʹ��loadProcessָ��", pUser->name);
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "����process�ļ�����鿴��������־");
	return true;
}

bool Gm::loadGift(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	Cmd::Session::t_gmCommand_SceneSession send;
	strncpy((char *)send.src_name, pUser->name, MAX_NAMESIZE);
	send.gm_cmd = Cmd::Session::GM_COMMAND_LOAD_GIFT;
	sessionClient->sendCmd(&send, sizeof(send));

	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "����������ݣ���鿴session��־");
	return true;
}

bool Gm::contribute(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_ContributeCountry_SceneSession send;
	send.dwCountry = atoi(prop["country"].c_str())?atoi(prop["country"].c_str()):2;
	send.dwValue = atoi(prop["num"].c_str())?atoi(prop["num"].c_str()):1000;
	send.byType = atoi(prop["type"].c_str())?atoi(prop["type"].c_str()):1;
	sessionClient->sendCmd(&send, sizeof(send));

	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "���ף�country=%u type=%u num=%u", send.dwCountry, send.byType, send.dwValue);
	return true;
}

bool Gm::refreshGeneral(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	zProperties prop;
	prop.parseCmdLine(para);

	Cmd::Session::t_gmCommand_SceneSession send;
	send.gm_cmd = Cmd::Session::GM_COMMAND_REFRESH_GENERAL;
	sessionClient->sendCmd(&send, sizeof(send));

	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "ˢ�´󽫾�����鿴������session����־");
	return true;
}

bool Gm::closenpc(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	Cmd::Session::t_CloseNpc_SceneSession send;
	sessionClient->sendCmd(&send, sizeof(send));

	ScenesService::gm_logger->trace("GM %s ʹ��closenpcָ��", pUser->name);
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�رչ���NPC׼��ͣ��");
	return true;
}

bool Gm::countrypower(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;

	Cmd::Session::t_DebugCountryPower_SceneSession send;
	sessionClient->sendCmd(&send, sizeof(send));

	ScenesService::gm_logger->trace("GM %s ʹ��countrypowerָ��", pUser->name);
	Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�����������ǿ������");
	return true;
}

void Gm::sendLog(SceneUser *pUser, const char *cmd, const char * content)
{
	using namespace Cmd::GmTool;
	t_Log_GmTool send;
	send.time = SceneTimeTick::currentTime.sec();
	send.charID = pUser->id;
	send.accID = pUser->accid;
	strncpy(send.GM, pUser->name, MAX_NAMESIZE);
	snprintf(send.pos, MAX_NAMESIZE, "%s (%u,%u)", pUser->scene->name, pUser->getPos().x, pUser->getPos().y);
	strncpy(send.cmd, cmd, MAX_NAMESIZE);
	strncpy(send.content, content, MAX_NAMESIZE);

	ScenesService::getInstance().sendCmdToSuperServer(&send, sizeof(send));
}

bool Gm::getgive(SceneUser *pUser, const char * para)
{
	if (!pUser) return false;
	pUser->charbase.goldgive +=10;
	pUser->Give_MatarialNum +=2;
	pUser->Card_num +=10;
    Channel::sendSys(pUser, Cmd::INFO_TYPE_GAME, "�����Ʒ");
	return true;

}
