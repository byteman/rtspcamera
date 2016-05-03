#ifndef  Timespan_H
#define  Timespan_H


#include "Core.h"
#include "Timestamp.h"


namespace CORE {


class CORE_API Timespan
 {
public:
	typedef Timestamp::TimeDiff TimeDiff;

	Timespan();
		 
	Timespan(TimeDiff microseconds);
	 
	Timespan(Int32 seconds, Int32 microseconds);
	 
	Timespan(Int32 days, Int32 hours, Int32 minutes, Int32 seconds, Int32 microseconds);
	 
	Timespan(const Timespan& timespan);
 
	~Timespan();
 
	Timespan& operator = (const Timespan& timespan);
 
	Timespan& operator = (TimeDiff microseconds);
		 
	Timespan& assign(Int32 days, Int32 hours, Int32 minutes, Int32 seconds, Int32 microseconds);
		 
	Timespan& assign(Int32 seconds, Int32 microseconds);
  
	bool operator == (const Timespan& ts) const;
	bool operator != (const Timespan& ts) const;
	bool operator >  (const Timespan& ts) const;
	bool operator >= (const Timespan& ts) const;
	bool operator <  (const Timespan& ts) const;
	bool operator <= (const Timespan& ts) const;

	bool operator == (TimeDiff microseconds) const;
	bool operator != (TimeDiff microseconds) const;
	bool operator >  (TimeDiff microseconds) const;
	bool operator >= (TimeDiff microseconds) const;
	bool operator <  (TimeDiff microseconds) const;
	bool operator <= (TimeDiff microseconds) const;
	
	Timespan operator + (const Timespan& d) const;
	Timespan operator - (const Timespan& d) const;
	Timespan& operator += (const Timespan& d);
	Timespan& operator -= (const Timespan& d);

	Timespan operator + (TimeDiff microseconds) const;
	Timespan operator - (TimeDiff microseconds) const;
	Timespan& operator += (TimeDiff microseconds);
	Timespan& operator -= (TimeDiff microseconds);

	Int32 days() const;
		 
	Int32 hours() const;
		 
	Int32 totalHours() const;
		 
	Int32 minutes() const;
		 
	Int32 totalMinutes() const;
		 
	Int32 seconds() const;
		 
	Int32 totalSeconds() const;
		 
	Int32 milliseconds() const;
		 
	TimeDiff totalMilliseconds() const;
		 
	Int32 microseconds() const;
		 
	Int32 useconds() const;
		 
	TimeDiff totalMicroseconds() const;
 
	static const TimeDiff MILLISECONDS; /// The number of microseconds in a millisecond.
	static const TimeDiff SECONDS;	  /// The number of microseconds in a second.
	static const TimeDiff MINUTES;	  /// The number of microseconds in a minute.
	static const TimeDiff HOURS;		/// The number of microseconds in a hour.
	static const TimeDiff DAYS;		 /// The number of microseconds in a day.

private:
	TimeDiff m_span;
};





}  


#endif  
