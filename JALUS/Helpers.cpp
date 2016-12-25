#include "Helpers.h"
#include "Missions.h"
#include "CDClient.h"

void Helpers::addMissionWithTasks(long long missionID, long long charID)
{
	Missions::addMission(missionID, charID);
	vector<MissionTask> tasks = CDClient::getMissionTasks(missionID);

	for (int i = 0; i < tasks.size(); i++)
	{
		MissionTask task = tasks.at(i);

		CurrentMissionTasks::addMissionTask(missionID, task.uid, charID);
	}
}
