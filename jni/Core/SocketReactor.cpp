#include "SocketReactor.h"
#include "SocketNotifier.h"
#include "Exception.h"
#include "SingletonHolder.h"
#include "Timestamp.h"
#include "Timespan.h"
#include "Logger.h"


namespace CORE {


SignalHandler::SignalHandler()
{
	while(1)
	{
		try
		{
			m_socket.bind(0);
			port=m_socket.address().port();
			break;
		}
		catch(...)
		{
			
		}
	}
}
		
SignalHandler::~SignalHandler()
{
}


void SignalHandler::notify()
{
	m_socket.sendTo("notify all", 10,SocketAddress("127.0.0.1",port),0);
}
	

Int32 SignalHandler::handle_input(Socket& socket)
{
	char buffer[32];
	Int32 n = m_socket.receiveBytes(buffer, sizeof(buffer));
	buffer[n]=0;
	return 0;
}


SocketReactor::SocketReactor():
	m_bStop(false),
	m_timeout(DEFAULT_TIMEOUT)
{
	SignalInit();
}


SocketReactor::SocketReactor(const Timespan& timeout):
	m_bStop(false),
	m_timeout(timeout)
{
	SignalInit();
}


void SocketReactor::SignalInit()
{
	addEventHandler(signalHandler.m_socket,&signalHandler,NetEventHandler::READ_MASK, "signalHandler");
}

SocketReactor::~SocketReactor()
{
	
}


void SocketReactor::run()
{
	Socket::SocketList readable;
	Socket::SocketList writable;
	Socket::SocketList except;

	if(!m_bStop)
	{
		readable.clear();
		writable.clear();
		except.clear();
		
		{
			Core_FastMutex::ScopedLock lock(m_mutex);
			for (Core_EventHandlerMap::iterator it = m_handlers.begin(); it != m_handlers.end(); ++it)
			{
				SocketNotifier *sockNtfPtr;

				sockNtfPtr = &it->second;
				
				if(sockNtfPtr->isSuspend())
				{
					continue;
				}
				
				if (sockNtfPtr->accepts(NetEventHandler::READ_MASK))
				{
					readable.push_back(it->first);
				}
				
				if (sockNtfPtr->accepts(NetEventHandler::WRITE_MASK))
				{
					writable.push_back(it->first);
				}
				
				if (sockNtfPtr->accepts(NetEventHandler::EXCEPT_MASK))
				{
					except.push_back(it->first);
				}
			}
		}

		
		if (Socket::select(readable, writable, except, m_timeout))
		{
			for (Socket::SocketList::iterator it1 = readable.begin(); it1 != readable.end(); ++it1)
			{
				dispatch(*it1, NetEventHandler::READ_MASK);
			}
			for (Socket::SocketList::iterator it2 = writable.begin(); it2 != writable.end(); ++it2)
			{
				dispatch(*it2, NetEventHandler::WRITE_MASK);
			}
			for (Socket::SocketList::iterator it3 = except.begin(); it3 != except.end(); ++it3)
			{
				dispatch(*it3, NetEventHandler::EXCEPT_MASK);
			}
		}

		bool run=true;
		while(run)
		{
			run=false;
			
			Core_FastMutex::ScopedLock lock(m_mutex);
			for (Core_EventHandlerMap::iterator it= m_handlers.begin(); it != m_handlers.end(); ++it)
			{
				SocketNotifier *sockNtfPtr;
				
				sockNtfPtr = &it->second;
				if(true==sockNtfPtr->isClear())
				{
					sockNtfPtr->removeEventHandler(sockNtfPtr->getClearMask());
					sockNtfPtr->setClear(false);
					if(sockNtfPtr->hasMask()==false)
					{
						m_handlers.erase(it);
					}
					run=true;
					break;
				}
			}
		}
	}
}

	
void SocketReactor::stop()
{
	m_bStop = true;
	notifyAll();

	Core_FastMutex::ScopedLock lock(m_mutex);
	m_handlers.clear();
}


void SocketReactor::start()
{
	m_bStop = false;
}


void SocketReactor::setTimeout(const Timespan& timeout)
{
	m_timeout = timeout;
}

	
const Timespan& SocketReactor::getTimeout() const
{
	return m_timeout;
}


SocketNotifier* SocketReactor::addEventHandler(Socket& socket, NetEventHandler* handler,Int32 mask, char *name)
{
	if(strlen(name)>99)
	{
		_LogError<<"NetEventHandler'name more long"<<_LogErrorEnd;
		throw LogicException("NetEventHandler'name more long");
	}
	
	SocketNotifier *sockNtfPtr;
	
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	Core_EventHandlerMap::iterator it = m_handlers.find(socket);
	if (it == m_handlers.end())
	{
		SocketNotifier sockNtf;
		m_handlers[socket] = sockNtf;
	}

	it = m_handlers.find(socket);
	sockNtfPtr = &it->second;
	sockNtfPtr->addEventHandler(handler, mask, name);

	notifyAll();
	
	return sockNtfPtr;
}


void SocketReactor::suspend(Socket& socket)
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	SocketNotifier *sockNtfPtr;
	
	Core_EventHandlerMap::iterator it = m_handlers.find(socket);
	if (it == m_handlers.end())
	{
		return;
	}
	else 
	{
		sockNtfPtr = &it->second;
	}

	sockNtfPtr->suspend();
	notifyAll();
}


void SocketReactor::resume(Socket& socket)
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	SocketNotifier *sockNtfPtr;
	
	Core_EventHandlerMap::iterator it = m_handlers.find(socket);
	if (it == m_handlers.end())
	{
		return;
	}
	else 
	{
		sockNtfPtr = &it->second;
	}

	sockNtfPtr->resume();
	notifyAll();

}


void SocketReactor::removeEventHandler(Socket& socket, Int32 mask)
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	Core_EventHandlerMap::iterator it = m_handlers.find(socket);
	if (it != m_handlers.end())
	{
		SocketNotifier *sockNtfPtr;
		sockNtfPtr = &it->second;
		sockNtfPtr->setClearMask(mask);
		sockNtfPtr->setClear(true);
	}
}


void SocketReactor::notifyAll()
{
	signalHandler.notify();
}


void SocketReactor::dispatch(Socket& socket,Int32 mask)
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	SocketNotifier *sockNtfPtr;
	
	Core_EventHandlerMap::iterator it = m_handlers.find(socket);
	if (it != m_handlers.end())
	{
		sockNtfPtr = &it->second;
		Int32 ret=0;

		{
			Core_FastMutex::ScopedUnLock unlock(m_mutex);

			try
			{
				ret=sockNtfPtr->dispatch(socket, mask);
			}
			catch(...)
			{
				_LogError<<"Sock dispatch error:"<<sockNtfPtr->getName()<<_LogErrorEnd;
				ret=-1;
			}
		}
		
		if(ret<0)
		{
			sockNtfPtr->setClearMask(mask);
			sockNtfPtr->setClear(true);
		}
	}
}

} 

