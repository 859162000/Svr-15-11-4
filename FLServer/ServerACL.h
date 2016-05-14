/**
 * \file
 * \version  $Id: ServerACL.h  $
 * \author  
 * \date 
 * \brief �洢��Ч���������б�
 * ��Ч�������б�洢��xml�ļ��У�������������ʱ���ȡ��Щ��Ϣ���ڴ棬
 * ��һ�����������������ӹ�����ʱ�򣬿��Ը�����Щ��Ϣ�ж���������Ƿ�Ϸ��ġ�
 */

#ifndef _ServerACL_h_
#define _ServerACL_h_

#include <string>
#include <ext/hash_map>

#include "zType.h"
#include "zMisc.h"
#include "Zebra.h"
#include "zRWLock.h"
#include "zNoncopyable.h"

struct ACLZone
{
	GameZone_t gameZone;
	std::string ip;
	unsigned short port;
	std::string name;
	std::string desc;

	ACLZone()
	{
		port = 0;
	}
	ACLZone(const ACLZone &acl)
	{
		gameZone = acl.gameZone;
		ip = acl.ip;
		port = acl.port;
		name = acl.name;
		desc = acl.desc;
	}
};

class ServerACL : zNoncopyable
{

	public:

		ServerACL() {};
		~ServerACL() {};

		bool init();
		void final();
		bool check(const char *strIP, const unsigned short port, GameZone_t &gameZone, std::string &name);

	private:

		bool add(const ACLZone &zone);

		/**
		 * \brief hash����
		 *
		 */
		struct GameZone_hash
		{
			size_t operator()(const GameZone_t &gameZone) const
			{
				__gnu_cxx::hash<DWORD> H;
				return H(gameZone.id);
			}
		};
		typedef __gnu_cxx::hash_map<const GameZone_t, ACLZone, GameZone_hash> Container;
		Container datas;
		zRWLock rwlock;

};

typedef singleton_default<ServerACL> ServerACLSingleton;

#endif

