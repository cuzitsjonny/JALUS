#pragma once
#include "Common.h"

class Chat
{
public:
	static void sendChatMessage(wstring message, SystemAddress receiver, wstring senderName = L"", long long senderObjectID = 1, bool isSenderMythran = false, unsigned char chatChannel = 1);
};