/*

This example demonstrates how to use Oracle nested cursors
with SQLAPI++ Library.

*/

#include <SQLAPI.h> // main SQLAPI++ header

#include <iostream>
using namespace std;

void OracleNestedCursors();

int main()
{
	OracleNestedCursors();
	return 0;
}

void OracleNestedCursors()
{
	SAConnection con;

	try
	{
		SACommand cmd;
		cmd.setConnection(&con);

		con.Connect(
			"prok", "dwadm", "dwadm", SA_Oracle_Client);

		// this select includes nested cursor
		cmd.setCommandText(
			"select "
			" 'nested cursor key', "
			" CURSOR(select * from dual) as nested_cursor "
			"from dual");

		cmd.Execute();

		while(cmd.FetchNext())
		{
			cout << "Resul set for '" << (const char*)cmd[1].asString() << "':\n";
			// get nested select and fetch from it
			SACommand *pNestedCursor = cmd["nested_cursor"];
			while(pNestedCursor->FetchNext())
			{
				cout << "\t" << (const char*)pNestedCursor->Field(1).asString() << "\n";
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
        cout << (const char*)x.ErrText() << "\n";
    }
}
