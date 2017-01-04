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
	GAME_MESSAGE_ID_ECHO_SYNC_SKILL = 1144
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
	static void fireEventClientSide(long long objectID, wstring args, long long object, long long senderID, SystemAddress receiver, long long param1 = 0, long param2 = -1);
};