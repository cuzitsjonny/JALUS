#include "Helpers.h"
#include "Missions.h"
#include "CDClient.h"
#include "InventoryItems.h"
#include "GameMessages.h"
#include "PingTool.h"
#include "Sessions.h"
#include "Chat.h"
#include "LUZCache.h"
#include "TransitionInfos.h"
#include "General.h"
#include "Server.h"
#include "ObjectsManager.h"
#include "Objects.h"
#include "Logger.h"
#include "Chat.h"
#include "Characters.h"
#include "Common.h"
#include "ValueStorage.h"
#include "Scheduler.h"
#include "ReplicaObject.h"

void Helpers::addMissionWithTasks(long long missionID, long long charID)
{
	Missions::addMission(missionID, charID);
	vector<MissionTask> tasks = CDClient::getMissionTasks(missionID);

	for (int i = 0; i < tasks.size(); i++)
	{
		MissionTask task = tasks.at(i);

		CurrentMissionTasks::addMissionTask(missionID, task.uid, charID);
	}
}

void Helpers::createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, bool drop, SystemAddress clientAddress)
{

	ItemType itemType = CDClient::getItemType(lot);
	InventoryType invType = InventoryItems::getInventoryType(itemType);

	vector<InventoryItem> getItems = InventoryItems::getInventoryItems(ownerID);

	long stackSize = CDClient::getStackSize(lot);
	//ItemType itemType = CDClient::getItemType(lot);
	//InventoryType invType = InventoryItems::getInventoryType(itemType);
	bool fail = false;
	for (int i = 0; i < getItems.size(); i++)
	{
		if (getItems.at(i).lot == lot)
		{
			//Logger::info("StackSize: " + std::to_string(stackSize));
			//Logger::info("getCount: " + std::to_string(getItems.at(i).count));
			if (getItems.at(i).count < stackSize || stackSize == 0)
			{
				//Logger::info("Add to stack");
				InventoryItem item = InventoryItems::getInventoryItem(getItems.at(i).objectID);
				InventoryItems::setCount(getItems.at(i).count+1, getItems.at(i).objectID);
				GameMessages::addItemToInventory(item.ownerID, item.isBound, lot, invType, 1, stackSize, item.objectID, getItems.at(i).slot, clientAddress);
				fail = false;
				break;
				//fucc u;
			}
			//break;
		}
		else
		{
			fail = true;
		}
	}

	if (fail == true)
	{
		//Logger::info("New stack");
		long long id = InventoryItems::createInventoryItem(ownerID, lot, count, isBound, isEquipped);
		InventoryItem item = InventoryItems::getInventoryItem(id);

		GameMessages::addItemToInventory(item.ownerID, item.isBound, item.lot, item.invType, item.count, stackSize, item.objectID, item.slot, clientAddress);
	}

}

void Helpers::createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, SystemAddress clientAddress)
{
	// old version for gmadditem command 
	long long id = InventoryItems::createInventoryItem(ownerID, lot, count, isBound, isEquipped);
	InventoryItem item = InventoryItems::getInventoryItem(id);

	//long stackSize = CDClient::getStackSize(lot);

	GameMessages::addItemToInventory(item.ownerID, item.isBound, item.lot, item.invType, item.count, count, item.objectID, item.slot, clientAddress);
	
}

void Helpers::broadcastEffect(long long objectID, long effectID, wstring effectType, float scale, string name, float priority, long long secondary, SystemAddress receiver, bool serialize)
{
	//GameMessages::playFXEffect(objectID, effectID, effectType, scale, name, priority, secondary, receiver);
	
	for (int k = 0; k < Server::getReplicaManager()->GetParticipantCount(); k++)
	{
		SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
		ReplicaObject* replica = ObjectsManager::getObjectBySystemAddress(receiver);

		if (replica != nullptr)
		{
			GameMessages::playFXEffect(replica->objectID, effectID, effectType, scale, name, priority, secondary, ObjectsManager::getObjectBySystemAddress(participant)->clientAddress, serialize);
		}
	}
}

void Helpers::sendGlobalChat(wstring message)
{
	for (int k = 0; k < Server::getReplicaManager()->GetParticipantCount(); k++)
	{
		SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
		//ReplicaObject* replica = ObjectsManager::getObjectBySystemAddress(receiver);
		Chat::sendChatMessage(message, ObjectsManager::getObjectBySystemAddress(participant)->clientAddress);
	}
	Logger::info("Server sent chat message! (Message: '" + to_string(message) + "')");
}

void Helpers::dropCoinsOnDeath(SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);
	ReplicaObject* replica = ObjectsManager::getObjectByID(session->charID);
	
	Position position;
	// pos
	position.x = replica->controllablePhysicsIndex->pos_x;
	position.y = replica->controllablePhysicsIndex->pos_y;
	position.z = replica->controllablePhysicsIndex->pos_z;
	// Updated coin values to reflect comments by Darwin.
	// 100th (1%) of the user's coins if you had more than 200 (if you had less, you lose 1 coin).
	// Max coin drop is 10000 coins.
	long long curCurrency = Characters::getCurrency(session->charID);
	if (curCurrency > 0)
	{
		long long dropCurrency = curCurrency * 0.01;
		long long newCurrency;

		if (dropCurrency >= 10000)
		{
			newCurrency = curCurrency - 10000;
			Characters::setCurrency(newCurrency, session->charID);
			GameMessages::setCurrency(session->charID, newCurrency, position, clientAddress);

			GameMessages::clientDropLoot(session->charID, 10000, 0, session->charID, session->charID, position, position, clientAddress);
		}
		else if (curCurrency >= 200)
		{
			newCurrency = curCurrency - dropCurrency;
			Characters::setCurrency(newCurrency, session->charID);
			GameMessages::setCurrency(session->charID, newCurrency, position, clientAddress);

			GameMessages::clientDropLoot(session->charID, dropCurrency, 0, session->charID, session->charID, position, position, clientAddress);
		}
		else if (curCurrency >= 100)
		{
			newCurrency = curCurrency - 1;
			Characters::setCurrency(newCurrency, session->charID);
			GameMessages::setCurrency(session->charID, newCurrency, position, clientAddress);

			GameMessages::clientDropLoot(session->charID, 1, 0, session->charID, session->charID, position, position, clientAddress);
		}
	}
}

void Helpers::deathCheck(long long charid, wstring deathType, SystemAddress clientAddress)
{
	ReplicaObject* player = ObjectsManager::getObjectByID(charid);

	if (player->statsIndex->cur_health > 0)
	{
		player->statsIndex->cur_health = 0;
		player->statsIndex->cur_armor = 0;
		player->statsIndex->cur_imagination = 0;
		Helpers::dropCoinsOnDeath(clientAddress);
		for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
		{
			SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

			GameMessages::die(charid, deathType, false, participant);
		}
	}
}

void Helpers::syncStatValues()
{
	Logger::info("Synced stats to database.");
	for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
	{
		SystemAddress clientAddress = Server::getReplicaManager()->GetParticipantAtIndex(i);
		Session* session = Sessions::getSession(clientAddress);
		ReplicaObject* player = ObjectsManager::getObjectByID(session->charID);
		/*ValueStorage::updateValueInDatabase(session->charID, "health", (long)ValueStorage::getValueInMemory(session->charID, "health"));
		ValueStorage::updateValueInDatabase(session->charID, "armor", (long)ValueStorage::getValueInMemory(session->charID, "armor"));
		ValueStorage::updateValueInDatabase(session->charID, "imagination", (long)ValueStorage::getValueInMemory(session->charID, "imagination"));*/

		ValueStorage::updateValueInDatabase(session->charID, "health", player->statsIndex->cur_health);
		ValueStorage::updateValueInDatabase(session->charID, "armor", player->statsIndex->cur_armor);
		ValueStorage::updateValueInDatabase(session->charID, "imagination", player->statsIndex->cur_imagination);

		//Logger::info("Synced stats to database.");
	}
}

long Helpers::doMaxedStatMath(long currentStat, long additionalStat, long maxStat)
{
	long newStat = currentStat + additionalStat;
	if (newStat <= maxStat)
	{
		Logger::info(to_string(newStat));
		return newStat;
	}
	else
	{
		Logger::info(to_string(newStat));
		return maxStat;
	}
}

double Helpers::randomInRange(double min, double max)
{
	return min + (rand() / (RAND_MAX / (max - min)));
}

void Helpers::respawnObject(ReplicaObject* replica, SystemAddress clientAddress)
{
	//Logger::info("Attempted respawn");
	ObjectsManager::spawnObject(replica);
	Server::getReplicaManager()->ReferencePointer(replica);
	GameMessages::playFXEffect(replica->objectID, 729, L"create", 1.0F, "regenerationflash", 1.0F, -1, clientAddress);
	//GameMessages::stopFXEffect(replica->objectID, "regenerationflash", false, clientAddress);
}

void Helpers::broadcastJonnysDumbEffects()
{
	for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
	{
		SystemAddress clientAddress = Server::getReplicaManager()->GetParticipantAtIndex(i);

		for (int k = 0; k < Server::getReplicaManager()->GetParticipantCount(); k++)
		{
			SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
			ReplicaObject* replica = ObjectsManager::getObjectBySystemAddress(participant);

			if (replica != nullptr)
			{
				GameMessages::stopFXEffect(replica->objectID, "wisp_hands", false, participant);
				GameMessages::stopFXEffect(ObjectsManager::getObjectBySystemAddress(participant)->objectID, "wisp_hands", false, clientAddress);

				GameMessages::playFXEffect(replica->objectID, 1573, L"on-anim", 1.0F, "wisp_hands", 1.0F, -1, participant);
				GameMessages::playFXEffect(ObjectsManager::getObjectBySystemAddress(participant)->objectID, 1573, L"on-anim", 1.0F, "wisp_hands", 1.0F, -1, clientAddress);

				GameMessages::stopFXEffect(replica->objectID, "wisp_hands_left", false, participant);
				GameMessages::stopFXEffect(ObjectsManager::getObjectBySystemAddress(participant)->objectID, "wisp_hands_left", false, clientAddress);

				GameMessages::playFXEffect(replica->objectID, 1579, L"on-anim", 1.0F, "wisp_hands_left", 1.0F, -1, participant);
				GameMessages::playFXEffect(ObjectsManager::getObjectBySystemAddress(participant)->objectID, 1579, L"on-anim", 1.0F, "wisp_hands_left", 1.0F, -1, clientAddress);
			}
		}
	}
}
