/**
 * \file
 * \version  $Id: zntop.cpp  $
 * \author 
 * \date 
 * \brief ʵ��������س���
 *
 * 
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termcap.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <getopt.h>
#include <argp.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include "zService.h"
#include "zArg.h"
#include "Zebra.h"
#include "zTime.h"
#include "CollectServerInfo.h"

/**
 * \brief ���ն�����ַ���
 *
 */
#define PUTP(x) (tputs(x, 1, putchar))


/**
 * \brief ����������س����࣬ʵʱ����ͳ�ƣ����Լ�ʱд�ļ�
 *
 * �����ʹ����Singleton���ģʽ����֤��һ�����̻���ֻ��һ�����ʵ����
 */
class zntopService : public zService
{

	public:

		/**
		 * \brief ��������
		 *
		 */
		~zntopService()
		{ 
			if (out) fclose(out);
		}

		/**
		 * \brief ��ȡ��Ψһ��ʵ��
		 *
		 * \return ���Ψһʵ��
		 */
		static zntopService &getInstance()
		{
			if (NULL == instance)
				instance = new zntopService();

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 *
		 */
		static void delInstance()
		{
			if (instance)
			{
				delete instance;
				instance = NULL;
			}
		}

		bool setup_terminal();
		void update();
		bool do_key(char c);

		/**
		 * \brief �����Ļ
		 *
		 */
		void clear_screen()
		{
			printf("%s", cl);
		}

		static void window_size(int signo);

	private:

		zntopService();

		bool init();
		bool serviceCallback();
		void final();

		static zntopService *instance;

		static const int BUFF_LEN = 200;		/**< �����С */
		static const long sleeptime = 1000;		/**< ˢ���ն���ʾ֮��ļ�������� */

		zTime started;							/**< ��ʼʱ�� */

		struct termios savetty;					/**< ������ն���Ϣ */
		struct termios rawtty;					/**< �����ԭʼ״̬�ն���Ϣ */
		char *cm;
		char *cl;
		char *top_clrtobot;
		char *top_clrtoeol;
		char *ho;
		char *md;
		char *me;
		char *mr;
		int lines;								/**< �ն����� */
		int cols;								/**< �ն����� */

		zTime writeFileTime;					/**< ���д�ļ�ʱ�� */
		std::string out_filename;				/**< д������Ϣ���ļ����ļ����� */
		int out_interval;						/**< д�ļ����Ƶ�� */
		FILE *out;								/**< �򿪵��ļ�ָ�� */

};

/**
 * \brief ���캯��
 *
 */
zntopService::zntopService()
	: zService("�����������")
{
	out= NULL;
	out_filename = Zebra::global["outfile"];
	if (atoi(Zebra::global["interval"].c_str()) > 0)
	{
		out_interval = atoi(Zebra::global["interval"].c_str());
	}
	else
		out_interval = 6;
}

/**
 * \brief ���Ψһʵ��
 *
 */
zntopService *zntopService::instance = NULL;

/**
 * \brief ��ʼ������
 *
 * \return ��ʼ���Ƿ�ɹ�
 */
bool zntopService::init()
{
	if (!zService::init())
		return false;

	Zebra::logger->debug(__PRETTY_FUNCTION__);

	//argp_parse(&argp, argc, argv, 0, 0, 0);

	if (!setup_terminal())
	{
		tcsetattr(0, TCSAFLUSH, &savetty);
		PUTP(tgoto(cm, 0, lines - 1));
		fputs("\r\n", stdout);
		if (out)
		{
			fclose(out);
			out = NULL;
		}
		return false;
	}

	window_size(0);
	clear_screen();

	::signal(SIGWINCH, window_size);
	::signal(SIGCONT, window_size);

	return true;
}

/**
 * \brief �������ص�����
 *
 * \return �ص��Ƿ�ɹ�
 */
bool zntopService::serviceCallback()
{
	struct timeval tv;
	fd_set in;
	char c;
	bool retval = true;

	ServerInfoSingleton::instance().getNetStat();
	ServerInfoSingleton::instance().getCpuInfo();

	update();

	tv.tv_sec = sleeptime / 1000;
	tv.tv_usec = (sleeptime % 1000) * 1000;

	FD_ZERO(&in);
	FD_SET(0, &in);
	if (TEMP_FAILURE_RETRY(::select(1, &in, NULL, NULL, &tv)) > 0 && read(0, &c, 1) == 1)
	{
		// keyboard pressed
		retval = do_key(c);
	}
	else
	{
		// no input, just wait for some time
		usleep(sleeptime * 1000);
	}

	return retval;
}

/**
 * \brief ����������
 *
 * ��ԭ�ն�����
 */
void zntopService::final()
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	tcsetattr(0, TCSAFLUSH, &savetty);
	PUTP(tgoto(cm, 0, lines - 1));
	fputs("\r\n", stdout);
	if (out)
	{
		fclose(out);
		out = NULL;
	}
}

/**
 * \brief ��ʼ���ն�
 *
 * return ��ʼ���ն��Ƿ�ɹ�
 */
bool zntopService::setup_terminal()
{
	const char *termtype;
	struct termios newtty;

	termtype = getenv("TERM");
	if (!termtype)
		termtype = "vt100";

	if (1 != tgetent(NULL, termtype))
	{
		fprintf(stderr, "Unknown terminal \"%s\" i $TERM\n", termtype);
		exit(1);
	}

	cm = tgetstr("cm", 0);
	cl = tgetstr("cl", 0);
	top_clrtobot = tgetstr("cd", 0);
	top_clrtoeol = tgetstr("ce", 0);
	ho = tgetstr("ho", 0);
	md = tgetstr("md", 0);
	me = tgetstr("me", 0);
	mr = tgetstr("mr", 0);

	if (-1 == tcgetattr(0, &savetty))
	{
		perror("tcgetattr() failed");
		return false;
	}

	newtty = savetty;
	newtty.c_lflag &= ~ICANON;
	newtty.c_lflag &= ~ECHO;
	newtty.c_cc[VMIN] = 1;
	newtty.c_cc[VTIME] = 0;

	if (-1 == tcsetattr(0, TCSAFLUSH, &newtty))
	{
		printf("cannot put tty into raw mode\n");
		return false;
	}

	tcgetattr(0, &rawtty);

	return true;
}

/**
 * \brief �����������ͳ����Ϣ���ն�
 *
 */
void zntopService::update()
{
	time_t interval;

	PUTP(tgoto(cm, 0, 0));

	PUTP(ho);
	//  PUTP(md);
	PUTP(top_clrtoeol);
	interval = started.elapse();
	if (interval >= (60 * 60))
		printf("uptime %lu hours, %lu minutes, %lu seconds\n",
				interval / (60 * 60),
				(interval % (60 * 60)) / 60,
				(interval % (60 * 60)) % 60);
	else if (interval >= 60)
		printf("uptime %lu minutes, %lu seconds\n", interval / 60, interval % 60);
	else
		printf("uptime %lu seconds\n", interval);
	printf("%zu network interface founded\n\n", ServerInfoSingleton::instance().if_vector.size());
	//  PUTP(me);

	PUTP(top_clrtoeol);
	printf("Cpu(s): %7s %7s %7s %7s %7s %7s %7s\n", "user", "nice", "system", "irq", "softirq", "iowait", "idle");
	printf("Cpu   : %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%%",
			(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[ServerInfoSingleton::instance().cpuInfo.CpuNum].usage.u / 10.0,
			(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[ServerInfoSingleton::instance().cpuInfo.CpuNum].usage.n / 10.0,
			(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[ServerInfoSingleton::instance().cpuInfo.CpuNum].usage.s / 10.0,
			(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[ServerInfoSingleton::instance().cpuInfo.CpuNum].usage.x / 10.0,
			(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[ServerInfoSingleton::instance().cpuInfo.CpuNum].usage.y / 10.0,
			(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[ServerInfoSingleton::instance().cpuInfo.CpuNum].usage.w / 10.0,
			(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[ServerInfoSingleton::instance().cpuInfo.CpuNum].usage.i / 10.0);
	printf("\n");
	for(ServerInfo::CpuInfo::size_type i = 0; i < ServerInfoSingleton::instance().cpuInfo.CpuNum; i++)
	{
		PUTP(top_clrtoeol);
		printf("Cpu%-3zu: %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%%",
				i,
				(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[i].usage.u / 10.0,
				(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[i].usage.n / 10.0,
				(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[i].usage.s / 10.0,
				(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[i].usage.x / 10.0,
				(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[i].usage.y / 10.0,
				(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[i].usage.w / 10.0,
				(float)ServerInfoSingleton::instance().cpuInfo.CpuUsage[i].usage.i / 10.0);
		printf("\n");
	}

	PUTP(mr);
	PUTP(top_clrtoeol);
	printf("%-8s %-24s %-24s ", "if", "rx(bps)", "tx(bps)");
	printf("\n");
	PUTP(me);

	PUTP(md);
	for(ServerInfo::NetIfInfo::size_type i = 0; i < ServerInfoSingleton::instance().if_vector.size(); i++)
	{
		char buffer[24];
		PUTP(top_clrtoeol);
		printf("%-8s ", ServerInfoSingleton::instance().if_vector[i].name);

		bzero(buffer, sizeof(buffer));
		if (ServerInfoSingleton::instance().if_vector[i].rx_speed > (1024 * 1024))
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "%.2fM", ServerInfoSingleton::instance().if_vector[i].rx_speed / (1024 * 1024));
		else if (ServerInfoSingleton::instance().if_vector[i].rx_speed > (1024))
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "%.2fK", ServerInfoSingleton::instance().if_vector[i].rx_speed / (1024));
		else
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "%.0f", ServerInfoSingleton::instance().if_vector[i].rx_speed);

		if (ServerInfoSingleton::instance().if_vector[i].rx_speed_max > (1024 * 1024))
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "/%.2fM", ServerInfoSingleton::instance().if_vector[i].rx_speed_max / (1024 * 1024));
		else if (ServerInfoSingleton::instance().if_vector[i].rx_speed_max > (1024))
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "/%.2fK", ServerInfoSingleton::instance().if_vector[i].rx_speed_max / (1024));
		else
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "/%.0f", ServerInfoSingleton::instance().if_vector[i].rx_speed_max);
		printf("%-24s ", buffer);

		bzero(buffer, sizeof(buffer));
		if (ServerInfoSingleton::instance().if_vector[i].tx_speed > (1024 * 1024))
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "%.2fM", ServerInfoSingleton::instance().if_vector[i].tx_speed / (1024 * 1024));
		else if (ServerInfoSingleton::instance().if_vector[i].tx_speed > (1024))
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "%.2fK", ServerInfoSingleton::instance().if_vector[i].tx_speed / (1024));
		else
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "%.0f", ServerInfoSingleton::instance().if_vector[i].tx_speed);

		if (ServerInfoSingleton::instance().if_vector[i].tx_speed_max > (1024 * 1024))
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "/%.2fM", ServerInfoSingleton::instance().if_vector[i].tx_speed_max / (1024 * 1024));
		else if (ServerInfoSingleton::instance().if_vector[i].tx_speed_max > (1024))
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "/%.2fK", ServerInfoSingleton::instance().if_vector[i].tx_speed_max / (1024));
		else
			snprintf(&buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer) - 1, "/%.0f", ServerInfoSingleton::instance().if_vector[i].tx_speed_max);
		printf("%-24s ", buffer);

		printf("\n");
	}
	PUTP(me);

	printf("\n");

	PUTP(mr);
	PUTP(top_clrtoeol);
	printf("%-8s %-24s %-24s ", "if", "total rx(bytes)", "total tx(bytes)");
	printf("\n");
	PUTP(me);

	PUTP(md);
	for(ServerInfo::NetIfInfo::size_type i = 0; i < ServerInfoSingleton::instance().if_vector.size(); i++)
	{
		PUTP(top_clrtoeol);
		printf("%-8s %-24Lu %-24Lu ", ServerInfoSingleton::instance().if_vector[i].name, ServerInfoSingleton::instance().if_vector[i].current.rx_bytes, ServerInfoSingleton::instance().if_vector[i].current.tx_bytes);
		printf("\n");
	}
	PUTP(me);

	printf("\n");

	PUTP(mr);
	PUTP(top_clrtoeol);
	printf("%-8s %-24s %-24s ", "if", "rx packets", "tx packets");
	printf("\n");
	PUTP(me);

	PUTP(md);
	for(ServerInfo::NetIfInfo::size_type i = 0; i < ServerInfoSingleton::instance().if_vector.size(); i++)
	{
		PUTP(top_clrtoeol);
		printf("%-8s %-24Lu %-24Lu ", ServerInfoSingleton::instance().if_vector[i].name, ServerInfoSingleton::instance().if_vector[i].rx_packets, ServerInfoSingleton::instance().if_vector[i].tx_packets);
		printf("\n");
	}
	PUTP(me);

	PUTP(tgoto(cm, 0, 1));
	printf("\n");
	PUTP(top_clrtoeol);

	//д�ļ�
	{
		zTime currentTime;
		if (writeFileTime.elapse(currentTime) >= out_interval)
		{
			writeFileTime = currentTime;
			if (NULL == out && out_filename.length())
			{
				out = fopen(out_filename.c_str(), "a+");
				if (out)
				{
					if (out) fprintf(out, "%-24s ", "time");
					for(ServerInfo::NetIfInfo::size_type i = 0; i < ServerInfoSingleton::instance().if_vector.size(); i++)
					{
						if (out) fprintf(out, "%-8s %-8s ", ServerInfoSingleton::instance().if_vector[i].name, ServerInfoSingleton::instance().if_vector[i].name);
					}
					if (out) fprintf(out, "\n");
				}
			}
			if (out)
			{
				char buffer[64];
				struct tm tmnow;
				time_t now = currentTime.sec();
				localtime_r(&now, &tmnow);
				
				strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", &tmnow);
				fprintf(out, "%-24s ", buffer);
			}
			for(ServerInfo::NetIfInfo::size_type i = 0; i < ServerInfoSingleton::instance().if_vector.size(); i++)
			{
				if (out) fprintf(out, "%-8.0f %-8.0f ", ServerInfoSingleton::instance().if_vector[i].rx_speed, ServerInfoSingleton::instance().if_vector[i].tx_speed);
			}
			if (out) fprintf(out, "\n");
			if (out) fflush(out);
		}
	}
}

/**
 * \brief ������
 * 
 * �൱�ڴ����ܼ�
 *
 */
bool zntopService::do_key(char c)
{
	if ('q' == c)
		return false;

	return true;
}

/**
 * \brief �����ն˴�С
 *
 * ͬʱҲ���ڴ��������ŵ��ź�
 *
 * \param signo �ź�
 */
void zntopService::window_size(int signo)
{
	struct winsize ws;

	if ((ioctl(1, TIOCGWINSZ, &ws) != -1)
			&& (ws.ws_col > 73)
			&& (ws.ws_row > 7))
	{
		zntopService::getInstance().cols = ws.ws_col;
		zntopService::getInstance().lines = ws.ws_row;
	}
	else
	{
		zntopService::getInstance().cols = tgetnum("co");
		zntopService::getInstance().lines = tgetnum("li");
	}

	zntopService::getInstance().clear_screen();
}

/**
 * \brief ��������
 *
 */
static const struct argp_option zntop_options[] = {
	{"outfile",		'f',	"filename",	0,	"Output filename.",											0},
	{"interval",	'i',	"seconds",	0,	"Output interval(seconds), minimum value is 2 seconds.",	0},
	{0,				0,		0,			0,	0,															0}
};

/**
 * \brief ��������������
 *
 * \param key �ؼ���
 * \param arg ֵ
 * \param state ״̬
 * \return �����Ƿ�ɹ�
 */
error_t zntop_parse_opt(int key, char *arg, struct argp_state * state)
{
	switch (key)
	{
		case 'f':
			Zebra::global["outfile"]=arg;
			break;
		case 'i':
			Zebra::global["interval"]=arg;
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/**
 * \brief ���������Ϣ
 *
 */
static char zntop_doc[] = "\nzntop\n" "\t��������ʵʱ��س���";

/**
 * \brief ����İ汾��Ϣ
 *
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING);


int main(int argc, char **argv)
{
	Zebra::logger=new zLogger();

	//����һά�����Լ���ά�����Ч��
#if 0
	const unsigned int high = 100, weight = 100, maxcount = 1000;
	std::vector<unsigned int> vec(high * weight, 999);
	{
		FunctionTime func_time(0,__PRETTY_FUNCTION__,"�������������Ҫ��ʱ��" , 32);
		for(unsigned int k = 0; k < maxcount; k++)
		{
			for(unsigned int i = 0; i < high; i++)
			{
				for(unsigned int j = 0; j < weight; j++)
				{
					unsigned int haha = vec[i * high +j];
					haha = haha * haha;
				}
			}
		}
	}

	unsigned int *one_dim = new unsigned int [high * weight];
	for(unsigned int i = 0; i < high * weight; i++)
	{
			one_dim[i] = 999;
	}
	{
		FunctionTime func_time(0,__PRETTY_FUNCTION__,"�������������Ҫ��ʱ��" , 32);
		for(unsigned int k = 0; k < maxcount; k++)
		{
			for(unsigned int i = 0; i < high; i++)
			{
				for(unsigned int j = 0; j < weight; j++)
				{
					unsigned int haha = one_dim[i * high +j];
					haha = haha * haha;
				}
			}
		}
	}
	unsigned int **two_dim = new unsigned int *[high];
	for(unsigned int i = 0; i < high; i++)
	{
		two_dim[i] = new unsigned int[weight];
		for(unsigned int j = 0; j < weight; j++)
		{
			two_dim[i][j] = 999;
		}
	}
	{
		FunctionTime func_time(0,__PRETTY_FUNCTION__,"�������������Ҫ��ʱ��" , 32);
		for(unsigned int k = 0; k < maxcount; k++)
		{
			for(unsigned int i = 0; i < high; i++)
			{
				for(unsigned int j = 0; j < weight; j++)
				{
					unsigned int haha = two_dim[i][j];
					haha = haha * haha;
				}
			}
		}
	}
	{
		FunctionTime func_time(0,__PRETTY_FUNCTION__,"�������������Ҫ��ʱ��" , 32);
		for(unsigned int k = 0; k < maxcount; k++)
		{
			for(unsigned int i = 0; i < high; i++)
			{
				for(unsigned int j = 0; j < weight; j++)
				{
					unsigned int haha = *(*(two_dim + i) + j);
					haha = haha * haha;
				}
			}
		}
	}

	return true;
#endif
	//���������в���
	zArg::getArg()->add(zntop_options, zntop_parse_opt, 0, zntop_doc);
	zArg::getArg()->parse(argc, argv);
	//Zebra::global.dump(std::cout);

	//std::string xmlStr;
	//std::cout << "(" << xmlStr.length() << ")" << xmlStr << std::endl;
	//std::cout << "=========================" << std::endl;
	//ServerInfoSingleton::instance().getServerInfo(xmlStr);
	//sleep(2);
	zntopService::getInstance().main();
	zntopService::delInstance();

	return EXIT_SUCCESS;
}

