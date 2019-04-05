#include <stdio.h>
#include <stdlib.h>

#include <SQLAPI.h>
#include <samisc.h>

int main(int argc, char* argv[])
{
	SAConnection con; // create connection object
	try
	{
		con.Connect(
			"ora111",  // database name
			"sys",				// user name
			"java",			// password
			SA_Oracle_Client);
		SACommand cmd(&con, _TSA("delete from t1"));
		cmd.Execute();
		cmd.setCommandText(_TSA("insert into t1(id,description) values(t1_seq.nextval, 'test') returning id into :idval"));
		SAParam& p = cmd.Param(_TSA("idval"));
		p.setParamDirType(SA_ParamOutput);
		// !important!
		p.setAsLong();
		p.setParamType(SA_dtLong);
		cmd.Execute();
		long nID = p.asLong();
		sa_tprintf(_TSA("NEW ID %d\n"), nID);
		cmd.setCommandText(_TSA("select id,description from t1"));
		cmd.Execute();
		while (cmd.FetchNext())
		{
			sa_tprintf(_TSA("%d "), cmd[1].asLong());
			sa_tprintf((const SAChar*)cmd[2].asString());
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
