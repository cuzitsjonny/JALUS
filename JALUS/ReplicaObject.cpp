#include "ReplicaObject.h"
#include "Logger.h"
#include "CDClient.h"
#include "Objects.h"
#include "TemporaryItems.h"
#include "Server.h"
#include "Characters.h"
#include "Accounts.h"
#include "LVLCache.h"
#include "ObjectsManager.h"
#include "CharacterStats.h"
#include <fstream>

ReplicaObject::ReplicaObject(long long objectID, long lot, wstring name, long gmLevel, Position pos, Rotation rot, long long spawnerID, long mapClone)
{
	this->objectID = objectID;
	this->lot = lot;
	this->name = name;
	this->gmLevel = gmLevel;
	this->spawnerID = spawnerID;
	this->mapClone = mapClone;

	if (lot != 176)
	{
		if (!Objects::isInSpawnerIDRange(objectID))
		{
			vector<ComponentsRegistryEntry> cr = CDClient::getComponentsRegistryEntries(lot);
			for (int i = 0; i < cr.size(); i++)
			{
				ComponentsRegistryEntry e = cr.at(i);

				switch (e.componentType)
				{

				case ReplicaComponentID::REPLICA_COMPONENT_ID_CHARACTER:
				{
					characterIndex = new CharacterIndex();

					characterIndex->flag_1 = true;
					characterIndex->level = Characters::getLevel(objectID);

					CharacterStyle style = CharacterStyles::getCharacterStyle(objectID);
					characterIndex->hair_color = style.hairColor;
					characterIndex->hair_style = style.hairStyle;
					characterIndex->shirt_color = style.shirtColor;
					characterIndex->pants_color = style.pantsColor;
					characterIndex->eyebrows_style = style.eyebrows;
					characterIndex->eyes_style = style.eyes;
					characterIndex->mouth_style = style.mouth;
					characterIndex->account_id = Characters::getAccountID(objectID);
					characterIndex->lego_score = Characters::getUniverseScore(objectID);

					CharacterStats stats = CharacterStats::getCharacterStats(objectID);
					characterIndex->totalAmountOfCurrencyCollected = stats.totalAmountOfCurrencyCollected;
					characterIndex->numberOfBricksCollected = stats.numberOfBricksCollected;
					characterIndex->numberOfSmashablesSmashed = stats.numberOfSmashablesSmashed;
					characterIndex->numberOfQuickBuildsCompleted = stats.numberOfQuickBuildsCompleted;
					characterIndex->numberOfEnemiesSmashed = stats.numberOfEnemiesSmashed;
					characterIndex->numberOfRocketsUsed = stats.numberOfRocketsUsed;
					characterIndex->numberOfMissionsCompleted = stats.numberOfMissionsCompleted;
					characterIndex->numberOfPetsTamed = stats.numberOfPetsTamed;
					characterIndex->numberOfImaginationPowerupsCollected = stats.numberOfImaginationPowerupsCollected;
					characterIndex->numberOfLifePowerupsCollected = stats.numberOfLifePowerupsCollected;
					characterIndex->numberOfArmorPowerupsCollected = stats.numberOfArmorPowerupsCollected;
					characterIndex->totalDistanceTraveled = stats.totalDistanceTraveled;
					characterIndex->numberOfTimesSmashed = stats.numberOfTimesSmashed;
					characterIndex->totalDamageTaken = stats.totalDamageTaken;
					characterIndex->totalDamageHealed = stats.totalDamageHealed;
					characterIndex->totalArmorRepaired = stats.totalArmorRepaired;
					characterIndex->totalImaginationRestored = stats.totalImaginationRestored;
					characterIndex->totalImaginationUsed = stats.totalImaginationUsed;
					characterIndex->totalDistanceDriven = stats.totalDistanceDriven;
					characterIndex->totalTimeAirborneInARaceCar = stats.totalTimeAirborneInARaceCar;
					characterIndex->numberOfRacingImaginationPowerupsCollected = stats.numberOfRacingImaginationPowerupsCollected;
					characterIndex->numberOfRacingImaginationCratesSmashed = stats.numberOfRacingImaginationCratesSmashed;
					characterIndex->numberOfTimesRaceCarBoostActivated = stats.numberOfTimesRaceCarBoostActivated;
					characterIndex->numberOfWrecksInARaceCar = stats.numberOfWrecksInARaceCar;
					characterIndex->numberOfRacingSmashablesSmashed = stats.numberOfRacingSmashablesSmashed;
					characterIndex->numberOfRacesFinished = stats.numberOfRacesFinished;
					characterIndex->numberOfFirstPlaceRacesFinished = stats.numberOfFirstPlaceRacesFinished;
					break;
				}

				case ReplicaComponentID::REPLICA_COMPONENT_ID_107:
				{
					index107 = new Index107();
					break;
				}

				case ReplicaComponentID::REPLICA_COMPONENT_ID_CONTROLLABLE_PHYSICS:
				{
					controllablePhysicsIndex = new ControllablePhysicsIndex();

					controllablePhysicsIndex->flag_5 = true;
					controllablePhysicsIndex->pos_x = pos.x;
					controllablePhysicsIndex->pos_y = pos.y;
					controllablePhysicsIndex->pos_z = pos.z;
					controllablePhysicsIndex->rot_x = rot.x;
					controllablePhysicsIndex->rot_y = rot.y;
					controllablePhysicsIndex->rot_z = rot.z;
					controllablePhysicsIndex->rot_w = rot.w;
					controllablePhysicsIndex->is_on_ground = true;
					break;
				}

				case ReplicaComponentID::REPLICA_COMPONENT_ID_DESTRUCTIBLE:
				{
					//Logger::info("A destructible object was created.");
					destructibleIndex = new DestructibleIndex();

					if (statsIndexParent < 0)
					{
						statsIndex = new StatsIndex();
						statsIndexParent = ReplicaComponentID::REPLICA_COMPONENT_ID_DESTRUCTIBLE;

						StatsIndexInfo info = CDClient::getStatsIndexInfo(e.componentID);
						statsIndex->flag_1 = true;
						statsIndex->faction_id = info.factionID;
						statsIndex->cur_health = info.health;
						statsIndex->max_health = info.health;
						statsIndex->cur_armor = info.armor;
						statsIndex->max_armor = info.armor;
						statsIndex->cur_imagination = info.imagination;
						statsIndex->max_imagination = info.imagination;
						statsIndex->is_smashable = info.isSmashable;
					}
					break;
				}

				case ReplicaComponentID::REPLICA_COMPONENT_ID_COLLECTIBLE:
				{
					collectibleIndex = new CollectibleIndex();

					string collectibleID = LVLCache::getObjectProperty("collectible_id", spawnerID).value;

					if (collectibleID.length() > 0)
					{
						collectibleIndex->collectible_id = stoi(collectibleID);
					}

					if (statsIndexParent < 0)
					{
						statsIndex = new StatsIndex();
						statsIndexParent = ReplicaComponentID::REPLICA_COMPONENT_ID_COLLECTIBLE;

						StatsIndexInfo info = CDClient::getStatsIndexInfo(e.componentID);
						statsIndex->flag_1 = true;
						statsIndex->faction_id = info.factionID;
						statsIndex->cur_health = info.health;
						statsIndex->max_health = info.health;
						statsIndex->cur_armor = info.armor;
						statsIndex->max_armor = info.armor;
						statsIndex->cur_imagination = info.imagination;
						statsIndex->max_imagination = info.imagination;
						statsIndex->is_smashable = info.isSmashable;
					}
					break;
				}

				case ReplicaComponentID::REPLICA_COMPONENT_ID_INVENTORY:
				{
					inventoryIndex = new InventoryIndex();

					vector<long> lots = CDClient::getInventoryIndexInfo(lot);
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
						inventoryIndex->items.push_back(item);
					}

					vector<InventoryItem> items = InventoryItems::getEquippedInventoryItems(objectID);
					for (int k = 0; k < items.size(); k++)
					{
						inventoryIndex->items.push_back(items.at(k));
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

					simplePhysicsIndex->flag_4 = true;
					simplePhysicsIndex->pos_x = pos.x;
					simplePhysicsIndex->pos_y = pos.y;
					simplePhysicsIndex->pos_z = pos.z;
					simplePhysicsIndex->rot_x = rot.x;
					simplePhysicsIndex->rot_y = rot.y;
					simplePhysicsIndex->rot_z = rot.z;
					simplePhysicsIndex->rot_w = rot.w;
					break;
				}

				case ReplicaComponentID::REPLICA_COMPONENT_ID_PHANTOM_PHYSICS:
				{
					phantomPhysicsIndex = new PhantomPhysicsIndex();

					phantomPhysicsIndex->flag_0 = true;
					phantomPhysicsIndex->pos_x = pos.x;
					phantomPhysicsIndex->pos_y = pos.y;
					phantomPhysicsIndex->pos_z = pos.z;
					phantomPhysicsIndex->rot_x = rot.x;
					phantomPhysicsIndex->rot_y = rot.y;
					phantomPhysicsIndex->rot_z = rot.z;
					phantomPhysicsIndex->rot_w = rot.w;
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

				case ReplicaComponentID::REPLICA_COMPONENT_ID_SCRIPTED_ACTIVITY:
				{
					scriptedActivityIndex = new ScriptedActivityIndex();
					

					scriptedActivityIndex->flag = true;
					scriptedActivityIndex->count = 0;
					scriptedActivityIndex->playerObjectID = objectID;

					scriptedActivityIndex->float_0 = 0.0;
					scriptedActivityIndex->float_1 = 0.0;
					scriptedActivityIndex->float_2 = 0.0;
					scriptedActivityIndex->float_3 = 0.0;
					scriptedActivityIndex->float_4 = 0.0;
					scriptedActivityIndex->float_5 = 0.0;
					scriptedActivityIndex->float_6 = 0.0;
					scriptedActivityIndex->float_7 = 0.0;
					scriptedActivityIndex->float_8 = 0.0;
					scriptedActivityIndex->float_9 = 0.0;


					break;
				}

				case ReplicaComponentID::REPLICA_COMPONENT_ID_BOUNCER:
				{
					bouncerIndex = new BouncerIndex();

					bouncerIndex->flag_0 = true;
					bouncerIndex->flag_1 = true;				


					break;
				}

				case ReplicaComponentID::REPLICA_COMPONENT_ID_REBUILD:
				{
					rebuildIndex = new RebuildIndex();

					/*string name = "buildActivator";
					
					long long id = Objects::generateObjectID();
					ReplicaObject* other = new ReplicaObject(id, 6604, to_wstring(name), 0, pos, rot);
					ObjectsManager::spawnObject(other);

					other->parentID = id;*/


					//rebuildIndex->flagTemp = true;
					//rebuildIndex->un32Temp = 0;


					/*rebuildIndex->scriptedActivityIndex->flag = true;
					rebuildIndex->scriptedActivityIndex->count = 0;*/

					rebuildIndex->scriptedActivityIndex;


					rebuildIndex->flag_1 = true;

					rebuildIndex->rebuildState = 0;
					rebuildIndex->success = false;
					rebuildIndex->enabled = true;

					rebuildIndex->rebuildTimePassed = 0.0;
					rebuildIndex->rebuildTimePaused = 0.0;

					rebuildIndex->flag_0 = false;

					rebuildIndex->pos_x = pos.x;
					rebuildIndex->pos_y = pos.y;
					rebuildIndex->pos_z = pos.z;

					rebuildIndex->flag_0_1 = true;


					

					break;
				}

				}
			}
		}
	}
	else
	{
		string spawntemplate = LVLCache::getObjectProperty("spawntemplate", objectID).value;
		string rebuild_activators = LVLCache::getObjectProperty("rebuild_activators", objectID).value;

		if (spawntemplate.length() > 0)
		{
			long long id = Objects::generateObjectID();
			ReplicaObject* replica = new ReplicaObject(id, stol(spawntemplate), L"", 0, pos, rot, objectID);
			replica->scale = scale;
			Server::getReplicaManager()->ReferencePointer(replica);

			//ReplicaObject* activatorReplica = nullptr;

			if (rebuild_activators.length() > 0)
			{
				long long activatorID = Objects::generateObjectID();

				Rotation activatorRot = Rotation();

				activatorRot.x = 0.0;
				activatorRot.y = 0.0;
				activatorRot.z = 0.0;
				activatorRot.w = 1.0;


				//ReplicaObject* activatorReplica = new ReplicaObject(activatorID, 6604, L"", 0, pos, activatorRot, objectID);
				ReplicaObject* activatorReplica = new ReplicaObject(activatorID, 6604, L"", 0, pos, activatorRot, -1);
				//activatorReplica = new ReplicaObject(activatorID, 6604, L"", 0, pos, activatorRot, -1);
				activatorReplica->scale = scale;

				activatorReplica->parentID = id;

				replica->childIDs.push_back(activatorID);

				replica->spawnerNodeID = 0;

				Server::getReplicaManager()->ReferencePointer(activatorReplica);
			}
			//Server::getReplicaManager()->ReferencePointer(replica);
			/*Server::getReplicaManager()->ReferencePointer(replica);
			if (activatorReplica != nullptr)
			{
				Server::getReplicaManager()->ReferencePointer(activatorReplica);
			}*/

		}

		/*if (rebuild_activators.length() > 0)
		{
			long long id = Objects::generateObjectID();
			ReplicaObject* replica = new ReplicaObject(id, stol(rebuild_activators), L"", 0, pos, rot, objectID);
			replica->scale = scale;

			Server::getReplicaManager()->ReferencePointer(replica);
		}*/
	}

}

ReplicaObject::~ReplicaObject()
{
	if (controllablePhysicsIndex != nullptr)
	{
		delete controllablePhysicsIndex;
	}
	if (simplePhysicsIndex != nullptr)
	{
		delete simplePhysicsIndex;
	}
	if (phantomPhysicsIndex != nullptr)
	{
		delete phantomPhysicsIndex;
	}
	if (destructibleIndex != nullptr)
	{
		delete destructibleIndex;
	}
	if (collectibleIndex != nullptr)
	{
		delete collectibleIndex;
	}
	if (statsIndex != nullptr)
	{
		delete statsIndex;
	}
	if (characterIndex != nullptr)
	{
		delete characterIndex;
	}
	if (inventoryIndex != nullptr)
	{
		delete inventoryIndex;
		TemporaryItems::removeItems(objectID);
	}
	if (scriptIndex != nullptr)
	{
		delete scriptIndex;
	}
	if (skillIndex != nullptr)
	{
		delete skillIndex;
	}
	if (scriptedActivityIndex != nullptr)
	{
		delete scriptedActivityIndex;
	}
	if (rebuildIndex != nullptr)
	{
		delete rebuildIndex;
	}
	if (bouncerIndex != nullptr)
	{
		delete bouncerIndex;
	}
	if (renderIndex != nullptr)
	{
		delete renderIndex;
	}
	if (index107 != nullptr)
	{
		delete index107;
	}

}

ReplicaReturnResult ReplicaObject::SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream* outBitStream, bool* includeTimestamp)
{
	if (!Objects::isInSpawnerIDRange(objectID))
	{
		writeToBitStream(outBitStream, true);
	}
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

		bitStream->Write((unsigned long)0); // time since created on server
		bitStream->Write(false); // has compressed LDF?
		bitStream->Write(false); // has TriggerComponent?

		bitStream->Write(spawnerID > -1);
		if (spawnerID > -1)
		{
			bitStream->Write(spawnerID);
		}

		bitStream->Write(spawnerNodeID > -1);
		if (spawnerNodeID > -1)
		{
			bitStream->Write(spawnerNodeID);
		}

		bitStream->Write(scale > -1.0);
		if (scale > -1.0)
		{
			bitStream->Write(scale);
		}

		bitStream->Write(false); // has object world state

		bitStream->Write(gmLevel > 0);
		if (gmLevel > 0)
		{
			bitStream->Write(gmLevel);
		}
	}

	bitStream->Write(true);

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

	if (controllablePhysicsIndex != nullptr)
		controllablePhysicsIndex->writeToBitStream(bitStream, isConstruction);
	if (simplePhysicsIndex != nullptr)
		simplePhysicsIndex->writeToBitStream(bitStream, isConstruction);
	if (phantomPhysicsIndex != nullptr)
		phantomPhysicsIndex->writeToBitStream(bitStream, isConstruction);
	if (destructibleIndex != nullptr)
		destructibleIndex->writeToBitStream(bitStream, isConstruction);
	if (statsIndexParent == ReplicaComponentID::REPLICA_COMPONENT_ID_DESTRUCTIBLE
		|| statsIndexParent == ReplicaComponentID::REPLICA_COMPONENT_ID_COLLECTIBLE)
		statsIndex->writeToBitStream(bitStream, isConstruction);
	if (collectibleIndex != nullptr)
		collectibleIndex->writeToBitStream(bitStream, isConstruction);
	if (characterIndex != nullptr)
		characterIndex->writeToBitStream(bitStream, isConstruction);
	if (inventoryIndex != nullptr)
		inventoryIndex->writeToBitStream(bitStream, isConstruction);
	if (scriptIndex != nullptr)
		scriptIndex->writeToBitStream(bitStream, isConstruction);
	if (skillIndex != nullptr)
		skillIndex->writeToBitStream(bitStream, isConstruction);
	if (scriptedActivityIndex != nullptr)
		scriptedActivityIndex->writeToBitStream(bitStream, isConstruction);
	if (rebuildIndex != nullptr)
		rebuildIndex->writeToBitStream(bitStream, isConstruction);
	if (bouncerIndex != nullptr)
		bouncerIndex->writeToBitStream(bitStream, isConstruction);
	if (renderIndex != nullptr)
		renderIndex->writeToBitStream(bitStream, isConstruction);
	if (index107 != nullptr)
		index107->writeToBitStream(bitStream, isConstruction);

	

	//if (lot == 6316 && logged == false)
	/*if (logged == false)
	{
		unsigned char* raw_data = bitStream->GetData();
		char* data = (char*)&raw_data[0];
		unsigned int size = bitStream->GetNumberOfBytesUsed();

		std::ofstream outfile("log.bin", std::ofstream::binary);
		outfile.write(data, size);
		logged = true;
	}*/
	

	
	


	/*if (lot == 6316)
	{
		char* streamDump;
		bitStream->Read(streamDump, bitStream->GetNumberOfBytesUsed());

		Logger::info("DATA FOR LOT 6316: " + string(streamDump));
	}*/


}
