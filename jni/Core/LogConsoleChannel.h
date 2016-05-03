#ifndef LogConsoleChannel_H
#define LogConsoleChannel_H


#include "Core.h"
#include "LogChannel.h"
#include "DatagramSocket.h"
#include "SocketAddress.h"


namespace CORE {


class  LogConsoleChannel : public LogChannel
{
public:
	
	LogConsoleChannel();
	
	virtual ~LogConsoleChannel();
	
public:
	
	virtual const char* name() const;
	
	virtual void open();
		
	virtual void close();
		
	virtual void log(const char *msg, const char *prio);
		
	virtual void setProperty(char* name, char* value);

	virtual CData getProperty(char* name) const;

private:

	DatagramSocket	m_udp;
	SocketAddress	m_remoteAddr;
	bool			m_isSend;
	
};


}


#endif

