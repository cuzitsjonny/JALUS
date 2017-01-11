#include "TransitionInfos.h"
#include "Config.h"
#include "Logger.h"
#include "Server.h"

const string ALPHANUM = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

string TransitionInfos::name;

void TransitionInfos::init(string name, string structure)
{
	TransitionInfos::name = name;

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

string TransitionInfos::generateTransitionInfo(SystemAddress clientAddress, long long accountID)
{
	string key = "";

	for (int i = 0; i < 32; i++)
	{
		key += ALPHANUM[rand() % sizeof(ALPHANUM)];
	}

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
		ss << " " << TransitionInfos::name << " ";
		ss << "(client_address, account_id, char_id, transition_key)";
		ss << " VALUES ";
		ss << "('" << clientAddress.ToString(false) << "', '" << accountID << "', '" << -1 << "', '" << key << "');";

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

	return key;
}

void TransitionInfos::insertTransitionInfo(SystemAddress clientAddress, long long accountID, long long charID, string transitionKey)
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
		ss << " " << TransitionInfos::name << " ";
		ss << "(client_address, account_id, char_id, transition_key)";
		ss << " VALUES ";
		ss << "('" << clientAddress.ToString(false) << "', '" << accountID << "', '" << charID << "', '" << transitionKey << "');";

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

void TransitionInfos::deleteTransitionInfo(SystemAddress clientAddress)
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
		ss << " " << TransitionInfos::name << " ";
		ss << "WHERE client_address = '" << clientAddress.ToString(false) << "';";

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

TransitionInfo TransitionInfos::getTransitionInfo(SystemAddress clientAddress)
{
	SAConnection con;
	SACommand cmd;

	TransitionInfo r = TransitionInfo();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT account_id, char_id, transition_key FROM";
		ss << " " << TransitionInfos::name << " ";
		ss << "WHERE client_address = '" << clientAddress.ToString(false) << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
		{
			r.clientAddress = clientAddress;
			r.accountID = cmd.Field("account_id").asNumeric();
			r.charID = cmd.Field("char_id").asNumeric();
			r.transitionKey = cmd.Field("transition_key").asString();
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
