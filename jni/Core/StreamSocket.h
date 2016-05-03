#ifndef StreamSocket_H
#define StreamSocket_H

#if defined(CORE_OS_FAMILY_WINDOWS)
#pragma warning(disable:4541)
#endif 


#include "Core.h"
#include "Socket.h"


namespace CORE {


class StreamSocketImpl;


class CORE_API StreamSocket: public Socket
{
public:
	StreamSocket();

	StreamSocket(const SocketAddress& address);

	StreamSocket(const Socket& socket);
	
	virtual ~StreamSocket();

	StreamSocket& operator = (const Socket& socket);

	Int32 connect(const SocketAddress& address);

	Int32 connect(const SocketAddress& address, const Timespan& timeout);

	Int32 connectNB(const SocketAddress& address);

	void shutdownReceive();
		
	void shutdownSend();
		
	void shutdown();
	
	Int32 sendBytes(const void* buffer, Int32 length, Int32 flags = 0);

	Int32 receiveBytes(void* buffer, Int32 length, Int32 flags = 0);

	Int32 bind(UInt16 port, bool reuseAddress = false);

	Int32 listen(Int32 backlog=5);

	StreamSocket acceptConnection(SocketAddress& clientAddr);

	StreamSocket acceptConnection();

	Int32 setOption(Int32 level, Int32 option, const Timespan& value);
		
public:
	StreamSocket(SocketImpl* pImpl);
	
};


} 


#endif 

