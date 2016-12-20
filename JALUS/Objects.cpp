#include "Objects.h"
#include "Config.h"
#include "Logger.h"
#include "TemporaryItems.h"

string Objects::name;

bool Objects::validate(long long objectID)
{
	SAConnection con;
	SACommand cmd;

	bool r = true;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT lot FROM";
		ss << " " << Objects::name << " ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = false;

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

	if (r)
		r = (!TemporaryItems::isObjectIDOccupied(objectID));

	// Implement ObjectsManager validation!

	return r;
}

void Objects::init(string name, string structure)
{
	Objects::name = name;

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

long long Objects::generateObjectID()
{
	/*long long min = 288300000000000000;
	long long max = 288399999999999999;*/

	long long min = 1000000000000000000;
	long long max = 1999999999999999999;

	long long rand = randomInRange(min, max);

	if (Objects::validate(rand))
		return rand;
	else
		return Objects::generateObjectID();
}

long long Objects::generateSpawnerID()
{
	long long min = 700000000000000000;
	long long max = 799999999999999999;

	long long rand = randomInRange(min, max);

	if (Objects::validate(rand))
		return rand;
	else
		return Objects::generateSpawnerID();
}

long long Objects::createObject(long lot)
{
	long long id;

	if (lot == 176)
		id = Objects::generateSpawnerID();
	else
		id = Objects::generateObjectID();

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
		ss << " " << Objects::name << " ";
		ss << "(id, lot)";
		ss << " VALUES ";
		ss << "('" << id << "', '" << lot << "');";

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

	return id;
}

void Objects::deleteObject(long long objectID)
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
		ss << " " << Objects::name << " ";
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

long Objects::getLOT(long long objectID)
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
		ss << "SELECT lot FROM";
		ss << " " << Objects::name << " ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("lot").asLong();

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
