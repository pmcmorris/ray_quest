@echo off
echo Mapping R: to '%CD%'
subst r: "%CD%"
r:
echo run bin\init_peter.bat
echo run bin\init_patrick.bat
echo run bin\init_andrew.bat

