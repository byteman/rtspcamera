#ifndef Core_Mutex_POSIX_H
#define Core_Mutex_POSIX_H


#include "Core.h"
#include <pthread.h>
#include <errno.h>


namespace CORE {


class CORE_API Core_MutexImpl
{
protected:
	Core_MutexImpl();
	
	Core_MutexImpl(bool fast);
	
	~Core_MutexImpl();
	
	void lockImpl();
	
	bool tryLockImpl();
	
	void unlockImpl();
	
private:
	
	pthread_mutex_t m_mutex;
	
};


class CORE_API Core_FastMutexImpl: public Core_MutexImpl
{
protected:
	Core_FastMutexImpl();
	~Core_FastMutexImpl();
};


//
// inlines
//
inline void Core_MutexImpl::lockImpl()
{
	pthread_mutex_lock(&m_mutex);
}


inline bool Core_MutexImpl::tryLockImpl()
{
	Int32 rc = pthread_mutex_trylock(&m_mutex);
	if (rc == 0)
		return true;
	else 
		return false;
}


inline void Core_MutexImpl::unlockImpl()
{
	pthread_mutex_unlock(&m_mutex);
}


} 


#endif

