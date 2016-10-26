#include <ibAPI.h>

void Check(
	const ISC_STATUS &error_code,
	ISC_STATUS *pSV,
	ibAPI *pibAPI)
{
	SAString sErr;

	if(error_code)
	{
		char sMsg[512];
		long nLen;
		ISC_STATUS *pvector; // Pointer to pointer to status vector.

		// walk through error vector and constract error string
		pvector = pSV; // (Re)set to start of status vector.
		while((nLen = pibAPI->isc_interprete(sMsg, &pvector)) != 0) // More messages?
		{
			if(!sErr.IsEmpty())
				sErr += "\n";
			sErr += SAString(sMsg, nLen);
		}

		throw SAException(
			SA_DBMS_API_Error,
			error_code, -1,
			sErr);
	}
}

void event_function(char *result, short length, char *updated);

typedef
struct tagEventData
{
	ibAPI *pibAPI;
	ibConnectionHandles *pibConnectionHandles;

	ISC_STATUS		StatusVector[20];		// Error status vector
	ISC_LONG event_id;
	char *event_buffer;
	char *result_buffer;
	long length;

	tagEventData()
	{
		pibAPI = NULL;
		pibConnectionHandles = NULL;

		event_id = 0;
		event_buffer = NULL;
		result_buffer = NULL;
		length = 0;
	}

	void que_events()
	{
		Check(pibAPI->isc_que_events(
			StatusVector,
			&pibConnectionHandles->m_db_handle,
			&event_id,
			(short)length,
			event_buffer,
			(isc_callback)event_function,
			this), StatusVector, pibAPI);
	}
} event_data_t;

DWORD WINAPI reque_event(void *pData)
{
	event_data_t &event_data = *(event_data_t*)pData;
	printf("%s\n", "reque_event");

	try
	{
		event_data.que_events();
	}
	catch(char *s)
	{
		printf("%s\n", s);
	}

	return 0;
}

void event_function(char *result, short length, char *updated)
{
	event_data_t &event_data = *(event_data_t*)result;

	for(short i = 0; i < length; ++i)
		event_data.result_buffer[i] = updated[i];

    event_data.pibAPI->isc_event_counts(
		(unsigned ISC_LONG *)event_data.StatusVector,
		(short)event_data.length,
		event_data.event_buffer,
		event_data.result_buffer);

	static bool bRegistered = false;
	if(bRegistered)
		printf("Fired!\n");
	else
		bRegistered = true;

	// use another thread to reque
	DWORD ThreadId;
	HANDLE hThread;
	hThread = CreateThread(
		NULL, 0,
		reque_event, &event_data,
		0, &ThreadId);
	WaitForSingleObject(hThread, INFINITE);
}

void InterBaseEvents()
{
	SAConnection con;
	SACommand cmd;
	event_data_t event_data;

	try
	{
		con.Connect(
			"transfer:d:/temp/siemens/controller.gdb",     // database name
			"sysdba",   // user name
			"masterkey",   // password
			SA_InterBase_Client);
		event_data.pibAPI = (ibAPI *)con.NativeAPI();
		event_data.pibConnectionHandles = (ibConnectionHandles *)con.NativeHandles();

		con.setAutoCommit(SA_AutoCommitOn);

		// register interest for "TestEvent" event
		event_data.length = event_data.pibAPI->isc_event_block(
			&event_data.event_buffer,
			&event_data.result_buffer,
			1,
			"TestEvent");

		event_data.que_events();
		
		// this should fire event
		cmd.setConnection(&con);
		cmd.setCommandText("a");
		cmd.Param("sEvent").setAsString() = "TestEvent";
		cmd.Execute();

		// wait for event to be fired
		getchar();
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
		printf("Err = %s\nCode = %d\n", (const char*)x.ErrText(),
			x.ErrNativeCode());
	}
}

