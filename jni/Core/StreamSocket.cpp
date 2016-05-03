#include "StreamSocket.h"
#include "StreamSocketImpl.h"
#include "Exception.h"


namespace CORE {


StreamSocket::StreamSocket(): Socket(new StreamSocketImpl)
{
}


StreamSocket::StreamSocket(const SocketAddress& address): Socket(new StreamSocketImpl)
{
	connect(address);
}


StreamSocket::StreamSocket(const Socket& socket): Socket(socket)
{
	if (!dynamic_cast<StreamSocketImpl*>(impl()))
	{
		throw InvalidArgumentException("Cannot assign incompatible socket");
	}
}


StreamSocket::StreamSocket(SocketImpl* pImpl): Socket(pImpl)
{
	if (!dynamic_cast<StreamSocketImpl*>(impl()))
	{
		throw InvalidArgumentException("Cannot assign incompatible socket");
	}
}


StreamSocket::~StreamSocket()
{
}


StreamSocket& StreamSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<StreamSocketImpl*>(socket.impl()))
	{
		Socket::operator = (socket);
	}
	else
	{
		throw InvalidArgumentException("Cannot assign incompatible socket");
	}
	return *this;
}


Int32 StreamSocket::connect(const SocketAddress& address)
{
	return impl()->connect(address);
}


Int32 StreamSocket::connect(const SocketAddress& address, const Timespan& timeout)
{
	return impl()->connect(address, timeout);
}


Int32 StreamSocket::connectNB(const SocketAddress& address)
{
	return impl()->connectNB(address);
}


void StreamSocket::shutdownReceive()
{
	impl()->shutdownReceive();
}

	
void StreamSocket::shutdownSend()
{
	impl()->shutdownSend();
}

	
void StreamSocket::shutdown()
{
	impl()->shutdown();
}


Int32 StreamSocket::sendBytes(const void* buffer, Int32 length, Int32 flags)
{
	return impl()->sendBytes(buffer, length, flags);
}


Int32 StreamSocket::receiveBytes(void* buffer, Int32 length, Int32 flags)
{
	return impl()->receiveBytes(buffer, length, flags);
}


Int32 StreamSocket::bind(UInt16 port, bool reuseAddress)
{
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);	 
	addr.sin_addr.s_addr = htonl(INADDR_ANY);		 
	SocketAddress address(reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr));
	return impl()->bind(address, reuseAddress);
}


Int32 StreamSocket::listen(Int32 backlog)
{
	return impl()->listen(backlog);
}


StreamSocket StreamSocket::acceptConnection(SocketAddress& clientAddr)
{
	return StreamSocket(impl()->acceptConnection(clientAddr));
}


StreamSocket StreamSocket::acceptConnection()
{
	SocketAddress clientAddr;
	return StreamSocket(impl()->acceptConnection(clientAddr));
}

Int32 StreamSocket::setOption(Int32 level, Int32 option, const Timespan& value)
{
	return impl()->setOption(level,option,value);
}


} 

