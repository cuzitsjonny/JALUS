#pragma once
#include "Common.h"
#include "Locations.h"

class LUZFile
{
public:
	unsigned long zoneID = 0;
	Position spawnPointPos = Position();
	Rotation spawnPointRot = Rotation();
	vector<string> childFiles = vector<string>();
	vector<string> childScenes = vector<string>();

	LUZFile(string path);
};

class LUZCache
{
private:
	static vector<LUZFile> files;
public:
	static void init();
	static LUZFile* getByZoneID(ZoneID zoneID);
	static vector<LUZFile> getAll();
};