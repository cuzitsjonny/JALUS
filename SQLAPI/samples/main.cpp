#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include <SQLAPI.h>

void CreateSampleData(SACommand &cmd, int nRows);

int main(int argv, char *argc[])
{
	SAConnection con;
	SACommand cmd;

	try
	{
		int cnt = 0;
		time_t start, finish;

		con.setOption(_TSA("SQL_COPT_SS_PRESERVE_CURSORS")) = _TSA("SQL_PC_OFF");

		con.Connect("tcp:bedlam-m.bedlam\\sql2014en,1433@test", "sa", "java", SA_SQLServer_Client);

		con.setIsolationLevel(SA_ReadCommitted);
		con.setAutoCommit(SA_AutoCommitOff);

		cmd.setConnection(&con);

		bool bCreateTable = false;
		if (bCreateTable) {
			CreateSampleData(cmd, 200);
			con.Commit();
		}

		int nBulkSize = 20;
		printf("Reading data with bulk support (Size=%d)...\n", nBulkSize);
		SAString sBulkSize;

		sBulkSize.Format("%d", nBulkSize);

		cmd.setCommandText("Select * from TEST_BULK");
		cmd.setOption("PreFetchRows") = sBulkSize;
		cmd.setOption("SQL_ATTR_CONCURRENCY") = "SQL_CONCUR_ROWVER";
		cmd.setOption("Scrollable") = "true";

		time(&start);
		cmd.Execute();
		while (cmd.FetchNext())
		{
			con.Commit();
			printf("Fetching...%d\n", ++cnt);
		}
		time(&finish);
		printf("Time in seconds: %g\n", difftime(finish, start));

	}
	catch (SAException &x) {
		try {
			con.Rollback();
		}
		catch (SAException &) {}
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

	for (int i = 0; i < nRows; ++i)
	{
		SAString s;
		s.Format("VC%d", i);
		cmd << (long)i << s;
		cmd.Execute();

		if (((i + 1) % 1000) == 0)
			printf("%d rows inserted...\n", i + 1);
	}
}

