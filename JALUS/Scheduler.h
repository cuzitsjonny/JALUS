#pragma once
#include "Common.h"

#include <future>

enum SchedulerTaskType
{
	TIMER_ASYNC,
	LATER_ASYNC,
	TIMER_SYNC,
	LATER_SYNC
};

class SchedulerTask
{
public:
	SchedulerTaskType type;
	function<void()> function;
	long long startTimestamp;
	long long delay;
	long long timerDelay;
	bool isAfterDelay = false;
};

class Scheduler
{
private:
	static vector<SchedulerTask>* tasks() { static vector<SchedulerTask> v; return &v; };

	template<class _Function, class... _Arguments>
	inline static function<void()> bindToObject(_Function && function, _Arguments &&... arguments);
public:
	template<class _Delay, class _Function, class... _Arguments>
	inline static void runTaskLater(_Delay delay, _Function && function, _Arguments &&... arguments);

	template<class _Delay, class _Function, class... _Arguments>
	inline static void runTaskTimer(_Delay delay, _Delay timerDelay, _Function && function, _Arguments &&... arguments);

	template<class _Delay, class _Function, class... _Arguments>
	inline static void runAsyncTaskLater(_Delay delay, _Function && function, _Arguments &&... arguments);

	template<class _Delay, class _Function, class... _Arguments>
	inline static void runAsyncTaskTimer(_Delay delay, _Delay timerDelay, _Function && function, _Arguments &&... arguments);

	static void tick()
	{
		vector<int> toDelete;

		for (int i = 0; i < tasks()->size(); i++)
		{
			SchedulerTask* task = &tasks()->at(i);
			auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

			if (!task->isAfterDelay)
			{
				if ((task->startTimestamp + task->delay) <= ms.count())
				{
					if (task->type == LATER_ASYNC)
					{
						async(task->function);
						toDelete.push_back(i);
					}

					if (task->type == TIMER_ASYNC)
					{
						async(task->function);
						task->isAfterDelay = true;
						task->startTimestamp = ms.count();
					}

					if (task->type == LATER_SYNC)
					{
						task->function;
						toDelete.push_back(i);
					}

					if (task->type == TIMER_SYNC)
					{
						task->function();
						task->isAfterDelay = true;
						task->startTimestamp = ms.count();
					}
				}
			}
			else
			{
				if ((task->startTimestamp + task->timerDelay) <= ms.count())
				{
					if (task->type == TIMER_ASYNC)
					{
						thread(task->function);
						task->startTimestamp = ms.count();
					}

					if (task->type == TIMER_SYNC)
					{
						task->function();
						task->startTimestamp = ms.count();
					}
				}
			}
		}

		if (toDelete.size() > 0)
		{
			reverse(toDelete.begin(), toDelete.end());

			for (int i = 0; i < toDelete.size(); i++)
			{
				tasks()->erase(tasks()->begin() + toDelete.at(i));
			}
		}
	}
};

template<class _Function, class... _Arguments>
inline function<void()> Scheduler::bindToObject(_Function && function, _Arguments &&... arguments)
{
	return bind(&function, arguments...);
}

template<class _Delay, class _Function, class... _Arguments>
inline void Scheduler::runTaskLater(_Delay delay, _Function && function, _Arguments &&... arguments)
{
	SchedulerTask task;

	task.type = LATER_SYNC;
	task.function = bindToObject(function, arguments...);
	task.startTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	task.delay = delay;

	tasks()->push_back(task);
}

template<class _Delay, class _Function, class... _Arguments>
inline void Scheduler::runTaskTimer(_Delay delay, _Delay timerDelay, _Function && function, _Arguments &&... arguments)
{
	SchedulerTask task;

	task.type = TIMER_SYNC;
	task.function = bindToObject(function, arguments...);
	task.startTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	task.delay = delay;
	task.timerDelay = timerDelay;

	tasks()->push_back(task);
}

template<class _Delay, class _Function, class... _Arguments>
inline void Scheduler::runAsyncTaskLater(_Delay delay, _Function && function, _Arguments &&... arguments)
{
	SchedulerTask task;

	task.type = LATER_ASYNC;
	task.function = bindToObject(function, arguments...);
	task.startTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	task.delay = delay;

	tasks()->push_back(task);
}

template<class _Delay, class _Function, class... _Arguments>
inline void Scheduler::runAsyncTaskTimer(_Delay delay, _Delay timerDelay, _Function && function, _Arguments &&... arguments)
{
	SchedulerTask task;

	task.type = TIMER_ASYNC;
	task.function = bindToObject(function, arguments...);
	task.startTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	task.delay = delay;
	task.timerDelay = timerDelay;

	tasks->push_back(task);
}
