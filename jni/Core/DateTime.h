#ifndef DateTime_H
#define DateTime_H

#include "Core.h"
#include "Timestamp.h"
#include "Timespan.h"
#include "Data.h"

namespace CORE {


class CORE_API DateTime	
{
public:
	enum Months
		/// Symbolic names for month numbers (1 to 12).
	{
		JANUARY = 1,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER
	};
	
	enum DaysOfWeek
		/// Symbolic names for week day numbers (0 to 6).
	{
		SUNDAY = 0,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};
		
	DateTime();

	DateTime(const Timestamp& timestamp);
		
	DateTime(Int32 year, Int32 month, Int32 day, Int32 hour = 0, Int32 minute = 0, Int32 second = 0, Int32 millisecond = 0, Int32 microsecond = 0);

	DateTime(double julianDay);

	DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff);

	DateTime(const DateTime& dateTime);

	~DateTime();

	DateTime& operator = (const DateTime& dateTime);
		
	DateTime& operator = (const Timestamp& timestamp);

	DateTime& operator = (double julianDay);

	DateTime& assign(Int32 year, Int32 month, Int32 day, Int32 hour = 0, Int32 minute = 0, Int32 second = 0, Int32 millisecond = 0, Int32 microseconds = 0);

	Int32 year() const;
		
	Int32 month() const;
	
	Int32 week(Int32 firstDayOfWeek = MONDAY) const;
	
	Int32 day() const;
		
	Int32 dayOfWeek() const;
	
	Int32 dayOfYear() const;
	
	Int32 hour() const;
		
	Int32 hourAMPM() const;
	
	bool isAM() const;

	bool isPM() const;
		
	Int32 minute() const;
		
	Int32 second() const;
		
	Int32 millisecond() const;
	
	Int32 microsecond() const;
	
	double julianDay() const;
		
	Timestamp timestamp() const;

	Timestamp::UtcTimeVal utcTime() const;
		
	bool operator == (const DateTime& dateTime) const;	
	bool operator != (const DateTime& dateTime) const;	
	bool operator <  (const DateTime& dateTime) const;	
	bool operator <= (const DateTime& dateTime) const;	
	bool operator >  (const DateTime& dateTime) const;	
	bool operator >= (const DateTime& dateTime) const;	

	DateTime  operator +  (const Timespan& span) const;
	DateTime  operator -  (const Timespan& span) const;
	Timespan  operator -  (const DateTime& dateTime) const;
	DateTime& operator += (const Timespan& span);
	DateTime& operator -= (const Timespan& span);
	
	void makeUTC(Int32 tzd);
		
	void makeLocal(Int32 tzd);
	
	static bool isLeapYear(Int32 year);
		
	static Int32 daysOfMonth(Int32 year, Int32 month);
		
	static bool isValid(Int32 year, Int32 month, Int32 day, Int32 hour = 0, Int32 minute = 0, Int32 second = 0, Int32 millisecond = 0, Int32 microsecond = 0);

	CData ToString();

	static void getLocalTime(int &year, int &month, int &day, int &hour, int &minute, int &second);

	static unsigned int toLocalTime(int year, int month, int day, int hour=0, int minute=0, int second=0);

		///@brief
		///得到机器的日历时间或者设置日历时间 
	static unsigned int getLocalTime();

	static void getLocalTime(unsigned int time, int &year, int &month, int &day, int &hour, int &minute, int &second);

protected:	
	static double toJulianDay(Timestamp::UtcTimeVal utcTime);
	
	static double toJulianDay(Int32 year, Int32 month, Int32 day, Int32 hour = 0, Int32 minute = 0, Int32 second = 0, Int32 millisecond = 0, Int32 microsecond = 0);
	
	static Timestamp::UtcTimeVal toUtcTime(double julianDay);
		
	void computeGregorian(double julianDay);

	void computeDaytime();

private:
	void checkLimit(short& lower, short& higher, short limit);
	void normalize();

	Timestamp::UtcTimeVal _utcTime;
	short  _year;
	short  _month;
	short  _day;
	short  _hour;
	short  _minute;
	short  _second;
	short  _millisecond;
	short  _microsecond;
};


//
// inlines
//
inline Timestamp DateTime::timestamp() const
{
	return Timestamp::fromUtcTime(_utcTime);
}


inline Timestamp::UtcTimeVal DateTime::utcTime() const
{
	return _utcTime;
}


inline Int32 DateTime::year() const
{
	return _year;
}

	
inline Int32 DateTime::month() const
{
	return _month;
}

	
inline Int32 DateTime::day() const
{
	return _day;
}

	
inline Int32 DateTime::hour() const
{
	return _hour;
}


inline Int32 DateTime::hourAMPM() const
{
	if (_hour < 1)
		return 12;
	else if (_hour > 12)
		return _hour - 12;
	else
		return _hour;
}


inline bool DateTime::isAM() const
{
	return _hour < 12;
}


inline bool DateTime::isPM() const
{
	return _hour >= 12;
}

	
inline Int32 DateTime::minute() const
{
	return _minute;
}

	
inline Int32 DateTime::second() const
{
	return _second;
}

	
inline Int32 DateTime::millisecond() const
{
	return _millisecond;
}


inline Int32 DateTime::microsecond() const
{
	return _microsecond;
}


inline bool DateTime::operator == (const DateTime& dateTime) const
{
	return _utcTime == dateTime._utcTime;
}


inline bool DateTime::operator != (const DateTime& dateTime) const	
{
	return _utcTime != dateTime._utcTime;
}


inline bool DateTime::operator <  (const DateTime& dateTime) const	
{
	return _utcTime < dateTime._utcTime;
}


inline bool DateTime::operator <= (const DateTime& dateTime) const
{
	return _utcTime <= dateTime._utcTime;
}


inline bool DateTime::operator >  (const DateTime& dateTime) const
{
	return _utcTime > dateTime._utcTime;
}


inline bool DateTime::operator >= (const DateTime& dateTime) const	
{
	return _utcTime >= dateTime._utcTime;
}


inline bool DateTime::isLeapYear(Int32 year)
{
	return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}



}


#endif 
