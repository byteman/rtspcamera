#ifdef	__cplusplus
extern "C"
{
#endif
#include "libavformat/avformat.h"
#ifdef __cplusplus
};
#endif
#include "RtspCameraCap.h"
#include "Utils.h"

RtspCameraCap cam;

class Detect:public CORE::Runnable
{
public:
	Detect(void);
	virtual ~Detect(void);
	bool start(std::string rtsp_url);
	bool stop();
	virtual void run(void);
private:
	CORE::Core_Thread _thread;
	std::string rtsp_url;
};

Detect::Detect(void)
{

}

Detect::~Detect(void)
{

}

bool Detect::start(std::string rtsp_url_addr)
{
    rtsp_url = rtsp_url_addr;
	_thread.start(*this);
}

bool Detect::stop()
{

}

void Detect::run(void)
{
	const char* psDevName;
	//Sleep(5000);
	while(1)
	{
	
		/*if(cam._bExit == true)
		{
			if(cam.isstop())
			{
				cam.open(rtsp_url.c_str(),0,0,0);
			}
			Sleep(10000);
		}*/
		if(cam.isstop())
		{
			cam.open(rtsp_url.c_str(),0,0,0);
		}
		Sleep(10000);
	}
	
}