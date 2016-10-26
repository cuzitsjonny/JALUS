
int SQLServer_CE()
{
	SAConnection con;
	SAConnection conCE;
    conCE.setOption(_TSA("UseAPI")) = _TSA("OLEDB");
	conCE.setOption(_TSA("OLEDBProvider")) = _TSA("CompactEdition");
	conCE.setOption(_TSA("CreateDatabase")) = _TSA("TRUE");

	try
	{
		con.Connect(
			_TSA("srv2@pubs"), _TSA("sa"), _TSA("sa"), SA_SQLServer_Client);
		printf("SQLServer Connected OK!\n");
		conCE.Connect(
			_TSA("C:\\test.sdf"), _TSA("sa"), _TSA("sa"), SA_SQLServer_Client);
		printf("SQLCE Connected OK!\n");

		SACommand cmd(&con, _TSA("select user"));
		cmd.Execute();
		printf("SQLServer query executed OK!\n");
		while(cmd.FetchNext())
		{
			printf("%s=%s\n",
				cmd.Field(1).Name().GetMultiByteChars(),
				cmd.Field(1).asString().GetMultiByteChars());
		}
		printf("SQLServer data fetched OK!\n");

		SACommand cmdCE(&conCE, _TSA("create table t1(f1 nvarchar(20))"));
		cmdCE.Execute();
		cmdCE.setCommandText(_TSA("insert into t1(f1) values('test')"));
		cmdCE.Execute();
		cmdCE.setCommandText(_TSA("select * from t1"));
		cmdCE.Execute();
		printf("SQLCE query executed OK!\n");
		while(cmdCE.FetchNext())
		{
			printf("%s=%s\n",
				cmdCE.Field(1).Name().GetMultiByteChars(),
				cmdCE.Field(1).asString().GetMultiByteChars());
		}
		printf("cmdCE data fetched OK!\n");
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
		printf("ERROR: %s\n", x.ErrText().GetMultiByteChars());
	}

	return 0;
}

int SQLServer_CE_GUID()
{
	SAConnection con;
	con.setOption(_TSA("OLEDBProvider")) = _TSA("CompactEdition");
	con.setOption(_TSA("CreateDatabase")) = _TSA("TRUE");

	try
	{
		con.Connect(
			_TSA("C:\\test.sdf"), _TSA("sa"), _TSA("sa"), SA_SQLServer_Client);
		printf("SQLCE Connected OK!\n");

		SACommand cmd(&con, _TSA("create table t1(f1 uniqueidentifier, f2 nvarchar(100))"));
		cmd.Execute();
		cmd.setCommandText(_TSA("insert into t1(f1, f2) values('{6F9619FF-8B86-D011-B42D-00C04FC964FF}','test')"));
		cmd.Execute();
		cmd.setCommandText(_TSA("select * from t1"));
		cmd.Execute();
		printf("SQLCE query executed OK!\n");
		while(cmd.FetchNext())
		{
			printf("%s=%s\n",
				cmd.Field(1).Name().GetMultiByteChars(),
				cmd.Field(1).asString().GetMultiByteChars());
			printf("%s=%s\n\n",
				cmd.Field(2).Name().GetMultiByteChars(),
				cmd.Field(2).asString().GetMultiByteChars());
		}
		printf("SQLCE data fetched OK!\n");
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
		printf("ERROR: %s\n", x.ErrText().GetMultiByteChars());
	}

	return 0;
}