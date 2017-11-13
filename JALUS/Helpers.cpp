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
	GameMessages::playFXEffect(objectID, effectID, effectType, scale, name, priority, secondary, receiver);
	
	for (int k = 0; k < Server::getReplicaManager()->GetParticipantCount(); k++)
	{
		SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
		ReplicaObject* replica = ObjectsManager::getObjectBySystemAddress(receiver);

		if (replica != nullptr)
		{
			GameMessages::playFXEffect(replica->objectID, effectID, effectType, scale, name, priority, secondary, ObjectsManager::getObjectBySystemAddress(participant)->clientAddress);
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
