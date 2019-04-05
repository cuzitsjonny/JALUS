#include <stdio.h>
#include <stdlib.h>

#include <conio.h>

#include <SQLAPI.h>
#include <samisc.h>

void test_scroll(SAConnection &con)
{
	SACommand cmd(&con);
    cmd.setOption(SACMD_SCROLLABLE) = SAOPT_TRUE;
	cmd.setOption("UseCursor") = "1";
	//cmd.setOption("UseDynamicCursor") = "1";

	cmd.setCommandText("select f1 from t1 order by f1");
	cmd.Execute();

	int ch;
	printf("ENTER n,p,f,l,a,r or q to quit.\n");

	do {
		ch = _getch();
		bool bResult = false;

		try
		{
			switch(ch)
			{
			case 'n':
				bResult = cmd.FetchNext();
				break;
			case 'p':
				bResult = cmd.FetchPrior();
				break;
			case 'f':
				bResult = cmd.FetchFirst();
				break;
			case 'l':
				bResult = cmd.FetchLast();
				break;
            case 'a':
                printf("ENTER row count: ");
                if (1 == scanf("%d", &ch))
                {
                    printf("%d\n", ch);
                    bResult = cmd.FetchPos(ch, false);
                }
                break;
            case 'r':
                printf("ENTER row count: ");
                if (1 == scanf("%d", &ch))
                {
                    printf("%d\n", ch);
                    bResult = cmd.FetchPos(ch, true);
                }
                break;
            }
		}
		catch(SAException &x)
		{
			printf("ERROR: %s\n", x.ErrText().GetMultiByteChars());
		}

		if( bResult )
            printf("RESULT: %s.\n", cmd[1].asString().GetMultiByteChars());
		else if( ch != 'q' )
			printf("NO DATA.\n");

	} while ( ch != 'q' );
}

int main(int argc, char **argv)
{
	SAConnection con;

	try
	{
		//con.setOption("UseAPI") = "OCI7";
		//con.Connect( "ora111", "sys", "java", SA_Oracle_Client);
		//con.setOption("UseAPI") = "DB-Library";
		//con.setOption("SSPROP_INIT_MARSCONNECTION") = "VARIANT_FALSE";
		//con.Connect( "BEDLAM-M\\EN2014@test", "sa", "java", SA_SQLServer_Client);
		//con.Connect( "LocalServer", "sa", "java", SA_ODBC_Client);
        //con.Connect("TestAccess", "", "", SA_ODBC_Client);
		//con.Connect( "test", "postgres", "java", SA_PostgreSQL_Client);
		//con.setAutoCommit(SA_AutoCommitOff);
        //con.Connect("test", "root", "", SA_MySQL_Client);
		con.Connect(_TSA("localhost@test2"), _TSA("admin"), _TSA("admin"), SA_CubeSQL_Client);

        test_scroll(con);
    }
	catch( SAException& x )
	{
		printf("ERROR: %s\n", x.ErrText().GetMultiByteChars());
		exit(1);
	}

	return 0;
}
