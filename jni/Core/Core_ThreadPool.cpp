#include "Core_ThreadPool.h"
#include "Runnable.h"
#include "Core_Thread.h"
#include "Core_Event.h"
#include <sstream>
#include <ctime>
#include <sstream>
#include "Logger.h"


namespace CORE {

/// Class PooledCore_Thread
/// It is used threadpool
class PooledCore_Thread: public Runnable
{
public:
	PooledCore_Thread(const CData& name);
	~PooledCore_Thread();

	void start();
	void start(Runnable& target);
	bool idle();
	void join();
	void activate();
	void release();
	void run();

private:
	volatile bool			m_bIdle;
	Runnable*		   m_pTarget;
	CData			m_name;
	Core_Thread			  m_thread;
	Core_Event			   m_targetReady;
	Core_Event			   m_targetCompleted;
	Core_Event		m_started;
	Core_FastMutex	m_mutex;
	Core_ThreadPool	*m_parent;

	friend class Core_ThreadPool;
};


PooledCore_Thread::PooledCore_Thread(const CData& name): 
	m_bIdle(true), 
	m_pTarget(0), 
	m_name(name), 
	m_thread(name),
	m_targetCompleted(false)
{

}


PooledCore_Thread::~PooledCore_Thread()
{
}


void PooledCore_Thread::start()
{
	m_thread.start(*this);
	m_started.wait();
}


void PooledCore_Thread::start(Runnable& target)
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	m_pTarget = &target;
	m_targetReady.set();
}


inline bool PooledCore_Thread::idle()
{
	return m_bIdle;
}


void PooledCore_Thread::activate()
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	m_bIdle = false;
	m_targetCompleted.reset();
}


void PooledCore_Thread::release()
{
	m_mutex.lock();
	m_pTarget = 0;
	m_mutex.unlock();

	if (m_thread.isRunning()) 
	{	
		m_targetReady.set();
		m_targetCompleted.wait();
	}
	else
	{
		delete this;
	}
}


void PooledCore_Thread::run()
{
	m_started.set();
	while(1)
	{
		m_targetReady.wait();
		
		Core_FastMutex::ScopedLock lock(m_mutex);
		if (m_pTarget) // a NULL target means kill yourself
		{
			try
			{
				Core_FastMutex::ScopedUnLock unlock(m_mutex);
				m_pTarget->run();
			}
			catch (...)
			{

			}

			m_pTarget  = 0;
			m_bIdle	 = true;
			m_targetCompleted.set();
			m_parent->m_event.set();
		}
		else
		{
			m_targetCompleted.set();
			break;
		}
	}

	delete this;
}


Core_ThreadPool::Core_ThreadPool(Int32 capacity): 
	m_nCapacity(capacity)
{
	
}


Core_ThreadPool::~Core_ThreadPool()
{
	
}


void Core_ThreadPool::startAll()
{
	for (Int32 i = 0; i < m_nCapacity; i++)
	{
		PooledCore_Thread* pCore_Thread = createThread();
		pCore_Thread->m_parent=this;
		m_threads.push_back(pCore_Thread);
		pCore_Thread->start();
	}
}


Int32 Core_ThreadPool::capacity() 
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	return m_nCapacity;
}


void Core_ThreadPool::setCapacity(int capacity)
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	m_nCapacity=capacity;
}


Int32 Core_ThreadPool::used()
{
	Core_FastMutex::ScopedLock lock(m_mutex);

	Int32 count = 0;
	for (Core_ThreadVec::const_iterator it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		if (!(*it)->idle())
		{
			++count;
		}
	}
	return count;
}


bool Core_ThreadPool::start(Runnable& target)
{
	PooledCore_Thread *pt=0;
	pt=getThread();
	
	if(0==pt)
	{
		return false;
	}

	pt->start(target);
	return true;
}


void Core_ThreadPool::stopAll()
{	
	m_event.set();
	
	Core_FastMutex::ScopedLock lock(m_mutex);

	for (Core_ThreadVec::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		(*it)->release();
	}
	m_threads.clear();
}


void Core_ThreadPool::waitValidThread()
{
	m_event.wait();
}


PooledCore_Thread* Core_ThreadPool::getThread()
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	PooledCore_Thread* pCore_Thread = 0;
	for (Core_ThreadVec::iterator it = m_threads.begin(); !pCore_Thread && it != m_threads.end(); ++it)
	{
		if ((*it)->idle())
		{
			pCore_Thread = *it;
		}
	}
	
	if (!pCore_Thread)
	{
		return 0;
	}
	
	pCore_Thread->activate();
	return pCore_Thread;
}


PooledCore_Thread* Core_ThreadPool::createThread()
{
	std::ostringstream name;
	static int serial=0;
	name << "Core_ThreadPool:" << ++serial;
	return new PooledCore_Thread(name.str());
}


Core_ThreadPool& Core_ThreadPool::instance()
{
	static SingletonHolder<Core_ThreadPool> sh;
	return *sh.get();
}


}

