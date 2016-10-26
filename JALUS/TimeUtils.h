#pragma once
#include "Common.h"

class TimeUtils
{
public:
	static long long millisecondsToSeconds(long long time);
	static long long millisecondsToMinutes(long long time);
	static long long millisecondsToHours(long long time);
	static long long millisecondsToDays(long long time);

	static long long secondsToMilliseconds(long long time);
	static long long secondsToMinutes(long long time);
	static long long secondsToHours(long long time);
	static long long secondsToDays(long long time);

	static long long minutesToMilliseconds(long long time);
	static long long minutesToSeconds(long long time);
	static long long minutesToHours(long long time);
	static long long minutesToDays(long long time);

	static long long hoursToMilliseconds(long long time);
	static long long hoursToSeconds(long long time);
	static long long hoursToMinutes(long long time);
	static long long hoursToDays(long long time);

	static long long daysToMilliseconds(long long time);
	static long long daysToSeconds(long long time);
	static long long daysToMinutes(long long time);
	static long long daysToHours(long long time);

	static long long parse(string time);
};