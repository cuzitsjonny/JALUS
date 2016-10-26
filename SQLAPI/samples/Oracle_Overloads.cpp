void Oracle_Overloads()
{
	SAConnection con;
	SACommand cmd;
	cmd.setConnection(&con);

	try
	{
		con.Connect(
			"test", "scott", "tiger", SA_Oracle_Client);

		// be default first overload will be described/called
		cmd.setCommandText("test_overload.func");
		cmd.Execute();
		cout << (const char*)cmd.Param("RETURN_VALUE").asString() << endl;

		// explicitly specify first overload
		cmd.setCommandText("test_overload.func");
		cmd.setOption("Overload") = "1";
		cmd.Execute();
		cout << (const char*)cmd.Param("RETURN_VALUE").asString() << endl;

		// explicitly specify second overload
		cmd.setCommandText("test_overload.func");
		cmd.setOption("Overload") = "2";
		cmd.Execute();
		cout << (const char*)cmd.Param("RETURN_VALUE").asString() << endl;

		// explicitly specify 3rd overload
		cmd.setCommandText("test_overload.func");
		cmd.setOption("Overload") = "3";
		cmd.Execute();
		cout << (const char*)cmd.Param("RETURN_VALUE").asString() << endl;

		// explicitly specify 4th overload
		cmd.setCommandText("test_overload.func");
		cmd.setOption("Overload") = "4";
		cmd.Execute();
		cout << (const char*)cmd.Param("RETURN_VALUE").asString() << endl;
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
        cout << (const char*)x.ErrText() << endl;
    }
}

/*
create or replace package pack1 as
 procedure pr1(p1 in integer, p2 in varchar2);
 procedure pr1(p1 in integer, p2 in integer);
end;
/

create or replace package body pack1 as
 procedure pr1(p1 in integer, p2 in integer) is
   begin
    null;
   end;
 procedure pr1(p1 in integer, p2 in varchar2) is
   begin
    null;
   end;
end;
/
*/

int main(int argc, char* argv[])
{
	SAConnection con;

	try
	{
		con.Connect("bedlam.cit:1521/ora102", "sys", "java",
			SA_Oracle_Client);

		SACommand cmd(&con, "pack1.pr1");
		cmd.setOption("Overload") = "1";
		printf("pack1.pr1 v.1\n");
		for(int i = 0; i < cmd.ParamCount(); ++i)
		{
			printf("%s type: %d\n",
				(const char*)cmd.ParamByIndex(i).Name(),
				cmd.ParamByIndex(i).ParamNativeType());
		}
		printf("\n");

		cmd.setCommandText("pack1.pr1");
		cmd.setOption("Overload") = "2";
		printf("pack1.pr1 v.2\n");
		for(int i = 0; i < cmd.ParamCount(); ++i)
		{
			printf("%s type: %d\n",
				(const char*)cmd.ParamByIndex(i).Name(),
				cmd.ParamByIndex(i).ParamNativeType());
		}
		printf("\n");
	}
    catch(SAException &x)
    {
		printf("ERROR: %s\n", (const char*)x.ErrText());
    }

	return 0;
}

