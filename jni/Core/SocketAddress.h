#ifndef SocketAddress_H
#define SocketAddress_H


#include "Core.h"
#include "Data.h"
#include <string.h>


namespace CORE {


class CORE_API SocketAddress
{
public:
	SocketAddress();
	
	SocketAddress(const CData& host, UInt16 port);

	SocketAddress(const CData& addr);

	SocketAddress(const struct sockaddr* addr, core_socklen_t length);

	SocketAddress(UInt32 port, DWORD addr);
	
	~SocketAddress();
		
	SocketAddress& operator = (const SocketAddress& addr);

	inline bool operator == (const SocketAddress& socket) const
	{
		return strcmp(toString(),socket.toString())==0;
	}

	char * host() const;
	
	UInt16 port() const;
		
	core_socklen_t length() const;
		
	const struct sockaddr* addr() const;

	char * toString() const;

	Int32 GetNetIPAddr();

	DWORD GetIpAddress();
	
	UInt32 GetPortNumber();
	
	Int32 SetPortNumber(UInt32 port);
	
	Int32 Set(UInt32 port, DWORD addr);

	Int32 Set(sockaddr_in addr);

	static bool check(const char *ip);

	static bool maskCheck(const char *mask);

	static CData getHostName();

	enum
	{
		MAX_ADDRESS_LENGTH = sizeof(struct sockaddr_in)
	};
	
private:
	
	struct sockaddr_in	 m_addr;
	char				 m_ToString[30];
};



} 


#endif 


