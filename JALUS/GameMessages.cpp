#include "GameMessages.h"
#include "Logger.h"
#include "Objects.h"
#include "Server.h"
#include "PacketUtils.h"

void GameMessages::processGameMessage(BitStream* data, SystemAddress clientAddress)
{
	long long objectID;
	data->Read(objectID);
	unsigned short gameMessageID;
	data->Read(gameMessageID);

	switch (gameMessageID)
	{

	case GAME_MESSAGE_ID_READY_FOR_UPDATES:
	{
		long long ready;
		data->Read(ready);
		Logger::info("Object is ready for updates! (Object: " + to_string(ready) + ") (Address: " + string(clientAddress.ToString()) + ")");

		long lot = Objects::getLOT(ready);
		if (lot > -1)
		{
			if (lot == 1)
			{
				Server::sendPacket(PacketUtils::createGMBase(ready, 1642), clientAddress);
				Server::sendPacket(PacketUtils::createGMBase(ready, 509), clientAddress);

				BitStream* flagChange = PacketUtils::createGMBase(ready, 472);
				flagChange->Write((unsigned char)185);
				flagChange->Write((unsigned long)0);
				Server::sendPacket(flagChange, clientAddress);
			}
		}
		break;
	}

	default:
		{
			Logger::info("Server received an unknown GameMessage! (GameMessageID: " + to_string(gameMessageID) + ") (ObjectID: " + to_string(objectID) + ") (Address: " + string(clientAddress.ToString()) + ")");
			break;
		}

	}
}
