LOCAL_PATH := $(call my-dir)

####$(warning $(LOCAL_PATH))
###ffmpeg库
include $(CLEAR_VARS)
LOCAL_MODULE := ijkffmpeg
LOCAL_SRC_FILES := ./libijkffmpeg.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_C_INCLUDES = Core\
$(LOCAL_PATH)/../include
LOCAL_MODULE := GetCameraStream
LOCAL_SRC_FILES := GetCameraStreamJNI.cpp RtspCameraCap.cpp Utils.cpp
LOCAL_LDLIBS  += -lm -llog -landroid 
LOCAL_CFLAGS += -DENALBE_LOG
LOCAL_SHARED_LIBRARIES += ijkffmpeg  
LOCAL_STATIC_LIBRARIES += Core 

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES = Core\
$(LOCAL_PATH)/../include
LOCAL_MODULE := video_main
LOCAL_SRC_FILES := main.cpp RtspCameraCap.cpp Utils.cpp
LOCAL_LDLIBS  += -lm -llog -landroid 

LOCAL_SHARED_LIBRARIES += ijkffmpeg 
LOCAL_STATIC_LIBRARIES += Core 

#include $(BUILD_EXECUTABLE)

$(call import-add-path,$(LOCAL_PATH))
$(call import-module,Core)