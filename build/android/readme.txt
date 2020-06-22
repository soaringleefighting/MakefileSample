## Android平台编译说明

### 方法1：单独命令方式
在android-ndk-r10e中执行如下命令：
（1）带优化编译：./ndk-build -B  -C ~/MakefileSample\build\android  PUREC=0
（2）纯C编译： 	./ndk-build -B  -C ~/MakefileSample\build\android PUREC=1

说明：android-ndk-r10e下载链接：https://blog.csdn.net/SoaringLee_fighting/article/details/106873454

### 方法2：脚本方式（Python）
（1）带优化编译： python build_andorid.py  /home/myshare/android-ndk-r10e  pure_c=0
（2）纯C编译：	 python build_andorid.py  /home/myshare/android-ndk-r10e  pure_c=1


### 方法3：脚本方式（shell） 
./build_android.sh ../../../android-ndk-r10e

