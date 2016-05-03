#ifndef SocketNotifier_H
#define SocketNotifier_H


#include "Core.h"
#include "Socket.h"
#include "NetEventHandler.h"
#include <set>


namespace CORE {


class CORE_API SocketNotifier
{
public:

	SocketNotifier();

	~SocketNotifier();

public:
		
	void addEventHandler(NetEventHandler *handler,Int32 mask, char *name);
		
	void removeEventHandler(Int32 mask);
		
	bool accepts(Int32 mask);
		
	Int32 dispatch(Socket& socket, Int32 mask);
		
	bool hasMask() const;

	void suspend();

	Int32 isSuspend();

	void resume();

	void setClear(bool clear);

	bool	isClear();

	void	setClearMask(Int32 mask);

	void clearClearMask(Int32 mask);

	Int32 getClearMask();

	char *getName();

private:

	Int32				m_nMask;
	Int32				m_nSuspend;
	bool					m_isClear;
	Int32				m_clearMask;
	NetEventHandler*		 m_ptrHandler;
	char					m_name[100];
};


} 



#endif


