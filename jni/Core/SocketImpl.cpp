#include "SocketImpl.h"
#include "StreamSocketImpl.h"
#include "Timestamp.h"
#include <string.h> // FD_SET needs memset on some platforms, so we can't use <cstring>


namespace CORE {


SocketImpl::SocketImpl():
	_sockfd(CORE_INVALID_SOCKET)
{
}


SocketImpl::SocketImpl(core_socket_t sockfd):
	_sockfd(sockfd)
{
}


SocketImpl::~SocketImpl()
{
	close();
}


void SocketImpl::init()
{


}


SocketImpl* SocketImpl::acceptConnection(SocketAddress& clientAddr)
{
	char buffer[SocketAddress::MAX_ADDRESS_LENGTH];
	struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
	core_socklen_t saLen = sizeof(buffer);
	core_socket_t sd;
	do
	{
		sd = ::accept(_sockfd, pSA, &saLen);
	}
	while (sd == CORE_INVALID_SOCKET && lastError() == CORE_EINTR);
	
	if (sd != CORE_INVALID_SOCKET)
	{
		clientAddr = SocketAddress(pSA, saLen);
		return new StreamSocketImpl(sd);
	}
	else
	{
		error(); 
	}
	return 0;
}


Int32 SocketImpl::connect(const SocketAddress& address)
{
	if (_sockfd == CORE_INVALID_SOCKET)
	{	
		init();
	}
	
	Int32 rc;
	do
	{
		rc = ::connect(_sockfd, address.addr(), address.length());
	}
	while (rc != 0 && lastError() == CORE_EINTR);
	if (rc != 0) 
	{
		error(address.toString());
	}

	return rc;
}


Int32 SocketImpl::connect(const SocketAddress& address, const Timespan& timeout)
{
	if (_sockfd == CORE_INVALID_SOCKET)
	{	
		init();
	}
	Int32 rc;
	setBlocking(false);
	try
	{
		rc = ::connect(_sockfd, address.addr(), address.length());
		
		if (rc != 0)
		{
			Int32 err;
			bool ret;
			err =lastError();
			
			if (err != CORE_EINPROGRESS &&err != CORE_EWOULDBLOCK)
			{
				error(address.toString());
			}
			
			ret=poll(timeout, SELECT_READ | SELECT_WRITE);
			if (!ret)
			{
				throw TimeoutException("connect timed out", address.toString());
			}
			err = socketError();
			if (err != 0) 
			{
				error(err);
			}
		}
	}
	catch (Exception&)
	{
		setBlocking(true);
		throw;
	}
	setBlocking(true);
	return rc;
}


Int32 SocketImpl::connectNB(const SocketAddress& address)
{
	if (_sockfd == CORE_INVALID_SOCKET)
	{	
		init();
	}
	
	setBlocking(false);
	Int32 rc = ::connect(_sockfd, address.addr(), address.length());
	if (rc != 0)
	{
		if (lastError() != CORE_EINPROGRESS && lastError() != CORE_EWOULDBLOCK)
		{
			error(address.toString());
		}
	}
	return rc;
}


Int32 SocketImpl::bind(const SocketAddress& address, bool reuseAddress)
{
	if (_sockfd == CORE_INVALID_SOCKET)
	{	
		init();
	}
	
	if (reuseAddress)
	{
		try
		{
			Int32 value = 1;
			setOption(SOL_SOCKET, SO_REUSEADDR, value);
		}
		catch (...)
		{
		}
	}
	Int32 rc = ::bind(_sockfd, address.addr(), address.length());
	if (rc != 0) 
	{
		error(address.toString());
	}

	return rc;
}

	
Int32 SocketImpl::listen(Int32 backlog)
{
	Int32 rc = ::listen(_sockfd, backlog);
	if (rc != 0) 
	{
		error();
	}
	return rc;
}


void SocketImpl::close()
{
	if (_sockfd != CORE_INVALID_SOCKET)
	{
		core_closesocket(_sockfd);
		_sockfd = CORE_INVALID_SOCKET;
	}
}


void SocketImpl::shutdownReceive()
{
	Int32 rc = ::shutdown(_sockfd, 0);
	if (rc != 0) 
	{
		error();
	}
}

	
void SocketImpl::shutdownSend()
{
	Int32 rc = ::shutdown(_sockfd, 1);
	if (rc != 0) 
	{
		error();
	}
}

	
void SocketImpl::shutdown()
{
	Int32 rc = ::shutdown(_sockfd, 2);
	if (rc != 0) 
	{
		error();
	}
}


Int32 SocketImpl::sendBytes(const void* buffer, Int32 length, Int32 flags)
{
#if defined(CORE_BROKEN_TIMEOUTS)
	if (_sndTimeout.totalMicroseconds() != 0)
	{
		if (!poll(_sndTimeout, SELECT_WRITE))
		{
			throw TimeoutException();
		}
	}
#endif

	Int32 rc;
	do
	{
		rc = ::send(_sockfd, reinterpret_cast<const char*>(buffer), length, flags);
	}
	while (rc < 0 && lastError() == CORE_EINTR);
	
	if (rc < 0) 
	{
		error();
	}
	return rc;
}


Int32 SocketImpl::receiveBytes(void* buffer, Int32 length, Int32 flags)
{
#if defined(CORE_BROKEN_TIMEOUTS)
	if (_recvTimeout.totalMicroseconds() != 0)
	{
		if (!poll(_recvTimeout, SELECT_READ))
		{
			throw TimeoutException();
		}
	}
#endif

	Int32 rc;
	do
	{
		rc = ::recv(_sockfd, reinterpret_cast<char*>(buffer), length, flags);
	}
	while (rc < 0 && lastError() == CORE_EINTR);
	
	if (rc < 0) 
	{
		if (lastError() == CORE_EAGAIN || lastError() == CORE_ETIMEDOUT)
		{
			throw TimeoutException();
		}
		else
		{
			error();
		}
	}
	return rc;
}


Int32 SocketImpl::sendTo(const void* buffer, Int32 length, const SocketAddress& address, Int32 flags)
{
	Int32 rc;
	do
	{
		rc = ::sendto(_sockfd, reinterpret_cast<const char*>(buffer), length, flags, address.addr(), address.length());
	}
	while (rc < 0 && lastError() == CORE_EINTR);
	
	if (rc < 0) 
	{
		error();
	}
	return rc;
}


Int32 SocketImpl::receiveFrom(void* buffer, Int32 length, SocketAddress& address, Int32 flags)
{
#if defined(CORE_BROKEN_TIMEOUTS)
	if (_recvTimeout.totalMicroseconds() != 0)
	{
		if (!poll(_recvTimeout, SELECT_READ))
		{
			throw TimeoutException();
		}
	}
#endif
	
	char abuffer[SocketAddress::MAX_ADDRESS_LENGTH];
	struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(abuffer);
	core_socklen_t saLen = sizeof(abuffer);
	Int32 rc;
	do
	{
		rc = ::recvfrom(_sockfd, reinterpret_cast<char*>(buffer), length, flags, pSA, &saLen);
	}
	while (rc < 0 && lastError() == CORE_EINTR);
	
	if (rc >= 0)
	{
		address = SocketAddress(pSA, saLen);
	}
	else
	{
		if (lastError() == CORE_EAGAIN || lastError() == CORE_ETIMEDOUT)
		{
			throw TimeoutException();
		}
		else
		{
			error();
		}
	}
	return rc;
}


Int32 SocketImpl::available()
{
	Int32 result;
	ioctl(FIONREAD, result);
	return result;
}


bool SocketImpl::poll(const Timespan& timeout, Int32 mode)
{
	fd_set fdRead;
	fd_set fdWrite;
	fd_set fdExcept;
	FD_ZERO(&fdRead);
	FD_ZERO(&fdWrite);
	FD_ZERO(&fdExcept);
	
	if (mode & SELECT_READ)
	{
		FD_SET(_sockfd, &fdRead);
	}
	
	if (mode & SELECT_WRITE)
	{
		FD_SET(_sockfd, &fdWrite);
	}
	
	if (mode & SELECT_ERROR)
	{
		FD_SET(_sockfd, &fdExcept);
	}
	
	Timespan remainingTime(timeout);
	Int32 rc;
	do
	{
		struct timeval tv;
		tv.tv_sec  = (Int32) remainingTime.totalSeconds();
		tv.tv_usec = (Int32) remainingTime.useconds();
		Timestamp start;
		rc = ::select(Int32(_sockfd) + 1, &fdRead, &fdWrite, &fdExcept, &tv);
		if (rc < 0 && lastError() == CORE_EINTR)
		{
			Timestamp end;
			Timespan waited = end - start;
			if (waited < remainingTime)
				remainingTime -= waited;
			else
				remainingTime = 0;
		}
	}
	while (rc < 0 && lastError() == CORE_EINTR);
	
	if (rc < 0) 
	{
		error();
	}
	return rc > 0; 
}

	
Int32 SocketImpl::setSendBufferSize(Int32 size)
{
	return setOption(SOL_SOCKET, SO_SNDBUF, size);
}

	
Int32 SocketImpl::getSendBufferSize()
{
	Int32 result;
	getOption(SOL_SOCKET, SO_SNDBUF, result);
	return result;
}


Int32 SocketImpl::setReceiveBufferSize(Int32 size)
{
	return setOption(SOL_SOCKET, SO_RCVBUF, size);
}

	
Int32 SocketImpl::getReceiveBufferSize()
{
	Int32 result;
	getOption(SOL_SOCKET, SO_RCVBUF, result);
	return result;
}


Int32 SocketImpl::setSendTimeout(const Timespan& timeout)
{
#if defined(_WIN32)
	Int32 value = (Int32) timeout.totalMilliseconds();
	return setOption(SOL_SOCKET, SO_SNDTIMEO, value);
#elif defined(CORE_BROKEN_TIMEOUTS)
	_sndTimeout = timeout;
#else
	return setOption(SOL_SOCKET, SO_SNDTIMEO, timeout);
#endif
}


Timespan SocketImpl::getSendTimeout()
{
	Timespan result;
#if defined(_WIN32)
	Int32 value;
	getOption(SOL_SOCKET, SO_SNDTIMEO, value);
	result = Timespan::TimeDiff(value)*1000;
#elif defined(CORE_BROKEN_TIMEOUTS)
	result = _sndTimeout;
#else
	getOption(SOL_SOCKET, SO_SNDTIMEO, result);
#endif
	return result;
}


Int32 SocketImpl::setReceiveTimeout(const Timespan& timeout)
{
	int ret = -1;
#ifndef CORE_BROKEN_TIMEOUTS
#if defined(_WIN32)
	Int32 value = (Int32) timeout.totalMilliseconds();
	ret = setOption(SOL_SOCKET, SO_RCVTIMEO, value);
#else
  ret = setOption(SOL_SOCKET, SO_RCVTIMEO, timeout);
#endif
#else
	_recvTimeout = timeout;
#endif

	return ret;
}


Timespan SocketImpl::getReceiveTimeout()
{
	Timespan result;
#if defined(_WIN32)
	Int32 value;
	getOption(SOL_SOCKET, SO_RCVTIMEO, value);
	result = Timespan::TimeDiff(value)*1000;
#elif defined(CORE_BROKEN_TIMEOUTS)
	result = _recvTimeout;
#else
	getOption(SOL_SOCKET, SO_RCVTIMEO, result);
#endif
	return result;
}

	
SocketAddress SocketImpl::address()
{
	char buffer[SocketAddress::MAX_ADDRESS_LENGTH];
	struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
	core_socklen_t saLen = sizeof(buffer);
	Int32 rc = ::getsockname(_sockfd, pSA, &saLen);
	if (rc == 0)
	{
		return SocketAddress(pSA, saLen);
	}
	else 
	{
		error();
	}
	return SocketAddress();
}

	
SocketAddress SocketImpl::peerAddress()
{
	char buffer[SocketAddress::MAX_ADDRESS_LENGTH];
	struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
	core_socklen_t saLen = sizeof(buffer);
	Int32 rc = ::getpeername(_sockfd, pSA, &saLen);
	if (rc == 0)
	{
		return SocketAddress(pSA, saLen);
	}
	else 
	{
		error();
	}
	return SocketAddress();
}


Int32 SocketImpl::setOption(Int32 level, Int32 option, Int32 value)
{
	return setRawOption(level, option, &value, sizeof(value));
}


Int32 SocketImpl::setOption(Int32 level, Int32 option, unsigned value)
{
	return setRawOption(level, option, &value, sizeof(value));
}


Int32 SocketImpl::setOption(Int32 level, Int32 option, unsigned char value)
{
	return setRawOption(level, option, &value, sizeof(value));
}

Int32 SocketImpl::setOption(Int32 level, Int32 option, const char* value,int valueLen)
{
	return setRawOption(level,option,value,valueLen);
}

Int32 SocketImpl::setOption(Int32 level, Int32 option, const Timespan& value)
{
	struct timeval tv;
	tv.tv_sec  = (Int32) value.totalSeconds();
	tv.tv_usec = (Int32) value.useconds();
	
	return setRawOption(level, option, &tv, sizeof(tv));
}

	
Int32 SocketImpl::setRawOption(Int32 level, Int32 option, const void* value, core_socklen_t length)
{
	Int32 rc = ::setsockopt(_sockfd, level, option, reinterpret_cast<const char*>(value), length);
	if (rc == -1) 
	{
		error();
	}
	return rc;
}


Int32 SocketImpl::getOption(Int32 level, Int32 option, Int32& value)
{
	core_socklen_t len = sizeof(value);
	return getRawOption(level, option, &value, len);
}


Int32 SocketImpl::getOption(Int32 level, Int32 option, unsigned& value)
{
	core_socklen_t len = sizeof(value);
	return getRawOption(level, option, &value, len);
}


Int32 SocketImpl::getOption(Int32 level, Int32 option, unsigned char& value)
{
	core_socklen_t len = sizeof(value);
	return getRawOption(level, option, &value, len);
}


int SocketImpl::getOption(Int32 level, Int32 option, Timespan& value)
{
	struct timeval tv;
	core_socklen_t len = sizeof(tv);
	int ret = getRawOption(level, option, &tv, len);
	value.assign(tv.tv_sec, tv.tv_usec);
	return ret;
}



Int32 SocketImpl::getRawOption(Int32 level, Int32 option, void* value, core_socklen_t& length)
{
	Int32 rc = ::getsockopt(_sockfd, level, option, reinterpret_cast<char*>(value), &length);
	if (rc == -1)
	{
		error();
	}
	return rc;
}


void SocketImpl::setBlocking(bool flag)
{
	Int32 arg = flag ? 0 : 1;
	ioctl(FIONBIO, arg);
}


Int32 SocketImpl::socketError()
{
	Int32 result;
	getOption(SOL_SOCKET, SO_ERROR, result);
	return result;
}



void SocketImpl::initSocket(Int32 af, Int32 type, Int32 proto)
{
	_sockfd = ::socket(af, type, proto);
	if (_sockfd == CORE_INVALID_SOCKET)
	{
		error();
	}
}


void SocketImpl::ioctl(Int32 request, Int32& arg)
{
#if defined(_WIN32)
	Int32 rc = ioctlsocket(_sockfd, request, reinterpret_cast<u_long*>(&arg));
#else
	Int32 rc = ::ioctl(_sockfd, request, &arg);
#endif
	if (rc != 0) 
	{
		error();
	}
}


void SocketImpl::ioctl(Int32 request, void* arg)
{
#if defined(_WIN32)
	Int32 rc = ioctlsocket(_sockfd, request, reinterpret_cast<u_long*>(arg));
#else
	Int32 rc = ::ioctl(_sockfd, request, arg);
#endif
	if (rc != 0) 
	{
		error();
	}
}


void SocketImpl::setSockfd(core_socket_t aSocket)
{
	_sockfd = aSocket;
}


void SocketImpl::error()
{
	CData empty = "from error()";
	error(lastError(), empty);
}


void SocketImpl::error(const CData& arg)
{
	error(lastError(), arg);
}


void SocketImpl::error(Int32 code)
{
	CData arg = "from error(Int32 code)";
	error(code, arg);
}


void SocketImpl::error(Int32 code, const CData& arg)
{
	switch (code)
	{
		case CORE_ESYSNOTREADY:
			throw NetException("Net subsystem not ready");
		case CORE_ENOTINIT:
			throw NetException("Net subsystem not initialized");
		case CORE_EINTR:
			throw IOException("Interrupted");
		case CORE_EACCES:
			throw IOException("Permission denied");
		case CORE_EFAULT:
			throw IOException("Bad address");
		case CORE_EINVAL:
			throw InvalidArgumentException();
		case CORE_EMFILE:
			throw IOException("Too many open files");
		case CORE_EWOULDBLOCK:
			throw IOException("Operation would block");
		case CORE_EINPROGRESS:
			throw IOException("Operation now in progress");
		case CORE_EALREADY:
			throw IOException("Operation already in progress");
		case CORE_ENOTSOCK:
			throw IOException("Socket operation attempted on non-socket");
		case CORE_EDESTADDRREQ:
			throw NetException("Destination address required");
		case CORE_EMSGSIZE:
			throw NetException("Message too long");
		case CORE_EPROTOTYPE:
			throw NetException("Wrong protocol type");
		case CORE_ENOPROTOOPT:
			throw NetException("Protocol not available");
		case CORE_EPROTONOSUPPORT:
			throw NetException("Protocol not supported");
		case CORE_ESOCKTNOSUPPORT:
			throw NetException("Socket type not supported");
		case CORE_ENOTSUP:
			throw NetException("Operation not supported");
		case CORE_EPFNOSUPPORT:
			throw NetException("Protocol family not supported");
		case CORE_EAFNOSUPPORT:
			throw NetException("Address family not supported");
		case CORE_EADDRINUSE:
			throw NetException("Address already in use", arg);
		case CORE_EADDRNOTAVAIL:
			throw NetException("Cannot assign requested address", arg);
		case CORE_ENETDOWN:
			throw NetException("Network is down");
		case CORE_ENETUNREACH:
			throw NetException("Network is unreachable");
		case CORE_ENETRESET:
			throw NetException("Network dropped connection on reset");
		case CORE_ECONNABORTED:
			throw ConnectionAbortedException();
		case CORE_ECONNRESET:
			throw ConnectionResetException();
		case CORE_ENOBUFS:
			throw IOException("No buffer space available");
		case CORE_EISCONN:
			throw NetException("Socket is already connected");
		case CORE_ENOTCONN:
			throw NetException("Socket is not connected");
		case CORE_ESHUTDOWN:
			throw NetException("Cannot send after socket shutdown");
		case CORE_ETIMEDOUT:
			throw TimeoutException();
		case CORE_ECONNREFUSED:
			throw ConnectionRefusedException(arg);
		case CORE_EHOSTDOWN:
			throw NetException("Host is down");
		case CORE_EHOSTUNREACH:
			throw NetException("No route to host");
		default:
			throw IOException(arg);
	}
}

} 

