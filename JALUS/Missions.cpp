#include "Missions.h"
#include "Config.h"
#include "Logger.h"
#include "CurrentMissionTasks.h"
#include "ReplicaObject.h"
#include "ObjectsManager.h"
#include "GameMessages.h"

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
				for (int l = 0; l < task->targets.size(); l++)
				{
					if (other->lot == task->targets.at(l))
					{
						task->value++;
						CurrentMissionTasks::setValue(task->uid, task->value, charID);

						vector<float> updates = vector<float>();
						updates.push_back(task->value);

						GameMessages::notifyMissionTask(charID, info->missionID, 0, updates, clientAddress);

						if (task->value == task->targetValue)
						{
							GameMessages::notifyMission(charID, info->missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
						}
					}
				}
				break;
			}

			case MISSION_TASK_TYPE_COLLECT_COLLECTIBLE:
			{
				for (int l = 0; l < task->targets.size(); l++)
				{
					if (other->lot == task->targets.at(l))
					{
						task->value++;
						CurrentMissionTasks::setValue(task->uid, task->value, charID);

						long collectibleID = other->collectibleIndex->collectible_id;
						vector<float> updates = vector<float>();
						updates.push_back((float)collectibleID);

						GameMessages::notifyMissionTask(charID, info->missionID, 0, updates, clientAddress);

						if (task->value >= task->targetValue)
						{
							GameMessages::notifyMission(charID, info->missionID, MissionState::MISSION_STATE_READY_TO_COMPLETE, true, clientAddress);
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
