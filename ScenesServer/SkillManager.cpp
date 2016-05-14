#include "SkillManager.h"
#include "Zebra.h"
#include "zMisc.h"


/**
 * \brief  ��ȡ��ʱid
 * \param  tempid ��ʱid
 * \return ��ȡ�ɹ�
 */
bool SkillManager::getUniqeID(DWORD &tempid)
{
	return true;
}

/**
 * \brief  ��ʹ����ϵ���ʱid�ͷ�
 * \param  tempid ʹ����ϵ���ʱid
  */
void SkillManager::putUniqeID(const DWORD &tempid)
{
}

/**
 * \brief  ���캯��
 */
UserSkillM::UserSkillM()
{
}

/**
 * \brief  ��������������еļ��ܶ���
   */
UserSkillM::~UserSkillM()
{
	for(zEntryTempID::hashmap::iterator it=zEntryTempID::ets.begin();it!=zEntryTempID::ets.end();it++)
	{
		zSkill *skill = (zSkill *)it->second;
		SAFE_DELETE(skill);
	}
}

/**
 * \brief  ������ʱid��ȡ���ܶ���
 * \param  id ��ʱid
 * \return ���ܶ���
 */
zSkill *UserSkillM::getSkillByTempID(DWORD id)
{
	return (zSkill *)getEntryByTempID(id);
}

/**
 * \brief  ������ʱid ɾ�����ܶ���
 * \param  id ��ʱid
  */
void UserSkillM::removeSkillByTempID(DWORD id)
{
	zEntry *e=getEntryByTempID(id);
	if(e)
		removeEntry(e);
}

/**
 * \brief  �ӹ�������ɾ��ָ���ļ��ܶ���
 * \param  s ��ɾ���ļ��ܶ���
 */
void UserSkillM::removeSkill(zSkill *s)
{
	removeEntry(s);
}

/**
 * \brief  ����һ�����ܶ��󵽹�������
 * \param  s ���ܶ���
 * \return true ���ӳɹ� false ����ʧ��
 */
bool UserSkillM::addSkill(zSkill *s)
{
	bool bret = false;
	if(s)
	{
		zSkill *ret = (zSkill *)getEntryByTempID(s->id);
		if(ret)
		{
			Zebra::logger->debug("����ID�ظ�(%ld)" , s->id);
		}

		bret = addEntry((zSkill *)s);
		if(!bret)
		{
			Zebra::logger->fatal("��Ӽ��ܱ�ʧ��");
		}
	}

	return bret;
}

/**
 * \brief  ���ݼ���id���Ҷ�Ӧ�ļ��ܶ���
  * \return ���ܶ���
 */
zSkill *UserSkillM::findSkill(DWORD skillid)
{
	//zSkill *s;
	//for(zEntryTempID::hashmap::iterator it=zEntryTempID::ets.begin();it!=zEntryTempID::ets.end();it++)
	//{
	//	s = (zSkill *)it->second;
	//	if(s->data.skillid == skillid)
	//	{
	//		return (zSkill *)it->second;
	//	}
	//}
	//return NULL;
	return (zSkill *)getEntryByTempID(skillid);
}

/**
 * \brief  ���ݼ���id���Ҷ�Ӧ�ļ��ܶ���
 * \param myKind ����ϵ��
 * \param mySubkind ��������
 * \return ����Ͷ��ļ��ܵ���
 */
DWORD UserSkillM::getPointInTree(DWORD myKind, DWORD mySubkind)
{
	zSkill *s;
	DWORD num=0;
	for(zEntryTempID::hashmap::iterator it=zEntryTempID::ets.begin();it!=zEntryTempID::ets.end();it++)
	{
		s = (zSkill *)it->second;
		if ((s->base->subkind == 1)&&(s->base->kind == myKind)) num+=s->base->level; //���ӻ���ϵ����
		if ((s->base->subkind !=1)&&(s->base->subkind == mySubkind)&&(s->base->kind == myKind)) num+=s->base->level;
	}
#ifdef _DEBUGLOG
	Zebra::logger->info("[���ܵ���]%u", num);
#endif 
	return num;
}

/**
 * \brief  �ص��������û����еļ��ܶ���
 * \param  exec �ص�����
 */
void UserSkillM::execEvery(UserSkillExec &exec)
{
	for(zEntryTempID::hashmap::iterator it=zEntryTempID::ets.begin();it!=zEntryTempID::ets.end();it++)
	{
		if(!exec.exec((zSkill *)it->second))
			return;
	}
}

/**
 * \brief  ������û����еļ�����ȴʱ��
 */
void UserSkillM::resetAllUseTime()
{
	zSkill *s;
	for(zEntryTempID::hashmap::iterator it=zEntryTempID::ets.begin();it!=zEntryTempID::ets.end();it++)
	{
		s = (zSkill *)it->second;
		s->resetUseTime();
	}
}

/**
 * \brief  ������û����еļ�����ȴʱ��
 */
void UserSkillM::clearAllUseTime()
{
	zSkill *s;
	for(zEntryTempID::hashmap::iterator it=zEntryTempID::ets.begin();it!=zEntryTempID::ets.end();it++)
	{
		s = (zSkill *)it->second;
		s->clearUseTime();
		s->refresh(true);
	}
}

/**
 * \brief  ˢ�¼���(�����������ܵȼ�����������)
 */
void UserSkillM::refresh()
{
	zSkill *s;
	for(zEntryTempID::hashmap::iterator it=zEntryTempID::ets.begin();it!=zEntryTempID::ets.end();it++)
	{
		s = (zSkill *)it->second;
		s->refresh();
	}
}

void UserSkillM::clear()
{
	SkillManager::clear();
}

int UserSkillM::size() const
{
	return SkillManager::size();
}

/**
 * \brief  ���ݼ���id���Ҷ�Ӧ�ļ��ܶ���ɾ��
  * \return ���ܶ���
 */
void UserSkillM::clearskill(DWORD skillid)
{
	SkillManager::removeEntry(getEntryByTempID(skillid));
}
