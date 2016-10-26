#pragma once
#include "Common.h"
#include "ZoneIDs.h"

class Config
{
private:
	static string filePath;
public:
	static void init();

	static unsigned short getSlots();
	static unsigned short getPort();

	static string getMySQLUsername();
	static string getMySQLPassword();
	static string getMySQLDatabase();
	static string getMySQLHost();

	static string getCharactersInstanceAddress();
	static unsigned short getCharactersInstancePort();
	static string getWorldInstanceAddress(ZoneID zoneID);
	static unsigned short getWorldInstancePort(ZoneID zoneID);
	static string getDomainInstanceAddress(string domain);
	static unsigned short getDomainInstancePort(string domain);

	static string getCDClientPath();

	static bool getIgnoreDomains();
};