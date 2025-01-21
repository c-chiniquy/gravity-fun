REM This batch file builds the project for Visual Studio 2019.
cmake -B build -G "Visual Studio 16 2019" -A x64
start build/gravity-fun.sln
