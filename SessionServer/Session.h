#ifndef _SESSION_H_
#define _SESSION_H_

#include "zUser.h"
#include <map>
#include <vector>
#include "zSceneManager.h"
#include "SessionCommand.h"
#include "SessionTask.h"
#include "CRelationManager.h"
#include "zTime.h"
#include <set>

/**
 * \brief �Ự��
 * �û��Ự�ͳ����Ự�Ļ���
 *
 */
class Session:private zNoncopyable
{

	private:
		///����ʱ��
		time_t createtime;
		///�ûỰ������
		SessionTask *task;

	protected:

		/**
		 * \brief ���캯��
		 * \param task �ûỰ������
		 */
		Session(SessionTask *task)
		{
			createtime=time(NULL);
			this->task=task;
		}

	public:
		///��������������
		DWORD reqAdopter;

		/**
		 * \brief �õ��ûỰ������
		 * \return �ûỰ������
		 */
		SessionTask *  getTask() const
		{
			return task;
		}
		
		/**
		 * \brief ��Է�������Ϣ
		 * \param pstrCmd Ҫ���͵���Ϣ
		 * \param nCmdLen ��Ϣ����
		 * \return �����Ƿ�ɹ�
		 */
		bool sendCmd(const void *pstrCmd, const int nCmdLen) const
		{
			if (task)
				return task->sendCmd(pstrCmd, nCmdLen);
			else
				return false;
		}

};

/**
 * \brief �����Ự
 *
 */
class SceneSession:public zScene,public Session
{

	public:

		///��ǰ��ͼ����������ҵȼ�Ϊ0��ʾ������
		BYTE level;
		/**
		 * \brief ���캯��
		 */
		SceneSession(SessionTask *task):zScene(),Session(task)
		{
		}

		/**
		 * \brief ͨ����Ϣע��һ����ͼ
		 * \param reginfo ��ͼע����Ϣ
		 * \return �Ƿ�ע��ɹ�
		 */
		bool reg(Cmd::Session::t_regScene_SceneSession *reginfo)
		{
			if(reginfo)
			{
				id=reginfo->dwID;
				tempid=reginfo->dwTempID;
				strncpy(name,(char *)reginfo->byName,MAX_NAMESIZE);
				file = reginfo->fileName;
				level = reginfo->byLevel;
				return true;
			}
			else
				return false;
		}

		///��Ӧ�ĵ�ͼ�ļ���
		std::string file;
};

/**
 * \brief �û��Ự��
 *
 */
class UserSession:public zUser,public Session
{

	public:
		//std::set<DWORD> cartoonList;
		//std::set<DWORD> adoptList;

		char autoReply[MAX_CHATINFO];//�Զ��ظ�

		//����
		static DWORD user_count;

		//��������
		static std::map<DWORD , DWORD> country_map;

		///�ʺ�id
		DWORD accid;
		///���id
		DWORD unionid;
		///����id
		DWORD country;
		/// ��������
		BYTE countryName[MAX_NAMESIZE+1];
		///����id
		DWORD septid;
		///����id
		DWORD schoolid;
		///��ʱid
		DWORD teamid;
		///�ȼ�
		WORD  level;
		///ְҵ
		WORD  occupation;
		/// ����ʱ��
		zRTime regTime;
		///ͷ��
		DWORD  face;
		
		// ��ѫֵ
		DWORD dwExploit;

		// �Ĳ�ֵ
		DWORD dwGrace;

		// ���ﵱǰ����
		QWORD qwExp;

		///ϵͳ������Ϣ
		BYTE sysSetting[20];//ϵͳ����

		///���ڵĳ����Ự
		SceneSession *scene;
		///����ϵ������
		CRelationManager relationManager;

		///�´η��̻��ʱ��
		zRTime nextBlessTime;

		//���徭��
		WORD septExp;


		UserSession(SessionTask *task);
		~UserSession();

		/**
		 * \brief ����ע��һ�����
		 * \param reginfo ��������Ϣ����Ϣ
		 * \return �Ƿ�ע��ɹ�
		 */
		bool reg(Cmd::Session::t_regUser_GateSession *reginfo)
		{
			if(reginfo)
			{
				accid=reginfo->accid;
				id=reginfo->dwID;
				tempid=reginfo->dwTempID;
				//TODO septid ��ʼ��
				level=reginfo->wdLevel;
				occupation=reginfo->wdOccupation;
				country = reginfo->wdCountry;
				std::map<DWORD , DWORD>::iterator iter = country_map.find(country);
				if(iter == country_map.end())
				{
					country_map[country] = 0;
				}
				country_map[country]++;
				strncpy(name,(char *)reginfo->byName,MAX_NAMESIZE);
				strncpy((char*)countryName, (char*)reginfo->byCountryName, MAX_NAMESIZE);
				relationManager.setUser(this);
				return true;
			}
			else
				return false;
		}
		static void getCountryUser(std::vector<std::pair<DWORD,DWORD> > &v)
		{
			std::map<DWORD , DWORD>::iterator iter;
			for( iter = country_map.begin() ; iter != country_map.end() ; iter ++)
			{
				v.push_back( std::make_pair(iter->first,iter->second) );
			}
		}

		/**
		 * \brief ��������ϵ��Ϣ
		 * \param regsuccess ע��ɹ�����Ϣ
		 * \return 
		 */
		bool setRelationData(const Cmd::Session::t_regUserSuccess_SceneSession *regsuccess)
		{
			if(regsuccess)
			{
				unionid	= regsuccess->dwUnionID;
				country = regsuccess->dwCountryID;
				septid = regsuccess->dwSeptID;
				schoolid = regsuccess->dwSchoolID;
				return true;
			}
			else
				return false;
		}

		/**
		  * \brief ������ż��Ϣ������
		  *
		  */
		void updateConsort();
		
		/**
		  * \brief ������ż��Ϣ������
		  *
		  */
		void updateCountryStar(); 

		bool checkChatCmd(const Cmd::stNullUserCmd *, const unsigned int nCmdLen) const;
		void sendCmdToMe(const void *pstrCmd, const unsigned int nCmdLen) const;
		void sendSysChat(int type, const char *pattern, ...) const;
		void sendGmChat(int type, const char* pattern, ...) const;
		void setFriendDegree(Cmd::Session::t_CountFriendDegree_SceneSession *);
		void sendFriendDegree(Cmd::Session::t_RequestFriendDegree_SceneSession *);
		bool forwardScene(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen);
};

#endif
