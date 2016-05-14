/**
 * \file
 * \version  $Id: CollectServerInfo.h $
 * \author  
 * \date 
 * \brief �����ռ���������Ϣ��
 */

#ifndef	_CollectServerInfo_h_
#define	_CollectServerInfo_h_

#include <sys/sysinfo.h>

#include "zType.h"
#include "zSocket.h"
#include "zTime.h"
#include "zMisc.h"

/**
 * \brief ��������Ϣ�ɼ���
 */
class ServerInfo
{

	public:

		/**
		 * \brief ����������Ϣ�ṹ
		 */
		struct NetIf
		{
			/**
			 * \brief �ֽ�����
			 */
			struct BytesInfo
			{
				zTime tsp;						//ʱ���
				unsigned long long rx_bytes;	//�����ֽ���
				unsigned long long tx_bytes;	//�����ֽ���

				/**
				 * \brief ȱʡ���캯��
				 */
				BytesInfo() : tsp()
				{
					rx_bytes = 0;
					tx_bytes = 0;
				}
				/**
				 * \brief �������캯��
				 * \param bi ������������
				 */
				BytesInfo(const BytesInfo &bi)
				{
					tsp = bi.tsp;
					rx_bytes = bi.rx_bytes;
					tx_bytes = bi.tx_bytes;
				}
				/**
				 * \brief ���ظ�ֵ������
				 * \param bi ����ֵ��Դ����
				 * \return ��ֵ�Ժ�ṹ������
				 */
				BytesInfo &operator= (const BytesInfo &bi)
				{
					tsp = bi.tsp;
					rx_bytes = bi.rx_bytes;
					tx_bytes = bi.tx_bytes;
					return *this;
				}
			};

			char name[MAX_NAMESIZE];		//��������
			BytesInfo prev;					//�ϴ�ɨ��������Ϣ
			BytesInfo current;				//��ǰɨ��������Ϣ
			float delta;					//����ɨ����ʱ��
			unsigned long long rx_packets;	//��ǰ���յ����ݰ���
			unsigned long long tx_packets;	//��ǰ���͵����ݰ���
			float rx_speed;					//���������ٶ�
			float rx_speed_max;				//���������ٶ����ֵ
			float tx_speed;					//���������ٶ�
			float tx_speed_max;				//���������ٶ����ֵ

			/**
			 * \brief ȱʡ���캯��
			 */
			NetIf() : prev(), current()
			{
				bzero(name, sizeof(name));
				delta = 0.0;
				rx_packets = 0LL;
				tx_packets = 0LL;
				rx_speed = 0.0;
				rx_speed_max = 0.0;
				tx_speed = 0.0;
				tx_speed_max = 0.0;
			}
			/**
			 * \brief �������캯��
			 * \param net Դ����
			 */
			NetIf(const NetIf &net)
			{
				strcpy(name, net.name);
				prev = net.prev;
				current = net.current;
				delta = net.delta;
				rx_packets = net.rx_packets;
				tx_packets = net.tx_packets;
				rx_speed = net.rx_speed;
				rx_speed_max = net.rx_speed_max;
				tx_speed = net.tx_speed;
				tx_speed_max = net.tx_speed_max;
			}
			/**
			 * \brief ���ظ�ֵ������
			 * \param net Դ����
			 * \return ���ظ�ֵ�Ժ�ṹ��������
			 */
			NetIf &operator= (const NetIf &net)
			{
				strcpy(name, net.name);
				prev = net.prev;
				current = net.current;
				delta = net.delta;
				rx_packets = net.rx_packets;
				tx_packets = net.tx_packets;
				rx_speed = net.rx_speed;
				rx_speed_max = net.rx_speed_max;
				tx_speed = net.tx_speed;
				tx_speed_max = net.tx_speed_max;
				return *this;
			}
		};

		/**
		 * \brief CPU��Ϣ
		 */
		struct CpuInfo
		{
			/**
			 * \brief ����CPU����Ϣ
			 */
			struct Cpu
			{
				/**
				 * \brief ����CPU��ʹ�����
				 */
				struct TickInfo
				{
					unsigned long long u,	//user: normal processes executing in user mode
								  n,		//nice: niced processes executing in user mode
								  s,		//system: processes executing in kernel mode
								  i,		//idle: twiddling thumbs
								  w,		//iowait: waiting for I/O to complete
								  x,		//irq: servicing interrupts
								  y;		//softirq: servicing softirqs
					/**
					 * \brief ���캯��
					 */
					TickInfo()
					{
						u = 0;
						n = 0;
						s = 0;
						i = 0;
						w = 0;
						x = 0;
						y = 0;
					}
					/**
					 * \brief �������캯��
					 * \param ti ���������Դ����
					 */
					TickInfo(const TickInfo &ti)
					{
						u = ti.u;
						n = ti.n;
						s = ti.s;
						i = ti.i;
						w = ti.w;
						x = ti.x;
						y = ti.y;
					}
					/**
					 * \brief ��ֵ�����
					 * \param ti ����ֵ��Դ����
					 * \return ��ֵ�Ժ�ṹ�ĳ�������
					 */
					TickInfo &operator= (const TickInfo &ti)
					{
						u = ti.u;
						n = ti.n;
						s = ti.s;
						i = ti.i;
						w = ti.w;
						x = ti.x;
						y = ti.y;
						return *this;
					}
					/**
					 * \brief �����Ѳɼ����ݼ���������
					 * \param pv ��һ�βɼ�������
					 * \param ct ��ǰ�ɼ�������
					 */
					void calculate(const TickInfo &pv, const TickInfo &ct)
					{
						long long u_frme, s_frme, n_frme, i_frme, w_frme, x_frme, y_frme, tot_frme, tz;
						float scale;

						u_frme = ct.u - pv.u;
						s_frme = ct.s - pv.s;
						n_frme = ct.n - pv.n;
						i_frme = (tz = (long long)(ct.i - pv.i)) < 0 ? 0 : tz;
						w_frme = ct.w - pv.w;
						x_frme = ct.x - pv.x;
						y_frme = ct.y - pv.y;
						tot_frme = u_frme + s_frme + n_frme + i_frme + w_frme + x_frme + y_frme;
						if (tot_frme < 1) tot_frme = 1;
						scale = 1000.0 / (float)tot_frme;

						u = (unsigned long long)((float)u_frme * scale);
						n = (unsigned long long)((float)n_frme * scale);
						s = (unsigned long long)((float)s_frme * scale);
						i = (unsigned long long)((float)i_frme * scale);
						w = (unsigned long long)((float)w_frme * scale);
						x = (unsigned long long)((float)x_frme * scale);
						y = (unsigned long long)((float)y_frme * scale);
					}
				};

				/**
				 * \brief CPU���
				 */
				const unsigned int id;
				/**
				 * \brief ��һ�βɼ�������
				 */
				TickInfo prev;
				/**
				 * \brief ��ǰ����
				 */
				TickInfo current;
				/**
				 * \brief ���������CPU����������
				 */
				TickInfo usage;
				/**
				 * \brief ���캯��
				 * \param id CPU���
				 */
				explicit Cpu(const unsigned int id)
					: id(id), prev(), current(), usage() { }
				/**
				 * \brief �������캯��
				 * \param cpu ���������Դ����
				 */
				Cpu(const Cpu &cpu) : id(cpu.id)
				{
					prev = cpu.prev;
					current = cpu.current;
					usage = cpu.usage;
				}
				/**
				 * \brief ��ֵ������
				 * \param cpu ����ֵ��Դ����
				 * \return ��ֵ�Ժ�����ݳ�������
				 */
				Cpu &operator= (const Cpu &cpu)
				{
					prev = cpu.prev;
					current = cpu.current;
					usage = cpu.usage;
					return *this;
				}
			};

			/**
			 * \brief CPU����
			 */
			const unsigned int CpuNum;
			/**
			 * \brief ����CPUʹ���ʵ�����
			 */
			std::vector<Cpu> CpuUsage;
			/**
			 * \brief �����С����
			 */
			typedef std::vector<Cpu>::size_type size_type;

			/**
			 * \brief ���캯��
			 * \param CpuNum CPU����
			 */
			CpuInfo(unsigned int CpuNum = getCpuNum()) : CpuNum(CpuNum)
			{
				for(unsigned int i = 0; i <= CpuNum; i++)
				{
					Cpu cpu(i);
					CpuUsage.push_back(cpu);
				}
			}
		};

		/**
		 * \brief ����������Ϣ������������
		 */
		typedef std::vector<NetIf> NetIfInfo;

		/**
		 * \brief ����CPU����
		 * \return CPU����
		 */
		static inline unsigned int getCpuNum()
		{
			return (unsigned int)sysconf(_SC_NPROCESSORS_ONLN);
		}

		int getCpuInfo();
		NetIfInfo::size_type getNetStat();
		void getServerInfo(std::string &xmlStr);

		CpuInfo cpuInfo;
		NetIfInfo if_vector;
		
};

typedef singleton_default<ServerInfo> ServerInfoSingleton;

#endif

