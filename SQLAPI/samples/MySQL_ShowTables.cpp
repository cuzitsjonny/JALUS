void MySQL_ShowTables()
{
	SAConnection con;
	try
	{
		con.Connect("localhost@mysql", "ODBC", "", SA_MySQL_Client);
		printf("SERVER: %s\n", (const char*)con.ServerVersionString());
		printf("CLIENT VER: %d.%d\n\n",
			(con.ClientVersion() >> 16),
			(con.ClientVersion() &0x0000FFFF));

		SACommand cmd(&con);
		//cmd.setCommandText("SHOW TABLES");
		cmd.setCommandText("SHOW COLUMNS from user");
		cmd.Execute();

		while( cmd.FetchNext() )
		{
			for( int i = 1; i <= cmd.FieldCount(); ++i)
			{
				printf("%s: %s, %s\n",
					(const char*)cmd[i].Name(),
					(const char*)cmd[i].asBytes(),
					(const char*)cmd[i].asString());
			}
			printf("\n");
		}
		printf("\n");
	}
    catch(SAException &x)
    {
        // print error message
        printf("%s\n", (const char*)x.ErrText());
    }

}

