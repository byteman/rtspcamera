#ifndef Core_Mutex_H
#define Core_Mutex_H


#include "Core.h"
#include "ScopedLock.h"


#if defined(CORE_OS_FAMILY_WINDOWS)
#ifdef _WIN32 
#include "Core_Mutex_WIN32.h"
#endif
#else
#include "Core_Mutex_POSIX.h"
#endif


namespace CORE {

/// Class Core_Mutex
/// It is same as Lock
class CORE_API Core_Mutex: private Core_MutexImpl
{
public:
		/// Using the ScopedLock class is the preferred way to automatically
		/// lock and unlock a mutex.
	typedef ScopedLock_T<Core_Mutex> ScopedLock;
	typedef ScopedUnLock_T<Core_Mutex> ScopedUnLock;
		/// Constructor
	Core_Mutex();

		/// Destructor
	~Core_Mutex();

		/// Lock the mutex.
	void lock();

		/// Tries to lock the mutex.
		/// return true if successfull,false otherwise.
		/// It returns immediately.
	bool tryLock();

		/// Unlock the mutex
	void unlock();
	
private:
	Core_Mutex(const Core_Mutex&);
	Core_Mutex& operator = (const Core_Mutex&);
};


class CORE_API Core_FastMutex: private Core_FastMutexImpl
{
public:
	typedef ScopedLock_T<Core_FastMutex> ScopedLock;
	typedef ScopedUnLock_T<Core_FastMutex> ScopedUnLock; 
	
	Core_FastMutex();
		
	~Core_FastMutex();

	void lock();

	bool tryLock();

	void unlock();
	
private:
	Core_FastMutex(const Core_FastMutex&);
	Core_FastMutex& operator = (const Core_FastMutex&);
};


//
// inlines
//
inline void Core_Mutex::lock()
{
	lockImpl();
}


inline bool Core_Mutex::tryLock()
{
	return tryLockImpl();
}


inline void Core_Mutex::unlock()
{
	unlockImpl();
}


inline void Core_FastMutex::lock()
{
	lockImpl();
}


inline bool Core_FastMutex::tryLock()
{
	return tryLockImpl();
}


inline void Core_FastMutex::unlock()
{
	unlockImpl();
}


} 


#endif 

