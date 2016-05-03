# Microsoft Developer Studio Project File - Name="Core" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Core - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Core.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Core.mak" CFG="Core - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Core - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Core - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WIN32_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORE_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /Core
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /Core
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../binR/Core.dll" /implib:"../../bin/Core.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WIN32_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORE_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /Core
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /Core
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/Core.dll" /implib:"../../bin/Core.lib" /pdbtype:sept /libpath:"..\..\bin"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Core - Win32 Release"
# Name "Core - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Allocator.cpp
# End Source File
# Begin Source File

SOURCE=..\AllocatorManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Configurable.cpp
# End Source File
# Begin Source File

SOURCE=..\Core_Event.cpp
# End Source File
# Begin Source File

SOURCE=..\Core_Mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\Core_Thread.cpp
# End Source File
# Begin Source File

SOURCE=..\Core_ThreadPool.cpp
# End Source File
# Begin Source File

SOURCE=..\Data.cpp
# End Source File
# Begin Source File

SOURCE=..\DatagramSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\DatagramSocketImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\DateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\Exception.cpp
# End Source File
# Begin Source File

SOURCE=..\ExceptionBase.cpp
# End Source File
# Begin Source File

SOURCE=..\Inifile.cpp
# End Source File
# Begin Source File

SOURCE=..\LogConsoleChannel.cpp
# End Source File
# Begin Source File

SOURCE=..\Logger.cpp
# End Source File
# Begin Source File

SOURCE=..\NetEventHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\ProtocolData.cpp
# End Source File
# Begin Source File

SOURCE=..\Random.cpp
# End Source File
# Begin Source File

SOURCE=..\Reactor.cpp
# End Source File
# Begin Source File

SOURCE=..\RefCountedObject.cpp
# End Source File
# Begin Source File

SOURCE=..\Runnable.cpp
# End Source File
# Begin Source File

SOURCE=..\Socket.cpp
# End Source File
# Begin Source File

SOURCE=..\SocketAddress.cpp
# End Source File
# Begin Source File

SOURCE=..\SocketImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\SocketNotifier.cpp
# End Source File
# Begin Source File

SOURCE=..\SocketReactor.cpp
# End Source File
# Begin Source File

SOURCE=..\StreamSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\StreamSocketImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\TimeNotifier.cpp
# End Source File
# Begin Source File

SOURCE=..\TimeReactor.cpp
# End Source File
# Begin Source File

SOURCE=..\Timespan.cpp
# End Source File
# Begin Source File

SOURCE=..\Timestamp.cpp
# End Source File
# Begin Source File

SOURCE=..\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=..\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Allocator.h
# End Source File
# Begin Source File

SOURCE=..\AllocatorManager.h
# End Source File
# Begin Source File

SOURCE=..\AutoPtr.h
# End Source File
# Begin Source File

SOURCE=..\Configurable.h
# End Source File
# Begin Source File

SOURCE=..\Core.h
# End Source File
# Begin Source File

SOURCE=..\Core_Event.h
# End Source File
# Begin Source File

SOURCE=..\Core_Mutex.h
# End Source File
# Begin Source File

SOURCE=..\Core_Thread.h
# End Source File
# Begin Source File

SOURCE=..\Core_ThreadPool.h
# End Source File
# Begin Source File

SOURCE=..\CoreInc.h
# End Source File
# Begin Source File

SOURCE=..\Data.h
# End Source File
# Begin Source File

SOURCE=..\DatagramSocket.h
# End Source File
# Begin Source File

SOURCE=..\DatagramSocketImpl.h
# End Source File
# Begin Source File

SOURCE=..\DateTime.h
# End Source File
# Begin Source File

SOURCE=..\Exception.h
# End Source File
# Begin Source File

SOURCE=..\ExceptionBase.h
# End Source File
# Begin Source File

SOURCE=..\Inifile.h
# End Source File
# Begin Source File

SOURCE=..\LogChannel.h
# End Source File
# Begin Source File

SOURCE=..\LogConsoleChannel.h
# End Source File
# Begin Source File

SOURCE=..\Logger.h
# End Source File
# Begin Source File

SOURCE=..\NetEventHandler.h
# End Source File
# Begin Source File

SOURCE=..\ProtocolData.h
# End Source File
# Begin Source File

SOURCE=..\Random.h
# End Source File
# Begin Source File

SOURCE=..\Reactor.h
# End Source File
# Begin Source File

SOURCE=..\RefCountedObject.h
# End Source File
# Begin Source File

SOURCE=..\Runnable.h
# End Source File
# Begin Source File

SOURCE=..\ScopedLock.h
# End Source File
# Begin Source File

SOURCE=..\SingletonHolder.h
# End Source File
# Begin Source File

SOURCE=..\Socket.h
# End Source File
# Begin Source File

SOURCE=..\SocketAddress.h
# End Source File
# Begin Source File

SOURCE=..\SocketImpl.h
# End Source File
# Begin Source File

SOURCE=..\SocketNotifier.h
# End Source File
# Begin Source File

SOURCE=..\SocketReactor.h
# End Source File
# Begin Source File

SOURCE=..\StreamSocket.h
# End Source File
# Begin Source File

SOURCE=..\StreamSocketImpl.h
# End Source File
# Begin Source File

SOURCE=..\TimeEventHandler.h
# End Source File
# Begin Source File

SOURCE=..\TimeNotifier.h
# End Source File
# Begin Source File

SOURCE=..\TimeReactor.h
# End Source File
# Begin Source File

SOURCE=..\Timespan.h
# End Source File
# Begin Source File

SOURCE=..\Timestamp.h
# End Source File
# Begin Source File

SOURCE=..\tinystr.h
# End Source File
# Begin Source File

SOURCE=..\tinyxml.h
# End Source File
# Begin Source File

SOURCE=..\Types.h
# End Source File
# End Group
# End Target
# End Project
