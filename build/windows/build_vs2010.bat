@rem 说明：Windows平台命令行编译脚本
@rem 作用：通过批处理脚本命令行编译VS工程
@echo off
@rem usage: build.bat Release/Debug Win32/x64
 

@rem set build_config="Debug|Win32"
@rem set build_config="Release|Win32"
@rem set build_config=$ALL

@rem 当前环境设置只在本文件中有效
@setlocal
@rem "命令行输入参数"
set build=%1
set platform=%2
set build_config="%build%|%platform%"

@echo %date% %time%  >build.log

@rem 当前脚本所在路径
@echo "编译目录..."
@echo %~dp0 
set build_root = %~dp0

@rem 各种文件和路径
set VCVARS="%VS100COMNTOOLS%../../VC/vcvarsall.bat"
set DEVENV="%VS100COMNTOOLS%../IDE/devenv.exe"

@rem 设置解决方案路径
set SOlUTION="%~dp0Transpose_alg/AVSample.sln"
@echo "解决方案..."
@echo %SOLUTION%

@rem 检查文件路径配置
if not exist %VCVARS% echo "VCVARS不存在！" &goto end
if not exist %DEVENV% echo "DEVENV不存在！" &goto end
if not exist %SOlUTION% echo "SOlUTION不存在！" &goto end

@rem 删除输出文件
if exist build.log del build.log

@rem 编译环境配置
@echo "环境配置..."
if %platform%=="Win32" (
     echo "Windows 32位环境配置..."
     call %VCVARS% x86)
   
if %platform%=="x64" (
	 echo "Windows 64位环境配置..."
	 call %VCVARS% x86_amd64)

@echo build_config:%build_config%,please waiting

@echo "开始编译" 
%DEVENV% %SOlUTION% /rebuild %build_config% /out build.log
@rem notepad build.log
@rem copy %~dp0bin\xvidcore.dll  %~dp0..\lib\
@rem copy %~dp0bin\xvidcore.lib  %~dp0..\lib\
@echo "编译成功！"

:end
@rem pause
@endlocal
