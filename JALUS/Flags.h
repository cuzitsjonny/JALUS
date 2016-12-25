#pragma once
#include "Common.h"

class Flag
{
public:
	bool value = false;
	long flagID = -1;
};

class Flags
{
public:
	static string name;

	static void init(string name, string structure);

	static void setFlagValue(bool value, long flagID, long long charID);
	static bool getFlagValue(long flagID, long long charID);
	static bool isFlagStored(long flagID, long long charID);
	static void deleteFlags(long long charID);
	static vector<Flag> getFlags(long long charID);
};