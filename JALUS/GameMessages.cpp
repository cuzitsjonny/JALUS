#include "GameMessages.h"
#include "Logger.h"

void GameMessages::processGameMessage(BitStream* data, SystemAddress clientAddress)
{
	long long objectID;
	data->Read(objectID);
	GameMessageID gameMessageID;
	data->Read(gameMessageID);

	switch (gameMessageID)
	{

	default:
		{
			Logger::info("Server received an unknown GameMessage! (GameMessageID: " + to_string(gameMessageID) + ") (ObjectID: " + to_string(objectID) + ") (Address: " + string(clientAddress.ToString()) + ")");
			break;
		}

	}
}
