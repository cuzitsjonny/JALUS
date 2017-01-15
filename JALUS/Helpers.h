#pragma once
#include "Common.h"
#include "ZoneIDs.h"

class Helpers
{
public:
	static void addMissionWithTasks(long long missionID, long long charID);
	static void createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, SystemAddress clientAddress);
};