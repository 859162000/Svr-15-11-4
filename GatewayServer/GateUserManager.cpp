/**
 * \file
 * \version  $Id: GateUserManager.cpp  $
 * \author  
 * \date 
 * \brief ʵ�������û�������
 */

#include "GateUserManager.h"
#include "GatewayServer.h"
#include "SceneClient.h"

GateUserManager *GateUserManager::gum(NULL);
//RecycleUserManager *RecycleUserManager::instance=NULL;

/**
 * \brief �õ�Ψһʵ��
 *
 *
 * \return Ψһʵ��
 */
GateUserManager * GateUserManager::getInstance()
{
	if(gum==NULL)
		gum=new GateUserManager();
	return gum;
}

/**
 * \brief ɾ��Ψһʵ��
 *
 *
 */
void GateUserManager::delInstance()
{
	SAFE_DELETE(gum);
}

GateUserManager::GateUserManager()
{
	inited=false;
	userUniqeID=NULL;
}

GateUserManager::~GateUserManager()
{
	final();
}

/**
 * \brief ж�������û�
 *
 *
 * \return 
 */
void GateUserManager::removeAllUser()
{
	struct UnloadAllExec :public execEntry<GateUser>
	{
		std::vector<DWORD> del_vec;
		UnloadAllExec()
		{
		}
		bool exec(GateUser *u)
		{
			del_vec.push_back(u->id);
			return true;
		}
	};
	UnloadAllExec exec;
	GateUserManager::getInstance()->execEveryUser(exec);
	for(std::vector<DWORD>::iterator iter = exec.del_vec.begin() ; iter != exec.del_vec.end() ; iter ++)
	{
		GateUser *pUser=(GateUser *)GateUserManager::getInstance()->getUserByID(*iter);
		if(pUser)
		{
			//Zebra::logger->trace("�û�%s(%ld)��ж�س���ע��",pUser->name,pUser->id);
			pUser->Terminate();
		}
	}

}
/**
 * \brief �õ�һ��tempid
 *
 *
 * \param tempid: �õ���tempid(���)
 * \return �õ��Ƿ�ɹ�
 */
void GateUserManager::removeUserBySceneClient(SceneClient *scene)
{
	struct UnloadSceneExec :public execEntry<GateUser>
	{
		SceneClient *scene;
		std::vector<DWORD> del_vec;
		UnloadSceneExec(SceneClient *s):scene(s)
		{
		}
		bool exec(GateUser *u)
		{
			if(u->scene == scene)
			{
				del_vec.push_back(u->id);
			}
			return true;
		}
	};
	UnloadSceneExec exec(scene);
	GateUserManager::getInstance()->execEveryUser(exec);
	for(std::vector<DWORD>::iterator iter = exec.del_vec.begin() ; iter != exec.del_vec.end() ; iter ++)
	{
		GateUser *pUser=(GateUser *)GateUserManager::getInstance()->getUserByID(*iter);
		if(pUser)
		{
			Zebra::logger->trace("�û�%s(%ld)��ж�س���ע��",pUser->name,pUser->id);
			pUser->Terminate();
		}
	}

}

/**
 * \brief �õ�һ��tempid
 *
 *
 * \param tempid: �õ���tempid(���)
 * \return �õ��Ƿ�ɹ�
 */
bool GateUserManager::getUniqeID(DWORD &tempid)
{
	if(userUniqeID)
	{
		tempid=userUniqeID->get();
		//Zebra::logger->debug("�õ�usertempid = %ld",tempid);
		return (tempid!=userUniqeID->invalid());
	}
	else
		return false;
}

/**
 * \brief �ջ�һ��tempid
 *
 *
 * \param tempid: Ҫ�ջص�tempid
 */
void GateUserManager::putUniqeID(const DWORD &tempid)
{
	if(userUniqeID)
	{
		userUniqeID->put(tempid);
		//Zebra::logger->debug("����usertempid = %ld",tempid);
	}
}

/**
 * \brief ���ݳ�ʼ������tempid�ķ�Χ
 *
 *
 * \return true
 */
bool GateUserManager::init()
{
	if(!inited)
	{
		//Ϊÿ�����ط��������ɲ��ཻ����û���ʱID������,��С�Ĵ�1000��ʼ,ÿ����4998��ID����
		DWORD firstTempID=1000+(GatewayService::getInstance().getServerID()%100)*5000;
		userUniqeID=new zUniqueDWORDID(firstTempID,firstTempID+4998);
		inited=true;
	}
	return inited;
}

/**
 * \brief ж�������û�������
 *
 */
void GateUserManager::final()
{
	SAFE_DELETE(userUniqeID);
	inited=false;
}

/**
 * \brief ����accid�õ�һ���û�
 *
 *
 * \param accid: ��ɫ��accid
 * \return �û�
 */
GateUser * GateUserManager::getUserByAccID(DWORD accid)
{
	rwlock.rdlock();
	GateUser *ret =NULL;
	GateUserAccountID::find(accid,ret);
	rwlock.unlock();
	return ret;
}

/**
 * \brief �����ع�������ɾ��һ���û�
 *
 *
 * \param accid: ��ɾ���û���accid
 */
void GateUserManager::removeUserOnlyByAccID(DWORD accid)
{
	rwlock.wrlock();
	GateUserAccountID::remove(accid);
	rwlock.unlock();
}

/**
 * \brief ��һ���û���ӵ��������û���������
 *
 *
 * \param user: ��Ҫ��ӵ��������е��û�
 * \return ��ӳɹ�����ͼ��,���򷵻�false 
 */
bool GateUserManager::addUserOnlyByAccID(GateUser *user)
{
	rwlock.wrlock();
	bool ret=GateUserAccountID::push(user);
	rwlock.unlock();
	return ret;
}
bool GateUserManager::addCountryUser(GateUser *user)
{
	rwlock.wrlock();
	bool ret=countryindex[user->countryID].insert(user).second;
	rwlock.unlock();
	return ret;
}
void GateUserManager::removeCountryUser(GateUser *user)
{
	rwlock.wrlock();
	CountryUserMap_iter iter = countryindex.find(user->countryID);
	if(iter != countryindex.end())
	{
		iter->second.erase(user);
	}
	rwlock.unlock();
}
template <class YourNpcEntry>
void GateUserManager::execAllOfCountry(const DWORD country,execEntry<YourNpcEntry> &callback)
{
	rwlock.rdlock();
	CountryUserMap_iter iter = countryindex.find(country);
	if(iter != countryindex.end())
	{
		for(GateUser_SET_iter iter_1 = iter->second.begin() ; iter_1 != iter->second.end() ; iter_1++)
		{
			callback.exec(*iter_1);
		}
	}
	rwlock.unlock();
}
void GateUserManager::sendCmdToCountry(const DWORD country , const void *pstrCmd, const unsigned int nCmdLen)
{
	int _sendLen;
	t_StackCmdQueue cmd_queue;
	DWORD _type=0;
	char _name[MAX_NAMESIZE];
	bzero(_name,sizeof(_name));
	DWORD cmdLen=nCmdLen;
	GateUser::cmdFilter((Cmd::stNullUserCmd *)pstrCmd,_type,_name,cmdLen);
	_sendLen = zSocket::packetPackZip(pstrCmd,cmdLen,cmd_queue); 
	rwlock.rdlock();
	CountryUserMap_iter iter = countryindex.find(country);
	if(iter != countryindex.end())
	{
		for(GateUser_SET_iter iter_1 = iter->second.begin() ; iter_1 != iter->second.end() ; iter_1++)
		{
			(*iter_1)->sendCmd(cmd_queue.rd_buf(),_sendLen,_type,_name,true);
		}
	}
	rwlock.unlock();
}
