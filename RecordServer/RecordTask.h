/**
 * \file
 * \version  $Id: RecordTask.h w $
 * \author  
 * \date 
 * \brief ���������������
 *
 */

#ifndef _RecordTask_h_
#define _RecordTask_h_

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "RecordCommand.h"
#include "zDBConnPool.h"

/**
 * \brief �����������������
 *
 */
class RecordTask : public zTCPTask
{

	public:

		/**
		 * \brief ���캯��
		 * ��Ϊ���������Ѿ�ѹ��������ͨ���ײ㴫�͵�ʱ��Ͳ���Ҫѹ����
		 * \param pool �������ӳ�ָ��
		 * \param sock TCP/IP�׽ӿ�
		 * \param addr ��ַ
		 */
		RecordTask(
				zTCPTaskPool *pool,
				const int sock,
				const struct sockaddr_in *addr = NULL) : zTCPTask(pool, sock, addr, false)
		{
			wdServerID = 0;
			wdServerType = UNKNOWNSERVER;
		}

		/**
		 * \brief ����������
		 *
		 */
		virtual ~RecordTask() {};

		int verifyConn();
		int recycleConn();
		bool msgParse(const Cmd::t_NullCmd *, const unsigned int);

		/**
		 * \brief ��ȡ���������
		 *
		 * \return ���������
		 */
		const WORD getID() const
		{
			return wdServerID;
		}

		/**
		 * \brief ��ȡ����������
		 *
		 * \return ����������
		 */
		const WORD getType() const
		{
			return wdServerType;
		}

	private:

		WORD wdServerID;					/**< ��������ţ�һ����Ψһ�� */
		WORD wdServerType;					/**< ���������� */

		bool verifyLogin(const Cmd::Record::t_LoginRecord *ptCmd);
		bool msgParse_Gateway(const Cmd::t_NullCmd *, const unsigned int);
		bool getSelectInfo(DWORD accid);
		bool msgParse_Scene(const Cmd::t_NullCmd *, const unsigned int);
		bool msgParse_Session(const Cmd::t_NullCmd*, const unsigned int);

		bool readCharBase(const Cmd::Record::t_ReadUser_SceneRecord *rev);
		bool saveCharBase(const Cmd::Record::t_WriteUser_SceneRecord *rev);

		static const dbCol charbase_define[];

#ifdef _TEST_DATA_LOG
		static const dbCol chartest_define[];
		bool readCharTest(Cmd::Record::t_Read_CharTest_SceneRecord *rev);
		bool insertCharTest(Cmd::Record::t_Insert_CharTest_SceneRecord *rev);
		bool updateCharTest(Cmd::Record::t_Update_CharTest_SceneRecord *rev);
		bool deleteCharTest(Cmd::Record::t_Delete_CharTest_SceneRecord *rev);
#endif
};

#endif

