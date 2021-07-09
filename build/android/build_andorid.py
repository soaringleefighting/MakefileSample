#coding=utf8
import os
import sys
import glob
import shutil
import platform
from sys import argv

share_lib_name = 'libAVSample.so'
static_lib_name =  'libAVSample.a'
static_exe = 'demo'
#检查是否为windows系统
def  isWindowsSys():
	return 'Windows' in platform.system()
#检查是否为Linux系统
def  isLinuxSys():
	return 'Linux' in platform.system()
#清空目录下的内容
def delDirFile(pathDir):
	for i in os.listdir(pathDir):
		file_path = os.path.join(pathDir, i)
		if os.path.isfile(file_path):
			os.remove(file_path)
		else:
			delDirFile(file_path)
			
def process(ndk_dir, android_dir, pure_c, delimiter):
	ndk_build = ndk_dir + delimiter + 'ndk-build'
	app_mk = android_dir + delimiter + 'jni' + delimiter + 'Application.mk'
	mk_dir = android_dir + delimiter + 'jni'
	static_lib_dir = android_dir + delimiter + 'obj' + delimiter + 'local' + delimiter
	out_sha_lib = android_dir + delimiter + 'out' + delimiter
	share_lib_dir = android_dir + delimiter + 'obj' + delimiter + 'local' + delimiter
	out_sta_lib = android_dir + delimiter + 'out' + delimiter
        final_out_lib= android_dir + delimiter + '..' + delimiter + '..' + delimiter + 'out' + delimiter + 'android' + delimiter
	if isWindowsSys():
		cpy = "copy "
	else:
		cpy = "cp -rf "
		
	opt = pure_c.split('=')[1]
	# "=================CLEAR OUTPUT DIR========================="
	outdir_files = glob.glob(android_dir + delimiter + 'out' + delimiter + '*')
	for outdir in outdir_files:
		print(outdir)
		delDirFile(outdir)
		isExist = os.path.exists(outdir)
		if(not isExist):
			os.mkdir(outdir)

	pFile = open(app_mk, 'w') #armeabi armeabi-v7a arm64-v8a x86 x86_64 mips mips64
	cmd = 'APP_ABI := armeabi armeabi-v7a arm64-v8a x86 x86_64 mips mips64\nAPP_PLATFORM := android-9\nNDK_TOOLCHAIN_VERSION := 4.9'
	pFile.writelines(cmd)
	pFile.write('\n')
	pFile.close()
	# "==================BUILD SHARED LIBRARY===================="
	### armeabi-v7a
        cmd = ndk_build + " -B " + "NDK_PROJECT_PATH:=" + android_dir + " APP_BUILD_SCRIPT:=" + mk_dir + delimiter + "Android_lib.mk" + " V=1 SHARED=1" + " PURE_C=" + opt
	print(cmd)
	os.system(cmd)
	cmd = cpy + share_lib_dir + "armeabi-v7a" + delimiter + share_lib_name + "  " + out_sha_lib + "armeabi-v7a" + delimiter 
	print(cmd)
	os.system(cmd)
    
        ##其他平台依次类推
	### arm64-v8a
        cmd = cpy + share_lib_dir + "arm64-v8a" + delimiter + share_lib_name + "  " + out_sha_lib + "arm64-v8a" + delimiter 
	print(cmd)
        os.system(cmd)
   
    ### armeabi
        cmd = cpy + share_lib_dir + "armeabi" + delimiter + share_lib_name + "  " + out_sha_lib + "armeabi" + delimiter 
	print(cmd)
        os.system(cmd)
        
    ### x86
        cmd = cpy + share_lib_dir + "x86" + delimiter + share_lib_name + "  " + out_sha_lib + "x86" + delimiter 
	print(cmd)
        os.system(cmd)
        
    ### x86_64
        cmd = cpy + share_lib_dir + "x86_64" + delimiter + share_lib_name + "  " + out_sha_lib + "x86_64" + delimiter 
	print(cmd)
        os.system(cmd)
        
    ### mips
        cmd = cpy + share_lib_dir + "mips" + delimiter + share_lib_name + "  " + out_sha_lib + "mips" + delimiter 
	print(cmd)
        os.system(cmd)  

    ### mips64
        cmd = cpy + share_lib_dir + "mips64" + delimiter + share_lib_name + "  " + out_sha_lib + "mips64" + delimiter 
	print(cmd)
        os.system(cmd)         
        
	# "==================BUILD STATIC LIBRARY===================="
	### armeabi-v7a
        cmd = ndk_build + " -B " + "NDK_PROJECT_PATH:=" + android_dir + " APP_BUILD_SCRIPT:=" + mk_dir + delimiter + "Android_lib.mk" + " V=1 SHARED=0" + " PURE_C=" + opt
	print(cmd)
	os.system(cmd)
	cmd = cpy + static_lib_dir + "armeabi-v7a" + delimiter + static_lib_name + "  " + out_sta_lib + "armeabi-v7a" + delimiter
	print(cmd)
	os.system(cmd)
    
    ### arm64-v8a
        cmd = cpy + static_lib_dir + "arm64-v8a" + delimiter + static_lib_name + "  " + out_sta_lib + "arm64-v8a" + delimiter 
	print(cmd)
        os.system(cmd)
    
    ### armeabi
        cmd = cpy + static_lib_dir + "armeabi" + delimiter + static_lib_name + "  " + out_sta_lib + "armeabi" + delimiter 
	print(cmd)
        os.system(cmd)
        
    ### x86
        cmd = cpy + static_lib_dir + "x86" + delimiter + static_lib_name + "  " + out_sta_lib + "x86" + delimiter 
	print(cmd)
        os.system(cmd)
    
    ### x86_64
        cmd = cpy + static_lib_dir + "x86_64" + delimiter + static_lib_name + "  " + out_sta_lib + "x86_64" + delimiter 
	print(cmd)
        os.system(cmd)
    
    ### mips
        cmd = cpy + static_lib_dir + "mips" + delimiter + static_lib_name + "  " + out_sta_lib + "mips" + delimiter 
	print(cmd)
        os.system(cmd)
    
    ### mips64
        cmd = cpy + static_lib_dir + "mips64" + delimiter + static_lib_name + "  " + out_sta_lib + "mips64" + delimiter 
	print(cmd)
        os.system(cmd)
    
	# "==================BUILD STATIC EXE ===================="
	cmd = ndk_build + " -B " + "NDK_PROJECT_PATH:=" + android_dir + " APP_BUILD_SCRIPT:=" + mk_dir + delimiter + "Android_app.mk"
	print(cmd)
        ### armeabi-v7a
	os.system(cmd)
	cmd = cpy + static_lib_dir + "armeabi-v7a" + delimiter + static_exe + "  " + out_sta_lib + "armeabi-v7a" + delimiter
	print(cmd)
	os.system(cmd)
        ### arm64-v8a
        cmd = cpy + static_lib_dir + "arm64-v8a" + delimiter + static_exe + "  " + out_sta_lib + "arm64-v8a" + delimiter
	print(cmd)
	os.system(cmd)
    
    ### armeabi
        cmd = cpy + static_lib_dir + "armeabi" + delimiter + static_exe + "  " + out_sta_lib + "armeabi" + delimiter
	print(cmd)
	os.system(cmd)
    
    ### x86
        cmd = cpy + static_lib_dir + "x86" + delimiter + static_exe + "  " + out_sta_lib + "x86" + delimiter
	print(cmd)
	os.system(cmd)
    
    ### x86_64
        cmd = cpy + static_lib_dir + "x86_64" + delimiter + static_exe + "  " + out_sta_lib + "x86_64" + delimiter
	print(cmd)
	os.system(cmd)
    
    ### mips
        cmd = cpy + static_lib_dir + "mips" + delimiter + static_exe + "  " + out_sta_lib + "mips" + delimiter
	print(cmd)
	os.system(cmd)
    
    ### mips64
        cmd = cpy + static_lib_dir + "mips64" + delimiter + static_exe + "  " + out_sta_lib + "mips64" + delimiter
	print(cmd)
	os.system(cmd)
    
    # "==================COPY build/android/OUT To OUT ===================="
        ### armeabi-v7a
        #outdir = final_out_lib + "armeabi-v7a"
        #isExist = os.path.exists(outdir)
	#if(not isExist):
        #        os.mkdir(outdir)
        cmd = cpy + out_sta_lib + "armeabi-v7a" + delimiter + "  " + final_out_lib
        print(cmd)
        os.system(cmd)

        ### arm64-v8a
        #outdir = final_out_lib + "arm64-v8a"
        #isExist = os.path.exists(outdir)
	#if(not isExist):
        #        os.mkdir(outdir)
        cmd = cpy + out_sta_lib + "arm64-v8a" + delimiter + "  " + final_out_lib 
        print(cmd)
        os.system(cmd)


        ### armeabi
        #outdir = final_out_lib + "armeabi"
        #isExist = os.path.exists(outdir)
	#if(not isExist):
        #        os.mkdir(outdir)
        cmd = cpy + out_sta_lib + "armeabi" + delimiter + "  " + final_out_lib
        print(cmd)
        os.system(cmd)

        ### x86
        #outdir = final_out_lib + "x86"
        #isExist = os.path.exists(outdir)
	#if(not isExist):
        #        os.mkdir(outdir)
        cmd = cpy + out_sta_lib + "x86" + delimiter + "  " + final_out_lib
        print(cmd)
        os.system(cmd)

        ### x86_64
        #outdir = final_out_lib + "x86_64"
        #isExist = os.path.exists(outdir)
	#if(not isExist):
        #        os.mkdir(outdir)
        cmd = cpy + out_sta_lib + "x86_64" + delimiter + "  " + final_out_lib
        print(cmd)
        os.system(cmd)

        ### mips
        #outdir = final_out_lib + "mips"
        #isExist = os.path.exists(outdir)
	#if(not isExist):
        #        os.mkdir(outdir)
        cmd = cpy + out_sta_lib + "mips" + delimiter + "  " + final_out_lib
        print(cmd)
        os.system(cmd)

        ### mips64
        #outdir = final_out_lib + "mips64"
        #isExist = os.path.exists(outdir)
	#if(not isExist):
        #        os.mkdir(outdir)
        cmd = cpy + out_sta_lib + "mips64" + delimiter + "  " + final_out_lib
        print(cmd)
        os.system(cmd)
        
##############################main函数入口######################
if __name__ == '__main__':
	if(len(argv) < 3):
		printf("Tips: build_android.py android_ndk_dir pure_c=0/1\n")
		exit()
	delimiter =os.path.sep
	ndk_dir = argv[1]
	pure_c = argv[2]
	android_dir = os.getcwd()
	process(ndk_dir,android_dir,pure_c,delimiter)

