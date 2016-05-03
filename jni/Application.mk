NDK_TOOLCHAIN_VERSION=4.9

APP_ABI := armeabi-v7a  
APP_PIE := true
APP_STL := gnustl_shared
APP_CPPFLAGS := -std=c++11 -frtti -fexceptions -Werror=format-security
#APP_CPPFLAGS += -fpermissive
APP_PLATFORM := android-14