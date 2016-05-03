#include "Core_Thread.h"
#include "Core_Mutex.h"
#include <sstream>


#if defined(CORE_OS_FAMILY_WINDOWS)
#include "Core_Thread_WIN32.cpp"
#else
#include "Core_Thread_POSIX.cpp"
#endif


namespace CORE {


Core_Thread::Core_Thread(): 
	m_nId(uniqueId()), 
	m_name(makeName())
{
}


Core_Thread::Core_Thread(const CData& name): 
	m_nId(uniqueId()), 
	m_name(name)
{
}


Core_Thread::~Core_Thread()
{

}


void Core_Thread::setPriority(Priority prio)
{
	setPriorityImpl(prio);
}


Core_Thread::Priority Core_Thread::getPriority() const
{
	return Priority(getPriorityImpl());
}


void Core_Thread::start(Runnable& target)
{
	startImpl(target);
}


void Core_Thread::stop()
{
	stopImpl();
}


void Core_Thread::join()
{
	joinImpl();
}


bool Core_Thread::join(Int32 milliseconds)
{
	return joinImpl(milliseconds);
}


bool Core_Thread::tryJoin(Int32 milliseconds)
{
	return joinImpl(milliseconds);
}


CData Core_Thread::makeName()
{
	std::ostringstream name;
	name <<"Core_Thread:" << m_nId;
	return name.str();
}


Int32 Core_Thread::uniqueId()
{
	static Core_FastMutex mtx;
	Core_FastMutex::ScopedLock lock(mtx);

	static unsigned count = 0;
	++count;
	return count;
}


void Core_Thread::setName(const CData& name)
{
	Core_FastMutex::ScopedLock lock(m_mutex);

	m_name = name;
}


} 

