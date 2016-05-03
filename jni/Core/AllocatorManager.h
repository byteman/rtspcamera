#ifndef AllocatorManager_H
#define AllocatorManager_H


#include "Core.h"
#include <map>
#include "Core_Mutex.h"


namespace CORE {


typedef struct _MemoryInfo
{
	void *ptr;
	unsigned int len;
	unsigned int lineno;
	char filename[256];
	
}MemoryInfo;


class CORE_API AllocatorManager
{
public:

	AllocatorManager();

	virtual ~AllocatorManager();

public:

	void* donew(unsigned int len, char filename[],int line);

	void dodelete(void *p);

	void dump();

	void dump(char *buf, int *bufLen);

	static AllocatorManager& instance(); 

public:

	typedef std::map<void*,MemoryInfo>MemMap;
	MemMap	m_memMap;

	Core_FastMutex m_mutex;
};


}


#endif


