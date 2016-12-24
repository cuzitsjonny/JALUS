#pragma once
#include "Common.h"
#include "Locations.h"

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
	GAME_MESSAGE_ID_SET_CURRENCY = 133
};

enum MissionState : unsigned long
{
	MISSION_STATE_UNKNOWN = 0,
	MISSION_STATE_AVAILABLE = 1,
	MISSION_STATE_ACTIVE = 2,
	MISSION_STATE_READY_TO_COMPLETE = 3,
	MISSION_STATE_COMPLETE = 8,
	MISSION_STATE_FAIL = 5,
	MISSION_STATE_READY_TO_COMPLETE_REPORTED = 6
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
	static void notifyMissionTask(long long objectID, long missionID, long taskIndex, vector<float> updates, SystemAddress receiver);
	static void die(long long objectID, wstring deathType, bool spawnLoot, SystemAddress receiver, long long killerID = 1, long long lootOwnerID = 1);
	static void resurrect(long long objectID, bool rezImmediately, SystemAddress receiver);
	static void teleport(long long objectID, bool noGravTeleport, bool ignoreY, bool setRotation, bool skipAllChecks, Position pos, SystemAddress receiver, Rotation rot = Rotation());
	static void modifyLegoScore(long long objectID, long long score, SystemAddress receiver);
	static void setCurrency(long long objectID, long long currency, Position pos, SystemAddress receiver);
};