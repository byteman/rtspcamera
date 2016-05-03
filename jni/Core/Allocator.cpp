#include "Allocator.h"
#include "AllocatorManager.h"


namespace CORE {


void* Allocator::operator new( size_t size, char filename[],int line)
{
	return AllocatorManager::instance().donew(size, filename, line);
}


void* Allocator::operator new[]( size_t size, char filename[],int line)
{			
	return AllocatorManager::instance().donew(size, filename, line);	
}


void* Allocator::operator new( size_t size)
{
	return AllocatorManager::instance().donew(size, 0, 0);
}


void* Allocator::operator new[]( size_t size)
{			
	return AllocatorManager::instance().donew(size, 0, 0);	
}


void Allocator::operator delete( void* p)
{
	AllocatorManager::instance().dodelete(p);
}


void Allocator::operator delete[]( void* p)
{
	AllocatorManager::instance().dodelete(p);		
}


void Allocator::operator delete( void* p, char filename[],int line)
{
	AllocatorManager::instance().dodelete(p);
}


void Allocator::operator delete[]( void* p, char filename[],int line)
{
	AllocatorManager::instance().dodelete(p);		
}


void* Allocator::getBuf(int len, char filename[],int line)
{
	return AllocatorManager::instance().donew(len, filename, line);
}


void Allocator::retBuf(void* p)
{
	AllocatorManager::instance().dodelete(p);
}


}


