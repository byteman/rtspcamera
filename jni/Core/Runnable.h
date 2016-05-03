#ifndef Runnable_H
#define Runnable_H


#include "Core.h"
#include "Runnable.h"

namespace CORE {

/// Class Runnable
/// It provides the run() method for an entry
/// pointer of a thread.
class CORE_API Runnable
{
public:	
	Runnable();
	virtual ~Runnable();

		/// Overridden by subclasses.
	virtual void run() = 0;
};


} 


#endif 

