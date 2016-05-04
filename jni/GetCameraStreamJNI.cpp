#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <stdio.h>
#include <android/log.h>
#include "RtspCameraCap.h"
#include "utils.h"
#include "cn_cloudwalk_camera_GetCameraStreamJNI.h"

JavaVM *g_vm = nullptr;

static jclass gCallbackCls = NULL;

RtspCameraCap camerastream;

JNIEnv* getEnv()
{
	JNIEnv *env = nullptr;
	auto err = g_vm->GetEnv(reinterpret_cast<void**>(&env),JNI_VERSION_1_4);

	if(err == JNI_EDETACHED)
	{
		LOGE("CURRENT THREAD NOT ATTACHED TO JAVA VM");
	}
	else if(err = JNI_EVERSION)
	{
		LOGE("VM DOESN'T SUPPORT REQUESTED JNI VERSION");
	}

	if(env)
	{
		return env;
	}

	//throw runtime_error("INVALID JNI ENV");
	throw ("INVALID JNI ENV");
}
jint JNI_OnLoad( JavaVM* vm, void* reserved )
{
	LOGE("---------------ONLOAD");
	g_vm = vm;
	
	LOGE("---------------ONLOAD");
	return JNI_VERSION_1_4;
}
std::string jtoString(JNIEnv *env,jstring s)
{
	if(s)
	{
		LOGE("GetCameraStreamInit -- %d",__LINE__);
		LOGE("GetCameraStreamInit -- %d",__LINE__);
		const char *chars = env->GetStringUTFChars(s,nullptr);
		LOGE("GetCameraStreamInit -- %d",__LINE__);
		if(chars)
		{
			LOGE("GetCameraStreamInit -- %d",__LINE__);
			string tmp(chars);
			env->ReleaseStringUTFChars(s,chars);
			return tmp;
		}
	}
	return "";
}

jstring stoJtring(const string &s)
{
	LOGE("GetCameraStreamInit -- %d",__LINE__);
	return getEnv()->NewStringUTF(s.data());
}

static bool existFile(std::string path)
{
	struct stat st;
	return stat(path.c_str(),&st) == 0;
}

int SCCallback(int id,int msg)
{
	JNIEnv *mainEnv = NULL;
	jint ret;
	if(NULL==gCallbackCls) 
	{
		LOGE("callback obs not inited");
		return -1;
	}
	if(g_vm == NULL) 
	{
		LOGE("gs_jvm == NULL");
		return -2;
	}
	//LOGV("SCCallback callback called1");
	g_vm->AttachCurrentThread(&mainEnv, NULL);
	if(mainEnv == NULL)
	{
		LOGE("SCCallback AttachCurrentThread failed");
		return -3;
	}

	//这里的签名很关键，可以用javap -classpath bin/classes -p -s cn.cloudwalk.camera.SCCallback 来查看正确的签名. 
	jmethodID handler = mainEnv->GetStaticMethodID(gCallbackCls,"SCMessageHandler", "(II)I");
	
	if(handler==NULL)
	{
		LOGE("SCCallback GetStaticMethod SCMessageHandler failed");
		return -4;
	}
	

	ret = (jint)mainEnv->CallStaticIntMethod(gCallbackCls,handler ,id,msg);
	//LOGV("end call");
	
	g_vm->DetachCurrentThread();
	
	return ret;
	
}


static void InitCallBackObjs(JNIEnv* env)
{
	

	if (gCallbackCls == NULL) {
		LOGE("InitCallBackObjs");
		RtspCameraCap::init();
		
		//cn.cloudwalk.nativeservice.CloudwalkFaceSDK
		LOGE("SCInit find class");
		camerastream.setCallBack(SCCallback);
		jclass localRefCls  =  env->FindClass("cn/cloudwalk/camera/SCCallback");
		if(localRefCls == NULL)
		{
			LOGE("NoClassDefFoundError   cn/cloudwalk/camera/SCCallback \n");
			return;
		}
		
		gCallbackCls = (jclass)env->NewGlobalRef(localRefCls);
		if (NULL == gCallbackCls) {
			LOGE("NULL == gCallbackCls \n" );
			return;
		}		
	}
	
}
jint Java_cn_cloudwalk_camera_GetCameraStreamJNI_Open(JNIEnv *env,jclass,jstring jpath,jint timeout_ms,jboolean auto_reconnect,jboolean tcp)
{
	LOGE("GetCameraStreamInit called");
	InitCallBackObjs(env);
	LOGE("GetCameraStreamInit -- %d",__LINE__);
	std::string path = jtoString(env,jpath);
	LOGE("GetCameraStreamInit22 -- %d",__LINE__);

	int err = camerastream.connect(path.c_str(),timeout_ms,auto_reconnect,tcp);
	LOGE("GetCameraStreamInit -- %d",__LINE__);
	
	
	return err;
	

}

jint Java_cn_cloudwalk_camera_GetCameraStreamJNI_Pause
  (JNIEnv *, jclass,jboolean bPause)
{
	int err = camerastream.pause(bPause);
	return err;
}
jbyteArray Java_cn_cloudwalk_camera_GetCameraStreamJNI_GetImage(JNIEnv *env,jclass,jint timeout)
{
	
	media_packt *pkt = camerastream.getimage(timeout);
	if(pkt == NULL) return NULL;
	
	jint length = pkt->size;
	jbyteArray bytes = env->NewByteArray(length);
	if (bytes != NULL) {
		//LOGE("SetByteArrayRegion ok");
		env->SetByteArrayRegion(bytes, 0, length,(jbyte*)pkt->pdata);
		delete pkt;
	}
	return bytes;  

}

jobject Java_cn_cloudwalk_camera_GetCameraStreamJNI_GetMediaInfo(JNIEnv *env,jclass){
	//###获取实例类##########
	jclass objectClass = env->FindClass("cn/cloudwalk/camera/MediaInfo");
	if(objectClass == NULL)
	{
		LOGI("NoClassDefFoundError   cn/cloudwalk/camera/MediaInfo \n");
	}

	//#####建立新对象########
	jobject joInfo = env->AllocObject(objectClass);

	//######获取函数返回值######
	jfieldID jret = (env)->GetFieldID(objectClass,"width","I");
	(env)->SetIntField(joInfo, jret,(int)camerastream._width);
	if(joInfo == 0)
	{
		return nullptr;
	}

	jret = (env)->GetFieldID(objectClass,"height","I");
	(env)->SetIntField(joInfo, jret,(int)camerastream._height);
	if(joInfo== 0)
	{
		return nullptr;
	}
	jret = (env)->GetFieldID(objectClass,"format","I");
	(env)->SetIntField(joInfo, jret,(int)camerastream._format);
	if(joInfo== 0)
	{
		return nullptr;
	}
	
	return joInfo;
}

jint Java_cn_cloudwalk_camera_GetCameraStreamJNI_Close(JNIEnv *,jclass)
{
	int err = camerastream.stop();
	
	return err;
}
