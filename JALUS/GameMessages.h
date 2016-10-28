#pragma once
#include "Common.h"

typedef unsigned short GameMessageID;

class GameMessages
{
public:
	static void processGameMessage(BitStream* data, SystemAddress clientAddress);
};