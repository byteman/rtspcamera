#include "Core_Mutex_POSIX.h"


namespace CORE {


Core_MutexImpl::Core_MutexImpl()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
	pthread_mutexattr_settype_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#elif defined(PTHREAD_MUTEX_RECURSIVE)
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#else

#endif
	pthread_mutex_init(&m_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}


Core_MutexImpl::Core_MutexImpl(bool fast)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
	pthread_mutexattr_settype_np(&attr, fast ? PTHREAD_MUTEX_NORMAL_NP : PTHREAD_MUTEX_RECURSIVE_NP);
#elif defined(PTHREAD_MUTEX_RECURSIVE)
	pthread_mutexattr_settype(&attr, fast ? PTHREAD_MUTEX_NORMAL : PTHREAD_MUTEX_RECURSIVE);
#else

#endif
	pthread_mutex_init(&m_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}


Core_MutexImpl::~Core_MutexImpl()
{
	pthread_mutex_destroy(&m_mutex);
}


Core_FastMutexImpl::Core_FastMutexImpl(): Core_MutexImpl(true)
{
}


Core_FastMutexImpl::~Core_FastMutexImpl()
{
}


}

