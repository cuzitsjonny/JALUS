/*
create table my_tools_tbl (f1 integer, f2 clob, primary key (f1));

CREATE OR REPLACE PACKAGE my_tools IS
        TYPE ref_cursor IS REF CURSOR;
END my_tools ;
/
CREATE OR REPLACE PACKAGE BODY my_tools IS
END my_tools;
/

CREATE OR REPLACE PROCEDURE my_test(a CLOB, b INTEGER, oc out my_tools.ref_cursor) AS
BEGIN
 insert into my_tools_tbl values (b, a);
 OPEN oc FOR SELECT f1, f2 FROM my_tools_tbl where f1 = b;
END;
/

*/
void Oracle_SP_RefCursor_CLOB()
{
SAConnection con;
SACommand cmd;
cmd.setConnection(&con);

try
{
	con.Connect(
		"dima",
		"system", "manager", SA_Oracle_Client);
	cout << "Connected OK!" << "\n";
	cout << (const char*)con.ServerVersionString() << endl;
	cout << (con.ClientVersion() >> 16) << "." << (con.ClientVersion() &0x0000FFFF) << endl;
	cout << SAGlobals::GetVersionMajor() << SAGlobals::GetVersionMinor() << SAGlobals::GetVersionBuild() << endl;

	cmd.setCommandText("delete from my_tools_tbl");
	cmd.Execute();

	cmd.setCommandText("my_test");
	cmd.Param("a").setAsCLob() = "<ROWSET><ROW><NAZOV>id_obj</NAZOV></ROW><ROW><NAZOV>id_vlast</NAZOV></ROW><ROW><NAZOV>typ</NAZOV></ROW></ROWSET>";
	cmd.Param("b").setAsLong() = 1029;
	cmd.Execute();
	cout << "Stored procedure executed OK!" << "\n";

	SACommand *pRefCursor = cmd.Param("oc").asCursor();
	while(pRefCursor->FetchNext())
	{
		cout 
			<< (const char*)pRefCursor->Field(1).Name() << "="
			<< (const char*)pRefCursor->Field(1).asString() << "\n";
		cout 
			<< (const char*)pRefCursor->Field(2).Name() << "="
			<< (const char*)pRefCursor->Field(2).asString() << "\n";
	}
	cout << "Ref cursor fetched OK!" << "\n";
}
catch(SAException &x)
{
    try
    {
        con.Rollback();
    }
    catch(SAException &)
    {
    }
    cout << (const char*)x.ErrText() << endl;
}
}
