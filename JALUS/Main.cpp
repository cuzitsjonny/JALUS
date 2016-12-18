#include "Common.h"
#include "PingTool.h"
#include "Server.h"
#include "Logger.h"
#include "Config.h"
#include "ServerLoop.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 1)
	{
		ServerRole serverRole = ServerRole::INVALID_SERVER_ROLE;

		if (iequals(argv[1], "AUTH") || iequals(argv[1], "AUTHENTICATION"))
			serverRole = ServerRole::SERVER_ROLE_AUTHENTICATION;
		else if (iequals(argv[1], "CHARS") || iequals(argv[1], "CHARACTERS"))
			serverRole = ServerRole::SERVER_ROLE_CHARACTERS;
		else
		{
			ZoneID zoneID = ZoneIDs::fromString(argv[1]);

			if (zoneID != ZoneID::INVALID_ZONE_ID)
				serverRole = ServerRoles::fromZoneID(zoneID);
		}

		if (serverRole != ServerRole::INVALID_SERVER_ROLE)
		{
			Server::init(serverRole);
			
			bool success = Server::start();

			if (success)
			{
				Logger::info("Server started! (Port: " + to_string(Config::getPort()) + ") (ServerRole: " + ServerRoles::toString(serverRole) + ")");
				ServerLoop::start();

				//for (int i = 0; i < 14098; i++)
				//{
				//	string file = ".\\test\\world_2 (" + to_string(i + 1) + ").bin";
				//	string newName = "";

				//	if (!fexists(".\\test2\\"))
				//		CreateDirectory(".\\test2\\", NULL);

				//	auto v = readAllBytes(file.c_str());
				//	BitStream* bs = new BitStream();

				//	for (int k = 0; k < v.size(); k++)
				//	{
				//		bs->Write(v.at(k));
				//	}

				//	unsigned char id;
				//	bs->Read(id);

				//	/*if (id == ID_REPLICA_MANAGER_CONSTRUCTION)
				//	{
				//		newName = "REPLICA_" + to_string(i) + ".bin";
				//	}
				//	else*/ if (id == ID_USER_PACKET_ENUM)
				//	{
				//		unsigned short rct;
				//		bs->Read(rct);
				//		unsigned long packetID;
				//		bs->Read(packetID);
				//		bs->IgnoreBytes(1);

				//		/*if (packetID == 0x0c)
				//		{
				//			long long objectID;
				//			bs->Read(objectID);
				//			unsigned short gmID;
				//			bs->Read(gmID);

				//			newName = "GM_" + to_string(gmID) + "_" + to_string(objectID) + ".bin";
				//		}*/

				//		newName = to_string(rct) + "-" + to_string(packetID);
				//		if (packetID == 0x0c)
				//		{
				//			long long objectID;
				//			bs->Read(objectID);
				//			unsigned short gmID;
				//			bs->Read(gmID);

				//			newName += ("-" + to_string(gmID) + "-" + to_string(objectID));
				//		}

				//		newName += ".bin";
				//	}

				//	if (newName != "")
				//	{
				//		saveToFile(bs, (".\\test2\\" + newName).c_str());
				//	}
				//}
			}
			else
				Logger::error("Port " + to_string(Config::getPort()) + " seems to be occupied!");
		}
		else
			cout << argv[1] << " is not a valid startup parameter!" << endl;
	}

	return 0;
}