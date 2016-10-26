#include "PingTool.h"

SocketDescriptor PingTool::socketDescriptor;

void PingTool::init()
{
	socketDescriptor = SocketDescriptor();
}

bool PingTool::ping(const char* host, unsigned short port, const char* password, int passwordLength)
{
	bool r = false;

	RakPeerInterface* client = RakNetworkFactory::GetRakPeerInterface();
	bool result = client->Startup(1, 30, &socketDescriptor, 1);

	if (result)
	{
		result = client->Connect(host, port, password, passwordLength);

		if (result)
		{
			Packet* packet;
			bool receivedResponse = false;
			while (!receivedResponse)
			{
				packet = client->Receive();
				if (packet == NULL)
					continue;

				receivedResponse = true;

				if (packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
					r = true;
				else
					r = false;

				client->DeallocatePacket(packet);
			}
		}
	}

	RakNetworkFactory::DestroyRakPeerInterface(client);

	return r;
}
