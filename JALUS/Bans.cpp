#include "Bans.h"
#include "Config.h"
#include "TimeUtils.h"
#include "Logger.h"
#include "Server.h"

string Bans::name;

void Bans::init(string name, string structure)
{
	Bans::name = name;

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

void Bans::ban(string gotBanned, long long banned, bool isIPBan, string reason, long long howLong)
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
		ss << "REPLACE INTO";
		ss << " " << Bans::name << " ";
		ss << "(got_banned, banned, is_ip_ban, reason, banned_timestamp, how_long)";
		ss << " VALUES ";
		ss << "(:1, '" << banned << "', '" << isIPBan << "', :2, '" << ms.count() << "', '" << howLong << "');";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = gotBanned.c_str();
		cmd.Param(2).setAsString() = reason.c_str();
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

void Bans::unban(string gotBanned, bool isIPBan)
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
		ss << " " << Bans::name << " ";
		ss << "WHERE got_banned = :1 AND is_ip_ban = '" << isIPBan << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = gotBanned.c_str();
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

bool Bans::isBanned(string key, bool isIPBan)
{
	SAConnection con;
	SACommand cmd;

	long long bannedTimestamp = -1;
	long long howLong = -1;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT banned_timestamp, how_long FROM";
		ss << " " << Bans::name << " ";
		ss << "WHERE got_banned = :1;";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = key.c_str();
		cmd.Execute();

		if (cmd.FetchFirst())
		{
			bannedTimestamp = cmd.Field("banned_timestamp").asNumeric();
			howLong = cmd.Field("how_long").asNumeric();
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

	if (bannedTimestamp != -1)
	{
		if (howLong != -1)
		{
			auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			long long diff = ms.count() - (bannedTimestamp + howLong);

			return (diff < 0);
		}
		else
			return true;
	}
	else
		return false;
}

BanInfo Bans::getInfo(string gotBanned, bool isIPBan)
{
	SAConnection con;
	SACommand cmd;

	BanInfo r = BanInfo();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT banned, reason, banned_timestamp, how_long FROM";
		ss << " " << Bans::name << " ";
		ss << "WHERE got_banned = :1 AND is_ip_ban = '" << isIPBan << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Param(1).setAsString() = gotBanned.c_str();
		cmd.Execute();

		if (cmd.FetchFirst())
		{
			r.gotBanned = gotBanned;
			r.banned = cmd.Field("banned").asNumeric();
			r.isIPBan = isIPBan;
			r.reason = cmd.Field("reason").asString();
			r.bannedTimestamp = cmd.Field("banned_timestamp").asNumeric();
			r.howLong = cmd.Field("how_long").asNumeric();
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

string Bans::toTimeString(long long time)
{
	string r = "";

	if (time != -1) {
		bool startedYet = false;

		long long l = TimeUtils::millisecondsToDays(time);
		if (l > 0) {
			r += to_string(l) + " ";
			r += ((l > 1) ? "days" : "day");
			startedYet = true;
			time -= TimeUtils::daysToMilliseconds(l);
		}

		l = TimeUtils::millisecondsToHours(time);
		if (l > 0) {
			if (startedYet)
				r += ", ";

			r += to_string(l) + " ";
			r += ((l > 1) ? "hours" : "hour");
			startedYet = true;
			time -= TimeUtils::hoursToMilliseconds(l);
		}

		l = TimeUtils::millisecondsToMinutes(time);
		if (l > 0) {
			if (startedYet)
				r += ", ";

			r += to_string(l) + " ";
			r += ((l > 1) ? "minutes" : "minute");
			startedYet = true;
			time -= TimeUtils::minutesToMilliseconds(l);
		}

		l = TimeUtils::millisecondsToSeconds(time);
		if (l > 0) {
			if (startedYet)
				r += ", ";

			r += to_string(l) + " ";
			r += ((l > 1) ? "seconds" : "second");
			startedYet = true;
			time -= TimeUtils::secondsToMilliseconds(l);
		}

		if (time > 0) {
			if (startedYet)
				r += ", ";

			r += to_string(time) + " ";
			r += ((time > 1) ? "milliseconds" : "millisecond");
		}
	}
	else
		r += "FOREVER";

	return r;
}
