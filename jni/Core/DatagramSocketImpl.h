#ifndef DatagramSocketImpl_H
#define DatagramSocketImpl_H


#include "Core.h"
#include "SocketImpl.h"


namespace CORE {


class CORE_API DatagramSocketImpl: public SocketImpl
{
public:
	
	DatagramSocketImpl();
	
	DatagramSocketImpl(core_socket_t sockfd);	

	virtual void init();
	
protected:
	~DatagramSocketImpl();
};


} 



#endif 

