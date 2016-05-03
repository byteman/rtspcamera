#include "Exception.h"
#include <typeinfo>


namespace CORE {

#define CORE_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)									\
	CLS::CLS()														 \
	{																 \
	}																 \
	CLS::CLS(const CData& msg): BASE(msg)							 \
	{																 \
	}																 \
	CLS::CLS(const CData& msg, const CData& arg): BASE(msg, arg)		  \
	{																 \
	}																 \
	CLS::~CLS() throw()												 \
	{																 \
	}																 \
	CLS& CLS::operator = (const CLS& exc)									 \
	{																 \
		BASE::operator = (exc);											 \
		return *this;													 \
	}																 \
	const char* CLS::name() const throw()									 \
	{																 \
		return NAME;													 \
	}																 \
	const char* CLS::className() const throw()								 \
	{																 \
		return typeid(*this).name();										 \
	}																 \
	Exception* CLS::clone() const											 \
	{																 \
		return new CLS(*this);											 \
	}																 \
	void CLS::rethrow() const											 \
	{																 \
		throw *this;													 \
	}

CORE_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
CORE_IMPLEMENT_EXCEPTION(AssertionViolationException, Exception, "Assertion violation")
CORE_IMPLEMENT_EXCEPTION(NullPointerException, Exception, "Null pointer")
CORE_IMPLEMENT_EXCEPTION(BugcheckException, Exception, "Bugcheck")
CORE_IMPLEMENT_EXCEPTION(InvalidArgumentException, Exception, "Invalid argument")
CORE_IMPLEMENT_EXCEPTION(NotImplementedException, Exception, "Not implemented")
CORE_IMPLEMENT_EXCEPTION(RangeException, Exception, "Out of range")
CORE_IMPLEMENT_EXCEPTION(IllegalStateException, Exception, "Illegal state")
CORE_IMPLEMENT_EXCEPTION(InvalidAccessException, Exception, "Invalid access")
CORE_IMPLEMENT_EXCEPTION(SignalException, Exception, "Signal received")
CORE_IMPLEMENT_EXCEPTION(UnhandledException, Exception, "Signal received")

CORE_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
CORE_IMPLEMENT_EXCEPTION(NotFoundException, Exception, "Not found")
CORE_IMPLEMENT_EXCEPTION(ExistsException, Exception, "Exists")
CORE_IMPLEMENT_EXCEPTION(TimeoutException, Exception, "Timeout")
CORE_IMPLEMENT_EXCEPTION(SystemException, Exception, "System exception")
CORE_IMPLEMENT_EXCEPTION(RegularExpressionException, Exception, "Error in regular expression")
CORE_IMPLEMENT_EXCEPTION(LibraryLoadException, Exception, "Cannot load library")
CORE_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, Exception, "Library already loaded")
CORE_IMPLEMENT_EXCEPTION(NoCore_ThreadAvailableException, Exception, "No thread available")
CORE_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, Exception, "Property not supported")
CORE_IMPLEMENT_EXCEPTION(PoolOverflowException, Exception, "Pool overflow")
CORE_IMPLEMENT_EXCEPTION(NoPermissionException, Exception, "No permission")
CORE_IMPLEMENT_EXCEPTION(OutOfMemoryException, Exception, "Out of memory")
CORE_IMPLEMENT_EXCEPTION(DataException, Exception, "Data error")

CORE_IMPLEMENT_EXCEPTION(DataFormatException, Exception, "Bad data format")
CORE_IMPLEMENT_EXCEPTION(SyntaxException, Exception, "Syntax error")
CORE_IMPLEMENT_EXCEPTION(CircularReferenceException, Exception, "Circular reference")
CORE_IMPLEMENT_EXCEPTION(PathSyntaxException, Exception, "Bad path syntax")
CORE_IMPLEMENT_EXCEPTION(IOException, Exception, "I/O error")
CORE_IMPLEMENT_EXCEPTION(FileException, Exception, "File access error")
CORE_IMPLEMENT_EXCEPTION(FileExistsException, Exception, "File exists")
CORE_IMPLEMENT_EXCEPTION(FileNotFoundException, Exception, "File not found")
CORE_IMPLEMENT_EXCEPTION(PathNotFoundException, Exception, "Path not found")
CORE_IMPLEMENT_EXCEPTION(FileReadOnlyException, Exception, "File is read-only")
CORE_IMPLEMENT_EXCEPTION(FileAccessDeniedException, Exception, "Access to file denied")
CORE_IMPLEMENT_EXCEPTION(CreateFileException, Exception, "Cannot create file")
CORE_IMPLEMENT_EXCEPTION(OpenFileException, Exception, "Cannot open file")
CORE_IMPLEMENT_EXCEPTION(WriteFileException, Exception, "Cannot write file")
CORE_IMPLEMENT_EXCEPTION(ReadFileException, Exception, "Cannot read file")
CORE_IMPLEMENT_EXCEPTION(UnknownURISchemeException, Exception, "Unknown URI scheme")


CORE_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
CORE_IMPLEMENT_EXCEPTION(BadCastException, Exception, "Bad cast exception")

CORE_IMPLEMENT_EXCEPTION(NetException, Exception, "Net Exception")
CORE_IMPLEMENT_EXCEPTION(InvalidAddressException, Exception, "Invalid address")
CORE_IMPLEMENT_EXCEPTION(ServiceNotFoundException, Exception, "Service not found")
CORE_IMPLEMENT_EXCEPTION(ConnectionAbortedException, Exception, "Software caused connection abort")
CORE_IMPLEMENT_EXCEPTION(ConnectionResetException, Exception, "Connection reset by peer")
CORE_IMPLEMENT_EXCEPTION(ConnectionRefusedException, Exception, "Connection refused")

} 

