#pragma once
#include "Common.h"

class TransitionInfo
{
public:
	SystemAddress clientAddress = UNASSIGNED_SYSTEM_ADDRESS;
	long long accountID = -1;
	long long charID = -1;
	string transitionKey = "";
};

class TransitionInfos
{
public:
	static string name;

	static void init(string name, string structure);

	static string generateTransitionInfo(SystemAddress clientAddress, long long accountID);
	static void insertTransitionInfo(SystemAddress clientAddress, long long accountID, long long charID, string transitionKey);
	static void deleteTransitionInfo(SystemAddress clientAddress);

	static TransitionInfo getTransitionInfo(SystemAddress clientAddress);
};