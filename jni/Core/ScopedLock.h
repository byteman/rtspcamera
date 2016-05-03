#ifndef ScopedLock_T_H
#define ScopedLock_T_H


#include "Core.h"


namespace CORE {

/// Class ScopedLock_T
/// Automatically lock mutex when constructor
/// Automatically unlock mutex when destructor
template <class M>
class ScopedLock_T
{
public:
	inline ScopedLock_T(M& mutex): m_mutex(mutex)
	{
		m_mutex.lock();
	}
	inline ~ScopedLock_T()
	{
		m_mutex.unlock();
	}

private:
	M& m_mutex;

	ScopedLock_T();
	ScopedLock_T(const ScopedLock_T&);
	ScopedLock_T& operator = (const ScopedLock_T&);
};


/// Class ScopedUnLock_T
/// Automatically unlock mutex when constructor
/// Automatically lock mutex when destructor
template <class M>
class ScopedUnLock_T
{
public:
	inline ScopedUnLock_T(M& mutex): m_mutex(mutex)
	{
		m_mutex.unlock();
	}
	inline ~ScopedUnLock_T()
	{
		m_mutex.lock();
	}

private:
	M& m_mutex;

	ScopedUnLock_T();
	ScopedUnLock_T(const ScopedUnLock_T&);
	ScopedUnLock_T& operator = (const ScopedUnLock_T&);
};


} 


#endif 

