#pragma once
#include "Server.h"

class CharactersInstance
{
public:
	static void processPacket(BitStream* data, SystemAddress clientAddress, ClientToWorldPacketID packetID);

	static void sendCharacterList(SystemAddress clientAddress);
	static long long performCharacterCreation(BitStream* data, SystemAddress clientAddress);
	static long long performCharacterDeletion(BitStream* data, SystemAddress clientAddress);
	static long long performCharacterRenaming(BitStream* data, SystemAddress clientAddress);
	static long long redirectToWorldInstance(BitStream* data, SystemAddress clientAddress);
};