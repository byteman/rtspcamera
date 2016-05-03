ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk

#cp -rf libs/* ../../MobileBankLibrary/MobileBankLib/jni/mylib/
cp -rf obj/local/armeabi-v7a/*.a ../binRArm/armeabi-v7a/
cp -rf obj/local/arm64-v8a/*.a ../binRArm/arm64-v8a/
cp -rf libs/armeabi-v7a/*.so ../binRArm/armeabi-v7a/
cp -rf libs/arm64-v8a/*.so ../binRArm/arm64-v8a/
#ndk-build NDK_DEBUG=0 NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk
