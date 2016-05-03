#include "Core_Mutex.h"

#if defined(CORE_OS_FAMILY_WINDOWS)
#include "Core_Mutex_WIN32.cpp"
#else
#include "Core_Mutex_POSIX.cpp"
#endif


namespace CORE {


Core_Mutex::Core_Mutex()
{
}


Core_Mutex::~Core_Mutex()
{
}


Core_FastMutex::Core_FastMutex()
{
}


Core_FastMutex::~Core_FastMutex()
{
}


} 

