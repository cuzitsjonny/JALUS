#pragma once
#include "Common.h"

class BanInfo
{
public:
	string gotBanned = "";
	long long banned = -1;
	bool isIPBan = false;
	string reason = "";
	long long bannedTimestamp = -1;
	long long howLong = -1;
};

class Bans
{
public:
	static string name;

	static void init(string name, string structure);

	static void ban(string gotBanned, long long banned, bool isIPBan, string reason, long long howLong);
	static void unban(string gotBanned, bool isIPBan);

	static bool isBanned(string key, bool isIPBan);
	static BanInfo getInfo(string gotBanned, bool isIPBan);

	static string toTimeString(long long time);
};