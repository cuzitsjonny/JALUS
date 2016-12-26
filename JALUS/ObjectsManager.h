#pragma once
#include "Common.h"
#include "ReplicaObject.h"

class ObjectsManager
{
public:
	static void addPlayer(ReplicaObject* player, SystemAddress clientAddress);
	static void removePlayer(SystemAddress clientAddress);
	static void spawnObject(ReplicaObject* object, SystemAddress clientAddress = UNASSIGNED_SYSTEM_ADDRESS);
	static void despawnObject(ReplicaObject* object, SystemAddress clientAddress = UNASSIGNED_SYSTEM_ADDRESS);
	static void serializeObject(ReplicaObject* object, SystemAddress clientAddress = UNASSIGNED_SYSTEM_ADDRESS);
	static ReplicaObject* getObjectByID(long long objectID);
	static ReplicaObject* getObjectBySystemAddress(SystemAddress clientAddress);
	static vector<ReplicaObject*> getObjectsByLOT(long lot);
};