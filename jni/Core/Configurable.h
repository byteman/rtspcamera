#ifndef Configurable_H
#define Configurable_H

#if defined(CORE_OS_FAMILY_WINDOWS)
#pragma warning(disable:4786)
#endif 


#include "Core.h"
#include <map>
#include "Data.h"

namespace CORE {


class CORE_API Configurable
{
public:
	Configurable();

	virtual ~Configurable();
		
	virtual void setProperty(const CData name, const CData value);
		
	virtual CData getProperty(const CData& name);

	virtual void configureUpdate();
	
private:
	
	typedef std::map<CData, CData> StringMap;
	
	StringMap m_map;
	
	
};


} 


#endif 


