#pragma once
#include "Common.h"

#define OBJECT_ID_MIN 1000000000000000000
#define OBJECT_ID_MAX 1999999999999999999

#define SPAWNER_ID_MIN 70000000000000
#define SPAWNER_ID_MAX 79999999999999

class Objects
{
private:
	static bool validate(long long objectID);
public:
	static string name;

	static void init(string name, string structure);

	static long long generateObjectID();
	static long long generateSpawnerID();

	static bool isInSpawnerIDRange(long long objectID);

	static long long createObject(long lot);
	static void deleteObject(long long objectID);
	static long getLOT(long long objectID);
};