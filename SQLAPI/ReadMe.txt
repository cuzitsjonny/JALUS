****************************************************************
               Welcome to SQLAPI++ Library 4.2.5.2
****************************************************************

1. INTRODUCTION
Thank you for using SQLAPI++ Library.
This file contains important notes about the installation and the
product itself.

2a. INSTALLATION (Windows version)
Files under 'SQLAPI' folder after unpacking the SQLAPI++ self-extracting archive:

ReadMe.txt 		- this file
license.txt		- SQLAPI++ license agreement and warranty
bin\*.*			- SQLAPI++ DLL and sample binaries (read more at bin\index.txt)
lib\*.*			- dynamic and static librares (read more at lib\index.txt)
include\*.h		- public include files
doc\*.*			- SQLAPI++ documentation in HTML format
examples\*.cpp		- SQLAPI++ steps
vs????\*		- SQLAPI++ precompiled binaries for Microsoft Visual Studio 2005 (x86 and x86-64)
MinGW-?.?\*		- SQLAPI++ precompiled binaries for MinGW g++ (x86 and x86-64)
bcc-cbxe?\*		- SQLAPI++ precompiled binaries for Embarcadero RAD Studio (x86 and x86-64)

These files are only for registered SQLAPI++ version:

src\*.cpp		- SQLAPI++ sources
src\*.h			- SQLAPI++ private headers
src\Makefile.mvc	- SQLAPI++ makefile for MSVC++
src\sqlapi_msvc.bat	- batch file to compile SQLAPI++ (release & debug) for MSVC++
src\Makefile.bcc	- SQLAPI++ makefile for Borland C++ compiler
src\sqlapi_bcc.bat	- batch file to compile SQLAPI++ (release & debug) for Borland C++ compiler
src\Makefile.mingw	- SQLAPI++ makefile for MinGW
src\sqlapi_mingw.bat	- batch file to compile SQLAPI++ (release & debug) for MinGW

SQLAPI++ binaries are compiled with Microsoft Visual Studio (x86 and x64).
The multithread- and DLL-specific version of the run-time libraries are used.
You must recompile the binaries for another run-time library configurations.
The same is true for Borland/Embarcadero and MinGW binaries.

2b. INSTALLATION (Linux/Unix version)
Files after the installation:
ReadMe.txt		- this file
license.txt		- SQLAPI++ license agreement and warranty
bin\*			- simple executable to test database connection
doc\*.*			- SQLAPI++ documentation in HTML format
examples\*.cpp		- SQLAPI++ steps
include\*.h		- public include files
lib\*.*			- shared and static SQLAPI++ libraries
g++-?.?.tar.gz		- archives of SQLAPI++ binaries compiled with different g++ versions (x86 and amd64)

These files are only for registered SQLAPI++ version:
src\*.cpp		- SQLAPI++ sources
src\*.h			- SQLAPI++ private headers
src\Makefile		- SQLAPI++ makefile for GNU make
src\sqlapi_gcc		- batch file to compile SQLAPI++ (release & debug)
src\Makefile.ss		- SQLAPI++ makefile for Solaris Studio C++ compiler (GNU 'make' required)
src\sqlapi_ss		- batch file to compile SQLAPI++ Solaris Studio C++ compiler (release & debug)

We recommend to recompile the SQLAPI++ binaries on platform it should be used.

3. LATEST CHANGES

Version 4.2.5.2
-- SQL Anywhere: Fixes crash when unimplemented scrollable fetching used (thanks Christian Schmitz).
-- Oracle: Fixed external connection handles attaching (thanks Frank Hilliger).
-- General: sa_strncpy/sa_wcsncpy related fixes (thanks Manuel Baerten).

Version 4.2.5
-- General: Fixed parameter by name searching (thanks Frank Hilliger).
-- SQL Anywhere: Fixed API initialization on Linux/Unix (thanks Christian Schmitz).
-- General: Use safe CRT/LIBC functions when available.
-- SQLite: Fixed default date/time type search option value (thanks Christian Schmitz).
-- General: Added SAException::CommandText() method.
-- General: Fixed STATIC DBMS API loading (thanks Manfred Kubica).
-- SQLServer (OLEDB): Fixed bug with multiple connections.
-- ODBC: API code reorganized.
-- PostgreSQL: Added support for stored procedures.
-- General: Throw SAException instead or assertion when required DBMS API function cannot be loaded.
-- PostgreSQL: Do not destroy the result set returned by function so it's possible to process for example 'return table...' results (thanks Mike Moening).
-- PostgreSQL: Improved function/procedure parameters detection for the current/default schema (thanks Mike Moening).
-- ODBC: Increased the field/parameter length that should be converted to SA_dtString (helps with some buggy drivers, thanks Christian Schmitz).
-- General: Fixed using native API on Linux/Unix (thanks Peter Klotz).

Version 4.2.4
-- SQLServer (OLEDB): API code reorganized.
-- SQLServer (ODBC): API code reorganized.
-- SQLBase: API code reorganized.
-- Interbase: API code reorganized.
-- SQL Anywhere: API code reorganized.
-- DB2: API code reorganized (special "STATIC" value for "DB2CLI.LIBS" option).
-- Informix: API code reorganized (special "STATIC" value for "INFCLI.LIBS" option).
-- Oracle: OCI8 code reorganized (special "STATIC" value for "OCI8.LIBS" option).
-- Oracle: Added support for Implicit Results (thanks Georgiy Pakhutin).
-- Oracle: Do not read timezone data for for the date/type types without it because this is too slow (thanks Sven REUTTER).
-- Sybase: API code reorganized (special "STATIC" value for "SYBCT.LIBS" option).
-- CubeSQL: API code reorganized (special "STATIC" value for "CUBESQL.LIBS" option).
-- PostgreSQL: API code reorganized (special "STATIC" value for "LIBPQ.LIBS" option).
-- SQLite: Added code to detect if the field in the result set is required (thanks Karoly Harmath).
-- SQLite: API code reorganized (special "STATIC" value for "SQLITE.LIBS" option).
-- MySQL: API code reorganized (special "STATIC" value for "MYSQL.LIBS" option).
-- General: Sources split.
-- SQLServer (ODBC): Fixed multi-result command procesing (Brendan Wilson).
-- ODBC: Fixed input string buffer processing (thanks Alexander Goldnik).
-- SQLServer (ODBC): Fixed bug with stored procedures have the output parameters and result sets with LOB fields (thanks Mike Moening).

Version 4.2.3
-- General: Added SAConnection::GetNextCommand method (enumerates SACommand-s).
-- General: SA_USE_STL build option uses a map for input parameter names (improved performance for parameter by name searching).
-- CubeSQL: Fixed setAutoCommit method (thanks Christian Schmitz).
-- ODBC: Fixed input buffer length for SA_dtString (thanks Christian Schmitz).
-- Sybase: Fixed connection problem related to OCS error messages 5701 and 5704 (thanks CK Yang).
-- General: Optimized parameters parsing/creation algorithm for numeric binding parameter names.
-- PostgreSQL: RefCursor implementation (thanks Danielle Gutfinger).
-- CubeSQL: Added cubeSqlAPI::SetTraceFunction method (thanks Christian Schmitz).
-- CubeSQL: Use GetUTF8Chars instead of GetMultiByteChars for Unicode SQLAPI (thanks Christian Schmitz).

Version 4.2.2
-- SQLServer: Support for Microsoft ODBC Driver 17 for SQL Server on Windows.
-- ODBC: Fixed binding terminated null character (thanks Jarek Karciarz).
-- SQL Anywhere: Support for API version 1 (thanks Christian Schmitz).
-- InterBase: Fixed bug for modern InterBase versions without older XSQLVAR_V1 and SQLDA_VERSION1 support.
-- InterBase: Fixed bug with boolean type differences for InterBase and Firebird.

Version 4.2.1
-- CubeSQL: Added experimental support of CubeSQL (http://www.sqlabs.com/).
-- PostgreSQL: Microseconds instead of milliseconds used for the datetime values (thansk Sun-ho Lee).
-- Oracle: Now UNICODE SQLAPI processes "OCI_ATTR_CHARSET_FORM" option so "SQLCS_IMPLICIT" value can be used instead of default "SQLCS_NCHAR". 
-- ODBC: Added "ODBCDisableLongLengthQuery" option to avoid crashes in incorrect ODBC drivers (thanks Christian Schmitz).
-- ODBC: Added odbcExternalConnection class.
-- SQLServer(ODBC): Auto-detect original connection string format.
-- SQLite: Fixed bug with multi-command statement syntax parsing (thanks Christian Schmitz).
-- MySQL: Use character set option before the connection established (username/password can contains not only ASCII characters, thank Bojan Hrnkas).
-- SQL Anywhere: Use the connection mutex with the cursor to prevent 42W22 error.
-- SQL Anywhere: Fixed setup auto-commit mode (thanks Peter Koukoulis).

Version 4.1.12
-- PostgreSQL: Corrected numeric field precision/scale(set -1) length(set 0) values when they are not defined by the server (thanks Peter Koukoulis).
-- General: Added Intel Compiler build scripts (thanks Luca Teodori).
-- SQL Anywhere: Use thread-safe library on Linux/Unix (thanks Peter Koukoulis).
-- Informix: Fixed data length/indicator size (thanks Florian Apolloner)
-- Informix: Fixed memory leak with SQLAttributeCol using according to IBM IT21378 (thanks Florian Apolloner).
-- SQLServer(OLEDB): Added ssOleDbAPI::ProcessSQLServerErrorInfo() property that allows to exclude SQL Server erro iformation processing.
-- MySQL: Added for loading by default also MariaDB client library (thanks Daniel Hideghethy).
-- SQLite: Initail support for multi-command queries.
-- SQLite: Added 'sqlite3_memory_used' and 'sqlite3_memory_highwater' native API functions (thanks Christian Schmitz).
-- SQL Anywhere: SAConnection::setAutoCommit() command fixed (thanks Peter Koukoulis).
-- SQLServer(ODBC): Added "SQL_COPT_SS_PRESERVE_CURSORS" connection option.
-- General: Fixed character parameter type binding for all ODBC API (thanks Morten Sølvberg).
-- SQLServer(ODBC): Fixed bug with multi-result statemets and LONG/LOB field dadata fetching (thanks Frederick Couste).

Version 4.1.11
-- General: Fixed bug with ODBC/CLI API and multiresult statement field binding (thanks Raymond Allen).
-- General: Added build fiels for IBM xlC compiler, fixed DBMS lib names for AIX (thanks Ralph Dagdag).
-- SQLServer(ODBC): Fixed problem with incorrect driver version number reported by sqlsrv32.dll (thanks Matt Fisher).
-- Informix: Added support for "PreFetchRows" option.
-- ODBC, DB2, Informix, SQLServer(ODBC): added "ExecDirect" SACommand option (use SQLExecDirect instead of SQLExecute).
-- ODBC: fixed SQL_ATTR_ROWS_FETCHED_PTR attribute setup bug (thanks Ivano Cassis).
-- PostgreSQL: Added "SetCursorName" command option.
-- SQLServer (ODBC): Added support for Microsoft ODBC Driver 13 for SQL Server.
-- ODBC: Fixed long text data length detection for Unicode variant (thanks Bjoern Eggstein).
-- SQL Anywhere: Fixed bug with procedure/function parameters parsing (thanks Trent Bowman).
-- General: Fixed UTF-8 mode of SAMultibyte2UnicodeConverter (thanks Emma Qin)
-- General: Fixed quoted parameter name parsing (like :"my param", thanks Vyacheslav E.)
-- InterBase: Fixed UTF-8 character set name (thanks Maria Harmon).

Version 4.1.10
-- General: Fixed SAInterval internals.
-- MySQL: Fixed statement API SA_dtInteral binding (thanks Sophia Wang).
-- SQL Anywhere: Fixed bug with prepared statement execution (thanks Brad Blankenburg).
-- SQLite: Added sqlite3_load_extension and sqlite3_free API functions.
-- Sybase: Added CS_SEC_ENCRYPTION, CS_SEC_EXTENDED_ENCRYPTION, CS_SEC_NON_ENCRYPTION_RETRY connection options.
-- MySQL: Fixed bug with the first SACommand::FetchPrior() call (thanks Dave Sulentic).
-- PostgreSQL: Ignore unknown date/time formats (thanks Maria Harmon).
-- MySQL: For multi-thread version try to load also MYSQL client lirary name without "_r" suffix (thanks Michael Noe).
-- General: Added SA_STATIC_PGSQL build option.
-- MySQL: Added MYSQL_SHARED_MEMORY_BASE_NAME connection option.

Version 4.1.9
-- Oracle: Fixed bug with CLOB reading and using multi-byte client encoding (thanks Peter Klotz).

Version 4.1.8
-- Sybase: Fixed bug with ASE 16.x version detection (thanks Frank Hilliger).
-- General: Fixed utf8.c code for correct results with and without output buffer, Unicode 3.0 standards (thanks Peter Klotz).
-- General: Fixed SAComand::Field(index) throws SAException when index is wrong (Christian Schmitz).
-- PostgreSQL: fixed using "APPNAME" connection option when connection string dbname already includes other options.
-- Oracle: try to get the LOB size before read.
-- General: added missed 'SAValueRead::operator SANumeric() const' body (thanks Balázs Kádár).
-- General: fixed SIZE_MAX definition bug (thanks Peter Klotz).
-- General: Fixed the SAString::SetUTF16Chars method for correct new []/delete [] (thanks Matt Feemster).

Version 4.1.7
-- General: Fixed the query text comments parsing (thanks Georgiy Pakhutin).
-- SQLServer(OLEDB): Fixed zero-scale numeric reading for SQLCE (thanks Gerardo Goitiandia).
-- Oracle: fixed INTERVAL data reading (thanks Leo Namuco).
-- SQLite: fixed binding 64-bit integer parameters (thanks Balázs Kádár).
-- MySQL: ignore missed myodbc_remove_escape.

Version 4.1.6
-- ODBC: added ODBCAddLongTextBufferSpace command option (thanks Christian Schmitz).
-- PostgreSQL: fixed function parameters reading.
-- SQLite: added sqlite3_key and sqlite3_rekey API functions (thanks Christian Schmitz).
-- Oracle: connection pool support (EXPERIMENTAL).
-- SQLServer(ODBC): Fixed LOB output procedure parameter reading.
-- Informix: Ignore unknown data types (set field value to NULL).
-- DB2: Ignore unknown data types (set field value to NULL).
-- SQLBase: Ignore unknown data types (set field value to NULL).
-- ODBC: Ignore unknown data types (set field value to NULL).
-- General: Use specific internal SQLAPI library error native codes.
-- Oracle: Added OCI_ATTR_PREFETCH_ROWS and OCI_ATTR_PREFETCH_MEMORY options.
-- General: Added SAException& SAException::operator=(const SAException &other) (thanks Georgiy Pakhutin).
-- MySQL: Added ability to use named pipes connection.
-- DB2: Fixed bug with LONG/LOB 'NULL' field detection on 64-bit platfroms (thanks Frank Hilliger, also fixed for Informix, ODBC and SQLServer/ODBC).

Version 4.1.5
-- Oracle: Added OCI_ATTR_RECEIVE_TIMEOUT, OCI_ATTR_SEND_TIMEOUT conection options.
-- SQLServer(ODBC): Fixed datetime parameter precision issue (thanks Balázs Kádár).
-- PostgreSQL: Fixed SADateTime to internal format conversion at 64-bit (thanks Stefan Csomor).
-- General: Support for MinGW64.
-- InterBase: Fixed LOB reading/writing bug (thanks Pascal Geenens).
-- Oracle: Added ora8ExternalConnection class (thanks Frank Hilliger).
-- General: Fixed SAException constructor for correct nested exception copying (thanks Martin Rötzl).
-- MySQL: Added microseconds support (thanks Javier Cuevas Domingo).
-- General: Added SACommand::FetchPos method (EXPERIMENTAL).
-- Oracle: Added timezone support with SADateTime (thanks Frank Hilliger).
-- General: Added timezone information into SADateTime class (thanks Frank Hilliger).
-- Oracle: Fixed Long/LOB reading issues at multi-threading applications (thanks Niklas Bergh).
-- ODBC: Fixed the error message/code assigning for some drivers (thanks Christian Schmit).
-- SQLite: Fixed the error processing issue at multi-threading applications (thanks Niklas Bergh).
-- Mysql: Added connection option "SkipServerInit" (thanks Pierre-Yves Thomas).
-- SQLite: Added sqlite3_enable_load_extension API function.

Version 4.1.4
-- MySQL: Added support for libmysqlclient.so.18 (thanks Christian Schmitz).
-- MySQL: Fixed user function execution when there is no any input parameter (thanks Sebastian Hempel).
-- PostgreSQL: Fixed the connection error text for SAException (thanks Jesus Malo Poyatos).
-- SQLServer: OLEDB+ODBC API, ignore unknown data types (set field value to NULL).
-- General: Added SAException::NestedException() and SAException::ErrMessage() methods.
-- MySQL: Added SSL parameter used with mysql_ssl_set (thanks Christian Schmitz).
-- General: Added checking the memory allocation result and throwing an exception when allocation fails.
-- General: Fixed problem with IPv6 address using in MySQL and PostgreSQL connection strings (thanks Sridhar Gollapudi).
-- Informix: Fixed long/lob binding (thanks Christian Schmitz).
-- General: Fixed millisecond calculation in SADateTime::GetTimeValue(SYSTEMTIME &st) method (thanks Jocelyn Pelletier).
-- ODBC: Fixed procedure support for sources without schema support (thanks Joris Koster).
-- SQLServer: OLEDB API, added support for ISQLServerErrorInfo (thanks Georgiy Pakhutin).
-- SQLServer: OLEDB API, added support for SACommand::setBatchExceptionPreHandler (thanks Georgiy Pakhutin).
-- General: Added pAddlData parameter for SACommand::setBatchExceptionPreHandler (thanks Georgiy Pakhutin).

Version 4.1.3
-- SQLite: Added ReadLongOrLob support (thanks Jeremy A. Ford).
-- General: Fixed multi-byte(including UTF-8) to Unicode converter.
-- General: Added initial support for SQL Anywhere.
-- SQLite: Added sqlite3_update_hook API function.
-- MySQL: Added 'MYSQL_SECURE_AUTH' connection option (thanks Christian Schmitz).
-- Oracle: Fixed LONG data type reading/writing with Unicode version (thanks Jimmy Michiels).
-- SQLite: Optimized integer data reading (thanks Christian Schmitz).
-- General: Use -fPIC(g++)/-KPIC(Solaris Stidio) compilation option even for the static 64-bit library (thanks Christian Schmitz).
-- Oracle: Fixed crash if using Oracle database with LDAP configuration and OpenLDAP (thanks Alfred Gebert).
-- PostgreSQL: Do not use utf8 encoding for the error messages when the connection is bad (thanks jonathan.gonzalez at kepler.com.mx).
-- ODBC: Fixed procedure parameter parsing (thanks Joris Koster).
-- ODBC: Fixed SANumeric data processing (thanks Max Lipshits).
-- ODBC: Added SACommand option "ODBCUseSQLGetData" (use SQLGetData for any result set fields).
-- ODBC: Use SQLGetData for any field in the result set after long/LOB field.
-- General: Fixed empty string data returned with SAString::GetUTF16Chars (thanks Christian Schmitz).

Version 4.1.2
-- DB2: Use SQL_SSHORT instead of SQL_BIT data binding for SA_dtBool parameters (thanks Frank Hilliger).
-- MySQL: Added per-connection query execution mutex+lock.
-- MySQL: Added mysql_library_init call that resolves thread bug (thanks Michael Hufer and Frank Hilliger).
-- ODBC: Fixed  the first character truncation of long/clob text field on Linux/Unix.
-- General: Renamed all names with  suffix Win32 to Windows one.
-- ODBC: Added build option to configure SQLWCHAR type on Linux/Unix (thanks Christian Schmitz).
-- DB2: Fixed 64-bit SQLLEN/SQLPOINTER and related types data length (thanks Frank Hilliger).
-- PostgreSQL: Fixed long binary/blob binding (thanks slv1970 at me.com).
-- General: Addded ability to trace the real query text sent to DBMS API (experimental, SAGlobals::SetTraceFunction).

Version 4.1.1
-- General: SAString method GetUTF16Chars, GetUTF16CharsLength, SetUTF16Chars available in not-Unicode version.
-- SQLServer: Added support for Microsoft ODBC Driver 11 for SQL Server on Linux.
-- Informix: Fixed bug with data type conversion (thanks Jay Sridhar).
-- InterBase: Fixed the 64-bit DLL name (thanks Junior Tcheho).
-- SQLServer: Fixed problem with binding result set fields after any long/LOB one.
-- General: Fixed ODBC/CLI scrollable cursor fetching with row prefetched > 1 (DB2, Informix, ODBC, MSSQL affected).
-- Oracle: Added "NLS_CHAR" connection option.
-- PostgreSQL: Fixed long binary/blob binding (thanks Yan Dai).
-- General: Fixed 'SANumeric::operator sa_uint64_t()' for Windows (thanks Junior Tcheho).
-- DB2: Removed assertion when SQLGetDescRec is not exported by DB2 ODBC/CLI library (thanks Jes�s Malo Poyatos).
-- DB2: Added workaround for incorrect 64-bit SQLDescribeCol (thanks Jes�s Malo Poyatos).
-- General: Added 'SetCursorName' SACommand option for ODBC/CLI API-s that uses SQLSetCursorName function (thanks Christian Schmitz).

Version 4.0.3
-- Informix: Added Unicode support.
-- SQLServer: Use ODBC API by default instead of OLEDB, ODBC client allowed with MinGw.
-- General: Fixed bug with dummy data converter that can produce LOB/Long data reading problems (thanks Peter Klotz).
-- SQLite: Start transaction indirectly before the first query executed (thanks Frank Hilliger).
-- Sybase: Added 15.7 context support.
-- DB2: Fixed LOB/LONG field reading (thanks Fernand Alcatrao).
-- SQLServer: ODBC API, fixed bug with connection string parsing (thanks Hide Ishikuri).
-- Sybase: Don't using read only cursor by default (thanks Ian Hodgkinson).
-- DB2: Use trusted connection when username is empty.
-- General: Fixed procedure parameters binding (thanks Joel Baby Jose).
-- ODBC: Added Unicode support for Linux/Unix.
-- SQLite: Added #ifdef-#endif for static sqlite3_table_column_metadata (thanks Jesus Malo Poyatos).
-- PostgreSQL: Added PQping/PQpingParams API functions.
-- PostgreSQL: Fixed connection code bug when application name specified (thanks Alexander Horak).
-- SQLite: Fixed problem with string data conversion in Unicode version (thanks Jacob Pedersen).

Version 4.0.2
-- General: Fixed SAString binary lenght calculation (thanks Christian Schmitz).
-- PostgreSQL: Added support for executing PostgreSQL function.
-- General: Bind variable can start with '_' and comment processing added ('--' until end-of-line and /* .. */, thanks Georgiy Pakhutin).
-- PostgreSQL: Added processing for PQsetdbLogin pgoptions parameter passed with the connection string.
-- SQLServer: Added DBPROP_SSCE_DBCASESENSITIVE connection optioon for CompactEdition (thanks Jacob Pedersen).
-- ODBC: Binary field Interpreted as a long binary if the column size is 0 (thanks Scott at touchnet).
-- DB2: Added Unicode support for Linux/Unix (thanks Jonathan Gonz�lez Encarnaci�n).
-- Sybase: Added timeout message processing into callback function (CS_TIMEOUT works now, thanks Michael Graf).
-- SQLServer: Added SSPROP_INIT_TRUST_SERVER_CERTIFICATE OLEDB connection option.
-- SQLite: Added sqlite3_table_column_metadata API fuinction.
-- MySQL: Added function support.
-- General: Fixed using wcscasecmp for MacOS X (thanks Christian Schmitz).
-- MySQL: Fixed procedure/function parameters parsing (thanks icqwjj).
-- InterBase: Fixed query preparing and executing with national symbols, Unicode version (thanks Petr Liska).
-- SQLServer: Fixed IssNCliCursor::ConvertString and IssOleDbCursor::ConvertString virtual methods(thanks Petr Liska).
-- Sybase: Fixed IsybCursor::ConvertString virtual method.
-- InterBase: Fixed IibCursor::ConvertString virtual method.
-- SQLite: Fixed SACommand::isResultSet always returns true.
-- General: Fixed SAString::SetUTF16Chars method (avoid crash when the source string is NULL).
-- General: Avoided double conversion of UTF-8 and UTF-16 data (SA_STRING_EXT required, thanks Christian Schmitz).
-- General: Fixed UTF-16 code (wrong conversion).
-- General: SONAME added for the shared library (thanks Pobinger Gerhard Stefan).

Version 4.0.1
-- MySQL: Fixed VARCHAR field data truncation with statement API (thanks Abhay Rawat).
-- General: SA_MINDEP build option replaced with SA_RUNTIME=(dynamic|static).
-- Oracle: Fixed NCLOB/NCHAR/NVARCHAR reading for UTF16 encoding.
-- General: Unicode Linux/Unix configuration is supported now (except ODBC).
-- General: Certain SAString methods return SIZE_MAX now instead of (-1).
-- General: Changed all internal buffer size types for size_t.
-- PostgreSQL: Fixed time string parsing(thanks Andrew Smolko).
-- PostgreSQL: Fixed string escaping.
-- ODBC: Fixed SQLDriverConnect related bug (thanks Mike Moening).
-- SQLite: Added backup API functions (thanks Dave Fileccia).
-- General: SADateTime default constructor is public again.
-- General: SAValueRead::asInterval() method substracts SADateTime(0.0) from SA_dtDateTime value instead of the current time.
-- SQLite: Added "SQLiteTransactionType" connection option (thanks Frank Hilliger).
-- SQLServer: Ready for SQL Server 2012 Native Client.
-- Oracle: Function returned value parameter name changed from "Result" to "RETURN_VALUE".

Version 3.8.3
-- PostgreSQL: Added support for PQfformat API function.
-- Sybase: Added SA_SYBASE_LARGE_IDENTIFIERS build option (CS_NO_LARGE_IDENTIFIERS uses by default).
-- Sybase: Added CS_SYB_CHARSET connection option (thanks Leonid O Volkov).
-- PostgreSQL: Added SA_RepeatableRead isolation level support (actual for PG 9.1, thanks Andrew Smolko).
-- PostgreSQL: Fixed binary values escaping (thanks combit.net and Bjoern Eggstein).
-- MySQL: Fixed API mysql_ssl_set_t for the modern parameter set (thanks Robert Osada).
-- General: Fixed SACommand::ParseInputMarkers method (thanks Michael Gandlin).
-- SQLite: Bind SA_dtUlong parameter as 64-bit integer instead of int (thanks Derick Naef).
-- General: Align the default buffer length for long/LOB data to sizeof(SAChar) (thanks Emir Subasic).
-- DB2: Fixed bug with binding an empty LOB (thanks Martin R�tzl).
-- SQLServer: ODBC, fixed empty LOBs binding and reading.
-- Informix: Fixed empty LOBs binding and reading.
-- ODBC: Fixed empty LOBs binding and reading.
-- Oracle: Fixed bug with PL/SQL functions return boolean (thanks Andrew Simmons).  

Version 3.8.2
-- MySQL: Fixed the field describing bug (thanks Brad DeJong).
-- General: samisc.h header moved into general include folder (SAMutex, SACriticalSectionScope classes).
-- General: Added SA_ODBC_INCLUDES build option.
-- ODBC: Fixed name buffer length for SQLDescribeCol API function.
-- ODBC: Added the catalog name into ODBC and MSSQL(ODBC) DescribeParamSP method.
-- General: Use neutral value of 'struct tm' DST flag (SADateTime class).
-- General: Use ISO format (YYYY-MM-DDTHH:MI:SS) for SA_dtDateTime values at SAValueRead::asString method.
-- Informix: Fixed Informix SAconnection::isAlive.
-- Informix: Fixed SACommand::isResultSet.
-- General: Fixed SACommand::Destroy.
-- Oracle: Fixed bug with package method executing that should be resolved by synonym (thanks Michael Lyle).
-- Interbase: Fixed SACommand::isResultSet.
-- Interbase: Implemented SA_UNICODE_WITH_UTF8 option.
-- Sybase: Fixed SA_UNICODE_WITH_UTF8 option implementation.
-- General: SADateTime::SADateTime() constructor is private now.
-- General: Added SAGlobals::StringToClient and SAGlobals::ClientToString methods.
-- General: Fixed Borland C++ build files.
-- General: Added SA_STATIC_SYBASE build option.

Version 3.8.1
-- General: Removed SA_SCROLLABLE_CURSOR build option, this features is always available now.
-- DB2: Added scrollable cursor support.
-- Informix: Added scrollable cursor support.
-- SQLBase: Added scrollable cursor support.
-- Sybase: Added scrollable cursor support.
-- General: Removed SA_UNICODE_WITH_UTF8 build option, this features is always turned on now.
-- PostgreSQL: Fixed bug with the application name parameter (thanks Bjoern Eggstein).
-- General: Added conversion from numeric value into SAValueRead::asBool method.
-- SQLServer: OLEDB, added support for Compact Edition 4.0 (thanks Jacob Pedersen).
-- MySQL: Now SAField::FieldSize methods returns the 'character' length for character fields (thanks Jonathan Gonz�lez Encarnaci�n).

Version 3.7.35
-- Oracle: Fixed LOB reading with OCI 10 and above (thansk Anca Elena Arhip).

Version 3.7.34
-- SQLServer: OLEDB, fixed the memory leak at 'Check' function (SAExecption throwing, thanks Michael Soliman).
-- SQLServer: OLEDB, removed assertion for SQLServer 2008 date and time types.
-- ODBC: FreeTDS, fixed the bug with missing first character of the long text/string data.
-- SQLite: sqlite3_open_v2() support, the statement is closed when there is no result or an error occurs (thanks Mario Degenhardt).
-- MySQL: Added CLIENT_REMEMBER_OPTIONS and MYSQL_OPT_RECONNECT connection options (thanks Steven Van Ingelgem).
-- PostgreSQL: Added "APPNAME" connection option (version 9.x and above).
-- Sybase: Added correct description of the output procedure parameters (ASE version >= 12.5, thanks Georgiy Pakhutin).
-- General: Fixed bug with SAValueRead::asBool() on BigEndian platforms (thansk Vlad Romascanu).
-- PostgreSQL: Added 'E' prefix for binary sequnces passed to server version 8.2.4 and above.
-- MySQL: Fixed again binding of the binary data (thanks Martin Herb).

Version 3.7.33
-- SQLServer: OLEDB, fixed parameter date type name for ICommandWithParameters::SetParameterInfo (thanks Michael Soliman).
-- General: SACommand::Param(int) method performance improved.
-- MySQL: Fixed binding of the binary data (thanks Martin Herb).
-- MySQL: Fixed the error processing when mysql_fetch_row returns NULL (thanks Michael Graf).
-- Sybase: Added 15.5 context support.
-- Sybase: Added SA_UNICODE_WITH_UTF8 build option support (EXPERIMENTAL).
-- MySQL: Added scrollable cursor support (EXPERIMENTAL).
-- General: Added SA_32BIT build option (Linux/Unix g++ build files).
-- ODBC: 2-byte buffer used with SQLGetData (getting the long field size, thanks David Stewart).
-- General: SA_STRING_EXT build option added (string buffer allocation optimization, thanks iontrading.com).
-- Sybase: Fix for 64-bit DLL names under Windows (thanks iontrading.com).
-- ODBC: Set SQL_ATTR_ODBC_CURSORS later to avoid crash when using iODBC (thanks iontrading.com).
-- General: Use C runtime routines at SAString::CompareNoCase (thanks iontrading.com).

Version 3.7.32
-- SQLite: Now (if "SQLiteDateValueType" = "DOUBLE") SQLAPI++ interprets a datetime value as a Julian Day JD double value (before we used SADateTime interpretation).
-- SQLite: Added "SQLiteDateValueType" option, SQLAPI++ uses "YYYY-MM-DD HH:MM:SS.SSS" formatted strings for date/time parameters by default.
-- SQLite: Changed "SQLiteDateTypes" option default value (now it's "DATE,DATETIME,TIME,TIMESTAMP").
-- MySQL: Fixed bug with the binding of zero-length blob- and byte- fields (thanks Taneli Otala).
-- Sybase: Connection options "SYBINTL.LIBS", "SYBCOMN.LIBS", "SYBTCL.LIBS", "SYBCT.LIBS", "SYBCS.LIBS" added.
-- General: Fixed problem with MacOS X and asctime_r function (thanks Michael Dickey).
-- General: More information provided if DBMS API loading fails.
-- General: 8-byte data length is allowed for SA_dtULong type.
-- SQLite: SA_STATIC_SQLITE build option added.
-- SQLite: The server/client version number is returned in common format now (hi- and low-word parts).
-- Oracle: SQLT_FLT binding type is used instead of SQLT_BDOUBLE even when it's supported by OCI.
-- General: SA_MANIFEST_EMB build option added.

Version 3.7.31
-- ODBC: Numeric data truncation bug fixed (thanks Gebert, Alfred).
-- SQLServer: OLEDB and ODBC, fixed bug with long/LOB data binding (thanks Thomas H�u�ermann).
-- SQLServer: OLEDB, added DBPROP_COMMITPRESERVE command parameter.
-- ODBC: fixed bug with long/LOB data binding.
-- Sybase: fixed bug with empty string binding? there was '\0' instead of ' ' (thanks Georgiy Pakhutin).

Version 3.7.30
-- General: Added new methods - SAConnection::Reset() and SACommand::Reset().
-- PostgreSQL: Fixed memory leak when "UseCursor" option is set (thanks Vlad Romascanu).
-- Oracle: Fixed problem with OCI version 8.1.x and 9.0.x.
-- SQLServer: Fixed bug with unsigned 32-bit integer parameters (thanks Kyle Stock).
-- SQLServer: Fixed problem with [n]varchar|varbinary(max) output parameters (thanks Alfred Gebert).
-- SQLServer: Fixed bug with SAParam::setAsBool method (thanks Jacob Pedersen).
-- General: Performance improved (Christian Zink).
-- Sybase: Fixed bug in error handling (thanks Volodymyr Drobot).
-- MySQL: Fixed bug with UTF-8 client charcter set and BLob binding (Jason Tai).
-- SQLServer: Fixed SA_tdBool data binding (thanks Jacob Pedersen).
-- DB2: Fixed problem with prefetching on 64-bit platforms (thanks Cal Heldenbrand).
-- Oracle: Added experimental scrollable cursor support (thanks combit.net and Alexander Horak).
-- SQLServer: Added native (ODBC) API support.
-- PostgreSQL: Connection option "UseDLLonWin32" replaced by "LIBPQ.LIBS" (can define several library names).
-- ODBC: Connection option "UseManagerOnUNIX" replaced by "ODBC.LIBS" (also can define several library names).
-- MySQL: Connection option "MYSQL.LIBS" added (can define several library names).
-- Oracle: Connection option "OCI8.LIBS" added (can define several library names).
-- DB2: Connection option "DB2CLI.LIBS" added (can define several library names).
-- Informix: Connection option "INFCLI.LIBS" added (can define several library names).
-- SQLBase: Connection option "SQLBASE.LIBS" added (can define several library names).
-- SQLite: Connection option "SQLITE.LIBS" added (can define several library names).
-- InterBase: Connection option "ClientLibraryName" replaced by "IBASE.LIBS" (also can define several library names).
-- Sybase: Bind ' ' character for empty procedure parameter strings (thanks Vlad Imshenetskiy).
-- Sybase: SA_PARAM_USE_DEFAULT support added.

Version 3.7.29
-- General: Added Borland compilation option SA_MINDEP. Now dynamic RTL is used by default with bcc compiler.
-- DB2: Fixed bug with Unicode DB2 data type(vargraphic) interpretation.
-- General: Fixed Borland C++ SQLServer OLEDB compilation problem (sa_max() using instead of max()).
-- SQLite: Fixed bug at the text data conversion with Unicode configuration (thanks Holger Fischer).
-- SQLite: Added new SAConection option "SQLiteDateTypes" (thanks Jonathan Gonzalez Encarnacion).
-- SQLite: Fixed bug with SAConnection::setAutoCommit functionality (thanks Jonathan Gonzalez Encarnacion).
-- Oracle: Fixed temporary LOBs memory leak (thansk Marc Oliver R. Castaneda).
-- Sybase: Fixed 64-bit related INT type size bug (thanks Jon Linkins).
-- SQLite: Added declared type processing if there are no any rows at the result set (thanks Jonathan Gonzalez Encarnacion).
-- SQLServer: Fixed bug with "Execute_riid" option is set to "IID_IStream" (thanks Patrik Luscan).
-- MySQL: Fixed datetime and boolean type processing if statement API is used (thanks Christophe Benoit).
-- MySQL: Fixed bug with non-SELECt queries if statement API is used (thanks Christophe Benoit).
-- General: Fixed bug with SANumeric from string operator (thanks Christophe Benoit).
-- Firebird: Fixed 64-bit version (thanks evenreal.com). 
-- Oracle: Fixed CLOB output parameter reading with ReadLob2 (thanks Gebert, Alfred).
-- Oracle: Fixed field and output parameter buffer size for multi-byte client character sets (thanks Gebert, Alfred).

Version 3.7.28
-- SQLServer: Added SAConnection OLEDB options "CompactEdition.3.0" and "CompactEdition.3.5" (thanks Johannes Maly).
-- SQLServer: Fixed processing of 'real' and 'tinyint' field types for SQLCE (thanks Jacob Pedersen).
-- General: Linux/Unix, added SQLAPI_SOLARIS compilation option and fixed pthread compilation problem (thanks Bogdan Pytlik).
-- SQLServer: OLEDB, fixed bug with empty second result set at the same scrollable SACommand (thanks Bjoern Eggstein).
-- General: Linux/Unix, added SQLAPI_SCOOSR5 compilation option (implemented for SCO OpenServer 5.0.7 by Andrew Thornton).
-- General: Linux/Unix, removed stabs+ debug information format specification (thanks Andrew Thornton).
-- MySQL: Fixed bug when it was impossible to process several result sets those a procedure returns (thanks Klass, Markus).
-- Oracle: Added a code that uses new OCILogRead2 when available (thanks Jeff Huttel).
-- MySQL: Fixed problem with prepared statements and different MYSQL_BIND versions (thanks Alexander Petrossian).
-- General: Several performance improvements (thanks Mike Moening).
-- MySQL: Fixed bug with SA_UNICODE_WITH_UTF8 and text fields when statement API used (thanks Jason at goffconcepts.com).
-- DB2: Fixed bug with text fields (UNICODE version).
-- General: For 'SACommand::Field(const SAString &sField)' added the second cycle for the field searching by short name (thanks Felix Nawothnig).
-- Sybase: Fixed long text field reading (SA_UNICODE).
-- Sybase: Added ability to define procedure parameters when the procedure name includes its number (ASE, like 'procedure1;1', thanks Vlad Imshenetskiy).
-- Sybase: Added method to detect server type (ASA or ASE) to avoid any extra internal exception processing (thanks Vlad Imshenetskiy).
-- SQLServer: Added 2008 OLEDB client support.
-- SQLServer: Added 2008 new data type support (thanks Justin Matthews).
-- SQLServer: Added fixes those allows to use "ICommandPrepare" = "skip" with procedures (without preparing there was string data truncation, thanks Justin Matthews).
-- MySQL: Added using 'KILL QUERY' command instead of mysql_kill with MySQL version >= 5.0 (thanks Andrew Thornton).
-- General: Documentation improved (thanks Justin Matthews and Kaarel Odraks).
-- Oracle: Fixed double values binding - using SQLT_BDOUBLE rather then SQLT_FLT (thanks Alan Hardin).

Version 3.7.27
-- General: Now 'SANumeric::operator double() const' again uses strtod() variant but with '.' replaced by localeconv()->decimal_point (thanks Alasdair Richardson).
-- General: 'SAGlobals::UnloadAPI() = true' behavior fixed (thanks Christophe Benoit).
-- MySQL: Fixed bug at ImyCursor::MySQLEscapeString(), if If a string is passed in that only contains escapable chars (thanks Oliver Watson).
-- SQLServer: Fixed bug with scrollable statement reusing that results in wrong result set processing (thanks Alexander Horak).

Version 3.7.26
- ODBC: Now SA_dtNumeric SQLAPI++ type is used by default for SQL_NUMERIC, SQL_DECIMAL and SQL_BIGINT (before SA_dtDouble was there).
- ODBC: Added new option SAConnection option ODBCUseNumeric.
- PostgreSQL: Added support for SA_UNICODE_WITH_UTF8 build option (experimental).
- Sybase: sybAPI::DefaultLongMaxLength method added to control max 'long' field size (when the using of ct_get_data is impossible).
- Sybase: SAGlobals::SetSybaseMessageCallback method renamed/moved to sybAPI::SetMessageCallback (still EXPERIMENTAL).
- InterBase: Added ability to use new Firebird fb_interpret call instead of unsafe isc_interprete (thanks Diane Downie).
- General: SQLAPI++ parameters and fields buffers allocation optimized (thanks Jeff Huttel).
- Sybase: CS_LOCALE connection option added.
- SQLServer: Fixed bug with numeric parameter presicion (truncation, thanks Jean-Claude Chauve).
- PostgreSQL: Fixed bug with SAParam::setAsULong (thanks Vlad Romascanu).
- Sybase: Added 'date' type support for the stored procedure parameters (thanks Vlad Imshenetskiy).
- Sybase: Fixed problem with the procedure parameters describing when procedure is located in another database (Georgiy Pakhutin).
- ODBC: Added new connection options - SQL_DRIVER_PROMPT,SQL_DRIVER_COMPLETE,SQL_DRIVER_COMPLETE_REQUIRED,DSN.
- SQLServer: Fixed bug with procedure preparation.

Version 3.7.25
- General: saConnectionHandler_t callback added at SAConnection::Connect method.
- SQLServer: SQLCE, "CreateDatabase" option added.
- SQLServer: SQLCE, use SA_dtString program type for uniqueidentifier fields.
- Oracle: Fixed memory leak when SYS_REFCURSOR parameters returns a cursor field (thanks mera.ru)
- General: Added compilation option SA_UNICODE_WITH_UTF8. Used with MySQL to turn on utf-8 client character set for SA_UNICODE configuration.
- General: SA_UNICODE configuration. Added SAString methods GetUTF8Chars, GetUTF8CharsLength, SetUTF8Chars.
- Oracle: Fixed option processing OCI_ATTR_CHARSET_FORM=SQLCS_NCHAR for NCLOB binding (thanks Andreas Wenzel).
- Oracle: Fixed binding(SA_UNICODE) CLOB procedure parameters (thanks Marcel Rassinger).
- General: Fixed 'SANumeric::operator double() const' bug with locale settings those use a special character as a decimal delimiter instead of '.'.
- SQLite: Added new API functions - sqlite3_threadsafe and sqlite3_last_insert_rowid.
- SQLite: Fixed wrong API initialization (thanks Peter Klotz).
- SQLite: Added "BusyTimeout" connection option.
- General: Added version info into sqlapi.dll (thanks Frank Hilliger).
- Sybase: SACommand::Cancel should work now (thanks Georgiy Pakhutin).
- SQLServer: OLEDB, SACommand::Cancel works with multiple results (required SQLNCLI, thanks Georgiy Pakhutin).
- SQLServer: OLEDB, the default value of "ICommadPrepare" option was changed to "skip".
- Sybase: "SybaseResultType" and "SybaseResultCount" SACommand options added.
- Sybase: SAGlobals::SetSybaseMessageCallback method added (EXPERIMENTAL, allows to process Sybase messages manually).

Version 3.7.24
- MySQL: Connection options MYSQL_OPT_READ_TIMEOUT, MYSQL_OPT_WRITE_TIMEOUT and MYSQL_OPT_CONNECT_TIMEOUT added.
- MySQL: Version 5.1 supported now.
- Oracle: CLOB/BLOB truncation bug fixed.
- Oracle: REFCURSOR memory leak problem fixed.
- SQLite: Initial support for SQLite added.
- ODBC: Added the special code for situations when the ODBC driver returns 0 bytes instead of SQL_NO_DATA (LongChar reading, thanks Frank Hilliger).
- SQLServer: SQLOLEDB layer modified for Compact Edition 3.5.
- SQLServer: Added Compact Edition specific options. Now CE layer is available at UNICODE version only.
- DB2: Fixed the problem with column names (UNICODE version, thanks Adrian Studer).
- General: Fixed the problem with the SANumeric rounding (thanks Mike Moening).
- MySQL: Added SAInterval parameter processing (thanks Frank Hilliger).

Version 3.7.23
- PostgreSQL: The error message includes the diagnostic trouble code now (thanks Helmut Ebersmann).
- MySQL: Fixed bug with affected rows value (thanks Christoph Merten).
- MySQL: Fixed bug with datetime/timestamp second part/fraction (thanks Helmut Ebersmann).
- General: SACommand::ParseInputMarkers fixed for tabulator after parameter name (thanks Adrian Studer). 
- SQLServer: IssOleDbCursor::SetParameterInfo fixed for correct type name (thanks to Phillip at spectorsoft.com).
- SQLServer: SQLOLEDB layer works with Compact/Mobile edition now (SQLAPI should be compiled with SA_UNICODE, thanks John Hague).
- Sybase: New scheme of the client API libraries loading , SA_INCLUDES_SYBASE building parameter added (thanks Max Hales).
- Sybase: CS_LOGIN_TIMEOUT and CS_TIMEOUT connection parameters added (thanks Beaty, Robert).
- Oracle/General: Fixed issue with SANumeric/double conversion (thanks Adrian Studer).
- Oracle: Fixed problem with CLOB and varying-width client-side character sets (UTF-8).
- General: Build scripts updated. More samples are provided now.

Version 3.7.22
- PostgreSQL: Fixed string escaping bug (buffer size too small, thanks combit.net).
- SQL Server: Added statement preparation for each executing when option "ICommandPrepare" = "SetParameterInfo" used (thanks to Chris Hecker).
- SQL Server: Fixed string parameters buffer size bug on x86_64/SQLOLEDB.
- MySQL: Fixed access violation bug with fetching empty text field data (thanks Albert Perdon).
- MySQL: Fixed bug with non-initialized statement handle (thanks Mario Lavalliere).
- ODBC: The code modified to work correctly with long/blob fields under Linux and FreeTDS driver.
- ODBC: SAConnection::isAlive() uses SQLGetConnectAttr/SQL_ATTR_CONNECTION_DEAD now.
- SQL Server: Added SAConnection parameter "OLEDBProvider" (SQLOLEDB connection).

Version 3.7.21
- Oracle: "APPNAME" SAConnection parameter added (OCI_ATTR_CLIENT_IDENTIFIER is used, thanks Frank Hilliger).
- MySQL: Fixed bug with SAConnection::isAlive().
- SQL Server: "ICommandPrepare" SACommand parameter value "SetParameterInfo" added (fixes http://support.microsoft.com/kb/235053).
- MySQL: "UseStatement" SACommand parameter added (SQLAPI++ can use MySQL statement API now).
- MySQL: Fixed long character type support with UNICODE.
- Oracle: Fixed CLOB type support with UNICODE.
- ODBC: Fixed access violation bug at IodbcConnection::Check (UNICODE, thanks Jay Sridhar).
- DB2: Fixed possible access violation bug at Idb2Connection::Check (UNICODE).

Version 3.7.20
- Sybase: Version 15.x support for Linux/UNIX (.so names changed - thanks Ronan O'Sullivan).
- Oracle: Crash fixed with the OCIDateTime descriptor freeing.
- DB2: Unicode support improved.
- General: Added common SAConnection option: "APPNAME", "WSID" (they are supported for SQL Server and Sybase at the moment).
- SQL Server (OLDEDB): Added SAConnection options SSPROP_INIT_FILENAME, SSPROP_INIT_ENCRYPT, SSPROP_INIT_WSID.
- PostgreSQL: Added using of the PQescape* functions if they are available.
- DB2: SA_dtString procedure parameter type is used for the LONG VARCHAR database type.
- MySQL: Added support for multiple result set.
- SQL Server (OLDEDB): x86_64 fixes.
- DB2: All diagnostic messages are combined into SAException error text now.
- Sybase: CS_HAFAILOVER connection parameter added.
- General: Fixed a SADateTime::TmFromDate problem with the rounding.
- DB2: XML type support added.
- DB2: Fixed x86_64 support.

Version 3.7.19
- Oracle: Added procedure/function support for Oracle PL/SQL BOOLEAN type.
- Oracle: Support for BINARY_FLOAT and BINARY_DOUBLE types.
- Oracle: Memory leak fixed with SYS_REFCURSOR & TIMESTAMP/BLOB/CLOB fields.
- General: Added bool& SAGlobals::UnloadAPI() - prevent from DBMS API unloading.
- General: SADateTime::SADateTime(double dt) - round the fraction result to microseconds.
- General: Added SAInterval class for time intervals (experimental, only MySQL support for now).
- MySQL: Support for multi-result statements.
- MySQL: Set by default connection flags CLIENT_MULTI_STATEMENTS and CLIENT_MULTI_RESULTS.
- SQLBase: Fixed access violation at sqldes() C API function with SQLBase version 10.
- MySQL: Added support for the 'bit' type.
- PostgreSQL: Fixed problem with bytea/text on AIX 64-bit (seems common 64-bit platform problem).
- Sybase: Fixed MinGW version.
- ODBC:	Windows x64 fixes. Should work now.
- Oracle: Fixed bug with DateTime parameters and an execution of prepared statements.

Version 3.7.18
- General: Added SACommand::operator << (unsigned short Value) and SACommand::operator << (unsigned long Value).
- MySQL: Added "CharacterSet" SAConnection option.
- Oracle: Fixed bug with SADateTime to Oracle OCIDateTime conversion.
- SQLServer: Fixed bug when it was impossible to fetch several result sets with single query.
- MySQL: Added mysql_thread_init() and mysql_thread_end() API functions.
- MySQL: Added mysql_server_end() call before MySQL client library is released.

Version 3.7.17
- General: AIX, HP-UX and MacOS X support improved.
- SQLServer: Added SSPROP_INIT_MARSCONNECTION connection (OLEDB) option.
- PostgreSQL: Server side cursor implemented for 'SELECT ...' statements.
- InterBase: Modified to start transaction only when query executed (thanks Fabrice Aeschbacher).
- General: New SQLAPI++ data types added: SA_dtUShort and SA_dtULong.
- General: Added SAString methods SAString::MakeUpper(), SAString::MakeLower().
- General: Experimental 'scrollable cursor' functionality implemented (PostgreSQL,MSSQL,ODBC thanks combit.net).
- General: Fixed SACommand::ParseInputMarkers for '=' parameter name delimiter.
- Oracle: Fixed memory leaks and troubles with BLOB and TIMESTAMP descriptors.
- Oracle: UNICODE version uses UCS2 client character set.
- Oracle: Fixed bug at IoraConnection::CnvtNumericToInternal with numbers like [-]0.001
- Oracle: Fixed memory leak with temporary BLOBs (thanks Frank Hilliger).
- General: Experimental methods SAConnection::Destroy() added (to destroy broken connection).
- General: Experimental method SAConnection::isAlive() added (to check if a remote database is alive).
- MySQL: mysql_ping was removed from SAConnection::IsConnected().
- ODBC: Fixed column size and precision for SADateTime input parameters.
- Sybase: Fixed "CS_HOSTNAME" connection option (thanks Ulrich Gievers).
- Sybase: Bug fixed with INTEGER-s on x86-64.
- MySQL: Bug fixed long text/binary procedure parameters.
- Oracle: New SAConnection "UseTimeStamp" option (controls if SQLAPI should use Oracle TIMESTAMP functions).
- Oracle: Fixed bug with procedure datetime parameters.
- DB2: Fixed bug with INTEGER-s on x86-64.
- ODBC:	Fixed bug with SAException::ErrMsg() at UNICODE configuration. UNICODE support improved.
- General: Fixed bug at SAString::FormatV(...) on Linux/UNIX (thanks Michael Teske)
- SQLServer: DB-Library client included into MinGW build
- MySQL: Fixed the procedure parameters parsing code

Version 3.7.16
- General: SAString::Replace(...) - serious bug fixed.

Version 3.7.15
- General: Fixed SAString::FormatV(...) (thansk Bernd Holz).
- ODBC: Support for GUID type (thanks Alexander Horak).
- Oracle: SADateTime fraction support.
- General: SAString::Replace(...) was improved (thanks Arthur Finkel).
- MySQL: Fixed the procedure parameters parsing code (thanks Znamenacek Pavel).

Version 3.7.14
- General: Fixed memory leak in SAConnection destructor (thanks Diane Downie).
- MySQL: Support for NEWDECIMAL data type.
- SQLBase: Linux/UNIX support.
- General: Fixed UNIX implementation of the SAString::Format for 64-bit integer (thanks Boris Daniel).
- MySQL: Added support for a procedure result set.
- InterBase: Boolean support (InterBase v7).
- PostgreSQL: New connection option "ClientEncoding".
- Oracle: New connection option "ConnectAs" (SYSDBA or SYSOPER, OCI8 only).
- General: MinGW (GNU C++ for Win32) support.
- Interbase: New global option "ClientLibraryName" to define the API library name.
- General: SAString::FormatV fixed for ISO C99 va_copy (thanks Mathew Kuzhithattil Aniyan).
- General: Microsoft Platform SDK integration. X86, IA64, AMD64 target support.
- PostgreSQL: field size, precision, scale support (thanks Alexander Horak).

Version 3.7.12
- SAConnection::~SAConnection(): possible GPF fixed (thanks Diane Downie).
- SANumeric: 64-bit integer operators and constructor added.
- MySQL: added alpha support for MySQL 5.x procedures.
- SQL Server(OLEDB): MSSQL native error code is returned by SAExeption::ErrNativeCode() now.
- Interbase/Firebird: SADateTime::Fraction() support (thanks Fabrice Aeschbacher).
- DB2: support for SQL_GRAPHIC, SQL_VARGRAPHIC, SQL_LONGVARGRAPHIC  types added.
- General: fixes to compile with VS.NET 2005 under AMD64 (thanks Peter Klotz).
- General: some improvements for threaded applications.
- General: Unusual (not ".so") extensions for shared libraries can be defined at compilation process.
- SACommand::ParseInputMarkers(...): bug fixed with ':' symbol in string constants.

Version 3.7.11
- ODBC: connection-level option added to control ODBC's connection pooling. 
- MySQL: added support for MySQL clients version 4.1 and higher.
- MySQL: connection-level options added to control client_flag parameter when connecting to MySQL.
- SQL Server: command-level option added: DBPROP_REMOVEDELETED.
- Sybase: command-level option added: ct_cursor.
- General: small memory leak fixed.

Version 3.7.10
- ODBC: bulk fetching support improved as a work-around for some non-compliant drivers.
- Oracle: added support for new date/times types in Oracle 9i.
- Oracle: added read support for interval types. They are defaulted to strings.
- SQL Server: added read support for fields of type VARIANT. They are defaulted to strings.
- SAString:Format(): bug fixed with "I64" option implementation on Windows platform.
- SAString::Format(): small change to better support variable argument list on some picky UNIX platforms.

Version 3.7.9
- SQL Server: new command related option ("SSPROP_INIT_APPNAME") added.
- Oracle: small memory leak fixed with Oracle 9i client.

Version 3.7.8
- PostgreSQL: support for TIMESTAMPTZOID type added.
- Oracle: bulk fetch implementation improved for columns of type NUMBER.

Version 3.7.7
- SQL Server (OLE DB): bug fixed when reading large NTEXT columns. 
- SQLBase: bug fixed when using Receive Long String variables in stored procedures. 

Version 3.7.6
- PostgreSQL: int8 datatype support improved. 
- Sybase: money datatype support improved. 
- MySQL: support for MySQL 4.x client improved.

Version 3.7.5
- Borland C++ Builder 6 support improved.
- ODBC: new command related option ("PreFetchRows") added.
- DB2: Bug fixed when fetching with "PreFetchRows" option.
- Oracle: "PreFetchRows" option implementation improved.

Version 3.7.4
- InterBase: new connection related options("TPB_LockResolution", "TPB_AccessMode") added.
- SQL Server (OLE DB): new command related option ("Execute_riid") to set the requested interface for the rowset added.
- Oracle: support for pls_integer and binary_integer types added.

Version 3.7.3
- PostgreSQL, MySQL: support for binding/retrieving double values improved (bug fixed when current locale's decimal point is not '.').

Version 3.7.2
- SQLBase: support added for stored commands.
- Oracle: new parameter and field related options added ("OCI_ATTR_CHARSET_ID", "OCI_ATTR_CHARSET_FORM").
- Linux/Unix: some changes applied to be compatible with Alpha platform.

Version 3.7.1
- SQL Server (OLE DB): memory leaks fixed in SAConnection::ClientVersion(), SAConnection::ServerVersion() and SAConnection::ServerVersionString().
- SQL Server (OLE DB): new connection related option("CoInitializeEx_COINIT") to set the COM library initialization mode added.
- DB2: new command related option added ("ReturnStatus").

Version 3.6.9
- ODBC (iODBC on Linux/Unix): bug fixed when executing DELETEs or UPDATEs that don't affect any rows (function sequence error).
- PostgreSQL: small memory leak fixed.
- Sybase: new connection related option ("CS_VERSION") to set the version of Client-Library behavior added.

Version 3.6.8
- Oracle (Linux/Unix): bug fixed when calling a stored procedure while server is shutting down.
- Multithreading support improved.

Version 3.6.7
- SADateTime::GetCurrentTime() method renamed to SADateTime::currentDateTime().
- SQLBase: bug fixed with in/out parameters of type NUMBER.
- SQLBase: bug (microseconds truncation) fixed when selecting date/time values.
- Oracle: support added for logging in using external authentication.

Version 3.6.6
- SQL Server (OLE DB): New command related option("DBPROP_COMMANDTIMEOUT ") to set command time-out added.
- Oracle: Bug fixed when selecting ROWID.
- DB2: Support added for stored procedure return status.

Version 3.6.5
- SQL Server (OLE DB): New command related option ("ICommandPrepare") to control current command preparation added.
- _SA macros changed to _TSA macros to be compatible with Microsoft Visual C++.NET.

Version 3.6.4
- Oracle: Memory leak bug when calling a stored procedure with REF CURSOR parameter(s) fixed.
- SQL Server: Bug when selecting text and image fields with PreFetchRows option fixed.
- InterBase: Big and high precision numbers support improved.

Version 3.6.3
- Oracle: Bug when binding using setAsNumeric() has been fixed.
- DB2 (Linux/Unix): Bug in NUMERIC and DECIMAL data support has been fixed.

Version 3.6.2
- Sybase: Support has been added for new OpenClient 12.5 features.
- SQL Server (OLE DB): Some new rowset options have been added.

Version 3.6.1
- Default parameters value support has been improved.

Version 3.5.9
- Big and high precision numbers support has been improved (Oracle, InterBase, SQLBase, Informix).

Version 3.5.8
- Oracle: bug when retrieving numeric values fixed.

Version 3.5.7
- DB2: big and high precision numbers support has been improved.

Version 3.5.6
- Oracle: bug fixed (access violation) when selecting numeric fileds.

Version 3.5.5
- Sybase: new command option ("CS_BULK_LOGIN") to describe whether or not a connection can perform bulk copy operations into a database is available.
- Big and high precision numbers support has been improved (Oracle, Sybase, SQL Server, MySQL and PostgreSQL).
- MySQL: reporting precision, scale and nullability has been improved.

Version 3.5.4
- Documentation improved.

Version 3.5.3
- Oracle: new option to specify which overload to call is now available for Oracle command ("Overload").
- Oracle: bug fixed when reading data of RAW type.
- DB2: new option is now available for DB2 command ("PreFetchRow").
- SQL Server (OLE DB): new option ("SSPROP_INIT_AUTOTRANSLATE") for configuring OEM/ANSI character translation is available.

Version 3.5.2
- InterBase: commit management improved.
- Oracle (OCI7): native error number reporting fixed.
- SQL Server (OLE DB): bug fixed when reading data of NTEXT type.

Version 3.5.1
- PostgreSQL support.
- SQL Server (OLE DB): the ability to use DBPROP_SERVERCURSOR rowset property added.
- String and binary data support has been improved.

Version 3.4.7
- InterBase: bug fixed when binding NULL values.
- MySQL: setting transaction isolation level implemented.

Version 3.4.6
- C type 'bool' supporting added.

Version 3.4.4
- SQL Server (OLE DB): COM initialization improved.

Version 3.4.3
- SQL Server (DB-Library): error when returning output parameters from stored procedures is fixed.
- InterBase: FireBird client support added.

Version 3.4.2
- Sybase (Sun Solaris): error when connecting is fixed.

Version 3.4.1
- MySQL suport.
- Sybase: New option is now available for Sybase command (PreFetchNext).
- Documentation improved.

Version 3.3.4
- InterBase: error (stack overflow) when calling stored procedures with no parameters is fixed.
- Oracle, SQLBase: SACommand::isResultSet implementation fixed. Now it returns false after result set is completely fetched.
- SQL Server (OLE DB): bug fixed when reading text or image collumns with SQL Server 7.0 client.

Version 3.3.2
- Documentation improved.

Version 3.3.1
- Support for using SQL Server OLE DB API added. Default API for accessing SQL Server changed from DB-Library to OLE DB.
- DB2: fetching multiple result sets from stored procedures implemented.
- SACommand::FetchParamsNext method deprecated.
- Documentation improved.

Version 3.2.5
- SQL Server: the ability to use trusted connection added.
- Numeric data reading methods now support type conversion from string data.
- Documentation improved.

Version 3.2.4
- Sybase: minor changes made to improve multi-threading support.

Version 3.2.3
- Minor changes made in SQLAPI++ source codes to make it compatible with Sun Workshop Pro compiler.

Version 3.2.2
- InterBase: Truncation of VARCHAR fields. Bug fixed.

Version 3.2.1
- Sun SPARC Solaris: SIGBUS error (non-alignment of numeric data). Bug fixed.
- Oracle 8i: Ability to bind BLob(CLob) parameters in stored procedures is implemented.

Version 3.1.6
- SQL Server: bug with smalldatetime type fixed.

Version 3.1.5
- SQL Server: Ability to connect to named instances of SQL Server 2000 added.

Version 3.1.4
- Sybase: New options are now available for Sybase connection ("CS_PACKETSIZE", "CS_APPNAME", "CS_HOSTNAME").

Version 3.1.3
- Oracle: Executing a function in Oracle 8i resulted in ORA-01426. Bug fixed.
- SQLServer: Bug fixed when binding parameters to a command with "OpenCursor" option set.
- Sybase: Executing a stored procedure through "Exec" statement resulted in access violation. Bug fixed.

Version 3.1.2
- Documentation improved (Server Specific Guide).
- DB2, Informix, ODBC: native error code returned by SAException::NativeError() was invalid. Bug fixed.
- InterBase: Support for database connection cache added.

Version 3.1.1
- Oracle 8: Access violation when updating/inserting CLob fields. Bug fixed.
- Oracle (Linux/Unix): Error "Unimplemented or unreasonable conversion requested" when working with output (return) parameters is fixed.
- Oracle: update/insert SQL statement with two or more BLob (CLob) parameters results in "OCI_INVALID_HANDLE" exception. Bug fixed.
- ODBC: bug with DateTime values fixed.

Version 3.0.1
- Unix version released.
- Oracle: Support for Oracle REF CURSORs and nested cursors added.

Version 2.3.11
- SAString::Right method: bug fixed.
- SQLBase: Automatic detecting of stored procedure parameters was incorrect when parameter's name include numeric digits, underscore (_) or special characters (#, @ or $). Bug fixed.
- SQL Server: "Open Cursor" option improved.
- SQL Server (version 7, version 2000): data type for datetime parameters in stored procedures has been detected as SA_dtUnknown. Bug fixed.

Version 2.3.10
- Sybase: support for returning status result code from stored procedures.

Version 2.3.9
- SQLServer: bug fixed when calling a stored procedure with "owner.proc_name" syntax on SQLServer 2000.
- ODBC (MS Access): bug fixed when binding long data.

Version 2.3.8
- SQLServer: BIT datatype support added.
- SQLServer: bug (assertion) fixed when working with DECIMAL or NUMERIC fields in debug version.

Version 2.3.7
- Cancelling queries support added.

Version 2.3.6
- Informix: error "String data right truncation" when binding string parameters is fixed.

Version 2.3.5
- SQLServer: Support for using DB-Library cursor functions  added.

Version 2.3.4
- InterBase: Support for client character set declaring added.
- InterBase: Support for connection with a role added.

Version 2.3.3
- Date/time support improved. SADateTime::GetCurrentTime method added.

Version 2.3.2
- SQLBase: Support for Cursor-context preservation.
- SAString &SAConnection::Option(const SAString&) is replaced by SAConnection::setOption
- SAString &SACommand::Option(const SAString&) is replaced by SACommand::setOption

Version 2.3.1
- InterBase: Support for SQL Dialect 2 and 3 added. SQL Dialect is now customizable.
- InterBase: Bug fixed when reading DECIMAL and NUMERIC fields from database

Version 2.2.1
- Fraction of a second (milli, micro, nano seconds) support added.
- Date/time support improved.

Version 2.1.1
- Date/time support improved.

Version 2.0.1
- Linux version released. 
- Informix: error when calling stored procedures with no parameters is fixed. 
- Informix: error when describing stored procedure input parameters is fixed. 
- Informix, DB2: GPF when handling errors fixed.
- Oracle OCI7: multi-threaded support added.
- Documentation is improved. A great number of examples added.

Version 1.6.1
- Sybase: returning multiple result sets from stored procedure or batch is implemented. 
- SQL server: returning multiple result sets from stored procedure or batch is implemented. 
- Error when compiling with Borland compilers using data alignment other than 8 bytes fixed.

Version 1.5.1
- Sybase support (both ASE and ASA)
- Oracle8: Error reporting improved when calling non-existent procedure(function) from existent package (f.ex., sys.dbms_output.aaaaaa). 
- SQL server: the abillity to connect to default database is implemented. 
- SQL Server: SAException::ErrNativeCode always returned 0 (not native error code). Fixed. 
- DB2, Informix, ODBC: bug fixed when calling stored procedures with schema (f. ex., tester.TestProc) 
- Microsoft VC++ 5.0 backward compatible. 
- Oracle 7.3.3 client - loading error fixed. 
- DB2, Informix, ODBC: bug fixed when a searched update or delete statement that does not affect any rows is executed. 
- DB2, Informix, ODBC: bug when returning strings from stored procedures fixed. 
- Error when linknig with Borland compilers fixed 

Version 1.4.1
- Informix support 
- Documentation improved

Version 1.3.1
- DB2 support 
- BFILE support in Oracle OCI8 
- Bug in Oracle when binding BLobs fixed 
- Bug in SQLServer when reading timestamp fields fixed

Version 1.2.1
- Bug in ODBC implementation when fetching character data (in some cases data can be truncated) fixed 
- Possibility of getting error offset (SAExecption::ErrPos) 
- Autocommit management (SAConnection::AutoCommit and SAConnection::setAutoCommit) 
- Documentation improved

Version 1.1.1
- Support for managing transaction isolation levels added. 
- Transaction policy on SQLServer changed. Now all commands from one connection are in the same transaction. 
- Reported bugs fixed. 
- Documentation improved.

Version 1.0.1
- the first version released

4. ORDERING (for trial version only)
If you like SQLAPI++ Library and you want to receive the registered version
you should purchase SQLAPI++ Personal license for EVERY developer, 
or one Sile (Site+) license for all developers in your company.
Visit http://www.sqlapi.com/Order for more information.
