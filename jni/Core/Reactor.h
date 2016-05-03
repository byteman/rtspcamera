#ifndef Reactor_H
#define Reactor_H


#include "Core.h"
#include "Core_Thread.h"
#include "Runnable.h"
#include <string>
#include "SocketReactor.h"
#include "TimeReactor.h"


#define DefaultReactor \
	CORE::Reactor::defaultReactor


namespace CORE {


/// Class Reactor
/// It includes SocketReactor and TimeReactor
class CORE_API Reactor:public Runnable
{
public:
	
	Reactor();
	
	virtual ~Reactor();
	
public:

	void run();
	
	void stop();

	void start();

	Int32 addTimer(TimeEventHandler* handler,Int32 seconds, char *name);

	Int32 removeTimer(Int32 id);

	SocketNotifier* addEventHandler(Socket& socket, NetEventHandler* handler,Int32 mask, char  *name);

	void removeEventHandler(Socket& socket,Int32 mask);

	void suspend(Socket& socket);

	void resume(Socket& socket);
	
	static Reactor& instance();

private:
	
	//volatile bool  m_bStop;
	bool  m_bStop;
	SocketReactor m_sockReactor;
	TimeReactor m_timeReactor;
	Core_Thread m_thread; 


};


}


#endif

