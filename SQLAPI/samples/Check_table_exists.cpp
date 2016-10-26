#include <stdio.h>
#include <stdlib.h>

#include <SQLAPI.h>
#include <samisc.h>

void connect(SAConnection &con)
{
	sa_printf(_TSA("\nConnect to DB..."));

	try
	{
		con.Connect(_TSA("BEDLAM-M\\EN2012@test"), _TSA(""), _TSA(""), SA_SQLServer_Client);
	}
	catch(SAException &x)
	{
		sa_printf(_TSA("ERR:\n"));
		sa_printf((const SAChar*)x.ErrText());
		sa_printf(_TSA("\n"));

		exit(1);
	}

	sa_printf(_TSA("Done\n"));
}

void createTable(SAConnection& con)
{
	sa_printf(_TSA("\nCreate table..."));

	try
	{
		SACommand cmd(&con, _TSA("CREATE TABLE t1(f1 int IDENTITY(1,1) NOT NULL, f2 varchar(200))"));
		cmd.Execute();
	}
	catch(SAException &x)
	{
		sa_printf(_TSA("ERR:\n"));
		sa_printf((const SAChar*)x.ErrText());
		sa_printf(_TSA("\n"));

		exit(2);
	}

	sa_printf(_TSA("Done\n"));
}

int main(int argc, char* argv[])
{
	SAConnection con; // create connection object

	connect(con);

	try
	{
		sa_printf(_TSA("\nCheck table exists..."));

		SACommand cmd(&con, _TSA("select * from t1"));
		cmd.Execute();

		sa_printf(_TSA("Done\n"));
	}
	catch(SAException &x)
	{
		sa_printf(_TSA("ERR:\n"));
		sa_printf((const SAChar*)x.ErrText());
		sa_printf(_TSA("\n"));

		if( con.isAlive() )
			createTable(con);			
		else
			exit(1);
	}	

	try
	{
		SACommand cmd(&con, _TSA("insert into t1(f2) values('test')"));
		cmd.Execute();
		con.Commit();

		cmd.setCommandText(_TSA("select f2 from t1"));
		cmd.Execute();

		while( cmd.isResultSet() )
		{
			while( cmd.FetchNext() )
			{
				sa_printf((const SAChar*)cmd[1].asString());
				sa_printf(_TSA("\n"));
			}
		}
	}
	catch(SAException &x)
	{
		sa_printf(_TSA("\nERR: "));
		sa_printf((const SAChar*)x.ErrText());
		sa_printf(_TSA("\n"));
	}
	return 0;
}
