
DWORD WINAPI Oracle_Cancel_Thread(void *_pCon)
{
	SAConnection *pCon = (SAConnection *)_pCon;
	SACommand cmd;
	cmd.setConnection(pCon);

	try
	{
		cmd.setCommandText("Update DEPT set DNAME=DNAME");
		//cmd.setCommandText("sys.DBMS_PIPE.receive_message");
		//cmd << SAPos("pipename") << "myPipe" << SAPos("timeout") << 10L;
		cout << "Calling Execute!" << endl;
		cmd.Execute();
		cout << "Execute has been called!" << endl;
	}
    catch(SAException &x)
    {
		cout << "Oracle_Cancel_Thread: " << (const char*)x.ErrText() << endl;
	}

	return 0;
}

void Oracle_Cancel()
{
	SAConnection con2;
	SAConnection con;
	SACommand cmd2;
	SACommand cmd;
	cmd2.setConnection(&con2);
	cmd.setConnection(&con);

	try
	{
		con2.Connect(
			"test", "scott", "tiger", SA_Oracle_Client);
		//con.setOption("UseAPI") = "OCI7";
		con.Connect(
			"test", "scott", "tiger", SA_Oracle_Client);

		// block
		cmd2.setCommandText("Update DEPT set DNAME=DNAME");
		cmd2.Execute();

		DWORD ThreadId;
		HANDLE hThread;
		hThread = ::CreateThread(
			NULL, 0,
			Oracle_Cancel_Thread, &con,
			0, &ThreadId);
		WaitForSingleObject(hThread, 1000);
		cout << "Calling Cancel!" << endl;
		cmd.Open();
		cmd.Cancel();
		cout << "Cancel has been called!" << endl;

		WaitForSingleObject(hThread, 2000);
		cout << "Calling con2.Rollback()!" << endl;
		con2.Rollback();
		cout << "con2.Rollback() has been called!" << endl;

		WaitForSingleObject(hThread, INFINITE);
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
