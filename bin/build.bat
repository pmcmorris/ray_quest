@echo off

set C_FLAGS=-Wall -pedantic -std=c99 -g -O3
set C_LIBS=-l gdi32
::set C_LINK=-Wl,--subsystem,windows
set C_LINK=-Wl,--subsystem,console

:: path for the executable were building
set output_path=%build_dir%\%target%%filetype_exe%

:: ensure the build folder exists
md %build_dir% 2> NUL

:: build the game
gcc %C_FLAGS% %src_dir%\bulk.c -o %output_path% %C_LIBS% %C_LINK%

