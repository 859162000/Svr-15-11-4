/**
 * \file
 * \version  $Id: BillClientManager.h  $
 * \author  
 * \date 
 * \brief ����ͳһ�û�ƽ̨�Ʒѿͻ������ӵĹ�������
 */


#ifndef _BillClientManager_h_
#define _BillClientManager_h_

#include <set>

#include "zType.h"
#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "BillClient.h"
#include "zTime.h"
#include "zLogger.h"
#include "zConfile.h"

/**
 * \brief ͳһ�û�ƽ̨��½�������ļƷѿͻ��������������
 */
class BillClientManager : public SingletonBase<BillClientManager>
{

	public:

		bool init(const std::string &confile, const std::string &tradelog, BillCallback &bc);
		void timeAction(const zTime &ct);
		void add(BillClient *billClient);
		void remove(BillClient *billClient);

		bool action(BillData *bd);
		void execEvery();

		/**
		 * \brief ���׼�¼��log
		 */
		static zLogger *tlogger;

		/**
		 * \brief ��ֵ������Դ,�������ļ���ȡ��
		 */
		static int	source;

	private:

		friend class SingletonBase<BillClientManager>;
		BillClientManager();
		~BillClientManager();

		BillCallback bc;

		/**
		 * \brief �ͻ������ӹ����
		 */
		zTCPClientTaskPool *billClientPool;
		/**
		 * \brief ���ж�����������ʱ���¼
		 */
		zTime actionTimer;
		unsigned int maxID;

		struct lt_client
		{
			bool operator()(BillClient *s1, BillClient *s2) const
			{
				if (s1->getID() == s2->getID())
					return s1->getNetType() < s2->getNetType();
				else
					return s1->getID() < s2->getID();
			}
		};
		/**
		 * \brief ��������Ѿ��ɹ���������������
		 */
		typedef std::multiset<BillClient *, lt_client> BillClientContainer;
		typedef BillClientContainer::iterator iter;
		typedef BillClientContainer::const_iterator const_iter;
		typedef BillClientContainer::size_type size_type;
		/**
		 * \brief ��������Ѿ��ɹ�����������
		 */
		BillClientContainer allClients;
		/**
		 * \brief �������ʻ������
		 */
		zRWLock rwlock;

};

#endif

