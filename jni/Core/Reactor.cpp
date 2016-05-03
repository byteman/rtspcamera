#include "Reactor.h"
#include "SingletonHolder.h"
#include "SocketReactor.h"
#include "TimeReactor.h"
#include "Logger.h"


namespace CORE {


Reactor::Reactor():
	m_bStop(false)
{
	
}


Reactor::~Reactor()
{

}


void Reactor::stop()
{
	m_sockReactor.stop();
	m_timeReactor.stop();
	m_sockReactor.notifyAll();

	m_sockReactor.run();
	m_bStop = true;
	m_thread.join();	
	m_thread.stop();
}


void Reactor::start()
{

	m_sockReactor.start();
	
	m_timeReactor.start();

	m_bStop = false;
	m_thread.setName("Reactor_Core_Thread");
	m_thread.start(*this);
}


void Reactor::run()
{
	while(!m_bStop)
	{
		m_sockReactor.run();
		m_timeReactor.run();
		m_sockReactor.setTimeout(m_timeReactor.getTimeoutTmp());
	}
}


Reactor& Reactor::instance()
{
	
	static SingletonHolder<Reactor> sh; 
	
	Reactor* p = sh.get();

	Reactor& tmp = *p;
	
	return tmp;
}

Int32 Reactor::addTimer(TimeEventHandler* handler,Int32 seconds, char *name)
{
	return m_timeReactor.addTimer(handler, seconds, name);
}


Int32 Reactor::removeTimer(Int32 id)
{
	return m_timeReactor.removeTimer(id);
}


SocketNotifier* Reactor::addEventHandler(Socket& socket, NetEventHandler* handler,Int32 mask, char  *name)
{
	return m_sockReactor.addEventHandler(socket, handler, mask, name);
}


void Reactor::removeEventHandler(Socket& socket,Int32 mask)
{
	m_sockReactor.removeEventHandler(socket, mask);
}


void Reactor::suspend(Socket& socket)
{
	m_sockReactor.suspend(socket);
}


void Reactor::resume(Socket& socket)
{
	m_sockReactor.resume(socket);
}

	
}


