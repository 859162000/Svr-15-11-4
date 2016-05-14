/**
 * \file
 * \version  $Id: BillCallback.h  $
 * \author  
 * \date 
 * \brief ����Ʒѿͻ��˽ӿ�
 * <p>
 * �ṩ�Ʒѿͻ��˻ص��������Ͷ��壬<br>
 * �Լ��Ʒѿͻ���ģ���ʼ���ͻ��սӿڡ�
 * </p>
 */


#ifndef _BillCallback_h_
#define _BillCallback_h_

#ifndef _SingletonBase_
#define _SingletonBase_
template <typename T>
class SingletonBase
{
	public:
		SingletonBase() {}
		virtual ~SingletonBase() {}
		static T& getInstance()
		{
			assert(instance);
			return *instance;
		}
		static void newInstance()
		{
			SAFE_DELETE(instance);
			instance = new T();
		}
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}
	protected:
		static T* instance;
	private:
		SingletonBase(const SingletonBase&);
		SingletonBase & operator= (const SingletonBase &);
};
template <typename T> T* SingletonBase<T>::instance = NULL;
#endif

#include "UserCommand.h"
#ifdef	__cplusplus
extern "C" {
#endif
	/**
	 * \brief �Ʒѽ��׻ص�����
	 * ��ص������д����ݴ���
	 */
	struct BillData
	{ 
		unsigned int	uid;																				//UID 
		char					tid[Cmd::UserServer::SEQ_MAX_LENGTH +1];		//�������к� 
		int						at;																					//�������� 
		int						subat;																			//������
		unsigned int	point;																			//�۷ѵ���	
		int						result;																			//���׽����1�ɹ���0ʧ��
		int 					balance;																		//���
		int 					bonus;																			//����
		int	 					hadfilled;																	//������ֵ�ı�־,1=������ֵ,0=û��
		time_t				timeout;																		//��ʱʱ���
		char					cardid[Cmd::UserServer::CARDID_LENGTH +1];	//��ֵ����
		char					ip[MAX_IP_LENGTH];       										//ip��ַ
		char					remark[Cmd::UserServer::REMARK_LENGTH +1];	//��ע
	};

	/**
	 * \brief �Ʒѻص������ӿڶ���
	 * ��д��Щ�ص�������ʱ����Ҫ��֤�Ƿ�����ģʽ��<br>
	 * �����Щ��������̫�û���ɶ��������������
	 */
	struct BillCallback
	{
		/**
		 * \brief �Ʒѻص��ӿں���
		 * �Ʒѣ����������
		 * ��Ϸ���б��ػ�ȡ�Ƿ�ɹ�����д���ݿ��
		 * ��ֵ
		 * ��ѯ�����Ѽ�¼����Ϣ
		 * \param uid �˺ű��
		 * \return �Ʒ��Ƿ�ɹ�
		 */
		bool (*action)(const BillData *bd);
	};

	/**
	 * \brief ��ʼ���Ʒѿͻ���ģ��
	 * \param confile �Ʒѷ������б��ļ���
	 * \param tradelog ������־�ļ���
	 * \param bc ��ʼ�Ļص�����
	 * \return ��ʼ���Ƿ�ɹ�
	 */
	extern bool Bill_init(const char *confile, const char *tradelog, struct BillCallback *bc);
	/**
	 * \brief �����ͷżƷѿͻ���ģ�����Դ
	 * �ر����ӣ��ͷ��̵߳Ȳ���
	 */
	extern void Bill_final();
	/**
	 * \brief ʱ��ص�����
	 * ��Ҫ����ͻ������Ӷ�������
	 */
	extern void Bill_timeAction();
	/**
	 * \brief �Ʒ�
	 * ��������ң���ֵ����ѯ��
	 * \param uid �˺ű��
	 * \return �Ʒ��Ƿ�ɹ����������true����bd->tid����ν��׵�Ψһ���к�
	 */
	extern bool Bill_action(BillData *bd);

#ifdef	__cplusplus
}
#endif

#endif

