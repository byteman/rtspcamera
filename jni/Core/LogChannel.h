#ifndef LogChannel_H
#define LogChannel_H


#include "Data.h"
#include "Core_Mutex.h"


namespace CORE {


class LogChannel
{
public:
	
	virtual const char* name() const = 0;
	
	virtual void open() = 0;
		
	virtual void close() = 0;
		
	virtual void log(const char *msg, const char *prio) = 0;
		
	virtual void setProperty(char* name, char* value) = 0;

	virtual CData getProperty(char* name) const = 0;
		
};


} 


#endif 

