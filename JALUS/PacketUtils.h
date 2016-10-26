#pragma once
#include "Server.h"
#include "CharacterStyles.h"

class PacketUtils
{
public:
	static BitStream* createPacketBase(unsigned short remoteConnectionType, unsigned long packetID);
	static BitStream* createGMBase(long long objectID, short gameMessageID);

	static void writeStringToBitStream(string str, BitStream* bitStream, size_t length);
	static void writeWStringToBitStream(wstring wstr, BitStream* bitStream, size_t length);

	static string readStringFromBitStream(BitStream* bitStream, size_t length);
	static wstring readWStringFromBitStream(BitStream* bitStream, size_t length);

	static CharacterStyle readCharacterStyleFromBitStream(BitStream* bitStream);
};