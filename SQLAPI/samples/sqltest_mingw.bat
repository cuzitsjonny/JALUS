@echo off
rem set path=D:\MinGW\4.9.2-tdm64\bin;%PATH%
rem set path=d:\mingw\4.6.3\bin;%PATH%
rem set path=d:\mingw\4.5.1\bin;%PATH%
rem set path=d:\mingw\4.4.1\bin;%PATH%
set path=d:\mingw\3.4.2\bin;%PATH%
rem set PATH=c:\Dev-Cpp\bin;%PATH%
rem set PATH=C:\Program Files\CodeBlocks\MinGW\bin;%PATH%
rem set PATH=c:\mingw\bin;%PATH%

rem ***********************************
rem Special build configuration defines
rem ***********************************
rem CFG=(relese|debug)
rem CFG_LINK=(dynamic|static)
rem OBJS - specify test program object file (sqltest.obj by default)
rem ***********************************
rem SA_64BIT		build 64-bit version
rem SA_32BIT		build 32-bit version
rem ***********************************

set SA_OPTIONS=SA_UNICODE=1 OBJS=bugtest.obj

echo ------------------------
echo --- Building release ---
echo ------------------------
mingw32-make -f Makefile.mingw %SA_OPTIONS% CFG=release CFG_LINK=dynamic safe_clean all	
mingw32-make -f Makefile.mingw %SA_OPTIONS% CFG=release CFG_LINK=static safe_clean all	

echo ----------------------
echo --- Building debug ---
echo ----------------------
mingw32-make -f Makefile.mingw %SA_OPTIONS% CFG=debug CFG_LINK=dynamic safe_clean all	
mingw32-make -f Makefile.mingw %SA_OPTIONS% CFG=debug CFG_LINK=static safe_clean all	
