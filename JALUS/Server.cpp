#include "Server.h"
#include "Config.h"
#include "Logger.h"
#include "PingTool.h"
#include "Accounts.h"
#include "Bans.h"
#include "TransitionInfos.h"
#include "Objects.h"
#include "Characters.h"
#include "CharacterStyles.h"
#include "InventoryItems.h"
#include "Locations.h"
#include "CharacterStats.h"
#include "LUZCache.h"
#include "LVLCache.h"
#include "Missions.h"
#include "CurrentMissionTasks.h"
#include "Flags.h"

ServerRole Server::serverRole;
SocketDescriptor Server::socketDescriptor;
NetworkIDManager Server::networkIDManager;
ReplicaManager Server::replicaManager;
RakPeerInterface* Server::peerInterface;

void Server::init(ServerRole serverRole)
{
	Server::serverRole = serverRole;

	Config::init();
	Logger::init();
	PingTool::init();

	Accounts::init("Accounts", "id BIGINT(20) AUTO_INCREMENT PRIMARY KEY, username CHAR(32), password_hash CHAR(128), login_tries TINYINT(1) NOT NULL DEFAULT 0, locked_timestamp BIGINT(20) NOT NULL DEFAULT -1, front_character BIGINT(20) NOT NULL DEFAULT -1");
	Bans::init("Bans", "got_banned CHAR(20) UNIQUE, banned BIGINT(20), is_ip_ban TINYINT(1), reason TINYTEXT, banned_timestamp BIGINT(20), how_long BIGINT(20)");
	TransitionInfos::init("TransitionInfos", "client_address CHAR(30) UNIQUE, account_id BIGINT(20), char_id BIGINT(20), transition_key CHAR(32)");
	Objects::init("Objects", "id BIGINT(20) PRIMARY KEY, lot INT(11)");
	Characters::init("Characters", "id BIGINT(20) PRIMARY KEY, account_id BIGINT(20), name CHAR(32), unapproved_name CHAR(32), is_unapproved_name_rejected TINYINT(1), level INT(11) NOT NULL DEFAULT 1, max_inventory INT(11) NOT NULL DEFAULT 24, max_health INT(11) NOT NULL DEFAULT 4, max_imagination INT(11) NOT NULL DEFAULT 0, universe_score BIGINT(20) NOT NULL DEFAULT 0, currency BIGINT(20) NOT NULL DEFAULT 0, reputation BIGINT(20) NOT NULL DEFAULT 0, gm_level INT(11)");
	CharacterStyles::init("CharacterStyles", "id BIGINT(20) PRIMARY KEY, shirt_color INT(11), shirt_style INT(11), pants_color INT(11), hair_style INT(11), hair_color INT(11), lh INT(11), rh INT(11), eyebrows INT(11), eyes INT(11), mouth INT(11)");
	InventoryItems::init("InventoryItems", "id BIGINT(20) PRIMARY KEY, owner_id BIGINT(20), item_type TINYINT(2), inventory_type TINYINT(2), slot SMALLINT(6), count INT(11), is_bound TINYINT(1), is_equipped TINYINT(1)");
	Locations::init("Locations", "id BIGINT(20) PRIMARY KEY, pos_x FLOAT, pos_y FLOAT, pos_z FLOAT, rot_x FLOAT, rot_y FLOAT, rot_z FLOAT, rot_w FLOAT, zone_id SMALLINT(6), map_clone INT(11)");
	//Locations::init("Locations", "id BIGINT(20) PRIMARY KEY, pos_x FLOAT, pos_y FLOAT, pos_z FLOAT, rot_x FLOAT, rot_y FLOAT, rot_z FLOAT, rot_w FLOAT, zone_id SMALLINT(6), map_clone INT(11), last_zone_id SMALLINT(6)");
	CharacterStats::init("CharacterStats", "id BIGINT(20) PRIMARY KEY, total_amount_of_currency_collected BIGINT(20) UNSIGNED, number_of_bricks_collected BIGINT(20) UNSIGNED, number_of_smashables_smashed BIGINT(20) UNSIGNED, number_of_quick_builds_completed BIGINT(20) UNSIGNED, number_of_enemies_smashed BIGINT(20) UNSIGNED, number_of_rockets_used BIGINT(20) UNSIGNED, number_of_missions_completed BIGINT(20) UNSIGNED, number_of_pets_tamed BIGINT(20) UNSIGNED, number_of_imagination_powerups_collected BIGINT(20) UNSIGNED, number_of_life_powerups_collected BIGINT(20) UNSIGNED, number_of_armor_powerups_collected BIGINT(20) UNSIGNED, total_distance_traveled BIGINT(20) UNSIGNED, number_of_times_smashed BIGINT(20) UNSIGNED, total_damage_taken BIGINT(20) UNSIGNED, total_damage_healed BIGINT(20) UNSIGNED, total_armor_repaired BIGINT(20) UNSIGNED, total_imagination_restored BIGINT(20) UNSIGNED, total_imagination_used BIGINT(20) UNSIGNED, total_distance_driven BIGINT(20) UNSIGNED, total_time_airborne_in_a_race_car BIGINT(20) UNSIGNED, number_of_racing_imagination_powerups_collected BIGINT(20) UNSIGNED, number_of_racing_imagination_crates_smashed BIGINT(20) UNSIGNED, number_of_times_race_car_boost_activated BIGINT(20) UNSIGNED, number_of_wrecks_in_a_race_car BIGINT(20) UNSIGNED, number_of_racing_smashables_smashed BIGINT(20) UNSIGNED, number_of_races_finished BIGINT(20) UNSIGNED, number_of_first_place_races_finished BIGINT(20) UNSIGNED");
	Missions::init("Missions", "id BIGINT(20) AUTO_INCREMENT PRIMARY KEY, mission_id INT(11), character_id BIGINT(20), is_done TINYINT(1), done_timestamp BIGINT(20), done_count INT(11)");
	CurrentMissionTasks::init("CurrentMissionTasks", "id BIGINT(20) AUTO_INCREMENT PRIMARY KEY, mission_id INT(11), character_id BIGINT(20), unique_id INT(11), value VARCHAR(255)");
	Flags::init("Flags", "id BIGINT(20), value TINYINT(1), flag_id INT(11), unique_id BIGINT(20) AUTO_INCREMENT PRIMARY KEY");
	
	LUZCache::init();
}

bool Server::start()
{
	bool r;

	Server::peerInterface = RakNetworkFactory::GetRakPeerInterface();
	Server::socketDescriptor = SocketDescriptor(Config::getPort(), 0);

	r = Server::peerInterface->Startup(Config::getSlots(), 30, &Server::socketDescriptor, 1);

	if (r)
	{
		Server::peerInterface->SetMaximumIncomingConnections(Config::getSlots());
		Server::peerInterface->SetIncomingPassword("3.25 ND1", 8);

		if (Server::isWorldInstance())
		{
			Server::peerInterface->AttachPlugin(&Server::replicaManager);
			Server::peerInterface->SetNetworkIDManager(&Server::networkIDManager);

			/*Server::replicaManager.SetAutoParticipateNewConnections(true);*/
			Server::replicaManager.SetAutoConstructToNewParticipants(true);
			Server::replicaManager.SetAutoSerializeInScope(true);
			Server::replicaManager.SetDefaultScope(true);

			Server::networkIDManager.SetIsNetworkIDAuthority(true);

			LVLCache::loadObjects();
		}
	}

	return r;
}

void Server::shutdown(int errorCode)
{
	RakNetworkFactory::DestroyRakPeerInterface(Server::peerInterface);
	Logger::shutdown();
	exit(errorCode);
}

ServerRole Server::getServerRole()
{
	return Server::serverRole;
}

RakPeerInterface* Server::getPeerInterface()
{
	return Server::peerInterface;
}

ReplicaManager* Server::getReplicaManager()
{
	return &Server::replicaManager;
}

bool Server::isAuthenticationInstance()
{
	return (Server::serverRole == ServerRole::SERVER_ROLE_AUTHENTICATION);
}

bool Server::isCharactersInstance()
{
	return (Server::serverRole == ServerRole::SERVER_ROLE_CHARACTERS);
}

bool Server::isWorldInstance()
{
	switch (Server::serverRole)
	{

	case SERVER_ROLE_VENTURE_EXPLORER:
	case SERVER_ROLE_RETURN_TO_VENTURE_EXPLORER:
	case SERVER_ROLE_AVANT_GARDENS:
	case SERVER_ROLE_AVANT_GARDENS_SURVIVAL:
	case SERVER_ROLE_SPIDER_QUEEN_BATTLE:
	case SERVER_ROLE_BLOCK_YARD:
	case SERVER_ROLE_AVANT_GROVE:
	case SERVER_ROLE_NIMBUS_STATION:
	case SERVER_ROLE_PET_COVE:
	case SERVER_ROLE_VERTIGO_LOOP_RACETRACK:
	case SERVER_ROLE_BATTLE_OF_NIMBUS_STATION:
	case SERVER_ROLE_NIMBUS_ROCK:
	case SERVER_ROLE_NIMBUS_ISLE:
	case SERVER_ROLE_GNARLED_FOREST:
	case SERVER_ROLE_CANYON_COVE:
	case SERVER_ROLE_CHANTEY_SHANTEY:
	case SERVER_ROLE_FORBIDDEN_VALLEY:
	case SERVER_ROLE_FORBIDDEN_VALLEY_DRAGON:
	case SERVER_ROLE_DRAGONMAW_CHASM:
	case SERVER_ROLE_RAVEN_BLUFF:
	case SERVER_ROLE_STARBASE_3001:
	case SERVER_ROLE_DEEP_FREEZE:
	case SERVER_ROLE_ROBOT_CITY:
	case SERVER_ROLE_MOON_BASE:
	case SERVER_ROLE_PORTABELLO:
	case SERVER_ROLE_LEGO_CLUB:
	case SERVER_ROLE_CRUX_PRIME:
	case SERVER_ROLE_NEXUS_TOWER:
	case SERVER_ROLE_NINJAGO_MONASTERY:
	case SERVER_ROLE_FRAKJAW_BATTLE:
		return true;

	default:
		return false;

	}
}

void Server::sendPacket(BitStream* bitStream, SystemAddress address)
{
	Server::peerInterface->Send(bitStream, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, address, false);
	delete bitStream;
}
