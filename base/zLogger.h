/**
 * \file
 * \version  $Id: zLogger.h  $
 * \author  
 * \date 
 * 
 * \brief Zebra�µ���־ϵͳ�����ļ�
 *
 * ���ļ��Ƕ�����������<code>zLogger::zLevel</code>,<code>zLogger</code>,<code>zLogger::zLoggerLocalFileAppender</code>
 */

#ifndef _ZLOGGER_H_
#define _ZLOGGER_H_
#include <log4cxx/logger.h>
#include <log4cxx/dailyrollingfileappender.h>
#include <log4cxx/rolling/timebasedrollingpolicy.h>
#include <log4cxx/logstring.h>
#include <log4cxx/helpers/datetimedateformat.h>
#include "zType.h"
#include "zMutex.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

/**
 * \def MSGBUF_MAX
 * \brief Zebra��־ϵͳ����������־���� 
 */
#define MSGBUF_MAX 4096

/**
 * \brief Zebra��Ŀ����־�࣬��Log4cxx���������ġ�
 *
 * Ŀǰʵ��������д��־��ʽ��������̨�������ļ���Syslogϵͳ��Syslog�ĵȼ�Ϊuser��
 *
 * Ĭ����־������#DEBUG
 *
 * ����Ϊ�̰߳�ȫ�ࡣ
 */

namespace log4cxx
{
	namespace spi{
		namespace location{
			class LocationInfo;
			}
		}

	class XFactory:
		public virtual spi::LoggerFactory,
		public virtual helpers::ObjectImpl
	{
	public:
		DECLARE_ABSTRACT_LOG4CXX_OBJECT(XFactory)
		BEGIN_LOG4CXX_CAST_MAP()
			LOG4CXX_CAST_ENTRY(XFactory)
			LOG4CXX_CAST_ENTRY(spi::LoggerFactory)
		END_LOG4CXX_CAST_MAP()

		XFactory();
		virtual LoggerPtr makeNewLoggerInstance(
			log4cxx::helpers::Pool& pool,const log4cxx::LogString& name) const; 
	};
	
	typedef helpers::ObjectPtrT<XFactory> XFactoryPtr;

	class XLogger : public Logger
	{
	static XFactoryPtr factory;
	public:
		DECLARE_ABSTRACT_LOG4CXX_OBJECT(XLogger)
		BEGIN_LOG4CXX_CAST_MAP()
			LOG4CXX_CAST_ENTRY(XLogger)
			LOG4CXX_CAST_ENTRY_CHAIN(Logger)
		END_LOG4CXX_CAST_MAP()
		
		XLogger(log4cxx::helpers::Pool& pool,const log4cxx::LogString& name1):Logger(pool,name1) {}
		void activateOptions() {}
		
		void setName(const std::string& Name)
		{
			name = Name;
		}	
		
		static LoggerPtr getLogger(const log4cxx::LogString& name);
		
		static LoggerPtr getLogger(const log4cxx::helpers::Class& clazz);	
	};
	
	typedef helpers::ObjectPtrT<XLogger> XLoggerPtr;
};

class zLogger
{
	public:
		/**
		 * \brief zLevel�����˼�����־�ȼ�
		 *
		 * ������log4cxx�ṩ�ı�׼��־�ȼ���Ϊ��־�ȼ��⣬���Զ�������Ϸ��־�ȼ�.
		 *
		 * ������־�ȼ���ϵΪ #OFF> #FATAL> #ERROR> #WARN> #INFO> #DEBUG> #ALL
		 *
		 * ��Ϸ��־�ȼ���ϵΪ #ALARM> #IFFY> #TRACE> #GBUG
		 *
		 * ��Ϸ��־�ȼ��������־�ȼ���ϵ: #ALARM=#ERROR, #IFFY=#WARN, #TRACE=#INFO, #GBUG=#DEBUG
		 */
//			class zLevel
//			{
//				friend class zLogger;
//				private:
			/**
			 * \brief Zebra��Ŀ��֧����־�ȼ����ֶ���
			 */
//				enum zLevelInt
//				{
//					ALARM_INT	= Level::ERROR_INT,
//					IFFY_INT	= Level::WARN_INT,
//					TRACE_INT	= Level::INFO_INT,
//					GBUG_INT	= Level::DEBUG_INT
//				};
//
//				static const LevelPtr LEVELALARM;
//				static const LevelPtr LEVELIFFY;
//				static const LevelPtr LEVELTRACE;
//				static const LevelPtr LEVELGBUG;

//				LevelPtr zlevel;
//				zLevel(LevelPtr level);

//				public:
			/**
			 * \brief ��zLogger�ȼ�����ΪOFF��������#forceLog���������򲻻�����κ���־
//				 */
//				static const zLevel * OFF;
			/**
			 * \brief ��zLogger�ȼ�����ΪFATAL��ֻ���FATAL�ȼ�����־
			 *
			 * �������������Ѿ��޷��ṩ�����ķ����ܡ�
			 */
//				static const zLevel * FATAL;
			/**
			 * \brief ��zLogger�ȼ�����ΪERROR��ֻ������ڵ��ڴ˵ȼ�����־
			 *
			 * ���󣬿��ܲ����ṩĳ�ַ��񣬵����Ա�֤������ȷ���С�
			 */
//				static const zLevel * ERROR;
			/**
			 * \brief ��zLogger�ȼ�����ΪALARM����ERRORͬһ����
			 *
			 * ��������Ϸ���ݷ������󣬱����⵽����ң���Ϸ�����쳣�ȵȡ���ERRORͬһ����
			 */
//				static const zLevel * ALARM;
			/**
			 * \brief ��zLogger�ȼ�����ΪWARN��ֻ������ڵ��ڴ˵ȼ�����־
			 *
			 * ���棬ĳЩ�ط���Ҫ����ע�⣬����û�������ļ�����������Ĭ��ѡ�����ʹ�á�
			 */
//				static const zLevel * WARN;
			/**
			 * \brief ��zLogger�ȼ�����ΪIFFY��
			 *
			 * ���ɵģ���Ҫ׷���һЩ��Ϸ���ݣ�����˵һ���䶯�����ݳ���ĳ�ַ�Χ����WARNͬһ����
			 */
//				static const zLevel * IFFY;
			/**
			 * \brief ��zLogger�ȼ�����ΪINFO��ֻ������ڵ��ڴ˵ȼ�����־
			 *
			 * ��Ϣ���ṩһ����Ϣ��¼��������һЩ����״̬�ļ�¼��
			 */
//				static const zLevel * INFO;
			/**
			 * \brief ��zLogger�ȼ�����ΪTRACE
			 *
			 * 	��Ϸ���٣�����Ҫ��Ϸ�����еĹؼ����ݸ��٣������պ�������⡣
			 *
			 * 	�����ɫ������PK������������ٵȵ���Ϸ�¼�����INFOͬһ����
			 */
//				static const zLevel * TRACE;
			/**
			 * \brief ��zLogger�ȼ�����ΪDEBUG��������еȼ�����־
			 */
//				static const zLevel * DEBUG;
			/**
			 * \brief ��zLogger�ȼ�����ΪGBUG
			 *
			 * �����õ���Ϸ���ݡ���DEBUGͬһ����
			 */
//				static const zLevel * GBUG;
			/**
			 * \brief ��zLogger�ȼ�����ΪALL��������еȼ�����־
//				 */
//				static const zLevel * ALL;
//			};

		zLogger(const std::string & name="Zebra");
		~zLogger();

		const log4cxx::LogString & getName();
		void setName(const std::string & setName);
		bool addConsoleLog();
		void removeConsoleLog();
		bool addLocalFileLog(const std::string &file);
		void removeLocalFileLog(const std::string &file);
		bool addSysLog(const std::string & host);
		void removeSysLog(const std::string & host);

//		void setLevel(const log4cxx::zLevel * zLevelPtr);
		void setLevel(const std::string &level);
//		bool log(const log4cxx::zLevel * zLevelPtr,const char * pattern, ...);
//		bool forceLog(const log4cxx::zLevel * zLevelPtr,const char * pattern, ...);
		bool debug(const char * pattern, ...);
		bool error(const char * pattern, ...);
		bool error_out(const char * pattern);
		bool info(const char * pattern, ...);
		bool fatal(const char * pattern, ...);
		bool warn(const char * pattern, ...);
//		bool alarm(const char * pattern, ...);
//		bool iffy(const char * pattern, ...);
		bool trace(const char * pattern, ...);
//		bool gbug(const char * pattern, ...);

	private:
		/**
		 * \brief ����ֻΪ��������ȷ��ʱ��д�ļ���,����log4cxx::DailyRollingFileAppender�̳�ʵ�ֵ�
		 *
		 * ������#setTimeZone�������ع���#activateOptions����,�������÷���μ�log4cxx�ֲ�
		 */
//			class zLoggerLocalFileAppender:public DailyRollingFileAppender
//			{
//				private:
				/**
				 * \brief д��־�ļ������õ�ʱ��
				 */
//					static log4cxx::helpers::TimeZonePtr tz;
//				public:
//					zLoggerLocalFileAppender();
//					~zLoggerLocalFileAppender();
//
//					void setTimeZone(const std::string &timeZone);
//					void activateOptions();
//			};

//			class PowerLogger:public Logger
//			{
//				public:
//					void setName(const log4cxx::LogString  &s)
//					{
//						name=s;
//					}
				
//				protected:
//					PowerLogger(log4cxx::helpers::Pool& pool,const log4cxx::LogString &s):Logger(pool,s) { }

//			};

	//	LoggerPtr logger;
		log4cxx::XLoggerPtr logger;
		char message[MSGBUF_MAX];
		zMutex msgMut;
//		zServerInfoManager* m_info;
};

#endif
