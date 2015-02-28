
set VsCmdSetupBat="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\VsDevCmd.bat"
set MSBuildExe="C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe"
set CMakeExe="D:\Program Files\CMake\bin\cmake.exe"


call %VsCmdSetupBat%

mkdir ".\build_vs2008_tmp"
cd "build_vs2008_tmp"
%CMakeExe% ".." -G "Visual Studio 9 2008"
%MSBuildExe% "LuminoMath.sln" /p:Configuration=Debug
%MSBuildExe% "LuminoMath.sln" /p:Configuration=Release
"Debug\UnitTest.exe"
if "ERRORLEVEL" LEQ "0" goto UNITTEST_FAILED
"Release\UnitTest.exe"
if "ERRORLEVEL" LEQ "0" goto UNITTEST_FAILED
cd ".."

mkdir ".\build_vs2010_tmp"
cd "build_vs2010_tmp"
%CMakeExe% ".." -G "Visual Studio 10"
%MSBuildExe% "LuminoMath.sln" /p:Configuration=Debug
%MSBuildExe% "LuminoMath.sln" /p:Configuration=Release
"Debug\UnitTest.exe"
if "ERRORLEVEL" LEQ "0" goto UNITTEST_FAILED
"Release\UnitTest.exe"
if "ERRORLEVEL" LEQ "0" goto UNITTEST_FAILED
cd ".."

mkdir ".\build_vs2012_tmp"
cd "build_vs2012_tmp"
%CMakeExe% ".." -G "Visual Studio 11"
%MSBuildExe% "LuminoMath.sln" /p:Configuration=Debug
%MSBuildExe% "LuminoMath.sln" /p:Configuration=Release
"Debug\UnitTest.exe"
if "ERRORLEVEL" LEQ "0" goto UNITTEST_FAILED
"Release\UnitTest.exe"
if "ERRORLEVEL" LEQ "0" goto UNITTEST_FAILED
cd ".."

mkdir ".\build_vs2013_tmp"
cd "build_vs2013_tmp"
%CMakeExe% ".." -G "Visual Studio 12"
%MSBuildExe% "LuminoMath.sln" /p:Configuration=Debug
%MSBuildExe% "LuminoMath.sln" /p:Configuration=Release
"Debug\UnitTest.exe"
if "ERRORLEVEL" LEQ "0" goto UNITTEST_FAILED
"Release\UnitTest.exe"
if "ERRORLEVEL" LEQ "0" goto UNITTEST_FAILED
cd ".."

pause
exit

:UNITTEST_FAILED
echo "UnitTest failed."
pause
exit
