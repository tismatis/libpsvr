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
cmake --build generated --target INSTALL --config RELEASE
goto:eof

:handleError
echo "BUILD FAILED"
exit /b 1
goto:eof

:exit
echo "BUILD SUCCESSFUL"
exit /b 0
goto:eof
