#include "LDF.h"
#include "PacketUtils.h"

LDF::LDF()
{
	this->keys = 0;
	this->storage = new RakNet::BitStream();
}

LDF::~LDF()
{
	delete this->storage;
}

// WSTRING | 0
void LDF::writeWString(std::wstring key, std::wstring value)
{
	keys++;
	storage->Write((unsigned char)(key.length() * 2));
	PacketUtils::writeWStringToBitStream(key, storage, key.length());
	storage->Write((unsigned char)0);

	storage->Write((unsigned long)value.length());
	PacketUtils::writeWStringToBitStream(value, storage, value.length());
}

// S32 | 1
void LDF::writeLong(std::wstring key, long value)
{
	keys++;
	storage->Write((unsigned char)(key.length() * 2));
	PacketUtils::writeWStringToBitStream(key, storage, key.length());
	storage->Write((unsigned char)1);

	storage->Write(value);
}

// FLOAT | 3
void LDF::writeFloat(std::wstring key, float value)
{
	keys++;
	storage->Write((unsigned char)(key.length() * 2));
	PacketUtils::writeWStringToBitStream(key, storage, key.length());
	storage->Write((unsigned char)3);

	storage->Write(value);
}

// U32 | 5
void LDF::writeUnsignedLong(std::wstring key, unsigned long value)
{
	keys++;
	storage->Write((unsigned char)(key.length() * 2));
	PacketUtils::writeWStringToBitStream(key, storage, key.length());
	storage->Write((unsigned char)5);

	storage->Write(value);
}

// BOOLEAN | 7
void LDF::writeBoolean(std::wstring key, bool value)
{
	keys++;
	storage->Write((unsigned char)(key.length() * 2));
	PacketUtils::writeWStringToBitStream(key, storage, key.length());
	storage->Write((unsigned char)7);

	storage->Write((unsigned char)value);
}

// S64 | 8
void LDF::writeLongLong(std::wstring key, long long value)
{
	keys++;
	storage->Write((unsigned char)(key.length() * 2));
	PacketUtils::writeWStringToBitStream(key, storage, key.length());
	storage->Write((unsigned char)8);

	storage->Write(value);
}

// S64 (OBJECTID) | 9
void LDF::writeObjectID(std::wstring key, long long value)
{
	keys++;
	storage->Write((unsigned char)(key.length() * 2));
	PacketUtils::writeWStringToBitStream(key, storage, key.length());
	storage->Write((unsigned char)9);

	storage->Write(value);
}

// RAW DATA (BYTES) | 13
void LDF::writeRaw(std::wstring key, RakNet::BitStream* value)
{
	keys++;
	storage->Write((unsigned char)(key.length() * 2));
	PacketUtils::writeWStringToBitStream(key, storage, key.length());
	storage->Write((unsigned char)13);

	storage->Write((unsigned long)value->GetNumberOfBytesUsed());
	storage->Write(value, value->GetNumberOfBitsUsed());

	delete value;
}

void LDF::writeToBitStream(RakNet::BitStream* bitStream)
{
	bitStream->Write(keys);
	bitStream->Write(storage, storage->GetNumberOfBitsUsed());
}

unsigned long LDF::getSize()
{
	return (unsigned long)(storage->GetNumberOfBytesUsed() + 4);
}
