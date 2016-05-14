/**
 * \file
 * \version  $Id: BillClient.cpp $
 * \author  
 * \date 
 * \brief ����Ʒѷ��������ӿͻ���
 *
 */

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "BillCommand.h"
#include "BillClient.h"
#include "GatewayServer.h"
#include "SceneClient.h"
#include "GateUserManager.h"
#include "Zebra.h"
#include "LoginSessionManager.h"
#include "GatewayTaskManager.h"

/**
 * \brief �Ʒѷ��������ӿͻ���
 *
 * һ������ֻ��һ���Ʒѷ���������������ֻ��Ҫ����һ��ָ�룬����Ҫ���ӹ�����֮��Ķ���
 *
 */
BillClient *accountClient = NULL;

/**
 * \brief ������Bill������������
 *
 * \return �����Ƿ�ɹ�
 */
bool BillClient::connectToBillServer()
{
	if (!connect())
	{
		Zebra::logger->error("����Bill������ʧ��");
		return false;
	}

	using namespace Cmd::Bill;
	t_LoginBill tCmd;
	tCmd.wdServerID = GatewayService::getInstance().getServerID();
	tCmd.wdServerType = GatewayService::getInstance().getServerType();

	return sendCmd(&tCmd, sizeof(tCmd));
}

/**
 * \brief ����zThread�еĴ��麯�������̵߳����ص����������ڴ�����յ���ָ��
 *
 */
void BillClient::run()
{
	zTCPBufferClient::run();

	while(!GatewayService::getInstance().isTerminate())
	{
		while(!connect())
		{
			Zebra::logger->error("���ӼƷѷ�����ʧ��");
			zThread::msleep(1000);
		}
		Cmd::Super::t_restart_ServerEntry_NotifyOther notify;
		notify.srcID=GatewayService::getInstance().getServerID();
		notify.dstID=this->getServerID();
		GatewayService::getInstance().sendCmdToSuperServer(&notify, sizeof(notify));
		zThread::msleep(2000);
		connect();
		using namespace Cmd::Bill;
		t_LoginBill tCmd;
		tCmd.wdServerID = GatewayService::getInstance().getServerID();
		tCmd.wdServerType = GatewayService::getInstance().getServerType();

		if(sendCmd(&tCmd, sizeof(tCmd)))
		{
			zTCPBufferClient::run();
		}
			// */
		zThread::msleep(1000);
	}
	//��Bill֮������ӶϿ�����Ҫ�رշ�����
	GatewayService::getInstance().Terminate();
}

/**
 * \brief ��������Bill������������ָ��
 *
 * \param ptNullCmd ��������ָ��
 * \param nCmdLen ��������ָ���
 * \return �����Ƿ�ɹ�
 */
bool BillClient::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::Bill;
	using namespace Cmd;

	if (CMD_GATE == ptNullCmd->cmd)
	{
		switch(ptNullCmd->para)
		{
			case PARA_GATE_LOGINVERIFY_RETURN:
				{
					t_LoginVerify_Gateway_Return *ptCmd = (t_LoginVerify_Gateway_Return *)ptNullCmd;

					if (ptCmd->retcode)
					{
						Zebra::logger->debug("��½�Ʒ���֤ͨ��%u",ptCmd->accid);
						GatewayTaskManager::getInstance().accountVerifyOK(ptCmd->accid, true);
					}
					else
					{
						Zebra::logger->error("��½�Ʒ���֤û��ͨ��%u",ptCmd->accid);
						GatewayTaskManager::getInstance().accountVerifyOK(ptCmd->accid, false);
					}

					return true;
				}
				break;
			case PARA_GATE_NEWSESSION:
				{
					t_NewSession_Gateway *ptCmd = (t_NewSession_Gateway *)ptNullCmd;
					Cmd::Super::t_NewSession_Gateway tCmd;

					Zebra::logger->info("��½��Ϣ��%u, %u, %s, %u", ptCmd->session.accid, ptCmd->session.loginTempID, ptCmd->session.pstrIP, ptCmd->session.wdPort);
					//Zebra::logger->info("get key��%u, %u, %u, %u, %u, %u, %u, %u", ptCmd->session.des_key[0], ptCmd->session.des_key[1], ptCmd->session.des_key[2], ptCmd->session.des_key[3], ptCmd->session.des_key[4], ptCmd->session.des_key[5], ptCmd->session.des_key[6], ptCmd->session.des_key[7]);
					LoginSessionManager::getInstance().put(ptCmd->session);
					tCmd.session = ptCmd->session;
					//bcopy(&ptCmd->session, &tCmd.session, sizeof(tCmd.session));

					return GatewayService::getInstance().sendCmdToSuperServer(&tCmd, sizeof(tCmd));
				}
				break;
			default:
				break;
		}
		return false;
	}
	
	if (CMD_REDEEM == ptNullCmd->cmd)
	{
		switch(ptNullCmd->para)
		{
			case PARA_GATE_REDEEM_GOLD:
				{
					t_Redeem_Gold_Gateway *ptCmd=(t_Redeem_Gold_Gateway *)ptNullCmd; 
					GateUser *pUser=GateUserManager::getInstance()->getUserByAccID(ptCmd->accid);
					if(pUser)
					{
						stRedeemGold rd;
						rd.dwNum = ptCmd->dwGold;
						rd.dwBalance = ptCmd->dwBalance;
						rd.byReturn = ptCmd->byReturn;
						pUser->forwardScene(&rd,sizeof(rd));
						//Zebra::logger->debug("Billת���������Ķһ����ָ��");
					}
					return true;
				}
				break;
			case PARA_GATE_REDEEM_MONTH_CARD:
				{
					t_Redeem_MonthCard_Gateway *ptCmd=(t_Redeem_MonthCard_Gateway *)ptNullCmd; 
					GateUser *pUser=GateUserManager::getInstance()->getUserByAccID(ptCmd->accid);
					if(pUser)
					{
						stRedeemMonthCard rd;
						rd.byReturn = ptCmd->byReturn;
						rd.dwNum = ptCmd->dwNum;
						rd.dwBalance = ptCmd->dwBalance;
						pUser->forwardScene(&rd,sizeof(rd));
						if(ptCmd->byReturn ==Cmd::REDEEM_SUCCESS) 
						{
							pUser->setVip(ptCmd->dwNum>0?true:false);
						}
						//Zebra::logger->debug("Billת���������Ķһ��¿�ָ��");
					}
					return true;
				}
				break;
			case PARA_GATE_RETURN_CARD_GOLD:
				{
					t_Return_Card_Gold *ptCmd=(t_Return_Card_Gold *)ptNullCmd; 
					GateUser *pUser=GateUserManager::getInstance()->getUserByAccID(ptCmd->accid);
					if(pUser)
					{
						stReturnCardAndGold rc;
						rc.byReturn = ptCmd->byReturn;
						rc.dwGold = ptCmd->dwGold;
						rc.dwMonthCard = ptCmd->dwMonthCard;
						pUser->forwardScene(&rc,sizeof(rc));
						if(ptCmd->byReturn ==Cmd::REDEEM_SUCCESS) 
						{
							pUser->setVip(ptCmd->dwMonthCard>0?true:false);
						}
						//Zebra::logger->debug("Billת���������Ĳ�ѯ����¿�ָ��");
					}
					return true;
				}
				break;
			case PARA_GATE_RETURN_POINT:
				{
					 
					t_Return_Point *ptCmd=(t_Return_Point *)ptNullCmd; 
					GateUser *pUser=GateUserManager::getInstance()->getUserByAccID(ptCmd->accid);
					if(pUser)
					{
						stReturnRequestPoint rc;
						rc.byReturn = ptCmd->byReturn;
						rc.dwPoint = ptCmd->dwPoint;
						pUser->forwardScene(&rc,sizeof(rc));
					}
					return true;
				}
				break;
			case PARA_GATE_RETURN_CARD:
				{
					t_Return_ObjCard *ptCmd=(t_Return_ObjCard *)ptNullCmd; 
					GateUser *pUser=GateUserManager::getInstance()->getUserByAccID(ptCmd->accid);
					if(pUser)
					{
						stReturnConSumeCardCard  ccc;
						ccc.byReturn = ptCmd->byReturn;
						ccc.byType = ptCmd->subatt;
						ccc.balance = ptCmd->balance;
						pUser->forwardScene(&ccc,sizeof(ccc));
					}
				}
				break;
			default:
				break;
		}
		return false;
	}
	if (CMD_FORWARD == ptNullCmd->cmd)
	{
		switch(ptNullCmd->para)
		{
			case PARA_FORWARD_USER:
				{
					t_Bill_ForwardUser *rev=(t_Bill_ForwardUser *)ptNullCmd;
					GateUser *pUser=(GateUser *)GateUserManager::getInstance()->getUserByAccID(rev->dwAccid);
					if(!pUser ||  !pUser->sendCmd(rev->data,rev->size))
						Zebra::logger->debug("ת��BILL�������ʺ�%ld��%u %u��Ϣʧ��",rev->dwAccid,
								((Cmd::stNullUserCmd *)rev->data)->byCmd,((Cmd::stNullUserCmd *)rev->data)->byParam);
					else
					{       
						//Zebra::logger->debug("ת��BILL�������ʺ�%ld��%u %u��Ϣ�ɹ�",rev->dwID,
						//		((Cmd::stNullUserCmd *)rev->data)->byCmd,((Cmd::stNullUserCmd *)rev->data)->byParam);
					}
					return true;
				}
				break;
			case PARA_FORWARD_BILL_TO_SCENE:
				{
					t_Bill_ForwardBillToScene *rev =(t_Bill_ForwardBillToScene*)ptNullCmd;
					GateUser *pUser=(GateUser *)GateUserManager::getInstance()->getUserByAccID(rev->id);
					if(!pUser || !pUser->forwardSceneBill((const Cmd::stNullUserCmd*)rev->data,(unsigned int)rev->size))
					{
						Zebra::logger->debug("ת��BILL�������ʺ�%ld�ĳ���%u %u��Ϣʧ��",rev->id,
								((Cmd::stNullUserCmd *)rev->data)->byCmd,((Cmd::stNullUserCmd *)rev->data)->byParam);
					}
				}
				break;

		}
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

