#include <sybAPI.h>

void Sybase_API()
{
	SAConnection con, con2;

	try
	{
		CS_RETCODE rc;

		con2.Connect("snx@test", "sa", "sybase", SA_Sybase_Client);
		sybAPI *psybAPI2 = (sybAPI *)con2.NativeAPI();
		sybConnectionHandles *psybConnectionHandles2 = (sybConnectionHandles *)con2.NativeHandles();

		CS_CHAR	server_name[1024];
		CS_INT snamelen;
		rc = psybAPI2->ct_con_props(
			psybConnectionHandles2->m_connection,
			CS_GET,
			CS_SERVERNAME,
			server_name,
			sizeof(server_name),
			&snamelen);

		con.setClient(SA_Sybase_Client);
		sybAPI *psybAPI = (sybAPI *)con.NativeAPI();
		sybConnectionHandles *psybConnectionHandles = (sybConnectionHandles *)con.NativeHandles();

		CS_LOGINFO	*pLOGINFO = NULL;
		psybAPI2->ct_getloginfo(psybConnectionHandles2->m_connection, &pLOGINFO);
		// allocate connection handle manually
		psybAPI->ct_con_alloc(psybConnectionHandles->m_context, &psybConnectionHandles->m_connection);
		// and copy login info into it
		psybAPI->ct_setloginfo(psybConnectionHandles->m_connection, pLOGINFO);

		rc = psybAPI->ct_connect(
			psybConnectionHandles->m_connection,
			server_name,
			snamelen);

		rc = rc;
		SACommand cmd(&con, "Select user");
		cmd.Execute();
		cmd.FetchNext();
		cout << (const char*)cmd[1].asString();
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
		printf("%s\ncode = %d\n", (const char*)x.ErrText(),
			x.ErrNativeCode());
	}
}