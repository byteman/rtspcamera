#ifndef Core_Event_H
#define Core_Event_H


#include "Core.h"
#include "Core_Event.h"

#if defined(CORE_OS_FAMILY_WINDOWS)
#include "Core_Event_WIN32.h"
#else
#include "Core_Event_POSIX.h"
#endif


namespace CORE {

/// Class Core_Event
/// An synchronization object
class CORE_API Core_Event: private Core_EventImpl
{
public:
		/// @brief
		/// If autoReset is true, the event object  
		/// will automatically reset after a wait()
		/// successfully.
	Core_Event(bool autoReset = true);

		/// @brief
		/// Destroys the event.
	~Core_Event();

		/// @brief
		/// Signals the event object.
		/// If autoReset is true,only one thread 
		/// waiting for the event can resume execution.
		/// If autoReset is false,all threads waiting for 
		/// the event can resume exrcution.
		/// @note
		/// This maybe throw exception.
	void set();

		/// @brief
		/// Waits for the event to become signalled.
		/// @note
		/// This maybe throw exception.
	void wait();

		/// @brief
		/// Waits for the event to become signalled.
		/// Throws a TimeoutException if the event
		/// does not become signalled within the specified
		/// time interval.
		/// @note
		/// This maybe throw exception.
	bool wait(Int32 milliseconds);

		/// @brief
		/// Waits for the event.
		/// return true if the event become signalled
		/// within the time intervel, false otherwise.
		/// @note
		/// This maybe throw exception.
	bool tryWait(Int32 milliseconds);
 
		/// @brief
		/// Set the event to unsignalled state.
		/// @note
		/// This maybe throw exception.
	void reset();
	
private:
	
	Core_Event(const Core_Event&);
	
	Core_Event& operator = (const Core_Event&);
};


//
// inlines
//
inline void Core_Event::set()
{
	setImpl();
}


inline void Core_Event::wait() 
{
	waitImpl();
}


inline bool Core_Event::wait(Int32 milliseconds)
{
	return waitImpl(milliseconds);
}


inline bool Core_Event::tryWait(Int32 milliseconds)
{
	return waitImpl(milliseconds);
}


inline void Core_Event::reset()
{
	resetImpl();
}


} 


#endif 

