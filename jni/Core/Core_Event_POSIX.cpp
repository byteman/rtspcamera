#include "Core_Event_POSIX.h"
#include <sys/time.h>
#include "Exception.h"


namespace CORE {


Core_EventImpl::Core_EventImpl(bool autoReset): m_auto(autoReset), m_state(false)
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond, NULL);
}


Core_EventImpl::~Core_EventImpl()
{
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
}


void Core_EventImpl::waitImpl()
{
	pthread_mutex_lock(&m_mutex);
	while (!m_state) 
	{
		pthread_cond_wait(&m_cond, &m_mutex);
	}
	
	if (m_auto)
	{
		m_state = false;
	}
	pthread_mutex_unlock(&m_mutex);
}


bool Core_EventImpl::waitImpl(Int32 milliseconds)
{
	int rc = 0;
	struct timespec abstime;


	struct timeval tv;
	gettimeofday(&tv, NULL);
	abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;
	abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}


	//pthread_mutex_lock(&m_mutex);
	
	while (!m_state) 
	{
		if ((rc = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))
		{
			if (rc == ETIMEDOUT) break;
		}
	}
	if (rc == 0 &&m_auto) m_state = false;
	pthread_mutex_unlock(&m_mutex);
	return rc == 0;
}


}

