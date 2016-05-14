/**
 * \file
 * \version  $Id: zLogger.cpp  $
 * \author  
 * \date 
 * \brief Zebra��Ŀ��־ϵͳ�����ļ�
 *
 */

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include "zLogger.h"
#include "zTime.h"
#include <stdarg.h>
#include <sys/stat.h>

#include <log4cxx/level.h>
#include <log4cxx/helpers/loglog.h>
#include <log4cxx/logger.h>
#include <log4cxx/rollingfileappender.h>
#include <log4cxx/dailyrollingfileappender.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/rolling/rolloverdescription.h>
#include <log4cxx/helpers/synchronized.h>
#include <log4cxx/helpers/fileoutputstream.h>
#include <log4cxx/helpers/date.h>
#include <log4cxx/logstring.h>
#include <log4cxx/spi/loggerfactory.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/spi/location/locationinfo.h>
#include <log4cxx/helpers/dateformat.h>
#include <log4cxx/net/syslogappender.h>
#include <locale.h>
#include <iostream>
#include <time.h>

using namespace log4cxx::net;

class LocalDailyRollingFileAppender : public log4cxx::DailyRollingFileAppender
{
	public:
		LocalDailyRollingFileAppender();
		virtual ~LocalDailyRollingFileAppender();
		std::string filenamepattern;

		bool relinkfile(log4cxx::helpers::Pool& pool);
		bool my_rollover(log4cxx::helpers::Pool& p);

		virtual void subAppend(const log4cxx::spi::LoggingEventPtr& event,log4cxx::helpers::Pool& pool);
		virtual void activateOptions(log4cxx::helpers::Pool& pool);
};

class LocalTimeBasedRollingPolicy : public log4cxx::rolling::TimeBasedRollingPolicy
{
	public:
		std::string getCurFileName(log4cxx::helpers::Pool& pool);
		~LocalTimeBasedRollingPolicy(){}	
};

LocalDailyRollingFileAppender::LocalDailyRollingFileAppender()
{}

LocalDailyRollingFileAppender::~LocalDailyRollingFileAppender()
{
	close();
}

bool LocalDailyRollingFileAppender::relinkfile(log4cxx::helpers::Pool& pool)
{
	using namespace log4cxx;
	using namespace log4cxx::rolling;
	using namespace log4cxx::helpers;
	using namespace log4cxx::pattern;

	LocalTimeBasedRollingPolicy policy;
	policy.setFileNamePattern(filenamepattern);
	policy.activateOptions(pool);
	std::string symlinkname = getFile();
	std::string filename = policy.getCurFileName(pool);
	
	struct stat statbuf;
	if(::lstat(symlinkname.c_str(),&statbuf) >= 0)
	{
		if(!S_ISLNK(statbuf.st_mode))
		{
			std::string msg = symlinkname;
			msg += "�ļ��Ѵ��ڣ������Ƿ�������";
			log4cxx::helpers::LogLog::warn(LOG4CXX_STR(msg));
			return false;
		}
	}

	if(-1 == ::unlink(symlinkname.c_str()))
	{}
		
	if(-1 == ::symlink(filename.c_str(),symlinkname.c_str()))
	{
		std::string msg = symlinkname;
		msg += "������������ʧ��";
		msg += symlinkname;
		msg += "-->>";
		msg += filename;
		log4cxx::helpers::LogLog::warn(LOG4CXX_STR(msg));
		return false;
	}
}

bool LocalDailyRollingFileAppender::my_rollover(log4cxx::helpers::Pool& p)
{
	using namespace log4cxx;
	using namespace log4cxx::rolling;
	using namespace log4cxx::helpers;
	using namespace log4cxx::spi;

	if(rollingPolicy != NULL)
	{
		{
			synchronized sync(mutex);
			try{
				RolloverDescriptionPtr rollover1(rollingPolicy->rollover(getFile(),p));
				if(rollover1 != NULL)
				{
					if(rollover1->getActiveFileName() == getFile())
					{
						closeWriter();
						bool success = true;
						this->relinkfile(p);
						if(success)
						{
							if(rollover1->getAppend()){
							fileLength = File().setPath(rollover1->getActiveFileName()).length(p);
							}else{fileLength = 0; }
					
						ActionPtr asyncAction(rollover1->getAsynchronous());
						if(asyncAction != NULL)
						{
							asyncAction->execute(p);
						}
						setFile(rollover1->getActiveFileName(),rollover1->getAppend(),bufferedIO,bufferSize,p);
						}
						else{
							setFile(rollover1->getActiveFileName(),true,bufferedIO,bufferSize,p);
							}
						}else{
							OutputStreamPtr os(new FileOutputStream(rollover1->getActiveFileName(),rollover1->getAppend()));
							log4cxx::helpers::WriterPtr newWriter(createWriter(os));
							closeWriter();
							setFile(rollover1->getActiveFileName());
							setWriter(newWriter);
							
							this->relinkfile(p);	
							if(rollover1->getAppend()){
								fileLength = File().setPath(rollover1->getActiveFileName()).length(p);
							}else{
								fileLength = 0;
								}

							ActionPtr asyncAction(rollover1->getAsynchronous());
							if(asyncAction!=NULL)
							{asyncAction->execute(p);}
						
							writeHeader(p);	
					}
					
					return true;
				}
			} catch(std::exception &ex){LogLog::warn(LOG4CXX_STR("excption during rollover"));}
		}
	}
}

void LocalDailyRollingFileAppender::subAppend(const log4cxx::spi::LoggingEventPtr& event,log4cxx::helpers::Pool& p)
{
	std::string filename;
	std::string filename2;
	
	if(triggeringPolicy->isTriggeringEvent(this,event,getFile(),getFileLength()))
	{	
		my_rollover(p);
	}	
	FileAppender::subAppend(event,p);
}
	
void LocalDailyRollingFileAppender::activateOptions(log4cxx::helpers::Pool& pool)
{
	using namespace log4cxx;
	using namespace log4cxx::rolling;
	using namespace log4cxx::helpers;
	using namespace log4cxx::pattern;

	LocalTimeBasedRollingPolicy* policy = new LocalTimeBasedRollingPolicy();
	log4cxx::LogString pattern(getFile());
	
	bool inLiteral = false;
	bool inPattern = false;
	
	for(size_t i = 0;i<datePattern.length();i++){
		if(datePattern[i] == 0x27){
			inLiteral = !inLiteral;
		if(inLiteral && inPattern){
			pattern.append(1,(logchar)0x7D);
			inPattern = false;
			}
		}else{
			if(!inLiteral && !inPattern){
				const logchar dbrace[] = {0x25,0x64,0x7B,0};
				pattern.append(dbrace);
				inPattern = true;
			}
			
			pattern.append(1,datePattern[i]);	
		}
	}

	if(inPattern){
		pattern.append(1,(logchar)0x7D);
		}

	policy->setFileNamePattern(pattern);
	filenamepattern = pattern;
	setTriggeringPolicy(policy);
	setRollingPolicy(policy);
	if(this->relinkfile(pool))
	{
		RollingFileAppenderSkeleton::activateOptions(pool);
	}	

}

std::string LocalTimeBasedRollingPolicy::getCurFileName(log4cxx::helpers::Pool& pool)
{
	using namespace log4cxx;
	using namespace log4cxx::rolling;
	using namespace log4cxx::helpers;
	using namespace log4cxx::pattern;
	
	log4cxx::LogString buf;
	ObjectPtr obj(new Date());
	formatFileName(obj,buf,pool);
	
	return buf;
}

using namespace log4cxx;
using namespace log4cxx::spi;

IMPLEMENT_LOG4CXX_OBJECT(XLogger)
IMPLEMENT_LOG4CXX_OBJECT(XFactory)

namespace log4cxx
{
	XFactoryPtr XLogger::factory = new XFactory();
	
	LoggerPtr XLogger::getLogger(const log4cxx::LogString& name)
	{
		return LogManager::getLogger(name,factory);
	}

	LoggerPtr XLogger::getLogger(const log4cxx::helpers::Class& clazz)
	{
		return XLogger::getLogger(clazz.getName());
	}

	XFactory::XFactory()
	{
	}
	
	LoggerPtr XFactory::makeNewLoggerInstance(log4cxx::helpers::Pool& pool,const log4cxx::LogString & name) const
	{return new XLogger(pool,name);}
	
};
/*const LevelPtr zLogger::zLevel::LEVELALARM(new Level(ALARM_INT,log4cxx::LogString("ALARM"),3));
const LevelPtr zLogger::zLevel::LEVELIFFY(new Level(IFFY_INT,log4cxx::LogString("IFFY"),3));
const LevelPtr zLogger::zLevel::LEVELTRACE(new Level(TRACE_INT,log4cxx::LogString("TRACE"),3));
const LevelPtr zLogger::zLevel::LEVELGBUG(new Level(GBUG_INT,log4cxx::LogString("GBUG"),3));

const zLogger::zLevel *  zLogger::zLevel::OFF=new zLevel(log4cxx::Level::OFF_INT);
const zLogger::zLevel *  zLogger::zLevel::FATAL=new zLevel(log4cxx::Level::FATAL_INT);
const zLogger::zLevel *  zLogger::zLevel::ALARM=new zLevel(LEVELALARM);
const zLogger::zLevel *  zLogger::zLevel::ERROR=new zLevel(log4cxx::Level::ERROR_INT);
const zLogger::zLevel *  zLogger::zLevel::IFFY=new zLevel(LEVELIFFY);
const zLogger::zLevel *  zLogger::zLevel::WARN=new zLevel(log4cxx::Level::WARN_INT);
const zLogger::zLevel *  zLogger::zLevel::TRACE=new zLevel(LEVELTRACE);
const zLogger::zLevel *  zLogger::zLevel::INFO=new zLevel(log4cxx::Level::INFO_INT);
const zLogger::zLevel *  zLogger::zLevel::GBUG=new zLevel(LEVELGBUG);
const zLogger::zLevel *  zLogger::zLevel::DEBUG=new zLevel(log4cxx::Level::DEBUG_INT);
const zLogger::zLevel *  zLogger::zLevel::ALL=new zLevel(log4cxx::Level::ALL_INT);
*/
/**
 * \brief zLevel���캯��
 * \param  level �ȼ����֣����ڲ�����
 */
//zLogger::zLevel::zLevel(LevelPtr level):zlevel(level)
//{
//}


//TimeZonePtr zLogger::zLoggerLocalFileAppender::tz(TimeZone::getDefault());

/**
 * \brief ����һ�������ļ�Appender 
 */
//zLogger::zLoggerLocalFileAppender::zLoggerLocalFileAppender()
//{
//}

/**
 * \brief ����ʱ������DateFormat�ڴ�
 */
//zLogger::zLoggerLocalFileAppender::~zLoggerLocalFileAppender()
//{
//	SAFE_DELETE(df);
//}

/**
 * \brief ����ʱ��
 *
 * \param timeZone ʱ���ַ��� 
 */
//void zLogger::zLoggerLocalFileAppender::setTimeZone(const std::string &timeZone)
//{
//	tz=TimeZone::getTimeZone(timeZone);
//}

/**
 * \brief ���������õ�ѡ��
 */
//void zLogger::zLoggerLocalFileAppender::activateOptions()
//{
//	log4cxx::PatternLayoutPtr rc = ZEBRA_NEW log4cxx::PatternLayout("\%d{\%y\%m\%d-\%H:\%M:%S }%c %5p: %m%n");
	//����ʱ����Ԥ���ļ�
/*	rc.setTimeZone(tz);
	std::string ss;
	zRTime::getLocalTZ(ss);
	char tzstr[64];
	strncpy(tzstr,"TZ=",3);
	strncpy(tzstr+3,ss.c_str(),60);
	DailyRollingFileAppender::activateOptions();
*/	//����DailyRollingFileAppender::activateOptions()���޸�ʱ��������������ִ��������޸Ļ��� 
/*	putenv(tzstr);
	tzset();
	if (!datePattern.empty() && !fileName.empty())
	{
		SAFE_DELETE(df);
		df=new helpers::DateFormat(datePattern,tz);

		int64_t lastModified = 0;
		struct stat fileStats;
		if (::stat(T2A(fileName.c_str()), &fileStats) == 0)
			lastModified = (int64_t)fileStats.st_mtime * 1000; 

		scheduledFilename = fileName + df->format(lastModified);
	} 
}*/

/**
 * \brief ����һ��zLogger 
 *
 * \param  name zLogger�����֣�����������������־�е�ÿһ��
 */
zLogger::zLogger(const std::string &name)
{
	///std::cout << __PRETTY_FUNCTION__ << std::endl;
	bzero(message, sizeof(message));
//	logger = Logger::getLogger(name);
	logger = log4cxx::XLogger::getLogger(name);

	addConsoleLog();
	setLevel("debug");
}

/**
 * \brief ��������
 */
zLogger::~zLogger()
{
}

/**
 * \brief �õ�Logger�����֣���������ÿ����־��Ϣ��
 * \return	Logger����
 */
const log4cxx::LogString & zLogger::getName()
{
	return logger->getName();
}

/**
 * \brief ����Logger�����֣���������ÿ����־��Ϣ��
 * \param Ҫ�����õ�����
 */
void zLogger::setName(const std::string & Name)
{
	//PowerLogger *log=(PowerLogger *)logger;
	//PowerLogger * pl=logger.p;
//	((PowerLogger *)logger.p)->setName(setName);
	logger->setName(Name);
}

/**
 * \brief ��ӿ���̨���Log
 * \return	�ɹ�����true�����򷵻�false 
 */
bool zLogger::addConsoleLog()
{
	PatternLayoutPtr cpl = new PatternLayout("\%d{\%y\%m\%d-\%H:\%M:\%S }%c %5p: %m%n");
	std::string s;
//	cpl->setTimeZone(zRTime::getLocalTZ(s));
	log4cxx::helpers::Pool pool;
	cpl->activateOptions(pool);

	ConsoleAppenderPtr carp = new ConsoleAppender(cpl);
	carp->setEncoding(LOG4CXX_STR("utf-8"));
	carp->setName("console");

	logger->addAppender(carp);
	return true;
}

/**
 * \brief �Ƴ�����̨Log���
 */
void zLogger::removeConsoleLog()
{
	AppenderPtr ap=logger->getAppender("console");
	logger->removeAppender(ap);
	ap->close();
}

/**
 * \brief ��һ�������ļ�Log���
 *
 * \param file Ҫ������ļ�����Logger���Զ������ʱ���׺ 
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::addLocalFileLog(const std::string &filename)
{
	PatternLayoutPtr fpl = new PatternLayout("\%d{\%y\%m\%d-\%H:\%M:\%S }%c %5p: %m%n");
	log4cxx::helpers::Pool pool;
	fpl->activateOptions(pool);

	LocalDailyRollingFileAppender * farp = new  LocalDailyRollingFileAppender();
	farp->setDatePattern(".\%y\%m\%d-\%H");
	farp->setLayout(fpl);
	farp->setName(filename);
        farp->setFile(filename);
//	farp->setEncoding(LOG4CXX_STR("utf-8"));
	farp->activateOptions(pool);

	logger->addAppender(farp);
	return true;
}

/**
 * \brief �Ƴ�һ�������ļ�Log���
 * \param file Ҫ�Ƴ���Log�ļ��� 
 */
void zLogger::removeLocalFileLog(const std::string &file)
{
	log4cxx::AppenderPtr ap=logger->getAppender("localfile:"+file);
	logger->removeAppender(ap);
	ap->close();
}

/**
 * \brief ���һ��Syslog���
 * \param host Syslog��������ַ,Ĭ��Ϊ������
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::addSysLog(const std::string & host="127.0.0.1")
{
	PatternLayoutPtr pls = new PatternLayout("%c %5p: %m%n");
	SyslogAppenderPtr sarp=new SyslogAppender(pls,SyslogAppender::getFacility("user"));
	sarp->setName(host);
	sarp->setSyslogHost (host);
	log4cxx::helpers::Pool pool;
	sarp->activateOptions(pool);
	logger->addAppender(sarp);
	return true;
}

/**
 * \brief �Ƴ�һ��Syslog���
 * \param host Ҫ�Ƴ���Syslog��������ַ,Ĭ��Ϊ������
 */
void zLogger::removeSysLog(const std::string & host="127.0.0.1")
{
	AppenderPtr ap=logger->getAppender(host);
	logger->removeAppender(ap);
	ap->close();
}

#define getMessage(msg,msglen,pat)	\
do	\
{	\
	va_list ap;	\
	bzero(msg, msglen);	\
	va_start(ap, pat);		\
	vsnprintf(msg, msglen - 1, pat, ap);	\
	va_end(ap);	\
}while(false)

/**
 * \brief ����д��־�ȼ�
 * \param  zLevelPtr ��־�ȼ�.�μ� #zLogger::zLevel
 */
//void zLogger::setLevel(const log4cxx::zLevel * zLevelPtr)
//{
//	logger->setLevel(zLevelPtr->zlevel);
//}

/**
 * \brief ����д��־�ȼ�
 * \param  level ��־�ȼ�
 */
void zLogger::setLevel(const std::string &level)
{
	if ("off" == level)
		logger->setLevel(log4cxx::Level::getOff());
	else if ("fatal" == level)
		logger->setLevel(log4cxx::Level::getFatal());
//	else if ("alarm" == level)
//		setLevel(zLevel::ALARM);
	else if ("error" == level)
		logger->setLevel(log4cxx::Level::getError());
//	else if ("iffy" == level)
//		setLevel(zLevel::IFFY);
	else if ("warn" == level)
		logger->setLevel(log4cxx::Level::getWarn());
	else if ("trace" == level)
		logger->setLevel(log4cxx::Level::getTrace());
	else if ("info" == level)
		logger->setLevel(log4cxx::Level::getInfo());
//	else if ("gbug" == level)
//		setLevel(zLevel::GBUG);
	else if ("debug" == level)
		logger->setLevel(log4cxx::Level::getDebug());
	else
		logger->setLevel(log4cxx::Level::getTrace());
}

/**
 * \brief д��־
 * \param  zLevelPtr ��־�ȼ��μ� #zLogger::zLevel
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
/*bool zLogger::log(const log4cxx::zLevel * zLevelPtr,const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevelPtr->zlevel,message);
	msgMut.unlock();
	return true;
}*/

/**
 * \brief ǿ��д��־,������־�ȼ�����
 * \param  zLevelPtr ��־�ȼ��μ� #zLogger::zLevel
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
/*bool zLogger::forceLog(const log4cxx::zLevel * zLevelPtr,const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->forcedLog(zLevelPtr->zlevel,message);
	msgMut.unlock();
	return true;
}
*/
/**
 * \brief дfatal������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::fatal(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->fatal(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дerror������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::error(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->error(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дerror������־
 * \param  buf �������
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::error_out(const char * buf)
{
	msgMut.lock();
	logger->error(buf);
	msgMut.unlock();
	return true;
}

/**
 * \brief дwarn������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::warn(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->warn(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дinfo������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::info(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->info(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дdebug������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::debug(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->debug(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дalarm��Ϸ��־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
/*bool zLogger::alarm(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::ALARM->zlevel,message);
	msgMut.unlock();
	return true;
}
*/
/**
 * \brief дiffy��Ϸ��־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
/*bool zLogger::iffy(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::IFFY->zlevel,message);
	msgMut.unlock();
	return true;
}
*/
/**
 * \brief дtrace��Ϸ��־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::trace(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->trace(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дgbug��Ϸ��־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
/*bool zLogger::gbug(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::GBUG->zlevel,message);
	msgMut.unlock();
	return true;
}*/
