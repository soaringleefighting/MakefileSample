@rem ˵����Windowsƽ̨�����б���ű�
@rem ���ã�ͨ��������ű������б���VS����
@echo off
@rem usage: build.bat Release/Debug Win32/x64
 

@rem set build_config="Debug|Win32"
@rem set build_config="Release|Win32"
@rem set build_config=$ALL

@rem ��ǰ��������ֻ�ڱ��ļ�����Ч
@setlocal
@rem "�������������"
set build=%1
set platform=%2
set build_config="%build%|%platform%"

@echo %date% %time%  >build.log

@rem ��ǰ�ű�����·��
@echo "����Ŀ¼..."
@echo %~dp0 
set build_root = %~dp0

@rem �����ļ���·��
set VCVARS="%VS100COMNTOOLS%../../VC/vcvarsall.bat"
set DEVENV="%VS100COMNTOOLS%../IDE/devenv.exe"

@rem ���ý������·��
set SOlUTION="%~dp0Transpose_alg/AVSample.sln"
@echo "�������..."
@echo %SOLUTION%

@rem ����ļ�·������
if not exist %VCVARS% echo "VCVARS�����ڣ�" &goto end
if not exist %DEVENV% echo "DEVENV�����ڣ�" &goto end
if not exist %SOlUTION% echo "SOlUTION�����ڣ�" &goto end

@rem ɾ������ļ�
if exist build.log del build.log

@rem ���뻷������
@echo "��������..."
if %platform%=="Win32" (
     echo "Windows 32λ��������..."
     call %VCVARS% x86)
   
if %platform%=="x64" (
	 echo "Windows 64λ��������..."
	 call %VCVARS% x86_amd64)

@echo build_config:%build_config%,please waiting

@echo "��ʼ����" 
%DEVENV% %SOlUTION% /rebuild %build_config% /out build.log
@rem notepad build.log
@rem copy %~dp0bin\xvidcore.dll  %~dp0..\lib\
@rem copy %~dp0bin\xvidcore.lib  %~dp0..\lib\
@echo "����ɹ���"

:end
@rem pause
@endlocal
