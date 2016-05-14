/**
 * \file CDare.h
 * \version  $Id: CGem.h  $
 * \author  
 * \date 
 * \brief ���廤�����������
 *
 */


#ifndef _CLASS_CGEM_H_
#define _CLASS_CGEM_H_

#include <vector>
#include "zMutex.h"
#include "zMisc.h"
#include "zType.h"
#include "zEntry.h"
#include "zEntryManager.h"
#include "Command.h"
#include "SessionCommand.h"
#include "zUser.h"
#include "zUniqueID.h"
#include "zTime.h"
#include "zSingleton.h"

class UserSession;
class zRWLock;
class CGem;

typedef zUniqueID<DWORD> zUniqueDWORDID;

struct ArhatMapPoint
{
	DWORD dwMapID;
	DWORD x;
	DWORD y;
};
const int mappoint_num = 10;

/**
 * \brief ���������¼������
 *
 * �����ս��¼������,ʹ��Singletonģʽ
 */
class CGemM:public zEntryManager<zEntryTempID>, 
	    public Singleton<CGemM>
{
	friend class SingletonFactory<CGemM>;

	private:
	zUniqueDWORDID *channelUniqeID;
	zRWLock rwlock;

	CGemM();

	public:
	~CGemM();

	bool getUniqeID(DWORD &tempid);
	void putUniqeID(const DWORD &tempid);

	template <class YourEntry>
		bool execEveryOne(execEntry<YourEntry> &exec)
		{
			rwlock.rdlock();
			bool ret=execEveryEntry<>(exec);
			rwlock.unlock();
			return ret;
		}

	template <class YourEntry>
		void removeOne_if(removeEntry_Pred<YourEntry> &pred)
		{
			rwlock.wrlock();
			removeEntry_if<>(pred);
			rwlock.unlock();
		}

	ArhatMapPoint wait_point[mappoint_num];
	bool init();
	static void destroyMe();

	/**
	 * \brief ��������������Ķ�ʱ���ص�����
	 *
	 * ��ʱ�������ж�ս��¼��ɾ����Ч��¼�������ﵽʱ��Ķ�ս����ش���
	 */
	void timer();


	/**
	 * \brief �����û��Ķ�ս����
	 *
	 * \param pUser ���͸������������Ӧ��UserSession����
	 * \param ptNullCmd �յ�������
	 * \param cmdLen �����
	 * \return �ǻ����������������õ���Ӧ��������Ϊtrue, ����Ϊfalse
	 *
	 */
	bool processUserMessage(UserSession *pUser, const Cmd::stNullUserCmd *ptNullCmd, const unsigned int cmdLen);

	/**
	 * \brief ���������͹����ķ���������Ϣ
	 *
	 * \param cmd �������������͹���������ָ��
	 * \param cmdLen �����
	 *
	 * \return ���Ѷ���ķ�������������õ���Ӧ��������true, ����Ϊfalse.
	 *
	 */
	bool processSceneMessage(const Cmd::t_NullCmd *cmd, const unsigned int cmdLen);

	/**
	 * \brief ���ҷ��������Ļ�������
	 *
	 *  ��ID2Ϊ0ʱ�������Ƿ���ID1��ս�ļ�¼��dwType�����������롣
	 *
	 * \param dwType ��ս����:Cmd:UNION_DARE, Cmd:SCHOOL_DARE, Cmd:SEPT_DARE
	 * \param dwID1  ����ID
	 * \param dwID2  �ط�ID
	 *
	 * \return �ҵ��򷵻ض�Ӧ��ս��¼��ָ��
	 */
	CGem * findGem(DWORD dwCountryID);

	/**
	 * \brief ����ID���һ�������
	 *
	 * \param dwID  GEM ID
	 *
	 * \return �ҵ��򷵻ض�Ӧ���������¼��ָ��
	 */
	CGem*  findGemByID(DWORD dwID);

	/**
	 * \brief �����µĶ�ս��¼
	 *
	 * \param dwCountryID ������������
	 *
	 * \return û���ظ���¼��������ɹ�������true,���򷵻�false
	 *
	 */
	bool addNewGem(DWORD dwCountryID);
				

	void forceEnd();

	/**
	 * \brief �û����ߴ���
	 *
	 * �û�����ʱ��ͬʱ�жϸ��û��Ķ�ս״̬��������Ӧ����
	 *
	 * \param pUser �����û�
	 *
	 */
	void userOnline(UserSession* pUser);
	
	/**
	 * \brief �û����ߴ���
	 *
	 * �û�����ʱ��ͬʱ״̬�������Ļ���״̬��������Ӧ����
	 *
	 * \param pUser �����û�
	 *
	 */
	void userOffline(UserSession* pUser);
};

/// �޺�
class CArhat
{
	public:
	DWORD x;
	DWORD y;
	DWORD dwCountryID;
	DWORD dwMapRealID;
	BYTE  byState; // ״̬��0Ϊ����δ���˻�ȡ��1Ϊ�����ѱ��˻�ȡ
	DWORD dwHoldUserID; // ����Ŀǰ�������ID
	DWORD dwID;	// �޺���ID

	CArhat()
	{
		x = 0;
		y = 0;
		dwCountryID = 0;
		dwMapRealID = 0;
		byState = 0;
		dwHoldUserID = 0;
		dwID = 0;
	}

	bool refreshNPC();
	bool clearNPC();
};

class CGem : public zEntry
{
	protected:
		zRWLock rwlock;                             // ��д��
		time_t tmStart;				    // ��ʼʱ��

	public:
		enum{
			GEM_READY,		// ��������׼��״̬
			GEM_ACTIVE,		// �����������״̬
			GEM_READY_OVER,        // ��������׼������״̬
			GEM_OVER		// �������״̬
		};

		CGem();

		virtual ~CGem();

		/**
		 * \brief ʱ���¼�����ص���������TimeTick.cpp�к����ص�
		 *
		 *  ����ս����DARE_READY״̬����ӣ���ȡ����ս��������DARE_RETURN_GOLD״̬
		 *  ����ս����DARE_READY_QUESTION  ����ս����DARE_READY
		 *
		 *
		 */
		virtual void timer();	//ʱ���¼�����ص�


		// ״̬������,��timer��״̬��Ǩ�Ĵ���
		virtual void setReadyState();
		virtual void setActiveState();
		virtual void setReadyOverState();
		virtual void setOverState();

		/**
		 * \brief ���Ͷ�ս����״̬������
		 *
		 *  ��������Ӧ�����Ӷ�ս��¼����sendNineToMeʱ�����ж�ս״̬�ļ���,��
		 *  ����ֻ���û����ߴ���ʱ�����á�
		 *
		 */
		virtual void sendActiveStateToScene(UserSession* pUser);

		/**
		 * \brief �ж��Ƿ����ڻ�������ʱ��
		 *
		 * \return ������ڻ�������ʱ�ڣ�����TRUE,���򷵻�FALSE
		 */
		virtual bool isActivePeriod();

		void	printState();

		void    holdDragon(UserSession* pUser);
		void    holdTiger(UserSession* pUser);

		void    resetDragon();
		void    resetTiger();

		CArhat dragon; // ����
		CArhat tiger;   // ����
		BYTE   state;       	// ����״̬
		DWORD  dwCountryID;	// ����������������
};
#endif

