#ifndef NetEventHandler_H
#define NetEventHandler_H


#include "Core.h"
#include "Socket.h"


namespace CORE {


class CORE_API NetEventHandler
{
public:
	enum Core_EventMask
	{
		NULL_MASK		=	0,
		READ_MASK		=	(1<<1),
				WRITE_MASK		=	(1<<2),
				EXCEPT_MASK	=	(1<<3),
				ACCEPT_MASK	 =	READ_MASK,
				CONNECT_MASK	=	WRITE_MASK,
				CLOSE_MASK		=	(1<<6),
				CLRALL_MASK	=	(0xffff)
	};
public:
	NetEventHandler();

	virtual ~NetEventHandler();
		
public:
	
	virtual Int32 handle_input(Socket &sock);
	
	virtual Int32 handle_output(Socket &sock);
	
	virtual Int32 handle_exception(Socket &sock);
	
	virtual Int32 handle_timeout(Socket &sock);

	virtual Int32 handle_close(Socket &sock);

};


}


#endif 

