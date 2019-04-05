#include <stdio.h>
#include <stdlib.h>

#include <SQLAPI.h>
#include <samisc.h>

int main(int argc, char** argv)
{
	SAConnection con;

	try
	{
		con.setOption(_TSA("DB2CLI.LIBS")) =
			_TSA("D:\\download\\DB2\\clidriver\\bin\\db2cli64.dll");
		con.Connect(_TSA("Hostname=bedlam-home.bedlam;Database=test;ServiceName=50000;Protocol=TCPIP"),
			_TSA("db2inst1"), _TSA("java"), SA_DB2_Client);
		SACommand cmd(&con);
		cmd.setCommandText(_TSA("select f2 from t1 where f1=1"));
		cmd.Execute();
		while (cmd.FetchNext()) {
			sa_tprintf((const SAChar*)cmd[1].asString());
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