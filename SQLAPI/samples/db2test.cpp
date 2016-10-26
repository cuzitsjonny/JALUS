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
//	setlocale(LC_ALL, "");

	SAConnection con;

	try
	{
		con.Connect("test", "", "", SA_DB2_Client);

		long nVersionServer = con.ServerVersion();
		short minor = (short)(nVersionServer & 0xFFFF);
		short major = (short)(nVersionServer >> 16);
		sa_printf(_TSA("Server: %") SA_FMT_STR _TSA("\n"), (const SAChar*)con.ServerVersionString());
		sa_printf(_TSA("Server version: %hd.%hd\n"), major, minor);
		
		long nVersionClient = con.ClientVersion();
		if(nVersionClient)
		{
			short minor = (short)(nVersionClient & 0xFFFF);
			short major = (short)(nVersionClient >> 16);
	
			sa_printf(_TSA("Client version: %hd.%hd\n"), major, minor);
		}
		else
		{
			sa_printf(_TSA("Client version: unknown after connection\n"));
		}
		
		SACommand cmd(&con, "select f1,f2 from t1");

		cmd.Execute();

		while( cmd.FetchNext() )
		{
		    sa_printf(_TSA("Row fetched\n"));
		    sa_printf(_TSA("F1: %d, F2: %") SA_FMT_STR _TSA("\n"), cmd[1].asLong(), (const SAChar*)cmd[2].asString());
		}
	}
	catch(SAException &x)
	{
		// print error message
		sa_printf(_TSA("Error text: %") SA_FMT_STR _TSA("\n"), (const SAChar*)x.ErrText());

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
	}

	return 0;
}

