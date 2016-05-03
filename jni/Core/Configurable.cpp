#include "Configurable.h"


namespace CORE {


Configurable::Configurable()
{

}


Configurable::~Configurable()
{

}


void Configurable::setProperty(const CData name, const CData value)
{
	m_map[name] = value;
}
		

CData Configurable::getProperty(const CData& name)
{
	CData value("");
	StringMap::iterator it = m_map.find(name);
	
	if (it == m_map.end())
	{
		return value;
	}
	else 
	{
		return it->second;
	}
}


void Configurable::configureUpdate()
{
}


};


