#ifndef Socket_H
#define Socket_H


#include "Core.h"
#include "SocketImpl.h"
#include <vector>


namespace CORE {


#if defined(_WIN32)

class CORE_API NetworkInitializer
{
public:
	NetworkInitializer()
	{
		WORD	version = MAKEWORD(2, 2);
		WSADATA data;
		WSAStartup(version, &data);
	}
		
	~NetworkInitializer()
	{
		WSACleanup();
	}
};
	
static NetworkInitializer networkInitializer;
#endif // _WIN32


/// Socket is the common base class for
/// StreamSocket, DatagramSocket 
///
/// It provides operations common to all socket types.
class CORE_API Socket
{
public:
	enum SelectMode
	{
		SELECT_READ  = 1,
		SELECT_WRITE = 2,
		SELECT_ERROR = 4
	};
	
	typedef std::vector<Socket> SocketList;

	Socket();

	Socket(const Socket& socket);
		
	Socket& operator = (const Socket& socket);
		
	virtual ~Socket();
		
	bool operator == (const Socket& socket) const;

	bool operator != (const Socket& socket) const;

	bool operator <  (const Socket& socket) const;
	
	bool operator <= (const Socket& socket) const;

	bool operator >  (const Socket& socket) const;

	bool operator >= (const Socket& socket) const;
		
	void close();

	static Int32 select(SocketList& readList, SocketList& writeList, SocketList& exceptList, const Timespan& timeout);

	bool poll(const CORE::Timespan& timeout, Int32 mode) const;

	Int32 available() const;

	Int32 setSendBufferSize(Int32 size);
		
	Int32 getSendBufferSize() const;

	Int32 setReceiveBufferSize(Int32 size);
		
	Int32 getReceiveBufferSize() const;

	void setSendTimeout(const Timespan& timeout);
	
	Timespan getSendTimeout() const;

	Int32 setReceiveTimeout(const Timespan& timeout);
	
	Timespan getReceiveTimeout() const;

	Int32 setOption(Int32 level, Int32 option, Int32 value);

	Int32 setOption(Int32 level, Int32 option, unsigned value);

	Int32 setOption(Int32 level, Int32 option, unsigned char value);
		
	Int32 setOption(Int32 level, Int32 option, const Timespan& value);

	Int32 setOption(Int32 level, Int32 option, const char* value,int valueLen);
		
	Int32 getOption(Int32 level, Int32 option, Int32& value) const;

	Int32 getOption(Int32 level, Int32 option, unsigned& value) const;

	Int32 getOption(Int32 level, Int32 option, unsigned char& value) const;

	Int32 getOption(Int32 level, Int32 option, Timespan& value) const;
											
	void setBlocking(bool flag);

	SocketAddress address() const;
		
	SocketAddress peerAddress() const;

	SocketImpl* impl() const;
		
	core_socket_t sockfd() const;
protected:
	
	Socket(SocketImpl* pImpl);

	

private:
	SocketImpl* _pImpl;
	
};


//
// inlines
//
inline bool Socket::operator == (const Socket& socket) const
{
	return _pImpl == socket._pImpl;
}


inline bool Socket::operator != (const Socket& socket) const
{
	return _pImpl != socket._pImpl;
}


inline bool Socket::operator <  (const Socket& socket) const
{
	return _pImpl < socket._pImpl;
}


inline bool Socket::operator <= (const Socket& socket) const
{
	return _pImpl <= socket._pImpl;
}


inline bool Socket::operator >  (const Socket& socket) const
{
	return _pImpl > socket._pImpl;
}


inline bool Socket::operator >= (const Socket& socket) const
{
	return _pImpl >= socket._pImpl;
}

inline void Socket::close()
{
	_pImpl->close();
}


inline bool Socket::poll(const CORE::Timespan& timeout, Int32 mode) const
{
	return _pImpl->poll(timeout, mode);
}


inline Int32 Socket::available() const
{
	return _pImpl->available();
}


inline Int32 Socket::setSendBufferSize(Int32 size)
{
	return _pImpl->setSendBufferSize(size);
}

	
inline Int32 Socket::getSendBufferSize() const
{
	return _pImpl->getSendBufferSize();
}


inline Int32 Socket::setReceiveBufferSize(Int32 size)
{
	return _pImpl->setReceiveBufferSize(size);
}

	
inline Int32 Socket::getReceiveBufferSize() const
{
	return _pImpl->getReceiveBufferSize();
}


inline void Socket::setSendTimeout(const Timespan& timeout)
{
	_pImpl->setSendTimeout(timeout);
}


inline CORE::Timespan Socket::getSendTimeout() const
{
	return _pImpl->getSendTimeout();
}


inline Int32 Socket::setReceiveTimeout(const Timespan& timeout)
{
	return _pImpl->setReceiveTimeout(timeout);
}


inline CORE::Timespan Socket::getReceiveTimeout() const
{
	return _pImpl->getReceiveTimeout();
}


inline Int32 Socket::setOption(Int32 level, Int32 option, Int32 value)
{
	return _pImpl->setOption(level, option, value);
}


inline Int32 Socket::setOption(Int32 level, Int32 option, unsigned value)
{
	return _pImpl->setOption(level, option, value);
}


inline Int32 Socket::setOption(Int32 level, Int32 option, unsigned char value)
{
	return _pImpl->setOption(level, option, value);
}


inline Int32 Socket::setOption(Int32 level, Int32 option, const Timespan& value)
{
	return _pImpl->setOption(level, option, value);
}

inline Int32 Socket::setOption(Int32 level, Int32 option, const char* value,int valueLen)
{
	return _pImpl->setOption(level, option, value, valueLen);
}	

inline Int32 Socket::getOption(Int32 level, Int32 option, Int32& value) const
{
	return _pImpl->getOption(level, option, value);
}


inline Int32 Socket::getOption(Int32 level, Int32 option, unsigned& value) const
{
	return _pImpl->getOption(level, option, value);
}


inline Int32 Socket::getOption(Int32 level, Int32 option, unsigned char& value) const
{
	return _pImpl->getOption(level, option, value);
}


inline Int32 Socket::getOption(Int32 level, Int32 option, Timespan& value) const
{
	return _pImpl->getOption(level, option, value);
}


inline void Socket::setBlocking(bool flag)
{
	_pImpl->setBlocking(flag);
}


inline SocketImpl* Socket::impl() const
{
	return _pImpl;
}


inline core_socket_t Socket::sockfd() const
{
	return _pImpl->sockfd();
}


inline SocketAddress Socket::address() const
{
	return _pImpl->address();
}

	
inline SocketAddress Socket::peerAddress() const
{
	return _pImpl->peerAddress();
}



} 


#endif 

