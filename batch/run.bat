@REM Execute release build

@echo off

cd /d %proj_path%

echo.
echo ------------RUNNING-------------
echo.

if not exist %proj_path%\build\%1\Boids.exe (
    echo PROGRAM NOT BUILT

    @REM Exit without closing cmd
    exit /b 1
)

cd %proj_path%\build\%1\
call %proj_path%\build\%1\Boids.exe
set _errorlevel=%ERRORLEVEL%
cd ..\..

echo.
echo Program executed successfully
echo Exit Code: 0x%=ExitCode% (%_errorlevel%)