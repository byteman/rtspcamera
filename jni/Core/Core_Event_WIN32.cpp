#include "Core_Event_WIN32.h"


namespace CORE {


Core_EventImpl::Core_EventImpl(bool autoReset)
{
	m_event = CreateEvent(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
}


Core_EventImpl::~Core_EventImpl()
{
	CloseHandle(m_event);
}


void Core_EventImpl::waitImpl()
{
	WaitForSingleObject(m_event, INFINITE);
}

bool Core_EventImpl::waitImpl(Int32 milliseconds)
{
	switch (WaitForSingleObject(m_event, milliseconds + 1))
	{
		case WAIT_TIMEOUT:
		{
			return false;
		}
		case WAIT_OBJECT_0:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}


}

