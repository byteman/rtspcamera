#include "Logger.h"
#include "Exception.h"



namespace CORE {




Logger::Logger():
	m_level(PRIO_TRACE),
	m_name("Logger")
{
	m_logChannel = (LogChannel*)&m_Log;
    m_logChannel->setProperty("addr","192.168.0.213:7000");
}


Logger::~Logger()
{
	
}


void Logger::setLevel(const CData& level)  
{
	Core_FastMutex::ScopedLock lock(m_mapMtx);
	
	if (level == "fatal")
	{
		m_level = PRIO_FATAL;
	}
	else if (level == "critical")
	{
		m_level = PRIO_CRITICAL;
	}
	else if (level == "error")
	{
		m_level = PRIO_ERROR;
	}
	else if (level == "warning")
	{
		m_level = PRIO_WARNING;
	}
	else if (level == "notice")
	{
		m_level = PRIO_NOTICE;
	}
	else if (level == "information")
	{
		m_level = PRIO_INFORMATION;
	}
	else if (level == "debug")
	{
		m_level = PRIO_DEBUG;
	}
	else if (level == "trace")
	{
		m_level = PRIO_TRACE;
	}

}


void Logger::setProperty(char* name, char* value)
{
	Core_FastMutex::ScopedLock lock(m_mapMtx);

	if(m_logChannel)
	{
		m_logChannel->setProperty(name,value);
	}
}


Logger& Logger::instance()
{
	static SingletonHolder<Logger> sh;
	return *sh.get();
}


Int32 Logger::getLevel() const
{
	return m_level;
}


void Logger::fatal(const char* msg)
{
	log(msg, PRIO_FATAL);
}


void Logger::critical(const char* msg)
{
	log(msg, PRIO_CRITICAL);
}


void Logger::error(const char* msg)
{
	log(msg, PRIO_ERROR);
}


void Logger::warning(const char* msg)
{
	log(msg, PRIO_WARNING);
}


void Logger::notice(const char* msg)
{
	log(msg, PRIO_NOTICE);
}


void Logger::information(const char* msg)
{
	log(msg, PRIO_INFORMATION);
}


void Logger::debug(const char* msg)
{
	log(msg, PRIO_DEBUG);
}


void Logger::trace(const char* msg)
{
	log(msg, PRIO_TRACE);
}


void Logger::log(const char* text, Priority prio)
{
	const char *strPrio[]={
		"fatal",
		"critical",
		"error",
		"warning",
		"notice",
		"information",
		"debug",
		"trace"
	};
	
	Core_FastMutex::ScopedLock lock(m_mapMtx);
	if(m_logChannel)
	{
		m_logChannel->log(text, strPrio[prio]);
	}
}


}

