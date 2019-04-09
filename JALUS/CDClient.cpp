#include "CDClient.h"
#include "Config.h"
#include "Logger.h"

// !!! SQLite besitzt keine SACommand::FetchFirst() Funktion !!!

ItemType CDClient::getItemType(long lot)
{
	SAConnection con;
	SACommand cmd;

	ItemType r = ItemType::INVALID_ITEM_TYPE;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT component_id FROM";
		ss << " ComponentsRegistry ";
		ss << "WHERE id = '" << lot << "' AND component_type = '11';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchNext())
		{
			long componentID = cmd.Field("component_id").asLong();

			ss.str("");

			ss << "SELECT itemType, stackSize FROM";
			ss << " ItemComponent ";
			ss << "WHERE id = '" << componentID << "';";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			if (cmd.FetchNext())
				r = (ItemType)cmd.Field("itemType").asLong();
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

string CDClient::getMinifigureName(long name_first, long name_middle, long name_last)
{
	SAConnection con;
	SACommand cmd;

	string r = "";

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT name FROM";
		ss << " MinifigName_First ";
		ss << "WHERE id = '" << (name_first + 1) << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchNext())
			r += cmd.Field("name").asString();

		ss.str("");

		ss << "SELECT name FROM";
		ss << " MinifigName_Middle ";
		ss << "WHERE id = '" << (name_middle + 1) << "';";

		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchNext())
			r += cmd.Field("name").asString();

		ss.str("");

		ss << "SELECT name FROM";
		ss << " MinifigName_Last ";
		ss << "WHERE id = '" << (name_last + 1) << "';";

		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchNext())
			r += cmd.Field("name").asString();

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

bool CDClient::isActivityZone(ZoneID zoneID)
{
	SAConnection con;
	SACommand cmd;

	bool r = false;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT ActivityID FROM";
		ss << " Activities ";
		ss << "WHERE instanceMapID = '" << zoneID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		r = cmd.FetchNext();

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

vector<ComponentsRegistryEntry> CDClient::getComponentsRegistryEntries(long lot)
{
	SAConnection con;
	SACommand cmd;

	vector<ComponentsRegistryEntry> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT component_type, component_id FROM";
		ss << " ComponentsRegistry ";
		ss << "WHERE id = '" << lot << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			ComponentsRegistryEntry entry = ComponentsRegistryEntry();

			entry.componentType = cmd.Field("component_type").asLong();
			entry.componentID = cmd.Field("component_id").asLong();

			r.push_back(entry);
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

vector<long> CDClient::getInventoryIndexInfo(long lot)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT component_id FROM";
		ss << " ComponentsRegistry ";
		ss << "WHERE id = '" << lot << "' AND component_type = '17';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		long componentID = -1;
		if (cmd.FetchNext())
		{
			componentID = cmd.Field("component_id").asLong();
		}

		if (componentID > 0)
		{
			ss.str("");

			ss << "SELECT itemid FROM";
			ss << " InventoryComponent ";
			ss << "WHERE id = '" << componentID << "' AND equip = '1';";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			while (cmd.FetchNext())
			{
				r.push_back(cmd.Field("itemid").asLong());
			}
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

StatsIndexInfo CDClient::getStatsIndexInfo(long componentID)
{
	SAConnection con;
	SACommand cmd;

	StatsIndexInfo r = StatsIndexInfo();

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT factionList, life, armor, imagination, isSmashable FROM";
		ss << " DestructibleComponent ";
		ss << "WHERE id = '" << componentID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchNext())
		{
			r.factionID = cmd.Field("factionList").asLong();
			r.health = cmd.Field("life").asLong();
			r.armor = cmd.Field("armor").asLong();
			r.imagination = cmd.Field("imagination").asLong();
			r.isSmashable = cmd.Field("isSmashable").asBool();
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

vector<MissionNPCInfo> CDClient::getMissionNPCIndexInfo(long lot)
{
	SAConnection con;
	SACommand cmd;

	vector<MissionNPCInfo> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT component_id FROM";
		ss << " ComponentsRegistry ";
		ss << "WHERE id = '" << lot << "' AND component_type = '73';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		long componentID = -1;
		if (cmd.FetchNext())
		{
			componentID = cmd.Field("component_id").asLong();
		}

		if (componentID > 0)
		{
			ss.str("");

			ss << "SELECT missionID, offersMission, acceptsMission FROM";
			ss << " MissionNPCComponent ";
			ss << "WHERE id = '" << componentID << "';";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			while (cmd.FetchNext())
			{
				MissionNPCInfo mi = MissionNPCInfo();

				mi.missionID = cmd.Field("missionID").asLong();
				mi.offersMission = cmd.Field("offersMission").asBool();
				mi.acceptsMission = cmd.Field("acceptsMission").asBool();

				r.push_back(mi);
			}
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

vector<long> CDClient::getPrereqMissionIDs(long missionID)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT prereqMissionID FROM";
		ss << " Missions ";
		ss << "WHERE id = '" << missionID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			string prereqStr = string(cmd.Field("prereqMissionID").asString());
			vector<string> p = split(prereqStr, ',');

			for (int i = 0; i < p.size(); i++)
			{
				r.push_back(stol(p.at(i)));
			}
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

vector<MissionTask> CDClient::getMissionTasks(long missionID)
{
	SAConnection con;
	SACommand cmd;

	vector<MissionTask> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT taskType, target, targetGroup, targetValue, uid FROM";
		ss << " MissionTasks ";
		ss << "WHERE id = '" << missionID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			MissionTask mt = MissionTask();

			mt.type = (MissionTaskType)cmd.Field("taskType").asLong();
			mt.targetValue = (float)cmd.Field("targetValue").asDouble();
			mt.uid = cmd.Field("uid").asLong();

			string targetStr = string(cmd.Field("target").asString());
			if (targetStr.length() > 0)
			{
				mt.targets.push_back(stol(targetStr));
			}

			string targetGroupStr = string(cmd.Field("targetGroup").asString());
			if (targetGroupStr.length() > 0)
			{
				vector<string> p = split(targetGroupStr, ',');
				for (int i = 0; i < p.size(); i++)
				{
					if (p.at(i).length() > 0)
						mt.targets.push_back(stol(p.at(i)));
				}
			}

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

MissionRewards CDClient::getMissionRewards(long missionID)
{
	SAConnection con;
	SACommand cmd;

	MissionRewards r = MissionRewards();

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT reward_item1, reward_item1_count, reward_item2, reward_item2_count, reward_item3, reward_item3_count, reward_item4, reward_item4_count, reward_emote, reward_emote2, reward_emote3, reward_emote4, reward_maximagination, reward_maxhealth, reward_maxinventory, LegoScore, reward_reputation, reward_currency FROM";
		ss << " Missions ";
		ss << "WHERE id = '" << missionID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchNext())
		{
			for (int i = 1; i < 5; i++)
			{
				string index = to_string(i);
				string lot = string(cmd.Field(("reward_item" + index).c_str()).asString());
				string count = string(cmd.Field(("reward_item" + index + "_count").c_str()).asString());

				if (lot.length() > 0 && count.length() > 0)
				{
					r.itemLOTs.push_back(stol(lot));
					r.itemCounts.push_back(stol(count));
				}

				if (i == 1)
					index = "";

				string emote = string(cmd.Field(("reward_emote" + index).c_str()).asString());

				if (emote.length() > 0)
					r.emotes.push_back(stol(emote));
			}

			r.currency = cmd.Field("reward_currency").asNumeric();
			r.reputation = cmd.Field("reward_reputation").asNumeric();
			r.universeScore = cmd.Field("LegoScore").asNumeric();

			r.maxHealth = cmd.Field("reward_maxhealth").asLong();
			r.maxImagination = cmd.Field("reward_maximagination").asLong();
			r.maxInventory = cmd.Field("reward_maxinventory").asLong();
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

/*RebuildInfo CDClient::getRebuildInfo(long lot)
{
	SAConnection con;
	SACommand cmd;

	RebuildInfo r = RebuildInfo();

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT component_id FROM";
		ss << " ComponentsRegistry ";
		ss << "WHERE id = '" << lot << "' AND component_type = '48';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		long componentID = -1;
		if (cmd.FetchNext())
		{
			componentID = cmd.Field("component_id").asLong();
		}

		if (componentID > 0)
		{
			ss.str("");

			ss << "SELECT reset_time, complete_time, take_imagination, interruptible, self_activator, custom_modules, activityID, post_imagination_cost, time_before_smash FROM";
			ss << " RebuildComponent ";
			ss << "WHERE id = '" << componentID << "';";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			while (cmd.FetchNext())
			{
				r.reset_time = cmd.Field("reset_time").asDouble;
				r.complete_time = cmd.Field("complete_time").asDouble;
				r.take_imagination = cmd.Field("take_imagination").asLong;
				r.interruptible = cmd.Field("interruptible").asBool;
				
				r.activityID = cmd.Field("activityID").asLong;
				r.post_imagination_cost = (cmd.Field("post_imagination_cost").asLong;
				r.time_before_smash = cmd.Field("time_before_smash").asDouble;
			}
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
*/

bool CDClient::isMission(long missionID)
{
	SAConnection con;
	SACommand cmd;

	bool r = false;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT isMission FROM";
		ss << " Missions ";
		ss << "WHERE id = '" << missionID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchNext())
		{
			r = cmd.Field("isMission").asBool();
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

vector<long> CDClient::getAchievements(long prereqMissionID)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT id FROM";
		ss << " Missions ";

		if (prereqMissionID > -1)
		{
			ss << "WHERE prereqMissionID LIKE '%" << prereqMissionID << "%' AND isMission = '0';";
		}
		else
		{
			ss << "WHERE (prereqMissionID IS NULL OR prereqMissionID = '') AND isMission = '0';";
		}

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			r.push_back(cmd.Field("id").asLong());
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

long CDClient::lookUpLevel(long long universeScore)
{
	SAConnection con;
	SACommand cmd;

	long r = 1;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT id, requiredUScore FROM";
		ss << " LevelProgressionLookup ";
		ss << "WHERE 1;";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			long long uScore = cmd.Field("requiredUScore").asNumeric();
			long l = cmd.Field("id").asLong();

			if (universeScore >= uScore)
			{
				if (l > r)
					r = l;
			}
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

long long CDClient::lookUpUniverseScore(long level)
{
	SAConnection con;
	SACommand cmd;

	long long r = 1;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT requiredUScore FROM";
		ss << " LevelProgressionLookup ";
		ss << "WHERE id = '" << level << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			long long uScore = cmd.Field("requiredUScore").asNumeric();
			//long l = cmd.Field("id").asLong();

			r = uScore;

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

vector<long> CDClient::getItemDrops(long lot, long LootTableIndex)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT itemid FROM LootTable WHERE LootTableIndex = " << LootTableIndex << ";";
		/*ss << "SELECT itemid FROM LootTable WHERE LootTableIndex = ";
		ss << "(SELECT LootTableIndex FROM LootMatrix WHERE LootMatrixIndex = ";
		ss << "(SELECT LootMatrixIndex FROM DestructibleComponent WHERE id = ";
		ss << "(SELECT component_id FROM ComponentsRegistry WHERE id = '" << lot << "' AND component_type = '7')));";*/

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			//long items = cmd.Field("itemid").asLong();
			
			r.push_back(cmd.Field("itemid").asLong());



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

vector<double> CDClient::getDropProbs(long lot, long row)
{
	SAConnection con;
	SACommand cmd;

	vector<double> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		// SQL code
		/*SELECT percent, minToDrop, maxToDrop, LootTableIndex FROM LootMatrix WHERE LootMatrixIndex =
		(SELECT LootMatrixIndex FROM DestructibleComponent WHERE id =
		(SELECT component_id FROM ComponentsRegistry WHERE id = '6415' AND component_type = '7'))*/

		// SQL code to get specific item drop possible.
		/*SELECT * FROM Objects WHERE id =
		(SELECT itemid FROM LootTable WHERE LootTableIndex =
		(SELECT LootTableIndex FROM LootMatrix WHERE LootMatrixIndex = 
		(SELECT LootMatrixIndex FROM DestructibleComponent WHERE id = 
		(SELECT component_id FROM ComponentsRegistry WHERE id = '11989' AND component_type = '7')) LIMIT 1 OFFSET 0))*/

		stringstream ss;
		ss << "SELECT percent, minToDrop, maxToDrop, LootTableIndex FROM LootMatrix WHERE LootMatrixIndex =";
		ss << "(SELECT LootMatrixIndex FROM DestructibleComponent WHERE id = ";
		ss << "(SELECT component_id FROM ComponentsRegistry WHERE id = '" << lot << "' AND component_type = '7')) LIMIT 1 OFFSET " << row << ";";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			//double percent = cmd.Field("percent").asDouble();
			//long minToDrop = cmd.Field("minToDrop").asLong();
			//long maxToDrop = cmd.Field("maxToDrop").asLong();
			//long lootTableIndex = cmd.Field("LootTableIndex").asLong();

			r.push_back(cmd.Field("percent").asDouble());
			r.push_back(cmd.Field("minToDrop").asLong());
			r.push_back(cmd.Field("maxToDrop").asLong());
			r.push_back(cmd.Field("LootTableIndex").asLong());



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

vector<long> CDClient::getCoinDrops(long lot)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT minvalue, maxvalue FROM CurrencyTable WHERE CurrencyIndex = ";
		ss << "(SELECT CurrencyIndex FROM DestructibleComponent WHERE id = ";
		ss << "(SELECT component_id FROM ComponentsRegistry WHERE id = '" << lot << "' AND component_type = '7'));";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			//long minvalue = cmd.Field("minvalue").asLong();
			//long maxvalue = cmd.Field("maxvalue").asLong();
			
			r.push_back(cmd.Field("minvalue").asLong());
			r.push_back(cmd.Field("maxvalue").asLong());



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

long CDClient::getLootTableIndexCount(long lot)
{
	SAConnection con;
	SACommand cmd;

	long r = 0;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT COUNT(*) As rowCount FROM LootMatrix WHERE LootMatrixIndex = ";
		ss << "(SELECT LootMatrixIndex FROM DestructibleComponent WHERE id = ";
		ss << "(SELECT component_id FROM ComponentsRegistry WHERE id = '" << lot << "' AND component_type = '7'));";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			long lootTableIndexCount = cmd.Field("rowCount").asLong();

			//r.push_back(cmd.Field("RowCount").asLong());
			r = cmd.Field("rowCount").asLong();


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

long CDClient::getIsPowerup(long lot)
{
	SAConnection con;
	SACommand cmd;

	long r = 0;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		//ss << "SELECT * FROM Objects WHERE type = 'Powerup';";
		//ss << "SELECT type FROM Objects WHERE id = '" << lot << "';";
		//ss << "SELECT type FROM Objects WHERE id = '" << lot << "';";
		ss << "SELECT COUNT(*) As isPowerup FROM Objects WHERE type = 'Powerup' AND id = '" << lot << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			long isPowerup = cmd.Field("isPowerup").asLong();

			r = cmd.Field("isPowerup").asLong();

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

long CDClient::getStackSize(long lot)
{
	SAConnection con;
	SACommand cmd;

	long r = 0;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT stackSize FROM ItemComponent WHERE id = ";
		ss << "(SELECT component_id FROM ComponentsRegistry WHERE id = '" << lot << "' AND component_type = '11');";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			long stackSize = cmd.Field("stackSize").asLong();

			r = cmd.Field("stackSize").asLong();

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

long CDClient::getSkillID(long lot, long castOnType)
{
	SAConnection con;
	SACommand cmd;

	long r = 0;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT skillID FROM ObjectSkills WHERE objectTemplate = '" << lot << "' AND castOnType = '" << castOnType << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			long skillID = cmd.Field("skillID").asLong();

			r = cmd.Field("skillID").asLong();

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

vector<long> CDClient::getSkillIDs(long lot, long castOnType)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT skillID FROM ObjectSkills WHERE objectTemplate = '" << lot << "' AND castOnType = '" << castOnType << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			//vector<long> test;
			//long skillID = cmd.Field("skillID").asLong();

			r.push_back(cmd.Field("skillID").asLong());

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

vector<long> CDClient::getBonus(long skillID)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT imBonusUI, lifeBonusUI, armorBonusUI FROM SkillBehavior WHERE skillID = '" << skillID << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			//long skillID = cmd.Field("skillID").asLong();

			r.push_back(cmd.Field("imBonusUI").asLong());
			r.push_back(cmd.Field("lifeBonusUI").asLong());
			r.push_back(cmd.Field("armorBonusUI").asLong());

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

long CDClient::getTemplateID(long skillID)
{
	SAConnection con;
	SACommand cmd;

	long r = 0;

	/*SELECT name FROM BehaviorTemplateName WHERE templateID =
	(SELECT templateID FROM BehaviorTemplate WHERE behaviorID =
	(SELECT behaviorID FROM SkillBehavior WHERE skillID =
	(SELECT skillID FROM ObjectSkills WHERE objectTemplate = '4880' AND castOnType = '0')))*/

	/*SELECT name FROM BehaviorTemplateName WHERE templateID =
	(SELECT templateID FROM BehaviorTemplate WHERE behaviorID =
	(SELECT behaviorID FROM SkillBehavior WHERE skillID = '148'))*/

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT templateID FROM BehaviorTemplate WHERE behaviorID =";
		ss << "(SELECT behaviorID FROM SkillBehavior WHERE skillID = '" << skillID << "');";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			long templateID = cmd.Field("templateID").asLong();

			r = cmd.Field("templateID").asLong();

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

string CDClient::getTemplateIDName(long skillID)
{
	SAConnection con;
	SACommand cmd;

	string r ;

	/*SELECT name FROM BehaviorTemplateName WHERE templateID =
	(SELECT templateID FROM BehaviorTemplate WHERE behaviorID =
	(SELECT behaviorID FROM SkillBehavior WHERE skillID = 
	(SELECT skillID FROM ObjectSkills WHERE objectTemplate = '4880' AND castOnType = '0')))*/

	/*SELECT name FROM BehaviorTemplateName WHERE templateID =
	(SELECT templateID FROM BehaviorTemplate WHERE behaviorID =
	(SELECT behaviorID FROM SkillBehavior WHERE skillID = '148'))*/

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT name FROM BehaviorTemplateName WHERE templateID = ";
		ss << "(SELECT templateID FROM BehaviorTemplate WHERE behaviorID =";
		ss << "(SELECT behaviorID FROM SkillBehavior WHERE skillID = '" << skillID << "'));";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			string name = cmd.Field("name").asString();

			r = cmd.Field("name").asString();

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

bool CDClient::getIsEquippable(long lot)
{
	SAConnection con;
	SACommand cmd;

	bool r = false;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		//ss << "SELECT equipLocation FROM ItemComponent WHERE id = '" << lot << "';";
		ss << "SELECT equipLocation  FROM ItemComponent WHERE id = ";
		ss << "(SELECT component_id FROM ComponentsRegistry WHERE id = '" << lot << "' AND component_type = '11');";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			string equipLocation = cmd.Field("equipLocation").asString();
			string test;
			if (equipLocation == "")
			{
				Logger::info("This is a test:" + equipLocation + ". This concludes the test");
				r = false;
			}
			else {
				r = true;
			}

			//r = cmd.Field("equipLocation").asBool();

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
bool CDClient::hasSubItems(long lot)
{
	SAConnection con;
	SACommand cmd;

	bool r = false;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		//ss << "SELECT equipLocation FROM ItemComponent WHERE id = '" << lot << "';";
		ss << "SELECT subItems FROM ItemComponent WHERE id = ";
		ss << "(SELECT component_id FROM ComponentsRegistry WHERE id = '" << lot << "' AND component_type = '11');";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			string items = cmd.Field("subItems").asString();
			if (items == "")
				r = false;
			else
				r = true;


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
vector<long> CDClient::getSubItems(long lot)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT subItems FROM ItemComponent WHERE id = ";
		ss << "(SELECT component_id FROM ComponentsRegistry WHERE id = '" << lot << "' AND component_type = '11');";
		//ss << "SELECT subItems FROM ItemComponent WHERE id = '" << lot << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			string subItemStr = string(cmd.Field("subItems").asString());
			vector<string> p = split(subItemStr, ',');
			//Logger::info("SubItemStr: " + subItemStr);
			for (int i = 0; i < p.size(); i++)
			{
				//Logger::info("SubItemsIndv: " + p[i]);
				try {
					r.push_back(stol(p.at(i)));
				}
				catch (exception& e) {
					// Literally nothing here
				}					
			}

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
bool CDClient::isAnimationValid(string animationID)
{
	SAConnection con;
	SACommand cmd;

	bool r = false;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT animationGroupID FROM Animations WHERE animation_type = '" << animationID << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			string isValid = cmd.Field("animationGroupID").asString();
			if (isValid == "")
				r = false;
			else
				r = true;

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
string CDClient::getAnimationOfEmote(int emoteID)
{
	SAConnection con;
	SACommand cmd;

	string r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT animationName FROM Emotes WHERE id = '" << emoteID << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
			r = cmd.Field("animationName").asString();


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


string CDClient::getBrickColorName(int brickColor)
{
	SAConnection con;
	SACommand cmd;

	string r;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT description FROM BrickColors WHERE id = '" << brickColor << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
			r = cmd.Field("description").asString();


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

bool CDClient::getBrickColorCharacterValid(int brickColor)
{
	SAConnection con;
	SACommand cmd;

	bool r = false;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT validCharacters FROM BrickColors WHERE id = '" << brickColor << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
			r = cmd.Field("validCharacters").asBool();

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

long CDClient::getTemplateFromName(string name)
{
	SAConnection con;
	SACommand cmd;

	long r = -1;

	try
	{
		con.Connect(Config::getCDClientPath().c_str(), "", "", SA_SQLite_Client);

		stringstream ss;
		ss << "SELECT id FROM Objects WHERE name = '" << name << "';";
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
			r = cmd.Field("id").asLong();


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