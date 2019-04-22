#include "CharactersInstance.h"
#include "Logger.h"
#include "PacketUtils.h"
#include "General.h"
#include "Accounts.h"
#include "Sessions.h"
#include "Characters.h"
#include "Locations.h"
#include "InventoryItems.h"
#include "Config.h"
#include "TransitionInfos.h"
#include "Objects.h"
#include "ValueStorage.h"

void CharactersInstance::processPacket(BitStream* data, SystemAddress clientAddress, ClientToWorldPacketID packetID)
{
	switch (packetID)
	{
		
	case CLIENT_WORLD_VALIDATION:
	{
		bool success = General::performValidation(data, clientAddress);
		Logger::info("Client performed validation! (Success: " + to_string(success) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}
		
	case CLIENT_WORLD_CHARACTER_LIST_REQUEST:
	{
		CharactersInstance::sendCharacterList(clientAddress);
		Logger::info("Client received character list! (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}
		
	case CLIENT_WORLD_CHARACTER_CREATE_REQUEST:
	{
		long long id = CharactersInstance::performCharacterCreation(data, clientAddress);
		Logger::info("Client created a new character! (ID: " + to_string(id) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}
		
	case CLIENT_WORLD_CHARACTER_DELETE_REQUEST:
	{
		long long id = CharactersInstance::performCharacterDeletion(data, clientAddress);
		Logger::info("Client deleted a character! (ID: " + to_string(id) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}
		
	case CLIENT_WORLD_CHARACTER_RENAME_REQUEST:
	{
		long long id = CharactersInstance::performCharacterRenaming(data, clientAddress);
		Logger::info("Client renamed a character! (ID: " + to_string(id) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	case CLIENT_WORLD_LOGIN_REQUEST:
	{
		long long id = CharactersInstance::redirectToWorldInstance(data, clientAddress);
		Logger::info("Client got redirected to the stated WorldInstance of the picked character! (ID: " + to_string(id) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}
		
	default:
	{
		Logger::info("Server received a packet with unknown PacketID! (PacketID: " + to_string(packetID) + ") (RemoteConnectionType: " + to_string(RemoteConnectionType::RCT_CLIENT_TO_WORLD) + ") (Address: " + string(clientAddress.ToString()) + ")");
		break;
	}

	}
}

void CharactersInstance::sendCharacterList(SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		vector<long long> chars = Characters::getCharacterIDs(session->accountID);
		unsigned char index = 0;
		long long frontChar = Accounts::getFrontCharacter(session->accountID);

		for (int i = 0; i < chars.size(); i++)
		{
			if (chars.at(i) == frontChar)
				index = i;
		}

		BitStream* response = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_CHARACTER_LIST_RESPONSE);
		response->Write((unsigned char)chars.size());
		response->Write(index);

		for (int i = 0; i < chars.size(); i++)
		{
			long long id = chars.at(i);

			response->Write(id);
			response->Write((unsigned long)0);
			
			PacketUtils::writeWStringToBitStream(to_wstring(Characters::getName(id)), response, 33);
			PacketUtils::writeWStringToBitStream(to_wstring(Characters::getUnapprovedName(id)), response, 33);

			response->Write((unsigned char)Characters::isUnapprovedNameRejected(id));
			response->Write((unsigned char)0);

			response->Write((unsigned long long)0);
			response->Write((unsigned short)0);

			CharacterStyle style = CharacterStyles::getCharacterStyle(id);

			response->Write(style.shirtColor);
			response->Write(style.shirtStyle);
			response->Write(style.pantsColor);
			response->Write(style.hairStyle);
			response->Write(style.hairColor);
			response->Write(style.lh);
			response->Write(style.rh);
			response->Write(style.eyebrows);
			response->Write(style.eyes);
			response->Write(style.mouth);

			response->Write((unsigned long)0);

			Location loc = Locations::getLocation(id);
			
			//Logger::info("ZoneID: " + std::to_string(loc.zoneID) + " for " + std::to_string(id));
						

			response->Write(loc.zoneID); // ZONE ID; 0 FOR VE MOVIE

			response->Write((unsigned short)0);
			response->Write(loc.mapClone);

			response->Write((unsigned long long)0);

			vector<long> equippedLOTs = InventoryItems::getEquippedLOTs(id);

			response->Write((unsigned short)equippedLOTs.size());
			for (int k = 0; k < equippedLOTs.size(); k++)
			{
				response->Write(equippedLOTs.at(k));
			}
		}

		Server::sendPacket(response, clientAddress);

	}
}

long long CharactersInstance::performCharacterCreation(BitStream* data, SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		if ((data->GetNumberOfUnreadBits() / 8) >= 128)
		{
			string customName = to_string(PacketUtils::readWStringFromBitStream(data, 33));
			bool rejectCustomName = ((customName.length() > 0) && ((Characters::getCharacterID(customName) > -1) || Characters::isCustomNameAlreadyPending(customName)));

			long name_f, name_m, name_l;
			data->Read(name_f);
			data->Read(name_m);
			data->Read(name_l);

			string name = CDClient::getMinifigureName(name_f, name_m, name_l);

			// ToDo: Unimplement that later!
			if (customName.length() > 0)
			{
				if (!rejectCustomName)
				{
					name = customName;
					customName = "";
				}
			}

			data->IgnoreBytes(9);

			CharacterStyle style = PacketUtils::readCharacterStyleFromBitStream(data);

			long long id = Characters::createCharacter(session->accountID, name, customName, rejectCustomName, 0, style);
			Accounts::setFrontCharacter(id, session->accountID);

			ValueStorage::createValueInDatabase(id, "health", (long)4);
			ValueStorage::createValueInDatabase(id, "armor", (long)0);
			ValueStorage::createValueInDatabase(id, "imagination", (long)0);
			ValueStorage::createValueInDatabase(id, "wisp", (long)0);

			BitStream* response = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_ADD_FRIEND_REQUEST);
			response->Write((unsigned char)0);

			Server::sendPacket(response, clientAddress);
			CharactersInstance::sendCharacterList(clientAddress);

			return id;
		}
	}

	return -1;
}

long long CharactersInstance::performCharacterDeletion(BitStream* data, SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		if ((data->GetNumberOfUnreadBits() / 8) >= 8)
		{
			long long id;
			data->Read(id);

			Characters::deleteCharacter(id);
			ValueStorage::removeValueFromDatabase(id, "health");
			ValueStorage::removeValueFromDatabase(id, "armor");
			ValueStorage::removeValueFromDatabase(id, "imagination");
			ValueStorage::removeValueFromDatabase(id, "wisp");
			if (id == Accounts::getFrontCharacter(session->accountID))
				Accounts::setFrontCharacter(-1, session->accountID);

			BitStream* response = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_DELETE_CHARACTER_RESPONSE);
			response->Write((unsigned char)1);

			Server::sendPacket(response, clientAddress);

			return id;
		}
	}

	return -1;
}

long long CharactersInstance::performCharacterRenaming(BitStream* data, SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		if ((data->GetNumberOfUnreadBits() / 8) >= 74)
		{
			long long id;
			data->Read(id);

			string name = to_string(PacketUtils::readWStringFromBitStream(data, 33));

			bool rejectName = ((Characters::getCharacterID(name) > -1) || Characters::isCustomNameAlreadyPending(name));

			BitStream* response = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_CHARACTER_RENAME_RESPONSE);
			if (!rejectName)
			{
				Characters::setUnapprovedName(name, id);
				Characters::setIsUnapprovedNameRejected(false, id);
				Accounts::setFrontCharacter(id, session->accountID);
				response->Write((unsigned char)0);
			}
			else
				response->Write((unsigned char)2);

			Server::sendPacket(response, clientAddress);

			if (!rejectName)
				CharactersInstance::sendCharacterList(clientAddress);

			return id;
		}
	}

	return -1;
}

long long CharactersInstance::redirectToWorldInstance(BitStream* data, SystemAddress clientAddress)
{
	Session* session = Sessions::getSession(clientAddress);

	if (session != nullptr)
	{
		if ((data->GetNumberOfUnreadBits() / 8) >= 8)
		{
			long long id;
			data->Read(id);

			Location loc = Locations::getLocation(id);

			if (loc.zoneID != ZoneID::INVALID_ZONE_ID)
			{
				string nextInstanceAddress = Config::getWorldInstanceAddress(loc.zoneID);
				unsigned short nextInstancePort = Config::getWorldInstancePort(loc.zoneID);

				Accounts::setFrontCharacter(id, session->accountID);

				long value = ValueStorage::getValueFromDatabase(id, "wisp");
				if (value = -1)
					ValueStorage::createValueInDatabase(id, "wisp", (long)0);

				TransitionInfos::insertTransitionInfo(clientAddress, session->accountID, id, session->transitionKey);
				General::redirectToServer(clientAddress, nextInstanceAddress, nextInstancePort, false);
			}

			return id;
		}
	}

	return -1;
}

