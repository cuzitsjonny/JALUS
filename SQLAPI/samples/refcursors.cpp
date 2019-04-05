/*

This example demonstrates how to use Oracle REF CURSORs
with SQLAPI++ Library.

To execute and test this example, first run following script
in SQLPlus to create test stored procedure that will return
REF CURSOR:

CREATE or replace PACKAGE TestRefCursorPkg AS
	TYPE TestRefCursorTyp IS REF CURSOR;
	PROCEDURE TestRefCursorProc (
	RefCursor OUT TestRefCursorTyp
	);
END TestRefCursorPkg;
/

CREATE or replace PACKAGE BODY TestRefCursorPkg AS
	PROCEDURE TestRefCursorProc (
	RefCursor OUT TestRefCursorTyp) IS
	localCursor TestRefCursorTyp;
	BEGIN
	    RefCursor := NULL;
		OPEN localCursor FOR SELECT * FROM dual;
		RefCursor := localCursor;
	END TestRefCursorProc;
END TestRefCursorPkg;
/

*/

#include <SQLAPI.h> // main SQLAPI++ header

#include <iostream>
using namespace std;

void OracleRefCursor();

int main()
{
	OracleRefCursor();
	return 0;
}


void OracleRefCursor()
{
	SAConnection con;

	try
	{
		SACommand cmd(&con);

		con.Connect(
			"demo", "scott", "tiger", SA_Oracle_Client);
		cout << "Connected OK!" << "\n";

		cmd.setCommandText("TestRefCursorPkg.TestRefCursorProc");
		cmd.Execute();
		cout << "Stored procedure executed OK!" << "\n";

		SACommand *pRefCursor = cmd.Param("REFCURSOR");
		while(pRefCursor->FetchNext())
		{
			cout 
				<< (const char*)pRefCursor->Field(1).Name() << "="
				<< (const char*)pRefCursor->Field(1).asString() << "\n";
		}
		cout << "Ref cursor fetched OK!" << "\n";
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

