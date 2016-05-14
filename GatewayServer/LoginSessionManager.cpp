/**
 * \file
 * \version  $Id: LoginSessionManager.cpp  $
 * \author  
 * \date
 * \brief �����½�Ự����֮ǰ����Ϣ
 *
 * 
 */


#include <ext/hash_map>

#include "zMutex.h"
#include "SuperCommand.h"
#include "LoginSessionManager.h"

LoginSessionManager *LoginSessionManager::instance = NULL;

/**
 * \brief ������������һ����¼
 *
 * \param session �Ự��Ϣ
 */
void LoginSessionManager::put(const t_NewLoginSession &session)
{
	Zebra::logger->trace("LoginSessionManager::put");
	mlock.lock();
	//Zebra::logger->debug("put: %u %u %u %u %u %u %u %u", session.des_key[0], session.des_key[1], session.des_key[2], session.des_key[3], session.des_key[4], session.des_key[5], session.des_key[6], session.des_key[7]);
	LoginSessionHashmap_iterator it = sessionData.find(session.accid);
	if (it != sessionData.end())
	{
		//�ҵ��ˣ�ֻ�Ǹ���
		it->second.session = session;
		it->second.timestamp.now();
	}
	else
	{
		//û���ҵ�����Ҫ�����µļ�¼
		sessionData.insert(LoginSessionHashmap_pair(session.accid, LoginSession(session)));
	}
	mlock.unlock();
}

/**
 * \brief ��֤��½�����Ƿ���ȷ
 *
 * \param loginTempID ��½��ʱ���
 * \param accid ��½�ʺ�
 * \return ��֤�Ƿ�ɹ�
 */
bool LoginSessionManager::verify(const DWORD loginTempID, const DWORD accid,char *numPassword, DES_cblock *key)
{
	Zebra::logger->trace("LoginSessionManager::verify");
	bool retval = false;
	mlock.lock();
	LoginSessionHashmap_iterator it = sessionData.find(accid);
	if (it != sessionData.end()
			&& loginTempID == it->second.session.loginTempID)
	{
		retval = true;
		if (0!=key)
		{
			bcopy(it->second.session.des_key, key, sizeof(DES_cblock));
			//Zebra::logger->debug("������Կ��%u %u %u %u %u %u %u %u", it->second.session.des_key[0], it->second.session.des_key[1], it->second.session.des_key[2], it->second.session.des_key[3], it->second.session.des_key[4], it->second.session.des_key[5], it->second.session.des_key[6], it->second.session.des_key[7]);
		}
		bcopy(it->second.session.numpasswd,numPassword,sizeof(it->second.session.numpasswd));
		sessionData.erase(it);
	}
	mlock.unlock();
	return retval;
}

/**
 * \brief ���³�ʱ�ĵ�½��Ϣ
 */
void LoginSessionManager::update(const zRTime &ct)
{
	Zebra::logger->trace("LoginSessionManager::update");
	if (checkUpdateTime(ct))
	{
		mlock.lock();
		if (!sessionData.empty())
		{
			zTime currentTime;
			LoginSessionHashmap_iterator it;
			for(it = sessionData.begin(); it != sessionData.end();)
			{
				if (it->second.timestamp.elapse(currentTime) >= 10)
				{
					Zebra::logger->debug("��½��Ϣ��ʱ��%u, %u, %u", it->second.session.accid, it->second.session.loginTempID, it->second.session.wdGatewayID);
					LoginSessionHashmap_iterator tmp = it;
					it++;
					sessionData.erase(tmp);
				}
				else
					it++;
			}
		}
		mlock.unlock();
	}
}

