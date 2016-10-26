
/*
create   procedure test 
 @obj uniqueidentifier,
 @user int,
 @function int,
 @acc int OUTPUT
as
 set @acc=5
 select @obj
*/
void SQLServer_SP_uniqueidentifier()
{
	SAConnection con;
	SACommand cmd;
	cmd.setConnection(&con);

    try
    {
		con.Connect(
			"srv2@pubs",
			"sa", "sa", SA_SQLServer_Client);
		cout << "Connected OK!" << "\n";

		cmd.setCommandText("proc1");
		unsigned char a[16] = {
			0x00, 0x5B, 0x58, 0x67,
			0x50, 0xB2,
			0x37, 0x4B,
			0xA3, 0x57, 
			0x6E, 0xDA, 0x54, 0x0D, 0x22, 0x5E};
		cmd.Param("obj").setAsBytes() =
			SAString((const void*)a, sizeof(a));
		cmd.Param("user").setAsLong() = 1;
		cmd.Param("function").setAsLong() = 4;
		cmd.Execute();
		cout << "Stored procedure executed OK!" << "\n";

		while(cmd.FetchNext())
			cout << (const char*)cmd.Field(1).asString() << endl;

		//============================
		//Return value can be equal 5, but it isn't!
		//=============================
		cout << cmd.Param("acc").asLong() << endl;
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
        cout << (const char*)x.ErrText() << endl;
    }
}


void SQLServer_uniqueidentifier()
{
	SAConnection con;
	SACommand cmd;
	cmd.setConnection(&con);

    try
    {
		con.Connect("srv2@pubs", "sa", "sa", SA_SQLServer_Client);

		bool bDropTable = true;
		bool bCreateTable = true;
		if(bDropTable)
		{
			cmd.setCommandText(
				"drop table test_uniqueidentifier");
			cmd.Execute();
			cout << "Table dropped..." << endl;
		}
		if(bCreateTable)
		{
			cmd.setCommandText(
				"create table test_uniqueidentifier ("
				" f1 integer ,"
				" f2 uniqueidentifier)");
			cmd.Execute();
			cout << "Table created..." << endl;
		}

		cmd.setCommandText("delete from test_uniqueidentifier");
		cmd.Execute();

		cmd.setCommandText(
			"Declare @myid uniqueidentifier;"
			"Set @myid = NEWID();"
			"Insert into test_uniqueidentifier values (:1, @myid);"
			"Select @myid");
		cmd << 1L;
		cmd.Execute();
		cmd.FetchNext();
		SAString sMyID = cmd[1].asBytes();

		cmd.setCommandText(
			"Insert into test_uniqueidentifier values (:1, :2);"
			"Insert into test_uniqueidentifier values (:3, :4);"
			"Select a.f1, a.f2 "
			"from test_uniqueidentifier a JOIN test_uniqueidentifier b "
			"on a.f2 = b.f2");
		cmd << 2L << SABytes(sMyID);
		cmd << 3L << SABytes(SAString(
			"\x6F\x96\x19\xFF\x8B\x86\xD0\x11\xB4\x2D\x00\xC0\x4F\xC9\x64\xFF", 16));
		cmd.Execute();

		while(cmd.FetchNext())
		{
			cout << (const char*)cmd[1].asString() << "\t"
				<< (const char*)cmd[2].asString() << endl;
		}
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
        cout << (const char*)x.ErrText() << endl;
    }
}
