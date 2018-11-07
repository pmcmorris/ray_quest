@echo off

:: what we are building
set target=quest

:: windows file extensions
set filetype_exe=.exe
set filetype_obj=.obj
set filetype_lib=.lib

:: root path of the project
set project_path=%CD%
:: remove a trailing slash if present (e.g. can happen when at the root of a drive)
if %project_path:~-1%==\ set project_path=%project_path:~0,-1%

:: shorthand for common locations
set build_dir=%project_path%\build
set data_dir=%project_path%\data
set src_dir=%project_path%\src
set bin_dir=%project_path%\bin

:: add our scripts dir to the path
path==%bin_dir%;%PATH%

