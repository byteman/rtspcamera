#ifndef Core_Thread_H
#define Core_Thread_H


#include "Core.h"
#include "Core_Mutex.h"
#include "Data.h"


#if defined(CORE_OS_FAMILY_WINDOWS)
#include "Core_Thread_WIN32.h"
#else
#include "Core_Thread_POSIX.h"
#endif


namespace CORE {


class Runnable;

/// Class Core_Thread
/// For threads
class CORE_API Core_Thread: private Core_ThreadImpl
{
public:	
	enum Priority
	{
		PRIO_LOWEST  = PRIO_LOWEST_IMPL, /// The lowest thread priority.
		PRIO_LOW	 = PRIO_LOW_IMPL,	/// A lower than normal thread priority.
		PRIO_NORMAL  = PRIO_NORMAL_IMPL, /// The normal thread priority.
		PRIO_HIGH	= PRIO_HIGH_IMPL,   /// A higher than normal thread priority.
		PRIO_HIGHEST = PRIO_HIGHEST_IMPL /// The highest thread priority.
	};	
	
		/// @brief
		/// Constructor.
		/// Call start() to start it.
	Core_Thread();
	
		/// @brief
		/// Creates a named thread.
	Core_Thread(const CData& name);
	
		/// @brief
		/// Destructor.
	~Core_Thread();
	
		/// @brief
		/// returns the unique id of the thread.
	Int32 id() const;
	
		/// @brief
		/// returns the name of the thread.
	CData name() const;
	
		/// @brief
		/// return the name of the thread.
	CData getName() const;
	
		/// @brief
		/// Sets the name of the thread.
	void setName(const CData& name);
	
		/// @brief
		/// Sets the priority of the thread.
	void setPriority(Priority prio);
	
		/// @brief
		/// Gets the priority of the thread.
	Priority getPriority() const;
	
		/// @brief
		/// Starts the thread.
	void start(Runnable& target);

		/// @brief
		/// Stops the thread.
		/// @note
		/// You must do it as follow:
		///
		/// step 1.	stat(target1);
		/// step 2.	join();
		/// step 3.	stop();
		/// step 4.	stat(target2);
		/// ...
	void stop();
	
		/// @brief
		/// Waits until the thread completes execution.
	void join();
		
	bool join(Int32 milliseconds);
		
	bool tryJoin(Int32 milliseconds);
	
		/// @brief
		/// returns the state.
	bool isRunning() const;
	
		/// @brief
		/// Sleep the current thread.
	static void sleep(Int32 milliseconds);
	
		/// @brief
		/// yield cpu to the other threads.
	static void yield();
	
		/// @brief
		/// returns the Core_Thread object for currently active thread.
	static Core_Thread* current();

protected:
	
		/// @brief
		/// Creates and returns a name for a thread.
	CData makeName();
	
		/// @brief
		/// Creates and returns unique id for a thread
	static Int32 uniqueId();
		
private:
	
	Core_Thread(const Core_Thread&);
	
	Core_Thread& operator = (const Core_Thread&);

	Int32						 m_nId;
	CData			 m_name;
	mutable Core_FastMutex	m_mutex;

	friend class PooledCore_Thread;
};


//
// inlines
//
inline Int32 Core_Thread::id() const
{
	return m_nId;
}


inline CData Core_Thread::name() const
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	return m_name;
}


inline CData Core_Thread::getName() const
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	return m_name;
}


inline bool Core_Thread::isRunning() const
{
	return isRunningImpl();
}


inline void Core_Thread::sleep(Int32 milliseconds)
{
	sleepImpl(milliseconds);
}


inline void Core_Thread::yield()
{
	yieldImpl();
}


inline Core_Thread* Core_Thread::current()
{
	return static_cast<Core_Thread*>(currentImpl());
}


} 


#endif 

