#ifndef Core_Event_WIN32_H
#define Core_Event_WIN32_H


#include "Core.h"

namespace CORE {


class CORE_API Core_EventImpl
{
protected:
	
	Core_EventImpl(bool autoReset = true);	
	
	~Core_EventImpl();
	
	void setImpl();
	
	void waitImpl();
	
	bool waitImpl(Int32 milliseconds);
	
	void resetImpl();
	
private:
	
	HANDLE m_event;
};


//
// inlines
//
inline void Core_EventImpl::setImpl()
{
	SetEvent(m_event);
}


inline void Core_EventImpl::resetImpl()
{
	ResetEvent(m_event);
}


}


#endif 

