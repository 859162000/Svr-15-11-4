/**
 * \file
 * \version  $Id: GateUserManager.h  $
 * \author  
 * \date
 * \brief ���������û�������
 */

#ifndef _GATEUSERMANAGER_H_
#define _GATEUSERMANAGER_H_
#include "GateUser.h"
#include "zUserManager.h"
#include "zUniqueID.h"
/**
 * \brief GateUser���ʺ�IDΪkeyֵ��ָ����������Ҫ�̳�ʹ��
 */
class GateUserAccountID:protected LimitHash<DWORD,GateUser *>
{
	protected:

		/**
		 * \brief ��GateUser����������
		 * \param e Ҫ�����GateUser
		 * \return �ɹ�����true,���򷵻�false
		 */
		bool push(GateUser * &e)
		{
			GateUser *account;
			return (!find(e->accid,account) && insert(e->accid,e));
		}

	public:
		GateUserAccountID() {}
		virtual ~GateUserAccountID() {}
		/**
		 * \brief ͨ���ʺ�ID�õ�GateUser
		 * \param accid Ҫ�õ�GateUser���ʺ�ID
		 * \return ����GateUserָ��,δ�ҵ�����NULL
		 */
		virtual GateUser * getUserByAccID(DWORD accid) =0;
		/**
		 * \brief ͨ���ʺ�IDɾ��GateUser�������ʺ��������Ƴ�
		 * \param accid Ҫɾ����GateUser���ʺ�ID
		 */
		virtual void removeUserOnlyByAccID(DWORD accid) =0;
		/**
		 * \brief ͨ���ʺ�ID���GateUser������ӵ��ʺ�������
		 * \param user Ҫ��ӵ�GateUser
		 */
		virtual bool addUserOnlyByAccID(GateUser *user) =0;
};

/**
 * \brief �����û�������
 *
 */
class GateUserManager:public zUserManager,protected GateUserAccountID
{
	private:
		bool inited;
		zUniqueDWORDID *userUniqeID;
		static GateUserManager *gum;

		GateUserManager();
		~GateUserManager();
		bool getUniqeID(DWORD &tempid);
		void putUniqeID(const DWORD &tempid);
	public:
		static GateUserManager *getInstance();
		static void delInstance();
		bool init();
		void final();
		GateUser * getUserByAccID(DWORD accid);
		void removeUserOnlyByAccID(DWORD accid);
		bool addUserOnlyByAccID(GateUser *user);
		void removeUserBySceneClient(SceneClient *scene);
		void removeAllUser();
		bool addCountryUser(GateUser *user);
		void removeCountryUser(GateUser *user);
		template <class YourNpcEntry>
		void execAllOfCountry(const DWORD country,execEntry<YourNpcEntry> &callback);
		void sendCmdToCountry(const DWORD country , const void *pstrCmd, const unsigned int nCmdLen);
	private:
		typedef std::set<GateUser *> GateUser_SET;
		typedef GateUser_SET::iterator GateUser_SET_iter;
		typedef __gnu_cxx::hash_map<DWORD, GateUser_SET> CountryUserMap;
		typedef CountryUserMap::iterator CountryUserMap_iter;
		CountryUserMap countryindex;
};
/* 
class RecycleUserManager:public zUserManager
{
	private:
		RecycleUserManager()
		{
		}
		~RecycleUserManager()
		{
		}
	public:
		static RecycleUserManager *getInstance();
		static void delInstance();
	private:
		static RecycleUserManager *instance;
};
// */
#endif
