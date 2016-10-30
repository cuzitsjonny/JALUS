#include "PacketUtils.h"

BitStream* PacketUtils::createPacketBase(unsigned short remoteConnectionType, unsigned long packetID)
{
	BitStream* bs = new BitStream();

	bs->Write((unsigned char)ID_USER_PACKET_ENUM);
	bs->Write(remoteConnectionType);
	bs->Write(packetID);
	bs->Write((unsigned char)0);

	return bs;
}

BitStream* PacketUtils::createGMBase(long long objectID, short gameMessageID)
{
	BitStream* bs = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_GAME_MSG);

	bs->Write(objectID);
	bs->Write(gameMessageID);

	return bs;
}

void PacketUtils::writeStringToBitStream(string str, BitStream* bitStream, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		if (i < str.length())
			bitStream->Write(str[i]);
		else
			bitStream->Write((unsigned char)0);
	}
}

void PacketUtils::writeWStringToBitStream(wstring wstr, BitStream* bitStream, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		if (i < wstr.length())
			bitStream->Write(wstr[i]);
		else
			bitStream->Write((unsigned short)0);
	}
}

string PacketUtils::readStringFromBitStream(BitStream* bitStream, size_t length)
{
	vector<char> buf;

	bool flag = true;
	for (size_t i = 0; i < length; i++)
	{
		char c;
		bitStream->Read(c);

		if (c != 0)
		{
			if (flag)
				buf.push_back(c);
		}
		else
			flag = false;
	}

	string str = string(buf.begin(), buf.end());
	buf.clear();

	return str;
}

wstring PacketUtils::readWStringFromBitStream(BitStream* bitStream, size_t length)
{
	vector<wchar_t> buf;

	bool flag = true;
	for (size_t i = 0; i < length; i++)
	{
		wchar_t c;
		bitStream->Read(c);

		if (c != 0)
		{
			if (flag)
				buf.push_back(c);
		}
		else
			flag = false;
	}

	wstring wstr = wstring(buf.begin(), buf.end());
	buf.clear();

	return wstr;
}

CharacterStyle PacketUtils::readCharacterStyleFromBitStream(BitStream* bitStream)
{
	CharacterStyle style;

	bitStream->Read(style.shirtColor);
	bitStream->Read(style.shirtStyle);
	bitStream->Read(style.pantsColor);
	bitStream->Read(style.hairStyle);
	bitStream->Read(style.hairColor);
	bitStream->Read(style.lh);
	bitStream->Read(style.rh);
	bitStream->Read(style.eyebrows);
	bitStream->Read(style.eyes);
	bitStream->Read(style.mouth);

	return style;
}
