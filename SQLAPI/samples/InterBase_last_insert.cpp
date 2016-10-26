#include <stdio.h>
#include <stdlib.h>

#include <SQLAPI.h>
#include <samisc.h>

int FB2_LastInsert()
{
	/*
	CREATE TABLE T1
	(
	F1 INTEGER NOT NULL,
	F2 VARCHAR(20) NOT NULL
	);

	CREATE GENERATOR GEN_T1_F1;
	SET GENERATOR GEN_T1_F1 TO 0;

	SET TERM !! ;
	CREATE TRIGGER T1_BI FOR T1
	ACTIVE BEFORE INSERT POSITION 0
	AS
	BEGIN
	IF (NEW.F1 IS NULL) THEN
	NEW.F1 = GEN_ID(GEN_T1_F1, 1);
	END!!
	SET TERM ; !!
	*/

	SAConnection con;
	SACommand cmd(&con,
		_TSA("INSERT INTO t1 (f2) values ('val x') returning f1"));

	try
	{
		con.Connect(_TSA("test"), _TSA("sysdba"), _TSA("masterkey"),
			SA_InterBase_Client);
		cmd.Execute();

		if (cmd.FetchNext())
			printf("F1 value: %d\n", cmd.Field(1).asLong());
	}
	catch (SAException &x)
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
		catch (SAException &)
		{
		}

		// print error message
		printf("%s\n", x.ErrText().GetMultiByteChars());
	}

	return 0;
}
