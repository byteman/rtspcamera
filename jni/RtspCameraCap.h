#pragma once
#include <string>
#include <vector>
#include "Core_Thread.h"
#include "Core_Event.h"
#include "libavformat/avformat.h"

using namespace std;

struct media_packt
{
	media_packt(unsigned char* buffer, int size);
	~media_packt();
	unsigned char* pdata;
	int   size;
	int type;
	int width;
	int height;
};
class RtspCameraCap;
class Detect:public CORE::Runnable
{
public:
	Detect();
	virtual ~Detect(void);
	bool start(RtspCameraCap* pCamera,std::string rtsp_url);
	bool stop();
	virtual void run(void);
private:
	CORE::Core_Thread _thread;
	std::string rtsp_url;
	bool _decExit;
	CORE::Core_Event _evtReady;
	RtspCameraCap* _pCamera;
};
typedef int (*SCCallbackType)(int id,int msg);

class RtspCameraCap:public CORE::Runnable
{
public:
	RtspCameraCap(void);
	virtual ~RtspCameraCap(void);

	bool start();
	bool stop();
	void setCallBack(SCCallbackType callback);
	static int init ();
	virtual void run();
	media_packt* getimage(int timeout);
	int open(const char* psDevName ,int timeout_ms,bool auto_reconnect);
	bool isstop() {return _bExit;}
	int istimeout();
	int _width;  //流的宽度
	int _height;  //流的高度
	int _format;  //编码格式H264，mpeg4
private:
	AVFormatContext	*pFormatCtx_Video;
	CORE::Core_Thread _thread;
	CORE::Core_FastMutex _mutex;
	int stream_index;
	bool _bExit;
	int timeout;
	CORE::Core_Event _evtExit;
	CORE::Core_Event _evtReady;
	CORE::Core_Event _evtData;
	SCCallbackType   _callback;
	Detect dector;
};



