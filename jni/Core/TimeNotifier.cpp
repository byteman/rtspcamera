#include "TimeNotifier.h"
#include "Logger.h"


namespace CORE {

TimeNotifier::TimeNotifier():
	m_ptrHandler(0),
	m_isClear(false),
	m_tmp(0)
{
	
}


	
TimeNotifier::~TimeNotifier()
{
}


void TimeNotifier::setTimeout(Int32 seconds)
{
	m_timeout=seconds;
	m_tmp=m_timeout;
}


Int32 TimeNotifier::getId() const
{
	return m_nId;
}


void TimeNotifier::setId(Int32 id)
{
	m_nId=id;
}


void TimeNotifier::setClear(bool clear)
{
	m_isClear=clear;
}


bool	TimeNotifier::isClear()
{
	return m_isClear;
}


char *TimeNotifier::getName()
{
	return m_name;
}
	
TimeEventHandler* TimeNotifier::getHandler() const
{
	return m_ptrHandler;
}


void TimeNotifier::addEventHandler(TimeEventHandler *handler, char *name)
{
	m_ptrHandler=handler;
	memset(m_name,0,100);
	memcpy(m_name, name, strlen(name));
}

	
void TimeNotifier::removeEventHandler()
{
	m_ptrHandler=0; 
}


Int32 TimeNotifier::dispatch()
{
	Int32 ret=0;
	ret=m_ptrHandler->handle_time(m_nId);
	return ret;
}


bool TimeNotifier::isTimeout()
{
	m_tmp--;
	
	if(m_tmp<=0)
	{
		m_tmp=m_timeout;
		return true;
	}
	
	return false;
}


} 
