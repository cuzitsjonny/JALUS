#pragma once
#include "Common.h"

class General
{
public:
	static bool performHandshake(BitStream* data, SystemAddress clientAddress);
	static bool performValidation(BitStream* data, SystemAddress clientAddress);
	static void redirectToServer(SystemAddress clientAddress, string serverAddress, unsigned short serverPort, bool displayDimensionalShiftAnnouncement);
};