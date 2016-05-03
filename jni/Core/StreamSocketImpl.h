#ifndef StreamSocketImpl_H
#define StreamSocketImpl_H


#include "Core.h"
#include "SocketImpl.h"


namespace CORE {


class CORE_API StreamSocketImpl: public SocketImpl
{
public:
	StreamSocketImpl();

	StreamSocketImpl(core_socket_t sockfd);

	virtual void init();
	
protected:
	virtual ~StreamSocketImpl();
};


}


#endif

