/**
 * \file
 * \version  $Id: zLogger.h  $
 * \author  
 * \date 
 * 
 * \brief Zebra新的日志系统声明文件
 *
 * 本文件是定义了三个类<code>zLogger::zLevel</code>,<code>zLogger</code>,<code>zLogger::zLoggerLocalFileAppender</code>
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
 * \brief Zebra日志系统定义的最大日志长度 
 */
#define MSGBUF_MAX 4096

/**
 * \brief Zebra项目的日志类，以Log4cxx基础构建的。
 *
 * 目前实现了三种写日志方式，即控制台、本地文件和Syslog系统。Syslog的等级为user。
 *
 * 默认日志级别是#DEBUG
 *
 * 此类为线程安全类。
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
		 * \brief zLevel声明了几个日志等级
		 *
		 * 除了用log4cxx提供的标准日志等级作为日志等级外，还自定义了游戏日志等级.
		 *
		 * 程序日志等级关系为 #OFF> #FATAL> #ERROR> #WARN> #INFO> #DEBUG> #ALL
		 *
		 * 游戏日志等级关系为 #ALARM> #IFFY> #TRACE> #GBUG
		 *
		 * 游戏日志等级与程序日志等级关系: #ALARM=#ERROR, #IFFY=#WARN, #TRACE=#INFO, #GBUG=#DEBUG
		 */
//			class zLevel
//			{
//				friend class zLogger;
//				private:
			/**
			 * \brief Zebra项目所支持日志等级数字定义
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
			 * \brief 当zLogger等级设置为OFF，除了用#forceLog函数，否则不会输出任何日志
//				 */
//				static const zLevel * OFF;
			/**
			 * \brief 当zLogger等级设置为FATAL，只输出FATAL等级的日志
			 *
			 * 程序致命错误，已经无法提供正常的服务功能。
			 */
//				static const zLevel * FATAL;
			/**
			 * \brief 当zLogger等级设置为ERROR，只输出大于等于此等级的日志
			 *
			 * 错误，可能不能提供某种服务，但可以保证程序正确运行。
			 */
//				static const zLevel * ERROR;
			/**
			 * \brief 当zLogger等级设置为ALARM，与ERROR同一级别
			 *
			 * 报警，游戏数据发生错误，比如检测到有外挂，游戏数据异常等等。与ERROR同一级别。
			 */
//				static const zLevel * ALARM;
			/**
			 * \brief 当zLogger等级设置为WARN，只输出大于等于此等级的日志
			 *
			 * 警告，某些地方需要引起注意，比如没有配置文件，但程序用默认选项可以使用。
			 */
//				static const zLevel * WARN;
			/**
			 * \brief 当zLogger等级设置为IFFY。
			 *
			 * 可疑的，需要追查的一些游戏数据，比如说一个变动的数据超出某种范围。与WARN同一级别。
			 */
//				static const zLevel * IFFY;
			/**
			 * \brief 当zLogger等级设置为INFO，只输出大于等于此等级的日志
			 *
			 * 信息，提供一般信息记录，多用于一些程序状态的记录。
			 */
//				static const zLevel * INFO;
			/**
			 * \brief 当zLogger等级设置为TRACE
			 *
			 * 	游戏跟踪，几乎要游戏中所有的关键数据跟踪，便于日后查找问题。
			 *
			 * 	比如角色升级，PK死亡，任务跟踪等等游戏事件。与INFO同一级别。
			 */
//				static const zLevel * TRACE;
			/**
			 * \brief 当zLogger等级设置为DEBUG，输出所有等级的日志
			 */
//				static const zLevel * DEBUG;
			/**
			 * \brief 当zLogger等级设置为GBUG
			 *
			 * 调试用的游戏数据。与DEBUG同一级别。
			 */
//				static const zLevel * GBUG;
			/**
			 * \brief 当zLogger等级设置为ALL，输出所有等级的日志
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
		 * \brief 本类只为了能以正确的时间写文件名,而从log4cxx::DailyRollingFileAppender继承实现的
		 *
		 * 增加了#setTimeZone函数和重构了#activateOptions函数,其他的用法请参见log4cxx手册
		 */
//			class zLoggerLocalFileAppender:public DailyRollingFileAppender
//			{
//				private:
				/**
				 * \brief 写日志文件名所用的时区
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
