#ifndef DATA_H_
#define DATA_H_

#include <string>
#include "Core.h"
#pragma warning(disable : 4786)
#ifndef _WIN32
#define INT_MAX 1024
#endif
const int CDATA_NPOS = INT_MAX;
/**
 * @file Data.h
 * Data.h 2006:9:11 19:13:24o
 * @version 1.0
 *  
 * 
 *
 *
 *
 * @note
 */
/** 
	CData represents binary data and strings in a thread-safe manner.
	This version is implemented as copy on copy buffers.  note that
	the implementation of std::string in gcc prior to 3.1 is
	<b>not</b> thread-safe -- thus, CData is the only thread-safe
	option you may have.
	<p>
	<b>EXAMPLES</b>
	<p>
	<PRE>
	  CData x;
	  x = "asdf"; // set string to asdf
	  x = "1024"; // set string to 1024
	  x.convertInt() == 1024;  // this is true
	  x.convertString() == string("1024"); // this is also true

	  x += "a";  // now the string is 1024a

	  x += "b";  // now it's 1024ab

	  LocalScopeAllocator lo;
	  printf("%s\n", x.getCData(lo)); // this is how you get a char&#42; from a CData

	  CData buf = "1000,1001,1002";

	  bool matchFail;
	  CData tok = buf.parse(",", &matchfail);

	  // now, tok == "1000", buf == "1001,1002", matchfail == false

	  tok = buf.parse(",", &matchfail);

	  // at this point tok == "1001", buf == "1002", matchfail == false

	  tok = buf.parse(",", &matchfail);

	  // now, there is no , in buf (value: "1002" ) so the result is 
	  // tok == "", buf == "1002", matchfaile == true

	</PRE>

 
 */

namespace CORE {

class CORE_API CData
{
   public:
	  /// "no position" -- used to indicate non-position when returning a value
 //	 static const int npos;

	  ///
	  CData();
	  ///
	  CData( const char* str );
	  ///
	  CData( const char* buffer, int length );
	  ///
	  CData( const CData& data );
	  ///
	  CData( const std::string& str);
	  ///
	  explicit CData(int value);
	  ///
	  explicit CData(unsigned long value);
	  ///
	  explicit CData(double value, int precision = 4);
	  ///
	  explicit CData(bool value);
	  ///
	  explicit CData(char c);
	  
	  ///
	  ~CData();

	  ///
	  bool operator>(const CData& ) const ;
	  ///
	  bool operator<(const CData& ) const;
	  ///
	  bool operator>(const char* ) const ;
	  ///
	  bool operator<(const char* ) const;
	  ///
	  CData& operator=(const char* str);
	  ///
	  CData& operator=(const CData& data);
	  ///
	  bool operator==( const char* str ) const;
	  ///
	  bool operator==( const CData& data ) const;
	  ///
	  bool operator!=( const char* str ) const;
	  ///
	  bool operator!=( const CData& data ) const;
 //	 friend bool operator==( const char* str, const CData& d );
 //	 friend bool operator!=( const char* str, const CData& d );

	/** 
		return a c-style string from a CData. 

		<P>
		The copy is made in the passed-in buffer.

		@param buf   pointer to buffer to copy to 
		@param len   length of buf

		@return a c-style string
	*/

	  const char* getCData(char* buf, int len) const;

	/** 
		@deprecated
		return a c-style string from a CData. 
		<p>

		this function returns a pointer to the internal buffer, so
		it is not threadsafe.  That is, a simultaneous read and
		write can cause undefined behavior in the reader.

		@return a c-style string
	*/
	  const char* c_str() const; // return C style string

	  ///
	  char getChar(int i) const;
	  ///
	  void setchar(int i, char c);
	  ///
	  void setChar(int i, char c) { setchar(i, c); }
		
	  /// 
	  char operator[](int i) const;
	  ///returns the length of the contained data
	  int length() const;
	  ///
	  int size() const {return length();}
	  ///
	  bool empty() const {return size()==0;}

	/** Get the holding capacity of the object, which can be more
	 * then the actual length of the string. */

	  int capacity() const { return mCapacity; }
	  ///Pre-allocate buffer of the given size
	  void setBufferSize(int size); // suggest a size

	  ///
	  int compare(const char* str, int length) const;
	  ///
	  int compare(const CData& data) const;
	  ///
	  int compareNoCase(const char* str, int length) const;
	  ///
	  int compareNoCase(const CData& data) const;

	  ///
	  CData operator+( const CData& data) const;
	  ///
	  CData operator+( const char* str) const;
	  ///
	  CData operator+( char c) const;
	  ///
	  CData& operator+=(const CData& d);
	  ///
	  CData& operator+=(const char*);
	  ///
	  CData& operator+=(const char c);

	  ///
	  void erase();
	  ///
	  CData& lowercase();
	  ///
	  CData& uppercase();
	  ///判断是否是字母和数字
	  int Characters_numeral_juge();

	   ///判断是否是数字
	  int checkIsNumber(); 


	  ///
	  std::string convertString() const;
	  ///
	  int convertInt() const;
	  ///
	  long convertLong() const;
	  ///
	  double convertDouble() const;

	  /** 
		  match (and eat) the first contiguous block of matched
		  characters.

	  @param match   the characters to be matched.

	  @param matchFail a pointer to a bool.  set true if no
	  characters in match can be found, false if any characters in
	  match are found.  If matchFail is NULL, it is ignored.

	  @return the data prior to the first block of matching
	  characters.  if no match, returns "".

	  @see #matchChar
	  */
	  CData parse(const char* match, bool* matchFail = 0 );
	  /** get the next line in the text, delimited by \r\n or \n .

		  Differs from parse("\r\n", matchFail) in that if there is
		  a blank line (which has the contiguous text \r\n\r\n),
		  parse will merely skip the empty line, while getLine will
		  return the empty line as an empty CData.

	  @param matchFail if it is not NULL, it will be set to true
	  if the match fails, or false otherwise.

	  @return the next line in the text.

	  */
	  CData getLine(bool* matchFail = 0 );

	  /// removes spaces before and after a string.
	  CData& removeSpaces();
	  /// remove leading white space.
	  CData& removeLWS();

	   /** returns a substring of this object
		  @param first	  the first character to be part of the substring
		  @param last	   the last character of the substring, or -1 to 
		  mean the last character overall
		  thus, x.substring(0, -1) == x.
	  */
	  CData substring(int first, int last = -1) const;
	  /** returns a substring of this object
		  @param first	  the first character to be part of the substring
		  @param count	  the number of characters to be part of the substring
	  */
	  CData substr(int first, int count = CDATA_NPOS) const 
	  {
		 return substring(first, (count == CDATA_NPOS 
								  ? -1
								  : first+count));
	  }

	  /// find a string in the object
	  int find(const CData& match, int start = 0) const;

	  /// find a string in the object
	  int find(const char* match, int start = 0) const;

	  /**Searches beginning at pos, for the first character that is 
		*equal to any character within find.
		*/
	  int find_first_of(CData find, int start = 0) const;

	  /**Searches beginning at pos, for the first character that is not 
		*equal to any character within find
		*/
	  int find_first_not_of(CData find, int start = 0) const;

	  ///Same as find_first_of() except searches backword 
	  int find_last_of(CData find, int start = 0) const;
	  ///Same as find_first_not_of() except searches backword 
	  int find_last_not_of(CData find, int start = 0) const;

	  CData& replace(int startPos, int numElements, const CData& replacement);

	  CData& replace_all(CData&   str,const   CData&   old_value,const   CData&   new_value)   ;

	  static bool isIn(char c,const char *match);

//	 friend std::ostream& operator<<(std::ostream& s, const CData& data);
	CData(int capacity, bool); // for internal use

   private:
	  
	  void resize(int s);
	  
	  CData& replace(int startpos, int endpos, const char* replaceStr, int replaceLength);

	  int mLength;
	  char* mBuf;
	  int mCapacity;
};



#ifdef WIN32
inline
CData
operator+(const char* c, const CData& d)
{
   return CData(c) + d;
}
#endif

};

#endif
