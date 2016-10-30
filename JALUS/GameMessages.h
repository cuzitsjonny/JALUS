#pragma once
#include "Common.h"

enum GameMessageID : unsigned short
{
	GAME_MESSAGE_ID_READY_FOR_UPDATES = 888
};

class GameMessages
{
public:
	static void processGameMessage(BitStream* data, SystemAddress clientAddress);
};