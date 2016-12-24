#pragma once
#include "CurrentMissionTasks.h"

class MissionInfo
{
public:
	long missionID = -1;
	bool isDone = false;
	long long doneTimestamp = -1;
	long doneCount = 0;
	vector<MissionTask> missionTasks = vector<MissionTask>();
};

class Missions
{
public:
	static string name;

	static void init(string name, string structure);

	static void addMission(long missionID, long long charID);
	static void deleteMissions(long long charID);
	static void setMissionDone(long long missionID, long long charID);
	static void incrementMissionDoneCount(long long missionID, long long charID);

	static bool hasDoneMission(long missionID, long long charID);
	static bool isDoingMission(long missionID, long long charID);
	static long long getMissionDoneTimestamp(long missionID, long long charID);
	static long getMissionDoneCount(long missionID, long long charID);

	static vector<MissionInfo> getAllDoneMissions(long long charID);
	static vector<MissionInfo> getAllCurrentMissions(long long charID);
};