#ifndef CBase64_H
#define CBase64_H


#include "Core.h"
#include <string>


namespace CORE {
	

#if 0
	FILE *file;
	file=fopen("1.jpg","rb");

	char buf[1024*32];
	int len;
	len=fread(buf,1,1024*32,file);
	fclose(file);
	
	std::string str;
	str=CORE::CBase64::Encode((const unsigned char * )buf,len);
	
	std::string out;
	out=CORE::CBase64::Decode(str.c_str(),str.size(),len);

	file=fopen("dst.jpg","wb");
	fwrite(out.c_str(),1,out.size(),file);
	fclose(file);
#endif

class CORE_API CBase64  
{
public:

	static std::string encode(const std::string in_str);

	static std::string decode(const std::string in_str);
	
	static std::string Encode(const unsigned char* Data,int DataByte);

	static std::string Decode(const char* Data,int DataByte,int& OutByte);

};


}


#endif 


