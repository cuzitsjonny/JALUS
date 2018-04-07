#pragma once
#include "Common.h"
#include "Locations.h"
#include "InventoryItems.h"

enum GameMessageID : unsigned short
{
	GAME_MESSAGE_ID_READY_FOR_UPDATES = 888,
	GAME_MESSAGE_ID_REQUEST_USE = 364,
	GAME_MESSAGE_ID_OFFER_MISSION = 248,
	GAME_MESSAGE_ID_NOTIFY_MISSION = 254,
	GAME_MESSAGE_ID_MISSION_DIALOGUE_OK = 520,
	GAME_MESSAGE_ID_NOTIFY_MISSION_TASK = 255,
	GAME_MESSAGE_ID_HAS_BEEN_COLLECTED = 486,
	GAME_MESSAGE_ID_DROP_CLIENT_LOOT = 30,
	GAME_MESSAGE_ID_DIE = 37,
	GAME_MESSAGE_ID_REQUEST_DIE = 38,
	GAME_MESSAGE_ID_RESURRECT = 160,
	GAME_MESSAGE_ID_REQUEST_RESURRECT = 159,
	GAME_MESSAGE_ID_TELEPORT = 19,
	GAME_MESSAGE_ID_MODIFY_LEGO_SCORE = 1459,
	GAME_MESSAGE_ID_SET_CURRENCY = 133,
	GAME_MESSAGE_ID_SET_FLAG = 471,
	GAME_MESSAGE_ID_NOTIFY_CLIENT_FLAG_CHANGE = 472,
	GAME_MESSAGE_ID_NOTIFY_SERVER_LEVEL_PROCESSING_COMPLETE = 1734,
	GAME_MESSAGE_ID_UPDATE_REPUTATION = 746,
	GAME_MESSAGE_ID_SET_INVENTORY_SIZE = 389,
	GAME_MESSAGE_ID_RESTORE_TO_POST_LOAD_STATS = 1468,
	GAME_MESSAGE_ID_RESPOND_TO_MISSION = 249,
	GAME_MESSAGE_ID_PLAY_FX_EFFECT = 154,
	GAME_MESSAGE_ID_STOP_FX_EFFECT = 155,
	GAME_MESSAGE_ID_FIRE_EVENT_SERVER_SIDE = 770,
	GAME_MESSAGE_ID_FIRE_EVENT_CLIENT_SIDE = 1213,
	GAME_MESSAGE_ID_TOOGLE_GHOST_REFERENCE_OVERRIDE = 767,
	GAME_MESSAGE_ID_SET_GHOST_REFERENCE_POSITION = 768,
	GAME_MESSAGE_ID_START_SKILL = 119,
	GAME_MESSAGE_ID_ECHO_START_SKILL = 118,
	GAME_MESSAGE_ID_SYNC_SKILL = 1145,
	GAME_MESSAGE_ID_ECHO_SYNC_SKILL = 1144,
	GAME_MESSAGE_ID_REQUEST_SERVER_PROJECTILE_IMPACT = 1148,
	GAME_MESSAGE_ID_DO_CLIENT_PROJECTILE_IMPACT = 1151,
	GAME_MESSAGE_ID_ADD_ITEM_TO_INVENTORY_CLIENT_SYNC = 227,
	GAME_MESSAGE_ID_PARSE_CHAT_MESSAGE = 850,
	GAME_MESSAGE_ID_SMASH_ME = 1202,
	GAME_MESSAGE_ID_PICKUP_ITEM = 139,
	GAME_MESSAGE_ID_PICKUP_CURRENCY = 137,
	GAME_MESSAGE_ID_PLAYER_LOADED = 505,
	GAME_MESSAGE_MOVE_ITEM_IN_INVENTORY = 224,
	GAME_MESSAGE_REMOVE_ITEM_FROM_INVENTORY = 230,
	GAME_MESSAGE_EQUIP_INVENTORY = 231,
	GAME_MESSAGE_UNEQUIP_INVENTORY = 233,
	GAME_MESSAGE_ENABLE_REBUILD = 213,
	GAME_MESSAGE_REBUILD_CANCEL = 209,
	GAME_MESSAGE_MODIFY_GHOSTING_DISTANCE = 1485,
	GAME_MESSAGE_FIRE_EVENT_SERVER_SIDE = 770,
	GAME_MESSAGE_START_BUILDING_WITH_ITEM = 1057,
	GAME_MESSAGE_START_ARRANGING_WITH_ITEM = 1077,
	//skills
	GAME_MESSAGE_ID_ADD_SKILL = 127,
	GAME_MESSAGE_ID_REMOVE_SKILL = 128,
	//racing
	GAME_MESSAGE_ID_VEHICLE_LOCK_INPUT = 1230,
	GAME_MESSAGE_ID_VEHICLE_UNLOCK_INPUT = 1231,
	GAME_MESSAGE_ID_RACING_RESET_PLAYER_TO_LAST_RESET = 1252,
	GAME_MESSAGE_ID_RACING_SET_PLAYER_RESET_INFO = 1254,
	GAME_MESSAGE_ID_RACING_PLAYER_INFO_RESET_FINISHED = 1255,
	GAME_MESSAGE_ID_LOCK_NODE_ROTATION = 1260,
	GAME_MESSAGE_ID_VEHICLE_SET_WHEEL_LOCK_STATE = 1273,
	GAME_MESSAGE_ID_NOTIFY_VEHICLE_OF_RACING_OBJECT = 1276,
	//vehicle actions
	GAME_MESSAGE_ID_VEHICLE_ADD_PASSIVE_BOOST_ACTION = 1340,
	GAME_MESSAGE_ID_VEHICLE_REMOVE_PASSIVE_BOOST_ACTION = 1341,
	GAME_MESSAGE_ID_VEHICLE_NOTIFY_SERVER_ADD_PASSIVE_BOOST_ACTION = 1342,
	GAME_MESSAGE_ID_VEHICLE_NOTIFY_SERVER_REMOVE_PASSIVE_BOOST_ACTION = 1343,
	//racing loading
	GAME_MESSAGE_ID_NOTIFY_RACING_CLIENT = 1390,
	GAME_MESSAGE_ID_RACING_PLAYER_LOADED = 1392,
	GAME_MESSAGE_ID_RACING_CLIENT_READY = 1393,
	//building
	GAME_MESSAGE_ID_START_BUILDING_WITH_ITEM = 1057,
	GAME_MESSAGE_ID_SET_BUILD_MODE = 1068

};

enum MultiInteractType : unsigned long
{
	MULTI_INTERACT_TYPE_MISSION = 0
};

enum MissionState : unsigned long
{
	MISSION_STATE_UNKNOWN = 0,
	MISSION_STATE_AVAILABLE,
	MISSION_STATE_ACTIVE,
	MISSION_STATE_READY_TO_COMPLETE,
	MISSION_STATE_COMPLETE,
	MISSION_STATE_FAIL,
	MISSION_STATE_READY_TO_COMPLETE_REPORTED,
	MISSION_STATE_REMOVE_FROM_MISSION_CHECKER = 8
};

// This is Simon's enum //
//enum MissionState : unsigned short {
//	MISSION_AVAILABLE = 1, //0 or 1 (not sure)
//	MISSION_ACTIVE,
//	MISSION_READY_TO_COMPLETE,
//	MISSION_COMPLETE,
//	MISSION_FAIL,
//	MISSION_READY_TO_COMPLETE_REPORTED,
//	MISSION_UNKNOWN8 = 8 //? this is used by the imagination mission
//};

class GameMessages
{
public:
	static void processGameMessage(BitStream* data, SystemAddress clientAddress);
	static void offerMission(long long objectID, long missionID, long long offererID, SystemAddress receiver, bool sendTwice = true);
	static void notifyMission(long long objectID, long missionID, MissionState missionState, bool sendingRewards, SystemAddress receiver);
	static void notifyMissionTask(long long objectID, long missionID, long taskIndex, float update, SystemAddress receiver);
	static void die(long long objectID, wstring deathType, bool spawnLoot, SystemAddress receiver, long long killerID = 1, long long lootOwnerID = 1);
	static void resurrect(long long objectID, bool rezImmediately, SystemAddress receiver);
	static void teleport(long long objectID, bool noGravTeleport, bool ignoreY, bool setRotation, bool skipAllChecks, Position pos, SystemAddress receiver, Rotation rot = Rotation());
	static void modifyLegoScore(long long objectID, long long score, bool updateScoreBar, SystemAddress receiver);
	static void setCurrency(long long objectID, long long currency, Position pos, SystemAddress receiver);
	static void notifyClientFlagChange(long long objectID, long flagID, bool value, SystemAddress receiver);
	static void updateReputation(long long objectID, long long reputation, SystemAddress receiver);
	static void setInventorySize(long long objectID, InventoryType type, long size, SystemAddress receiver);
	static void playFXEffect(long long objectID, long effectID, wstring effectType, float scale, string name, float priority, long long secondary, SystemAddress receiver, bool serialize = true);
	static void stopFXEffect(long long objectID, string name, bool killImmediately, SystemAddress receiver);
	static void fireEventClientSide(long long objectID, wstring args, long long object, long long senderID, SystemAddress receiver, long long param1 = 0, long param2 = -1);
	static void addItemToInventory(long long objectID, bool isBound, long lot, InventoryType invType, long count, long totalCount, long long stackObjectID, short slot, SystemAddress receiver, bool showFlyingLoot = true);
	static void clientDropLoot(long long objectID, int iCurrency, long lot, long long owner, long long sourceObj, Position spawnPosition, Position finalPosition, SystemAddress receiver);
	static void addSkill(long long objectID, long skillid, long slot, SystemAddress receiver);
	static void removeSkill(long long objectID, long skillid, bool fromSkillSet, SystemAddress receiver);
};