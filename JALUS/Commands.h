#pragma once
#include "Common.h"

class CommandSender
{
private:
	long long senderID;
public:
	CommandSender(long long senderID);

	void sendMessage(string msg);

	long long getSenderID();
};

class Commands
{
public:
	static void performCommand(CommandSender sender, string cmd, vector<string> args);
};