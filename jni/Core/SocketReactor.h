#ifndef SocketReactor_H
#define SocketReactor_H

#if defined(CORE_OS_FAMILY_WINDOWS)
#pragma warning(disable:4786)
#endif 

#include "Core.h"
#include "Socket.h"
#include "Runnable.h"
#include "Timespan.h"
#include "Timestamp.h"
#include "AutoPtr.h"
#include "DatagramSocket.h"
#include "SocketNotifier.h"
#include "Core_Thread.h"
#include "NetEventHandler.h"
#include <map>
#include <string>



namespace CORE {


class Socket;
class SocketNotifier;

//
// Class SignalHandler
// It is a help class for waking up the reactor
//
class SignalHandler:public NetEventHandler
{
public:
	SignalHandler();
		
	~SignalHandler();

	void notify();
	
	virtual Int32 handle_input(Socket& socket);
	
private:
	DatagramSocket	   m_socket;
	Int32 port;
	
	friend class SocketReactor;
};


/// Class SocketReactor
class CORE_API SocketReactor: public Runnable
{
public:
	SocketReactor();

	SocketReactor(const Timespan& timeout);

	virtual ~SocketReactor();

	void run();
		
	void stop();

	void start();

	void setTimeout(const Timespan& timeout);
		
	const Timespan& getTimeout() const;

		/// @brief:
		/// Adds a handler and the "event mask" to the SocketReactor.
		/// @param:
		/// socket--The socket handle.
		/// handler--The callbacker.
		/// mask--The event mask
	SocketNotifier* addEventHandler(Socket& socket, NetEventHandler* handler,Int32 mask, char *name);

		/// @brief:
		/// Removes  a "event mask"  from a handler.
		/// If the handler's mask is NULL_MASK,it will be destroy by the SocketReactor
		/// @param:
		/// socket--The socket handle.
		/// mask--The event mask that will be removed from the handler.
	void removeEventHandler(Socket& socket, Int32 mask);

	void notifyAll();
	
		/// @brief:
		/// Suspend the socket,so it can't get the notification of readable,writable,error
	void suspend(Socket& socket);

		/// @brief:
		/// Resume the socket
	void resume(Socket& socket);

protected:
	
	void dispatch(Socket& socket,Int32 mask);

private:

	typedef std::map<Socket, SocketNotifier>Core_EventHandlerMap;

	void SignalInit();
	
	enum
	{
		DEFAULT_TIMEOUT = 1000000
	};

	bool				m_bStop;
	Timespan		  m_timeout;
	Core_EventHandlerMap	 m_handlers;
	Core_FastMutex		 m_mutex;
	SignalHandler		 signalHandler;

	friend class SocketNotifier;
};


} 



#endif

