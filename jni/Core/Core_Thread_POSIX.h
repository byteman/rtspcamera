#ifndef Core_Thread_POSIX_H
#define Core_Thread_POSIX_H


#include "Core.h"
#include "Runnable.h"
#include "Core_Event.h"
#include "RefCountedObject.h"
#include "AutoPtr.h"
#include <pthread.h>
#if !defined(CORE_NO_SYS_SELECT_H)
#include <sys/select.h>
#endif
#include <errno.h>


namespace CORE {


class CORE_API Core_ThreadImpl
{
public:	
	enum Priority
	{
		PRIO_LOWEST_IMPL,
		PRIO_LOW_IMPL,
		PRIO_NORMAL_IMPL,
		PRIO_HIGH_IMPL,
		PRIO_HIGHEST_IMPL
	};

	Core_ThreadImpl();			
	
	~Core_ThreadImpl();

	Runnable& targetImpl() const;
	
	void setPriorityImpl(Int32 prio);
	
	Int32 getPriorityImpl() const;
	
	void startImpl(Runnable& target);

	void stopImpl();

	void joinImpl();
	
	bool joinImpl(Int32 milliseconds);
	
	bool isRunningImpl() const;
	
	static void sleepImpl(Int32 milliseconds);
	
	static void yieldImpl();
	
	static Core_ThreadImpl* currentImpl();

protected:
	static void* entry(void* pCore_Thread);
	
	static Int32 mapPrio(Int32 prio);

private:
	struct Core_ThreadData: public RefCountedObject
	{
		Core_ThreadData():
			pTarget(0),
			thread(0),
			prio(PRIO_NORMAL_IMPL),
			done(false)
		{
		}

		Runnable* pTarget;
		pthread_t thread;
		Int32	   prio;
		Core_Event	 done;
		bool		isRun;
	};
	
	AutoPtr<Core_ThreadData> m_pData;
};


//
// inlines
//
inline Int32 Core_ThreadImpl::getPriorityImpl() const
{
	return m_pData->prio;
}


inline void Core_ThreadImpl::sleepImpl(Int32 milliseconds)
{
	struct timeval tv;
	tv.tv_sec  = milliseconds / 1000;
	tv.tv_usec = (milliseconds % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);	 
}


inline void Core_ThreadImpl::yieldImpl()
{
	sched_yield();
}


} 


#endif 

