#ifndef TimeEventHandler_H
#define TimeEventHandler_H


#include "Core.h"
#include "Data.h"

namespace CORE  {


class CORE_API TimeEventHandler
{
public:
	
	virtual Int32 handle_time(Int32 id) = 0;

};


}


#endif


