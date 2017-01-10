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
			Logger::info("Object is ready for updates! (Object: " + to_string(ready) + ") (Address: " + string(clientAddress.ToString()) + ")");

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

					for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
					{
						SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

						GameMessages::playFXEffect(session->charID, 2599, L"create", 1.0F, "mythranBodyGlow", 1.0F, -1, participant);
						GameMessages::playFXEffect(ObjectsManager::getObjectBySystemAddress(participant)->objectID, 2599, L"create", 1.0F, "mythranBodyGlow", 1.0F, -1, clientAddress);

						GameMessages::playFXEffect(session->charID, 1564, L"cast", 1.0F, "binocular_alert", 1.0F, -1, participant);
						GameMessages::playFXEffect(ObjectsManager::getObjectBySystemAddress(participant)->objectID, 1564, L"cast", 1.0F, "binocular_alert", 1.0F, -1, clientAddress);
					}

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
						vector<ObjectProperty> properties = LVLCache::getObjectProperties(binocs.at(i)->objectID);
						string number = "";

						for (int k = 0; k < properties.size(); k++)
						{
							ObjectProperty pro = properties.at(k);

							if (iequals(pro.key, "number"))
								number = pro.value;
						}

						if (number.length() > 0)
						{
							long flagID = ServerRoles::toZoneID(Server::getServerRole()) + stol(number);

							if (!Flags::getFlagValue(flagID, session->charID))
							{
								GameMessages::playFXEffect(binocs.at(i)->objectID, 1564, L"cast", 1.0F, "binocular_alert", 1.0F, -1, clientAddress);
							}
						}
					}

					vector<ReplicaObject*> plaques = ObjectsManager::getObjectsByLOT(8139);
					for (int i = 0; i < plaques.size(); i++)
					{
						vector<ObjectProperty> properties = LVLCache::getObjectProperties(plaques.at(i)->objectID);
						string storyText = "";

						for (int k = 0; k < properties.size(); k++)
						{
							ObjectProperty pro = properties.at(k);

							if (iequals(pro.key, "storyText"))
								storyText = pro.value;
						}

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
					vector<ObjectProperty> properties = LVLCache::getObjectProperties(objectID);
					string number = "";

					for (int k = 0; k < properties.size(); k++)
					{
						ObjectProperty pro = properties.at(k);

						if (iequals(pro.key, "number"))
							number = pro.value;
					}

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
					vector<ObjectProperty> properties = LVLCache::getObjectProperties(objectID);
					string storyText = "";

					for (int k = 0; k < properties.size(); k++)
					{
						ObjectProperty pro = properties.at(k);

						if (iequals(pro.key, "storyText"))
							storyText = pro.value;
					}

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
					info->rewardLOT = rewardLOT;
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
