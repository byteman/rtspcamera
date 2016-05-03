#include "DatagramSocketImpl.h"

namespace CORE {


DatagramSocketImpl::DatagramSocketImpl()
{
	initSocket(AF_INET, SOCK_DGRAM);
}


DatagramSocketImpl::DatagramSocketImpl(core_socket_t sockfd):
	SocketImpl(sockfd)
{

}


DatagramSocketImpl::~DatagramSocketImpl()
{

}


void DatagramSocketImpl::init()
{
	initSocket(AF_INET, SOCK_DGRAM);
}


} 

