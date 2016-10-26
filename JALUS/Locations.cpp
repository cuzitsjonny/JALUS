#include "Locations.h"
#include "Config.h"
#include "Logger.h"

string Locations::name;

void Locations::init(string name, string structure)
{
	Locations::name = name;

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

void Locations::saveLocation(Location loc, long long objectID)
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
		ss << "REPLACE INTO";
		ss << " " << Locations::name << " ";
		ss << "(id, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, rot_w, zone_id, map_clone)";
		ss << " VALUES ";
		ss << "('" << objectID << "', '" << loc.position.x << "', '" << loc.position.y << "', '" << loc.position.z << "', '" << loc.rotation.x << "', '" << loc.rotation.y << "', '" << loc.rotation.z << "', '" << loc.rotation.w << "', '" << loc.zoneID << "', '" << loc.mapClone << "');";

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

void Locations::deleteLocation(long long objectID)
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
		ss << " " << Locations::name << " ";
		ss << "WHERE id = '" << objectID << "';";

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

Location Locations::getLocation(long long objectID)
{
	SAConnection con;
	SACommand cmd;

	Location r = Location();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT * FROM";
		ss << " " << Locations::name << " ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
		{
			r.position.x = cmd.Field("pos_x").asDouble();
			r.position.y = cmd.Field("pos_y").asDouble();
			r.position.z = cmd.Field("pos_z").asDouble();
			r.rotation.x = cmd.Field("rot_x").asDouble();
			r.rotation.y = cmd.Field("rot_y").asDouble();
			r.rotation.z = cmd.Field("rot_z").asDouble();
			r.rotation.w = cmd.Field("rot_w").asDouble();
			r.zoneID = (ZoneID)cmd.Field("zone_id").asLong();
			r.mapClone = cmd.Field("map_clone").asLong();
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
