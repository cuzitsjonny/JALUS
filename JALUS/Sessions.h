#pragma once
#include "Common.h"

class Session
{
public:
	SystemAddress clientAddress = UNASSIGNED_SYSTEM_ADDRESS;
	long long accountID = -1;
	long long charID = -1;
	string transitionKey = "";
};

class Sessions
{
private:
	static vector<Session> sessions;
public:
	static void addSession(SystemAddress clientAddress, long long accountID, long long charID, string transitionKey);
	static void removeSession(SystemAddress clientAddress);
	static Session* getSession(SystemAddress clientAddress);
};