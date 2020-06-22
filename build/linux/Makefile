#########################################################################
## Function: makefile for all platform
## Platform: linux, arm, ios(ios_pack), macos
## Author: 	 lipeng
## Modified: 2020.6.18 created
#########################################################################
all: clean
##################################Linux平台#########################################
	make -f Makefile_lib  platform=x86_64 PUREC=0
	make -f Makefile_demo platform=x86_64 PUREC=0
	
	make -f Makefile_lib  platform=x86_32 PUREC=0
	make -f Makefile_demo platform=x86_32 PUREC=0
	
##################################ARM平台#########################################
	make -f Makefile_lib platform=arm32  CROSS=arm-linux-androideabi- PUREC=0
	make -f Makefile_demo platform=arm32 CROSS=arm-linux-androideabi- PUREC=0

	make -f Makefile_lib platform=arm64   CROSS=aarch64-linux-android- PUREC=0
	make -f Makefile_demo platform=arm64  CROSS=aarch64-linux-android- PUREC=0
	
##################################IOS平台#########################################
	#make -f Makefile_lib target_plat=ios platform=ios32 PUREC=1
	#make -f Makefile_demo target_plat=ios platform=ios32  PUREC=1
	#make -f Makefile_lib target_plat=ios platform=ios64  PUREC=1
	#make -f Makefile_demo target_plat=ios platform=ios64  PUREC=1


##################################MAC平台#########################################
	#make -f Makefile_lib target_plat=mac platform=x86_64 PUREC=0
	#make -f Makefile_demo target_plat=mac platform=x86_64 PUREC=0
	
	#make -f Makefile_lib target_plat=mac platform=x86_32 PUREC=0
	#make -f Makefile_demo target_plat=mac platform=x86_32 PUREC=0

clean:
##################################Linux平台#########################################
	make -f Makefile_lib  platform=x86_64 PUREC=0 clean
	make -f Makefile_demo platform=x86_64 PUREC=0 clean
	
	make -f Makefile_lib  platform=x86_32 PUREC=0 clean
	make -f Makefile_demo platform=x86_32 PUREC=0 clean
	
##################################ARM平台#########################################
	make -f Makefile_lib platform=arm32  CROSS=arm-linux-androideabi- PUREC=0 clean
	make -f Makefile_demo platform=arm32 CROSS=arm-linux-androideabi- PUREC=0 clean

	make -f Makefile_lib platform=arm64   CROSS=aarch64-linux-android- PUREC=0 clean
	make -f Makefile_demo platform=arm64  CROSS=aarch64-linux-android- PUREC=0 clean
	
##################################IOS平台#########################################
	#make -f Makefile_lib target_plat=ios platform=ios32 PUREC=1 clean
	#make -f Makefile_demo target_plat=ios platform=ios32  PUREC=1 clean
	#make -f Makefile_lib target_plat=ios platform=ios64  PUREC=1 clean
	#make -f Makefile_demo target_plat=ios platform=ios64  PUREC=1 clean


##################################MAC平台#########################################
	#make -f Makefile_lib target_plat=mac platform=x86_64 PUREC=0 clean
	#make -f Makefile_demo target_plat=mac platform=x86_64 PUREC=0 clean
	
	#make -f Makefile_lib target_plat=mac platform=x86_32 PUREC=0 clean
	#make -f Makefile_demo target_plat=mac platform=x86_32 PUREC=0 clean
	