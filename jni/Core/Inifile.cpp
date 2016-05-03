#include "Inifile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SingletonHolder.h"


namespace CORE {


#define LEFT_BRACE '['
#define RIGHT_BRACE ']'


Inifile::Inifile():
	m_isReload(true),
	m_isLoad(false)
{

	memset(m_buf,0x00,MAX_FILE_SIZE);

}


Inifile::~Inifile()
{



}


Int32 Inifile::load_ini_file(const char *file, char *buf,Int32 *file_size)
{
	FILE *in = NULL;
	Int32 i=0;
	*file_size =0;

	in = fopen(file,"r");
	printf("Inifile::load_ini_file\n");
	
	if( 0 == in) {
		return 0;
	}
	buf[i]=fgetc(in);
		
	//load initialization file
	while( buf[i]!= (char)EOF) {
		i++;
		buf[i]=fgetc(in);
	}
		
	buf[i]='\0';
	*file_size = i;

	fclose(in);
	return 1;
}


Int32 Inifile::isnewline(char c)
{
	return ('\n' == c ||  '\r' == c )? 1 : 0;
}


Int32 Inifile::isend(char c)
{
	return '\0'==c? 1 : 0;
}


Int32 Inifile::isleftbarce(char c)
{
	return LEFT_BRACE == c? 1 : 0;
}


Int32 Inifile::isrightbrace(char c )
{
	return RIGHT_BRACE == c? 1 : 0;
}


 Int32 Inifile::parse_file(const char *section, const char *key, const char *buf,Int32 *sec_s,Int32 *sec_e,
					  Int32 *key_s,Int32 *key_e, Int32 *value_s, Int32 *value_e)
{
	const char *p = buf;
	Int32 i=0;

	*sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

	while( !isend(p[i]) ) 
	{
		//find the section
		if( ( 0==i ||  isnewline(p[i-1]) ) && isleftbarce(p[i]) )
		{
			Int32 section_start=i+1;

			//find the ']'
			do {
				i++;
			} while( !isrightbrace(p[i]) && !isend(p[i]));

			if( 0 == strncmp(p+section_start,section, i-section_start)&&(i-section_start)==strlen(section)) 
			{
				Int32 newline_start=0;

				i++;

				//Skip over space char after ']'
				while(isspace(p[i])) {
					i++;
				}

				//find the section
				*sec_s = section_start;
				*sec_e = i;

				while( ! (isnewline(p[i-1]) && isleftbarce(p[i])) 
				&& !isend(p[i]) ) 
				{
					Int32 j=0;
					//get a new line
					newline_start = i;

					while( !isnewline(p[i]) &&  !isend(p[i]) ) 
					{
						i++;
					}
					
					//now i  is equal to end of the line
					j = newline_start;

					if(';' != p[j]) //skip over comment
					{
						while(j < i && p[j]!='=') {
							j++;
							if('=' == p[j]) {
								if(strncmp(key,p+newline_start,j-newline_start)==0&&(j-newline_start)==strlen(key))
								{
									//find the key ok
									*key_s = newline_start;
									*key_e = j-1;

									*value_s = j+1;
									*value_e = i;

									return 1;
								}
							}
						}
					}

					i++;
				}
			}
		}
		else
		{
			i++;
		}
	}
	return 0;
}


Int32 Inifile::read_profile_string( const char *section, const char *key,char *value, 
		 Int32 size, const char *file)
{
	Int32 sec_s,sec_e,key_s,key_e, value_s, value_e;

	// Lock
	Core_FastMutex::ScopedLock lock(_mutex);

	if(false==m_isLoad)
	{
		m_isLoad=true;
		if(!load_ini_file(file,m_buf,&m_file_size))
		{
			return -1;
		}
	}
	else
	{
		if(true==m_isReload)
		{
			if(!load_ini_file(file,m_buf,&m_file_size))
			{
				return -1;
			}
		}
	}
	
	if(!parse_file(section,key,m_buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e))
	{
		return -1;  
	}
	else
	{
		Int32 cpcount = value_e -value_s;

		if( size-1 < cpcount)
		{
			cpcount =  size-1;
		}
	
		memset(value, 0, size);
		memcpy(value,m_buf+value_s, cpcount );
		value[cpcount] = '\0';
		
		return 0;
	}
}


void Inifile::setReload(bool isReload)
{
	m_isReload=isReload;
}

Int32 Inifile::write_profile_string(const char *section, const char *key,
					const char *value, const char *file)
{
	char w_buf[MAX_FILE_SIZE]={0};
	Int32 sec_s=0,sec_e,key_s,key_e, value_s, value_e;
	Int32 value_len = (Int32)strlen(value);
	FILE *out;

	// Lock
	Core_FastMutex::ScopedLock lock(_mutex);

	if(false==m_isLoad)
	{
		m_isLoad=true;
		if(!load_ini_file(file,m_buf,&m_file_size))
		{
			sec_s = -1;
		}
	}
	else
	{
		if(true==m_isReload)
		{
			if(!load_ini_file(file,m_buf,&m_file_size))
			{
				sec_s = -1;
			}
		}
	}

	if(sec_s != -1)
	{
		parse_file(section,key,m_buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e);
	}

	if( -1 == sec_s)
	{
		
		if(0==m_file_size)
		{
			sprintf(w_buf+m_file_size,"[%s]\n%s=%s\n",section,key,value);
		}
		else
		{
			//not find the section, then add the new section at end of the file
			memcpy(w_buf,m_buf,m_file_size);
			sprintf(w_buf+m_file_size,"\n[%s]\n%s=%s\n",section,key,value);
		}
	}
	else if(-1 == key_s)
	{
		//not find the key, then add the new key & value at end of the section
		memcpy(w_buf,m_buf,sec_e);
		sprintf(w_buf+sec_e,"%s=%s\n",key,value);
		sprintf(w_buf+sec_e+strlen(key)+strlen(value)+2,m_buf+sec_e, m_file_size - sec_e);
	}
	else
	{
		//update value with new value
		memcpy(w_buf,m_buf,value_s);
		memcpy(w_buf+value_s,value, value_len);
		memcpy(w_buf+value_s+value_len, m_buf+value_e, m_file_size - value_e);
	}
	
	out = fopen(file,"w");
	if(NULL == out)
	{
		return -1;
	}
	
	if(-1 == fputs(w_buf,out) )
	{
		fclose(out);
		return 0;
	}

	fclose(out);

	strcpy(m_buf, w_buf);
	m_file_size=strlen(w_buf);
	return 0;
}


Inifile& Inifile::instance()
{
	static CORE::SingletonHolder<Inifile> sh;
	return *sh.get();
}


}

