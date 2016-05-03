#include "Random.h"
#include <time.h>


namespace CORE {


Random::Random():
	numcalls(0)
{
	
}

Random::~Random()
{
}

UInt32 Random::randomUInt32()
{
	UInt32 seed;
	unsigned char byte;
	UInt32 retval;

	retval = 0;
	if (numcalls == 0)
	{
		numcalls++;
		seed =(UInt32)time(NULL);
		seed *= numcalls;
		seed -= clock();
		seed += (UInt32)time(NULL);
		srand(seed);
	}
	byte = randomByte();
	retval |= ((UInt32)byte);
	byte = randomByte();
	retval |= (((UInt32)byte)<<8);
	byte = randomByte();
	retval |= (((UInt32)byte)<<16);
	byte = randomByte();
	retval |= (((UInt32)byte)<<24);

	return retval;
}


UInt16 Random::randomUInt16()
{
	UInt32 seed;
	unsigned char byte;
	UInt16 retval;

	retval = 0;
	numcalls++;
	seed =(UInt32)time(NULL);
	seed += numcalls;
	seed += clock();
	srand(seed);
	byte = randomByte();
	retval |= ((UInt16)byte);
	byte = randomByte();
	retval |= (((UInt16)byte)<<8);

	return retval;
}


Random& Random::defaultRandom()
{
	static SingletonHolder<Random> sh;
	return *sh.get();
}

void Random::GenerateCallId(CData & Guid)
{
	Guid = ((CData)(unsigned long)randomUInt32());
	Guid = Guid + "-" + (CData)randomUInt16() + "-" + (CData)randomUInt16();
}


}




	