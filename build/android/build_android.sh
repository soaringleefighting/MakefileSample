#!/bin/bash

if [ $# -ne 1 ]; then
	echo "./build.sh ndk_dir"
	exit
fi

# get current dir
PJTDIR=$(pwd)

#check if we are in the ndk directory
cd $1
if [ ! -f "ndk-build" ]; then
	echo "Run this script under NDK directory\n"
	exit
fi
echo "------------------------clear output dir-------------------------"
find $PJTDIR/obj -name "*.[od]" | xargs rm -rf
find $PJTDIR/obj -name "*.so" | xargs rm -rf
find $PJTDIR/libs -name  "*.so" | xargs rm -rf
find $PJTDIR/out -name "*.so" | xargs rm -rf

find $PJTDIR/obj -name "*.[od]" | xargs rm -rf
find $PJTDIR/obj -name "*.a" | xargs rm -rf
find $PJTDIR/libs -name  "*.a" | xargs rm -rf
find $PJTDIR/out -name "*.a" | xargs rm -rf
echo "------------------------build static library----------------------"
cp $PJTDIR/jni/Android_lib.mk $PJTDIR/jni/Android.mk
./ndk-build SHARED=0 BUILD_VER=REV -B -C $PJTDIR/jni
cp  $PJTDIR/obj/local/armeabi/libAVSample.a  $PJTDIR/out/armeabi/
cp  $PJTDIR/obj/local/armeabi-v7a/libAVSample.a  $PJTDIR/out/armeabi-v7a/
cp  $PJTDIR/obj/local/arm64-v8a/libAVSample.a  $PJTDIR/out/arm64-v8a/
cp  $PJTDIR/obj/local/x86/libAVSample.a  $PJTDIR/out/x86/
cp  $PJTDIR/obj/local/x86_64/libAVSample.a  $PJTDIR/out/x86_64/
cp  $PJTDIR/obj/local/mips/libAVSample.a  $PJTDIR/out/mips/
cp  $PJTDIR/obj/local/mips64/libAVSample.a  $PJTDIR/out/mips64/

echo "------------------------build dynamic library----------------------"
cp $PJTDIR/jni/Android_lib.mk $PJTDIR/jni/Android.mk
./ndk-build SHARED=1 BUILD_VER=REV -B -C $PJTDIR/jni
cp  $PJTDIR/obj/local/armeabi/libAVSample.so $PJTDIR/out/armeabi/
cp  $PJTDIR/obj/local/armeabi-v7a/libAVSample.so  $PJTDIR/out/armeabi-v7a/
cp  $PJTDIR/obj/local/arm64-v8a/libAVSample.so  $PJTDIR/out/arm64-v8a/
cp  $PJTDIR/obj/local/x86/libAVSample.so  $PJTDIR/out/x86/
cp  $PJTDIR/obj/local/x86_64/libAVSample.so  $PJTDIR/out/x86_64/
cp  $PJTDIR/obj/local/mips/libAVSample.so  $PJTDIR/out/mips/
cp  $PJTDIR/obj/local/mips64/libAVSample.so  $PJTDIR/out/mips64/

echo "------------------------build executable----------------------" 
cp $PJTDIR/jni/Android_app.mk  $PJTDIR/jni/Android.mk
./ndk-build  BUILD_VER=REV -B -C $PJTDIR/jni
cp  $PJTDIR/obj/local/armeabi/demo  $PJTDIR/out/armeabi/
cp  $PJTDIR/obj/local/armeabi-v7a/demo  $PJTDIR/out/armeabi-v7a/
cp  $PJTDIR/obj/local/arm64-v8a/demo  $PJTDIR/out/arm64-v8a/
cp  $PJTDIR/obj/local/x86/demo  $PJTDIR/out/x86/
cp  $PJTDIR/obj/local/x86_64/demo  $PJTDIR/out/x86_64/
cp  $PJTDIR/obj/local/mips/demo  $PJTDIR/out/mips/
cp  $PJTDIR/obj/local/mips64/demo  $PJTDIR/out/mips64/


echo "------------------------tar---------------------" 
tar -zcvf $PJTDIR/out/libAVSample_android_armeabi.tar.gz  -C $PJTDIR/out/armeabi/ .
tar -zcvf $PJTDIR/out/libAVSample_android_armeabi-v7a.tar.gz  -C $PJTDIR/out/armeabi-v7a/ .
tar -zcvf $PJTDIR/out/libAVSample_android_arm64-v8a.tar.gz  -C $PJTDIR/out/arm64-v8a/ .
tar -zcvf $PJTDIR/out/libAVSample_android_x86.tar.gz  -C $PJTDIR/out/x86/ .
tar -zcvf $PJTDIR/out/libAVSample_android_x86_64.tar.gz  -C $PJTDIR/out/x86_64/ .
tar -zcvf $PJTDIR/out/libAVSample_android_mips.tar.gz  -C $PJTDIR/out/mips/ .
tar -zcvf $PJTDIR/out/libAVSample_android_mips64.tar.gz  -C $PJTDIR/out/mips64/ .
echo "------------------------check results---------------------" 
ls -l $PJTDIR/out
ls -l $PJTDIR/out/armeabi
ls -l $PJTDIR/out/armeabi-v7a
ls -l $PJTDIR/out/arm64-v8a
ls -l $PJTDIR/out/x86
ls -l $PJTDIR/out/x86_64
ls -l $PJTDIR/out/mips
ls -l $PJTDIR/out/mips64
