#pragma once
#include "Common.h"
#include "Server.h"

class ServerLoop
{
private:
	static bool run;
	static std::vector<char> input;

	static void parseUserInput();
public:
	static void start();
	static void stop();

	static void routePacket(unsigned short remoteConnectionType, unsigned long packetID, BitStream* data, SystemAddress clientAddress);
};