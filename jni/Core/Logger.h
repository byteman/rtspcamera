#ifndef Logger_H
#define Logger_H


#include "Core.h"
#include "LogChannel.h"
#include "SingletonHolder.h"
#include "Exception.h"
#include "AutoPtr.h"
#include <map>
#include <list>
#include <cstddef>
#include <stdio.h>
#include <stdarg.h>
#include <sstream>

#include "LogConsoleChannel.h"
namespace CORE {


#define  DefaultLogger \
	CORE::Logger::instance


/// 
/// Class Logger
/// It is used for log
class CORE_API Logger
{
public:
	
	enum Priority
	{
		PRIO_FATAL = 0,   /// A fatal error. The application will most likely terminate. This is the highest priority.
		PRIO_CRITICAL,	/// A critical error. The application might not be able to continue running successfully.
		PRIO_ERROR,	   /// An error. An operation did not complete successfully, but the application as a whole is not affected.
		PRIO_WARNING,	 /// A warning. An operation completed with an unexpected result.
		PRIO_NOTICE,	  /// A notice, which is an information with just a higher priority.
		PRIO_INFORMATION, /// An informational message, usually denoting the successful completion of an operation.
		PRIO_DEBUG,	   /// A debugging message.
		PRIO_TRACE		/// A tracing message. This is the lowest priority.
	};
	
public:
	Logger();
	
	~Logger();


public:

		///@brief
		///   - fatal
		///   - critical
		///   - error
		///   - warning
		///   - notice
		///   - information
		///   - debug
		///   - trace
	void setLevel(const CData& level);

		/// Get the level
	Int32 getLevel() const;

	void setProperty(char* name, char* value);

	void fatal(const char* msg);

	void critical(const char* msg);

	void error(const char* msg);

	void warning(const char* msg);

	void notice(const char* msg);

	void information(const char* msg);

	void debug(const char* msg);

	void trace(const char* msg);

	void log(const char* text, Priority prio);

	static Logger& instance();
		
protected:
	
	LogChannel*		m_logChannel;
	Int32				 m_level;
	mutable Core_FastMutex	m_mapMtx;
	CData			 m_name;
	LogConsoleChannel m_Log;
};


///
///
/// eg.
/// _LogFatal<<10<<_LogFatalEnd;
/// _LogFatal<<"name:"<<12345<<" "<<"value:"<<12.123<<" "<<_LogFatalEnd;
///


#define LogEnd(x) \
			 " [file:" << __FILE__ << " line:" << __LINE__<<"]\n"; \
			 CORE::Logger::instance().x(logTmpStr.str().c_str());\
			 logTmpStr.str("");\
		};

#define	 _LogFatal\
		if(CORE::Logger::instance().getLevel()>=CORE::Logger::PRIO_FATAL)\
		{\
			std::ostringstream logTmpStr;\
			logTmpStr
			
#define	 _LogFatalEnd			 LogEnd(fatal)

#define	 _LogCritical\
		if(CORE::Logger::instance().getLevel()>=CORE::Logger::PRIO_CRITICAL)\
		{\
			std::ostringstream logTmpStr;\
			logTmpStr
			
#define	 _LogCriticalEnd		 LogEnd(critical)

#define	 _LogError\
		if(CORE::Logger::instance().getLevel()>=CORE::Logger::PRIO_ERROR)\
		{\
			std::ostringstream logTmpStr;\
			logTmpStr
			
#define	 _LogErrorEnd			 LogEnd(error)

#define	 _LogWarning\
		if(CORE::Logger::instance().getLevel()>=CORE::Logger::PRIO_WARNING)\
		{\
			std::ostringstream logTmpStr;\
			logTmpStr
			
#define	 _LogWarningEnd		 LogEnd(warning)

#define	 _LogNotice\
		if(CORE::Logger::instance().getLevel()>=CORE::Logger::PRIO_NOTICE)\
		{\
			std::ostringstream logTmpStr;\
			logTmpStr
			
#define	 _LogNoticeEnd		 LogEnd(notice)

#define	 _LogInformation\
		if(CORE::Logger::instance().getLevel()>=CORE::Logger::PRIO_INFORMATION)\
		{\
			std::ostringstream logTmpStr;\
			logTmpStr
			
#define	 _LogInformationEnd	 LogEnd(information)

#define	 _LogDebug\
		if(CORE::Logger::instance().getLevel()>=CORE::Logger::PRIO_DEBUG)\
		{\
			std::ostringstream logTmpStr;\
			logTmpStr
			
#define	 _LogDebugEnd		 LogEnd(debug )
#if 0
#define	 _LogTrace\
		if(CORE::Logger::instance().getLevel()>=CORE::Logger::PRIO_TRACE)\
		{\
			std::ostringstream logTmpStr;\
			logTmpStr
			
#define	 _LogTraceEnd		 LogEnd(trace)
#else
#define	 _LogTrace	
#define	 _LogTraceEnd
#endif
}


#endif 


