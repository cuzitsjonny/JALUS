#include "WorldInstance.h"
#include "General.h"
#include "Logger.h"
#include "Sessions.h"
#include "PacketUtils.h"
#include "Locations.h"
#include "CDClient.h"
#include "LDF.h"
#include "Characters.h"
#include "InventoryItems.h"
#include "ServerLoop.h"
#include "ReplicaObject.h"
#include "GameMessages.h"

void WorldInstance::processWorldPacket(BitStream* data, SystemAddress clientAddress, ClientToWorldPacketID packetID)
{
	switch (packetID)
	{

	case CLIENT_WORLD_VALIDATION:
	{
		bool success = General::performValidation(data, clientAddress);
		Logger::info("Client performed validation! (Success: " + to_string(success) + ") (Address: " + string(clientAddress.ToString()) + ")");

		if (success)
		{
			WorldInstance::sendWorldInfo(clientAddress);
			Logger::info("Client received world info! (Address: " + string(clientAddress.ToString()) + ")");
		}
		break;
	}

	case CLIENT_WORLD_LEVEL_LOAD_COMPLETE:
	{
		WorldInstance::sendCharacterData(clientAddress);
		Logger::info("Client received character data! (Address: " + string(clientAddress.ToString()) + ")");
		WorldInstance::sendServerState(clientAddress);
		Logger::info("Client received server state! (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	case CLIENT_WORLD_ROUTE_PACKET:
	{
		unsigned long length;
		unsigned short remoteConnectionType = RemoteConnectionType::RCT_CLIENT_TO_WORLD;
		unsigned long rPacketID = packetID;

		while (remoteConnectionType == RemoteConnectionType::RCT_CLIENT_TO_WORLD && rPacketID == ClientToWorldPacketID::CLIENT_WORLD_ROUTE_PACKET)
		{
			data->Read(length);
			data->Read(remoteConnectionType);
			data->Read(rPacketID);
			data->IgnoreBytes(1);
		}

		ServerLoop::routePacket(remoteConnectionType, rPacketID, data, clientAddress);
		break;
	}

	case CLIENT_WORLD_GAME_MSG:
	{
		GameMessages::processGameMessage(data, clientAddress);
		break;
	}

	default:
	{
		Logger::info("Server received a packet with unknown PacketID! (PacketID: " + to_string(packetID) + ") (RemoteConnectionType: " + to_string(RemoteConnectionType::RCT_CLIENT_TO_WORLD) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	}
}

void WorldInstance::processChatPacket(BitStream* data, SystemAddress clientAddress, ChatPacketID packetID)
{
	switch (packetID)
	{

	default:
	{
		Logger::info("Server received a packet with unknown PacketID! (PacketID: " + to_string(packetID) + ") (RemoteConnectionType: " + to_string(RemoteConnectionType::RCT_CHAT) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	}
}

void WorldInstance::sendWorldInfo(SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		BitStream* packet = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_LOAD_STATIC_ZONE);

		Location loc = Locations::getLocation(session->charID);
		packet->Write(loc.zoneID);
		packet->Write((unsigned short)0);
		packet->Write(loc.mapClone);

		vector<unsigned char> checksum = ZoneIDs::getChecksum(loc.zoneID);
		for (int i = 0; i < checksum.size(); i++)
		{
			packet->Write(checksum.at(i));
		}

		packet->Write((unsigned short)0);

		packet->Write(loc.position.x);
		packet->Write(loc.position.y);
		packet->Write(loc.position.z);

		if (CDClient::isActivityZone(loc.zoneID))
			packet->Write((unsigned long)4);
		else
			packet->Write((unsigned long)0);

		Server::sendPacket(packet, clientAddress);
	}
}

void WorldInstance::sendCharacterData(SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		BitStream* packet = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_CREATE_CHARACTER);

		LDF ldf;
		ldf.writeLongLong(L"accountID", session->accountID);
		ldf.writeLong(L"chatmode", 0);
		ldf.writeBoolean(L"editor_enabled", false);
		ldf.writeLong(L"editor_level", 0);
		ldf.writeBoolean(L"freetrial", false);
		ldf.writeBoolean(L"legoclub", true);
		ldf.writeWString(L"name", to_wstring(Characters::getName(session->charID)));
		ldf.writeLongLong(L"reputation", Characters::getReputation(session->charID));
		ldf.writeBoolean(L"requiresrename", Characters::isUnapprovedNameRejected(session->charID));
		ldf.writeObjectID(L"objid", session->charID);
		ldf.writeLong(L"template", 1);

		stringstream xml;
		xml << "<obj v=\"1\">";
		xml << "<buff/>";
		xml << "<skil/>";
		xml << "<mf/>";
		xml << "<flag/>";
		xml << "<pet/>";
		xml << "<mnt/>";
		xml << "<dest/>";
		xml << "<char acct=\"" << session->accountID << "\" cc=\"" << Characters::getCurrency(session->charID) << "\" gm=\"" << Characters::getGMLevel(session->charID) << "\" ft=\"1\"/>";
		xml << "<lvl l=\"" << Characters::getLevel(session->charID) << "\"/>";

		xml << "<inv>";
		xml << "<bag>";

		long inventorySize = Characters::getInventorySize(session->charID);
		xml << "<b t=\"0\" m=\"" << inventorySize << "\"/>";
		/*xml << "<b t=\"2\" m=\"" << inventorySize << "\"/>";
		xml << "<b t=\"5\" m=\"" << inventorySize << "\"/>";
		xml << "<b t=\"7\" m=\"" << inventorySize << "\"/>";*/

		xml << "</bag>";

		vector<InventoryItem> items = InventoryItems::getInventoryItems(session->charID);
		xml << "<items>";

		InventoryType current = InventoryType::INVENTORY_TYPE_DEFAULT;

		while (current != InventoryType::INVALID_INVENTORY_TYPE)
		{
			vector<InventoryItem> temp;

			for (int i = 0; i < items.size(); i++)
			{
				if (items.at(i).invType == current)
					temp.push_back(items.at(i));
			}

			if (temp.size() > 0)
			{
				xml << "<in t=\"" << current << "\">";

				for (int i = 0; i < temp.size(); i++)
				{
					xml << "<i l=\"" << temp.at(i).lot << "\" id=\"" << temp.at(i).objectID << "\" s=\"" << temp.at(i).slot << "\"";

					if (temp.at(i).count > 1)
						xml << " c=\"" << temp.at(i).count << "\"";

					if (temp.at(i).isBound)
						xml << " b=\"1\"";

					if (temp.at(i).isEquipped)
						xml << " eq=\"1\"";

					xml << "/>";
				}

				xml << "</in>";
			}

			switch (current)
			{
				
			case INVENTORY_TYPE_DEFAULT:
				current = InventoryType::INVENTORY_TYPE_BRICKS;
				break;

			case INVENTORY_TYPE_BRICKS:
				current = InventoryType::INVENTORY_TYPE_MODELS;
				break;

			case INVENTORY_TYPE_MODELS:
				current = InventoryType::INVENTORY_TYPE_BEHAVIORS;
				break;

			case INVENTORY_TYPE_BEHAVIORS:
				current = InventoryType::_INVENTORY_TYPE_PROPERTIES;
				break;

			case _INVENTORY_TYPE_PROPERTIES:
				current = InventoryType::INVENTORY_TYPE_COMMENDATIONS;
				break;

			case INVENTORY_TYPE_COMMENDATIONS:
				current = InventoryType::INVALID_INVENTORY_TYPE;
				break;

			}
		}

		xml << "</items>";
		xml << "</inv>";
		xml << "</obj>";

		BitStream* bitStream = new BitStream();
		PacketUtils::writeStringToBitStream(xml.str(), bitStream, xml.str().length());
		ldf.writeRaw(L"xmlData", bitStream);

		packet->Write((unsigned long)ldf.getSize() + 1);
		packet->Write((unsigned char)0);
		ldf.writeToBitStream(packet);

		Server::sendPacket(packet, clientAddress);
	}
}

void WorldInstance::sendServerState(SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		BitStream* packet = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_SERVER_STATES);
		packet->Write((unsigned char)1);

		Server::sendPacket(packet, clientAddress);

		BitStream* replica = new BitStream();
		replica->Write((unsigned char)ID_REPLICA_MANAGER_CONSTRUCTION);
		replica->Write(true);
		replica->Write((unsigned short)42);

		replica->Write(session->charID);
		replica->Write((unsigned long)1);

		string name = Characters::getName(session->charID);
		replica->Write((unsigned char)name.length());
		for (int i = 0; i < name.length(); i++)
		{
			replica->Write(name[i]);
		}

		replica->Write((unsigned long)0);
		replica->Write((unsigned char)0);

		// ControllablePhysics
		replica->Write(false);
		replica->Write(true);

		for (int i = 0; i < 7; i++)
		{
			replica->Write((unsigned long)0);
		}

		replica->Write(false);

		replica->Write(true);
		replica->Write((unsigned long)0);
		replica->Write(false);

		replica->Write(true);
		replica->Write(false);

		replica->Write(true);

		Location loc = Locations::getLocation(session->charID);
		replica->Write(loc.position.x);
		replica->Write(loc.position.y);
		replica->Write(loc.position.z);
		replica->Write(loc.rotation.x);
		replica->Write(loc.rotation.y);
		replica->Write(loc.rotation.z);
		replica->Write(loc.rotation.w);
		replica->Write(true);
		replica->Write(false);

		replica->Write(false);
		replica->Write(false);
		replica->Write(false);

		// Destructible
		replica->Write(false);
		replica->Write(false);

		// Stats
		replica->Write(true);

		for (int i = 0; i < 9; i++)
		{
			replica->Write((unsigned long)0);
		}

		replica->Write(true);
		replica->Write((unsigned long)4);
		replica->Write((float)4);
		replica->Write((unsigned long)0);
		replica->Write((float)0);
		replica->Write((unsigned long)0);
		replica->Write((float)0);

		replica->Write((unsigned long)0);
		replica->Write(true);
		replica->Write(false);
		replica->Write(false);

		replica->Write((float)4);
		replica->Write((float)0);
		replica->Write((float)0);

		replica->Write((unsigned long)1);
		replica->Write((unsigned long)1);

		replica->Write(false);
		replica->Write(false);
		replica->Write(false);

		replica->Write(true);
		replica->Write(false);

		// Character
		replica->Write(true);
		replica->Write(false);
		replica->Write((unsigned char)0);

		replica->Write(true);
		replica->Write((unsigned long)1);

		replica->Write(true);
		replica->Write(false);
		replica->Write(true);

		replica->Write(false);
		replica->Write(false);
		replica->Write(false);
		replica->Write(false);

		CharacterStyle style = CharacterStyles::getCharacterStyle(session->charID);
		replica->Write(style.hairColor);
		replica->Write(style.hairStyle);
		replica->Write((unsigned long)0);
		replica->Write(style.shirtColor);
		replica->Write(style.pantsColor);
		replica->Write((unsigned long)0);
		replica->Write((unsigned long)0);
		replica->Write(style.eyebrows);
		replica->Write(style.eyes);
		replica->Write(style.mouth);
		replica->Write(session->accountID);
		replica->Write((unsigned long long)0);
		replica->Write((unsigned long long)0);
		replica->Write(Characters::getUniverseScore(session->charID));
		replica->Write(false);

		for (int i = 0; i < 27; i++)
		{
			replica->Write((unsigned long long)0);
		}

		replica->Write(false);
		replica->Write(false);

		replica->Write(true);
		replica->Write(false);
		replica->Write(false);
		replica->Write((unsigned char)0);
		replica->Write(false);
		replica->Write((unsigned char)0);

		replica->Write(true);
		replica->Write((unsigned long)0);

		replica->Write(true);
		replica->Write((unsigned long long)0);
		replica->Write((unsigned char)0);
		replica->Write(false);
		replica->Write((long)-1);

		// Inventory
		replica->Write(true);
		vector<InventoryItem> items = InventoryItems::getEquippedInventoryItems(session->charID);
		replica->Write((unsigned long)items.size());

		for (int i = 0; i < items.size(); i++)
		{
			InventoryItem item = items.at(i);

			replica->Write(item.objectID);
			replica->Write(item.lot);

			replica->Write(false);

			replica->Write(true);
			replica->Write(item.count);

			replica->Write(item.slot > 0);
			if (item.slot > 0)
				replica->Write(item.slot);

			replica->Write(false);
			replica->Write(false);
			replica->Write(true);
		}

		replica->Write(true);
		replica->Write((unsigned long)0);

		// Skill
		replica->Write(false);

		// Render
		replica->Write((unsigned long)0);

		// 107
		replica->Write(true);
		replica->Write((unsigned long long)0);

		Server::sendPacket(replica, clientAddress);

		Server::sendPacket(PacketUtils::createGMBase(session->charID, 1642), clientAddress);
		Server::sendPacket(PacketUtils::createGMBase(session->charID, 509), clientAddress);
	}
}
