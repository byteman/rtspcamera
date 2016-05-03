
#include <stdio.h>
#include   <time.h>   
#include <iostream>
#include <string>


#include "Utils.h"
#include "Inifile.h"
#include "DateTime.h"
#ifdef   WIN32     
#define   LOCALTIME_R(t)     localtime((t))     
#else     
#define   LOCALTIME_R(t)     localtime_r((t),   (struct   tm   *)&tmres)     
#endif  

const char *get_error_text(const int error)
{
	static char error_buffer[255];
	av_strerror(error, error_buffer, sizeof(error_buffer));
	return error_buffer;
}
std::string get_log_filename()
{
	int year,  mon, day,hour, min,sec;
	CORE::DateTime dt;
	char log_path[256] = {0,};
	dt.getLocalTime(year,  mon, day,hour, min,sec);
    sprintf(log_path,"record_%04d%02d%02d%02d%02d%02d.log",year, mon,       
        day,   hour,   min,   sec);     
	return log_path;
}
#ifdef WIN32
#include <Windows.h>
std::string GetProgramDir()  
{   
    char exeFullPath[MAX_PATH]; // Full path
    std::string strPath = "";
 
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
    strPath=(std::string)exeFullPath;    // Get full path of the file
    int pos = strPath.find_last_of('\\', strPath.length());
    return strPath.substr(0, pos);  // Return the directory without the file name
}  
#else
std::string GetProgramDir()  
{
	return "/sdcard/";
}
#endif


static void YUV420p_to_RGB24(unsigned char *yuv420[3], unsigned char *rgb24, int width, int height) 
{
//  int begin = GetTickCount();
	int R,G,B,Y,U,V;
	int x,y;
	int nWidth = width>>1; //色度信号宽度
	for (y=0;y<height;y++)
	{
	for (x=0;x<width;x++)
	{
	Y = *(yuv420[0] + y*width + x);
	U = *(yuv420[1] + ((y>>1)*nWidth) + (x>>1));
	V = *(yuv420[2] + ((y>>1)*nWidth) + (x>>1));
	R = Y + 1.402*(V-128);
	G = Y - 0.34414*(U-128) - 0.71414*(V-128);
	B = Y + 1.772*(U-128);

	//防止越界
	if (R>255)R=255;
	if (R<0)R=0;
	if (G>255)G=255;
	if (G<0)G=0;
	if (B>255)B=255;
	if (B<0)B=0;
   
	*(rgb24 + ((height-y-1)*width + x)*3) = B;
	*(rgb24 + ((height-y-1)*width + x)*3 + 1) = G;
	*(rgb24 + ((height-y-1)*width + x)*3 + 2) = R;
  }
 }
}

static FILE *report_file;
static int report_file_level = AV_LOG_DEBUG;
#define va_copy(dst, src) ((dst) = (src))
static void log_callback_report(void *ptr, int level, const char *fmt, va_list vl)
{
    va_list vl2;
    char line[1024];
    static int print_prefix = 1;

    va_copy(vl2, vl);
    av_log_default_callback(ptr, level, fmt, vl);
    av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
    va_end(vl2);
    if (report_file_level >= level) {
        fputs(line, report_file);
        fflush(report_file);
    }
}
struct log_def{
	const char* desc;
	int level;
};
static struct log_def loglist[] = {
	{"quiet",AV_LOG_QUIET},
	{"debug",AV_LOG_DEBUG},
	{"verbose",AV_LOG_VERBOSE},
	{"info",AV_LOG_INFO},
	{"warning",AV_LOG_WARNING},
	{"error",AV_LOG_ERROR},
	{"fatal",AV_LOG_FATAL},
	{"panic",AV_LOG_PANIC},
	
};
#define ARRAY_SIZE(a,b) (sizeof(a)/sizeof(b))
static int parse_log_level(const char* logdesc)
{
	
	for(int i = 0; i < ARRAY_SIZE(loglist,struct log_def);i++)
	{
		if(!strcmp(logdesc, loglist[i].desc))
		{
			return loglist[i].level;
		}
	}
	return AV_LOG_QUIET;
}
int init_report_file(std::string log_config,std::string log_file )
{
	char buf[32] = {0,};
	report_file = fopen(log_file.c_str(), "w");
    if (!report_file) {
        int ret = AVERROR(errno);
        av_log(NULL, AV_LOG_ERROR, "Failed to open report \"%s\": %s\n",
               log_file.c_str(), strerror(errno));
        return 1;
		LOGE("%s %d",__FILE__,__LINE__);
    }
	LOGE("%s %d",__FILE__,__LINE__);
	av_log_set_callback(log_callback_report);
	return 0;
	CORE::Inifile cfg;
	int num = cfg.read_profile_string("log","enable", buf,sizeof(buf), log_config.c_str());
	LOGE("%s %d",__FILE__,__LINE__);
	//int num = GetPrivateProfileString("log","enable","0", buf,sizeof(buf), log_config.c_str());
	if(num < 0) return 0;
	if( 0 != strcmp(buf,"1"))
	{
		LOGE("%s %d",__FILE__,__LINE__);
		return 0;
	}
	LOGE("%s %d",__FILE__,__LINE__);
	memset(buf,0,32);
	num = cfg.read_profile_string("log","level", buf,sizeof(buf), log_config.c_str());
	
	report_file_level = parse_log_level(buf);
	LOGE("%s %d",__FILE__,__LINE__);
	report_file = fopen(log_file.c_str(), "w");
    if (!report_file) {
        int ret = AVERROR(errno);
        av_log(NULL, AV_LOG_ERROR, "Failed to open report \"%s\": %s\n",
               log_file.c_str(), strerror(errno));
        return 1;
		LOGE("%s %d",__FILE__,__LINE__);
    }
	LOGE("%s %d",__FILE__,__LINE__);
	av_log_set_callback(log_callback_report);
	av_log(NULL,AV_LOG_ERROR,"录像模块加载成功");
	LOGE("%s %d",__FILE__,__LINE__);
	return 0;
}
std::string ws2s(const std::wstring& ws)
{
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

std::wstring s2ws(const std::string& s)
{
    setlocale(LC_ALL, "chs"); 
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}
AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height, int algin)
{
    AVFrame *picture;
    int ret;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;

    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;

    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, algin);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        return NULL;
    }

    return picture;
}
void fill_black_color(AVFrame * frame)
{
	if(frame==NULL) return;
	memset(frame->data[0],0,frame->width*frame->height);
	memset(frame->data[1],0x80,frame->width*frame->height/4);
	memset(frame->data[2],0x80,frame->width*frame->height/4);
}
AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
                                  uint64_t channel_layout,
                                  int sample_rate, int nb_samples)
{
    AVFrame *frame = av_frame_alloc();
    int ret;

    if (!frame) {
        av_log(NULL,AV_LOG_ERROR, "Error allocating an audio frame\n");
        return NULL;
    }

    frame->format = sample_fmt;
    frame->channel_layout = channel_layout;
    frame->sample_rate = sample_rate;
    frame->nb_samples = nb_samples;

    if (nb_samples) {
        ret = av_frame_get_buffer(frame, 0);
        if (ret < 0) {
            av_log(NULL,AV_LOG_ERROR, "Error allocating an audio buffer\n");
            return NULL;
        }
    }

    return frame;
}

int find_stream_index(AVFormatContext* pCtx,enum AVMediaType type )
{
	int index = -1;
	if(pCtx == NULL) return index;
	for(int i = 0; i < pCtx->nb_streams; i++)
	{
	
		if (pCtx->streams[i]->codec->codec_type == type)
		{
			index = i;
			break;
		}
	
	}
	return index;
}

/* check that a given sample format is supported by the encoder */
int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt)
{
	const enum AVSampleFormat *p = codec->sample_fmts;

	while (*p != AV_SAMPLE_FMT_NONE) {
		if (*p == sample_fmt)
			return 1;
		p++;
	}
	return 0;
}
int select_sample_fmt(const AVCodec *codec,AVSampleFormat fmt)
{
	const enum AVSampleFormat *p;

	if (!codec->sample_fmts)
	{
		av_log(NULL,AV_LOG_ERROR,"no support fmts defalt to S16\r\n");
		return AV_SAMPLE_FMT_S16;
	}

	p = codec->sample_fmts;
	while (*p != AV_SAMPLE_FMT_NONE) {
		if(fmt == *p) return fmt;
		p++;
	}
	return codec->sample_fmts[0];
}
/* just pick the highest supported samplerate */
int select_sample_rate(const AVCodec *codec,int sample_rate)
{
	const int *p;
	int best_samplerate = 0;

	if (!codec->supported_samplerates)
		return 44100;

	p = codec->supported_samplerates;
	while (*p) {
		best_samplerate = FFMAX(*p, best_samplerate);
		if(*p == sample_rate) return sample_rate;
		p++;
	}
	return best_samplerate;
}

/* select layout with the highest channel count */
uint64_t select_channel_layout(const AVCodec *codec, int layout)
{
	const uint64_t *p;
	uint64_t best_ch_layout = 0;
	int best_nb_channels   = 0;

	if (!codec->channel_layouts)
		return AV_CH_LAYOUT_STEREO;

	p = codec->channel_layouts;
	while (*p) {
		int nb_channels = av_get_channel_layout_nb_channels(*p);

		if (nb_channels > best_nb_channels) {
			best_ch_layout    = *p;
			best_nb_channels = nb_channels;
			if(layout == *p) return layout;
		}
		p++;
	}
	return best_ch_layout;
}

AVPixelFormat select_pixel_fmt( const AVCodec *codec, AVPixelFormat fmt )
{
	const enum AVPixelFormat *p;

	if (!codec->pix_fmts)
	{
		av_log(NULL,AV_LOG_ERROR,"no support pixel_fmts defalt to AV_PIX_FMT_YUV420P\r\n");
		return AV_PIX_FMT_YUV420P;
	}

	p = codec->pix_fmts;
	while (*p != AV_PIX_FMT_NONE) {
		if(fmt == *p) return fmt;
		p++;
	}
	return codec->pix_fmts[0];
}

void print_err( int err )
{
	char errbuf[1024];
	av_strerror(err, errbuf, 1024);
	av_log(NULL, AV_LOG_ERROR, "*****err! : %s\n", errbuf);
}

MyFile::MyFile(const char* file)
{
	OpenWriteOnly(file);
}
MyFile::~MyFile()
{
	Close();
}
bool MyFile::Open(const char* file,const char* mode)
{
	_file = file;
	
	_fp = fopen(file,mode);
	//fout.open(file,std::ios::binary);
	if(_fp==NULL) return false;

	return true;
}
bool MyFile::Close()
{
	if(_fp)
	{
		fclose(_fp);
	}
	fout.close();
	return true;
}
int  MyFile::FillBuffer(unsigned char val, int size)
{
	int rt = 0;
	for(int i = 0; i < size; i++)
	{
		rt += fwrite(&val,1,1,_fp);
	}
	return rt;
}
int  MyFile::FillTestBuffer()
{
	int i = 0;
	for(i = 0 ; i < 120; i++)
		FillBuffer(0x23,320);
	for(i = 0 ; i < 120; i++)
		FillBuffer(0x80,320);

	for(i = 0 ; i < 60; i++)
		FillBuffer(0x23,160);
	for(i = 0 ; i < 60; i++)
		FillBuffer(0x80,160);

	for(i = 0 ; i < 60; i++)
		FillBuffer(0x23,160);
	for(i = 0 ; i < 60; i++)
		FillBuffer(0x80,160);
	return 0;


}
int MyFile::WriteFrame(AVFrame* frame)
{
	int ret = 0;
	if(frame==NULL) return 0;
	switch(frame->format)
	{
		case AV_PIX_FMT_YUV420P:
			ret = WriteYUV420P(frame);
			break;
		case AV_PIX_FMT_RGB24:
		case AV_PIX_FMT_BGR24:
			ret = WriteRGB24(frame);
			break;
	}
	return ret;
}

int  MyFile::WriteBuffer(void* buffer, int size)
{
	//如果buffer无效，或者数据长度不够，写入不会成功，返回0
	return fwrite(buffer,size,1,_fp);	
}

int  MyFile::WriteRGB24(AVFrame* frame)
{

	return WriteBuffer(frame->data[0],frame->width*frame->height*3);
}
int  MyFile::WriteYUV420P(AVFrame* frame)
{
	int y_size = frame->width*frame->height;
	int size = y_size+y_size/4+y_size/4;
	int ret = 0;
	ret += WriteBuffer(frame->data[0], y_size);
	ret += WriteBuffer(frame->data[1], y_size/4);
	ret += WriteBuffer(frame->data[2], y_size/4);
	return ret;

}

bool MyFile::Open(const char* file,AVPixelFormat format, int width, int height,int fps)
{
	isReadOnly = true;
	_width = width;
	_height = height;

	_frameSize = avpicture_get_size(format, width, height);

	_fp = fopen(file,"rb");
	if(_fp == NULL) return false;
	

	_frame = alloc_picture(format,width,height,16);
	if(_frame == NULL) return false;
	
	_format = format;
	//ReadBuffer(_buffer,320*240*1.5);
	return (_frame!=NULL); 
}
AVFrame*  MyFile::ReadFrame()
{
	bool rt = false;
	if(!isReadOnly) return NULL;

	switch(_format)
	{
		case AV_PIX_FMT_YUV420P:
			rt = ReadYUV420P(_frame);
			break;
		case AV_PIX_FMT_RGB24:
		case AV_PIX_FMT_BGR24:
			rt = ReadRGB24(_frame);
			break;
		default:
			return NULL;
			
	}
	if(!rt) return NULL;
	return _frame;
}

int  MyFile::ReadBuffer(void* buffer, int size)
{
	return fread(buffer,size,1,_fp);
}
bool  MyFile::ReadRGB24(AVFrame* frame)
{
	int ret = ReadBuffer(frame->data[0],frame->width*frame->height);
	return (ret == _frameSize);
}
bool MyFile::ReadYUV420P(AVFrame* frame)
{
	
	int ret = 0;

	//memcpy(frame->data[0],_buffer,frame->width*frame->height);
	//memcpy(frame->data[1],_buffer+frame->width*frame->height,frame->width*frame->height/4);
	//memcpy(frame->data[2],_buffer+(frame->width*frame->height)+(frame->width*frame->height)/4,frame->width*frame->height/4);
	//return true;



	//fseek(_fp,0,SEEK_CUR);
	ret += ReadBuffer(frame->data[0],frame->width*frame->height);
	ret += ReadBuffer(frame->data[1],frame->width*frame->height/4);
	ret += ReadBuffer(frame->data[2],frame->width*frame->height/4);

	
	return (ret == 3);

}

bool MyFile::OpenReadOnly( const char* file )
{
	return Open(file,"rb");
}

bool MyFile::OpenWriteOnly( const char* file )
{
	return Open(file,"wb");
}
