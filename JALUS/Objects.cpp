#include "Objects.h"
#include "Config.h"
#include "Logger.h"
#include "TemporaryItems.h"
#include "Server.h"
#include "ReplicaObject.h"

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

	int i = 0;
	while (r && i < Server::getReplicaManager()->GetReplicaCount())
	{
		ReplicaObject* replica = (ReplicaObject*)Server::getReplicaManager()->GetReplicaAtIndex(i);
		r = (replica->objectID != objectID);
		i++;
	}

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
	long long rand = randomInRange(OBJECT_ID_MIN, OBJECT_ID_MAX);

	if (Objects::validate(rand))
		return rand;
	else
		return Objects::generateObjectID();
}

long long Objects::generateSpawnerID()
{
	long long rand = randomInRange(SPAWNER_ID_MIN, SPAWNER_ID_MAX);

	if (Objects::validate(rand))
		return rand;
	else
		return Objects::generateSpawnerID();
}

bool Objects::isInSpawnerIDRange(long long objectID)
{
	return (objectID <= SPAWNER_ID_MAX && objectID >= SPAWNER_ID_MIN);
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
