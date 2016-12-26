#include "CurrentMissionTasks.h"
#include "Config.h"
#include "Logger.h"

string CurrentMissionTasks::name;

void CurrentMissionTasks::init(string name, string structure)
{
	CurrentMissionTasks::name = name;

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

void CurrentMissionTasks::addMissionTask(long missionID, long uniqueID, long long charID)
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
		ss << " " << CurrentMissionTasks::name << " ";
		ss << "(id, mission_id, character_id, unique_id, value)";
		ss << " VALUES ";
		ss << "(NULL, '" << missionID << "', '" << charID << "', '" << uniqueID << "', '');";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		Logger::info(string(x.ErrMessage()));
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void CurrentMissionTasks::deleteMissionTasks(long missionID, long long charID)
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
		ss << " " << CurrentMissionTasks::name << " ";
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

void CurrentMissionTasks::deleteMissionTasks(long long charID)
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
		ss << " " << CurrentMissionTasks::name << " ";
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

void CurrentMissionTasks::setValue(long uniqueID, vector<float> value, long long charID)
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
		ss << " " << CurrentMissionTasks::name << " ";
		ss << "SET value = '" << Vectors::toString(value) << "' ";
		ss << "WHERE unique_id = '" << uniqueID << "' AND character_id = '" << charID << "';";

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

vector<float> CurrentMissionTasks::getValue(long uniqueID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	vector<float> r = vector<float>();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT value FROM";
		ss << " " << CurrentMissionTasks::name << " ";
		ss << "WHERE unique_id = '" << uniqueID << "' AND character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
		{
			r = Vectors::fromString(string(cmd.Field("value").asString()));
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

vector<MissionTask> CurrentMissionTasks::getMissionTasks(long missionID, long long charID)
{
	SAConnection con;
	SACommand cmd;

	vector<MissionTask> r = vector<MissionTask>();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT unique_id, value FROM";
		ss << " " << CurrentMissionTasks::name << " ";
		ss << "WHERE mission_id = '" << missionID << "' AND character_id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			MissionTask mt = MissionTask();

			mt.uid = cmd.Field("unique_id").asLong();
			mt.value = Vectors::fromString(string(cmd.Field("value").asString()));

			r.push_back(mt);
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

string Vectors::toString(vector<float> v)
{
	string str = "";

	for (int i = 0; i < v.size(); i++)
	{
		if (i == 0)
		{
			str += to_string(v.at(0));
		}
		else
		{
			str += ("," + to_string(v.at(i)));
		}
	}

	return str;
}

vector<float> Vectors::fromString(string str)
{
	vector<float> r = vector<float>();
	vector<string> p = split(str, ',');

	for (int i = 0; i < p.size(); i++)
	{
		r.push_back(stof(p.at(i)));
	}

	return r;
}

bool Vectors::contains(vector<float> v, float f)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v.at(i) == f)
			return true;
	}

	return false;
}
