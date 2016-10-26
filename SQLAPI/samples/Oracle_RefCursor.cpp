#include <stdio.h>
#include <stdlib.h>

#include <SQLAPI.h>
#include <samisc.h>

void Ora_Fetch_Cursor(SACommand& cmd, int stage)
{
	for (int s = 1; s < stage; ++s) printf(" ");

	printf("START CURSOR #%d\n", stage);

	try
	{
		if (cmd.isResultSet())
		{
			//cmd.setOption(SACMD_PREFETCH_ROWS) = _TSA("5");
			while (cmd.FetchNext())
			{
				for (int i = 1; i <= cmd.FieldCount(); ++i)
				{
					SAField& f = cmd.Field(i);
					if (SA_dtCursor == f.FieldType())
					{
						SACommand* cur = f.asCursor();
						if (NULL == cur)
						{
							for (int s = 1; s < stage; ++s) printf(" ");

							printf("NULL result set\n");
						}
						else
							Ora_Fetch_Cursor(*cur, stage + 1);
					}
					else
					{
						for (int s = 1; s < stage; ++s) printf(" ");

						printf("VAL #%d: %s\n",
							i, f.asString().GetMultiByteChars());
					}
				}
			}
		}
		else
		{
			for (int s = 1; s < stage; ++s) printf(" ");

			printf("Empty result set\n");
		}
	}
	catch (SAException &x)
	{
		for (int s = 1; s < stage; ++s) printf(" ");

		printf("ERROR: %s\n", x.ErrText().GetMultiByteChars());
	}

	for (int s = 1; s < stage; ++s) printf(" ");

	printf("END CURSOR #%d\n", stage);
}

/*
create function func1
return sys_refcursor
is
v_cur			sys_refcursor;
begin
open v_cur for
select
1 a
, cursor(select level l from dual connect by level < 5) c
from dual
connect by level < 5;
return v_cur;
end;

create procedure proc1 (p_cur out sys_refcursor)
is
begin
open p_cur for
select level l from dual connect by level < 50;
end;
/
*/

int Oracle_RefCur()
{
	SAConnection con;
	//con.setOption(_TSA("UseAPI")) = _TSA("OCI7");
	try {
		con.Connect(_TSA("ora102"),
			_TSA("scott"), _TSA("tiger"), SA_Oracle_Client);

		SACommand cmd(&con, _TSA("select func1() from t1"));
		//SACommand cmd(&con, _TSA("proc1"));
		while (true)
		{
			//cmd.setOption(SACMD_PREFETCH_ROWS) = _TSA("5");
			cmd.Execute();
			Ora_Fetch_Cursor(cmd, 1);

			/*
			SACommand* pCur = cmd.Param(_TSA("p_routes")).asCursor();
			if( NULL != pCur )
			Ora_Fetch_Cursor(*pCur, 1);
			*/
		}
	}
	catch (SAException &x)
	{
		printf("ERROR: %s\n", x.ErrText().GetMultiByteChars());
	}

	return 0;
}
