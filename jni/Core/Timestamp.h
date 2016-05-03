#ifndef  Timestamp_H
#define  Timestamp_H

#include "Core.h"
#include <ctime>

namespace CORE {



class CORE_API Timestamp
 {
public:
	typedef Int64 TimeVal;	/// monotonic UTC time value in microsecond resolution
	typedef Int64 UtcTimeVal; /// monotonic UTC time value in 100 nanosecond resolution
	typedef Int64 TimeDiff;   /// difference between two timestamps in microseconds

	Timestamp();
		 
	Timestamp(TimeVal tv);
		 
	Timestamp(const Timestamp& other);
		 
	~Timestamp();
		 
	Timestamp& operator = (const Timestamp& other);
	Timestamp& operator = (TimeVal tv);
		 
	void update();
 
	bool operator == (const Timestamp& ts) const;
	bool operator != (const Timestamp& ts) const;
	bool operator >  (const Timestamp& ts) const;
	bool operator >= (const Timestamp& ts) const;
	bool operator <  (const Timestamp& ts) const;
	bool operator <= (const Timestamp& ts) const;
	
	Timestamp  operator +  (TimeDiff d) const;
	Timestamp  operator -  (TimeDiff d) const;
	TimeDiff   operator -  (const Timestamp& ts) const;
	Timestamp& operator += (TimeDiff d);
	Timestamp& operator -= (TimeDiff d);
	
	time_t epochTime() const;
		 
	UtcTimeVal utcTime() const;
	 
	TimeVal epochMicroseconds() const;
	 
	TimeDiff elapsed() const;
	 
	bool isElapsed(TimeDiff interval) const;
			  
	static Timestamp fromUtcTime(UtcTimeVal val);
		 
	static TimeVal resolution();
 
#if defined(_WIN32)
	static Timestamp fromFileTimeNP(UInt32 fileTimeLow, UInt32 fileTimeHigh);
	void toFileTimeNP(UInt32& fileTimeLow, UInt32& fileTimeHigh) const;
#endif

private:
	TimeVal _ts;

public:
	
	static unsigned int System_GetTicks();

	static void System_Delay(int ms);
};


//
// inlines
//
inline bool Timestamp::operator == (const Timestamp& ts) const
{
	return _ts == ts._ts;
}


inline bool Timestamp::operator != (const Timestamp& ts) const
{
	return _ts != ts._ts;
}


inline bool Timestamp::operator >  (const Timestamp& ts) const
{
	return _ts > ts._ts;
}


inline bool Timestamp::operator >= (const Timestamp& ts) const
{
	return _ts >= ts._ts;
}


inline bool Timestamp::operator <  (const Timestamp& ts) const
{
	return _ts < ts._ts;
}


inline bool Timestamp::operator <= (const Timestamp& ts) const
{
	return _ts <= ts._ts;
}


inline Timestamp Timestamp::operator + (Timestamp::TimeDiff d) const
{
	return Timestamp(_ts + d);
}


inline Timestamp Timestamp::operator - (Timestamp::TimeDiff d) const
{
	return Timestamp(_ts - d);
}


inline Timestamp::TimeDiff Timestamp::operator - (const Timestamp& ts) const
{
	return _ts - ts._ts;
}


inline Timestamp& Timestamp::operator += (Timestamp::TimeDiff d)
{
	_ts += d;
	return *this;
}


inline Timestamp& Timestamp::operator -= (Timestamp::TimeDiff d)
{
	_ts -= d;
	return *this;
}


inline time_t Timestamp::epochTime() const
{
	return time_t(_ts/resolution());
}


inline Timestamp::UtcTimeVal Timestamp::utcTime() const
{
	return _ts*10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}


inline Timestamp::TimeVal Timestamp::epochMicroseconds() const
{
	return _ts;
}


inline Timestamp::TimeDiff Timestamp::elapsed() const
{
	Timestamp now;
	return now - *this;
}


inline bool Timestamp::isElapsed(Timestamp::TimeDiff interval) const
{
	Timestamp now;
	Timestamp::TimeDiff diff = now - *this;
	return diff >= interval;
}


inline Timestamp::TimeVal Timestamp::resolution()
{
	return 1000000;
}


} 


#endif  
