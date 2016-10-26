void ODBC_Excel()
{
	SAConnection con; // connection object
	
	try
	{
		SACommand cmd(&con);
		con.Connect("Driver={Microsoft Excel Driver (*.xls)};DefaultDir=D:\\", "", "", SA_ODBC_Client);
		cout << (const char*)con.ServerVersionString() << endl;
		cout << (con.ClientVersion() >> 16) << "."
			<< (con.ClientVersion() &0x0000FFFF) << endl;

		cmd.setCommandText("SELECT t1.f1,t1.f2 FROM `D:\\test`.`Sheet1$` t1");
		cmd.Execute();

		while(cmd.FetchNext())
			cout << (const char*)cmd[1].asString() << " " <<
				(const char*)cmd[2].asString() << endl;

	}
    catch(SAException &x)
    {
        // print error message
        printf("%s\n", (const char*)x.ErrText());
    }
}
