# MakefileSample
This is a sample project for demonstrating how to use makefile simply.

## 一、支持平台
Linux(ARM), Windows, Android, Mac, IOS

## 二、各平台编译说明

### 1、Linux平台
(1) Linux64位： 
make -f Makefile_lib platform=x86_64 PUREC=1

make -f Makefile_demo platform=x86_64 PUREC=1

(2) linux32位： 
make -f Makefile_lib platform=x86_32 PUREC=1

make -f Makefile_demo platform=x86_32 PUREC=1

### 2、Mac平台
(1) Mac64位：

make -f Makefile_lib target_plat=mac platform=x86_64 PUREC=1

make -f Makefile_demo target_plat=mac platform=x86_64 PUREC=1

(2) Mac32位：

make -f Makefile_lib target_plat=mac platform=x86_32 PUREC=1

make -f Makefile_demo target_plat=mac platform=x86_32 PUREC=1

### 3、IOS平台
(1) IOS32位：

make -f Makefile_lib target_plat=ios platform=ios32 PUREC=1

make -f Makefile_demo target_plat=ios platform=ios32  PUREC=1

(2) IOS64位：

make -f Makefile_lib target_plat=ios platform=ios64  PUREC=1

make -f Makefile_demo target_plat=ios platform=ios64  PUREC=1

(3) IOS Universal Pack：

cd ./build/ios

make -f Makefile_ios_pack

### 4、Android平台
#### 4.1、单独命令方式
在android-ndk-r10e中执行如下命令：

（1）带优化编译：./ndk-build -B  -C ~/MakefileSample\build\android  PUREC=0

（2）纯C编译： 	./ndk-build -B  -C ~/MakefileSample\build\android PUREC=1

说明：android-ndk-r10e下载链接：https://blog.csdn.net/SoaringLee_fighting/article/details/106873454

#### 4.2、脚本方式（Python）
（1）带优化编译： python build_andorid.py  /home/myshare/android-ndk-r10e  pure_c=0

（2）纯C编译：	 python build_andorid.py  /home/myshare/android-ndk-r10e  pure_c=1


#### 4.3、脚本方式（shell） 
./build_android.sh /home/myshare/android-ndk-r10e


### 5、Windows平台
#### 5.1、方法一（bat）
cd ./build/windows

启动cmd:

build_vs2010.bat release Win32

build_vs2010.bat release x64

#### 5.2、方法二（MinGW）

首先启动minGW和VC环境，运行msys_vs2010_win32.bat或msys_vs2010_win64.bat，

cd ./build/windowsmake

make -f Makefile_demo platform=win32

make -f Makefile_demo platform=x64
