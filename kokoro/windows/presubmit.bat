@echo on

SETLOCAL ENABLEDELAYEDEXPANSION

SET BUILD_ROOT=%cd%
SET PATH=C:\python36;C:\Program Files\cmake\bin;%PATH%
SET SRC=%cd%\github\cppdap

cd %SRC%
if !ERRORLEVEL! neq 0 exit !ERRORLEVEL!

git submodule update --init
if !ERRORLEVEL! neq 0 exit !ERRORLEVEL!

SET MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild"
SET CONFIG=Release

mkdir %SRC%\build
cd %SRC%\build
if !ERRORLEVEL! neq 0 exit !ERRORLEVEL!

IF /I "%BUILD_SYSTEM%"=="cmake" (
    cmake .. -G "%BUILD_GENERATOR%" "-DCPPDAP_BUILD_TESTS=1" "-DCPPDAP_BUILD_EXAMPLES=1" "-DCPPDAP_WARNINGS_AS_ERRORS=1"
    if !ERRORLEVEL! neq 0 exit !ERRORLEVEL!
    %MSBUILD% /p:Configuration=%CONFIG% cppdap.sln
    if !ERRORLEVEL! neq 0 exit !ERRORLEVEL!
    Release\cppdap-unittests.exe
    if !ERRORLEVEL! neq 0 exit !ERRORLEVEL!
) ELSE (
    echo "Unknown build system: %BUILD_SYSTEM%"
    exit /b 1
)
