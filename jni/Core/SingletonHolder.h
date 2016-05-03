#ifndef SingletonHolder_H
#define SingletonHolder_H


#include "Core.h"
#include "Core_Mutex.h"


namespace CORE {


template <class S>
class SingletonHolder
{
public:
	
	SingletonHolder()
	{
		_pS = 0;
	}
	
	~SingletonHolder()
	{
		delete _pS;
	}
	
	S* get()
	{
		Core_FastMutex::ScopedLock lock(_m);
		if (!_pS) 
		{
			_pS = new S;
		}
		return _pS;
	}
	
private:
	
	S* _pS;
	Core_FastMutex _m;
};


} 


#endif 

