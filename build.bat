@echo off

::Build project
call :build || goto handleError

::Exit batch script
goto exit

::--------------------------
::|Function defintions below
::--------------------------

::Function that builds the project using make
:build
echo "Building project"
cd LibPSVR
make lib
tree /f
goto:eof

:handleError
echo "BUILD FAILED"
exit /b 1
goto:eof

:exit
echo "BUILD SUCCESSFUL"
exit /b 0
goto:eof
