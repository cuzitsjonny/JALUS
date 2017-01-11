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

void Helpers::pingAndPrintResult(string host, unsigned short port, SystemAddress clientAddress)
{
	bool b = PingTool::ping(host.c_str(), port, "3.25 ND1", 8);

	Session* session = Sessions::getSession(clientAddress);
	if (session != nullptr)
	{
		Chat::sendChatMessage(L"PingResult received!\n{\n   Address: " + to_wstring(host) + L",\n   Port: " + to_wstring(port) + L",\n   IsReachable: " + to_wstring(b) + L"\n}", clientAddress);
	}
}

void Helpers::pingAndRedirect(string host, unsigned short port, SystemAddress clientAddress, string redirMessage, ZoneID zoneID, bool registerRedir)
{
	bool b = PingTool::ping(host.c_str(), port, "3.25 ND1", 8);

	if (b)
	{
		Session* session = Sessions::getSession(clientAddress);
		if (session != nullptr)
		{
			if (redirMessage.length() > 0)
			{
				Chat::sendChatMessage(to_wstring(redirMessage), clientAddress);
			}

			if (zoneID != ZoneID::INVALID_ZONE_ID)
			{
				Location loc;

				loc.zoneID = zoneID;
				loc.mapClone = 0;

				Position spp = LUZCache::getByZoneID(loc.zoneID)->spawnPointPos;
				Rotation spr = LUZCache::getByZoneID(loc.zoneID)->spawnPointRot;

				loc.position.x = spp.x;
				loc.position.y = spp.y;
				loc.position.z = spp.z;
				loc.rotation.x = spr.x;
				loc.rotation.y = spr.y;
				loc.rotation.z = spr.z;
				loc.rotation.w = spr.w;

				Locations::saveLocation(loc, session->charID);
			}

			TransitionInfos::insertTransitionInfo(session->clientAddress, session->accountID, session->charID, session->transitionKey);
			General::redirectToServer(session->clientAddress, host, port, false);

			if (registerRedir)
			{
				session->gotRedirected = true;
			}
		}
	}
	else
	{
		if (redirMessage.length() > 0)
		{
			Chat::sendChatMessage(L"The instance you want to connect to is not available at the moment!\nPlease try again later.", clientAddress);
		}
	}
}
