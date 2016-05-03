#ifndef Allocator_H
#define Allocator_H


#include "Core.h"


#define core_new	  new(__FILE__, __LINE__)
#define core_delete	  delete
#define core_malloc(x)	 CORE::Allocator::getBuf(x,__FILE__, __LINE__)
#define core_free(x)		 CORE::Allocator::retBuf(x)
#define core_mem_tracker	CORE::AllocatorManager::instance().dump


namespace CORE {


class CORE_API Allocator
{
public:

	void* operator new( size_t size);

	void* operator new[]( size_t size);
	
	void* operator new( size_t size, char filename[],int line);

	void* operator new[]( size_t size, char filename[],int line);

	void operator delete( void* p);

	void operator delete[]( void* p);

	void operator delete( void* p, char filename[],int line);

	void operator delete[]( void* p, char filename[],int line);

public:

	static void* getBuf(int len, char filename[],int line);

	static void retBuf(void* p);
};


}


#endif


