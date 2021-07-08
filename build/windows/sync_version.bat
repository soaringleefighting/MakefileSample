echo off
@rem chcp 936
SetLocal EnableDelayedExpansion

echo 运行路径: %cd%
echo 脚本路径: %0
set scriptPath=%~dp0
set versionPath=%~dp0..\..\src



echo 获取Git版本信息...
set variable_file=%scriptPath%temp_variable.txt
echo %variable_file%
echo. 2>%variable_file%

git branch >> %variable_file%
git log -1 --pretty=format:"%%n%%h%%n%%ci%%n" >> %variable_file%

set branch_name="master"
set commit_hash="00000"
set commit_date="19700101"


set count=1
for /f "tokens=* delims=\n" %%i in (%variable_file%) do (
	if !count! == 1 ( set branch_name="%%i" )
	if !count! == 2 ( set commit_hash="%%i" ) 
	if !count! == 3 ( set commit_date="%%i" ) 
	set /A count=!count!+1
)


set branch_name=%branch_name: =%
set branch_name=%branch_name:>=%
set branch_name=%branch_name:"=%
set commit_hash=%commit_hash: =%
set commit_hash=%commit_hash:>=%
set commit_hash=%commit_hash:"=%
set commit_date=%commit_date: =%
set commit_date=%commit_date:>=%
set commit_date=%commit_date:"=%
	
echo 分支： %branch_name%
echo 哈希:  %commit_hash%
echo 日期： %commit_date%


if exist %variable_file% del %variable_file%


echo 生成最新版本信息文件...
set new_revision_file="%scriptPath%RevisionNew.h"
@rem echo %new_revision_file%

echo. 2>%new_revision_file%
for /f "tokens=*" %%i in ('type "%scriptPath%\Revision.t"') do (
	set str=%%i
	set str=!str:$SG_BRANCH_NAME$=%branch_name%!
	set str=!str:$SG_COMMIT_HASH$=%commit_hash%!
	set str=!str:$SG_COMMIT_DATE$=%commit_date%!
	echo !str! >>%new_revision_file%
)



echo 比较新旧版本信息文件...
set old_revision_file=%versionPath%\version.h
if not exist %old_revision_file% ( 
	echo 不存在旧版本信息文件，直接拷贝新文件
	copy /Y %new_revision_file% %old_revision_file%
	goto end
)


fc /n %new_revision_file% %old_revision_file%
::

if %errorlevel% == 1 (
	echo 版本有更新，拷贝新文件！
	copy /Y %new_revision_file% %old_revision_file%
) else if %errorlevel% NEQ 0 (
	echo 比较失败!
) else (
	echo 版本文件已经是最新！
)


:end

if exist %new_revision_file% del %new_revision_file%