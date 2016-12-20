#pragma once
#include "Common.h"

class CharacterStats
{
public:
	unsigned long long totalAmountOfCurrencyCollected = 0;
	unsigned long long numberOfBricksCollected = 0;
	unsigned long long numberOfSmashablesSmashed = 0;
	unsigned long long numberOfQuickBuildsCompleted = 0;
	unsigned long long numberOfEnemiesSmashed = 0;
	unsigned long long numberOfRocketsUsed = 0;
	unsigned long long numberOfMissionsCompleted = 0;
	unsigned long long numberOfPetsTamed = 0;
	unsigned long long numberOfImaginationPowerupsCollected = 0;
	unsigned long long numberOfLifePowerupsCollected = 0;
	unsigned long long numberOfArmorPowerupsCollected = 0;
	unsigned long long totalDistanceTraveled = 0;
	unsigned long long numberOfTimesSmashed = 0;
	unsigned long long totalDamageTaken = 0;
	unsigned long long totalDamageHealed = 0;
	unsigned long long totalArmorRepaired = 0;
	unsigned long long totalImaginationRestored = 0;
	unsigned long long totalImaginationUsed = 0;
	unsigned long long totalDistanceDriven = 0;
	unsigned long long totalTimeAirborneInARaceCar = 0;
	unsigned long long numberOfRacingImaginationPowerupsCollected = 0;
	unsigned long long numberOfRacingImaginationCratesSmashed = 0;
	unsigned long long numberOfTimesRaceCarBoostActivated = 0;
	unsigned long long numberOfWrecksInARaceCar = 0;
	unsigned long long numberOfRacingSmashablesSmashed = 0;
	unsigned long long numberOfRacesFinished = 0;
	unsigned long long numberOfFirstPlaceRacesFinished = 0;

	static string name;

	static void init(string name, string structure);

	static void saveCharacterStats(CharacterStats stats, long long charID);
	static void deleteCharacterStats(long long charID);
	static CharacterStats getCharacterStats(long long charID);
};