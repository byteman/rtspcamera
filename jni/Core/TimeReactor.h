#ifndef TimeReactor_H
#define TimeReactor_H

#if defined(CORE_OS_FAMILY_WINDOWS)
#pragma warning(disable:4786)
#endif 


#include "Core.h"
#include "Runnable.h"
#include "TimeEventHandler.h"
#include "TimeNotifier.h"
#include "Timespan.h"
#include "Timestamp.h"
#include "Core_Thread.h"
#include <map>
#include <string>
#include <sstream>


namespace CORE  {


// Class TimeReactor
// It is used for managing timer
class CORE_API TimeReactor:public Runnable
{
public:
	
	TimeReactor();

	TimeReactor(const Timespan& timeout);
	
	virtual ~TimeReactor();
	
public:
		/// @brief
		///	Adds a timer to the TimeReactor.
		/// @param
		/// handler--The callbacker.
		/// seconds--The interval.
		/// @return 
		/// Int32--return the time id.
	Int32 addTimer(TimeEventHandler* handler,Int32 seconds, char *name="");

		/// @brief
		/// Removes a timer from the TimeReactor.
		/// @param
		/// id--The time id.
		/// @return
		/// Int32--return 0 if success,overwise return -1.
	Int32 removeTimer(Int32 id);

	void run();

	void stop();

	void start();

		/// @brief
		/// Sets the precision 
	void setTimeout(const Timespan& timeout);

		/// @brief
		/// Gets the precision 
	const Timespan& getTimeout() const;

		/// @brief
		/// Gets the timeout value 
	const Timespan& getTimeoutTmp() const;

		/// @brief 
		/// Gets timer count
	Int32 getTimerCount();

protected:

		/// @brief
		/// Call back the timer
	void dispatch();
	
private:
	enum
	{
		DEFAULT_TIMEOUT = 1000000
	};
	
	typedef std::map<Int32, TimeNotifier>Core_EventHandlerMap;

	static Int32		 m_nTimeId;
	
	Core_FastMutex		 m_mutex;
	Core_EventHandlerMap	 m_handlers;
	Timespan		  m_timeout;
	Timespan		   m_timeoutTmp;
	Timestamp		 m_preTime;
	volatile   bool		 m_bStop;
	int				m_hasClear;
};


}


#endif

