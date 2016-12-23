#include "GameMessages.h"
#include "Logger.h"
#include "Objects.h"
#include "Server.h"
#include "PacketUtils.h"
#include "ObjectsManager.h"
#include "CDClient.h"
#include "Missions.h"
#include "Sessions.h"

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

					vector<MissionInfo> misCur = Missions::getAllCurrentMissions(session->charID);
					if (misCur.size() > 0)
					{
						for (int i = 0; i < misCur.size(); i++)
						{
							MissionInfo info = misCur.at(i);

							MissionInfo newInfo = MissionInfo();
							newInfo.missionID = info.missionID;

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
										/*Logger::info("Submitting MissionID " + to_string(info.missionID) + " with Value " + to_string(withInfo.value) + " and Index " + to_string(k + 1));
										GameMessages::notifyMission(session->charID, info.missionID, MissionState::MISSION_STATE_ACTIVE, false, clientAddress);
										GameMessages::notifyMissionTask(session->charID, info.missionID, (k + 1), withInfo.value, clientAddress);*/
									}
								}
							}

							replica->currentMissions.push_back(newInfo);
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
			long multiInteractType;
			data->Read(multiInteractType);
			long long objectID;
			data->Read(objectID);

			long lot = ObjectsManager::getObjectByID(objectID)->lot;
			ReplicaObject* replica = ObjectsManager::getObjectByID(session->charID);

			for (int i = 0; i < replica->currentMissions.size(); i++)
			{
				MissionInfo* info = &replica->currentMissions.at(i);

				for (int k = 0; k < info->missionTasks.size(); k++)
				{
					MissionTask* task = &info->missionTasks.at(k);

					switch (task->type)
					{

					case MISSION_TASK_TYPE_INTERACT:
					{
						for (int l = 0; l < task->targets.size(); l++)
						{
							if (lot == task->targets.at(l))
							{
								task->value++;
								CurrentMissionTasks::setValue(task->uid, task->value, session->charID);
								GameMessages::notifyMissionTask(session->charID, info->missionID, k, task->value, clientAddress);

								if (task->value == task->targetValue)
								{
									GameMessages::notifyMission(session->charID, info->missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
								}
							}
						}
						break;
					}

					default:
						break;

					}
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
					GameMessages::notifyMission(session->charID, missionID, MissionState::MISSION_STATE_COMPLETE, false, clientAddress);
					Missions::setMissionDone(missionID, session->charID);
					Missions::incrementMissionDoneCount(missionID, session->charID);
					CurrentMissionTasks::removeMissionTasks(missionID, session->charID);
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
						GameMessages::notifyMission(session->charID, info.missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
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
			ReplicaObject* collectible = ObjectsManager::getObjectByID(objectID);
			ReplicaObject* replica = ObjectsManager::getObjectByID(session->charID);

			for (int i = 0; i < replica->currentMissions.size(); i++)
			{
				MissionInfo* info = &replica->currentMissions.at(i);

				for (int k = 0; k < info->missionTasks.size(); k++)
				{
					MissionTask* task = &info->missionTasks.at(k);

					switch (task->type)
					{

					case MISSION_TASK_TYPE_COLLECT_COLLECTIBLE:
					{
						for (int l = 0; l < task->targets.size(); l++)
						{
							Logger::info("Target: " + to_string(task->targets.at(l)));
							if (collectible->lot == task->targets.at(l))
							{
								task->value++;
								CurrentMissionTasks::setValue(task->uid, task->value, session->charID);

								if (collectible->lot == 4768)
								{
									long collectibleID = collectible->collectibleIndex->collectible_id + 256000;
									GameMessages::notifyMissionTask(session->charID, info->missionID, k, collectibleID, clientAddress);
								}
								else
								{
									GameMessages::notifyMissionTask(session->charID, info->missionID, k, task->value, clientAddress);
								}

								if (task->value >= task->targetValue)
								{
									GameMessages::notifyMission(session->charID, info->missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
								}
							}
						}
						break;
					}

					default:
						break;

					}
				}
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

void GameMessages::notifyMissionTask(long objectID, long missionID, long taskIndex, float updatedValue, SystemAddress receiver)
{
	BitStream* packet = PacketUtils::createGMBase(objectID, GameMessageID::GAME_MESSAGE_ID_NOTIFY_MISSION_TASK);

	packet->Write(missionID);
	packet->Write(1 << (taskIndex + 1));
	packet->Write((unsigned char)1);
	packet->Write(updatedValue);

	Server::sendPacket(packet, receiver);
}
