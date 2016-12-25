#pragma once
#include "Common.h"

enum MissionTaskType : long
{
	MISSION_TASK_TYPE_UNKNOWN = 0,
	MISSION_TASK_TYPE_HARDCODED = 1,
	MISSION_TASK_TYPE_COLLECT_COLLECTIBLE = 3,
	MISSION_TASK_TYPE_INTERACT = 4,
	MISSION_TASK_TYPE_COLLECT_ITEM = 11,
	MISSION_TASK_TYPE_FLAG_CHANGE = 24
};

class MissionTask
{
public:
	long uid = -1;
	float value = 0.0F;
	float targetValue = 0.0F;
	vector<long> targets = vector<long>();
	MissionTaskType type = MissionTaskType::MISSION_TASK_TYPE_UNKNOWN;
};

class CurrentMissionTasks
{
public:
	static string name;

	static void init(string name, string structure);

	static void addMissionTask(long missionID, long uniqueID, long long charID);
	static void deleteMissionTasks(long missionID, long long charID);
	static void deleteMissionTasks(long long charID);

	static void setValue(long uniqueID, float value, long long charID);
	static float getValue(long uniqueID, long long charID);

	static vector<MissionTask> getMissionTasks(long missionID, long long charID);
};