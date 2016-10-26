#pragma once
#include "Common.h"

class LDF
{
private:
	unsigned long keys;
	RakNet::BitStream* storage;
public:
	LDF();
	~LDF();

	// WSTRING | 0
	void writeWString(std::wstring key, std::wstring value);

	// S32 | 1
	void writeLong(std::wstring key, long value);

	// FLOAT | 3
	void writeFloat(std::wstring key, float value);

	// U32 | 5
	void writeUnsignedLong(std::wstring key, unsigned long value);

	// BOOLEAN | 7
	void writeBoolean(std::wstring key, bool value);

	// S64 | 8
	void writeLongLong(std::wstring key, long long value);

	// S64 (OBJECTID) | 9
	void writeObjectID(std::wstring key, long long value);

	// RAW DATA (BYTES) | 13
	void writeRaw(std::wstring key, RakNet::BitStream* value);

	void writeToBitStream(RakNet::BitStream* bitStream);

	unsigned long getSize();
};