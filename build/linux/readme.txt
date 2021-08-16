Linux/ARM/MacOS/IOS系统下编译方法说明：

gas-preprocessor.pl是用于IOS平台下的汇编解释器，来源于x264。

1、Linux平台
(1) Linux64位：
纯C编译： 
make -f Makefile_lib  platform=x86_64 PUREC=1
make -f Makefile_demo platform=x86_64 PUREC=1
汇编编译：
make -f Makefile_lib  platform=x86_64 PUREC=0
make -f Makefile_demo platform=x86_64 PUREC=0

(2) linux32位： 
纯C编译：
make -f Makefile_lib  platform=x86_32 PUREC=1
make -f Makefile_demo platform=x86_32 PUREC=1
汇编编译：
make -f Makefile_lib  platform=x86_32 PUREC=0
make -f Makefile_demo platform=x86_32 PUREC=0

2、ARM平台
(1) ARM32位：
纯C编译：
make -f Makefile_lib  platform=arm32  CROSS=arm-linux-androideabi- PUREC=1
make -f Makefile_demo platform=arm32  CROSS=arm-linux-androideabi- PUREC=1
NEON编译：
make -f Makefile_lib  platform=arm32  CROSS=arm-linux-androideabi- PUREC=0
make -f Makefile_demo platform=arm32  CROSS=arm-linux-androideabi- PUREC=0

(2) ARM64位:
纯C编译：
make -f Makefile_lib  platform=arm64  CROSS=aarch64-linux-android- PUREC=1
make -f Makefile_demo platform=arm64  CROSS=aarch64-linux-android- PUREC=1
NEON编译： 
make -f Makefile_lib  platform=arm64  CROSS=aarch64-linux-android- PUREC=0
make -f Makefile_demo platform=arm64  CROSS=aarch64-linux-android- PUREC=0

3、MacOS平台
(1) MacOS X86_64位：
纯C编译：
make -f Makefile_lib  target_plat=mac platform=x86_64 PUREC=1
make -f Makefile_demo target_plat=mac platform=x86_64 PUREC=1
汇编编译：
make -f Makefile_lib  target_plat=mac platform=x86_64 PUREC=0
make -f Makefile_demo target_plat=mac platform=x86_64 PUREC=0

(2) MacOS X86_32位：(deprecated)
纯C编译：
make -f Makefile_lib  target_plat=mac platform=x86_32 PUREC=1
make -f Makefile_demo target_plat=mac platform=x86_32 PUREC=1
汇编编译：
make -f Makefile_lib  target_plat=mac platform=x86_32 PUREC=0
make -f Makefile_demo target_plat=mac platform=x86_32 PUREC=0

(3) MacOS ARM64位：
纯C编译：
make -f Makefile_lib  target_plat=mac platform=arm64 PUREC=1
make -f Makefile_demo target_plat=mac platform=arm64 PUREC=1 
汇编编译：
make -f Makefile_lib  target_plat=mac platform=arm64 PUREC=0 
make -f Makefile_demo target_plat=mac platform=arm64 PUREC=0 

4、IOS平台
(1) IOS32位：
纯C编译：
make -f Makefile_lib  target_plat=ios platform=ios32 PUREC=1
make -f Makefile_demo target_plat=ios platform=ios32 PUREC=1
汇编编译：
make -f Makefile_lib  target_plat=ios platform=ios32 PUREC=0
make -f Makefile_demo target_plat=ios platform=ios32 PUREC=0

(2) IOS64位：
纯C编译：
make -f Makefile_lib  target_plat=ios platform=ios64 PUREC=1
make -f Makefile_demo target_plat=ios platform=ios64 PUREC=1
汇编编译：
make -f Makefile_lib  target_plat=ios platform=ios64 PUREC=0
make -f Makefile_demo target_plat=ios platform=ios64 PUREC=0

(3) IOS Universal Pack：
cd ./build/ios
make -f Makefile_ios_pack

4、MIPS平台
纯C编译：
make -f Makefile_lib platform=mips64 CROSS=loongarch64-linux-gnu-
make -f Makefile_demo platform=mips64 CROSS=loongarch64-linux-gnu-
