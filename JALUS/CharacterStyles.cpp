#include "CharacterStyles.h"
#include "CDClient.h"
#include "Config.h"
#include "Logger.h"
#include "Server.h"

string CharacterStyles::name;

void CharacterStyles::init(string name, string structure)
{
	CharacterStyles::name = name;

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

long CharacterStyles::getLOTFromStyle(long shirtColor, long shirtStyle) // CDCLIENT
{
	long lot = -1;

	if (CDClient::getBrickColorCharacterValid(shirtColor)) {
		string shirtName = CDClient::getBrickColorName(shirtColor);
		lot = CDClient::getTemplateFromName(toTitle(shirtName) + " Shirt " + to_string(shirtStyle));
	}

	return lot;

}

long CharacterStyles::getLOTFromStyle(long pantsColor) // CDCLIENT
{
	long lot = -1;

	if (CDClient::getBrickColorCharacterValid(pantsColor)) {
		string pantsName = CDClient::getBrickColorName(pantsColor);
		lot = CDClient::getTemplateFromName(toTitle(pantsName) + " Pants");
	}

	return lot;

}

void CharacterStyles::saveCharacterStyle(CharacterStyle style, long long charID)
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
		ss << " " << CharacterStyles::name << " ";
		ss << "(id, shirt_color, shirt_style, pants_color, hair_style, hair_color, lh, rh, eyebrows, eyes, mouth)";
		ss << " VALUES ";
		ss << "('" << charID << "', '" << style.shirtColor << "', '" << style.shirtStyle << "', '" << style.pantsColor << "', '" << style.hairStyle << "', '" << style.hairColor;
		ss << "', '" << style.lh << "', '" << style.rh << "', '" << style.eyebrows << "', '" << style.eyes << "', '" << style.mouth << "');";

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

void CharacterStyles::deleteCharacterStyle(long long charID)
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
		ss << " " << CharacterStyles::name << " ";
		ss << "WHERE id = '" << charID << "';";

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

CharacterStyle CharacterStyles::getCharacterStyle(long long charID)
{
	SAConnection con;
	SACommand cmd;

	CharacterStyle r = CharacterStyle();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT * FROM";
		ss << " " << CharacterStyles::name << " ";
		ss << "WHERE id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
		{
			r.shirtColor = cmd.Field("shirt_color").asLong();
			r.shirtStyle = cmd.Field("shirt_style").asLong();
			r.pantsColor = cmd.Field("pants_color").asLong();
			r.hairStyle = cmd.Field("hair_style").asLong();
			r.hairColor = cmd.Field("hair_color").asLong();
			r.lh = cmd.Field("lh").asLong();
			r.lh = cmd.Field("rh").asLong();
			r.eyebrows = cmd.Field("eyebrows").asLong();
			r.eyes = cmd.Field("eyes").asLong();
			r.mouth = cmd.Field("mouth").asLong();
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
