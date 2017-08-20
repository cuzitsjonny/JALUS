#pragma once
#include "Common.h"
#include "Logger.h"

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
	unsigned long id;
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
	
		static unsigned long generateID()
		{
		unsigned long id = 0;
		bool foundFree = false;
		
			do
			{
			bool occupied = false;
			
				for (int i = 0; i < tasks()->size(); i++)
				{
				SchedulerTask task = tasks()->at(i);
				
					if (task.id == id)
					{
					occupied = true;
					}
				}
			
				if (occupied)
				{
				id++;
				}
			else
				{
				foundFree = true;
				}
			} while (!foundFree);
			
				return id;
			}
public:
	template<class _Delay, class _Function, class... _Arguments>
	inline static unsigned long runTaskLater(_Delay delay, _Function && function, _Arguments &&... arguments);

	template<class _Delay, class _Function, class... _Arguments>
	inline static unsigned long runTaskTimer(_Delay delay, _Delay timerDelay, _Function && function, _Arguments &&... arguments);

	template<class _Delay, class _Function, class... _Arguments>
	inline static unsigned long runAsyncTaskLater(_Delay delay, _Function && function, _Arguments &&... arguments);

	template<class _Delay, class _Function, class... _Arguments>
	inline static unsigned long runAsyncTaskTimer(_Delay delay, _Delay timerDelay, _Function && function, _Arguments &&... arguments);

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

					else if (task->type == TIMER_ASYNC)
					{
						async(task->function);
						task->isAfterDelay = true;
						task->startTimestamp = ms.count();
					}

					else if (task->type == LATER_SYNC)
					{
						task->function;
						toDelete.push_back(i);
					}

					else if (task->type == TIMER_SYNC)
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
						async(task->function);
						task->startTimestamp = ms.count();
					}

					else if (task->type == TIMER_SYNC)
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

	static void cancelTask(unsigned long taskID)
	{
		for (int i = 0; i < tasks()->size(); i++)
		{
			SchedulerTask task = tasks()->at(i);
			
			if (task.id == taskID)
				{
			tasks()->erase(tasks()->begin() + i);
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
inline unsigned long Scheduler::runTaskLater(_Delay delay, _Function && function, _Arguments &&... arguments)
{
	SchedulerTask task;

	task.type = LATER_SYNC;
	task.function = bindToObject(function, arguments...);
	task.startTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	task.delay = delay;

	tasks()->push_back(task);

	return task.id;
}

template<class _Delay, class _Function, class... _Arguments>
inline unsigned long Scheduler::runTaskTimer(_Delay delay, _Delay timerDelay, _Function && function, _Arguments &&... arguments)
{
	SchedulerTask task;

	task.id = generateID();
	task.type = TIMER_SYNC;
	task.function = bindToObject(function, arguments...);
	task.startTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	task.delay = delay;
	task.timerDelay = timerDelay;

	tasks()->push_back(task);

	return task.id;
}

template<class _Delay, class _Function, class... _Arguments>
inline unsigned long Scheduler::runAsyncTaskLater(_Delay delay, _Function && function, _Arguments &&... arguments)
{
	SchedulerTask task;

	task.id = generateID();
	task.type = LATER_ASYNC;
	task.function = bindToObject(function, arguments...);
	task.startTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	task.delay = delay;

	tasks()->push_back(task);

	return task.id;
}

template<class _Delay, class _Function, class... _Arguments>
inline unsigned long Scheduler::runAsyncTaskTimer(_Delay delay, _Delay timerDelay, _Function && function, _Arguments &&... arguments)
{
	SchedulerTask task;

	task.id = generateID();
	task.type = TIMER_ASYNC;
	task.function = bindToObject(function, arguments...);
	task.startTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	task.delay = delay;
	task.timerDelay = timerDelay;

	tasks->push_back(task);

	return task.id;
}
