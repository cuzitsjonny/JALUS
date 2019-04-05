@echo off

rem Register Environment variables for Borland C++
rem set BCCDIR=C:\Borland\BC502
rem set BCCDIR=D:\Borland\BCC55
rem set BCCDIR="C:\Program Files\Borland\BDS\4.0"
rem set BCCDIR="C:\Program Files\CodeGear\RAD Studio\5.0"
rem set BCCDIR="C:\Program Files\CodeGear\RAD Studio\6.0"
rem set BCCDIR="C:\Program Files\Embarcadero\RAD Studio\8.0"
rem set BCCDIR="C:\Program Files (x86)\Embarcadero\RAD Studio\8.0"
rem set BCCDIR="C:\Program Files (x86)\Embarcadero\RAD Studio\9.0"
rem set BCCDIR="C:\Program Files (x86)\Embarcadero\RAD Studio\10.0"
rem set BCCDIR="C:\Program Files (x86)\Embarcadero\RAD Studio\11.0"
rem set BCCDIR="C:\Program Files (x86)\Embarcadero\RAD Studio\12.0"
rem set BCCDIR="C:\Program Files (x86)\Embarcadero\Studio\14.0"
set BCCDIR="C:\Program Files (x86)\Embarcadero\Studio\15.0"
rem set BCCDIR="C:\Program Files (x86)\Embarcadero\Studio\16.0"
rem set BCCDIR="C:\Program Files (x86)\Borland\CBuilder6"
rem For Borrland 5.02
rem PATH=%BCCDIR%\Bin
PATH=%BCCDIR%\Bin;%PATH%

rem ***********************************
rem Special build configuration defines
rem ***********************************
rem CFG=(relese|debug)
rem CFG_LINK=(dynamic|static)
rem SA_RUNTIME=(dynamic|static)
rem SA_UNICODE		build UNICODE version
rem OBJS		specify test program object file (sqltest.obj by default)
rem SA_64BIT		build 64-bit version
rem ***********************************
set SA_OPTIONS=

echo ------------------------
echo --- Building release ---
echo ------------------------
make /f Makefile.bcc %SA_OPTIONS% CFG=release CFG_LINK=dynamic safe_clean all
make /f Makefile.bcc %SA_OPTIONS% CFG=release CFG_LINK=static safe_clean all

echo ----------------------
echo --- Building debug ---
echo ----------------------
make /f Makefile.bcc %SA_OPTIONS% CFG=debug CFG_LINK=dynamic safe_clean all
make /f Makefile.bcc %SA_OPTIONS% CFG=debug CFG_LINK=static safe_clean all

