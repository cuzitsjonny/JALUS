// example of Informix connection string
// tested with version 2.8 and 3.32
// replace SERVICE=, HOST=, SERVER=, DATABASE=, UID=, PWD=
// with your values
const char *sConnectString =
	"DRIVER={};"
	"SERVICE=ol_deep;"
	"HOST=deep;"
	"PROTOCOL=OLSOCTCP;"
	"SERVER=ol_deep;"
	"DATABASE=sysmaster;";


void Informix_Connect()
{
	SAConnection con;
	SACommand cmd;
	cmd.setConnection(&con);
	cmd.setCommandText("Select * from authors");

	try
	{
		con.Connect(
//			"DRIVER={};"
			"PROTOCOL=olsoctcp;"	// protocol in lower case
			"SERVICE=1025;"
			"SERVER=ol_dell;"
			"HOST=dell.cit;"
			"DATABASE=sysmaster;",
			"informix", "informix", SA_Informix_Client);

		cmd.Execute();
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
