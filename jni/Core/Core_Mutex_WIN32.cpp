#include "Core_Mutex_WIN32.h"


namespace CORE {


Core_MutexImpl::Core_MutexImpl()
{
	InitializeCriticalSectionAndSpinCount(&m_cs, 4000);
}


Core_MutexImpl::~Core_MutexImpl()
{
	DeleteCriticalSection(&m_cs);
}


} 

