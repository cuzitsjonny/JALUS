#include "CharacterStyles.h"
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

long CharacterStyles::getLOTFromStyle(long shirtColor, long shirtStyle)
{
	long lot = -1;

	if (shirtStyle > 34)
	{
		switch (shirtColor)
		{

		case 0:
			lot = 5730;
			break;

		case 1:
			lot = 5736;
			break;

		case 3:
			lot = 5808;
			break;

		case 5:
			lot = 5754;
			break;

		case 6:
			lot = 5760;
			break;

		case 7:
			lot = 5766;
			break;

		case 8:
			lot = 5772;
			break;

		case 9:
			lot = 5778;
			break;

		case 10:
			lot = 5784;
			break;

		case 11:
			lot = 5802;
			break;

		case 13:
			lot = 5796;
			break;

		case 14:
			lot = 5802;
			break;

		case 15:
			lot = 5808;
			break;

		case 16:
			lot = 5814;
			break;

		case 84:
			lot = 5820;
			break;

		case 96:
			lot = 5826;
			break;

		}

		lot += (shirtStyle - 35);
	}
	else
	{
		switch (shirtColor)
		{

		case 0:
			lot = 4049;
			break;

		case 1:
			lot = 4083;
			break;

		case 3:
			lot = 4151;
			break;

		case 5:
			lot = 4185;
			break;

		case 6:
			lot = 4219;
			break;

		case 7:
			lot = 4263;
			break;

		case 8:
			lot = 4287;
			break;

		case 9:
			lot = 4321;
			break;

		case 10:
			lot = 4355;
			break;

		case 11:
			lot = 4389;
			break;

		case 13:
			lot = 4423;
			break;

		case 14:
			lot = 4457;
			break;

		case 15:
			lot = 4491;
			break;

		case 16:
			lot = 4525;
			break;

		case 84:
			lot = 4559;
			break;

		case 96:
			lot = 4593;
			break;

		}

		lot += (shirtStyle - 1);
	}

	return lot;
}

long CharacterStyles::getLOTFromStyle(long pantsColor)
{
	long lot = -1;

	switch (pantsColor)
	{

	case 0:
		lot = 2508;
		break;

	case 1:
		lot = 2519;
		break;

	case 3:
		lot = 2515;
		break;

	case 5:
		lot = 2509;
		break;

	case 6:
		lot = 2524;
		break;

	case 7:
		lot = 2521;
		break;

	case 8:
		lot = 2522;
		break;

	case 9:
		lot = 2526;
		break;

	case 10:
		lot = 2523;
		break;

	case 11:
		lot = 2513;
		break;

	case 13:
		lot = 2527;
		break;

	case 14:
		lot = 2517;
		break;

	case 15:
		lot = 2516;
		break;

	case 16:
		lot = 2511;
		break;

	case 84:
		lot = 2520;
		break;

	case 96:
		lot = 2514;
		break;

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
