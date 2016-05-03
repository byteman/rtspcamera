#ifndef Inifile_H
#define Inifile_H


#include "Core.h"
#include "Core_Mutex.h"


namespace CORE  {


#define MAX_FILE_SIZE (5 * 1024)


/// Class Inifile
/// Reads and writes ini file
class CORE_API Inifile
{
public:
	
	Inifile();
	
	virtual ~Inifile();
public:
		/// Reads "key" from ini file
	 Int32 read_profile_string( const char *section, const char *key,char *value, 
		 Int32 size,const char *file);

		 /// Write "key" to ini file
	 Int32 write_profile_string(const char *section, const char *key,
					const char *value, const char *file);

	 void setReload(bool isReload);
	 
private:
	Int32 load_ini_file(const char *file, char *buf,Int32 *file_size);
	
	Int32 isnewline(char c);
	
	Int32 isend(char c);
	
	Int32 isleftbarce(char c);
	
	Int32 isrightbrace(char c );
	
	 Int32 parse_file(const char *section, const char *key, const char *buf,Int32 *sec_s,Int32 *sec_e,
					  Int32 *key_s,Int32 *key_e, Int32 *value_s, Int32 *value_e);

public:

	static Inifile& instance();
	
private:
	mutable Core_FastMutex  _mutex;
	char m_buf[MAX_FILE_SIZE];
	Int32 m_file_size;
	bool m_isReload;
	bool m_isLoad;
};


 }

#endif 

