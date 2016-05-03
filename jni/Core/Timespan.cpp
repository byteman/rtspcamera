#include "Timespan.h"


namespace CORE {


const Timespan::TimeDiff Timespan::MILLISECONDS = 1000;
const Timespan::TimeDiff Timespan::SECONDS	  = 1000*Timespan::MILLISECONDS;
const Timespan::TimeDiff Timespan::MINUTES	  =   60*Timespan::SECONDS;
const Timespan::TimeDiff Timespan::HOURS		=   60*Timespan::MINUTES;
const Timespan::TimeDiff Timespan::DAYS		 =   24*Timespan::HOURS;


Timespan::Timespan():
	m_span(0)
{
}

	
Timespan::Timespan(TimeDiff microseconds):
	m_span(microseconds)
{
}


Timespan::Timespan(Int32 seconds, Int32 microseconds):
	m_span(TimeDiff(seconds)*SECONDS + microseconds)
{
}

	
Timespan::Timespan(Int32 days, Int32 hours, Int32 minutes, Int32 seconds, Int32 microseconds):
	m_span(TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS + TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS + TimeDiff(days)*DAYS)
{
}


Timespan::Timespan(const Timespan& timespan):
	m_span(timespan.m_span)
{
}


Timespan::~Timespan()
{
}


Timespan& Timespan::operator = (const Timespan& timespan)
{
	m_span = timespan.m_span;
	return *this;
}


Timespan& Timespan::operator = (TimeDiff microseconds)
{
	m_span = microseconds;
	return *this;
}


Timespan& Timespan::assign(Int32 days, Int32 hours, Int32 minutes, Int32 seconds, Int32 microseconds)
{
	m_span = TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS + TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS + TimeDiff(days)*DAYS;
	return *this;
}


Timespan& Timespan::assign(Int32 seconds, Int32 microseconds)
{
	m_span = TimeDiff(seconds)*SECONDS + TimeDiff(microseconds);
	return *this;
}


Timespan Timespan::operator + (const Timespan& d) const
{
	return Timespan(m_span + d.m_span);
}


Timespan Timespan::operator - (const Timespan& d) const
{
	return Timespan(m_span - d.m_span);
}


Timespan& Timespan::operator += (const Timespan& d)
{
	m_span += d.m_span;
	return *this;
}


Timespan& Timespan::operator -= (const Timespan& d)
{
	m_span -= d.m_span;
	return *this;
}


Timespan Timespan::operator + (TimeDiff microseconds) const
{
	return Timespan(m_span + microseconds);
}


Timespan Timespan::operator - (TimeDiff microseconds) const
{
	return Timespan(m_span - microseconds);
}


Timespan& Timespan::operator += (TimeDiff microseconds)
{
	m_span += microseconds;
	return *this;
}


Timespan& Timespan::operator -= (TimeDiff microseconds)
{
	m_span -= microseconds;
	return *this;
}


Int32 Timespan::days() const
{
	return Int32(m_span/DAYS);
}


Int32 Timespan::hours() const
{
	return Int32((m_span/HOURS) % 24);
}

	
Int32 Timespan::totalHours() const
{
	return Int32(m_span/HOURS);
}

	
Int32 Timespan::minutes() const
{
	return Int32((m_span/MINUTES) % 60);
}

	
Int32 Timespan::totalMinutes() const
{
	return Int32(m_span/MINUTES);
}

	
Int32 Timespan::seconds() const
{
	return Int32((m_span/SECONDS) % 60);
}

	
Int32 Timespan::totalSeconds() const
{
	return Int32(m_span/SECONDS);
}

	
Int32 Timespan::milliseconds() const
{
	return Int32((m_span/MILLISECONDS) % 1000);
}

	
Timespan::TimeDiff Timespan::totalMilliseconds() const
{
	return m_span/MILLISECONDS;
}

	
Int32 Timespan::microseconds() const
{
	return Int32(m_span % 1000);
}


Int32 Timespan::useconds() const
{
	return Int32(m_span % 1000000);
}

	
Timespan::TimeDiff Timespan::totalMicroseconds() const
{
	return m_span;
}


bool Timespan::operator == (const Timespan& ts) const
{
	return m_span == ts.m_span;
}


bool Timespan::operator != (const Timespan& ts) const
{
	return m_span != ts.m_span;
}


bool Timespan::operator >  (const Timespan& ts) const
{
	return m_span > ts.m_span;
}


bool Timespan::operator >= (const Timespan& ts) const
{
	return m_span >= ts.m_span;
}


bool Timespan::operator <  (const Timespan& ts) const
{
	return m_span < ts.m_span;
}


bool Timespan::operator <= (const Timespan& ts) const
{
	return m_span <= ts.m_span;
}


bool Timespan::operator == (TimeDiff microseconds) const
{
	return m_span == microseconds;
}


bool Timespan::operator != (TimeDiff microseconds) const
{
	return m_span != microseconds;
}


bool Timespan::operator >  (TimeDiff microseconds) const
{
	return m_span > microseconds;
}


bool Timespan::operator >= (TimeDiff microseconds) const
{
	return m_span >= microseconds;
}


bool Timespan::operator <  (TimeDiff microseconds) const
{
	return m_span < microseconds;
}


bool Timespan::operator <= (TimeDiff microseconds) const
{
	return m_span <= microseconds;
}

} 
