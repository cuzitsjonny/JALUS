#include "General.h"
#include "PacketUtils.h"
#include "TransitionInfos.h"
#include "Sessions.h"
#include "Accounts.h"

bool General::performHandshake(BitStream* data, SystemAddress clientAddress)
{
	if ((data->GetNumberOfUnreadBits() / 8) == 51)
	{
		unsigned long gameVersion;
		data->Read(gameVersion);

		BitStream* response = PacketUtils::createPacketBase(RCT_GENERAL, GENERAL_PACKET_VERSION_CONFIRM);
		response->Write((unsigned long)171022);
		response->Write((unsigned long)147);
		response->Write((unsigned long)(Server::isAuthenticationInstance() ? RCT_CLIENT_TO_AUTH : RCT_CLIENT_TO_WORLD));
		response->Write((unsigned long)GetCurrentProcessId());
		response->Write((unsigned short)255);
		PacketUtils::writeStringToBitStream("127.0.0.1", response, 33);

		Server::sendPacket(response, clientAddress);

		return (gameVersion == 171022);
	}

	return false;
}

bool General::performValidation(BitStream* data, SystemAddress clientAddress)
{
	string username = to_string(PacketUtils::readWStringFromBitStream(data, 33));
	string transitionKey = to_string(PacketUtils::readWStringFromBitStream(data, 33));

	size_t pos = username.find("\\");
	if (pos != string::npos)
		username = username.substr((pos + 1), username.length());

	TransitionInfo i = TransitionInfos::getTransitionInfo(clientAddress);
	TransitionInfos::deleteTransitionInfo(clientAddress);

	bool r = (i.accountID == Accounts::getAccountID(username) && i.transitionKey == transitionKey);

	if (!r)
	{
		BitStream* response = PacketUtils::createPacketBase(RCT_GENERAL, GENERAL_PACKET_DISCONNECT_NOTIFY);
		response->Write(DisconnectID::DISCONNECT_INVALID_TRANSITION_KEY);
		response->Write((unsigned long)0);
		Server::sendPacket(response, clientAddress);
	}
	else
		Sessions::addSession(clientAddress, i.accountID, i.charID, i.transitionKey);

	return r;
}

void General::redirectToServer(SystemAddress clientAddress, string serverAddress, unsigned short serverPort, bool displayDimensionalShiftAnnouncement)
{
	BitStream* packet = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_TRANSFER_TO_WORLD);

	PacketUtils::writeStringToBitStream(serverAddress, packet, 33);
	packet->Write(serverPort);
	packet->Write((unsigned char)displayDimensionalShiftAnnouncement);

	Server::sendPacket(packet, clientAddress);
}
