#ifndef SocketImpl_H
#define SocketImpl_H


#include "Core.h"
#include "SocketAddress.h"
#include "RefCountedObject.h"
#include "Timespan.h"
#include "Exception.h"


namespace CORE {


/// Subclasses implement specific socket types like
/// stream or datagram sockets.
///
/// You should not create any instances of this class.
class CORE_API SocketImpl: public RefCountedObject
{
public:
	enum SelectMode
	{
		SELECT_READ  = 1,
		SELECT_WRITE = 2,
		SELECT_ERROR = 4
	};

		/// @brief
		/// returns a new TCP socket for the connection
		/// with the client.
		///
		/// The client socket's address is returned in clientAddr.
	virtual SocketImpl* acceptConnection(SocketAddress& clientAddr);

		/// @brief
		/// Initializes the socket and establishes a connection to 
		/// the TCP server at the given address.
	virtual Int32 connect(const SocketAddress& address);

	virtual Int32 connect(const SocketAddress& address, const Timespan& timeout);

		/// @brief
		/// Initializes the socket and establishes a connection to 
		/// the TCP server at the given address. Prior to opening the
		/// connection the socket is set to nonblocking mode.
	virtual Int32 connectNB(const SocketAddress& address);

		/// @brief
		/// Bind a local address to the socket.
	virtual Int32 bind(const SocketAddress& address, bool reuseAddress = false);

		/// @brief
		/// Puts the socket into listening state.
	virtual Int32 listen(Int32 backlog = 5);

		/// @brief
		/// Close the socket.
	virtual void close();

		/// @brief
		/// Shuts down the receiving part of the socket connection.
	virtual void shutdownReceive();
	
		/// @brief	
		/// Shuts down the sending part of the socket connection.
	virtual void shutdownSend();
	
		/// @brief	
		/// Shuts down both the receiving and the sending part
		/// of the socket connection.
	virtual void shutdown();
	
		/// @brief
		/// Sends the contents of the given buffer through
		/// the socket.
	virtual Int32 sendBytes(const void* buffer, Int32 length, Int32 flags = 0);

		/// @brief
		/// Receives data from the socket and stores it
		/// in buffer. 
	virtual Int32 receiveBytes(void* buffer, Int32 length, Int32 flags = 0);

		/// @brief
		/// Sends the contents of the given buffer through
		/// the socket to the given address.
	virtual Int32 sendTo(const void* buffer, Int32 length, const SocketAddress& address, Int32 flags = 0);

		/// @brief
		/// Receives data from the socket and stores it
		/// in buffer. 
	virtual Int32 receiveFrom(void* buffer, Int32 length, SocketAddress& address, Int32 flags = 0);

		/// @brief
		/// returns the number of bytes available that can be read
		/// without causing the socket to block.
	virtual Int32 available();

		/// @brief
		/// Determines the status of the socket, using a 
		/// call to select().
	virtual bool poll(const Timespan& timeout, Int32 mode);

		/// @brief
		/// Sets the size of the send buffer.
	virtual Int32 setSendBufferSize(Int32 size);

		/// @brief
		/// returns the size of the send buffer.
	virtual Int32 getSendBufferSize();

		/// @brief
		/// Sets the size of the receive buffer.
	virtual Int32 setReceiveBufferSize(Int32 size);

		/// @brief
		/// returns the size of the receive buffer.
	virtual Int32 getReceiveBufferSize();

		/// @brief
		/// Sets the send timeout for the socket.
	virtual Int32 setSendTimeout(const Timespan& timeout);

		/// @brief
		/// returns the send timeout for the socket.
	virtual Timespan getSendTimeout();

		/// @brief
		/// Sets the send timeout for the socket.
	virtual Int32 setReceiveTimeout(const Timespan& timeout);

		/// @brief
		/// returns the receive timeout for the socket.
	virtual Timespan getReceiveTimeout();

		/// @brief
		/// returns the IP address and port number of the socket.
	virtual SocketAddress address();

		/// @brief
		/// returns the IP address and port number of the peer socket.
	virtual SocketAddress peerAddress();

	virtual void init();
	
	Int32 setOption(Int32 level, Int32 option, Int32 value);

	Int32 setOption(Int32 level, Int32 option, unsigned value);

	Int32 setOption(Int32 level, Int32 option, unsigned char value);
		
	Int32 setOption(Int32 level, Int32 option, const Timespan& value);

	Int32 setOption(Int32 level, Int32 option, const char* value,int valueLen);
		
	virtual Int32 setRawOption(Int32 level, Int32 option, const void* value, core_socklen_t length);
		
	Int32 getOption(Int32 level, Int32 option, Int32& value);

	Int32 getOption(Int32 level, Int32 option, unsigned& value);

	Int32 getOption(Int32 level, Int32 option, unsigned char& value);

	Int32 getOption(Int32 level, Int32 option, Timespan& value);
	
	virtual Int32 getRawOption(Int32 level, Int32 option, void* value, core_socklen_t& length);

		/// @brief
		/// Sets the socket in blocking mode if flag is true,
		/// disables blocking mode if flag is false.
	void setBlocking(bool flag);

		/// @brief
		/// returns the value of the SO_ERROR socket option.
	Int32 socketError();

		/// @brief
		/// returns the socket descriptor for socket.
	core_socket_t sockfd();
	
	void ioctl(Int32 request, Int32& arg);

	void ioctl(Int32 request, void* arg);

		/// @brief
		/// returns true if the socket is initialized.
	bool initialized() const;
	
protected:
	SocketImpl();
		
	SocketImpl(core_socket_t sockfd);

	virtual ~SocketImpl();

		/// @brief
		/// Creates the socket.
	void initSocket(Int32 af, Int32 type, Int32 proto = 0);

	void setSockfd(core_socket_t aSocket);

		/// @brief
		/// Sets a socket to CORE_INVALID_SOCKET.
	void invalidate();

		/// @brief
		/// returns the last error code.
	static Int32 lastError();

		/// @brief
		/// Throws an exception for the last error.
	static void error();

	static void error(const CData& arg);

	static void error(Int32 code);
		
	static void error(Int32 code, const CData& arg);

private:
	SocketImpl(const SocketImpl&);
	SocketImpl& operator = (const SocketImpl&);
	
	core_socket_t _sockfd;
	
#if defined(CORE_BROKEN_TIMEOUTS)
	Timespan _recvTimeout;
	Timespan _sndTimeout;
#endif
	
	friend class Socket;
};


//
// inlines
//
inline core_socket_t SocketImpl::sockfd()
{
	return _sockfd;
}


inline bool SocketImpl::initialized() const
{
	return _sockfd != CORE_INVALID_SOCKET;
}


inline Int32 SocketImpl::lastError()
{
#if defined(_WIN32)
	return WSAGetLastError();
#else
	return errno;
#endif
}


inline void SocketImpl::invalidate()
{
	_sockfd = CORE_INVALID_SOCKET;
}


} 


#endif 

