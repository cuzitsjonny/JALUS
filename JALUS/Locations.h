#pragma once
#include "Common.h"
#include "ZoneIDs.h"

class Position
{
public:
	float x = 0.0F;
	float y = 0.0F;
	float z = 0.0F;
};

class Rotation
{
public:
	float x = 0.0F;
	float y = 0.0F;
	float z = 0.0F;
	float w = 0.0F;
};

class Location
{
public:
	Position position;
	Rotation rotation;
	ZoneID zoneID = ZoneID::INVALID_ZONE_ID;
	long mapClone = 0;
};

class Locations
{
public:
	static string name;

	static void init(string name, string structure);

	static void saveLocation(Location loc, long long objectID);
	static void deleteLocation(long long objectID);
	static Location getLocation(long long objectID);
};