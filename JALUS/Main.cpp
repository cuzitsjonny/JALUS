#include "Common.h"
#include "PingTool.h"
#include "Server.h"
#include "Logger.h"
#include "Config.h"
#include "ServerLoop.h"
#include "TimeUtils.h"
#include "resource.h"
//#include "JALUS.rc"

int _tmain(int argc, _TCHAR* argv[])
{
	/*HWND hWnd = GetConsoleWindow();
	HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(JALUS_LOGO));
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);*/

	if (argc > 1)
	{
		ServerRole serverRole = ServerRole::INVALID_SERVER_ROLE;

		if (iequals(argv[1], "AUTH") || iequals(argv[1], "AUTHENTICATION")) {
			serverRole = ServerRole::SERVER_ROLE_AUTHENTICATION;
			SetConsoleTitle(ServerRoles::toString(serverRole).c_str());
		}
		else if (iequals(argv[1], "CHARS") || iequals(argv[1], "CHARACTERS")) {
			serverRole = ServerRole::SERVER_ROLE_CHARACTERS;
			SetConsoleTitle(ServerRoles::toString(serverRole).c_str());
		}
		else
		{
			ZoneID zoneID = ZoneIDs::fromString(argv[1]);

			if (zoneID != ZoneID::INVALID_ZONE_ID) {
				serverRole = ServerRoles::fromZoneID(zoneID);
				SetConsoleTitle(string(ZoneIDs::toPrintString(ServerRoles::toZoneID(serverRole))).c_str());
			}
		}

		if (serverRole != ServerRole::INVALID_SERVER_ROLE)
		{
			Server::init(serverRole);

			bool success = Server::start();
			if (success)
			{
				Logger::info("Server started! (Port: " + to_string(Config::getPort()) + ") (ServerRole: " + ServerRoles::toString(serverRole) + ")");

				ServerLoop::start();

				Server::shutdown();
			}
			else
				Logger::error("Port " + to_string(Config::getPort()) + " seems to be occupied!");
		}
		else
			cout << argv[1] << " is not a valid startup parameter!" << endl;
	}

	return 0;
}