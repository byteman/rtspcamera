#ifndef Exception_H
#define Exception_H

#if defined(CORE_OS_FAMILY_WINDOWS)
#pragma warning(disable: 4541)
#endif 


#include "Core.h"
#include "ExceptionBase.h"

namespace CORE {

//
// All Exception class declare
//
#define CORE_DECLARE_EXCEPTION(CLS, BASE)					\
	class CORE_API CLS: public BASE							\
	{													\
	public:												\
		CLS();											\
		CLS(const CData& msg);							\
		CLS(const CData& msg, const CData& arg);		\
		~CLS() throw();									\
		CLS& operator = (const CLS& exc);						\
		const char* name() const throw();						\
		const char* className() const throw();					\
		Exception* clone() const;								\
		void rethrow() const;								\
	};


//
// Standard exception classes
//
CORE_DECLARE_EXCEPTION(LogicException, Exception)
CORE_DECLARE_EXCEPTION(AssertionViolationException, Exception)
CORE_DECLARE_EXCEPTION(NullPointerException, Exception)
CORE_DECLARE_EXCEPTION(BugcheckException, Exception)
CORE_DECLARE_EXCEPTION(InvalidArgumentException, Exception)
CORE_DECLARE_EXCEPTION(NotImplementedException, Exception)
CORE_DECLARE_EXCEPTION(RangeException, Exception)
CORE_DECLARE_EXCEPTION(IllegalStateException, Exception)
CORE_DECLARE_EXCEPTION(InvalidAccessException, Exception)
CORE_DECLARE_EXCEPTION(SignalException, Exception)
CORE_DECLARE_EXCEPTION(UnhandledException, Exception)
CORE_DECLARE_EXCEPTION(RuntimeException, Exception)
CORE_DECLARE_EXCEPTION(NotFoundException, Exception)
CORE_DECLARE_EXCEPTION(ExistsException, Exception)
CORE_DECLARE_EXCEPTION(TimeoutException, Exception)
CORE_DECLARE_EXCEPTION(SystemException, Exception)
CORE_DECLARE_EXCEPTION(RegularExpressionException, Exception)
CORE_DECLARE_EXCEPTION(LibraryLoadException, Exception)
CORE_DECLARE_EXCEPTION(LibraryAlreadyLoadedException, Exception)
CORE_DECLARE_EXCEPTION(NoCore_ThreadAvailableException, Exception)
CORE_DECLARE_EXCEPTION(PropertyNotSupportedException, Exception)
CORE_DECLARE_EXCEPTION(PoolOverflowException, Exception)
CORE_DECLARE_EXCEPTION(NoPermissionException, Exception)
CORE_DECLARE_EXCEPTION(OutOfMemoryException, Exception)
CORE_DECLARE_EXCEPTION(DataException, Exception)
CORE_DECLARE_EXCEPTION(DataFormatException, Exception)
CORE_DECLARE_EXCEPTION(SyntaxException, Exception)
CORE_DECLARE_EXCEPTION(CircularReferenceException, Exception)
CORE_DECLARE_EXCEPTION(PathSyntaxException, Exception)
CORE_DECLARE_EXCEPTION(IOException, Exception)
CORE_DECLARE_EXCEPTION(FileException, Exception)
CORE_DECLARE_EXCEPTION(FileExistsException, Exception)
CORE_DECLARE_EXCEPTION(FileNotFoundException, Exception)
CORE_DECLARE_EXCEPTION(PathNotFoundException, Exception)
CORE_DECLARE_EXCEPTION(FileReadOnlyException, Exception)
CORE_DECLARE_EXCEPTION(FileAccessDeniedException, Exception)
CORE_DECLARE_EXCEPTION(CreateFileException, Exception)
CORE_DECLARE_EXCEPTION(OpenFileException, Exception)
CORE_DECLARE_EXCEPTION(WriteFileException, Exception)
CORE_DECLARE_EXCEPTION(ReadFileException, Exception)
CORE_DECLARE_EXCEPTION(UnknownURISchemeException, Exception)
CORE_DECLARE_EXCEPTION(ApplicationException, Exception)
CORE_DECLARE_EXCEPTION(BadCastException, Exception)
//
// Net
//
CORE_DECLARE_EXCEPTION(NetException, Exception)
CORE_DECLARE_EXCEPTION(InvalidAddressException, Exception)
CORE_DECLARE_EXCEPTION(ServiceNotFoundException, Exception)
CORE_DECLARE_EXCEPTION(ConnectionAbortedException, Exception)
CORE_DECLARE_EXCEPTION(ConnectionResetException, Exception)
CORE_DECLARE_EXCEPTION(ConnectionRefusedException, Exception)

}

#endif 

