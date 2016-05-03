#include "SocketNotifier.h"
#include "SocketReactor.h"


namespace CORE {


SocketNotifier::SocketNotifier():
	m_nMask(0),
	m_clearMask(0),
	m_nSuspend(0),
	m_ptrHandler(0),
	m_isClear(false)
{
}

	
SocketNotifier::~SocketNotifier()
{
}


void SocketNotifier::suspend()
{
	m_nSuspend=1;
}


Int32 SocketNotifier::isSuspend()
{
	return m_nSuspend;
}


void SocketNotifier::resume()
{
	m_nSuspend=0;
}


void SocketNotifier::setClear(bool clear)
{
	m_isClear=clear;
}


bool	SocketNotifier::isClear()
{
	return m_isClear;
}


void	SocketNotifier::setClearMask(Int32 mask)
{
	m_clearMask|=mask;
}


void SocketNotifier::clearClearMask(Int32 mask)
{
	m_clearMask&=~mask;
}


Int32 SocketNotifier::getClearMask()
{
	return m_clearMask;
}


char *SocketNotifier::getName()
{
	return m_name;
}

void SocketNotifier::addEventHandler(NetEventHandler *handler,Int32 mask, char *name)
{
	clearClearMask(mask);
	m_ptrHandler=handler;
	m_nMask|=mask;
	memset(m_name, 0 , 100);
	memcpy(m_name, name, strlen(name));
}

	
void SocketNotifier::removeEventHandler(Int32 mask)
{
	m_nMask&=(~mask);
	if(NetEventHandler::NULL_MASK==m_nMask)
	{
		m_ptrHandler=0;
	}
}


bool SocketNotifier::hasMask() const
{
	return ((m_nMask&NetEventHandler::READ_MASK)|(m_nMask&NetEventHandler::WRITE_MASK)|(m_nMask&NetEventHandler::EXCEPT_MASK))!=0;
}



bool SocketNotifier::accepts(Int32 mask)
{
	return ((m_nMask&mask)==mask);

}


Int32 SocketNotifier::dispatch(Socket& socket, Int32 mask)
{
	if(0==m_ptrHandler)
	{
		return 0;
	}
	
	Int32 ret=0;
	switch(mask)
	{
		case NetEventHandler::READ_MASK:
			ret=m_ptrHandler->handle_input(socket);
			break;
		case NetEventHandler::WRITE_MASK:
			ret=m_ptrHandler->handle_output(socket);
			break;
		case NetEventHandler::EXCEPT_MASK:
			ret=m_ptrHandler->handle_exception(socket);
			break;
		case NetEventHandler::CLOSE_MASK:
			ret=m_ptrHandler->handle_close(socket);
			break;
		default:
			ret=0;
	}
	
	return ret;
}


} 


