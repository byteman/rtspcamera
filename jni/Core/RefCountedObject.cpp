#include "RefCountedObject.h"


namespace CORE {


RefCountedObject::RefCountedObject(): _rc(1)
{
}


RefCountedObject::~RefCountedObject()
{
}


void RefCountedObject::duplicate() const
{
	_rcCore_Mutex.lock();
	++_rc;
	_rcCore_Mutex.unlock();
}


void RefCountedObject::release() const
{
	_rcCore_Mutex.lock();
	Int32 rc = --_rc;
	_rcCore_Mutex.unlock();
	if (rc == 0) 
	{
		delete this;
	}
}


} 

