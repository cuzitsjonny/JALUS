#include "Accounts.h"
#include "Config.h"
#include "TimeUtils.h"
#include "Logger.h"
#include "Server.h"

string Accounts::name;

void Accounts::init(string name, string structure)
{
	Accounts::name = name;

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

void Accounts::createAccount(string username, string passwordHash)
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
		ss << " " << Accounts::name << " ";
		ss << "(id, username, password_hash)";
		ss << " VALUES ";
		ss << "(NULL, '" << username << "', '" << passwordHash << "');";

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

void Accounts::deleteAccount(long long accountID)
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
		ss << " " << Accounts::name << " ";
		ss << "WHERE id = '" << accountID << "';";

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

long long Accounts::getAccountID(string username)
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
		ss << "SELECT id FROM";
		ss << " " << Accounts::name << " ";
		ss << "WHERE username = :1;";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = username.c_str();
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("id").asNumeric();

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

string Accounts::getUsername(long long accountID)
{
	SAConnection con;
	SACommand cmd;

	string r = "";

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT username FROM";
		ss << " " << Accounts::name << " ";
		ss << "WHERE id = '" << accountID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("username").asString();

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

string Accounts::getPasswordHash(long long accountID)
{
	SAConnection con;
	SACommand cmd;

	string r = "";

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT password_hash FROM";
		ss << " " << Accounts::name << " ";
		ss << "WHERE id = '" << accountID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("password_hash").asString();

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

short Accounts::getLoginTries(long long accountID)
{
	SAConnection con;
	SACommand cmd;

	short r = -1;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT login_tries FROM";
		ss << " " << Accounts::name << " ";
		ss << "WHERE id = '" << accountID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("login_tries").asShort();

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

long long Accounts::getFrontCharacter(long long accountID)
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
		ss << "SELECT front_character FROM";
		ss << " " << Accounts::name << " ";
		ss << "WHERE id = '" << accountID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("front_character").asNumeric();

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

bool Accounts::isLocked(long long accountID)
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
		ss << "SELECT locked_timestamp FROM";
		ss << " " << Accounts::name << " ";
		ss << "WHERE id = '" << accountID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
			r = cmd.Field("locked_timestamp").asNumeric();

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

	if (r != -1)
	{
		auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		long long diff = ms.count() - (r + TimeUtils::hoursToMilliseconds(3));

		if (diff < 0)
			return true;
	}

	return false;
}

void Accounts::setPasswordHash(string passwordHash, long long accountID)
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
		ss << " " << Accounts::name << " ";
		ss << "SET password_hash = :1 ";
		ss << "WHERE id = '" << accountID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = passwordHash.c_str();
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

void Accounts::setLoginTries(short loginTries, long long accountID)
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
		ss << " " << Accounts::name << " ";
		ss << "SET login_tries = '" << loginTries << "' ";
		ss << "WHERE id = '" << accountID << "';";

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

void Accounts::setFrontCharacter(long long frontCharID, long long accountID)
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
		ss << " " << Accounts::name << " ";
		ss << "SET front_character = '" << frontCharID << "' ";
		ss << "WHERE id = '" << accountID << "';";

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

void Accounts::updateLockedTimestamp(long long accountID)
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
		ss << " " << Accounts::name << " ";
		ss << "SET locked_timestamp = '" << ms.count() << "' ";
		ss << "WHERE id = '" << accountID << "';";

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

void Accounts::setGMLevel(long gmLevel, long long accountID)
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
		ss << " " << Accounts::name << " ";
		ss << "SET gm_level = '" << gmLevel << "' ";
		ss << "WHERE id = '" << accountID << "';";

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

long Accounts::getGMLevel(long long accountID)
{
	SAConnection con;
	SACommand cmd;

	long r = -1;

	if (accountID != -1) {
		try
		{
			con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
				Config::getMySQLUsername().c_str(),
				Config::getMySQLPassword().c_str(),
				SA_MySQL_Client);

			stringstream ss;
			ss << "SELECT gm_level FROM";
			ss << " " << Accounts::name << " ";
			ss << "WHERE id = '" << accountID << "';";

			cmd.setConnection(&con);
			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			if (cmd.FetchFirst())
				r = cmd.Field("gm_level").asLong();


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
	else {
		r = 9;
	}

	return r;
}
