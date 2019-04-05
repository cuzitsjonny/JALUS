void Oracle_PLSQL_Return()
{
	SAConnection con;
	SACommand cmd;
	cmd.setConnection(&con);

	SAString sBlock = "begin  :omessage := 'my string'; end;";
	cmd.setCommandText(sBlock);

	try
	{
		con.Connect(
			"test", "scott", "tiger", SA_Oracle_Client);

		cmd.DestroyParams();
		cmd.CreateParam("omessage", SA_dtString, -1, 20, -1, -1, SA_ParamInputOutput);
		cmd.Cre

		cmd.Execute();

		printf("%s\n", (const SAChar*)cmd.Param("omessage").asString());
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