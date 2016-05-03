#include "Core_Event.h"


#if defined(CORE_OS_FAMILY_WINDOWS)
#include "Core_Event_WIN32.cpp"
#else
#include "Core_Event_POSIX.cpp"
#endif
 

namespace CORE {
 
 
Core_Event::Core_Event(bool autoReset): Core_EventImpl(autoReset)
{ 
}
 

Core_Event::~Core_Event()
{
}


} 

