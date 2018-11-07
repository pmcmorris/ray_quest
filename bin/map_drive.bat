@echo off
echo "Mapping V: to '%CD%'"
subst v: "%CD%"
v:
echo "run bin\init_peter.bat"
echo "run bin\init_patrick.bat"
echo "run bin\init_andrew.bat"

