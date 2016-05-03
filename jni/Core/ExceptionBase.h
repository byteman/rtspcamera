#ifndef ExceptionBase_H
#define ExceptionBase_H


#include <stdexcept>
#include "Core.h"
#include "Data.h"

namespace CORE {


class CORE_API Exception: public std::exception
{
public:
	Exception(const CData& msg);

	Exception(const CData& msg, const CData& arg);

	~Exception() throw();
 
	Exception& operator = (const Exception& exc);
 
	virtual const char* name() const throw();
		 
	virtual const char* className() const throw();		
		
	virtual const char* what() const throw();
		 
	const CData& message() const;
		 
	CData displayText() const;
 
	virtual Exception* clone() const;
		 
	virtual void rethrow() const;
 
protected:
	Exception();
		 
private:
	CData _msg;
};


//
// inlines
//
inline const CData& Exception::message() const
{
	return _msg;
}

}



#endif

