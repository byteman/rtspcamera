#include <assert.h>
#include <algorithm>
#include <ctype.h>
#include <math.h>

#include "Data.h"
#if defined(WIN32) || defined(__QNX__)
#define strcasecmp(a,b) stricmp(a,b)
#define strncasecmp(a,b,c) strnicmp(a,b,c)
#endif
 
using namespace std;
//const int CData::npos = INT_MAX;

namespace CORE {

CData::CData() 
   : mLength(0), 
	 mBuf(0),
	 mCapacity(127)
{
   mBuf = new char[mCapacity+1];
   mBuf[0] = 0;
}

CData::CData(int capacity, bool) 
   : mLength(0), 
	 mBuf(0), 
	 mCapacity(capacity)
{
   mBuf = new char[capacity+1];
   mBuf[0] = 0;
}

CData::CData(const char* str, int length) 
   : mLength(length),
	 mBuf(new char[mLength+1]),
	 mCapacity(length)
{
   assert(str);
   memcpy(mBuf, str, length);
   mBuf[length] = 0;
}


CData::CData(const char* str) 
   : mLength(0), 
	 mBuf(0),
	 mCapacity(0)
{
   assert(str);
   mLength = strlen(str);
   mCapacity = mLength;
   mBuf = new char[mLength+1];
   memcpy(mBuf, str, mLength+1);
}

CData::CData(const string& str) : 
   mLength(str.size()), 
   mBuf(new char[mLength+1]),
   mCapacity(mLength)
{
   memcpy(mBuf, str.c_str(), mLength+1);
}

CData::CData(int val)
   : mLength(0), 
	 mBuf(0),
	 mCapacity(0)
{
   if (val == 0)
   {
	  mBuf = new char[2];
	  mBuf[0] = '0';
	  mBuf[1] = 0;
	  mLength = 1;
	  return;
   }

   bool neg = false;
   
   int value = val;
   if (value < 0)
   {
	  value = -value;
	  neg = true;
   }

   int c = 0;
   int v = value;
   while (v /= 10)
   {
	  c++;
   }

   if (neg)
   {
	  c++;
   }

   mLength = c+1;
   mCapacity = c+1;
   mBuf = new char[c+2];
   mBuf[c+1] = 0;
   
   v = value;
   while (v)
   {
	  mBuf[c--] = '0' + v%10;
	  v /= 10;
   }

   if (neg)
   {
	  mBuf[0] = '-';
   }
}

CData::CData(double value, int precision)
   : mLength(0), 
	 mBuf(0),
	 mCapacity(0)

{
   assert(precision < 10);

   double v = value;
   bool neg = (value < 0.0);
   
   if (neg)
   {
	  v = -v;
   }

   CData m((unsigned long)v);

   // remainder
   v = v - floor(v);

   int p = precision;
   while (p--)
   {
	  v *= 10;
   }

   int dec = (int)floor(v+0.5);

   CData d;

   if (dec == 0)
   {
	  d = "0";
   }
   else
   {
	  d.resize(precision);
	  d.mBuf[precision] = 0;
	  p = precision;
	  // neglect trailing zeros
	  bool significant = false;
	  while (p--)
	  {
		 if (dec % 10 || significant)
		 {
			significant = true;
			d.mLength++;
			d.mBuf[p] = '0' + (dec % 10);
		 }
		 else
		 {
			d.mBuf[p] = 0;
		 }
		 
		 dec /= 10;
	  }
   }

   if (neg)
   {
	  resize(m.length() + d.length() + 2);
	  mBuf[0] = '-';
	  memcpy(mBuf+1, m.mBuf, m.length());
	  mBuf[1+m.length()] = '.';
	  memcpy(mBuf+1+m.length()+1, d.mBuf, d.length()+1);
	  mLength = m.length() + d.length() + 2;
   }
   else
   {
	  resize(m.length() + d.length() + 1);
	  memcpy(mBuf, m.mBuf, m.length());
	  mBuf[m.length()] = '.';
	  memcpy(mBuf+m.length()+1, d.mBuf, d.length()+1);
	  mLength = m.length() + d.length() + 1;
   }
}

CData::CData(unsigned long value)
   : mLength(0), 
	 mBuf(0),
	 mCapacity(0)
{
   if (value == 0)
   {
	  mBuf = new char[2];
	  mBuf[0] = '0';
	  mBuf[1] = 0;
	  mLength = 1;
	  return;
   }

   int c = 0;
   unsigned long v = value;
   while (v /= 10)
   {
	  c++;
   }

   mLength = c+1;
   mCapacity = c+1;
   mBuf = new char[c+2];
   mBuf[c+1] = 0;
   
   v = value;
   while (v)
   {
	  mBuf[c--] = '0' + v%10;
	  v /= 10;
   }
}

CData::CData(char c)
   : mLength(1), 
	 mBuf(0),
	 mCapacity(mLength)
{
   mBuf = new char[2];
   mBuf[0] = c;
   mBuf[1] = 0;
}

CData::CData(bool value)
   : mLength(0), 
	 mBuf(0),
	 mCapacity(0)
{
   static char* truec = "true";
   static char* falsec = "false";

   if (value)
   {
	  mBuf = new char[5];
	  mLength = 4;
	  mCapacity = 4;
	  memcpy(mBuf, truec, 5);
   }
   else
   {
	  mBuf = new char[6];
	  mLength = 5;
	  mCapacity = 5;
	  memcpy(mBuf, falsec, 6);
   }
}

CData::CData(const CData& rhs) 
   : mLength(rhs.mLength),
	 mBuf(new char[mLength+1]), 
	 mCapacity(mLength)
{
   memcpy(mBuf, rhs.mBuf, mLength+1);
}

CData::~CData()
{
   delete[] mBuf;
}

CData&
CData::operator=(const char* str)
{
   assert(str);
   const int len = strlen(str);
   if (len > mCapacity)
   {
	  resize(len);
   }
   mLength = len;
   memcpy(mBuf, str, mLength+1);

   return *this;
}

CData&
CData::operator=( const CData& data ) 
{
   if (&data != this)
   {
	  if (data.length() > mCapacity)
	  {
		 resize(data.length());
	  }
	  mLength = data.length();
	  memcpy(mBuf, data.mBuf, data.mLength+1);
   }
   
   return *this;
}

const char*
CData::c_str() const
{
   return mBuf;
}

const char*
CData::getCData(char* buf, int len) const
{
   assert(len);
   strncpy(buf, mBuf, len-1);
   buf[len-1] = 0;
   return buf;
}

/*
const char*
CData::getCData(LocalScopeAllocator& lo) const
{
   lo.allocate(mLength+1);
   memcpy(lo, mBuf, mLength+1);
   return lo;
}
*/
char
CData::getChar(int i) const
{
   assert(i<mLength);
   return mBuf[i];
}

void
CData::setchar(int i, char c)
{
   if (i >= mLength)
   {
	  resize(i+1);
	  mLength = i+1;
	  memset(mBuf + mLength, 0, mCapacity-mLength+1);
   }
   
   assert(i<mLength);
   mBuf[i] = c;
}

char
CData::operator[](int i) const
{
   assert(i<mLength);
   return mBuf[i];
}

int
CData::length() const
    {                                
   return mLength;
}

void
CData::setBufferSize(int newCapacity)
{
   resize(newCapacity);
}

void
CData::resize(int newCapacity)
{
   if (newCapacity < mCapacity)
   {
	  return;
   }
   char* oldBuf = mBuf;
   mBuf = new char[newCapacity+1];
   for (int i = 0; i < mLength; i++)
   {
	  mBuf[i] = oldBuf[i];
   }
   mBuf[mLength] = 0;
   mCapacity = newCapacity;
   delete[] oldBuf;
}

bool
CData::operator==(const char* str) const
{
   assert(str);
   return strcmp(mBuf, str) == 0;
}

bool
CData::operator==( const CData& data) const
{
   return strcmp(mBuf, data.mBuf) == 0;
}

bool
CData::operator!=(const char* str) const
{
   assert(str);
   return strcmp(mBuf, str) != 0;
}

bool
CData::operator!=(const CData& data) const
{
   return strcmp(mBuf, data.mBuf) != 0;
}

bool
CData::operator>(const CData& data) const
{
   return strcmp(mBuf, data.mBuf) > 0;
}

bool
CData::operator<(const CData& data) const
{
   return strcmp(mBuf, data.mBuf) < 0;
}

bool
CData::operator>(const char* data) const
{
   return strcmp(mBuf, data) > 0;
}

bool
CData::operator<(const char* data) const
{
   return strcmp(mBuf, data) < 0;
}

int
CData::compare(const char* str, int length) const
{
   return strncmp(mBuf, str, length);
}

int
CData::compare(const CData& data) const
{
   return strncmp(mBuf, data.mBuf, data.length());
}

int
CData::compareNoCase(const char* str, int length) const
{
   return strncasecmp(mBuf, str, length);
}

int
CData::compareNoCase(const CData& data) const
{
   return strncasecmp(mBuf, data.mBuf, data.length());
}

CData
CData::operator+(const CData& data) const
{
   // reserve
   CData ret(mLength + data.mLength, true);
   memcpy(ret.mBuf, mBuf, mLength);
   memcpy(ret.mBuf+mLength, data.mBuf, data.mLength+1);
   ret.mLength = mLength + data.mLength;
   return ret;
}

CData
CData::operator+(const char* str) const
{
   assert(str);
   // reserve
   int l = strlen(str);
   CData ret(length() + l, true);
   memcpy(ret.mBuf, mBuf, mLength);
   memcpy(ret.mBuf+mLength, str, l+1);
   ret.mLength = mLength + l;
   return ret;
}


CData
CData::operator+(const char c) const
{
   // reserve
   CData ret(length() + 1, true);
   memcpy(ret.mBuf, mBuf, mLength);
   ret.mBuf[mLength] = c;
   ret.mBuf[mLength+1] = 0;
   ret.mLength = mLength + 1;
   return ret;
}


CData&
CData::operator+=(const CData& data)
{
   if (mCapacity < mLength + data.mLength)
   {
	  resize(mLength + data.mLength);
   }

   memcpy(mBuf+mLength, data.mBuf, data.mLength+1);
   mLength += data.mLength;
   return *this;
}

CData&
CData::operator+=(const char* str)
{
   assert(str);
   int l = strlen(str);
   if (mCapacity < mLength + l)
   {
	  resize(mLength + l);
   }

   memcpy(mBuf+mLength, str, l+1);
   mLength += l;
   return *this;
}


CData&
CData::operator+=(const char c)
{
   if (mCapacity < mLength + 1)
   {
	  resize(mLength + 1);
   }

   mBuf[mLength] = c;
   mBuf[mLength+1] = 0;
   mLength += 1;
   return *this;
}


CData&
CData::lowercase()
{
   char* p = mBuf;
   while ((*p = tolower(*p)) != 0)
   {
	  p++;
   }
   return *this;
}

CData&
CData::uppercase()
{
   char* p = mBuf;
   while ((*p = toupper(*p)) != 0)
   {
	  p++;
   }
   return *this;
}

int
CData::Characters_numeral_juge()
{
	CData tmp;
	tmp=mBuf;
   char* p = (char*)tmp.mBuf;
   while ((*p = toupper(*p)) != 0)
   {
		if(((*p>='a')&&(*p<='z'))||((*p>='A')&&(*p<='Z'))||((*p>='0')&&(*p<='9'))) 
		{
			p++;
		}
		else
		{
			return -1;
		}
   }
   return 0;
}

int CData::checkIsNumber()
{
	CData tmp;
	tmp=mBuf;
	char* p = (char*)tmp.mBuf;
	while ((*p = toupper(*p)) != 0)
	{
		if(((*p>='0')&&(*p<='9'))||*p=='.') 
		{
			p++;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}



void
CData::erase()
{
   mLength = 0;
   mBuf[0] = 0;
}

int CData::convertInt() const
{
   int val = 0;
   char* p = mBuf;
   int l = mLength;
   int s = 1;

   while (isspace(*p++))
   {
	  l--;
   }
   p--;
   
   if (*p == '-')
   {
	  s = -1;
	  p++;
	  l--;
   }
   
   while (l--)
   {
	  char c = *p++;
	  if ((c >= '0') && (c <= '9'))
	  {
		 val *= 10;
		 val += c - '0';
	  }
	  else
	  {
		 return s*val;
	  }
   }

   return s*val;
}

long CData::convertLong() const
{
   long val = 0;
   char* p = mBuf;
   int l = mLength;
   int s = 1;

   while (isspace(*p++))
   {
	  l--;
   }
   p--;

   if (*p == '-')
   {
	  s = -1;
	  p++;
	  l--;
   }
   
   while (l--)
   {
	  char c = *p++;
	  if ((c >= '0') && (c <= '9'))
	  {
		 val *= 10;
		 val += c - '0';
	  }
	  else
	  {
		 return s*val;
	  }
   }

   return s*val;
}

string CData::convertString() const
{
   return string(mBuf);
}

double CData::convertDouble() const
{
   long val = 0;
   char* p = mBuf;
   int s = 1;

   while (isspace(*p++));
   p--;
   
   if (*p == '-')
   {
	  s = -1;
	  p++;
   }
   
   while (isdigit(*p))
   {
	  val *= 10;
	  val += *p - '0';
	  p++;
   }

   if (*p == '.')
   {
	  p++;
	  long d = 0;
	  double div = 1.0;
	  while (isdigit(*p))
	  {
		 d *= 10;
		 d += *p - '0';
		 div *= 10;
		 p++;
	  }
	  return s*(val + d/div);
   }

   return s*val;
}

bool CData::isIn(char c, const char* match)
{
   char p;
   while((p = *match++))
   {
	  if (p == c)
	  {
		 return true;
	  }
   }
	
   return false;
}

CData 
CData::parse(const char* match, bool* matchFail )
{
   assert(match);
   
   int start = 0;
   bool foundAny = false;

   // find start
   while (!foundAny && (start < mLength))
   {
	  foundAny = isIn(mBuf[start++], match);
   }

   // find end
   if (foundAny)
   {
	  int end = --start;
	  while (end < mLength &&
			 isIn(mBuf[end], match))
	  {
	end++;
	  }
	  //	  end--;

	  CData result(mBuf, start);

	  memcpy(mBuf, mBuf + end, mLength - end + 1);
	  mLength = mLength - end;
	  mBuf[mLength] = '\0';

	  assert(mLength >= 0);
	  if (matchFail)
	  {
		 *matchFail = false;
	  }
	  return result;
   }
   else
   {
	  if (matchFail)
	  {
		 *matchFail = true;
	  }
	  CData result;//(0, true);
	  return result;
   }
}

CData 
CData::getLine(bool* matchFail )
{
   const int STARTING = 0;
   const int HAS_CR = 1;
   const int HAS_LF = 2;
   const int HAS_CRLF = 3;

   int state = STARTING;
   int pos = 0;

   bool foundAny = false;
   while (!foundAny && (pos < mLength))
   {
	  char p = mBuf[pos];
	  switch(p)
	  {
		 case '\r':
			state = HAS_CR;
			break;
		 case '\n':
			if (state == HAS_CR)
			{
			   state = HAS_CRLF;
			}
			else
			{
			   state = HAS_LF;
			}
			foundAny = true;
			break;
		 default:
			state = STARTING;
	  }
	  pos++;
   }

   int pos2 = pos;

   if(state == HAS_CRLF)
   {
	  pos--;
   }

   if (foundAny)
   {
	  CData result(mBuf, pos-1);
	   
	  memcpy(mBuf, mBuf+pos2, mLength-pos2+1);
	  mLength -= pos2;

	  if (matchFail)
	  {
		 *matchFail = false;
	  }
	  return result;
   }
   else
   {
	  if (matchFail)
	  {
		 *matchFail = true;
	  }
	  CData result;
	  return result;
   }
}

CData 
CData::substring(int first, int last) const
{
   if (last == -1 ||
	   last == CDATA_NPOS)
   {
	  last = mLength;
   }
   assert(first <= last);
   assert(first >= 0);
   assert(last <= mLength);
   return CData(mBuf+first, last-first);
}


int
CData::find(const CData& match, int start) const
{
   assert(start >= 0);
   assert(start <= mLength);
   char* pos = std::search(mBuf+start, mBuf+mLength,
						   match.mBuf, match.mBuf + match.mLength);
   if (pos == mBuf+mLength)
   {
	  if (match.mLength == 0)
	  {
		 if (mLength == 0)
		 {
			return CDATA_NPOS;
		 }
		 return 0;
	  }
	  return CDATA_NPOS;
   }
   return pos - mBuf;
}

int
CData::find_first_of(CData find, int start) const
{
   string f = find.convertString();
   string me = this->convertString();
   string::size_type p = me.find_first_of(f, start);
   if (p == string::npos)
   {
	  return CDATA_NPOS;
   }
   else
   {
	  return p;
   }
}

int
CData::find_first_not_of(CData find, int start) const
{
   string f = find.convertString();
   string me = this->convertString();
   string::size_type p = me.find_first_not_of(f, start);
   if (p == string::npos)
   {
	  return CDATA_NPOS;
   }
   else
   {
	  return p;
   }
}

int
CData::find_last_of(CData find, int start) const
{
   string f = find.convertString();
   string me = this->convertString();
   string::size_type p = me.find_last_of(f, start);
   if (p == string::npos)
   {
	  return CDATA_NPOS;
   }
   else
   {
	  return p;
   }
}

int
CData::find_last_not_of(CData find, int start) const
{
   string f = find.convertString();
   string me = this->convertString();
   string::size_type p = me.find_last_not_of(f, start);
   if (p == string::npos)
   {
	  return CDATA_NPOS;
   }
   else
   {
	  return p;
   }
}

int
CData::find(const char* match, int start) const
{
   assert(start >= 0);
   assert(start <= mLength);
   const int matchLength = strlen(match);

   char* m = const_cast<char*>(match);
   char* b = const_cast<char*>(mBuf);
   char* pos = std::search(b+start, b+mLength,
						   m, m+matchLength);
   assert(pos <= b+mLength);
   assert(pos >= mBuf);
   if (pos == b+mLength)
   {
	  if (matchLength == 0)
	  {
		 if (mLength == 0)
		 {
			return CDATA_NPOS;
		 }
		 return 0;
	  }
	  return CDATA_NPOS;
   }
   return pos - mBuf;
}

CData&
CData::removeSpaces()
{
   int firstChar = 0;
 
   while ((firstChar < mLength) && 
		  mBuf[firstChar] == ' ')
   {
	  firstChar++;
   }
 
   int lastChar = mLength-1;
   while ((lastChar > 0) && (mBuf[lastChar] == ' '))
   {
	  lastChar--;
   }
   
   if (firstChar > lastChar)
   {
	  erase();
   }		  
   else
   {
	  memcpy(mBuf, mBuf+firstChar, lastChar-firstChar+1);
	  mLength = lastChar-firstChar+1;
	  mBuf[mLength] = 0;
   }
   return *this;
}
	
// the functionality of this method has changed from the implementation in
// CopyOnWrite. It now looks for folded lines which mean a CRLF followed by
// whitespace and replaces the CRLF with two spaces (' '). This should allow the
// parser to continue since lines will no longer be folded 
CData&
CData::removeLWS()
{
   char* p = mBuf;
   char* end = mBuf + mLength - 2;
   while (p != 0 && p < end)
   {
	  // look for CRLF + ' ' or '\t' since this indicates line folding
	  // will not go past end, since && will short circuit when == 0
	  if (*p == '\r' && *(p+1) == '\n' && (*(p+2) == ' ' || (*(p+2) == '\t')))
	  {
		 *p++ = ' '; // replace \r with ' '
		 *p++ = ' ';// replace \n with ' '
	  }
	  else
	  {
		 p++;
	  }
   }

   return *this;
}


CData& 
CData::replace(int startPos, int numElements, const CData& replacement)
{
   assert((startPos + numElements) <= mLength);
   return replace(startPos, startPos + numElements, replacement.c_str(), replacement.size());
}

 
CData& 
CData::replace_all(CData&   str,const   CData&   old_value,const   CData&   new_value)   
{   
    while(true)   {   
        string::size_type   pos(0);   
        pos=str.find(old_value);
        if( (pos != CDATA_NPOS)&& (pos !=string::npos))      
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}   




CData&
CData::replace(int startPos, int endPos, const char* replaceStr, int replaceLength)
{
   assert(replaceStr);
   assert(startPos < endPos);
   assert(endPos <= mLength);

   resize(startPos + replaceLength + (mLength - endPos));

   memcpy(mBuf + startPos + replaceLength, 
		  mBuf + endPos, mLength-endPos + 1);
   
   memcpy(mBuf + startPos, replaceStr, replaceLength);
   mLength = startPos + replaceLength + (mLength - endPos);
   return *this;
}


//ostream&
//operator<<(ostream& s, const CData& data)
//{
//   s << data.mBuf;
//   return s;
//}

};

