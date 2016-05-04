#ifdef	__cplusplus
extern "C"
{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
//#include "libavdevice/avdevice.h"
//#include "libavutil/audio_fifo.h"
//#include "libavformat/avio.h"
#include "libswresample/swresample.h"
#include "libavfilter/avfiltergraph.h" 
#include "libavfilter/buffersink.h"  
#include "libavfilter/buffersrc.h" 
//#include "libavutil/pixdesc.h"


#ifdef __cplusplus
};
#endif

#include "RtspCameraCap.h"
#include "Utils.h"

#include <queue>
#include <list>



media_packt::media_packt(unsigned char* buffer, int len):
	pdata(NULL),size(0)
{
	 if(len == 0) return;
     pdata = new unsigned char[len];
	 memcpy(pdata,buffer,len);
     size = len;
}
media_packt::~media_packt()
{
	if(pdata)
	{
		delete []pdata;
		pdata = NULL;
		size = 0;
	}
}
//static queue<media_packt*> q; //声明队列
static std::list<media_packt*> q;
static int init_ffmpeg_env(int enable_log,std::string path)
{

#ifdef WIN32
	std::string log_cfg = path+"\\record.ini";
	std::string log_out = path+"\\"+get_log_filename();
#else
	std::string log_cfg = path+"/record.ini";
	std::string log_out = path+"/"+get_log_filename();
#endif

	return init_report_file(log_cfg,log_out);

}
RtspCameraCap::RtspCameraCap(void):
	pFormatCtx_Video(NULL),
		timeout(0),
		_callback(NULL),
		_pause(false)
{
	_bExit = true;
}


RtspCameraCap::~RtspCameraCap(void)
{

}

int RtspCameraCap::istimeout()
{
	timeout++;
	if(timeout > 40) {
		timeout=0;
		return 1; //这个就是超时的返回
	}
	else
		return 0;
}
static int AVInterruptCallBackFun(void *arg)
{
	RtspCameraCap *Camera = (RtspCameraCap*)arg;
	if(Camera == NULL)return 0;
	return Camera->istimeout();
}
int RtspCameraCap::init ()
{
	av_register_all();
	avformat_network_init();
	init_ffmpeg_env(0,"/sdcard");
}
#define EXIT1(n) avformat_free_context(pFormatCtx_Video);pFormatCtx_Video = NULL;return n;

#define EXIT2(n) avformat_close_input(&pFormatCtx_Video);avformat_free_context(pFormatCtx_Video);pFormatCtx_Video = NULL;return n;

void RtspCameraCap::setCallBack(SCCallbackType callback)
{
	_callback = callback;

}
int RtspCameraCap::connect(const char* psDevName ,int timeout_ms,bool auto_reconnect,bool tcp )
{
	LOGE("begin connect ");

	//init();

	if(auto_reconnect)
	{
		dector.start(this,psDevName,timeout_ms,tcp);
	}
	return 0;
}

int RtspCameraCap::pause(bool bPause)
{
	_pause = bPause;
}

int RtspCameraCap::open(const char* psDevName,int timeout_ms,bool auto_reconnect,bool tcp)

{
	int err = 0;

	if(_bExit==false) //如果已经启动了连接.
	{
		
	}
	if(pFormatCtx_Video!=NULL)
	{
		LOGE("pFormatCtx_Video!=NULL");
		return -7;
	}
	LOGE("open -- %d",__LINE__);
	pFormatCtx_Video = avformat_alloc_context();

	if(pFormatCtx_Video == NULL)
	{
		LOGE("avformat_alloc_context failed");
		return -6;
	}
	AVDictionary *avdic=NULL;
	if(tcp)
	{
		char option_key[]="rtsp_transport";
		char option_value[]="tcp";							//udp会丢包导致花屏
		av_dict_set(&avdic,option_key,option_value,0);

	}

	av_dict_set_int(&avdic, "stimeout", timeout_ms*1000, 0);			//设置avformat_open_input的超时时间

	int result=avformat_open_input(&pFormatCtx_Video, psDevName,NULL,&avdic);
	if (result < 0){
		LOGE("Couldn't connect %s",psDevName);
		EXIT1(-1)
	}

	pFormatCtx_Video->probesize = 10 *1024;
	pFormatCtx_Video->max_analyze_duration = 2 * AV_TIME_BASE;

	if(avformat_find_stream_info(pFormatCtx_Video,NULL)<0)
	{
		LOGE("Couldn't find stream information.\n");
		EXIT2(-2)
	}
	LOGE("find best stream.....\n");
	stream_index = av_find_best_stream(pFormatCtx_Video, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	if (stream_index < 0) {
		LOGE("Can't find video stream in input file\n");
		EXIT2(-3)
	}
	LOGE("find streams[stream_index]\n");
	if (pFormatCtx_Video->streams[stream_index]->codec->codec_type != AVMEDIA_TYPE_VIDEO)
	{
		LOGE("Couldn't find video stream information\n");
		EXIT2(-4)
	}
	AVCodecContext* pAVCodecContext = pFormatCtx_Video->streams[stream_index]->codec; 
	if(pAVCodecContext == NULL)
	{
		LOGE("NULL stream index[%d]\n",stream_index);
		EXIT2(-5)
	}
    _width =  pAVCodecContext->width;
    _height = pAVCodecContext->height;
	if( pAVCodecContext->codec_id == AV_CODEC_ID_H264){
		_format = 1;
	}
	else if( pAVCodecContext->codec_id == AV_CODEC_ID_MPEG4){
		_format = 2;
	}
	else _format = 0;
	LOGE("width = %d,height = %d,open-- %d",_width,_height,_format,__LINE__);
	if(_callback)
	{
		LOGE(" connect ok callback");
		_callback(1,0);
	}
	LOGE("thread start....\n");
	start();
	LOGE("thread start....ok\n");

	return 0;

}


bool RtspCameraCap::start()
{
	_thread.start(*this);
	_evtReady.wait();
	return true;
}
bool RtspCameraCap::stop()
{
	if(_bExit)
	{
		return true;
	}
	_bExit = true;
	_evtExit.wait();
	dector.stop();
	return true;
}

void RtspCameraCap::run()
{
	LOGE("open -- %d",__LINE__);
	AVPacket packet;
	int err = 0;
	av_init_packet(&packet);
	_evtReady.set();
	_bExit = false;
	LOGE("open -- %d",__LINE__);
	pFormatCtx_Video->interrupt_callback.callback = AVInterruptCallBackFun;  
	pFormatCtx_Video->interrupt_callback.opaque = this;  
	
	while(!_bExit)
	{
		packet.data = NULL;
		packet.size = 0;
		//LOGE("open -- %d",__LINE__);
		err = av_read_frame(pFormatCtx_Video, &packet);
		if (err < 0)
		{
			_bExit = true;
			LOGE("av_read_frame err=%d thread exit",err);
			break;
			//continue;
		}
		timeout = 0;
		if(_pause)
		{
			continue;
		}
#if 1
		if(packet.stream_index == stream_index)
		{
			CORE::Core_FastMutex::ScopedLock lock(_mutex);
			#if 1
			if(q.size() > 25)
			{
				media_packt* discard_pkt = q.front();
				q.pop_front();
				if(discard_pkt != NULL)
					delete discard_pkt;
				discard_pkt = NULL;
			}
			#endif
			media_packt* pkt = new media_packt(packet.data,packet.size);
			
			q.push_back(pkt);
			_evtData.set();
		}
#endif
		av_free_packet(&packet);
		
	}
	avformat_close_input(&pFormatCtx_Video);
	avformat_free_context(pFormatCtx_Video);
	pFormatCtx_Video = NULL;
	_evtExit.set();
	_thread.stop();
	
	#if 1
	if(_callback)
	{
		LOGE(" disconnect callback");
		_callback(0,err);
	}
	_evtData.set();
	#endif
}

media_packt* RtspCameraCap::getimage(int timeout)
{
	if(_bExit) 
	{
		return NULL;
	}
	while(q.empty())
	{
		if(timeout <=0 )
		{
			
			_evtData.wait();
		}
		else
		{
			//LOGE("being wait--%d",timeout);
			if(false == _evtData.tryWait(timeout))
			{
			//timeout return NULL;
				//LOGE("wait timeout--%d",timeout);
				return NULL;
			}
		}
		if(_bExit) 
		{
			return NULL;
		}
		
	}
	CORE::Core_FastMutex::ScopedLock lock(_mutex);
	media_packt* pkt = q.front();
	q.pop_front();
	return pkt;

}

Detect::Detect():
	_pCamera(NULL),
	_timeout(2000),
	_tcp(false)
{
	_decExit = true;
	//static int cout = 0;
	//printf("cnt=%d",cout++);
}

Detect::~Detect(void)
{
	
}

bool Detect::start(RtspCameraCap* pCamera,std::string rtsp_url_addr,int timeout, bool tcp)
{
	LOGE("Detect start");
	if(!_decExit) 
	{
		LOGE("Detect has started");
		return true;

	}
	_pCamera = pCamera;
    rtsp_url = rtsp_url_addr;
	_timeout = timeout;
	_tcp = tcp;
	_thread.start(*this);
	_evtReady.wait();
	return true;
}

bool Detect::stop()
{
	if(_decExit)
	{
		return true;
	}
	_decExit =true;
	return true;
}

void Detect::run(void)
{
	_decExit = false;
	_evtReady.set();
	//CORE::Core_Thread::sleep(5000);
	while(!_decExit)
	{
	
		LOGE("rtsp dector's running --%d\n",__LINE__);
		if(_pCamera->isstop())
		{
		
			LOGE("reopen the camera again --%d\n",__LINE__);
			int err = _pCamera->open(rtsp_url.c_str(),_timeout,true,_tcp);
			LOGE("opened the camera err=%d\n",err);
		}
		CORE::Core_Thread::sleep(6000);
	}
	
}
