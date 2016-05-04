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
	bool _tcp;
	int _timeout;
};

Detect::Detect(void):
	timeout(2000),
		tcp(false)
{

}

Detect::~Detect(void)
{

}

bool Detect::start(std::string rtsp_url_addr,int timeout,bool tcp)
{
    rtsp_url = rtsp_url_addr;
	_tcp = tcp;
	_timeout = timeout;
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
			cam.open(rtsp_url.c_str(),_timeout,true,_tcp);
		}
		Sleep(10000);
	}
	
}