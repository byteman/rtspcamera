#include "DatagramSocket.h"
#include "DatagramSocketImpl.h"
#include "Exception.h"



namespace CORE {


DatagramSocket::DatagramSocket(): Socket(new DatagramSocketImpl)
{

}



DatagramSocket::DatagramSocket(UInt16 port, bool reuseAddress): Socket(new DatagramSocketImpl)
{
	bind(port, reuseAddress);
}


DatagramSocket::DatagramSocket(const Socket& socket): Socket(socket)
{
	if (!dynamic_cast<DatagramSocketImpl*>(impl()))
	{
		throw InvalidArgumentException("Cannot assign incompatible socket");
	}
}


DatagramSocket::DatagramSocket(SocketImpl* pImpl): Socket(pImpl)
{
	if (!dynamic_cast<DatagramSocketImpl*>(impl()))
	{
		throw InvalidArgumentException("Cannot assign incompatible socket");
	}
}


DatagramSocket::~DatagramSocket()
{
}


DatagramSocket& DatagramSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<DatagramSocketImpl*>(socket.impl()))
	{
		Socket::operator = (socket);
	}
	else
	{
		throw InvalidArgumentException("Cannot assign incompatible socket");
	}
	return *this;
}


Int32 DatagramSocket::connect(const SocketAddress& address)
{
	return impl()->connect(address);
}


Int32 DatagramSocket::bind(UInt16 port, bool reuseAddress)
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);		 
	addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	
	SocketAddress address(reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr));
	return impl()->bind(address, reuseAddress);
}


Int32 DatagramSocket::bindaddr(SocketAddress& address, bool reuseAddress)
{

	return impl()->bind(address, reuseAddress);
}

Int32 DatagramSocket::sendBytes(const void* buffer, Int32 length, Int32 flags)
{
	return impl()->sendBytes(buffer, length, flags);
}


Int32 DatagramSocket::receiveBytes(void* buffer, Int32 length, Int32 flags)
{
	return impl()->receiveBytes(buffer, length, flags);
}


Int32 DatagramSocket::sendTo(const void* buffer, Int32 length, const SocketAddress& address, Int32 flags)
{
	return impl()->sendTo(buffer, length, address, flags);
}


Int32 DatagramSocket::receiveFrom(void* buffer, Int32 length, SocketAddress& address, Int32 flags)
{
	return impl()->receiveFrom(buffer, length, address, flags);
}


} 


