#pragma once
#include "Server.h"

class WorldInstance
{
public:
	static void processWorldPacket(BitStream* data, SystemAddress clientAddress, ClientToWorldPacketID packetID);
	static void processChatPacket(BitStream* data, SystemAddress clientAddress, ChatPacketID packetID);
	static void sendWorldInfo(SystemAddress clientAddress);
	static void sendCharacterData(SystemAddress clientAddress);
	static void sendServerState(SystemAddress clientAddress);
	static void broadcastPositionUpdate(BitStream* data, SystemAddress clientAddress);
};