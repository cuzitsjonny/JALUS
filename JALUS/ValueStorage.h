#pragma once
#include "Common.h"

class ValueStoring
{
public:
	long long charid;
	string type;
	long value;
};

class ValueStorage
{
private:
	static vector<ValueStoring> valueStorage;
public:
	static void createValueInMemory(long long charid, string type, long value);
	static void updateValueInMemory(long long charid, string type, long value);
	static void removeValueFromMemory(long long charid, string type);
	static long getValueInMemory(long long charid, string type);
};