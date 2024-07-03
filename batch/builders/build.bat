@echo off

cd /d %proj_path%

echo.
echo ------------BUILDING------------
echo.

cd build
cmake --build . --config %1

echo.
if %ERRORLEVEL% == 0 (
    echo BUILD SUCCESS
) else (
    echo BUILD FAILURE
)

cd ..