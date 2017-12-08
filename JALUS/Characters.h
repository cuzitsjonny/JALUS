#pragma once
#include "Common.h"
#include "CharacterStyles.h"

class Characters
{
public:
	static string name;

	static void init(string name, string structure);

	static long long createCharacter(long long accountID, string name, string unapprovedName, bool isUnapprovedNameRejected, long gmLevel, CharacterStyle style);
	static void deleteCharacter(long long charID);

	static long long getCharacterID(string name);
	static bool isCustomNameAlreadyPending(string name);
	static long long getAccountID(long long charID);
	static vector<long long> getCharacterIDs(long long accountID);
	static string getName(long long charID);
	static string getUnapprovedName(long long charID);
	static bool isUnapprovedNameRejected(long long charID);
	static long getLevel(long long charID);
	static long long getUniverseScore(long long charID);
	static long long getCurrency(long long charID);
	static long long getReputation(long long charID);
	static long getGMLevel(long long charID);
	static long getMaxInventory(long long charID);
	static long getMaxHealth(long long charID);
	static long getMaxImagination(long long charID);

	static long getHealth(long long charID);
	static long getArmor(long long charID);
	static long getImagination(long long charID);

	static void setName(string name, long long charID);
	static void setUnapprovedName(string unapprovedName, long long charID);
	static void setIsUnapprovedNameRejected(bool isUnapprovedNameRejected, long long charID);
	static void setLevel(long level, long long charID);
	static void setUniverseScore(long long universeScore, long long charID);
	static void setCurrency(long long currency, long long charID);
	static void setReputation(long long reputation, long long charID);
	static void setGMLevel(long gmLevel, long long charID);
	static void setMaxInventory(long maxInventory, long long charID);
	static void setMaxHealth(long maxHealth, long long charID);
	static void setMaxImagination(long maxImagination, long long charID);

	static void setHealth(long health, long long charID);
	static void setArmor(long armor, long long charID);
	static void setImagination(long imagination, long long charID);

};

