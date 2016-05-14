/**
 * \file
 * \version  $Id: BillClient.h  $
 * \author  
 * \date 
 * \brief ����Ʒѿͻ���
 * <p>
 * �ṩ�Ʒѿͻ��˻ص��������Ͷ��壬<br>
 * �Լ��Ʒѿͻ���ģ���ʼ���ͻ��սӿڡ�
 * </p>
 */


#ifndef _BillClient_h_
#define _BillClient_h_

#include "BillCallback.h"
#include "zTCPClientTask.h"
#include "MessageQueue.h"

enum NetType
{
	NetType_near = 0,		//����·�ɣ������������ŷ���������ͨ������ͨ������
	NetType_far = 1			//Զ��·�ɣ�����������ͨ����������ͨ�������ŷ�����
};

/**
 * \brief �Ʒѿͻ�������
 */
class BillClient : public zTCPClientTask , public MessageQueue
{
	public:

		BillClient(const std::string &ip, const unsigned short port, BillCallback &bc, const unsigned int my_id);
		~BillClient();

		int checkRebound();
		void addToContainer();
		void removeFromContainer();
		bool connect();
		bool action(BillData *bd);
		bool msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool cmdMsgParse(const Cmd::t_NullCmd *, const unsigned int);

		const NetType getNetType() const
		{
			return netType;
		}

		const unsigned int getID() const
		{
			return my_id;
		}

	private:

		BillCallback &bc;
		NetType netType;
		unsigned int my_id;
		GameZone_t gameZone;
		char gameZone_str[6];

};


#endif

