/**
 * \file
 * \version  $Id: OfflineMessage.cpp  $
 * \author  
 * \date 
 * \brief ������Ϣ������ʵ��
 *
 */

#include <set>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include "Zebra.h"
#include "OfflineMessage.h"
#include "Session.h"
#include "zTime.h"
#include "zMisc.h"

#define FILEPOWER 509
#define MAX_MESSAGE_NUMBER 5

struct ltstring
{
	bool operator()(const std::string s1, const std::string s2) const
	{
		return s1<s2;
	}
};

/**
 * \brief ��ʼ����������ϵͳ��ɾ������������Ϣ��
 * \author fqnewman
 * \return ʧ�ܷ���false,�ɹ�����true
 */
bool COfflineMessage::init()
{
	rootpath = Zebra::global["offlineMsgPath"];
	if (rootpath == "")
	{
		rootpath=std::string("/tmp/offlineMsg");
	}
	else
	{
		rootpath+=std::string("/offlineMsg");
	}

	
	if (access(rootpath.c_str(), F_OK) == 0)
	{
		if (!zMisc::rmDirTree(rootpath.c_str()))
		{
			Zebra::logger->error("��ʼ��ɾ��������ϢĿ¼[%s]ʧ��",rootpath.c_str());
			return false;
		}
	}

	if (mkdir(rootpath.c_str(), FILEPOWER)!=0)
	{
		Zebra::logger->error("��ʼ������������ϢĿ¼ʧ��");
		return false;
	}
	return true;
}

/**
 * \brief д��������Ϣ
 *
 * \param type:	��Ϣ����
 * \param id:	��ɫID
 * \param ptNullCmd: ��Ϣ����
 * \param cmdLen:	��Ϣ����
 *
 * \author fqnewman
 */
void COfflineMessage::writeOfflineMessage(const BYTE &type, const DWORD &id, const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen)
{
	std::set<std::string, ltstring> filelist;
	char buf[MAX_NAMESIZE];
	sprintf(buf,"/%u",id);
	std::string myPath = rootpath+buf;

	if (access(myPath.c_str(), F_OK) != 0)
	{
		mkdir(myPath.c_str(), FILEPOWER);   /// �����ʺ�Ŀ¼
	}

	sprintf(buf,"/%u",ptNullCmd->byCmd);
	myPath = myPath+buf;

	if (access(myPath.c_str(), F_OK) != 0)
	{
		mkdir(myPath.c_str(), FILEPOWER);  /// ������Ϣ����Ŀ¼
	}

	sprintf(buf,"/%u",ptNullCmd->byParam);
	myPath = myPath+buf;

	if (access(myPath.c_str(), F_OK) != 0)
	{
		mkdir(myPath.c_str(), FILEPOWER);  /// ������Ϣ��Ŀ¼
	}

	sprintf(buf,"/%u",type);
	myPath = myPath+buf;

	if (access(myPath.c_str(), F_OK) != 0)
	{
		mkdir(myPath.c_str(), FILEPOWER);  /// �����Զ�������Ŀ¼
	}
/// ��ʼ����ļ���Ŀ
	struct dirent *record;
	DIR* tDir = opendir(myPath.c_str());
	if (tDir != NULL)
	{
		while(NULL != (record = readdir(tDir)))
		{
			if (record->d_type == DT_REG)
			{
				filelist.insert(std::string(myPath+"/"+record->d_name));
			}
		}
		closedir(tDir);
	}
	while(filelist.size()>= MAX_MESSAGE_NUMBER) /// ���������Ϣ��Ŀ����ĳ������Ϣ��Ŀ����ɾ���ϵļ�¼�Ա���ָ��������
	{
		unlink(filelist.begin()->c_str());
		filelist.erase(filelist.begin());
	}

	do {
		zRTime ctv;
		sprintf(buf,"/%lu",ctv.sec());
	}while(access(std::string(myPath+buf).c_str(), F_OK) == 0);
    
	myPath = myPath+buf;  // ���յ�·���ļ���

//	Zebra::logger->debug("д��һ���µ�������Ϣ[%s]",myPath.c_str());
	int fd;

	if ((int)-1 != (fd = open(myPath.c_str(), O_CREAT|O_WRONLY, FILEPOWER)))
	{
		write(fd,ptNullCmd,cmdLen);
		close(fd);
	}
	else
	{
		Zebra::logger->error("�޷�д���ɫ[%u]��������Ϣ",id);
	}
}

/**
 * \brief ���ߵĽ�ɫ�����Լ���������Ϣ
 * \param pUser:	���ߵĽ�ɫ
 * \author: fqnewman
 */
void COfflineMessage::getOfflineMessage(const UserSession *pUser)
{
	char buf[MAX_NAMESIZE];
	sprintf(buf,"/%u",pUser->id);
	std::string myPath = rootpath+buf;
	getOfflineMessageSetAndSend(pUser, myPath);
}

/**
 * \brief ��ָ��·����ʼ���ұ��㼫����Ŀ¼�е�������Ϣ������
 * \param pUser:	��ɫ
 * \param path:	·��
 * \author fqnewman
 */
void COfflineMessage::getOfflineMessageSetAndSend(const UserSession *pUser, std::string path)
{
	struct dirent *record;
	std::set<std::string, ltstring> filelist;
	std::set<std::string, ltstring>::iterator tIterator;

	DIR* tDir = opendir(path.c_str());
	if (tDir == NULL) return;
	while(NULL != (record = readdir(tDir)))
	{
		if (record->d_type == DT_DIR)
		{
			if (strcmp(record->d_name,".") == 0 ||	strcmp(record->d_name,"..") == 0) continue;

			getOfflineMessageSetAndSend(pUser, (path+"/"+record->d_name).c_str());
		}
		else if (record->d_type == DT_REG)
		{
			filelist.insert(std::string(path+"/"+record->d_name));
		}
	}
	closedir(tDir);

	for (tIterator = filelist.begin(); tIterator != filelist.end(); tIterator++)
	{
		int fd;
		unsigned int cmdLen;
		BYTE buf[zSocket::MAX_DATASIZE];

		bzero(buf,zSocket::MAX_DATASIZE);
		if ((int)-1 != (fd = open((*tIterator).c_str(), O_RDONLY)))
		{
			cmdLen = read(fd,buf,zSocket::MAX_DATASIZE);
			pUser->sendCmdToMe(buf, cmdLen);
			close(fd);
		}
		else
		{
			Zebra::logger->error("�޷���ȡ��ɫ[%s]��������Ϣ",pUser->name);
		}
		unlink(tIterator->c_str());
	}

	rmdir(path.c_str());
	return;
}
