@echo off

IF EXIST "%build_dir%" (
    del /s /f /q "%build_dir%" > NUL
    rmdir /s /q "%build_dir%"
)

