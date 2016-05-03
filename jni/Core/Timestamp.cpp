#include "Timestamp.h"
#if defined(__LINUX__)
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif

#ifdef WIN32
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment(lib, "Winmm.lib")
#else
	#include <stdio.h>
	#include <sys/time.h>
	#include <signal.h>
	#include <unistd.h>
	#include <string.h>
	#include <errno.h>
#endif


namespace CORE {

#ifdef WIN32
	static int start;
#else
	static struct timeval start;
#endif


class SystemSystemPrivateData
{
public:
	SystemSystemPrivateData()
	{
		#ifdef WIN32
			timeBeginPeriod(1);		
			start = timeGetTime();
		#else
			gettimeofday(&start, NULL);
		#endif
	}
			
	~SystemSystemPrivateData()
	{
		
	}
};
static SystemSystemPrivateData gSystemSystemPrivateData;



Timestamp::Timestamp()
{
	update();
}


Timestamp::Timestamp(TimeVal tv)
{
	_ts = tv;
}


Timestamp::Timestamp(const Timestamp& other)
{
	_ts = other._ts;
}


Timestamp::~Timestamp()
{
}


Timestamp& Timestamp::operator = (const Timestamp& other)
{
	_ts = other._ts;
	return *this;
}


Timestamp& Timestamp::operator = (TimeVal tv)
{
	_ts = tv;
	return *this;
}


Timestamp Timestamp::fromUtcTime(UtcTimeVal val)
{
	val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return Timestamp(val);
}


void Timestamp::update()
{
#if defined(CORE_OS_FAMILY_WINDOWS)

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart  = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	_ts = ts.QuadPart/10;

	//YMM
	UInt64 i;
	i=8*3600;
	i*=1000;
	i*=1000;
	_ts+=i;
	
#else

	struct timeval tv;
	gettimeofday(&tv, NULL);
	_ts = TimeVal(tv.tv_sec)*resolution() + tv.tv_usec;
	
#endif
}


#if defined(_WIN32)


Timestamp Timestamp::fromFileTimeNP(UInt32 fileTimeLow, UInt32 fileTimeHigh)
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;
	
	ULARGE_INTEGER ts;
	ts.LowPart  = fileTimeLow;
	ts.HighPart = fileTimeHigh;
	ts.QuadPart -= epoch.QuadPart;

	return Timestamp(ts.QuadPart/10);
}


void Timestamp::toFileTimeNP(UInt32& fileTimeLow, UInt32& fileTimeHigh) const
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.QuadPart  = _ts*10;
	ts.QuadPart += epoch.QuadPart;
	fileTimeLow  = ts.LowPart;
	fileTimeHigh = ts.HighPart;
}

#endif


unsigned int Timestamp::System_GetTicks()
{
#ifdef WIN32
	#define TIME_WRAP_VALUE	(~(DWORD)0)
	int now, ticks;
	now = timeGetTime();
	if ( now < start ) 
	{
		ticks = (TIME_WRAP_VALUE-start) + now;
	} 
	else 
	{
		ticks = (now - start);
	}
	
	return(ticks);
#else
	unsigned int ticks;
	struct timeval now;
	gettimeofday(&now, NULL);
	ticks=(now.tv_sec-start.tv_sec)*1000+(now.tv_usec-start.tv_usec)/1000;
	return(ticks);
#endif
}


void Timestamp::System_Delay(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms<<10);
#endif
}

}

