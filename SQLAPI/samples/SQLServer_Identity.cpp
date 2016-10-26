void SQLServer_Identity()
{
	SAConnection con;
	SACommand cmd;
	cmd.setConnection(&con);

	try
	{
		con.Connect(
			"test", "sa", "", SA_SQLServer_Client);

		cmd.setCommandText(
			"INSERT INTO cmnpart VALUES (:cmndescription)\n"
			"commit\n"
			"SELECT @@identity cmnpartid");

		cmd << SAPos("cmndescription") << "Value1";
		cmd.Execute();
		cmd.FetchNext();
		long id1 = cmd[1];	// by index
		cout << "id1 is " << id1 << "\n";

		cmd << SAPos("cmndescription") << "Value2";
		cmd.Execute();
		cmd.FetchNext();
		long id2 = cmd["cmnpartid"];	// by name
		cout << "id2 is " << id2 << "\n";

		if(id2 == id1 + 1)
			cout << "Everything is OK!\n";
		else
			cout << "Problem! Something is not working!\n";
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
