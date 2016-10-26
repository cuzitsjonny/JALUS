#include "Config.h"
#include "Server.h"
#include "IniFile.h"

string Config::filePath;

void Config::init()
{
	Config::filePath = ".\\configs\\" + ServerRoles::toString(Server::getServerRole()) + ".ini";

	if (!fexists(Config::filePath.c_str()))
	{
		CreateDirectory(".\\configs", NULL);
		CIniFile::Create(Config::filePath);

		CIniFile::SetValue("MySQLUsername", "root", "Settings", Config::filePath);
		CIniFile::SetValue("MySQLPassword", "", "Settings", Config::filePath);
		CIniFile::SetValue("MySQLDatabase", "db", "Settings", Config::filePath);
		CIniFile::SetValue("MySQLHost", "localhost", "Settings", Config::filePath);

		if (Server::isAuthenticationInstance())
		{
			CIniFile::SetValue("Slots", to_string(20), "Settings", Config::filePath);
			CIniFile::SetValue("Port", to_string(1001), "Settings", Config::filePath);
			CIniFile::SetValue("CharactersInstanceAddress", "localhost", "Settings", Config::filePath);
			CIniFile::SetValue("CharactersInstancePort", to_string(1101), "Settings", Config::filePath);
			CIniFile::SetValue("IgnoreDomains", to_string(1), "Settings", Config::filePath);
		}

		if (Server::isCharactersInstance())
		{
			CIniFile::SetValue("Slots", to_string(80), "Settings", Config::filePath);
			CIniFile::SetValue("Port", to_string(1101), "Settings", Config::filePath);
			CIniFile::SetValue("CDClientPath", ".\\cdclient.sqlite", "Settings", Config::filePath);

			for (int i = 2, port = 1201; i < ServerRole::SERVER_ROLE_COUNT; i++, port++)
			{
				CIniFile::SetValue(ServerRoles::toString((ServerRole)i) + "Address", "localhost", "Settings", Config::filePath);
				CIniFile::SetValue(ServerRoles::toString((ServerRole)i) + "Port", to_string(port), "Settings", Config::filePath);
			}
		}

		if (Server::isWorldInstance())
		{
			CIniFile::SetValue("Slots", to_string(40), "Settings", filePath);
			CIniFile::SetValue("Port", to_string((1201 - 2) + Server::getServerRole()), "Settings", Config::filePath);
			CIniFile::SetValue("CDClientPath", ".\\cdclient.sqlite", "Settings", Config::filePath);
			CIniFile::SetValue("CharactersInstanceAddress", "localhost", "Settings", filePath);
			CIniFile::SetValue("CharactersInstancePort", to_string(1101), "Settings", filePath);
		}
	}
}

unsigned short Config::getSlots()
{
	return stol(CIniFile::GetValue("Slots", "Settings", Config::filePath));
}

unsigned short Config::getPort()
{
	return stol(CIniFile::GetValue("Port", "Settings", Config::filePath));
}

string Config::getMySQLUsername()
{
	return CIniFile::GetValue("MySQLUsername", "Settings", Config::filePath);
}

string Config::getMySQLPassword()
{
	return CIniFile::GetValue("MySQLPassword", "Settings", Config::filePath);
}

string Config::getMySQLDatabase()
{
	return CIniFile::GetValue("MySQLDatabase", "Settings", Config::filePath);
}

string Config::getMySQLHost()
{
	return CIniFile::GetValue("MySQLHost", "Settings", Config::filePath);
}

string Config::getCharactersInstanceAddress()
{
	return CIniFile::GetValue("CharactersInstanceAddress", "Settings", Config::filePath);
}

unsigned short Config::getCharactersInstancePort()
{
	return stol(CIniFile::GetValue("CharactersInstancePort", "Settings", Config::filePath));
}

string Config::getWorldInstanceAddress(ZoneID zoneID)
{
	return CIniFile::GetValue("WorldInstance" + to_string(zoneID) + "Address", "Settings", Config::filePath);
}

unsigned short Config::getWorldInstancePort(ZoneID zoneID)
{
	return stol(CIniFile::GetValue("WorldInstance" + to_string(zoneID) + "Port", "Settings", Config::filePath));
}

string Config::getDomainInstanceAddress(string domain)
{
	return CIniFile::GetValue("DomainInstance" + domain + "Address", "Settings", Config::filePath);
}

unsigned short Config::getDomainInstancePort(string domain)
{
	string v = CIniFile::GetValue("DomainInstance" + domain + "Port", "Settings", Config::filePath);

	if (v.length() > 0)
		return stol(v);
	else
		return USHRT_MAX;
}

string Config::getCDClientPath()
{
	return CIniFile::GetValue("CDClientPath", "Settings", Config::filePath);
}

bool Config::getIgnoreDomains()
{
	return (stol(CIniFile::GetValue("IgnoreDomains", "Settings", Config::filePath)) > 0);
}
