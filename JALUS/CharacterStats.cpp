#include "CharacterStats.h"
#include "Config.h"
#include "Logger.h"

string CharacterStats::name;

void CharacterStats::init(string name, string structure)
{
	CharacterStats::name = name;

	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "CREATE TABLE IF NOT EXISTS";
		ss << " " << name << " ";
		ss << "(";
		ss << structure;
		ss << ");";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		Logger::error("Failed to init SQLTable! (Name: " + name + ") Error: '" + string(x.ErrMessage()) + "'");
		exit(-1);
	}
}

void CharacterStats::saveCharacterStats(CharacterStats stats, long long charID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "REPLACE INTO";
		ss << " " << CharacterStats::name << " ";
		ss << "(id, total_amount_of_currency_collected, number_of_bricks_collected, number_of_smashables_smashed, ";
		ss << "number_of_quick_builds_completed, number_of_enemies_smashed, number_of_rockets_used, ";
		ss << "number_of_missions_completed, number_of_pets_tamed, number_of_imagination_powerups_collected, ";
		ss << "number_of_life_powerups_collected, number_of_armor_powerups_collected, total_distance_traveled, ";
		ss << "number_of_times_smashed, total_damage_taken, total_damage_healed, ";
		ss << "total_armor_repaired, total_imagination_restored, total_imagination_used, ";
		ss << "total_distance_driven, total_time_airborne_in_a_race_car, number_of_racing_imagination_powerups_collected, ";
		ss << "number_of_racing_imagination_crates_smashed, number_of_times_race_car_boost_activated, number_of_wrecks_in_a_race_car, ";
		ss << "number_of_racing_smashables_smashed, number_of_races_finished, number_of_first_place_races_finished)";
		ss << " VALUES ";
		ss << "(";
		ss << "'" << charID << "'";
		ss << ", '" << stats.totalAmountOfCurrencyCollected << "'";
		ss << ", '" << stats.numberOfBricksCollected << "'";
		ss << ", '" << stats.numberOfSmashablesSmashed << "'";
		ss << ", '" << stats.numberOfQuickBuildsCompleted << "'";
		ss << ", '" << stats.numberOfEnemiesSmashed << "'";
		ss << ", '" << stats.numberOfRocketsUsed << "'";
		ss << ", '" << stats.numberOfMissionsCompleted << "'";
		ss << ", '" << stats.numberOfPetsTamed << "'";
		ss << ", '" << stats.numberOfImaginationPowerupsCollected << "'";
		ss << ", '" << stats.numberOfLifePowerupsCollected << "'";
		ss << ", '" << stats.numberOfArmorPowerupsCollected << "'";
		ss << ", '" << stats.totalDistanceTraveled << "'";
		ss << ", '" << stats.numberOfTimesSmashed << "'";
		ss << ", '" << stats.totalDamageTaken << "'";
		ss << ", '" << stats.totalDamageHealed << "'";
		ss << ", '" << stats.totalArmorRepaired << "'";
		ss << ", '" << stats.totalImaginationRestored << "'";
		ss << ", '" << stats.totalImaginationUsed << "'";
		ss << ", '" << stats.totalDistanceDriven << "'";
		ss << ", '" << stats.totalTimeAirborneInARaceCar << "'";
		ss << ", '" << stats.numberOfRacingImaginationPowerupsCollected << "'";
		ss << ", '" << stats.numberOfRacingImaginationCratesSmashed << "'";
		ss << ", '" << stats.numberOfTimesRaceCarBoostActivated << "'";
		ss << ", '" << stats.numberOfWrecksInARaceCar << "'";
		ss << ", '" << stats.numberOfRacingSmashablesSmashed << "'";
		ss << ", '" << stats.numberOfRacesFinished << "'";
		ss << ", '" << stats.numberOfFirstPlaceRacesFinished << "'";
		ss << ");";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void CharacterStats::deleteCharacterStats(long long charID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "DELETE FROM";
		ss << " " << CharacterStats::name << " ";
		ss << "WHERE id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

CharacterStats CharacterStats::getCharacterStats(long long charID)
{
	SAConnection con;
	SACommand cmd;

	CharacterStats r = CharacterStats();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT * FROM";
		ss << " " << CharacterStats::name << " ";
		ss << "WHERE id = '" << charID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
		{
			r.totalAmountOfCurrencyCollected = cmd.Field("total_amount_of_currency_collected").asNumeric();
			r.numberOfBricksCollected = cmd.Field("number_of_bricks_collected").asNumeric();
			r.numberOfSmashablesSmashed = cmd.Field("number_of_smashables_smashed").asNumeric();
			r.numberOfQuickBuildsCompleted = cmd.Field("number_of_quick_builds_completed").asNumeric();
			r.numberOfEnemiesSmashed = cmd.Field("number_of_enemies_smashed").asNumeric();
			r.numberOfRocketsUsed = cmd.Field("number_of_rockets_used").asNumeric();
			r.numberOfMissionsCompleted = cmd.Field("number_of_missions_completed").asNumeric();
			r.numberOfPetsTamed = cmd.Field("number_of_pets_tamed").asNumeric();
			r.numberOfImaginationPowerupsCollected = cmd.Field("number_of_imagination_powerups_collected").asNumeric();
			r.numberOfLifePowerupsCollected = cmd.Field("number_of_life_powerups_collected").asNumeric();
			r.numberOfArmorPowerupsCollected = cmd.Field("number_of_armor_powerups_collected").asNumeric();
			r.totalDistanceTraveled = cmd.Field("total_distance_traveled").asNumeric();
			r.numberOfTimesSmashed = cmd.Field("number_of_times_smashed").asNumeric();
			r.totalDamageTaken = cmd.Field("total_damage_taken").asNumeric();
			r.totalDamageHealed = cmd.Field("total_damage_healed").asNumeric();
			r.totalArmorRepaired = cmd.Field("total_armor_repaired").asNumeric();
			r.totalImaginationRestored = cmd.Field("total_imagination_restored").asNumeric();
			r.totalImaginationUsed = cmd.Field("total_imagination_used").asNumeric();
			r.totalDistanceDriven = cmd.Field("total_distance_driven").asNumeric();
			r.totalTimeAirborneInARaceCar = cmd.Field("total_time_airborne_in_a_race_car").asNumeric();
			r.numberOfRacingImaginationPowerupsCollected = cmd.Field("number_of_racing_imagination_powerups_collected").asNumeric();
			r.numberOfRacingImaginationCratesSmashed = cmd.Field("number_of_racing_imagination_crates_smashed").asNumeric();
			r.numberOfTimesRaceCarBoostActivated = cmd.Field("number_of_times_race_car_boost_activated").asNumeric();
			r.numberOfWrecksInARaceCar = cmd.Field("number_of_wrecks_in_a_race_car").asNumeric();
			r.numberOfRacingSmashablesSmashed = cmd.Field("number_of_racing_smashables_smashed").asNumeric();
			r.numberOfRacesFinished = cmd.Field("number_of_races_finished").asNumeric();
			r.numberOfFirstPlaceRacesFinished = cmd.Field("number_of_first_place_races_finished").asNumeric();
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}
