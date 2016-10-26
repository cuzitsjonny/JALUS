#pragma once
#include "Common.h"

class PingTool
{
private:
	static SocketDescriptor socketDescriptor;
public:
	static void init();
	static bool ping(const char* host, unsigned short port, const char* password, int passwordLength);
};