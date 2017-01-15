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

void Helpers::createSyncedItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, SystemAddress clientAddress)
{
	long long id = InventoryItems::createInventoryItem(ownerID, lot, count, isBound, isEquipped);
	InventoryItem item = InventoryItems::getInventoryItem(id);

	GameMessages::addItemToInventory(item.ownerID, item.isBound, item.lot, item.invType, item.count, item.count, item.objectID, item.slot, clientAddress);
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
