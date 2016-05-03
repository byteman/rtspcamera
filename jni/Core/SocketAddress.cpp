#include "SocketAddress.h"
#include "RefCountedObject.h"
#include <cstring>



namespace CORE {

//
// SocketAddress
//
SocketAddress::SocketAddress()
{
	memset(&m_addr, 0, sizeof(m_addr));	
}

	
SocketAddress::SocketAddress(const CData& addr, UInt16 port)
{
	memset(&m_addr, 0, sizeof(m_addr));

	struct sockaddr_in ia;
	memset(&ia, 0, sizeof(ia));
	ia.sin_addr.s_addr= inet_addr(addr.c_str());
	if (ia.sin_addr.s_addr== INADDR_NONE && addr != "255.255.255.255")
	{
		return ;
	}
	else
	{
		memcpy(&m_addr, &ia, sizeof(m_addr));
	}

	m_addr.sin_family	 = AF_INET;
	m_addr.sin_port	 = htons(port);
}

SocketAddress::SocketAddress(UInt32 port, DWORD addr)
{
	Set(port, addr);
}

SocketAddress::SocketAddress(const struct sockaddr* addr, core_socklen_t length)
{
	if (length == sizeof(struct sockaddr_in))
	{
		memcpy(&m_addr, reinterpret_cast<const struct sockaddr_in*>(addr), sizeof(m_addr));
	}
}


SocketAddress::SocketAddress(const CData& addr)
{
	CData biao(":");
	CData host;
	CData port;
	Int32 pos;
	if(strstr(addr.c_str(), ":")==0)
	{
		return;
	}
	
	pos=(Int32)addr.find(biao);
	host=addr.substr(0,pos);
	port=addr.substr(pos+1,addr.size() - host.size() - 1);

	//
	memset(&m_addr, 0, sizeof(m_addr));

	struct sockaddr_in ia;
	memset(&ia, 0, sizeof(ia));
	ia.sin_addr.s_addr= inet_addr(host.c_str());
	if (ia.sin_addr.s_addr== INADDR_NONE && host != "255.255.255.255")
	{
		return ;
	}
	else
	{
		memcpy(&m_addr, &ia, sizeof(m_addr));
	}

	m_addr.sin_family	 = AF_INET;
	m_addr.sin_port	 = htons((Int32)atoi(port.c_str()));
	
}


SocketAddress::~SocketAddress()
{
	
}

	
SocketAddress& SocketAddress::operator = (const SocketAddress& addr)
{
	this->m_addr=addr.m_addr;
	return *this;
}


UInt16 SocketAddress::port() const
{
	return ntohs(m_addr.sin_port );
}

UInt32 SocketAddress::GetPortNumber()
{
	return port();
}

Int32 SocketAddress::SetPortNumber(UInt32 port)
{
	m_addr.sin_port = htons(port);
	return 0;
}


static bool isNum(char *str)
{
	int len;
	len=strlen(str);
	if(len>3)
	{
		return false;
	}

	int i;
	for(i=0;i<len;i++)
	{
		if(str[i]>'9'||str[i]<'0')
		{
			return false;
		}
	}

	return true;
}


static bool isMaskNum(char *str)
{
	int len;
	len=strlen(str);
	if(len>3)
	{
		return false;
	}

	int i;
	for(i=0;i<len;i++)
	{
		if(str[i]!='0'&&str[i]!='2'&&str[i]!='5')
		{
			return false;
		}
	}

	return true;
}


bool SocketAddress::check(const char *ip)
{
	if(0==ip||strlen(ip)>15)
	{
		return false;
	}

	char *ptr[5];
	int i=0;
	char buf[16];
	char *str;
	memset(buf, 0, 16);
	memcpy(buf, ip, strlen(ip));
	memset(ptr, 0, sizeof(char*)*5);
	str=buf;
	while((ptr[i]=strtok(str,"."))!=0) 
	{
		i++;
		str=0;
		if(i>4)
		{
			return false;
		}
	}

	if(i!=4)
	{
		return false;
	}

	for(i=0;i<4;i++)
	{
		if(isNum(ptr[i])==false)
		{
			return false;
		}
	}
	
	return true;
}


bool SocketAddress::maskCheck(const char *mask)
{
	if(0==mask||strlen(mask)>15)
	{
		return false;
	}

	char *ptr[5];
	int i=0;
	char buf[16];
	char *str;
	memset(buf, 0, 16);
	memcpy(buf, mask, strlen(mask));
	memset(ptr, 0, sizeof(char*)*5);
	str=buf;
	while((ptr[i]=strtok(str,"."))!=0) 
	{
		i++;
		str=0;
		if(i>4)
		{
			return false;
		}
	}

	if(i!=4)
	{
		return false;
	}

	for(i=0;i<4;i++)
	{
		if(isMaskNum(ptr[i])==false)
		{
			return false;
		}
	}

	int value;
	for(i=0;i<4;i++)
	{
		value=atoi(ptr[i]);
		if(value!=255&&value!=0)
		{
			return false;
		}
	}
	
	return true;
}


CData SocketAddress::getHostName()
{
	CData addr="";
	
	//通过本机主机名. 
	char host_name[256]; 
	gethostname(host_name,sizeof(host_name)); 
	
	struct hostent *hp; 
	struct in_addr sa; 
	char *buf; 
	
	hp = gethostbyname(host_name); 
	
	if (hp != NULL) 
	{ 
		//循环获取本地主机名 
		for (int i = 0; hp->h_addr_list[i]; i++) 
		{ 
			memcpy (&sa, hp->h_addr_list[i],hp->h_length); 
			buf = inet_ntoa(sa); 
			addr=buf;
		} 
	} 
	
	return addr;
}

Int32 SocketAddress::Set(UInt32 port, DWORD addr)
{
	memset(&m_addr, 0, sizeof(m_addr));

	struct sockaddr_in ia;
	memset(&ia, 0, sizeof(ia));
	ia.sin_addr.s_addr= addr;
	if (ia.sin_addr.s_addr== INADDR_NONE)// && addr != "255.255.255.255")
	{
		return -1;
	}
	else
	{
		memcpy(&m_addr, &ia, sizeof(m_addr));
	}

	m_addr.sin_family	 = AF_INET;
	m_addr.sin_port	 = htons(port);
	return 0;
}

Int32 SocketAddress::GetNetIPAddr()
{
	return htonl(m_addr.sin_addr.s_addr);
}

DWORD SocketAddress::GetIpAddress()
{
	return m_addr.sin_addr.s_addr;
}

char * SocketAddress::host() const
{
	char * host=inet_ntoa(m_addr.sin_addr);
	return host;
}


core_socklen_t SocketAddress::length() const
{
	return sizeof(m_addr);
}

	
const struct sockaddr* SocketAddress::addr() const
{
	return reinterpret_cast<const struct sockaddr*>(&m_addr);
}


char * SocketAddress::toString() const
{
	char buf[10];
	memset((char *)m_ToString,0x00,sizeof(m_ToString));
	memcpy((char *)m_ToString,host(),strlen(host()));
	memcpy((char *)(m_ToString+strlen(m_ToString)),":",sizeof(":"));

	sprintf(buf, "%d", port());
	memcpy((char *)(m_ToString+strlen(m_ToString)),buf,strlen(buf));
	return (char *)m_ToString;
}

Int32 SocketAddress::Set(sockaddr_in addr)
{
	m_addr = addr;
	return 0;
}


}

