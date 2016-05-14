/**
 * \file
 * \version  $Id: RoleTask.cpp  $
 * \author 
 * \date 
 * \brief �������ݿ���ʷ�����������
 */

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <ext/hash_map>

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "RoleTask.h"
#include "Zebra.h"
#include "zDBConnPool.h"
#include "roleRegServer.h"
#include "URLManager.h"
#include "RoleregCommand.h"
#include "Zebra.h"

/**
 * \brief ��װһ��������
 */
class RoleDataContainer
{

	public:

		/**
		 * \brief ���캯��
		 */
		RoleDataContainer() {};

		/**
		 * \brief ��������
		 */
		~RoleDataContainer()
		{
			mlock.lock();
			barrel.clear();
			mlock.unlock();
		}

		/**
		 * \brief ������ݵ�������
		 * \param data ����ӵ�����
		 * \return ��ӵ������Ƿ�ɹ�
		 */
		bool add(const RoleData &data)
		{
			zMutex_scope_lock scope_lock(mlock);
			container_type::const_iterator it = barrel.find(data.name);
			if (it == barrel.end())
			{
				//û���ҵ�
				std::pair<container_type::iterator, bool> p = barrel.insert(container_type::value_type(data.name, data));
				return p.second;
			}
			else
			{
				//�ҵ�����
				return false;
			}
		}

		/**
		 * \brief ��������ɾ��ָ�����Ƶ�����
		 * \param name ָ��ɾ��������
		 */
		void remove(const char *name)
		{
			zMutex_scope_lock scope_lock(mlock);
			container_type::iterator it = barrel.find(name);
			if (it != barrel.end())
			{
				//�ҵ�����
				barrel.erase(it);
			}
		}

	private:

		struct eqstr
		{
			bool operator()(const char* s1, const char* s2) const
			{
				return strcmp(s1, s2) == 0;
			}
		};
		/**
		 * \brief ��������
		 */
		typedef __gnu_cxx::hash_map<const char *, RoleData, __gnu_cxx::hash<const char *>, eqstr> container_type;
		/**
		 * \brief �������ʻ������
		 */
		zMutex mlock;
		/**
		 * \brief ��������
		 */
		container_type barrel;

};

static RoleDataContainer SetRoleInfo; //��Ž�ɫ��Ϣ��container

int RoleTask::verifyConn()
{
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//����ֻ�Ǵӻ���ȡ���ݰ������Բ������û������ֱ�ӷ���
			return 0;
		else
		{
			using namespace Cmd::RoleReg;
			t_LoginRoleReg *ptCmd = (t_LoginRoleReg *)pstrCmd;
			if (CMD_LOGIN == ptCmd->cmd && PARA_LOGIN == ptCmd->para)
			{
				Zebra::logger->debug("�ͻ�������ͨ����֤");
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		return retcode;
	}
}

int RoleTask::waitSync()
{
	using namespace Cmd::RoleReg;
	t_LoginRoleReg cmd;
	Zebra::logger->debug("$$$$$$$$$");
	if (sendCmd(&cmd, sizeof(cmd)))
		return 1;
	else
		return -1;
}

/**
 * \brief �������Ը������ӵ�ָ��
 * \param ptNullCmd �������ָ��
 * \param nCmdLen ָ���
 * \return �����Ƿ�ɹ�
 */
bool RoleTask::msgParse(const Cmd::t_NullCmd * ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::RoleReg;
	switch(ptNullCmd->cmd)
	{
		case CMD_ROLEREG:
			if (msgParse_loginServer(ptNullCmd, nCmdLen))
			{
				return true;
			}
		break;
	}
	return false;
}

/**
 * \brief ��ɫ�Ĵ�����ɾ��
 * \param ptNullCmd ������ָ��
 * \param nCmdLen ָ���
 * \return ����ɾ����ɫ�Ƿ�ɹ�
 */
bool RoleTask::msgParse_loginServer(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::RoleReg;
	switch(ptNullCmd->para)
	{
		case PARA_CHARNAME_ROLEREG:
		{
			t_Charname_Rolereg *ptCmd = (t_Charname_Rolereg *)ptNullCmd;
			t_Charname_Rolereg Cmd;
			Cmd.wdServerID 	= ptCmd->wdServerID;
			Cmd.accid			= ptCmd->accid;
			Cmd.gameZone	= ptCmd->gameZone;
			strcpy(Cmd.name, ptCmd->name);
			Cmd.state = ptCmd->state;

			unsigned int hash = URLManager::hashString(ptCmd->name);
			Zebra::logger->debug("====����=====%s", URLManager::getInstance().roleregTableName(hash).c_str());
			Zebra::logger->debug("hash=%u", hash);
			Zebra::logger->debug("ptCmd->state=%u", ptCmd->state);

			if (ptCmd->state & ROLEREG_STATE_TEST)//����ɫ��
			{
				RoleData tmpinfo;
				//������COPYһ��
				tmpinfo.accid = ptCmd->accid;
				tmpinfo.zone= ptCmd->gameZone.zone;
				tmpinfo.game=  ptCmd->gameZone.game;
				strcpy(tmpinfo.name, ptCmd->name);
				Zebra::logger->debug("accid=%u",	tmpinfo.accid);
				Zebra::logger->debug("zone=%u",	tmpinfo.zone);
				Zebra::logger->debug("game=%u",	tmpinfo.game);
				Zebra::logger->debug("name=%s",	tmpinfo.name);
				if (SetRoleInfo.add(tmpinfo))
				{
					//��ӳɹ�����ʾԭ��û������
					char where[128];
					DWORD accID=0;
					static const dbCol sel_con_define[]	= {
						{"`ACCID`", zDBConnPool::DB_DWORD, sizeof(DWORD)},
						{NULL, 0, 0}
					};
					memset(where, 0, sizeof(where));
					sprintf(where, "NAME = '%s'", tmpinfo.name);
					connHandleID handle = roleRegService::dbConnPool->getHandle(&hash);
					Zebra::logger->debug("handle=%u", handle);
					if ((connHandleID)-1 != handle)
					{
						if ((unsigned int)1 == roleRegService::dbConnPool->exeSelectLimit(handle, 
									URLManager::getInstance().roleregTableName(hash).c_str(), 
									sel_con_define, where, NULL, 1, (unsigned char *)&accID))
						{
							//�����Ѵ������ݿ��У���������е�����
							SetRoleInfo.remove(tmpinfo.name);
							Cmd.state |= ROLEREG_STATE_HAS;
							Zebra::logger->debug("�����Ѵ������ݿ���");
						}
						roleRegService::dbConnPool->putHandle(handle);
					}
					else
					{
						//��ȡ���ݿ���������������е�����
						SetRoleInfo.remove(tmpinfo.name);
						Cmd.state |= ROLEREG_STATE_HAS;
						Zebra::logger->debug("��ȡ���ݿ�������");
					}
				}
				else
				{
					Cmd.state |= ROLEREG_STATE_HAS;
					Zebra::logger->debug("�����Ѵ��ڻ�����");
				}
			}
			else if (ptCmd->state & ROLEREG_STATE_WRITE)//��д
			{
				RoleData Tmpinfo;
				Tmpinfo.accid = ptCmd->accid;
				Tmpinfo.zone= ptCmd->gameZone.zone;
				Tmpinfo.game=  ptCmd->gameZone.game;
				strcpy(Tmpinfo.name, ptCmd->name);
				Zebra::logger->debug("##########");
				Zebra::logger->debug("name = %s ", Tmpinfo.name);
				//���ݿ�������
				static const dbCol creat_con_define[]	= {
					{"`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE])},
					{"`GAME`", zDBConnPool::DB_WORD, sizeof(WORD)},
					{"`ZONE`", zDBConnPool::DB_WORD, sizeof(WORD)},
					{"`ACCID`", zDBConnPool::DB_DWORD, sizeof(DWORD)},
					{NULL, 0, 0}
				};
				connHandleID handle = roleRegService::dbConnPool->getHandle(&hash);
				Zebra::logger->debug("handle=%u", handle);
				if ((connHandleID)-1 != handle)
				{
					if ((unsigned int)-1 != roleRegService::dbConnPool->exeInsert(handle, 
								URLManager::getInstance().roleregTableName(hash).c_str(), 
								creat_con_define, (unsigned char *)&Tmpinfo))
					{
						Cmd.state |= ROLEREG_STATE_OK;
						Zebra::logger->debug("��д���ݿ�ɹ�");
					}
					else
					{
						Cmd.state &= ~ROLEREG_STATE_OK;
						Zebra::logger->debug("��д���ݿ�ʧ��");
					}
					roleRegService::dbConnPool->putHandle(handle);
				}
				else
				{
					Cmd.state &= ~ROLEREG_STATE_OK;
					Zebra::logger->debug("��ȡ���ʧ��");
				}
				//����ṹ�иý�ɫ
				SetRoleInfo.remove(ptCmd->name);
			}
			else if (ptCmd->state & ROLEREG_STATE_CLEAN)//���
			{
				Zebra::logger->debug("name = %s ", ptCmd->name);
				char cWhere[128];
				memset(cWhere, 0, sizeof(cWhere));
				sprintf(cWhere, "NAME = '%s'", ptCmd->name);
				connHandleID handle = roleRegService::dbConnPool->getHandle(&hash);
				Zebra::logger->debug("handle=%u", handle);
				if ((connHandleID) -1 != handle)
				{
					if ((unsigned int)-1 == roleRegService::dbConnPool->exeDelete(handle,
									URLManager::getInstance().roleregTableName(hash).c_str(), cWhere))
					{
						Cmd.state &= ~ROLEREG_STATE_OK;
						Zebra::logger->debug("ɾ����ɫ%sʧ��", ptCmd->name);
					}
					else
					{
						Cmd.state |= ROLEREG_STATE_OK;
						Zebra::logger->debug("ɾ����ɫ%s�ɹ�", ptCmd->name);
					}
					roleRegService::dbConnPool->putHandle(handle);
				}
				else
				{
					Cmd.state &= ~ROLEREG_STATE_OK;
					Zebra::logger->debug("��ȡ���ʧ��");
				}
			}

			Zebra::logger->debug("Cmd.state =%u", Cmd.state);
			return sendCmd(&Cmd, sizeof(Cmd));
		}
		break;
	}
	return false;
}


