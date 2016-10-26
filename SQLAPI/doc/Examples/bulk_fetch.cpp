
#include <stdio.h>  // for printf
#include <time.h>  // for time
#include <SQLAPI.h> // main SQLAPI++ header

void CreateSampleData(SACommand &cmd, int nRows);

int main(int argv, char *argc[])
{
    SAConnection con; // connection object
    SACommand cmd(&con);
    
    try
    {
        con.Connect(
            "test",
            "tester",
            "tester", SA_Oracle_Client);
        
        // Create and Insert test table
        bool bCreateTable = false;	// set to true if you want to create sample data
        if(bCreateTable)	// set to false when table is created and populated
        {
            CreateSampleData(cmd, 200000);
            con.Commit();
        }
        
        time_t start, finish;
        
        printf("Reading data without bulk support (default)...\n");
        time(&start);
        cmd.setCommandText("Select * from TEST_BULK");
        cmd.Execute();
        while(cmd.FetchNext())
        {
        }
        time(&finish);
        printf("Time in seconds: %g\n", difftime(finish, start));
        
        int nBulkSize = 1000;
        printf("Reading data with bulk support (Size=%d)...\n", nBulkSize);
        SAString sBulkSize;
        sBulkSize.Format("%d", nBulkSize);
        time(&start);
        cmd.setCommandText("Select * from TEST_BULK");
        cmd.setOption("PreFetchRows") = sBulkSize;
        cmd.Execute();
        while(cmd.FetchNext())
        {
        }
        time(&finish);
        printf("Time in seconds: %g\n", difftime(finish, start));
    }
    catch(SAException &x)
    {
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message
        printf("%s\n", (const char*)x.ErrText());
    }

	return 0;
}

void CreateSampleData(SACommand &cmd, int nRows)
{
    printf("Creating test table...\n");
    cmd.setCommandText(
        "CREATE TABLE TEST_BULK ("
        "       FINTEGER             INTEGER NOT NULL,"
        "       FVARCHAR20           VARCHAR(20),"
        "       PRIMARY KEY (FINTEGER)"
        ")");
    
    cmd.Execute();
    printf("Populating test table (rows=%d)...\n", nRows);
    cmd.setCommandText(
        "Insert into TEST_BULK (FINTEGER, FVARCHAR20) values (:1, :2)");
    
    for(int i = 0; i < nRows; ++i)
    {
        SAString s;
        s.Format("VC%d", i);
        cmd << (long)i << s;
        cmd.Execute();
        
        if(((i+1) % 1000) == 0)
            printf("%d rows inserted...\n", i+1);
    }
}
