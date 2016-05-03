#ifndef Core_Mutex_WIN32_H
#define Core_Mutex_WIN32_H


#include "Core.h"


namespace CORE {


class CORE_API Core_MutexImpl
{
protected:
	
	Core_MutexImpl();
	
	~Core_MutexImpl();
	
	void lockImpl();
	
	bool tryLockImpl();
	
	void unlockImpl();
	
private:
	
	CRITICAL_SECTION m_cs;
	
};


typedef Core_MutexImpl Core_FastMutexImpl;


//
// inlines
//
inline void Core_MutexImpl::lockImpl()
{
	EnterCriticalSection(&m_cs);
}


inline bool Core_MutexImpl::tryLockImpl()
{
	return (TryEnterCriticalSection(&m_cs) == TRUE);
}


inline void Core_MutexImpl::unlockImpl()
{
	LeaveCriticalSection(&m_cs);
}


}


#endif 

