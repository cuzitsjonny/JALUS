#include <SQLAPI.h>

using namespace std;

void Oracle_PreFetchRows()
{
	SAConnection con;
	SACommand cmd(&con,"select * from test_tbl");

    try
    {
		//con.setOption("UseAPI") = "OCI7";
		con.Connect(
			"test",
			"scott", "tiger", SA_Oracle_Client);


		char *sPreFetchRows[] = {"1", "2", "3", "4", "5", "6", "7"};
		for(int i = 0; i < sizeof(sPreFetchRows)/sizeof(char*); ++i)
		{
			cmd.setOption("PreFetchRows") = sPreFetchRows[i];
			cmd.Execute();
			
			while(cmd.FetchNext())
			{
				cout 
					<< cmd[1].isNull()
					<< endl;
			}

			cout << endl;
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
