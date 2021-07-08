echo off
chcp 936
SetLocal EnableDelayedExpansion

echo run_dir: %cd%
echo bat_dir: %0
set scriptPath=%~dp0
set versionPath=%~dp0..\..\src



echo Get git version info...
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
	
echo Branch: %branch_name%
echo Hash  : %commit_hash%
echo Date  : %commit_date%


if exist %variable_file% del %variable_file%


echo Generate the newest version file
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



echo Compare version info
set old_revision_file=%versionPath%\version.h
if not exist %old_revision_file% ( 
@rem 不存在旧版本信息文件，直接拷贝新文件
	echo not exist old version file, just copy new file!
	copy /Y %new_revision_file% %old_revision_file%
	goto end
)


fc /n %new_revision_file% %old_revision_file%
::

if %errorlevel% == 1 (
@rem 版本有更新，拷贝新文件
	echo version changed, copy new file!
	copy /Y %new_revision_file% %old_revision_file%
) else if %errorlevel% NEQ 0 (
	echo compare failed!
) else (
	echo version file already updated!
)


:end

if exist %new_revision_file% del %new_revision_file%