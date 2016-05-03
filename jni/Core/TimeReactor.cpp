#include "TimeReactor.h"
#include "SingletonHolder.h"
#include "Logger.h"


namespace CORE  {


Int32 TimeReactor::m_nTimeId=1;


TimeReactor::TimeReactor():
	m_timeout(DEFAULT_TIMEOUT),
	m_bStop(false)
{
	m_timeoutTmp=m_timeout;
}


TimeReactor::TimeReactor(const Timespan& timeout):
	m_timeout(timeout),
	m_bStop(false)
{
	m_timeoutTmp=m_timeout;
}


TimeReactor::~TimeReactor()
{



}


Int32 TimeReactor::addTimer(TimeEventHandler* handler,Int32 seconds, char *name)
{
	if(strlen(name)>99)
	{
		_LogError<<"TimeEventHandler'name more long"<<_LogErrorEnd;
		throw LogicException("TimeEventHandler'name more long");
	}
	
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	Int32 id;
	id=m_nTimeId;

	
	Core_EventHandlerMap::iterator it = m_handlers.find(id);
	while (it != m_handlers.end())
	{
		m_nTimeId++;
		if(10000==m_nTimeId)
		{
			m_nTimeId=1;
		}
		id=m_nTimeId;
		it = m_handlers.find(id);
	}

	m_nTimeId++;
	if(10000==m_nTimeId)
	{
		m_nTimeId=1;
	}

	TimeNotifier timerNtf;
	timerNtf.setId(id);
	timerNtf.setTimeout(seconds);
	timerNtf.addEventHandler(handler, name);
	m_handlers[id] = timerNtf;
	return id;
}


Int32 TimeReactor::removeTimer(Int32 id)
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	Core_EventHandlerMap::iterator it = m_handlers.find(id);
	if (it != m_handlers.end())
	{
		TimeNotifier *timerNtf = &it->second;
		timerNtf->removeEventHandler();
		if(timerNtf->isClear())
		{
			if(m_hasClear>0)
			{
				m_hasClear--;
			}
		}
		m_handlers.erase(it);
		return 0;
	}
	return -1;
}


void TimeReactor::run()
{
	if(!m_bStop)
	{
		m_hasClear=0;
		Timestamp start;
		m_timeoutTmp=m_timeoutTmp-(start-m_preTime);
		m_preTime=start;
		if(m_timeoutTmp<=Timespan(0))
		{
			m_timeoutTmp=m_timeout;
			dispatch();
		}

		if(m_timeoutTmp>=Timespan(2,0))
		{
			m_timeoutTmp=m_timeout;
		}

		while(m_hasClear>0)
		{
			Core_FastMutex::ScopedLock lock(m_mutex);

			for (Core_EventHandlerMap::iterator it = m_handlers.begin(); it != m_handlers.end(); ++it)
			{
				TimeNotifier *timerNtf = &it->second;
				if(true==timerNtf->isClear())
				{
					m_handlers.erase(it);
					m_hasClear--;
					break;
				}
			}
		}
	}
}


void TimeReactor::stop()
{
	m_bStop = true;
}


void TimeReactor::start()
{
	m_bStop = false;
}


Int32 TimeReactor::getTimerCount()
{
	Core_FastMutex::ScopedLock lock(m_mutex);

	return (Int32)m_handlers.size();
}


void TimeReactor::dispatch()
{
	Core_FastMutex::ScopedLock lock(m_mutex);

	for (Core_EventHandlerMap::iterator it = m_handlers.begin(); it != m_handlers.end(); ++it)
	{
		TimeNotifier *timerNtf = &it->second;
		
		if(timerNtf->isTimeout())
		{
			Int32 ret;

			try
			{
				ret=timerNtf->dispatch();
			}
			catch(...)
			{
				_LogError<<"timer dispatch error:"<<timerNtf->getName()<<_LogErrorEnd;
				ret=-1;
			}
			
			if(ret<0)
			{
				m_hasClear++;
				timerNtf->setClear(true);
			}
		}
	}
}


void TimeReactor::setTimeout(const Timespan& timeout)
{
	m_timeout = timeout;
}

	
const Timespan& TimeReactor::getTimeout() const
{
	return m_timeout;
}


const Timespan& TimeReactor::getTimeoutTmp() const
{
	return m_timeoutTmp;
}


}



