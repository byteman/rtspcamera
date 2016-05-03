#include "Core_Thread_POSIX.h"
#include "Exception.h"
#include "Logger.h"
#include <signal.h>
#include <map>
#include "Core_Thread.h"

//
// Block SIGPIPE in main thread.
//
#if defined(CORE_OS_FAMILY_UNIX)
namespace
{
	class SignalBlocker
	{
	public:
		SignalBlocker()
		{
			sigset_t sset;
			sigemptyset(&sset);
			sigaddset(&sset, SIGPIPE); 
			pthread_sigmask(SIG_BLOCK, &sset, 0);
		}
		~SignalBlocker()
		{
		}
	};
	
	static SignalBlocker signalBlocker;
}
#endif


namespace CORE {


Core_FastMutex g_mutex;
typedef std::map<int, Core_ThreadImpl*> Core_ThreadMap;
Core_ThreadMap	 g_map;


Core_ThreadImpl::Core_ThreadImpl():
	m_pData(new Core_ThreadData)
{
	
}

			
Core_ThreadImpl::~Core_ThreadImpl()
{
	if (m_pData->pTarget)
	{
		pthread_detach(m_pData->thread);
	}
}


void Core_ThreadImpl::setPriorityImpl(Int32 prio)
{
	if (prio != m_pData->prio)
	{
		m_pData->prio = prio;
		if (m_pData->pTarget)
		{
			struct sched_param par;
			par.sched_priority = mapPrio(m_pData->prio);
			pthread_setschedparam(m_pData->thread, SCHED_OTHER, &par);
		}
	}
}


void Core_ThreadImpl::stopImpl()
{
	if (m_pData->pTarget)
	{
		pthread_detach(m_pData->thread);
		m_pData->pTarget=0;
	}
}

void Core_ThreadImpl::startImpl(Runnable& target)
{
	if (m_pData->pTarget) 
	{
		return;
	}

	m_pData->pTarget = &target;
	m_pData->isRun=true;
	pthread_create(&m_pData->thread, NULL, entry, this);

	if (m_pData->prio != PRIO_NORMAL_IMPL)
	{
		struct sched_param par;
		par.sched_priority = mapPrio(m_pData->prio);
		pthread_setschedparam(m_pData->thread, SCHED_OTHER, &par);
	}
}


void Core_ThreadImpl::joinImpl()
{
	m_pData->done.wait();
	void* result;
	pthread_join(m_pData->thread, &result);
}


bool Core_ThreadImpl::joinImpl(Int32 milliseconds)
{
	if (m_pData->done.tryWait(milliseconds))
	{
		void* result;
		if (pthread_join(m_pData->thread, &result))
		{
			return false;
		}
		return true;
	}
	else 
	{
		return false;
	}
}


bool Core_ThreadImpl::isRunningImpl() const
{
	return m_pData->isRun;
}


Core_ThreadImpl* Core_ThreadImpl::currentImpl()
{
	Core_FastMutex::ScopedLock lock(g_mutex);
	
	Core_ThreadMap::iterator it = g_map.find(pthread_self());
	if (it!= g_map.end())
	{
		return it->second;
	}

	return 0;
}


void* Core_ThreadImpl::entry(void* pCore_Thread)
{
#if defined(CORE_OS_FAMILY_UNIX)
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigaddset(&sset, SIGPIPE); 
	pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif

	int id=pthread_self();
	
	{
		Core_FastMutex::ScopedLock lock(g_mutex);
		g_map[id]=(Core_ThreadImpl*)pCore_Thread;
	}
	
	Core_ThreadImpl* pCore_ThreadImpl = reinterpret_cast<Core_ThreadImpl*>(pCore_Thread);
	AutoPtr<Core_ThreadData> pData = pCore_ThreadImpl->m_pData;

	try
	{
		pData->pTarget->run();
	}
	catch (...)
	{
		Core_Thread* current;
		current=Core_Thread::current();
		_LogFatal<<"Core_Thread Error:"<<current->getName().c_str()<<_LogFatalEnd;
	}
	
	pData->isRun=false;
	pData->done.set();

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


Int32 Core_ThreadImpl::mapPrio(Int32 prio)
{
	static const Int32 pmin = sched_get_priority_min(SCHED_OTHER);
	static const Int32 pmax = sched_get_priority_max(SCHED_OTHER);

	switch (prio)
	{
		case PRIO_LOWEST_IMPL:
		{
			return pmin;
		}	
		case PRIO_LOW_IMPL:
		{
			return pmin + (pmax - pmin)/4;
		}	
		case PRIO_NORMAL_IMPL:
		{
			return pmin + (pmax - pmin)/2;
		}	
		case PRIO_HIGH_IMPL:
		{
			return pmin + 3*(pmax - pmin)/4;
		}
		case PRIO_HIGHEST_IMPL:
		{
			return pmax;
		}	
		default:
		{
			return -1;
		}
	}
	return -1; 
}


}

