Linux系统下编译方法说明：

1、Linux平台
(1) Linux64位：
纯C编译： 
make -f Makefile_lib platform=x86_64 PUREC=1
make -f Makefile_demo platform=x86_64 PUREC=1
汇编编译：
make -f Makefile_lib platform=x86_64 PUREC=0
make -f Makefile_demo  platform=x86_64 PUREC=0

(2) linux32位： 
纯C编译：
make -f Makefile_lib platform=x86_32 PUREC=1
make -f Makefile_demo platform=x86_32 PUREC=1
汇编编译：
make -f Makefile_lib platform=x86_32 PUREC=0
make -f Makefile_demo platform=x86_32 PUREC=0

2、ARM平台
(1) ARM32位：
make -f Makefile_lib platform=arm32  CROSS=arm-linux-androideabi-
make -f Makefile_demo platform=arm32  CROSS=arm-linux-androideabi-

(2) ARM64位:


2、Mac平台
(1) Mac64位：
make -f Makefile_lib target_plat=mac platform=x86_64 PUREC=1
make -f Makefile_demo target_plat=mac platform=x86_64 PUREC=1
(2) Mac32位：
make -f Makefile_lib target_plat=mac platform=x86_32 PUREC=1
make -f Makefile_demo target_plat=mac platform=x86_32 PUREC=1

3、IOS平台
(1) IOS32位：
make -f Makefile_lib target_plat=ios platform=ios32 PUREC=1
make -f Makefile_demo target_plat=ios platform=ios32  PUREC=1
(2) IOS64位：
make -f Makefile_demo target_plat=ios platform=ios64  PUREC=1