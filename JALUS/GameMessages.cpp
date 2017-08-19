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




		case GAME_MESSAGE_ID_DROP_CLIENT_LOOT:
		{

			BitStream* packet = PacketUtils::createGMBase(session->charID, 30);
			Location loc = Locations::getLocation(session->charID);

			long lot = 10431;

			long long lootid = Objects::createObject(lot);

			packet->Write(true);

			packet->Write(true);
			packet->Write(loc.position.x);
			packet->Write(loc.position.y);
			packet->Write(loc.position.z);
			packet->Write((int)0);
			packet->Write(lot);
			packet->Write(lootid);
			packet->Write(session->charID);
			packet->Write(session->charID);
			packet->Write(true);
			//packet->Write(0);
			//packet->Write(L"NiPoint3::ZERO");
			packet->Write(loc.position.x);
			packet->Write(loc.position.y + 3);
			packet->Write(loc.position.z);



			/*BitStream* packet = PacketUtils::createGMBase(session->charID, 30);

			long lot = 10431;

			long long lootid = Objects::createObject(lot);

			packet->Write((bool)false);
			packet->Write(L"NiPoint3::ZERO");
			packet->Write((int)0);
			packet->Write(10431);
			packet->Write(lootid);
			packet->Write(session->charID);
			packet->Write(0);
			packet->Write(L"NiPoint3::ZERO");
			

			Server::sendPacket(packet, clientAddress);*/


			/*ReplicaObject* player = ObjectsManager::getObjectByID(session->charID);

			Position pos = Position();
			pos.x = player->controllablePhysicsIndex->pos_x;
			pos.y = player->controllablePhysicsIndex->pos_y;
			pos.z = player->controllablePhysicsIndex->pos_z;

			for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
			{
				SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

				GameMessages::clientDropLoot(session->charID, true, pos, 0, 0, 10431, session->charID, 0, pos, participant);
			}*/







		}


		case GAME_MESSAGE_ID_SYNC_SKILL:
		{
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


			Logger::info(std::to_string(waste1));
			Logger::info(std::to_string(waste2));
			Logger::info(std::to_string(waste3));
			Logger::info(std::to_string(itemId));


			ReplicaObject* replica = ObjectsManager::getObjectByID(itemId);

			if (replica != nullptr)
			{
				if (replica->simplePhysicsIndex != nullptr)
				{
					/*float objPos_x = replica->simplePhysicsIndex->pos_x;
					float objPos_y = replica->simplePhysicsIndex->pos_y;
					float objPos_z = replica->simplePhysicsIndex->pos_z;*/

					Position finalPosition;
					finalPosition.x = replica->simplePhysicsIndex->pos_x;
					finalPosition.y = replica->simplePhysicsIndex->pos_y;
					finalPosition.z = replica->simplePhysicsIndex->pos_z;

					Position spawnPosition;
					spawnPosition.x = replica->simplePhysicsIndex->pos_x;
					spawnPosition.y = replica->simplePhysicsIndex->pos_y;
					spawnPosition.z = replica->simplePhysicsIndex->pos_z;


					/*finalPosition.x = objPos_x;
					finalPosition.y = objPos_y;
					finalPosition.z = objPos_z;*/

					/*spawnPosition.x = objPos_x;
					spawnPosition.y = objPos_y;
					spawnPosition.z = objPos_z;*/

					//Logger::info("Position: " + std::to_string(objPos_x) + ", " + std::to_string(objPos_y) + ", " + std::to_string(objPos_z));

					long lot = 10431;

					long long lootid = Objects::createObject(lot);

					//ReplicaObject* replica = ObjectsManager::getObjectByID(objectID);


					for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
					{
						SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

						GameMessages::clientDropLoot(session->charID, 0, lot, lootid, session->charID, itemId, spawnPosition, finalPosition, participant);
					}
				}

			}

		
			//for (int i = 0; i < Server::getReplicaManager()->GetReplicaCount(); i++)
			//{

				/*ReplicaObject* replica = ObjectsManager::getObjectByID(itemId);

				float objPos_x = replica->simplePhysicsIndex->pos_x;
				float objPos_y = replica->simplePhysicsIndex->pos_y;
				float objPos_z = replica->simplePhysicsIndex->pos_z;


				//Position finalPosition;
				//Position spawnPosition;

				//SimplePhysicsIndex* index = other->simplePhysicsIndex;


				//replica->simplePhysicsIndex->pos_x




				//ReplicaObject* other = (ReplicaObject*)Server::getReplicaManager()->GetReplicaAtIndex(i);


				//SimplePhysicsIndex* index = other->simplePhysicsIndex;

				//sender.sendMessage("Position: " + std::to_string(index->pos_x) + ", " + std::to_string(index->pos_y) + ", " + std::to_string(index->pos_z));

				//Logger::info("Position: " + std::to_string(index->pos_x) + ", " + std::to_string(index->pos_y) + ", " + std::to_string(index->pos_z));
				Logger::info("Position: " + std::to_string(objPos_x) + ", " + std::to_string(objPos_y) + ", " + std::to_string(objPos_z));*/

			//}

			/*for (int i = 0; i < Server::getReplicaManager()->GetReplicaCount(); i++)
			{
				ReplicaObject* other = (ReplicaObject*)Server::getReplicaManager()->GetReplicaAtIndex(i);

				if (other->clientAddress == UNASSIGNED_SYSTEM_ADDRESS)
				{
					if (other->simplePhysicsIndex != nullptr)
					{
						SimplePhysicsIndex* index = other->simplePhysicsIndex;

						if (index->pos_x >= min_x && index->pos_x <= max_x)
						{
							sender.sendMessage("x Coords: index" + std::to_string(index->pos_x) + ", min " + std::to_string(min_x) + ", and max " + std::to_string(max_x));
							if (index->pos_y >= min_y && index->pos_y <= max_y)
							{
								sender.sendMessage("y Coords: index" + std::to_string(index->pos_y) + ", min " + std::to_string(min_y) + ", and max " + std::to_string(max_y));
								if (index->pos_z >= min_z && index->pos_z <= max_z)
								{
									sender.sendMessage("z Coords: index" + std::to_string(index->pos_z) + ", min " + std::to_string(min_z) + ", and max " + std::to_string(max_z));
									match.push_back(other);
								}
							}
						}
					}
				}
			}*/

			/*if (itemId > 1)
			{
				long lot = 10431;

				long long lootid = Objects::createObject(lot);

				ReplicaObject* replica = ObjectsManager::getObjectByID(itemId);

				ReplicaObject* other = (ReplicaObject*)Server::getReplicaManager()->GetReplicaAtIndex(i);
				
				Position finalPosition;
				Position spawnPosition;

				//SimplePhysicsIndex* index = other->simplePhysicsIndex;

				
				replica->simplePhysicsIndex->pos_x = finalPosition.x;
				replica->simplePhysicsIndex->pos_y = finalPosition.y;
				replica->simplePhysicsIndex->pos_z = finalPosition.z;

				replica->simplePhysicsIndex->pos_x = spawnPosition.x;
				replica->simplePhysicsIndex->pos_y = spawnPosition.y;
				replica->simplePhysicsIndex->pos_z = spawnPosition.z;



				/*replica->controllablePhysicsIndex->pos_x = finalPosition.x;
				replica->controllablePhysicsIndex->pos_y = finalPosition.y;
				replica->controllablePhysicsIndex->pos_z = finalPosition.z;

				replica->controllablePhysicsIndex->pos_x = spawnPosition.x;
				replica->controllablePhysicsIndex->pos_y = spawnPosition.y;
				replica->controllablePhysicsIndex->pos_z = spawnPosition.z;*/

				/*Logger::info(std::to_string(session->charID));
				Logger::info(std::to_string(lot));
				Logger::info(std::to_string(lootid));
				//Logger::info(std::to_string(itemId));

				Logger::info(std::to_string(spawnPosition.x));
				Logger::info(std::to_string(spawnPosition.y+2));
				Logger::info(std::to_string(spawnPosition.z));

				Logger::info(std::to_string(finalPosition.x));
				Logger::info(std::to_string(finalPosition.y));
				Logger::info(std::to_string(finalPosition.z));


				
				for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
				{
					SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);



					//GameMessages::clientDropLoot(session->charID, 0, lot, lootid, session->charID, itemId, spawnPosition, finalPosition, session->clientAddress);
					GameMessages::clientDropLoot(session->charID, 1, lot, lootid, session->charID, itemId, spawnPosition, finalPosition, participant);
				}
			}*/

			/*
			
			Method 1

			What I can tell, I need to read the entry for a smashable from the LUZ files to get the smashable_loot_matrix. 
			So for example, Smashable 4804 has a smashable_loot_matrix of 1:153. So 153 is the LootMatrixIndex. 
			I search the LootMatrix table in CDClient for 153, and I can get the LootTableIndex which is 85. 
			I then cycle though every entry that has a LootTableIndex of 85 and get what items it can drop.
			
			*/
			/*ControllablePhysicsIndex* smashed = (ControllablePhysicsIndex*)ObjectsManager::getObjectByID(itemId);
			if (smashed != NULL)
			{
				unsigned long smashedLot = itemId;
				std::vector<unsigned long> itemsToDrop;

				//we need to check for special cases in VE
				//check to see if they have the rocket part
				//if they dont, give it to them, if they do, pick a new one

				//classic rocket parts
				//blue rocket parts
				//yellow rocket parts
				//pink rocket parts


				if (true=true)
				{

					long lot = 10431;

					long long lootid = Objects::createObject(lot);

					//ReplicaObject* replica = ObjectsManager::getObjectByID(objectID);



					for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
					{
						SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

						GameMessages::clientDropLoot(session->charID, 0, lot, lootid, session->charID, itemId, participant);
					}



				}*/



				
				/*switch (smashedLot) {
				case 4803: {
					//this is the all nose cones case
					for (int i = 0; (rand() % 2) + 3 > i; i++)
					{
						itemsToDrop.push_back(163);
					}

					int possibleItems[4] = { 4713, 14444, 14451, 14454 };
					int temp = possibleItems[rand() % 3];
					//if (InventoryItems::getInventoryItem(session->charID)) {
						
					//if (InventoryTable::getItem(s.activeCharId, temp) < 0) {
						itemsToDrop.push_back(temp);
					//}
					break;
				}
				case 4804: {
					//common engines case
					for (int i = 0; (rand() % 2) + 3 > i; i++)
					{
						itemsToDrop.push_back(163);
					}
					int possibleItems[4] = { 4715, 14446, 14453, 14456 };
					int temp = possibleItems[rand() % 3];
					//if (InventoryTable::getItem(s.activeCharId, temp) < 0) {
						itemsToDrop.push_back(temp);
					//}
				}
				case 4805: {
					//this is the all cockpits case
					for (int i = 0; (rand() % 2) + 3 > i; i++)
					{
						itemsToDrop.push_back(163);
					}
					int possibleItems[4] = { 4714, 14445, 14452, 14455 };
					int temp = possibleItems[rand() % 3];
					//if (InventoryTable::getItem(s.activeCharId, temp) < 0) {
						itemsToDrop.push_back(temp);
					//}
					break;
				}
				case 6377: {
					//this is the all engines case
					for (int i = 0; (rand() % 2) + 3 > i; i++)
					{
						itemsToDrop.push_back(163);
					}

					int possibleItems[4] = { 4715, 14446, 14453, 14456 };
					int temp = possibleItems[rand() % 3];
					//if (InventoryTable::getItem(s.activeCharId, temp) < 0) {
						itemsToDrop.push_back(temp);
					//}
					break;
				}

				default: {
					itemsToDrop.push_back(7749);
					break;
				}
				}

				for (int i = 0; i < itemsToDrop.size(); i++)
				{
					long long lootid = Objects::createObject(itemsToDrop.at(i));

					switch (itemsToDrop.at(i)) {
					case 150: //old coin
						break;
					case 163: //1
						SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);
						//Position pos = Locations::getLocation(itemId);
						//GameMessages::dropClientLoot(session->charID, smashed->getPosition().add(COMPONENT1_POSITION((rand() % 20) - 10, 5, (rand() % 20) - 10)), 1, 0, lootid, s.activeCharId, itemId, smashed->getPosition());
						GameMessages::clientDropLoot(session->charID, 1, lootid, session->charID, itemId, e lsession->charID, smashed, participant);
						break;
					case 164: //bronze 5
						break;
					case 165: //10
						break;
					case 166: //bronze 25
						break;
					case 1635: //100
						break;
					case 1636: //silver 1
						break;
					case 1657: //1000
						break;
					case 1658: //silver 25
						break;
					case 1659: //gold 1
						break;
					case 1660: //gold 10
						break;
					case 1661: //gold 25
						break;
					default:
						GameMSG::dropClientLoot(s.activeCharId, smashed->getPosition().add(COMPONENT1_POSITION((rand() % 20) - 10, 5, (rand() % 20) - 10)), 0, itemsToDrop.at(i), lootid, 0, itemId, smashed->getPosition());
						break;
					}
				}
			}*/

		}

		case GAME_MESSAGE_PLAY_CINEMATIC:
		{
			
			//BitStream* enableJetpack = PacketUtils::createGMBase(session->charID, 561);
			BitStream* playCinematic = PacketUtils::createGMBase(session->charID, 762);
			//BitStream* playCinematic = PacketUtils::createGMBase(objectID, 0xd5); // We'll actually add the GM ID to the like this others struct later
			playCinematic->Write((bool)true); // allowGhostUpdates
			playCinematic->Write((bool)false); // bCloseMultiInteract
			playCinematic->Write((bool)true); // bSendServerNotify
			playCinematic->Write((bool)false); // bUseControlledObjectForAudioListener
			playCinematic->Write(L"RETURN"); // EndBehavior
			playCinematic->Write((bool)false); // hidePlayerDuringCine
			playCinematic->Write((float)-1.0f); // leadIn
			playCinematic->Write((bool)false); // leavePlaterLockedWhenFinished
			playCinematic->Write((bool)true); // lockPlayer
			playCinematic->Write((std::wstring)L"introcinematiccrash.bik"); // pathName
			playCinematic->Write((bool)false); // result
			playCinematic->Write((bool)false); // skipIfSamePath
			playCinematic->Write((float)0.0f); // startTimeAdvance

			Server::sendPacket(playCinematic, clientAddress);

		}


		case GAME_MESSAGE_END_CINEMATIC:
		{

			//BitStream* enableJetpack = PacketUtils::createGMBase(session->charID, 561);
			BitStream* endCinematic = PacketUtils::createGMBase(session->charID, 763);
			//BitStream* playCinematic = PacketUtils::createGMBase(objectID, 0xd5); // We'll actually add the GM ID to the like this others struct later
			endCinematic->Write((float)-1.0f); // leadOut
			endCinematic->Write((bool)false); // leavePlayerLocked
			endCinematic->Write((std::wstring)L"introcinematiccrash.bik"); // pathName


			Server::sendPacket(endCinematic, clientAddress);

		}

		case GAME_MESSAGE_CINEMATIC_UPDATE:
		{

			//BitStream* enableJetpack = PacketUtils::createGMBase(session->charID, 561);
			BitStream* cinematicUpdate = PacketUtils::createGMBase(session->charID, 763);
			//BitStream* playCinematic = PacketUtils::createGMBase(objectID, 0xd5); // We'll actually add the GM ID to the like this others struct later
			cinematicUpdate->Write(L"STARTED"); // event
			cinematicUpdate->Write((float)-1.0f); // overallTime
			cinematicUpdate->Write((std::wstring)L"introcinematiccrash.bik"); // pathName
			cinematicUpdate->Write((float)-1.0f); // pathTime
			cinematicUpdate->Write((int)-1); // waypoint


			Server::sendPacket(cinematicUpdate, clientAddress);

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
				/*if (lot == 6604)
				{
					BitStream* packet = PacketUtils::createGMBase(objectID, 0xd5); // We'll actually add the GM ID to the like this others struct later
					packet->Write(true);
					packet->Write(false);
					packet->Write(false);
					packet->Write(L"REASON_NOT_GIVEN");
					packet->Write(0.0f);
					packet->Write(session->charID);
				}*/


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
			for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
			{
				SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

				GameMessages::die(session->charID, L"electro-shock-death", false, participant);
			}
			break;
		}

		case GAME_MESSAGE_ID_REQUEST_RESURRECT:
		{
			Position spawnPos = LUZCache::getByZoneID(ServerRoles::toZoneID(Server::getServerRole()))->spawnPointPos;
			Rotation spawnRot = LUZCache::getByZoneID(ServerRoles::toZoneID(Server::getServerRole()))->spawnPointRot;

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
					if (rewardLOT != ObjectsManager::getObjectByID(receiver)->lot)
					{
						info->rewardLOT = rewardLOT;
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


void GameMessages::clientDropLoot(long long objectID, int iCurrency, long lot, long long lootid, long long owner, long long sourceObj, Position spawnPosition, Position finalPosition, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_DROP_CLIENT_LOOT);
	//ReplicaObject* replica = ObjectsManager::getObjectByID(objectID);
	//ReplicaObject* testing = ObjectsManager::getObjectByID(sourceObj);

	

	//spawnPosition.x = spawnPosition.x + ((rand() % 20) - 10);
	//spawnPosition.y = pos_y;
	//spawnPosition.z = pos_z;

	//long long lootid = Objects::createObject(lot);

	packet->Write(true);
	
	packet->Write(true);


	packet->Write(finalPosition.x + ((rand() % 20) - 10)); // finalPosition X
	packet->Write(finalPosition.y); // finalPosition Y
	packet->Write(finalPosition.z + ((rand() % 20) - 10)); // finalPosition Z

	packet->Write(iCurrency);
	packet->Write(lot);
	//packet->Write((int)0);
	packet->Write(lootid);
	packet->Write(owner);
	packet->Write(sourceObj);

	packet->Write(true);

	packet->Write(spawnPosition.x);
	packet->Write(spawnPosition.y+1);
	packet->Write(spawnPosition.z);


	//packet->Write(spawnPosition.x + ((rand() % 20) - 10));
	//packet->Write(spawnPosition.y + 5);
	//packet->Write(spawnPosition.z + ((rand() % 20) - 10));


	Server::sendPacket(packet, receiver);
}
