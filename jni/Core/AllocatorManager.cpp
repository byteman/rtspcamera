#include "AllocatorManager.h"


#include <malloc.h>


#include <string.h>
#include <sstream>
#include "SingletonHolder.h"
#include "Logger.h"


namespace CORE {


AllocatorManager::AllocatorManager()
{


}


AllocatorManager::~AllocatorManager()
{

}


void* AllocatorManager::donew(unsigned int len, char filename[],int line)
{	
	void *p;
	p = malloc(len);
	
	MemoryInfo meminf;
	meminf.ptr = p;
	meminf.len=len;
	meminf.lineno = line;
	strcpy(meminf.filename,filename);

	{
		Core_FastMutex::ScopedLock lock(m_mutex);
		
		MemMap::iterator it = m_memMap.find(p);
		if (it == m_memMap.end())
		{
			m_memMap[p] = meminf;
		}
	}

	return p;
}


void AllocatorManager::dodelete(void *p)
{
	{
		Core_FastMutex::ScopedLock lock(m_mutex);
	
		MemMap::iterator it = m_memMap.find(p);
		if (it != m_memMap.end())
		{
			m_memMap.erase(it);
		}
	}

	free(p);
	p=0;
}


void AllocatorManager::dump()
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	MemoryInfo info;
	int count=0;
	int len=0;
	std::ostringstream str_info;
	
	MemMap::iterator it;
	
	for(it=m_memMap.begin();it!=m_memMap.end();++it)
	{
		info=it->second;
		count++;
		len+=info.len;
#if 1
		str_info<<"["<<info.filename
			<<" line:"<<info.lineno
			<<"] [len:"<<info.len
			<<"] \n";
#endif
	}
	
	if (count == 0)
	{
		str_info<<"没有动态内存分配";
	}
	else
	{
		str_info<<"内存分配:"<<count<<"	  动态内存总数:"<<len<<"字节\n";
	}

	_LogInformation<<"\n\n@@@@@@@@@@@@@@@@@@@ 程序动态内存跟踪 @@@@@@@@@@@@@@@@@@@\n"
		<<str_info.str().c_str()<<_LogInformationEnd;
	str_info.str("");
}


void AllocatorManager::dump(char *buf, int *bufLen)
{
	Core_FastMutex::ScopedLock lock(m_mutex);
	
	MemoryInfo info;
	int count=0;
	int len=0;
	std::ostringstream str_info;
	
	MemMap::iterator it;
	
	for(it=m_memMap.begin();it!=m_memMap.end();++it)
	{
		info=it->second;
		count++;
		len+=info.len;
		str_info<<"["<<info.filename
			<<" line:"<<info.lineno
			<<"] [len:"<<info.len
			<<"] \n";
	}
	
	if (count == 0)
	{
		str_info<<"No memory leaks found";
	}
	else
	{
		str_info<<"Leaks found:"<<count<<" all len:"<<len;
	}

	if(*bufLen<strlen(str_info.str().c_str()))
	{
		*bufLen=0;
		return;
	}
	
	*bufLen=strlen(str_info.str().c_str());
	memcpy(buf, str_info.str().c_str(), *bufLen);
	buf[*bufLen]=0;
	str_info.str("");
}


AllocatorManager& AllocatorManager::instance()
{
	static SingletonHolder<AllocatorManager> sh;
	return *sh.get();
}


}

