#pragma once
#include "Common.h"

class Accounts
{
public:
	static string name;

	static void init(string name, string structure);

	static void createAccount(string username, string passwordHash);
	static void deleteAccount(long long accountID);

	static long long getAccountID(string username);
	static string getUsername(long long accountID);
	static string getPasswordHash(long long accountID);
	static short getLoginTries(long long accountID);
	static long long getFrontCharacter(long long accountID);
	static bool isLocked(long long accountID);

	static void setPasswordHash(string passwordHash, long long accountID);
	static void setLoginTries(short loginTries, long long accountID);
	static void setFrontCharacter(long long frontCharID, long long accountID);
	static void updateLockedTimestamp(long long accountID);
};