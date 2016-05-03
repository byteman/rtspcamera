#ifndef Types_INCLUDED
#define Types_INCLUDED


#include "Core.h"
#include <math.h>

namespace CORE {


#if defined(_MSC_VER)
	typedef signed char				Int8;
	typedef unsigned char			  UInt8;
	typedef signed short			   Int16;
	typedef unsigned short			 UInt16;
	typedef signed int				 Int32;
	typedef unsigned int			   UInt32;
	typedef signed __int64			 Int64;
	typedef unsigned __int64	   UInt64;
	typedef unsigned long			DWORD;
	typedef unsigned short			WORD; 
	
#elif defined(__GNUC__)
	//
	// Unix/GCC
	//
	typedef signed char			Int8;
	typedef unsigned char		  UInt8;
	typedef signed short		   Int16;
	typedef unsigned short		 UInt16;
	typedef signed int			 Int32;
	typedef unsigned int		   UInt32;
	typedef signed long			IntPtr;
	typedef unsigned long		  UIntPtr;
	typedef signed long long   Int64;
	typedef unsigned long long UInt64;
	typedef unsigned long			DWORD;
	typedef unsigned short			WORD;
#endif

#if defined(CORE_OS_FAMILY_WINDOWS)
#define CORE_INVALID_SOCKET  INVALID_SOCKET
#define core_socket_t		SOCKET
#define core_socklen_t	   int
#define core_closesocket(s)  closesocket(s)
#define CORE_EINTR		   WSAEINTR
#define CORE_EACCES		  WSAEACCES
#define CORE_EFAULT		  WSAEFAULT
#define CORE_EINVAL		  WSAEINVAL
#define CORE_EMFILE		  WSAEMFILE
#define CORE_EAGAIN		  WSAEWOULDBLOCK
#define CORE_EWOULDBLOCK	 WSAEWOULDBLOCK
#define CORE_EINPROGRESS	 WSAEINPROGRESS
#define CORE_EALREADY		WSAEALREADY
#define CORE_ENOTSOCK		WSAENOTSOCK
#define CORE_EDESTADDRREQ	WSAEDESTADDRREQ
#define CORE_EMSGSIZE		WSAEMSGSIZE
#define CORE_EPROTOTYPE	  WSAEPROTOTYPE
#define CORE_ENOPROTOOPT	 WSAENOPROTOOPT
#define CORE_EPROTONOSUPPORT WSAEPROTONOSUPPORT
#define CORE_ESOCKTNOSUPPORT WSAESOCKTNOSUPPORT
#define CORE_ENOTSUP		 WSAEOPNOTSUPP
#define CORE_EPFNOSUPPORT	WSAEPFNOSUPPORT
#define CORE_EAFNOSUPPORT	WSAEAFNOSUPPORT
#define CORE_EADDRINUSE	  WSAEADDRINUSE
#define CORE_EADDRNOTAVAIL   WSAEADDRNOTAVAIL
#define CORE_ENETDOWN		WSAENETDOWN
#define CORE_ENETUNREACH	 WSAENETUNREACH
#define CORE_ENETRESET	   WSAENETRESET
#define CORE_ECONNABORTED	WSAECONNABORTED
#define CORE_ECONNRESET	  WSAECONNRESET
#define CORE_ENOBUFS		 WSAENOBUFS
#define CORE_EISCONN		 WSAEISCONN
#define CORE_ENOTCONN		WSAENOTCONN
#define CORE_ESHUTDOWN	   WSAESHUTDOWN
#define CORE_ETIMEDOUT	   WSAETIMEDOUT
#define CORE_ECONNREFUSED	WSAECONNREFUSED
#define CORE_EHOSTDOWN	   WSAEHOSTDOWN
#define CORE_EHOSTUNREACH	WSAEHOSTUNREACH
#define CORE_ESYSNOTREADY	WSASYSNOTREADY
#define CORE_ENOTINIT		WSANOTINITIALISED
#define CORE_HOST_NOT_FOUND  WSAHOST_NOT_FOUND
#define CORE_TRY_AGAIN	   WSATRY_AGAIN
#define CORE_NO_RECOVERY	 WSANO_RECOVERY
#define CORE_NO_DATA		 WSANO_DATA
#elif defined(CORE_OS_FAMILY_UNIX)
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <net/if.h>
#define CORE_INVALID_SOCKET  -1
#define core_socket_t		int
#define core_socklen_t	   socklen_t
#define core_closesocket(s)  ::close(s)
#define CORE_EINTR		   EINTR
#define CORE_EACCES		  EACCES
#define CORE_EFAULT		  EFAULT
#define CORE_EINVAL		  EINVAL
#define CORE_EMFILE		  EMFILE
#define CORE_EAGAIN		  EAGAIN
#define CORE_EWOULDBLOCK	 EWOULDBLOCK
#define CORE_EINPROGRESS	 EINPROGRESS
#define CORE_EALREADY		EALREADY
#define CORE_ENOTSOCK		ENOTSOCK
#define CORE_EDESTADDRREQ	EDESTADDRREQ
#define CORE_EMSGSIZE		EMSGSIZE
#define CORE_EPROTOTYPE	  EPROTOTYPE
#define CORE_ENOPROTOOPT	 ENOPROTOOPT
#define CORE_EPROTONOSUPPORT EPROTONOSUPPORT
#define CORE_ESOCKTNOSUPPORT -1
#define CORE_ENOTSUP		 ENOTSUP
#define CORE_EPFNOSUPPORT	EPFNOSUPPORT
#define CORE_EAFNOSUPPORT	EAFNOSUPPORT
#define CORE_EADDRINUSE	  EADDRINUSE
#define CORE_EADDRNOTAVAIL   EADDRNOTAVAIL
#define CORE_ENETDOWN		ENETDOWN
#define CORE_ENETUNREACH	 ENETUNREACH
#define CORE_ENETRESET	   ENETRESET
#define CORE_ECONNABORTED	ECONNABORTED
#define CORE_ECONNRESET	  ECONNRESET
#define CORE_ENOBUFS		 ENOBUFS
#define CORE_EISCONN		 EISCONN
#define CORE_ENOTCONN		ENOTCONN
#define CORE_ESHUTDOWN	   -2
#define CORE_ETIMEDOUT	   ETIMEDOUT
#define CORE_ECONNREFUSED	ECONNREFUSED
#define CORE_EHOSTDOWN	   -3
#define CORE_EHOSTUNREACH	EHOSTUNREACH
#define CORE_ESYSNOTREADY	-4
#define CORE_ENOTINIT		-5
#define CORE_HOST_NOT_FOUND  HOST_NOT_FOUND
#define CORE_TRY_AGAIN	   TRY_AGAIN
#define CORE_NO_RECOVERY	 NO_RECOVERY
#define CORE_NO_DATA		 NO_DATA
#endif

#ifndef INADDR_NONE
#define INADDR_NONE 0xFFFFFFFF
#endif


} 




#endif 

