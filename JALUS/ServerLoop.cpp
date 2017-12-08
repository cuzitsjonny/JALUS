#include "ServerLoop.h"
#include "Logger.h"
#include "Commands.h"
#include "General.h"
#include "AuthenticationInstance.h"
#include "CharactersInstance.h"
#include "Sessions.h"
#include "WorldInstance.h"
#include "ReplicaObject.h"
#include "ObjectsManager.h"
#include "Scheduler.h"
#include "GameMessages.h"
#include "Helpers.h"
#include "ValueStorage.h"
#include "Characters.h"
#include <RakSleep.h>

bool ServerLoop::run;
vector<char> ServerLoop::input;

void ServerLoop::parseUserInput()
{
	if (_kbhit())
	{
		char c = _getch();

		switch (c)
		{

		case 8:
		{
			if (ServerLoop::input.size() > 0)
			{
				ServerLoop::input.pop_back();
				cout << "\b \b";
			}
			break;
		}

		case 13:
		{
			if (ServerLoop::input.size() > 0)
			{
				vector<string> rawCMD = split(string(input.begin(), input.end()), ' ');

				string cmd = "";
				string curArgument = "";
				vector<string> args;

				bool opened = false;
				for (int i = 0; i < rawCMD.size(); i++)
				{
					for (int k = 0; k < rawCMD.at(i).length(); k++)
					{
						char v = rawCMD.at(i)[k];

						if (i == 0)
							cmd += v;
						else
						{
							if ((k == 0 || k == rawCMD.at(i).length() - 1) && v == '"')
							{
								if (opened)
								{
									args.push_back(curArgument);
									curArgument = "";
									opened = false;
								}
								else
									opened = true;
							}
							else
								curArgument += v;
						}
					}

					if (!opened)
					{
						if (curArgument != "")
						{
							args.push_back(curArgument);
							curArgument = "";
						}
					}
					else
						curArgument += " ";
				}

				cout << endl;

				Commands::performCommand(CommandSender(-1), cmd, args);

				args.clear();
				rawCMD.clear();
				ServerLoop::input.clear();

				Logger::setConsoleOutputMuted(false);
			}
			else
			{
				cout << "\b\b \b";
				Logger::setConsoleOutputMuted(false);
			}
			break;
		}

		case 27:
		{
			if (Logger::isConsoleOutputMuted())
			{
				for (int i = 0; i < ServerLoop::input.size(); i++)
				{
					cout << "\b \b";
				}

				cout << "\b\b \b";

				ServerLoop::input.clear();
				Logger::setConsoleOutputMuted(false);
			}
			break;
		}

		default:
		{
			if (/*(48 <= c && c <= 57) || (65 <= c && c <= 90) || (97 <= c && c <= 122) || c == 32 || c == ',' || c == '.' || c == '-' || c == '"' || c == '!' || c == '?'*/ true)
			{
				if (!Logger::isConsoleOutputMuted())
				{
					cout << "> ";
					Logger::setConsoleOutputMuted(true);
				}

				ServerLoop::input.push_back(c);
				cout << c;
			}
			break;
		}

		}
	}
}

void ServerLoop::start()
{
	Packet* packet;
	BitStream* data;
	SystemAddress clientAddress;

	Scheduler::runTaskTimer(100, 100, parseUserInput);

	if (Server::isWorldInstance())
	{
		Scheduler::runTaskTimer(1000, 1000, Helpers::broadcastJonnysDumbEffects);
	}

	ServerLoop::run = true;
	while (ServerLoop::run)
	{
		RakSleep(30);
		Scheduler::tick();

		packet = Server::getPeerInterface()->Receive();
		if (packet == NULL)
			continue;

		data = new BitStream(packet->data, packet->length, false);
		clientAddress = packet->systemAddress;

		if ((data->GetNumberOfUnreadBits() / 8) >= 1)
		{
			unsigned char rakNetPacketID;
			data->Read(rakNetPacketID);

			switch (rakNetPacketID)
			{

			case ID_NEW_INCOMING_CONNECTION:
			{
				Logger::info("Client connected! (Address: " + string(clientAddress.ToString()) + ")");
				break;
			}

			case ID_CONNECTION_LOST:
			case ID_DISCONNECTION_NOTIFICATION:
			{
				if (Server::isCharactersInstance() || Server::isWorldInstance())
				{
					if (Server::isWorldInstance())
					{
						ReplicaObject* replica = ObjectsManager::getObjectBySystemAddress(clientAddress);

						if (replica != nullptr)
						{
							Session* session = Sessions::getSession(clientAddress);
							if (!Sessions::getSession(clientAddress)->gotRedirected)
							{
								ControllablePhysicsIndex* index = replica->controllablePhysicsIndex;

								Location loc = Location();
								loc.zoneID = ServerRoles::toZoneID(Server::getServerRole());
								loc.mapClone = 0;
								loc.position.x = index->pos_x;
								loc.position.y = index->pos_y;
								loc.position.z = index->pos_z;
								loc.rotation.x = index->rot_x;
								loc.rotation.y = index->rot_y;
								loc.rotation.z = index->rot_z;
								loc.rotation.w = index->rot_w;
								Locations::saveLocation(loc, replica->objectID);

								Characters::setHealth(ValueStorage::getValueInMemory(session->charID, "health"), session->charID);
								Characters::setArmor(ValueStorage::getValueInMemory(session->charID, "armor"), session->charID);
								Characters::setImagination(ValueStorage::getValueInMemory(session->charID, "imagination"), session->charID);

								ValueStorage::removeValueFromMemory(session->charID, "health");
								ValueStorage::removeValueFromMemory(session->charID, "armor");
								ValueStorage::removeValueFromMemory(session->charID, "imagination");

							}
						}

						ObjectsManager::removePlayer(clientAddress);
					}
					
					Sessions::removeSession(clientAddress);
				}
				Logger::info("Client disconnected! (Address: " + string(clientAddress.ToString()) + ")");
				break;
			}

			case ID_USER_PACKET_ENUM:
			{
				if ((data->GetNumberOfUnreadBits() / 8) >= 7)
				{
					unsigned short remoteConnectionType;
					unsigned long packetID;
					
					data->Read(remoteConnectionType);
					data->Read(packetID);
					data->IgnoreBytes(1);

					ServerLoop::routePacket(remoteConnectionType, packetID, data, clientAddress);
				}
				break;
			}

			default:
			{
				Logger::info("Server received a packet with unknown MessageID! (MessageID: " + to_string((int)rakNetPacketID) + ") (Address: " + string(clientAddress.ToString()) + ")");
				break;
			}

			}
		}

		Server::getPeerInterface()->DeallocatePacket(packet);
		delete data;
	}
}

void ServerLoop::stop()
{
	ServerLoop::run = false;
}

void ServerLoop::routePacket(unsigned short remoteConnectionType, unsigned long packetID, BitStream* data, SystemAddress clientAddress)
{
	switch (remoteConnectionType)
	{

	case RCT_GENERAL:
	{
		switch (packetID)
		{

		case GENERAL_PACKET_VERSION_CONFIRM:
		{
			bool success = General::performHandshake(data, clientAddress);
			Logger::info("Client performed handshake! (Success: " + to_string(success) + ") (Address: " + string(clientAddress.ToString()) + ")");
			break;
		}

		default:
		{
			Logger::info("Server received a packet with unknown PacketID! (PacketID: " + to_string(packetID) + ") (RemoteConnectionType: " + to_string(remoteConnectionType) + ") (Address: " + string(clientAddress.ToString()) + ")");
			break;
		}

		}
		break;
	}

	case RCT_CLIENT_TO_AUTH:
	{
		switch (packetID)
		{

		case CLIENT_AUTH_PACKET_LOGIN_REQUEST:
		{
			if (Server::isAuthenticationInstance())
				AuthenticationInstance::performAuthentication(data, clientAddress);
			else
				Logger::info("Server received an invalid packet! (PacketID: " + to_string(packetID) + ") (RemoteConnectionType: " + to_string(remoteConnectionType) + ") (Address: " + string(clientAddress.ToString()) + ")");
			break;
		}

		default:
		{
			Logger::info("Server received a packet with unknown PacketID! (PacketID: " + to_string(packetID) + ") (RemoteConnectionType: " + to_string(remoteConnectionType) + ") (Address: " + string(clientAddress.ToString()) + ")");
			break;
		}

		}
		break;
	}

	case RCT_CHAT:
	{
		if (Server::isWorldInstance())
		{
			WorldInstance::processChatPacket(data, clientAddress, (ChatPacketID)packetID);
		}
		else
			Logger::info("Server received an invalid packet! (PacketID: " + to_string(packetID) + ") (RemoteConnectionType: " + to_string(remoteConnectionType) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	case RCT_CLIENT_TO_WORLD:
	{
		if (Server::isCharactersInstance())
		{
			CharactersInstance::processPacket(data, clientAddress, (ClientToWorldPacketID)packetID);
		}
		else if (Server::isWorldInstance())
		{
			WorldInstance::processWorldPacket(data, clientAddress, (ClientToWorldPacketID)packetID);
		}
		else
			Logger::info("Server received an invalid packet! (PacketID: " + to_string(packetID) + ") (RemoteConnectionType: " + to_string(remoteConnectionType) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	default:
	{
		Logger::info("Server received a packet with unknown RemoteConnectionType! (RemoteConnectionType: " + to_string(remoteConnectionType) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	}
}
