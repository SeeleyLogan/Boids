@echo off
set proj_path=%CD%
set path=%proj_path%\batch;%proj_path%\batch\shortcuts;%proj_path%\batch\builders;%path%

cd build
cmake ..
cd ..