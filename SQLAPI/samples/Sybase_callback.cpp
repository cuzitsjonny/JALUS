// -I../SQLAPI/include -I../SQLAPI/include/sybase
#include <sybAPI.h>

void sybMsgCallback(void *pMessageStruct, bool bIsServerMessage, void *pAddInfo)
{
	if (bIsServerMessage)
	{
		// native OCS server message
		CS_SERVERMSG *message = (CS_SERVERMSG *)pMessageStruct;
		//..
	}
	else
	{
		// native OCS client message
		CS_CLIENTMSG *message = (CS_CLIENTMSG *)pMessageStruct;
		//..
	}
}

void test1()
{
	SAConnection con;
	con.Connect("...", "sa", "pwd", SA_Sybase_Client);

	// per connection handler
	sybAPI::SetMessageCallback(sybMsgCallback, NULL, &con);
	//...
}
