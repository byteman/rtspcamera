#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T16:34:16
#
#-------------------------------------------------

TARGET = Core
TEMPLATE = lib

DEFINES += CORE_LIBRARY
CONFIG -= qt
CONFIG(debug, debug|release) {
        DESTDIR +=../binDX86
        OBJECTS_DIR += ./DX86
}

CONFIG(release, debug|release) {
        DESTDIR +=../binRArm
        OBJECTS_DIR += ./RArm
}




SOURCES +=     Timestamp.cpp \
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
    NetEventHandler.cpp \
    Logger.cpp \
    LogConsoleChannel.cpp \
    Inifile.cpp \
    ExceptionBase.cpp \
    Exception.cpp \
    DateTime.cpp \
    DatagramSocketImpl.cpp \
    DatagramSocket.cpp \
    Data.cpp \
    Core_Thread_WIN32.cpp \
    Core_Thread_POSIX.cpp \
    Core_ThreadPool.cpp \
    Core_Thread.cpp \
    Core_Mutex_WIN32.cpp \
    Core_Mutex_POSIX.cpp \
    Core_Mutex.cpp \
    Core_Event_WIN32.cpp \
    Core_Event_POSIX.cpp \
    Core_Event.cpp \
    Configurable.cpp \
    Ascii.cpp \
    AllocatorManager.cpp \
    Allocator.cpp \
    Base64.cpp

HEADERS += Types.h \
    tinyxml.h \
    tinystr.h \
    Timestamp.h \
    Timespan.h \
    TimeReactor.h \
    TimeNotifier.h \
    TimeEventHandler.h \
    StringTokenizer.h \
    StreamSocketImpl.h \
    StreamSocket.h \
    SocketReactor.h \
    SocketNotifier.h \
    SocketImpl.h \
    SocketAddress.h \
    Socket.h \
    SingletonHolder.h \
    ScopedLock.h \
    Runnable.h \
    RefCountedObject.h \
    Reactor.h \
    Random.h \
    ProtocolData.h \
    NetEventHandler.h \
    Logger.h \
    LogConsoleChannel.h \
    LogChannel.h \
    Inifile.h \
    ExceptionBase.h \
    Exception.h \
    DateTime.h \
    DatagramSocketImpl.h \
    DatagramSocket.h \
    Data.h \
    Core_Thread_WIN32.h \
    Core_Thread_POSIX.h \
    Core_ThreadPool.h \
    Core_Thread.h \
    Core_Mutex_WIN32.h \
    Core_Mutex_POSIX.h \
    Core_Mutex.h \
    CoreInc.h \
    Core_Event_WIN32.h \
    Core_Event_POSIX.h \
    Core_Event.h \
    Core.h \
    Configurable.h \
    AutoPtr.h \
    Ascii.h \
    AllocatorManager.h \
    Allocator.h \
    Base64.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE4555354
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Core.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    Makefile
