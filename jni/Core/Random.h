#ifndef Random_H
#define Random_H


#include "Core.h"
#include "SingletonHolder.h"
#include "Data.h"


namespace CORE {


#define RANDOM_UINT32\
	CORE::Random::defaultRandom().randomUInt32

#define RANDOM_UINT16\
	CORE::Random::defaultRandom().randomUInt16

#define RANDOM_UINT8\
	CORE::Random::defaultRandom().randomByte


class CORE_API Random
{
public:
	
	Random();
	
	virtual ~Random();

public:
	
	UInt32 randomUInt32();
	
	UInt16 randomUInt16();
	
	static unsigned char randomByte() 
	{ 
		return ((unsigned char)(Int32)((256.0*(double)rand())/((double)RAND_MAX+1.0))); 
	}

	static Random& defaultRandom();
	
	void GenerateCallId(CData & Guid);
	
private:
	
	Int32 numcalls;
	
};


}


#endif



