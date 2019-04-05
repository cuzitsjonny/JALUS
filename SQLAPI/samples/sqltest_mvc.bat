@echo off

rem Register Environment variables for MSVC
rem call "C:\Program Files (x86)\Microsoft Visual Studio\VC98\Bin\VCVARS32.BAT"
rem call "C:\Program Files\Microsoft Visual Studio .NET\VC7\Bin\VCVARS32.BAT"
rem call "c:\Program Files\Microsoft Visual Studio .NET 2003\VC7\Bin\VCVARS32.BAT"
rem call "c:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat" x86
rem call "c:\Program Files (x86)\Microsoft Visual Studio 8\VC\vcvarsall.bat" x86_amd64
rem call "c:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
rem call "c:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
rem call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
rem call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
rem call "c:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_x64
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64

rem ***********************************
rem Special build configuration defines
rem ***********************************
rem CFG=(relese|debug)
rem CFG_LINK=(dynamic|static)
rem SA_RUNTIME=(dynamic|static)	build with static/dynamic RTL/LIBC
rem SA_UNICODE		build UNICODE version
rem OBJS		specify test program object file (sqltest.obj by default)

set SA_OPTIONS=SA_64BIT=1 SA_UNICODE=1 OBJS=sqltest.obj

echo ------------------------
echo --- Building release ---
echo ------------------------
nmake /nologo /f Makefile.mvc %SA_OPTIONS% CFG=release CFG_LINK=dynamic safe_clean all
nmake /nologo /f Makefile.mvc %SA_OPTIONS% CFG=release CFG_LINK=static safe_clean all

echo ----------------------
echo --- Building debug ---
echo ----------------------
nmake /nologo /f Makefile.mvc %SA_OPTIONS% CFG=debug CFG_LINK=dynamic safe_clean all
nmake /nologo /f Makefile.mvc %SA_OPTIONS% CFG=debug CFG_LINK=static safe_clean all

