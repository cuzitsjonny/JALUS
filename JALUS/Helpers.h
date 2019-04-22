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

	static void createProxyItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, SystemAddress clientAddress, bool showFlyingLoot);

	static void equip(long long charID, long long objectID, SystemAddress clientAddress, bool saved);
	static void unequip(long long charID, long long objectID, SystemAddress clientAddress);

	static void setupProxy(long long charID, long long objectID, SystemAddress clientAddress);
	static void resetProxy(long long charID, long long objectID, SystemAddress clientAddress);

	static void addSkill(long long charID, long lot, SystemAddress clientAddress);
	static void removeSkill(long long charID, long lot , SystemAddress clientAddress);

	static void broadcastEffect(long long objectID, long effectID, wstring effectType, float scale, string name, float priority, long long secondary, SystemAddress receiver, bool serialize = true);
	//static void broadcastAnimation(long long objectID, wstring animation, SystemAddress receiver);
	static void broadcastAnimation(long long objectID, string animation);
	static void sendGlobalChat(wstring message);

	static void dropCoinsOnDeath(SystemAddress clientAddress);
	static void deathCheck(long long charid, wstring deathType, SystemAddress clientAddress);
	static void syncStatValues();
	static long doMaxedStatMath(long currentStat, long additionalStat, long maxStat);
	static double randomInRange(double min, double max);

	static string getTitle(long long charID, string name);

	static void respawnObject(ReplicaObject* replica);
	static void broadcastJonnysDumbEffects();
};