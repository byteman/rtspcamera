#ifndef Core_Thread_WIN32_H
#define Core_Thread_WIN32_H


#include "Core.h"
#include "Runnable.h"


namespace CORE {


class CORE_API Core_ThreadImpl
{
public:	
	enum Priority
	{
		PRIO_LOWEST_IMPL  = THREAD_PRIORITY_LOWEST,
		PRIO_LOW_IMPL	 = THREAD_PRIORITY_BELOW_NORMAL,
		PRIO_NORMAL_IMPL  = THREAD_PRIORITY_NORMAL,
		PRIO_HIGH_IMPL	= THREAD_PRIORITY_ABOVE_NORMAL,
		PRIO_HIGHEST_IMPL = THREAD_PRIORITY_HIGHEST
	};

	Core_ThreadImpl();	
	
	~Core_ThreadImpl();

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
#if defined(_DLL)
	static DWORD WINAPI entry(LPVOID pCore_Thread);
#else
	static unsigned __stdcall entry(void* pCore_Thread);
#endif

private:
	Runnable*	 m_pTarget;
	HANDLE		m_thread;
	Int32		   m_nPrio;
};


//
// inlines
//
inline Int32 Core_ThreadImpl::getPriorityImpl() const
{
	return m_nPrio;
}


inline void Core_ThreadImpl::sleepImpl(Int32 milliseconds)
{
	Sleep(DWORD(milliseconds));
}


inline void Core_ThreadImpl::yieldImpl()
{
	Sleep(0);
}


}


#endif 

