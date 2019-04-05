#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <SQLAPI.h> // main SQLAPI++ header
#include <samisc.h>

#ifdef SA_UNICODE
#ifdef SQLAPI_WINDOWS
#define SA_FMT_STR L"s"
#else
#define SA_FMT_STR L"S"
#include <wchar.h>
#include <wctype.h>
#endif
#else
#define SA_FMT_STR "s"
#endif

int main(int/* argc*/, char** /* argv[]*/)
{
	setlocale(LC_ALL, "");

	SAConnection con;

	// print menu
	sa_tprintf(_TSA("1.\tOracle\n"));
	sa_tprintf(_TSA("2.\tSQL Server\n"));
	sa_tprintf(_TSA("3.\tDB2\n"));
	sa_tprintf(_TSA("4.\tInformix\n"));
	sa_tprintf(_TSA("5.\tSybase\n"));
	sa_tprintf(_TSA("6.\tInterBase\n"));
	sa_tprintf(_TSA("7.\tSQLBase\n"));
	sa_tprintf(_TSA("8.\tMySQL\n"));
	sa_tprintf(_TSA("9.\tPostrgeSQL\n"));
	sa_tprintf(_TSA("0.\tODBC\n"));
	sa_tprintf(_TSA("a.\tSQLite\n"));
	sa_tprintf(_TSA("b.\tSQLAnywhere\n"));
	sa_tprintf(_TSA("c.\tCubeSQL\n"));


	SAChar ch = (SAChar)sa_gettchar();
	try
	{
		switch(ch)
		{
		case _TSA('1'):
			con.setClient(SA_Oracle_Client);
			break;
		case _TSA('2'):
			con.setClient(SA_SQLServer_Client);
			break;
		case _TSA('3'):
			con.setClient(SA_DB2_Client);
			break;
		case _TSA('4'):
			con.setClient(SA_Informix_Client);
			break;
		case _TSA('5'):
			con.setClient(SA_Sybase_Client);
			break;
		case _TSA('6'):
			con.setClient(SA_InterBase_Client);
			break;
		case _TSA('7'):
			con.setClient(SA_SQLBase_Client);
			break;
		case _TSA('8'):
			con.setClient(SA_MySQL_Client);
			break;
		case _TSA('9'):
			con.setClient(SA_PostgreSQL_Client);
			break;
		case _TSA('0'):
			con.setClient(SA_ODBC_Client);
			break;
		case _TSA('a'):
			con.setClient(SA_SQLite_Client);
			break;
		case _TSA('b'):
			con.setClient(SA_SQLAnywhere_Client);
			break;
		case _TSA('c'):
			con.setClient(SA_CubeSQL_Client);
			break;
		default:
			return 0;
		}

		long nVersionClient = con.ClientVersion();
		if(nVersionClient)
		{
			short minor = (short)(nVersionClient & 0xFFFF);
			short major = (short)(nVersionClient >> 16);
		
			sa_tprintf(_TSA("Client version: %hd.%hd\n"), major, minor);
		}
		else
		{
			sa_tprintf(_TSA("Client version: unknown before connection\n"));
		}

		sa_tprintf(_TSA("Database name (connection string):\t"));
		SAString sDatabase;
		sa_tscanf(_TSA("%") SA_FMT_STR, sDatabase.GetBuffer(1024));
		sDatabase.ReleaseBuffer();
		sa_tprintf(_TSA("User name:\t"));
		SAString sUsername;
		sa_tscanf(_TSA("%") SA_FMT_STR, sUsername.GetBuffer(1024));
		sUsername.ReleaseBuffer();
		if( 0 == sUsername.CompareNoCase(_TSA("-")) )
			sUsername.Empty();
		sa_tprintf(_TSA("Password:\t"));
		SAString sPassword;
		sa_tscanf(_TSA("%") SA_FMT_STR, sPassword.GetBuffer(1024));
		sPassword.ReleaseBuffer();
		if( 0 == sPassword.CompareNoCase(_TSA("-")) )
			sPassword.Empty();

		con.Connect(sDatabase, sUsername, sPassword);

		long nVersionServer = con.ServerVersion();
		short minor = (short)(nVersionServer & 0xFFFF);
		short major = (short)(nVersionServer >> 16);
		sa_tprintf(_TSA("Server: %") SA_FMT_STR _TSA("\n"), (const SAChar*)con.ServerVersionString());
		sa_tprintf(_TSA("Server version: %hd.%hd\n"), major, minor);
		
		if(!nVersionClient)
		{
			nVersionClient = con.ClientVersion();
			if(nVersionClient)
			{
				short minor = (short)(nVersionClient & 0xFFFF);
				short major = (short)(nVersionClient >> 16);
		
				sa_tprintf(_TSA("Client version: %hd.%hd\n"), major, minor);
			}
			else
			{
				sa_tprintf(_TSA("Client version: unknown after connection\n"));
			}
		}
	}
	catch(SAException &x)
	{
		// SAConnection::Rollback()
		// can also throw an exception
		// (if a network error for example),
		// we will be ready
		try
		{
			// on error rollback changes
			con.Rollback();
		}
		catch(SAException &)
		{
		}
		// print error message
		sa_tprintf(_TSA("Error text: %") SA_FMT_STR _TSA("\n"), (const SAChar*)x.ErrText());
	}

	return 0;
}

