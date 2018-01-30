#pragma once
#include "Common.h"
#include "ZoneIDs.h"
#include "ReplicaObject.h"

class Helpers
{
public:
	static void addMissionWithTasks(long long missionID, long long charID);
	static void createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, bool drop , SystemAddress clientAddress);
	static void createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, SystemAddress clientAddress);
	static void broadcastEffect(long long objectID, long effectID, wstring effectType, float scale, string name, float priority, long long secondary, SystemAddress receiver, bool serialize = true);
	static void sendGlobalChat(wstring message);
	static void dropCoinsOnDeath(SystemAddress clientAddress);
	static void deathCheck(long long charid, wstring deathType, SystemAddress clientAddress);
	static void syncStatValues();
	static long doMaxedStatMath(long currentStat, long additionalStat, long maxStat);
	static long respawnObject(ReplicaObject* replica, long timer);
	static void broadcastJonnysDumbEffects();
};