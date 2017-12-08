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
#include "ObjectsManager.h"
#include "LUZCache.h"
#include "Missions.h"
#include "Chat.h"
#include "Config.h"
#include "TransitionInfos.h"
#include "CharactersInstance.h"
#include "Helpers.h"
#include "ValueStorage.h"

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

	case CLIENT_WORLD_POSITION_UPDATE:
	{
		WorldInstance::broadcastPositionUpdate(data, clientAddress);
		break;
	}

	case CLIENT_WORLD_STRING_CHECK:
	{
		Session* session = Sessions::getSession(clientAddress);

		if (session != nullptr)
		{
			data->IgnoreBytes(1);
			unsigned char requestID;
			data->Read(requestID);

			BitStream* packet = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_CHAT_MODERATION_STRING);

			packet->Write((unsigned char)1);
			packet->Write((unsigned short)0);
			packet->Write(requestID);

			Server::sendPacket(packet, clientAddress);
		}
		break;
	}

	case CLIENT_WORLD_GENERAL_CHAT_MESSAGE:
	{
		string message = WorldInstance::broadcastChatMessage(data, clientAddress);
		Logger::info("Client sent chat message! (Message: '" + message + "') (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	case CLIENT_WORLD_CHARACTER_LIST_REQUEST:
	{
		Session* session = Sessions::getSession(clientAddress);

		if (session != nullptr)
		{
			string nextInstanceAddress = Config::getCharactersInstanceAddress();
			unsigned short nextInstancePort = Config::getCharactersInstancePort();

			TransitionInfos::insertTransitionInfo(clientAddress, session->accountID, session->charID, session->transitionKey);
			CharactersInstance::sendCharacterList(clientAddress);
			General::redirectToServer(clientAddress, nextInstanceAddress, nextInstancePort, false);
		}
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
	//[53-05-00-02] (world info?)
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		BitStream* packet = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_LOAD_STATIC_ZONE);

		Location loc = Locations::getLocation(session->charID);

		//long zoneTest = 0;

		if (loc.zoneID == ZoneID::ZONE_ID_VE_MOVIE_SCENE)
		{ 
			loc.zoneID = ZoneID::ZONE_ID_VENTURE_EXPLORER;
			Locations::saveLocation(loc, session->charID);
			packet->Write(ZoneID::ZONE_ID_VENTURE_EXPLORER);
		}
		else
		{
			//Logger::info("Loading world: " + std::to_string(loc.zoneID));
			packet->Write(loc.zoneID);
		}

		//packet->Write(loc.zoneID);
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


		//Logger::info("This is a test log. Player location coords:" + to_string((float)loc.position.x) + ", " + to_string((float)loc.position.y) + ", " + to_string((float)loc.position.z) + " (Address: " + string(clientAddress.ToString()) + ")");

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
		ldf.writeLong(L"chatmode", 1);
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

		xml << "<mis>";

		vector<MissionInfo> misDone = Missions::getAllDoneMissions(session->charID);
		if (misDone.size() > 0)
		{
			xml << "<done>";

			for (int i = 0; i < misDone.size(); i++)
			{
				MissionInfo info = misDone.at(i);

				xml << "<m id=\"" << info.missionID << "\" cts=\"" << info.doneTimestamp << "\" cct=\"" << info.doneCount << "\"/>";
			}

			xml << "</done>";
		}

		vector<MissionInfo> misCur = Missions::getAllCurrentMissions(session->charID);
		if (misCur.size() > 0)
		{
			xml << "<cur>";

			for (int i = 0; i < misCur.size(); i++)
			{
				MissionInfo info = misCur.at(i);

				xml << "<m id=\"" << info.missionID << "\">";

				for (int k = 0; k < info.missionTasks.size(); k++)
				{
					xml << "<sv v=\"" << info.missionTasks.at(k).value.size() << "\"/>";
				}

				xml << "</m>";
			}

			xml << "</cur>";
		}

		xml << "</mis>";

		xml << "<mf";

		CharacterStyle style = CharacterStyles::getCharacterStyle(session->charID);

		xml << " hc=\"" << style.hairColor << "\"";
		xml << " hs=\"" << style.hairStyle << "\"";
		xml << " t=\"" << style.shirtColor << "\"";
		xml << " l=\"" << style.pantsColor << "\"";
		xml << " es=\"" << style.eyebrows << "\"";
		xml << " ess=\"" << style.eyes << "\"";
		xml << " ms=\"" << style.mouth << "\"";

		xml << "/>";

		xml << "<buff/>";
		xml << "<skil/>";
		xml << "<flag/>";
		xml << "<pet/>";
		xml << "<mnt/>";
		xml << "<dest/>";
		xml << "<char acct=\"" << session->accountID << "\" cc=\"" << Characters::getCurrency(session->charID) << "\" gm=\"" << Characters::getGMLevel(session->charID) << "\" ft=\"1\"/>";
		xml << "<lvl l=\"" << Characters::getLevel(session->charID) << "\"/>";

		xml << "<inv>";
		xml << "<bag>";

		long inventorySize = Characters::getMaxInventory(session->charID);
		xml << "<b t=\"0\" m=\"" << inventorySize << "\"/>";
		//xml << "<b t=\"2\" m=\"" << "10000" << "\"/>";
		xml << "<b t=\"5\" m=\"" << "200" << "\"/>";
		xml << "<b t=\"7\" m=\"" << inventorySize << "\"/>";

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

#include "Objects.h"

void WorldInstance::sendServerState(SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		BitStream* packet = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_SERVER_STATES);
		packet->Write((unsigned char)1);
		Server::sendPacket(packet, clientAddress);

		Location loc = Locations::getLocation(session->charID);
		wstring name = to_wstring(Characters::getName(session->charID));
		long gmLevel = Characters::getGMLevel(session->charID);

		ReplicaObject* replica = new ReplicaObject(session->charID, 1, name, gmLevel, loc.position, loc.rotation);
		replica->clientAddress = clientAddress;

		replica->statsIndex->max_health = Characters::getMaxHealth(session->charID);
		replica->statsIndex->cur_health = Characters::getHealth(session->charID);

		replica->statsIndex->max_imagination = Characters::getMaxImagination(session->charID);
		replica->statsIndex->cur_imagination = Characters::getImagination(session->charID);

		replica->statsIndex->cur_armor = Characters::getArmor(session->charID);

		ValueStorage::createValueInMemory(session->charID, "health", replica->statsIndex->cur_health);
		ValueStorage::createValueInMemory(session->charID, "imagination", replica->statsIndex->cur_imagination);
		ValueStorage::createValueInMemory(session->charID, "armor", replica->statsIndex->cur_armor);

		if (replica->statsIndex->cur_health == 0)
		{ 
			for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
			{
				SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

				GameMessages::die(session->charID, L"electro-shock-death", false, participant);
			}
		}

		ObjectsManager::addPlayer(replica, clientAddress);
	}
}

void WorldInstance::broadcastPositionUpdate(BitStream* data, SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		ReplicaObject* replica = ObjectsManager::getObjectByID(session->charID);
		ControllablePhysicsIndex* index = replica->controllablePhysicsIndex;

		data->Read(index->pos_x);
		data->Read(index->pos_y);
		data->Read(index->pos_z);

		data->Read(index->rot_x);
		data->Read(index->rot_y);
		data->Read(index->rot_z);
		data->Read(index->rot_w);

		data->Read(index->is_on_ground);
		data->Read(index->data_5_8);

		data->Read(index->flag_velocity);
		if (index->flag_velocity)
		{
			data->Read(index->vel_x);
			data->Read(index->vel_y);
			data->Read(index->vel_z);
		}

		data->Read(index->flag_angular_velocity);
		if (index->flag_angular_velocity)
		{
			data->Read(index->ang_vel_x);
			data->Read(index->ang_vel_y);
			data->Read(index->ang_vel_z);
		}

		data->Read(index->flag_5_11);
		if (index->flag_5_11)
		{
			data->Read(index->data_5_11_0);
			data->Read(index->data_5_11_1);
			data->Read(index->data_5_11_2);
			data->Read(index->data_5_11_3);

			data->Read(index->flag_5_11_4);
			if (index->flag_5_11_4)
			{
				data->Read(index->data_5_11_4_0);
				data->Read(index->data_5_11_4_1);
				data->Read(index->data_5_11_4_2);
			}
		}

		for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
		{
			SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

			if (participant != clientAddress)
				ObjectsManager::serializeObject(replica, participant);
		}

		switch (ServerRoles::toZoneID(Server::getServerRole()))
		{

		case ZONE_ID_VENTURE_EXPLORER:
		{
			if (index->pos_y < 562)
			{
				Helpers::deathCheck(session->charID, L"electro-shock-death", clientAddress);
			}
			break;
		}

		default:
			break;

		}
	}
}

string WorldInstance::broadcastChatMessage(BitStream* data, SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		unsigned char byte1;
		data->Read(byte1);
		unsigned char byte2;
		data->Read(byte2);
		unsigned char byte3;
		data->Read(byte3);

		unsigned long len;
		data->Read(len);
		wstring message = PacketUtils::readWStringFromBitStream(data, len);

		for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
		{
			SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

			Chat::sendChatMessage(message, participant, to_wstring(Characters::getName(session->charID)), session->charID, false, byte1);
		}

		return to_string(message);
	}

	return "ERROR: NO SESSION FOUND!";
}
