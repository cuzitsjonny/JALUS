#include "CDClient.h"
#include "Config.h"

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

			ss << "SELECT itemType FROM";
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

vector<long> CDClient::getInventoryComponentEntries(long lot)
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
