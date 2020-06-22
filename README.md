# MakefileSample
This is a sample project for demonstrating how to use makefile simply.


## 各平台编译说明

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

make -f Makefile_demo target_plat=ios platform=ios64  PUREC=1

### 4、Android平台
build_andorid.py F:\software\android-ndk-r10e pure_c=0
