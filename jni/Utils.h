#ifndef UTILS_H
#define UTILS_H

#include <string>
#ifdef	__cplusplus
extern "C"
{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/pixdesc.h"

#ifdef __cplusplus
};
#endif
#include <fstream>

#ifdef ENALBE_LOG
#ifdef ANDROID

#include <android/log.h>
#define LOG_TAG "NativeCodec"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define LOGV(...) printf
#define LOGE(...) printf
#define LOGI(...) printf
#define LOGD(...) printf
#endif // ANDROID

#else
#define LOGV(...)
#define LOGE(...) 
#define LOGI(...) 
#define LOGD(...) 	
#endif

#ifdef ENABLE_EXE_LOG
#define LOGV(...) printf
#define LOGE(...) printf
#define LOGI(...) printf
#define LOGD(...) printf
#endif
std::string get_log_filename();
std::string GetProgramDir();
std::string GBKToUTF8(const char* strGBK);
int init_report_file(std::string log_config,std::string log_file );
int GBKToUTF8_V2(const char * lpGBKStr, char * lpUTF8Str,int nUTF8StrLen);
AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height, int algin);
AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
                                  uint64_t channel_layout,
                                  int sample_rate, int nb_samples);
void fill_black_color(AVFrame * frame);
std::string ws2s(const std::wstring& ws);
std::wstring s2ws(const std::string& s);

int find_stream_index(AVFormatContext* pCtx,enum AVMediaType type );
int select_sample_fmt(const AVCodec *codec,AVSampleFormat fmt);
int select_sample_rate(const AVCodec *codec,int sample_rate);
uint64_t select_channel_layout(const AVCodec *codec, int layout);
AVPixelFormat select_pixel_fmt(const AVCodec *codec, AVPixelFormat fmt);
void print_err(int err);
const char *get_error_text(const int error);

class MyFile
{
public:
	MyFile():
		_fp(NULL)
	{
		_file = "";
		isReadOnly = false;
		_frame = NULL;
		_frameSize = 0;
		_format = AV_PIX_FMT_NONE;
	}
	~MyFile();
	MyFile(const char* file);
	bool OpenReadOnly(const char* file);
	bool OpenWriteOnly(const char* file);
	bool Open(const char* file,const char* mode);
	bool Open(const char* file,AVPixelFormat format, int width, int height,int fps);
	bool Close();
	int  FillBuffer(unsigned char val, int size);
	int  FillTestBuffer();
	int	 WriteFrame(AVFrame* frame);
	int  WriteBuffer(void* buffer, int size);
	int  WriteRGB24(AVFrame* frame);
	int  WriteYUV420P(AVFrame* frame);
	AVFrame*  ReadFrame();
	int  ReadBuffer(void* buffer, int size);
	bool  ReadRGB24(AVFrame* frame);
	bool  ReadYUV420P(AVFrame* frame);
	
private:
	std::string _file;
	FILE* _fp;
	std::ofstream fout;
	//unsigned char *_buffer;
	int _width,_height,_fps;
	bool isReadOnly;
	AVFrame* _frame;
	int _frameSize;
	AVPixelFormat _format;
	unsigned char _buffer[320*240*3];
};





#endif