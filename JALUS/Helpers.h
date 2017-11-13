#pragma once
#include "Common.h"
#include "ZoneIDs.h"

class Helpers
{
public:
	static void addMissionWithTasks(long long missionID, long long charID);
	static void createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, bool drop , SystemAddress clientAddress);
	static void createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, SystemAddress clientAddress);
	static void broadcastEffect(long long objectID, long effectID, wstring effectType, float scale, string name, float priority, long long secondary, SystemAddress receiver, bool serialize = true);
	static void Helpers::sendGlobalChat(wstring message);
	static void broadcastJonnysDumbEffects();
};