#include "ExceptionBase.h"
#include <typeinfo>


namespace CORE {


Exception::Exception()
{
}


Exception::Exception(const CData& msg)
{
	_msg = msg;
}


Exception::Exception(const CData& msg, const CData& arg)
{
	_msg = msg;
	if (!arg.empty())
	{
		_msg +=(": ");
		_msg +=(arg);
	}
}


Exception::~Exception() throw()
{
	
}


Exception& Exception::operator = (const Exception& exc)
{
	if (&exc != this)
	{
		_msg = exc._msg;
	}
	return *this;
}


const char* Exception::name() const throw()
{
	return "Exception";
}


const char* Exception::className() const throw()
{
	return typeid(*this).name();
}

	
const char* Exception::what() const throw()
{
	return name();
}

	
CData Exception::displayText() const
{
	CData txt = name();
	if (!_msg.empty())
	{
		txt +=(": ");
		txt +=(_msg);
	}
	return txt;
}


Exception* Exception::clone() const
{
	return new Exception(*this);
}


void Exception::rethrow() const
{
	throw *this;
}


}

