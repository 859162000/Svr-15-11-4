/**
 * \file
 * \version  $Id: RecordSessionManager.h  $
 * \author  
 * \date 
 * \brief ���������ڱ�֤��������ظ���½
 *
 * 
 */


#ifndef _RecordSessionManager_h_
#define _RecordSessionManager_h_

#include <ext/hash_map>

#include "zType.h"
#include "zMutex.h"
#include "zTime.h"

/**
 * \brief ��ɫ������ȡд��ĻỰ��¼
 *
 */
struct RecordSession
{
	DWORD accid;			/// �ʺ�
	DWORD id;				/// ��ɫ���
	WORD  wdServerID;		/// ���������
	zTime lastsavetime;		/// ���һ�δ浵ʱ��

	/**
	 * \brief ȱʡ���캯��
	 *
	 */
	RecordSession(const DWORD accid, const DWORD id, const WORD wdServerID) : lastsavetime()
	{
		this->accid = accid;
		this->id = id;
		this->wdServerID = wdServerID;
	}

	/**
	 * \brief �������캯��
	 *
	 */
	RecordSession(const RecordSession& rs)
	{
		accid = rs.accid;
		id = rs.id;
		wdServerID = rs.wdServerID;
		lastsavetime = rs.lastsavetime;
	}

	/**
	 * \brief ��ֵ�������ţ�û��ʵ�֣����õ���
	 *
	 */
	RecordSession & operator= (const RecordSession &rs);

	const bool operator== (const RecordSession &rs);
	/*{
		return (accid == rs.accid && id == rs.id);
	}*/

};

class RecordSessionManager
{

	public:

		/**
		 * \brief Ĭ����������
		 *
		 */
		~RecordSessionManager()
		{
			sessionMap.clear();
		}

		/**
		 * \brief �������Ψһʵ��
		 *
		 * ʵ����Singleton���ģʽ����֤��һ��������ֻ��һ�����ʵ��
		 *
		 */
		static RecordSessionManager &getInstance()
		{
			if (NULL == instance)
				instance = new RecordSessionManager;

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		bool add(const DWORD accid, const DWORD id, const WORD wdServerID);
		bool verify(const DWORD accid, const DWORD id, const WORD wdServerID);
		bool remove(const DWORD accid, const DWORD id, const WORD wdServerID);
		void removeAllByServerID(const WORD wdServerID);
		bool empty()
		{
			return sessionMap.empty(); 
		}

	private:

		/**
		 * \brief ���Ψһʵ��ָ��
		 *
		 */
		static RecordSessionManager *instance;

		/**
		 * \brief Ĭ�Ϲ��캯��
		 *
		 */
		RecordSessionManager() {};

		/**
		 * \brief ������������
		 *
		 */
		typedef __gnu_cxx::hash_map<DWORD, RecordSession> RecordSessionHashmap;
		/**
		 * \brief ������������������
		 *
		 */
		typedef RecordSessionHashmap::iterator RecordSessionHashmap_iterator;
		/**
		 * \brief ����������ֵ������
		 *
		 */
		typedef RecordSessionHashmap::value_type RecordSessionHashmap_pair;
		/**
		 * \brief �洢�����ʺ��б���Ϣ������
		 *
		 */
		RecordSessionHashmap sessionMap;
		/**
		 * \brief �������
		 *
		 */
		zMutex mlock;

};

#endif

