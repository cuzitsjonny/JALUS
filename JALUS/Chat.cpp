#include "Chat.h"
#include "PacketUtils.h"

void Chat::sendChatMessage(wstring message, SystemAddress receiver, wstring senderName, long long senderObjectID, bool isSenderMythran, unsigned char chatChannel)
{
	BitStream* packet = PacketUtils::createPacketBase(RCT_CHAT, CHAT_PACKET_GENERAL_CHAT_MESSAGE);

	packet->Write((unsigned long long)0);
	packet->Write(chatChannel);
	packet->Write((unsigned char)(message.length() + 1));
	packet->Write((unsigned short)0);
	packet->Write((unsigned char)0);
	
	for (int i = 0; i < 33; i++)
	{
		if (i < senderName.length())
		{
			packet->Write(senderName.at(i));
		}
		else
		{
			packet->Write((unsigned short)0);
		}
	}

	packet->Write(senderObjectID);
	packet->Write((unsigned short)0);
	packet->Write((unsigned char)isSenderMythran);
	
	for (int i = 0; i < message.length(); i++)
	{
		packet->Write(message.at(i));
	}

	packet->Write((unsigned short)0);

	Server::sendPacket(packet, receiver);
}
