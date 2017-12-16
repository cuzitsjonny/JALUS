#include "ZoneIDs.h"

ZoneID ZoneIDs::fromString(string str)
{
	string l = "";

	for (int i = 0; i < str.length(); i++) {
		l += tolower(str[i]);
	}

	if (l == "venture_explorer" || l == "venture explorer" || l == "ve" || l == "1000")
		return ZoneID::ZONE_ID_VENTURE_EXPLORER;

	else if (l == "return_to_venture_explorer" || l == "return to venture explorer" || l == "1001")
		return ZoneID::ZONE_ID_RETURN_TO_VENTURE_EXPLORER;

	else if (l == "avant_gardens" || l == "avant gardens" || l == "ag" || l == "1100")
		return ZoneID::ZONE_ID_AVANT_GARDENS;

	else if (l == "avant_gardens_survival" || l == "avant gardens survival" || l == "1101")
		return ZoneID::ZONE_ID_AVANT_GARDENS_SURVIVAL;

	else if (l == "spider_queen_battle" || l == "spider queen battle" || l == "1102")
		return ZoneID::ZONE_ID_SPIDER_QUEEN_BATTLE;

	else if (l == "block_yard" || l == "block yard" || l == "1150")
		return ZoneID::ZONE_ID_BLOCK_YARD;

	else if (l == "avant_grove" || l == "avant grove" || l == "1151")
		return ZoneID::ZONE_ID_AVANT_GROVE;

	else if (l == "nimbus_station" || l == "nimbus station" || l == "ns" || l == "1200")
		return ZoneID::ZONE_ID_NIMBUS_STATION;

	else if (l == "pet_cove" || l == "pet cove" || l == "1201")
		return ZoneID::ZONE_ID_PET_COVE;

	else if (l == "vertigo_loop_racetrack" || l == "vertigo run racetrack" || l == "1203")
		return ZoneID::ZONE_ID_VERTIGO_LOOP_RACETRACK;

	else if (l == "battle_of_nimbus_station" || l == "battle of nimbus station" || l == "1204")
		return ZoneID::ZONE_ID_BATTLE_OF_NIMBUS_STATION;

	else if (l == "nimbus_rock" || l == "nimbus rock" || l == "1250")
		return ZoneID::ZONE_ID_NIMBUS_ROCK;

	else if (l == "nimbus_isle" || l == "nimbus isle" || l == "1251")
		return ZoneID::ZONE_ID_NIMBUS_ISLE;

	else if (l == "gnarled_forest" || l == "gnarled forest" || l == "gf" || l == "1300")
		return ZoneID::ZONE_ID_GNARLED_FOREST;

	else if (l == "canyon_cove" || l == "canyon cove" || l == "1302")
		return ZoneID::ZONE_ID_CANYON_COVE;

	else if (l == "chantey_shantey" || l == "chantey shantey" || l == "1350")
		return ZoneID::ZONE_ID_CHANTEY_SHANTEY;

	else if (l == "forbidden_valley" || l == "forbidden valley" || l == "fv" || l == "1400")
		return ZoneID::ZONE_ID_FORBIDDEN_VALLEY;

	else if (l == "forbidden_valley_dragon" || l == "forbidden valley dragon" || l == "1402")
		return ZoneID::ZONE_ID_FORBIDDEN_VALLEY_DRAGON;

	else if (l == "dragonmaw_chasm" || l == "dragonmaw chasm" || l == "1403")
		return ZoneID::ZONE_ID_DRAGONMAW_CHASM;

	else if (l == "raven_bluff" || l == "raven bluff" || l == "1450")
		return ZoneID::ZONE_ID_RAVEN_BLUFF;

	else if (l == "starbase_3001" || l == "starbase 3001" || l == "sb" || l == "1600")
		return ZoneID::ZONE_ID_STARBASE_3001;

	else if (l == "deep_freeze" || l == "deep freeze" || l == "1601")
		return ZoneID::ZONE_ID_DEEP_FREEZE;

	else if (l == "robot_city" || l == "robot city" || l == "1602")
		return ZoneID::ZONE_ID_ROBOT_CITY;

	else if (l == "moon_base" || l == "moon base" || l == "1603")
		return ZoneID::ZONE_ID_MOON_BASE;

	else if (l == "portabello" || l == "1604")
		return ZoneID::ZONE_ID_PORTABELLO;

	else if (l == "lego_club" || l == "lego club" || l == "lc" || l == "1700")
		return ZoneID::ZONE_ID_LEGO_CLUB;

	else if (l == "crux_prime" || l == "crux prime" || l == "cp" || l == "1800")
		return ZoneID::ZONE_ID_CRUX_PRIME;

	else if (l == "nexus_tower" || l == "nexus tower" || l == "nt" || l == "1900")
		return ZoneID::ZONE_ID_NEXUS_TOWER;

	else if (l == "ninjago_monastery" || l == "ninjago monastery" || l == "nm" || l == "2000")
		return ZoneID::ZONE_ID_NINJAGO_MONASTERY;

	else if (l == "frakjaw_battle" || l == "frakjaw battle" || l == "2001")
		return ZoneID::ZONE_ID_FRAKJAW_BATTLE;

	else return ZoneID::INVALID_ZONE_ID;
}

string ZoneIDs::toString(ZoneID zoneID)
{
	switch (zoneID)
	{

	case ZONE_ID_VENTURE_EXPLORER:
		return "ZONE_ID_VENTURE_EXPLORER";

	case ZONE_ID_RETURN_TO_VENTURE_EXPLORER:
		return "ZONE_ID_RETURN_TO_VENTURE_EXPLORER";

	case ZONE_ID_AVANT_GARDENS:
		return "ZONE_ID_AVANT_GARDENS";

	case ZONE_ID_AVANT_GARDENS_SURVIVAL:
		return "ZONE_ID_AVANT_GARDENS_SURVIVAL";

	case ZONE_ID_SPIDER_QUEEN_BATTLE:
		return "ZONE_ID_SPIDER_QUEEN_BATTLE";

	case ZONE_ID_BLOCK_YARD:
		return "ZONE_ID_BLOCK_YARD";

	case ZONE_ID_AVANT_GROVE:
		return "ZONE_ID_AVANT_GROVE";

	case ZONE_ID_NIMBUS_STATION:
		return "ZONE_ID_NIMBUS_STATION";

	case ZONE_ID_PET_COVE:
		return "ZONE_ID_PET_COVE";

	case ZONE_ID_VERTIGO_LOOP_RACETRACK:
		return "ZONE_ID_VERTIGO_LOOP_RACETRACK";

	case ZONE_ID_BATTLE_OF_NIMBUS_STATION:
		return "ZONE_ID_BATTLE_OF_NIMBUS_STATION";

	case ZONE_ID_NIMBUS_ROCK:
		return "ZONE_ID_NIMBUS_ROCK";

	case ZONE_ID_NIMBUS_ISLE:
		return "ZONE_ID_NIMBUS_ISLE";

	case ZONE_ID_GNARLED_FOREST:
		return "ZONE_ID_GNARLED_FOREST";

	case ZONE_ID_CANYON_COVE:
		return "ZONE_ID_CANYON_COVE";

	case ZONE_ID_CHANTEY_SHANTEY:
		return "ZONE_ID_CHANTEY_SHANTEY";

	case ZONE_ID_FORBIDDEN_VALLEY:
		return "ZONE_ID_FORBIDDEN_VALLEY";

	case ZONE_ID_FORBIDDEN_VALLEY_DRAGON:
		return "ZONE_ID_FORBIDDEN_VALLEY_DRAGON";

	case ZONE_ID_DRAGONMAW_CHASM:
		return "ZONE_ID_DRAGONMAW_CHASM";

	case ZONE_ID_RAVEN_BLUFF:
		return "ZONE_ID_RAVEN_BLUFF";

	case ZONE_ID_STARBASE_3001:
		return "ZONE_ID_STARBASE_3001";

	case ZONE_ID_DEEP_FREEZE:
		return "ZONE_ID_DEEP_FREEZE";

	case ZONE_ID_ROBOT_CITY:
		return "ZONE_ID_ROBOT_CITY";

	case ZONE_ID_MOON_BASE:
		return "ZONE_ID_MOON_BASE";

	case ZONE_ID_PORTABELLO:
		return "ZONE_ID_PORTABELLO";

	case ZONE_ID_LEGO_CLUB:
		return "ZONE_ID_LEGO_CLUB";

	case ZONE_ID_CRUX_PRIME:
		return "ZONE_ID_CRUX_PRIME";

	case ZONE_ID_NEXUS_TOWER:
		return "ZONE_ID_NEXUS_TOWER";

	case ZONE_ID_NINJAGO_MONASTERY:
		return "ZONE_ID_NINJAGO_MONASTERY";

	case ZONE_ID_FRAKJAW_BATTLE:
		return "ZONE_ID_FRAKJAW_BATTLE";

	default:
		return "INVALID_ZONE_ID";

	}
}

string ZoneIDs::toPrintString(ZoneID zoneID)
{
	switch (zoneID)
	{

	case ZONE_ID_VENTURE_EXPLORER:
		return "Venture Explorer";

	case ZONE_ID_RETURN_TO_VENTURE_EXPLORER:
		return "Return to Venture Explorer";

	case ZONE_ID_AVANT_GARDENS:
		return "Avant Gardens";

	case ZONE_ID_AVANT_GARDENS_SURVIVAL:
		return "Avant Gardens Survival";

	case ZONE_ID_SPIDER_QUEEN_BATTLE:
		return "Spider Queen Battle";

	case ZONE_ID_BLOCK_YARD:
		return "Block Yard";

	case ZONE_ID_AVANT_GROVE:
		return "Avant Grove";

	case ZONE_ID_NIMBUS_STATION:
		return "Nimbus Station";

	case ZONE_ID_PET_COVE:
		return "Pet Cove";

	case ZONE_ID_VERTIGO_LOOP_RACETRACK:
		return "Vertigo Loop Racetrack";

	case ZONE_ID_BATTLE_OF_NIMBUS_STATION:
		return "Battle of Nimbus Station";

	case ZONE_ID_NIMBUS_ROCK:
		return "Nimbus Rock";

	case ZONE_ID_NIMBUS_ISLE:
		return "Nimbus Isle";

	case ZONE_ID_GNARLED_FOREST:
		return "Gnarled Forest";

	case ZONE_ID_CANYON_COVE:
		return "Canyon Cove";

	case ZONE_ID_CHANTEY_SHANTEY:
		return "Chantey Shantey";

	case ZONE_ID_FORBIDDEN_VALLEY:
		return "Forbidden Valley";

	case ZONE_ID_FORBIDDEN_VALLEY_DRAGON:
		return "Forbidden Valley Dragon";

	case ZONE_ID_DRAGONMAW_CHASM:
		return "Dragonmaw Chasm";

	case ZONE_ID_RAVEN_BLUFF:
		return "Raven Bluff";

	case ZONE_ID_STARBASE_3001:
		return "Starbase 3001";

	case ZONE_ID_DEEP_FREEZE:
		return "Deep Freeze";

	case ZONE_ID_ROBOT_CITY:
		return "Robot City";

	case ZONE_ID_MOON_BASE:
		return "Moon Base";

	case ZONE_ID_PORTABELLO:
		//return "Portabello";
		return "Frostburgh";

	case ZONE_ID_LEGO_CLUB:
		return "LEGO Club";

	case ZONE_ID_CRUX_PRIME:
		return "Crux Prime";

	case ZONE_ID_NEXUS_TOWER:
		return "Nexus Tower";

	case ZONE_ID_NINJAGO_MONASTERY:
		return "Ninjago Monastery";

	case ZONE_ID_FRAKJAW_BATTLE:
		return "Frakjaw Battle";

	default:
		return "INVALID_ZONE_ID";

	}
}

vector<unsigned char> ZoneIDs::getChecksum(ZoneID zoneID)
{
	vector<unsigned char> r;

	switch (zoneID)
	{

	case ZONE_ID_VENTURE_EXPLORER:
		r.push_back(0x7c);
		r.push_back(0x08);
		r.push_back(0xb8);
		r.push_back(0x20);
		break;

	case ZONE_ID_RETURN_TO_VENTURE_EXPLORER:
		r.push_back(0x3c);
		r.push_back(0x0a);
		r.push_back(0x68);
		r.push_back(0x26);
		break;

	case ZONE_ID_AVANT_GARDENS:
		r.push_back(0x11);
		r.push_back(0x55);
		r.push_back(0x52);
		r.push_back(0x49);
		break;

	case ZONE_ID_AVANT_GARDENS_SURVIVAL:
		r.push_back(0xe2);
		r.push_back(0x14);
		r.push_back(0x82);
		r.push_back(0x53);
		break;

	case ZONE_ID_SPIDER_QUEEN_BATTLE:
	case ZONE_ID_BLOCK_YARD:
		r.push_back(0xda);
		r.push_back(0x03);
		r.push_back(0xd4);
		r.push_back(0x0f);
		break;

	case ZONE_ID_AVANT_GROVE:
		r.push_back(0x03);
		r.push_back(0x03);
		r.push_back(0x89);
		r.push_back(0x0a);
		break;

	case ZONE_ID_NIMBUS_STATION:
		r.push_back(0x30);
		r.push_back(0x6b);
		r.push_back(0x1e);
		r.push_back(0xda);
		break;

	case ZONE_ID_PET_COVE:
		r.push_back(0x30);
		r.push_back(0x13);
		r.push_back(0x6e);
		r.push_back(0x47);
		break;

	case ZONE_ID_VERTIGO_LOOP_RACETRACK:
		r.push_back(0x02);
		r.push_back(0x05);
		r.push_back(0xfc);
		r.push_back(0x10);
		break;

	case ZONE_ID_BATTLE_OF_NIMBUS_STATION:
		r.push_back(0x58);
		r.push_back(0x02);
		r.push_back(0xd4);
		r.push_back(0x07);
		break;

	case ZONE_ID_NIMBUS_ROCK:
		r.push_back(0x91);
		r.push_back(0x01);
		r.push_back(0x8d);
		r.push_back(0x05);
		break;

	case ZONE_ID_NIMBUS_ISLE:
		r.push_back(0x5d);
		r.push_back(0x04);
		r.push_back(0x4f);
		r.push_back(0x09);
		break;

	case ZONE_ID_GNARLED_FOREST:
		r.push_back(0x90);
		r.push_back(0xc2);
		r.push_back(0xea);
		r.push_back(0x12);
		break;

	case ZONE_ID_CANYON_COVE:
		r.push_back(0xef);
		r.push_back(0x02);
		r.push_back(0x77);
		r.push_back(0x0b);
		break;

	case ZONE_ID_CHANTEY_SHANTEY:
		r.push_back(0x5c);
		r.push_back(0x01);
		r.push_back(0xb6);
		r.push_back(0x04);
		break;

	case ZONE_ID_FORBIDDEN_VALLEY:
		r.push_back(0x0d);
		r.push_back(0x76);
		r.push_back(0x19);
		r.push_back(0x85);
		break;

	case ZONE_ID_FORBIDDEN_VALLEY_DRAGON:
		r.push_back(0x87);
		r.push_back(0x01);
		r.push_back(0xf5);
		r.push_back(0x02);
		break;

	case ZONE_ID_DRAGONMAW_CHASM:
		r.push_back(0x4e);
		r.push_back(0x0f);
		r.push_back(0x85);
		r.push_back(0x81);
		break;

	case ZONE_ID_RAVEN_BLUFF:
		r.push_back(0x26);
		r.push_back(0x01);
		r.push_back(0xf0);
		r.push_back(0x03);
		break;

	case ZONE_ID_STARBASE_3001:
		r.push_back(0xee);
		r.push_back(0x02);
		r.push_back(0xc2);
		r.push_back(0x07);
		break;

	case ZONE_ID_DEEP_FREEZE:
		r.push_back(0x06);
		r.push_back(0x01);
		r.push_back(0x32);
		r.push_back(0x02);
		break;

	case ZONE_ID_ROBOT_CITY:
		r.push_back(0x7f);
		r.push_back(0x03);
		r.push_back(0x93);
		r.push_back(0x07);
		break;

	case ZONE_ID_MOON_BASE:
		r.push_back(0xad);
		r.push_back(0x01);
		r.push_back(0x3b);
		r.push_back(0x04);
		break;

	case ZONE_ID_PORTABELLO:
		//13600646
		//2766fbab
		r.push_back(0x46);
		r.push_back(0x06);
		r.push_back(0x60);
		r.push_back(0x13);
		
		/*r.push_back(0xdd);
		r.push_back(0x07);
		r.push_back(0x15);
		r.push_back(0x18);*/

		break;

	case ZONE_ID_LEGO_CLUB:
		r.push_back(0x38);
		r.push_back(0x01);
		r.push_back(0x04);
		r.push_back(0x02);
		break;

	case ZONE_ID_CRUX_PRIME:
		r.push_back(0x99);
		r.push_back(0xa3);
		r.push_back(0x17);
		r.push_back(0x4b);
		break;

	case ZONE_ID_NEXUS_TOWER:
		r.push_back(0x3c);
		r.push_back(0xf4);
		r.push_back(0x4a);
		r.push_back(0x9e);
		break;

	case ZONE_ID_NINJAGO_MONASTERY:
		r.push_back(0x74);
		r.push_back(0x2c);
		r.push_back(0x69);
		r.push_back(0x4d);
		break;

	case ZONE_ID_FRAKJAW_BATTLE:
		r.push_back(0xef);
		r.push_back(0x00);
		r.push_back(0xeb);
		r.push_back(0x09);
		break;

	}

	return r;
}
