#include "GameMessages.h"
#include "Logger.h"
#include "Objects.h"
#include "Server.h"
#include "PacketUtils.h"
#include "ObjectsManager.h"
#include "CDClient.h"
#include "Missions.h"
#include "Sessions.h"
#include "Chat.h"
#include "LUZCache.h"
#include "Flags.h"
#include "Characters.h"
#include "LVLCache.h"
#include "Commands.h"
#include "Helpers.h"
#include "ItemDrops.h"
#include "Common.h"
#include "ValueStorage.h"
#include "Scheduler.h"

void GameMessages::processGameMessage(BitStream* data, SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		long long objectID;
		data->Read(objectID);
		unsigned short gameMessageID;
		data->Read(gameMessageID);

		switch (gameMessageID)
		{

			// racing
		case GAME_MESSAGE_ID_RACING_RESET_PLAYER_TO_LAST_RESET:
		{
			Logger::info("GAME_MESSAGE_ID_RACING_RESET_PLAYER_TO_LAST_RESET");
			break;
		}

		case GAME_MESSAGE_ID_RACING_SET_PLAYER_RESET_INFO:
		{
			Logger::info("GAME_MESSAGE_ID_RACING_SET_PLAYER_RESET_INFO");
			break;
		}

		case GAME_MESSAGE_ID_RACING_PLAYER_INFO_RESET_FINISHED:
		{
			Logger::info("GAME_MESSAGE_ID_RACING_PLAYER_INFO_RESET_FINISHED");
			break;
		}

		case GAME_MESSAGE_ID_LOCK_NODE_ROTATION:
		{
			Logger::info("GAME_MESSAGE_ID_LOCK_NODE_ROTATION");
			break;
		}

		case GAME_MESSAGE_ID_VEHICLE_SET_WHEEL_LOCK_STATE:
		{
			Logger::info("GAME_MESSAGE_ID_VEHICLE_SET_WHEEL_LOCK_STATE");
			break;
		}

		case GAME_MESSAGE_ID_NOTIFY_VEHICLE_OF_RACING_OBJECT:
		{
			Logger::info("GAME_MESSAGE_ID_NOTIFY_VEHICLE_OF_RACING_OBJECT");
			break;
		}

		// vehicle actions


		// racing loading
		case GAME_MESSAGE_ID_NOTIFY_RACING_CLIENT:
		{
			Logger::info("GAME_MESSAGE_ID_NOTIFY_RACING_CLIENT");
			break;
		}

		case GAME_MESSAGE_ID_RACING_PLAYER_LOADED:
		{
			Logger::info("GAME_MESSAGE_ID_RACING_PLAYER_LOADED");
			break;
		}

		case GAME_MESSAGE_ID_RACING_CLIENT_READY:
		{
			Logger::info("GAME_MESSAGE_ID_RACING_CLIENT_READY");
			break;
		}


		// item drops
		// item drops and inventory
		case GAME_MESSAGE_ID_PICKUP_CURRENCY:
		{

			unsigned long currency;
			Position position;

			data->Read(currency);
			data->Read(position);
			
			long long curCurrency = Characters::getCurrency(session->charID);
			long long newCurrency = curCurrency + currency;
			Characters::setCurrency(newCurrency, session->charID);
			GameMessages::setCurrency(session->charID, newCurrency, position, clientAddress);

			break;
		}

		case GAME_MESSAGE_ID_PICKUP_ITEM:
		{

			//BitStream* pickupItem = PacketUtils::createGMBase(session->charID, 139);

			long long lootObj;
			long long playerID;

			data->Read(lootObj);
			data->Read(playerID);

			//Logger::info("lootObj " + std::to_string(lootObj));
			//Logger::info("playerID " + std::to_string(playerID));


			//long getObjLOT = Objects::getLOT(lootObj);
			long getObjLOT = ItemDrops::getDroppedItem(lootObj);
			//Objects::deleteObject(lootObj);
			//ItemDrops::removeDroppedItem(lootObj);
			ReplicaObject* player = ObjectsManager::getObjectByID(session->charID);
			long isPowerup = CDClient::getIsPowerup(getObjLOT);
			if (isPowerup == 1)
			{
				//Logger::info("Todo: make this add to stats"); //health line below
				//if (getObjLOT == 177 || getObjLOT == 11915 || getObjLOT == 11916 || getObjLOT == 11917 || getObjLOT == 11920)
				long maxHealth = player->statsIndex->max_health;
				long curHealth = player->statsIndex->cur_health;

				long maxArmor = player->statsIndex->max_armor;
				long curArmor = player->statsIndex->cur_armor;

				long maxImagination = player->statsIndex->max_imagination;
				long curImagination = player->statsIndex->cur_imagination;

				Logger::info("POWERUP LOT " + to_string(getObjLOT));
				if (getObjLOT == 177) // 1 #			HEALTH
				{
					player->statsIndex->cur_health = Helpers::doMaxedStatMath(curHealth, 1, maxHealth);
				}
				else if (getObjLOT == 11915) // 2
				{
					player->statsIndex->cur_health = Helpers::doMaxedStatMath(curHealth, 2, maxHealth);
				}
				else if (getObjLOT == 11916) // 3
				{
					player->statsIndex->cur_health = Helpers::doMaxedStatMath(curHealth, 3, maxHealth);
				}
				else if (getObjLOT == 11917) // 5
				{
					player->statsIndex->cur_health = Helpers::doMaxedStatMath(curHealth, 5, maxHealth);
				}
				else if (getObjLOT == 11920) // 10
				{
					player->statsIndex->cur_health = Helpers::doMaxedStatMath(curHealth, 10, maxHealth);
				}
				else if (getObjLOT == 6431) // 1 #		ARMOR
				{
					player->statsIndex->cur_armor = Helpers::doMaxedStatMath(curArmor, 1, maxArmor);
				}
				else if (getObjLOT == 11912) // 2
				{
					player->statsIndex->cur_armor = Helpers::doMaxedStatMath(curArmor, 2, maxArmor);
				}
				else if (getObjLOT == 11913) // 3
				{
					player->statsIndex->cur_armor = Helpers::doMaxedStatMath(curArmor, 3, maxArmor);
				}
				else if (getObjLOT == 11914) // 5
				{
					player->statsIndex->cur_armor = Helpers::doMaxedStatMath(curArmor, 5, maxArmor);
				}
				else if (getObjLOT == 11919) // 10
				{
					player->statsIndex->cur_armor = Helpers::doMaxedStatMath(curArmor, 10, maxArmor);
				}
				else if (getObjLOT == 935) // 1			IMAGINATION
				{
					player->statsIndex->cur_imagination = Helpers::doMaxedStatMath(curImagination, 1, maxImagination);
				}
				else if (getObjLOT == 4035) // 2
				{
					player->statsIndex->cur_imagination = Helpers::doMaxedStatMath(curImagination, 2, maxImagination);
				}
				else if (getObjLOT == 11910) // 3
				{
					player->statsIndex->cur_imagination = Helpers::doMaxedStatMath(curImagination, 3, maxImagination);
				}
				else if (getObjLOT == 11911) // 5
				{
					player->statsIndex->cur_imagination = Helpers::doMaxedStatMath(curImagination, 5, maxImagination);
				}
				else if (getObjLOT == 11918) // 10
				{
					player->statsIndex->cur_imagination = Helpers::doMaxedStatMath(curImagination, 10, maxImagination);
				}
			}
			else 
			{
				//Logger::info("LOT: " + std::to_string(getObjLOT));
				//Logger::info("Creating synced item stack");
				Helpers::createSyncedItemStack(playerID, getObjLOT, 1, false, false, true, session->clientAddress);

				if (playerID == session->charID)
				{
					Missions::callOnMissionTaskUpdate(MissionTaskType::MISSION_TASK_TYPE_COLLECT_ITEM, session->charID, lootObj, clientAddress);

					string number = LVLCache::getObjectProperty("number", objectID).value;

					if (number.length() > 0)
					{
						long flagID = ServerRoles::toZoneID(Server::getServerRole()) + stol(number);
						Flags::setFlagValue(true, flagID, session->charID);
						Missions::callOnMissionTaskUpdate(MissionTaskType::MISSION_TASK_TYPE_FLAG_CHANGE, session->charID, flagID, clientAddress);
						GameMessages::fireEventClientSide(objectID, L"achieve", objectID, session->charID, clientAddress);
					}

				}
			}

			ItemDrops::removeDroppedItem(lootObj);
			ObjectsManager::serializeObject(player);

			break;
		}

		/*case GAME_MESSAGE_ID_PLAYER_LOADED: 
		{
			long long object;
			data->Read(object);
			
			//Logger::info("Player has finished loading into the world.");

		}*/

		case GAME_MESSAGE_REMOVE_ITEM_FROM_INVENTORY:
		{ // FROM JLUNI 2 (aka ION which is JLUNI)
			/*bool mode;
			data->Read(mode);
			unsigned long uk1;
			data->Read(uk1);
			unsigned long long uk2;
			data->Read(uk2);
			bool a1, a2, a3, a4, a5, a6, a7;
			data->Read(a1);
			data->Read(a2);
			data->Read(a3);
			data->Read(a4);
			data->Read(a5);
			data->Read(a6);
			data->Read(a7);
			long long itemid;
			data->Read(itemid);
			unsigned char uk4;
			data->Read(uk4);
			unsigned long uk5;
			data->Read(uk5);

			Logger::info("Start listing data");

			Logger::info(to_string(mode));
			Logger::info(to_string(uk1));
			Logger::info(to_string(uk2));
			Logger::info(to_string(a1));
			Logger::info(to_string(a2));
			Logger::info(to_string(a3));
			Logger::info(to_string(a4));
			Logger::info(to_string(a5));
			Logger::info(to_string(a6));
			Logger::info(to_string(a7));
			Logger::info(to_string(itemid));
			Logger::info(to_string(uk4));
			Logger::info(to_string(uk5));

			if (mode == true) {

				Logger::info("Deleted item from users inventory");
				InventoryItems::deleteInventoryItem(itemid);
				Objects::deleteObject(itemid);
			}*/
			bool confirmed;
			bool deleteItem;
			bool outSuccess;
			int invType;
			int lootTypeSource;
			wstring extraInfo;
			bool forceDeletion;
			long long lootTypeSourceID;
			long long objid;
			long lot;
			long long requestingObjectID;
			unsigned long stackCount;
			unsigned long stackRemaining;
			long long subkey;
			long long tradeID;

			data->Read(confirmed);
			data->Read(deleteItem);
			data->Read(outSuccess);
			data->Read(invType);
			data->Read(lootTypeSource);
			data->Read(extraInfo);
			data->Read(forceDeletion);
			data->Read(lootTypeSourceID);
			data->Read(objid);
			data->Read(lot);
			data->Read(requestingObjectID);
			data->Read(stackCount);
			data->Read(stackRemaining);
			data->Read(subkey);
			data->Read(tradeID);

			Logger::info(to_string(confirmed));
			Logger::info(to_string(deleteItem));
			Logger::info(to_string(outSuccess));
			Logger::info(to_string(invType));
			Logger::info(to_string(lootTypeSource));
			Logger::info(to_string(extraInfo));
			Logger::info(to_string(forceDeletion));
			Logger::info(to_string(lootTypeSourceID));
			Logger::info(to_string(objid));
			Logger::info(to_string(lot));
			Logger::info(to_string(requestingObjectID));
			Logger::info(to_string(stackCount));
			Logger::info(to_string(stackRemaining));
			Logger::info(to_string(subkey));
			Logger::info(to_string(tradeID));

			if (confirmed == true) {

				if (stackRemaining == 0) {
					Logger::info("Deleted item from users inventory");
					InventoryItems::deleteInventoryItem(objid);
					Objects::deleteObject(objid);
				}
				else if (stackRemaining > 0) {
					InventoryItems::setCount(stackRemaining, objid);
				}
			}
		}

		case GAME_MESSAGE_EQUIP_INVENTORY:
		{

			//BitStream* pickupItem = PacketUtils::createGMBase(session->charID, 139);

			bool ignoreCooldown;
			bool outSuccess;
			long long itemid;
			data->Read(ignoreCooldown);
			data->Read(outSuccess);
			data->Read(itemid);
			
			//ReplicaObject* replica = ObjectsManager::getObjectByID(itemid);
			ReplicaObject* player = ObjectsManager::getObjectByID(session->charID);
			int s = player->inventoryIndex->items.size();
			for (int i = 0; i < s; i++) {
				Logger::info("Item: " + std::to_string(player->inventoryIndex->items[i].objectID));
			}
			Logger::info("Number of items in replica player object" + to_string(player->inventoryIndex->items.size()));

			InventoryItems::setIsEquipped(true, itemid);

			vector<InventoryItem> items = InventoryItems::getEquippedInventoryItems(session->charID);
			long lot = Objects::getLOT(itemid);
			long itemType = CDClient::getItemType(lot);
			long long unEquipID = NULL;
			//Logger::info("Items equipped: " + std::to_string(items.size()));
			for (int k = 0; k < items.size(); k++)
			{
				if (itemType == items[k].itemType  && itemid != items[k].objectID) {	
					InventoryItems::setIsEquipped(false, items[k].objectID);
					unEquipID = items[k].objectID;
					Logger::info("Found matching item type to unequip, New type: " + to_string(itemType) + " Old item: " + to_string(items[k].itemType));
				}
				if (itemid == items[k].objectID) {
					player->inventoryIndex->items.push_back(items.at(k));
				}
			}
			if (unEquipID != NULL) {
				for (int i = 0; i < s; i++) {
					if (player->inventoryIndex->items[i].objectID == unEquipID) {
						player->inventoryIndex->items.erase(player->inventoryIndex->items.begin() + i);
						break;
					}
				}
			}
			s = player->inventoryIndex->items.size();
			for (int i = 0; i < s; i++) {
				Logger::info("Updated Items: " + std::to_string(player->inventoryIndex->items[i].objectID));
			}
			// add skills
			long hotbarslot = 4;
			if (itemType == ItemType::ITEM_TYPE_HAIR || ItemType::ITEM_TYPE_HAT)
				hotbarslot = 3;
			if (itemType == ItemType::ITEM_TYPE_NECK)
				hotbarslot = 2;
			if (itemType == ItemType::ITEM_TYPE_RIGHT_HAND)
				hotbarslot = 0;
			if (itemType == ItemType::ITEM_TYPE_LEFT_HAND)
				hotbarslot = 1;

			// SlitherStriker = 13276
			// Nightlasher = 13275
			// Energy Spork = 13277
			// Zapzapper = 13278

			long skillid = CDClient::getSkillID(lot, 0);
			if (lot == 13276 ||
				lot == 13275 ||
				lot == 13277 ||
				lot == 13278)
				skillid = 148;
			if (skillid != -1)
				GameMessages::addSkill(session->charID, skillid, hotbarslot, clientAddress);

			ObjectsManager::serializeObject(player);

			Logger::info("Equipped item " + std::to_string(itemid) + " for player " + std::to_string(session->charID));
			break;
		}
		
		case GAME_MESSAGE_UNEQUIP_INVENTORY:
		{
			bool evenIfDead;
			bool ignoreCooldown;
			bool outSuccess;
			long long itemToUnequip;
			long long replacementObjectID;
			data->Read(evenIfDead);
			data->Read(ignoreCooldown);
			data->Read(outSuccess);
			data->Read(itemToUnequip);
			data->Read(replacementObjectID);

			/*Logger::info("evenIfDead: " + std::to_string(evenIfDead));
			Logger::info("ignoreCooldown: " + std::to_string(ignoreCooldown));
			Logger::info("outSuccess: " + std::to_string(outSuccess));
			Logger::info("itemToUnequip: " + std::to_string(itemToUnequip));
			Logger::info("replacementObjectID: " + std::to_string(replacementObjectID));*/

			ReplicaObject* player = ObjectsManager::getObjectByID(session->charID);
			int s = player->inventoryIndex->items.size();
			long lot = Objects::getLOT(itemToUnequip);
			
			//vector<InventoryItem> items = InventoryItems::getInventoryItems(session->charID);
			//vector<InventoryItem> items = InventoryItems::getEquippedInventoryItems(session->charID);
			//Logger::info("Items equipped: " + std::to_string(items.size()));

			for (int k = 0; k < s; k++)
			{
				if (player->inventoryIndex->items[k].objectID == itemToUnequip)
				{
					if (player->inventoryIndex->items[k].isEquipped == true)
					{
						//player->inventoryIndex->items.pop_back();
						InventoryItems::setIsEquipped(false, itemToUnequip);
						player->inventoryIndex->items.erase(player->inventoryIndex->items.begin() + k);

						Logger::info("Unequipped item " + std::to_string(itemToUnequip) + " for player " + std::to_string(session->charID));

						// SlitherStriker = 13276
						// Nightlasher = 13275
						// Energy Spork = 13277
						// Zapzapper = 13278
						long skillid = CDClient::getSkillID(lot, 0);
						if (lot == 13276 ||
							lot == 13275 ||
							lot == 13277 ||
							lot == 13278)
							skillid = 148;
						if (skillid != -1)
							GameMessages::removeSkill(session->charID, skillid, false, clientAddress);
					}
				}
			}
			
			ObjectsManager::serializeObject(player);

			break;
		}

		case GAME_MESSAGE_MOVE_ITEM_IN_INVENTORY:
		{
			bool flag;
			data->Read(flag);
			long long objid;
			data->Read(objid);
			long long unknown;
			data->Read(unknown);
			unsigned long slot;
			data->Read(slot);

			// get inventory type of object being moved
			short inventoryType = InventoryItems::getInventoryTypeFromItem(objid);

			// get owner id for the object being moved
			long long owner_id = InventoryItems::getOwnerID(objid);

			// get the slot of the item that is about to be moved
			short getSlotFromItem = InventoryItems::getSlotFromItem(objid, owner_id);

			// get the item in the slot that the item we are moving is going to
			long long getItemFromSlot = InventoryItems::getItemFromSlot(owner_id, inventoryType, slot);

			if (getItemFromSlot <= OBJECT_ID_MAX && getItemFromSlot >= OBJECT_ID_MIN)
			{
				InventoryItems::setSlot(getSlotFromItem, getItemFromSlot);
				InventoryItems::setSlot(slot, objid);
				Logger::info("Swapped slots for ObjectID's [" + std::to_string(objid) + "] and [" + std::to_string(getItemFromSlot) + "] for player " + std::to_string(owner_id));
			}
			else
			{
				InventoryItems::setSlot(slot, objid);
				Logger::info("Moved ObjectID [" + std::to_string(objid) + "] to slot " + std::to_string(slot) + " for player " + std::to_string(owner_id));
			}

			break;
		}

		case GAME_MESSAGE_ID_SYNC_SKILL:
		{
			//Get couins DestructibleComponent, CurrencyIndex, and then look at currency table
			// Example:
			/*SELECT minvalue, maxvalue FROM CurrencyTable WHERE CurrencyIndex = 
			(SELECT CurrencyIndex FROM DestructibleComponent WHERE id = 
			(SELECT component_id FROM ComponentsRegistry WHERE id = 4804 AND component_type = '7'));*/

			//Logger::info("SyncSkill was called");
			bool done;
			int bitstreamSize;
			RakNet::BitStream bitstream;
			long behaviorHandle;
			long uiSkillHandle;

			data->Read(done);
			data->Read(bitstreamSize);
			for (int i = 0; i < bitstreamSize; i++) {
				unsigned char temp;
				data->Read(temp);
				bitstream.Write(temp);
			}
			data->Read(behaviorHandle);
			data->Read(uiSkillHandle);

			//response

			long waste1;
			bool waste2;
			bool waste3;
			long long itemId;
			bitstream.Read(waste1);
			bitstream.Read(waste2);
			bitstream.Read(waste3);
			bitstream.Read(itemId);


			//Logger::info("BitstreamSize: " + std::to_string(bitstreamSize));

			//Logger::info(std::to_string(waste1));
			//Logger::info(std::to_string(waste2));
			//Logger::info(std::to_string(waste3));
			//Logger::info(std::to_string(itemId));

			Logger::info("ItemID: " + std::to_string(itemId));

			

			ReplicaObject* replica = ObjectsManager::getObjectByID(itemId);

			if (replica != nullptr)
			{
				if (replica->simplePhysicsIndex != nullptr)
				{
					Position finalPosition;
					Position spawnPosition;
					if (replica->simplePhysicsIndex != nullptr)
					{
						// rand
						finalPosition.x = replica->simplePhysicsIndex->pos_x + ((rand() % 20) - 10);
						finalPosition.y = replica->simplePhysicsIndex->pos_y;
						finalPosition.z = replica->simplePhysicsIndex->pos_z + ((rand() % 20) - 10);

						spawnPosition.x = replica->simplePhysicsIndex->pos_x;
						spawnPosition.y = replica->simplePhysicsIndex->pos_y + 1;
						spawnPosition.z = replica->simplePhysicsIndex->pos_z;
					}
					/*else if (replica->controllablePhysicsIndex != nullptr)
					{

						finalPosition.x = replica->controllablePhysicsIndex->pos_x;
						finalPosition.y = replica->controllablePhysicsIndex->pos_y;
						finalPosition.z = replica->controllablePhysicsIndex->pos_z;

						spawnPosition.x = replica->controllablePhysicsIndex->pos_x;
						spawnPosition.y = replica->controllablePhysicsIndex->pos_y;
						spawnPosition.z = replica->controllablePhysicsIndex->pos_z;
					}*/

					long lootTableIndexCount = CDClient::getLootTableIndexCount(replica->lot);

					for (int k = 0; k < lootTableIndexCount; k++)
					{
						vector<long double> probabilities = CDClient::getDropProbs(replica->lot, k);

						vector<long> items = CDClient::getItemDrops(replica->lot, probabilities.at(3));
						
						long double r3 = 0 + static_cast <long double> (rand()) / (static_cast <long double> (RAND_MAX / (1 - 0)));
						
						long randMinMax = probabilities.at(1) + static_cast <long> (rand()) / (static_cast <long> (RAND_MAX / (probabilities.at(2) - probabilities.at(1))));
						
						//for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
						//{
						//	SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

							if (r3 < probabilities.at(0))
							{
								for (int k = 0; k < randMinMax; k++)
								{
									long randNum = 0 + static_cast <long> (rand()) / (static_cast <long> (RAND_MAX / (items.size() - 0)));
									
									if (items.at(randNum) != 13763) // 13763 is the lot of faction tokens. 
									{ // It will change to your specific faction (if you're in one) once dropped.
									// Blacklisting it for now since we aren't in Nimbus Station yet.
										GameMessages::clientDropLoot(session->charID, 0, items.at(randNum), session->charID, itemId, spawnPosition, finalPosition, clientAddress);
									}

								}
							}
						//}
					}


					vector<long> coinsMinMax = CDClient::getCoinDrops(replica->lot);

					long randCoin = coinsMinMax.at(0) + static_cast <long> (rand()) / (static_cast <long> (RAND_MAX / (coinsMinMax.at(1) - coinsMinMax.at(0))));

					//for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
					//{
						//SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);
												
						if (randCoin > 0) // If this isn't here and the random number is 0, it will spawn LOT 0, which is just a question mark.
						{
							GameMessages::clientDropLoot(session->charID, randCoin, 0, session->charID, itemId, spawnPosition, finalPosition, clientAddress);
						}
					//}
									
					//Comment by lcdr on how he does item drops
					/*well this seems like one of the things where having one project would avoid having to reinvent all of this for every project
						but oh well
						i've always said info should be shared openly, no matter the project
						so here goes
						this is what i do
						get yourself a random value between 0 and 1
						now, the loot matrix has entries with the loot table, the probability, min and max to drop
						so for each of these
						you check whether your random value is below the probability
						if yes:
						get yourself a random integer between min and max
						and that's how many times you choose a random entry from the loot table
						and each time add that entry to the list of your loot
						when you're done, that's your loot to drop
						a possible variation is to get the random value for each loot table instead for all
						that would make higher rolls less dramatic
						but so far it works well enough*/

					
				}


				ReplicaObject* charObj = ObjectsManager::getObjectByID(session->charID);
				if (replica != charObj)
				{
					//Logger::info("Starting respawn");
					Position pos;
					Rotation rot;
					pos.x = replica->simplePhysicsIndex->pos_x;
					pos.y = replica->simplePhysicsIndex->pos_y;
					pos.z = replica->simplePhysicsIndex->pos_z;
					// Rotation
					rot.w = replica->simplePhysicsIndex->rot_w;
					rot.x = replica->simplePhysicsIndex->rot_x;
					rot.y = replica->simplePhysicsIndex->rot_y;
					rot.z = replica->simplePhysicsIndex->rot_z;

					ReplicaObject* newReplica = new ReplicaObject(replica->objectID, replica->lot, replica->name, replica->gmLevel, pos, rot);
					newReplica->scale = replica->scale;

					//Logger::info("ObjectID: " + to_string(replica->objectID));
					string respawn = LVLCache::getObjectProperty("respawn", newReplica->objectID).value;
					Logger::info("Respawn: " + respawn);

					//string message = "Object respawned";
					//Scheduler::runAsyncTaskLater(5000, Logger::info, "Log test");

					ObjectsManager::despawnObject(replica);
					// 7 seconds
					//Scheduler::runTaskLater(stoi(respawn), Helpers::
					Scheduler::runAsyncTaskLater(7000, Helpers::respawnObject, newReplica, clientAddress);
					//Helpers::respawnObject(newReplica, 7000);

				}

			}
			/*ReplicaObject* charObj = ObjectsManager::getObjectByID(session->charID);
			if (replica != charObj)
			{

				ObjectsManager::despawnObject(replica);
				//Scheduler::runTaskTimer(7000, 7000, Helpers::syncStatValues);
				
				// 7 seconds
				Scheduler::runTaskLater(7000, [newReplica]() {
					Server::getReplicaManager()->ReferencePointer(newReplica); 
					ObjectsManager::spawnObject(newReplica);
				});
				//Server::getReplicaManager()->ReferencePointer(replica);
			}*/
			
			break;
		}

		
		case GAME_MESSAGE_ENABLE_REBUILD:
		{
			
			//BitStream* enableRebuild = PacketUtils::createGMBase(session->charID, 213);
			BitStream* packet = PacketUtils::createGMBase(session->charID, 213);

			Logger::info("EnableRebuild GM was called.");


			/*enableRebuild->Write((bool)true);
			enableRebuild->Write((bool)false);
			enableRebuild->Write((bool)false);
			enableRebuild->Write(L"REASON_NOT_GIVEN");
			enableRebuild->Write((float)0.0f);
			enableRebuild->Write();*/

			//BitStream* packet = PacketUtils::createGMBase(objectID, 0xd5); // We'll actually add the GM ID to the like this others struct later
			packet->Write(true);
			packet->Write(false);
			packet->Write(false);
			packet->Write(L"REASON_NOT_GIVEN");
			packet->Write(0.0f);
			packet->Write(session->charID);


			//Server::sendPacket(enableRebuild, clientAddress);
			Server::sendPacket(packet, clientAddress);


			break;

		}

		case GAME_MESSAGE_ID_TOOGLE_GHOST_REFERENCE_OVERRIDE:
		case GAME_MESSAGE_ID_SET_GHOST_REFERENCE_POSITION:
		{
			// Don't worry about those. They're just NetDevil bullshit. Actually those will be useful to fix achievements later.
			break;
		}

		case GAME_MESSAGE_ID_READY_FOR_UPDATES:
		{
			long long ready;
			data->Read(ready);

			long lot = Objects::getLOT(ready);
			ReplicaObject* replica = ObjectsManager::getObjectByID(session->charID);

			if (lot > -1)
			{
				if (lot == 1)
				{
					Server::sendPacket(PacketUtils::createGMBase(ready, 1642), clientAddress);
					Server::sendPacket(PacketUtils::createGMBase(ready, 509), clientAddress);

					BitStream* flagChange = PacketUtils::createGMBase(ready, 472);
					flagChange->Write((unsigned char)185);
					flagChange->Write((unsigned long)0);
					Server::sendPacket(flagChange, clientAddress);

					//BitStream* enableJetpack = PacketUtils::createGMBase(ready, 561);
					//enableJetpack->Write(true);
					//enableJetpack->Write(false);
					//enableJetpack->Write(true);
					//enableJetpack->Write(false); // effectID
					//enableJetpack->Write(false); // fAirspeed
					//enableJetpack->Write(false); // fMaxAirspeed
					//enableJetpack->Write(false); // fVertVel
					//enableJetpack->Write(false); // iWarningEffectID
					//Server::sendPacket(enableJetpack, clientAddress);

					Server::sendPacket(PacketUtils::createGMBase(session->charID, GameMessageID::GAME_MESSAGE_ID_RESTORE_TO_POST_LOAD_STATS), clientAddress);

					vector<MissionInfo> misCur = Missions::getAllCurrentMissions(session->charID);
					if (misCur.size() > 0)
					{
						for (int i = 0; i < misCur.size(); i++)
						//for (int i = misCur.size(); i > 0; i--)
						{
							MissionInfo info = misCur.at(i);

							MissionInfo newInfo = MissionInfo();
							newInfo.missionID = info.missionID;

							GameMessages::notifyMission(session->charID, info.missionID, MissionState::MISSION_STATE_ACTIVE, false, clientAddress);

							vector<MissionTask> tasksCur = CDClient::getMissionTasks(info.missionID);

							for (int k = 0; k < info.missionTasks.size(); k++)
							{
								MissionTask withValue = info.missionTasks.at(k);

								for (int l = 0; l < tasksCur.size(); l++)
								{
									MissionTask withInfo = tasksCur.at(l);

									if (withValue.uid == withInfo.uid)
									{
										withInfo.value = withValue.value;
										newInfo.missionTasks.push_back(withInfo);

										switch (withInfo.type)
										{

										case MISSION_TASK_TYPE_FLAG_CHANGE:
										{
											for (int m = 0; m < withInfo.value.size(); m++)
											{
												GameMessages::notifyMissionTask(session->charID, info.missionID, k, (m + 1), clientAddress);
											}
											break;
										}

										default:
										{
											for (int m = 0; m < withInfo.value.size(); m++)
											{
												GameMessages::notifyMissionTask(session->charID, info.missionID, k, withInfo.value.at(m), clientAddress);
											}
											break;
										}

										}

										//if (withInfo.value.size() >= withInfo.targetValue)
										//{
										//	if (!CDClient::isMission(info.missionID))
										//	/*{
										//		GameMessages::notifyMission(session->charID, info.missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
										//	}
										//	else*/
										//	{
										//		Missions::completeMission(info.missionID, session->charID, clientAddress);
										//	}
										//}
									}
								}
							}

							replica->currentMissions.push_back(newInfo);
						}
					}

					vector<Flag> flags = Flags::getFlags(session->charID);
					for (int i = 0; i < flags.size(); i++)
					{
						Flag f = flags.at(i);

						GameMessages::notifyClientFlagChange(session->charID, f.flagID, f.value, clientAddress);
					}

					vector<ReplicaObject*> binocs = ObjectsManager::getObjectsByLOT(6842);
					for (int i = 0; i < binocs.size(); i++)
					{
						string number = LVLCache::getObjectProperty("number", binocs.at(i)->objectID).value;

						if (number.length() > 0)
						{
							long flagID = ServerRoles::toZoneID(Server::getServerRole()) + stol(number);

							if (!Flags::getFlagValue(flagID, session->charID))
							{
								GameMessages::playFXEffect(binocs.at(i)->objectID, 1564, L"cast", 1.0F, "binocular_alert", 1.0F, -1, clientAddress);
							}
							else
							{
								GameMessages::fireEventClientSide(binocs.at(i)->objectID, L"achieve", binocs.at(i)->objectID, session->charID, clientAddress);
							}
						}
					}

					vector<ReplicaObject*> plaques = ObjectsManager::getObjectsByLOT(8139);
					for (int i = 0; i < plaques.size(); i++)
					{
						string storyText = LVLCache::getObjectProperty("storyText", plaques.at(i)->objectID).value;

						if (storyText.length() > 0)
						{
							vector<string> p = split(storyText, '_');
							long flagID = ServerRoles::toZoneID(Server::getServerRole()) + stol(p.at(p.size() - 1)) + 10000;

							if (!Flags::getFlagValue(flagID, session->charID))
							{
								GameMessages::playFXEffect(plaques.at(i)->objectID, 2854, L"attract", 1.0F, "plaque_attract", 1.0F, -1, clientAddress);
							}
							else
							{
								GameMessages::playFXEffect(plaques.at(i)->objectID, 2855, L"display", 1.0F, "plaquefx", 1.0F, -1, clientAddress);
								GameMessages::fireEventClientSide(plaques.at(i)->objectID, L"achieve", plaques.at(i)->objectID, session->charID, clientAddress);
							}
						}
					}
				}
			}
			break;
		}
		case GAME_MESSAGE_ID_REQUEST_USE:
		{
			bool isMultiInteractUse;
			data->Read(isMultiInteractUse);
			long multiInteractID;
			data->Read(multiInteractID);
			MultiInteractType multiInteractType;
			data->Read(multiInteractType);
			long long objectID;
			data->Read(objectID);

			Missions::callOnMissionTaskUpdate(MissionTaskType::MISSION_TASK_TYPE_INTERACT, session->charID, objectID, clientAddress);

			if (isMultiInteractUse)
			{
				switch (multiInteractType)
				{

				case MULTI_INTERACT_TYPE_MISSION:
				{
					GameMessages::offerMission(session->charID, multiInteractID, objectID, clientAddress);
					break;
				}

				default:
					break;

				}
			}
			else
			{
				ReplicaObject* replica = ObjectsManager::getObjectByID(objectID);
				if (replica != nullptr)
				{
					long lot = ObjectsManager::getObjectByID(objectID)->lot;

					if (lot == 6842)
					{
						string number = LVLCache::getObjectProperty("number", objectID).value;

						if (number.length() > 0)
						{
							long flagID = ServerRoles::toZoneID(Server::getServerRole()) + stol(number);
							Flags::setFlagValue(true, flagID, session->charID);
							Missions::callOnMissionTaskUpdate(MissionTaskType::MISSION_TASK_TYPE_FLAG_CHANGE, session->charID, flagID, clientAddress);
							GameMessages::fireEventClientSide(objectID, L"achieve", objectID, session->charID, clientAddress);
						}
					}

					if (lot == 8139)
					{
						string storyText = LVLCache::getObjectProperty("storyText", objectID).value;

						if (storyText.length() > 0)
						{
							vector<string> p = split(storyText, '_');
							long flagID = ServerRoles::toZoneID(Server::getServerRole()) + stol(p.at(p.size() - 1)) + 10000;
							Flags::setFlagValue(true, flagID, session->charID);
							Missions::callOnMissionTaskUpdate(MissionTaskType::MISSION_TASK_TYPE_FLAG_CHANGE, session->charID, flagID, clientAddress);
							GameMessages::fireEventClientSide(objectID, L"achieve", objectID, session->charID, clientAddress);
						}
					}

					vector<MissionNPCInfo> mnpc = CDClient::getMissionNPCIndexInfo(lot);

					for (int i = 0; i < mnpc.size(); i++)
					{
						MissionNPCInfo info = mnpc.at(i);

						if (info.acceptsMission)
						{
							if (Missions::isDoingMission(info.missionID, session->charID))
							{
								GameMessages::offerMission(session->charID, info.missionID, objectID, clientAddress, false);
							}
						}
					}

					for (int i = 0; i < mnpc.size(); i++)
					{
						MissionNPCInfo info = mnpc.at(i);

						if (info.offersMission)
						{
							if (!Missions::hasDoneMission(info.missionID, session->charID))
							{
								vector<long> prereq = CDClient::getPrereqMissionIDs(info.missionID);
								bool ready = true;

								for (int k = 0; k < prereq.size(); k++)
								{
									if (ready)
										ready = Missions::hasDoneMission(prereq.at(k), session->charID);
								}

								if (ready)
									GameMessages::offerMission(session->charID, info.missionID, objectID, clientAddress);
							}
						}
					}
				}
			}
			break;
		}

		case GAME_MESSAGE_ID_MISSION_DIALOGUE_OK:
		{
			bool isComplete;
			data->Read(isComplete);
			MissionState missionState;
			data->Read(missionState);
			long missionID;
			data->Read(missionID);
			long long responder;
			data->Read(responder);

			Logger::info("MissionDialogueOK received! {");
			Logger::info("   objectID: " + to_string(objectID));
			Logger::info("   isComplete: " + to_string(isComplete));
			Logger::info("   missionState: " + to_string(missionState));
			Logger::info("   missionID: " + to_string(missionID));
			Logger::info("   responder: " + to_string(responder));
			Logger::info("}");

			if (!Missions::isDoingMission(missionID, session->charID))
			{
				Missions::addMission(missionID, session->charID);
				vector<MissionTask> tasks = CDClient::getMissionTasks(missionID);

				MissionInfo info = MissionInfo();
				info.missionID = missionID;

				for (int i = 0; i < tasks.size(); i++)
				{
					MissionTask task = tasks.at(i);

					CurrentMissionTasks::addMissionTask(missionID, task.uid, session->charID);
					info.missionTasks.push_back(task);
				}

				ObjectsManager::getObjectByID(session->charID)->currentMissions.push_back(info);

				GameMessages::notifyMission(session->charID, missionID, MissionState::MISSION_STATE_ACTIVE, isComplete, clientAddress);
			}
			else
			{
				if (isComplete)
				{
					Missions::completeMission(missionID, session->charID, clientAddress);
				}
			}

			long lot = ObjectsManager::getObjectByID(objectID)->lot;
			vector<MissionNPCInfo> mnpc = CDClient::getMissionNPCIndexInfo(lot);

			for (int i = 0; i < mnpc.size(); i++)
			{
				MissionNPCInfo info = mnpc.at(i);

				if (info.acceptsMission)
				{
					if (Missions::isDoingMission(info.missionID, session->charID))
					{
						GameMessages::offerMission(session->charID, info.missionID, objectID, clientAddress);
					}
				}
			}

			for (int i = 0; i < mnpc.size(); i++)
			{
				MissionNPCInfo info = mnpc.at(i);

				if (info.offersMission)
				{
					if (!Missions::hasDoneMission(info.missionID, session->charID))
					{
						vector<long> prereq = CDClient::getPrereqMissionIDs(info.missionID);
						bool ready = true;

						for (int k = 0; k < prereq.size(); k++)
						{
							if (ready)
								ready = Missions::hasDoneMission(prereq.at(k), session->charID);
						}

						if (ready)
							GameMessages::offerMission(session->charID, info.missionID, objectID, clientAddress);
					}
				}
			}
			break;
		}

		case GAME_MESSAGE_ID_HAS_BEEN_COLLECTED:
		{
			long long playerID;
			data->Read(playerID);

			if (playerID == session->charID)
			{
				Missions::callOnMissionTaskUpdate(MissionTaskType::MISSION_TASK_TYPE_COLLECT_COLLECTIBLE, session->charID, objectID, clientAddress);
			}
			break;
		}

		case GAME_MESSAGE_ID_REQUEST_DIE:
		case GAME_MESSAGE_ID_SMASH_ME:
		{
			switch (ServerRoles::toZoneID(Server::getServerRole()))
			{

			case ZONE_ID_VENTURE_EXPLORER:
			{
				Helpers::deathCheck(session->charID, L"electro-shock-death", clientAddress);
				break;
			}

			default:
			{
				Helpers::deathCheck(session->charID, L"electro-shock-death", clientAddress);
				break;
			}

			}
			break;
		}

		case GAME_MESSAGE_ID_REQUEST_RESURRECT:
		{
			Position spawnPos = LUZCache::getByZoneID(ServerRoles::toZoneID(Server::getServerRole()))->spawnPointPos;
			Rotation spawnRot = LUZCache::getByZoneID(ServerRoles::toZoneID(Server::getServerRole()))->spawnPointRot;

			ReplicaObject* player = ObjectsManager::getObjectByID(session->charID);
			player->statsIndex->cur_health = 4;
			player->statsIndex->cur_armor = 0;
			player->statsIndex->cur_imagination = 6;

			for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
			{
				SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

				GameMessages::teleport(session->charID, false, false, true, true, spawnPos, participant, spawnRot);
				GameMessages::resurrect(session->charID, false, participant);
			}
			break;
		}

		case GAME_MESSAGE_ID_SET_FLAG:
		{
			bool value;
			data->Read(value);
			long flagID;
			data->Read(flagID);

			if (objectID == session->charID)
			{
				Flags::setFlagValue(value, flagID, session->charID);
				Missions::callOnMissionTaskUpdate(MissionTaskType::MISSION_TASK_TYPE_FLAG_CHANGE, session->charID, flagID, clientAddress);
			}
			break;
		}

		case GAME_MESSAGE_ID_NOTIFY_SERVER_LEVEL_PROCESSING_COMPLETE:
		{
			ReplicaObject* replica = ObjectsManager::getObjectByID(objectID);

			if (replica->clientAddress != UNASSIGNED_SYSTEM_ADDRESS)
			{
				long level = CDClient::lookUpLevel(replica->characterIndex->lego_score);
				replica->characterIndex->level = level;
				Characters::setLevel(level, objectID);
				
				ObjectsManager::serializeObject(replica);
				
				Helpers::broadcastEffect(replica->objectID, 7074, L"create", 1.0F, "levelup_body_glow", 1.0F, -1, clientAddress);
				Helpers::sendGlobalChat(to_wstring(Characters::getName(replica->objectID) + " has reached Level " + std::to_string(level) + "!"));

			}
			break;
		}

		case GAME_MESSAGE_ID_RESPOND_TO_MISSION:
		{
			long missionID;
			data->Read(missionID);
			long long playerID;
			data->Read(playerID);
			long long receiver;
			data->Read(receiver);
			long rewardLOT;
			data->Read(rewardLOT);

			Logger::info("RespondToMission received! {");
			Logger::info("   objectID: " + to_string(objectID));
			Logger::info("   missionID: " + to_string(missionID));
			Logger::info("   playerID: " + to_string(playerID));
			Logger::info("   receiver: " + to_string(receiver));
			Logger::info("   rewardLOT: " + to_string(rewardLOT));
			Logger::info("}");

			ReplicaObject* replica = ObjectsManager::getObjectByID(session->charID);

			for (int i = 0; i < replica->currentMissions.size(); i++)
			{
				MissionInfo* info = &replica->currentMissions.at(i);

				if (info->missionID == missionID)
				{
					if (rewardLOT > 0)
					{
						if (rewardLOT != ObjectsManager::getObjectByID(receiver)->lot)
						{
							info->rewardLOT = rewardLOT;
							//Missions::rewardMission(missionID, receiver, clientAddress);
						}
					}
				}
			}
			break;
		}

		case GAME_MESSAGE_ID_START_SKILL:
		{
			bool usedMouse = false;
			long long consumableItemID = 0;
			float casterLatency = 0.0F;
			long castType = 0;
			float lcp_x = 0.0F;
			float lcp_y = 0.0F;
			float lcp_z = 0.0F;
			long long optionalOriginatorID;
			long long optionalTargetID = 0;
			float orr_x = 0.0F;
			float orr_y = 0.0F;
			float orr_z = 0.0F;
			float orr_w = 0.0F;
			BitStream* bitStream = new BitStream();
			unsigned long skillID;
			unsigned long uiSkillHandle = 0;

			bool f;

			data->Read(usedMouse);

			data->Read(f);
			if (f)
			{
				data->Read(consumableItemID);
			}

			data->Read(f);
			if (f)
			{
				data->Read(casterLatency);
			}

			data->Read(f);
			if (f)
			{
				data->Read(castType);
			}

			data->Read(f);
			if (f)
			{
				data->Read(lcp_x);
				data->Read(lcp_y);
				data->Read(lcp_z);
			}

			data->Read(optionalOriginatorID);

			data->Read(f);
			if (f)
			{
				data->Read(optionalTargetID);
			}

			data->Read(f);
			if (f)
			{
				data->Read(orr_x);
				data->Read(orr_y);
				data->Read(orr_z);
				data->Read(orr_w);
			}

			unsigned long len;
			data->Read(len);

			for (int i = 0; i < len; i++)
			{
				char c;
				data->Read(c);
				bitStream->Write(c);
			}

			data->Read(skillID);

			data->Read(f);
			if (f)
			{
				data->Read(uiSkillHandle);
			}
			
			Logger::info("StartSkill received! {");
			Logger::info("   usedMouse: " + to_string(usedMouse));
			Logger::info("   consumableItemID: " + to_string(consumableItemID));
			Logger::info("   casterLatency: " + to_string(casterLatency));
			Logger::info("   castType: " + to_string(castType));
			Logger::info("   optionalOriginatorID: " + to_string(optionalOriginatorID));
			Logger::info("   optionalTargetID: " + to_string(optionalTargetID));
			Logger::info("   skillID: " + to_string(skillID));
			Logger::info("   bitStream Size: " + to_string(bitStream->GetNumberOfBytesUsed()));
			Logger::info("}");


			{ // code to detect behavior
				string type = CDClient::getTemplateIDName(skillID);
			}

			//GameMessages::echoStartSkill(session->charID, usedMouse, consumableItemID, casterLatency, castType, lcp_x, lcp_y, lcp_z, optionalOriginatorID, optionalTargetID, orr_x, orr_y, orr_z, orr_w, bitStream, skillID, uiSkillHandle, participant);
			BitStream* packet = PacketUtils::createGMBase(session->charID, GameMessageID::GAME_MESSAGE_ID_ECHO_START_SKILL);

			packet->Write(usedMouse);
			
			if (casterLatency == 0.0F)
			{
				packet->Write((bool)false);
			}
			else
			{
				packet->Write((bool)true);
				packet->Write(casterLatency);
			}

			if (castType == 0)
			{
				packet->Write((bool)false);
			}
			else
			{
				packet->Write((bool)true);
				packet->Write(castType);
			}

			if (lcp_x == 0.0F && lcp_y == 0.0F && lcp_z == 0.0F)
			{
				packet->Write((bool)false);
			}
			else
			{
				packet->Write((bool)true);
				packet->Write(lcp_x);
				packet->Write(lcp_y);
				packet->Write(lcp_z);
			}

			packet->Write(optionalOriginatorID);

			if (optionalTargetID == 0)
			{
				packet->Write((bool)false);
			}
			else
			{
				packet->Write((bool)true);
				packet->Write(optionalTargetID);
			}

			if (orr_x == 0.0F && orr_y == 0.0F && orr_z == 0.0F && orr_w == 0.0F)
			{
				packet->Write((bool)false);
			}
			else
			{
				packet->Write((bool)true);
				packet->Write(orr_x);
				packet->Write(orr_y);
				packet->Write(orr_z);
				packet->Write(orr_w);
			}

			packet->Write(bitStream->GetNumberOfBytesUsed()); //bitStream->GetNumberOfBytesUsed()
			//for (int i = 0; i < bitStream->GetNumberOfBytesUsed(); i++)
			//{
				packet->Write(bitStream);
			//}

			packet->Write(skillID);

			if (uiSkillHandle == 0)
			{
				packet->Write((bool)false);
			}
			else
			{
				packet->Write((bool)true);
				packet->Write(uiSkillHandle);
			}

			Server::broadcastPacket(packet, clientAddress);


			break;
		}

		case GAME_MESSAGE_ID_REQUEST_SERVER_PROJECTILE_IMPACT:
		{
			long long i64LocalID = 0;
			long long i64TargetID = 0;
			BitStream* bitStream = new BitStream();

			bool f;
			data->Read(f);
			if (f)
			{
				data->Read(i64LocalID);
			}

			if (f)
			{
				data->Read(i64TargetID);
			}

			unsigned long len;
			data->Read(len);

			for (int i = 0; i < len; i++)
			{
				char c;
				data->Read(c);
				bitStream->Write(c);
			}


			BitStream* packet = PacketUtils::createGMBase(session->charID, GameMessageID::GAME_MESSAGE_ID_DO_CLIENT_PROJECTILE_IMPACT);

			if (i64LocalID == 0)
			{
				packet->Write((bool)false);
			}
			else {
				packet->Write((bool)true);
				packet->Write(i64LocalID);
			}

			if (session->charID == 0)
			{
				packet->Write((bool)false);
			}
			else {
				packet->Write((bool)true);
				packet->Write(session->charID);
			}

			if (i64TargetID == 0)
			{
				packet->Write((bool)false);
			}
			else {
				packet->Write((bool)true);
				packet->Write(i64TargetID);
			}

			packet->Write(bitStream->GetNumberOfBytesUsed()); 
			packet->Write(bitStream);

			Server::broadcastPacket(packet, clientAddress);
			break;
		}

		case GAME_MESSAGE_MODIFY_GHOSTING_DISTANCE:
		{
			//Added reading in this message in an attempt to solve the game hanging up when loading in
			float fDistanceScalar;
			data->Read(fDistanceScalar);
			Logger::info("Modify Ghosting Distance");
			break;
		}
		case GAME_MESSAGE_FIRE_EVENT_SERVER_SIDE:
		{
			wstring str;
			int p1;
			int p2;
			int p3;
			long long senderID;

			data->Read(str);
			data->Read(p1);
			data->Read(p2);
			data->Read(p3);
			data->Read(senderID);

			Logger::info("Fire Event Server Side");
			break;
		}
		case GAME_MESSAGE_START_BUILDING_WITH_ITEM:
		{
			Logger::info("Start Buiding With Item");
			bool firstTime;
			bool success;
			int sourceBag;
			long long sourceID;
			long sourceLot;
			int sourceType;
			long long targetID;
			long targetLot;
			float x;
			float y;
			float z;
			int targetType;

			data->Read(firstTime);
			data->Read(success);
			data->Read(sourceBag);
			data->Read(sourceID);
			data->Read(sourceLot);
			data->Read(sourceType);
			data->Read(targetID);
			data->Read(targetLot);
			data->Read(x);
			data->Read(y);
			data->Read(z);
			data->Read(targetType);

			BitStream* packet = PacketUtils::createGMBase(session->charID, GameMessageID::GAME_MESSAGE_START_ARRANGING_WITH_ITEM);

			packet->Write(firstTime);
			long long buildAreaID = 0;
			packet->Write(buildAreaID);
			packet->Write(x);
			packet->Write(y);
			packet->Write(z);
			packet->Write(sourceBag);
			packet->Write(sourceID);
			packet->Write(sourceLot);
			packet->Write(sourceType);
			packet->Write(targetID);
			packet->Write(targetLot);
			packet->Write(x);
			packet->Write(y);
			packet->Write(z);
			packet->Write(targetType);

			Server::sendPacket(packet, clientAddress);
			break;
		}
		case GAME_MESSAGE_ID_PARSE_CHAT_MESSAGE:
		{
			unsigned long clientState;
			data->Read(clientState);

			wstring str = L"";
			unsigned long len;
			data->Read(len);

			for (int i = 0; i < len; i++)
			{
				wchar_t c;
				data->Read(c);
				str += c;
			}

			if (str[0] == L'/')
			{
				Logger::info("Character " + to_string(session->charID) + " used a command! (Command: '" + to_string(str) + "')");

				vector<string> rawCMD = split(to_string(str).substr(1), ' ');

				string cmd = "";
				string curArgument = "";
				vector<string> args;

				bool opened = false;
				for (int i = 0; i < rawCMD.size(); i++)
				{
					for (int k = 0; k < rawCMD.at(i).length(); k++)
					{
						char v = rawCMD.at(i)[k];

						if (i == 0)
							cmd += v;
						else
						{
							if ((k == 0 || k == rawCMD.at(i).length() - 1) && v == '"')
							{
								if (opened)
								{
									args.push_back(curArgument);
									curArgument = "";
									opened = false;
								}
								else
									opened = true;
							}
							else
								curArgument += v;
						}
					}

					if (!opened)
					{
						if (curArgument != "")
						{
							args.push_back(curArgument);
							curArgument = "";
						}
					}
					else
						curArgument += " ";
				}

				Commands::performCommand(CommandSender(session->charID, clientAddress), cmd, args);

				args.clear();
				rawCMD.clear();
			}
			break;
		}

		default:
		{
			Logger::info("Server received an unknown GameMessage! (GameMessageID: " + to_string(gameMessageID) + ") (ObjectID: " + to_string(objectID) + ") (Address: " + string(clientAddress.ToString()) + ")");
			break;
		}

		}
	}
}

void GameMessages::offerMission(long long objectID, long missionID, long long offererID, SystemAddress receiver, bool sendTwice)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_OFFER_MISSION);

	packet->Write(missionID);
	packet->Write(offererID);

	Server::sendPacket(packet, receiver);

	if (sendTwice)
	{
		BitStream* packet2 = PacketUtils::createGMBase(offererID, GameMessageID::GAME_MESSAGE_ID_OFFER_MISSION);

		packet2->Write(missionID);
		packet2->Write(offererID);

		Server::sendPacket(packet2, receiver);
	}
}

void GameMessages::notifyMission(long long objectID, long missionID, MissionState missionState, bool sendingRewards, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_NOTIFY_MISSION);

	packet->Write(missionID);
	packet->Write(missionState);
	packet->Write(sendingRewards);

	Server::sendPacket(packet, receiver);
}

void GameMessages::notifyMissionTask(long long objectID, long missionID, long taskIndex, float update, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_NOTIFY_MISSION_TASK);

	packet->Write(missionID);
	packet->Write(1 << (taskIndex + 1));
	packet->Write((unsigned char)1);
	packet->Write(update);

	Server::sendPacket(packet, receiver);
}

void GameMessages::die(long long objectID, wstring deathType, bool spawnLoot, SystemAddress receiver, long long killerID, long long lootOwnerID)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_DIE);

	//ReplicaObject* player = ObjectsManager::getObjectByID(receiver);
	//player->statsIndex->cur_health = "";

	packet->Write(true);
	packet->Write(spawnLoot);
	packet->Write(false);

	packet->Write((unsigned long)deathType.length());
	for (int i = 0; i < deathType.length(); i++)
	{
		packet->Write(deathType[i]);
	}

	packet->Write(0.0F);
	packet->Write(0.0F);
	packet->Write(0.0F);
	packet->Write(false);

	if (killerID > 1)
	{
		packet->Write(killerID);
	}
	else
	{
		packet->Write(objectID);
	}

	if (lootOwnerID > 1)
	{
		packet->Write(true);
		packet->Write(lootOwnerID); 
	}
	else
	{
		packet->Write(false);
	}

	Server::sendPacket(packet, receiver);
}

void GameMessages::resurrect(long long objectID, bool rezImmediately, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_RESURRECT);

	packet->Write(rezImmediately);

	Server::sendPacket(packet, receiver);
}

void GameMessages::teleport(long long objectID, bool noGravTeleport, bool ignoreY, bool setRotation, bool skipAllChecks, Position pos, SystemAddress receiver, Rotation rot)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_TELEPORT);
	ReplicaObject* replica = ObjectsManager::getObjectByID(objectID);

	packet->Write(ignoreY);
	packet->Write(setRotation);
	packet->Write(skipAllChecks);
	packet->Write(pos.x);
	packet->Write(pos.y);
	packet->Write(pos.z);
	packet->Write(true);
	packet->Write(noGravTeleport);

	replica->controllablePhysicsIndex->pos_x = pos.x;
	replica->controllablePhysicsIndex->pos_y = pos.y;
	replica->controllablePhysicsIndex->pos_z = pos.z;

	if (setRotation)
	{
		packet->Write(rot.w);
		packet->Write(rot.x);
		packet->Write(rot.y);
		packet->Write(rot.z);

		replica->controllablePhysicsIndex->rot_w = rot.w;
		replica->controllablePhysicsIndex->rot_x = rot.x;
		replica->controllablePhysicsIndex->rot_y = rot.y;
		replica->controllablePhysicsIndex->rot_z = rot.z;
	}

	Server::sendPacket(packet, receiver);
	ObjectsManager::serializeObject(replica);
}

void GameMessages::modifyLegoScore(long long objectID, long long score, bool updateScoreBar, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_MODIFY_LEGO_SCORE);

	packet->Write(score);

	if (updateScoreBar)
	{
		packet->Write((long)129);
		packet->Write((unsigned char)0);
	}
	else
	{
		packet->Write(true);
		packet->Write((unsigned long)0);
	}

	Server::sendPacket(packet, receiver);
}

void GameMessages::setCurrency(long long objectID, long long currency, Position pos, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_SET_CURRENCY);

	packet->Write(currency);
	packet->Write(false);
	packet->Write(pos.x);
	packet->Write(pos.y);
	packet->Write(pos.z);
	packet->Write(false);
	packet->Write(false);
	packet->Write(false);
	packet->Write(false);

	Server::sendPacket(packet, receiver);
}

void GameMessages::notifyClientFlagChange(long long objectID, long flagID, bool value, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_NOTIFY_CLIENT_FLAG_CHANGE);

	packet->Write(value);
	packet->Write(flagID);

	Server::sendPacket(packet, receiver);
}

void GameMessages::updateReputation(long long objectID, long long reputation, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_UPDATE_REPUTATION);

	packet->Write(reputation);

	Server::sendPacket(packet, receiver);
}

void GameMessages::setInventorySize(long long objectID, InventoryType type, long size, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_SET_INVENTORY_SIZE);

	packet->Write(type);
	packet->Write(size);

	Server::sendPacket(packet, receiver);
}

void GameMessages::playFXEffect(long long objectID, long effectID, wstring effectType, float scale, string name, float priority, long long secondary, SystemAddress receiver, bool serialize)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_PLAY_FX_EFFECT);

	packet->Write(true);
	packet->Write(effectID);
	
	packet->Write((unsigned long)effectType.length());
	for (int i = 0; i < effectType.length(); i++)
	{
		packet->Write(effectType[i]);
	}

	packet->Write(scale != 1.0F);
	if (scale != 1.0F)
	{
		packet->Write(scale);
	}

	packet->Write((unsigned long)name.length());
	for (int i = 0; i < name.length(); i++)
	{
		packet->Write(name[i]);
	}

	packet->Write(priority != 1.0F);
	if (priority != 1.0F)
	{
		packet->Write(priority);
	}

	packet->Write(secondary > -1);
	if (secondary > -1)
	{
		packet->Write(secondary);
	}

	packet->Write(serialize);

	Server::sendPacket(packet, receiver);
}

void GameMessages::stopFXEffect(long long objectID, string name, bool killImmediately, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_STOP_FX_EFFECT);

	packet->Write(killImmediately);

	packet->Write((unsigned long)name.length());
	for (int i = 0; i < name.length(); i++)
	{
		packet->Write(name[i]);
	}

	Server::sendPacket(packet, receiver);
}

void GameMessages::fireEventClientSide(long long objectID, wstring args, long long object, long long senderID, SystemAddress receiver, long long param1, long param2)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_FIRE_EVENT_CLIENT_SIDE);

	packet->Write((unsigned long)args.length());
	for (int i = 0; i < args.length(); i++)
	{
		packet->Write(args[i]);
	}

	packet->Write(object);
	
	packet->Write(param1 != 0);
	if (param1 != 0)
	{
		packet->Write(param1);
	}

	packet->Write(param2 > -1);
	if (param2 > -1)
	{
		packet->Write(param2);
	}

	packet->Write(senderID);

	Server::sendPacket(packet, receiver);
}

void GameMessages::addItemToInventory(long long objectID, bool isBound, long lot, InventoryType invType, long count, long totalCount, long long stackObjectID, short slot, SystemAddress receiver, bool showFlyingLoot)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_ADD_ITEM_TO_INVENTORY_CLIENT_SYNC);

	packet->Write(isBound);
	packet->Write(false);
	packet->Write(false);
	packet->Write(false);
	packet->Write((unsigned long)0);
	packet->Write(lot);
	packet->Write(false);

	packet->Write(true);
	packet->Write(invType);

	packet->Write(true);
	packet->Write(count);

	packet->Write(true);
	packet->Write(totalCount);

	packet->Write(stackObjectID);
	packet->Write(0.0F);
	packet->Write(0.0F);
	packet->Write(0.0F);
	packet->Write(showFlyingLoot);
	packet->Write((unsigned long)slot);

	Server::sendPacket(packet, receiver);
}


void GameMessages::clientDropLoot(long long objectID, int iCurrency, long lot, long long owner, long long sourceObj, Position spawnPosition, Position finalPosition, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_DROP_CLIENT_LOOT);
	long long lootid; 
	if (lot > 0) // if the LOT is 0, it is either a coin or powerup
	{
		lootid = ItemDrops::createDroppedItem(lot, false);
	}
	else {
		lootid = ItemDrops::createDroppedItem(lot, true);
	}

	packet->Write(true);
	packet->Write(true);

	packet->Write(finalPosition.x);
	packet->Write(finalPosition.y);
	packet->Write(finalPosition.z);

	packet->Write(iCurrency);
	packet->Write(lot);
	packet->Write(lootid);
	packet->Write(owner);
	packet->Write(sourceObj);

	packet->Write(true);

	packet->Write(spawnPosition.x);
	packet->Write(spawnPosition.y);
	packet->Write(spawnPosition.z);

	Server::sendPacket(packet, receiver);
}

//void GameMessages::addSkill(int AICombatWeight, bool bFromSkillSet, int castType, long long objectID, unsigned long skillid, unsigned long slot, SystemAddress receiver)
void GameMessages::addSkill(long long objectID, long skillid, long slot, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_ADD_SKILL);

	for (int i = 0; i < 5; i++) {
		packet->Write((bool)false);
	}

	//code from Pluto for now

	packet->Write(skillid);
	packet->Write((bool)true);
	packet->Write(slot);
	packet->Write((bool)true);



	Server::sendPacket(packet, receiver);
}

void GameMessages::removeSkill(long long objectID, long skillid, bool fromSkillSet, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_REMOVE_SKILL);

	packet->Write(fromSkillSet);
	packet->Write(skillid);

	Server::sendPacket(packet, receiver);
}



