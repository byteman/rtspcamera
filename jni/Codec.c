#include "CameraCap.h"
#include "ijksdl_codec_android_mediacodec.h"
#include "ijksdl_codec_android_mediacodec_java.h"

SDL_AMediaCodec* vedio_object;
void init(JNIEnv* env)
{
	jobject vedio;
	vedio_object = SDL_AMediaCodecJava_init(env,vedio);
	if(vedio_object == NULL)
	{
		printf("init failed! \n");
	}
}

AVPacket* decodec(unsigned char* data,int size)
{
	SDL_AMediaFormat* format;
	SDL_AMediaCodecJava_createByCodecName(env,"vedio/avc");
	if(vedio_object == NULL)
	{
		printf("创建解码器失败!\n");
	}
	SDL_AMediaCodec_configure(vedio_object,format,NULL,NULL,0);
	SDL_AMediaCodec_start(vedio_object);
}