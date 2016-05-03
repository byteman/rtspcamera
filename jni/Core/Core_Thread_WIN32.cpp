#include "Core_Thread_WIN32.h"
#include <process.h>
#include "Core_Thread.h"
#include <map>
#include "Logger.h"


namespace CORE {

Core_FastMutex g_mutex;
typedef std::map<int, Core_ThreadImpl*> Core_ThreadMap;
Core_ThreadMap	 g_map;


Core_ThreadImpl::Core_ThreadImpl(): m_pTarget(0), m_thread(0), m_nPrio(PRIO_NORMAL_IMPL)
{
	
}

			
Core_ThreadImpl::~Core_ThreadImpl()
{
	if (m_thread) 
	{
		CloseHandle(m_thread);
	}
}


void Core_ThreadImpl::setPriorityImpl(Int32 prio)
{
	if (prio != m_nPrio)
	{
		m_nPrio = prio;
		if (m_thread)
		{
			SetThreadPriority(m_thread, m_nPrio);
		}
	}
}


void Core_ThreadImpl::stopImpl()
{
	if (m_thread) 
	{
		CloseHandle(m_thread);
		m_thread=0;
	}
}


void Core_ThreadImpl::startImpl(Runnable& target)
{
	if (m_thread) 
	{
		return;
	}

	m_pTarget = &target;
#if defined(_DLL)
	DWORD threadId;
	m_thread = CreateThread(NULL, 0, entry, this, 0, &threadId);
#else
	unsigned threadId;
	m_thread = (HANDLE) _beginthreadex(NULL, 0, entry, this, 0, &threadId);
#endif
}


void Core_ThreadImpl::joinImpl()
{
	if (!m_thread) return;

	WaitForSingleObject(m_thread, INFINITE);

}


bool Core_ThreadImpl::joinImpl(Int32 milliseconds)
{
	if (!m_thread) return true;

	switch (WaitForSingleObject(m_thread, milliseconds + 1))
	{
		case WAIT_TIMEOUT:
		{
			return false;
		}
		case WAIT_OBJECT_0:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}


bool Core_ThreadImpl::isRunningImpl() const
{
	if (m_thread)
	{
		DWORD ec = 0;
		return GetExitCodeThread(m_thread, &ec) && ec == STILL_ACTIVE;
	}
	return false;
}


Core_ThreadImpl* Core_ThreadImpl::currentImpl()
{
	Core_FastMutex::ScopedLock lock(g_mutex);
	
	Core_ThreadMap::iterator it = g_map.find(GetCurrentThreadId());
	if (it!= g_map.end())
	{
		return it->second;
	}

	return 0;
}


#if defined(_DLL)
DWORD WINAPI Core_ThreadImpl::entry(LPVOID pCore_Thread)
#else
unsigned __stdcall Core_ThreadImpl::entry(void* pCore_Thread)
#endif
{
	int id=GetCurrentThreadId();
	
	{
		Core_FastMutex::ScopedLock lock(g_mutex);
		g_map[id]=(Core_ThreadImpl*)pCore_Thread;
	}
	
	try
	{
		reinterpret_cast<Core_ThreadImpl*>(pCore_Thread)->m_pTarget->run();
	}
	catch (...)
	{
		Core_Thread* current;
		current=Core_Thread::current();
		_LogFatal<<"Core_Thread Error:"<<current->getName().c_str()<<_LogFatalEnd;
	}

	{
		Core_FastMutex::ScopedLock lock(g_mutex);
	
		Core_ThreadMap::iterator it = g_map.find(id);
		if (it != g_map.end())
		{
			g_map.erase(it);
		}
	}
	
	return 0;
}


}

