@echo off

::Initialise
set PROJECT_ROOT=%cd%

::Generate the project files
call :generateProjectFiles || goto handleError

::Exit batch script
goto exit

::Function generates project files for the configured ide
:generateProjectFiles
@echo off
IF NOT EXIST %PROJECT_ROOT%\generated mkdir %PROJECT_ROOT%\generated
pushd %PROJECT_ROOT%\generated
echo "Rebuilding Project files..."
echo "Running cmake in %PROJECT_ROOT%"
cmake .. -G "Visual Studio 17 2022"
popd
goto:eof

:handleError
echo "GENERATE FAILED"
exit /b 1
goto:eof

:exit
echo "GENERATE SUCCESSFUL"
exit /b 0
goto:eof