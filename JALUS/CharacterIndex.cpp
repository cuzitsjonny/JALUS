#include "CharacterIndex.h"

void CharacterIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(flag_0);
	if (flag_0)
	{
		bitStream->Write(flag_0_0);
		if (flag_0_0)
		{
			bitStream->Write(vehicle_object_id);
		}
		bitStream->Write(data_0_1);
	}

	bitStream->Write(flag_1);
	if (flag_1)
	{
		bitStream->Write(level);
	}

	bitStream->Write(flag_2);
	if (flag_2)
	{
		bitStream->Write(data_2_0);
		bitStream->Write(data_2_1);
	}

	if (isConstruction)
	{
		bitStream->Write(flag_3);
		if (flag_3)
		{
			bitStream->Write(data_3_0);
		}

		bitStream->Write(flag_4);
		if (flag_4)
		{
			bitStream->Write(data_4_0);
		}

		bitStream->Write(flag_5);
		if (flag_5)
		{
			bitStream->Write(data_5_0);
		}

		bitStream->Write(flag_6);
		if (flag_6)
		{
			bitStream->Write(data_6_0);
		}

		bitStream->Write(hair_color);
		bitStream->Write(hair_style);
		bitStream->Write(hd);
		bitStream->Write(shirt_color);
		bitStream->Write(pants_color);
		bitStream->Write(cd);
		bitStream->Write(hdc);
		bitStream->Write(eyebrows_style);
		bitStream->Write(eyes_style);
		bitStream->Write(mouth_style);
		bitStream->Write(account_id);
		bitStream->Write(llog);
		bitStream->Write(data_19);
		bitStream->Write(lego_score);
		bitStream->Write(is_free_to_play);
		bitStream->Write(totalAmountOfCurrencyCollected);
		bitStream->Write(numberOfBricksCollected);
		bitStream->Write(numberOfSmashablesSmashed);
		bitStream->Write(numberOfQuickBuildsCompleted);
		bitStream->Write(numberOfEnemiesSmashed);
		bitStream->Write(numberOfRocketsUsed);
		bitStream->Write(numberOfMissionsCompleted);
		bitStream->Write(numberOfPetsTamed);
		bitStream->Write(numberOfImaginationPowerupsCollected);
		bitStream->Write(numberOfLifePowerupsCollected);
		bitStream->Write(numberOfArmorPowerupsCollected);
		bitStream->Write(totalDistanceTraveled);
		bitStream->Write(numberOfTimesSmashed);
		bitStream->Write(totalDamageTaken);
		bitStream->Write(totalDamageHealed);
		bitStream->Write(totalArmorRepaired);
		bitStream->Write(totalImaginationRestored);
		bitStream->Write(totalImaginationUsed);
		bitStream->Write(totalDistanceDriven);
		bitStream->Write(totalTimeAirborneInARaceCar);
		bitStream->Write(numberOfRacingImaginationPowerupsCollected);
		bitStream->Write(numberOfRacingImaginationCratesSmashed);
		bitStream->Write(numberOfTimesRaceCarBoostActivated);
		bitStream->Write(numberOfWrecksInARaceCar);
		bitStream->Write(numberOfRacingSmashablesSmashed);
		bitStream->Write(numberOfRacesFinished);
		bitStream->Write(numberOfFirstPlaceRacesFinished);
		bitStream->Write(data_49);
		bitStream->Write(landing_rocket_info.length() > 0);
		if (landing_rocket_info.length() > 0)
		{
			bitStream->Write((unsigned short)landing_rocket_info.length());
			for (int i = 0; i < landing_rocket_info.length(); i++)
			{
				bitStream->Write(landing_rocket_info[i]);
			}
		}
	}

	bitStream->Write(flag_51);
	if (flag_51)
	{
		bitStream->Write(data_51_0);
		bitStream->Write(data_51_1);
		bitStream->Write(data_51_2);
		bitStream->Write(data_51_3);
		bitStream->Write(data_51_4);
	}

	bitStream->Write(flag_52);
	if (flag_52)
	{
		bitStream->Write(data_52_0);
	}

	bitStream->Write(flag_53);
	if (flag_53)
	{
		bitStream->Write(data_53_0);
		bitStream->Write(data_53_1);
		bitStream->Write(data_53_2);
		bitStream->Write(data_53_3);
	}
}
