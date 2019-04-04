#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <SQLAPI.h>
#include <samisc.h>

#include <myAPI.h>

using namespace std;

void MySQL_InsertID()
{
	SAConnection con; // connection object
    SACommand cmd(&con, "insert into t2(f2) values('new line')");
	
	try
	{
		con.Connect("test", "ODBC", "", SA_MySQL_Client);
		cout << (const char*)con.ServerVersionString() << endl;
		cout << (con.ClientVersion() >> 16) << "."
			<< (con.ClientVersion() &0x0000FFFF) << endl;

		cmd.Execute();
		
		myAPI* p_myAPI = (myAPI*)con.NativeAPI();
		myConnectionHandles* p_myConnHandles =
			(myConnectionHandles*)con.NativeHandles();


		cout << "new value of the auto increment field: " <<
			(long)p_myAPI->mysql_insert_id(p_myConnHandles->mysql) << endl;
	}
    catch(SAException &x)
    {
        // print error message
        printf("%s\n", (const char*)x.ErrText());
    }
}

int main(int argc, char* argv[])
{
	SAConnection con; // create connection object

	try
	{
		con.Connect(
			"test",  // database name
			"root",				// user name
			"",			// password
			SA_MySQL_Client);

		SACommand cmd(&con, _TSA("insert into t2(f2) values('xx')"));
		cmd.Execute();

		cmd.setCommandText(_TSA("select last_insert_id()"));
		cmd.Execute();

		while (cmd.FetchNext())
		{
			sa_tprintf(_TSA("%d"), cmd[1].asLong());
			sa_tprintf(_TSA("\n"));
		}
	}
	catch (SAException &x)
	{
		sa_tprintf(_TSA("ERR: "));
		sa_tprintf((const SAChar*)x.ErrText());
		sa_tprintf(_TSA("\n"));
	}

	return 0;
}