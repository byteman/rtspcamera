#include "Socket.h"
#include "StreamSocketImpl.h"
#include "Timestamp.h"
#include <string>
#include "Logger.h"


namespace CORE {


Socket::Socket():
	_pImpl(0)
{
}


Socket::Socket(SocketImpl* pImpl):
	_pImpl(pImpl)
{
	
}


Socket::Socket(const Socket& socket):
	_pImpl(socket._pImpl)
{
	_pImpl->duplicate();
}

	
Socket& Socket::operator = (const Socket& socket)
{
	if (&socket != this)
	{
		if (_pImpl) 
		{
			_pImpl->release();
		}
		
		_pImpl = socket._pImpl;
		
		if (_pImpl)
		{
			_pImpl->duplicate();
		}
	}
	return *this;
}


Socket::~Socket()
{
	_pImpl->release();
}


Int32 Socket::select(SocketList& readList, SocketList& writeList, SocketList& exceptList, const Timespan& timeout)
{
	fd_set fdRead;
	fd_set fdWrite;
	fd_set fdExcept;
	Int32 nfd = 0;
	
	FD_ZERO(&fdRead);
	SocketList::const_iterator it;
	for (it = readList.begin(); it != readList.end(); ++it)
	{
		if (Int32(it->sockfd()) > nfd)
		{
			nfd = Int32(it->sockfd());
		}
		FD_SET(it->sockfd(), &fdRead);
	}
	
	FD_ZERO(&fdWrite);
	for ( it = writeList.begin(); it != writeList.end(); ++it)
	{
		if (Int32(it->sockfd()) > nfd)
		{
			nfd = Int32(it->sockfd());
		}
		FD_SET(it->sockfd(), &fdWrite);
	}
	
	FD_ZERO(&fdExcept);
	for ( it = exceptList.begin(); it != exceptList.end(); ++it)
	{
		if (Int32(it->sockfd()) > nfd)
		{
			nfd = Int32(it->sockfd());
		}
		FD_SET(it->sockfd(), &fdExcept);
	}
	
	Timespan remainingTime(timeout);
	Int32 rc;

	try
	{
		do
		{
			struct timeval tv;
			tv.tv_sec  = (Int32) remainingTime.totalSeconds();
			tv.tv_usec = (Int32) remainingTime.useconds();
			Timestamp start;
		
			rc = ::select(nfd + 1, &fdRead, &fdWrite, &fdExcept, &tv);
			
			if (rc < 0 && SocketImpl::lastError() == CORE_EINTR)
			{
				Timestamp end;
				Timespan waited = end - start;
				if (waited < remainingTime)
				{
					remainingTime -= waited;
				}
				else
				{
					remainingTime = 0;
				}
			}
		}
		while (rc < 0 && SocketImpl::lastError() == CORE_EINTR);
	}
	catch(...)
	{
		_LogError<<"select error"<<_LogErrorEnd;
	}
	
	SocketList readyReadList;
	for ( it = readList.begin(); it != readList.end(); ++it)
	{
		if (FD_ISSET(it->sockfd(), &fdRead))
		{
			readyReadList.push_back(*it);
		}
	}
	std::swap(readList, readyReadList);
	
	SocketList readyWriteList;
	for ( it = writeList.begin(); it != writeList.end(); ++it)
	{
		if (FD_ISSET(it->sockfd(), &fdWrite))
		{
			readyWriteList.push_back(*it);
		}
	}
	std::swap(writeList, readyWriteList);
	
	SocketList readyExceptList;
	for ( it = exceptList.begin(); it != exceptList.end(); ++it)
	{
		if (FD_ISSET(it->sockfd(), &fdExcept))
		{
			readyExceptList.push_back(*it);
		}
	}
	std::swap(exceptList, readyExceptList);	
	
	return rc; 
}


} 

