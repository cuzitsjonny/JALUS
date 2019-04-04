#include <stdio.h>
#include <stdlib.h>

#include <SQLAPI.h>
#include <samisc.h>

int main(int argc, char** argv)
{
	SAConnection con;

	try
	{
		//con.setOption(_TSA("ClientEncoding")) = _TSA("latin1");
		//con.Connect(_TSA("DSN=ol_informix1170;CLIENT_LOCALE=ru_RU.1251"), _TSA("informix"), _TSA("java"),
		//con.Connect(_TSA("Server=ol_informix1170;Database=test;DB_LOCALE=en_US.57372;NEEDODBCTYPESONLY=1"), _TSA("informix"), _TSA("java"),
		con.Connect(_TSA("Server=ol_informix1170;Database=test"), _TSA("informix"), _TSA("java"), SA_Informix_Client);

		//SACommand cmd(&con, _TSA("update t1 set f2=:1 where f1=1"));
		SACommand cmd(&con, _TSA("insert into t1(f1,f3) values(2,:1)"));
		cmd.Param(1).setAsBLob() = _TSA("01234567890");
		cmd.Execute();
		con.Commit();
		
		//cmd.setCommandText(_TSA("select f2 from t1"));
		cmd.setCommandText(_TSA("select f3 from t1"));
		cmd.Execute();
		while (cmd.FetchNext()) {
			SAString sStrData = cmd[1].asString();
			sa_tprintf((const SAChar*)sStrData);
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
