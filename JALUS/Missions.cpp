#include "Missions.h"
#include "Config.h"
#include "Logger.h"
#include "CurrentMissionTasks.h"
#include "ReplicaObject.h"
#include "ObjectsManager.h"
#include "GameMessages.h"
#include "Characters.h"
#include "Server.h"
#include "PacketUtils.h"
#include "Flags.h"
#include "LVLCache.h"
#include "Helpers.h"

string Missions::name;

void Missions::init(string name, string structure)
{
	Missions::name = name;

	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "CREATE TABLE IF NOT EXISTS";
		ss << " " << name << " ";
		ss << "(";
		ss << structure;
		ss << ");";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		Logger::error("Failed to init SQLTable! (Name: " + name + ") Error: '" + string(x.ErrMessage()) + "'");
		exit(-1);
	}
}

void Missions::addMission(long missionID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "INSERT INTO";
		ss << " " << Missions::name << " ";
		ss << "(id, mission_id, character_id, is_done, done_timestamp, done_count)";
		ss << " VALUES ";
		ss << "('NULL', '" << missionID << "', '" << charID << "', '" << 0 << "', '" << -1 << "', '" << 0 << "');";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void Missions::deleteMissions(long long charID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "DELETE FROM";
		ss << " " << Missions::name << " ";
		ss << "WHERE character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void Missions::setMissionDone(long long missionID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

		stringstream ss;
		ss << "UPDATE";
		ss << " " << Missions::name << " ";
		ss << "SET is_done = '" << 1 << "', done_timestamp = '" << ms.count() << "' ";
		ss << "WHERE mission_id = '" << missionID << "' AND character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void Missions::incrementMissionDoneCount(long long missionID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

		stringstream ss;
		ss << "UPDATE";
		ss << " " << Missions::name << " ";
		ss << "SET done_count = done_count + 1 ";
		ss << "WHERE mission_id = '" << missionID << "' AND character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

bool Missions::hasDoneMission(long missionID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	bool r = false;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT is_done FROM";
		ss << " " << Missions::name << " ";
		ss << "WHERE mission_id = '" << missionID << "' AND character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("is_done").asBool();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

bool Missions::isDoingMission(long missionID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	bool r = false;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT is_done FROM";
		ss << " " << Missions::name << " ";
		ss << "WHERE mission_id = '" << missionID << "' AND character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst()) {
			r = !cmd.Field("is_done").asBool();
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

long long Missions::getMissionDoneTimestamp(long missionID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	long long r = -1;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT done_timestamp FROM";
		ss << " " << Missions::name << " ";
		ss << "WHERE mission_id = '" << missionID << "' AND character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("done_timestamp").asNumeric();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

long Missions::getMissionDoneCount(long missionID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	long r = -1;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT done_count FROM";
		ss << " " << Missions::name << " ";
		ss << "WHERE mission_id = '" << missionID << "' AND character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("done_count").asLong();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

vector<MissionInfo> Missions::getAllDoneMissions(long long charID)
{
	SAConnection con;
	SACommand cmd;

	vector<MissionInfo> r = vector<MissionInfo>();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT mission_id, done_timestamp, done_count FROM";
		ss << " " << Missions::name << " ";
		ss << "WHERE character_id = '" << charID << "' AND is_done = '1';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			MissionInfo mi = MissionInfo();

			mi.missionID = cmd.Field("mission_id").asLong();
			mi.doneTimestamp = cmd.Field("done_timestamp").asNumeric();
			mi.doneCount = cmd.Field("done_count").asLong();

			r.push_back(mi);
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

vector<MissionInfo> Missions::getAllCurrentMissions(long long charID)
{
	SAConnection con;
	SACommand cmd;

	vector<MissionInfo> r = vector<MissionInfo>();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT mission_id FROM";
		ss << " " << Missions::name << " ";
		ss << "WHERE character_id = '" << charID << "' AND is_done = '0';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			MissionInfo mi = MissionInfo();

			mi.missionID = cmd.Field("mission_id").asLong();

			r.push_back(mi);
		}

		con.Commit();
		con.Disconnect();

		for (int i = 0; i < r.size(); i++)
		{
			vector<MissionTask> tasks = CurrentMissionTasks::getMissionTasks(r.at(i).missionID, charID);
			r.at(i).missionTasks = tasks;
		}
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

void Missions::callOnMissionTaskUpdate(MissionTaskType taskType, long long charID, long long objectID, SystemAddress clientAddress)
{
	ReplicaObject* other = ObjectsManager::getObjectByID(objectID);
	ReplicaObject* player = ObjectsManager::getObjectByID(charID);

	for (int i = 0; i < player->currentMissions.size(); i++)
	{
		MissionInfo* info = &player->currentMissions.at(i);

		for (int k = 0; k < info->missionTasks.size(); k++)
		{
			MissionTask* task = &info->missionTasks.at(k);

			switch (task->type)
			{

			case MISSION_TASK_TYPE_INTERACT:
			{
				if (other != nullptr)
				{
					for (int l = 0; l < task->targets.size(); l++)
					{
						if (other->lot == task->targets.at(l))
						{
							if (!Vectors::contains(task->value, (float)other->lot))
								task->value.push_back((float)other->lot);

							CurrentMissionTasks::setValue(task->uid, task->value, charID);
							GameMessages::notifyMissionTask(charID, info->missionID, k, task->value.at(task->value.size() - 1), clientAddress);

							if (task->value.size() >= task->targetValue)
							{
								if (!CDClient::isMission(info->missionID))
									/*{
									GameMessages::notifyMission(charID, info->missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
									}
									else*/
								{
									Missions::completeMission(info->missionID, charID, clientAddress);
								}
							}
						}
					}
				}
				break;
			}

			case MISSION_TASK_TYPE_COLLECT_COLLECTIBLE:
			{
				if (other != nullptr)
				{
					for (int l = 0; l < task->targets.size(); l++)
					{
						if (other->lot == task->targets.at(l))
						{
							long collectibleID = other->collectibleIndex->collectible_id;
							if (!Vectors::contains(task->value, (float)collectibleID))
								task->value.push_back((float)collectibleID);

							CurrentMissionTasks::setValue(task->uid, task->value, charID);
							GameMessages::notifyMissionTask(charID, info->missionID, k, task->value.at(task->value.size() - 1), clientAddress);

							if (task->value.size() >= task->targetValue)
							{
								if (!CDClient::isMission(info->missionID))
									/*{
									GameMessages::notifyMission(charID, info->missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
									}
									else*/
								{
									Missions::completeMission(info->missionID, charID, clientAddress);
								}
							}
						}
					}
				}
				break;
			}

			case MISSION_TASK_TYPE_FLAG_CHANGE:
			{
				for (int l = 0; l < task->targets.size(); l++)
				{
					if (objectID == task->targets.at(l))
					{
						if (!Vectors::contains(task->value, (float)objectID))
							task->value.push_back((float)objectID);

						CurrentMissionTasks::setValue(task->uid, task->value, charID);
						GameMessages::notifyMissionTask(charID, info->missionID, k, task->value.size(), clientAddress);

						if (task->value.size() >= task->targetValue)
						{
							if (!CDClient::isMission(info->missionID))
							/*{
								GameMessages::notifyMission(charID, info->missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
							}
							else*/
							{
								Missions::completeMission(info->missionID, charID, clientAddress);
							}
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
}

void Missions::completeMission(long missionID, long long charID, SystemAddress clientAddress)
{
	if (!Missions::hasDoneMission(missionID, charID))
	{
		if (missionID == 173)
		{
			Missions::completeMission(664, charID, clientAddress);
		}

		GameMessages::notifyMission(charID, missionID, MissionState::MISSION_STATE_COMPLETE, true, clientAddress);
		Missions::setMissionDone(missionID, charID);
		Missions::incrementMissionDoneCount(missionID, charID);
		CurrentMissionTasks::deleteMissionTasks(missionID, charID);
		Missions::rewardMission(missionID, charID, clientAddress);
		GameMessages::notifyMission(charID, missionID, MissionState::MISSION_STATE_REMOVE_FROM_MISSION_CHECKER, false, clientAddress);
	}
}

void Missions::rewardMission(long missionID, long long charID, SystemAddress clientAddress)
{
	MissionRewards rewards = CDClient::getMissionRewards(missionID);
	ReplicaObject* replica = ObjectsManager::getObjectByID(charID);
	bool isMission = CDClient::isMission(missionID);

	long long newUniverseScore = Characters::getUniverseScore(charID) + rewards.universeScore;
	Characters::setUniverseScore(newUniverseScore, charID);
	replica->characterIndex->lego_score = newUniverseScore;
	GameMessages::modifyLegoScore(charID, rewards.universeScore, isMission, clientAddress);

	long long newReputation = Characters::getReputation(charID) + rewards.reputation;
	Characters::setReputation(newReputation, charID);
	GameMessages::updateReputation(charID, newReputation, clientAddress);

	Position pos = Position();
	pos.x = replica->controllablePhysicsIndex->pos_x;
	pos.y = replica->controllablePhysicsIndex->pos_y;
	pos.z = replica->controllablePhysicsIndex->pos_z;

	long long curCurrency = Characters::getCurrency(charID);
	long long newCurrency = curCurrency + rewards.currency;
	Characters::setCurrency(newCurrency, charID);
	if (isMission) GameMessages::setCurrency(charID, newCurrency, pos, clientAddress);

	if (rewards.maxHealth > -1)
	{
		long newMaxHealth = Characters::getMaxHealth(charID) + rewards.maxHealth;
		Characters::setMaxHealth(newMaxHealth, charID);
		replica->statsIndex->max_health = (float)newMaxHealth;
		replica->statsIndex->cur_health = (float)newMaxHealth;
	}

	if (rewards.maxImagination > -1)
	{
		long newMaxImagination = Characters::getMaxImagination(charID) + rewards.maxImagination;
		Characters::setMaxImagination(newMaxImagination, charID);
		replica->statsIndex->max_imagination = (float)newMaxImagination;
		replica->statsIndex->cur_imagination = (float)newMaxImagination;
	}

	if (rewards.maxInventory > -1)
	{
		long newMaxInventory = Characters::getMaxInventory(charID) + rewards.maxInventory;
		Characters::setMaxInventory(newMaxInventory, charID);
		GameMessages::setInventorySize(charID, InventoryType::INVENTORY_TYPE_DEFAULT, newMaxInventory, clientAddress);
	}

	for (int i = 0; i < replica->currentMissions.size(); i++)
	{
		if (replica->currentMissions.at(i).missionID == missionID)
		{
			if (replica->currentMissions.at(i).rewardLOT > 0)
			{
				for (int k = 0; k < rewards.itemLOTs.size(); k++)
				{
					if (rewards.itemLOTs.at(k) == replica->currentMissions.at(i).rewardLOT)
					{
						Helpers::createSyncedItemStack(charID, replica->currentMissions.at(i).rewardLOT, rewards.itemCounts.at(k), false, false, clientAddress);
					}
				}
			}
			else
			{
				for (int k = 0; k < rewards.itemLOTs.size(); k++)
				{
					Helpers::createSyncedItemStack(charID, rewards.itemLOTs.at(k), rewards.itemCounts.at(k), false, false, clientAddress);
				}
			}
		}
	}

	ObjectsManager::serializeObject(replica);
	Server::sendPacket(PacketUtils::createGMBase(charID, GameMessageID::GAME_MESSAGE_ID_RESTORE_TO_POST_LOAD_STATS), clientAddress);
}
