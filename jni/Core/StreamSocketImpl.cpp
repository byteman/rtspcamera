#include "StreamSocketImpl.h"


namespace CORE {


StreamSocketImpl::StreamSocketImpl()
{
	initSocket(AF_INET, SOCK_STREAM);
}


StreamSocketImpl::StreamSocketImpl(core_socket_t sockfd): SocketImpl(sockfd)
{

}


StreamSocketImpl::~StreamSocketImpl()
{

}


void StreamSocketImpl::init()
{
	initSocket(AF_INET, SOCK_STREAM);
}


} 

