/**
 * \file
 * \version  $Id: CountryInfo.cpp $
 * \author 
 * \date 
 * \brief 
 *
 * 
 */
#include "CountryInfo.h"
#include "zSubNetService.h"
#include "Zebra.h"
#include "zMisc.h"
#include "GatewayServer.h"
#include "GatewayTask.h"
#include "BillClient.h"
#include "SceneClient.h"
#include "RecordClient.h"
#include "GateUserManager.h"
#include "zConfile.h"
#include "LoginSessionManager.h"
#include "GatewayTaskManager.h"
#include "TimeTick.h"
#include "RoleregCommand.h"
#include "RecordClient.h"
#include "zXMLParser.h"


/**
 * \brief ���ݹ���id�õ�������Ϣ
 *
 *
 * \param country_id: ����id
 * \return ������Ϣ
 */
CountryInfo::Info *CountryInfo::getInfo(unsigned int country_id)
{
	for(StrVec_iterator iter = country_info.begin() ; iter != country_info.end() ; iter ++)
	{
		if(iter->countryid == country_id)
		{
			return &*iter;
		}
	}
	return NULL;
}
/**
 * \brief �������ļ��ж�ȡ������Ϣ
 *
 * \return ��ȡ�Ƿ�ɹ�
 */
bool CountryInfo::reload()
{
	zXMLParser parser=zXMLParser();
	std::string scenesinfofile=Zebra::global["sceneinfofile"];
	Zebra::logger->debug("%s", scenesinfofile.c_str());
	if(scenesinfofile=="")
		scenesinfofile="scenesinfo.xml";
	if(parser.initFile(scenesinfofile.c_str()))
	{
		xmlNodePtr root=parser.getRootNode("ScenesInfo");
		xmlNodePtr countryNode=parser.getChildNode(root,"countryinfo");
		if(countryNode)
		{
			xmlNodePtr subnode = parser.getChildNode(countryNode , "country");
			while(subnode)
			{
				if(strcmp((char*)subnode->name , "country") == 0)
				{
					CountryDic info;
					bzero(&info , sizeof(info));
					parser.getNodePropNum(subnode , "id" , &info.id , sizeof(info.id));
					parser.getNodePropStr(subnode , "name" , info.name , sizeof(info.name));
					parser.getNodePropNum(subnode , "mapID" , &info.mapid , sizeof(info.mapid));
					parser.getNodePropNum(subnode , "function" , &info.function , sizeof(info.function));
					Zebra::logger->info("���¶�ȡ��������(%u , %s , %u , %u)" , info.id , info.name ,info.mapid ,info.function);
					CountryMap_iter iter = country_dic.find(info.id);
					if(iter !=country_dic.end())
					{
							Zebra::logger->debug("reload�����ֵ�ɹ�%s",info.name);
						iter->second.function=info.function;
					}
					for(StrVec_iterator iter_1 = country_info.begin() ; iter_1 != country_info.end() ; iter_1 ++)
					{
						if(info.id == (*iter_1).countryid)
						{
							Zebra::logger->debug("reload������Ϣ�ɹ�%s",info.name);
							(*iter_1).function=info.function;
							break;
						}
					}
				}
				subnode = parser.getNextNode(subnode, NULL);
			}
		}
	}
	return true;
}
/**
 * \brief �������ļ��ж�ȡ������Ϣ
 *
 * \return ��ȡ�Ƿ�ɹ�
 */
bool CountryInfo::init()
{
	bool inited = false;
	zXMLParser parser=zXMLParser();
	std::string scenesinfofile=Zebra::global["sceneinfofile"];
	Zebra::logger->debug("%s", scenesinfofile.c_str());
	if(scenesinfofile=="")
		scenesinfofile="scenesinfo.xml";
	if(parser.initFile(scenesinfofile.c_str()))
	{
		xmlNodePtr root=parser.getRootNode("ScenesInfo");
		xmlNodePtr mapNode=parser.getChildNode(root,"mapinfo");
		if(mapNode)
		{
			xmlNodePtr subnode = parser.getChildNode(mapNode , "map");
			while(subnode)
			{
				if(strcmp((char*)subnode->name , "map") == 0)
				{
					MapDic info;
					bzero(&info , sizeof(info));
					parser.getNodePropNum(subnode , "mapID" , &info.id , sizeof(info.id));
					parser.getNodePropStr(subnode , "name" , info.name , sizeof(info.name));
					parser.getNodePropStr(subnode , "fileName" , info.filename , sizeof(info.filename));
					parser.getNodePropNum(subnode , "backto" , &info.backto , sizeof(info.backto));
					Zebra::logger->info("���ص�ͼ����(%u , %s , %s , %u)",info.id,info.name, info.filename , info.backto);
					map_dic.insert(MapMap_value_type(info.id , info));
				}
				subnode = parser.getNextNode(subnode, NULL);
			}
		}
		xmlNodePtr countryNode=parser.getChildNode(root,"countryinfo");
		if(countryNode)
		{
			xmlNodePtr subnode = parser.getChildNode(countryNode , "country");
			while(subnode)
			{
				if(strcmp((char*)subnode->name , "country") == 0)
				{
					CountryDic info;
					bzero(&info , sizeof(info));
					parser.getNodePropNum(subnode , "id" , &info.id , sizeof(info.id));
					parser.getNodePropStr(subnode , "name" , info.name , sizeof(info.name));
					parser.getNodePropNum(subnode , "mapID" , &info.mapid , sizeof(info.mapid));
					parser.getNodePropNum(subnode , "function" , &info.function , sizeof(info.function));
					Zebra::logger->info("���ع�������(%u , %s , %u , %u)" , info.id , info.name ,info.mapid ,info.function);
					country_dic.insert(CountryMap_value_type(info.id , info));
					Info info_1;
					info_1.countryid = info.id;
					info_1.countryname = info.name;
					info_1.function = info.function;
					if(info.mapid)
					{
						MapMap_iter map_iter = map_dic.find(info.mapid);
						if(map_iter == map_dic.end())
						{
							Zebra::logger->error("�õ���ͼ����ʧ��");
							continue;
						}
						inited = true;
						info_1.mapname = info_1.countryname + "��" + map_iter->second.name;
					}
					country_info.push_back(info_1);
					Zebra::logger->debug("country_info.size()=%d",country_info.size());
				}
				subnode = parser.getNextNode(subnode, NULL);
			}
		}
	}
	if(inited)
	{
		for(StrVec_iterator iter = country_info.begin() ; iter != country_info.end() ; iter++)
		{
			Zebra::logger->info("��ȡ������Ϣ:%s(%d),%s" , 
					(*iter).countryname.c_str() , (*iter).countryid , (*iter).mapname.c_str());
		}
	}
	return inited;
}

/**
 * \brief ���ù�������
 *
 * \param ptCmd ����ָ��
 */
void CountryInfo::setCountryOrder(Cmd::Session::CountrOrder *ptCmd)
{
	mutex.lock(); 
	bzero(country_order , sizeof(country_order));
	for(int i = 0 ; i < (int)ptCmd->size ; i++)
	{
		country_order[i]=ptCmd->order[i].country;
	}
	mutex.unlock(); 
}
/**
 * \brief �õ����й�������
 *
 *
 * \param buf:����������Ƶ�buf
 * \return ��ȡ���Ĺ�������
 */
int CountryInfo::getAll(char *buf)
{
	int size = 0;
	if(!buf)
	{
		return size;
	}
	mutex.lock(); 
	Cmd::Country_Info *info = (Cmd::Country_Info*)buf;
	for(StrVec_iterator iter = country_info.begin() ; iter != country_info.end() ; iter++)
	{
		bool ok=true;
		for(int i = 0; country_order[i] != 0; i ++)
		{
			if((*iter).countryid == country_order[i])
			{
				ok=false;
				break;
			}
		}
		if(ok)
		{
			//���������ҿ���ע��
			//if(!((*iter).function & 0x1))
			//{
				info[size].id = (*iter).countryid;
				info[size].enableLogin = ((*iter).function & 0x2)==0?1:0;
				info[size].enableRegister = ((*iter).function & 0x1)==0?1:0;
				bcopy((*iter).countryname.c_str() , info[size].pstrName , MAX_NAMESIZE);
				size++;
			//}
		}
	}
	for(int i = 0; country_order[i] != 0; i ++)
	{
		for(StrVec_iterator iter = country_info.begin() ; iter != country_info.end() ; iter++)
		{
			if((*iter).countryid == country_order[i])
			{
				//���������ҿ���ע��
				//if(!((*iter).function & 0x1))
				//{
					info[size].id = (*iter).countryid;
					info[size].enableLogin = ((*iter).function & 0x2)==0?1:0;
					info[size].enableRegister = ((*iter).function & 0x1)==0?1:0;
					bcopy((*iter).countryname.c_str() , info[size].pstrName , MAX_NAMESIZE);
					size++;
					break;
				//}
			}
		}
	}
	mutex.unlock(); 
	return size;
}
/**
 * \brief �õ���������
 *
 * \return ��������
 */
int CountryInfo::getCountrySize()
{
	return country_info.size();
}
/**
 * \brief ����hash���mapid�õ������ļ��б����mapid
 *
 * \param map_id: hash���mapid
 * \return �����ļ��е�mapid
 */
unsigned int CountryInfo::getRealMapID(unsigned int map_id)
{
	return map_id & 0x0000FFFF;
}
/**
 * \brief ����hash��ĵ�ͼ���Ƶõ������ļ��еĵ�ͼ����
 *
 * \param name: hash��ĵ�ͼ����
 * \return �����ļ��еĵ�ͼ����
 */
const char *CountryInfo::getRealMapName(const char *name)
{
	const char *real = strstr(name , "��");
	if( real != NULL)
	{
		return real + 2;
	}
	else
	{
		return name;
	}
}
/**
 * \brief ������id�Ƿ�Ϸ�
 *
 *
 * \param country_id:����id
 * \return ������ڸù���id���ع���id,���򷵻�-1
 */
unsigned int CountryInfo::getCountryID(unsigned int country_id)
{
	Info *info = getInfo(country_id);
	if(info)
	{
		return info->countryid;
	}
	return (DWORD)-1;
}
/**
 * \brief ���ݹ���id�õ���������
 *
 *
 * \param country_id:����id
 * \return �ҵ����ع������Ʒ��򷵻�""
 */
std::string CountryInfo::getCountryName(unsigned int country_id)
{
	Info *info = getInfo(country_id);
	if(info)
	{
		return info->countryname;
	}
	return "";
}
/**
 * \brief ���߹���id�õ��ù��ҳ�����map������
 *
 *
 * \param country_id:��������
 * \return �����ص�ͼ����
 */
std::string CountryInfo::getMapName(unsigned int country_id)
{
	Info *info = getInfo(country_id);
	if(info)
	{
		return info->mapname;
	}
	return "";
}

/**
 * \brief ���ݹ���id�õ��ù����Ƿ������½
 * \param country_id:��������
 * \return true���� false������
 */
bool CountryInfo::isEnableLogin(unsigned int country_id)
{
	Info *info = getInfo(country_id);
	if(info)
	{
		return !(info->function&0x2);
	}
	return false;
}

/**
 * \brief ���ݹ���id�õ��ù����Ƿ�����ע��
 * \param country_id:��������
 * \return true���� false������
 */
bool CountryInfo::isEnableRegister(unsigned int country_id)
{
	Info *info = getInfo(country_id);
	if(info)
	{
		return !(info->function&0x1);
	}
	return false;
}

/**
 * \brief ���ݹ���ID����Թ���function��״̬���
 * \param country_id:��������
 */
void CountryInfo::processChange(GateUser *pUser, Cmd::Scene::t_ChangeCountryStatus *rev)
{
	if (!rev||!pUser) return;
	switch(rev->oper)
	{
		case Cmd::Scene::ENABLE_REGISTER:
			{
				Info *info = getInfo(rev->country);
				if(info)
				{
					info->function = info->function&(~0x1);
				}
			}
			break;
		case Cmd::Scene::DISABLE_REGISTER:
			{
				Info *info = getInfo(rev->country);
				if(info)
				{
					info->function = info->function|0x1;
				}
			}
			break;
		case Cmd::Scene::ENABLE_LOGIN:
			{
				Info *info = getInfo(rev->country);
				if(info)
				{
					info->function = info->function&(~0x2);
				}
			}
			break;
		case Cmd::Scene::DISABLE_LOGIN:
			{
				Info *info = getInfo(rev->country);
				if(info)
				{
					info->function = info->function|0x2;
				}
			}
			break;
		case Cmd::Scene::LIST_COUNTRY_INFO:
			{
				for(StrVec_iterator iter = country_info.begin() ; iter != country_info.end() ; iter++)
				{
					Cmd::stChannelChatUserCmd send;
					send.dwType=Cmd::CHAT_TYPE_SYSTEM;
					send.dwSysInfoType = Cmd::INFO_TYPE_GAME;
					bzero(send.pstrName, sizeof(send.pstrName));
					bzero(send.pstrChat, sizeof(send.pstrChat));

					if (pUser)
					{
						sprintf((char*)send.pstrChat, "����:%s ID:%u ע��:%s ��¼:%s", 
						(*iter).countryname.c_str(), (*iter).countryid, (((*iter).function&0x1)==0)?"��":"��", (((*iter).function&0x2)==0)?"��":"��");
						pUser->sendCmd(&send, sizeof(send));
					}
				}
			}
			break;
		default:
			break;
	}
}
