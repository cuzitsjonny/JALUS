#include "ReplicaObject.h"
#include "Logger.h"
#include "CDClient.h"
#include "Objects.h"
#include "TemporaryItems.h"
#include "Server.h"

ReplicaObject::ReplicaObject(long long objectID, long lot, wstring name, long gmLevel)
{
	this->objectID = objectID;
	this->lot = lot;
	this->name = name;
	this->gmLevel = gmLevel;
	this->statsIndexParent = -1;

	childIDs = vector<long long>();

	vector<ComponentsRegistryEntry> cr = CDClient::getComponentsRegistryEntries(lot);
	for (int i = 0; i < cr.size(); i++)
	{
		ComponentsRegistryEntry e = cr.at(i);

		switch (e.componentType)
		{

		case ReplicaComponentID::REPLICA_COMPONENT_ID_CHARACTER:
		{
			characterIndex = new CharacterIndex();
			break;
		}

		case ReplicaComponentID::REPLICA_COMPONENT_ID_107:
		{
			component107Index = new Component107Index();
			break;
		}

		case ReplicaComponentID::REPLICA_COMPONENT_ID_CONTROLLABLE_PHYSICS:
		{
			controllablePhysicsIndex = new ControllablePhysicsIndex();
			break;
		}

		case ReplicaComponentID::REPLICA_COMPONENT_ID_DESTRUCTIBLE:
		{
			destructibleIndex = new DestructibleIndex();

			if (statsIndexParent < 0)
			{
				statsIndex = new StatsIndex();
				statsIndexParent = ReplicaComponentID::REPLICA_COMPONENT_ID_DESTRUCTIBLE;

				StatsIndexInfo info = CDClient::getStatsIndexInfo(e.componentID);
				statsIndex->flag_1 = true;

				statsIndex->faction_id = info.factionID;
				statsIndex->current_health = info.health;
				statsIndex->max_health = info.health;
				statsIndex->current_armor = info.armor;
				statsIndex->max_armor = info.armor;
				statsIndex->current_imagination = info.imagination;
				statsIndex->max_imagination = info.imagination;
				statsIndex->is_smashable = info.isSmashable;
			}
			break;
		}

		case ReplicaComponentID::REPLICA_COMPONENT_ID_INVENTORY:
		{
			inventoryIndex = new InventoryIndex((lot == 1), objectID);

			vector<long> lots = CDClient::getInventoryComponentEntries(lot);
			for (int k = 0; k < lots.size(); k++)
			{
				long long id = Objects::generateObjectID();

				InventoryItem item = InventoryItem();
				item.objectID = id;
				item.lot = lots.at(k);
				item.count = 1;
				item.slot = k;
				item.ownerID = objectID;

				TemporaryItems::addItem(item);
			}
			break;
		}

		case ReplicaComponentID::REPLICA_COMPONENT_ID_RENDER:
		{
			renderIndex = new RenderIndex();
			break;
		}

		case ReplicaComponentID::REPLICA_COMPONENT_ID_SCRIPT:
		{
			scriptIndex = new ScriptIndex();
			break;
		}

		case ReplicaComponentID::REPLICA_COMPONENT_ID_SIMPLE_PHYSICS:
		{
			simplePhysicsIndex = new SimplePhysicsIndex();
			break;
		}

		case ReplicaComponentID::REPLICA_COMPONENT_ID_SKILL:
		{
			skillIndex = new SkillIndex();
			break;
		}

		default:
		{
			Logger::info("LOT " + to_string(lot) + " requested an unknown component with ID " + to_string(e.componentType) + "!");
			break;
		}

		}
	}
}

ReplicaObject::~ReplicaObject()
{
	if (controllablePhysicsIndex != nullptr)
		delete controllablePhysicsIndex;
	if (simplePhysicsIndex != nullptr)
		delete simplePhysicsIndex;
	if (destructibleIndex != nullptr)
		delete destructibleIndex;
	if (statsIndex != nullptr)
		delete statsIndex;
	if (characterIndex != nullptr)
		delete characterIndex;
	if (inventoryIndex != nullptr)
		delete inventoryIndex;
	if (scriptIndex != nullptr)
		delete scriptIndex;
	if (skillIndex != nullptr)
		delete skillIndex;
	if (renderIndex != nullptr)
		delete renderIndex;
	if (component107Index != nullptr)
		delete component107Index;
}

ReplicaReturnResult ReplicaObject::SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream* outBitStream, bool* includeTimestamp)
{
	outBitStream->Write((unsigned char)0x24);
	outBitStream->Write(true);
	outBitStream->Write((short)0x0a0a);
	saveToFile(outBitStream, ".\\test_rm_0.bin");
	writeToBitStream(outBitStream, true);
	saveToFile(outBitStream, ".\\test_rm_written.bin");
	//Server::getReplicaManager()->SetScope(this, true, systemAddress, false);
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult ReplicaObject::SendDestruction(RakNet::BitStream* outBitStream, SystemAddress systemAddress, bool* includeTimestamp)
{
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult ReplicaObject::ReceiveDestruction(RakNet::BitStream* inBitStream, SystemAddress systemAddress, RakNetTime timestamp)
{
	Logger::info("Client sent object destruction! (Address: " + string(systemAddress.ToString()) + ")");

	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult ReplicaObject::SendScopeChange(bool inScope, RakNet::BitStream* outBitStream, RakNetTime currentTime, SystemAddress systemAddress, bool* includeTimestamp)
{
	outBitStream->Write(inScope);
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult ReplicaObject::ReceiveScopeChange(RakNet::BitStream* inBitStream, SystemAddress systemAddress, RakNetTime timestamp)
{
	bool inScope;
	inBitStream->Read(inScope);

	Logger::info("Client sent scope change! (Address: " + string(systemAddress.ToString()) + ")");

	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult ReplicaObject::Serialize(bool* sendTimestamp, RakNet::BitStream* outBitStream, RakNetTime lastSendTime, PacketPriority* priority, PacketReliability* reliability, RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags)
{
	writeToBitStream(outBitStream, false);
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult ReplicaObject::Deserialize(RakNet::BitStream* inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress)
{
	Logger::info("Client sent object serialization! (Address: " + string(systemAddress.ToString()) + ")");

	return REPLICA_PROCESSING_DONE;
}

void ReplicaObject::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write(objectID);
		bitStream->Write(lot);

		bitStream->Write((unsigned char)name.length());
		for (int i = 0; i < name.length(); i++)
		{
			bitStream->Write(name[i]);
		}

		bitStream->Write((unsigned long)0);

		bitStream->Write(false);
		bitStream->Write(false);

		bitStream->Write(spawnerID > -1);
		if (spawnerID > -1)
			bitStream->Write(spawnerID);

		bitStream->Write(false);
		bitStream->Write(false);
		bitStream->Write(false);

		bitStream->Write(lot == 1);
		if (lot == 1)
			bitStream->Write((unsigned char)gmLevel);
	}

	bitStream->Write(parentID > -1 || childIDs.size() > 0);
	if (parentID > -1 || childIDs.size() > 0)
	{
		bitStream->Write(parentID > -1);
		if (parentID > -1)
		{
			bitStream->Write(parentID);
			bitStream->Write(false);
		}

		bitStream->Write(childIDs.size() > 0);
		if (childIDs.size() > 0)
		{
			bitStream->Write((unsigned short)childIDs.size());
			for (int i = 0; i < childIDs.size(); i++)
			{
				bitStream->Write(childIDs.at(i));
			}
		}
	}

	if (controllablePhysicsIndex != nullptr)
		controllablePhysicsIndex->writeToBitStream(bitStream, isConstruction);
	if (simplePhysicsIndex != nullptr)
		simplePhysicsIndex->writeToBitStream(bitStream, isConstruction);
	if (destructibleIndex != nullptr)
		destructibleIndex->writeToBitStream(bitStream, isConstruction);
	if (statsIndexParent = ReplicaComponentID::REPLICA_COMPONENT_ID_DESTRUCTIBLE)
		statsIndex->writeToBitStream(bitStream, isConstruction);
	if (characterIndex != nullptr)
		characterIndex->writeToBitStream(bitStream, isConstruction);
	if (inventoryIndex != nullptr)
		inventoryIndex->writeToBitStream(bitStream, isConstruction);
	if (scriptIndex != nullptr)
		scriptIndex->writeToBitStream(bitStream, isConstruction);
	if (skillIndex != nullptr)
		skillIndex->writeToBitStream(bitStream, isConstruction);
	if (renderIndex != nullptr)
		renderIndex->writeToBitStream(bitStream, isConstruction);
	if (component107Index != nullptr)
		component107Index->writeToBitStream(bitStream, isConstruction);
}
