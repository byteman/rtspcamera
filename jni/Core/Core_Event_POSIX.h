#ifndef Core_Event_POSIX_H
#define Core_Event_POSIX_H


#include "Core.h"
#include <pthread.h>
#include "Logger.h"


namespace CORE {


class CORE_API Core_EventImpl
{
protected:
	Core_EventImpl(bool autoReset);	
	
	~Core_EventImpl();
	
	void setImpl();
	
	void waitImpl();
	
	bool waitImpl(Int32 milliseconds);
	
	void resetImpl();
	
private:
	bool			m_auto;
	volatile bool   m_state;
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
};


//
// inlines
//
inline void Core_EventImpl::setImpl()
{
	pthread_mutex_lock(&m_mutex);
	m_state = true;
	pthread_cond_broadcast(&m_cond);
	pthread_mutex_unlock(&m_mutex);
}


inline void Core_EventImpl::resetImpl()
{
	pthread_mutex_lock(&m_mutex);
	m_state = false;
	pthread_mutex_unlock(&m_mutex);
}


} 


#endif 

