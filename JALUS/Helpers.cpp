#include "Helpers.h"
#include "Missions.h"
#include "CDClient.h"
#include "InventoryItems.h"
#include "TemporaryItems.h"
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
#include "Characters.h"
#include "Common.h"
#include "ValueStorage.h"
#include "Scheduler.h"
#include "ReplicaObject.h"
#include "Commands.h"

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

void Helpers::createProxyItemStack(long long ownerID, long lot, long count, bool isBound, bool isEquipped, SystemAddress clientAddress, bool showFlyingLoot)
{
	//long long id = InventoryItems::createInventoryItem(ownerID, lot, count, isBound, isEquipped, true);
	long long id = InventoryItems::createInventoryItem(ownerID, lot, count, isBound, isEquipped);
	InventoryItem item = InventoryItems::getInventoryItem(id);
	item.invType = InventoryType::_INVENTORY_TYPE_TEMP_ITEMS;

	GameMessages::addItemToInventory(item.ownerID, item.isBound, item.lot, item.invType, item.count, count, item.objectID, item.slot, clientAddress, showFlyingLoot);
	InventoryItems::setInventoryType(InventoryType::_INVENTORY_TYPE_TEMP_ITEMS, item.objectID);
	Helpers::equip(item.ownerID, item.objectID, clientAddress, false);
}

void Helpers::equip(long long charID, long long objectID, SystemAddress clientAddress, bool saved, bool proxy)
{
	ReplicaObject* player = ObjectsManager::getObjectByID(charID);
	vector<InventoryItem> items;
	if (proxy) {
		items = TemporaryItems::getItems(charID);
	}
	else {
		items = InventoryItems::getInventoryItems(charID);
	}
	for (int k = items.size(); k--;) {
		if (items[k].objectID == objectID) {
			if (saved)
				InventoryItems::setIsEquipped(true, objectID);
			player->inventoryIndex->items.push_back(items[k]);
			Helpers::addSkill(charID, items[k].lot, clientAddress);
			ObjectsManager::serializeObject(player);
			break;
		}
	}
}

void Helpers::unequip(long long charID, long long objectID, SystemAddress clientAddress, bool proxy)
{
	ReplicaObject* player = ObjectsManager::getObjectByID(charID);
	for (int k = player->inventoryIndex->items.size(); k--;)
	{
		if (player->inventoryIndex->items[k].objectID == objectID) {
			InventoryItems::setIsEquipped(false, player->inventoryIndex->items[k].objectID);
			player->inventoryIndex->items[k].isEquipped = false;
			player->inventoryIndex->items.erase(player->inventoryIndex->items.begin() + (k));
			Helpers::removeSkill(charID, player->inventoryIndex->items[k].lot, clientAddress);
			ObjectsManager::serializeObject(player);
			break;
		}
	}
}

void Helpers::setupProxy(long long charID, long long objectID, SystemAddress clientAddress)
{
	ReplicaObject* player = ObjectsManager::getObjectByID(charID);
	long lot = Objects::getLOT(objectID);
	if (CDClient::hasSubItems(Objects::getLOT(objectID)))
	{
		vector<long> subs = CDClient::getSubItems(Objects::getLOT(objectID));
		for (int i = 0; i < subs.size(); i++)
			Helpers::createProxyItemStack(charID, subs[i], 0, false, false, clientAddress, false);
	}
}


void Helpers::resetProxy(long long charID, long long objectID, SystemAddress clientAddress)
{
	ReplicaObject* player = ObjectsManager::getObjectByID(charID);
	if (CDClient::hasSubItems(Objects::getLOT(objectID)))
	{
		vector<long> subs = CDClient::getSubItems(Objects::getLOT(objectID));
		for (int i = subs.size(); i--;)
		{
			for (int k = player->inventoryIndex->items.size(); k--;)
			{
				if (player->inventoryIndex->items[k].objectID != objectID)
				{
					if (player->inventoryIndex->items[k].lot == subs[i])
					{
						if (player->inventoryIndex->items[k].count == 0)
						{
							Helpers::unequip(charID, player->inventoryIndex->items[k].objectID, clientAddress);
						}
					}
				}
			}
		}
	}
}

/*void Helpers::proxyGarbageCollection(long long charID)
{
	vector<InventoryItem> items = InventoryItems::getInventoryItems(charID);
	for (int k = items.size(); k--;) {
		if (items[k].isProxy == true) {
			InventoryItems::deleteInventoryItem(items[k].objectID);
		}
	}
}*/

void Helpers::addSkill(long long charID, long lot, SystemAddress clientAddress) {
	long skillid = CDClient::getSkillID(lot, 0);
	if (lot == 13276 ||
		lot == 13275 ||
		lot == 13277 ||
		lot == 13278)
		skillid = 148;
	if (skillid != -1) {
		long itemType = CDClient::getItemType(lot);
		long hotbarslot = 4;
		if (itemType == ItemType::ITEM_TYPE_HAIR || ItemType::ITEM_TYPE_HAT)
			hotbarslot = 3;
		if (itemType == ItemType::ITEM_TYPE_NECK)
			hotbarslot = 2;
		if (itemType == ItemType::ITEM_TYPE_RIGHT_HAND)
			hotbarslot = 0;
		if (itemType == ItemType::ITEM_TYPE_LEFT_HAND)
			hotbarslot = 1;
		if (itemType != ItemType::INVALID_ITEM_TYPE) {
			//Logger::info("Adding skill for LOT " + to_string(lot));
			GameMessages::addSkill(charID, skillid, hotbarslot, clientAddress);
		}
	}
}

void Helpers::removeSkill(long long charID, long lot, SystemAddress clientAddress) {
	long skillid = CDClient::getSkillID(lot, 0);
	if (lot == 13276 ||
		lot == 13275 ||
		lot == 13277 ||
		lot == 13278)
		skillid = 148;
	if (skillid != -1) {
		//Logger::info("Removing skill for LOT " + to_string(lot));
		GameMessages::removeSkill(charID, skillid, false, clientAddress);
	}
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

void Helpers::broadcastAnimation(long long objectID, string animation)
{
	for (int k = 0; k < Server::getReplicaManager()->GetParticipantCount(); k++)
	{
		SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
		ReplicaObject* replica = ObjectsManager::getObjectByID(objectID);

		if (replica != nullptr)
			GameMessages::playAnimation(objectID, to_wstring(animation), true, ObjectsManager::getObjectBySystemAddress(participant)->clientAddress);
	}
}

void Helpers::sendGlobalChat(wstring message, CommandSender sender)
{
	for (int k = 0; k < Server::getReplicaManager()->GetParticipantCount(); k++)
	{
		SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
		//ReplicaObject* replica = ObjectsManager::getObjectBySystemAddress(receiver);
		Chat::sendChatMessage(message, ObjectsManager::getObjectBySystemAddress(participant)->clientAddress);
	}
	try {
		if (sender.getSenderID() == -1)
			sender.sendMessage("Server sent chat message! (Message: '" + to_string(message) + "')");
		else
			Logger::info("Server sent chat message! (Message: '" + to_string(message) + "')");
	}
	catch (exception& e) {
		Logger::info("Server sent chat message! (Message: '" + to_string(message) + "')");
	}
	//Logger::info("Server sent chat message! (Message: '" + to_string(message) + "')");
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
	//Logger::info("Synced stats to database.");
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
		//Logger::info(to_string(newStat));
		return newStat;
	}
	else
	{
		//Logger::info(to_string(newStat));
		return maxStat;
	}
}

double Helpers::randomInRange(double min, double max)
{
	return min + (rand() / (RAND_MAX / (max - min)));
	//return min + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (max - min)));
}

string Helpers::getTitle(long long charID, string name){
	long value = ValueStorage::getValueFromDatabase(charID, "title");
	if (value == 0) {
		// DO NOTHING
	}
	else if (value == 98) {
		name += " - Administrator-Developer";
	}
	else if (value == 9) {
		name += " - Admin";
	}
	else if (value == 8) {
		name += " - Developer";
	}
	else if (value == 7) {
		name += " - Creator";
	}
	else if (value == 5) {
		name += " - Moderator";
	}
	else if (value == 3) {
		name += " - Derp";
	}
	else {
		name += " - Player";
	}

	return name;

}

void Helpers::respawnObject(ReplicaObject* replica)
{
	//Logger::info("Attempted respawn");
	ObjectsManager::spawnObject(replica);
	//Server::getReplicaManager()->ReferencePointer(replica);
	for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
	{
		SystemAddress clientAddress = Server::getReplicaManager()->GetParticipantAtIndex(i);
		GameMessages::playFXEffect(replica->objectID, 729, L"create", 1.0F, "regenerationflash", 1.0F, -1, clientAddress);
	}
	//GameMessages::stopFXEffect(replica->objectID, "regenerationflash", false, clientAddress);
}

void Helpers::broadcastPacket(BitStream* bitStream, SystemAddress address)
{
	for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
	{
		Server::sendPacket(bitStream, Server::getReplicaManager()->GetParticipantAtIndex(i));
	}
}

void Helpers::write(BitStream* bitStream, string text) {
	unsigned long s1 = text.size();
	unsigned long size = (s1 << 1) + 1;
	Logger::info(to_string(size));
	bitStream->Write(size);
	for (unsigned char k = 0; k < s1; k++) {
		bitStream->Write((unsigned char)text.at(k));
	}
}

void Helpers::write2(BitStream* bitStream, string text) {
	bitStream->Write((unsigned long)text.size());
	for (unsigned char k = 0; k < text.size(); k++) {
		bitStream->Write((unsigned char)text.at(k));
	}
}

void Helpers::broadcastJonnysDumbEffects()
{
	for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
	{
		SystemAddress clientAddress = Server::getReplicaManager()->GetParticipantAtIndex(i);
		Session* player = Sessions::getSession(clientAddress);
		if (ValueStorage::getValueInMemory(player->charID, "wisp") == 1) {
			for (int k = 0; k < Server::getReplicaManager()->GetParticipantCount(); k++)
			{
				SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
				ReplicaObject* replica = ObjectsManager::getObjectBySystemAddress(participant);

				if (ValueStorage::getValueInMemory(replica->objectID, "wisp") == 1) {
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
	}
}
