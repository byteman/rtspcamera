#ifndef DatagramSocket_H
#define DatagramSocket_H


#include "Core.h"
#include "Socket.h"


namespace CORE {


class CORE_API DatagramSocket: public Socket
{
public:
	DatagramSocket();

	DatagramSocket(UInt16 port, bool reuseAddress = false);

	DatagramSocket(const Socket& socket);

	~DatagramSocket();

	DatagramSocket& operator = (const Socket& socket);

	Int32 connect(const SocketAddress& address);

	Int32 bind(UInt16 port, bool reuseAddress = false);

	Int32 bindaddr(SocketAddress& address, bool reuseAddress = false);

	Int32 sendBytes(const void* buffer, Int32 length, Int32 flags = 0);

	Int32 receiveBytes(void* buffer, Int32 length, Int32 flags = 0);

	Int32 sendTo(const void* buffer, Int32 length, const SocketAddress& address, Int32 flags = 0);
	
	Int32 receiveFrom(void* buffer, Int32 length, SocketAddress& address, Int32 flags = 0);

protected:
	DatagramSocket(SocketImpl* pImpl);
};



} 



#endif 

