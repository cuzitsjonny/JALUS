#pragma once
#include "Common.h"

enum MissionTaskType : long
{
	MISSION_TASK_TYPE_UNKNOWN = 0,
	MISSION_TASK_TYPE_HARDCODED = 1,
	MISSION_TASK_TYPE_QUICKBUILD = 2,
	MISSION_TASK_TYPE_COLLECT_COLLECTIBLE = 3,
	MISSION_TASK_TYPE_INTERACT = 4,
	MISSION_TASK_TYPE_USE_EMOTE = 5,
	MISSION_TASK_TYPE_USE_COMSUMABLE = 9,
	MISSION_TASK_TYPE_USE_SKILL = 10,
	MISSION_TASK_TYPE_COLLECT_ITEM = 11,
	MISSION_TASK_TYPE_DISCOVER = 12,
	MISSION_TASK_TYPE_MINI_ACHIEVE = 14,
	MISSION_TASK_TYPE_INTERACT_2 = 15,
	MISSION_TASK_TYPE_MISSION_COMPLETE = 16,
	MISSION_TASK_TYPE_TAME_PET = 22,
	MISSION_TASK_TYPE_FLAG_CHANGE = 24
};

class MissionTask
{
public:
	long uid = -1;
	vector<float> value = vector<float>();
	float targetValue = 0.0F;
	vector<long> targets = vector<long>();
	MissionTaskType type = MissionTaskType::MISSION_TASK_TYPE_UNKNOWN;
};

class Vectors
{
public:
	static string toString(vector<float> v);
	static vector<float> fromString(string str);
	static bool contains(vector<float> v, float f);
};

class CurrentMissionTasks
{
public:
	static string name;

	static void init(string name, string structure);

	static void addMissionTask(long missionID, long uniqueID, long long charID);
	static void deleteMissionTasks(long missionID, long long charID);
	static void deleteMissionTasks(long long charID);

	static void setValue(long uniqueID, vector<float> value, long long charID);
	static vector<float> getValue(long uniqueID, long long charID);

	static vector<MissionTask> getMissionTasks(long missionID, long long charID);
};