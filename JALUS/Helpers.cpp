#include "Helpers.h"
#include "Missions.h"
#include "CDClient.h"
#include "InventoryItems.h"
#include "GameMessages.h"

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
