#pragma once
#include "Common.h"

enum GameMessageID : unsigned short
{
	GAME_MESSAGE_ID_READY_FOR_UPDATES = 888,
	GAME_MESSAGE_ID_REQUEST_USE = 364,
	GAME_MESSAGE_ID_OFFER_MISSION = 248,
	GAME_MESSAGE_ID_NOTIFY_MISSION = 254,
	GAME_MESSAGE_ID_MISSION_DIALOGUE_OK = 520,
	GAME_MESSAGE_ID_NOTIFY_MISSION_TASK = 255,
	GAME_MESSAGE_ID_HAS_BEEN_COLLECTED = 486
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

class GameMessages
{
public:
	static void processGameMessage(BitStream* data, SystemAddress clientAddress);
	static void offerMission(long long objectID, long missionID, long long offererID, SystemAddress receiver, bool sendTwice = true);
	static void notifyMission(long long objectID, long missionID, MissionState missionState, bool sendingRewards, SystemAddress receiver);
	static void notifyMissionTask(long objectID, long missionID, long taskIndex, float updatedValue, SystemAddress receiver);
};