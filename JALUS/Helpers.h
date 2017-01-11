#pragma once
#include "Common.h"
#include "ZoneIDs.h"

class Helpers
{
public:
	static void addMissionWithTasks(long long missionID, long long charID);
	static void createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, SystemAddress clientAddress);
	static void pingAndPrintResult(string host, unsigned short port, SystemAddress clientAddress);
	static void pingAndRedirect(string host, unsigned short port, SystemAddress clientAddress, string redirMessage = "", ZoneID zoneID = ZoneID::INVALID_ZONE_ID, bool registerRedir = true);
};