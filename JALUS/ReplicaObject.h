#pragma once
#include "Common.h"
#include "ReplicaComponent.h"
#include "ControllablePhysicsIndex.h"
#include "SimplePhysicsIndex.h"
#include "DestructibleIndex.h"
#include "StatsIndex.h"
#include "ScriptIndex.h"
#include "SkillIndex.h"
#include "RenderIndex.h"
#include "Index107.h"
#include "InventoryIndex.h"
#include "CharacterIndex.h"
#include "Locations.h"
#include "Missions.h"
#include "CollectibleIndex.h"
#include "PhantomPhysicsIndex.h"
#include "BouncerIndex.h"
#include "RebuildIndex.h"
#include "ScriptedActivityIndex.h"
#include "ModuleAssemblyIndex.h"

class ReplicaObject : public Replica
{
public:

	bool logged = false;


	long long objectID;
	long lot;
	wstring name;
	long gmLevel;
	bool broadcast = true;
	SystemAddress clientAddress = UNASSIGNED_SYSTEM_ADDRESS;
	long long spawnerID = -1;
	long mapClone = 0;
	long long parentID = -1;
	vector<long long> childIDs = vector<long long>();
	long spawnerNodeID = -1;
	float scale = -1.0;
	vector<MissionInfo> currentMissions = vector<MissionInfo>();

	int statsIndexParent = -1;
	ModuleAssemblyIndex* moduleAssemblyIndex = nullptr;
	ControllablePhysicsIndex* controllablePhysicsIndex = nullptr;
	SimplePhysicsIndex* simplePhysicsIndex = nullptr;
	PhantomPhysicsIndex* phantomPhysicsIndex = nullptr;
	DestructibleIndex* destructibleIndex = nullptr;
	StatsIndex* statsIndex = nullptr;
	CollectibleIndex* collectibleIndex = nullptr;
	CharacterIndex* characterIndex = nullptr;
	InventoryIndex* inventoryIndex = nullptr;
	ScriptIndex* scriptIndex = nullptr;
	SkillIndex* skillIndex = nullptr;
	ScriptedActivityIndex* scriptedActivityIndex = nullptr;
	RebuildIndex* rebuildIndex = nullptr;
	BouncerIndex* bouncerIndex = nullptr;
	RenderIndex* renderIndex = nullptr;
	Index107* index107 = nullptr;
	

	ReplicaObject(long long objectID, long lot, wstring name, long gmLevel, Position pos, Rotation rot, long long spawnerID = -1, long mapClone = 0);
	~ReplicaObject();

	ReplicaReturnResult SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream* outBitStream, bool* includeTimestamp);
	ReplicaReturnResult SendDestruction(RakNet::BitStream* outBitStream, SystemAddress systemAddress, bool* includeTimestamp);
	ReplicaReturnResult ReceiveDestruction(RakNet::BitStream* inBitStream, SystemAddress systemAddress, RakNetTime timestamp);
	ReplicaReturnResult SendScopeChange(bool inScope, RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress systemAddress, bool* includeTimestamp);
	ReplicaReturnResult ReceiveScopeChange(RakNet::BitStream* inBitStream, SystemAddress systemAddress, RakNetTime timestamp);
	ReplicaReturnResult Serialize(bool* sendTimestamp, RakNet::BitStream* outBitStream, RakNetTime lastSendTime, PacketPriority* priority, PacketReliability* reliability, RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags);
	ReplicaReturnResult Deserialize(RakNet::BitStream* inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress);
	int GetSortPriority(void) const { return 0; }

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};