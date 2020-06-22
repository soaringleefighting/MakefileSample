#########################################################################
## Function: makefile for all platform
## Platform: linux,arm, ios, macos
## Author: 	 lipeng
## Modified: 2020.6.18 created
#########################################################################

##################################Linux平台#########################################
make -f Makefile_lib platform=x86_32
make -f Makefile_demo platform=x86_32 

make -f Makefile_lib platform=x86_64
make -f Makefile_demo platform=x86_64 



##################################ARM平台#########################################
make -f Makefile_lib platform=arm 	CROSS=arm-none-linux-gnueabi-
make -f Makefile_demo platform=arm  CROSS=arm-none-linux-gnueabi-

make -f Makefile_lib platform=arm 	CROSS=aarch64-linux-gnu-
make -f Makefile_demo platform=arm  CROSS=aarch64-linux-gnu-


##################################IOS平台#########################################




##################################MAC平台#########################################
