LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
$(warning "the value of LOCAL_PATH is $(LOCAL_PATH)") 
LOCAL_MODULE := Core
#LOCAL_MODULE_FILENAME := libcore
$(warning "the value of LOCAL_MODULE_FILENAME is $(LOCAL_MODULE_FILENAME)") 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_SRC_FILES	:= \
	LogConsoleChannel.cpp \
	Logger.cpp \
	tinyxmlparser.cpp \
	tinyxmlerror.cpp \
	tinyxml.cpp \
	tinystr.cpp \
	Timestamp.cpp \
	Timespan.cpp \
	TimeReactor.cpp \
	TimeNotifier.cpp \
	StringTokenizer.cpp \
	StreamSocketImpl.cpp \
	StreamSocket.cpp \
	SocketReactor.cpp \
	SocketNotifier.cpp \
	SocketImpl.cpp \
	SocketAddress.cpp \
	Socket.cpp \
	Runnable.cpp \
	RefCountedObject.cpp \
	Reactor.cpp \
	Random.cpp \
	ProtocolData.cpp \
	NetEventHandler.cpp \
	Inifile.cpp \
	ExceptionBase.cpp \
	Exception.cpp \
	DateTime.cpp \
	DatagramSocketImpl.cpp \
	DatagramSocket.cpp \
	Data.cpp \
	Core_ThreadPool.cpp \
	Core_Thread.cpp \
	Core_Mutex.cpp \
	Core_Event.cpp \
	Configurable.cpp \
	Base64.cpp \
	Ascii.cpp \
	AllocatorManager.cpp \
	Allocator.cpp 
	

include $(BUILD_STATIC_LIBRARY)
