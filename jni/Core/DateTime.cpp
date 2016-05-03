#include "DateTime.h"
#include "Timespan.h"
#include <algorithm>
#include <cmath>
//#include "Types.h"
using namespace std;
namespace CORE {


inline double DateTime::toJulianDay(Timestamp::UtcTimeVal utcTime)
{
	double utcDays = double(utcTime)/864000000000.0;
	return utcDays + 2299160.5; // first day of Gregorian reform (Oct 15 1582)
}


inline Timestamp::UtcTimeVal DateTime::toUtcTime(double julianDay)
{
	return Timestamp::UtcTimeVal((julianDay - 2299160.5)*864000000000.0);
}


DateTime::DateTime()
{
	Timestamp now;
	_utcTime = now.utcTime();
	printf("utc_time=%lld\n",_utcTime);
	double juli = julianDay();
	printf("*****************core juli = %0.1f\n",juli);
	computeGregorian(juli);
	computeDaytime();
}


DateTime::DateTime(const Timestamp& timestamp):
	_utcTime(timestamp.utcTime())
{
	computeGregorian(julianDay());
	computeDaytime();
}

	
DateTime::DateTime(Int32 year, Int32 month, Int32 day, Int32 hour, Int32 minute, Int32 second, Int32 millisecond, Int32 microsecond):
	_year(year),
	_month(month),
	_day(day),
	_hour(hour),
	_minute(minute),
	_second(second),
	_millisecond(millisecond),
	_microsecond(microsecond)
{
	_utcTime = toUtcTime(toJulianDay(year, month, day)) + 10*(hour*Timespan::HOURS + minute*Timespan::MINUTES + second*Timespan::SECONDS + millisecond*Timespan::MILLISECONDS + microsecond);
}


DateTime::DateTime(double julianDay):
	_utcTime(toUtcTime(julianDay))
{
	computeGregorian(julianDay);
}


DateTime::DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff):
	_utcTime(utcTime + diff*10)
{
	computeGregorian(julianDay());
	computeDaytime();
}


DateTime::DateTime(const DateTime& dateTime):
	_utcTime(dateTime._utcTime),
	_year(dateTime._year),
	_month(dateTime._month),
	_day(dateTime._day),
	_hour(dateTime._hour),
	_minute(dateTime._minute),
	_second(dateTime._second),
	_millisecond(dateTime._millisecond),
	_microsecond(dateTime._microsecond)
{
}


DateTime::~DateTime()
{
}


DateTime& DateTime::operator = (const DateTime& dateTime)
{
	if (&dateTime != this)
	{
		_utcTime	 = dateTime._utcTime;
		_year		= dateTime._year;
		_month	   = dateTime._month;
		_day		 = dateTime._day;
		_hour		= dateTime._hour;
		_minute	  = dateTime._minute;
		_second	  = dateTime._second;
		_millisecond = dateTime._millisecond;
		_microsecond = dateTime._microsecond;
	}
	return *this;
}

	
DateTime& DateTime::operator = (const Timestamp& timestamp)
{
	_utcTime = timestamp.utcTime();
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}


DateTime& DateTime::operator = (double julianDay)
{
	_utcTime = toUtcTime(julianDay);
	computeGregorian(julianDay);
	return *this;
}


DateTime& DateTime::assign(Int32 year, Int32 month, Int32 day, Int32 hour, Int32 minute, Int32 second, Int32 millisecond, Int32 microsecond)
{
	_utcTime	 = toUtcTime(toJulianDay(year, month, day)) + 10*(hour*Timespan::HOURS + minute*Timespan::MINUTES + second*Timespan::SECONDS + millisecond*Timespan::MILLISECONDS + microsecond);
	_year		= year;
	_month	   = month;
	_day		 = day;
	_hour		= hour;
	_minute	  = minute;
	_second	  = second;
	_millisecond = millisecond;
	_microsecond = microsecond;
	
	return *this;
}


Int32 DateTime::dayOfWeek() const
{
	return Int32((floor(julianDay() + 1.5))) % 7;
}


Int32 DateTime::dayOfYear() const
{
	Int32 doy = 0;
	for (Int32 month = 1; month < _month; ++month)
		doy += daysOfMonth(_year, month);
	doy += _day;
	return doy;
}


Int32 DateTime::daysOfMonth(Int32 year, Int32 month)
{
	static Int32 daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	if (month == 2 && isLeapYear(year))
		return 29;
	else
		return daysOfMonthTable[month];
}


bool DateTime::isValid(Int32 year, Int32 month, Int32 day, Int32 hour, Int32 minute, Int32 second, Int32 millisecond, Int32 microsecond)
{
	return
		(year >= 0 && year <= 9999) &&
		(month >= 1 && month <= 12) &&
		(day >= 1 && day <= daysOfMonth(year, month)) &&
		(hour >= 0 && hour <= 23) &&
		(minute >= 0 && minute <= 59) &&
		(second >= 0 && second <= 59) &&
		(millisecond >= 0 && millisecond <= 999) &&
		(microsecond >= 0 && microsecond <= 999);
}


Int32 DateTime::week(Int32 firstDayOfWeek) const
{
	/// find the first firstDayOfWeek.
	Int32 baseDay = 1;
	while (DateTime(_year, 1, baseDay).dayOfWeek() != firstDayOfWeek) ++baseDay;

	Int32 doy  = dayOfYear();
	Int32 offs = baseDay <= 4 ? 0 : 1; 
	if (doy < baseDay)
		return offs;
	else
		return (doy - baseDay)/7 + 1 + offs;
}


double DateTime::julianDay() const
{
	return toJulianDay(_utcTime);
}


DateTime DateTime::operator + (const Timespan& span) const
{
	return DateTime(_utcTime, span.totalMicroseconds());
}


DateTime DateTime::operator - (const Timespan& span) const
{
	return DateTime(_utcTime, -span.totalMicroseconds());
}


Timespan DateTime::operator - (const DateTime& dateTime) const
{
	return Timespan((_utcTime - dateTime._utcTime)/10);
}


DateTime& DateTime::operator += (const Timespan& span)
{
	_utcTime += span.totalMicroseconds()*10;
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}


DateTime& DateTime::operator -= (const Timespan& span)
{
	_utcTime -= span.totalMicroseconds()*10;
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}


void DateTime::makeUTC(Int32 tzd)
{
	operator -= (Timespan(((Timestamp::TimeDiff) tzd)*Timespan::SECONDS));
}

	
void DateTime::makeLocal(Int32 tzd)
{
	operator += (Timespan(((Timestamp::TimeDiff) tzd)*Timespan::SECONDS));
}


double DateTime::toJulianDay(Int32 year, Int32 month, Int32 day, Int32 hour, Int32 minute, Int32 second, Int32 millisecond, Int32 microsecond)
{
	// lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
	static Int32 lookup[] = {-91, -60, -30, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337};
 
	// day to double
	double dday = double(day) + ((double((hour*60 + minute)*60 + second)*1000 + millisecond)*1000 + microsecond)/86400000000.0;
	if (month < 3)
	{
		month += 12;
		--year;
	}
	double dyear = double(year);
	return dday + lookup[month] + 365*year + floor(dyear/4) - floor(dyear/100) + floor(dyear/400) + 1721118.5;
}


void DateTime::checkLimit(short& lower, short& higher, short limit)
{
	if (lower > limit)
	{
		higher += short(lower / limit);
		lower   = short(lower % limit);
	}
}


void DateTime::normalize()
{
	checkLimit(_microsecond, _millisecond, 999);
	checkLimit(_millisecond, _second, 999);
	checkLimit(_second, _minute, 59);
	checkLimit(_minute, _hour, 59);
	checkLimit(_hour, _day, 23);

	if (_day > daysOfMonth(_year, _month))
	{
		_day -= daysOfMonth(_year, _month);
		if (++_month > 12)
		{
			++_year;
			_month -= 12;
		}
	}
}

 
void DateTime::computeGregorian(double julianDay)
{
	double z	= floor(julianDay - 1721118.5);
	printf("-----------------------core-------------------");
	printf("z=%0.1f\n",z);
	double r	= julianDay - 1721118.5 - z;
	printf("r=%0.1f\n",r);
	double g	= z - 0.25;
	printf("g=%0.1f\n",g);
	double a	= floor(g / 36524.25);
	printf("a=%0.1f\n",a);
	double b	= a - floor(a/4);
	printf("b=%0.1f\n",b);
	_year	   = short(floor((b + g)/365.25));
	printf("_year=%d\n",_year);
	double c	= b + z - floor(365.25*_year);
	_month	  = short(floor((5*c + 456)/153));
	double dday = c - floor((153.0*_month - 457)/5) + r;
	_day		= short(dday);
	if (_month > 12)
	{
		++_year;
		_month -= 12;
	}
	r	  *= 24;
	_hour   = short(floor(r));
	r	  -= floor(r);
	r	  *= 60;
	_minute = short(floor(r));
	r	  -= floor(r);
	r	  *= 60;
	_second = short(floor(r));
	r	  -= floor(r);
	r	  *= 1000;
	_millisecond = short(floor(r));
	r	  -= floor(r);  
	r	  *= 1000;
	_microsecond = short(r + 0.5);
	printf("core*********************year=%d,mon=%d,day=%d\n",_year,_month,_day);
	normalize();
	printf("-----------------------core end-------------------");

}


void DateTime::computeDaytime()
{
	Timespan span(_utcTime/10);
	_hour		= span.hours();
	_minute	  = span.minutes();
	_second	  = span.seconds();
	_millisecond = span.milliseconds();
	_microsecond = span.microseconds();
}

CData DateTime::ToString()
{
	CData cdTime;
	cdTime = CData(_year);
	cdTime += "-";
	cdTime += CData(_month);
	cdTime += "-";
	cdTime += CData(_day);
	cdTime += " ";
	cdTime += CData(_hour);
	cdTime += ":";
	cdTime += CData(_minute);
	cdTime += ":";
	cdTime += CData(_second);
	return cdTime;
}

void DateTime::getLocalTime(int &year, int &month, int &day, int &hour, int &minute, int &second)
{
	/*
	struct tm {
	int tm_sec;   // seconds after the minute ¡ª [0, 60]£¨×¢£¨1£©£©
	int tm_min;   // minutes after the hour ¡ª [0, 59]
	int tm_hour;  // hours since midnight ¡ª [0, 23]
	int tm_mday;  // day of the month ¡ª [1, 31]
	int tm_mon;   // months since January ¡ª [0, 11]
	int tm_year;  // years since 1900
	int tm_wday;  // days since Sunday ¡ª [0, 6]
	int tm_yday;  // days since January 1 ¡ª [0, 365]
	int tm_isdst; // Daylight Saving Time flag
	};
	*/
	
	time_t timer; 
	struct tm *tblock; 
	timer=time(0); 
	tblock=localtime(&timer); 
	year=tblock->tm_year+1900;
	month=tblock->tm_mon+1;
	day=tblock->tm_mday;
	hour=tblock->tm_hour;
	minute=tblock->tm_min;
	second=tblock->tm_sec;
}


unsigned int DateTime::toLocalTime(int year, int month, int day, int hour, int minute, int second)
{
	time_t t; 
	DateTime date(year, month, day, hour, minute, second);
	Timestamp timestamp;
	timestamp=date.timestamp();
	t=timestamp.epochTime();
	return t;
}


unsigned int DateTime::getLocalTime()
{
	time_t t; 
	Timestamp timestamp;
	t=timestamp.epochTime();
	return t;
}


void DateTime::getLocalTime(unsigned int time, int &year, int &month, int &day, int &hour, int &minute, int &second)
{
	Timestamp timestamp(time*Timestamp::resolution());
	DateTime date(timestamp);
	year=date.year();
	month=date.month();
	day=date.day();
	hour=date.hour();
	minute=date.minute();
	second=date.second();
}


}


