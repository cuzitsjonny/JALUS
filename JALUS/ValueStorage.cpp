#include "ValueStorage.h"
#include "Config.h"
#include "Logger.h"
#include "Server.h"

vector<ValueStoring> ValueStorage::valueStorage;
string ValueStorage::name;

void ValueStorage::init(string name, string structure)
{
	ValueStorage::name = name;

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
		Server::shutdown(-1);
	}
}

//
//
//
//						BEGIN DATABASE STORAGE
//
//
//

void ValueStorage::createValueInDatabase(long long charID, string type, long value)
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
		ss << " " << ValueStorage::name << " ";
		ss << "(id, type, value)";
		ss << " VALUES ";
		ss << "('" << charID << "', :1, '" << value << "');";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = type.c_str();
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

void ValueStorage::updateValueInDatabase(long long charID, string type, long value)
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
		ss << "UPDATE";
		ss << " " << ValueStorage::name << " ";
		ss << "SET value = '" << value << "' ";
		ss << "WHERE id = '" << charID << "' AND type = :1;";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = type.c_str();
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
			//ValueStorage::createValueInDatabase(charID, type, value);
		}
		catch (SAException &) {}
	}
}

void ValueStorage::removeValueFromDatabase(long long charID, string type)
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
		ss << " " << ValueStorage::name << " ";
		ss << "WHERE id = '" << charID << "' AND type = :1;";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = type.c_str();
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

long ValueStorage::getValueFromDatabase(long long charID, string type)
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
		ss << "SELECT value FROM";
		ss << " " << ValueStorage::name << " ";
		ss << "WHERE id = '" << charID << "' AND type = :1;";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = type.c_str();
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("value").asLong();

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

//
//
//
//						BEGIN MEMORY STORAGE
//
//
//

void ValueStorage::createValueInMemory(long long charid, string type, long value)
{
	ValueStoring values;
	values.charid = charid;
	values.type = type;
	values.value = value;

	valueStorage.push_back(values);
}

void ValueStorage::updateValueInMemory(long long charid, string type, long value)
{
	bool found = false;
	for (int i = 0; i < valueStorage.size() && !found; i++)
	{
		if (valueStorage.at(i).charid == charid)
		{
			if (valueStorage.at(i).type == type)
			{
				valueStorage.at(i).value = value;
				found = true;
			}
		}
	}
	if (found == false) {
		ValueStorage::createValueInMemory(charid, type, value);
	}
}

void ValueStorage::removeValueFromMemory(long long charid, string type)
{
	bool found = false;
	for (int i = 0; i < valueStorage.size() && !found; i++)
	{
		if (valueStorage.at(i).charid == charid)
		{
			if (valueStorage.at(i).type == type)
			{
				valueStorage.erase(valueStorage.begin() + i);
				found = true;
			}
		}
	}
}

long ValueStorage::getValueInMemory(long long charid, string type)
{
	bool found = false;
	for (int i = 0; i < valueStorage.size(); i++)
	{
		if (valueStorage.at(i).charid == charid)
		{
			if (valueStorage.at(i).type == type)
			{
				return valueStorage.at(i).value;
				found = true;
			}
		}
	}
	if (found == false)
		return false;
}