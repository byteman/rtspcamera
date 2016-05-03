#include "LogConsoleChannel.h"
#include "DateTime.h"
#include <sstream>

#define _DEBUG_LOG_ 0
#define _DEBUG_LOG_PATH_ "./run.log"
#ifdef _DEBUG_LOG_
#include <sys/stat.h>  
#define  FILE_MAX_LEN 1024*1024*2
#endif
namespace CORE {


LogConsoleChannel::LogConsoleChannel() 
{

}


LogConsoleChannel::~LogConsoleChannel()
{

}


const char* LogConsoleChannel::name() const
{
	return "NetConsole";
}


void LogConsoleChannel::log(const char *msg, const char *prio)
{
	std::ostringstream str;
	
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	DateTime::getLocalTime(year, month, day, hour, minute, second);
	
	str<<"["<<prio<<"] "<<year<<"-"<<month<<"-"<<day<<" "<<hour<<":"<<minute<<":"<<second<<" "<<msg;
	//printf(str.str().c_str());

    #ifdef _DEBUG_LOG_
	int iFileSize = 0;
	struct stat f_stat;
	FILE* pFile = NULL;
    if( stat( _DEBUG_LOG_PATH_, &f_stat ) != -1 )
	{
		iFileSize =  (long)f_stat.st_size;
	}

	if (iFileSize > FILE_MAX_LEN)
	{
        pFile = fopen(_DEBUG_LOG_PATH_, "w+");
	} 
	else
	{
        pFile = fopen(_DEBUG_LOG_PATH_, "a+");
	}

	if(NULL != pFile)
	{
		// 写日志至文件
		fprintf(pFile, "%s", str.str().c_str());
		//int ret = fwrite(str.str().c_str(),1,str.str().length(),pFile );
		fclose(pFile);
	}
	#endif


	if(true==m_isSend)
	{
		try
		{
			m_udp.sendTo(str.str().c_str(), strlen(str.str().c_str()), m_remoteAddr, 0);
		}
		catch(...)
		{
			m_isSend=false;
			printf("@@@@@@@@ Remote log send error! @@@@@@@@\n");
		}
	}
	str.str("");
}


void LogConsoleChannel::open()
{

}
		

void LogConsoleChannel::close()
{

}
		

void LogConsoleChannel::setProperty(char* name, char* value)
{
	if(strcmp(name, "addr")==0)
	{
		m_remoteAddr=SocketAddress(value);
		m_isSend=true;
	}
	else
	{
		m_isSend=false;	
	}
}


CData LogConsoleChannel::getProperty(char* name) const
{
	CData tmp;
	return tmp;
}
	

}


